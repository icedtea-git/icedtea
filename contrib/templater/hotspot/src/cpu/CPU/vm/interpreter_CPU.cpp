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
#include "incls/_interpreter_@@cpu@@.cpp.incl"

#define __ _masm->

address AbstractInterpreterGenerator::generate_slow_signature_handler()
{
#ifdef PPC
  address start = __ pc();

  const Address param_words_addr(
    Rmethod, methodOopDesc::size_of_parameters_offset());

  const Register required_bytes  = r11;
  const Register available_bytes = r12;
  const Register register_images = r14;

  // Calculate the size of the parameter list space.  We can't
  // know how much we'll need before parsing the signature so
  // we allocate for the worst case.  Note that this calculation
  // is repeated in InterpreterRuntime::slow_signature_handler.
  __ lhz (required_bytes, param_words_addr);
#ifdef PPC32
  // We can always fit at least three parameters in registers
  __ subi (required_bytes, required_bytes, 3);

  // Floats get converted to doubles and may need aligning
  __ shift_left (r0, required_bytes, 1);
  __ add (required_bytes, required_bytes, r0);
#else
  // Allocate space for JNIEnv and a possible mirror handle.
  __ addi (required_bytes, required_bytes, 2);
#endif
  __ shift_left (required_bytes, required_bytes, LogBytesPerWord);  
  __ mr (register_images, required_bytes);
  
  // Add to this the size of the local variable space
  __ addi (required_bytes, required_bytes,
	   (InterpreterRuntime::gp_reg_max + 1 - 
	    InterpreterRuntime::gp_reg_start) * wordSize +
	   (InterpreterRuntime::fp_reg_max + 1 - 
	    InterpreterRuntime::fp_reg_start) * 8);

  // Extend the frame
#ifdef CC_INTERP
  // The area we have to play with is the area between the link
  // area and the expression stack (which has zero size, this
  // being a native method).  Note that we treat any slop_factor
  // as available space here.  Note also that this is duplicated
  // in InterpreterRuntime::SignatureHandlerGenerator::generate.
  __ load (available_bytes,
	   Address(Rstate, BytecodeInterpreter::stack_limit_offset()));
  __ addi (available_bytes, available_bytes,
	   (1 - StackFrame::link_area_words) * wordSize);
  __ sub (available_bytes, available_bytes, r1);
#else
  Unimplemented();
#endif
  __ maybe_extend_frame (required_bytes, available_bytes);

  // Fill in the parameter list space and register images
  __ la (r6, Address(r1, StackFrame::link_area_words * wordSize));
  __ call_VM (noreg,
              CAST_FROM_FN_PTR(address,
                               InterpreterRuntime::slow_signature_handler),
              Rmethod, Rlocals, r6,
              CALL_VM_PRESERVE_LR);

  // Load the register images into the registers
  const Register src = r11;

  __ addi (src, r1, StackFrame::link_area_words * wordSize);
  __ add (src, src, register_images);

  int offset = 0;
  for (int i = InterpreterRuntime::gp_reg_start;
       i <= InterpreterRuntime::gp_reg_max; i++) {
    __ load (as_Register(i), Address(src, offset));
    offset += wordSize;
  }
  for (int i = InterpreterRuntime::fp_reg_start;
       i <= InterpreterRuntime::fp_reg_max; i++) {
    __ lfd (as_FloatRegister(i), Address(src, offset));
    offset += 8;
  }

  // Return the result handler
  __ mr (r0, r3);
  __ blr ();

  return start;
#else
  Unimplemented();
#endif // PPC
}

address InterpreterGenerator::generate_math_entry(
    AbstractInterpreter::MethodKind kind)
{
  if (!InlineIntrinsics)
    return NULL;

#ifdef PPC
  address start = __ pc();

  switch (kind) {
  case Interpreter::java_lang_math_sin:
    __ unimplemented (__FILE__, __LINE__);
    break;

  case Interpreter::java_lang_math_cos:
    __ unimplemented (__FILE__, __LINE__);
    break;

  case Interpreter::java_lang_math_tan:
    __ unimplemented (__FILE__, __LINE__);
    break;

  case Interpreter::java_lang_math_abs:
    __ unimplemented (__FILE__, __LINE__);
    break;

  case Interpreter::java_lang_math_log:
    __ unimplemented (__FILE__, __LINE__);
    break;

  case Interpreter::java_lang_math_log10:
    __ unimplemented (__FILE__, __LINE__);
    break;

  case Interpreter::java_lang_math_sqrt: 
    __ unimplemented (__FILE__, __LINE__);
    break;

  default:
    ShouldNotReachHere();
  }

  return start;
#else
  Unimplemented();
#endif // PPC
}

address InterpreterGenerator::generate_abstract_entry()
{
  return UnimplementedEntry();
}

int AbstractInterpreter::size_activation(methodOop method,
                                         int tempcount,
                                         int popframe_extra_args,
                                         int moncount,
                                         int callee_param_count,
                                         int callee_locals,
                                         bool is_top_frame)
{
  Unimplemented();
}

void Deoptimization::unwind_callee_save_values(frame* f,
					       vframeArray* vframe_array)
{
  Unimplemented();
}
