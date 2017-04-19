<?php
/**
 * @file
 * ucberkeley_cas.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function ucberkeley_cas_strongarm() {
  $export = array();

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
      'server' => 'ucb_prod',
    ),
  );
  $export['cas_attributes'] = $strongarm;

  if (array_key_exists('PANTHEON_ENVIRONMENT', $_ENV)) {
    $strongarm = new stdClass();
    $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
    $strongarm->api_version = 1;
    $strongarm->name = 'cas_cert';
    $strongarm->value = '/etc/ssl/certs/ca-bundle.crt';
    $export['cas_cert'] = $strongarm;
  }

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_port';
  $strongarm->value = '443';
  $export['cas_port'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_version';
  $strongarm->value = '3.0';
  $export['cas_version'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_mail_status_activated_notify';
  $strongarm->value = 0;
  $export['user_mail_status_activated_notify'] = $strongarm;

  return $export;
}
