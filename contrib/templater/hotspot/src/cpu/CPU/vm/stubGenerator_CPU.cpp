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
#include "incls/_stubGenerator_@@cpu@@.cpp.incl"

// Declaration and definition of StubGenerator (no .hpp file).
// For a more detailed description of the stub routine structure
// see the comment in stubRoutines.hpp

#ifdef PPC
int StubRoutines::_call_stub_base_size = 0;
#endif // PPC

#define __ _masm->

class StubGenerator: public StubCodeGenerator
{
 private:
#ifdef PPC
  // Call stubs are used to call Java from C
  //
  // Arguments:
  //   r3:   call wrapper address         address
  //   r4:   result                       address
  //   r5:   result type                  BasicType
  //   r6:   method                       methodOop
  //   r7:   (interpreter) entry point    address
  //   r8:   parameters                   intptr_t*
  //   r9:   parameter size (in words)    int
  //   r10:  thread                       Thread*

  // The general layout of stack frames is identical for both ABIs:
  //
  //     | ...                  |
  // +-> | Caller's link area   |  high addresses
  // |   +----------------------+
  // |   | Register save area   |
  // |   | Local variable space |
  // |   | Parameter list space |
  // +---+ Link area            |  low addresses
  //     +----------------------+
  //
  // Specifically:
  //  * The local variable space is where the method stack will go.
  //    At method entry this is populated with the parameters.
  //    The entry then extends this to accomodate locals.
  //  * We don't use the parameter list space at all (though ppc64
  //    requires us to allocate one)
  //  * We do a full register save before we enter the interpreter.
  // 
  // So the specific layouts are as follows:
  //
  //   32-bit ABI:
  //
  //     | ...                            |
  //     +--------------------------------+
  //     | LR save word                   |
  // +-> | Back chain                     |
  // |   +--------------------------------+  -----------------------------    
  // |   | GPR save area              r31 |                 high addresses
  // |   |                            ... |  18 words
  // |   |                            r14 |
  // |   +--------------------------------+
  // |   | CR save area                   |  1 word
  // |   +--------------------------------+
  // |   | Local variable space           |
  // |   |                    result_type |  1 word
  // |   |                         result |  1 word
  // |   |                   call_wrapper |  1 word
  // |   |                    parameter 0 |
  // |   |                          ...   |  n words
  // |   |                  parameter n-1 |
  // |   |                        padding |  0-3 words
  // |   +--------------------------------+
  // |   | LR save word                   |  1 word
  // +---+ Back chain                     |  1 word          low addresses
  //     +--------------------------------+  -----------------------------
  //
  //   64-bit ABI:
  //
  //     | ...                            |
  //     | LR save word                   |
  //     | CR save word                   |
  // +-> | Back chain                     |
  // |   +--------------------------------+  -----------------------------    
  // |   | GPR save area              r31 |                 high addresses
  // |   |                            ... |  18 words
  // |   |                            r14 |
  // |   +--------------------------------+
  // |   | Local variable space           |
  // |   |                    result_type |  1 word
  // |   |                         result |  1 word
  // |   |                   call_wrapper |  1 word
  // |   |                    parameter 0 |
  // |   |                          ...   |  n words
  // |   |                  parameter n-1 |
  // |   |                        padding |  0-1 words
  // |   +--------------------------------+
  // |   | Parameter list space           |  8 (unused) words
  // |   +--------------------------------+
  // |   | TOC save word                  |  1 word
  // |   | Reserved                       |  2 words
  // |   | LR save word                   |  1 word
  // |   | CR save word                   |  1 word
  // +---+ Back chain                     |  1 word          low addresses
  //     +--------------------------------+  -----------------------------

