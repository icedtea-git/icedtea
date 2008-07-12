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
  llvm::Value* thread() const
  {
    return function()->thread();
  }
#ifndef PRODUCT
  bool debug() const
  {
    return function()->debug();
  }
#endif // PRODUCT

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
  
 private:
  SharkTrackingState* current_state()
  {
    if (_current_state == NULL)
      _current_state = new SharkTrackingState(entry_state());
    return _current_state;
  }

 private:
  llvm::Value* method()
  {
    return current_state()->method();
  }
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

  // Code generation
 public:
  void parse();

  // Constant pool
 private:
  llvm::Value* _constant_pool_check;
  llvm::Value* _constant_pool_value;
  llvm::Value* _constant_pool_tags_check;
  llvm::Value* _constant_pool_tags_value;
  llvm::Value* _constant_pool_cache_check;
  llvm::Value* _constant_pool_cache_value;
  
 private:
  llvm::Value* constant_pool();
  llvm::Value* constant_pool_tags();
  llvm::Value* constant_pool_cache();
  llvm::Value* constant_pool_tag_at(int which);
  llvm::Value* constant_pool_object_at(int which);
  llvm::Value* constant_pool_cache_entry_at(int which);

  // Error checking
 private:
  void check_null(SharkValue* value);
  void check_bounds(SharkValue* array, SharkValue* index);
  void check_pending_exception();
  
  // Safepoints
 private:
  void add_safepoint();

  // _ldc* bytecodes
 private:
  void do_ldc();

  // _*aload and *astore bytecodes
 private:
  void do_aload(BasicType basic_type);
  void do_astore(BasicType basic_type);

  // _get* and _put* bytecodes
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

  // _*return bytecodes
 private:
  void do_return(BasicType basic_type);

  // _if* bytecodes
 private:
  void do_if(llvm::ICmpInst::Predicate p, SharkValue* b, SharkValue* a);

  // _*switch bytecodes
 private:
  void do_tableswitch();

  // _invoke* bytecodes
 private:
  void do_call();

  // _checkcast and _instanceof
 private:
  void do_instance_check();
};
