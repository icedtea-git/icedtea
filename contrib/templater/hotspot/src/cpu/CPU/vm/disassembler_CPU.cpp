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

#include "incls/_precompiled.incl"
#include "incls/_disassembler_@@cpu@@.cpp.incl"

#ifndef PRODUCT
#ifdef PPC
class InstructionParser {
 private:
  intptr_t _addr;
  int      _instruction;
  int      _bits;

 public:
  InstructionParser(address addr)
    : _addr((intptr_t) addr),
      _instruction(*(int *) addr),
      _bits(26) {}

  intptr_t pc() const {
    return _addr;
  }

  int instruction() const {
    return _instruction;
  }

  int opcode() const {
    return (_instruction >> 26) & 0x3f;
  }

  int extended_opcode() const {
    switch(opcode()) {
    case 19:
    case 31:
    case 63:
      return (_instruction >> 1) & 0x3ff;

    case 30:
      return (_instruction >> 2) & 7;
      
    case 58:
    case 62:
      return _instruction & 3;

    default:
      return -1;
    }
  }

  int read(int nbits) {
    _bits -= nbits;
    assert(_bits >= 0, "oops");
    return (_instruction >> _bits) & ((1 << nbits) - 1);
  }

  int read_signed(int nbits) {
    int v = read(nbits);
    if (v & (1 << (nbits - 1)))
      v |= ~((1 << nbits) - 1);
    return v;
  }

  int read_reg() {
    return read(5);
  }
  
  int read_xo() {
    int xo = read(10);
    assert(xo == extended_opcode(), "should be");
    return xo;
  }
};

class InstructionDefinition {
 public:
  typedef void (*printer_t)(InstructionParser& p,
                            const InstructionDefinition& d,
                            outputStream *st);
 private:  
  int         _opcode;
  int         _xo;
  const char *_mnemonic;
  printer_t   _printer;

 public:
  InstructionDefinition(int opcode, const char *mn, printer_t p)
    : _opcode(opcode), _xo(-1), _mnemonic(mn), _printer(p) {}

  InstructionDefinition(int opcode, int xo, const char *mn, printer_t p)
    : _opcode(opcode), _xo(xo), _mnemonic(mn), _printer(p) {}

  int opcode() const {
    return _opcode;
  }

  int extended_opcode() const {
    return _xo;
  }

  const char *mnemonic() const {
    return _mnemonic;
  }

  const printer_t printer() const {
    return _printer;
  }
};

#define CONDITION(BO, BI) (((BO) << 2) | ((BI) & 3))
const char *branch_condition(int BO, int BI)
{
  switch (CONDITION(BO, BI)) {
  case CONDITION(12, 0):
    return "lt";
    
  case CONDITION(4, 1):
    return "le";
    
  case CONDITION(12, 2):
    return "eq";
    
  case CONDITION(4, 0):
    return "ge";

  case CONDITION(12, 1):
    return "gt";

  case CONDITION(4, 2):
    return "ne";

  case CONDITION(16, 0):
    return "dnz";

  case CONDITION(20, 0):
    return "";

  default:
    tty->print_cr("BO = %d, BI = %d", BO, BI & 3);
    ShouldNotReachHere();
  }
}
#undef CONDITION

#define INSTRUCTION_PRINTER(name) \
  static void name(InstructionParser& p, \
                   const InstructionDefinition& d, \
                   outputStream *st)

// Instruction printers

INSTRUCTION_PRINTER(print_B_bc)
{
  assert(p.opcode() == 16, "should be");

  int BO = p.read(5);
  int BI = p.read(5);
  int BD = p.read_signed(14);
  int AA = p.read(1);
  int LK = p.read(1);

  if (BO == 20) {
    if (BI == 31 && BD == 1 && AA == 0 && LK == 1) {
      st->print("mpclr");
      return;
    }
    st->print_cr("BO = %d, BI = %d", BO, BI & 3);
    ShouldNotReachHere();
  }
  
  const char *cond = branch_condition(BO, BI);
  int cr = BI >> 2;
  address tgt = (address) (p.pc() + (BD << 2));

  if (cr)
    st->print("b%s%s%s cr%d, %p", cond, LK ? "l" : "", AA ? "a" : "", cr, tgt);
  else
    st->print("b%s%s%s %p", cond, LK ? "l" : "", AA ? "a" : "", tgt);
}  

