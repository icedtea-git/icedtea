/*
 * Copyright 1999-2006 Sun Microsystems, Inc.  All Rights Reserved.
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
#include "incls/_c1_CodeStubs_ppc.cpp.incl"

#define __ ce->masm()->

RangeCheckStub::RangeCheckStub(CodeEmitInfo* info, LIR_Opr index,
                               bool throw_index_out_of_bounds_exception)
  : _throw_index_out_of_bounds_exception(throw_index_out_of_bounds_exception),
    _index(index)
{
  _info = new CodeEmitInfo(info);
}

void RangeCheckStub::emit_code(LIR_Assembler* ce)
{
  Unimplemented();
}

void DivByZeroStub::emit_code(LIR_Assembler* ce)
{
  Unimplemented();
}

NewInstanceStub::NewInstanceStub(LIR_Opr klass_reg, LIR_Opr result,
                                 ciInstanceKlass* klass, CodeEmitInfo* info,
                                 Runtime1::StubID stub_id)
{
  _result = result;
  _klass = klass;
  _klass_reg = klass_reg;
  _info = new CodeEmitInfo(info);
  assert(stub_id == Runtime1::new_instance_id ||
         stub_id == Runtime1::fast_new_instance_id ||
         stub_id == Runtime1::fast_new_instance_init_check_id,
         "need new_instance id");
  _stub_id = stub_id;
}

void NewInstanceStub::emit_code(LIR_Assembler* ce)
{
  __ unimplemented (__FILE__, __LINE__);
}

MonitorEnterStub::MonitorEnterStub(LIR_Opr obj_reg, LIR_Opr lock_reg,
                                   CodeEmitInfo* info)
  : MonitorAccessStub(obj_reg, lock_reg)
{
  Unimplemented();
}

void MonitorEnterStub::emit_code(LIR_Assembler* ce)
{
  Unimplemented();
}

void MonitorExitStub::emit_code(LIR_Assembler* ce)
{
  Unimplemented();
}

#ifdef XXX_EVIL_EVIL_EVIL
int PatchingStub::_patch_info_offset = 0;
#endif

void ImplicitNullCheckStub::emit_code(LIR_Assembler* ce)
{
  Unimplemented();
}

ArrayStoreExceptionStub::ArrayStoreExceptionStub(CodeEmitInfo* info)
  : _info(info)
{
  Unimplemented();
}

void ArrayStoreExceptionStub::emit_code(LIR_Assembler* ce)
{
  Unimplemented();
}

void ArrayCopyStub::emit_code(LIR_Assembler* ce)
{
  Unimplemented();
}
