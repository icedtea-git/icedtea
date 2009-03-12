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

class SharkValue : public ResourceObj {
 protected:
  SharkValue() {}

  // Type access
 public:
  virtual ciType* type() const
  {
    ShouldNotCallThis();
  }

  virtual BasicType basic_type() const
  {
    ShouldNotCallThis();
  }
  virtual int size() const
  {
    ShouldNotCallThis();
  }
  bool is_one_word() const
  {
    return size() == 1;
  }
  bool is_two_word() const
  {
    return size() == 2;
  }

  virtual bool is_jint() const
  {
    return false;
  }
  virtual bool is_jlong() const
  {
    return false;
  }
  virtual bool is_jfloat() const
  {
    return false;
  }
  virtual bool is_jdouble() const
  {
    return false;
  }
  virtual bool is_jobject() const
  {
    return false;
  }
  virtual bool is_jarray() const
  {
    return false;
  }
  virtual bool is_returnAddress() const
  {
    return false;
  }

  // Typed conversions to LLVM values
 public:
  virtual llvm::Value* jint_value() const
  {
    ShouldNotCallThis();
  }
  virtual llvm::Value* jlong_value() const
  {
    ShouldNotCallThis();
  }
  virtual llvm::Value* jfloat_value() const
  {
    ShouldNotCallThis();
  }
  virtual llvm::Value* jdouble_value() const
  {
    ShouldNotCallThis();
  }
  virtual llvm::Value* jobject_value() const
  {
    ShouldNotCallThis();
  }
  virtual llvm::Value* jarray_value() const
  {
    ShouldNotCallThis();
  }
  virtual int returnAddress_value() const
  {
    ShouldNotCallThis();
  }
  
  // Constants of various types
 public:
  static SharkValue* jint_constant(jint value)
  {
    SharkValue *result = create_jint(LLVMValue::jint_constant(value));
    if (value != 0)
      result->set_zero_checked(true);
    return result;
  }
  static SharkValue* jlong_constant(jlong value)
  {
    SharkValue *result = create_jlong(LLVMValue::jlong_constant(value));
    if (value != 0)
      result->set_zero_checked(true);
    return result;
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
    return create_jobject(LLVMValue::null());
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

  // Typed "conversion" from return address
 public:
  static inline SharkValue* create_returnAddress(int bci);

  // Type-losing conversions, use with care
 public:
  static inline SharkValue* create_generic(ciType* type, llvm::Value* value);
  virtual llvm::Value* generic_value() const
  {
    ShouldNotCallThis();
  }
  virtual llvm::Value* intptr_value(llvm::IRBuilder<>* builder) const
  {
    ShouldNotCallThis();
  }

  // Phi-style stuff
 public:
  virtual void addIncoming(SharkValue *value, llvm::BasicBlock* block)
  {
    ShouldNotCallThis();
  }

  // Repeated null and divide-by-zero check removal
 public:
  virtual bool zero_checked() const
  {
    ShouldNotCallThis();
  }
  virtual void set_zero_checked(bool zero_checked) 
  {
    ShouldNotCallThis();
  }
};

class SharkComputableValue : public SharkValue {
  friend class SharkValue;
  
 protected:
  SharkComputableValue(ciType* type, llvm::Value* value)
    : _type(type), _llvm_value(value), _zero_checked(false) {}

 private:
  ciType*      _type;
  llvm::Value* _llvm_value;
  bool         _zero_checked;

 private:
  llvm::Value* llvm_value() const
  {
    return _llvm_value;
  }

  // Type access
 public:
  ciType* type() const
  {
    return _type;
  }

 public:  
  BasicType basic_type() const
  {
    return type()->basic_type();
  }
  int size() const
  {
    return type()->size();
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

  // Type-losing conversions, use with care
 public:
  llvm::Value* generic_value() const
  {
    return llvm_value();
  }
  llvm::Value* intptr_value(llvm::IRBuilder<>* builder) const
  {
    assert(is_jobject(), "should be");
    return builder->CreatePtrToInt(llvm_value(), SharkType::intptr_type());
  }

  // Wrapped PHINodes
 public:
  void addIncoming(SharkValue *value, llvm::BasicBlock* block)
  {
    assert(llvm::isa<llvm::PHINode>(generic_value()), "should be");
    ((llvm::PHINode *) generic_value())->addIncoming(
      value->generic_value(), block);
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

inline SharkValue* SharkValue::create_generic(ciType* type, llvm::Value* value)
{
  return new SharkComputableValue(type, value);    
}

class SharkReturnAddressValue : public SharkValue {
  friend class SharkValue;

 protected:
  SharkReturnAddressValue(int bci)
    : _bci(bci) {}

 private:
  int _bci;

  // Type access
 public:  
  BasicType basic_type() const
  {
    return T_ADDRESS;
  }
  int size() const
  {
    return 1;
  }

 public:
  bool is_returnAddress() const
  {
    return true;
  }

  // Typed "conversion"
 public:
  int returnAddress_value() const
  {
    return _bci;
  }

  // Type-losing "conversion", use with care
 public:
  llvm::Value* generic_value() const
  {
    return LLVMValue::intptr_constant(_bci);
  }

  // Phi-style stuff
 public:
  void addIncoming(SharkValue *value, llvm::BasicBlock* block)
  {
    assert(_bci == value->returnAddress_value(), "should be");
  }
};

inline SharkValue* SharkValue::create_returnAddress(int bci)
{
  return new SharkReturnAddressValue(bci);
}
