<?php
// $Id$

/**
 * @file
 * simpletest ldap server fake data
 */

$fake_ldap_server_data['ldapauthor1']['methodResponses']['connect'] = LDAP_SUCCESS;

$fake_ldap_server_data['ldapauthor1']['search_results']['member=cn=verykool,ou=special guests,ou=guest accounts,dc=ad,dc=myuniveristy,dc=edu']['ou=groups,dc=ad,dc=myuniveristy,dc=edu'] = array(
  0 => array('count' => 1, 'dn' => 'ou=content editors,ou=groups,dc=ad,dc=myuniveristy,dc=edu'),
  1 => array('count' => 1, 'dn' => 'ou=content approvers,ou=groups,dc=ad,dc=myuniveristy,dc=edu'),
  'count' => 2,
);


/**
 * fake ldap data.  should return data in format that php ldap extension methods would
 *
 * users go in $servers[<sid>]['users'][<dn>] where <sid> is the server id
 */
$fake_ldap_server_data['ldapauthor1']['users']['cn=verykool,ou=special guests,ou=guest accounts,dc=ad,dc=myuniveristy,dc=edu']['attr'] = array(
  'dn' => 'cn=verykool,ou=special guests,ou=guest accounts,dc=ad,dc=myuniveristy,dc=edu',
  'mail' => array( 0 => 'verykool@myuniversity.edu', 'count' => 1),
  'sAMAccountName' => array( 0 => 'verykool', 'count' => 1),
  'password' => array( 0 => 'goodpwd', 'count' => 1),
  'memberOf' => array(
    0 => 'cn=netadmins,ou=it,dc=ad,dc=myuniveristy,dc=edu',
    'count' => 1,
    ),
);

/**
 * test users should include service account if one is being used
 */
$fake_ldap_server_data['ldapauthor1']['users']['cn=service-account,dc=ad,dc=myuniveristy,dc=edu']['attr'] = array(
  'dn' => 'cn=service-account,dc=ad,dc=myuniveristy,dc=edu',
  'mail' => array( 0 => 'service-account@myuniversity.edu', 'count' => 1),
  'sAMAccountName' => array( 0 => 'service-account', 'count' => 1),
  'memberOf' => array(
    0 => 'CN=service_accounts,OU=ServiceAccountGroups,DC=ad,DC=myuniveristy,DC=edu',
    'count' => 1,
    ),
  'password' => array( 0 => 'goodpwd', 'count' => 1),
);
