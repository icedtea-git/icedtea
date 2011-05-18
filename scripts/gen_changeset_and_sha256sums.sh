#!/bin/bash

DOWNLOAD_DIR=$1

if test "x$DOWNLOAD_DIR" = "x"; then
    DOWNLOAD_DIR=/home/downloads/java/icedtea ;
fi

rm -f /tmp/changesets /tmp/sums
for repos in corba hotspot jaxp jaxws jdk langtools openjdk
do
    echo Generating changeset and checksum for $repos using $DOWNLOAD_DIR/$repos.tar.gz
    file=$DOWNLOAD_DIR/$repos.tar.gz
    id=$(echo $repos|tr '[a-z]' '[A-Z]')
    sha256sum=$(sha256sum $file|awk '{print $1}')
    changeset=$(tar tzf $file|head -n1|sed -r "s#[a-z0-9-]*-([0-9a-z]*)/.*#\1#")
    echo "${id}_CHANGESET = $changeset" >> /tmp/changesets
    echo "${id}_SHA256SUM = $sha256sum" >> /tmp/sums
done
