#!/bin/bash

# Copyright (C) 2016 Red Hat, Inc.
# Copyright (C) 2024 Andrew John Hughes
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
HOTSPOT=$3
DOWNLOAD_URL=$4

if test "${TMPDIR}" = ""; then
    TMPDIR=/tmp;
fi

if test "${COMPRESSION_TYPE}" = ""; then
    echo "ERROR: Compression type must be specified.";
    echo "$0 <COMPRESSION_TYPE> <DOWNLOAD_DIR> <HOTSPOT> <DOWNLOAD_URL>"
    exit 1;
fi

echo "Using compression type ${COMPRESSION_TYPE}";

if test "${DOWNLOAD_DIR}" = ""; then
    echo "ERROR: Download directory must be specified.";
    echo "$0 <COMPRESSION_TYPE> <DOWNLOAD_DIR> <HOTSPOT> <DOWNLOAD_URL>";
    exit 2;
fi

if ! echo "${DOWNLOAD_DIR}" | grep -E -q 'icedtea[78]/[0-9.]*$' ; then
    echo "ERROR: Download directory must end in the form 'icedtea<major_ver>/<version number>'";
    exit 3;
fi

echo "Using download directory ${DOWNLOAD_DIR}";

if test "${DOWNLOAD_URL}" = ""; then
    DOWNLOAD_URL=https://icedtea.classpath.org/download/drops;
fi

echo "Using download URL ${DOWNLOAD_URL}";

if test "$HOTSPOT" = ""; then
    HOTSPOT=default;
fi

echo "Using HotSpot archive: $HOTSPOT"

rm -f "${TMPDIR}/changesets" "${TMPDIR}/sums" "${TMPDIR}/hotspot.map"

if test "$HOTSPOT" = "default"; then
    repo=openjdk
    file="$DOWNLOAD_DIR/${repo}-git.tar.${COMPRESSION_TYPE}"
    echo "Generating changeset and checksum for OpenJDK using ${file}"
    if [ -e "$file" ] ; then
	id=$(echo $repo|tr '[:lower:]' '[:upper:]')
	sha256sum=$(sha256sum "$file"|awk '{print $1}')
	changeset=$(tar tf "$file"|head -n1|sed -r "s#[a-z0-9-]*-([0-9a-z]*)/.*#\1#")
	name=$(echo "${DOWNLOAD_DIR}"|sed -r 's#.*(icedtea.*)#\1#'|sed 's#[78]/#-#')
	rm -vf "${DOWNLOAD_DIR}/${name}-${repo}-*-git.tar.${COMPRESSION_TYPE}"
	ln -svf "${repo}-git.tar.${COMPRESSION_TYPE}" "${DOWNLOAD_DIR}/${name}-${repo}-${changeset}-git.tar.${COMPRESSION_TYPE}"
	echo "${id}_CHANGESET = $changeset" >> "${TMPDIR}/changesets"
	echo "${id}_SHA256SUM = $sha256sum" >> "${TMPDIR}/sums"
    fi
else
    file="${DOWNLOAD_DIR}/${HOTSPOT}-git.tar.${COMPRESSION_TYPE}"
    if [ -e "${file}" ] ; then
	echo "Generating changeset and checksum for ${HOTSPOT} using ${file}"
	sha256sum=$(sha256sum "$file"|awk '{print $1}')
	name=$(echo "${DOWNLOAD_DIR}"|sed -r 's#.*(icedtea.*)#\1#'|sed 's#[78]/#-#')
	jdk_version=$(echo "${DOWNLOAD_DIR}"|sed -r 's#.*icedtea([0-9])/[0-9.]*$#\1#')
	changeset=$(tar tf "$file"|head -n1|sed -r "s#[a-z0-9-]*-([0-9a-z]*)/.*#\1#")
	rm -vf "${DOWNLOAD_DIR}/${name}-${HOTSPOT}-*-git.tar.${COMPRESSION_TYPE}"
	ln -svf "${HOTSPOT}-git.tar.${COMPRESSION_TYPE}" "${DOWNLOAD_DIR}/${name}-${HOTSPOT}-${changeset}-git.tar.${COMPRESSION_TYPE}"
	drop_url="${DOWNLOAD_URL}/icedtea${jdk_version}/@ICEDTEA_RELEASE@";
	echo "${HOTSPOT} drop ${drop_url} ${changeset} ${sha256sum}" >> "${TMPDIR}/hotspot.map"
    fi ;
fi

# Local Variables:
# compile-command: "shellcheck gen_changeset_and_sha256sums.sh"
# fill-column: 80
# indent-tabs-mode: nil
# sh-basic-offset: 4
# End:
