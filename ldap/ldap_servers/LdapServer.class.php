<?php
// $Id: LdapServer.class.php,v 1.5.2.1 2011/02/08 06:01:00 johnbarclay Exp $

/**
 * @file
 * Defines server classes and related functions.
 *
 */

/**
 * LDAP Server Class
 *
 *  This class is used to create, work with, and eventually destroy ldap_server
 * objects.
 *
 * @todo make bindpw protected
 */
class LdapServer {
  // LDAP Settings

  const LDAP_CONNECT_ERROR = 0x5b;
  const LDAP_SUCCESS = 0x00;
  const LDAP_OPERATIONS_ERROR = 0x01;
  const LDAP_PROTOCOL_ERROR = 0x02;

  public $sid;
  public $name;
  public $status;
  public $ldap_type;
  public $address;
  public $port = 389;
  public $tls = FALSE;
  public $bind_method = 0;
  public $basedn = array();
  public $binddn = FALSE; // Default to an anonymous bind.
  public $bindpw = FALSE; // Default to an anonymous bind.
  public $user_dn_expression;
  public $user_attr;
  public $mail_attr;
  public $mail_template;
  public $unique_persistent_attr;
  public $allow_conflicting_drupal_accts = FALSE;
  public $ldapToDrupalUserPhp;
  public $testingDrupalUsername;
  public $detailed_watchdog_log;


  public $inDatabase = FALSE;

  public $connection;
  // direct mapping of db to object properties
  public static function field_to_properties_map() {
    return array( 'sid' => 'sid',
    'name'  => 'name' ,
    'status'  => 'status',
    'ldap_type'  => 'ldap_type',
    'address'  => 'address',
    'port'  => 'port',
    'tls'  => 'tls',
    'bind_method' => 'bind_method',
    'basedn'  => 'basedn',
    'binddn'  => 'binddn',
    'user_dn_expression' => 'user_dn_expression',
    'user_attr'  => 'user_attr',
    'mail_attr'  => 'mail_attr',
    'mail_template'  => 'mail_template',
    'unique_persistent_attr' => 'unique_persistent_attr',
    'allow_conflicting_drupal_accts' => 'allow_conflicting_drupal_accts',
    'ldap_to_drupal_user'  => 'ldapToDrupalUserPhp',
    'testing_drupal_username'  => 'testingDrupalUsername'
    );

  }

  /**
   * Constructor Method
   */
  function __construct($sid) {
    if (!is_scalar($sid)) {
      return;
    }
    $this->detailed_watchdog_log = variable_get('ldap_help_watchdog_detail', 0);
    $server_record = array();
    if (module_exists('ctools')) {
      ctools_include('export');
      $result = ctools_export_load_object('ldap_servers', 'names', array($sid));
      if (isset($result[$sid])) {
        $server_record[$sid] = $result[$sid];
        foreach ($server_record[$sid] as $property => $value) {
          $this->{$property} = $value;
        }
      }
    }
    else {
      $select = db_select('ldap_servers')
        ->fields('ldap_servers')
        ->condition('ldap_servers.sid',  $sid)
        ->execute();
      foreach ($select as $record) {
        $server_record[$record->sid] = $record;
      }
    }
    if (!isset($server_record[$sid])) {
      $this->inDatabase = FALSE;
      return;
    }
    $server_record = $server_record[$sid];

    if ($server_record) {
      $this->inDatabase = TRUE;
      $this->sid = $sid;
      $this->detailedWatchdogLog = variable_get('ldap_help_watchdog_detail', 0);
    }
    else {
      // @todo throw error
    }

    foreach ($this->field_to_properties_map() as $db_field_name => $property_name ) {
      if (isset($server_record->$db_field_name)) {
        $this->{$property_name} = $server_record->$db_field_name;
      }
    }
    if (is_scalar($this->basedn)) {
      $this->basedn = unserialize($this->basedn);
    }
    if (isset($server_record->bindpw) && $server_record->bindpw != '') {
      $this->bindpw = $server_record->bindpw;
      $this->bindpw = ldap_servers_decrypt($this->bindpw);
    }
  }