  address generate_call_stub(address& return_address)
  {
    assert (!TaggedStackInterpreter, "not supported");
    
    StubCodeMark mark(this, "StubRoutines", "call_stub");
    address start = __ enter();

    const Register call_wrapper    = r3;
    const Register result          = r4;
    const Register result_type     = r5;
    const Register method          = r6;
    const Register entry_point     = r7;
    const Register parameters      = r8;
    const Register parameter_words = r9;
    const Register thread          = r10;

#ifdef ASSERT
    // Make sure we have no pending exceptions
    {
      StackFrame frame;
      Label label;

      __ load (r0, Address(thread, Thread::pending_exception_offset()));
      __ compare (r0, 0);
      __ beq (label);
      __ prolog (frame);
      __ should_not_reach_here (__FILE__, __LINE__);
      __ epilog (frame);
      __ blr ();
      __ bind (label);
    }
#endif // ASSERT

    // Calculate the frame size
    StackFrame frame;
    for (int i = 0; i < StackFrame::max_crfs; i++)
      frame.get_cr_field();
    for (int i = 0; i < StackFrame::max_gprs; i++)
      frame.get_register();
    StubRoutines::set_call_stub_base_size(frame.unaligned_size() + 3*wordSize);
    // the 3 extra words are for call_wrapper, result and result_type

    const Register parameter_bytes = parameter_words;

    __ shift_left (parameter_bytes, parameter_words, LogBytesPerWord);    

    const Register frame_size = r11;
    const Register padding    = r12;

    __ addi (frame_size, parameter_bytes, StubRoutines::call_stub_base_size());
    __ calc_padding_for_alignment (padding, frame_size, StackAlignmentInBytes);
    __ add (frame_size, frame_size, padding);

    // Save the link register and create the new frame
    __ mflr (r0);
    __ store (r0, Address(r1, StackFrame::lr_save_offset * wordSize));
    __ neg (r0, frame_size);
    __ store_update_indexed (r1, r1, r0);
#ifdef PPC64
    __ mfcr (r0);
    __ store (r0, Address(r1, StackFrame::cr_save_offset * wordSize));
#endif // PPC64

    // Calculate the address of the interpreter's local variables
    const Register locals = frame_size;

    __ addi (locals, r1, frame.start_of_locals() - wordSize);
    __ add (locals, locals, padding);
    __ add (locals, locals, parameter_bytes);

    // Store the call wrapper address and the result stuff
    const int initial_offset = 1;
    int offset = initial_offset;

    __ store (call_wrapper, Address(locals, offset++ * wordSize));
    __ store (result,       Address(locals, offset++ * wordSize));
    __ store (result_type,  Address(locals, offset++ * wordSize));

    // Store the registers
#ifdef PPC32
    __ mfcr (r0);
    __ store (r0, Address(locals, offset++ * wordSize));
#endif // PPC32
    for (int i = 14; i < 32; i++) {
      __ store (as_Register(i), Address(locals, offset++ * wordSize));
    }
    const int final_offset = offset;

    // Store the location of call_wrapper
    frame::set_call_wrapper_offset((final_offset - initial_offset) * wordSize);

#ifdef ASSERT
    // Check that we wrote all the way to the end of the frame.
    // The frame may have been resized when we return from the
    // interpreter, so the start of the frame may have moved
    // but the end will be where we left it and we rely on this
    // to find our stuff.
    {
      StackFrame frame;
      Label label;

      __ load (r3, Address(r1, 0));
      __ subi (r3, r3, final_offset * wordSize);
      __ compare (r3, locals);
      __ beq (label);
      __ prolog (frame);
      __ should_not_reach_here (__FILE__, __LINE__);
      __ epilog (frame);
      __ blr ();
      __ bind (label);
    }
#endif // ASSERT

    // Pass parameters if any
    {
      Label loop, done;

      __ compare (parameter_bytes, 0);
      __ ble (done);

      const Register src = parameters;
      const Register dst = padding;

      __ mr (dst, locals);
      __ shift_right (r0, parameter_bytes, LogBytesPerWord);      
      __ mtctr (r0);
      __ bind (loop);
      __ load (r0, Address(src, 0));
      __ store (r0, Address(dst, 0));
      __ addi (src, src, wordSize);
      __ subi (dst, dst, wordSize);
      __ bdnz (loop);

      __ bind (done);
    }

    // Make the call
    __ mr (Rmethod, method);
    __ mr (Rlocals, locals);
    __ mr (Rthread, thread);
    __ mtctr (entry_point);
    __ bctrl();

    // This is used to identify call_stub stack frames
    return_address = __ pc();

    // Figure out where our stuff is stored
    __ load (locals, Address(r1, 0));
    __ subi (locals, locals, final_offset * wordSize);

#ifdef ASSERT
    // Rlocals should contain the address we just calculated.
    {
      StackFrame frame;
      Label label;

      __ compare (Rlocals, locals);
      __ beq (label);
      __ prolog (frame);
      __ should_not_reach_here (__FILE__, __LINE__);
      __ epilog (frame);
      __ blr ();
      __ bind (label);
    }
#endif // ASSERT
 
    // Is an exception being thrown?
    Label exit;

    __ load (r0, Address(Rthread, Thread::pending_exception_offset()));
    __ compare (r0, 0);
    __ bne (exit);

    // Store result depending on type
    const Register result_addr = r6;

    Label is_int, is_long, is_object;

    offset = initial_offset + 1; // skip call_wrapper
    __ load (result_addr, Address(locals, offset++ * wordSize));
    __ load (result_type, Address(locals, offset++ * wordSize));
    __ compare (result_type, T_INT);
    __ beq (is_int);
    __ compare (result_type, T_LONG);
    __ beq (is_long);
    __ compare (result_type, T_OBJECT);
    __ beq (is_object);
    
    __ should_not_reach_here (__FILE__, __LINE__);

    __ bind (is_int);
    __ stw (r3, Address(result_addr, 0));
    __ b (exit);
    
    __ bind (is_long);
#ifdef PPC32
    __ store (r4, Address(result_addr, wordSize));
#endif
    __ store (r3, Address(result_addr, 0));
    __ b (exit);
    
    __ bind (is_object);
    __ store (r3, Address(result_addr, 0));
    //__ b (exit);

    // Restore the registers
    __ bind (exit);
#ifdef PPC32
    __ load (r0, Address(locals, offset++ * wordSize));
    __ mtcr (r0);
#endif // PPC32
    for (int i = 14; i < 32; i++) {
      __ load (as_Register(i), Address(locals, offset++ * wordSize));
    }
#ifdef PPC64
    __ load (r0, Address(r1, StackFrame::cr_save_offset * wordSize));
    __ mtcr (r0);
#endif // PPC64
    assert (offset == final_offset, "save and restore must match");

    // Unwind and return
    __ load (r1, Address(r1, StackFrame::back_chain_offset * wordSize));
    __ load (r0, Address(r1, StackFrame::lr_save_offset * wordSize));
    __ mtlr (r0);
    __ blr ();
    
    return start;
  }
#endif // PPC

