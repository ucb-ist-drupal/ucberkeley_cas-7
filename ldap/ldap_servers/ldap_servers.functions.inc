<?php
// $Id: ldap_servers.functions.inc,v 1.3.2.1 2011/02/08 06:01:00 johnbarclay Exp $

/**
 * @file
 * collection of functions that don't belong in server object
 */



/**
 * returns new user account if created, otherwise integer error message such
 * as LDAP_CREATE_ACCOUNT_ALREADY_EXISTS, LDAP_CREATE_ERROR
 *
 */
function ldap_create_drupal_account($name, $mail, $dn, $sid, $edit = array()) {

  $edit['name'] = $name;
  $edit['pass'] = user_password(20);
  $edit['mail'] = $mail;
  $edit['init'] = $mail;
  $edit['status'] = 1;

  // save 'init' data to know the origin of the ldap authentication provisioned account
  $edit['data']['ldap_authentication']['init'] = array(
    'sid'  => $sid,
    'dn'   => $dn,
    'mail' => $mail,
  );

  if (!$account = user_save( NULL, $edit)) {
    drupal_set_message(t('User account creation failed because of system problems.'), 'error');
    return FALSE;
  }
  else {
    user_set_authmaps($account, array('authname_ldap_authentication' => $name));
  }
  return $account;
}


/**
  * Modify an LDAP Entry
  */
function ldap_user_modify($userdn, $attributes, $ldap_server) {
  $status = ldap_modify($ldap_server->connection, $userdn, $attributes);
  if (!$status) {
    watchdog(
      'ldap_servers',
      'Error: user_modify() failed to modify ldap entry w/ base DN "!dn" with values: !values',
      array('!dn' => $userdn, '!value' => var_export($attributes, TRUE)),
      WATCHDOG_ERROR
    );
  }

  return $status;
}

/**
 * Modify a password
 */
function ldap_password_modify($userdn, $new_password, $ldap_server) {

  $new_password = "\"" . $new_password . "\"";
  $len = drupal_strlen($new_password);
  $new_pass = NULL;
  for ($i = 0; $i < $len; $i++) {
    $new_pass .= "{$new_password{$i}}\000";
  }

  $status = ldap_mod_replace($ldap_server->connection, $userdn, array('unicodePwd' => $new_pass));
  if (!$status) {
    watchdog(
      'ldap_servers',
      'Error: password_modify() failed to modify ldap password w/ base DN "!dn"',
      array('!dn' => $userdn),
      WATCHDOG_ERROR
    );
  }

  return $status;
}

/**
 *
 *  this attempts to find bad dns, but should only be used as warningswe
 *  as the ldap spec allows for any old character to be escaped and ldap
 *  implementations may not follow the spec.
 *
 *  http://www.ietf.org/rfc/rfc2253.txt
 *
 */
function ldap_baddn($dn, $dn_name) {
  $result = array();
  $valid_attr_name = '[_a-zA-Z\d\s]';
  $valid_attr_values = '[_\-a-zA-Z\d\s]';
  $regex = '/^(' . $valid_attr_name . '*\=' . $valid_attr_values . '*[,]{1})*(' . $valid_attr_name . '*\=' . $valid_attr_values . '*){1}$/';
  $match = (preg_match($regex, $dn)) ? TRUE : FALSE;
  $result['boolean'] = $match;
  if (!$match) {
    $tokens = array('%dn' => htmlspecialchars($dn), '%dn_name' => $dn_name);
    $result['text'] = t('Possible invalid format for:', $tokens)
    . '<em>' . $tokens['%dn'] . '</em>.<br/>  '.
    t('The format may be correct for your ldap, but please double check.', $tokens);
  }
  return $result;
}

/**
 *
 *  this attempts to find bad dns, but should only be used as warningswe
 *  as the ldap spec allows for any old character to be escaped and ldap
 *  implementations may not follow the spec.
 *
 *  http://www.ietf.org/rfc/rfc2253.txt
 *
 */

function ldap_badattr($attr, $attr_name) {
  $result = array();
  $valid_attr_name = '[_a-zA-Z\d\s]';
  $regex = '/^(' . $valid_attr_name . '){1,}$/';
  $match = (preg_match($regex, $attr)) ? TRUE : FALSE;
  $result['boolean'] = $match;
  if (!$match) {
    $tokens = array('%attr' => htmlspecialchars($attr), '%attr_name' => $attr_name);
    $result['text'] = t('Possible invalid format for %attr_name:', $tokens) . ' <code><em>' . $tokens['%attr']
      . '</em></code><br/>' . t('The format may be correct for your ldap, but please double check.', $tokens);
  }
  return $result;
}

/**
 * @param array $ldap_entry
 * @param string $text
 * @return string text with tokens replaced
 */

function ldap_server_token_replace($ldap_entry, $text) {
  $desired_tokens = ldap_server_tokens_needed_for_template($text);
  $tokens = ldap_server_tokenize_entry($ldap_entry, $desired_tokens, LDAP_SERVERS_TOKEN_PRE, LDAP_SERVERS_TOKEN_POST);
  $result = str_replace(array_keys($tokens), array_values($tokens), $text);
  return $result;
}


