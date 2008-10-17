/*
 * Copyright 1999-2007 Sun Microsystems, Inc.  All Rights Reserved.
 * Copyright 2008 Red Hat, Inc.
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
#include "incls/_sharkBytecodeTracer.cpp.incl"

using namespace llvm;

void SharkBytecodeTracer::decode(SharkBuilder*     builder,
                                 const SharkState* state,
                                 Value**           tos,
                                 Value**           tos2)
{
  if (state->stack_depth() == 0) {
    // nothing on the stack
    *tos = *tos2 = LLVMValue::intptr_constant(EMPTY_SLOT);
  }
  else if (state->stack_depth() == 1) {
    // one item on the stack
    decode_one_word(builder, state, 0, tos);
    *tos2 = LLVMValue::intptr_constant(EMPTY_SLOT);
  }
  else if (state->stack(0) == NULL) {
    // two words of a two-word type
    decode_two_word(builder, state, 0, tos, tos2);
  }
  else if (state->stack(1) == NULL) {
    // a one-word type followed by half of a two-word type
    decode_one_word(builder, state, 0, tos);
    decode_two_word(builder, state, 1, tos2, NULL);
  }
  else {
    // two one-word types
    decode_one_word(builder, state, 0, tos);
    decode_one_word(builder, state, 1, tos2);
  }
}

void SharkBytecodeTracer::decode_one_word(SharkBuilder*     builder,
                                          const SharkState* state,
                                          int               index,
                                          Value**           dst)
{
  SharkValue *value = state->stack(index);
  assert(value && value->is_one_word(), "should be");
  switch (value->basic_type()) {
  case T_BOOLEAN:
  case T_BYTE:
  case T_CHAR:
  case T_SHORT:
  case T_INT:
#ifdef _LP64
    *dst = builder->CreateIntCast(
      value->jint_value(), SharkType::intptr_type(), false);
#else
    *dst = value->jint_value();
#endif // _LP64
    break;

  case T_FLOAT:
    *dst = LLVMValue::intptr_constant(UNDECODABLE_SLOT);
    break;

  case T_OBJECT:
  case T_ARRAY:
    *dst = value->intptr_value(builder);
    break;

  case T_ADDRESS:
    *dst = LLVMValue::intptr_constant(value->returnAddress_value());
    break;

  default:
    tty->print_cr("Unhandled type %s", type2name(value->basic_type()));
    ShouldNotReachHere();
  }
}

void SharkBytecodeTracer::decode_two_word(SharkBuilder*     builder,
                                          const SharkState* state,
                                          int               index,
                                          Value**           dst,
                                          Value**           dst2)
{
  assert(state->stack(index) == NULL, "should be");
  SharkValue *value = state->stack(index + 1);
  assert(value && value->is_two_word(), "should be");

  *dst = LLVMValue::intptr_constant(UNDECODABLE_SLOT);
  if (dst2)
    *dst2 = LLVMValue::intptr_constant(UNDECODABLE_SLOT);
}
