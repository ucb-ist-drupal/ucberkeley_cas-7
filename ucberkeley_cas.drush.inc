<?php

/**
 * Implements hook_drush_command().
 */
function ucberkeley_cas_drush_command() {
  $commands['ldap-update-usernames'] = [
    'callback' => 'ucberkeley_cas_ldap_update_usernames',
    'description' => 'Load all users into a the queue and check for display names that have been updated in LDAP.',
    'aliases' => [
      'update-usernames',
      'luu'
    ],
    'examples' => [
      'Update all users in the site database' => 'drush ldap-update-usernames'
    ],
  ];

  return $commands;
}

function ucberkeley_cas_ldap_update_usernames () {
  // Add users to queue if it is empty.
  ucberkeley_cas_add_users_queue();

  // Get the queue object.
  $queue = DrupalQueue::get('display_name');

  //Process the queue.
  while ($item = $queue->claimItem( 10)) {
    if (!ucberkeley_cas_update_display_name($item->data)) {
      drupal_set_message(
        t(
          'Failed to update uid @uid with name, @name.',
          [
            '@uid' => $item->cas_name,
            '@name' => $item->realname,
          ],
        ),
        'error',
      );
    }
    // Delete the item from the queue regardless of the success/failure of the update.
    $queue->deleteItem($item);
  }
}
