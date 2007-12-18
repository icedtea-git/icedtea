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

#include "incls/_precompiled.incl"
#include "incls/_interp_masm_@@cpu@@.cpp.incl"

#ifdef PPC
#ifdef CC_INTERP
REGISTER_DEFINITION(Register, Rstate);
#endif

// Interpreter-specific support for VM calls

void InterpreterMacroAssembler::call_VM_base(Register oop_result,
                                             address entry_point,
                                             CallVMFlags flags)
{
  // Set the Java frame anchor
  set_last_Java_frame(flags & CALL_VM_PRESERVE_LR ? r3 : noreg);

  // Make the call
  MacroAssembler::call_VM_base(oop_result, entry_point, flags);

  // Clear the Java frame anchor
  reset_last_Java_frame();

  // Reload anything that may have changed if there was a safepoint
  fixup_after_potential_safepoint();
}

void InterpreterMacroAssembler::call_VM_leaf_base(address entry_point)
{
  // Make the call
  MacroAssembler::call_VM_leaf_base(entry_point);
}

// Set the last Java frame pointer
// NB trashes LR unless you pass it a register to store it in

void InterpreterMacroAssembler::set_last_Java_frame(Register lr_save)
{
  assert_different_registers(lr_save, r0);

  if (lr_save->is_valid())
    mflr(lr_save);

  mpclr();
  mflr(r0);
  store(r0, Address(Rthread, JavaThread::last_Java_pc_offset()));
  store(r1, Address(Rthread, JavaThread::last_Java_sp_offset()));

  if (lr_save->is_valid())
    mtlr(lr_save);
}

// Clear the last Java frame pointer

void InterpreterMacroAssembler::reset_last_Java_frame()
{
  load(r0, 0);
  store(r0, Address(Rthread, JavaThread::last_Java_sp_offset()));
}

// Lock an object
//
// Arguments:
//  monitor: BasicObjectLock to be used for locking

void InterpreterMacroAssembler::lock_object(Register entry)
{
  assert (!UseHeavyMonitors, "not supported");
  assert (!UseBiasedLocking, "not supported");

  const Register lockee    = r3;
  const Register mark      = r4;
  const Register displaced = r5;

  assert_different_registers(entry, lockee, mark, displaced);

  Label done;

  load(lockee, Address(entry, BasicObjectLock::obj_offset_in_bytes()));
  la(mark, Address(lockee, oopDesc::mark_offset_in_bytes()));

  // This is based on the bit in BytecodeInterpreter::run()
  // that handles got_monitors messages.  The code in the
  // comments is taken from there.

  // displaced = lockee->mark()->set_unlocked()
  load(displaced, Address(mark, 0));
  ori(displaced, displaced, markOopDesc::unlocked_value);

  // entry->lock()->set_displaced_header(displaced)
  store(displaced, Address(entry, BasicObjectLock::lock_offset_in_bytes() +
			   BasicLock::displaced_header_offset_in_bytes()));

  // Atomic::cmpxchg_ptr(entry, lockee->mark_addr(), displaced)
  // If this succeeds we saw an unlocked object and are done.
  cmpxchg_(entry, mark, displaced);
  beq(done);

  unimplemented(__FILE__, __LINE__);
  bind(done);
}

// Unlock an object. Throws an IllegalMonitorException if
// object is not locked by current thread.
//
// Arguments:
//  monitor: BasicObjectLock for lock
// 
void InterpreterMacroAssembler::unlock_object(Register entry)
{
  assert (!UseHeavyMonitors, "not supported");
  assert (!UseBiasedLocking, "not supported");

  const Register lockee = r3;
  const Register mark   = r4;
  const Register lock   = r5;
  const Register header = r6;

  assert_different_registers(entry, lockee, mark, lock, header);

  Label unlock, done;

  load(lockee, Address(entry, BasicObjectLock::obj_offset_in_bytes()));
  la(mark, Address(lockee, oopDesc::mark_offset_in_bytes()));

  // Check that the object has not already been unlocked
  compare(lockee, 0);
  bne(unlock);
  unimplemented(__FILE__, __LINE__);
  bind(unlock);

  // lock = entry->lock()
  la(lock, Address(entry, BasicObjectLock::lock_offset_in_bytes()));
  
  // header = lock->displaced_header()
  load(header, Address(lock, BasicLock::displaced_header_offset_in_bytes()));

  // entry->set_obj(NULL)
  load(r0, 0);
  load(r0, Address(entry, BasicObjectLock::obj_offset_in_bytes()));

  // If it was recursive then we're done
  compare(header, 0);
  beq(done);
  
  // Atomic::cmpxchg_ptr(header, lockee->mark_addr(), lock)
  // If this succeeds we unlocked the object and are done.
  cmpxchg_(header, mark, lock);
  beq(done);

  unimplemented(__FILE__, __LINE__);
  bind(done);
}

// Reload everything that might have changed after a safepoint

void InterpreterMacroAssembler::fixup_after_potential_safepoint()
{
#ifdef CC_INTERP
  load(Rmethod, STATE(_method));
  verify_oop(Rmethod);
#else
  Unimplemented();
#endif
}
#endif // PPC
