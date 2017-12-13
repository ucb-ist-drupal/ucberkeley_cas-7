# UC Berkeley CAS 7.x
## Introduction

UC Berkeley CAS is a Drupal
"[feature](https://www.drupal.org/docs/7/modules/features/bundling-site-settings-using-features)" (a collection of
modules and configuration) which adds UC Berkeley CalNet
(CAS) authentication to a Drupal site. Once UC Berkeley CAS is enabled
logging into your site via CalNet will be possible by visiting http://EXAMPLE.berkeley.edu/cas.

UC Berkeley CAS was developed to provide CAS authentication to sites using 
the [Open Berkeley](https://open.berkeley.edu) Drupal [distribution](https://www.drupal.org/docs/7/distributions). Since this 
feature was developed to work on any Drupal site, Web Platform Solutions was granted 
permission to make it publically available to the UC Berkeley Drupal community.

!!! warning

    **No Warranty**: This software is provided "as is." UC Berkeley IST does not provide support for this code when it is used outside of the [Open Berkeley](https://open.berkeley.edu) product. Anyone using this code does so at their own risk and would do well to monitor 
    releases to the Drupal modules, phpCAS and other 3rd-party libraries themselves. (That said, 
    [see this information about upgrading parts of this feature](faq/#i-notice-that-there-are-upgrades-available-for-some-of-these-modules-is-it-safe-to-upgrade-them).)

UC Berkeley CAS applies a default configuration to the modules it
installs. This configuration assumes that everyone, including the site
administrators, will login to the site using CalNet.  See [Standard
Configuration](#standard-configuration).

## Security Considerations 

By default ucberkeley_cas is configured so that anyone logging into
your site **must** use UCB CalNet authentication. If you visit
http://EXAMPLE.berkeley.edu/user/login (the standard Drupal login URL) after installing UC Berkeley
CAS, you will encounter a CAS login form instead of the familiar Drupal
login form. The reason for this is that Drupal's standard
authentication is insecure -- unless used in conjunction with the https protocol. (Drupal standard authentication is vulnerable to (1)
username/password interception (especially if a wireless network is in
use) and (2) session hijacking.) A second reason for requiring CalNet authentication is to prevent the complexity that results from allowing multiple authentication channels (i.e. CalNet _and_ Drupal standard authentication.)

!!! note

    CalNet/CAS authentication is not immune to attack on a site using
    the http protocol. A CAS ticket could theoretically be hijacked, however
    exploiting an stolen CAS ticket is significantly more difficult
    than exploiting Drupal's standard authentication token. **The most
    secure configuration for your site would be to use https in
    addition to UC Berkeley CAS.**

Additionally, UC Berkeley CAS does not provide a login link on any of your pages.
You may provide this yourself if you choose. A common practice is to simply instruct
site builders to visit the path `/cas` when they want to login.  When a login link
appears on site pages, it will be discovered by nefarious bots attempting brute force 
login attacks. It also invites users to attempt to login when they really don't need to authenticate.

## Quick Start 

1. See [Requirements](#requirements)
2. Install and enable [the most recent release of ucberkeley_cas](https://github.com/bwood/ucberkeley_cas-7/releases). (More info: [Installing](#installation))
3. Using an incognito window (or a second browser) in which you are not logged to the site as User 1, visit the login url
http://example.com/cas and login with your CalNet id.
4. In your main window, where you are logged in as User 1, edit the new user that got
created in the last step and assign it the "administrator" role (or whichever role is appropriate). (More info:
[Setup a CalNet-authenticated administrator](#setup-a-calnet-authenticated-administrator))
5. Visit the path /admin/config/people/ucbcas on your site for recommendations on further configuration.
6. Subscribe to [ucberkeley-cas-drupal-users@lists.berkeley.edu](https://calmail.berkeley.edu/manage/list/listinfo/ucberkeley-cas-drupal-users@lists.berkeley.edu) for information on releases and security advisories.

## Requirements 

You need the following modules installed:

* [Features](http://drupal.org/project/features)
* [Strongarm](http://drupal.org/project/strongarm)
* [Token](http://drupal.org/project/token)
* [CTools](http://drupal.org/project/ctools)
* [Default Config](http://drupal.org/project/defaultconfig)
* [Entity](https://www.drupal.org/project/entity)

UC Berkeley CAS will install these modules:

* [CAS](http://drupal.org/project/cas)
* [CAS Attributes](http://drupal.org/project/cas_attributes)
* [LDAP](http://drupal.org/project/ldap)

Specific versions of the above modules are specified in ucberkeley_cas-7.x.make. If your site is using unsupported versions of these modules, you'll be notified of the problem when you attemp to enable UC Berkeley CAS.

## CAS Registration 

In order to use CalNet (CAS) authentication, your website must be registered with 
CalNet. [Register your site here](https://calnetweb.berkeley.edu/calnet-technologists/cas/casifying-your-web-application-or-web-server/cas-registration).
  Make sure your registration is approved before you install UC Berkeley CAS 
on a production site.

### Localhost Sites Do Not Require Registration 

Developers working local webserver should use one of these domains:

* `http://localhost`
* `http://local`
* `http://127.0.0.1`.

These urls will work with the CAS server auth-test.berkeley.edu and don't 
require CAS registration. Other common development URLs (e.g. http://*.dev, and 
Acquia Dev Desktop's `http://example.dd`) will not work the the auth-test CAS 
server.

## Installation 

1. Make sure your site meets the requirements above.
2. Download the tarball included with [the latest release of UC Berkeley CAS](https://github.com/bwood/ucberkeley_cas-7/releases),
(e.g. ucberkeley_cas-7.x-x.x.tar.gz) to the host serving your Drupal site.
3. Unarchive the module in sites/all/modules
4. Enable the module at admin/modules.
5. Make sure your [CAS registration|https://calnetweb.berkeley.edu/calnet-technologists/cas/casifying-your-web-application-or-web-server/cas-registration] 
has been processed.
6. Test your site:

If your site runs at http://EXAMPLE.berkeley.edu, go to
http://EXAMPLE.berkeley.edu/cas.  You should see the CAS login
page.  When you authenticate successfully you should be returned to
your Drupal site and you should see "Logged in as \[your name\]." 

Go to http://EXAMPLE.berkeley.edu/user.  You should see the email
address and full name that was retrieved from LDAP for your account.

## Setup a CalNet-authenticated Administrator 


User 1 is the "superuser" on a Drupal site. (This account is often
named "admin.") If you are installing UC Berkeley CAS, you are
probably logged in as User 1. Once UC Berkeley CAS is enabled on
your site, the best practice is to stop logging in as this user and to
login using your CalNet credentials instead. Here's how to set that
up:

1. Log into your site as User 1 and enable the UC Berkeley CAS module.
2. Using an incognito window, visit your CAS url which will be something like http://EXAMPLE.berkeley.edu/cas. When you successfully authenticate using CAS a new Drupal user will be created that is associated with your CalNet credentials.
3. Back in your main window where you are still logged in as User 1, visit your people page.  The url for this page is something like http://EXAMPLE.berkeley.edu/admin/people. You should see a new user with a username matching your first and last names. Click the "edit" link to the right of this user and assign it the administrator role. 
4. Now in your first browser you can logout of the User 1 account and visit the CAS url (above) to log back in via CalNet. At this point you should be able to do anything that User 1 could do.
5. **Make sure that you have set a hard-to-guess password for User 1.** 

Should something go wrong with CAS or the CAS module on your site, you can still login as User 1 using the [administrator back door](#back_door). (Please do not use the backdoor unless it is really necessary.)

*It's a good idea to change the password on your User 1 account to a long random string.  This will make it less likely that your site would be compromised by a brute force password attack. If you forget the password for User 1 [it can be reset](https://www.drupal.org/node/44164). When you need to login as User 1 you can use the [administrator back door](#back_door) which includes a link to reset your password.*


## The Administrator "Back Door" 

Above, we mentioned that the path /user/login will not provide the traditional Drupal login form.

If for some reason you can't login as your CalNet-authenticated
administrator, you can still login as User 1 at
http://example.com/user/admin_login.

!!! warning

    This is not a secure form and it suffers from the Drupal
    standard authentication vulnerabilities described above. Do not distribute this 
    URL to your users and do not make a practice of logging in via this URL.

## What if auth.berkeley.edu goes down?

This would affect all campus services depending on CAS.  The 
responsibility for fixing the problem would be on the CalNet team.

Sites using UC Berkeley CAS can be toggled to use an offsite CAS cluster should 
this scenario arise.

Toggle your site to use the offsite CAS cluster:
```
$ drush vset ucberkeley_envconf_cas_backup_server_enabled 1
```
Confirm that cas_server was updated to the backup server:
```
$ drush vget cas_server
cas_server: cas-p4.calnet.berkeley.edu
```
Try authenticating to your site.

When the auth.berkeley.edu is back online, toggle your site to use this server
again:
```
$ drush vset ucberkeley_envconf_cas_backup_server_enabled 0
ucberkeley_envconf_cas_backup_server_enabled was set to "0".                                               [success]
```
Confirm the change:
```
$ drush vget cas_server
cas_server: auth-test.berkeley.edu
```

## Upgrading 
### Upgrading to a new version of ucberkeley_cas (version 2.x or 3.x) 

Follow this procedure to upgrade ucberkeley_cas:

Upgrade the module on your development site first and test it.
Before you upgrade the module on your live site, make a backup of your
live site's database just in case there is a problem.

1. Delete the ucberkeley_cas folder form your modules directory.
2. Replace it with the new version of the module downloaded which you
downloaded and unarchived
3. Run update.php on your drupal site.

The new version should now be working.  Test it on your development
site to make sure that logging in and out of the site is working.

!!! warning
    **Do not uninstall the cas module found in ucberkeley_cas
    folder.** (By "uninstall" we mean "run the Drupal uninstall process
    at /admin/modules/uninstall.)  Doing this can cause problems with
    your existing CAS users. 

### Upgrading from legacy ucb\_cas 1.x to ucberkeley\_cas 2.x 

The module UC Berkeley CAS (ucberkeley\_cas) is a replacement for the older UCB CAS (ucb\_cas) module.  UCB CAS must be removed from your system before UC Berkeley CAS can be installed.

Here's what to do:

1. Disable UCB CAS by un-checking its entry at /admin/modules and clicking submit (or by using drush).
2. Do not tell Drupal to "uninstall" UCB CAS. Also do not "uninstall" the CAS module. By this we mean do not use the "Uninstall" tab which is available at the `/admin/modules` path when you are logged into your site as an administrator.
3. Using your file manager simply remove the ucb_cas folder from your site (look under `/sites/all/modules` or `/profiles`).
4. Check that you have added and enabled the other modules required by UC Berkeley CAS to your site. See the [Requirements section](#requirements).
5. Enable UC Berkeley CAS. (At this point you may see a message about ucb_envconf. See the instructions below.)
6. Run update.php

#### If you are using ucb\_envconf 1.x, upgrade to ucberkeley\_envconf 2.x 

_For background on UC Berkeley Environments Configurations see [this UC Berkeley Environments Configuration section](#the-uc-berkeley-environment-configurations-module)._

The module UCB Berkeley CAS (ucberkeley_cas) can optionally be used with the UC 
Berkeley Environment Configurations module. If you have upgraded from an old 
(1.x) version of UC Berkeley CAS, you may also have an old version of UC 
Berkeley Environments Configurations installed. If this is the case, the UC 
Berkeley CAS installer will detect the problem and warn you. Here's what to do 
if that happens:

1. Disable UC Berkeley Environment Configurations by un-checking its entry at /admin/modules and clicking submit.
2. Download <a href="https://github.com/ucb-ist-drupal/ucberkeley_envconf-7/releases">the latest version of UC Berkeley Environment Configurations</a>.
3. Using your file manager simply remove the ucb_envconf folder from your site (look under /sites/all/modules or /profiles). (The 1.0 version was called ucb_envconf.  The 2.0 version is called ucberkeley\_envconf.)
4. Unpack the new version of UC Berkeley Environment Configurations and copy it into /sites/all/modules (or your preferred module directory.) 
5. Enable ucberkeley_envconf. (Running update.php is not necessary.)

At this point your site will be using the latest version of UC Berkeley Environment Configurations.

### My site already uses CAS, but I want to switch to ucberkeley_cas 

1. Remove the directories for cas, cas_attributes and ldap from
`site/all/modules` (or wherever they reside).
2. Unpack ucberkeley_cas into `sites/all/modules` or your desired module directory.
3. At admin/modules enable the UC Berkeley CAS module. UC Berkeley CAS will tell
you if there is anything wrong.
4. Run update.php.

UC Berkeley CAS will attempt to configure the ldap server for ldap.berkeley.edu 
at `/admin/config/people/ldap/servers`.  If this duplicates servers that you 
previously configured, disable your old entries.

If you are using UC Berkeley CAS with UCB Environments Configurations,
it's important that your LDAP server uses the same "sid" (see the `ldap_servers` table
in the database) that is specified in `ucberkeley_cas.features.defaultconfig.inc`.

## Uninstalling 

To remove UC Berkeley CAS from your site see the modules listed in the [Requirements](#requirements) section above.

1. Disable these modules at admin/modules.
2. Uninstall these modules at admin/modules/uninstall. *Uninstalling will disable any CAS-authenticated users on your site. Re-enabling those users is not straightforward.*

### Avoid Uninstalling the CAS Module 

If you have accounts on your site that were created while the UC Berkeley CAS
module was enabled, avoid uninstalling the cas module found in the 
ucberekely_cas folder. If you uninstall the cas module you will find that your 
existing CalNet-authenticated accounts can no longer login. They will be
greeted with a PHP error like

  `Integrity constraint violation: 1062 Duplicate entry 'Brian Wood'...`

The reason for this error is that the uninstallation removed the
cas\_users database table and the data it contained.  This is not an
issue with ucberkeley_cas, but with the cas module itself.

In order to preserve your CalNet-authenticated accounts follow this
uninstall/reinstall procedure:

1. Using phpMyAdmin (or the mysql command line client), export the
contents of your ucberkeley_cas table to a file. In phpMyAdmin click on the
cas_user table then click on the Export link at the bottom of the
page.  Choose "Dump all rows" at the next screen and submit the
form. This will download a file called cas_user.sql which will contain
data like:
```
  INSERT INTO `cas_user` (`aid`, `uid`, `cas_name`) VALUES
  (1, 15, '212373'),
  (3, 19, '212372');
```
2. Uncheck the UC Berkeley CAS module on the Modules administration page and
submit the form. 

3. Click the Uninstall tab at the top of the page and uninstall UCB
CAS.

4. Reinstall the UC Berkeley CAS module by checking its box on the Modules
administration page and submitting the form.

5. In phpMyAdmin select your database and choose the SQL tab. Replace
the contents of the sql query text area with the insert statements
from step 1. Submit the form. 

Now your existing CalNet-authenticated accounts should be able to
login via CAS.

#### Alternate procedure

You could also simply delete all of your existing CalNet users and
then recreate them by having the users log in again after you
reinstall the ucberkeley_cas module.  *If you do this, the recreated users
will no longer be the owners of any content that they created.* The sql
solution above will preserve the users association to their content.

## Standard Configuration 

The ucberkeley_cas module has made some configuration decisions for you.
These decisions can be overridden by you. See the [Configuration Details](configuration)
section below.

### The Authenticated User role 

With the ucberkeley_cas standard configuration, when a user logs in
via CalNet for the first time Drupal will create an account for them
and assign them to the "authenticated user" role. **We suggest that
you avoid assigning the "authenticated user" role any privileges
beyond those assigned to the "anonymous user" role.** The best way to
manage privileges for your CalNet users is to create a role like
"editor" which is allowed to create content. You should then review
each new person in the "authenticated user" role and decide whether or
not to assign them your "editor" role.  (Drupal's Rules module can be used
to send you automatic emails each time a new user account is created
on your site.)

### Combining Drupal Standard Authentication with CAS 

"Mixed mode authentication" describes then scenario where a site is
configured to allow some users to authenticate using CalNet and others
to authenticate using Drupal standard authentication. The ucberkeley_cas
standard configuration is not geared towards this scenario.  Instead
all users are directed to CalNet for authentication.

Mixed mode authentication presents user experience challenges. The
user must be presented with two different login mechanisms and must
choose between them every time they login to the site. It becomes easy 
for a user to create two different accounts, one that authenticates via
CalNet and one that uses Drupal standard authentication. Typically this 
is undesirable.

If you choose to alter the UC Berkeley CAS settings on your site to
allow mixed mode authentication, please remember that you should be
running your UCB site using https if you are using Drupal
standard authentication.

### Use Case Recommendations Page 

After installation see `/admin/config/people/ucberkely_cas` for recommendations
on fine tuning your CAS configuration according to common UCB Drupal
site use cases.

## User accounts for testing

[These test accounts](https://calnetweb.berkeley.edu/calnet-technologists/ldap-directory-service/resources-developers/universal-test-ids) can be useful for testing your website.

## Reporting Bugs 

If you think you've found a bug with UC Berkeley CAS please report it to 
ist-drupal@lists.berkeley.edu. Make sure your bug report includes:

1. The exact steps we should take to recreate the problem.
2. The version of ucberkeley_cas that you are using.
3. The version of Drupal that you are using.
