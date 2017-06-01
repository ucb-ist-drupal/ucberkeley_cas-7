# Configuration Details
## Is it okay to modify the default configuration?
If the value does not exist in `ucberkeley_cas.strongarm.inc`, yes.  Most of 
the configuration is in `ucberkeley_cas.features.defaultconfig.inc` -- these
values can be overridden with no consequences.  

If you enter a new value in the administrative interface for a value that is 
configured in the strongarm file, your change will work, but Drupal's "features" 
module will report that the feature is in the "overridden state."

Before you change the configuration, we recommend that you review the
configurations notes below.

## CAS Configuration 

Site path: `admin/config/people/cas`.


### Logout Behavior 

By default UC Berkeley CAS creates a URL Alias (at
`admin/config/search/path`) that redirects `/user/logout` to
`/caslogout`. With this configuration in place, when a logged in user
clicks the logout link on your site they will not be able to log back
into your site until they enter their CalNet username and password
again. Using this alias is considered a best practice for UC Berkeley
Drupal sites.

The above configuration is not compatible with "single sign-on"
scenarios. Configuring a UC Berkeley Drupal site to participate in
"single sign-on" is not recommended. However, removing the above alias
will provide this behavior.

Without this alias in place if a user logs out of your site it is
possible for them to login again (while their Drupal login session is
still valid) by simply revisiting the /cas url which will not prompt
them for their password again. This configuration is considered less
secure. It opens up the possibility of unauthorized access if users
login from public computers (e.g. in a library) or if they don't lock
their screen when they leave their computer.

### Initial login destination and Logout destination 


You can configure the landing pages for the user after successful
login/logout.

### Automatically create Drupal accounts 

If you do not want Drupal to create accounts for every CalNet user who
attempts to log in to your site, go to `admin/config/people/cas`, open
the User Accounts section, and uncheck Automatically create Drupal
accounts. As an alternative, you can pre-create CAS users at
`/admin/people/cas/create`.

### Check with the CAS server to see if the user is already logged in? 

*Be careful with this setting.*

UC Berkeley CAS disables this feature by default. Enabling it in conjunction 
with "Automatically create Drupal accounts" will result in the dreaded 
"Drive-By User Creation" scenario. I.E. if you are logged into site A and you 
visit site B (B being this site with both of these settings enabled) you will 
be instantly logged in and an account will be created for you. This can result 
in lots of accounts being created on a site for people unfamiliar to the site 
administrators.  By default new users are assigned 
[the "Authenticated User" role](#the-authenticated-user-role) which does not 
have any more permissions than the Anonymous User role.

### Users cannot change password 

Unchecking this is likely to cause confusion.  Users
should change their passwords via CalNet. See [Change password
URL](#change-password-url) further down.

*Note:* Even if "users cannot change password" is enabled, users
 with the Administrator role (including User 1), or users with the
 Drupal permission 'administer users,' _can_ change the passwords on
 other accounts _in some cases_ at /admin/people.  It works like this:
 If the user being edited is associated with a CAS uid, the
 administrator will see disabled password boxes on the user form and a
 note indicating that the password for this user can't be changed
 since they are a CAS user.  If the user being edited is a regular
 Drupal user (not associated with a CAS uid) then the administrator
 will be allowed to change the user's password. Also a non-CAS user
 will be able to edit her own password, but
 [please see the section on mixing authentication modes](index.html#combining-drupal-standard-authentication-with-cas).

### Change Password URL 

This setting is blank because it can cause confusion.

The intention of ucberkeley_cas is that all users log into the site
using CalNet/CAS authentication as opposed to Drupal's standard
authentication.  Therefore changing your site password would require
changing your CalNet password (which can be done at
https://net-auth.berkeley.edu/cgi-bin/krbcpw) and would result in your
password changing for **all** CalNet authenticated applications.  A
user presented with a "change password" url might not understand the
ramifications here.

### Drupal Login Invitation 

This setting is blank because it can cause confusion.

This adds a link to your login block allowing users to login using
Drupal's standard authentication instead of CalNet.  It's best to
require ALL of your users to login via CAS and not to give them the
option of using Drupal's authentication.  If you need to allow people
who don't have a CalNet ID to login to your site, you can add a value
like "Non-UCB people login here" to this text box.

## CAS Attributes configuration 

Site path: admin/config/people/cas/attributes

### Fetch CAS Attributes 

The default setting is "only when a CAS account is created (i.e., the
first login of a CAS user)."This means that a user can edit their
Drupal profile (assuming they have permission to do so) and change the
name or email address that we found for them in LDAP.  Their edits
will not be over written by a new LDAP lookup on their next login.

