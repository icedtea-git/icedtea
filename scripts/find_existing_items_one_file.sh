#!/bin/bash

# Copyright (C) 2022 Red Hat, Inc.
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

NEWS_FILE=$1
CMD=$2

if test "${NEWS_FILE}" = ""; then
    echo "Need edited NEWS file.";
    exit 1;
fi

if test "${CMD}" = ""; then
    echo "Defaulting to diff as VCS command";
    CMD="diff";
fi

BASE=$(dirname "${NEWS_FILE}")
if [ -e "${BASE}/.hg" ] ; then
    echo "Found Mercurial repository.";
    VCS=hg
elif [ -e "${BASE}/.git" ] ; then
    echo "Found git repository.";
    VCS=git
else
    echo "No version repository found.";
    exit 2;
fi

for id in $(${VCS} ${CMD} "${NEWS_FILE}" |grep -E '^\+  - (S|JDK-)([0-9]{7})'|sed -r 's#^\+  - (S|JDK-)([0-9]{7}).*#\2#');
do
    count=$(grep -c "${id}" NEWS)
    if test "${count}" -gt 1 ; then
	echo "${id} is duplicated (appears ${count} times)";
	grep "${id}" NEWS
    fi;
done

# Local Variables:
# compile-command: "shellcheck find_existing_items_one_file.sh"
# fill-column: 80
# indent-tabs-mode: nil
# sh-basic-offset: 4
# End:
