<?php

/**
 * @file
 * Provides CAS user registration administrative pages.
 */

/**
 * Creates a CAS user registration page.
 */
function cas_add_user_form() {
  $form = array();

  $form['account']['cas_name'] = array(
    '#type' => 'textfield',
    '#title' => t('CAS username'),
    '#required' => TRUE,
    '#default_value' => '',
    '#description' => t('Registration will proceded as if the user with the specified CAS username just logged in.'),
    '#element_validate' => array('_cas_user_register_cas_name_element_validate'),
    '#weight' => -10,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create new account'),
  );
  return $form;
}

function cas_add_user_form_submit($form, &$form_state) {
  $options = array(
    'invoke_cas_user_presave' => TRUE,
  );
  $account = cas_user_register($form_state['values']['cas_name'], $options);

  // Terminate if an error occurred while registering the user.
  if (!$account) {
    drupal_set_message(t("Error saving user account."), 'error');
    $form_state['redirect'] = '';
    return;
  }

  // Set these in case another module needs the values.
  $form_state['user'] = $account;
  $form_state['values']['uid'] = $account->uid;

  drupal_set_message(t('Created a new user account for <a href="@url">%name</a>. No e-mail has been sent.', array('@url' => url("user/$account->uid"), '%name' => $account->name)));
}
