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
      _num_predecessors(0),
      _entered_backwards(false),
      _entry_state(NULL),
      _current_state(NULL)
  { initialize(); }

 private:
  void initialize();

 private:
  SharkFunction*     _function;
  ciTypeFlow::Block* _ciblock;
  llvm::BasicBlock*  _entry_block;

 public:
  SharkFunction* function() const
  {
    return _function;
  }
  ciTypeFlow::Block* ciblock() const
  {
    return _ciblock;
  }
  llvm::BasicBlock* entry_block() const
  {
    return _entry_block;
  }

 public:
  SharkBuilder* builder() const
  {
    return function()->builder();
  }
  bool failing() const
  {
    return function()->failing();
  }
  void record_method_not_compilable(const char* reason) const
  {
    function()->record_method_not_compilable(reason);
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
  int num_successors() const
  {
    return ciblock()->successors()->length();
  }
  SharkBlock* successor(int index) const
  {
    return function()->block(ciblock()->successors()->at(index)->pre_order());
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
  int         _num_predecessors;
  bool        _entered_backwards;
  SharkState* _entry_state;

 public:
  void add_predecessor(SharkBlock* predecessor)
  {
    _num_predecessors++;
    if (predecessor && !_entered_backwards) {
      if (predecessor->index() >= this->index())
        _entered_backwards = true;
    }
  }
 private:
  bool never_entered() const
  {
    return _num_predecessors == 0;
  }
  bool needs_phis() const
  {
    return _entered_backwards || (_num_predecessors > 1);
  }
  SharkState* entry_state()
  {
    if (_entry_state == NULL) {
      assert(needs_phis(), "should do");
      _entry_state = new SharkPHIState(this);
    }
    return _entry_state;
  }

 public:
  void add_incoming(SharkState* incoming_state)
  {
    if (needs_phis()) {
      ((SharkPHIState *) entry_state())->add_incoming(incoming_state);
    }
    else {
      assert(_entry_state == NULL, "should be");
      _entry_state = incoming_state;
    }
  }

  // Current state
 private:
  SharkTrackingState* _current_state;
  
 public:
  SharkTrackingState* current_state()
  {
    if (_current_state == NULL)
      _current_state = new SharkTrackingState(entry_state());
    return _current_state;
  }

 public:
  llvm::Value* method()
  {
    return current_state()->method();
  }
 private:
  SharkValue* local(int index)
  {
    return current_state()->local(index);
  }
  void set_local(int index, SharkValue* value)
  {
    current_state()->set_local(index, value);
  }
  void push(SharkValue* value)
  {
    current_state()->push(value);
  }
  SharkValue* pop()
  {
    return current_state()->pop();
  }
  int stack_depth()
  {
    return current_state()->stack_depth();
  }
  SharkValue* stack(int slot)
  {
    return current_state()->stack(slot);
  }  

  // Handy macros for the various pop, swap and dup bytecodes
 private:
  SharkValue* pop_and_assert_one_word()
  {
    SharkValue* result = pop();
    assert(result->is_one_word(), "should be");
    return result;
  }
  SharkValue* pop_and_assert_two_word()
  {
    SharkValue* result = pop();
    assert(result->is_two_word(), "should be");
    return result;
  }

  // VM calls
 private:
  llvm::CallInst* call_vm_base(llvm::Constant* callee,
                               llvm::Value**   args_start,
                               llvm::Value**   args_end);

 public:
  llvm::CallInst* call_vm(llvm::Constant* callee,
                          llvm::Value*    arg1)
  {
    llvm::Value *args[] = {thread(), arg1};
    return call_vm_base(callee, args, args + 2);
  }
  llvm::CallInst* call_vm(llvm::Constant* callee,
                          llvm::Value*    arg1,
                          llvm::Value*    arg2)
  {
    llvm::Value *args[] = {thread(), arg1, arg2};
    return call_vm_base(callee, args, args + 3);
  }
  llvm::CallInst* call_vm(llvm::Constant* callee,
                          llvm::Value*    arg1,
                          llvm::Value*    arg2,
                          llvm::Value*    arg3)
  {
    llvm::Value *args[] = {thread(), arg1, arg2, arg3};
    return call_vm_base(callee, args, args + 4);
  }

  // Code generation
 public:
  void parse();

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
  void check_pending_exception();
  
  // Safepoints
 private:
  void add_safepoint();

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

  // lcmp
 private:
  void do_lcmp();

  // *return
 private:
  void do_return(BasicType basic_type);

  // if*
 private:
  void do_if(llvm::ICmpInst::Predicate p, SharkValue* b, SharkValue* a);

  // *switch
 private:
  void do_tableswitch();

  // invoke*
 private:
  void do_call();

  // checkcast and instanceof
 private:
  void do_instance_check();

  // new and newarray
 private:
  void do_new();
  void do_newarray();

  // monitorenter and monitorexit
 private:
  void do_monitorenter();
  void do_monitorexit();
};
