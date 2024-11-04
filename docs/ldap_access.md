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
 
### Pantheon Secrets

`plugins/key_provider/pantheon.inc` is a key provider for Key-7.x-3.4 which retrieves the LDAP bind secret from the
Pantheon Secrets service.

Use the Pantheon secrets terminus plugin to set the secret on your site using `--scope=web`.