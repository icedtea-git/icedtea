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

NEWS_FILE=$1

if test "x${NEWS_FILE}" = x; then
    echo "Need edited NEWS file.";
    exit -1;
fi

BASE=$(dirname ${NEWS_FILE})
if [ -e ${BASE}/.hg ] ; then
    echo "Found Mercurial repository.";
    VCS=hg
elif [ -e ${BASE}/.git ] ; then
    echo "Found git repository.";
    VCS=git
else
    echo "No version repository found.";
    exit 1;
fi

for id in `${VCS} diff ${NEWS_FILE} |egrep '^\+  - (S|JDK-)([0-9]{7})'|sed -r 's#^\+  - (S|JDK-)([0-9]{7}).*#\2#'`;
do
    count=$(cat NEWS | grep $id |wc -l)
    if test ${count} -gt 1 ; then
	echo "${id} is duplicated (appears ${count} times)";
	cat NEWS | grep $id
    fi;
done

