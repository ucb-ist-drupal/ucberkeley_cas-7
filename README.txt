README ucb_cas-7.x
==================

TABLE OF CONTENTS
-----------------
*   Purpose
*   Quick Start
*   Standard Configuration 
    *   Security
    *   User Account creation (*Important*)
    *   Standard configuration doesn't support "mixed mode authentication"
    *   Admin recommendations page
*   Requirements
*   UCB CalNet Registration
*   Installing
    * Installing if your site already uses the modules that ucb_cas provides
*   Setup a Calnet-authenticated Administrator
*   Administrator "back door" for lockouts
*   Disabling 
*   Uninstalling 
    *   Uninstalling/re-installing and preserving your Calnet users (Important)
*  Upgrading to a new version of ucb_cas
*  Configuration Details
*  Launching your site (Important)
*  Drush vget (varaible get) may not be accurate for the cas_server variable
*  FAQ
*  Authors


PURPOSE
-------

UCB CAS is a collection of modules needed use UC Berkeley CalNet
authentication and UC Berkeley LDAP with a Drupal site. Once UCB CAS
is enabled logging into your site via CalNet should "just work."

UCB CAS applies a default configuration to the modules it
installs. This configuration assumes that everyone, including the site
administrators, will login to the site using Calnet/CAS.  See Standard
Configuration.

QUICK START
-----------

1. See: Requirements
2. Install and enable ucb_cas. (More info: Installing)
3. Visit (the unpublicized) login url http://example.com/cas and login
with your calnet id.
4. As User 1 edit the new user that got created in step 2 and assign
it the "administrator" role. (More info: Setup a Calnet-authenticated
administrator)
5. Visit http://example.com/admin/config/people/ucbcas

Note: See Administrator "back door" for lockouts

STANDARD CONFIGURATION
----------------------

The ucb_cas module has made some configuration decisions for you.
These decsions can be overridden by you. See the Configuration Details
section below.

*Security*

