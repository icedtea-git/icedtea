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
    return LP64_ONLY(llvm::Type::Int64Ty)
           NOT_LP64 (llvm::Type::Int32Ty);
  }

  // VM types
 private:
  static const llvm::PointerType*  _cpCacheEntry_type;
  static const llvm::FunctionType* _method_entry_type;
  static const llvm::PointerType*  _methodOop_type;
  static const llvm::PointerType*  _oop_type;
  static const llvm::PointerType*  _thread_type;
  static const llvm::PointerType*  _zeroStack_type;
  
 public:
  static const llvm::PointerType* cpCacheEntry_type()
  {
    return _cpCacheEntry_type;
  }
  static const llvm::FunctionType* method_entry_type()
  {
    return _method_entry_type;
  }
  static const llvm::PointerType* methodOop_type()
  {
    return _methodOop_type;
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
  static const llvm::IntegerType* jbyte_type()
  {
    return llvm::Type::Int8Ty;
  }
  static const llvm::IntegerType* jchar_type()
  {
    return llvm::Type::Int16Ty;
  }
  static const llvm::IntegerType* jshort_type()
  {
    return llvm::Type::Int16Ty;
  }
  static const llvm::IntegerType* jint_type()
  {
    return llvm::Type::Int32Ty;
  }
  static const llvm::IntegerType* jlong_type()
  {
    return llvm::Type::Int64Ty;
  }
  static const llvm::Type* jfloat_type()
  {
    return llvm::Type::FloatTy;
  }
  static const llvm::Type* jdouble_type()
  {
    return llvm::Type::DoubleTy;
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
