/*
 * Copyright 1999-2006 Sun Microsystems, Inc.  All Rights Reserved.
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

 public:
  enum {
#ifdef XXX_EVIL_EVIL_EVIL
    first_available_sp_in_frame = 0,
    frame_pad_in_bytes = 0
#endif
  };

  static LIR_Opr gpr_opr[nof_cpu_regs];
  static LIR_Opr gpr_oop_opr[nof_cpu_regs];
  static LIR_Opr fpr_opr[nof_fpu_regs];

  static LIR_Opr as_long_opr(Register r, Register r2) {
    return LIR_OprFact::double_cpu(cpu_reg2rnr(r), cpu_reg2rnr(r2));
  }

  static VMReg fpu_regname(int n);

  static bool is_caller_save_register(LIR_Opr reg);
