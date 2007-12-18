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
#include "incls/_cppInterpreter_@@cpu@@.cpp.incl"

#ifdef CC_INTERP
#ifdef PPC

// The address of this function is stored in the LR save area
// while we are recursed in the frame manager/C++ interpreter.
// We could use an address in the frame manager but having it
// this way makes things look nicer in the debugger and catches
// us if we attempt to "return" from a re-dispatched frame.
extern "C" void RecursiveInterpreterActivation(interpreterState istate)
{
  ShouldNotReachHere(); 
}

#define __ _masm->

// Non-volatile registers we use
const Register          Rmonitor = r27;
const ConditionRegister CRsync   = cr2;
const ConditionRegister CRstatic = cr3;

// slop_factor is two extra slots on the expression stack so
// that we always have room to store a result when returning
// from a call without parameters that returns a result.
// This is the "static long no_params() method" issue.  It
// may not be needed for native calls -- it may not be needed
// at all -- but if it is needed then it's going to start
// writing all over the link area on PPC32 at least so better
// safe than sorry for now.
const int slop_factor = 2 * wordSize;

// Stuff for inter-entry jumping
static Label fast_accessor_slow_entry_path;

// Stuff for caching identical entries
static address normal_entry = NULL;
static address native_entry = NULL;
#endif // PPC

int AbstractInterpreter::BasicType_as_index(BasicType type)
{
  int i = 0;
  switch (type) {
    case T_BOOLEAN: i = 0; break;
    case T_CHAR   : i = 1; break;
    case T_BYTE   : i = 2; break;
    case T_SHORT  : i = 3; break;
    case T_INT    : i = 4; break;
    case T_LONG   : i = 5; break;
    case T_VOID   : i = 6; break;
    case T_FLOAT  : i = 7; break;
    case T_DOUBLE : i = 8; break;
    case T_OBJECT : i = 9; break;
    case T_ARRAY  : i = 9; break;
    default       : ShouldNotReachHere();
  }
  assert(0 <= i && i < AbstractInterpreter::number_of_result_handlers,
         "index out of bounds");
  return i;
}

// Is this pc anywhere within code owned by the interpreter?
// This only works for code that we have generated.  It clearly
// misses all of the actual C++ interpreter implementation.

bool CppInterpreter::contains(address pc)
{
#ifdef PPC
  return pc == CAST_FROM_FN_PTR(address, RecursiveInterpreterActivation)
    || _code->contains(pc);
#else
  Unimplemented();
#endif // PPC
}

#ifdef PPC
// A result is the register or registers defined in the native ABI
// for that type, unless it is an OOP in which case it will have
// been unboxed and saved in the frame.  Preprocess it.

address CppInterpreterGenerator::generate_result_handler_for(BasicType type)
{
  address start = __ pc();

  switch (type) {
  case T_VOID:
    break;

  case T_BOOLEAN:
    {
      Label zero;

      __ compare (r3, 0);
      __ beq (zero);
      __ load (r3, 1);
      __ bind (zero);
    }
    break;

  case T_CHAR:
    __ andi_ (r3, r3, 0xffff);
    break;

  case T_BYTE:
    __ extsb (r3, r3);
    break;
    
  case T_SHORT:
    __ extsh (r3, r3);
    break;

  case T_INT:
#ifdef PPC64
    __ extsw (r3, r3);
#endif
    break;

  case T_LONG:
  case T_FLOAT:
  case T_DOUBLE:
    break;

  case T_OBJECT:
    __ load (r3, STATE(_oop_temp));
    __ verify_oop (r3);
    break;

  default:
    ShouldNotReachHere();
  }
  __ blr ();

  return start;
}

// A result is the register or registers defined in the native ABI
// for that type.  Push it from there onto the top of the caller's
// expression stack.
// 
// Arguments:
//  Rlocals: the top of the caller's expression stack
//
// Returns:
//  Rlocals: the adjusted top of the caller's expression stack

address CppInterpreterGenerator::generate_tosca_to_stack_converter(
    BasicType type)
{
  address start = __ pc();

  switch (type) {
  case T_VOID:
    break;

  case T_BOOLEAN:
  case T_CHAR:
  case T_BYTE:
  case T_SHORT:
  case T_INT:
    __ stw (r3, Address(Rlocals, 0));
    __ subi (Rlocals, Rlocals, wordSize);
    break;
    
  case T_LONG:
    __ store (r3, Address(Rlocals, -wordSize));
#ifdef PPC32
    __ store (r4, Address(Rlocals, 0));
#endif
    __ subi (Rlocals, Rlocals, wordSize * 2);
    break;

  case T_FLOAT:
    __ stfs (f1, Address(Rlocals, 0));
    __ subi (Rlocals, Rlocals, wordSize);
    break;

  case T_DOUBLE:
    __ stfd (f1, Address(Rlocals, -wordSize));
    __ subi (Rlocals, Rlocals, wordSize * 2);
    break;

  case T_OBJECT:
    __ verify_oop (r3);
    __ store (r3, Address(Rlocals, 0));
    __ subi (Rlocals, Rlocals, wordSize);
    break;

  default:
    ShouldNotReachHere();
  }
  __ blr ();

  return start;
}

// A result is at the top of the Java expression stack of the method
// that has just returned.  Push it from there onto the top of the
// caller's expression stack.
// 
// Arguments:
//  Rstate:  the interpreter state of the method that has just returned
//  Rlocals: the top of the caller's expression stack
//
// Returns:
//  Rlocals: the adjusted top of the caller's expression stack

