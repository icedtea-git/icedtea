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

COMPRESSION_TYPE=$1
DOWNLOAD_DIR=$2
ROOT_URL=$3

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
    echo "$0 <COMPRESSION_TYPE> <DOWNLOAD_DIR> <ROOT_URL>"
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

rm -f /tmp/changesets /tmp/sums
for repos in corba jaxp jaxws jdk langtools openjdk $NASHORN
do
    file=$DOWNLOAD_DIR/$repos.tar.${COMPRESSION_TYPE}
    echo Generating changeset and checksum for $repos using ${file}
    if [ -e $file ] ; then
      id=$(echo $repos|tr '[a-z]' '[A-Z]')
      sha256sum=$(sha256sum $file|awk '{print $1}')
      changeset=$(tar tf $file|head -n1|sed -r "s#[a-z0-9-]*-([0-9a-z]*)/.*#\1#")
      name=$(echo ${DOWNLOAD_DIR}|sed -r 's#.*(icedtea.*)#\1#'|sed 's#7/#-#')
      ln -sf ${file} ${DOWNLOAD_DIR}/${name}-${repos}-${changeset}.tar.${COMPRESSION_TYPE}
      echo "${id}_CHANGESET = $changeset" >> /tmp/changesets
      echo "${id}_SHA256SUM = $sha256sum" >> /tmp/sums
    fi
done

file=${DOWNLOAD_DIR}/hotspot.tar.${COMPRESSION_TYPE}
echo Generating changeset and checksum for hotspot using ${file}
if [ -e ${file} ] ; then
  sha256sum=$(sha256sum $file|awk '{print $1}')
  name=$(echo ${DOWNLOAD_DIR}|sed -r 's#.*(icedtea.*)#\1#'|sed 's#7/#-#')
  changeset=$(tar tf $file|head -n1|sed -r "s#[a-z0-9-]*-([0-9a-z]*)/.*#\1#")
  ln -sf ${file} ${DOWNLOAD_DIR}/${name}-hotspot-$changeset.tar.${COMPRESSION_TYPE}
  echo "default ${ROOT_URL}/hotspot ${changeset} ${sha256sum}" > /tmp/hotspot.map
fi
    
