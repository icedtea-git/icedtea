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
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

# List changes made in the OpenJDK tree between the old changesets and the new ones to be committed to IcedTea

TREE=$1
WORKING_DIR=$2
SCRIPT_DIR=$(dirname ${0})

if test x${TREE} = x; then
    echo "No tree specified.";
    echo "${0} <TREE> <WORKING_DIR>";
    exit -1;
fi

if test x$WORKING_DIR = x; then
    echo "Using $PWD as working directory.";
    WORKING_DIR=${PWD};
fi

if test x${TMPDIR} = x; then
    TMPDIR=/tmp;
fi

echo "Tree: ${TREE}"
echo "Working directory: ${WORKING_DIR}"

rm -f ${TMPDIR}/fixes2 ${TMPDIR}/fixes
pushd ${WORKING_DIR}

if ! git diff --quiet --no-ext-diff Makefile.am ; then
    DIFF_COMMAND="diff";
elif ! git diff --cached --quiet --no-ext-diff Makefile.am ; then
    DIFF_COMMAND="diff --cached";
fi

echo "Repository OPENJDK";
if test "x${DIFF_COMMAND}" = "x"; then
    echo "Assuming changes are committed";
    id1=$(git show HEAD:Makefile.am|grep "^OPENJDK_CHANGESET ="|head -n1|sed 's#.*=\W##');
    id2=$(git show HEAD^:Makefile.am|grep "^OPENJDK_CHANGESET ="|tail -n1|sed 's#.*=\W##');
else
    echo "Using diff command ${DIFF_COMMAND}";
    id1=$(git ${DIFF_COMMAND} Makefile.am|grep "\-OPENJDK_CHANGESET ="|head -n1|sed 's#.*=\W##');
    id2=$(git ${DIFF_COMMAND} Makefile.am|grep "\+OPENJDK_CHANGESET ="|tail -n1|sed 's#.*=\W##');
fi

echo "Found old Makefile.am OPENJDK_CHANGESET id ${id1}";
echo "Found new Makefile.am OPENJDK_CHANGESET id ${id2}";
if test "x$id1" != x -a "x$id2" != x -a "x$id1" != "x$id2"; then
    echo "Changeset changed from ${id1} to ${id2}";
    git --git-dir=${TREE}/.git log --no-merges --pretty=format:%B "${id1}...${id2}" | \
	egrep '^[0-9]{7}' | \
	sed -r 's#^([0-9])#  - JDK-\1#' >> ${TMPDIR}/fixes2;
else
    echo "No change.";
fi

HS_MAP=hotspot.map.in;
if [ ! -e ${HS_MAP} ] ; then
    HS_MAP=hotspot.map;
fi

if [ -e  ${HS_MAP} ] ; then
    if ! git diff --quiet --no-ext-diff ${HS_MAP} ; then
	DIFF_COMMAND="diff";
    elif ! git diff --cached --quiet --no-ext-diff ${HS_MAP} ; then
	DIFF_COMMAND="diff --cached";
    else
	DIFF_COMMAND="";
    fi
    HOTSPOT_BUILDS=$(grep -v '^#' ${HS_MAP} | awk '{print $1}');
    echo "HotSpot builds: ${HOTSPOT_BUILDS}"
    for builds in ${HOTSPOT_BUILDS}; do
	if test "x$builds" = "xdefault"; then name="HOTSPOT"; else name=${builds}; fi
	echo "Repository $name";
	if test "x${DIFF_COMMAND}" = "x"; then
	    echo "Assuming changes are committed";
	    hs1=$(git show HEAD:${HS_MAP}|grep "^${builds}"|awk '{print $4}');
	    hs2=$(git show HEAD^:${HS_MAP}|grep "^${builds}"|awk '{print $4}');
	else
	    echo "Using diff command ${DIFF_COMMAND}";
	    hs1=$(git ${DIFF_COMMAND} ${HS_MAP}|grep "^-${builds}"|awk '{print $4}');
	    hs2=$(git ${DIFF_COMMAND} ${HS_MAP}|grep "^+${builds}"|awk '{print $4}');
	fi
	echo "Found old ${HS_MAP} ${builds} id ${hs1}";
	echo "Found new ${HS_MAP} ${builds} id ${hs2}";
	if test "x$hs1" != x -a "x$hs2" != x -a "x$hs1" != "x$hs2"; then
	    echo "Changeset changed from ${hs1} to ${hs2}";
	    git --git-dir=${TREE}/.git log --no-merges --pretty=format:%B "${hs1}...${hs2}" | \
		egrep '^[0-9]{7}' | \
		sed -r 's#^([0-9])#  - JDK-\1#' >> ${TMPDIR}/fixes2;
	else
	    echo "No change.";
	fi;
    done
fi

sort ${TMPDIR}/fixes2 | uniq > ${TMPDIR}/fixes
rm -f ${TMPDIR}/fixes2

echo "In ${TMPDIR}/fixes:"
cat ${TMPDIR}/fixes
