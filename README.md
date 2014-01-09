# UC Berkeley CAS 7.x #

*   [Purpose]("#purpose")
    *   [Enhanced Security](#enhanced_security)
*   [Quick Start](#quick_start)
*   [Requirements](#requirements)
    *   [UC Berkeley CalNet Registration](#registration)
*   [Installation](#installation) 
*   [Setup a CalNet-authenticated Administrator](#setup_a)
*   [Administrator "back door"](#back_door)
*   [Upgrading](#upgrading)
    *   [Upgrading from ucb\_cas 1.x to ucberkeley\_cas 2.x](#1.x_2.x)
    *   [Upgrading to a new version of ucberkeley_cas 2.x](#to_newver)
    *   [My site already users CAS, and I want to switch to ucberkeley_cas](#my_sitealready)
*   [Uninstalling](#uninstalling)
    *   [Avoid uninstalling the cas module](#avoid_uninstall)
*   [Standard Configuration](#standard_configuration) 
    *   [The Authenticated User Role](#authenticated_user) 
    *   [Combining Drupal Standard Authentication with CAS](#mixed_mode)
    *   [Use Case Recommendations](#use_case)
*  [Configuration Details](#configuration_details)
    *  [Is it okay to modify the default configuration?](#okay_modify)
    *  [CAS Configuration](#cas_configuration)
        *  [Logout Behavior](#logout_behavior)
        *  [Initial login destination and Logout destination](#initial_login)
        *  [Automatically create Drupal accounts](#automatically_create)
        *  [Users cannot change password](#users_cannot)
        *  [Change Password URL](#change_password)
        *  [Drupal Login Invitation](#drupal_login)
    *  [Cas Attributes configuration](#cas_attributes)
        *  [Fetch CAS Attributes](#fetch_cas)
*  [The UC Berkeley Environment Configurations module](#envconf)
    *  [Drush vget cas_server with UC Berkeley Environment Configurations](#envconf_drush)
*  [FAQ](#faq)
*  [Reporting Bugs](#bugs)
*  [Authors](#authors)


<a name="purpose">
# Purpose #
</a>
UC Berkeley CAS is a Drupal
"[feature](http://drupal.org/projects/features)" (a collection of
modules and configuration) which adds UC Berkeley CalNet
(CAS) authentication to a Drupal site. Once UC Berkeley CAS is enabled
logging into your site via CalNet will be possible by visiting http://EXAMPLE.berkeleyedu/cas

UC Berkeley CAS applies a default configuration to the modules it
installs. This configuration assumes that everyone, including the site
administrators, will login to the site using CalNet.  See Standard
Configuration.

<a name = "enhanced_security">
## Enhanced Security ##
</a>

By default ucberkeley_cas is configured so that anyone logging into
your site **must** use UCB CalNet authentication. If you visit
http://EXAMPLE.berkeley.edu/user/login after installing UC Berkeley
CAS, you will find a CAS login form instead of the familiar Drupal
login form. The reason for this is that Drupal's standard
authentication is insecure, unless used in conjunction with SSL
(https). Drupal standard authentication is vulnerable to (1)
username/password interception (especially if a wireless network is in
use) and (2) session hijacking.

(CalNet/CAS authentication is not immune to attack on a site running
under http. A CAS ticket could theoretically be hijacked, however
exploiting an stolen CAS ticket is significantly more difficult
than exploiting Drupal's standard authentication token. The most
secure configuration for your site would be to use SSL (https) in
addition to UC Berkeley CAS.)

<a name="#quick_start">
# Quick Start #
</a>

1. See: Requirements
2. Install and enable ucberkeley_cas. (More info: Installing)
3. Using a second browser visit the (unpublicized) login url
http://example.com/cas and login with your CalNet id.
4. In your first browser, as User 1, edit the new user that got
created in the last step and assign it the "administrator" role. (More info:
Setup a CalNet-authenticated administrator)
5. Visit the path /admin/config/people/ucbcas on your site for recommendations on further configuration.

<a name="requirements">
# Requirements #
</a>

You need the following modules installed:

* [Features](http://drupal.org/project/features)
* [Strongarm](http://drupal.org/project/strongarm)
* [Token](http://drupal.org/project/token)
* [CTools](http://drupal.org/project/ctools)
* [Default Config](http://drupal.org/project/defaultconfig)

The UC Berkeley CAS feature already contains these modules:

* CAS
* CAS Attributes
* LDAP

<a name="registration">
## CalNet Registration ##
</a>
In order to use CalNet authentication, your website must be registered with 
CalNet. Make sure your registration is approved before you install UC Berkeley CAS 
on a production site.

Pantheon customers can use [this email template with default Pantheon settings](https://wikihub.berkeley.edu/display/drupal/Launch+your+Pantheon+site?src=search#LaunchyourPantheonsite-RegisterforCalNetauthentication).  Non-Pantheon customers can use [this generic template](https://wikihub.berkeley.edu/display/calnet/CAS+Registration).

Answering "Yes" to the "Requires Re-authentication" is considered the best
practice for UC Berkeley Drupal sites. For more about this see [Logout Behavior](#Logout_Behavior).

### Localhost Sites Do Not Require Registration ###

Developers working local webserver do not need to register URLs like
http://localhostand http:127.0.0.1. These urls will work with the CAS
server auth-test.berkeley.edu.


<a name="installation">
# Installation #
</a>

1. Make sure your site meets the requirements above.
2. Download ucberkeley_cas-7.x-x.x.tar.gz to the computer running your Drupal site.
3. Unarchive the module in sites/all/modules
4. Enable the module at admin/modules.  
5. Test your site:

If your site runs at http://EXAMPLE.berkeley.edu, go to
http://EXAMPLE.berkeley.edu/cas.  You should see the CAS login
page.  When you authenticate successfully you should be returned to
your Drupal site and you should see "Logged in as \[your name\]." 

Go to http://EXAMPLE.berkeley.edu/user.  You should see the email
address and full name that was retrieved from LDAP for your account.

<a name = "setup_a">
# Setup a CalNet-authenticated Administrator #
</a>

User 1 (the account is often named "admin) is the "superuser" on a Drupal
site. Instead of using logging in as this user, you should grant your
CalNet-authenticated user account the administrator role and always
login (via CalNet) with that account.  Here's how:

1. Enable the ucberkeley_cas module
2. Login to your site by visiting http://EXAMPLE.berkeley.edu/cas
3. Drupal creates a user for you upon successful authentication. Visit
http://EXAMPLE.berkeley.edu/admin/people, edit your user and assign it the
"administrator role.

Now your CalNet user can do anything that User 1 can do.

We strongly recommend that you avoid logging into your site as User 1
since this user will use Drupal standard authentication which is
insecure unless used in combination with SSL (https).

<a name = "back_door">
# The Administrator "Back Door" #
</a>

Above, we mentioned that the path /user/login will not provide the traditional Drupal login form.

If for some reason you can't login as your CalNet-authenticated
administrator, you can still login as User 1 at
http://example.com/user/admin_login.

**WARNING:** This is not a secure form and it suffers from the Drupal
standard authentication vulnerabilities described above. Do not use
this form unless you really have to.

<a name = "upgrading">
# Upgrading #
</a>

<a name = "1.x_2.x">
## Upgrading from ucb_cas 1.x to ucberkeley_cas 2.x ##
</a>

The module UC Berkeley CAS (ucberkeley_cas) is a replacement for UCB CAS (ucb_cas).  UCB CAS must be removed from your system before UC Berkeley CAS can be installed.

Here's what to do:

1. Disable UCB CAS by un-checking its entry at /admin/modules and clicking submit (or by using drush).
2. (Do not tell Drupal to "uninstall" UCB CAS. Also do not "uninstall" the CAS module. By this we mean do not use the "Uninstall" tab which is available at the /admin/modules path when you are logged into your site as an administrator. Also do not use the drush pm-uninstall command to uninstall these modules.)
3. Using your file manager simply remove the ucb_cas folder from your site (look under /sites/all/modules or /profiles).
4. Check that you have added and enabled the other modules required by UC Berkeley CAS to your site. (These modules (cas, cas_attributes, ldap...) might already be in the ucberkeley_cas folder.)
5. Enable UC Berkeley CAS. (At this point you may see a message about ucb_envconf. See the instructions below.)
6. Run update.php

At this point it's a good idea to:

a. Visit admin/reports/updates and verify that you have the latest version of cas, cas_attributes and ldap.
b. Visit admin/reports/status and verify that you have the latest version of the phpCAS library.  (Compare http://downloads.jasig.org/cas-clients/php/current/).

### If you are using ucb_envconf 1.x, upgrade to ucberkeley_envconf 2.x ###

(Using [ucberkeley_envconf](#envconf) is recommended if you host your site on Pantheon.)

The module UCB Berkeley CAS (ucberkeley_cas) requires version 2.0 or later of UC Berkeley Environment Configurations. An older version of UC Berkeley Environment Configurations has been detect on this site.

Here's what to do:

1. Disable UC Berkeley Environment Configurations by un-checking its entry at /admin/modules and clicking submit.
2. Download <a href="http://drupal-apps.berkeley.edu/content/ucb-environment-configurations">the new version of UC Berkeley Environment Configurations</a>.
3. Using your file manager simply remove the ucb_envconf folder from your site (look under /sites/all/modules or /profiles).
4. Unpack the new version of UC Berkeley Environment Configurations and copy it into /sites/all/modules (or your preferred module directory.) 
5. Enable ucberkeley_envconf. (Running update.php is not necessary.)

At this point your site will be using the latest version of UC Berkeley Environment Configurations.

<a name = "to_newver">
## Upgrading to a new version of ucberkeley_cas 2.x ##
</a>
Follow this procedure to upgrade ucberkeley_cas:

Upgrade the module on your development site first and test it.
Before you upgrade the module on your live site, make a backup of your
live site's database just in case there is a problem.

1. Delete the ucberkeley_cas folder form your modules directory.
2. Replace it with the new version of the module downloaded which you
downloaded and unarchived
3. run update.php on your drupal site

The new version should now be working.  Test it on your development
site to make sure that logging in and out of the site is working.

**NOTE: Do not uninstall the cas module found in ucberkeley_cas
folder.** By "uninstall" we mean "run the Drupal uninstall process
at /admin/modules/uninstall.  Doing this can cause problems with
your existing CAS users. (Deleting and replacing the entire
ucberkeley\_cas folder is fine.)

<a name = "my_sitealready">
## My site already uses CAS, but I want to switch to ucberkeley_cas ##
</a>

1. Remove the directories for cas, cas_attributes and ldap from
site/all/modules (or wherever they reside).
2. Unpack ucberkeley_cas into sites/all/modules or your desired module directory.
3. At admin/modules enable the UC Berkeley CAS module. UC Berkeley CAS will tell
you if there is anything wrong.
4. Run update.php.

UC Berkeley CAS will attempt to setup ldap servers for ldap.berkeley.edu and
ldap-test.berkeley.edu at /admin/config/people/ldap/servers.  If these duplicate servers that you have configured, you can disable your old entries.

If you are using UC Berkeley CAS with UCB Environments Configurations,
it's important that your LDAP servers have the "sids" (see the
database) specified by ucberkeley_cas.features.defaultconfig.inc.

<a name = "#uninstalling">
# Uninstalling #
</a>
To remove UC Berkeley CAS from your site see the modules listed in the[Requirements](#requirements) section above.

1. Disable these modules at admin/modules.
2. Uninstall these modules at admin/modules/uninstall.

<a name = "#avoid_uninstall">
## Avoid Uninstalling the CAS Module ##
</a>
If you have accounts on your site that were created while the UC Berkeley CAS
module was enabled, avoid uninstalling the cas module found in the ucberekely_cas folder. If you uninstall the cas module you will find that your existing
CalNet-authenticated accounts can no longer login. They will be
greeted with a PHP error like

  Integrity constraint violation: 1062 Duplicate entry 'Brian Wood'...

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

  INSERT INTO `cas_user` (`aid`, `uid`, `cas_name`) VALUES
  (1, 15, '212373'),
  (3, 19, '212372');

2. Uncheck the UC Berkeley CAS module on the Modules administration page and
submit the form. 

3. Click the Uninstall tab at the top of the page and uninstall UCB
CAS.

4. Reinstall the UC Berkeley CAS module by checking it on the Modules
administration page and submitting the form.

5. In phpMyAdmin select your database and choose the SQL tab. Replace
the contents of the sql query text area with the insert statements
from step 1. Submit the form. 

Now your existing CalNet-authenticated accounts should be able to
login via CAS.

Alternative procedure:

You could also simply delete all of your existing CalNet users and
then recreate them by having the users log in again after you
reinstall the ucberkeley_cas module.  If you do this, the recreated users
will no longer be the owners of any content that they created. The sql
solution above will preserve the users association to their content.

<a name = "standard_configuration">
# Standard Configuration #
</a>

The ucberkeley_cas module has made some configuration decisions for you.
These decisions can be overridden by you. See the Configuration Details
section below.

<a name = "authenticated_user">
## The Authenticated User role ##
</a>

With the ucberkeley_cas standard configuration, when a user logs in
via CalNet for the first time Drupal will create an account for them
and assign them to the "authenticated user" role. **We suggests that
you avoid assigning the "authenticated user" role any privileges
beyond those assigned to the "anonymous user" role.** The best way to
manage privileges for your CalNet users is to create a role like
"editor" which is allowed to create content. You should then review
each new person in the "authenticated user" role and decide whether or
not to assign them your "editor" role.  (Drupal's Rules module can be used
to send you automatic emails each time a new user account is created
on your site.)

<a name = "mixed_mode">
## Combining Drupal Standard Authentication with CAS ##
</a>

"Mixed mode authentication" describes then scenario where a site is
configured to allow some users to authenticate using CalNet and others
to authenticate using Drupal standard authentication. The ucberkeley_cas
standard configuration is not geared towards this scenario.  Instead
all users are directed to CalNet for authentication.

Mixed mode authentication presents user experience challenges. The
user must be presented with two different login mechanisms and must
choose between them every time they login to the site.

If you choose to alter the UC Berkeley CAS settings on your site to
allow mixed mode authentication, please remember that you should be
running your UCB site using SSL (https) if you are using Drupal
standard authentication.

<a name = "use_case">
## Use Case Recommendations Page ##
</a>

After installation see /admin/config/people/ucbcas for recommendations
on fine tuning your CAS configuration according to common UCB Drupal
site use cases.

<a name="configuration_details">
# Configuration Details #
</a>
In order to make CAS and LDAP work out-of-the-box when you install UCB
CAS, we've made some configuration decisions for you.  These decisions
are aimed at defining "best practices" for using CAS and LDAP with
your Drupal site.  That said, if you don't like our decisions, you can
override them on the appropriate admin page on your site.

<a name="okay_modify">
## Is it okay to modify the default configuration?#
</a>
Yes.

If you are familiar with Drupal features, you may worry that
overriding this configuration will result in you having to "revert"
the feature when it is time to upgrade the code.  This is not the case
with UC Berkeley CAS.  Since the configuration was set using the
Default Config module, changing these values will not put the UC
Berkeley CAS feature into the "overridden" state.

Before you change the configuration, we recommend that you review the
configurations notes below.

<a name="cas_configuration">
## CAS Configuration ##
</a>

Site path: admin/config/people/cas:

<a name="Logout_Behavior">
### Logout Behavior ###
</a>
As mentioned in the CalNet Registration section, requesting that the
UC Berkeley CAS server "require re-authentication" for your site is
the most secure way of configuring your Drupal site. With this
configuration in place, when a logged in user clicks the logout link
on your site they will not be able to log back into your site until
they enter their CalNet username and password again. Without this
configuration in place if a user logs out of your site it is possible
for them to login again (while their Drupal login session is still
valid) by simply revisiting the /cas url which (in this situation)
will not prompt them for their password again. This scenario is
undesirable especially when the logged in user is at a public computer
(e.g. in a library) or if they don't lock their screen when they leave
their computer.

By default UC Berkeley CAS creates a URL Alias that redirects the
"/user/logout" to "/caslogout." This alias is present to mimic the
the "require re-authentication" behavior for sites that *did not*
specify "require re-authentication: yes" in their CAS registration
form.  Relying on these aliases for security is acceptable, but this
makes logging back into the site after logout a bit less convenient.

If your site is using "require re-authentication" the presence of the
above alias will not negatively affect your site. However it will mean
that users are redirected to a generic CAS Logout page when they
follow your logout link.  If you have specified "require
re-authentication" and you want your users to be redirected to, for
example, your site's home page after logout, you can 1. delete the
logout alias at /admin/config/search/path and then 2. specify the
logout destination at /admin/config/people/cas Login/Logout Destinations.

<a name="initial_login">
### Initial login destination and Logout destination ###
</a>

You can configure the landing pages for the user after successful
login/logout.

<a name="automatically_create">
### Automatically create Drupal accounts ###
</a>  
If you do not want Drupal to create accounts for every CalNet user who
attempts to log in to your site, go to admin/config/people/cas, open
the User Accounts section, and uncheck Automatically create Drupal
accounts. As an alternative, you can pre-create CAS users at
/admin/people/cas/create.

<a name="users_cannot">
### Users cannot change password ###
</a>

Unchecking this is very likely to cause confusion.  Users
should change their passwords via CalNet. See [Change password
URL](#change_password) further down.

<a name="change_password">
### Change Password URL ###
</a>

This setting is blank because it can cause confusion.

The intention of ucberkeley_cas is that all users log into the site
using CalNet/CAS authentication as opposed to Drupal's standard
authentication.  Therefore changing your site password would require
changing your CalNet password (which can be done at
https://net-auth.berkeley.edu/cgi-bin/krbcpw) and would result in your
password changing for **all** CalNet authenticated applications.  A
user presented with a "change password" url might not understand the
ramifications here.

<a name="drupal_login">
### Drupal Login Invitation ###
</a>

This setting is blank because it can cause confusion.

This adds a link to your login block allowing users to login using
Drupal's standard authentication instead of CalNet.  It's best to
require ALL of your users to login via CAS and not to give them the
option of using Drupal's authentication.  If you need to allow people
who don't have a CalNet ID to login to your site, you can add a value
like "Non-UCB people login here" to this text box.

IMPORTANT: If you allow standard Drupal authentication to your site
you **are very strongly encouraged** to run your site at an https URL.
Failure to do so is a significant security. For example, anyone
logging into your site from a public wireless network can easily have
their password stolen.

<a name="cas_attributes">
## Cas Attributes configuration ##
</a>

Site path: admin/config/people/cas/attributes

<a name="fetch_cas">
### Fetch CAS Attributes ###
</a>

The default setting is "only when a CAS account is created (i.e., the
first login of a CAS user)."This means that a user can edit their
Drupal profile (assuming they have permission to do so) and change the
name or email address that we found for them in LDAP.  Their edits
will not be over written by a new LDAP lookup on their next login.

<a name = "envconf">
# The UC Berkeley Environment Configurations module #
</a>

The module
[UC Berkeley Environment Configurations](https://github.com/ucb-ist-drupal/ucb_envconf-7)
ensures that your cas and ldap server settings are correct based on
your development environment on [Pantheon](http://getpantheon.com). UC
Berkeley Environment Configurations ensures that your Dev and Test
sites on Pantheon use:

* CAS Server: auth-test.berkeley.edu
* LDAP Server: ldap-test.berkeley.edu

and your Live site uses: 

* CAS Server: auth.berkeley.edu
* LDAP Server: ldap.berkeley.edu

If you are not using this module, you'll need to manually edit these
server settings when you copy your database between the Dev, Test and
Live environments. To manage this manually make these changes at:

* admin/config/people/cas
* admin/config/people/cas/attributes

<a name = "#envconf_drush">
## Drush vget cas_server with UC Berkeley Environment Configurations ##
</a>

This only applies to sites using the ucberkeley_envconf module:

drush @somealias vget cas_server

Because this module applies configuration on hook\_boot() and because
hook\_boot doesn't run when you issue 'drush vget', you will encounter
situations where 'drush vget' reports the wrong value.  If you visit
the corresponding admin page, you should see the right value.

Theoretically you could get the correct value with 

drush @somealias php-eval "echo variable\_get('cas\_server', NULL);"

<a name = "#faq">
# FAQ #
</a>


Q. When logging in I get the error "user warning: Duplicate entry
'Brian Wood' for key 'name' query: UPDATE users SET name = 'Brian
Wood', mail = 'bwood@example.com', data = 'a:0:{}' WHERE uid = 7 in
/Users/bwood/Sites/dev6/modules/user/user.module on line 248."

A. See [Avoid uninstalling the CAS module](#avoid_uninstall)


Q. When I try to edit a user created by the cas module, I get a
validation error on the email address.  Why is this?

A. All accounts on a Drupal site must have unique email addresses.
Often a site admin user their own address for User 1 and then they
CalNet authenticate to create a new account for themselves.  The
account gets created, but if they try to edit it, they get a
validation error on the email field since it is the email that is
already in use by User 1. To fix this, change the User 1 email.

<a name = "bugs">
# Reporting Bugs #
</a>

If you think you've found a bug with UC Berkeley CAS please report it at https://github.com/ucb-ist-drupal/ucberkeley_cas-7/issues. Make sure your bug report includes:

1. The exact steps we should take to recreate the problem.
2. The version of ucberkeley_cas that you are using.
3. The version of Drupal that you are using.


<a name="authors">
# Authors #
</a>
Brian Wood


<!--  LocalWords:  newver sitealready drupal ploiting src sids
 -->
<!--  LocalWords:  LaunchyourPantheonsite ucberekely somealias
 -->
<!--  LocalWords:  RegisterforCalNetauthentication
 -->
