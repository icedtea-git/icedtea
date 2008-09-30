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

Constant* SharkRuntime::_find_exception_handler;
Constant* SharkRuntime::_monitorenter;
Constant* SharkRuntime::_monitorexit;
Constant* SharkRuntime::_new_instance;
Constant* SharkRuntime::_newarray;
Constant* SharkRuntime::_anewarray;
Constant* SharkRuntime::_multianewarray;
Constant* SharkRuntime::_resolve_get_put;
Constant* SharkRuntime::_resolve_invoke;
Constant* SharkRuntime::_resolve_klass;
Constant* SharkRuntime::_safepoint;
Constant* SharkRuntime::_throw_ArrayIndexOutOfBoundsException;
Constant* SharkRuntime::_throw_NullPointerException;
Constant* SharkRuntime::_trace_bytecode;

Constant* SharkRuntime::_dump;
Constant* SharkRuntime::_is_subtype_of;
Constant* SharkRuntime::_should_not_reach_here;
Constant* SharkRuntime::_unimplemented;
Constant* SharkRuntime::_uncommon_trap;

void SharkRuntime::initialize(SharkBuilder* builder)
{
  // VM calls
  std::vector<const Type*> params;
  params.push_back(SharkType::thread_type());
  params.push_back(PointerType::getUnqual(SharkType::jint_type()));
  params.push_back(SharkType::jint_type());
  _find_exception_handler = builder->make_function(
    (intptr_t) find_exception_handler_C,
    FunctionType::get(SharkType::jint_type(), params, false),
    "SharkRuntime__find_exception_handler");

  params.clear();
  params.push_back(SharkType::thread_type());
  params.push_back(PointerType::getUnqual(SharkType::monitor_type()));
  _monitorenter = builder->make_function(
    (intptr_t) monitorenter_C,
    FunctionType::get(Type::VoidTy, params, false),
    "SharkRuntime__monitorenter");
  _monitorexit = builder->make_function(
    (intptr_t) monitorexit_C,
    FunctionType::get(Type::VoidTy, params, false),
    "SharkRuntime__monitorexit");

  params.clear();
  params.push_back(SharkType::thread_type());
  params.push_back(SharkType::jint_type());
  _new_instance = builder->make_function(
    (intptr_t) new_instance_C,
    FunctionType::get(Type::VoidTy, params, false),
    "SharkRuntime__new_instance");
  _resolve_klass = builder->make_function(
    (intptr_t) resolve_klass_C,
    FunctionType::get(Type::VoidTy, params, false),
    "SharkRuntime__resolve_klass");

  params.clear();
  params.push_back(SharkType::thread_type());
  params.push_back(SharkType::jint_type());
  params.push_back(SharkType::jint_type());
  _newarray = builder->make_function(
    (intptr_t) newarray_C,
    FunctionType::get(Type::VoidTy, params, false),
    "SharkRuntime__newarray");
  _anewarray = builder->make_function(
    (intptr_t) anewarray_C,
    FunctionType::get(Type::VoidTy, params, false),
    "SharkRuntime__anewarray");

  params.clear();
  params.push_back(SharkType::thread_type());
  params.push_back(SharkType::jint_type());
  params.push_back(SharkType::jint_type());
  params.push_back(PointerType::getUnqual(SharkType::jint_type()));
  _multianewarray = builder->make_function(
    (intptr_t) multianewarray_C,
    FunctionType::get(Type::VoidTy, params, false),
    "SharkRuntime__multianewarray");

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

  params.clear();
  params.push_back(SharkType::thread_type());
  _safepoint = builder->make_function(
    (intptr_t) SafepointSynchronize::block,
    FunctionType::get(Type::VoidTy, params, false),
    "SafepointSynchronize__block");

  params.clear();
  params.push_back(SharkType::thread_type());
  params.push_back(SharkType::intptr_type());
  params.push_back(SharkType::jint_type());
  params.push_back(SharkType::jint_type());
  _throw_ArrayIndexOutOfBoundsException = builder->make_function(
    (intptr_t) throw_ArrayIndexOutOfBoundsException_C,
    FunctionType::get(Type::VoidTy, params, false),
    "SharkRuntime__throw_ArrayIndexOutOfBoundsException");

  params.clear();
  params.push_back(SharkType::thread_type());
  params.push_back(SharkType::intptr_type());
  params.push_back(SharkType::jint_type());
  _throw_NullPointerException = builder->make_function(
    (intptr_t) throw_NullPointerException_C,
    FunctionType::get(Type::VoidTy, params, false),
    "SharkRuntime__throw_NullPointerException");

  params.clear();
  params.push_back(SharkType::thread_type());
  params.push_back(SharkType::jint_type());
  params.push_back(SharkType::intptr_type());
  params.push_back(SharkType::intptr_type());
  _trace_bytecode = builder->make_function(
    (intptr_t) trace_bytecode_C,
    FunctionType::get(Type::VoidTy, params, false),
    "SharkRuntime__trace_bytecode");

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

  params.clear();
  params.push_back(SharkType::thread_type());
  params.push_back(SharkType::jint_type());
  _uncommon_trap = builder->make_function(
    (intptr_t) uncommon_trap_C,
    FunctionType::get(Type::VoidTy, params, false),
    "SharkRuntime__uncommon_trap");
}

