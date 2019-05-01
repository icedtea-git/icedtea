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

DOWNLOAD_DIR=$1
URL=$2
TAG=$3
CHECKOUT_DIR=$4
RUNNING_DIR=$(dirname $0)
echo $PWD

if [ $(echo $0|grep '8$') ]; then
    echo "Assuming OpenJDK 8 and later";
    OPENJDK8=true;
    NASHORN=nashorn;
else
    echo "Assuming OpenJDK 7 and earlier";
    OPENJDK8=false;
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

if test "x$TAG" = "x"; then
    TAG=tip ;
elif echo ${TAG}|grep '^icedtea' ; then
    if test "x$CHECKOUT_DIR" = "x"; then
	echo "No checkout directory found.";
	exit -1;
    fi
fi

echo "URL = $URL, TAG = $TAG";
echo Downloading new tarballs to $DOWNLOAD_DIR
pushd $DOWNLOAD_DIR
for repos in . corba jaxp jaxws langtools hotspot jdk $NASHORN;
do
    if test "x$repos" = "x."; then
	FILENAME=openjdk;
    else
	FILENAME=$repos;
    fi
    if echo ${TAG} | egrep '^(icedtea|jdk)' > /dev/null ; then
	CHANGESET=$(hg log -r ${TAG} -R ${CHECKOUT_DIR}/$repos | head -n1| awk -F ':' '{print $3}')
    else
	CHANGESET=${TAG}
    fi
    wget -O ${FILENAME}.tar.gz ${URL}/$repos/archive/${CHANGESET}.tar.gz
done
echo Removing outdated symlinks
find $DOWNLOAD_DIR -maxdepth 1 -type l -exec rm -vf '{}' ';'
popd
echo Generating new changeset IDs and SHA256 sums
if test "x$OPENJDK8" = "xfalse"; then
  $RUNNING_DIR/gen_changeset_and_sha256sums.sh gz $DOWNLOAD_DIR ${URL}
else
  $RUNNING_DIR/gen_changeset_and_sha256sums_8.sh gz $DOWNLOAD_DIR ${URL}
fi
