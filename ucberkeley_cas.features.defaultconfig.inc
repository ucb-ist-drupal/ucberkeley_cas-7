<?php
/**
 * @file
 * ucberkeley_cas.features.defaultconfig.inc
 */

/**
 * Implements hook_defaultconfig_features().
 */
function ucberkeley_cas_defaultconfig_features() {
  return array(
    'ucberkeley_cas' => array(
      'default_ldap_servers' => 'default_ldap_servers',
      'strongarm' => 'strongarm',
    ),
  );
}

/**
 * Implements hook_defaultconfig_default_ldap_servers().
 */
function ucberkeley_cas_defaultconfig_default_ldap_servers() {
  $export = array();

  $ldap_servers_conf = new stdClass();
  $ldap_servers_conf->disabled = FALSE; /* Edit this to true to make a default ldap_servers_conf disabled initially */
  $ldap_servers_conf->api_version = 1;
  $ldap_servers_conf->sid = 'ucb_prod';
  $ldap_servers_conf->name = 'ldap.berkeley.edu';
  $ldap_servers_conf->status = TRUE;
  $ldap_servers_conf->ldap_type = 'default';
  $ldap_servers_conf->address = 'ldap.berkeley.edu';
  $ldap_servers_conf->port = 389;
  $ldap_servers_conf->tls = FALSE;
  $ldap_servers_conf->bind_method = TRUE;
  $ldap_servers_conf->binddn = '';
  $ldap_servers_conf->bindpw = '';
  $ldap_servers_conf->basedn = array(
    0 => 'ou=people,dc=berkeley,dc=edu',
  );
  $ldap_servers_conf->user_attr = 'uid';
  $ldap_servers_conf->account_name_attr = '';
  $ldap_servers_conf->mail_attr = 'mail';
  $ldap_servers_conf->mail_template = '';
  $ldap_servers_conf->allow_conflicting_drupal_accts = FALSE;
  $ldap_servers_conf->unique_persistent_attr = '';
  $ldap_servers_conf->user_dn_expression = '';
  $ldap_servers_conf->ldap_to_drupal_user = '';
  $ldap_servers_conf->testing_drupal_username = '213108';
  $ldap_servers_conf->group_object_category = '';
  $ldap_servers_conf->search_pagination = FALSE;
  $ldap_servers_conf->search_page_size = 1000;
  $ldap_servers_conf->weight = 0;
  $export['ucb_prod'] = $ldap_servers_conf;

  $ldap_servers_conf = new stdClass();
  $ldap_servers_conf->disabled = FALSE; /* Edit this to true to make a default ldap_servers_conf disabled initially */
  $ldap_servers_conf->api_version = 1;
  $ldap_servers_conf->sid = 'ucb_test';
  $ldap_servers_conf->name = 'ldap-test.berkeley.edu';
  $ldap_servers_conf->status = TRUE;
  $ldap_servers_conf->ldap_type = 'default';
  $ldap_servers_conf->address = 'ldap-test.berkeley.edu';
  $ldap_servers_conf->port = 389;
  $ldap_servers_conf->tls = FALSE;
  $ldap_servers_conf->bind_method = TRUE;
  $ldap_servers_conf->binddn = '';
  $ldap_servers_conf->bindpw = '';
  $ldap_servers_conf->basedn = array(
    0 => 'ou=people,dc=berkeley,dc=edu',
  );
  $ldap_servers_conf->user_attr = 'uid';
  $ldap_servers_conf->account_name_attr = '';
  $ldap_servers_conf->mail_attr = 'mail';
  $ldap_servers_conf->mail_template = '';
  $ldap_servers_conf->allow_conflicting_drupal_accts = FALSE;
  $ldap_servers_conf->unique_persistent_attr = '';
  $ldap_servers_conf->user_dn_expression = '';
  $ldap_servers_conf->ldap_to_drupal_user = '';
  $ldap_servers_conf->testing_drupal_username = '213108';
  $ldap_servers_conf->group_object_category = '';
  $ldap_servers_conf->search_pagination = FALSE;
  $ldap_servers_conf->search_page_size = 1000;
  $ldap_servers_conf->weight = 0;
  $export['ucb_test'] = $ldap_servers_conf;

  return $export;
}

/**
 * Implements hook_defaultconfig_strongarm().
 */
function ucberkeley_cas_defaultconfig_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_access';
  $strongarm->value = '0';
  $export['cas_access'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_attributes';
  $strongarm->value = array(
    'sync_every_login' => '0',
    'relations' => array(
      'name' => '[cas:ldap:displayname]',
      'mail' => '[cas:ldap:mail]',
    ),
    'ldap' => array(
      'server' => 'ucb_test',
    ),
  );
  $export['cas_attributes'] = $strongarm;

  // Ensure that we have the correct rid for authenticated user.  The rid could be something other than "2"
  $rid_auth_user = array_search('authenticated user', user_roles());
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_auto_assigned_role';
  $strongarm->value = array(
    $rid_auth_user => 1,
  );
  $export['cas_auto_assigned_role'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_cert';
  $strongarm->value = '';
  $export['cas_cert'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_changePasswordURL';
  $strongarm->value = '';
  $export['cas_changePasswordURL'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_check_first';
  $strongarm->value = 0;
  $export['cas_check_first'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_debugfile';
  $strongarm->value = '';
  $export['cas_debugfile'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_domain';
  $strongarm->value = '';
  $export['cas_domain'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_exclude';
  $strongarm->value = '';
  $export['cas_exclude'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_first_login_destination';
  $strongarm->value = '';
  $export['cas_first_login_destination'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_hide_email';
  $strongarm->value = 0;
  $export['cas_hide_email'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_hide_password';
  $strongarm->value = 1;
  $export['cas_hide_password'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_library_dir';
  $strongarm->value = 'CAS';
  $export['cas_library_dir'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_library_use_libraries';
  $strongarm->value = '0';
  $export['cas_library_use_libraries'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_login_drupal_invite';
  $strongarm->value = '';
  $export['cas_login_drupal_invite'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_login_form';
  $strongarm->value = '2';
  $export['cas_login_form'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_login_invite';
  $strongarm->value = 'Log in using CalNet';
  $export['cas_login_invite'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_login_message';
  $strongarm->value = 'Logged in via CalNet as %cas_username.';
  $export['cas_login_message'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_login_redir_message';
  $strongarm->value = 'You will be redirected to the secure CalNet login page.';
  $export['cas_login_redir_message'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_logout_destination';
  $strongarm->value = '<front>';
  $export['cas_logout_destination'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_pages';
  $strongarm->value = '';
  $export['cas_pages'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_pgtformat';
  $strongarm->value = 'plain';
  $export['cas_pgtformat'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_pgtpath';
  $strongarm->value = '';
  $export['cas_pgtpath'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_port';
  $strongarm->value = '443';
  $export['cas_port'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_proxy';
  $strongarm->value = 0;
  $export['cas_proxy'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_registerURL';
  $strongarm->value = '';
  $export['cas_registerURL'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_server';
  $strongarm->value = 'auth-test.berkeley.edu';
  $export['cas_server'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_uri';
  $strongarm->value = '/cas';
  $export['cas_uri'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_user_register';
  $strongarm->value = 1;
  $export['cas_user_register'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_version';
  $strongarm->value = '2.0';
  $export['cas_version'] = $strongarm;

  return $export;
}
