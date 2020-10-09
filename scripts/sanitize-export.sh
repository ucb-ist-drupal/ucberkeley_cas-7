#!/usr/bin/env bash
if [[ ! $PWD =~ scripts$ ]]; then
  echo "Please cd to the 'scripts' directory before running this."
  exit 1
fi
cd ..
sed -E -i '' \
-e "s/$ldap_servers_conf->bind_method = 1;/$ldap_servers_conf->bind_method = 3;/" \
-e "s/$ldap_servers_conf->binddn = '[^']+';/$ldap_servers_conf->binddn = '';/" \
-e "s/$ldap_servers_conf->bindpw = '[^']+';/$ldap_servers_conf->bindpw = '';/" \
-e "s/$strongarm->value = 'auth.berkeley.edu';/$strongarm->value = 'auth-test.berkeley.edu';/" \
-e "s/$ldap_servers_conf->testing_drupal_username = '[^']+';/$ldap_servers_conf->testing_drupal_username = '213108';/" \
-e "s/1 => 'ou=guests,dc=berkeley,dc=edu',//" \
ucberkeley_cas.features.defaultconfig.inc
# -B: write the .orig file to /tmp.  Don't want this backup in the project.
patch -B /tmp/ < patches/defaultconfig-overrides.patch


