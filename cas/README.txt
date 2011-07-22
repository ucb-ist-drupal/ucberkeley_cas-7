Requirements
============
PHP 5 with the following modules:
  curl, openssl, dom, zlib, and xml
phpCAS version 1.0.0 or later.

Installation
============

* Place the cas folder in your Drupal modules directory.

* Download phpCAS from http://www.ja-sig.org/wiki/display/CASC/phpCAS  You will
  need version 1.0.0 or later.

* There are several locations you can install the phpCAS library.

  1. Module directory installation. This means installing the library folder
     under the moduels directory, so that the file
     sites/<site>/modules/cas/CAS/CAS.php exists.

  2. System wide installation. See the phpCAS installation guide, currently at
     https://wiki.jasig.org/display/CASC/phpCAS+installation+guide

  3. Libraries API installation. Install and enable the Libraries API module,
     available at http://drupal.org/project/libraries. Then extract phpCAS so
     that sites/<site>/libraries/CAS/CAS.php exists.

* Go to Administer > Site Building > Modules and enable this module.

* Go to Administer > User Management > CAS Setings to configure the CAS module.
  Depending on where and how you installed the phpCAS library, you may need
  to configure the path to CAS.php. The current library version will be
  displayed if the library is found.

Upgrading from 6.x-2.x / Associating CAS usernames with Drupal users
====================================================================

The following options have been depreciated:
* "Is Drupal also the CAS user repository?"
* "If Drupal is not the user repository, should CAS hijack users with the same name?"

The CAS module uses a lookup table (cas_user) to associate CAS usernames with
their corresponding Drupal user ids. The depreciated options bypassed this
lookup table and let users log in if their CAS username matched their Drupal
name. The update.php script has automatically inserted entries into the lookup
table so that your users will continue to be able to log in as before.

You can see the results of the update script and manage CAS usernames on the
"Administer >> User Management >> Users" (admin/user/user) page. The CAS
usernames are shown in parentheses next to the Drupal username. The bulk
operations drop-down includes options for rapidly creating and removing CAS
usernames. The "Create CAS username" option will assign a CAS username to each
selected account that matches their Drupal name. The "Remove CAS usernames"
option will remove all CAS usernames from the selected accounts.

API Changes Since 6.x-2.x
=========================
The hooks hook_auth_name() and hook_auth_filter() were combined and renamed
to hook_cas_user_alter(). See cas.api.php.

Testing
=======
The CAS module comes with built-in test routines. To enable testing on a
development site, download and enable the 'SimpleTest' module
(http://drupal.org/project/simpletest). The CAS test routines require a
version of SimpleTest newer than 6.x-2.11 -- for example, 6.x-2.x-dev.
Then navigate to Admin > Site Building > Testing. The CAS test routines are
available under "Central Authentication Service".

Note, the CAS test routines will automatically download phpCAS from the JASIG
website, to ensure a version compatible with the test routines, and so that
the tests may run successfully on qa.drupal.org.
