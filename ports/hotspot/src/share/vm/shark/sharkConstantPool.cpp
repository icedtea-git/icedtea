/*
 * Copyright 1999-2007 Sun Microsystems, Inc.  All Rights Reserved.
 * Copyright 2008, 2009 Red Hat, Inc.
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
#include "incls/_sharkConstantPool.cpp.incl"

using namespace llvm;

Value *SharkConstantPool::constants()
{
  Value *m = method();
  if (m != _constants_method) {
    _constants = builder()->CreateValueOfStructEntry(
      m, methodOopDesc::constants_offset(),
      SharkType::oop_type(), "constants");
    _constants_method = m;
  }
  return _constants;
}

Value *SharkConstantPool::tags()
{
  Value *cp = constants();
  if (cp != _tags_constants) {
    _tags = builder()->CreateValueOfStructEntry(
      cp, in_ByteSize(constantPoolOopDesc::tags_offset_in_bytes()),
      SharkType::oop_type(), "tags");
    _tags_constants = cp;
  }
  return _tags;
}

Value *SharkConstantPool::cache()
{
  Value *cp = constants();
  if (cp != _cache_constants) {
    _cache = builder()->CreateValueOfStructEntry(
      cp, in_ByteSize(constantPoolOopDesc::cache_offset_in_bytes()),
      SharkType::oop_type(), "cache");
    _cache_constants = cp;
  }
  return _cache;
}

Value *SharkConstantPool::object_at(int which)
{
  return builder()->CreateLoad(
    builder()->CreateArrayAddress(
      constants(),
      T_OBJECT, in_ByteSize(sizeof(constantPoolOopDesc)),
      LLVMValue::jint_constant(which)));
}

Value *SharkConstantPool::tag_at(int which)
{
  return builder()->CreateLoad(
    builder()->CreateArrayAddress(
      tags(), T_BYTE, LLVMValue::jint_constant(which)));
}

Value *SharkConstantPool::cache_entry_at(int which)
{
  // Takes a constant pool cache index in byte-swapped byte order
  // (which comes from the bytecodes after rewriting).  This is a
  // bizarre hack but it's the same as
  // constantPoolOopDesc::field_or_method_at().
  which = Bytes::swap_u2(which);
  assert(target()->holder()->is_cache_entry_resolved(which, block()->bc()),
         "should be");

  return builder()->CreateIntToPtr(
    builder()->CreateAdd(
      builder()->CreatePtrToInt(
        cache(), SharkType::intptr_type()),
      LLVMValue::intptr_constant(
        in_bytes(constantPoolCacheOopDesc::base_offset()) +
        which * sizeof(ConstantPoolCacheEntry))),
    SharkType::cpCacheEntry_type());
}

Value *SharkConstantPool::java_mirror()
{
  Value *cp = constants();

  Value *pool_holder = builder()->CreateValueOfStructEntry(
    cp,
    in_ByteSize(constantPoolOopDesc::pool_holder_offset_in_bytes()),
    SharkType::oop_type(),
    "pool_holder");

  Value *klass_part = builder()->CreateAddressOfStructEntry(
    pool_holder,
    in_ByteSize(klassOopDesc::klass_part_offset_in_bytes()),
    SharkType::klass_type(),
    "klass_part");

  // XXX should there be a memory barrier before this load?
  return builder()->CreateValueOfStructEntry(
    klass_part,
    in_ByteSize(Klass::java_mirror_offset_in_bytes()),
    SharkType::oop_type(),
    "java_mirror");
}
