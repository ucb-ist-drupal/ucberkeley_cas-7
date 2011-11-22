<?php
// $Id$

/**
 * @file
 * simpltest authorization config
 */

$servers['ldapauthor1']['properties']  = array(
    'name'  => 'Test LDAP Server 1 for LDAP Authorization' ,
    'inDatabase' => TRUE,
    'status'  => 1,
    'ldap_type'  => 'ad',
    'address'  => 'ad.myuniveristy.edu',
    'port'  => 389,
    'tls'  => FALSE,
    'bind_method' => LDAP_SERVERS_BIND_METHOD_SERVICE_ACCT,
    'basedn'  => array(
       'ou=campus accounts,dc=ad,dc=myuniveristy,dc=edu',
       'ou=education,dc=ad,dc=myuniveristy,dc=edu',
       'ou=guest accounts,dc=ad,dc=myuniveristy,dc=edu',
      ),
    'binddn'  => 'cn=service-account,dc=ad,dc=myuniveristy,dc=edu',
    'bindpw' => 'goodpwd',
    'user_dn_expression' => 'user_dn_expression',
    'user_attr'  => 'sAMAccountName',
    'mail_attr'  => 'mail',
    'ldapToDrupalUserPhp'  => NULL,
    'testingDrupalUsername'  => 'jdoe'
  );



$servers['ldapauthor1']['methodResponses']['connect'] = LDAP_SUCCESS;

$servers['ldapauthor1']['search_results']['(member="cn=verykool,ou=special guests,ou=guest accounts,dc=ad,dc=myuniveristy,dc=edu")']['ou=groups,dc=ad,dc=myuniveristy,dc=edu'] = array(
    0 => array('count' => 1, 'dn' => 'ou=content editors,ou=groups,dc=ad,dc=myuniveristy,dc=edu'),
    1 => array('count' => 1, 'dn' => 'ou=content approvers,ou=groups,dc=ad,dc=myuniveristy,dc=edu'),
    'count' => 2,
  );

$servers['ldapauthor1']['search_results']['(member="cn=jkool,ou=guest accounts,dc=ad,dc=myuniveristy,dc=edu")']['ou=groups,dc=ad,dc=myuniveristy,dc=edu'] = array(
    0 => array('count' => 1, 'dn' => 'ou=content editors,ou=groups,dc=ad,dc=myuniveristy,dc=edu'),
    'count' => 1,
  );

/**
 * fake user data array below 'attr' should mimick ldap user result data
 */
$servers['ldapauthor1']['users']['cn=jdoe,ou=campus accounts,dc=ad,dc=myuniveristy,dc=edu']['attr'] = array(
    'dn' => 'cn=jdoe,ou=campus accounts,dc=ad,dc=myuniveristy,dc=edu',
    'mail' => array( 0 => 'jdoe@myuniversity.edu', 'count' => 1),
    'sAMAccountName' => array( 0 => 'jdoe', 'count' => 1),
    'password' => array( 0 => 'goodpwd', 'count' => 1),
  );


$servers['ldapauthor1']['users']['cn=jkool,ou=guest accounts,dc=ad,dc=myuniveristy,dc=edu']['attr'] = array(
    'dn' => 'cn=jkool,ou=guest accounts,dc=ad,dc=myuniveristy,dc=edu',
    'mail' => array( 0 => 'jkool@guests.myuniversity.edu', 'count' => 1),
    'sAMAccountName' => array( 0 => 'jkool', 'count' => 1),
    'password' => array( 0 => 'goodpwd', 'count' => 1),
    'memberOf' => array( 0 => 'cn=sysadmins,ou=it,dc=ad,dc=myuniveristy,dc=edu', 'count' => 1),
  );

// duplicate of previous with escaped commas in cn.
$servers['ldapauthor1']['users']['cn=Flintstone\, Wilma,ou=guest accounts,dc=ad,dc=myuniveristy,dc=edu']['attr'] = array(
    'dn' => 'cn=Flintstone\, Wilma,ou=guest accounts,dc=ad,dc=myuniveristy,dc=edu',
    'mail' => array( 0 => 'wilmaf@guests.myuniversity.edu', 'count' => 1),
    'sAMAccountName' => array( 0 => 'wilmaf', 'count' => 1),
    'password' => array( 0 => 'goodpwd', 'count' => 1),
    'memberOf' => array(
       0 => 'cn=sysadmins,ou=it,dc=ad,dc=myuniveristy,dc=edu',
       1 => 'cn=punctuated\,comma\,freaks,ou=it,dc=ad,dc=myuniveristy,dc=edu',
      'count' => 2
      ),
  );