INSTRUCTION_PRINTER(print_I_b)
{
  assert(p.opcode() == 18, "should be");

  int LI = p.read_signed(24);
  int AA = p.read(1);
  int LK = p.read(1);

  address tgt = (address) (p.pc() + (LI << 2));

  st->print("b%s%s %p", LK ? "l" : "", AA ? "a" : "", tgt);
}

INSTRUCTION_PRINTER(print_D_reg_reg_simm)
{
  int RT = p.read_reg();
  int RA = p.read_reg();
  int SI = p.read_signed(16);

  if (RA == 0) {
    switch (p.opcode()) {
    case 14:
      st->print("li r%d, %d", RT, SI);
      return;

    case 15:
      st->print("lis r%d, %d", RT, SI);
      return;
    }
  }

  const char *mnemonic = d.mnemonic();
  if (p.opcode() == 14 && SI < 0) {
    mnemonic = "subi";
    SI = -SI;
  }

  st->print("%s r%d, %s%d, %d", mnemonic, RT, RA ? "r" : "", RA, SI); 
}

INSTRUCTION_PRINTER(print_D_reg_reg_uimm)
{
  int RT = p.read_reg();
  int RA = p.read_reg();
  int UI = p.read(16);

  if (p.opcode() == 24 && RT == 0 && RA == 0 && UI == 0)
    st->print("nop");
  else
    st->print("%s r%d, r%d, %d", d.mnemonic(), RA, RT, UI); 
}

INSTRUCTION_PRINTER(print_D_reg_addr)
{
  int RT = p.read_reg();
  int RA = p.read_reg();
  int D  = p.read_signed(16);

  st->print("%s r%d, %d(%s%d)", d.mnemonic(), RT, D, RA ? "r" : "", RA); 
}

INSTRUCTION_PRINTER(print_D_freg_addr)
{
  int FRT = p.read_reg();
  int RA  = p.read_reg();
  int D   = p.read_signed(16);

  st->print("%s f%d, %d(%s%d)", d.mnemonic(), FRT, D, RA ? "r" : "", RA); 
}

INSTRUCTION_PRINTER(print_DS_reg_addr)
{
  int RT = p.read_reg();
  int RA = p.read_reg();
  int DS = p.read_signed(14);

  st->print("%s r%d, %d(%s%d)", d.mnemonic(), RT, DS << 2, RA ? "r" : "", RA); 
}

INSTRUCTION_PRINTER(print_X_bcspr)
{
  assert(p.opcode() == 19, "should be");

  int BO = p.read(5);
  int BI = p.read(5);
  int x1 = p.read(5);
  int xo = p.read_xo();
  int LK = p.read(1);

  assert(x1 == 0, "should do");

  const char *cond = branch_condition(BO, BI);
  const char *spr;
  switch (xo) {
  case 16:
    spr = "lr";
    break;

  case 528:
    spr = "ctr";
    break;

  default:
    st->print_cr("xo = %d", xo);
    ShouldNotReachHere();
  }

  st->print("b%s%s%s", cond, spr, LK ? "l" : "");
}
  
INSTRUCTION_PRINTER(print_M)
{
  assert(p.opcode() == 21, "should be");

  int RS = p.read_reg();
  int RA = p.read_reg();
  int SH = p.read(5);
  int MB = p.read(5);
  int ME = p.read(5);
  int Rc = p.read(1);

  if (MB == 0 && 31 - SH == ME)
    st->print("slwi%s r%d, r%d, %d", Rc ? "." : "", RA, RS, SH);
  else if (ME == 31 && 32 - MB == SH)
    st->print("srwi%s r%d, r%d, %d", Rc ? "." : "", RA, RS, MB);      
  else
    ShouldNotReachHere();
}

INSTRUCTION_PRINTER(print_MD)
{
  assert(p.opcode() == 30, "should be");

  int RS  = p.read_reg();
  int RA  = p.read_reg();
  int shl = p.read(5);
  int mel = p.read(5);
  int meh = p.read(1);
  int xo  = p.read(3);
  int shh = p.read(1);
  int Rc  = p.read(1);

  int SH = shh << 5 | shl;
  int ME = meh << 5 | mel;

  if (xo == 0) {
    if (SH = 64 - ME)
      st->print("srdi%s r%d, r%d, %d", Rc ? "." : "", RA, RS, ME);
    else
      ShouldNotReachHere();
  }
  else if (xo == 1) {
    if (ME == 63 - SH)
      st->print("sldi%s r%d, r%d, %d", Rc ? "." : "", RA, RS, SH);
    else
      ShouldNotReachHere();
  }
  else
    ShouldNotReachHere();
}

