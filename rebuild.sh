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
mv $BUILD_DIR/build_ucberkeley_cas/libraries/phpcas $BUILD_DIR/ucberkeley_cas/cas/CAS
rm -rf $BUILD_DIR/build_ucberkeley_cas
rm $BUILD_DIR/ucberkeley_cas/.gitignore
cd $BUILD_DIR
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

