<?php

/**
 * @file
 * Token module integration.
 */

/**
 * Implements hook_token_list().
 */
function _cas_token_list($type = 'all') {
  $tokens = array();

  if ($type == 'cas' || $type == 'all') {
    $tokens['cas']['cas-name'] = t('The CAS username.');
    // $tokens['cas']['cas-name-raw'] = t('The CAS username.');
  }

  return $tokens;
}

/**
 * Implements hook_token_values().
 */
function _cas_token_values($type, $object = NULL) {
  $values = array();

  if ($type == 'cas' && !empty($object)) {
    $cas = $object;
    $values['cas-name'] = check_plain($cas);
    // $values['cas-name-raw'] = $cas;
  }

  return $values;
}
