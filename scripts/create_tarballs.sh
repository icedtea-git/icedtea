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

CHECKOUT_DIR=$1
DOWNLOAD_DIR=$2
TAG=$3
HOTSPOT=$4
RUNNING_DIR=$(dirname $0)
DOWNLOAD_URL=http://icedtea.classpath.org/download/drops

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

if test "x$HOTSPOT" = "x" -o "x$HOTSPOT" = "xdefault"; then
    HOTSPOT=default;
    ARCHIVED_REPOS=". corba jaxp jaxws langtools hotspot jdk $NASHORN"
else
    ARCHIVED_REPOS=${HOTSPOT}
    echo "Only archiving HotSpot tree as ${HOTSPOT}"
fi

echo "TAG = $TAG";
echo "Creating new tarballs in $DOWNLOAD_DIR"
echo "Using checkout directory $CHECKOUT_DIR"
echo "Using HotSpot archive: $HOTSPOT"
URL=$(hg -R ${CHECKOUT_DIR} paths default)
echo "Upstream URL is ${URL}"
pushd $DOWNLOAD_DIR
echo "Compiling tarballs for ${ARCHIVED_REPOS}"
for repos in ${ARCHIVED_REPOS};
do
    DIRNAME=${repos}
    if test "x$repos" = "x."; then
	FILENAME=openjdk;
	PREFIX=$(echo ${URL}|sed -r 's#.*/([^/]*)$#\1#'|sed 's#\.#-#')
	echo "Prefix for root tree is ${PREFIX}"
	REPONAME=${PREFIX};
    elif test "x$repos" = "x${HOTSPOT}"; then
	FILENAME=${repos}
	REPONAME=hotspot;
	DIRNAME=hotspot;
    else
	FILENAME=${repos};
	REPONAME=${repos};
    fi
    if echo ${TAG} | egrep '^(icedtea|jdk|aarch64|tip)' > /dev/null ; then
	CHANGESET=$(hg log -r ${TAG} -R ${CHECKOUT_DIR}/${DIRNAME} | head -n1| awk -F ':' '{print $3}')
    else
	CHANGESET=${TAG}
    fi
    if test "x${CTYPE}" = "xxz"; then
	MCTYPE=tar;
    else
	MCTYPE=t${CTYPE};
	SUFFIX=.${CTYPE}
    fi
    echo "Creating ${FILENAME}.tar.${CTYPE} containing ${REPONAME}-${CHANGESET} from tag ${TAG} in ${CHECKOUT_DIR}/${repos}"
    rm -f ${FILENAME}.tar.${CTYPE}
    hg archive -R ${CHECKOUT_DIR}/${DIRNAME} -t ${MCTYPE} -r ${TAG} -p ${REPONAME}-${CHANGESET} ${FILENAME}.tar${SUFFIX}
    if test "x${CTYPE}" = "xxz"; then xz -v ${FILENAME}.tar; fi
done
echo Removing outdated symlinks
find $DOWNLOAD_DIR -maxdepth 1 -type l -exec rm -vf '{}' ';'
popd
echo Generating new changeset IDs and SHA256 sums
echo URL = ${URL}
if test "x$OPENJDK8" = "xfalse"; then
  $RUNNING_DIR/gen_changeset_and_sha256sums.sh bz2 $DOWNLOAD_DIR ${DOWNLOAD_URL} ${URL} ${HOTSPOT}
else
  $RUNNING_DIR/gen_changeset_and_sha256sums_8.sh xz $DOWNLOAD_DIR ${DOWNLOAD_URL} ${URL} ${HOTSPOT}
fi
