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
    return llvm::ConstantInt::get(SharkType::jbyte_type(), value, true);
  }
  static llvm::ConstantInt* jint_constant(jint value)
  {
    return llvm::ConstantInt::get(SharkType::jint_type(), value, true);
  }
  static llvm::ConstantInt* jlong_constant(jlong value)
  {
    return llvm::ConstantInt::get(SharkType::jlong_type(), value, true);
  }
  static llvm::ConstantFP* jfloat_constant(jfloat value)
  {
    return llvm::ConstantFP::get(SharkType::jfloat_type(), value);
  }
  static llvm::ConstantFP* jdouble_constant(jdouble value)
  {
    return llvm::ConstantFP::get(SharkType::jdouble_type(), value);
  }
  static llvm::ConstantPointerNull* null()
  {
    return llvm::ConstantPointerNull::get(SharkType::jobject_type());
  }

 public:
  static llvm::ConstantInt* intptr_constant(jint value)
  {
    return llvm::ConstantInt::get(SharkType::intptr_type(), value, false);
  }
};

class SharkValue : public ResourceObj {
 protected:
  SharkValue(ciType* type, llvm::Value* value)
    : _type(type), _llvm_value(value), _zero_checked(false) {}

 private:
  ciType*      _type;
  llvm::Value* _llvm_value;
  bool         _zero_checked;

 public:
  ciType* type() const
  {
    return _type;
  }
 private:
  llvm::Value* llvm_value() const
  {
    return _llvm_value;
  }

 public:  
  BasicType basic_type() const
  {
    return type()->basic_type();
  }
  bool is_one_word() const
  {
    return type()->size() == 1;
  }
  bool is_two_word() const
  {
    return type()->size() == 2;
  }

 public:
  bool is_jint() const
  {
    return llvm_value()->getType() == SharkType::jint_type();
  }
  bool is_jlong() const
  {
    return llvm_value()->getType() == SharkType::jlong_type();
  }
  bool is_jfloat() const
  {
    return llvm_value()->getType() == SharkType::jfloat_type();
  }
  bool is_jdouble() const
  {
    return llvm_value()->getType() == SharkType::jdouble_type();
  }
  bool is_jobject() const
  {
    return llvm_value()->getType() == SharkType::jobject_type();
  }
  bool is_jarray() const
  {
    return basic_type() == T_ARRAY;
  }

  // Typed conversions to LLVM values
 public:
  llvm::Value* jint_value() const
  {
    assert(is_jint(), "should be");
    return llvm_value();
  }
  llvm::Value* jlong_value() const
  {
    assert(is_jlong(), "should be");
    return llvm_value();
  }
  llvm::Value* jfloat_value() const
  {
    assert(is_jfloat(), "should be");
    return llvm_value();
  }
  llvm::Value* jdouble_value() const
  {
    assert(is_jdouble(), "should be");
    return llvm_value();
  }
  llvm::Value* jobject_value() const
  {
    assert(is_jobject(), "should be");
    return llvm_value();
  }
  llvm::Value* jarray_value() const
  {
    assert(is_jarray(), "should be");
    return llvm_value();
  }

  // Typed conversion from LLVM values
 public:
  static SharkValue* create_jint(llvm::Value* value)
  {
    assert(value->getType() == SharkType::jint_type(), "should be");
    return create_generic(ciType::make(T_INT), value);
  }
  static SharkValue* create_jlong(llvm::Value* value)
  {
    assert(value->getType() == SharkType::jlong_type(), "should be");
    return create_generic(ciType::make(T_LONG), value);
  }
  static SharkValue* create_jfloat(llvm::Value* value)
  {
    assert(value->getType() == SharkType::jfloat_type(), "should be");
    return create_generic(ciType::make(T_FLOAT), value);
  }
  static SharkValue* create_jdouble(llvm::Value* value)
  {
    assert(value->getType() == SharkType::jdouble_type(), "should be");
    return create_generic(ciType::make(T_DOUBLE), value);
  }
  static SharkValue* create_jobject(llvm::Value* value)
  {
    assert(value->getType() == SharkType::jobject_type(), "should be");
    return create_generic(ciType::make(T_OBJECT), value);
  }

  // Typed conversion from HotSpot ciConstants
 public:
  static SharkValue* from_ciConstant(ciConstant value)
  {
    switch (value.basic_type()) {
    case T_BOOLEAN:
      return SharkValue::jint_constant(value.as_boolean());

    case T_BYTE:
      return SharkValue::jint_constant(value.as_byte());

    case T_CHAR:
      return SharkValue::jint_constant(value.as_char());

    case T_SHORT:
      return SharkValue::jint_constant(value.as_short());

    case T_INT:
      return SharkValue::jint_constant(value.as_int());

    case T_LONG:
      return SharkValue::jlong_constant(value.as_long());
      
    case T_FLOAT:
      return SharkValue::jfloat_constant(value.as_float());
      
    case T_DOUBLE:
      return SharkValue::jdouble_constant(value.as_double());
      
    case T_OBJECT:
    case T_ARRAY:
      return NULL;

    default:
      tty->print_cr("Unhandled type %s", type2name(value.basic_type()));
    }
    ShouldNotReachHere();
  }

  // Type-losing conversions, to be avoided where possible
 public:
  static SharkValue* create_generic(ciType* type, llvm::Value* value)
  {
    return new SharkValue(type, value);    
  }
  llvm::Value* generic_value() const
  {
    return llvm_value();
  }
  llvm::Value* intptr_value(llvm::IRBuilder<>* builder) const
  {
    assert(is_jobject(), "should be");
    return builder->CreatePtrToInt(llvm_value(), SharkType::intptr_type());
  }

  // Constants of various types
 public:
  static SharkValue* jint_constant(jint value)
  {
    return create_jint(LLVMValue::jint_constant(value));
  }
  static SharkValue* jlong_constant(jlong value)
  {
    return create_jlong(LLVMValue::jlong_constant(value));
  }
  static SharkValue* jfloat_constant(jfloat value)
  {
    return create_jfloat(LLVMValue::jfloat_constant(value));
  }
  static SharkValue* jdouble_constant(jdouble value)
  {
    return create_jdouble(LLVMValue::jdouble_constant(value));
  }
  static SharkValue* null()
  {
    return create_generic(ciType::make(T_OBJECT), LLVMValue::null());
  }

  // Repeated null and divide-by-zero check removal
 public:
  bool zero_checked() const
  {
    return _zero_checked;
  }
  void set_zero_checked(bool zero_checked)
  {
    _zero_checked = zero_checked;
  }
};
