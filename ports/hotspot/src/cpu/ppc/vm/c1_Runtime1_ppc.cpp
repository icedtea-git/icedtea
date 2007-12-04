/*
 * Copyright 1999-2007 Sun Microsystems, Inc.  All Rights Reserved.
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
#include "incls/_c1_Runtime1_ppc.cpp.incl"

#define __ sasm->

OopMapSet* Runtime1::generate_code_for(StubID id, StubAssembler* sasm)
{
  OopMapSet* oop_maps = NULL;
  switch (id) {
  case forward_exception_id:
    __ unimplemented (__FILE__, __LINE__);

    oop_maps = new OopMapSet();
    sasm->set_frame_size(StackAlignmentInBytes);
    break;

  case new_instance_id:
    __ unimplemented (__FILE__, __LINE__);

    oop_maps = new OopMapSet();
    sasm->set_frame_size(StackAlignmentInBytes);
    break;

  case fast_new_instance_id:
    __ unimplemented (__FILE__, __LINE__);

    oop_maps = new OopMapSet();
    sasm->set_frame_size(StackAlignmentInBytes);
    break;

  case fast_new_instance_init_check_id:
    __ unimplemented (__FILE__, __LINE__);

    oop_maps = new OopMapSet();
    sasm->set_frame_size(StackAlignmentInBytes);
    break;

  case counter_overflow_id:
    __ unimplemented (__FILE__, __LINE__);
    break;

  case new_type_array_id:
    __ unimplemented (__FILE__, __LINE__);

    oop_maps = new OopMapSet();
    sasm->set_frame_size(StackAlignmentInBytes);
    break;

  case new_object_array_id:
    __ unimplemented (__FILE__, __LINE__);

    oop_maps = new OopMapSet();
    sasm->set_frame_size(StackAlignmentInBytes);
    break;

  case new_multi_array_id:
    __ unimplemented (__FILE__, __LINE__);

    oop_maps = new OopMapSet();
    sasm->set_frame_size(StackAlignmentInBytes);
    break;

  case register_finalizer_id:
    __ unimplemented (__FILE__, __LINE__);

    oop_maps = new OopMapSet();
    sasm->set_frame_size(StackAlignmentInBytes);
    break;

  case throw_range_check_failed_id:
    __ unimplemented (__FILE__, __LINE__);

    oop_maps = new OopMapSet();
    sasm->set_frame_size(StackAlignmentInBytes);
    break;

  case throw_index_exception_id:
    __ unimplemented (__FILE__, __LINE__);

    oop_maps = new OopMapSet();
    sasm->set_frame_size(StackAlignmentInBytes);
    break;

  case throw_div0_exception_id:
    __ unimplemented (__FILE__, __LINE__);

    oop_maps = new OopMapSet();
    sasm->set_frame_size(StackAlignmentInBytes);
    break;

  case throw_null_pointer_exception_id:
    __ unimplemented (__FILE__, __LINE__);

    oop_maps = new OopMapSet();
    sasm->set_frame_size(StackAlignmentInBytes);
    break;

  case handle_exception_nofpu_id:
    __ unimplemented (__FILE__, __LINE__);

    oop_maps = new OopMapSet();
    sasm->set_frame_size(StackAlignmentInBytes);
    break;

  case handle_exception_id:
    __ unimplemented (__FILE__, __LINE__);

    oop_maps = new OopMapSet();
    sasm->set_frame_size(StackAlignmentInBytes);
    break;

  case unwind_exception_id:
    __ unimplemented (__FILE__, __LINE__);
    break;

  case throw_array_store_exception_id:
    __ unimplemented (__FILE__, __LINE__);

    oop_maps = new OopMapSet();
    sasm->set_frame_size(StackAlignmentInBytes);
    break;

  case throw_class_cast_exception_id:
    __ unimplemented (__FILE__, __LINE__);

    oop_maps = new OopMapSet();
    sasm->set_frame_size(StackAlignmentInBytes);
    break;

  case throw_incompatible_class_change_error_id:
    __ unimplemented (__FILE__, __LINE__);

    oop_maps = new OopMapSet();
    sasm->set_frame_size(StackAlignmentInBytes);
    break;

  case slow_subtype_check_id:
    __ unimplemented (__FILE__, __LINE__);
    break;

  case monitorenter_nofpu_id:
    __ unimplemented (__FILE__, __LINE__);

    oop_maps = new OopMapSet();
    sasm->set_frame_size(StackAlignmentInBytes);
    break;

  case monitorenter_id:
    __ unimplemented (__FILE__, __LINE__);

    oop_maps = new OopMapSet();
    sasm->set_frame_size(StackAlignmentInBytes);
    break;

  case monitorexit_nofpu_id:
    __ unimplemented (__FILE__, __LINE__);

    oop_maps = new OopMapSet();
    sasm->set_frame_size(StackAlignmentInBytes);
    break;

  case monitorexit_id:
    __ unimplemented (__FILE__, __LINE__);

    oop_maps = new OopMapSet();
    sasm->set_frame_size(StackAlignmentInBytes);
    break;

  case access_field_patching_id:
    __ unimplemented (__FILE__, __LINE__);

    oop_maps = new OopMapSet();
    sasm->set_frame_size(StackAlignmentInBytes);
    break;

  case load_klass_patching_id:
    __ unimplemented (__FILE__, __LINE__);

    oop_maps = new OopMapSet();
    sasm->set_frame_size(StackAlignmentInBytes);
    break;

  case jvmti_exception_throw_id:
    __ unimplemented (__FILE__, __LINE__);

    oop_maps = new OopMapSet();
    sasm->set_frame_size(StackAlignmentInBytes);
    break;

  case dtrace_object_alloc_id:
    __ unimplemented (__FILE__, __LINE__);
    break;

  case fpu2long_stub_id:
    __ unimplemented (__FILE__, __LINE__);
    break;

  default:
    ShouldNotReachHere();
  }
  return oop_maps;
}

void Runtime1::initialize_pd()
{
  // nothing to do
}
