<?php
// $Id$
/**
 * @file
 * ldap_authorization_simpletest.ldap_authorization.inc
 */

/**
 * Implements hook_default_ldap_authorization().
 */
function ldap_authorization_simpletest1_default_ldap_authorization() {
  $export = array();

  $ldap_authorization_conf = new stdClass;
  $ldap_authorization_conf->disabled = FALSE; /* Edit this to true to make a default ldap_authorization_conf disabled initially */
  $ldap_authorization_conf->api_version = 1;
  $ldap_authorization_conf->sid = 'ldapauthor1';
  $ldap_authorization_conf->consumer_type = 'drupal_role';
  $ldap_authorization_conf->consumer_module = 'ldap_authorization_drupal_role';
  $ldap_authorization_conf->description = 'UIUC AD';
  $ldap_authorization_conf->status = TRUE;
  $ldap_authorization_conf->only_ldap_authenticated = TRUE;
  $ldap_authorization_conf->derive_from_dn = FALSE;
  $ldap_authorization_conf->derive_from_dn_attr = NULL;
  $ldap_authorization_conf->derive_from_attr = TRUE;
  $ldap_authorization_conf->derive_from_attr_attr = 'memberOf';
  $ldap_authorization_conf->derive_from_entry = FALSE;
  $ldap_authorization_conf->derive_from_entry_entries = '';
  $ldap_authorization_conf->derive_from_entry_attr = '';
  $ldap_authorization_conf->mappings = 'cn=netadmins,ou=it,dc=ad,dc=myuniveristy,dc=edu|netadmins';
  $ldap_authorization_conf->use_filter = TRUE;
  $ldap_authorization_conf->synch_to_ldap = FALSE;
  $ldap_authorization_conf->synch_on_logon = TRUE;
  $ldap_authorization_conf->revoke_ldap_provisioned = TRUE;
  $ldap_authorization_conf->create_consumers = TRUE;
  $ldap_authorization_conf->regrant_ldap_provisioned = TRUE;
  $export['drupal_role'] = $ldap_authorization_conf;

  return $export;
}
