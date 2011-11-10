PURPOSE
-------

UCB CAS is a collection of modules needed use UC Berkeley CalNet
authentication and UC Berkeley LDAP with a Drupal site. Once UCB CAS
is enabled logging into your site via CalNet should "just work."

When a user logs in via CalNet for the first time Drupal will create
an account for them and assign them to the "authenticated user"
role. Be very cautious with assigning the "authenticated user" role
any privileges beyond what the "anoymous user" role has. The best way
to manange privileges for your CalNet users is to create a role like
"editor" which is allowed to create content. You should then review
each new person in the "authenticated user" role and decide whether or
not to assign them your "editor" role.  (The Rules module can be used
to send you automatic emails each time a new user account is created
on your site.)

REQUIREMENTS
------------

Since UCB CAS installs multiple modules on your site, it is best to
ensure that those modules do not already exist on your site.  The files for these modules should not exist in any of the directories that drupal scans for module files (e.g. sites/all/modules, sites/EXAMPLE/modules, sites/modules/EXAMPLE/, profiles/EXAMPLE...).  If you are already using any of these modules on your site, do the following:

1. Disable the modules at admin/build/modules
2. Remove the files for the modules from your site
3. Install UCB CAS
4. Run update.php

The modules installed by UCB CAS are:

cas
cas_attributes (includes cas_ldap)
ldap_integration (includes ldapauth, ldapdata, ldapgroups)
pathauto
token

INSTALLATION
-------------

1. Make sure your site meets the requirements above.
2. Download ucb_cas-6.x-x.x.tar.gz to the computer running your Drupal site.
3. Unarchive the module in sites/all/modules
4. Enable the module at admin/build/modules.  You ONLY need to enable the UCB CAS module the other modules will be enabled and configured for you. 
5. Test your site:

If your site runs at http://example-dev.berkeley.edu, go to
http://example-dev.berkeley.edu/cas.  You should see the CAS login
page.  When you authenticate successfully you should be returned to
your Drupal site and you should see "Logged in as YOUR NAME." 

Go to http://example-dev.berkeley.edu/user.  You should see the email
address that was retrieved from LDAP for your account.


AUTHORS
-------
Brian Wood, UC Berkeley
