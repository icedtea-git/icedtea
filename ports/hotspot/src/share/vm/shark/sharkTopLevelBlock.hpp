/*
 * Copyright 1999-2007 Sun Microsystems, Inc.  All Rights Reserved.
 * Copyright 2008, 2009 Red Hat, Inc.
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

class SharkTopLevelBlock : public SharkBlock {
 public:
  SharkTopLevelBlock(SharkFunction* function, ciTypeFlow::Block* ciblock)
    : SharkBlock(function->builder(), function->target(), function->iter()),
      _function(function),
      _ciblock(ciblock),
      _trap_request(TRAP_UNCHECKED),
      _entered(false),
      _needs_phis(false),
      _entry_state(NULL),
      _entry_block(NULL) {}

 private:
  SharkFunction*     _function;
  ciTypeFlow::Block* _ciblock;

 public:
  SharkFunction* function() const
  {
    return _function;
  }
  ciTypeFlow::Block* ciblock() const
  {
    return _ciblock;
  }

 public:
  llvm::Value* thread() const
  {
    return function()->thread();
  }

  // Typeflow properties
 public:
  int index() const
  {
    return ciblock()->pre_order();
  }
  bool is_private_copy() const
  {
    return ciblock()->is_backedge_copy();
  }
  int stack_depth_at_entry() const
  {
    return ciblock()->stack_size();
  }
  ciType* local_type_at_entry(int index) const
  {
    return ciblock()->local_type_at(index);
  }
  ciType* stack_type_at_entry(int slot) const
  {
    return ciblock()->stack_type_at(slot);
  }  
  int start() const
  {
    return ciblock()->start();
  }
  int limit() const
  {
    return ciblock()->limit();
  }
  bool falls_through() const
  {
    return ciblock()->control() == ciBlock::fall_through_bci;
  }
  int num_exceptions() const
  {
    return ciblock()->exceptions()->length();
  }
  SharkTopLevelBlock* exception(int index) const
  {
    return function()->block(ciblock()->exceptions()->at(index)->pre_order());
  }
  int num_successors() const
  {
    return ciblock()->successors()->length();
  }
  SharkTopLevelBlock* successor(int index) const
  {
    return function()->block(ciblock()->successors()->at(index)->pre_order());
  }
  SharkTopLevelBlock* bci_successor(int bci) const;

  // Traps
 private:
  enum {
    TRAP_UNCHECKED = 232323, // > any constant pool index
    TRAP_NO_TRAPS
  };
  int _trap_request;

 public:
  int trap_request()
  {
    if (_trap_request == TRAP_UNCHECKED)
      _trap_request = scan_for_traps();
    return _trap_request;
  }
  bool has_trap()
  {
    return trap_request() != TRAP_NO_TRAPS;
  }

 private:
  int scan_for_traps();

  // Entry state
 private:
  bool _entered;
  bool _needs_phis;

 public:
  bool entered() const
  {
    return _entered;
  }
  bool needs_phis() const
  {
    return _needs_phis;
  }

 private:
  void enter(SharkTopLevelBlock* predecessor, bool is_exception);

 public:
  void enter()
  {
    enter(NULL, false);
  }

 private:
  SharkState* _entry_state;

 private:
  SharkState* entry_state();

 private:
  llvm::BasicBlock* _entry_block;

 public:
  llvm::BasicBlock* entry_block() const
  {
    return _entry_block;
  }

 public:
  void initialize();

 public:
  void add_incoming(SharkState* incoming_state);

  // Method
 public:
  llvm::Value* method()
  {
    return current_state()->method();
  }

  // Code generation
 public:
  void emit_IR();

  // Helpers
 private:
  void do_zero_check(SharkValue* value);
  llvm::Value* lookup_for_ldc();
  llvm::Value* lookup_for_field_access();

  // VM calls
 private:
  llvm::CallInst* call_vm_nocheck(llvm::Constant* callee,
                                  llvm::Value**   args_start,
                                  llvm::Value**   args_end)
  {
    current_state()->decache_for_VM_call();
    function()->set_last_Java_frame();
    llvm::CallInst *res = builder()->CreateCall(callee, args_start, args_end);
    function()->reset_last_Java_frame();
    current_state()->cache_after_VM_call();
    return res;
  }

  llvm::CallInst* call_vm(llvm::Constant* callee,
                          llvm::Value**   args_start,
                          llvm::Value**   args_end)
  {
    llvm::CallInst* res = call_vm_nocheck(callee, args_start, args_end);
    check_pending_exception();
    return res;
  }

 public:
  llvm::CallInst* call_vm(llvm::Constant* callee)
  {
    llvm::Value *args[] = {thread()};
    return call_vm(callee, args, args + 1);
  }
  llvm::CallInst* call_vm(llvm::Constant* callee,
                          llvm::Value*    arg1)
  {
    llvm::Value *args[] = {thread(), arg1};
    return call_vm(callee, args, args + 2);
  }
  llvm::CallInst* call_vm(llvm::Constant* callee,
                          llvm::Value*    arg1,
                          llvm::Value*    arg2)
  {
    llvm::Value *args[] = {thread(), arg1, arg2};
    return call_vm(callee, args, args + 3);
  }
  llvm::CallInst* call_vm(llvm::Constant* callee,
                          llvm::Value*    arg1,
                          llvm::Value*    arg2,
                          llvm::Value*    arg3)
  {
    llvm::Value *args[] = {thread(), arg1, arg2, arg3};
    return call_vm(callee, args, args + 4);
  }

  llvm::CallInst* call_vm_nocheck(llvm::Constant* callee)
  {
    llvm::Value *args[] = {thread()};
    return call_vm_nocheck(callee, args, args + 1);
  }
  llvm::CallInst* call_vm_nocheck(llvm::Constant* callee,
                                  llvm::Value*    arg1)
  {
    llvm::Value *args[] = {thread(), arg1};
    return call_vm_nocheck(callee, args, args + 2);
  }
  llvm::CallInst* call_vm_nocheck(llvm::Constant* callee,
                                  llvm::Value*    arg1,
                                  llvm::Value*    arg2)
  {
    llvm::Value *args[] = {thread(), arg1, arg2};
    return call_vm_nocheck(callee, args, args + 3);
  }
  llvm::CallInst* call_vm_nocheck(llvm::Constant* callee,
                                  llvm::Value*    arg1,
                                  llvm::Value*    arg2,
                                  llvm::Value*    arg3)
  {
    llvm::Value *args[] = {thread(), arg1, arg2, arg3};
    return call_vm_nocheck(callee, args, args + 4);
  }

  // Whole-method synchronization
 public:
  void acquire_method_lock();  
  void release_method_lock();  

  // Error checking
 private:
  void check_bounds(SharkValue* array, SharkValue* index);
  void check_pending_exception(bool attempt_catch = true);
  void handle_exception(llvm::Value* exception, bool attempt_catch = true);

  // Safepoints
 private:
  void add_safepoint();

  // Returns
 private:
  void call_register_finalizer(llvm::Value* receiver);
  void handle_return(BasicType type, llvm::Value* exception);
  void release_locked_monitors();

  // arraylength
 private:
  void do_arraylength();

  // *aload and *astore
 private:
  void do_aload(BasicType basic_type);
  void do_astore(BasicType basic_type);

  // *return and athrow
 private:
  void do_return(BasicType type);
  void do_athrow();

  // goto*
 private:
  void do_goto();

  // jsr* and ret
 private:
  void do_jsr();
  void do_ret();

  // if*
 private:
  void do_if(llvm::ICmpInst::Predicate p, SharkValue* b, SharkValue* a);

  // tableswitch and lookupswitch
 private:
  void do_switch();

  // invoke*
 private:
  enum CallType {
    CALL_DIRECT,
    CALL_VIRTUAL,
    CALL_INTERFACE
  };
  CallType get_call_type(ciMethod* method);
  llvm::Value* get_callee(CallType    call_type,
                          ciMethod*   method,
                          SharkValue* receiver);

  llvm::Value* get_direct_callee(ciMethod* method);
  llvm::Value* get_virtual_callee(SharkValue* receiver, ciMethod* method);

  llvm::Value* get_virtual_callee(llvm::Value* cache, SharkValue* receiver);
  llvm::Value* get_interface_callee(SharkValue* receiver);

  void do_call();

  // checkcast and instanceof
 private:
  void do_instance_check();

  // new and *newarray
 private:
  void do_new();
  void do_newarray();
  void do_anewarray();
  void do_multianewarray();

  // monitorenter and monitorexit
 private:
  void do_monitorenter();
  void do_monitorexit();
};