INSTRUCTION_PRINTER(print_X_reg_reg_reg)
{
  int RT = p.read_reg();
  int RA = p.read_reg();
  int RB = p.read_reg();
  int xo = p.read_xo();
  int Rc = p.read(1);

  const char *mnemonic = d.mnemonic();
  if (xo == 40) {
    mnemonic = "sub";
    int s = RA;
    RA = RB;
    RB = s;
  } 
  
  st->print("%s%s r%d, r%d, r%d", mnemonic, Rc ? "." : "", RT, RA, RB);
}

INSTRUCTION_PRINTER(print_X_reg_regorzero_reg)
{
  int RT = p.read_reg();
  int RA = p.read_reg();
  int RB = p.read_reg();
  int xo = p.read_xo();
  int Rc = p.read(1);

  const char *mnemonic = d.mnemonic();
  if (xo == 40) {
    mnemonic = "sub";
    int s = RA;
    RA = RB;
    RB = s;
  } 
  
  st->print("%s%s r%d, %s%d, r%d", mnemonic, Rc ? "." : "", RT,
            RA ? "r" : "", RA, RB);
}

INSTRUCTION_PRINTER(print_X_res_regorzero_reg)
{
  int x1 = p.read_reg();
  int RA = p.read_reg();
  int RB = p.read_reg();
  int xo = p.read_xo();
  int Rc = p.read(1);

  assert(x1 == 0, "should do");
  
  st->print("%s%s %s%d, r%d", d.mnemonic(), Rc ? "." : "",
            RA ? "r" : "", RA, RB);
}

INSTRUCTION_PRINTER(print_X_reg_reg_res)
{
  int RT = p.read_reg();
  int RA = p.read_reg();
  int x1 = p.read_reg();
  int xo = p.read_xo();
  int Rc = p.read(1);

  assert(x1 == 0, "should do");
  
  st->print("%s%s r%d, r%d", d.mnemonic(), Rc ? "." : "", RT, RA);
}

INSTRUCTION_PRINTER(print_X_reg_res_res)
{
  int RT = p.read_reg();
  int x1 = p.read_reg();
  int x2 = p.read_reg();
  int xo = p.read_xo();
  int Rc = p.read(1);

  assert(x1 == 0 && x2 == 0, "should do");
  
  st->print("%s%s r%d", d.mnemonic(), Rc ? "." : "", RT);
}

INSTRUCTION_PRINTER(print_X_switched_regreg_reg)
{
  int RS = p.read_reg();
  int RA = p.read_reg();
  int RB = p.read_reg();
  int xo = p.read_xo();
  int Rc = p.read(1);

  if (xo == 444 && RS == RB && Rc == 0)
    st->print("mr r%d, r%d", RA, RB);
  else
    st->print("%s%s r%d, r%d, r%d", d.mnemonic(), Rc ? "." : "", RA, RS, RB);
}

INSTRUCTION_PRINTER(print_X_switched_regreg)
{
  int RS = p.read_reg();
  int RA = p.read_reg();
  int x1 = p.read(5);
  int xo = p.read_xo();
  int Rc = p.read(1);

  assert(x1 == 0, "should do");
  
  st->print("%s%s r%d, r%d", d.mnemonic(), Rc ? "." : "", RA, RS);
}

INSTRUCTION_PRINTER(print_X_reserved)
{
  int x1 = p.read(5);
  int x2 = p.read(5);
  int x3 = p.read(5);
  int xo = p.read_xo();
  int Rc = p.read(1);

  assert(x1 == 0 && x2 == 0 && x3 == 0 && Rc == 0, "should do");
  
  st->print("%s", d.mnemonic());
}

