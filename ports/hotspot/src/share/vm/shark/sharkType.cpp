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
#include "incls/_sharkType.cpp.incl"

using namespace llvm;

const PointerType*  SharkType::_cpCacheEntry_type;
const FunctionType* SharkType::_entry_point_type;
const PointerType*  SharkType::_itableOffsetEntry_type;
const PointerType*  SharkType::_klass_type;
const PointerType*  SharkType::_methodOop_type;
const ArrayType*    SharkType::_monitor_type;
const PointerType*  SharkType::_oop_type;
const PointerType*  SharkType::_thread_type;
const PointerType*  SharkType::_zeroStack_type;

const Type* SharkType::_to_stackType_tab[T_CONFLICT + 1];
const Type* SharkType::_to_arrayType_tab[T_CONFLICT + 1];

void SharkType::initialize()
{
  // VM types
  _cpCacheEntry_type = PointerType::getUnqual(
#if SHARK_LLVM_VERSION >= 26
    ArrayType::get(Type::getInt8Ty(getGlobalContext()), sizeof(ConstantPoolCacheEntry)));
#else
    ArrayType::get(Type::Int8Ty, sizeof(ConstantPoolCacheEntry)));
#endif

  _itableOffsetEntry_type = PointerType::getUnqual(
#if SHARK_LLVM_VERSION >= 26
    ArrayType::get(Type::getInt8Ty(getGlobalContext()), itableOffsetEntry::size() * wordSize));
#else
    ArrayType::get(Type::Int8Ty, itableOffsetEntry::size() * wordSize));
#endif

  _klass_type = PointerType::getUnqual(
#if SHARK_LLVM_VERSION >= 26
    ArrayType::get(Type::getInt8Ty(getGlobalContext()), sizeof(Klass)));
#else
    ArrayType::get(Type::Int8Ty, sizeof(Klass)));
#endif

  _methodOop_type = PointerType::getUnqual(
#if SHARK_LLVM_VERSION >= 26
    ArrayType::get(Type::getInt8Ty(getGlobalContext()), sizeof(methodOopDesc)));
#else
    ArrayType::get(Type::Int8Ty, sizeof(methodOopDesc)));
#endif

  _monitor_type = ArrayType::get(
#if SHARK_LLVM_VERSION >= 26
      Type::getInt8Ty(getGlobalContext()),
#else
      Type::Int8Ty,
#endif
      frame::interpreter_frame_monitor_size() * wordSize);

  _oop_type = PointerType::getUnqual(
#if SHARK_LLVM_VERSION >= 26
    ArrayType::get(Type::getInt8Ty(getGlobalContext()), sizeof(oopDesc)));
#else
    ArrayType::get(Type::Int8Ty, sizeof(oopDesc)));
#endif

  _thread_type = PointerType::getUnqual(
#if SHARK_LLVM_VERSION >= 26
    ArrayType::get(Type::getInt8Ty(getGlobalContext()), sizeof(JavaThread)));
#else
    ArrayType::get(Type::Int8Ty, sizeof(JavaThread)));
#endif

  _zeroStack_type = PointerType::getUnqual(
#if SHARK_LLVM_VERSION >= 26
    ArrayType::get(Type::getInt8Ty(getGlobalContext()), sizeof(ZeroStack)));
#else
    ArrayType::get(Type::Int8Ty, sizeof(ZeroStack)));
#endif

  std::vector<const Type*> params;
  params.push_back(methodOop_type());
  params.push_back(intptr_type());
  params.push_back(thread_type());
#if SHARK_LLVM_VERSION >= 26
  _entry_point_type = FunctionType::get(Type::getVoidTy(getGlobalContext()), params, false);
#else
  _entry_point_type = FunctionType::get(Type::VoidTy, params, false);
#endif

  // Java types a) on the stack and in fields, and b) in arrays
  for (int i = 0; i < T_CONFLICT + 1; i++) {
    switch (i) {
    case T_BOOLEAN:
      _to_stackType_tab[i] = jint_type();
      _to_arrayType_tab[i] = jboolean_type();
      break;
      
    case T_BYTE:
      _to_stackType_tab[i] = jint_type();
      _to_arrayType_tab[i] = jbyte_type();
      break;

    case T_CHAR:
      _to_stackType_tab[i] = jint_type();
      _to_arrayType_tab[i] = jchar_type();
      break;

    case T_SHORT:
      _to_stackType_tab[i] = jint_type();
      _to_arrayType_tab[i] = jshort_type();
      break;

    case T_INT:
      _to_stackType_tab[i] = jint_type();
      _to_arrayType_tab[i] = jint_type();
      break;

    case T_LONG:
      _to_stackType_tab[i] = jlong_type();
      _to_arrayType_tab[i] = jlong_type();
      break;

    case T_FLOAT:
      _to_stackType_tab[i] = jfloat_type();
      _to_arrayType_tab[i] = jfloat_type();
      break;

    case T_DOUBLE:
      _to_stackType_tab[i] = jdouble_type();
      _to_arrayType_tab[i] = jdouble_type();
      break;

    case T_OBJECT:
    case T_ARRAY:
      _to_stackType_tab[i] = jobject_type();
      _to_arrayType_tab[i] = jobject_type();
      break;

    case T_ADDRESS:
      _to_stackType_tab[i] = intptr_type();
      break;
    }
  }
}
