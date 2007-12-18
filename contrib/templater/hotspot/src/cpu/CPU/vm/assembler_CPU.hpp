/*
 * Copyright 1997-2007 Sun Microsystems, Inc.  All Rights Reserved.
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

// The definitions needed for @@cpu@@ assembly code generation.

#ifdef PPC
// Non-volatile registers used by the interpreter

REGISTER_DECLARATION(Register, Rthread, r31);
REGISTER_DECLARATION(Register, Rmethod, r30);
REGISTER_DECLARATION(Register, Rlocals, r29);


// Address is an abstraction used to represent a memory location

class Address {
 private:
  const Register _base;
  const int16_t  _displacement;

 public:
  Address(const Register base, int16_t displacement)
    : _base(base),
      _displacement(displacement)
  {
#ifdef PPC
    assert(base != r0, "can't use r0 for addressing");
#endif // PPC
  }

#ifdef ASSERT
  Address(Register base, ByteSize displacement)
    : _base(base),
      _displacement(in_bytes(displacement)) {}
#endif // ASSERT

  const Register base() const { return _base; }
  int displacement() const { return (int) _displacement & 0xffff; }
};


#endif // PPC
// The @@cpu@@ Assembler: Pure assembler doing NO optimizations on
// the instruction level; i.e., what you write is what you get.
// The Assembler is generating code into a CodeBuffer.

class Assembler : public AbstractAssembler {
 public:
  Assembler(CodeBuffer* code) : AbstractAssembler(code) {}
#ifdef PPC

 private:
  static bool is_simm(int x, int nbits)
  {
    return -(1 << nbits - 1) <= x && x < (1 << nbits - 1);
  }

 public:
  static bool is_simm16(int x)
  {
    return is_simm(x, 16);
  }

 private:
  // Instruction emitters for the various forms.
  // Every instruction should ultimately come through one of these.
  void emit_instruction(int opcode, int li, bool aa, bool lk);
  void emit_instruction(int opcode, int bo, int bi, int bd, bool aa, bool lk);
  void emit_instruction(int opcode, int a, int b, int c);
  void emit_instruction(int opcode, int a, int b, int c, int d);
  void emit_instruction(int opcode, int a, int b, int c, int xo, bool rc);
  void emit_instruction(int opcode, int a, int b, int c, int d, int e,bool rc);

  // Wrappers for the instruction emitters.
  // These handle casting and stuff.
  void emit_instruction(int opcode, Register a, Register b, int c);
  void emit_instruction(int opcode, Register a, Register b, int c, int d);
  void emit_instruction(int opcode, FloatRegister a, Register b, int c);
  void emit_instruction(int opcode, Register a, const Address& b);
  void emit_instruction(int opcode, Register a, const Address& b, int c);
  void emit_instruction(int opcode, FloatRegister a, const Address& b);
  void emit_instruction(int opcode, Register a, int b, int c, int xo,
			bool rc);
  void emit_instruction(int opcode, Register a,  Register b, Register c,
			int xo, bool rc);
  void emit_instruction(int opcode, Register a, SpecialPurposeRegister b,
			int c, int xo, bool rc);
  void emit_instruction(int opcode, SpecialPurposeRegister a, Register b,
			int c, int xo, bool rc);
  void emit_instruction(int opcode, Register a, Register b, int c, int d,
			int e, bool rc);
  void emit_instruction(int opcode, int a, Register b, Register c, int d,
			bool rc);
  void emit_instruction(int opcode, ConditionRegister a, bool l,
			Register b, Register c, int d, bool rc);
  void emit_instruction(int opcode, ConditionRegister a, bool l,
			Register b, int c);
  void emit_instruction(int opcode, FloatRegister a, int b,
			FloatRegister c, int xo, bool rc);

  // Helper for computing branch targets
  intptr_t branch_target(address branch, address target, int bits);

 public:
  // Instructions common to 32- and 64-bit implementations
  void add(Register dst, Register a, Register b);
  void addi(Register dst, Register a, int b);
  void addis(Register dst, Register a, int b);
  void andi_(Register dst, Register a, int b);
  void b(address a);
  void bc(int bo, int bi, address a);
  void bcctrl(int bo, int bi);
  void bcl(int bo, int bi, address a);
  void bclr(int bo, int bi);
  void bclrl(int bo, int bi);
  void bl(address a);
  void cmp(ConditionRegister dst, bool l, Register a, Register b);
  void cmpi(ConditionRegister dst, bool l, Register a, int b);
  void dcbf(Register a, Register b);
  void extsb(Register dst, Register src);
  void extsh(Register dst, Register src);
  void fmr(FloatRegister dst, FloatRegister src);
  void icbi(Register a, Register b);
  void isync();
  void lbzx(Register dst, Register a, Register b);
  void lfd(FloatRegister dst, const Address& src);
  void lfs(FloatRegister dst, const Address& src);
  void lhz(Register dst, const Address& src);
  void lhzx(Register dst, Register a, Register b);
  void lwarx(Register dst, Register a, Register b);
  void lwz(Register dst, const Address& src);
  void lwzx(Register dst, Register a, Register b);
  void mfcr(Register dst);
  void mfspr(Register dst, SpecialPurposeRegister src);
  void mtcrf(int mask, Register src);
  void mtspr(SpecialPurposeRegister dst, Register src);
  void neg(Register dst, Register src);
  void OR(Register dst, Register a, Register b);
  void ori(Register dst, Register a, int b);
  void oris(Register dst, Register a, int b);
  void rlwinm(Register dst, Register a, int b, int mb, int me);
  void stfd(FloatRegister src, const Address& dst);
  void stfs(FloatRegister src, const Address& dst);
  void stw(Register src, const Address& dst);
  void stwcx_(Register src, Register a, Register b);
  void stwu(Register src, const Address& dst);
  void stwux(Register src, Register a, Register b);
  void stwx(Register src, Register a, Register b);
  void subf(Register dst, Register a, Register b);
  void subfic(Register dst, Register a, int b);
  void sync();

  // Instructions for 64-bit implementations only
#ifdef PPC64
  void extsw(Register dst, Register src);
  void ld(Register dst, const Address& src);
  void ldarx(Register dst, Register a, Register b);
  void ldx(Register dst, Register a, Register b);
  void std(Register src, const Address& dst);
  void stdcx_(Register src, Register a, Register b);
  void stdu(Register src, const Address& dst);
  void stdux(Register src, Register a, Register b);
  void stdx(Register src, Register a, Register b);
  void rldicl(Register dst, Register a, int sh, int mb);
  void rldicr(Register dst, Register a, int sh, int me);
#endif // PPC64

  // Standard mnemonics common to 32- and 64-bit implementations
  void bctrl();
  void bdnz(Label& l);
  void beq(Label& l);
  void beqlr();
  void bge(Label& l);
  void bgt(Label& l);
  void ble(Label& l);
  void blelr();
  void blr();
  void blrl();
  void blt(Label& l);
  void bne(Label& l);
  void bne(ConditionRegister r, Label& l);
  void cmpw(ConditionRegister dst, Register a, Register b);
  void cmpwi(ConditionRegister dst, Register a, int b);
  void la(Register dst, Address value);
  void li(Register dst, int value);
  void lis(Register dst, int value);
  void mr(Register dst, Register src);
  void mfctr(Register dst);
  void mflr(Register dst);
  void mtcr(Register src);
  void mtctr(Register src);
  void mtlr(Register src);
  void nop();
  void slwi(Register dst, Register a, int b);
  void srwi(Register dst, Register a, int b);
  void sub(Register dst, Register a, Register b);
  void subi(Register dst, Register a, int b);

  // Standard mnemonics for 64-bit implementations only
#ifdef PPC64
  void cmpd(ConditionRegister dst, Register a, Register b);
  void cmpdi(ConditionRegister dst, Register a, int b);
  void sldi(Register dst, Register a, int b);
  void srdi(Register dst, Register a, int b);
#endif // PPC64

  // Wrappers for branch instructions
  void b(Label& l);
  void bc(int bo, int bi, Label& l);
  void bl(Label& l);
#endif // PPC

  // Function to fix up forward branches
  void pd_patch_instruction(address branch, address target);
#ifndef PRODUCT  
  static void pd_print_patched_instruction(address branch);
#endif // PRODUCT
};

#ifdef PPC

// StackFrame is used to generate prologs and epilogs

class StackFrame {
 private:
  int _params;
  int _locals;
  int _crfs;
  int _gprs;
  int _fprs;

  enum State {
    done_nothing,
    done_prolog,
    done_epilog
  };
  State _state;

  int _frame_size;

  bool _save_volatiles;
  
 public:
  enum LinkArea {
    back_chain_offset,
#ifdef PPC64
    cr_save_offset,
#endif // PPC64
    lr_save_offset,
#ifdef PPC64
    reserved_1,
    reserved_2,
    saved_toc_offset,
#endif // PPC64
    link_area_words
  };

  enum Constants {
#ifdef PPC32
    min_params = 0,
#else
    min_params = 8,
#endif // PPC32
    max_gprs  = 18, // r14-r31
    max_fprs  = 18, // f14-f31
    max_crfs  = 3,  // cr2-cr4
    num_vgprs = 11, // r0 and r3-r12
    num_vsprs = 1,  // ctr
    num_vfprs = 14, // f0-f13

    words_per_fpr = 8 / wordSize
  };
  
 public:
  StackFrame()
    : _params(0),
      _locals(0),
      _crfs(0),
      _gprs(0),
      _fprs(0),
      _state(done_nothing),
      _save_volatiles(false) {}

#ifdef ASSERT
  ~StackFrame() { assert(_state != done_prolog, "stack not restored"); }
#endif // ASSERT

  const Address get_parameter();
  const Address get_local_variable();
  const ConditionRegister get_cr_field();
  const Register get_register();
  const FloatRegister get_float_register();

 protected:
  void generate_prolog(MacroAssembler *masm);
  void generate_epilog(MacroAssembler *masm);

  void set_save_volatiles(bool value);

  friend class MacroAssembler;

 public:
  int unaligned_size();
  int start_of_locals();
};


// Flags for MacroAssembler::call_VM

enum CallVMFlags {
  CALL_VM_DEFAULTS            = 0,
  CALL_VM_NO_EXCEPTION_CHECKS = 1,
  CALL_VM_PRESERVE_LR         = 2
};


#endif // PPC
// MacroAssembler extends Assembler by frequently used macros.
//
// Instructions for which a 'better' code sequence exists depending
// on arguments should also go in here.

class MacroAssembler : public Assembler {
#ifdef PPC
 protected:
  // Support for VM calls
  //
  // This is the base routine called by the different versions of
  // call_VM_leaf. The interpreter may customize this version by
  // overriding it for its purposes (e.g., to save/restore additional
  // registers when doing a VM call).
  virtual void call_VM_leaf_base(address entry_point);
  
  // This is the base routine called by the different versions of
  // call_VM. The interpreter may customize this version by overriding
  // it for its purposes (e.g., to save/restore additional registers
  // when doing a VM call).
  virtual void call_VM_base(Register oop_result,
                            address entry_point,
                            CallVMFlags flags);

 private:
  void call_VM_pass_args(Register arg_1,
                         Register arg_2 = noreg,
                         Register arg_3 = noreg);
  
#endif // PPC
 public:
  MacroAssembler(CodeBuffer* code) : Assembler(code) {}

  void align(int modulus);
#ifdef PPC

  void prolog(StackFrame& frame) { frame.generate_prolog(this); }
  void epilog(StackFrame& frame) { frame.generate_epilog(this); }

  // Non-standard mnemonics
  void lbax(Register dst, Register a, Register b);
  void lhax(Register dst, Register a, Register b);
  void lwa(Register dst, const Address& src);
  void lwax(Register dst, Register a, Register b);
  void mpclr();
  
  // Operations which are different on PPC32/64
  void call(address addr);
  void call(Register addr);
  void compare(Register a, int b);
  void compare(Register a, Register b);
  void compare(ConditionRegister dst, Register a, int b);
  void compare(ConditionRegister dst, Register a, Register b);
  address enter();
  void load(Register dst, long value);
  void load(Register dst, const Address& src);
  void load_and_reserve_indexed(Register dst, Register a, Register b);
  void load_indexed(Register dst, Register a, Register b);
  void shift_left(Register dst, Register a, int b);
  void shift_right(Register dst, Register a, int b);
  void store(Register src, const Address& dst);
  void store_conditional_indexed(Register dst, Register a, Register b);
  void store_indexed(Register src, Register a, Register b);
  void store_update(Register src, const Address& dst);
  void store_update_indexed(Register src, Register a, Register b);

  void cmpxchg_(Register exchange, Register dst, Register compare);

  // Support for VM calls
  void call_VM(Register oop_result, 
               address entry_point, 
               CallVMFlags flags = CALL_VM_DEFAULTS);
  void call_VM(Register oop_result, 
               address entry_point, 
               Register arg_1, 
               CallVMFlags flags = CALL_VM_DEFAULTS);
  void call_VM(Register oop_result,
               address entry_point,
               Register arg_1, Register arg_2,
               CallVMFlags flags = CALL_VM_DEFAULTS);
  void call_VM(Register oop_result,
               address entry_point, 
               Register arg_1, Register arg_2, Register arg_3,
               CallVMFlags flags = CALL_VM_DEFAULTS);

  void call_VM_leaf(address entry_point);
  void call_VM_leaf(address entry_point,
                    Register arg_1);
  void call_VM_leaf(address entry_point,
                    Register arg_1, Register arg_2);
  void call_VM_leaf(address entry_point,
                    Register arg_1, Register arg_2, Register arg_3);

  // Support for serializing memory accesses between threads
  void serialize_memory(Register tmp1, Register tmp2);

  // Support for NULL-checks
  void null_check(Register reg, int offset = -1);
  static bool needs_explicit_null_check(intptr_t offset);

  // Support for VerifyOops
  void verify_oop(Register reg, const char* s = "broken oop");

  void calc_padding_for_alignment(Register dst, Register src, int align);
  void maybe_extend_frame(Register required_bytes, Register available_bytes);
  void get_mirror_handle(Register dst);

 private:
  void report_and_die(address function,
		      const char* file, int line,
		      const char* message = NULL);

 public:
  void should_not_reach_here(const char* file, int line);
  void unimplemented(const char* file, int line);
  void untested(const char* file, int line, const char* message);

  address generate_unimplemented_stub(const char* file, int line);
  address generate_unimplemented_entry(const char* file, int line);

#ifndef PRODUCT  
  void dump_int(Register src);
  void dump_int(const char* prefix, Register src);
#endif // PRODUCT

#endif // PPC
  void bang_stack_with_offset(int offset);
};

#ifdef ASSERT
inline bool AbstractAssembler::pd_check_instruction_mark()
{
  Unimplemented();
}
#endif

#ifdef PPC
#define UnimplementedStub() \
  (__ generate_unimplemented_stub(__FILE__, __LINE__))
#define UnimplementedEntry() \
  (__ generate_unimplemented_entry(__FILE__, __LINE__))
#endif // PPC