By default ucb_cas is configured so that anyone logging into your site
must use UCB Calnet authentication. The reason for this is that
Drupal's standard authentication is insecure, unless used in
conjunction with SSL (https). Drupal standard authentication is
vulnerable to 1) username/password interception (especially if a
wireless network is in use) and 2) session hijacking. (See "Setup a
Calnet-authenticated administrator.")

(Calnet/CAS authentication is not immune to attack on a site running
under http.  In this scenario the CAS ticket could possibly be
hijacked. The combination of https with ucb_cas's default
configuration is a good idea.)

*User account creation (IMPORTANT)*

With the ucb_cas standard configuration, when a user logs in via
CalNet for the first time Drupal will create an account for them and
assign them to the "authenticated user" role. Be very cautious with
assigning the "authenticated user" role any privileges beyond what the
"anonymous user" role has. The best way to manange privileges for your
CalNet users is to create a role like "editor" which is allowed to
create content. You should then review each new person in the
"authenticated user" role and decide whether or not to assign them
your "editor" role.  (The Rules module can be used to send you
automatic emails each time a new user account is created on your
site.)

*Standard configuration doesn't support "mixed mode authentication"*

"Mixed mode authentication" describes then scenario where a site is
configured to allow some users to authenticate using Calnet and others
to authenticate using Drupal standard authentication. The ucb_cas
standard configuration is not geared towards this scenario.  Instead
all users are directed to Calnet for authentication.

Mixed mode authentication presents user experience challenges. The
user must be presented with two different login mechanisms and must
choose between them everytime they login to the site. A module
(ucb_mma) is planned to address this use case.  Until then you may
choose to adjust the ucb_cas default settings, if mma is what you
need. Please remember that you should be running your UCB site using
SSL (https) if you are using Drupal standard authentication.

*Admin recommendations page*

After installation see /admin/config/people/ucbcas for recommendations
on fine tuning your CAS configuration according to common UCB Drupal
site use cases.

REQUIREMENTS
------------
Your Drupal site must be registered with the UCB Calnet service. (See: UCB Calnet Registration)

The modules installed by ucb_cas-7.x are:

cas
cas_attributes (includes cas_ldap)
ldap (includes ldap_servers and others)

Since UCB CAS installs multiple modules on your site, its install
process will ensure that those modules do not already exist on your
site.  If conflicting files are found a friendly message will appear
and the installer will abort. If you see this error message when you
enable the module, check the directories that drupal scans for module
files (e.g. sites/all/modules, sites/EXAMPLE/modules,
sites/modules/EXAMPLE/, profiles/EXAMPLE...) for conflicting modules.
If you find conflicts:

1. Disable the modules at admin/modules
2. Remove the files for the modules from your site
3. Install UCB CAS
4. Run update.php


UCB CALNET REGISTRATION
-----------------------

In order to use CalNet authentication, your website must be registered with 
CalNet. Make sure your registration is approved before you install UCB CAS 
on a production site.

Developers working locally may use either localhost or 127.0.0.1, with or 
without a port number, as their site URL without needing to register.

To register, see https://wikihub.berkeley.edu/display/calnet/CAS+Registration.


INSTALLING
----------
(This process is tested with drush.)
1. Make sure your site meets the requirements above.
2. Download ucb_cas-7.x-x.x.tar.gz to the computer running your Drupal site.
3. Unarchive the module in sites/all/modules
4. Enable the module at admin/modules.  You ONLY need to enable the
UCB CAS module the other modules will be enabled and configured for
you.
5. Test your site:

If your site runs at http://example-dev.berkeley.edu, go to
http://example-dev.berkeley.edu/cas.  You should see the CAS login
page.  When you authenticate successfully you should be returned to
your Drupal site and you should see "Logged in as YOUR NAME." 

Go to http://example-dev.berkeley.edu/user.  You should see the email
address that was retrieved from LDAP for your account.

Installing if your site already uses the modules that ucb_cas provides
----------------------------------------------------------------------

1. Remove the directories for cas, cas_attributes and ldap from
site/all/modules (or wherever they reside).

2. Disable the ldap configuration modules from /admin/modules
(the module administsration page). You don't need to disable the cas
modules.  (Do not run the uninstall process for the cas module at
/admin/modules.)

3. Unpack ucb_cas into sites/all/modules.

4. At admin/modules enable the UCB CAS module. UCB CAS will tell
you if there is anything wrong.

5. Run update.php.

If UCB CAS finds that you have setup LDAP servers, it will rename, and
disable them, but they will be preserved in case you need to refer
back to them.  Then it will install LDAP servers with parameters known
to work. If you are using UCB CAS with UCB Environments
Configurations, it's important that your LDAP servers have the "sids"
(see the database) specified by ucb_cas.install.


SETUP A CALNET-AUTHENTICATED ADMINISTRATOR
------------------------------------------

User 1 (the account is often named "admin) is the "superuser" on a Drupal
site. Instead of using logging in as this user, you should grant your
Calnet-authenticated user account the administrator role and always
login (via Calnet) with that account.  Here's how:

1. Enable the ucb_cas module
2. Login to your site by visiting http://example.com/cas
3. Drupal creates a user for you upon successful authentication. Visit
http://example.com/admin/people, edit your user and assign it the
"administrator role.

Now your Calnet user can do anything that User 1 can do.


THE ADMINISTRATOR "BACK DOOR" FOR LOCKOUTS
------------------------------------------

If for some reason you can't login as your calnet-authenticated
administrator, you can login as User 1 at
http://example.com/user/admin_login.

WARNING: The admin_login page is included to help people who are
otherwised locked out of their sites.  It should only be used to
recover from a lockout. It is not a secure form and it suffers from
the Drupal standard authentication vulnerabilities described above.


DISABLING
---------
(This process is tested with drush.)

For maximum flexibilty, disabling ucb_cas does not disable the
companion cas_attributes nor ldap modules. However uninstalling
ucb_cas *will* disable and uninstall the companion modules.

A module is "disabled" when you uncheck it at /admin/modules and
submit the form.

A module is "uninstalled" when you 1) disable it and 2) uninstall it
at admin/modules/uninstall.

UNINSTALLING
------------
(This process is tested with drush.)

To remove UCB CAS from your site do the following:

1. Disable the UCB CAS module at admin/modules. (As mentioned above
you do not need to disable each individual module that UCB CAS
installed.)