JRT_ENTRY(int, SharkRuntime::find_exception_handler_C(JavaThread* thread,
                                                      int*        indexes,
                                                      int         num_indexes))
{
  constantPoolHandle pool(thread, method(thread)->constants());
  KlassHandle exc_klass(thread, ((oop) tos_at(thread, 0))->klass());

  for (int i = 0; i < num_indexes; i++) {
    klassOop tmp = pool->klass_at(indexes[i], CHECK_0);
    KlassHandle chk_klass(thread, tmp);

    if (exc_klass() == chk_klass())
      return i;

    if (exc_klass()->klass_part()->is_subtype_of(chk_klass()))
      return i;
  }

  return -1;
}
JRT_END

JRT_ENTRY(void, SharkRuntime::monitorenter_C(JavaThread*      thread,
                                             BasicObjectLock* lock))
{
  if (PrintBiasedLockingStatistics)
    Atomic::inc(BiasedLocking::slow_path_entry_count_addr());

  Handle object(thread, lock->obj());  
  assert(Universe::heap()->is_in_reserved_or_null(object()), "should be");
  if (UseBiasedLocking) {
    // Retry fast entry if bias is revoked to avoid unnecessary inflation
    ObjectSynchronizer::fast_enter(object, lock->lock(), true, CHECK);
  } else {
    ObjectSynchronizer::slow_enter(object, lock->lock(), CHECK);
  }
  assert(Universe::heap()->is_in_reserved_or_null(lock->obj()), "should be");
}
JRT_END

JRT_ENTRY(void, SharkRuntime::monitorexit_C(JavaThread*      thread,
                                            BasicObjectLock* lock))
{
  Handle object(thread, lock->obj());  
  assert(Universe::heap()->is_in_reserved_or_null(object()), "should be");
  if (lock == NULL || object()->is_unlocked()) {
    THROW(vmSymbols::java_lang_IllegalMonitorStateException());
  }
  ObjectSynchronizer::slow_exit(object(), lock->lock(), thread);

  // Free entry. This must be done here, since a pending exception
  // might be installed on exit. If it is not cleared, the exception
  // handling code will try to unlock the monitor again.
  lock->set_obj(NULL); 
}
JRT_END
  
JRT_ENTRY(void, SharkRuntime::new_instance_C(JavaThread* thread, int index))
{
  klassOop k_oop = method(thread)->constants()->klass_at(index, CHECK);
  instanceKlassHandle klass(THREAD, k_oop);
  
  // Make sure we are not instantiating an abstract klass
  klass->check_valid_for_instantiation(true, CHECK);

  // Make sure klass is initialized
  klass->initialize(CHECK);    

  // At this point the class may not be fully initialized
  // because of recursive initialization. If it is fully
  // initialized & has_finalized is not set, we rewrite
  // it into its fast version (Note: no locking is needed
  // here since this is an atomic byte write and can be
  // done more than once).
  //
  // Note: In case of classes with has_finalized we don't
  //       rewrite since that saves us an extra check in
  //       the fast version which then would call the
  //       slow version anyway (and do a call back into
  //       Java).
  //       If we have a breakpoint, then we don't rewrite
  //       because the _breakpoint bytecode would be lost.
  oop obj = klass->allocate_instance(CHECK);
  thread->set_vm_result(obj);  
}
JRT_END

