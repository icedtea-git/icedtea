/*
 * Copyright 2003-2007 Sun Microsystems, Inc.  All Rights Reserved.
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

// This file specializes the assember with interpreter-specific macros

#ifdef PPC
#ifdef CC_INTERP
REGISTER_DECLARATION(Register, Rstate, r28);

#define STATE(field_name) \
  (Address(Rstate, byte_offset_of(BytecodeInterpreter, field_name)))
#endif // CC_INTERP

#endif // PPC
class InterpreterMacroAssembler : public MacroAssembler {
#ifdef PPC
 protected:
  // Support for VM calls
  virtual void call_VM_leaf_base(address entry_point);
  virtual void call_VM_base(Register oop_result,
                            address entry_point,
                            CallVMFlags flags);

#endif // PPC
 public:
  InterpreterMacroAssembler(CodeBuffer* code) : MacroAssembler(code) {}
#ifdef PPC

  // Frame anchor tracking
  void set_last_Java_frame(Register lr_save = noreg);
  void reset_last_Java_frame();

  // Object locking
  void lock_object(Register entry);
  void unlock_object(Register entry);

  // Safepoints
  void fixup_after_potential_safepoint();
#endif // PPC
};