/**
 * Turn an ldap entry into a token array suitable for the t() function
 * @param ldap entry array $ldap_entry
 * @param string prefix token prefix such as !,%,[
 * @param string suffix token suffix such as ]
 * @param $token_keys either an array of key names such as array('cn', 'dn') or string 'all' to return all tokens.
 * @return token array suitable for t() functions of with lowercase keys as exemplified below


an ldap entry such as:

   'dn' => 'cn=jdoe,ou=campus accounts,ou=toledo campus,dc=ad,dc=myuniveristy,dc=edu',
    'mail' => array( 0 => 'jdoe@myuniversity.edu', 'count' => 1),
    'sAMAccountName' => array( 0 => 'jdoe', 'count' => 1),

should return tokens such as:

    -- from dn attribute
    [cn] = jdoe
    [cn:0] = jdoe
    [cn:last] => jdoe
    [ou] = campus accounts
    [ou:0] = campus accounts
    [ou:1] = toledo campus
    [ou:last] = toledo campus
    [dc] = ad
    [dc:0] = ad
    [dc:1] = myuniveristy
    [dc:2] = edu
    [dc:last] = edu

    -- from other attributes
    [mail] = jdoe@myuniversity.edu
    [mail:0] = jdoe@myuniversity.edu
    [mail:last] = jdoe@myuniversity.edu
    [samaccountname] = jdoe
    [samaccountname:0] = jdoe
    [samaccountname:last] = jdoe

 */
function ldap_server_tokenize_entry($ldap_entry, $token_keys = 'all', $pre = LDAP_SERVERS_TOKEN_PRE, $post = LDAP_SERVERS_TOKEN_POST) {

  $tokens = array();

  // 1. tokenize dn
  $dn_parts = ldap_explode_dn($ldap_entry['dn'], 0);
  unset($dn_parts['count']);
  $parts_count = array();
  $parts_last_value = array();
  foreach($dn_parts as $pair) {
    list($attr_name, $attr_value) = explode('=', $pair);
    $attr_value = check_plain($attr_value);
    if (!isset($parts_count[$attr_name])) {
      $tokens[$pre .  strtolower($attr_name) . $post] = $attr_value;
      $parts_count[$attr_name] = 0;
    }
    $tokens[$pre .  strtolower($attr_name) . LDAP_SERVERS_TOKEN_DEL . (int)$parts_count[$attr_name] . $post] = $attr_value;

    $parts_last_value[$attr_name] = $attr_value;
    $parts_count[$attr_name]++;
  }
  foreach ($parts_count as $attr_name => $count) {
    $tokens[$pre . strtolower($attr_name) . LDAP_SERVERS_TOKEN_DEL . 'last' . $post] = $parts_last_value[$attr_name];
  }

  // tokenize other attributes
  if ($token_keys == 'all') {
    $token_keys = array_keys($ldap_entry);
    $token_keys = array_filter($token_keys, "is_string");
    foreach ($token_keys as $attr_name) {
      $attr_value = $ldap_entry[$attr_name];
      if (is_array($attr_value) && is_scalar($attr_value[0]) && $attr_value['count'] == 1) {
        $tokens[$pre . strtolower($attr_name) . $post] = check_plain($attr_value[0]);
        $tokens[$pre . strtolower($attr_name) . LDAP_SERVERS_TOKEN_DEL . '0' . $post] = check_plain($attr_value[0]);
        $tokens[$pre . strtolower($attr_name) . LDAP_SERVERS_TOKEN_DEL . 'last' . $post] = check_plain($attr_value[0]);
      }
      elseif (is_array($attr_value) && $attr_value['count'] > 1) {
        $tokens[$pre .  strtolower($attr_name) . LDAP_SERVERS_TOKEN_DEL . 'last'. $post] = check_plain($attr_value[$attr_value['count']-1]);
        for ($i=0; $i<$attr_value['count']; $i++) {
          $tokens[$pre .  strtolower($attr_name) . LDAP_SERVERS_TOKEN_DEL . $i . $post] = check_plain($attr_value[$i]);
        }
      }
      elseif(is_scalar($attr_value)) {
        $tokens[$pre .  strtolower($attr_name) . $post] = check_plain($attr_value);
      }
    }
  }
  else {
    foreach ($token_keys as $token_key) {
      $parts = explode(LDAP_SERVERS_TOKEN_DEL, $token_key);
      $last_key = $parts[count($parts) -1];
      if ($last_key == 'last') {
        $attr_name = join(LDAP_SERVERS_TOKEN_DEL, array_pop($parts));
        $count =  $ldap_entry[$attr_name]['count'];
        $value = $ldap_entry[$attr_name][$count-1];
      }
      elseif (is_numeric($last_key) || $last_key == '0') {
        $discard = array_pop($parts);
        $attr_name = join(LDAP_SERVERS_TOKEN_DEL, $parts);
        $value = $ldap_entry[$attr_name][(int)$last_key];
      }
      else {
        $value = $ldap_entry[$token_key][0];
      }
      $tokens[$pre .  strtolower($token_key) . $post] = check_plain($value);
    }
  }

  // include the dn.  it will not be handled correctly by previous loops
  $tokens[$pre .  'dn' . $post] = check_plain($ldap_entry['dn']);

  return $tokens;
}

/**
 * @param string $template in form [cn]@myuniversity.edu
 * @return array of all tokens in the template such as array('cn')
 */
function ldap_server_tokens_needed_for_template($template, $pre = LDAP_SERVERS_TOKEN_PRE, $post = LDAP_SERVERS_TOKEN_POST) {
  preg_match_all('/
    \[             # [ - pattern start
    ([^\[\]]*)  # match $type not containing whitespace : [ or ]
    \]             # ] - pattern end
    /x', $template, $matches);

  return @$matches[1];

}

