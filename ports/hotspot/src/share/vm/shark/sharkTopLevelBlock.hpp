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
    : SharkBlock(function->builder(),
                 function->target(),
                 function->iter(),
                 function->thread()),
      _function(function),
      _ciblock(ciblock),
      _entered(false),
      _has_trap(false),
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

  // Typeflow properties
 public:
  int index() const
  {
    return ciblock()->pre_order();
  }
  bool is_backedge_copy() const
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
  bool _has_trap;
  int  _trap_request;
  int  _trap_bci;

  void set_trap(int trap_request, int trap_bci)
  {
    assert(!has_trap(), "shouldn't have");
    _has_trap     = true;
    _trap_request = trap_request;
    _trap_bci     = trap_bci;
  }

 private:
  bool has_trap()
  {
    return _has_trap;
  }
  int trap_request()
  {
    assert(has_trap(), "should have");
    return _trap_request;
  }
  int trap_bci()
  {
    assert(has_trap(), "should have");
    return _trap_bci;
  }

 private:
  void scan_for_traps();

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

  // Temporary oop storage
 public:
  void set_oop_tmp(llvm::Value* value)
  {
    assert(value, "value must be non-NULL (will be reset by get_oop_tmp)");
    assert(!current_state()->oop_tmp(), "oop_tmp gets and sets must match");
    current_state()->set_oop_tmp(value);
  }
  llvm::Value* get_oop_tmp()
  {
    llvm::Value* value = current_state()->oop_tmp();
    assert(value, "oop_tmp gets and sets must match");
    current_state()->set_oop_tmp(NULL);
    return value;
  }

  // Monitors
 private:
  int num_monitors()
  {
    return current_state()->num_monitors();
  }
  int set_num_monitors(int num_monitors)
  {
    current_state()->set_num_monitors(num_monitors);
  }

  // Code generation
 public:
  void emit_IR();

  // Branch helpers
 private:
  void do_branch(int successor_index);

  // Zero checks
 private:
  void do_zero_check(SharkValue* value);
  void zero_check_value(SharkValue* value, llvm::BasicBlock* continue_block);

 public:
  void do_deferred_zero_check(SharkValue*       value,
                              int               bci,
                              SharkState*       saved_state,
                              llvm::BasicBlock* continue_block);
  // Exceptions
  enum ExceptionActionMask {
    // The actual bitmasks that things test against
    EAM_CHECK         = 1, // whether to check for pending exceptions
    EAM_HANDLE        = 2, // whether to attempt to handle pending exceptions
    EAM_MONITOR_FUDGE = 4, // whether the monitor count needs adjusting

    // More convenient values for passing
    EX_CHECK_NONE     = 0,
    EX_CHECK_NO_CATCH = EAM_CHECK,
    EX_CHECK_FULL     = EAM_CHECK | EAM_HANDLE
  };
  void check_pending_exception(int action);
  void handle_exception(llvm::Value* exception, int action);

  // VM calls
 private:
  llvm::CallInst* call_vm(llvm::Constant* callee,
                          llvm::Value**   args_start,
                          llvm::Value**   args_end,
                          int             exception_action)
  {
    current_state()->decache_for_VM_call();
    function()->set_last_Java_frame();
    llvm::CallInst *res = builder()->CreateCall(callee, args_start, args_end);
    function()->reset_last_Java_frame();
    current_state()->cache_after_VM_call();
    if (exception_action & EAM_CHECK) {
      check_pending_exception(exception_action);
      current_state()->set_has_safepointed(true);
    }
    return res;
  }

 public:
  llvm::CallInst* call_vm(llvm::Constant* callee,
                          int             exception_action)
  {
    llvm::Value *args[] = {thread()};
    return call_vm(callee, args, args + 1, exception_action);
  }
  llvm::CallInst* call_vm(llvm::Constant* callee,
                          llvm::Value*    arg1,
                          int             exception_action)
  {
    llvm::Value *args[] = {thread(), arg1};
    return call_vm(callee, args, args + 2, exception_action);
  }
  llvm::CallInst* call_vm(llvm::Constant* callee,
                          llvm::Value*    arg1,
                          llvm::Value*    arg2,
                          int             exception_action)
  {
    llvm::Value *args[] = {thread(), arg1, arg2};
    return call_vm(callee, args, args + 3, exception_action);
  }
  llvm::CallInst* call_vm(llvm::Constant* callee,
                          llvm::Value*    arg1,
                          llvm::Value*    arg2,
                          llvm::Value*    arg3,
                          int             exception_action)
  {
    llvm::Value *args[] = {thread(), arg1, arg2, arg3};
    return call_vm(callee, args, args + 4, exception_action);
  }

  // Synchronization
 private:
  void acquire_lock(llvm::Value* lockee, int exception_action);
  void release_lock(int exception_action);

 public:
  void acquire_method_lock();

  // Bounds checks
 private:
  void check_bounds(SharkValue* array, SharkValue* index);

  // Safepoints
 private:
  void maybe_add_safepoint();
  void maybe_add_backedge_safepoint();

  // Loop safepoint removal
 private:
  bool _can_reach_visited;

  bool can_reach(SharkTopLevelBlock* other);
  bool can_reach_helper(SharkTopLevelBlock* other);

  // Traps
 private:
  void do_trap(int trap_request);

  // Returns
 private:
  void call_register_finalizer(llvm::Value* receiver);
  void handle_return(BasicType type, llvm::Value* exception);

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
  void do_if_helper(llvm::ICmpInst::Predicate p,
                    llvm::Value*              b,
                    llvm::Value*              a,
                    SharkState*               if_taken_state,
                    SharkState*               not_taken_state);
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
  bool static_subtype_check(ciKlass* check_klass, ciKlass* object_klass);
  void do_full_instance_check(ciKlass* klass);
  void do_trapping_instance_check(ciKlass* klass);

  void do_instance_check();
  bool maybe_do_instanceof_if();

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
