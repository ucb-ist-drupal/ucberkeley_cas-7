<?php

/**
 * @file
 * Provides settings pages for the CAS LDAP module.
 */

/**
 * Lists available CAS Attributes.
 */
function cas_ldap_list(&$form_state) {
  $form = array();

  $cas_attributes = variable_get('cas_attributes', array());
  if (empty($cas_attributes['ldap']['server'])) {
    // No CAS server configured.
    $form['info'] = array(
      '#value' =>t('An <a href="@url">LDAP server</a> must be selected.', array('@url' => url('admin/user/cas/attributes'))),
    );
    return $form;
  }
  $ldap_server = $cas_attributes['ldap']['server'];

  if (isset($form_state['values']['username'])) {
    $username = $form_state['values']['username'];
  }
  elseif (cas_current_user()) {
    $username = cas_current_user();
  }
  else {
    $username = $GLOBALS['user']->name;
  }

  $form['username'] = array(
    '#type' => 'textfield',
    '#title' => 'Username',
    '#default_value' => $username,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Query',
  );

  $header = array('Token', 'Value');
  $rows = array();

  $attributes = cas_ldap_attributes($username);
  if ($attributes) {
    foreach ($attributes as $attribute => $value) {
      if (is_numeric($attribute)) {
        continue;
      }
      if (is_array($value)) {
        $value = $value[0];
      }
      $rows[] = array(
        t('[cas-ldap-@attribute]', array('@attribute' => drupal_strtolower($attribute))),
        check_plain($value),
      );
    }
  }

  if (empty($rows)) {
    $rows[] = array(
      'data' => array(
        array(
          'data' => t('No LDAP attributes were returned by the LDAP server. Please ensure the <a href="@url">LDAP server</a> is configured correctly and a valid username is supplied.', array('@url' => url('admin/settings/ldap/ldapauth/edit/' . $ldap_server))),
          'colspan' => 2,
        ),
      ),
    );
  }

  $form['ldap_tokens'] = array(
    '#value' => theme('table', $header, $rows),
  );

  return $form;
}

function cas_ldap_list_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}
