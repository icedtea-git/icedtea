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
#include "incls/_sharkEntry.cpp.incl"

#ifndef PRODUCT
void SharkEntry::print_statistics(const char* name) const
{
  address start = code_start();
  address limit = code_limit();

  ttyLocker ttyl;
  tty->print(" [%p-%p): %s (%d bytes code", start, limit, name, limit - start);
  print_pd_statistics(start, limit);
  tty->print_cr(")");
}

// Lots of the stuff down here is machine- and LLVM-specific.
// It's only debug stuff though, and none of it's critical.

address SharkEntry::code_start() const
{
  return (address) entry_point();
}

address SharkEntry::code_limit() const
{
#ifdef PPC
  // LLVM seems to insert three junk instructions and a null after
  // every function.  Only the first junk instruction seems to be
  // kept after the next function is generated, however, so this
  // method will only work before you generate another function.
  // I wish there was a nicer way to do this, but that's life...
  uint32_t *limit = (uint32_t *) code_start();
  while (*limit)
    limit++;
  assert(limit[-1] == 0xd143cfec && limit[-2] == 0xd143cfec, "should be");
  limit -= 3;
  return (address) limit;
#else
  Unimplemented();
#endif // PPC
}

void SharkEntry::print_pd_statistics(address start, address limit) const
{
#ifdef PPC
  uint32_t *pc = (uint32_t *) start;
  uint32_t instr;

  // Walk over the bit that allocates the frame
  instr = *(pc++);
  assert (instr == 0x7c0802a6, "expecting 'mflr r0'");

  instr = *(pc++);
  assert (instr == NOT_LP64(0x90010004) LP64_ONLY(0xf8010004),
          "expecting st" NOT_LP64("w") LP64_ONLY("d") " r0,4(r1)");

  instr = *(pc++);
  assert ((instr & 0xffff8001) == NOT_LP64(0x94218000) LP64_ONLY(0xf8218001),
          "expecting st" NOT_LP64("w") LP64_ONLY("d") "u r1,-X(r1)");
  int frame_size = -((instr | 0xffff0000) LP64_ONLY(& 0xfffffffc));
  tty->print(", %d bytes stack", frame_size);

  // Walk over the bit that stores the non-volatile registers
  int first_reg = -1;
  int next_slot = frame_size - wordSize;
  int last_reg = -1;
  while (pc < (uint32_t *) limit) {
    instr = *(pc++);

    // The opcode should be stw/std
    int opcode = instr >> 26;
    if (opcode != NOT_LP64(36) LP64_ONLY(62))
      break;

    // The destination should be next_slot(r1)
    int ra = (instr & 0x001f0000) >> 16;
    if (ra != 1)
      break;

    int ds = instr & 0x0000ffff;
    if (ds != next_slot)
      break;
    next_slot -= wordSize;

    // The source should be the next register after last_reg
    int rs = (instr & 0x03e00000) >> 21;
    if (first_reg == -1) {
      assert(rs >= 13, "storing a non-volatile register?");
      first_reg = last_reg = rs;
    }
    else {
      assert(rs == last_reg + 1, "register stores out of order?");
      last_reg = rs;
    }
  }

  if (first_reg == -1) {
    tty->print(", 0 registers");
  }
  else {
    int num_registers = last_reg - first_reg + 1;
    if (num_registers == 1)
      tty->print(", 1 register");
    else
      tty->print(", %d registers", num_registers);
    if (num_registers >= 19)
      tty->print("!");
  }
#endif // PPC
}
#endif // !PRODUCT
