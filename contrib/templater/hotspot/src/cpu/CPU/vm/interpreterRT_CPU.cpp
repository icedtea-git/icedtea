/*
 * Copyright 2003-2005 Sun Microsystems, Inc.  All Rights Reserved.
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
#include "incls/_interpreterRT_@@cpu@@.cpp.incl"

#define __ _masm->

// Implementation of SignatureHandlerGenerator

void InterpreterRuntime::SignatureHandlerGenerator::pass_int()
{
#ifdef PPC
  const Address src(Rlocals, Interpreter::local_offset_in_bytes(offset()));

  if (_gp_reg <= gp_reg_max) {
    __ lwa (as_Register(_gp_reg++), src);
#ifdef PPC64
    _st_arg++;
#endif
  }
  else {
    pass_on_stack(src, T_INT);
  }
#else
  Unimplemented();
#endif // PPC
}

void InterpreterRuntime::SignatureHandlerGenerator::pass_long()
{
#ifdef PPC
#ifdef PPC32
  const Address srch(Rlocals, Interpreter::local_offset_in_bytes(offset()));
  const Address srcl(Rlocals, Interpreter::local_offset_in_bytes(offset()+1));

  if (_gp_reg < gp_reg_max) {
    if (!(_gp_reg & 1))
      _gp_reg++;
    __ lwz (as_Register(_gp_reg++), srcl);
    __ lwz (as_Register(_gp_reg++), srch);
  }
  else {
    pass_on_stack(srcl, T_LONG);
  }
#else
  const Address src(Rlocals, Interpreter::local_offset_in_bytes(offset() + 1));

  if (_gp_reg <= gp_reg_max) {
    __ ld (as_Register(_gp_reg++), src);
    _st_arg++;
  }
  else {
    pass_on_stack(src, T_LONG);
  }
#endif // PPC32
#else
  Unimplemented();
#endif // PPC
}

void InterpreterRuntime::SignatureHandlerGenerator::pass_float()
{
#ifdef PPC
  const Address src(Rlocals, Interpreter::local_offset_in_bytes(offset()));

  if (_fp_reg <= fp_reg_max) {
    __ lfs (as_FloatRegister(_fp_reg++), src);
#ifdef PPC64
    _st_arg++;
#endif
  }
  else {
    pass_on_stack(src, T_FLOAT);
  }
#else
  Unimplemented();
#endif // PPC
}

void InterpreterRuntime::SignatureHandlerGenerator::pass_double()
{
#ifdef PPC
  const Address src(Rlocals, Interpreter::local_offset_in_bytes(offset() + 1));

  if (_fp_reg <= fp_reg_max) {
    __ lfd (as_FloatRegister(_fp_reg++), src);
#ifdef PPC64
    _st_arg++;
#endif
  }
  else {
    pass_on_stack(src, T_DOUBLE);
  }
#else
  Unimplemented();
#endif // PPC
}

void InterpreterRuntime::SignatureHandlerGenerator::pass_object()
{
#ifdef PPC
  const Address src(Rlocals, Interpreter::local_offset_in_bytes(offset()));

  if (_gp_reg <= gp_reg_max) {
    const Register dst = as_Register(_gp_reg++);
    Label null;
    
    __ load (dst, src);
    __ compare (dst, 0);
    __ beq (null);
    __ la (dst, src);
    __ bind (null);

#ifdef PPC64
    _st_arg++;
#endif
  }
  else {
    pass_on_stack(src, T_OBJECT);
  }
#else
  Unimplemented();
#endif // PPC
}

#ifdef PPC
void InterpreterRuntime::SignatureHandlerGenerator::pass_on_stack(
  const Address& src, BasicType type)
{
#ifdef PPC32
  if (_st_arg & 1) {
    if (type == T_LONG || type == T_FLOAT || type == T_DOUBLE)
      _st_arg++;
  }
#endif // PPC32

  assert(src.base() == Rlocals, "what?");
  _st_args->append(StackArgument(
    _masm,
    src.displacement(),
    (StackFrame::link_area_words + _st_arg++) * wordSize,
    type));

#ifdef PPC32
  if (type == T_LONG || type == T_FLOAT || type == T_DOUBLE)
    _st_arg++;
#endif // PPC32
}

void InterpreterRuntime::StackArgument::pass()
{
  const Address src(Rlocals, _src_offset);
  const Address dst(r1, _dst_offset);
  Label null;
  
  switch (_type) {
    case T_INT:
      __ lwa (r0, src);
      __ store (r0, dst);
      break;

    case T_LONG:
#ifdef PPC32
      __ lwz (r0, src);
      __ stw (r0, dst);
      __ lwz (r0, Address(Rlocals, _src_offset + wordSize));
      __ stw (r0, Address(r1, _dst_offset + wordSize));
#else 
      __ ld (r0, src);
      __ std (r0, dst);
#endif
      break;

    case T_FLOAT:
#ifdef PPC32
      __ lfs (f0, src);
      __ stfd (f0, dst);
#else
      __ lwz (r0, src);
      __ stw (r0, Address(r1, _dst_offset + wordSize));
#endif
      break;

    case T_DOUBLE:
      __ lfd (f0, src);
      __ stfd (f0, dst);
      break;

    case T_OBJECT:
      __ load (r0, src);
      __ compare (r0, 0);
      __ beq (null);
      __ la (r0, src);
      __ bind (null);
      __ store (r0, dst);
      break;

  default:
    ShouldNotReachHere();
  }
}

#endif // PPC
void InterpreterRuntime::SignatureHandlerGenerator::generate(
  uint64_t fingerprint) 
{
#ifdef PPC
  // Generate code to handle register arguments
  iterate(fingerprint);

  // Generate code to handle stack arguments
  if (_st_arg > StackFrame::min_params) {
    const Register required_bytes  = r11;
    const Register available_bytes = r12;

    __ load (required_bytes, _st_arg * wordSize);
#ifdef CC_INTERP
    // The area we have to play with is the area between the link
    // area and the expression stack (which has zero size, this
    // being a native method).  Note that we treat any slop_factor
    // as available space here.  Note also that this is duplicated
    // in AbstractInterpreterGenerator::generate_slow_signature_handler.
    __ load (available_bytes,
	     Address(Rstate, BytecodeInterpreter::stack_limit_offset()));
    __ addi (available_bytes, available_bytes,
	     (1 - StackFrame::link_area_words) * wordSize);
    __ sub (available_bytes, available_bytes, r1);
#else
    Unimplemented();
#endif
    __ maybe_extend_frame (required_bytes, available_bytes);

    for (int i = 0; i < _st_args->length(); i++)
      _st_args->at(i).pass();
  }

  // Return result handler
  __ load (r0, (intptr_t)Interpreter::result_handler(method()->result_type()));
  __ blr ();

  __ flush ();
#else
  Unimplemented();
#endif // PPC
}
#ifdef PPC


// Implementation of SlowSignatureHandler

void InterpreterRuntime::SlowSignatureHandler::pass_int()
{
  jint src = *(jint *)
    (_from + Interpreter::local_offset_in_bytes(offset()));

  if (_gp_regs <= _gp_reg_max) {
    *(jint *)(_gp_regs++) = src;
#ifdef PPC64
    _st_args++;
#endif
  }
  else {
    *(jint *)(_st_args++) = src;
  }
}

void InterpreterRuntime::SlowSignatureHandler::pass_long()
{
#ifdef PPC32
  intptr_t srch = *(intptr_t *)
    (_from + Interpreter::local_offset_in_bytes(offset()));
  intptr_t srcl = *(intptr_t *)
    (_from + Interpreter::local_offset_in_bytes(offset() + 1));

  if (_gp_regs < _gp_reg_max) {
    if (!((_gp_reg_max - _gp_regs) & 1))
      _gp_regs++;
    *(_gp_regs++) = srcl;
    *(_gp_regs++) = srch;
  }
  else {
    *(_st_args++) = srcl;
    *(_st_args++) = srch;    
  }
#else
  intptr_t src = *(intptr_t *)
    (_from + Interpreter::local_offset_in_bytes(offset() + 1));

  if (_gp_regs <= _gp_reg_max) {
    *(_gp_regs++) = src;
    _st_args++;
  }
  else {
    *(_st_args++) = src;
  }
#endif  
}

void InterpreterRuntime::SlowSignatureHandler::pass_float()
{
  Unimplemented();
}

void InterpreterRuntime::SlowSignatureHandler::pass_double()
{
  Unimplemented();
}

void InterpreterRuntime::SlowSignatureHandler::pass_object()
{
  intptr_t *src = (intptr_t *)
    (_from + Interpreter::local_offset_in_bytes(offset()));

  if (*src == 0)
    src = NULL;

  if (_gp_regs <= _gp_reg_max) {
    *(intptr_t **)(_gp_regs++) = src;
#ifdef PPC64
    _st_args++;
#endif
  }
  else {
    *(intptr_t **)(_st_args++) = src;
  }
}

IRT_ENTRY(address, 
          InterpreterRuntime::slow_signature_handler(JavaThread* thread,
                                                     methodOopDesc* method,
                                                     intptr_t* from,
                                                     intptr_t* to))
  methodHandle m(thread, (methodOop) method);
  assert(m->is_native(), "sanity check");

  // Handle arguments
  intptr_t *st_args = to;
#ifdef PPC32
  intptr_t *gp_regs = st_args + (method->size_of_parameters() - 3) * 3;
#else
  intptr_t *gp_regs = st_args + (method->size_of_parameters() + 2);
#endif
  intptr_t *fp_regs = gp_regs + (gp_reg_max + 1 - gp_reg_start);

  SlowSignatureHandler(m,
		       (address) from,
		       gp_regs,
		       (double *) fp_regs,
		       st_args).iterate(UCONST64(-1));

  // Return result handler
  return Interpreter::result_handler(m->result_type());
IRT_END

#endif // PPC

// Implementation of SignatureHandlerLibrary

void SignatureHandlerLibrary::pd_set_handler(address handler) {}
