# Drush Commands
## CAS Drush Command Examples
Here are some examples of CAS [drush](http://www.drush.org/en/master/) commands using the aforementioned [test accounts](index.html#user-accounts-for-testing):

Create a user.  (You need to know their UID.)
```
bwood@mbp modules$ drush cas-user-create 212372
 uid        :  5
 name       :  AFF-NORMAL TEST, Jr., ThD
 mail       :  test-212372-2@berkeley.edu
 status     :  1
 cas_name   :  212372
```

Add a role to a user. (You need to know their UID.)
```
$ drush cas-user-add-role administrator 277777
Added the administrator role to uid 2                                                                                       [success]
```

You can find a user's UID at using the [UC Berkeley Directory](http://www.berkeley.edu/directory).

## Building this module with 'drush make'

The fact that there is not a ucberkeley_cas.make file included here is
intentional.  When we build the Open Berkeley distribution we do not want the  `drush
make` command to discover a makefile in here and build it. Instead we want to use the
product of ucberkeley_cas-standalone.make.  See rebuild.sh.

