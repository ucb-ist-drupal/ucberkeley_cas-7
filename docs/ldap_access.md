# LDAP Access
## Guest Accounts and Other LDAP OUs
By default UC Berkeley CAS can only retrieve data for users in the public LDAP 
People OU. If a user from the Guests OU (i.e. a guest account) logs in, the user 
will see `Logged in as 999999999`.  This number is the user's LDAP user id (UID). 
This section describes the steps to enable UC Berkeley CAS to gain access to the 
LDAP attributes containing the user's name and email if the user does not belong 
to a public OU.

## Request a privileged LDAP bind

Use this form to [request
a "privileged bind" from the CalNet team](https://calnetweb.berkeley.edu/calnet-technologists/ldap-directory-service/resources-developers/applying-directory-access).

## Test your privileged bind

Use a tool such as [ldapsearch](https://wikihub.berkeley.edu/x/jwRbC) to test that your privileged bind can read the needed attributes for an example user.

## Protect the bind password in your website configuration

You will receive a 'binddn' (which is analgous to a username) as well as a password from the CalNet team. 
A best practice is to avoid storing this password in your site's database. (Backups
of your database could be stored in less secure locations. Searching a database
dump will reveal this password.)
 
### Lockr 

!!! note

    Lockr is one possible solution to this problem.  This section does not constitute and endorsement of Lockr's services by UC Berkeley IST.
    
The service [Lockr](https://lockr.io) integrates well with Pantheon. To use Lockr:

1) Register for an account at [Lockr.io](https://lockr.io).
2) Install and enable the [Lockr](https://www.drupal.org/project/lockr) module.
3) Register your site to use Lockr.
```
drush @mysite.dev lockr-register my-email-registered-with-lockr@berkeley.edu --password=my-lockr-password
```
!!! note

    If your site is hosted on Pantheon and you have not yet signed up for Lockr, you can skip
    step 1 and leave off `--password=my-lockr-password` in step 3.  You will be sent a confirmation email with a 
    link to retrieve your password.

4) Install and enable the [Key](https://www.drupal.org/project/key) module on your site.
5) Configure the Key module to protect your password.
```
drush @mysite.dev key-save my-key-name 'my-key-value' --label='LDAP password for EXAMPLE OU bind' --key-provider=lockr --key-type=authentication
```

6) (Optional) Verify that the key module can retrieve your password value:
```
$ drush @mysite.dev ev "print key_get_key_value('my-key-name')"
my-key-value
```

You can do the equivalent of steps 3 and 5 via the Drupal administrative pages for the Key and Lockr modules.

7) Add additional configuration to LDAP module

At `/admin/config/people/ldap/servers/edit/ucb_prod`:

(Replace the all-caps placeholders with their values.)

* Binding Method for Searches: Service account bind
* DN for non-anonymous search: uid=MY-UID,ou=applications,dc=berkeley,dc=edu
* Password for non-anonymous search (should be a select menu): MY-KEY-NAME
  * UC Berkeley CAS already comes with a patched version of the Drupal LDAP module 
    which will work in conjunction with the Key module once Key is installed. This patch provides this
    select menu if the Key module is installed and enabled.
* Base DNs for LDAP users, groups, and other entries: On two separate lines:
  * ou=people,dc=berkeley,dc=edu
  * ou=MY-OU,dc=berkeley,dc=edu


Alternatively, this SQL updates all of the above configuration:

```
; replace MY-UID with value for your binddn
; replace MY-OU with name of the additional OU you will be searching
; replace NUM with the number of characters in ou=MY-OU,dc=berkeley,dc=edu after the MY-OU replacement
UPDATE ldap_servers set 
bind_method=1, 
bindpw=MY-KEY-NAME, 
binddn='uid=MY-UID,ou=applications,dc=berkeley,dc=edu',
basedn='a:2:{i:0;s:28:"ou=people,dc=berkeley,dc=edu";i:1;s:NUM:"ou=MY-OU,dc=berkeley,dc=edu";}' ; 
WHERE name=ldap.berkeley.edu
```


