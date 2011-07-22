<?php

/**
 * @file
 * Token module integration.
 */

/**
 * Implements hook_token_list().
 */
function _cas_ldap_token_list($type = 'all') {
  $tokens = array();

  if ($type == 'cas' || $type == 'all') {
    $tokens['cas']['cas-ldap-?'] = t('An LDAP attribute of the user. Replace ? with the attribute name in lower case. <a href="@url">Available tokens</a>.', array('@url' => url('admin/user/cas/attributes/ldap')));
    // $tokens['cas']['cas-ldap-?-raw'] = t('A CAS attribute of the user. Replace ? with the attribute name in lower case. <a href="@url">Available tokens</a>.', array('@url' => url('admin/user/cas/attributes/ldap')));
  }

  return $tokens;
}

/**
 * Implements hook_token_values.
 */
function _cas_ldap_token_values($type, $object = NULL) {
  $values = array();

  if ($type == 'cas' && !empty($object)) {
    $cas = $object;
    $attributes = cas_ldap_attributes($cas);
    foreach ($attributes as $key => $value) {
      if (is_array($value)) {
        $value = $value[0];
      }
      $values['cas-ldap-' . $key] = check_plain($value);
      // $values['cas-attribute-' . $key . '-raw'] = $value;
    }
    $keys = array_keys($attributes);
    $values['cas-ldap-?'] = t('Available attributes: %keys', array('%keys' => implode(', ', $keys)));
    // $values['cas-ldap-?-raw'] = $values['cas-attribute-?'];
  }

  return $values;
}
