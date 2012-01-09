TABLE OF CONTENTS
-----------------
1. Purpose
2. Standard Configuration (IMPORTANT)
2. UCB CalNet Registration
3. Requirements
4. Installing ucb_cas
5. Disabling ucb_cas
6. Uninstalling ucb_cas
7. Configuration Details
8. Launching your site (Important)
9. Authors

***** IMPORTANT ******
This was adapted from the 6.x README. There may be some D6-specific
things still to be corrected.
***** IMPORTANT ******

PURPOSE
-------

UCB CAS is a collection of modules needed use UC Berkeley CalNet
authentication and UC Berkeley LDAP with a Drupal site. Once UCB CAS
is enabled logging into your site via CalNet should "just work."

STANDARD CONFIGURATION
----------------------

The ucb_cas module has made some configuration decisions for you.
These decsions can be overridden by you. See the Configuration Details
section below.

IMPORTANT: User account creation: With the ucb_cas standard
configuration, when a user logs in via CalNet for the first time
Drupal will create an account for them and assign them to the
"authenticated user" role. Be very cautious with assigning the
"authenticated user" role any privileges beyond what the "anonymous
user" role has. The best way to manange privileges for your CalNet
users is to create a role like "editor" which is allowed to create
content. You should then review each new person in the "authenticated
user" role and decide whether or not to assign them your "editor"
role.  (The Rules module can be used to send you automatic emails each
time a new user account is created on your site.)


UCB CALNET REGISTRATION
-----------------------

In order to use CalNet authentication, your website must be registered with 
CalNet. Make sure your registration is approved before you install UCB CAS 
on a production site.

Developers working locally may use either localhost or 127.0.0.1, with or 
without a port number, as their site URL without needing to register.

To register, see https://wikihub.berkeley.edu/display/calnet/CAS+Registration.

REQUIREMENTS
------------
The modules installed by UCB CAS are:

cas
cas_attributes (includes cas_ldap)
ldap (includes ldap_servers and others)

Since UCB CAS installs multiple modules on your site, its install
process will ensure that those modules do not already exist on your
site.  If conflicting files are found a friendly message will appear and the
installer will abort.

If you see this error message when you enable the module, check the
directories that drupal scans for module files
(e.g. sites/all/modules, sites/EXAMPLE/modules,
sites/modules/EXAMPLE/, profiles/EXAMPLE...) for conflicting modules.
If you find conflicts:

1. Disable the modules at admin/modules
2. Remove the files for the modules from your site
3. Install UCB CAS
4. Run update.php


INSTALLING
----------

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

DISABLING
---------

For maximum flexibilty, disabling ucb_cas does not disable the
companion cas_attributes nor ldap modules. However uninstalling
ucb_cas *will* disable and uninstall the companion modules.

A module is "disabled" when you uncheck it at /admin/modules and
submit the form.

A module is "uninstalled" when you 1) disable it and 2) uninstall it
at admin/modules/uninstall.

UNINSTALLING
------------

To remove UCB CAS from your site do the following:

1. Disable the UCB CAS module at admin/modules. (As mentioned above
you do not need to disable each individual module that UCB CAS
installed.)

2. Uninstall the UCB CAS module at admin/modules/uninstall.
This step will disable and uninstall each module that UCB CAS
installed.  It will also remove variables that UCB CAS added your
site's variables table.

CONFIGURATION DETAILS
---------------------

In order to make CAS and LDAP work out-of-the-box when you install UCB
CAS, we've made some configuration decisions for you.  These decisions
are aimed at defining "best practices" for using CAS and LDAP with
your Drupal site.  That said, if you don't like our decisions, you can
override them on the appropriate admin page on your site.

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

LAUNCHING YOUR SITE (IMPORTANT)
-------------------------------

	Your site is using the servers ldap-test.berkeley.edu and
	auth-test.berkeley.edu.  These are the correct servers to use
	for site development and testing.  When you make your site
	live, you should change these servers to ldap.berkeley.edu and
	auth.berkeley.edu. Make these changes at:

        admin/config/people/cas
        admin/config/people/cas/attributes

        (A module to help automate this is in the works.)

AUTHORS
-------
Brian Wood, UC Berkeley, http://drupal.org/user/164217
