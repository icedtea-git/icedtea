/*
 * Copyright 1997-2007 Sun Microsystems, Inc.  All Rights Reserved.
 * Copyright 2007, 2008 Red Hat, Inc.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa Clara,
 * CA 95054 USA or visit www.sun.com if you need additional information or
 * have any questions.
 *
 */

// The definitions needed for zero assembly code generation.

// The zero Assembler: Pure assembler doing NO optimizations on
// the instruction level; i.e., what you write is what you get.
// The Assembler is generating code into a CodeBuffer.

class Assembler : public AbstractAssembler {
 public:
  Assembler(CodeBuffer* code) : AbstractAssembler(code) {}

  // Function to fix up forward branches
  void pd_patch_instruction(address branch, address target);
#ifndef PRODUCT
  static void pd_print_patched_instruction(address branch);
#endif // PRODUCT
};

// MacroAssembler extends Assembler by frequently used macros.
//
// Instructions for which a 'better' code sequence exists depending
// on arguments should also go in here.

class MacroAssembler : public Assembler {
 public:
  MacroAssembler(CodeBuffer* code) : Assembler(code) {}

  void align(int modulus);
  void bang_stack_with_offset(int offset);
};

#ifdef ASSERT
inline bool AbstractAssembler::pd_check_instruction_mark()
{
  Unimplemented();
}
#endif

address UnimplementedStub();
address UnimplementedEntry();
address ShouldNotReachHereStub();

// Nothing to do with the assembler (or lack of),
// just a real convenient place to include these.
#include <ffi.h>
#include <stack_zero.hpp>
