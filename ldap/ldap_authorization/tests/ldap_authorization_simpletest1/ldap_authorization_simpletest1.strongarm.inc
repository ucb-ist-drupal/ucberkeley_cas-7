<?php
// $Id$
/**
 * @file
 * ldap_authorization_simplettest.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function ldap_authorization_simpletest1_strongarm() {
  $export = array();

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ldap_authentication_conf';
  $strongarm->value = array(
    'sids' => array(
      'ldapauthor1' => 'ldapauthor1',
    ),
    'authenticationMode' => LDAP_AUTHENTICATION_EXCLUSIVE,
    'loginConflictResolve' => LDAP_AUTHENTICATION_CONFLICT_LOG,
    'acctCreation' => LDAP_AUTHENTICATION_ACCT_CREATION_LDAP_BEHAVIOR,
    'ldapUserHelpLinkUrl' => 'https://passwords.cites.uiuc.edu/',
    'ldapUserHelpLinkText' => 'Illinois Passwords',
    'emailOption' => LDAP_AUTHENTICATION_EMAIL_FIELD_DISABLE,
    'emailUpdate' => LDAP_AUTHENTICATION_EMAIL_UPDATE_ON_LDAP_CHANGE_DISABLE,
    'allowOnlyIfTextInDn' => NULL,
    'excludeIfTextInDn' => NULL,
    'allowTestPhp' => NULL,
    'excludeIfNoAuthorizations' => NULL,
  );

  $export['ldap_authentication_conf'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ldap_servers_encrypt_key';
  $strongarm->value = 'mfSZv7pOKi';
  $export['ldap_servers_encrypt_key'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ldap_servers_encryption';
  $strongarm->value = '6';
  $export['ldap_servers_encryption'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ldap_servers_require_ssl_for_credentails';
  $strongarm->value = 0;
  $export['ldap_servers_require_ssl_for_credentails'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_email_verification';
  $strongarm->value = 1;
  $export['user_email_verification'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_register';
  $strongarm->value = USER_REGISTER_VISITORS_ADMINISTRATIVE_APPROVAL;
  $export['user_register'] = $strongarm;

  return $export;
}
