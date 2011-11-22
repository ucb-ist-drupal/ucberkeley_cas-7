<?php
// $Id$
/**
 * @file
 * ldap_authorization_simpletest.ldap_servers.inc
 */

/**
 * Implements hook_default_ldap_servers().
 */
function ldap_authorization_simpletest1_default_ldap_servers() {
  $export = array();

  $ldap_servers_conf = new stdClass;
  $ldap_servers_conf->disabled = FALSE; /* Edit this to true to make a default ldap_servers_conf disabled initially */
  $ldap_servers_conf->api_version = 1;
  $ldap_servers_conf->sid = 'ldapauthor1';
  $ldap_servers_conf->name = 'Test LDAP Server 1 for LDAP Authorization';
  $ldap_servers_conf->status = 1;
  $ldap_servers_conf->ldap_type = 'ad';
  $ldap_servers_conf->address = 'ad.myuniveristy.edu';
  $ldap_servers_conf->port = 389;
  $ldap_servers_conf->tls = FALSE;
  $ldap_servers_conf->bind_method = LDAP_SERVERS_BIND_METHOD_SERVICE_ACCT;
  $ldap_servers_conf->basedn = serialize(array(
       'ou=campus accounts,dc=ad,dc=myuniveristy,dc=edu',
       'ou=education,dc=ad,dc=myuniveristy,dc=edu',
       'ou=guest accounts,dc=ad,dc=myuniveristy,dc=edu',
      ));
  $ldap_servers_conf->binddn = 'cn=service-account,dc=ad,dc=myuniveristy,dc=edu';
  $ldap_servers_conf->user_dn_expression = '';
  $ldap_servers_conf->user_attr = 'sAMAccountName';
  $ldap_servers_conf->mail_attr = 'mail';
  $ldap_servers_conf->ldap_to_drupal_user = '';
  $ldap_servers_conf->testing_drupal_username = 'jdoe';
  $ldap_servers_conf->weight = 0;
  $export['ldapauthor1'] = $ldap_servers_conf;

  return $export;
}
