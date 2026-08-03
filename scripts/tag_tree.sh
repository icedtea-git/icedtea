#!/bin/bash

# Copyright (C) 2026 Red Hat, Inc.
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

if test "$TAG" = ""; then
  echo "No tag specified.";
  echo "$0 <TAG> <TREE> <MAKEFILE>"
  exit 1;
fi

if test "$TREE" = ""; then
  echo "No tree specified.";
  echo "$0 <TAG> <TREE> <MAKEFILE>"
  exit 2;
fi

if test "$MAKEFILE" = ""; then
    echo "No makefile specified. Using ${PWD}/Makefile.am";
    MAKEFILE=${PWD}/Makefile.am
fi

echo "Using changesets from $MAKEFILE..." >&2
echo "Tagging with $TAG..." >&2

if [ -d "${TREE}/.git" ] ; then
    REPO_TYPE=git;
elif [ -d "${TREE}/.hg" ] ; then
    REPO_TYPE=hg;
else
    echo "${TREE} does not appear to be a git or Mercurial repository.":
    exit 3;
fi

grep '^[A-Z]*_CHANGESET = [a-z0-9]*$' "${MAKEFILE}" | while IFS= read -r line
do
    repo=$(echo "$line"|tr '[:upper:]' '[:lower:]'|sed 's#_changeset.*##')
    id=${line##*CHANGESET[[:space:]]=[[:space:]]}
    if test "$repo" = "openjdk" ; then repo=; fi
    if test "${REPO_TYPE}" = "hg"; then
	echo "hg tag -R $TREE/$repo -r ${id} ${TAG}"
	hg tag -R "$TREE/$repo" -r "${id}" "${TAG}"
    else
	echo "git -C ${TREE}/${repo} tag -s -m ${TAG} ${TAG} ${id}"
	git -C "${TREE}/${repo}" tag -s -m "${TAG}" "${TAG}" "${id}"
    fi
done

HSMAP=$(dirname "${MAKEFILE}")/hotspot.map.in
if [ -e "${HSMAP}" ] && grep -q '^default' "${HSMAP}" ; then
    HSCHANGESET=$(awk 'version==$1 {print $4}' version=default "${HSMAP}")
    if test "${REPO_TYPE}" = "hg"; then
	echo "hg tag -R $TREE/hotspot -r ${HSCHANGESET} ${TAG}"
	hg tag -R "${TREE}/hotspot" -r "${HSCHANGESET}" "${TAG}"
    else
	echo "default HotSpot found but the repository is a git repository";
	exit 4;
    fi
fi

# Local Variables:
# compile-command: "shellcheck tag_tree.sh"
# fill-column: 80
# indent-tabs-mode: nil
# sh-basic-offset: 4
# End:
