# 
# Copyright 2004-2007 Sun Microsystems, Inc.  All Rights Reserved.
# Copyright 2007 Red Hat, Inc.
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

	.global SpinPause
	.global SafeFetch32
	.global SafeFetchN

        .section ".opd","aw"
SpinPause:	.quad .SpinPause,   .TOC.@tocbase,0
SafeFetch32:	.quad .SafeFetch32, .TOC.@tocbase,0
SafeFetchN:	.quad .SafeFetchN,  .TOC.@tocbase,0
	.previous
	.global .SpinPause
	.global .SafeFetch32
	.global .SafeFetchN
		
	# int SpinPause()
.SpinPause:	
	nop
	blr	

	# int SafeFetch32(int *adr, int errValue)
.SafeFetch32:
	mr %r5, %r3
	mr %r3, %r4
	lwz %r3, 0(%r5)
	blr

	# intptr_t SafeFetchN(intptr_t *adr, intptr_t errValue)
.SafeFetchN:
	mr %r5, %r3
	mr %r3, %r4
	ld %r3, 0(%r5)
	blr


	.global _Copy_conjoint_jlongs_atomic
        .section ".opd","aw"
_Copy_conjoint_jlongs_atomic:	
	.quad ._Copy_conjoint_jlongs_atomic, .TOC.@tocbase,0
	.previous
	.global ._Copy_conjoint_jlongs_atomic

	# void _Copy_conjoint_jlongs_atomic(jlong* from,
	#                                   jlong* to, 
	#                                   size_t count)
	# if (from > to) {
	#   for (int i = 0; i < count; i++) { 
        #     to[count] = from[count];
        #   }
        # } else {
        #   while (count-- >= 0) {
        #     to[count] = from[count];
        #   }
        # }
._Copy_conjoint_jlongs_atomic:
	sldi %r5, %r5, 3
	cmpd %r3, %r4
	ble 3f
cla_LeftToRight:
	li %r6, 0
	b 2f
1:	ldx %r0, %r3, %r6
	stdx %r0, %r4, %r6
	addi %r6, %r6, 8
2:	cmpd %r6, %r5
	blt 1b
	blr
cla_RightToLeft:
	subi %r5, %r5, 8
	ldx %r0, %r3, %r5
	stdx %r0, %r4, %r5
3:	cmpdi %r5, 0
	bgt cla_RightToLeft
	blr
