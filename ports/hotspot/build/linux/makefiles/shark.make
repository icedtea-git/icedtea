#
# Copyright 1999-2005 Sun Microsystems, Inc.  All Rights Reserved.
# Copyright 2008 Red Hat, Inc.
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
#

# Sets make macros for making shark version of VM

TYPE = SHARK

VM_SUBDIR = server

CFLAGS += -DSHARK

# Something these files fail with GCC at higher optimization levels.
# An llvm::Value ends up NULL, causing segfaults in LLVM when it is
# used.  Observed with 4.1.2 20070925 (Red Hat 4.1.2-33) and 4.3.2.
OPT_CFLAGS/sharkBlock.o = -O0
OPT_CFLAGS/sharkMonitor.o = -O0

# Something in this file fails with GCC at higher optimization levels.
# The part of ciTypeFlow::StateVector::meet_exception() that fills in
# local variables stops part way through leaving the rest set to T_TOP
# (ie uninitialized).  The VM then aborts with a ShouldNotReachHere()
# in SharkPHIState::initialize().  Observed with 4.3.2.
OPT_CFLAGS/ciTypeFlow.o = -O1
