#! /bin/sh

# Example:
# sh ucberkeley_cas-build-standalone.sh /some/directory

if [ -d "$@" ];
then
  BUILD_DIR=$@
else
  BUILD_DIR=/tmp
fi

echo "Building in $BUILD_DIR:\n"

rm -rf $BUILD_DIR/ucberkeley_cas
drush make -y --no-core --contrib-destination=. ucberkeley_cas-7.x.make $BUILD_DIR/build_ucberkeley_cas
mv $BUILD_DIR/build_ucberkeley_cas/modules/* $BUILD_DIR
mv $BUILD_DIR/cas* $BUILD_DIR/ucberkeley_cas/
mv $BUILD_DIR/ldap $BUILD_DIR/ucberkeley_cas/
mv $BUILD_DIR/build_ucberkeley_cas/libraries/phpcas/CAS* $BUILD_DIR/ucberkeley_cas/cas/CAS
rm -rf $BUILD_DIR/build_ucberkeley_cas


