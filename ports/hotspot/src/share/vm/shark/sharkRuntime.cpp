/*
 * Copyright 1999-2007 Sun Microsystems, Inc.  All Rights Reserved.
 * Copyright 2008 Red Hat, Inc.
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
#include "incls/_sharkRuntime.cpp.incl"

using namespace llvm;

Constant* SharkRuntime::_newarray;
Constant* SharkRuntime::_new_instance;
Constant* SharkRuntime::_resolve_get_put;
Constant* SharkRuntime::_resolve_invoke;

Constant* SharkRuntime::_dump;
Constant* SharkRuntime::_is_subtype_of;
Constant* SharkRuntime::_should_not_reach_here;
Constant* SharkRuntime::_unimplemented;

void SharkRuntime::initialize(SharkBuilder* builder)
{
  // VM calls
  std::vector<const Type*> params;
  params.push_back(SharkType::thread_type());
  params.push_back(SharkType::jint_type());
  params.push_back(SharkType::jint_type());
  _newarray = builder->make_function(
    (intptr_t) newarray_C,
    FunctionType::get(Type::VoidTy, params, false),
    "SharkRuntime__newarray");

  params.clear();
  params.push_back(SharkType::thread_type());
  params.push_back(SharkType::jobject_type());
  _new_instance = builder->make_function(
    (intptr_t) new_instance_C,
    FunctionType::get(Type::VoidTy, params, false),
    "SharkRuntime__new_instance");

  params.clear();
  params.push_back(SharkType::thread_type());
  params.push_back(SharkType::cpCacheEntry_type());
  params.push_back(SharkType::jint_type());
  params.push_back(SharkType::jint_type());
  _resolve_get_put = builder->make_function(
    (intptr_t) resolve_get_put_C,
    FunctionType::get(Type::VoidTy, params, false),
    "SharkRuntime__resolve_get_put");
  _resolve_invoke = builder->make_function(
    (intptr_t) resolve_invoke_C,
    FunctionType::get(Type::VoidTy, params, false),
    "SharkRuntime__resolve_invoke");

  // Non-VM calls
  params.clear();
  params.push_back(SharkType::intptr_type());
  params.push_back(SharkType::intptr_type());
  _dump = builder->make_function(
    (intptr_t) dump_C,
    FunctionType::get(Type::VoidTy, params, false),
    "SharkRuntime__dump");

  params.clear();
  params.push_back(SharkType::oop_type());
  params.push_back(SharkType::oop_type());
  assert(sizeof(bool) == 1, "fix this");
  _is_subtype_of = builder->make_function(
    (intptr_t) is_subtype_of_C,
    FunctionType::get(Type::Int8Ty, params, false),
    "SharkRuntime__is_subtype_of");

  params.clear();
  params.push_back(SharkType::intptr_type());
  params.push_back(SharkType::jint_type());
  _should_not_reach_here = builder->make_function(
    (intptr_t) report_should_not_reach_here,
    FunctionType::get(Type::VoidTy, params, false),
    "report_should_not_reach_here");
  _unimplemented = builder->make_function(
    (intptr_t) report_unimplemented,
    FunctionType::get(Type::VoidTy, params, false),
    "report_unimplemented");
}

JRT_ENTRY(void, SharkRuntime::newarray_C(JavaThread* thread,
                                         BasicType type,
                                         int size))
{
  oop obj = oopFactory::new_typeArray(type, size, CHECK);
  thread->set_vm_result(obj);
}
JRT_END

JRT_ENTRY(void, SharkRuntime::new_instance_C(JavaThread* thread,
                                             klassOop klass))
{
  // These checks are cheap to make and support reflective allocation.
  int lh = Klass::cast(klass)->layout_helper();
  if (Klass::layout_helper_needs_slow_path(lh)
      || !instanceKlass::cast(klass)->is_initialized()) {
    KlassHandle kh(THREAD, klass);
    kh->check_valid_for_instantiation(false, THREAD);
    if (!HAS_PENDING_EXCEPTION) {
      instanceKlass::cast(kh())->initialize(THREAD);
    }
    if (!HAS_PENDING_EXCEPTION) {
      klass = kh();
    } else {
      klass = NULL;
    }
  }

  if (klass != NULL) {
    // Scavenge and allocate an instance.
    oop result = instanceKlass::cast(klass)->allocate_instance(THREAD);
    thread->set_vm_result(result);
  }
}
JRT_END

JRT_ENTRY(void, SharkRuntime::resolve_get_put_C(JavaThread*             thread,
                                                ConstantPoolCacheEntry* entry,
                                                int                     bci,
                                                Bytecodes::Code      bytecode))
{
  Unimplemented();
}
JRT_END

JRT_ENTRY(void, SharkRuntime::resolve_invoke_C(JavaThread*             thread,
                                               ConstantPoolCacheEntry* entry,
                                               int                     bci,
                                               Bytecodes::Code       bytecode))
{
  // Find the receiver
  Handle receiver(thread, NULL);
  if (bytecode == Bytecodes::_invokevirtual || bytecode == Bytecodes::_invokeinterface) {
    ResourceMark rm(thread);
    methodHandle mh(thread, method(thread));
    Bytecode_invoke *call = Bytecode_invoke_at(mh, bci);
    symbolHandle signature(thread, call->signature());
    ArgumentSizeComputer asc(signature);
    receiver = Handle(thread, (oop) tos_at(thread, asc.size()));
    assert(
      receiver.is_null() ||
      (Universe::heap()->is_in_reserved(receiver()) &&
       Universe::heap()->is_in_reserved(receiver->klass())), "sanity check");
  }

  // Resolve the method
  CallInfo info;
  {
    constantPoolHandle pool(thread, method(thread)->constants());
    JvmtiHideSingleStepping jhss(thread);
    LinkResolver::resolve_invoke(
      info, receiver, pool, two_byte_index(thread, bci), bytecode, CHECK);
    if (JvmtiExport::can_hotswap_or_post_breakpoint()) {
      int retry_count = 0;
      while (info.resolved_method()->is_old()) {
        // It is very unlikely that method is redefined more than 100
        // times in the middle of resolve. If it is looping here more
        // than 100 times means then there could be a bug here.
        guarantee((retry_count++ < 100),
                  "Could not resolve to latest version of redefined method");
        // method is redefined in the middle of resolve so re-try.
        LinkResolver::resolve_invoke(
          info, receiver, pool, two_byte_index(thread, bci), bytecode, CHECK);
      }
    }
  }

  // Update the cache entry if necessary
  if (entry->is_resolved(bytecode))
    return;

  if (bytecode == Bytecodes::_invokeinterface) {
    Unimplemented();
  }
  else {
    entry->set_method(
      bytecode,
      info.resolved_method(),
      info.vtable_index());
  }
}
JRT_END

// Non-VM calls
// Nothing in these must ever GC!

void SharkRuntime::dump_C(const char *name, intptr_t value)
{
  oop valueOop = (oop) value;
  tty->print("%s = ", name);
  if (valueOop->is_oop(true))
    valueOop->print_on(tty);
  else if (value >= ' ' && value <= '~')
    tty->print("'%c' (%d)", value, value);
  else
    tty->print("%p", value);
  tty->print_cr("");
}

bool SharkRuntime::is_subtype_of_C(klassOop check_klass, klassOop object_klass)
{
  return object_klass->klass_part()->is_subtype_of(check_klass);
}