address CppInterpreterGenerator::generate_stack_to_stack_converter(
    BasicType type)
{
  const Register stack = r3;

  address start = __ pc();

  switch (type) {
  case T_VOID:
    break;

  case T_BOOLEAN:
  case T_CHAR:
  case T_BYTE:
  case T_SHORT:
  case T_INT:
  case T_FLOAT:
    __ load (stack, STATE(_stack));
    __ lwz (r0, Address(stack, wordSize));
    __ stw (r0, Address(Rlocals, 0));
    __ subi (Rlocals, Rlocals, wordSize);
    break;

  case T_LONG:
  case T_DOUBLE:
    __ load (stack, STATE(_stack));
    __ load (r0, Address(stack, wordSize));
    __ store (r0, Address(Rlocals, -wordSize));
#ifdef PPC32
    __ load (r0, Address(stack, wordSize * 2));
    __ store (r0, Address(Rlocals, 0));
#endif
    __ subi (Rlocals, Rlocals, wordSize * 2);
    break;

  case T_OBJECT:
    __ load (stack, STATE(_stack));
    __ load (r0, Address(stack, wordSize));
    __ verify_oop (r0);
    __ store (r0, Address(Rlocals, 0));
    __ subi (Rlocals, Rlocals, wordSize);
    break;

  default:
    ShouldNotReachHere();
  }
  __ blr ();

  return start;
}

// A result is at the top of the Java expression stack of the method
// that has just returned.  Copy it from there into the register or
// registers defined in the native ABI for that type.
// 
// Arguments:
//  Rstate:  the interpreter state of the method that has just returned

address CppInterpreterGenerator::generate_stack_to_native_abi_converter(
    BasicType type)
{
  const Register stack = r5;

  address start = __ pc();

  switch (type) {
  case T_VOID:
    break;

  case T_BOOLEAN:
  case T_CHAR:
  case T_BYTE:
  case T_SHORT:
  case T_INT:
    __ load (stack, STATE(_stack));
    __ lwa (r3, Address(stack, wordSize));
    break;

  case T_LONG:
    __ load (stack, STATE(_stack));
    __ load (r3, Address(stack, wordSize));
#ifdef PPC32
    __ load (r4, Address(stack, wordSize * 2));
#endif
    break;

  case T_FLOAT:
    __ load (stack, STATE(_stack));
    __ lfs (f1, Address(stack, wordSize));
    break;

  case T_DOUBLE:
    __ load (stack, STATE(_stack));
    __ lfd (f1, Address(stack, wordSize));
    break;

  case T_OBJECT:
    __ load (stack, STATE(_stack));
    __ load (r3, Address(stack, wordSize));
    __ verify_oop (r3);
    break;
    
  default:
    ShouldNotReachHere();
  }
  __ blr ();

  return start;
}

// C++ Interpreter stub for empty methods.

address InterpreterGenerator::generate_empty_entry()
{
  if (!UseFastEmptyMethods)
    return NULL;

  Label& slow_path = fast_accessor_slow_entry_path;
  
  address start = __ pc();

  // Drop into the slow path if we need a safepoint check.
  __ load (r3, (intptr_t) SafepointSynchronize::address_of_state());
  __ load (r0, Address(r3, 0));
  __ compare (r0, SafepointSynchronize::_not_synchronized);
  __ bne (slow_path);

  // Ok, we're done :)
  __ blr ();

  return start;
}

// C++ Interpreter stub for "calling" an accessor method.

address InterpreterGenerator::generate_accessor_entry()
{
  if (!UseFastAccessorMethods)
    return NULL;

  Label& slow_path = fast_accessor_slow_entry_path;
  
  address start = __ pc();

  // Drop into the slow path if we need a safepoint check.
  __ load (r3, (intptr_t) SafepointSynchronize::address_of_state());
  __ load (r0, Address(r3, 0));
  __ compare (r0, SafepointSynchronize::_not_synchronized);
  __ bne (slow_path);
  
  // Load the object pointer and drop into the slow path
  // if we have a NullPointerException.
  const Register object = r4;

  __ load (object, Address(Rlocals, 0));
  __ compare (object, 0);
  __ beq (slow_path);

  // Read the field index from the bytecode, which looks like this:
  //  0:  0x2a:    aload_0
  //  1:  0xb4:    getfield
  //  2:             index (high byte)
  //  3:             index (low byte)
  //  4:  0xac/b0: ireturn/areturn
  const Register index = r5;
  
  __ load (index, Address(Rmethod, methodOopDesc::const_offset()));
  __ lwz (index, Address(index, constMethodOopDesc::codes_offset()));
#ifdef ASSERT
  {
    Label ok;
    __ shift_right (r0, index, 16);
    __ compare (r0, (Bytecodes::_aload_0 << 8) | Bytecodes::_getfield);
    __ beq (ok);
    __ should_not_reach_here (__FILE__, __LINE__);
    __ bind (ok);
  }
#endif
  __ andi_ (index, index, 0xffff);

  // Locate the entry in the constant pool cache
  const Register entry = r6;
  
  __ load (entry, Address(Rmethod, methodOopDesc::constants_offset()));
  __ load (entry, Address(entry,constantPoolOopDesc::cache_offset_in_bytes()));
  __ la (entry, Address(entry, constantPoolCacheOopDesc::base_offset()));
  __ shift_left(r0, index,
       exact_log2(in_words(ConstantPoolCacheEntry::size())) + LogBytesPerWord);
  __ add (entry, entry, r0);

  // Check the validity of the cache entry by testing whether the
  // _indices field contains Bytecode::_getfield in b1 byte.
  __ load (r0, Address(entry, ConstantPoolCacheEntry::indices_offset()));
  __ shift_right (r0, r0, 16);
  __ andi_ (r0, r0, 0xff);
  __ compare (r0, Bytecodes::_getfield);
  __ bne (slow_path);

  // Calculate the type and offset of the field
  const Register offset = r7;
  const Register type   = r8;

  __ load (offset, Address(entry, ConstantPoolCacheEntry::f2_offset()));
  __ load (type, Address(entry, ConstantPoolCacheEntry::flags_offset()));
  ConstantPoolCacheEntry::verify_tosBits();
  __ shift_right (type, type, ConstantPoolCacheEntry::tosBits);

  // Load the value
  Label is_object, is_int, is_byte, is_short, is_char;

  __ compare (type, atos);
  __ beq (is_object);
  __ compare (type, itos);
  __ beq (is_int);
  __ compare (type, btos);
  __ beq (is_byte);
  __ compare (type, stos);
  __ beq (is_short);
  __ compare (type, ctos);
  __ beq (is_char);

  __ load (r3, (intptr_t) "error: unknown type: %d\n");
  __ mr (r4, type);
  __ call (CAST_FROM_FN_PTR(address, printf));
  __ should_not_reach_here (__FILE__, __LINE__);

  __ bind (is_object);
  __ load_indexed (r3, object, offset);
  __ blr ();

  __ bind (is_int);
  __ lwax (r3, object, offset);
  __ blr ();

  __ bind (is_byte);
  __ lbax (r3, object, offset);
  __ blr ();

  __ bind (is_short);
  __ lhax (r3, object, offset);
  __ blr ();

  __ bind (is_char);
  __ lhzx (r3, object, offset);
  __ blr ();

  return start;  
}

