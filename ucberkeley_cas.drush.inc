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
    'options' => [
      'verbose' => 'Log messages regarding all users, not just those who had display name updates.'
    ],
    'examples' => [
      'Update all users in the site database' => 'drush ldap-update-usernames',
      'Update all users in the site database and use verbose logging.' => 'drush luu --verbose'
    ],
  ];

  return $commands;
}

function ucberkeley_cas_ldap_update_usernames () {

  // Only proceed if the display name update is enabled.
  if (!ucberkeley_cas_update_display_name_enabled()) {
    drupal_set_message(
      t('Display name update is disabled for this site. (See the variable ucberkeley_cas_update_display_name.)'),
      'warning',
    );
    return TRUE;
  }

  // Only proceed if cron is not running.
  if (ucberkeley_cas_cron_recent()) {
    return TRUE;
  }
  // Update existing queue items that are nearing expiration.
  ucberkeley_cas_queue_expire_update();

  // Add users to queue if it is empty.
  ucberkeley_cas_add_users_queue();

  // Get the queue object.
  $queue = DrupalQueue::get('display_name');

  // Create the ldap bind.
  ucberkeley_cas_ldap_bind();

  $verbose = drush_get_option('verbose');
  //Process the queue.
  while ($item = $queue->claimItem( 10)) {
    if (!ucberkeley_cas_update_display_name($item->data, $verbose)) {
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