// duplicate of previous with quoted cn.
$servers['ldapauthor1']['users']['cn=Rubble\, Barney,ou=guest accounts,dc=ad,dc=myuniveristy,dc=edu']['attr'] = array(
    'dn' => 'cn=Rubble\, Barney,ou=guest accounts,dc=ad,dc=myuniveristy,dc=edu',
    'mail' => array( 0 => 'barneyr@guests.myuniversity.edu', 'count' => 1),
    'sAMAccountName' => array( 0 => 'barneyr', 'count' => 1),
    'password' => array( 0 => 'goodpwd', 'count' => 1),
    'memberOf' => array(
       0 => 'cn=sysadmins,ou=it,dc=ad,dc=myuniveristy,dc=edu',
       1 => 'cn="punctuated,comma,freaks",ou=it,dc=ad,dc=myuniveristy,dc=edu',
      'count' => 2
      ),
  );


$servers['ldapauthor1']['users']['cn=unkool,ou=lost,dc=ad,dc=myuniveristy,dc=edu']['attr'] = array(
    'dn' => 'cn=unkool,ou=lost,dc=ad,dc=myuniveristy,dc=edu',
    'mail' => array( 0 => 'unkool@nowhere.myuniversity.edu', 'count' => 1),
    'sAMAccountName' => array( 0 => 'jkool', 'count' => 1),
    'password' => array( 0 => 'goodpwd', 'count' => 1),
    'memberOf' => array( 0 => 'cn=unknown_people,ou=nowhere,dc=ad,dc=myuniveristy,dc=edu', 'count' => 1),
  );

$servers['ldapauthor1']['users']['cn=verykool,ou=special guests,ou=guest accounts,dc=ad,dc=myuniveristy,dc=edu']['attr'] = array(
    'dn' => 'cn=verykool,ou=special guests,ou=guest accounts,dc=ad,dc=myuniveristy,dc=edu',
    'mail' => array( 0 => 'verykool@myuniversity.edu', 'count' => 1),
    'sAMAccountName' => array( 0 => 'verykool', 'count' => 1),
    'password' => array( 0 => 'goodpwd', 'count' => 1),
    'meMBErof' => array(
      0 => 'cn=sysadmins,ou=it,dc=ad,dc=myuniveristy,dc=edu',
      1 => 'CN=NETadmins,ou=it,dc=ad,dc=myuniveristy,dc=edu',
      2 => 'cn=phone operators,ou=it,dc=ad,dc=myuniveristy,dc=edu',
      'count' => 3,
      ),
  );



$servers['ldapauthor1']['users']['cn=newkool,ou=special guests,ou=guest accounts,dc=ad,dc=myuniveristy,dc=edu']['attr'] = array(
    'dn' => 'cn=newkool,ou=guest accounts,dc=ad,dc=myuniveristy,dc=edu',
    'mail' => array( 0 => 'newkool@guests.myuniversity.edu', 'count' => 1),
    'sAMAccountName' => array( 0 => 'newkool', 'count' => 1),
    'password' => array( 0 => 'goodpwd', 'count' => 1),
    'memberOf' => array( 0 => 'cn=sysadmins,ou=it,dc=ad,dc=myuniveristy,dc=edu', 'count' => 1),
  );


/**
 * test users should include service account if one is being used
 */
$servers['ldapauthor1']['users']['cn=service-account,dc=ad,dc=myuniveristy,dc=edu']['attr'] = array(
    'dn' => 'cn=service-account,dc=ad,dc=myuniveristy,dc=edu',
    'mail' => array( 0 => 'service-account@myuniversity.edu', 'count' => 1),
    'sAMAccountName' => array( 0 => 'service-account', 'count' => 1),
    'memberOf' => array(
      0 => 'CN=service_accounts,OU=ServiceAccountGroups,DC=ad,DC=myuniveristy,DC=edu',
      'count' => 2,
      ),
    'password' => array( 0 => 'goodpwd', 'count' => 1),
  );
