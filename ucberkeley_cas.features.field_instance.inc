<?php

/**
 * @file
 * ucberkeley_cas.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function ucberkeley_cas_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'user-user-field_display_name'.
  $field_instances['user-user-field_display_name'] = array(
    'bundle' => 'user',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Display names should be updated in the system of record (UC Path for faculty and staff; Student Information Systems for students). See our <a href="https://open.berkeley.edu/guides/site-builders-guide/manage-users">Manage Users documentation</a> for more information.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => -10,
      ),
      'featured' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'user',
    'field_name' => 'field_display_name',
    'label' => 'Display name',
    'required' => 0,
    'settings' => array(
      'linkit' => array(
        'button_text' => 'Search',
        'enable' => 0,
        'profile' => '',
      ),
      'text_processing' => 0,
      'user_register_form' => 0,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'maxlength_js' => 0,
        'maxlength_js_label' => 'Content limited to @limit characters, remaining: <strong>@remaining</strong>',
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 0,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Display name');
  t('Display names should be updated in the system of record (UC Path for faculty and staff; Student Information Systems for students). See our <a href="https://open.berkeley.edu/guides/site-builders-guide/manage-users">Manage Users documentation</a> for more information.');

  return $field_instances;
}