// C++ Interpreter stub for calling a native method.
// This sets up a somewhat different looking stack for calling the
// native method than the typical interpreter frame setup but still
// has the pointer to an interpreter state.

address InterpreterGenerator::generate_native_entry(bool synchronized)
{
  const Register handler  = r14;
  const Register function = r15;

  assert_different_registers(Rmethod, Rlocals, Rthread, Rstate, Rmonitor,
			     handler, function);

  // We use the same code for synchronized and not
  if (native_entry)
    return native_entry;

  address start = __ pc();

  // Allocate and initialize our stack frame.
  __ load (Rstate, 0);
  generate_compute_interpreter_state(true);

  // Make sure method is native and not abstract
#ifdef ASSERT
  {
    Label ok;
    __ lwz (r0, Address(Rmethod, methodOopDesc::access_flags_offset()));
    __ andi_ (r0, r0, JVM_ACC_NATIVE | JVM_ACC_ABSTRACT);
    __ compare (r0, JVM_ACC_NATIVE);
    __ beq (ok);
    __ should_not_reach_here (__FILE__, __LINE__);
    __ bind (ok);
  }
#endif

  // Lock if necessary
  Label not_synchronized_1;
  
  __ bne (CRsync, not_synchronized_1);
  __ lock_object (Rmonitor);
  __ bind (not_synchronized_1);
  
  // Get signature handler
  const Address signature_handler_addr(
    Rmethod, methodOopDesc::signature_handler_offset());

  Label return_to_caller, got_signature_handler;

  __ load (handler, signature_handler_addr);
  __ compare (handler, 0);
  __ bne (got_signature_handler);
  __ call_VM (noreg,
              CAST_FROM_FN_PTR(address,
                               InterpreterRuntime::prepare_native_call),
              Rmethod,
              CALL_VM_NO_EXCEPTION_CHECKS);
  __ load (r0, Address(Rthread, Thread::pending_exception_offset()));
  __ compare (r0, 0);
  __ bne (return_to_caller);
  __ load (handler, signature_handler_addr);
  __ bind (got_signature_handler); 

  // Get the native function entry point
  const Address native_function_addr(
    Rmethod, methodOopDesc::native_function_offset());

  Label got_function;

  __ load (function, native_function_addr);
#ifdef ASSERT
  {
    // InterpreterRuntime::prepare_native_call() sets the mirror
    // handle and native function address first and the signature
    // handler last, so function should always be set here.
    Label ok;
    __ compare (function, 0);
    __ bne (ok);
    __ should_not_reach_here (__FILE__, __LINE__);
    __ bind (ok);
  }
#endif

  // Call signature handler
  __ mtctr (handler);
  __ bctrl ();
  __ mr (handler, r0);

  // Pass JNIEnv
  __ la (r3, Address(Rthread, JavaThread::jni_environment_offset()));

  // Pass mirror handle if static
  const Address oop_temp_addr = STATE(_oop_temp);

  Label not_static;

  __ bne (CRstatic, not_static);
  __ get_mirror_handle (r4);
  __ store (r4, oop_temp_addr);
  __ la (r4, oop_temp_addr);
  __ bind (not_static);

  // Set up the Java frame anchor
  __ set_last_Java_frame ();

  // Change the thread state to native
  const Address thread_state_addr(Rthread, JavaThread::thread_state_offset());
#ifdef ASSERT
  {
    Label ok;
    __ lwz (r0, thread_state_addr);
    __ compare (r0, _thread_in_Java);
    __ beq (ok);
    __ should_not_reach_here (__FILE__, __LINE__);
    __ bind (ok);
  }
#endif
  __ load (r0, _thread_in_native);
  __ stw (r0, thread_state_addr);

  // Make the call
  __ call (function);
  __ fixup_after_potential_safepoint ();

  // The result will be in r3 (and maybe r4 on 32-bit) or f1.
  // Wherever it is, we need to store it before calling anything
  const Register r3_save      = r16;
#ifdef PPC32
  const Register r4_save      = r17;
#endif
  const FloatRegister f1_save = f14;

  __ mr (r3_save, r3);
#ifdef PPC32
  __ mr (r4_save, r4);
#endif
  __ fmr (f1_save, f1);

  // Switch thread to "native transition" state before reading the
  // synchronization state.  This additional state is necessary
  // because reading and testing the synchronization state is not
  // atomic with respect to garbage collection.
  __ load (r0, _thread_in_native_trans);
  __ stw (r0, thread_state_addr);

  // Ensure the new state is visible to the VM thread.
  if(os::is_MP()) {
    if (UseMembar)
      __ sync ();
    else
      __ serialize_memory (r3, r4);
  }

  // Check for safepoint operation in progress and/or pending
  // suspend requests.  We use a leaf call in order to leave
  // the last_Java_frame setup undisturbed.
  Label block, no_block;

  __ load (r3, (intptr_t) SafepointSynchronize::address_of_state());
  __ lwz (r0, Address(r3, 0));
  __ compare (r0, SafepointSynchronize::_not_synchronized);
  __ bne (block);
  __ lwz (r0, Address(Rthread, JavaThread::suspend_flags_offset()));
  __ compare (r0, 0);
  __ beq (no_block);
  __ bind (block);
  __ call_VM_leaf (
       CAST_FROM_FN_PTR(address, 
                        JavaThread::check_special_condition_for_native_trans));
  __ fixup_after_potential_safepoint ();
  __ bind (no_block);

  // Change the thread state
  __ load (r0, _thread_in_Java);
  __ stw (r0, thread_state_addr);

  // Reset the frame anchor  
  __ reset_last_Java_frame ();

  // If the result was an OOP then unbox it and store it in the frame
  // (where it will be safe from garbage collection) before we release
  // the handle it might be protected by
  Label non_oop, store_oop;
  
  __ load (r0, (intptr_t) AbstractInterpreter::result_handler(T_OBJECT));
  __ compare (r0, handler);
  __ bne (non_oop);
  __ compare (r3_save, 0);
  __ beq (store_oop);
  __ load (r3_save, Address(r3_save, 0));
  __ bind (store_oop);
  __ store (r3_save, STATE(_oop_temp));
  __ bind (non_oop);

  // Reset handle block
  __ load (r3, Address(Rthread, JavaThread::active_handles_offset()));
  __ load (r0, 0);
  __ stw (r0, Address(r3, JNIHandleBlock::top_offset_in_bytes()));

  // If there is an exception we skip the result handler and return.
  // Note that this also skips unlocking which seems totally wrong,
  // but apparently this is what the asm interpreter does so we do
  // too.
  __ load (r0, Address(Rthread, Thread::pending_exception_offset()));
  __ compare (r0, 0);
  __ bne (return_to_caller);
  
  // Unlock if necessary
  Label not_synchronized_2;
  
  __ bne (CRsync, not_synchronized_2);
  __ unlock_object (Rmonitor);
  __ bind (not_synchronized_2);

  // Restore saved result and call the result handler
  __ mr (r3, r3_save);
#ifdef PPC32
  __ mr (r4, r4_save);
#endif
  __ fmr (f1, f1_save);
  __ mtctr (handler);
  __ bctrl ();
  
  // Unwind the current activation and return
  __ bind (return_to_caller);

  generate_unwind_interpreter_state();
  __ blr ();

  native_entry = start;
  return start;
}

