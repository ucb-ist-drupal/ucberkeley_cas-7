<?php

/**
 * @file
 * Provides settings pages for the CAS Attributes module.
 */

/**
 * Administrative settings form.
 */
function cas_attributes_admin_settings() {
  // Get current settings, and add default options.
  $cas_attributes = variable_get('cas_attributes', array());
  $cas_attributes += array('sync_every_login' => NULL, 'relations' => array('mail' => NULL, 'name' => NULL));
  $relations = $cas_attributes['relations'];

  $form['cas_attributes'] = array(
    '#tree' => TRUE,
    '#type' => NULL,
  );

  $form['cas_attributes']['sync_every_login'] = array(
    '#type' => 'radios',
    '#title' => t('Fetch CAS Attributes'),
    '#default_value' => $cas_attributes['sync_every_login'],
    '#options' => array(
      0 => 'only when a CAS account is created (i.e., the first login of a CAS user).',
      1 => 'every time a CAS user logs in.'
    ),
    '#weight' => -10,
  );

  $form['cas_attributes']['relations'] = array(
    '#type' => 'fieldset',
    '#title' => t('CAS attribute mappings'),
  );

  $form['cas_attributes']['relations']['help'] = array(
    '#markup' => t('Token replacement strings used to populate each <a href="@url">user field</a>?  Only text fields are eligible to be populated.  Entries left blank will be ignored.', array('@url' => url('admin/config/people/accounts/fields'))),
  );

  // Provide fields for username and e-mail address.
  $form['cas_attributes']['relations']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' => isset($relations['name']) ? $relations['name'] : '',
    '#size' => 50,
    '#description' => t('The account username.'),
  );
  $form['cas_attributes']['relations']['mail'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail address'),
    '#default_value' => isset($relations['mail']) ? $relations['mail'] : '',
    '#size' => 50,
    '#description' => t('The account e-mail address.'),
  );

  // Provide fields for fields attached to the user bundle.
  foreach (field_info_instances('user', 'user') as $name => $instance) {
    $field = field_info_field($instance['field_name']);
    if ($field['type'] == 'text') {
      $form['cas_attributes']['relations'][$name] = array(
        '#type' => 'textfield',
        '#title' => t($instance['label']),
        '#default_value' => isset($relations[$name]) ? $relations[$name] : '',
        '#size' => 50,
        '#description' => t('The account field with name %field_name.', array('%field_name' => $instance['field_name'])),
      );
    }
  }

  if (module_exists('token')) {
    $form['token_tree'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('cas'),
      '#global_types' => FALSE,
    );
  }
  else {
    $form['token_tree'] = array(
      '#markup' => '<p>' . t('Enable the <a href="@drupal-token">Token module</a> to view the available token browser.', array('@drupal-token' => 'http://drupal.org/project/token')) . '</p>',
    );
  }

  return system_settings_form($form);
}

/**
 * Lists available CAS Attributes.
 */
function cas_attributes_list() {
  cas_phpcas_load();
  cas_phpcas_init();

  phpCAS::forceAuthentication();
  $attributes = phpCAS::getAttributes();

  $header = array('Token', 'Value');
  $rows = array();

  foreach ($attributes as $attribute => $value) {
    if (is_array($value)) {
      $value = $value[0];
    }
    $rows[] = array(
      t('[cas:attribute:@attribute]', array('@attribute' => drupal_strtolower($attribute))),
      check_plain($value),
    );
  }

  if (empty($rows)) {
    $rows[] = array(
      'data' => array(
        array(
          'data' => t('No CAS attributes were returned by the CAS server.'),
          'colspan' => 2,
        ),
      ),
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}