2. Uninstall the UCB CAS module at admin/modules/uninstall.
This step will disable and uninstall each module that UCB CAS
installed.  It will also remove variables that UCB CAS added your
site's variables table.

*Uninstalling/re-installing and preserving your Calnet users*

In order to reset the site to the default ucb_cas configuration,
administrators may decide to uninstall and then re-install ucb_cas.
If you have accounts on your site that were created while the UCB CAS
module was enabled, care should be taken with this
uninstalling/reinstalling. If you simply uninstall ucb_cas (or the cas
submodule) and then reinstall it, you will find that your existing
Calnet-authenticated accounts can no longer login. They will be
greeted with a PHP error like

  Integrity constraint violation: 1062 Duplicate entry 'Brian Wood'...

The reason for this error is that the uninstallation removed the
cas_users database table and the data it contained.  This is not an
issue with ucb_cas, but with the cas module itself.

In order to preserve your Calnet-authenticated accounts follow this
uninstall/reinstall procedure:

1. Using phpMyAdmin (or the mysql command line client), export the
contents of your ucb_cas table to a file. In phpMyAdmin click on the
cas_user table then click on the Export link at the bottom of the
page.  Choose "Dump all rows" at the next screen and submit the
form. This will download a file called cas_user.sql which will contain
data like:

  INSERT INTO `cas_user` (`aid`, `uid`, `cas_name`) VALUES
  (1, 15, '212373'),
  (3, 19, '212372');

2. Uncheck the UCB CAS module on the Modules administration page and
submit the form. 

3. Click the Uninstall tab at the top of the page and uninstall UCB
CAS.

4. Reinstall the UCB CAS module by checking it on the Modules
administration page and submitting the form.

5. In phpMyAdmin select your database and choose the SQL tab. Replace
the contents of the sql query text area with the insert statements
from step 1. Submit the form. 

Now your existing Calnet-authenticated accounts should be able to
login via CAS.

Alternative procedure:

You could also simply delete all of your existing calnet users and
then recreate them by having the users log in again after you
reinstall the ucb_cas module.  If you do this, the recreated users
will no longer be the owners of any content that they created. The sql
solution above will preserve the users association to their content.


UPGRADING TO A NEW VERSION OF UCB_CAS
-------------------------------------
Follow this procedure to upgrade ucb_cas:

Upgrade the module on your developement site first and test it.
Before you upgrade the module on your live site, make a backup of your
live site's database just in case there is a problem.

(Do not disable and uninstall ucb_cas at /admin/modules.)

1. delete the ucb_cas folder form your modules directory

2. replace it with the new version of the module downloaded which you
downloaded and unarchived

3. run update.php on your drupal site

The new version should now be working.  Test it on your development
site to make sure that logging in and out of the site is working.


CONFIGURATION DETAILS
---------------------

In order to make CAS and LDAP work out-of-the-box when you install UCB
CAS, we've made some configuration decisions for you.  These decisions
are aimed at defining "best practices" for using CAS and LDAP with
your Drupal site.  That said, if you don't like our decisions, you can
override them on the appropriate admin page on your site.

UCB CAS Configuration at /admin/config/people/cas:

  *Logout Behavior*
 
    UCB CAS configures the site to request destruction of the CAS
    ticket when the user logs out. This is considered more secure. You
    can change this if you need "single signon" behavior. See
    /admin/config/people/cas.
   
  */user/logout takes you to /caslogout*

    This is considered best practice on campus.  If you need to change 
    this remove the alias at /admin/config/search/path.
     
