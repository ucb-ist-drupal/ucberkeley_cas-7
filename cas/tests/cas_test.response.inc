<?php

/**
 * @file
 * Response callbacks for the cas_test module.
 */

/**
 * Returns a CAS 2.0 service response for a validation success.
 *
 * @param $name
 *   CAS username.
 * @param $attributes
 *   If specified, the CAS attributes for the user.
 */
function theme_cas_service_validate_success($name, $attributes = array(), $style = 'jasig') {
  $output .= "<cas:serviceResponse xmlns:cas='http://www.yale.edu/tp/cas'>\n";
  $output .= "<cas:authenticationSuccess>\n";
  $output .= "<cas:user>$name</cas:user>\n";
  $output .= theme('cas_service_validate_attributes', $attributes, $style);
  $output .= "</cas:authenticationSuccess>\n";
  $output .= "</cas:serviceResponse>\n";

  return $output;
}

/**
 * Returns CAS attributes as part of a CAS 2.0 service response.
 *
 * @param $attributes
 *  The CAS attributes for the user
 * @param $style
 *   Must be one of:
 *     - 'jasig' (default)
 *     - 'rubycas'
 *     - 'name-value'
 */
function theme_cas_service_validate_attributes($attributes = array(), $style = 'jasig') {
  $output = '';
  switch($style) {
    case 'jasig':
    default:
      $output .= "<cas:attributes>\n";
      $output .= "<cas:attraStyle>Jasig</cas:attraStyle>\n";
      foreach ($attributes as $name => $values) {
        foreach ((array) $values as $value) {
          $output .= strtr("<cas:!name>!value</cas:!name>\n", array('!name' => $name, '!value' => $value));
        }
      }
      $output .= "</cas:attributes>\n";
    case 'rubycas':
      $output .= "<cas:attraStyle>RubyCAS</cas:attraStyle>\n";
      foreach ($attributes as $name => $values) {
        foreach ((array) $values as $value) {
          $output .= strtr("<cas:!name>!value</cas:!name>\n", array('!name' => $name, '!value' => $value));
        }
      }
    case 'name-value':
      $output .= "<cas:attribute name='attraStyle' value='Name-Value' />\n";
      foreach ($attributes as $name => $values) {
        foreach ((array) $values as $value) {
          $output .= strtr("<cas:attribute name='!name' value='!value' />\n", array('!name' => $name, '!value' => $value));
        }
      }
  }

  return $output;
}


/**
 * Returns a CAS 2.0 service response for a validation failure.
 *
 * @param $ticket
 * @param $error_code
 */
function theme_cas_service_validate_failure($ticket, $error_code) {
  $output = "<cas:serviceReponse xmlns:cas='http://www.yale.edu/tp/cas'>\n";
  $output .= "<cas:authenticationFailure code=\"$error_code\">\n";
  $output .= "Ticket $ticket not recognized.\n";
  $output .= "</cas:authenticationFailure>\n";
  $output .= "</cas:serviceResponse>\n";

  return $output;
}
