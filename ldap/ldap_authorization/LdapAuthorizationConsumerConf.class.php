<?php
// $Id$
/**
 * @file
 * class to encapsulate an ldap entry to authorization consumer ids mapping configuration
 *
 * this is the lightweight version of the class for use on logon etc.
 * the LdapAuthorizationConsumerConfAdmin extends this class and has save,
 * iterate, etc methods.
 *
 */

/**
 * LDAP Authorization Consumer Configuration
 */
class LdapAuthorizationConsumerConf {

  public $sid = NULL;
  public $consumerType = NULL;
  public $consumerModule = NULL;
  public $consumer = NULL;
  public $inDatabase = FALSE;


  public $description = NULL;
  public $status = NULL;
  public $onlyApplyToLdapAuthenticated = TRUE;

  public $deriveFromDn = FALSE;
  public $deriveFromDnAttr = NULL;

  public $deriveFromAttr = FALSE;
  public $deriveFromAttrAttr = NULL;
  public $deriveFromAttrUseFirstAttr = FALSE;

  public $deriveFromEntry = FALSE;
  public $deriveFromEntryEntries = NULL;
  public $deriveFromEntryAttr = NULL;
  public $deriveFromEntrySearchAll = FALSE;


  public $mappings = array();
  public $useMappingsAsFilter = TRUE;

  public $synchToLdap = FALSE;

  public $synchOnLogon = TRUE;
  public $synchManually = TRUE;

  public $revokeLdapProvisioned = TRUE;
  public $regrantLdapProvisioned = TRUE;
  public $createConsumers = TRUE;

  public $errorMsg = NULL;
  public $hasError = FALSE;
  public $errorName = NULL;


  public function clearError() {
    $this->hasError = FALSE;
    $this->errorMsg = NULL;
    $this->errorName = NULL;
  }
   /**
   * Constructor Method
   */
  function __construct(&$consumer, $_new = FALSE, $_sid = NULL) {
    $this->consumer = $consumer;
    $this->consumerType = $consumer->consumerType;
    if ($_new) {
      $this->inDatabase = FALSE;
    }
    else {
      $this->inDatabase = TRUE;
      $this->loadFromDb();
    }
  }

  protected function loadFromDb() {
    if (module_exists('ctools')) {
      ctools_include('export');
      if ($this->consumerType) {
        $result = ctools_export_load_object('ldap_authorization', 'names', array($this->consumerType));
      }
      else {
        $result = ctools_export_load_object('ldap_authorization', 'all');
      }
      // @todo, this is technically wrong, but I don't quite grok what we're doing in the non-ctools case - justintime
      $consumer_conf = array_pop($result);
      // There's no ctools api call to get the reserved properties, so instead of hardcoding a list of them
      // here, we just grab everything.  Basically, we sacrifice a few bytes of RAM for forward-compatibility.
      if ($consumer_conf) {
        foreach ($consumer_conf as $property => $value) {
          $this->$property = $value;
        }
      }
    }
    else {
      $select = db_select('ldap_authorization', 'ldap_authorization');
      $select->fields('ldap_authorization');
      if ($this->consumerType) {
        $select->condition('ldap_authorization.consumer_type',  $this->consumerType);
      }

      $consumer_conf = $select->execute()->fetchObject();
    }

    if (!$consumer_conf) {
      $this->inDatabase = FALSE;
      return;
    }

    $this->sid = $consumer_conf->sid;
    $this->consumerType = $consumer_conf->consumer_type;
    $this->description = $consumer_conf->description;
    $this->status = (bool)$consumer_conf->status;
    $this->onlyApplyToLdapAuthenticated  = (bool)(@$consumer_conf->only_ldap_authenticated);

    $this->deriveFromDn  = (bool)(@$consumer_conf->derive_from_dn);
    $this->deriveFromDnAttr = $consumer_conf->derive_from_dn_attr;

    $this->deriveFromAttr  = (bool)($consumer_conf->derive_from_attr);
    $this->deriveFromAttrAttr =  $this->linesToArray($consumer_conf->derive_from_attr_attr);
    $this->deriveFromAttrUseFirstAttr  = (bool)($consumer_conf->derive_from_attr_use_first_attr);
    $this->deriveFromEntrySearchAll = (bool)($consumer_conf->derive_from_entry_search_all);


    $this->deriveFromEntry  = (bool)(@$consumer_conf->derive_from_entry);
    $this->deriveFromEntryEntries = $this->linesToArray($consumer_conf->derive_from_entry_entries);
    $this->deriveFromEntryAttr = $consumer_conf->derive_from_entry_attr;

    $this->mappings = $this->pipeListToArray($consumer_conf->mappings);
    $this->useMappingsAsFilter  = (bool)(@$consumer_conf->use_filter);

    $this->synchToLdap = (bool)(@$consumer_conf->synch_to_ldap);
    $this->synchOnLogon = (bool)(@$consumer_conf->synch_on_logon);
    $this->regrantLdapProvisioned = (bool)(@$consumer_conf->regrant_ldap_provisioned);
    $this->revokeLdapProvisioned = (bool)(@$consumer_conf->revoke_ldap_provisioned);
    $this->createConsumers = (bool)(@$consumer_conf->create_consumers);


  }
  /**
   * Destructor Method
   */
  function __destruct() {

  }

  protected $_sid;
  protected $_new;

  protected $saveable = array(
    'sid',
    'consumerType',
    'description',
    'status',
    'onlyApplyToLdapAuthenticated',
    'deriveFromDn',
    'deriveFromDnAttr',
    'deriveFromAttr',
    'deriveFromAttrAttr',
    'deriveFromAttrUseFirstAttr',
    'deriveFromEntry',
    'deriveFromEntryEntries',
    'deriveFromEntryAttr',
    'deriveFromEntrySearchAll',
    'mappings',
    'useMappingsAsFilter',
    'synchToLdap',
    'synchOnLogon',
    'synchManually',
    'revokeLdapProvisioned',
    'createConsumers',
    'regrantLdapProvisioned',

  );


  protected function linesToArray($lines) {
    $lines = trim($lines);

    if ($lines) {
      $array = preg_split('/[\n\r]+/', $lines);
      foreach ($array as $i => $value) {
        $array[$i] = trim($value);
      }
    }
    else {
      $array = array();
    }
    return $array;
  }




  protected function pipeListToArray($mapping_list_txt) {
    $result_array = array();
    $mappings = preg_split('/[\n\r]+/', $mapping_list_txt);
    foreach ($mappings as $line) {
      if (count($mapping = explode('|', trim($line))) == 2) {
        $result_array[] = array(trim($mapping[0]), trim($mapping[1]));
      }
    }
    return $result_array;
  }
}
