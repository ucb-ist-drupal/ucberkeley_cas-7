<?php
// $Id$

/**
 * @file
 * simpletest authorization configuration
 */

$authorization['drupal_role'] = array(

  'consumerType' => 'drupal_role',  // change as desired
  'consumerModule' => 'ldap_authorization_drupal_role',

  'description' => 'UIUC AD',
  'status' =>  1,
  'onlyApplyToLdapAuthenticated' => 1,

  'deriveFromDn' => 0,
  'deriveFromDnAttr' => NULL,

  'deriveFromAttr' => 1,
  'deriveFromAttrAttr' => array('memberOf'),
  'deriveFromAttrUseFirstAttr' => 0,

  'deriveFromEntry' => 0,
  'deriveFromEntryEntries' => NULL,
  'deriveFromEntryAttr' => NULL,

  'mappings' => array(
    array('cn=SYSadmins,ou=it,dc=ad,dc=myuniveristy,dc=edu', 'sysadmins'),
    array('cn=netadmins,ou=it,dc=ad,dc=myuniveristy,dc=edu', 'netadmins'),
    array('cn=phone operators,ou=it,dc=ad,dc=myuniveristy,dc=edu', 'netadmins'),
    array('netadmins', 'netadmins2'),
    array('punctuated,comma,freaks', 'comma freaks'),
  ),

  'useMappingsAsFilter' => 1,

  'synchOnLogon' => 1,
  'synchManually' => 1,

  'revokeLdapProvisioned' => 1,
  'createConsumers' => 1,
  'regrantLdapProvisioned' => 1,
);
