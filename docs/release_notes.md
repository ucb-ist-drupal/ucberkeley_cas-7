# Release Notes
This section is for note on UC Berkeley CAS releases.  If there is no 
information here about the release you are interested in, please check 
[CHANGELOG.md](https://github.com/bwood/ucberkeley_cas-7/blob/master/CHANGELOG.md).

## ucberkeley_cas-7.x-4.0.0

The motivation for this major release was:

1. To prevent the error `PDOException: SQLSTATE[23000]: Integrity constraint violation: 1062 Duplicate entry 'Jane Smith' for key 'name': UPDATE {users} SET name=:db_update_placeholder_0, pass=:db_update_placehold`
which occurs in versions of UC Berkeley CAS prior to 4.0 when two users login to 
the site who have the same first and last names. The name field of the users table
must be unique. Prior to 4.0 UC Berkeley CAS populated the name field with 
"firstname lastname" which made displaying usernames simple. The drawback
was that this made integrity constaint violation errors possible, in rare cases.

2. Integration with the ldap_authorizations Drupal module. This module 
facilitates assigning roles to authenticated users based on LDAP attributes. 
([Open Berkeley](https://open.berkeley.edu) uses this approach to assign members of [CalGroups](https://calnetweb.berkeley.edu/calnet-technologists/calgroups-integration) to 
groups defined by the Organic Groups Drupal module.)

To make these things possible, UC Berkeley CAS 4.0 adds the [Real Name](https://www.drupal.org/project/realname) module to 
manage the display of usernames and it updates the users table to replace 
"firstname lastname" in the name field with the user's CAS user id (a unique 
number).  

### Updating to UC Berkeley CAS 4.0.0

1. Backup the code, files and database for your site to allow you to restore a 
known working state in the event that anything goes wrong when you upgrade to 
version 4.0.

2. This update should first be applied in a development or test copy of your 
website.  When you have done that and you are ready to apply it to your 
production site, consider putting the site in "maintenance mode" while you add 
the code and run the updates.

3. Remove your ucberkeley_cas directory and replace it with version 4.0 of the
code.

4. UC Berkeley CAS 4.0.0 introduces a new dependencies on the [Entity](https://www.drupal.org/project/entity) 
module. If entity is not installed and enabled on your site, please add it now.

5. Run the database updates by visiting Drupal's update.php URL while logged in 
to the site as user 1. Alternatively use `drush updb`.

6. Inspect the messages written to the screen during the update process. (Most
of these messages are also written to Drupal's log at /admin/reports/dblog --
if you have dblog enabled.) 

Step 4 is the critical part of the process. In this step the version 4.0 code 
updates all of the user records in your users table, in addition to enabling 
modules and clearing caches. 

If you have hundreds of users on your site step 4 can take 10-15 min depending 
on your server resources.  In this case you will see the update step 7404 repeat
several times as it processes batches of 20 users at a time:

```
$ drush updb 
 Ucberkeley_cas  7401  Enable Realname. Revert ucberkeley_cas feature. Revert user views.
 Ucberkeley_cas  7402  Hide the realname field on the user bundle since it duplicates field_display_name.
 Ucberkeley_cas  7403  Temporarily unset pathauto_user_pattern to speed up the big user update we are about to do.
 Ucberkeley_cas  7404  Update the users table: Replace user.name with CAS UID. Move user first and lastname to
                       field_display_name.
 Ucberkeley_cas  7405  Restore pathauto_user_pattern and update pathauto user aliases.
Do you wish to run all pending updates? (y/n): y
Performed update: ucberkeley_cas_update_7401                                                                 [ok]
Performed update: ucberkeley_cas_update_7402                                                                 [ok]
Performed update: ucberkeley_cas_update_7403                                                                 [ok]
Performed update: ucberkeley_cas_update_7404                                                                 [ok]
Performed update: ucberkeley_cas_update_7404                                                                 [ok]
Performed update: ucberkeley_cas_update_7404                                                                 [ok]
Performed update: ucberkeley_cas_update_7404                                                                 [ok]
Performed update: ucberkeley_cas_update_7404                                                                 [ok]
Performed update: ucberkeley_cas_update_7404                                                                 [ok]
Performed update: ucberkeley_cas_update_7404                                                                 [ok]
Users table update complete:                                                                                 [ok]
<pre>
New name assiged to for user@example.edu: admin
New name assiged to for 1070265: James Smith, CSM, MA
New name assiged to for 270864: Kevin Johnson, MA
New name assiged to for 1016492: Stephanie Adams
...
Performed update: ucberkeley_cas_update_7405                                                                 [ok]
'all' cache was cleared.                                                                                     [success]
Finished performing updates.                                                                                 [ok]
```

### Notes about Drupal views that display usernames
If you use the modules [Admin Views](https://www.drupal.org/project/admin_views) or [Total Control](https://www.drupal.org/project/total_control) UC Berkeley CAS 4.0 will 
make necessary alterations to these [views](https://www.drupal.org/project/views) so that user's names display correctly 
after the above updates.  See `ucberkeley_cas_views_default_views_alter()`. If 
you have custom views on your site that display user names you may need to 
implement similar modifications.

When you visit a view that displays user's names for the first time after updating 
to version 4.0, you may need to reload the page or (in some cases) clear your 
site caches (`drush cache-clear all`) to get the view to display data correctly. 
This should be a one-time fix. It's usually related to servers (like Pantheon) 
that do aggressive caching.
