<?php

/**
 * @file
 * ucberkeley_cas.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function ucberkeley_cas_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_display_name'.
  $field_bases['field_display_name'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_display_name',
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'linkchecker_exclude' => 0,
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 255,
    ),
    'translatable' => 0,
    'type' => 'text',
  );

  return $field_bases;
}
