/*
 * Copyright 2003-2007 Sun Microsystems, Inc.  All Rights Reserved.
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

#include "incls/_precompiled.incl"
#include "incls/_assembler_zero.cpp.incl"

int AbstractAssembler::code_fill_byte()
{
  return 0;
}

void Assembler::pd_patch_instruction(address branch, address target)
{
  Unimplemented();
}

#ifndef PRODUCT
void Assembler::pd_print_patched_instruction(address branch)
{
  Unimplemented();
}
#endif // PRODUCT

void MacroAssembler::align(int modulus)
{
  while (offset() % modulus != 0)
    emit_byte(AbstractAssembler::code_fill_byte());
}

void MacroAssembler::bang_stack_with_offset(int offset)
{
  Unimplemented();
}

void MacroAssembler::advance(int bytes)
{
  _code_pos += bytes;
  sync();
}

static void _UnimplementedStub()
{
  report_unimplemented(__FILE__, __LINE__);
}

address UnimplementedStub()
{
  return (address) _UnimplementedStub;
}

address UnimplementedEntry()
{
  return (address) _UnimplementedStub;
}

static void _ShouldNotReachHereStub()
{
  report_should_not_reach_here(__FILE__, __LINE__);
}

address ShouldNotReachHereStub()
{
  return (address) _ShouldNotReachHereStub;
}
