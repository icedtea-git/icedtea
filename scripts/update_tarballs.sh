#!/bin/bash

DOWNLOAD_DIR=$1
RUNNING_DIR=$(dirname $0)
echo $PWD

if test "x$DOWNLOAD_DIR" = "x"; then
    DOWNLOAD_DIR=/home/downloads/java/icedtea ;
fi

echo Downloading new tarballs to $DOWNLOAD_DIR
pushd $DOWNLOAD_DIR
for repos in corba jaxp jaxws langtools hotspot jdk;
do
	wget -O $repos.tar.gz http://icedtea.classpath.org/hg/icedtea7-forest/$repos/archive/tip.tar.gz
done
wget -O openjdk.tar.gz http://icedtea.classpath.org/hg/icedtea7-forest/archive/tip.tar.gz
echo Generating new changeset IDs and SHA256 sums
popd
$RUNNING_DIR/gen_changeset_and_sha256sums.sh $DOWNLOAD_DIR
