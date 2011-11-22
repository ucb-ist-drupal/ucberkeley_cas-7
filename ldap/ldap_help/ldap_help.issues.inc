<?php
// $Id: ldap_help.module,v 1.1.2.1 2011/02/08 06:14:20 johnbarclay Exp $

/**
 * @file
 * The ldap_help issues provides a filtered watchdog view for ldap issues.
 *
 */

function ldap_help_issues() {

  $text = '<h3>' . t('LDAP Help Module') . '</h3><p>' .
  t('How to report bugs in LDAP Project.') . '</p>';
  require_once('ldap_help.status.inc');
  $ldap_config = ldap_help_status('html');

  $path = drupal_get_path("module", "ldap_servers");

  $text .= <<<EOT

  <ol>
  <li>Search for existing issues. Be sure to include all statuses and versions
      in your searches. An issue may be closed but still be relevant.
    <ul>
      <li>Use the advanced search (<a href="http://drupal.org/project/issues/search/ldap">http://drupal.org/project/issues/search/ldap</a>) in the issue queue, searching on the test id in <em>issue tags</em>
        (e.g. LDAP_authen.user_profile_form.ldap.password) or
        a shortened version of it (e.g. LDAP_authen.user_profile_form.ldap).
      </li>
      <li>Do other searches in issue queue as many issues may not be tagged by test id.</li>

  </ul>
</li>


<li>Look for common problems/solutions at: <a href="http://drupal.org/node/997082">http://drupal.org/node/997082</a>.
This is editable, so actively participate in updateing this documentation.

</li>
<li>
Additional directions are at the beginning of the add issue form at
<a href="http://drupal.org/node/add/project-issue/ldap">http://drupal.org/node/add/project-issue/ldap.</a>

</li>

</ul>

</li>


</ol>

<div style="border: 1px solid #000000; padding: 10px; margin: 10px; text-align: center;">
  <p>Cut and past the html below into a file called<code>ldap_config.html</code> and attach it when reporting errors.
  <br/>The table below is the same.  Check that no sensitive data is in it.</p>
  <form>
  <textarea rows=10 cols=120 border=1 style="border: 1px solid #000000">$ldap_config</textarea>
  </form>
</div>
$ldap_config
EOT;


  return $text;
}
