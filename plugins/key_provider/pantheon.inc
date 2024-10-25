<?php
/**
 * @file
 * Key provider plugin for ucberkeley_cas.
 */

$plugin = [
  'label' => t('Pantheon Secrets'),
  'description' => t('Use Pantheon Secrets Manager.'),
  'storage method' => 'file',
  'key value' => array(
    'accepted' => FALSE,
    'required' => FALSE,
  ),
  'get key value' => 'key_provider_pantheon_get_key_value',
];

function key_provider_pantheon_default_configuration() {
  return [];
}

/**
 * Implementation of key_provider_get_key_value.
 * Retrieve key value from Pantheon Secrets.
 *
 * @param $config
 * @return string
 */
function key_provider_pantheon_get_key_value($config) {
  // pantheon_get_secret() is defined in a php prepend file in the Pantheon environment.
  if (function_exists('pantheon_get_secret')) {
    // We are in a Pantheon environment.
    $secret_value = pantheon_get_secret($config['id']);
  } else {
    // We are not on Pantheon.  Assume a local dev environment.
    // In local developemnt this secrets file should exist.
    $json_file = DRUPAL_ROOT . DIRECTORY_SEPARATOR . 'private' . DIRECTORY_SEPARATOR . 'secrets.json';

    if (!file_exists($json_file)) {
      drupal_set_message(t("Unable to read secrets file at @path in local dev environment.
      To fix this problem:
      1) Ensure that your secret(s) are set with 'terminus secrets:site:list mysite'.
      2) Dump your secret(s) with 'terminus secret:site:local-generate'.
      3) You will need to edit the secret value into the resulting file (replace the 'null') and move the file to the correct path.",
        ['@path' => $json_file]),
        'error');

      return "";
    }

    $json_content = file_get_contents($json_file);
    $data = json_decode($json_content, true);

    if (json_last_error() !== JSON_ERROR_NONE) {
      drupal_set_message(t("JSON error reading @path: @json_error", ['@path' => $json_file, '@json_error' => json_last_error_msg()]), 'error');
    }

    $secret_value = $data['Secrets'][$config['id']]['Value'];
  }

  return $secret_value;
}
