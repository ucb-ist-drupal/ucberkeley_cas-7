<?php

/**
 * @file
 * Token module integration.
 */

/**
 * Implements hook_token_list().
 */
function _cas_attributes_token_list($type = 'all') {
  $tokens = array();

  if ($type == 'cas' || $type == 'all') {
    if (function_exists('cas_phpcas_attributes')) {
      $tokens['cas']['cas-attribute-?'] = t('A CAS attribute of the user. Replace ? with the attribute name in lower case. (Only available if user is logged into CAS). <a href="@url">Available tokens</a>.', array('@url' => url('admin/user/cas/attributes/cas')));
      // $tokens['cas']['cas-attribute-?-raw'] = t('A CAS attribute of the user. Replace ? with the attribute name in lower case. (Only available if user is logged into CAS). <a href="@url">Available tokens</a>.', array('@url' => url('admin/user/cas/attributes/cas')));
    }
  }

  return $tokens;
}

/**
 * Implements hook_token_values.
 */
function _cas_attributes_token_values($type, $object = NULL) {
  $values = array();

  if ($type == 'cas' && !empty($object)) {
    $cas = $object;
    $attributes = array_change_key_case(cas_phpcas_attributes($cas));
    foreach ($attributes as $key => $value) {
      if (is_array($value)) {
        $value = $value[0];
      }
      $values['cas-attribute-' . $key] = check_plain($value);
      // $values['cas-attribute-' . $key . '-raw'] = $value;
    }
    $keys = array_keys($attributes);
    $values['cas-attribute-?'] = t('Available attributes: %keys', array('%keys' => implode(', ', $keys)));
    // $values['cas-attribute-?-raw'] = $values['cas-attribute-?'];
  }

  return $values;
}
