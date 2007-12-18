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
#include "incls/_sharedRuntime_@@cpu@@.cpp.incl"

DeoptimizationBlob *SharedRuntime::_deopt_blob;
SafepointBlob      *SharedRuntime::_polling_page_safepoint_handler_blob;
SafepointBlob      *SharedRuntime::_polling_page_return_handler_blob;
RuntimeStub        *SharedRuntime::_wrong_method_blob;
RuntimeStub        *SharedRuntime::_ic_miss_blob;
RuntimeStub        *SharedRuntime::_resolve_opt_virtual_call_blob;
RuntimeStub        *SharedRuntime::_resolve_virtual_call_blob;
RuntimeStub        *SharedRuntime::_resolve_static_call_blob;

#define __ masm->

#ifdef PPC
// Read the array of BasicTypes from a signature, and compute where
// the arguments should go.  Values in the VMRegPair regs array refer
// to 4-byte quantities.  XXX describe the mapping

// Note that the INPUTS in sig_bt are in units of Java argument words,
// which are either 32-bit or 64-bit depending on the build.  The
// OUTPUTS are in 32-bit units regardless of build.

// XXX I'm not very confident I have all the set1/set2's and ++/+=2's right

#endif // PPC
int SharedRuntime::java_calling_convention(const BasicType *sig_bt,
                                           VMRegPair *regs,
                                           int total_args_passed,
                                           int is_outgoing)
{
#ifdef PPC
  static const Register int_arg_register[8] = {
    r3, r4, r5, r6, r7, r8, r9, r10
  };
  const int int_reg_max = 8;

#ifdef PPC32
  static const FloatRegister fp_arg_register[8] = {
    f1, f2, f3, f4, f5, f6, f7, f8
  };
  const int fp_reg_max = 8;
#else
  static const FloatRegister fp_arg_register[13] = {
    f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13
  };
  const int fp_reg_max = 13;
#endif // PPC32

  int int_args = 0;
  int fp_args  = 0;
  int stk_args = 0;

  const int slots_per_reg = wordSize / 4;
  
  for (int i = 0; i < total_args_passed; i++) {
    switch (sig_bt[i]) {
    case T_BOOLEAN:
    case T_CHAR:
    case T_BYTE:
    case T_SHORT:
    case T_INT:
#ifdef PPC32
    case T_OBJECT:
    case T_ARRAY:
    case T_ADDRESS:
#endif // PPC32
      if (int_args < int_reg_max)
	regs[i].set1(int_arg_register[int_args++]->as_VMReg());
      else
	regs[i].set1(VMRegImpl::stack2reg(stk_args += slots_per_reg));
      break;

    case T_LONG:
      assert(sig_bt[i + 1] == T_VOID, "expecting void in other half");
#ifdef PPC32
      if (int_args < int_reg_max - 1) {
	if (int_args & 1)
	  int_args++;
	regs[i].set2(int_arg_register[int_args += 2]->as_VMReg());
      }
      else {
	if (stk_args & 1)
	  stk_args++;
	regs[i].set2(VMRegImpl::stack2reg(stk_args += 2));
      }
      break;
#else
      // fall through
    case T_OBJECT:
    case T_ARRAY:
    case T_ADDRESS:
      if (int_args < int_reg_max)
	regs[i].set2(int_arg_register[int_args++]->as_VMReg());
      else
	regs[i].set2(VMRegImpl::stack2reg(stk_args += 2));
      break;
#endif // PPC32

    case T_FLOAT:
      if (fp_args < fp_reg_max)
	regs[i].set1(fp_arg_register[fp_args++]->as_VMReg());
      else
	regs[i].set1(VMRegImpl::stack2reg(stk_args += slots_per_reg));
      break;

    case T_DOUBLE:
      assert(sig_bt[i + 1] == T_VOID, "expecting void in other half");
      if (fp_args < fp_reg_max)
	regs[i].set2(fp_arg_register[fp_args++]->as_VMReg());
      else {
	if (stk_args & 1)
	  stk_args++;
	regs[i].set2(VMRegImpl::stack2reg(stk_args += 2));
      }
      break;

    case T_VOID:
      assert(i != 0 && (sig_bt[i - 1] == T_LONG || sig_bt[i - 1] == T_DOUBLE),
	     "expecting long or double in other half");
      regs[i].set_bad();
      break;

    default:
      ShouldNotReachHere();
    }
  }
  return stk_args;
#else
  Unimplemented();
#endif // PPC
}