INSTRUCTION_PRINTER(print_X_cmp)
{
  int BF = p.read(3);
  int x1 = p.read(1);
  int L  = p.read(1);
  int RA = p.read_reg();
  int RB = p.read_reg();
  int xo = p.read_xo();
  int x2 = p.read(1);

  assert(p.opcode() == 31 && xo == 0, "should be");
  assert(x1 == 0 && x2 == 0, "should be");

  const char *mnemonic = L ? "cmpd" : "cmpw";
  if (BF)
    st->print("%s cr%d, r%d, r%d", mnemonic, BF, RA, RB);
  else
    st->print("%s r%d, r%d", mnemonic, RA, RB);
}

INSTRUCTION_PRINTER(print_D_cmpi)
{
  int BF = p.read(3);
  int x1 = p.read(1);
  int L  = p.read(1);
  int RA = p.read_reg();
  int SI = p.read_signed(16);

  assert(p.opcode() == 11 && x1 == 0, "should be");

  const char *mnemonic = L ? "cmpdi" : "cmpwi";
  if (BF)
    st->print("%s cr%d, r%d, %d", mnemonic, BF, RA, SI);
  else
    st->print("%s r%d, %d", mnemonic, RA, SI);
}

INSTRUCTION_PRINTER(print_X_mspr)
{
  int RT = p.read_reg();
  int sb = p.read(5);
  int sa = p.read(5);
  int xo = p.read_xo();
  int x1 = p.read(1);

  assert(x1 == 0, "should be");

  const char *spr;
  switch(sa << 5 | sb) {
  case 8:
    spr = "lr";
    break;

  case 9:
    spr = "ctr";
    break;

  default:
    st->print_cr("spr = %d", sa << 5 | sb);
    ShouldNotReachHere();
  }

  st->print("m%c%s r%d", d.mnemonic()[1], spr, RT);
}

INSTRUCTION_PRINTER(print_X_mtcrf)
{
  int RS  = p.read_reg();
  int x1  = p.read(1);
  int FXM = p.read(8);
  int x2  = p.read(1);
  int xo  = p.read_xo();
  int x3  = p.read(1);

  assert(x1 == 0 && x2 == 0 && x3 == 0, "should be");

  if (FXM == 255)
    st->print("mtcr r%d", RS);
  else
    st->print("%s %d, r%d", d.mnemonic(), FXM, RS);
}

INSTRUCTION_PRINTER(print_X_freg_res_freg)
{
  int FRT = p.read_reg();
  int x1  = p.read_reg();
  int FRB = p.read_reg();
  int xo  = p.read_xo();
  int Rc  = p.read(1);

  assert(x1 == 0, "should do");
  
  st->print("%s%s f%d, f%d", d.mnemonic(), Rc ? "." : "", FRT, FRB);
}

// Instruction table

