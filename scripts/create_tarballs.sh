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

CHECKOUT_DIR=$1
DOWNLOAD_DIR=$2
TAG=$3
HOTSPOT=$4
RUNNING_DIR=$(dirname $0)
DOWNLOAD_URL=https://icedtea.classpath.org/download/drops

echo "PWD: $PWD"
echo "DOWNLOAD URL: ${DOWNLOAD_URL}"

if [ $(echo $0|grep '8$') ]; then
    echo "Assuming OpenJDK 8 and later";
    OPENJDK8=true;
    NASHORN=nashorn;
    CTYPE=xz
else
    echo "Assuming OpenJDK 7 and earlier";
    OPENJDK8=false;
    CTYPE=bz2
fi
echo "COMPRESSION TYPE: ${CTYPE}"

if test "x$CHECKOUT_DIR" = "x"; then
    echo "ERROR: Checkout directory must be specified";
    echo "$0 <CHECKOUT_DIR> <DOWNLOAD_DIR> <TAG> <HOTSPOT>"
    exit -1;
fi

if test "x$DOWNLOAD_DIR" = "x"; then
    if test "x$OPENJDK8" = "xfalse"; then
      DOWNLOAD_DIR=/home/downloads/java/drops/icedtea7 ;
    else
      DOWNLOAD_DIR=/home/downloads/java/drops/icedtea8 ;
    fi
fi

if test "x$TAG" = "x"; then
    TAG=HEAD ;
elif echo ${TAG}|grep -q '^icedtea' ; then
    if test "x$CHECKOUT_DIR" = "x"; then
	echo "No checkout directory found.";
	exit -1;
    fi
fi

if test "x$HOTSPOT" = "x" -o "x$HOTSPOT" = "xdefault"; then
    HOTSPOT=default;
    REPONAME="."
else
    REPONAME=${HOTSPOT}
    echo "Only archiving HotSpot tree as ${HOTSPOT}"
fi

echo "TAG = $TAG";
echo "Creating new tarballs in $DOWNLOAD_DIR"
echo "Using checkout directory $CHECKOUT_DIR"
echo "Using HotSpot archive: $HOTSPOT"
pushd ${CHECKOUT_DIR}
URL=$(git remote show origin | grep 'Fetch' | cut -d ':' -f 2-)
echo "Upstream URL is ${URL}"
if [ ! -e ${DOWNLOAD_DIR} ] ; then
    echo "Creating ${DOWNLOAD_DIR}...";
    mkdir -pv ${DOWNLOAD_DIR}
fi
echo "Compiling tarballs for ${REPONAME}"
if test "x${REPONAME}" = "x."; then
    FILENAME=openjdk-git; 
    DIRNAME=${REPONAME}
    PREFIX=$(echo ${URL}|sed -e 's#/$##'|sed -e 's#\.git$##'|sed -r 's#.*/([^/]*)$#\1#'|sed 's#\.#-#')
    echo "Prefix for root tree is ${PREFIX}"
elif test "x${REPONAME}" = "x${HOTSPOT}"; then
    FILENAME=${REPONAME}-git
    DIRNAME=hotspot;
    PREFIX=hotspot;
fi
if echo ${TAG} | egrep '^(icedtea|jdk|aarch64|shenandoah|HEAD)' > /dev/null ; then
    CHANGESET=$(git show -s --format=%h ${TAG}^{commit})
else
    CHANGESET=${TAG}
fi
if test "x${CTYPE}" = "xxz"; then
    git config tar.tar.xz.command "xz -c"
elif test "x${CTYPE}" = "xbz2"; then
    git config tar.tar.bz2.command "bzip2 -c"
fi
echo "Creating ${FILENAME}.tar.${CTYPE} containing ${PREFIX}-${CHANGESET} from tag ${TAG} in ${CHECKOUT_DIR}"
rm -f ${DOWNLOAD_DIR}/${FILENAME}.tar.${CTYPE}
if test "x${DIRNAME}" = "x."; then
    git archive --prefix=${PREFIX}-${CHANGESET}/ -o ${DOWNLOAD_DIR}/${FILENAME}.tar.${CTYPE} ${TAG}
else
    echo "WARNING: Creating archive from a subtree; archive won't be reproducible due to use of current timestamp"
    git archive --prefix=${PREFIX}-${CHANGESET}/ -o ${DOWNLOAD_DIR}/${FILENAME}.tar.${CTYPE} ${TAG}:${DIRNAME}/
fi
popd
echo "Generating new changeset IDs and SHA256 sums with arguments:"
echo "\tDownload directory: ${DOWNLOAD_DIR}"
echo "\tHotSpot: ${HOTSPOT}"
echo "\tDownload URL: ${DOWNLOAD_URL}"
if test "x$OPENJDK8" = "xfalse"; then
  $RUNNING_DIR/gen_changeset_and_sha256sums.sh bz2 ${DOWNLOAD_DIR} ${HOTSPOT} ${DOWNLOAD_URL}
else
  $RUNNING_DIR/gen_changeset_and_sha256sums_8.sh xz ${DOWNLOAD_DIR} ${HOTSPOT} ${DOWNLOAD_URL}
fi
