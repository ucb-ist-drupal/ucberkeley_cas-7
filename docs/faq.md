# Frequently Asked Questions
## When will there be a Drupal 8 version of UC Berkeley CAS?
UC Berkeley CAS was developed for the Open Berkeley Drupal distribution.  UC 
Berkeley CAS 8.x will be created when Open Berkeley is upgraded to Drupal 8.x. 
Open Berkeley depends on > 100 Drupal modules many of which have not yet be upgraded 
to Drupal 8. It may be a while.

If you are interested in developing a 8.x version of this feature to be shared with 
the campus Drupal community, let us know at <web-platform@berkeley.edu> and we will do what we can to support this 
effort.
## How can I be alerted when there is an new release of UC Berkeley CAS and how can I keep up with new developments? 

Subscribe to [ucberkeley-cas-drupal-users@lists.berkeley.edu](https://calmail.berkeley.edu/manage/list/listinfo/ucberkeley-cas-drupal-users@lists.berkeley.edu).

You can also monitor this repository using Sibbell.com. Just as you might want 
to know immediately when a new release is issued for ucberkeley\_cas and 
ucberkeley\_envconf, the Open Berkeley team also needs to be alerted about 
releases for a number of GitHub-hosted projects. We’ve recently began 
monitoring GitHub releases using Sibbell.com (free) and we have been pleased 
with their service thus far. [More info on Sibbell here](http://www.davegaeddert.com/2014/10/11/sibbell-emails-for-new-releases-on-github/).

## When I login to my site I don't see my user's real name. I see "Logged in as 999999"

By default UC Berkeley CAS is configured to make an "anonymous bind" (an 
authenticated LDAP connection) to the UC Berkeley LDAP server. The anonymous 
LDAP user can only retrieve data (in this case, name and email) for users in the 
[People OU](https://calnetweb.berkeley.edu/calnet-technologists/ldap-directory-service/how-ldap-organized).
The user that has just authenticated might belong to a OU other than People, for 
example it may belong to the Guests OU. If your webapplication will need to 
lookup data for users in a non-public OU, you will need to [request
a "privileged bind" from the CalNet team](https://calnetweb.berkeley.edu/calnet-technologists/ldap-directory-service/resources-developers/applying-directory-access).
Then you will need to configure your bind information at `/admin/config/ldap`.
For more information see the [LDAP Access section](ldap_access.html).

## When logging in I see the error "user warning: Duplicate entry" 

This results in errors like these:
```
user warning: Duplicate entry
'Brian Wood' for key 'name' query: UPDATE users SET name = 'Brian
Wood', mail = 'bwood@example.com', data = 'a:0:{}' WHERE uid = 7 in
/Users/bwood/Sites/dev6/modules/user/user.module on line 248."
```
```
PDOException: SQLSTATE[23000]: Integrity constraint violation: 1062 Duplicate entry 'Brian Wood' for key 'name': UPDATE {users} SET name=:db_update_placeholder_0,
pass=:db_update_placehold
```

As of UC Berkeley CAS 3.0.0 you may encounter this problem if two users with 
the exact same first name and last name authenticate to your site. The Drupal 
username is constucted by appending the user's lastname to their first name. 
This is not guaranteed to be unique.

As of version 3.0.0, if two users named "John Smith" login to your sitelogin as 
an administrator user and edit the username of the first user so that it will 
be different from then full name of the second user.  E.g. Change the first 
users username to "John A. Smith." Once this is done, the second John Smith should 
be able to login. 

(A fix for this has been prioritized.)

This can also happen if you allowed Drupal to uninstall the CAS module. See 
[Avoid uninstalling the CAS module](index.html#avoid-uninstalling-the-cas-module).

## I get a "Not Found" when I try to login at user/admin_login 

This can happen if your site is not using clean urls.  Try accessing the administrator back door at http://example.com/?q=/user/admin_login. (Consider enabling clean urls at /?q=admin/config/search/clean-urls.)


## I notice that there are upgrades available for some of these modules. Is it safe to upgrade them? 

There are 2 kinds of releases for modules: Feature release (yellow on your 
Available Updates page) and Security release (red on your Available Updates 
page).  We *usually* upgrade ucberkeley_cas when there are  security releases. 
(The exception to this is: if a security release covers a condition that is mitigated in the 
default configuration of ucberkeley_cas (e.g. a vulnerability in the CAS Server
module which is not enabled by default) we may decline to upgrade this code.) 
We only update the ucberkeley_cas with feature releases when we need the new 
functionality that they provide. 

It’s safest to leave the modules at the versions we deliver in a release.  The 
ucberkeley_cas feature has not been tested with the later versions of these 
modules.  We can’t guarantee that everything will work.

## Why isn't ucberkeley\_cas hosted on http://drupal.org?

Two reasons: 
1. This module is specific to using Druapl at UC Berkeley and is not useful to the wider Drupal community.
2. This module bundles phpCAS which cannot be served from drupal.org for licensing reasons. 


## Lots of user accounts are being created on my site and I don't know why. 
[See this section](configuration/#check-with-the-cas-server-to-see-if-the-user-is-already-logged-in).

## Why do I sometimes get incorrectly bounced to the CalNet login page when I visit my site's homepage? 

Steps like these repeat the problem:

1. From Pantheon dashboard of site click "Visit \[Example site\]" and see the site homepage.
2. Select all and copy the location bar (url)
3. Reload the page and see the CAS login page (incorrect)
4. Cmd-L, Cmd-A, Cmd-V (replace the location bar with the site url) and click enter. See the site homepage
5. Reload the page and see the CAS login page (incorrect)

The cause of this is a bad cookie in your browser. Here are the steps to fix this in Chrome. (Similar steps should fix other browsers.):

In Chrome goto Settings > Show Advanced > Privacy > Clear Browsing Data and remove all cookies in last 4 weeks (or since beginning of time).

## Why can't I upgrade ucberkeley\_cas using a command like 'drush pm-updatecode' (upc)? 

A. For that to work the ucberkeley\_cas module would need to be hosted on http://drupal.org or another site that interfaces with this drupal update process.  


## This module require ldap\_servers, but that doesn't seem to be a module that exists on http://drupal.org. 

A. ldap\_servers is bundled in the module called LDAP. Sometimes this causes drush commands to be confused about what module to download.  You may need to download the LDAP module manually. See the [Requirements](#requirements) section for the specific version of LDAP that ucberkeley\_cas requires.  All of the releases of LDAP can be found [here](https://drupal.org/node/806060/release).

## When I installed ucberkely\_cas I got the message: _Module ucberkeley\_cas cannot be enabled because it depends on ldap\_servers (7.x-1.0-beta12) but 1.0-beta11 is available_ 

A. Check to see if you have another version of LDAP installed under /sites/all/modules or /profiles.  If so, remove this folder.  If find LDAP under the folder ucb\_cas, you should read about [upgrading from ucb\_cas 1.0](#1.x\_2.x).

## When I try to edit a user created by the cas module, I get a validation error on the email address. Why is this? 

A. All accounts on a Drupal site must have unique email addresses.
Often a site admin user their own address for User 1 and then they
CalNet authenticate to create a new account for themselves.  The
account gets created, but if they try to edit it, they get a
validation error on the email field since it is the email that is
already in use by User 1. To fix this, change the User 1 email.

## Why does the command 'drush @somealias vget cas\_server' retrun the wrong information? 

(This only applies to sites using the ucberkeley\_envconf module.)

Because the ucberkeley\_envconf module applies configuration on
hook\_boot() and because hook\_boot doesn't run when you issue 'drush
vget', you will encounter situations where 'drush vget' reports the
wrong value.  If you visit the corresponding admin page, you should
see the right value.

Theoretically you could get the correct value with 

drush @somealias php-eval "echo variable\_get('cas\_server', NULL);"

## When trying to use Libraries API with ucberkeley_cas I got a blank white screen. 

See https://drupal.org/node/1394666#comment-8886961

("WSOD" = "White Screen of Death")

## When trying to use UC Berkeley CAS from my local development environment (e.g. my laptop) I see "Authentication failure: Ticket not validated Reason: no response from the CAS server"

Full error:
```
CAS_AuthenticationException: CAS URL:
https://auth-test.berkeley.edu/cas/p3/serviceValidate?service=http%3A%2F%2Fopenucb-1540-drupal-plain.localhost%2Fcas&ticket=ST-4019-p0onbvnIaLMMVszxMYcD-auth-t1
Authentication failure: Ticket not validated Reason: no response from
the CAS server in CAS_Client->validateCAS20() (line 3195 of
/Users/bwood/Sites/other/openucb-1540-drupal-plain/sites/all/modules/ucberkeley_cas/cas/CAS/source/CAS/Client.php).
```

To fix this:

Download cacert.pem at http://curl.haxx.se/docs/caextract.html

Save that file in `/usr/local/share/certs` (or the path of your choice) on your laptop.
 
Run this drush command on your local website:

```
$ drush vset cas_cert '/usr/local/share/certs/cacert.pem'
cas_cert was set to "/usr/local/share/certs/cacert.pem".                                                                    [success]
```

Try authenticating now.