// Initial entry to C++ interpreter from the call_stub.
// This entry point is called the frame manager since it handles the
// generation of interpreter activation frames via requests directly
// from the vm (via call_stub) and via requests from the interpreter.
// The requests from the call_stub happen directly thru the entry
// point. Requests from the interpreter happen via returning from the
// interpreter and examining the message the interpreter has returned
// to the frame manager. The frame manager can take the following
// requests:
//
// NO_REQUEST - error, should never happen.
// MORE_MONITORS - need a new monitor. Shuffle the expression stack
//                 on down and allocate a new monitor.
// CALL_METHOD - set up a new activation to call a new method.  Very
//               similar to what happens during entry during the entry
//               via the call stub.
// RETURN_FROM_METHOD - remove an activation. Return to interpreter
//                      or call stub.
//
// Arguments:
//  Rmethod: address of methodOop
//  Rlocals: address of first parameter
//  Rthread: address of current thread
//
// Stack layout at entry:
//       | ...                          |
//       +------------------------------+
//   +-> | Link area                    |
//   |   +------------------------------+  -----------------------------
//   |   | Register save area           |                 high addresses
//   |   +------------------------------+  
//   |   | Caller's local variables     |
//   |   +------------------------------+
//   |   | Parameter  0                 |
//   |   |  ...                         |
//   |   | Parameter n-1                |
//   |   +------------------------------+
//   |   | Padding                      |
//   |   +------------------------------+
//   |   | Parameter list space (ppc64) |
//   |   +------------------------------+
//   +---+ Link area                    |                  low addresses
//       +------------------------------+  -----------------------------
//
// We are free to blow any registers we like because the call_stub
// which brought us here initially has preserved the callee save
// registers already.

address InterpreterGenerator::generate_normal_entry(bool synchronized)
{
  assert_different_registers(Rmethod, Rlocals, Rthread, Rstate, Rmonitor);
  
  Label re_dispatch;
  Label call_interpreter;
  Label call_method;
  Label call_non_interpreted_method;
  Label return_with_exception;
  Label return_from_method;
  Label resume_interpreter;
  Label return_to_initial_caller;
  Label more_monitors;
  Label throwing_exception;

  // We use the same code for synchronized and not
  if (normal_entry)
    return normal_entry;

  address start = __ pc();

  // There are two ways in which we can arrive at this entry.
  // There is the special case where a normal interpreted method
  // calls another normal interpreted method, and there is the
  // general case of when we enter from somewhere else: from
  // call_stub, from C1 or C2, or from a fast accessor which
  // deferred. In the special case we're already in frame manager
  // code: we arrive at re_dispatch with Rstate containing the
  // previous interpreter state.  In the general case we arrive
  // at start with no previous interpreter state so we set Rstate
  // to NULL to indicate this.
  __ bind (fast_accessor_slow_entry_path);
  __ load (Rstate, 0);
  __ bind (re_dispatch);

  // Adjust the caller's stack frame to accomodate any additional
  // local variables we have contiguously with our parameters.
  generate_adjust_callers_stack();

  // Allocate and initialize our stack frame.
  generate_compute_interpreter_state(false);

  // Call the interpreter ==============================================
  __ bind (call_interpreter);

  // We can setup the frame anchor with everything we want at
  // this point as we are thread_in_Java and no safepoints can
  // occur until we go to vm mode. We do have to clear flags
  // on return from vm but that is it
  __ set_last_Java_frame ();

  // Call interpreter
  address interpreter = JvmtiExport::can_post_interpreter_events() ?
    CAST_FROM_FN_PTR(address, BytecodeInterpreter::runWithChecks) :
    CAST_FROM_FN_PTR(address, BytecodeInterpreter::run);    

  __ mr (r3, Rstate);
  __ call (interpreter);
  __ fixup_after_potential_safepoint ();

  // Clear the frame anchor
  __ reset_last_Java_frame ();

  // Examine the message from the interpreter to decide what to do
  __ lwz (r4, STATE(_msg));
  __ compare (r4, BytecodeInterpreter::call_method);
  __ beq (call_method);
  __ compare (r4, BytecodeInterpreter::return_from_method);
  __ beq (return_from_method);
  __ compare (r4, BytecodeInterpreter::more_monitors);
  __ beq (more_monitors);
  __ compare (r4, BytecodeInterpreter::throwing_exception);
  __ beq (throwing_exception);

  __ load (r3, (intptr_t) "error: bad message from interpreter: %d\n");
  __ call (CAST_FROM_FN_PTR(address, printf));
  __ should_not_reach_here (__FILE__, __LINE__);

  // Handle a call_method message ======================================
  __ bind (call_method);

  __ load (Rmethod, STATE(_result._to_call._callee));
  __ verify_oop(Rmethod);
  __ load (Rlocals, STATE(_stack));
  __ lhz (r0, Address(Rmethod, methodOopDesc::size_of_parameters_offset()));
  __ shift_left (r0, r0, LogBytesPerWord);
  __ add (Rlocals, Rlocals, r0);

  __ load (r0, STATE(_result._to_call._callee_entry_point));
  __ load (r3, (intptr_t) start);
  __ compare (r0, r3);
  __ bne (call_non_interpreted_method);

  // Interpreted methods are intercepted and re-dispatched -----------
  __ load (r0, CAST_FROM_FN_PTR(intptr_t, RecursiveInterpreterActivation));
  __ mtlr (r0);
  __ b (re_dispatch);

  // Non-interpreted methods are dispatched normally -----------------
  __ bind (call_non_interpreted_method);
  __ mtctr (r0);
  __ bctrl ();

  // Restore Rstate
  __ load (Rstate, Address(r1, StackFrame::back_chain_offset * wordSize));
  __ subi (Rstate, Rstate, sizeof(BytecodeInterpreter));

  // Check for pending exceptions
  __ load (r0, Address(Rthread, Thread::pending_exception_offset()));
  __ compare (r0, 0);
  __ bne (return_with_exception);

  // Convert the result and resume
  generate_convert_result(CppInterpreter::_tosca_to_stack);
  __ b (resume_interpreter);

  // Handle a return_from_method message ===============================
  __ bind (return_from_method);

  __ load (r0, STATE(_prev_link));
  __ compare (r0, 0);
  __ beq (return_to_initial_caller);

  // "Return" from a re-dispatch -------------------------------------

  generate_convert_result(CppInterpreter::_stack_to_stack);
  generate_unwind_interpreter_state();

  // Resume the interpreter
  __ bind (resume_interpreter);

  __ store (Rlocals, STATE(_stack));
  __ load (Rlocals, STATE(_locals));
  __ load (Rmethod, STATE(_method));
  __ verify_oop(Rmethod);
  __ load (r0, BytecodeInterpreter::method_resume);
  __ stw (r0, STATE(_msg));
  __ b (call_interpreter);

  // Return to the initial caller (call_stub etc) --------------------
  __ bind (return_to_initial_caller);

  generate_convert_result(CppInterpreter::_stack_to_native_abi);
  generate_unwind_interpreter_state();
  __ blr ();

  // Handle a more_monitors message ====================================
  __ bind (more_monitors);

  generate_more_monitors();

  __ load (r0, BytecodeInterpreter::got_monitors);
  __ stw (r0, STATE(_msg));
  __ b (call_interpreter);

  // Handle a throwing_exception message ===============================
  __ bind (throwing_exception);

  // Check we actually have an exception
#ifdef ASSERT
  {
    Label ok;
    __ load (r0, Address(Rthread, Thread::pending_exception_offset()));
    __ compare (r0, 0);
    __ bne (ok);
    __ should_not_reach_here (__FILE__, __LINE__);
    __ bind (ok);
  }
#endif

  // Return to wherever
  generate_unwind_interpreter_state();
  __ bind (return_with_exception);
  __ compare (Rstate, 0);
  __ bne (resume_interpreter);
  __ blr ();

  normal_entry = start;
  return start;
}

// Adjust the caller's stack frame to accomodate any additional
// local variables we have contiguously with our parameters.
// 
// Arguments:
//  Rmethod: address of methodOop
//  Rlocals: address of local variables
//
// Stack layout at entry:
//       | ...                          |
//       +------------------------------+
//   +-> | Link area                    |
//   |   +------------------------------+  -----------------------------
//   |   | Register save area           |                 high addresses
//   |   +------------------------------+  
//   |   | Caller's local variables     |
//   |   +------------------------------+
//   |   | Parameter  0                 |
//   |   |  ...                         |
//   |   | Parameter n-1                |
//   |   +------------------------------+
//   |   | Padding                      |
//   |   +------------------------------+
//   |   | Parameter list space (ppc64) |
//   |   +------------------------------+
//   +---+ Link area                    |                  low addresses
//       +------------------------------+  -----------------------------
//
// Stack layout at exit:
//       | ...                          |
//       +------------------------------+
//   +-> | Link area                    |
//   |   +------------------------------+  -----------------------------
//   |   | Register save area           |                 high addresses
//   |   +------------------------------+  
//   |   | Caller's local variables     |
//   |   +------------------------------+
//   |   | Parameter  0                 | <-- Rlocals
//   |   |  ...                         |
//   |   | Parameter n-1                |
//   |   | Local variable  n            |
//   |   |  ...                         |
//   |   | Local variable m-1           |
//   |   +------------------------------+
//   |   | Padding                      |
//   |   +------------------------------+
//   |   | Parameter list space (ppc64) |
//   |   +------------------------------+
//   +---+ Link area                    |                  low addresses
//       +------------------------------+  -----------------------------

void CppInterpreterGenerator::generate_adjust_callers_stack()
{
  StackFrame frame;

  const int frame_header_size = frame.unaligned_size() + slop_factor;

  const Address param_words_addr(
    Rmethod, methodOopDesc::size_of_parameters_offset());
  const Address local_words_addr(
    Rmethod, methodOopDesc::size_of_locals_offset());

  const Register param_words = r3;
  const Register local_words = r4;

  Label loop, done;

  // Check whether extra locals are actually required
  __ lhz (param_words, param_words_addr);
  __ lhz (local_words, local_words_addr);
  __ compare (param_words, local_words);
  __ beq (done);

  // Extend the frame if necessary
  const Register required_bytes  = r5;
  const Register available_bytes = r6;

  __ shift_left (required_bytes, local_words, LogBytesPerWord);
  __ sub (available_bytes, Rlocals, r1);
  __ subi (available_bytes, available_bytes, frame_header_size);
  __ maybe_extend_frame (required_bytes, available_bytes);
  
  // Zero the extra locals
  const Register dst = r7;

  __ shift_left (dst, param_words, LogBytesPerWord);
  __ sub (dst, Rlocals, dst);
  __ sub (r0, local_words, param_words);
  __ mtctr (r0);
  __ load (r0, 0);
  __ bind (loop);
  __ store (r0, Address(dst, 0));
  __ subi (dst, dst, wordSize);
  __ bdnz (loop);
  
  __ bind (done);
}

// Create a new C++ interpreter stack frame and interpreter state
// object
// 
// Arguments:
//  Rmethod:  address of methodOop
//  Rlocals:  address of local variables
//  Rstate:   previous frame manager state (NULL from call_stub etc)
//
// Returns:
//  Rstate:   current frame manager state
//  CRsync:   whether the method is synchronized
//  Rmonitor: initial monitor (if synchronized)
//  CRstatic: whether the method is static
//
// The frame we create:
//       | ...                          |
//       +------------------------------+
//   +-> | Link area                    |
//   |   +------------------------------+  -----------------------------
//   |   | Interpreter state            | <-- Rstate      high addresses
//   |   |  ...                         |
//   |   |  ...                         |
//   |   +------------------------------+
//   |   | Monitor 0 (if synchronized)  |
//   |   +------------------------------+
//   |   | Expression stack slot 0      |
//   |   |  ...                         |
//   |   | Expression stack slot p-1    |
//   |   +------------------------------+ 
//   |   | Padding                      |
//   |   +------------------------------+
//   |   | Parameter list space (ppc64) |
//   |   +------------------------------+
//   +---+ Link area                    |                  low addresses
//       +------------------------------+  -----------------------------

void CppInterpreterGenerator::generate_compute_interpreter_state(bool native)
{
  StackFrame frame;

  const Address stack_words_addr(
    Rmethod, methodOopDesc::max_stack_offset());
  const Address access_flags_addr(
    Rmethod, methodOopDesc::access_flags_offset());

  Label not_synchronized_1, not_synchronized_2, not_synchronized_3;
  Label not_static, init_monitor;

  const int monitor_size = frame::interpreter_frame_monitor_size() * wordSize;

  // Calculate the access flags conditions
  const Register access_flags = r3;

  __ lwz (access_flags, access_flags_addr);
  __ andi_ (r0, access_flags, JVM_ACC_SYNCHRONIZED);
  __ compare (CRsync, r0, JVM_ACC_SYNCHRONIZED);
  __ andi_ (r0, access_flags, JVM_ACC_STATIC);
  __ compare (CRstatic, r0, JVM_ACC_STATIC);

  const int basic_frame_size =
    frame.unaligned_size() + sizeof(BytecodeInterpreter) + slop_factor;

  // Calculate the frame size
  const Register stack_size = r3;
  const Register frame_size = r4;
  const Register padding    = r5;

  if (native) {
    __ load (frame_size, basic_frame_size);
  }
  else {
    __ lhz (stack_size, stack_words_addr);
    __ shift_left (stack_size, stack_size, LogBytesPerWord);
    __ addi (frame_size, stack_size, basic_frame_size);
  }
  __ bne (CRsync, not_synchronized_1);
  __ addi (frame_size, frame_size, monitor_size);
  __ bind (not_synchronized_1);
  __ calc_padding_for_alignment (padding, frame_size, StackAlignmentInBytes);
  __ add (frame_size, frame_size, padding);

  // Save the link register and create the new frame
  __ mflr (r0);
  __ store (r0, Address(r1, StackFrame::lr_save_offset * wordSize));
  __ neg (r0, frame_size);
  __ store_update_indexed (r1, r1, r0);

  // Calculate everything's addresses
  const Register stack_limit  = r6;
  const Register stack        = r7;
  const Register stack_base   = Rmonitor;
  const Register monitor_base = r8;

  __ addi (stack_limit, r1, frame.start_of_locals() + slop_factor - wordSize);
  __ add (stack_limit, stack_limit, padding);
  if (native)
    __ mr (stack, stack_limit);
  else
    __ add (stack, stack_limit, stack_size);
  __ addi (stack_base, stack, wordSize);
  __ mr (monitor_base, stack_base);
  __ bne (CRsync, not_synchronized_2);
  __ addi (monitor_base, monitor_base, monitor_size);
  __ bind (not_synchronized_2);
  __ mr (r0, Rstate);
  __ mr (Rstate, monitor_base);

  // Initialise the interpreter state object
  __ store (Rlocals, STATE(_locals));
  __ store (Rmethod, STATE(_method));
  __ store (Rstate, STATE(_self_link));
  __ store (r0, STATE(_prev_link));
  __ store (stack_limit, STATE(_stack_limit));
  __ store (stack, STATE(_stack));
  __ store (stack_base, STATE(_stack_base));
  __ store (monitor_base, STATE(_monitor_base));
  __ store (Rthread, STATE(_thread));

#ifdef ASSERT
  {
    Label ok;
    __ load (r3, ThreadLocalStorage::thread_index());
    __ call (CAST_FROM_FN_PTR(address, pthread_getspecific));
    __ compare (Rthread, r3);
    __ beq (ok);
    __ should_not_reach_here (__FILE__, __LINE__);
    __ bind (ok);
  }
#endif

  if (!native) {
    __ load (r3, Address(Rmethod, methodOopDesc::const_offset()));
    __ addi (r3, r3, in_bytes(constMethodOopDesc::codes_offset()));
    __ store (r3, STATE(_bcp));
  }

  __ load (r3, Address(Rmethod, methodOopDesc::constants_offset()));
  __ load (r3, Address(r3, constantPoolOopDesc::cache_offset_in_bytes()));
  __ store (r3, STATE(_constants));

  __ load (r3, BytecodeInterpreter::method_entry);
  __ stw (r3, STATE(_msg)); 

  __ load (r3, 0);
  if (native)
    __ store (r3, STATE(_bcp));    
  __ store (r3, STATE(_oop_temp));
  __ store (r3, STATE(_mdx));
  __ store (r3, STATE(_result._to_call._callee));

  // Initialise the monitor if synchronized
  __ bne (CRsync, not_synchronized_3);
  __ bne (CRstatic, not_static);
  __ get_mirror_handle (r3);  
  __ b (init_monitor);
  __ bind (not_static);
  __ load (r3, Address(Rlocals, 0));
  __ bind (init_monitor);
  __ store (r3, Address(Rmonitor, BasicObjectLock::obj_offset_in_bytes()));
  __ bind (not_synchronized_3);
}

// Adjust the current stack frame to accomodate an additional monitor.
//
// Arguments:
//  Rmethod: address of methodOop
//  Rstate:  frame manager state

