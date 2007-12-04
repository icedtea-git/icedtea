/*
 * Copyright 2000-2005 Sun Microsystems, Inc.  All Rights Reserved.
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

// native word offsets from memory address (big endian)
enum {
  pd_lo_word_offset_in_bytes = BytesPerWord,
  pd_hi_word_offset_in_bytes = 0
};

// are explicit rounding operations required to implement the strictFP mode?
enum {
#ifdef XXX_EVIL_EVIL_EVIL
  pd_strict_fp_requires_explicit_rounding = false
#endif
};

// registers
enum {
#ifdef XXX_EVIL_EVIL_EVIL
  // numbers of registers used during code emission
  pd_nof_cpu_regs_frame_map = 32,
  pd_nof_fpu_regs_frame_map = 32,

  // numbers of registers killed by calls
  pd_nof_caller_save_cpu_regs_frame_map = 11,
  pd_nof_caller_save_fpu_regs_frame_map = 14,

  // numbers of registers visible to register allocator
  pd_nof_cpu_regs_reg_alloc = 0,
  pd_nof_fpu_regs_reg_alloc = 0,

  // numbers of registers visible to linear scan
  pd_nof_cpu_regs_linearscan = 32,
  pd_nof_fpu_regs_linearscan = 32,
  pd_nof_xmm_regs_linearscan = 0,

  // specifics
  pd_first_cpu_reg = 3,  // skip r0, r1 and r2
  pd_last_cpu_reg  = 30, // skip Rthread
  pd_first_fpu_reg = 0,
  pd_last_fpu_reg  = 31,
#endif
};

// encoding of float value in debug info
enum {
#ifdef XXX_EVIL_EVIL_EVIL
  pd_float_saved_as_double = false
#endif
};