  /**
   * Destructor Method
   */
  function __destruct() {
    // Close the server connection to be sure.
    $this->disconnect();
  }


  /**
   * Invoke Method
   */
  function __invoke() {
    $this->connect();
    $this->bind();
  }



  /**
   * Connect Method
   */
  function connect() {

    if (!$con = ldap_connect($this->address, $this->port)) {
      watchdog('user', 'LDAP Connect failure to ' . $this->address . ':' . $this->port);
      return LDAP_CONNECT_ERROR;
    }

    ldap_set_option($con, LDAP_OPT_PROTOCOL_VERSION, 3);
    ldap_set_option($con, LDAP_OPT_REFERRALS, 0);

    // Use TLS if we are configured and able to.
    if ($this->tls) {
      ldap_get_option($con, LDAP_OPT_PROTOCOL_VERSION, $vers);
      if ($vers == -1) {
        watchdog('user', 'Could not get LDAP protocol version.');
        return LDAP_PROTOCOL_ERROR;
      }
      if ($vers != 3) {
        watchdog('user', 'Could not start TLS, only supported by LDAP v3.');
        return LDAP_CONNECT_ERROR;
      }
      elseif (!function_exists('ldap_start_tls')) {
        watchdog('user', 'Could not start TLS. It does not seem to be supported by this PHP setup.');
        return LDAP_CONNECT_ERROR;
      }
      elseif (!ldap_start_tls($con)) {
        $msg =  t("Could not start TLS. (Error %errno: %error).", array('%errno' => ldap_errno($con), '%error' => ldap_error($con)));
        watchdog('user', $msg);
        return LDAP_CONNECT_ERROR;
      }
    }

  // Store the resulting resource
  $this->connection = $con;
  return LDAP_SUCCESS;
  }


  /**
	 * Bind (authenticate) against an active LDAP database.
	 *
	 * @param $userdn
	 *   The DN to bind against. If NULL, we use $this->binddn
	 * @param $pass
	 *   The password search base. If NULL, we use $this->bindpw
   *
   * @return
   *   Result of bind; TRUE if successful, FALSE otherwise.
   */
  function bind($userdn = NULL, $pass = NULL) {
    $userdn = ($userdn != NULL) ? $userdn : $this->binddn;
    $pass = ($pass != NULL) ? $pass : $this->bindpw;
    // Ensure that we have an active server connection.
    if (!$this->connection) {
      watchdog('ldap', "LDAP bind failure for user %user. Not connected to LDAP server.", array('%user' => $userdn));
      return LDAP_CONNECT_ERROR;
    }


    if (@!ldap_bind($this->connection, $userdn, $pass)) {
      watchdog('ldap', "LDAP bind failure for user %user. Error %errno: %error", array('%user' => $userdn, '%errno' => ldap_errno($this->connection), '%error' => ldap_error($this->connection)));
      return ldap_errno($this->connection);
    }

    return LDAP_SUCCESS;
  }

  /**
   * Disconnect (unbind) from an active LDAP server.
   */
  function disconnect() {
    if (!$this->connection) {
      // never bound or not currently bound, so no need to disconnect
      //watchdog('ldap', 'LDAP disconnect failure from '. $this->server_addr . ':' . $this->port);
    }
    else {
      ldap_unbind($this->connection);
      $this->connection = NULL;
    }
  }

  /**
   * Perform an LDAP search.  Must be connected and bound first.
   *
   *  @param params same as ldap_search() params except $link_identifier is omitted.
   *
   * @return
   *   An array of matching entries->attributes, or FALSE if the search is
   *   empty.
   */

