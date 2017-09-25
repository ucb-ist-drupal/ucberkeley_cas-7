<?php
/**
 * @file
 * ucberkeley_cas.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function ucberkeley_cas_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'administer realname'.
  $permissions['administer realname'] = array(
    'name' => 'administer realname',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'realname',
  );

  return $permissions;
}
