/*
 * Copyright 2000-2007 Sun Microsystems, Inc.  All Rights Reserved.
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

class VMRegImpl;
typedef VMRegImpl* VMReg;

// Use Register as shortcut
class RegisterImpl;
typedef RegisterImpl* Register;

inline Register as_Register(int encoding) {
  return (Register)(intptr_t) encoding;
}

// The implementation of integer registers for the @@cpu@@ architecture
class RegisterImpl : public AbstractRegisterImpl {
 public:
  enum {
#ifdef PPC
    number_of_registers = 32
#endif // PPC
  };

  // construction
  inline friend Register as_Register(int encoding);
  VMReg as_VMReg();

  // derived registers, offsets, and addresses
  Register successor() const
  {
    return as_Register(encoding() + 1);
  }

  // accessors
  int encoding() const
  {
    assert(is_valid(), "invalid register");
    return (intptr_t)this;
  }
  bool is_valid() const
  {
    return 0 <= (intptr_t) this && (intptr_t)this < number_of_registers;
  }
  const char* name() const;
};

CONSTANT_REGISTER_DECLARATION(Register, noreg, (-1));

// The integer registers of the @@cpu@@ architecture
#ifdef PPC
CONSTANT_REGISTER_DECLARATION(Register, r0,   (0));
CONSTANT_REGISTER_DECLARATION(Register, r1,   (1));
CONSTANT_REGISTER_DECLARATION(Register, r2,   (2));
CONSTANT_REGISTER_DECLARATION(Register, r3,   (3));
CONSTANT_REGISTER_DECLARATION(Register, r4,   (4));
CONSTANT_REGISTER_DECLARATION(Register, r5,   (5));
CONSTANT_REGISTER_DECLARATION(Register, r6,   (6));
CONSTANT_REGISTER_DECLARATION(Register, r7,   (7));
CONSTANT_REGISTER_DECLARATION(Register, r8,   (8));
CONSTANT_REGISTER_DECLARATION(Register, r9,   (9));
CONSTANT_REGISTER_DECLARATION(Register, r10, (10));
CONSTANT_REGISTER_DECLARATION(Register, r11, (11));
CONSTANT_REGISTER_DECLARATION(Register, r12, (12));
CONSTANT_REGISTER_DECLARATION(Register, r13, (13));
CONSTANT_REGISTER_DECLARATION(Register, r14, (14));
CONSTANT_REGISTER_DECLARATION(Register, r15, (15));
CONSTANT_REGISTER_DECLARATION(Register, r16, (16));
CONSTANT_REGISTER_DECLARATION(Register, r17, (17));
CONSTANT_REGISTER_DECLARATION(Register, r18, (18));
CONSTANT_REGISTER_DECLARATION(Register, r19, (19));
CONSTANT_REGISTER_DECLARATION(Register, r20, (20));
CONSTANT_REGISTER_DECLARATION(Register, r21, (21));
CONSTANT_REGISTER_DECLARATION(Register, r22, (22));
CONSTANT_REGISTER_DECLARATION(Register, r23, (23));
CONSTANT_REGISTER_DECLARATION(Register, r24, (24));
CONSTANT_REGISTER_DECLARATION(Register, r25, (25));
CONSTANT_REGISTER_DECLARATION(Register, r26, (26));
CONSTANT_REGISTER_DECLARATION(Register, r27, (27));
CONSTANT_REGISTER_DECLARATION(Register, r28, (28));
CONSTANT_REGISTER_DECLARATION(Register, r29, (29));
CONSTANT_REGISTER_DECLARATION(Register, r30, (30));
CONSTANT_REGISTER_DECLARATION(Register, r31, (31));
#endif // PPC

// Use FloatRegister as shortcut
class FloatRegisterImpl;
typedef FloatRegisterImpl* FloatRegister;

inline FloatRegister as_FloatRegister(int encoding) {
  return (FloatRegister)(intptr_t) encoding;
}

// The implementation of floating point registers for the @@cpu@@ architecture
class FloatRegisterImpl : public AbstractRegisterImpl {
 public:
  enum {
#ifdef PPC
    number_of_registers = 32
#endif // PPC
  };

  // construction
  inline friend FloatRegister as_FloatRegister(int encoding);
  VMReg as_VMReg();

  // derived registers, offsets, and addresses
  FloatRegister successor() const
  {
    return as_FloatRegister(encoding() + 1);
  }

  // accessors
  int encoding() const
  {
    assert(is_valid(), "invalid register");
    return (intptr_t)this;
  }
  bool is_valid() const
  {
    return 0 <= (intptr_t) this && (intptr_t)this < number_of_registers;
  }
  const char* name() const;
};

// The floating point registers of the @@cpu@@ architecture
#ifdef PPC
CONSTANT_REGISTER_DECLARATION(FloatRegister, f0,   (0));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f1,   (1));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f2,   (2));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f3,   (3));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f4,   (4));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f5,   (5));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f6,   (6));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f7,   (7));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f8,   (8));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f9,   (9));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f10, (10));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f11, (11));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f12, (12));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f13, (13));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f14, (14));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f15, (15));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f16, (16));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f17, (17));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f18, (18));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f19, (19));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f20, (20));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f21, (21));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f22, (22));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f23, (23));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f24, (24));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f25, (25));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f26, (26));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f27, (27));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f28, (28));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f29, (29));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f30, (30));
CONSTANT_REGISTER_DECLARATION(FloatRegister, f31, (31));

// Use ConditionRegister as shortcut
class ConditionRegisterImpl;
typedef ConditionRegisterImpl* ConditionRegister;

inline ConditionRegister as_ConditionRegister(int encoding) {
  return (ConditionRegister)(intptr_t) encoding;
}

// The implementation of condition registers for the ppc architecture
class ConditionRegisterImpl : public AbstractRegisterImpl {
 public:
  enum {
    number_of_registers = 8
  };

  // construction
  inline friend ConditionRegister as_ConditionRegister(int encoding);

  // accessors
  int encoding() const
  {
    assert(is_valid(), "invalid register");
    return (intptr_t)this;
  }
  bool is_valid() const
  {
    return 0 <= (intptr_t) this && (intptr_t)this < number_of_registers;
  }
};

// The condition registers registers of the ppc architecture
CONSTANT_REGISTER_DECLARATION(ConditionRegister, cr0, (0));
CONSTANT_REGISTER_DECLARATION(ConditionRegister, cr1, (1));
CONSTANT_REGISTER_DECLARATION(ConditionRegister, cr2, (2));
CONSTANT_REGISTER_DECLARATION(ConditionRegister, cr3, (3));
CONSTANT_REGISTER_DECLARATION(ConditionRegister, cr4, (4));
CONSTANT_REGISTER_DECLARATION(ConditionRegister, cr5, (5));
CONSTANT_REGISTER_DECLARATION(ConditionRegister, cr6, (6));
CONSTANT_REGISTER_DECLARATION(ConditionRegister, cr7, (7));

// Use SpecialPurposeRegister as shortcut
class SpecialPurposeRegisterImpl;
typedef SpecialPurposeRegisterImpl* SpecialPurposeRegister;

// The implementation of special purpose registers for the ppc architecture
class SpecialPurposeRegisterImpl : public AbstractRegisterImpl {
 public:
  enum {
    number_of_registers = 3
  };

  // accessors
  int encoding() const
  {
    assert(is_valid(), "invalid register");
    return (intptr_t)this;
  }
  bool is_valid() const
  {
    return (intptr_t)this == 1 || (intptr_t)this == 8 || (intptr_t)this == 9;
  }
};

// The special purpose registers registers of the ppc architecture
CONSTANT_REGISTER_DECLARATION(SpecialPurposeRegister, xer, (1));
CONSTANT_REGISTER_DECLARATION(SpecialPurposeRegister, lr,  (8));
CONSTANT_REGISTER_DECLARATION(SpecialPurposeRegister, ctr, (9));
#endif // PPC

class ConcreteRegisterImpl : public AbstractRegisterImpl {
 public:
  enum {
    number_of_registers = RegisterImpl::number_of_registers +
                          FloatRegisterImpl::number_of_registers
  };

  static const int max_gpr;
  static const int max_fpr;
};
