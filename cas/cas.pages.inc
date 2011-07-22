<?php

/**
 * @file
 * User page callbacks for the cas module.
 */

/**
 * Menu callback; Manage CAS identities for the specified user.
 */
function cas_user_identities($account) {
  drupal_set_title(check_plain($account->name));

  $header = array(t('CAS username'), t('Operations'));
  $rows = array();
  foreach ($account->cas_names as $aid => $cas_name) {
    $rows[] = array(check_plain($cas_name), l(t('Delete'), 'user/' . $account->uid . '/cas/delete/' . $aid));
  }

  $output = theme('table', $header, $rows);
  $output .= drupal_get_form('cas_user_add', $account);

  return $output;
}

/**
 * Form builder; Add a CAS identity.
 *
 * @ingroup forms
 * @see cas_user_add_validate()
 */
function cas_user_add($form_state, $account) {
  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $account->uid,
  );
  $form['cas_name'] = array(
    '#type' => 'textfield',
    '#title' => t('CAS username'),
  );
  $form['submit'] = array('#type' => 'submit', '#value' => t('Add a CAS username'));
  return $form;
}

function cas_user_add_validate($form, &$form_state) {
  // Check for existing entries.
  if (db_result(db_query("SELECT cas_name FROM {cas_user} WHERE cas_name = '%s'", $form_state['values']['cas_name']))) {
    form_set_error('cas_name', t('The CAS username is already in use on this site.'));
  }
}

function cas_user_add_submit($form, &$form_state) {
  db_query("INSERT INTO {cas_user} (uid, cas_name) VALUES (%d, '%s')", $form_state['values']['uid'], $form_state['values']['cas_name']);
  drupal_set_message(t('CAS username %cas_name added.', array('%cas_name' => $form_state['values']['cas_name'])));
}

/**
 * Menu callback; Delete the specified CAS username from the system.
 */
function cas_user_delete_form($form_state, $account, $aid = 0) {
  $form = array();
  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $account->uid,
  );
  $form['aid'] = array(
    '#type' => 'value',
    '#value' => $aid,
  );
  return confirm_form($form, t('Are you sure you want to delete the CAS username %cas_name for %user?', array('%cas_name' => $account->cas_names[$aid], '%user' => $account->name)), 'user/' . $account->uid . '/cas');
}

function cas_user_delete_form_submit($form, &$form_state) {
  $query = db_query("DELETE FROM {cas_user} WHERE uid = %d AND aid = %d", $form_state['values']['uid'], $form_state['values']['aid']);
  if ($query) {
    drupal_set_message(t('CAS username deleted.'));
  }
  $form_state['redirect'] = 'user/' . $form_state['values']['uid'] . '/cas';
}
