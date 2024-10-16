#! /bin/sh

# Example:
# sh rebuild.sh /some/directory

MAKEFILE="ucberkeley_cas-standalone.make"
AWK=${AWK:-awk}
TAR=${TAR:-tar}
LS=${LS:-ls}

echo "\nSelect your build mode.\n"
echo "  [1] Build in stand-alone mode mode. (This is what Open Berkeley uses.)"
echo "  [2] Build in development mode using ...standalone-dev.make.\n"
echo "Selection (default: 1): \c"
read SELECTION

if [ "$SELECTION" == "2" ];
then
  MAKEFILE="ucberkeley_cas-standalone-dev.make"
fi

echo "Enter the full path at which you want to build ucberkeley_cas (default: /tmp): \c"
read BUILD_DIR

if [ -z "$BUILD_DIR" ];
then
  BUILD_DIR="/tmp"
fi

if ! [ -d "$BUILD_DIR" ];
then
  echo "$BUILD_DIR is not a directory."
  exit 1
fi

echo "Building in $BUILD_DIR:\n"

# remove any old builds
if [ -d "$BUILD_DIR/ucberkeley_cas" ];
then
  rm -rf $BUILD_DIR/ucberkeley_cas
fi

drush make -y --no-core --no-cache --contrib-destination=. $MAKEFILE $BUILD_DIR/build_ucberkeley_cas
mv $BUILD_DIR/build_ucberkeley_cas/modules/* $BUILD_DIR
mv $BUILD_DIR/cas* $BUILD_DIR/ucberkeley_cas/
mv $BUILD_DIR/ldap $BUILD_DIR/ucberkeley_cas/
mv $BUILD_DIR/realname $BUILD_DIR/ucberkeley_cas/
rm -rf $BUILD_DIR/build_ucberkeley_cas
rm $BUILD_DIR/ucberkeley_cas/.gitignore

# Install pantheon-systems/customer-secrets-php-sdk
cd $BUILD_DIR/ucberkeley_cas
composer install

# Install phpCAS with composer
cd $BUILD_DIR/ucberkeley_cas/cas
mkdir composer
cd composer
composer -n require apereo/phpcas:^1.6.0

# Customize phpcas installation to work with D7 cas.
cd $BUILD_DIR/ucberkeley_cas/cas/composer/vendor/apereo/phpcas/source
# Rename the default entry point.
mv CAS.php phpCAS.php
# Add our entry point which requires composer's autoload.php and then calls the renamed entry point.
cp $BUILD_DIR/ucberkeley_cas/patches/files/CAS.php .

# remove drush datestamps from info files which result in unnecessary modifications.
cd $BUILD_DIR
find ucberkeley_cas -name "*.info" -print0 |xargs -0 sed -i.rebuild-bak -e '/datestamp = /d' -e 's/Information added by drush on [[:digit:]]\{4\}-[[:digit:]]\{2\}-[[:digit:]]\{2\}/Information added by drush/'
find ucberkeley_cas -name "*.rebuild-bak" | xargs rm -f

VER=`$AWK -F = '/version =.*$/{gsub(/ /, "", $0); print $2}' ucberkeley_cas/ucberkeley_cas.info`
echo ""
while [[ ! "$CONFIRM" == "y" ]] && [[ ! "$CONFIRM" == "n" ]]; do
  echo "Create this tarball: ucberkeley_cas-$VER.tar.gz? (y/n)"
  read CONFIRM
done

if [ "$CONFIRM" == "y" ];then
  $TAR zcf ucberkeley_cas-$VER.tar.gz ucberkeley_cas
else
  echo "Okay, no tarball."
fi

echo ""
echo "$LS $BUILD_DIR/ucberkeley_cas* :"
echo ""
$LS $BUILD_DIR/ucberkeley_cas*

