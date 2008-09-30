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

class SharkBlock : public ResourceObj {
 public:
  SharkBlock(SharkFunction* function, ciTypeFlow::Block* ciblock)
    : _function(function),
      _ciblock(ciblock),
      _entered(false),
      _needs_phis(false),
      _entry_state(NULL),
      _entry_block(NULL),
      _current_state(NULL) {}

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
  SharkBuilder* builder() const
  {
    return function()->builder();
  }
  ciMethod* target() const
  {
    return function()->target();
  }
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
  bool has_trap() const
  {
    return ciblock()->has_trap();
  }
  int trap_index() const
  {
    return ciblock()->trap_index();
  }
  bool is_private_copy() const
  {
    return ciblock()->is_private_copy();
  }
  int max_locals() const
  {
    return ciblock()->outer()->max_locals();
  }
  int max_stack() const
  {
    return ciblock()->outer()->max_stack();
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
  SharkBlock* exception(int index) const
  {
    return function()->block(ciblock()->exceptions()->at(index)->pre_order());
  }
  int num_successors() const
  {
    return ciblock()->successors()->length();
  }
  SharkBlock* successor(int index) const
  {
    return function()->block(ciblock()->successors()->at(index)->pre_order());
  }
  int jsr_ret_bci() const
  {
    int jsr_level = ciblock()->jsrset()->size();
    assert(jsr_level > 0, "should be");
    return ciblock()->jsrset()->record_at(jsr_level - 1)->return_address();
  }
  SharkBlock* bci_successor(int bci) const;

  // Bytecode stream
 public:
  ciBytecodeStream* iter() const
  {
    return function()->iter();
  }  
  Bytecodes::Code bc() const
  {
    return iter()->cur_bc();
  }
  int bci() const
  {
    return iter()->cur_bci();
  }

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
  void enter(SharkBlock* predecessor, bool is_exception);

 public:
  void enter()
  {
    enter(NULL, false);
  }

 private:
  SharkState* _entry_state;

 private:
  SharkState* entry_state()
  {
    if (_entry_state == NULL) {
      assert(needs_phis(), "should do");
      _entry_state = new SharkPHIState(this);
    }
    return _entry_state;
  }

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
  void add_incoming(SharkState* incoming_state)
  {
    if (needs_phis()) {
      ((SharkPHIState *) entry_state())->add_incoming(incoming_state);
    }
    else if (_entry_state != incoming_state) {
      assert(_entry_state == NULL, "should be");
      _entry_state = incoming_state;
    }
  }

  // Current state
 private:
  SharkTrackingState* _current_state;

 private:
  void set_current_state(SharkTrackingState* current_state)
  {
    _current_state = current_state;
  }

 public:
  SharkTrackingState* current_state()
  {
    if (_current_state == NULL)
      set_current_state(new SharkTrackingState(entry_state()));
    return _current_state;
  }

  // Method
 public:
  llvm::Value* method()
  {
    return current_state()->method();
  }

  // Local variables  
 private:
  SharkValue* local(int index)
  {
    SharkValue *value = current_state()->local(index);
    assert(value != NULL, "shouldn't be");
    assert(value->is_one_word() ||
           (index + 1 < max_locals() &&
            current_state()->local(index + 1) == NULL), "should be");
    return value;
  }
  void set_local(int index, SharkValue* value)
  {
    assert(value != NULL, "shouldn't be");
    current_state()->set_local(index, value);
    if (value->is_two_word())
      current_state()->set_local(index + 1, NULL);
  }

  // Expression stack (raw)
 private:
  void xpush(SharkValue* value)
  {
    current_state()->push(value);
  }
  SharkValue* xpop()
  {
    return current_state()->pop();
  }
  SharkValue* xstack(int slot)
  {
    SharkValue *value = current_state()->stack(slot);
    assert(value != NULL, "shouldn't be");
    assert(value->is_one_word() ||
           (slot > 0 &&
            current_state()->stack(slot - 1) == NULL), "should be");
    return value;
  }
  int xstack_depth()
  {
    return current_state()->stack_depth();
  }

  // Expression stack (cooked)
 private:
  void push(SharkValue* value)
  {
    assert(value != NULL, "shouldn't be");
    xpush(value);
    if (value->is_two_word())
      xpush(NULL);
  }
  SharkValue* pop()
  {
    int size = current_state()->stack(0) == NULL ? 2 : 1;
    if (size == 2)
      xpop();
    SharkValue *value = xpop();
    assert(value && value->size() == size, "should be");
    return value;
  }

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
  void check_null(SharkValue* object)
  {
    check_zero(object);
  }
  void check_divide_by_zero(SharkValue* value)
  {
    check_zero(value);
  }
  void check_zero(SharkValue* value);
  void check_bounds(SharkValue* array, SharkValue* index);
  void check_pending_exception(bool attempt_catch = true);
  void handle_exception(llvm::Value* exception, bool attempt_catch = true);

  // Safepoints
 private:
  void add_safepoint();

  // Returns
 private:
  void handle_return(BasicType type, llvm::Value* exception);
  void release_locked_monitors();

  // ldc*
 private:
  void do_ldc();

  // arraylength
 private:
  void do_arraylength();

  // *aload and *astore
 private:
  void do_aload(BasicType basic_type);
  void do_astore(BasicType basic_type);

  // *div and *rem
 private:
  void do_idiv()
  {
    do_div_or_rem(false, false);
  }
  void do_irem()
  {
    do_div_or_rem(false, true);
  }
  void do_ldiv()
  {
    do_div_or_rem(true, false);
  }
  void do_lrem()
  {
    do_div_or_rem(true, true);
  }
  void do_div_or_rem(bool is_long, bool is_rem);
  
  // get* and put*
 private:
  void do_getstatic()
  {
    do_field_access(true, false);
  }
  void do_getfield()
  {
    do_field_access(true, true);
  }
  void do_putstatic()
  {
    do_field_access(false, false);
  }
  void do_putfield()
  {
    do_field_access(false, true);
  }
  void do_field_access(bool is_get, bool is_field);

  // lcmp and [fd]cmp[lg]
 private:
  void do_lcmp();
  void do_fcmp(bool is_double, bool unordered_is_greater);

  // *return and athrow
 private:
  void do_return(BasicType type)
  {
    add_safepoint();
    handle_return(type, NULL);
  }
  void do_athrow()
  {
    SharkValue *exception = pop();
    check_null(exception);
    handle_exception(exception->jobject_value());
  }

  // if*
 private:
  void do_if(llvm::ICmpInst::Predicate p, SharkValue* b, SharkValue* a);

  // tableswitch and lookupswitch
 private:
  int switch_default_dest();
  int switch_table_length();
  int switch_key(int i);
  int switch_dest(int i);

  void do_switch();

  // invoke*
 private:
  llvm::Value* get_basic_callee(llvm::Value* cache);
  llvm::Value* get_virtual_callee(llvm::Value* cache, SharkValue* receiver);
  llvm::Value* get_interface_callee(llvm::Value* cache, SharkValue* receiver);

  llvm::Value* get_callee(llvm::Value* cache, SharkValue* receiver);

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

  // The big one
 public:
  void parse();
};
