ucberkeley_cas 7.x-3.0.0
========================
* OPENUCB-1540: Update included modules
  * cas updated 7.x-1.3 --> 7.x-1.5
    * Bugfixes
      * Fixes issues listed at https://www.drupal.org/project/cas/releases/7.x-1.4
      * Fixes issues listed at https://www.drupal.org/project/cas/releases/7.x-1.5
        * Makes it possible to login at /cas or /CAS or /cAs etc.
        * Fixes error 'Call to undefined function _drush_user_print_info()' when using `drush cas-user-create`
    * New features
      * Ability to add multiple CAS users at /admin/people/cas/create. Must specify CAS UIDs. 
      * Ability to add role to multiple users via `drush cas-user-add-role builder 212373,212374`. (Specify comma separated CAS UIDs.)
      * Config: admin/config/people/cas: Use "CAS Server 3.0 or higher."
      * Config: admin/config/people/cas: Redirect unauthorized user to CAS if they attempt to visit "node edit" or "node add" paths. 
  * cas_attributes updated 7.x-1.0-beta2 --> 7.x-1.0-rc3
    * Fixes issues listed at https://www.drupal.org/project/cas_attributes/releases/7.x-1.0-rc1
      * Note: https://www.drupal.org/node/1399304 only applies to `cas:attributes` tokens.  We are currently using `cas:ldap` tokens, and none of the tokens that we need are multivalued.
    * Fixes issues listed at https://www.drupal.org/project/cas_attributes/releases/7.x-1.0-rc2
    * Fixes issues listed at https://www.drupal.org/project/cas_attributes/releases/7.x-1.0-rc3
  * ldap updated 7.x-1.0-beta12 -> 7.x-2.0
    * Fixes issues listed at https://www.drupal.org/project/ldap/releases/7.x-2.0
    * Applied patch [fixing watchdog bug](https://www.drupal.org/node/2870792).
    * Applied patch [fixing update warning](https://www.drupal.org/node/2870798).
  
 New Features in the ucberkeley_cas configuration
   * Start TLS is enabled on the LDAP connection.
      
 Removed functionality:
   * Removed [Apps](https://www.drupal.org/project/apps) integration.
   
ucberkeley_cas 7.x-2.3.0
========================
* OPENUCB-1542: Update phpCAS to version 1.3.5. This is a security release for phpCAS, but the vulnerability is mitigated on UC Berkeley CAS servers.
* Begin using semantic version numbers.

ucberkeley_cas 7.x-2.2
======================
* OPENUCB-768: Move cas_attributes from defaultconfig to strongarm.  Add update
  hook to revert the feature.  This compliments the ucberkeley_envconf 2.2 which
  no does no management of cas_attributes. This change resovles a bug in version
  2.1 which yeilded blank cas_attributes for CAS administrators added during 
  'drush site-install'.


ucberkeley_cas 7.x-2.1
======================
* OPENUCB-768 remove ldap-test.berkeley.edu configuration.  Ldap-test is
  sporadically unavailable.  CalNet team approves us using production LDAP
  server in Dev and Test environments.

ucberkeley_cas 7.x-2.1-beta4
=============================
* OPENUCB-473
** Upgrade to phpCAS 1.3.3. https://www.mail-archive.com/cas-user@lists.jasig.org/msg17338.html
* OPENUCB-106
** Prevent activation emails sent when accounts are unblocked
* Improve rebuild.sh: Parse version from .info and create tarball
* Makefiles changed to use github.com/bwood. (Will change back to ucb-ist-drupal after open source petition.)


ucberkeley_cas 7.x-2.1-beta3
=============================
* OPENUCB-355
** Remove ucberkeley_cas.make to prevent openberkeley.make from building this project.

ucberkeley_cas 7.x-2.1-beta2
=============================
* OPENUCB-273
** Apply https://www.drupal.org/node/2057881 which prevents an Ajax error during the install_profile_modules task of openberkeley.profile
* OPENUCB-328
** Disable the "Gateway" option in CAS 1.3 (Redirection >  "Check with the CAS server to see if the user is already logged in?").
   Using this in combination with "Automatically create Drupal accounts" can result in random user accounts being created when a user
   with a valid CAS ticket visits a page on a site with Drupal's CAS module configured this way.
** Do not require "access content" permission to visit the administrator backdoor. Some sites may deny that permission to anonymous.
** Refactor build script and makefiles.

ucberkeley_cas 7.x-2.1-beta1
=============================
* OPENUCB-280
** Update to cas-7.x-1.3
** Improve help

ucberkeley_cas 7.x-2.0-alpha6
=============================
* OPENUCB-254
** Fix this php warning: Notice: Undefined index: messages in ucberkeley_cas_init() (line 19 of /Users/bwood/code/drupal/bwood/ucberkeley_cas-7/ucberkeley_cas.module).
** Improve make/rebuild.sh and makefiles

ucberkeley_cas 7.x-2.0-alpha5
=============================
* OPENUCB-254
** 5822ad8: Fix the overlay problem with drupal_set_message info created in hook_install.

ucberkeley_cas 7.x-2.0-alpha4
=============================
* OPENUCB-105
** Rewrite this module using Features and Default Config
** Re-namespace from ucb_ to ucberkeley_. (Less likely to result in conflicts.)
** Improve user messages. Improve Readme.

* OPENUCB-254
** Add an explicit dependency on features. Makes it clearer what the deps are for people
   enabling ucberkeley_cas from admin/modules UI.
** Require overlay.module disabled for install
** Prevent admins from trying to change passwords for CAS users on the user edit form.
** Prevent error when CAS-authenticated admin tries to change their password

ucb_cas 7.x-1.3-beta2
===================
* DUPCODE-47: clean up auto_assigned roles.  If you created a new role after installing ucb_cas
newly added users were automatically assigned the new role in some situations. Take a look at 
/admin/config/people/cas > User Accounts and ensure that the correct roles are selected there.

ucb_cas 7.x-1.3-beta1
===================
* Improve conflict checking during installation
* Backup (rename) sites ldap servers, if they exist, at installation

ucb_cas 7.x-1.2-beta2
===================
* DUPCODE-2: user/logout was not redirecting to caslogout. Fixed.

ucb_cas 7.x-1.2-beta1
===================
* update modules
** cas 7.x-x-1.2
** ldap 7.x-x-1.0-beta10
* update phpCAS 1.3.1

ucb_cas 7.x-1.1-beta1
===================
* CAS will play nicely with libraries.module
* phpCAS 1.3.0 update


ucb_cas 7.x-1-1.0-beta4
=====================
* Apps configure form

ucb_cas 7.x-1-1.0-beta3
=====================

* Apps compatibility
* Friendly messages with links added to hook_install

ucb_cas 7.x-1-1.0-beta2
=====================
DUPCODE-2: logout defaults to /caslogout
DUPCODE-8: UCB CAS admin page: Reommendations about login blocks
DUPCODE-1: Admin back door to prevent user1 lockouts
DUPCODE-7: remove https://net-auth.berkeley.edu/cgi-bin/krbcpw as logout location.

* Remove $sub_modules variable_get/set and use a PHP constant.
  Required for Pantheon install profile compatibility.

ucb_cas 7.x-1-1.0-beta1
=====================
