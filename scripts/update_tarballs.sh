#!/bin/bash

# Copyright (C) 2014 Red Hat, Inc.
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

DOWNLOAD_DIR=$1
URL=$2
TAG=$3
CHECKOUT_DIR=$4
RUNNING_DIR=$(dirname "${0}")
echo "${PWD}"

if echo "${0}" | grep -q '8$' ; then
    echo "Assuming OpenJDK 8 and later";
    OPENJDK8=true;
else
    echo "Assuming OpenJDK 7 and earlier";
    OPENJDK8=false;
fi

if test "$CHECKOUT_DIR" = ""; then
    echo "No checkout directory found.";
    exit 1;
fi

if test "${DOWNLOAD_DIR}" = ""; then
    if test "${OPENJDK8}" = "false"; then
      DOWNLOAD_DIR=/home/downloads/java/drops/icedtea7 ;
    else
      DOWNLOAD_DIR=/home/downloads/java/drops/icedtea8 ;
    fi
fi

if test "${URL}" = ""; then
    if test "${OPENJDK8}" = "false"; then
      URL=http://icedtea.classpath.org/hg/icedtea7-forest ;
    else
      URL=http://icedtea.classpath.org/hg/icedtea8-forest ;
    fi
fi

if test "${TAG}" = ""; then
    TAG=tip ;
fi

echo "URL = $URL, TAG = $TAG";
echo "Downloading new tarballs to $DOWNLOAD_DIR"
pushd "${DOWNLOAD_DIR}" || exit 2
for repo in . $("${RUNNING_DIR}/discover_trees.sh" "${CHECKOUT_DIR}");
do
    if test "${repo}" = "."; then
	FILENAME=openjdk;
    else
	FILENAME=${repo};
    fi
    if echo "${TAG}" | grep -E '^(icedtea|jdk)' > /dev/null ; then
	CHANGESET=$(hg log -r "${TAG}" -R "${CHECKOUT_DIR}/${repo}" | head -n1| awk -F ':' '{print $3}')
    else
	CHANGESET=${TAG}
    fi
    wget -O "${FILENAME}.tar.gz" "${URL}/${repo}/archive/${CHANGESET}.tar.gz"
done
echo "Removing outdated symlinks"
find "${DOWNLOAD_DIR}" -maxdepth 1 -type l -exec rm -vf '{}' ';'
popd || exit 3
echo "Generating new changeset IDs and SHA256 sums"
if test "${OPENJDK8}" = "false"; then
  "$RUNNING_DIR/gen_changeset_and_sha256sums.sh" gz "${DOWNLOAD_DIR}" "${URL}"
else
  "$RUNNING_DIR/gen_changeset_and_sha256sums_8.sh" gz "${DOWNLOAD_DIR}" "${URL}"
fi

# Local Variables:
# compile-command: "shellcheck update_tarballs.sh"
# fill-column: 80
# indent-tabs-mode: nil
# sh-basic-offset: 4
# End:
