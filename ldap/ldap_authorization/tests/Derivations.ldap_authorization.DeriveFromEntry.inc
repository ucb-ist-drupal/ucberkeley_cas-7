<?php
// $Id$

/**
 * @file
 * simpletest authorization configuration
 */

$authorization['drupal_role'] = array(
  'consumerModule' => 'ldap_authorization_drupal_role',

  'description' => 'UIUC AD',
  'status' => 1,
  'onlyApplyToLdapAuthenticated' => 1,

  'deriveFromDn' => 0,
  'deriveFromDnAttr' => NULL,

  'deriveFromAttr' => 0,
  'deriveFromAttrAttr' => array(),

  'deriveFromEntry' => 1,
  'deriveFromEntryEntries' => array('ou=groups,dc=ad,dc=myuniveristy,dc=edu'),
  'deriveFromEntryAttr' => 'member',

  'mappings' => array(
    0 => array('ou=content editors,ou=groups,dc=ad,dc=myuniveristy,dc=edu', 'content editors'),
    1 => array('ou=content approvers,ou=groups,dc=ad,dc=myuniveristy,dc=edu', 'content approvers'),
  ),

  'useMappingsAsFilter' => 1,

  'synchOnLogon' => 1,
  'synchManually' => 1,

  'revokeLdapProvisioned' => 1,
  'createConsumers' => 1,
  'regrantLdapProvisioned' => 1,
);
