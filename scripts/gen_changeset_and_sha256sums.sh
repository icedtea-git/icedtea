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
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

COMPRESSION_TYPE=$1
DOWNLOAD_DIR=$2
DOWNLOAD_URL=$3
ROOT_URL=$4
HOTSPOT=$5

if test "x${TMPDIR}" = "x"; then
    TMPDIR=/tmp;
fi

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
      URL=https://icedtea.classpath.org/hg/icedtea7-forest ;
    else
      URL=https://icedtea.classpath.org/hg/icedtea8-forest ;
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

rm -f ${TMPDIR}/changesets ${TMPDIR}/sums ${TMPDIR}/hotspot.map

if test "x$HOTSPOT" = "xdefault"; then
    repo=openjdk
    file=$DOWNLOAD_DIR/${repo}-git.tar.${COMPRESSION_TYPE}
    echo Generating changeset and checksum for OpenJDK using ${file}
    if [ -e $file ] ; then
	id=$(echo $repo|tr '[a-z]' '[A-Z]')
	sha256sum=$(sha256sum $file|awk '{print $1}')
	changeset=$(tar tf $file|head -n1|sed -r "s#[a-z0-9-]*-([0-9a-z]*)/.*#\1#")
	name=$(echo ${DOWNLOAD_DIR}|sed -r 's#.*(icedtea.*)#\1#'|sed 's#[78]/#-#')
	rm -vf ${DOWNLOAD_DIR}/${name}-${repo}-*-git.tar.${COMPRESSION_TYPE}
	ln -svf ${repo}-git.tar.${COMPRESSION_TYPE} ${DOWNLOAD_DIR}/${name}-${repo}-${changeset}-git.tar.${COMPRESSION_TYPE}
	echo "${id}_CHANGESET = $changeset" >> ${TMPDIR}/changesets
	echo "${id}_SHA256SUM = $sha256sum" >> ${TMPDIR}/sums
    fi
else
    file=${DOWNLOAD_DIR}/${HOTSPOT}-git.tar.${COMPRESSION_TYPE}
    if [ -e ${file} ] ; then
	echo Generating changeset and checksum for ${HOTSPOT} using ${file}
	sha256sum=$(sha256sum $file|awk '{print $1}')
	name=$(echo ${DOWNLOAD_DIR}|sed -r 's#.*(icedtea.*)#\1#'|sed 's#[78]/#-#')
	changeset=$(tar tf $file|head -n1|sed -r "s#[a-z0-9-]*-([0-9a-z]*)/.*#\1#")
	rm -vf ${DOWNLOAD_DIR}/${name}-${HOTSPOT}-*-git.tar.${COMPRESSION_TYPE}
	ln -svf ${HOTSPOT}-git.tar.${COMPRESSION_TYPE} ${DOWNLOAD_DIR}/${name}-${HOTSPOT}-${changeset}-git.tar.${COMPRESSION_TYPE}
	drop_url="${DOWNLOAD_URL}/icedtea${jdk_version}/@ICEDTEA_RELEASE@";
	echo "${HOTSPOT} drop ${drop_url} ${changeset} ${sha256sum}" >> ${TMPDIR}/hotspot.map
    fi ;
fi