  // These stubs get called from some dumb test routine.
  // I'll write them properly when they're called from
  // something that's actually doing something.
  address generate_arraycopy_stub(const char *name, int line)
  {
#ifdef PPC
    StubCodeMark mark(this, "StubRoutines", name);
    address start = __ enter();

    const Register from  = r3;  // source array address
    const Register to    = r4;  // destination array address
    const Register count = r5;  // element count

    __ compare (count, 0);
    __ beqlr ();
    __ unimplemented (__FILE__, line);
    __ blr ();

    return start;
#else
    return UnimplementedStub();
#endif // PPC
  }

  void generate_arraycopy_stubs()
  {
    // Call the conjoint generation methods immediately after
    // the disjoint ones so that short branches from the former
    // to the latter can be generated.
    StubRoutines::_jbyte_disjoint_arraycopy =
      generate_arraycopy_stub("jbyte_disjoint_arraycopy", __LINE__);      
    StubRoutines::_jbyte_arraycopy =
      generate_arraycopy_stub("jbyte_arraycopy", __LINE__);

    StubRoutines::_jshort_disjoint_arraycopy =
      generate_arraycopy_stub("jshort_disjoint_arraycopy", __LINE__);      
    StubRoutines::_jshort_arraycopy =
      generate_arraycopy_stub("jshort_arraycopy", __LINE__);

    StubRoutines::_jint_disjoint_arraycopy =
      generate_arraycopy_stub("jint_disjoint_arraycopy", __LINE__);      
    StubRoutines::_jint_arraycopy =
      generate_arraycopy_stub("jint_arraycopy", __LINE__);

    StubRoutines::_jlong_disjoint_arraycopy =
      generate_arraycopy_stub("jlong_disjoint_arraycopy", __LINE__);      
    StubRoutines::_jlong_arraycopy =
      generate_arraycopy_stub("jlong_arraycopy", __LINE__);

    StubRoutines::_oop_disjoint_arraycopy    = UnimplementedStub();
    StubRoutines::_oop_arraycopy             = UnimplementedStub();

    StubRoutines::_checkcast_arraycopy       = UnimplementedStub();
    StubRoutines::_unsafe_arraycopy          = UnimplementedStub();
    StubRoutines::_generic_arraycopy         = UnimplementedStub();

    // We don't generate specialized code for HeapWord-aligned source
    // arrays, so just use the code we've already generated
    StubRoutines::_arrayof_jbyte_disjoint_arraycopy =
      StubRoutines::_jbyte_disjoint_arraycopy;
    StubRoutines::_arrayof_jbyte_arraycopy =
      StubRoutines::_jbyte_arraycopy;

    StubRoutines::_arrayof_jshort_disjoint_arraycopy =
      StubRoutines::_jshort_disjoint_arraycopy;
    StubRoutines::_arrayof_jshort_arraycopy =
      StubRoutines::_jshort_arraycopy;

    StubRoutines::_arrayof_jint_disjoint_arraycopy =
      StubRoutines::_jint_disjoint_arraycopy;
    StubRoutines::_arrayof_jint_arraycopy =
      StubRoutines::_jint_arraycopy;

    StubRoutines::_arrayof_jlong_disjoint_arraycopy =
      StubRoutines::_jlong_disjoint_arraycopy;
    StubRoutines::_arrayof_jlong_arraycopy =
      StubRoutines::_jlong_arraycopy;

    StubRoutines::_arrayof_oop_disjoint_arraycopy =
      StubRoutines::_oop_disjoint_arraycopy;
    StubRoutines::_arrayof_oop_arraycopy =
      StubRoutines::_oop_arraycopy;
  }

