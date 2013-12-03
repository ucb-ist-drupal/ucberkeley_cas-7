#! /bin/sh

# Assume ucb_cas is there already
DIR="/Users/bwood/Work/ucbdrupal/ucb_cas-7.x"
cd $DIR
if [ "$?" != 0 ]; then
  echo "Aborting: can't cd to $DIR"
  exit 1
fi
echo "Working..." 
rm -rf cas cas_attributes ldap
# --contrib-destination=. results in trying to write stuff under /tmp...
# don't know how to override drush creation of modules/ at $contrib-destination
drush make -y --no-core make/ucb_cas-7.x.cas-ldap.make 
drush make -y --no-core --contrib-destination=phpcas make/ucb_cas-7.x.phpcas.make 
rm -rf phpcas/libraries/cas/CAS-1.3.1/docs
mv phpcas/libraries/cas/CAS-1.3.1 sites/all/modules/cas/CAS
mv sites/all/modules/* .
#clean up
rm -rf sites phpcas
echo "Done." 
