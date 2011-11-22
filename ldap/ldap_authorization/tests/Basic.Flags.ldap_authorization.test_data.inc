<?php
// $Id$

/**
 * @file
 * ldap_authorization test data
 */

$authorization['drupal_role'] = array(

  'consumerModule' => 'ldap_authorization_drupal_role',

  'description' => 'UIUC AD',
  'status' => 1,
  'onlyApplyToLdapAuthenticated' => 1,

  'deriveFromDn' => 1,
  'deriveFromDnAttr' => 'ou',

  'deriveFromAttr' => 0,
  'deriveFromAttrAttr' => array(),

  'deriveFromEntry' => 0,
  'deriveFromEntryEntries' => NULL,
  'deriveFromEntryAttr' => NULL,

  'mappings' => array(
    array('Campus Accounts', 'campus accounts'),
    array('guest accounts', 'guests'),
    array('special guests', 'special guests'),
    array('cn=sysadmins,ou=it,dc=ad,dc=myuniveristy,dc=edu', 'sysadmins'),
  ),

  'useMappingsAsFilter' => 1,

  'synchOnLogon' => 1,
  'synchManually' => 1,

  'revokeLdapProvisioned' => 1,
  'createConsumers' => 1,
  'regrantLdapProvisioned' => 1,

);