  void generate_initial()
  {
    // Generates all stubs and initializes the entry points

    // entry points that exist in all platforms Note: This is code
    // that could be shared among different platforms - however the
    // benefit seems to be smaller than the disadvantage of having a
    // much more complicated generator structure. See also comment in
    // stubRoutines.hpp.

    StubRoutines::_forward_exception_entry   = UnimplementedStub();
#ifdef PPC
    StubRoutines::_call_stub_entry =
      generate_call_stub(StubRoutines::_call_stub_return_address);
#else
    StubRoutines::_call_stub_entry           = UnimplementedStub();
#endif // PPC
    StubRoutines::_catch_exception_entry     = UnimplementedStub();

    // atomic calls
    StubRoutines::_atomic_xchg_entry         = UnimplementedStub();
    StubRoutines::_atomic_xchg_ptr_entry     = UnimplementedStub();
    StubRoutines::_atomic_cmpxchg_entry      = UnimplementedStub();
    StubRoutines::_atomic_cmpxchg_ptr_entry  = UnimplementedStub();
    StubRoutines::_atomic_cmpxchg_long_entry = UnimplementedStub();
    StubRoutines::_atomic_add_entry          = UnimplementedStub();
    StubRoutines::_atomic_add_ptr_entry      = UnimplementedStub();
    StubRoutines::_fence_entry               = UnimplementedStub();

    // amd64 does this here, sparc does it in generate_all()
    StubRoutines::_handler_for_unsafe_access_entry =
      UnimplementedStub();
  }

  void generate_all()
  {
    // Generates all stubs and initializes the entry points
    
    // These entry points require SharedInfo::stack0 to be set up in
    // non-core builds and need to be relocatable, so they each
    // fabricate a RuntimeStub internally.
    StubRoutines::_throw_AbstractMethodError_entry =
      UnimplementedStub();

    StubRoutines::_throw_ArithmeticException_entry =
      UnimplementedStub();

    StubRoutines::_throw_NullPointerException_entry =
      UnimplementedStub();

    StubRoutines::_throw_NullPointerException_at_call_entry =
      UnimplementedStub();

    StubRoutines::_throw_StackOverflowError_entry =
      UnimplementedStub();

    // support for verify_oop (must happen after universe_init)
    StubRoutines::_verify_oop_subroutine_entry =
      UnimplementedStub();

    // arraycopy stubs used by compilers
    generate_arraycopy_stubs();
  }

 public:
  StubGenerator(CodeBuffer* code, bool all) : StubCodeGenerator(code)
  { 
    if (all) {
      generate_all();
    } else {
      generate_initial();
    }
  }
};

void StubGenerator_generate(CodeBuffer* code, bool all)
{
  StubGenerator g(code, all);
}