AdapterHandlerEntry* SharedRuntime::generate_i2c2i_adapters(
			MacroAssembler *masm,
			int total_args_passed,
			int comp_args_on_stack,
			const BasicType *sig_bt,
			const VMRegPair *regs)
{
  address i2c_entry = UnimplementedStub();
  address c2i_entry = UnimplementedStub();
  address c2i_unverified_entry = UnimplementedStub();
  return new AdapterHandlerEntry(i2c_entry, c2i_entry, c2i_unverified_entry);
}

nmethod *SharedRuntime::generate_native_wrapper(MacroAssembler *masm,
                                                methodHandle method,
                                                int total_in_args,
                                                int comp_args_on_stack,
                                                BasicType *in_sig_bt,
                                                VMRegPair *in_regs,
                                                BasicType ret_type)
{
  Unimplemented();
}

int Deoptimization::last_frame_adjust(int callee_parameters, int callee_locals)
{
  Unimplemented();
}

uint SharedRuntime::out_preserve_stack_slots()
{
#ifdef PPC
#ifdef XXX_EVIL_EVIL_EVIL
  return 0;
#else
  Unimplemented();
#endif
#else
  Unimplemented();
#endif // PPC
}

static RuntimeStub* generate_unimplemented_runtime_stub(const char* file,
							int line,
							const char* name)
{
#ifdef PPC
  ResourceMark rm;
  CodeBuffer buffer(name, 1000, 512);
  MacroAssembler* masm = new MacroAssembler(&buffer);
  int frame_size_in_words = 0; // XXX
  OopMapSet *oop_maps = new OopMapSet();
  StackFrame frame = StackFrame();
  __ enter();
  __ prolog(frame);
  __ unimplemented(file, line);
  __ epilog(frame);
  __ blr();
  int frame_complete = __ offset(); // XXX
  masm->flush();
  return RuntimeStub::new_runtime_stub(name, &buffer, frame_complete,
				       frame_size_in_words, oop_maps, true);
#else
  Unimplemented();
#endif // PPC
}

static SafepointBlob* generate_unimplemented_safepoint_blob(const char* file,
							    int line)
{
#ifdef PPC
  ResourceMark rm;
  CodeBuffer buffer("handler_blob", 2048, 1024);
  MacroAssembler* masm = new MacroAssembler(&buffer);
  int frame_size_in_words = 0; // XXX
  OopMapSet *oop_maps = new OopMapSet();
  StackFrame frame = StackFrame();
  __ enter();
  __ prolog(frame);
  __ unimplemented(file, line);
  __ epilog(frame);
  __ blr();
  masm->flush();
  return SafepointBlob::create(&buffer, oop_maps, frame_size_in_words);
#else
  Unimplemented();
#endif // PPC
}

void SharedRuntime::generate_stubs()
{
  _wrong_method_blob =
    generate_unimplemented_runtime_stub(__FILE__, __LINE__,
					"wrong_method_stub");
  _ic_miss_blob =
    generate_unimplemented_runtime_stub(__FILE__, __LINE__,
					"ic_miss_stub");
  _resolve_opt_virtual_call_blob =
    generate_unimplemented_runtime_stub(__FILE__, __LINE__,
					"resolve_opt_virtual_call");
  _resolve_virtual_call_blob =
    generate_unimplemented_runtime_stub(__FILE__, __LINE__,
					"resolve_virtual_call");
  _resolve_static_call_blob =
    generate_unimplemented_runtime_stub(__FILE__, __LINE__,
					"resolve_static_call");

  _polling_page_safepoint_handler_blob =
    generate_unimplemented_safepoint_blob(__FILE__, __LINE__);
  _polling_page_return_handler_blob =
    generate_unimplemented_safepoint_blob(__FILE__, __LINE__);
}

int SharedRuntime::c_calling_convention(const BasicType *sig_bt,
                                         VMRegPair *regs,
                                         int total_args_passed)
{
  Unimplemented();
}
