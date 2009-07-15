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

class LLVMValue : public AllStatic {
 public:
  static llvm::ConstantInt* jbyte_constant(jbyte value)
  {
#if SHARK_LLVM_VERSION >= 26
    return llvm::getGlobalContext().getConstantInt(SharkType::jbyte_type(), value, true);
#else
    return llvm::ConstantInt::get(SharkType::jbyte_type(), value, true);
#endif
  }
  static llvm::ConstantInt* jint_constant(jint value)
  {
#if SHARK_LLVM_VERSION >= 26
    return llvm::getGlobalContext().getConstantInt(SharkType::jint_type(), value, true);
#else
    return llvm::ConstantInt::get(SharkType::jint_type(), value, true);
#endif
  }
  static llvm::ConstantInt* jlong_constant(jlong value)
  {
#if SHARK_LLVM_VERSION >= 26
    return llvm::getGlobalContext().getConstantInt(SharkType::jlong_type(), value, true);
#else
    return llvm::ConstantInt::get(SharkType::jlong_type(), value, true);
#endif
  }
#if SHARK_LLVM_VERSION >= 26
  static llvm::Constant* jfloat_constant(jfloat value)
  {
    return llvm::getGlobalContext().getConstantFP(SharkType::jfloat_type(), value); 
  }
#else
  static llvm::ConstantFP* jfloat_constant(jfloat value)
  {
    return llvm::ConstantFP::get(SharkType::jfloat_type(), value);
  }
#endif
#if SHARK_LLVM_VERSION >= 26
  static llvm::Constant* jdouble_constant(jdouble value)
  {
    return llvm::getGlobalContext().getConstantFP(SharkType::jdouble_type(), value);
  }
#else
  static llvm::ConstantFP* jdouble_constant(jdouble value)
  {
    return llvm::ConstantFP::get(SharkType::jdouble_type(), value);
  }
#endif
  static llvm::ConstantPointerNull* null()
  {
    return llvm::ConstantPointerNull::get(SharkType::jobject_type());
  }

 public:
  static llvm::ConstantInt* intptr_constant(intptr_t value)
  {
#if SHARK_LLVM_VERSION >= 26
    return llvm::getGlobalContext().getConstantInt(SharkType::intptr_type(), value, false);
#else
    return llvm::ConstantInt::get(SharkType::intptr_type(), value, false);
#endif
  }
};
