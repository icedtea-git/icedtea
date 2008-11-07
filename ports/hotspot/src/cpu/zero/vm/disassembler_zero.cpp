/*
 * Copyright 1997-2007 Sun Microsystems, Inc.  All Rights Reserved.
 * Copyright 2007 Red Hat, Inc.
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
#include "incls/_disassembler_zero.cpp.incl"

#ifndef PRODUCT
void Disassembler::decode(CodeBlob *cb, outputStream *st)
{
  Unimplemented();
}

void Disassembler::decode(nmethod *nm, outputStream *st)
{
#ifdef SHARK
  assert(st == NULL, "it's all going to stderr anyway");
  ((SharkEntry *) nm->instructions_begin())->llvm_function()->dump();
#else
  Unimplemented();
#endif // SHARK
}

void Disassembler::decode(u_char *begin, u_char *end, outputStream *st)
{
  Unimplemented();
}
#endif // PRODUCT

