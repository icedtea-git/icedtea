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

// Items on the stack and in local variables are tracked using
// SharkValue objects.  There are two types, SharkNormalValue
// and SharkAddressValue, but no code outside this file should
// ever refer to those directly.  The split is because of the
// way JSRs are handled: the typeflow pass expands them into
// multiple copies, so the return addresses pushed by jsr and
// popped by ret only exist at compile time.  Having separate
// classes for these allows us to check that our jsr handling
// is correct, via assertions.

class SharkBuilder;

class SharkValue : public ResourceObj {
 protected:
  SharkValue() {}

  // Type access
 public:
  virtual BasicType basic_type() const = 0;
  virtual ciType*   type()       const;

  virtual bool is_jint()    const;
  virtual bool is_jlong()   const;
  virtual bool is_jfloat()  const;
  virtual bool is_jdouble() const;
  virtual bool is_jobject() const;
  virtual bool is_jarray()  const;
  virtual bool is_address() const;

  virtual int size() const = 0;

  bool is_one_word() const
  {
    return size() == 1;
  }
  bool is_two_word() const
  {
    return size() == 2;
  }

  // Typed conversion from SharkValues
 public:
  virtual llvm::Value* jint_value()    const;
  virtual llvm::Value* jlong_value()   const;
  virtual llvm::Value* jfloat_value()  const;
  virtual llvm::Value* jdouble_value() const;
  virtual llvm::Value* jobject_value() const;
  virtual llvm::Value* jarray_value()  const;
  virtual int          address_value() const;

  // Typed conversion to SharkValues
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

  // Typed conversion from constants of various types
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
  static inline SharkValue* address_constant(int bci);

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
      ShouldNotReachHere();
    }
  }

  // Type-losing conversions -- use with care!
 public:
  virtual llvm::Value* generic_value() const = 0;
  virtual llvm::Value* intptr_value(SharkBuilder* builder) const;

  static inline SharkValue* create_generic(ciType* type, llvm::Value* value);

  // Phi-style stuff
 public:
  virtual void addIncoming(SharkValue *value, llvm::BasicBlock* block) = 0;

  // Repeated null and divide-by-zero check removal
 public:
  virtual bool zero_checked() const;
  virtual void set_zero_checked(bool zero_checked);
};

class SharkNormalValue : public SharkValue {
  friend class SharkValue;
  
 protected:
  SharkNormalValue(ciType* type, llvm::Value* value)
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
  ciType*   type()       const;
  BasicType basic_type() const;
  int       size()       const;

 public:
  bool is_jint()    const;
  bool is_jlong()   const;
  bool is_jfloat()  const;
  bool is_jdouble() const;
  bool is_jobject() const;
  bool is_jarray()  const;

  // Typed conversions to LLVM values
 public:
  llvm::Value* jint_value()    const;
  llvm::Value* jlong_value()   const;
  llvm::Value* jfloat_value()  const;
  llvm::Value* jdouble_value() const;
  llvm::Value* jobject_value() const;
  llvm::Value* jarray_value()  const;

  // Type-losing conversions, use with care
 public:
  llvm::Value* generic_value() const;
  llvm::Value* intptr_value(SharkBuilder* builder) const;

  // Wrapped PHINodes
 public:
  void addIncoming(SharkValue *value, llvm::BasicBlock* block);

  // Repeated null and divide-by-zero check removal
 public:
  bool zero_checked() const;
  void set_zero_checked(bool zero_checked);
};

inline SharkValue* SharkValue::create_generic(ciType* type, llvm::Value* value)
{
  return new SharkNormalValue(type, value);    
}

class SharkAddressValue : public SharkValue {
  friend class SharkValue;

 protected:
  SharkAddressValue(int bci)
    : _bci(bci) {}

 private:
  int _bci;

  // Type access
 public:  
  BasicType basic_type() const;
  int       size()       const;
  bool      is_address() const;

  // Typed conversion from SharkValues
 public:
  int address_value() const;

  // Type-losing conversion -- use with care!
 public:
  llvm::Value* generic_value() const;

  // Phi-style stuff
 public:
  void addIncoming(SharkValue *value, llvm::BasicBlock* block);
};

inline SharkValue* SharkValue::address_constant(int bci)
{
  return new SharkAddressValue(bci);
}
