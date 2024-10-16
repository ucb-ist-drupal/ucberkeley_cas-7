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
 * Implement key_provider_get_key_value.
 * Retrieve key value from Pantheon Secrets.
 *
 * @param $config
 * @return string
 */
function key_provider_pantheon_get_key_value($config) {
  $client = \PantheonSystems\CustomerSecrets\CustomerSecrets::create()->getClient();
  $secret = $client->getSecret($config['label']);
  $secret_value = $secret->getValue();

  return $secret_value;
}