void CppInterpreterGenerator::generate_more_monitors()
{
  StackFrame frame;

  const int frame_header_size = frame.unaligned_size() + slop_factor;
  const int monitor_size = frame::interpreter_frame_monitor_size() * wordSize;

  const Address stack_words_addr(Rmethod, methodOopDesc::max_stack_offset());

  Label loop_start, loop_test;

  // Extend the frame if necessary
  const Register required_bytes  = r3;
  const Register available_bytes = r4;

  __ load (required_bytes, frame_header_size + monitor_size);
  __ load (available_bytes, STATE(_stack_limit));
  __ addi (available_bytes, available_bytes, wordSize);
  __ sub (available_bytes, available_bytes, r1);
  __ maybe_extend_frame (required_bytes, available_bytes);

  // Move the expression stack contents
  const Register src = r3;
  const Register end = r4;
  const Register dst = r5;

  __ load (src, STATE(_stack));
  __ addi (src, src, wordSize);
  __ load (end, STATE(_stack_base));
  __ subi (dst, src, monitor_size);
  __ b (loop_test);
  __ bind (loop_start);
  __ load (r0, Address(src, 0));
  __ store (r0, Address(dst, 0));
  __ addi (src, src, wordSize);
  __ addi (dst, dst, wordSize);
  __ bind (loop_test);
  __ compare (src, end);
  __ blt (loop_start);

  // Move the expression stack pointers
  const Register tmp = r3;

  __ load (tmp, STATE(_stack_limit));
  __ subi (tmp, tmp, monitor_size);
  __ store (tmp, STATE(_stack_limit));
  __ load (tmp, STATE(_stack));
  __ subi (tmp, tmp, monitor_size);
  __ store (tmp, STATE(_stack));
  __ load (tmp, STATE(_stack_base));
  __ subi (tmp, tmp, monitor_size);
  __ store (tmp, STATE(_stack_base));

  // Zero the new monitor so the interpreter can find it.
  // NB tmp at this point contains _stack_base, the address
  // of the word after the expression stack -- which just
  // happens to be the address of our new monitor.
  __ load (r0, 0);
  __ store (r0, Address(tmp, BasicObjectLock::obj_offset_in_bytes()));
}

// Convert the a method's return value from one format to another

void CppInterpreterGenerator::generate_convert_result(address* converter_array)
{
  __ load (r5, (intptr_t) converter_array);
  __ lwz (r0, Address(Rmethod, methodOopDesc::result_index_offset()));
  __ shift_left (r0, r0, LogBytesPerWord);
  __ load_indexed (r0, r5, r0);
  __ mtctr (r0);
  __ bctrl ();
}

// Remove the activation created by generate_compute_interpreter_state.
// 
// Arguments:
//  Rstate:  frame manager state
//
// Returns:
//  Rstate:  previous frame manager state (NULL from call_stub etc)

void CppInterpreterGenerator::generate_unwind_interpreter_state()
{
  __ load (Rstate, STATE(_prev_link));
  __ load (r1, Address(r1, StackFrame::back_chain_offset * wordSize));
  __ load (r0, Address(r1, StackFrame::lr_save_offset * wordSize));
  __ mtlr (r0);  
}
#endif // PPC

address AbstractInterpreterGenerator::generate_method_entry(
    AbstractInterpreter::MethodKind kind) {

  address entry_point = NULL;
  bool synchronized = false;

  switch (kind) {
  case Interpreter::zerolocals: 
    break;

  case Interpreter::zerolocals_synchronized: 
    synchronized = true;
    break;

  case Interpreter::native: 
    entry_point = ((InterpreterGenerator*)this)->generate_native_entry(false);
    break;

  case Interpreter::native_synchronized: 
    entry_point = ((InterpreterGenerator*)this)->generate_native_entry(false);
    break;

  case Interpreter::empty: 
    entry_point = ((InterpreterGenerator*)this)->generate_empty_entry();
    break;

  case Interpreter::accessor: 
    entry_point = ((InterpreterGenerator*)this)->generate_accessor_entry();
    break;

  case Interpreter::abstract: 
    entry_point = ((InterpreterGenerator*)this)->generate_abstract_entry();
    break;

  case Interpreter::java_lang_math_sin:
  case Interpreter::java_lang_math_cos:
  case Interpreter::java_lang_math_tan:
  case Interpreter::java_lang_math_abs:
  case Interpreter::java_lang_math_log:
  case Interpreter::java_lang_math_log10:
  case Interpreter::java_lang_math_sqrt: 
    entry_point = ((InterpreterGenerator*)this)->generate_math_entry(kind);
    break;

  default:
    ShouldNotReachHere();
  }

  if (entry_point)
    return entry_point;

  return ((InterpreterGenerator*)this)->generate_normal_entry(false);
}

InterpreterGenerator::InterpreterGenerator(StubQueue* code)
 : CppInterpreterGenerator(code) {
   generate_all(); // down here so it can be "virtual"
}

int AbstractInterpreter::size_top_interpreter_activation(methodOop method)
{
#ifdef PPC
  StackFrame frame;

  int call_stub_frame = round_to(
    StubRoutines::call_stub_base_size() +
    method->max_locals() * wordSize, StackAlignmentInBytes);

  int interpreter_frame = round_to(
    frame.unaligned_size() +
    slop_factor + 
    method->max_stack() * wordSize +
    (method->is_synchronized() ?
     frame::interpreter_frame_monitor_size() * wordSize : 0) +
    sizeof(BytecodeInterpreter), StackAlignmentInBytes);

  return (call_stub_frame + interpreter_frame) / wordSize;
#else
  Unimplemented();
#endif // PPC
}

// Deoptimization helpers for C++ interpreter

int AbstractInterpreter::layout_activation(methodOop method,
					   int tempcount,
					   int popframe_extra_args,
					   int moncount,
					   int callee_param_count,
					   int callee_locals,
					   frame* caller,
					   frame* interpreter_frame,
					   bool is_top_frame)
{
  Unimplemented();
}

address CppInterpreter::return_entry(TosState state, int length)
{
  Unimplemented();
}

address CppInterpreter::deopt_entry(TosState state, int length)
{
  Unimplemented();
}

#endif // CC_INTERP
