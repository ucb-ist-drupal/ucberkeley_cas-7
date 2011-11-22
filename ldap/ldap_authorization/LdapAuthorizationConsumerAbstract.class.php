<?php
// $Id: LdapAuthorizationConsumerAbstract.class.php,v 1.2.2.1 2011/02/08 20:05:41 johnbarclay Exp $

/**
 * @file
 * abstract class to represent an ldap_authorization consumer
 * such as drupal_role, og_group, etc.  each authorization comsumer
 * will extend this class with its own class named
 * LdapAuthorizationConsumer<consumer type> such as LdapAuthorizationConsumerDrupalRole
 *
 */

class LdapAuthorizationConsumerAbstract {

  public $name;  // e.g. drupal role, og group
  public $namePlural; // e.g. drupal roles, og groups
  public $shortName; // e.g. role, group
  public $shortNamePlural; // e.g. roles, groups
  public $description;
  public $consumerConf; // each consumer type has cosumer conf object
  public $consumerModule;

  protected $_availableConsumerIDs;


  /**
   * @property boolean $allowSynchBothDirections
   *
   *  Does this consumer module support synching in both directions?
   *
   */
  public $allowSynchBothDirections = FALSE;

   /**
   * @property boolean $allowConsumerObjectCreation
   *
   *  Does this consumer module support creating consumer objects
   * (drupal roles,  og groups, etc.)
   *
   */

  public $allowConsumerObjectCreation = FALSE;


  /**
   * default consumer conf property values for this consumer type.
   * Should be overridden by child classes as appropriate
   */

  public $onlyApplyToLdapAuthenticatedDefault = TRUE;
  public $useMappingsAsFilterDefault = TRUE;
  public $synchOnLogonDefault = TRUE;
  public $synchManuallyDefault = TRUE;
  public $revokeLdapProvisionedDefault = TRUE;
  public $regrantLdapProvisioned = TRUE;
  public $createConsumersDefault = TRUE;
  public $detailedWatchdogLog = FALSE;



   /**
   * @property array $defaultableConsumerConfProperties
   * properties a consumer may provide defaults for
   * should include every item in "default mapping property values" above
   */
  public $defaultableConsumerConfProperties = array(
      'onlyApplyToLdapAuthenticated',
      'useMappingsAsFilter',
      'synchOnLogon',
      'synchManually',
      'revokeLdapProvisioned',
      'regrantLdapProvisioned',
      'createConsumers'
      );


 /**
   * Constructor Method
   *
   */
  function __construct($consumer_type, $params) {
    $this->consumerType = $consumer_type;
    $this->name = $params['consumer_name'];
    $this->namePlural= $params['consumer_name_plural'];
    $this->shortName = $params['consumer_short_name'];
    $this->shortNamePlural= $params['consumer_short_name_plural'];
    $this->description = $params['consumer_description'];
    $this->consumerModule = $params['consumer_module'];

    require_once('LdapAuthorizationConsumerConfAdmin.class.php');
    $this->consumerConf = new LdapAuthorizationConsumerConf($this);
  }


  /**
   * get list of all authorization consumer ids available to a this authorization consumer.  For
   * example for drupal_roles, this would be an array of drupal roles such
   * as array('admin', 'author', 'reviewer',... ).  For organic groups in
   * might be all the names of organic groups.
   *
   * return array in form array(id1, id2, id3,...)
   *
   */
  public function availableConsumerIDs() {
     // method must be overridden
  }

  /**
   *
   * create authorization consumers
   *
   * @param array $creates an array of authorization consumer ids in form array(id1, id2, id3,...)
   *
   * return array in form array(id1, id2, id3,...) representing all
   *   existing consumer ids ($this->availableConsumerIDs())
   *
   */
  public function createConsumers($creates) {
    // method must be overridden
  }

  /**
   * grant authorizations to a user
   *
   * @param object $user drupal user object
   *
   * @param $consumer_ids string or array of strings that are authorization consumer ids
   *
   * @param array $ldap_entry is ldap data from ldap entry which drupal user is mapped to
   *
   * @param boolean $user_save.  should user object be saved by authorizationGrant method
   *
   * @return array $results.  Array of form
   *   array(
   *    <authz consumer id1> => 1,
   *    <authz consumer id2> => 0,
   *   )
   *   where 1s and 0s represent success and failure to grant
   *
   *
   *  method may be desireable to override, if consumer benefits from adding grants as a group rather than one at a time
   */

  public function authorizationGrant(&$user, &$user_auth_data, $consumer_ids, &$ldap_entry, $user_save = TRUE) {
    $this->grantsAndRevokes('grant', $user, $user_auth_data, $consumer_ids, $ldap_entry, $user_save);
  }

  /**
   * revoke authorizations to a user
   *
   * @param object $user drupal user object
   *
   * @param $consumer_ids string or array of strings that are authorization consumer ids
   *
   * @param array $ldap_entry is ldap data from ldap entry which drupal user is mapped to
   *
   * @param boolean $user_save.  should user object be saved by authorizationGrant method
   *
   * @return array $results.  Array of form
   *   array(
   *    <authz consumer id1> => 1,
   *    <authz consumer id2> => 0,
   *   )
   *   where 1s and 0s represent success and failure to revoke
   *  $user_auth_data is returned by reference
   *
   *  method may be desireable to override, if consumer benefits from revoking grants as a group rather than one at a time
   */

