<?php

/**
 * @file
 * Token module integration.
 */

/**
 * Implements hook_token_info_alter().
 */
function cas_attributes_token_info_alter(&$data) {
  if (function_exists('cas_phpcas_attributes')) {
    $data['tokens']['cas']['attribute'] = array(
      'name' => t('Attribute'),
      'description' => t('A CAS attribute of the user. (Only available if user is logged into CAS). <a href="@url">Available tokens</a>.', array('@url' => url('admin/config/people/cas/attributes/cas'))),
      'dynamic' => TRUE,
    );
  }
}

/**
 * Implements hook_tokens().
 */
function cas_attributes_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $sanitize = !empty($options['sanitize']);
  $replacements = array();

  if ($type == 'cas' && !empty($data['cas'])) {
    $cas = $data['cas'];

    // Provide [cas:attribute:?] dynamic tokens.
    if ($attribute_tokens = token_find_with_prefix($tokens, 'attribute')) {
      $attribute = array_change_key_case(cas_phpcas_attributes($cas));
      foreach ($attribute_tokens as $name => $original) {
        $name = drupal_strtolower($name);
        if (isset($attribute[$name])) {
          $value = $attribute[$name];
          if (is_array($value)) {
            $value = $value[0];
          }
          $replacements[$original] = $sanitize ? check_plain($value) : $value;
        }
        elseif ($name == '?') {
          $keys = array_keys($attribute);
          if ($sanitize) {
            $keys = array_map('check_plain', $keys);
          }
          $replacements[$original] = t('Available attributes: %keys', array('%keys' => implode(', ', $keys)));
        }
      }
    }
  }

  return $replacements;
}
