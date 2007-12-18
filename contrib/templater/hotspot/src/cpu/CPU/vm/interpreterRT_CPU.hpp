/*
 * Copyright 2003-2005 Sun Microsystems, Inc.  All Rights Reserved.
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

// native method calls

#ifdef PPC
class StackArgument
{
 private:
  MacroAssembler* _masm;
  int             _src_offset;
  int             _dst_offset;
  BasicType       _type;

 public:
  StackArgument()
    : _masm(NULL),
      _src_offset(0),
      _dst_offset(0),
      _type(T_ILLEGAL) {}

  StackArgument(MacroAssembler* masm,
		int src_offset,
		int dst_offset,
		BasicType type)
    : _masm(masm),
      _src_offset(src_offset),
      _dst_offset(dst_offset),
      _type(type) {}

  void pass();
};

enum {
  gp_reg_start = 4, // r3 contains the JNIEnv
  gp_reg_max   = 10,
  fp_reg_start = 1,
#ifdef PPC32
  fp_reg_max   = 8
#else
  fp_reg_max   = 13
#endif // PPC32
};

#endif // PPC
class SignatureHandlerGenerator : public NativeSignatureIterator
{
#ifdef PPC
 private:
  MacroAssembler* _masm;

  int _gp_reg;
  int _fp_reg;
  int _st_arg;

  GrowableArray<StackArgument>* _st_args;

#endif // PPC
 private:
  void pass_int();
  void pass_long();
  void pass_float();
  void pass_double();
  void pass_object();

#ifdef PPC
  void pass_on_stack(const Address& src, BasicType type);
  
#endif
 public:
  SignatureHandlerGenerator(methodHandle method, CodeBuffer* buffer) 
    : NativeSignatureIterator(method) 
  {
#ifdef PPC
    _masm = new MacroAssembler(buffer);

    _gp_reg = gp_reg_start;
    if (method->is_static())
      _gp_reg++;
    _fp_reg = fp_reg_start;

#ifdef PPC32
    _st_arg = 0;
#else
    _st_arg = method->is_static() ? 2 : 1;
#endif

    _st_args  = new GrowableArray<StackArgument>();
#else
    Unimplemented();
#endif // PPC
  }

  void generate(uint64_t fingerprint);
};

#ifdef PPC
class SlowSignatureHandler : public NativeSignatureIterator
{
 private:
  address _from;

  intptr_t *_gp_regs;
  double   *_fp_regs;
  intptr_t *_st_args;

  intptr_t *_gp_reg_max;
  double   *_fp_reg_max;

 private:
  void pass_int();
  void pass_long();
  void pass_float();
  void pass_double();
  void pass_object();

 public:  
  SlowSignatureHandler(methodHandle method,
		       address from,
		       intptr_t *gp_regs,
		       double   *fp_regs,
		       intptr_t *st_args)
    : NativeSignatureIterator(method)
  {
    _from = from;

    _gp_regs = gp_regs;
    if (method->is_static())
      _gp_regs++;
    _fp_regs = fp_regs;

    _st_args = st_args;
#ifdef PPC64
    _st_args += method->is_static() ? 2 : 1;
#endif

    _gp_reg_max = gp_regs + gp_reg_max - gp_reg_start;
    _fp_reg_max = fp_regs + fp_reg_max - fp_reg_start;
  }
};
#endif // PPC
