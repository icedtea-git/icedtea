#!/bin/bash

# Copyright (C) 2016 Red Hat, Inc.
# Written by Andrew John Hughes <gnu.andrew@redhat.com>.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

COMPRESSION_TYPE=$1
DOWNLOAD_DIR=$2
DOWNLOAD_URL=$3
ROOT_URL=$4
HOTSPOT=$5

if [ $(echo $0|grep '_8') ]; then
    echo "Assuming OpenJDK 8 and later";
    OPENJDK8=true;
    NASHORN=nashorn;
else
    echo "Assuming OpenJDK 7 and earlier";
    OPENJDK8=false;
fi

if test "x$COMPRESSION_TYPE" = "x"; then
    echo "ERROR: Compression type must be specified.";
    echo "$0 <COMPRESSION_TYPE> <DOWNLOAD_DIR> <DOWNLOAD_URL> <ROOT_URL> <HOTSPOT>"
    exit -1;
fi

if test "x$DOWNLOAD_DIR" = "x"; then
    if test "x$OPENJDK8" = "xfalse"; then
      DOWNLOAD_DIR=/home/downloads/java/drops/icedtea7 ;
    else
      DOWNLOAD_DIR=/home/downloads/java/drops/icedtea8 ;
    fi
fi

if test "x$URL" = "x"; then
    if test "x$OPENJDK8" = "xfalse"; then
      URL=http://icedtea.classpath.org/hg/icedtea7-forest ;
    else
      URL=http://icedtea.classpath.org/hg/icedtea8-forest ;
    fi
fi

echo "Using URL: $ROOT_URL";
if echo ${ROOT_URL}|grep 'release' > /dev/null; then
    jdk_version=$(echo ${ROOT_URL}|sed -r 's#.*release/icedtea([0-9]).*#\1#')
    version=$(echo ${ROOT_URL}|sed -r 's#.*release/icedtea[0-9]-forest-([0-9.]*)#\1#')
    echo "OpenJDK version ${jdk_version}, release branch: ${version}";
elif echo ${ROOT_URL}|grep 'openjdk\.java\.net' > /dev/null; then
    jdk_version=$(echo ${ROOT_URL}|sed -r 's#.*jdk([0-9])u.*#\1#')
    echo "OpenJDK version ${jdk_version}";
else
    jdk_version=$(echo ${ROOT_URL}|sed -r 's#.*icedtea([0-9]).*#\1#')
    echo "OpenJDK version ${jdk_version}";
fi

if test "x$HOTSPOT" = "x"; then
    HOTSPOT=default;
fi

echo "Using HotSpot archive: $HOTSPOT"

rm -f /tmp/changesets /tmp/sums /tmp/hotspot.map

if test "x$HOTSPOT" = "xdefault"; then
    for repos in corba jaxp jaxws jdk langtools openjdk $NASHORN
    do
	file=$DOWNLOAD_DIR/$repos.tar.${COMPRESSION_TYPE}
	echo Generating changeset and checksum for $repos using ${file}
	if [ -e $file ] ; then
	    id=$(echo $repos|tr '[a-z]' '[A-Z]')
	    sha256sum=$(sha256sum $file|awk '{print $1}')
	    changeset=$(tar tf $file|head -n1|sed -r "s#[a-z0-9-]*-([0-9a-z]*)/.*#\1#")
	    name=$(echo ${DOWNLOAD_DIR}|sed -r 's#.*(icedtea.*)#\1#'|sed 's#[78]/#-#')
	    ln -sf ${repos}.tar.${COMPRESSION_TYPE} ${DOWNLOAD_DIR}/${name}-${repos}-${changeset}.tar.${COMPRESSION_TYPE}
	    echo "${id}_CHANGESET = $changeset" >> /tmp/changesets
	    echo "${id}_SHA256SUM = $sha256sum" >> /tmp/sums
	fi
    done
    hotspots=hotspot
else
    hotspots=${HOTSPOT}
fi

file=${DOWNLOAD_DIR}/${hotspots}.tar.${COMPRESSION_TYPE}
if [ -e ${file} ] ; then
    echo Generating changeset and checksum for ${hotspots} using ${file}
    sha256sum=$(sha256sum $file|awk '{print $1}')
    name=$(echo ${DOWNLOAD_DIR}|sed -r 's#.*(icedtea.*)#\1#'|sed 's#[78]/#-#')
    changeset=$(tar tf $file|head -n1|sed -r "s#[a-z0-9-]*-([0-9a-z]*)/.*#\1#")
    ln -sf ${hotspots}.tar.${COMPRESSION_TYPE} ${DOWNLOAD_DIR}/${name}-${hotspots}-${changeset}.tar.${COMPRESSION_TYPE}
    if test "x${hotspots}" = "xhotspot";
      then hsversion=default; else hsversion=${hotspots};
    fi
    drop_url="${DOWNLOAD_URL}/icedtea${jdk_version}/@ICEDTEA_RELEASE@";
    echo "${hsversion} drop ${drop_url} ${changeset} ${sha256sum}" >> /tmp/hotspot.map
fi ;