JRT_ENTRY(void, SharkRuntime::newarray_C(JavaThread* thread,
                                         BasicType   type,
                                         int         size))
{
  oop obj = oopFactory::new_typeArray(type, size, CHECK);
  thread->set_vm_result(obj);
}
JRT_END

JRT_ENTRY(void, SharkRuntime::anewarray_C(JavaThread* thread,
                                          int         index,
                                          int         size))
{
  klassOop klass = method(thread)->constants()->klass_at(index, CHECK);
  objArrayOop obj = oopFactory::new_objArray(klass, size, CHECK);
  thread->set_vm_result(obj);
}
JRT_END

JRT_ENTRY(void, SharkRuntime::multianewarray_C(JavaThread* thread,
                                               int         index,
                                               int         ndims,
                                               int*        dims))
{
  klassOop klass = method(thread)->constants()->klass_at(index, CHECK);
  oop obj = arrayKlass::cast(klass)->multi_allocate(ndims, dims, CHECK);
  thread->set_vm_result(obj);
}
JRT_END

JRT_ENTRY(void, SharkRuntime::resolve_get_put_C(JavaThread*             thread,
                                                ConstantPoolCacheEntry* entry,
                                                int                     bci,
                                                Bytecodes::Code      bytecode))
{
  // Resolve the field
  FieldAccessInfo info;
  {
    constantPoolHandle pool(thread, method(thread)->constants());
    JvmtiHideSingleStepping jhss(thread);
    LinkResolver::resolve_field(
      info, pool, two_byte_index(thread, bci), bytecode, false, CHECK);
  }

  // Check if link resolution caused the cache to be updated
  if (entry->is_resolved(bytecode))
    return;
  
  // Compute auxiliary field attributes
  TosState state = as_TosState(info.field_type());

  // We need to delay resolving put instructions on final fields
  // until we actually invoke one. This is required so we throw
  // exceptions at the correct place. If we do not resolve completely
  // in the current pass, leaving the put_code set to zero will
  // cause the next put instruction to reresolve.
  bool is_put =
    (bytecode == Bytecodes::_putfield || bytecode == Bytecodes::_putstatic);
  Bytecodes::Code put_code = (Bytecodes::Code) 0;

  // We also need to delay resolving getstatic instructions until the
  // class is intitialized.  This is required so that access to the
  // static field will call the initialization function every time
  // until the class is completely initialized as per 2.17.5 in JVM
  // Specification.
  instanceKlass *klass = instanceKlass::cast(info.klass()->as_klassOop());
  bool is_static =
    (bytecode == Bytecodes::_getstatic || bytecode == Bytecodes::_putstatic);
  bool uninitialized_static = (is_static && !klass->is_initialized());
  Bytecodes::Code get_code = (Bytecodes::Code) 0;

  if (!uninitialized_static) {
    get_code = ((is_static) ? Bytecodes::_getstatic : Bytecodes::_getfield);
    if (is_put || !info.access_flags().is_final()) {
      put_code = ((is_static) ? Bytecodes::_putstatic : Bytecodes::_putfield);
    }
  }

  // Update the cache entry
  entry->set_field(
    get_code,
    put_code,
    info.klass(),
    info.field_index(),
    info.field_offset(),
    state,
    info.access_flags().is_final(),
    info.access_flags().is_volatile());
}
JRT_END

