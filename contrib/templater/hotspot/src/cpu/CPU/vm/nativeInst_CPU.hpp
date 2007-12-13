/*
 * Copyright 2003-2006 Sun Microsystems, Inc.  All Rights Reserved.
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

// We have interfaces for the following instructions:
// - NativeInstruction
// - - NativeCall
// - - NativeMovConstReg
// - - NativeMovConstRegPatching
// - - NativeJump
// - - NativeIllegalOpCode
// - - NativeReturn   
// - - NativeReturnX (return with argument)
// - - NativePushConst
// - - NativeTstRegMem

// The base class for different kinds of native instruction abstractions.
// Provides the primitive operations to manipulate code relative to this.

class NativeInstruction VALUE_OBJ_CLASS_SPEC
{
 public:
  bool is_jump()
  {
    Unimplemented();
  }

  bool is_safepoint_poll()
  { 
    Unimplemented();
  }
};

inline NativeInstruction* nativeInstruction_at(address address)
{
  Unimplemented();
}

class NativeCall : public NativeInstruction 
{
 public:
  enum @@cpu@@_specific_constants {
#ifdef PPC
    instruction_size = 4,
#endif // PPC
  };

  address instruction_address() const       
  { 
    Unimplemented();
  }

  address next_instruction_address() const       
  { 
    Unimplemented();
  }

  address return_address() const
  {
    Unimplemented();
  }

  address destination() const
  {
    Unimplemented();
  }

  void set_destination_mt_safe(address dest)
  {
    Unimplemented();
  }

  void verify_alignment()
  {
    Unimplemented();
  }

  void verify()
  {
    Unimplemented();
  }

  static bool is_call_before(address return_address)
  {
    Unimplemented();
  }
};

inline NativeCall* nativeCall_before(address return_address)
{
  Unimplemented();
}

inline NativeCall* nativeCall_at(address address)
{
  Unimplemented();
}

class NativeMovConstReg : public NativeInstruction
{
 public:
  address next_instruction_address() const
  {
    Unimplemented();
  }

  intptr_t data() const
  {
    Unimplemented();
  }

  void set_data(intptr_t x)
  {
    Unimplemented();
  }
};

inline NativeMovConstReg* nativeMovConstReg_at(address address)
{
  Unimplemented();
}

class NativeMovRegMem : public NativeInstruction
{
 public:
  int offset() const
  {
    Unimplemented();
  }

  void set_offset(intptr_t x)
  {
    Unimplemented();
  }

  void add_offset_in_bytes(int add_offset)
  {
    Unimplemented();
  }
};

inline NativeMovRegMem* nativeMovRegMem_at(address address)
{
  Unimplemented();
}

class NativeJump : public NativeInstruction
{
 public:
  enum @@cpu@@_specific_constants {
#ifdef PPC
    instruction_size = 4,
#endif // PPC
  };

  address jump_destination() const
  {
    Unimplemented();
  }

  void set_jump_destination(address dest)
  {
    Unimplemented();
  }

  static void check_verified_entry_alignment(address entry,
                                             address verified_entry) {
#ifdef PPC
    // nothing to do for ppc
#else
    Unimplemented();
#endif // PPC
  }

  static void patch_verified_entry(address entry,
                                   address verified_entry,
                                   address dest) {
    Unimplemented();
  }
};

inline NativeJump* nativeJump_at(address address)
{
  Unimplemented();
}

class NativeGeneralJump : public NativeInstruction
{
 public:
  address jump_destination() const
  {
    Unimplemented();
  }

  static void insert_unconditional(address code_pos, address entry)
  {
    Unimplemented();
  }

  static void replace_mt_safe(address instr_addr, address code_buffer)
  {
    Unimplemented();
  }
};

inline NativeGeneralJump* nativeGeneralJump_at(address address)
{
  Unimplemented();
}