  public function authorizationRevoke(&$user, &$user_auth_data, $consumer_ids, &$ldap_entry, $user_save = TRUE) {
    $this->grantsAndRevokes('revoke', $user, $user_auth_data, $consumer_ids, $ldap_entry, $user_save);
  }


  protected function grantsAndRevokes($op, &$user, &$user_auth_data, $consumer_ids, &$ldap_entry, $user_save = TRUE) {
    $results = array();
    $watchdog_tokens = array();
    if (!is_array($consumer_ids)) {
      $consumer_ids = array($consumer_ids);
    }
    $watchdog_tokens['%username'] = $user->name;
    $watchdog_tokens['%action'] = $op;
    $consumer_ids_log = array();
    $users_authorization_ids = $this->usersAuthorizations($user);

    $watchdog_tokens['%users_authorization_ids'] = join(', ', $users_authorization_ids);
    foreach ($consumer_ids as $consumer_id) {
      $log = "consumer_id=$consumer_id, op=$op,";
      $results[$consumer_id] = TRUE;
      if ($op == 'grant'  && !in_array($consumer_id, $users_authorization_ids)) {
        $log .='grant existing role, ';
        if (!in_array($consumer_id, $this->availableConsumerIDs(TRUE))) {
          $log .= "consumer id not available for $op, ";
          if ($this->allowConsumerObjectCreation) {

            $this->createConsumers(array($consumer_id));
            if (in_array($consumer_id, $this->availableConsumerIDs(TRUE))) {
              $this->grantSingleAuthorization($user, $consumer_id, $user_auth_data);  // allow consuming module to add additional data to $user_auth_data
              $user_auth_data[$consumer_id] = array('date_granted' => time() );
              $log .= "created consumer object, ";
            }
            else {
              $log .= "tried and failed to create consumer object, ";
              $results[$consumer_id] = FALSE;
               // out of luck, failed to create consumer id
            }
          }
          else {
            $log .= "consumer does not support creating consumer object, ";
            // out of luck. can't create new consumer id.
            $results[$consumer_id] = FALSE;
          }
        }

        if ($results[$consumer_id]) {
          $log .= "granting existing consumer object, ";
          $results[$consumer_id] = $this->grantSingleAuthorization($user, $consumer_id, $user_auth_data); // allow consuming module to add additional data to $user_auth_data
          if ($results[$consumer_id]) {
            $user_auth_data[$consumer_id] = array('date_granted' => time() );
          }
          $log .= t(',result=') . (boolean)($results[$consumer_id]);
        }
      }
      elseif ($op == 'revoke') {
        if (isset($user_auth_data[$consumer_id])) {
          $log .= "revoking existing consumer object, ";
          if (in_array($consumer_id, $users_authorization_ids)) {
            $results[$consumer_id] = $this->revokeSingleAuthorization($user, $consumer_id, $user_auth_data);  // defer to default for $user_save param
            if ($results[$consumer_id]) {
              unset($user_auth_data[$consumer_id]);
            }
            $log .= t(',result=') . (boolean)($results[$consumer_id]);
          }
          else {
            unset($user_auth_data[$consumer_id]);
          }
        }
      }
      $consumer_ids_log[] = $log;
    }
    $watchdog_tokens['%consumer_ids_log'] = (count($consumer_ids_log)) ? join('<hr/>', $consumer_ids_log) : t('no actions');
    if ($user_save) {
      $user_edit = array();
      $user_edit['data']['ldap_authorizations'][$this->consumerType] = $user_auth_data;
      $user = user_save($user, $user_edit);
    }
    watchdog('ldap_authorization', '%username:
      <hr/>LdapAuthorizationConsumerAbstract grantsAndRevokes() method log.  action=%action:<br/> %consumer_ids_log
      ',
      $watchdog_tokens, WATCHDOG_DEBUG);
  }

  /**
   * @param drupal user object $user to have $consumer_id revoked
   * @param string $consumer_id $consumer_id such as drupal role name, og group name, etc.
   * @param array $user_auth_data array of $user data specific to this consumer type.
   *   stored in $user->data['ldap_authorization'][<consumer_type>] array
   *
   * return boolen TRUE on success, FALSE on fail.  If user save is FALSE, the user object will
   *   not be saved and reloaded, so a returned TRUE may be misleading.
   */

  public function revokeSingleAuthorization(&$user, $role_name, &$user_auth_data) {
     // method must be overridden
  }

  /**
  * @param drupal user object $user to have $consumer_id granted
  * @param string $consumer_id $consumer_id such as drupal role name, og group name, etc.
   * @param array $user_auth_data array of $user data specific to this consumer type.
   *   stored in $user->data['ldap_authorization'][<consumer_type>] array
  *
  * return boolen TRUE on success, FALSE on fail.  If user save is FALSE, the user object will
  *   not be saved and reloaded, so a returned TRUE may be misleading.
  */
  public function createSingleAuthorization(&$user, $role_name, &$user_auth_data) {
     // method must be overridden

  }


}