static const InstructionDefinition definitions[] = {
  InstructionDefinition( 8,      "subfic", print_D_reg_reg_simm),
  InstructionDefinition(11,      "cmpi",   print_D_cmpi),
  InstructionDefinition(14,      "addi",   print_D_reg_reg_simm),
  InstructionDefinition(15,      "addis",  print_D_reg_reg_simm),
  InstructionDefinition(16,      "bc",     print_B_bc),
  InstructionDefinition(18,      "b",      print_I_b),
  InstructionDefinition(19,  16, "bclr",   print_X_bcspr),
  InstructionDefinition(19, 150, "isync",  print_X_reserved),
  InstructionDefinition(19, 528, "bcctr",  print_X_bcspr),
  InstructionDefinition(21,      "rlwinm", print_M),
  InstructionDefinition(24,      "ori",    print_D_reg_reg_uimm),
  InstructionDefinition(25,      "oris",   print_D_reg_reg_uimm),
  InstructionDefinition(28,      "andi.",  print_D_reg_reg_uimm),
  InstructionDefinition(30,   0, "rldicl", print_MD),
  InstructionDefinition(30,   1, "rldicr", print_MD),
  InstructionDefinition(31,   0, "cmp",    print_X_cmp),
  InstructionDefinition(31,  19, "mfcr",   print_X_reg_res_res),
  InstructionDefinition(31,  20, "lwarx",  print_X_reg_regorzero_reg),
  InstructionDefinition(31,  21, "ldx",    print_X_reg_regorzero_reg),
  InstructionDefinition(31,  23, "lwzx",   print_X_reg_regorzero_reg),
  InstructionDefinition(31,  40, "subf",   print_X_reg_reg_reg),
  InstructionDefinition(31,  84, "ldarx",  print_X_reg_regorzero_reg),
  InstructionDefinition(31,  86, "dcbf",   print_X_res_regorzero_reg),
  InstructionDefinition(31,  87, "lbzx",   print_X_reg_regorzero_reg),
  InstructionDefinition(31, 104, "neg",    print_X_reg_reg_res),
  InstructionDefinition(31, 144, "mtcrf",  print_X_mtcrf),
  InstructionDefinition(31, 150, "stwcx",  print_X_reg_regorzero_reg),
  InstructionDefinition(31, 151, "stwx",   print_X_reg_regorzero_reg),
  InstructionDefinition(31, 181, "stdux",  print_X_reg_regorzero_reg),
  InstructionDefinition(31, 183, "stwux",  print_X_reg_regorzero_reg),
  InstructionDefinition(31, 214, "stdcx",  print_X_reg_regorzero_reg),
  InstructionDefinition(31, 266, "add",    print_X_reg_reg_reg),
  InstructionDefinition(31, 279, "lhzx",   print_X_reg_regorzero_reg),
  InstructionDefinition(31, 339, "mfspr",  print_X_mspr),
  InstructionDefinition(31, 444, "or",     print_X_switched_regreg_reg),
  InstructionDefinition(31, 467, "mtspr",  print_X_mspr),
  InstructionDefinition(31, 598, "sync",   print_X_reserved),
  InstructionDefinition(31, 922, "extsh",  print_X_switched_regreg),
  InstructionDefinition(31, 954, "extsb",  print_X_switched_regreg),
  InstructionDefinition(31, 982, "icbi",   print_X_res_regorzero_reg),
  InstructionDefinition(31, 986, "extsw",  print_X_switched_regreg),
  InstructionDefinition(32,      "lwz",    print_D_reg_addr),
  InstructionDefinition(36,      "stw",    print_D_reg_addr),
  InstructionDefinition(37,      "stwu",   print_D_reg_addr),
  InstructionDefinition(40,      "lhz",    print_D_reg_addr),
  InstructionDefinition(48,      "lfs",    print_D_freg_addr),
  InstructionDefinition(50,      "lfd",    print_D_freg_addr),
  InstructionDefinition(52,      "stfs",   print_D_freg_addr),
  InstructionDefinition(54,      "stfd",   print_D_freg_addr),
  InstructionDefinition(58,   0, "ld",     print_DS_reg_addr),
  InstructionDefinition(62,   0, "std",    print_DS_reg_addr),
  InstructionDefinition(62,   1, "stdu",   print_DS_reg_addr),
  InstructionDefinition(63,  72, "fmr",    print_X_freg_res_freg),
};

static const int definitions_count =
  sizeof(definitions) / sizeof(InstructionDefinition);

static void print_instruction_at(address addr, outputStream *st)
{
  InstructionParser p(addr);
  for (int i = 0; i < definitions_count; i++) {
    const InstructionDefinition& d = definitions[i];
    if (p.opcode() == d.opcode() &&
        p.extended_opcode() == d.extended_opcode()) {
      d.printer()(p, d, st);
      return;
    }
  }
  st->print(".long 0x%08x", p.instruction());
}

// External interface

#endif // PPC
void Disassembler::decode(CodeBlob *cb, outputStream *st)
{
#ifdef PPC
  st = st ? st : tty;
  st->print_cr("Decoding CodeBlob " INTPTR_FORMAT, cb);
  decode(cb->instructions_begin(), cb->instructions_end(), st);
#else
  Unimplemented();
#endif // PPC
}

void Disassembler::decode(nmethod *nm, outputStream *st)
{
  Unimplemented();
}

void Disassembler::decode(u_char *begin, u_char *end, outputStream *st)
{
#ifdef PPC
  st = st ? st : tty;
  CodeBlob *cb = CodeCache::find_blob_unsafe(begin);
  for (u_char *addr = begin; addr < end; addr += 4) {
    if (cb != NULL)
      cb->print_block_comment(st, (intptr_t)(addr - cb->instructions_begin()));

    st->print(PPC32_ONLY("%08x") PPC64_ONLY("%lx") ":   ", (intptr_t) addr);

    for (int i = 0; i < 4; i++)
      st->print("%02x ", addr[i]);
    st->print("   ");

    print_instruction_at((address) addr, st);

    st->cr();
  }
#else
  Unimplemented();
#endif // PPC
}
#endif // PRODUCT
