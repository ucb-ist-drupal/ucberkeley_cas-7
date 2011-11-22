<?php
// $Id:  $



/**
 * @file
 * abstract class to represent an ldap_authorization consumer
 * such as drupal_role, og_group, etc.
 *
 */
require_once(drupal_get_path('module', 'ldap_authorization') . '/LdapAuthorizationConsumerAbstract.class.php');
class LdapAuthorizationConsumerDrupalRole extends LdapAuthorizationConsumerAbstract {

  public $consumerType = 'drupal_role';
  public $allowSynchBothDirections = FALSE;
  public $allowConsumerObjectCreation = TRUE;
  public $onlyApplyToLdapAuthenticatedDefault = TRUE;
  public $useMappingsAsFilterDefault = TRUE;
  public $synchOnLogonDefault = TRUE;
  public $synchManuallyDefault = TRUE;
  public $revokeLdapProvisionedDefault = TRUE;
  public $regrantLdapProvisionedDefault = TRUE;
  public $createContainersDefault = TRUE;
  public $drupalRolesByName = array();

 /**
   * Constructor Method
   *
   */
  function __construct($consumer_type = NULL) {
    $params = ldap_authorization_drupal_role_ldap_authorization_consumer();
    $this->refreshConsumerIDs();
    parent::__construct('drupal_role', $params['drupal_role']);
  }

  public function refreshConsumerIDs() {
    $this->drupalRolesByName = array_flip(user_roles());
    $this->_availableConsumerIDs = array_values(user_roles(TRUE));
  }

  public function availableConsumerIDs($reset = FALSE) {
    if ($reset || ! is_array($this->_availableConsumerIDs)) {
      $this->refreshConsumerIDs();
    }
    return $this->_availableConsumerIDs;
  }

  /**
   * extends createConsumer method of base class
   *
   * creates of drupal roles may be mixed case.  drupal doesn't
   * differentiate, so case is ignored in comparing, but preserved
   * for the actual created role name saved.
   *
   * **/

  public function createConsumers($creates_mixed_case) {

    // 1. determins difference between existing drupal roles and ones that are requested to be created
    $existing_roles_mixed_case = $this->availableConsumerIDs();
    $creates_lower_case = array_map('drupal_strtolower', $creates_mixed_case);
    $existing_roles_lower_case = array_map('drupal_strtolower', $existing_roles_mixed_case);
    $roles_map_lc_to_mixed_case = array_combine($creates_lower_case, $creates_mixed_case);
    $roles_to_create =  array_unique(array_diff($creates_lower_case, $existing_roles_lower_case));

    // 2. create each role that is needed
    foreach ($roles_to_create as $i => $role_name_lowercase) {
      if (strlen($role_name_lowercase) > 63) {
        watchdog('ldap_authorization_drupal_role', 'Tried to create drupal role with name of over 63 characters (%group_name).  Please correct your drupal ldap_authorization settings', array('%group_name' => $role_name_lowercase));
        continue;
      }
      $role = new stdClass();
      $role->name = $roles_map_lc_to_mixed_case[$role_name_lowercase];
      if (! ($status = user_role_save($role))) {
        // if role is not created, remove from array to user object doesn't have it stored as granted
        watchdog('user', 'failed to create drupal role %role in ldap_authorizations module', array('%role' => $role->name));
      }
      else {
        $created[] = $role->name;
        watchdog('user', 'drupal role %role in ldap_authorizations module', array('%role' => $role->name));
      }
    }
    // 3. return all existing user roles and flush cache of consumer ids.
    $refreshed_available_consumer_ids = $this->availableConsumerIDs(TRUE);
    if ($this->detailedWatchdogLog) {
      $watchdog_tokens = array('%roles_to_create' => join(", ", $roles_to_create));
      $watchdog_tokens = array('%existing_roles' => join(", ", $existing_roles_mixed_case));
      $watchdog_tokens = array('%refreshed_available_consumer_ids' => join(", ", $refreshed_available_consumer_ids));
      watchdog('ldap_authorization',
        'LdapAuthorizationConsumerDrupalRole.createConsumers()
        roles to create: %roles_to_create;
        existing roles: %existing_roles;
        available roles after createConsumers call: %refreshed_available_consumer_ids;',
        $watchdog_tokens,
        WATCHDOG_DEBUG);
    }


    return $refreshed_available_consumer_ids;  // return actual roles that exist, in case of failure

  }

  public function revokeSingleAuthorization(&$user, $role_name, &$user_auth_data) {

    $user_edit = array('roles' => array_diff($user->roles, array($this->drupalRolesByName[$role_name] => $role_name)));
    $user = user_save($user, $user_edit);
    $result = ($user && !isset($user->roles[$this->drupalRolesByName[$role_name]]));

    if ($this->detailedWatchdogLog) {
      watchdog('ldap_authorization', 'LdapAuthorizationConsumerDrupalRole.revokeSingleAuthorization()
        revoked:  rid=%rid, role_name=%role_name for username=%username, result=%result',
        array('%rid' => $this->drupalRolesByName[$role_name], '%role_name' => $role_name, '%username' => $user->name,
          '%result' => $result), WATCHDOG_DEBUG);
    }

    return $result;

  }

  /**
   * extends grantSingleAuthorization()
   */

  public function grantSingleAuthorization(&$user, $role_name, &$user_auth_data) {
    if (! isset($this->drupalRolesByName[$role_name])) {
        watchdog('ldap_authorization', 'LdapAuthorizationConsumerDrupalRole.grantSingleAuthorization()
        failed to grant %username the role %role_name because role does not exist',
        array('%role_name' => $role_name, '%username' => $user->name),
        WATCHDOG_ERROR);
        return FALSE;
    }

    $user_edit = array('roles' => $user->roles + array($this->drupalRolesByName[$role_name] => $role_name));
    $user = user_save($user, $user_edit);
    $result = ($user && isset($user->roles[$this->drupalRolesByName[$role_name]]));


    if ($this->detailedWatchdogLog) {
      watchdog('ldap_authorization', 'LdapAuthorizationConsumerDrupalRole.grantSingleAuthorization()
        granted: rid=%rid, role_name=%role_name for username=%username, result=%result',
        array('%rid' => $this->drupalRolesByName[$role_name], '%role_name' => $role_name, '%username' => $user->name,
          '%result' => $result), WATCHDOG_DEBUG);
    }

    return $result;

  }

  public function usersAuthorizations(&$user) {
    return array_values($user->roles);
  }


}