JRT_ENTRY(void, SharkRuntime::resolve_invoke_C(JavaThread*             thread,
                                               ConstantPoolCacheEntry* entry,
                                               int                     bci,
                                               Bytecodes::Code       bytecode))
{
  // Find the receiver
  Handle receiver(thread, NULL);
  if (bytecode == Bytecodes::_invokevirtual ||
      bytecode == Bytecodes::_invokeinterface) {
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

  // Check if link resolution caused the cache to be updated
  if (entry->is_resolved(bytecode))
    return;

  // Update the cache entry
  methodHandle rm = info.resolved_method();
  if (bytecode == Bytecodes::_invokeinterface) {
    if (rm->method_holder() == SystemDictionary::object_klass()) {
      // Workaround for the case where we encounter an invokeinterface,
      // but should really have an invokevirtual since the resolved
      // method is a virtual method in java.lang.Object. This is a
      // corner case in the spec but is presumably legal, and while
      // javac does not generate this code there's no reason it could
      // not be produced by a compliant java compiler.  See
      // cpCacheOop.cpp for more details.
      assert(rm->is_final() || info.has_vtable_index(), "should be set");
      entry->set_method(bytecode, rm, info.vtable_index()); 
    }
    else {
      entry->set_interface_call(rm, klassItable::compute_itable_index(rm()));
    }
  }
  else {
    entry->set_method(bytecode, rm, info.vtable_index());
  }
}
JRT_END

JRT_ENTRY(void, SharkRuntime::resolve_klass_C(JavaThread* thread, int index))
{
  klassOop klass = method(thread)->constants()->klass_at(index, CHECK);
  thread->set_vm_result(klass);
}
JRT_END

JRT_ENTRY(void, SharkRuntime::throw_ArrayIndexOutOfBoundsException_C(
                                                     JavaThread* thread,
                                                     const char* file,
                                                     int         line,
                                                     int         index))
{
  char msg[jintAsStringSize];
  snprintf(msg, sizeof(msg), "%d", index);
  Exceptions::_throw_msg(
    thread, file, line, 
    vmSymbols::java_lang_ArrayIndexOutOfBoundsException(),
    msg);
}
JRT_END

JRT_ENTRY(void, SharkRuntime::throw_NullPointerException_C(JavaThread* thread,
                                                           const char* file,
                                                           int         line))
{
  Exceptions::_throw_msg(
    thread, file, line, 
    vmSymbols::java_lang_NullPointerException(),
    "");
}
JRT_END

JRT_ENTRY(void, SharkRuntime::trace_bytecode_C(JavaThread* thread,
                                               int         bci,
                                               intptr_t    tos,
                                               intptr_t    tos2))
{
#ifndef PRODUCT
  methodHandle mh(thread, method(thread));
  //BytecodeCounter::_counter_value++;
  BytecodeTracer::trace(mh, mh->code_base() + bci, tos, tos2);
#endif // !PRODUCT
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

void SharkRuntime::uncommon_trap_C(JavaThread* thread, int index)
{
  // In C2, uncommon_trap_blob creates a frame, so all the various
  // deoptimization functions expect to find the frame of the method
  // being deopted one frame down on the stack.  Create a dummy frame
  // to mirror this.
  ZeroStack *stack = thread->zero_stack();
  thread->push_zero_frame(DeoptimizerFrame::build(stack));

  // Initiate the trap
  thread->set_last_Java_frame();
  Deoptimization::UnrollBlock *urb =
    Deoptimization::uncommon_trap(thread, index);
  thread->reset_last_Java_frame();

  // Pop our dummy frame and the frame being deoptimized
  thread->pop_zero_frame();
  thread->pop_zero_frame();

  // Push skeleton frames
  int number_of_frames = urb->number_of_frames();
  for (int i = 0; i < number_of_frames; i++) {
    intptr_t size = urb->frame_sizes()[i];
    thread->push_zero_frame(InterpreterFrame::build(stack, size));
  }

  // Push another dummy frame
  thread->push_zero_frame(DeoptimizerFrame::build(stack));
  
  // Fill in the skeleton frames
  thread->set_last_Java_frame();
  Deoptimization::unpack_frames(thread, Deoptimization::Unpack_uncommon_trap);
  thread->reset_last_Java_frame();

  // Pop our dummy frame
  thread->pop_zero_frame();

  // Jump into the interpreter  
#ifdef CC_INTERP
  CppInterpreter::main_loop(number_of_frames - 1, thread);
#else
  Unimplemented();
#endif // CC_INTERP
}

DeoptimizerFrame* DeoptimizerFrame::build(ZeroStack* stack)
{
  if (header_words > stack->available_words()) {
    Unimplemented();
  }

  stack->push(0); // next_frame, filled in later
  intptr_t *fp = stack->sp();
  assert(fp - stack->sp() == next_frame_off, "should be");

  stack->push(DEOPTIMIZER_FRAME);
  assert(fp - stack->sp() == frame_type_off, "should be");

  return (DeoptimizerFrame *) fp;
}
