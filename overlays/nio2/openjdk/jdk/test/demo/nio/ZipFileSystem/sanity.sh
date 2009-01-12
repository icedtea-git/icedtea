#
# Copyright 2007-2008 Sun Microsystems, Inc.  All Rights Reserved.
# DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
#
# This code is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License version 2 only, as
# published by the Free Software Foundation.
#
# This code is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# version 2 for more details (a copy is included in the LICENSE file that
# accompanied this code).
#
# You should have received a copy of the GNU General Public License version
# 2 along with this work; if not, write to the Free Software Foundation,
# Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
#
# Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa Clara,
# CA 95054 USA or visit www.sun.com if you need additional information or
# have any questions.
#

# @test
# @bug 4313887
# @summary Sanity check ZipFileSystem demo
# @build Sanity
# @run shell sanity.sh

if [ -z "${TESTJAVA}" ]; then
    echo "Test must be run with jtreg"
    exit 0
fi

ZIPFS="${TESTJAVA}/demo/nio/ZipFileSystem/ZipFileSystem.jar"
if [ ! -r "${ZIPFS}" ]; then
    echo "${ZIPFS} not found"
    exit 0
fi

OS=`uname -s`
case "$OS" in
    Windows_* )
        CLASSPATH="${TESTCLASSES};${ZIPFS}"
        ;;
    * )
        CLASSPATH="${TESTCLASSES}:${ZIPFS}"
        ;;
esac
export CLASSPATH

failures=0

go() {
    echo ''
    ${TESTJAVA}/bin/java $1 $2 $3 2>&1
    if [ $? != 0 ]; then failures=`expr $failures + 1`; fi
}

# Run the tests

go Sanity  "${ZIPFS}"

#
# Results
#
echo ''
if [ $failures -gt 0 ];
  then echo "$failures test(s) failed";
  else echo "All test(s) passed"; fi
exit $failures
