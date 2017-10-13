#!/usr/bin/env drush
//<?php //this is here to make phpStorm use php syntax highlighting.

/**
 * @file
 * This drush script is for updating all user names with the values of their
 * LDAP name attributes.
 *
 * BACK UP YOUR DATABASE BEFORE USING THIS.
 *
 * NO GUARANTEE THAT THIS WILL WORK FOR YOU!
 *
 */

define('WATCHDOG_TYPE', __FILE__);

// Use the sandbox at your convenience to store the information needed
// to track progression between successive calls to the function.
if (!isset($sandbox['progress'])) {
  // The count of nodes visited so far.
  $sandbox['progress'] = 0;
  // Total nodes that must be visited.
  $sandbox['max'] = db_query('SELECT COUNT(uid) FROM {users}')->fetchField();
  // A place to store messages during the run.
  $sandbox['messages'] = array();
  // Last node read via the query.
  $sandbox['current_user'] = 0;
}
$limit = 3;

$sid = 'ucb_prod';
$ldap_server = new LdapServer($sid);
$attributes = array(
  'berkeleyEduFirstName',
  'berkeleyEduLastName',
  'givenname',
  'sn'
);

$query = db_select('users', 'u')
  ->fields('u', array('uid', 'name', 'init'))
  ->condition('u.uid', array(0, 1), 'NOT IN')
  ->orderBy('u.uid', 'ASC')
  ->condition('u.uid', $sandbox['current_user'], '>')
  ->range($sandbox['current_user'], $limit);

$result = $query->execute();

foreach ($result as $data) {
  //print_r($data);
  print $data->uid . "\n";
}

foreach ($users as $uid => $data) {

  // Skip anonymous user and user 1.
  if (in_array($uid, array(0, 1))) {
    unset($data);
    continue;
  }


  if (empty($data->cas_name)) {
    $msg = t("No CAS UID for @name. Skipping this user.", array('@name' => $data->name));
    watchdog(WATCHDOG_TYPE, $msg, array(), WATCHDOG_WARNING);
    drupal_set_message($msg, 'warning');
    unset($data);
    continue;
  }

  $filter = '(' . $ldap_server->user_attr . '=' . $data->cas_name . ')';

  foreach ($ldap_server->basedn as $basedn) {
    if (empty($basedn)) {
      continue;
    }
    $result = $ldap_server->search($basedn, $filter, $attributes);
    if (!$result || !isset($result['count']) || !$result['count']) {
      continue;
    }
    else {
      // Must find exactly one user.
      if ($result['count'] != 1) {
        $count = $result['count'];
        $msg = t("> 1 user found with @filter under @basedn.", array('@filter' => $filter, '@basedn' => $basedn));
        watchdog(WATCHDOG_TYPE, $msg, array(), WATCHDOG_WARNING);
        drupal_set_message($msg, 'warning');
        continue;
      }
      else {
        break;
      }
    }
  }

  $user_name = "";
  if ((!isset($result[0]['berkeleyedufirstname'][0]) || !isset($result[0]['berkeleyedulastname'][0]) ||
      empty($result[0]['berkeleyedufirstname'][0]) || !empty($result[0]['berkeleyedulastname'][0]))
    &&
    (!isset($result[0]['givenname'][0]) || !isset($result[0]['sn'][0]) ||
      empty($result[0]['givenname'][0]) || empty($result[0]['sn'][0]))
  ) {

    // Some (all?) of these people who are not found in LDAP are terminated
    // employees. Use the last name we had for them.
    // Remove ", BA" and ", PhD" etc from ends of names:
    $user_name = preg_replace("/,.*/", "", $data->name);
    // Admin could probably block/remove this account, so let them know.
    $msg = t("No LDAP names found for @cas_name, so using @name. (This person may no longer be employed by UC Berkeley.)", array('@cas_name' => $data->cas_name, '@name' =>  $user_name));
    watchdog(WATCHDOG_TYPE, $msg, array(), WATCHDOG_INFO);
    drupal_set_message($msg);
  }


  if (isset($result[0]['berkeleyedufirstname'][0]) && isset($result[0]['berkeleyedulastname'][0]) &&
    !empty($result[0]['berkeleyedufirstname'][0]) && !empty($result[0]['berkeleyedulastname'][0])
  ) {
    $user_name = $result[0]['berkeleyedufirstname'][0] . " " . $result[0]['berkeleyedulastname'][0];
  }
  elseif (isset($result[0]['givenname'][0]) && isset($result[0]['sn'][0]) &&
    !empty($result[0]['givenname'][0]) && !empty($result[0]['sn'][0])
  ) {
    $user_name = $result[0]['givenname'][0] . " " . $result[0]['sn'][0];
  }

  $wrapper = entity_metadata_wrapper('user', $data);
  $wrapper->field_display_name->set($user_name);
  $wrapper->name->set($data->cas_name);
  $wrapper->save();
  $msg = t("New name assigned for @cas_name: @display_name", array('@cas_name' => $data->cas_name, '@display_name' =>  $wrapper->field_display_name->value()));
  watchdog(WATCHDOG_TYPE, $msg, array(), WATCHDOG_INFO);
  drupal_set_message($msg);
  unset($data);
}


