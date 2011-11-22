<?php
// $Id: ldap_servers.inc,v 1.4.2.1 2011/02/08 06:01:00 johnbarclay Exp $

/**
 * @file
 * collection of functions that don't belong in server object
 */

function _ldap_servers_get_user_ldap_data($drupal_user, $sid) {
  $drupal_username =  (is_object($drupal_user)) ? $drupal_user->name : $drupal_user;
  $servers = ldap_servers_get_servers($sid, 'enabled');
  foreach ($servers as $sid => $ldap_server)  {
    if ($ldap_user = $ldap_server->user_lookup($drupal_username)) {
      return $ldap_user;
    }
    else {
      return FALSE;
    }
  }
}

/**
 * see ldap_servers_get_servers()
 */
function _ldap_servers_get_servers($sid, $type, $flatten, $reset) {
  require_once('LdapServerAdmin.class.php');
  static $servers;

  if ($reset) {
    $servers = array();
  }
  if (!isset($servers['all'])) {
    $servers['all'] = LdapServerAdmin::getLdapServerObjects('all');
  }
  if (!isset($servers['enabled'])) {
    $servers['enabled'] = array();
    foreach ($servers['all'] as $_sid => $ldap_server) {
      if ($ldap_server->status == 1) {
        $servers['enabled'][$_sid] = $ldap_server;
      }
    }
  }

  if ($sid) {
    if (!isset($servers[$type][$sid])) {
      return FALSE;
    }
    return ($flatten) ? $servers[$type][$sid] : array($sid => $servers[$type][$sid]);
  }

  if (isset($servers[$type])) {
    return $servers[$type];
  }
}

/**
 * see ldap_servers_get_servers()
 */
function _ldap_servers_get_simpletest_servers($sid, $type, $flatten, $reset = TRUE) {
  require_once('tests/LdapServerTest.class.php');

  $servers['all'] = LdapServerTest::getLdapServerObjects('all');

  foreach ($servers['all'] as $_sid => $ldap_server) {
    if ($ldap_server->status == 1) {
      $servers['enabled'][$_sid] = $ldap_server;
    }
  }


  if ($sid) {
    if (!isset($servers[$type][$sid])) {
      return FALSE;
    }
    return ($flatten) ? $servers[$type][$sid] : array($sid => $servers[$type][$sid]);

  }

  if (isset($servers[$type])) {
    return $servers[$type];
  }
}


function _ldap_servers_fields() {

$fields = array(

  'sid' => array(
    'schema' => array(
      'type' => 'varchar',
      'length' => 20,
      'not null' => TRUE,
    )
  ),

  'name' =>  array(
    'schema' => array(
      'type' => 'varchar',
      'length' => 255,
      'not null' => TRUE,
    )
  ),

  'status' => array(
    'schema' => array(
      'type' => 'int',
      'size' => 'tiny',
      'not null' => TRUE,
      'default' => 1,
    )
  ),

  'type' =>  array(
    'schema' => array(
      'type' => 'varchar',
      'length' => 20,
      'not null' => FALSE,
    )
  ),

  'address' => array(
  'schema' => array(
  'type' => 'varchar',
  'length' => 255,
  'not null' => TRUE,
  )
  ),

  'port' => array(
  'schema' => array(
  'type' => 'int',
  'not null' => TRUE,
  'default' => 389,
  )
  ),

  'tls' => array(
  'schema' => array(
  'type' => 'int',
  'size' => 'tiny',
  'not null' => TRUE,
  'default' => 0,
  )
  ),

  'basedn' => array(
  'schema' => array(
  'type' => 'text',
  )
  ),

  'binddn' => array(
    'schema' => array(
  'type' => 'varchar',
  'length' => 255,
  )
  ),

  'bindpw' => array(
  'schema' => array(
  'type' => 'varchar',
  'length' => 255,
  )
  ),


  'user_attr' =>  array(
    'schema' => array(
      'type' => 'varchar',
      'length' => 255,
      'not null' => TRUE,
    )
  ),

  'user_attr' =>  array(
    'schema' => array(
      'type' => 'varchar',
      'length' => 255,
      'not null' => FALSE,
    )
  ),


  'mail_attr' =>  array(
    'schema' => array(
      'type' => 'varchar',
      'length' => 255,
      'not null' => FALSE,
    )
  ),


  'ldap_to_drupal_user' =>  array(
    'schema' => array(
      'type' => 'varchar',
      'length' => 255,
      'not null' => FALSE,
    )
  ),

 'testing_drupal_username' =>  array(
    'schema' => array(
      'type' => 'varchar',
      'length' => 255,
      'not null' => FALSE,
    )
  ),

  'weight' =>  array(
  'schema' => array(
  'type' => 'int',
  'not null' => TRUE,
  'default' => 0,
  ),
  )

  );


  return $fields;
}
