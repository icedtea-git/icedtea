#!/bin/bash

# Copyright (C) 2022 Red Hat, Inc.
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

TAG=$1
TREE=$2
MAKEFILE=$3

if test "x$TAG" = "x"; then
  echo "No tag specified.";
  echo "$0 <TAG> <TREE> <MAKEFILE>"
  exit 1;
fi

if test "x$TREE" = "x"; then
  echo "No tree specified.";
  echo "$0 <TAG> <TREE> <MAKEFILE>"
  exit 2;
fi

if test "x$MAKEFILE" = "x"; then
    echo "No makefile specified. Using ${PWD}/Makefile.am";
    MAKEFILE=${PWD}/Makefile.am
fi

echo "Using changesets from $MAKEFILE..." >&2
echo "Tagging with $TAG..." >&2

if [ -d ${TREE}/.git ] ; then
    REPO_TYPE=git;
elif [ -d ${TREE}/.hg ] ; then
    REPO_TYPE=hg;
else
    echo "${TREE} does not appear to be a git or Mercurial repository.":
    exit 3;
fi

IFSBAK=${IFS}
IFS=$'\n';
for lines in `cat ${MAKEFILE}|grep '^[A-Z]*_CHANGESET = [a-z0-9]*$'`;
do
    repo=$(echo $lines|tr A-Z a-z|sed 's#_changeset.*##')
    id=$(echo $lines|sed 's#.*_CHANGESET\W=\W##')
    if test "x$repo" = "xopenjdk" ; then repo=; fi
    if test "x${REPO_TYPE}" = "xhg"; then
	echo "hg tag -R $TREE/$repo -r ${id} ${TAG}"
	hg tag -R $TREE/$repo -r ${id} ${TAG}
    else
	echo "git -C ${TREE}/${repo} tag -s -m ${TAG} ${TAG} ${id}"
	git -C ${TREE}/${repo} tag -s -m ${TAG} ${TAG} ${id}
    fi
done

IFS=${IFSBAK}
MAPFILE=$(dirname $MAKEFILE)/hotspot.map.in
if [ -e ${MAPFILE} ] && grep -q '^default' ${MAPFILE} ; then
    HSCHANGESET=$(awk 'version==$1 {print $4}' version=default ${MAPFILE})
    if test "x${REPO_TYPE}" = "xhg"; then
	echo "hg tag -R $TREE/hotspot -r ${HSCHANGESET} ${TAG}"
	hg tag -R $TREE/hotspot -r ${HSCHANGESET} ${TAG}
    else
	echo "default HotSpot found but the repository is a git repository";
	exit 4;
    fi
fi