  function search($base_dn = NULL, $filter, $attributes = array(), $attrsonly = 0, $sizelimit = 0, $timelimit = 0, $deref = NULL, $scope = LDAP_SCOPE_SUBTREE) {
    if ($base_dn == NULL) {
      if (count($this->basedn) == 1) {
        $base_dn = $this->basedn[0];
      }
      else {
        return FALSE;
      }
    }

    $attr_display =  is_array($attributes) ? join(',', $attributes) : 'none';
    $query = 'ldap_search() call: '. join(",\n", array(
      'base_dn: ' . $base_dn,
      'filter = ' . $filter,
      'attributes: ' . $attr_display,
      'attrsonly = ' .  $attrsonly,
      'sizelimit = ' .  $sizelimit,
      'timelimit = ' .  $timelimit,
      'deref = ' .  $deref,
      'scope = ' .  $scope,
      )
    );
    if ($this->detailed_watchdog_log) {
      watchdog('ldap_server', $query, array());
    }

    // When checking multiple servers, there's a chance we might not be connected yet.
    if (! $this->connection) {
      $this->connect();
      $this->bind();
    }


    switch ($scope) {
      case LDAP_SCOPE_SUBTREE:
        $result = ldap_search($this->connection, $base_dn, $filter, $attributes, $attrsonly, $sizelimit, $timelimit, $deref);
        if ($this->hasError()) {
          watchdog('ldap_server', 'ldap_search() function error. LDAP Error: %message, ldap_search() parameters: %query',
            array('%message' => $this->errorMsg('ldap'), '%query' => $query),
            WATCHDOG_ERROR);
        }
        break;

      case LDAP_SCOPE_BASE:
        $result = ldap_read($this->connection, $base_dn, $filter, $attributes, $attrsonly, $sizelimit, $timelimit, $deref);
        if ($this->hasError()) {
          watchdog('ldap_server', 'ldap_read() function error.  LDAP Error: %message, ldap_read() parameters: %query',
            array('%message' => $this->errorMsg('ldap'), '%query' => $query),
            WATCHDOG_ERROR);
        }
        break;

      case LDAP_SCOPE_ONELEVEL:
        $result = ldap_list($this->connection, $base_dn, $filter, $attributes, $attrsonly, $sizelimit, $timelimit, $deref);
        if ($this->hasError()) {
          watchdog('ldap_server', 'ldap_list() function error. LDAP Error: %message, ldap_list() parameters: %query',
            array('%message' => $this->errorMsg('ldap'), '%query' => $query),
            WATCHDOG_ERROR);
        }
        break;
    }

    if ($result && ldap_count_entries($this->connection, $result)) {
      $entries = ldap_get_entries($this->connection, $result);
      return $entries;
    }
    elseif ($this->ldapErrorNumber()) {
      $watchdog_tokens =  array('%basedn' => $base_dn, '%filter' => $filter,
        '%attributes' => print_r($attributes, TRUE), '%errmsg' => $this->errorMsg('ldap'),
        '%errno' => $this->ldapErrorNumber());
      watchdog('ldap', "LDAP ldap_search error. basedn: %basedn| filter: %filter| attributes:
        %attributes| errmsg: %errmsg| ldap err no: %errno|", $watchdog_tokens);
      return array();
    }
    else {
      return array();
    }
  }


  /**
   * Queries LDAP server for the user.
   *
   * @param $drupal_user_name
   *  drupal user name.
   *
   * @return
   *   An array with users LDAP data or NULL if not found.
   */
  function user_lookup($drupal_user_name) {
    $watchdog_tokens = array('%drupal_user_name' => $drupal_user_name);
    if ($this->ldapToDrupalUserPhp && module_exists('php')) {
      global $name;
      $old_name_value = $name;
      $name = $drupal_user_name;
      $code = "<?php global \$name; \n". $this->ldapToDrupalUserPhp . "; \n ?>";
      $watchdog_tokens['%code'] = $this->ldapToDrupalUserPhp;
      $code_result = php_eval($code);
      $watchdog_tokens['%code_result'] = $code_result;
      $ldap_username = $code_result;
      $watchdog_tokens['%ldap_username'] = $ldap_username;
      $name = $old_name_value;
      if ($this->detailedWatchdogLog) {
        watchdog('ldap_server', '%drupal_user_name tansformed to %ldap_username by applying code <code>%code</code>', $watchdog_tokens, WATCHDOG_DEBUG);
      }
    }
    else {
      $ldap_username = $drupal_user_name;
    }
    if (!$ldap_username) {
      return FALSE;
    }
   // print "$ldap_username from $drupal_user_name"; die;
    foreach ($this->basedn as $basedn) {
      if (empty($basedn)) continue;
      $filter = '('. $this->user_attr . '=' . $ldap_username . ')';
      $result = $this->search($basedn, $filter);
      if (!$result || !isset($result['count']) || !$result['count']) continue;

      // Must find exactly one user for authentication to work.
      if ($result['count'] != 1) {
        $count = $result['count'];
        watchdog('ldap_authentication', "Error: !count users found with $filter under $basedn.", array('!count' => $count), WATCHDOG_ERROR);
        continue;
      }
      $match = $result[0];
      // These lines serve to fix the attribute name in case a
      // naughty server (i.e.: MS Active Directory) is messing the
      // characters' case.
      // This was contributed by Dan "Gribnif" Wilga, and described
      // here: http://drupal.org/node/87833
      $name_attr = $this->user_attr;
      if (isset($match[$name_attr][0])) {

      }
      elseif (isset($match[drupal_strtolower($name_attr)][0])) {
        $name_attr = drupal_strtolower($name_attr);
      }
      else {
        if ($this->bind_method == LDAP_SERVERS_BIND_METHOD_ANON_USER) {
          $result = array(
            'dn' =>  $match['dn'],
            'mail' => $this->deriveEmailFromEntry($match),
            'attr' => $match,
            );
          return $result;
        }
        else {
          continue;
        }
      }

      // Finally, we must filter out results with spaces added before
      // or after, which are considered OK by LDAP but are no good for us
      // We allow lettercase independence, as requested by Marc Galera
      // on http://drupal.org/node/97728
      //
      // Some setups have multiple $name_attr per entry, as pointed out by
      // Clarence "sparr" Risher on http://drupal.org/node/102008, so we
      // loop through all possible options.
      foreach ($match[$name_attr] as $value) {
        if (drupal_strtolower(trim($value)) == drupal_strtolower($ldap_username)) {
          $result = array(
            'dn' =>  $match['dn'],
            'mail' => $this->deriveEmailFromEntry($match),
            'attr' => $match,
          );

          return $result;
        }
      }
    }
  }

  public function deriveEmailFromEntry($ldap_entry) {
    if ($this->mail_attr) { // not using template
      return @$ldap_entry[$this->mail_attr][0];
    }
    elseif ($this->mail_template) {  // template is of form [cn]@illinois.edu
      require_once('ldap_servers.functions.inc');
      return ldap_server_token_replace($ldap_entry, $this->mail_template);
    }
    else {
      return FALSE;
    }
  }


  /**
   * Error methods and properties.
   */

  public $detailedWatchdogLog = FALSE;
  protected $_errorMsg = NULL;
  protected $_hasError = FALSE;
  protected $_errorName = NULL;

  public function setError($_errorName, $_errorMsgText = NULL) {
    $this->_errorMsgText = $_errorMsgText;
    $this->_errorName = $_errorName;
    $this->_hasError = TRUE;
  }

  public function clearError() {
    $this->_hasError = FALSE;
    $this->_errorMsg = NULL;
    $this->_errorName = NULL;
  }

  public function hasError() {
    return ($this->_hasError || $this->ldapErrorNumber());
  }

  public function errorMsg($type = NULL) {
    if ($type == 'ldap' && $this->connection) {
      return ldap_err2str(ldap_errno($this->connection));
    }
    elseif ($type == NULL) {
      return $this->_errorMsg;
    }
    else {
      return NULL;
    }
  }

  public function errorName($type = NULL) {
    if ($type == 'ldap' && $this->connection) {
      return "LDAP Error: " . ldap_error($this->connection);
    }
    elseif ($type == NULL) {
      return $this->_errorName;
    }
    else {
      return NULL;
    }
  }

  public function ldapErrorNumber() {
    if ($this->connection && ldap_errno($this->connection)) {
      return ldap_errno($this->connection);
    }
    else {
      return FALSE;
    }
  }

}
