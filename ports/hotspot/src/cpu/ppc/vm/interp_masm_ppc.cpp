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
#include "incls/_interp_masm_ppc.cpp.incl"

// Lock object
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

// Unlocks an object. Throws an IllegalMonitorException if
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
