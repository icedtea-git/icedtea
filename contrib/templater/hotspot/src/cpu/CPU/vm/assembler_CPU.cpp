/*
 * Copyright 2003-2007 Sun Microsystems, Inc.  All Rights Reserved.
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
#include "incls/_assembler_@@cpu@@.cpp.incl"

#ifdef PPC

// Fill empty space with zeros.
// (0x00000000 is an illegal instruction on ppc)

int AbstractAssembler::code_fill_byte()
{
  return 0x00;
}

// Instruction emitters for the various forms.
// Every instruction should ultimately come through one of these.

void Assembler::emit_instruction(int opcode, int li, bool aa, bool lk)
{
  // I-form
  assert(!(opcode & ~0x3f), "invalid opcode");
  assert(!(li & ~0xffffff), "invalid operand");
  emit_long(opcode << 26 | li << 2 | aa << 1 | lk);
}
void Assembler::emit_instruction(int opcode, int bo, int bi, int bd,
				 bool aa, bool lk) {
  // B-form (includes SC-form)
  assert(!(opcode & ~0x3f), "invalid opcode");
  assert(!(bo & ~0x1f),     "invalid operand");
  assert(!(bi & ~0x1f),     "invalid operand");
  assert(!(bd & ~0x3fff),   "invalid operand");
  emit_long(opcode << 26 | bo << 21 | bi << 16 | bd << 2 | aa << 1 | lk);
}
void Assembler::emit_instruction(int opcode, int a, int b, int c)
{
  // D-form
  assert(!(opcode & ~0x3f), "invalid opcode");
  assert(!(a & ~0x1f),      "invalid operand");
  assert(!(b & ~0x1f),      "invalid operand");
  if (c < 0) {
    assert((c & ~0xffff) == ~0xffff, "invalid operand");
    c &= 0xffff;
  }
  else
    assert(!(c & ~0xffff),  "invalid operand");
  emit_long(opcode << 26 | a << 21 | b << 16 | c);
}
void Assembler::emit_instruction(int opcode, int a, int b, int c, int d)
{
  // DS-form
  assert(!(opcode & ~0x3f), "invalid opcode");
  assert(!(a & ~0x1f),      "invalid operand");
  assert(!(b & ~0x1f),      "invalid operand");
  if (c < 0) {
    assert((c & ~0x3fff) == ~0x3fff, "invalid operand");
    c &= 0x3fff;
  }
  else
    assert(!(c & ~0x3fff), "invalid operand");
  assert(!(d & ~0x3),       "invalid operand");
  emit_long(opcode << 26 | a << 21 | b << 16 | c << 2 | d);
}
void Assembler::emit_instruction(int opcode, int a, int b, int c, int xo,
				 bool rc) {
  // X-form
  assert(!(opcode & ~0x3f), "invalid opcode");
  assert(!(a & ~0x1f),      "invalid operand");
  assert(!(b & ~0x1f),      "invalid operand");
  assert(!(c & ~0x1f),      "invalid operand");
  assert(!(xo & ~0x3ff),    "invalid operand");
  emit_long(opcode << 26 | a << 21 | b << 16 | c << 11 | xo << 1 | rc);
}
void Assembler::emit_instruction(int opcode, int a, int b, int c, int d,
				 int e, bool rc) {
  switch (opcode) {
  case 21:
    // M-form    
    assert(!(a & ~0x1f),    "invalid operand");
    assert(!(b & ~0x1f),    "invalid operand");
    assert(!(c & ~0x1f),    "invalid operand");
    assert(!(d & ~0x1f),    "invalid operand");
    assert(!(e & ~0x1f),    "invalid operand");
    break;

  case 30:
    // MD-form
    assert(!(a & ~0x1f),    "invalid operand");
    assert(!(b & ~0x1f),    "invalid operand");
    assert(!(c & ~0x3f),    "invalid operand");
    assert(!(d & ~0x3f),    "invalid operand");
    assert(!(e & ~0x07),    "invalid operand");
    {
      int C = c & 0x1f;
      int D = d & 0x1f;
      int E = (d & 0x20) >> 1 | e << 1 | (c & 0x20) >> 5;

      c = C; d = D; e = E;
    }
    break;

  default:
    ShouldNotReachHere();
  }
  emit_long(opcode << 26 | a << 21 | b << 16 | c << 11 | d << 6 | e << 1 | rc);
}

// Wrappers for the instruction emitters.
// These handle casting and stuff.

void Assembler::emit_instruction(int opcode, Register a, Register b, int c)
{
  emit_instruction(opcode, a->encoding(), b->encoding(), c);
}
void Assembler::emit_instruction(int opcode, Register a, Register b, int c,
				 int d) {
  emit_instruction(opcode, a->encoding(), b->encoding(), c, d);
}
void Assembler::emit_instruction(int opcode, FloatRegister a, Register b,
				 int c) {
  emit_instruction(opcode, a->encoding(), b->encoding(), c);
}
void Assembler::emit_instruction(int opcode, Register a, const Address& b)
{
  emit_instruction(opcode, a, b.base(), b.displacement());
}
void Assembler::emit_instruction(int opcode, Register a, const Address& b,
				 int c) {
  emit_instruction(opcode, a, b.base(), b.displacement() >> 2, c);
}
void Assembler::emit_instruction(int opcode, FloatRegister a, const Address& b)
{
  emit_instruction(opcode, a, b.base(), b.displacement());
}
void Assembler::emit_instruction(int opcode, Register a, int b, int c,
				 int xo, bool rc) {
  emit_instruction(opcode, a->encoding(), b, c, xo, rc);
}
void Assembler::emit_instruction(int opcode, Register a,  Register b,
				 Register c, int xo, bool rc) {
  emit_instruction(opcode, a->encoding(), b->encoding(), c->encoding(),
		   xo, rc);
}
void Assembler::emit_instruction(int opcode, Register a,
				 SpecialPurposeRegister b, int c, int xo,
				 bool rc) {
  emit_instruction(opcode, a->encoding(), b->encoding(), c, xo, rc);
}
void Assembler::emit_instruction(int opcode, SpecialPurposeRegister a,
				 Register b, int c, int xo, bool rc) {
  emit_instruction(opcode, a->encoding(), b->encoding(), c, xo, rc);
}
void Assembler::emit_instruction(int opcode, Register a, Register b,
				 int c, int d, int e, bool rc) {
  emit_instruction(opcode, a->encoding(), b->encoding(), c, d, e, rc);
}
void Assembler::emit_instruction(int opcode, int a, Register b, Register c,
				 int d,	bool rc) {
  emit_instruction(opcode, a, b->encoding(), c->encoding(), d, rc);
}
void Assembler::emit_instruction(int opcode, ConditionRegister a, bool l,
				 Register b, Register c, int d, bool rc) {
  emit_instruction(opcode, a->encoding() << 2 | l, b, c, d, rc);
}
void Assembler::emit_instruction(int opcode, ConditionRegister a, bool l,
				 Register b, int c) {
  emit_instruction(opcode, a->encoding() << 2 | l, b->encoding(), c);
}
void Assembler::emit_instruction(int opcode, FloatRegister a, int b,
				 FloatRegister c, int xo, bool rc) {
  emit_instruction(opcode, a->encoding(), b, c->encoding(), xo, rc);
}

// Helpers for computing branch targets

intptr_t Assembler::branch_target(address branch, address target, int bits)
{
  assert(!((intptr_t) branch & 3), "invalid address");
  assert(!((intptr_t) target & 3), "invalid address");
  
  intptr_t disp = ((intptr_t) target - (intptr_t) branch) >> 2;

  intptr_t mask = (1 << bits) - 1;
  intptr_t msb = 1 << (bits - 1);

  if (disp & msb) {
    assert((disp & ~mask) == ~mask, "invalid displacement");
    disp &= mask;
  }
  else {
    assert(!(disp & ~mask), "invalid displacement");
  }
  return disp;
}

// Instructions common to 32- and 64-bit implementations

void Assembler::add(Register dst, Register a, Register b)
{
  emit_instruction(31, dst, a, b, 266, false);
}
void Assembler::addi(Register dst, Register a, int b)
{
  emit_instruction(14, dst, a, b);
}
void Assembler::addis(Register dst, Register a, int b)
{
  emit_instruction(15, dst, a, b);
}
void Assembler::andi_(Register dst, Register a, int b)
{
  emit_instruction(28, a, dst, b);
}
void Assembler::b(address a)
{
  emit_instruction(18, branch_target(pc(), a, 24), false, false);
}
void Assembler::bc(int bo, int bi, address a)
{
  emit_instruction(16, bo, bi, branch_target(pc(), a, 14), false, false);
}
void Assembler::bcctrl(int bo, int bi)
{
  emit_instruction(19, bo, bi, 0, 528, true);
}
void Assembler::bcl(int bo, int bi, address a)
{
  emit_instruction(16, bo, bi, branch_target(pc(), a, 14), false, true);
}
void Assembler::bclr(int bo, int bi)
{
  emit_instruction(19, bo, bi, 0, 16, false);
}
void Assembler::bclrl(int bo, int bi)
{
  emit_instruction(19, bo, bi, 0, 16, true);
}
void Assembler::bl(address a)
{
  emit_instruction(18, branch_target(pc(), a, 24), false, true);
}
void Assembler::cmp(ConditionRegister dst, bool l, Register a, Register b)
{
  emit_instruction(31, dst, l, a, b, 0, false);
}
void Assembler::cmpi(ConditionRegister dst, bool l, Register a, int b)
{
  emit_instruction(11, dst, l, a, b);
}
void Assembler::dcbf(Register a, Register b)
{
  emit_instruction(31, 0, a, b, 86, false);
}
void Assembler::extsb(Register dst, Register src)
{
  emit_instruction(31, src, dst, 0, 954, false);
}
void Assembler::extsh(Register dst, Register src)
{
  emit_instruction(31, src, dst, 0, 922, false);
}  
void Assembler::fmr(FloatRegister dst, FloatRegister src)
{
  emit_instruction(63, dst, 0, src, 72, false);
}
void Assembler::icbi(Register a, Register b)
{
  emit_instruction(31, 0, a, b, 982, false);
}
void Assembler::isync()
{
  emit_instruction(19, 0, 0, 0, 150, false);
}
void Assembler::lbzx(Register dst, Register a, Register b)
{
  emit_instruction(31, dst, a, b, 87, false);
}
void Assembler::lfd(FloatRegister dst, const Address& src)
{
  emit_instruction(50, dst, src);
}
void Assembler::lfs(FloatRegister dst, const Address& src)
{
  emit_instruction(48, dst, src);
}
void Assembler::lhz(Register dst, const Address& src)
{
  emit_instruction(40, dst, src);
}
void Assembler::lhzx(Register dst, Register a, Register b)
{
  emit_instruction(31, dst, a, b, 279, false);
}
void Assembler::lwarx(Register dst, Register a, Register b)
{
  emit_instruction(31, dst, a, b, 20, false);
}
void Assembler::lwz(Register dst, const Address& src)
{
  emit_instruction(32, dst, src);
}
void Assembler::lwzx(Register dst, Register a, Register b)
{
  emit_instruction(31, dst, a, b, 23, false);
}
void Assembler::mfcr(Register dst)
{
  emit_instruction(31, dst, 0, 0, 19, false);
}
void Assembler::mfspr(Register dst, SpecialPurposeRegister src)
{
  emit_instruction(31, dst, src, 0, 339, false);
}
void Assembler::mtcrf(int mask, Register src)
{
  emit_instruction(31, src, (mask & 0xf0) >> 4, (mask & 0xf) << 1, 144, false);
}
void Assembler::mtspr(SpecialPurposeRegister dst, Register src)
{
  emit_instruction(31, src, dst, 0, 467, false);
}
void Assembler::neg(Register dst, Register src)
{
  emit_instruction(31, dst, src, 0, 104, false);  
}
void Assembler::OR(Register dst, Register a, Register b)
{
  emit_instruction(31, a, dst, b, 444, false);
}
void Assembler::ori(Register dst, Register a, int b)
{
  emit_instruction(24, a, dst, b);
}
void Assembler::oris(Register dst, Register a, int b)
{
  emit_instruction(25, a, dst, b);
}
void Assembler::rlwinm(Register dst, Register a, int b, int mb, int me)
{
  emit_instruction(21, a, dst, b, mb, me, false);
}
void Assembler::stfd(FloatRegister src, const Address& dst)
{
  emit_instruction(54, src, dst);
}
void Assembler::stfs(FloatRegister src, const Address& dst)
{
  emit_instruction(52, src, dst);
}
void Assembler::stw(Register src, const Address& dst)
{
  emit_instruction(36, src, dst);
}
void Assembler::stwcx_(Register src, Register a, Register b)
{
  emit_instruction(31, src, a, b, 150, true);
}
void Assembler::stwu(Register src, const Address& dst)
{
  emit_instruction(37, src, dst);
}
void Assembler::stwux(Register src, Register a, Register b)
{
  emit_instruction(31, src, a, b, 183, false);
}
void Assembler::stwx(Register src, Register a, Register b)
{
  emit_instruction(31, src, a, b, 151, false);
}
void Assembler::subf(Register dst, Register a, Register b)
{
  emit_instruction(31, dst, a, b, 40, false);
}
void Assembler::subfic(Register dst, Register a, int b)
{
  emit_instruction(8, dst, a, b);
}
void Assembler::sync()
{
  emit_instruction(31, 0, 0, 0, 598, false);
}

// Instructions for 64-bit implementations only
#ifdef PPC64
void Assembler::extsw(Register dst, Register src)
{
  emit_instruction(31, src, dst, 0, 986, false);
}  
void Assembler::ld(Register dst, const Address& src)
{
  emit_instruction(58, dst, src, 0);  
}
void Assembler::ldarx(Register dst, Register a, Register b)
{
  emit_instruction(31, dst, a, b, 84, false);
}
void Assembler::ldx(Register dst, Register a, Register b)
{
  emit_instruction(31, dst, a, b, 21, false);
}
void Assembler::rldicl(Register dst, Register a, int sh, int mb)
{
  emit_instruction(30, a, dst, sh, mb, 0, false);
}
void Assembler::rldicr(Register dst, Register a, int sh, int me)
{
  emit_instruction(30, a, dst, sh, me, 1, false);
}
void Assembler::std(Register src, const Address& dst)
{
  emit_instruction(62, src, dst, 0);
}
void Assembler::stdcx_(Register src, Register a, Register b)
{
  emit_instruction(31, src, a, b, 214, true);
}
void Assembler::stdu(Register src, const Address& dst)
{
  emit_instruction(62, src, dst, 1);
}
void Assembler::stdux(Register src, Register a, Register b)
{
  emit_instruction(31, src, a, b, 181, false);
}
void Assembler::stdx(Register src, Register a, Register b)
{
  emit_instruction(31, src, a, b, 149, false);
}
#endif // PPC64

// Standard mnemonics common to 32- and 64-bit implementations

void Assembler::bctrl()
{
  bcctrl(20, 0);
}
void Assembler::bdnz(Label& l)
{
  bc(16, 0, l);
}
void Assembler::beq(Label& l)
{
  bc(12, 2, l);
}
void Assembler::beqlr()
{
  bclr(12, 2);
}
void Assembler::bge(Label& l)
{
  bc(4, 0, l);
}
void Assembler::bgt(Label& l)
{
  bc(12, 1, l);
}
void Assembler::ble(Label& l)
{
  bc(4, 1, l);
}
void Assembler::blelr()
{
  bclr(4, 1);
}
void Assembler::blr()
{
  bclr(20, 0);
}
void Assembler::blrl()
{
  bclrl(20, 0);
}
void Assembler::blt(Label& l)
{
  bc(12, 0, l);
}
void Assembler::bne(Label& l)
{
  bne(cr0, l);
}
void Assembler::bne(ConditionRegister r, Label& l)
{
  bc(4, (r->encoding() << 2) | 2, l);
}
void Assembler::cmpw(ConditionRegister dst, Register a, Register b)
{
  cmp(dst, false, a, b);
}
void Assembler::cmpwi(ConditionRegister dst, Register a, int b)
{
  cmpi(dst, false, a, b);
}
void Assembler::la(Register dst, Address value)
{
  addi(dst, value.base(), value.displacement());
}
void Assembler::li(Register dst, int value)
{
  addi(dst, 0, value);
}
void Assembler::lis(Register dst, int value)
{
  addis(dst, 0, value);
}
void Assembler::mr(Register dst, Register src)
{
  OR(dst, src, src);
}
void Assembler::mfctr(Register dst)
{
  mfspr(dst, ctr);
}
void Assembler::mflr(Register dst)
{
  mfspr(dst, lr);
}
void Assembler::mtcr(Register src)
{
  mtcrf(0xff, src);
}
void Assembler::mtctr(Register src)
{
  mtspr(ctr, src);
}
void Assembler::mtlr(Register src)
{
  mtspr(lr, src);
}
void Assembler::nop()
{
  ori(r0, r0, 0);
}
void Assembler::slwi(Register dst, Register a, int b)
{
  rlwinm(dst, a, b, 0, 31 - b);
}
void Assembler::srwi(Register dst, Register a, int b)
{
  rlwinm(dst, a, 32 - b, b, 31);
}
void Assembler::sub(Register dst, Register a, Register b)
{
  subf(dst, b, a);
}
void Assembler::subi(Register dst, Register a, int b)
{
  addi(dst, a, -b);
}

// Standard mnemonics for 64-bit implementations only
#ifdef PPC64
void Assembler::cmpd(ConditionRegister dst, Register a, Register b)
{
  cmp(dst, true, a, b);
}
void Assembler::cmpdi(ConditionRegister dst, Register a, int b)
{
  cmpi(dst, true, a, b);
}
void Assembler::sldi(Register dst, Register a, int b)
{
  rldicr(dst, a, b, 63 - b);
}
void Assembler::srdi(Register dst, Register a, int b)
{
  rldicl(dst, a, 64 - b, b);
}
#endif // PPC64

// Wrappers for branch instructions

void Assembler::b(Label &l)
{
  if (l.is_bound()) {
    b(target(l));
  }
  else {
    l.add_patch_at(code(), locator());
    b(pc());
  } 
}
void Assembler::bc(int bo, int bi, Label& l)
{
  if (l.is_bound()) {
    bc(bo, bi, target(l));
  }
  else {
    l.add_patch_at(code(), locator());
    bc(bo, bi, pc());
  } 
}
void Assembler::bl(Label &l)
{
  if (l.is_bound()) {
    bl(target(l));
  }
  else {
    l.add_patch_at(code(), locator());
    bl(pc());
  } 
}

// Function to fix up forward branches

void Assembler::pd_patch_instruction(address branch, address target)
{
  unsigned int instruction = *(unsigned int *) branch;
  unsigned int opcode = instruction >> 26;
  assert (!(instruction & 2), "must use relocations for absolute branches");

  switch (opcode) {
  case 16:
    // conditional branch, 14-bit displacement
    assert(!(instruction & 0xfffc), "should be zeroed");
    (*(unsigned int *) branch) |= branch_target(branch, target, 14) << 2;
    break;

  case 18:
    // unconditional branch, 24-bit displacement
    assert(!(instruction & 0x3fffffc), "should be zeroed");
    (*(unsigned int *) branch) |= branch_target(branch, target, 24) << 2;
    break;

  default:
    Unimplemented();
  }
}
#endif // PPC

#ifndef PRODUCT
void Assembler::pd_print_patched_instruction(address branch)
{
  Unimplemented();
}
#endif // PRODUCT

#ifdef PPC
// 32-bit ABI:
//
//     | ...                  |
//     +----------------------+
//     | LR save word         |
// +-> | Back chain           |
// |   +----------------------+  ---------------------------------------  
// |   | FPR save area    f31 |                           high addresses
// |   |                  ... |  2 * _fprs words
// |   |                  f14 |
// |   +----------------------+  
// |   | GPR save area    r31 |
// |   |                  ... |  _gprs words
// |   |                  r14 |
// |   +----------------------+
// |   | CR save area         |  1 word
// |   +----------------------+
// |   | Local variable space |  _locals words (+ padding if required)
// |   +----------------------+
// |   | Parameter list space |  _params words
// |   +----------------------+
// |   | LR save word         |  1 word
// +---+ Back chain           |  1 word                    low addresses
//     +----------------------+  ---------------------------------------

// 64-bit ABI:
//
//     | ...                  |
//     | LR save word         |
//     | CR save word         |
// +-> | Back chain           |
// |   +----------------------+  ---------------------------------------  
// |   | FPR save area    f31 |                           high addresses
// |   |                  ... |  _fprs words
// |   |                  f14 |
// |   +----------------------+  
// |   | GPR save area    r31 |
// |   |                  ... |  _gprs words
// |   |                  r14 |
// |   +----------------------+
// |   | Local variable space |  _locals words (+ padding if required)
// |   +----------------------+
// |   | Parameter list space |  _params words (minimum 8)
// |   +----------------------+
// |   | TOC save word        |  1 word
// |   | Reserved             |  2 words
// |   | LR save word         |  1 word
// |   | CR save word         |  1 word
// +---+ Back chain           |  1 word                    low addresses
//     +----------------------+  ---------------------------------------

const Address StackFrame::get_parameter()
{
  assert(_state == done_nothing, "can't change layout after prolog written");
  assert(_locals == 0, "get all parameters before allocating locals");
  return Address(r1, (link_area_words + _params++) * wordSize);
}
const Address StackFrame::get_local_variable()
{
  assert(_state == done_nothing, "can't change layout after prolog written");
  if (_params < min_params)
    _params = min_params;
  return Address(r1, (link_area_words + _params + _locals++) * wordSize);
}
const ConditionRegister StackFrame::get_cr_field()
{
  assert(_state == done_nothing, "can't change layout after prolog written");
  assert(_crfs < max_crfs, "no more fields!");
  return as_ConditionRegister(4 - _crfs++);
}
const Register StackFrame::get_register()
{
  assert(_state == done_nothing, "can't change layout after prolog written");
  assert(_gprs < max_gprs, "no more registers!");
  return as_Register(31 - _gprs++);
}
const FloatRegister StackFrame::get_float_register()
{
  assert(_state == done_nothing, "can't change layout after prolog written");
  assert(_fprs < max_fprs, "no more registers!");
  return as_FloatRegister(31 - _fprs++);
}
void StackFrame::set_save_volatiles(bool value)
{
  assert(_state == done_nothing, "can't change layout after prolog written");
  _save_volatiles = value;
}

int StackFrame::unaligned_size()
{
#ifdef PPC32
  int crfs = _crfs ? 1 : 0;
#else
  int crfs = 0;
#endif // PPC32

  int params = _params;
  if (params < min_params)
    params = min_params;

  int vgprs = _save_volatiles ? num_vgprs : 0;
  int vsprs = _save_volatiles ? num_vsprs : 0;
  int vfprs = _save_volatiles ? num_vfprs * words_per_fpr : 0;

  return (link_area_words +
	  params +
	  _locals +
	  vgprs +
	  vsprs +
	  vfprs +
	  crfs +
	  _gprs +
	  _fprs * words_per_fpr) * wordSize;
}

int StackFrame::start_of_locals()
{
  int params = _params;
  if (params < min_params)
    params = min_params;

  return (link_area_words + params) * wordSize;
}

#define __ masm->
void StackFrame::generate_prolog(MacroAssembler *masm)
{
  assert(_state == done_nothing, "invalid state");

  // Calculate the aligned frame size
  while (true) {
    _frame_size = unaligned_size();
    if (_frame_size % StackAlignmentInBytes == 0)
      break;
    _locals++;
  }
  int offset  = start_of_locals() / wordSize + _locals;
  const Address r0_save = Address(r1, offset++ * wordSize);

  // Save the link register and create the new frame
  if (_save_volatiles) {
    __ store_update (r1, Address(r1, -_frame_size));
    __ store (r0, r0_save);
    __ mflr (r0);
    __ store (r0, Address(r1, _frame_size + lr_save_offset * wordSize));
  }
  else {
    __ mflr (r0);
    __ store (r0, Address(r1, lr_save_offset * wordSize));
    __ store_update (r1, Address(r1, -_frame_size));
  }

  // Store the remaining volatile registers
  if (_save_volatiles) {
    for (int i = 3; i < 13; i++) {
      __ store (as_Register(i), Address(r1, offset++ * wordSize));
    }
    __ mfctr (r0);
    __ store (r0, Address(r1, offset++ * wordSize));    
    for (int i = 0; i < num_vfprs; i++) {
      __ stfd (as_FloatRegister(i), Address(r1, offset * wordSize));
      offset += words_per_fpr;
    }
  }

  // Store the non-volatile registers
  if (_crfs) {
    __ mfcr (r0);
#ifdef PPC32
    __ store (r0, Address(r1, offset++ * wordSize));
#else
    __ store (r0, Address(r1, cr_save_offset * wordSize));
#endif // PPC32
  }
  for (int i = 32 - _gprs; i < 32; i++) {
    __ store (as_Register(i), Address(r1, offset++ * wordSize));
  }
  for (int i = 32 - _fprs; i < 32; i++) {
    __ stfd (as_FloatRegister(i), Address(r1, offset * wordSize));
    offset += words_per_fpr;
  }

  // Restore r0 before continuing
  if (_save_volatiles)
    __ load (r0, r0_save);

  _state = done_prolog;
}
void StackFrame::generate_epilog(MacroAssembler *masm)
{
  assert(_state != done_nothing, "invalid state");

  int offset = start_of_locals() / wordSize + _locals;
  const Address r0_save = Address(r1, offset++ * wordSize);

  // Restore the volatile registers except r0
  if (_save_volatiles) {
    for (int i = 3; i < 13; i++) {
      __ load (as_Register(i), Address(r1, offset++ * wordSize));
    }
    __ load (r0, Address(r1, offset++ * wordSize));    
    __ mtctr (r0);
    for (int i = 0; i < num_vfprs; i++) {
      __ lfd (as_FloatRegister(i), Address(r1, offset * wordSize));
      offset += words_per_fpr;
    }
  }

  // Restore the non-volatile registers
  if (_crfs) {
#ifdef PPC32
    __ load (r0, Address(r1, offset++ * wordSize));
#else
    __ load (r0, Address(r1, cr_save_offset * wordSize));
#endif // PPC32    
    __ mtcr (r0);
  }
  for (int i = 32 - _gprs; i < 32; i++) {
    __ load (as_Register(i), Address(r1, offset++ * wordSize));
  }
  for (int i = 32 - _fprs; i < 32; i++) {
    __ lfd (as_FloatRegister(i), Address(r1, offset * wordSize));
    offset += words_per_fpr;
  }

  // Remove the frame and restore the link register
  if (_save_volatiles) {
    __ load (r0, Address(r1, _frame_size + lr_save_offset * wordSize));
    __ mtlr (r0);
    __ load (r0, r0_save);  
    __ addi (r1, r1, _frame_size);
  }
  else {
    __ addi (r1, r1, _frame_size);
    __ load (r0, Address(r1, lr_save_offset * wordSize));
    __ mtlr (r0);
  }    

  _state = done_epilog;
}
#undef __

void MacroAssembler::align(int modulus)
{
  assert(!((offset() % modulus) & 3), "invalid alignment");
  while (offset() % modulus != 0)
    nop();
}

// Non-standard mnemonics
void MacroAssembler::lbax(Register dst, Register a, Register b)
{
  lbzx(dst, a, b);
  extsb(dst, dst);
}
void MacroAssembler::lhax(Register dst, Register a, Register b)
{
  // NB there actually is an actual lhax instruction but the
  // manual mentions higher latency and gcc for one does not
  // use it.
  lhzx(dst, a, b);
  extsh(dst, dst);
}
void MacroAssembler::lwa(Register dst, const Address& src)
{
  lwz(dst, src);
#ifdef PPC64
  extsw(dst, dst);
#endif
}
void MacroAssembler::lwax(Register dst, Register a, Register b)
{
  lwzx(dst, a, b);
#ifdef PPC64
  extsw(dst, dst);
#endif
}
void MacroAssembler::mpclr()
{
  // move pc to lr
  // 20, 31 is a magic branch that preserves the link stack
  bcl(20, 31, pc() + 4);
}

// Operations which are different on PPC32/64

void MacroAssembler::call(address func)
{
  assert(((intptr_t) func & 3) == 0, "invalid address");

#ifdef PPC32
  const Register reg = r0;
#else
  const Register reg = r12;
#endif // PPC32

  load(reg, (intptr_t) func);
  call(reg);
}

void MacroAssembler::call(Register func)
{
#ifdef PPC64
  std(r2, Address(r1, 40));
  ld(r2, Address(func, 8));
  ld(r0, Address(func, 0));
  func = r0;
#endif // PPC64

  mtctr(func);
  bctrl();

#ifdef PPC64
  ld (r2, Address(r1, 40));
#endif // PPC64
}

void MacroAssembler::compare(Register a, int b)
{
  compare(cr0, a, b);
}

void MacroAssembler::compare(Register a, Register b)
{
  compare(cr0, a, b);
}

void MacroAssembler::compare(ConditionRegister dst, Register a, int b)
{
#ifdef PPC32
  cmpwi(dst, a, b);
#else
  cmpdi(dst, a, b);
#endif // PPC32 
}

void MacroAssembler::compare(ConditionRegister dst, Register a, Register b)
{
#ifdef PPC32
  cmpw(dst, a, b);
#else
  cmpd(dst, a, b);
#endif // PPC32 
}

address MacroAssembler::enter()
{
  address start = pc();
#ifdef PPC64
  // Sneak in a function descriptor
  // NB this is not relocatable!
  emit_address(start + 16);
  emit_address(NULL);  
#endif // PPC64
  return start;
}

void MacroAssembler::load(Register dst, long value)
{
#ifdef PPC32
  int a = value >> 16;
  int b = value & 0xffff;

  if (a) {
    lis(dst, a);
    if (b)
      ori(dst, dst, b);
  }
  else {
    li(dst, b);
  }
#else
  int a = (int)(value >> 48);
  int b = (int)(value >> 32) & 0xffff;
  int c = (int)(value >> 16) & 0xffff;
  int d = (int) value & 0xffff;

  if (a) {
    lis (dst, a);
    if (b)
      ori (dst, dst, b);
  }
  else if (b) {
    li (dst, b);
  }
  if (a || b)
    sldi (dst, dst, 32);

  if (c) {
    if (a || b)
      oris (dst, dst, c);
    else
      lis (dst, c);
    if (d)
      ori (dst, dst, d);
  }
  else if (d || !(a || b)) {
    if (a || b)
      ori (dst, dst, d);
    else
      li (dst, d);
  }
#endif // PPC32
}

void MacroAssembler::load(Register dst, const Address& src)
{
#ifdef PPC32
  lwz(dst, src);
#else
  ld(dst, src);
#endif // PPC32
}

void MacroAssembler::load_and_reserve_indexed(Register dst, Register a,
					      Register b) {
#ifdef PPC32
  lwarx(dst, a, b);
#else
  ldarx(dst, a, b);
#endif // PPC32
} 
 
void MacroAssembler::load_indexed(Register dst, Register a, Register b)
{
#ifdef PPC32
  lwzx(dst, a, b);
#else
  ldx(dst, a, b);
#endif // PPC32
}

void MacroAssembler::shift_left(Register dst, Register a, int b)
{
#ifdef PPC32
  slwi(dst, a, b);
#else
  sldi(dst, a, b);
#endif // PPC32
}

void MacroAssembler::shift_right(Register dst, Register a, int b)
{
#ifdef PPC32
  srwi(dst, a, b);
#else
  srdi(dst, a, b);
#endif // PPC32
}

void MacroAssembler::store(Register src, const Address& dst)
{
#ifdef PPC32
  stw(src, dst);
#else
  std(src, dst);
#endif // PPC32
}

void MacroAssembler::store_conditional_indexed(Register src, Register a,
					       Register b) {
#ifdef PPC32
  stwcx_(src, a, b);
#else
  stdcx_(src, a, b);
#endif // PPC32
}

void MacroAssembler::store_indexed(Register src, Register a, Register b)
{
#ifdef PPC32
  stwx(src, a, b);
#else
  stdx(src, a, b);
#endif // PPC32
}

void MacroAssembler::store_update(Register src, const Address& dst)
{
#ifdef PPC32
  stwu(src, dst);
#else
  stdu(src, dst);
#endif // PPC32
}

void MacroAssembler::store_update_indexed(Register src, Register a, Register b)
{
#ifdef PPC32
  stwux(src, a, b);
#else
  stdux(src, a, b);
#endif // PPC32
}

// Atomic compare and exchange
//
// if (*dst == comp)
//   *dst = exchange
//
// Returns:
//   cr0 is set to reflect whether the store was performed

void MacroAssembler::cmpxchg_(Register exchange, Register dst, Register comp)
{
  assert_different_registers(exchange, dst, comp, r0);

  Label loop, done;

  sync();
  bind(loop);
  load_and_reserve_indexed(r0, 0, dst);
  compare(r0, comp);
  bne(done);
  store_conditional_indexed(exchange, 0, dst);
  bne(loop);
  isync();
  bind(done);
}

void MacroAssembler::call_VM_pass_args(Register arg_1,
                                       Register arg_2,
                                       Register arg_3)
{
  if (arg_1 != r4) {
    assert(arg_2 != r4, "smashed argument");
    assert(arg_3 != r4, "smashed argument");
    mr(r4, arg_1);
  }

  if (arg_2 == noreg) {
    assert (arg_3 == noreg, "what?");
    return;
  }

  if (arg_2 != r5) {
    assert(arg_3 != r5, "smashed argument");
    mr(r5, arg_2);
  }

  if (arg_3 != noreg && arg_3 != r6) {
    mr(r6, arg_3);
  }
}
                                  
void MacroAssembler::call_VM_base(Register oop_result,
                                  address entry_point,
                                  CallVMFlags flags)
{
  StackFrame frame;

  if (flags & CALL_VM_PRESERVE_LR)
    prolog(frame);

  mr(r3, Rthread);
  call(entry_point);

  if (flags & CALL_VM_PRESERVE_LR)
    epilog(frame);

  if (!(flags & CALL_VM_NO_EXCEPTION_CHECKS)) {
    Label ok;
    load(r0, Address(Rthread, Thread::pending_exception_offset()));
    compare (r0, 0);
    beq(ok);
    unimplemented(__FILE__, __LINE__);
    bind(ok);
  }

  if (oop_result->is_valid()) {
    unimplemented(__FILE__, __LINE__);
  }
}

void MacroAssembler::call_VM(Register oop_result, 
                             address entry_point, 
                             CallVMFlags flags)
{
  call_VM_base(oop_result, entry_point, flags);
}
void MacroAssembler::call_VM(Register oop_result, 
                             address entry_point, 
                             Register arg_1, 
                             CallVMFlags flags)
{
  call_VM_pass_args(arg_1);
  call_VM_base(oop_result, entry_point, flags);
}
void MacroAssembler::call_VM(Register oop_result,
                             address entry_point,
                             Register arg_1, Register arg_2,
                             CallVMFlags flags)
{
  call_VM_pass_args(arg_1, arg_2);
  call_VM_base(oop_result, entry_point, flags);
}
void MacroAssembler::call_VM(Register oop_result,
                             address entry_point, 
                             Register arg_1, Register arg_2, Register arg_3,
                             CallVMFlags flags)
{
  call_VM_pass_args(arg_1, arg_2, arg_3);
  call_VM_base(oop_result, entry_point, flags);
}

void MacroAssembler::call_VM_leaf_base(address entry_point)
{
  mr(r3, Rthread);
  call(entry_point);
}

void MacroAssembler::call_VM_leaf(address entry_point)
{
  call_VM_leaf_base(entry_point);
}
void MacroAssembler::call_VM_leaf(address entry_point, Register arg_1)
{
  call_VM_pass_args(arg_1);
  call_VM_leaf_base(entry_point);
}
void MacroAssembler::call_VM_leaf(address entry_point, Register arg_1,
                                  Register arg_2)
{
  call_VM_pass_args(arg_1, arg_2);
  call_VM_leaf_base(entry_point);
}
void MacroAssembler::call_VM_leaf(address entry_point, Register arg_1,
                                  Register arg_2, Register arg_3)
{
  call_VM_pass_args(arg_1, arg_2, arg_3);
  call_VM_leaf_base(entry_point);
}

// Write serialization page so VM thread can do a pseudo remote membar.
void MacroAssembler::serialize_memory(Register tmp1, Register tmp2)
{
  // We use the current thread pointer to calculate a thread specific
  // offset to write to within the page. This minimizes bus traffic
  // due to cache line collision.
  shift_right(tmp1, Rthread, os::get_serialize_page_shift_count());
  andi_(tmp1, tmp1, os::vm_page_size() - sizeof(int));
  load(tmp2, (intptr_t) os::get_memory_serialize_page());
  stwx(tmp1, tmp2, tmp1);
}

void MacroAssembler::null_check(Register reg, int offset) {
  if (needs_explicit_null_check((intptr_t) offset)) {
    // provoke OS NULL exception if reg = NULL by
    // accessing M[reg] w/o changing any registers
    Unimplemented();
  }
  else {
    // nothing to do, (later) access of M[reg + offset]
    // will provoke OS NULL exception if reg = NULL
  }
}

void MacroAssembler::verify_oop(Register reg, const char* s)
{
  if (!VerifyOops)
    return;

  Unimplemented();
}

void MacroAssembler::calc_padding_for_alignment(
  Register dst, Register src, int align) {
#ifdef ASSERT
  {
    int tmp = align;
    while (!(tmp & 1))
      tmp >>= 1;
    assert (tmp == 1, "alignment must be a power of two");
  }
#endif // ASSERT
  andi_(dst, src, align - 1);
  subfic(dst, dst, align);
  andi_(dst, dst, align - 1);
}

void MacroAssembler::maybe_extend_frame(
  Register required_bytes, Register available_bytes)
{
  Label done;

  // Check whether we have enough space already
  compare(required_bytes, available_bytes);
  ble(done);

  // Calculate how many extra bytes we need to allocate
  const Register extra_bytes = required_bytes;
  const Register padding     = available_bytes;

  sub(extra_bytes, required_bytes, available_bytes);
  calc_padding_for_alignment(padding, extra_bytes, StackAlignmentInBytes);
  add(extra_bytes, extra_bytes, padding);

  // Extend the frame
  const Register saved_r1 = padding;
  const Register scratch  = extra_bytes;

  mr(saved_r1, r1);
  neg(scratch, extra_bytes);
  store_update_indexed(r1, r1, scratch);
  load(scratch, Address(saved_r1, StackFrame::back_chain_offset * wordSize));
  store(scratch, Address(r1, StackFrame::back_chain_offset * wordSize));
#ifdef PPC64
  load(scratch, Address(saved_r1, StackFrame::cr_save_offset * wordSize));
  store(scratch, Address(r1, StackFrame::cr_save_offset * wordSize));
#endif

  bind (done);
}

void MacroAssembler::get_mirror_handle(Register dst)
{
  load(dst, Address(Rmethod, methodOopDesc::constants_offset()));
  load(dst, Address(dst, constantPoolOopDesc::pool_holder_offset_in_bytes()));
  load(dst, Address(dst, klassOopDesc::klass_part_offset_in_bytes()
		         + Klass::java_mirror_offset_in_bytes()));
}

void MacroAssembler::report_and_die(address function,
				    const char* file, int line,
				    const char* message) {
  load(r3, (intptr_t) file);
  load(r4, line);
  if (message)
    load(r5, (intptr_t) message);
  call(function);
}

void MacroAssembler::should_not_reach_here(const char* file, int line)
{
  report_and_die(
    CAST_FROM_FN_PTR(address, report_should_not_reach_here), file, line);
}
void MacroAssembler::unimplemented(const char* file, int line)
{
  report_and_die(
    CAST_FROM_FN_PTR(address, report_unimplemented), file, line);
}
void MacroAssembler::untested(const char* file, int line, const char* message)
{
  report_and_die(
    CAST_FROM_FN_PTR(address, report_untested), file, line, message);
}

address MacroAssembler::generate_unimplemented_stub(const char* file, int line)
{
  address start = enter();
  StackFrame frame;
  prolog(frame);
  unimplemented(file, line);
  epilog(frame);
  blr();
  return start;  
}
address MacroAssembler::generate_unimplemented_entry(const char* file,int line)
{
  address start = pc();
  StackFrame frame;
  prolog(frame);
  unimplemented(file, line);
  epilog(frame);
  blr();
  return start;  
}

#ifndef PRODUCT
static void dump_int_helper(const char* prefix, long src)
{
  ttyLocker ttyl;
  ResourceMark rm;

  if (abs(src) < 1000000)
#ifdef _LP64
    tty->print_cr("%s = %ld", prefix, src);
#else
    tty->print_cr("%s = %d", prefix, src);
#endif // _LP64
  else
    tty->print_cr("%s = %p", prefix, src);
}

void MacroAssembler::dump_int(Register src)
{
  dump_int(src->name(), src);
}

void MacroAssembler::dump_int(const char* prefix, Register src)
{
  StackFrame frame;
  frame.set_save_volatiles(true);
  prolog(frame);
  if (src == r1) {
    int framesize = frame.unaligned_size();
    framesize += (StackAlignmentInBytes -
                  (framesize & (StackAlignmentInBytes - 1))) &
                 (StackAlignmentInBytes - 1);
    addi(r4, r1, framesize);
  }
  else if (src != r4) {
    mr(r4, src);
  }
  load(r3, (intptr_t) prefix);
  call(CAST_FROM_FN_PTR(address, dump_int_helper));
  epilog(frame);
}
#endif // PRODUCT

#endif // PPC
void MacroAssembler::bang_stack_with_offset(int offset)
{
  Unimplemented();
}
