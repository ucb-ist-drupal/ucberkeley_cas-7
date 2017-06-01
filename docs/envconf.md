# The UC Berkeley Environment Configurations Module 

[Download the latest version of UC Berkeley Environment Configurations](https://github.com/ucb-ist-drupal/ucberkeley_envconf-7/releases).

The module
[UC Berkeley Environment Configurations](https://github.com/ucb-ist-drupal/ucb_envconf-7)
ensures that your cas and ldap server settings are correct based on
your development environment on [Pantheon](http://pantheon.io). (If
you are not hosting your site on Pantheon, you don't need this
module.) UC Berkeley Environment Configurations ensures that your Dev
and Test sites on Pantheon use:

* CAS Server: auth-test.berkeley.edu

and your Live site uses: 

* CAS Server: auth.berkeley.edu

(The production LDAP server is intentionally used in all environments.)

If you are not using this module, you'll need to manually edit these
server settings when you copy your database between Pantheon's Dev, Test and
Live environments. To manage this manually make these changes at:

* admin/config/people/cas
* admin/config/people/cas/attributes