CAS Configuration at admin/config/people/cas:

  *Inital login destination* and *Logout destination*

	You may want to customize these. Feel free...

  *Automatically create Drupal accounts* 
  
       If you do not want Drupal to create accounts for every CalNet user 
       who attempts to log in to your site, go to admin/config/people/cas, 
       open the User Accounts section, and uncheck Automatically create 
       Drupal accounts.
  
  *Users cannot change password*

       Unchecking this is very likely to cause confusion.  Users
       should change their passwords via CalNet. See *Change password
       URL* further down.

  *Change Password URL*

     This setting is blank because it can cause confusion. The
     intention of ucb_cas is that all users log into the site using
     Calnet/CAS authentication as opposed to Drupal's standard
     authentication.  Therefore changing your site password would
     require changing your Calnet password (which can be done at
     https://net-auth.berkeley.edu/cgi-bin/krbcpw) and would result in
     your password changing for all Calnet authenticated applications.
     A user presented with a "change password" url might not
     understand the ramifications here.

  *Drupal Login Invitation*

	  This setting is blank because it can cause confusion.  It
	  adds a link to your login block allowing users to login
	  using Drupal's stadandard authentication instead of CalNet.
	  It's best to require ALL of your users to login via CAS and
	  not to give them the option of using Drupal's
	  authentication.  If you need to allow people who don't have
	  a CalNet ID to login to your site, you can add a value like
	  "Non-UCB people login here" to this text box.

	  IMPORTANT: If you allow standard Drupal authentication to
	  your site you MUST run your site at an https URL.  Failure
	  to do so is a significant security risk yielding multiple
	  vulnerabilities. For example, anyone logging into your site
	  from a public wireless network can easily have their
	  password stolen.

	  (There is a module in the works to facilitate using both CAS
	  and standard Drupal authentication on a site. Email
	  ist-drupal@lists.berkeley.edu for more information.)

Cas Attributes configuration (admin/config/people/cas/attributes)

  *Fetch CAS Attributes*

       You can change this to "only when a CAS account is created
       (i.e., the first login of a CAS user)."  That means your site
       will not reflect changes made to LDAP after the user account
       was created on your site.

LAUNCHING YOUR SITE (Important)
-------------------------------

The module [ucb_envconf](http://drupal-apps.berkeley.edu/node/4)
ensures that your cas and ldap server settings are correct based on
your development environment on Pantheon. If you are not using this
module, you'll need to manually edit these server settings when
whenever you migrate your site (or just the database) between you dev,
test and live environments. Here's the information for non-users of
ucb_envconf:

Your site is using the servers ldap-test.berkeley.edu and
auth-test.berkeley.edu.  These are the correct servers to use
for site development and testing.  When you make your site
live, you should change these servers to ldap.berkeley.edu and
auth.berkeley.edu. Make these changes at:

admin/config/people/cas
admin/config/people/cas/attributes

DRUSH VGET (VARAIBLE GET) MAY NOT BE ACCURATE FOR THE CAS_SERVER VARIABLE
-------------------------------------------------------------------------

This only applies to sites using the ucb_envconf module:

drush @somealias vget cas_server

Because this module applies configuration on hook_boot() and because
hook_boot doesn't run when you issue 'drush vget', you will encounter
situations where 'drush vget' reports the wrong value.  If you visit
the corresponding admin page, you should see the right value.

Theorectically you could get the correct value with 

drush @somealias php-eval "echo variable_get('cas_server', NULL);"


FAQ
---

Q. When logging in I get the error "user warning: Duplicate entry
'Brian Wood' for key 'name' query: UPDATE users SET name = 'Brian
Wood', mail = 'bwood@example.com', data = 'a:0:{}' WHERE uid = 7 in
/Users/bwood/Sites/dev6/modules/user/user.module on line 248."

A. See "Uninstalling/re-installing and preserving your Calnet users."


Q. When I try to edit a user created by the cas module, I get a
validation error on the email address.  Why is this?

A. All accounts on a Druapl site must have unique email addresses.
Ofter a site admin user their own address for User 1 and then they
Calnet authenticate to create a new account for themselves.  The
account gets created, but if they try to edit it, they get a
validation error on the email field since it is the email that is
already in use by User 1. To fix this, change the User 1 email.

Q. I created a new role and I noticed that newly added CAS users 
were automatically being assigned to this role.

A. Prior to 7.x-1.3-beta2, if you created a new role after 
installing ucb_cas newly added users were automatically assigned
the new role in some situations. Take a look at 
/admin/config/people/cas > User Accounts and ensure that the correct
roles are selected there.  If you find incorrect roles selected, just
unselect them.  This problem in the installer has been fixed.


AUTHORS
-------
Brian Wood, UC Berkeley, http://drupal.org/user/164217

CONTRIBUTORS
------------
Caroline Boyden, UC Berkeley
