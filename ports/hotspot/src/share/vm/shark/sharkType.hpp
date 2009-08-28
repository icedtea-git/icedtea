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

class SharkType : public AllStatic {
 public:
  static void initialize();

  // C types
 public:
  static const llvm::IntegerType* intptr_type()
  {
#if SHARK_LLVM_VERSION >= 26
    return LP64_ONLY(llvm::Type::getInt64Ty(llvm::getGlobalContext()))
           NOT_LP64 (llvm::Type::getInt32Ty(llvm::getGlobalContext()));
#else
    return LP64_ONLY(llvm::Type::Int64Ty)
           NOT_LP64 (llvm::Type::Int32Ty);
#endif
  }

  // VM types
 private:
  static const llvm::PointerType*  _cpCacheEntry_type;
  static const llvm::FunctionType* _entry_point_type;
  static const llvm::FunctionType* _osr_entry_point_type;
  static const llvm::PointerType*  _itableOffsetEntry_type;
  static const llvm::PointerType*  _klass_type;
  static const llvm::PointerType*  _methodOop_type;
  static const llvm::ArrayType*    _monitor_type;
  static const llvm::PointerType*  _oop_type;
  static const llvm::PointerType*  _thread_type;
  static const llvm::PointerType*  _zeroStack_type;
  
 public:
  static const llvm::PointerType* cpCacheEntry_type()
  {
    return _cpCacheEntry_type;
  }
  static const llvm::FunctionType* entry_point_type()
  {
    return _entry_point_type;
  }
  static const llvm::FunctionType* osr_entry_point_type()
  {
    return _osr_entry_point_type;
  }
  static const llvm::PointerType* itableOffsetEntry_type()
  {
    return _itableOffsetEntry_type;
  }
  static const llvm::PointerType* klass_type()
  {
    return _klass_type;
  }
  static const llvm::PointerType* methodOop_type()
  {
    return _methodOop_type;
  }
  static const llvm::ArrayType* monitor_type()
  {
    return _monitor_type;
  }
  static const llvm::PointerType* oop_type()
  {
    return _oop_type;
  }
  static const llvm::PointerType* thread_type()
  {
    return _thread_type;
  }
  static const llvm::PointerType* zeroStack_type()
  {
    return _zeroStack_type;
  }

  // Java types
 public:
  static const llvm::IntegerType* jboolean_type()
  {
#if SHARK_LLVM_VERSION >= 26
    return llvm::Type::getInt8Ty(llvm::getGlobalContext());
#else
    return llvm::Type::Int8Ty;
#endif
  }
  static const llvm::IntegerType* jbyte_type()
  {
#if SHARK_LLVM_VERSION >= 26
    return llvm::Type::getInt8Ty(llvm::getGlobalContext());
#else
    return llvm::Type::Int8Ty;
#endif
  }
  static const llvm::IntegerType* jchar_type()
  {
#if SHARK_LLVM_VERSION >= 26
    return llvm::Type::getInt16Ty(llvm::getGlobalContext());
#else
    return llvm::Type::Int16Ty;
#endif
  }
  static const llvm::IntegerType* jshort_type()
  {
#if SHARK_LLVM_VERSION >= 26
    return llvm::Type::getInt16Ty(llvm::getGlobalContext());
#else
    return llvm::Type::Int16Ty;
#endif
  }
  static const llvm::IntegerType* jint_type()
  {
#if SHARK_LLVM_VERSION >= 26
    return llvm::Type::getInt32Ty(llvm::getGlobalContext());
#else
    return llvm::Type::Int32Ty;
#endif
  }
  static const llvm::IntegerType* jlong_type()
  {
#if SHARK_LLVM_VERSION >= 26
    return llvm::Type::getInt64Ty(llvm::getGlobalContext());
#else
    return llvm::Type::Int64Ty;
#endif
  }
  static const llvm::Type* jfloat_type()
  {
#if SHARK_LLVM_VERSION >= 26
    return llvm::Type::getFloatTy(llvm::getGlobalContext());
#else
    return llvm::Type::FloatTy;
#endif
  }
  static const llvm::Type* jdouble_type()
  {
#if SHARK_LLVM_VERSION >= 26
    return llvm::Type::getDoubleTy(llvm::getGlobalContext());
#else
    return llvm::Type::DoubleTy;
#endif
  }
  static const llvm::PointerType* jobject_type()
  {
    return oop_type();
  }

  // Java types as they appear on the stack and in fields
 private:
  static const llvm::Type* _to_stackType_tab[T_CONFLICT + 1];

 public:
  static const llvm::Type* to_stackType(BasicType type)
  {
#ifdef ASSERT
    if (type < 0 || type > T_CONFLICT || _to_stackType_tab[type] == NULL) {
      tty->print_cr("Unhandled type %s", type2name(type));
      ShouldNotReachHere();
    }
#endif // ASSERT
    return _to_stackType_tab[type];
  }
  static const llvm::Type* to_stackType(ciType* type)
  {
    return to_stackType(type->basic_type());
  }

  // Java types as they appear in arrays
 private:
  static const llvm::Type* _to_arrayType_tab[T_CONFLICT + 1];

 public:
  static const llvm::Type* to_arrayType(BasicType type)
  {
#ifdef ASSERT
    if (type < 0 || type > T_CONFLICT || _to_arrayType_tab[type] == NULL) {
      tty->print_cr("Unhandled type %s", type2name(type));
      ShouldNotReachHere();
    }
#endif // ASSERT
    return _to_arrayType_tab[type];
  }
  static const llvm::Type* to_arrayType(ciType* type)
  {
    return to_arrayType(type->basic_type());
  }
};
