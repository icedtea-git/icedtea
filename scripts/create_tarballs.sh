#!/bin/bash

# Copyright (C) 2014 Red Hat, Inc.
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
RUNNING_DIR=$(dirname $0)
CTYPE=bz2
echo "PWD: $PWD"
echo "COMPRESSION TYPE: ${CTYPE}"

if [ $(echo $0|grep '8$') ]; then
    echo "Assuming OpenJDK 8 and later";
    OPENJDK8=true;
    NASHORN=nashorn;
else
    echo "Assuming OpenJDK 7 and earlier";
    OPENJDK8=false;
fi

if test "x$CHECKOUT_DIR" = "x"; then
    echo "ERROR: Checkout directory must be specified";
    echo "$0 <CHECKOUT_DIR> <DOWNLOAD_DIR> <TAG>"
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
    TAG=tip ;
elif echo ${TAG}|grep '^icedtea' ; then
    if test "x$CHECKOUT_DIR" = "x"; then
	echo "No checkout directory found.";
	exit -1;
    fi
fi

echo "TAG = $TAG";
echo "Creating new tarballs in $DOWNLOAD_DIR"
echo "Using checkout directory $CHECKOUT_DIR"
URL=$(hg -R ${CHECKOUT_DIR} paths default)
PREFIX=$(echo ${URL}|sed -r 's#.*/([^/]*)$#\1#'|sed 's#\.#-#')
echo "Upstream URL is ${URL}"
echo "Prefix for root tree is ${PREFIX}"
pushd $DOWNLOAD_DIR
for repos in . corba jaxp jaxws langtools hotspot jdk $NASHORN;
do
    if test "x$repos" = "x."; then
	FILENAME=openjdk;
	REPONAME=${PREFIX};
    else
	FILENAME=${repos};
	REPONAME=${repos};
    fi
    if echo ${TAG} | egrep '^(icedtea|jdk|tip)' > /dev/null ; then
	CHANGESET=$(hg log -r ${TAG} -R ${CHECKOUT_DIR}/$repos | head -n1| awk -F ':' '{print $3}')
    else
	CHANGESET=${TAG}
    fi
    echo "Creating ${FILENAME}.tar.${CTYPE} containing ${REPONAME}-${CHANGESET} from tag ${TAG} in ${CHECKOUT_DIR}/${repos}"
    hg archive -R ${CHECKOUT_DIR}/$repos -t t${CTYPE} -r ${TAG} -p ${REPONAME}-${CHANGESET} ${FILENAME}.tar.${CTYPE}
done
echo Removing outdated symlinks
find $DOWNLOAD_DIR -maxdepth 1 -type l -exec rm -vf '{}' ';'
popd
echo Generating new changeset IDs and SHA256 sums
echo URL = ${URL}
if test "x$OPENJDK8" = "xfalse"; then
  $RUNNING_DIR/gen_changeset_and_sha256sums.sh bz2 $DOWNLOAD_DIR ${URL}
else
  $RUNNING_DIR/gen_changeset_and_sha256sums_8.sh bz2 $DOWNLOAD_DIR ${URL}
fi
