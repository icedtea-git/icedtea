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

#include "incls/_precompiled.incl"
#include "incls/_icache_@@cpu@@.cpp.incl"

#define __ _masm->

#ifdef PPC
static int get_cache_block_size()
{
  // Measure the cache block size by zeroing the first cache block
  // within a page or non-zero bytes and seeing how many bytes got
  // zeroed.
  int pagesize = getpagesize();
  unsigned char *page;
  if (posix_memalign((void **)&page, pagesize, pagesize) != 0)
    return -1;
  memset(page, 0xff, pagesize);
  asm volatile("dcbz 0, %0" : : "r"(page));
  if (page[0] != 0)
    return -1;
  page[pagesize - 1] = 0xff;
  int blocksize = 0;
  while (page[blocksize] == 0)
    blocksize++;
  free(page);
  return blocksize;
}
#endif // PPC

void ICacheStubGenerator::generate_icache_flush(
  ICache::flush_icache_stub_t* flush_icache_stub) {

  StubCodeMark mark(this, "ICache", "flush_icache_stub");

#ifdef PPC
  guarantee(ICache::line_size <= get_cache_block_size(),"line_size too large");
  assert(ICache::line_size == 1 << ICache::log2_line_size, "doh");
  
  address start = __ enter();

  const Register start_addr = r3;
  const Register lines      = r4;
  const Register magic      = r5;

  const Register addr       = r12;

  Label loop1, loop2;

  __ compare (lines, 0);
  __ blelr ();
  
  __ mr (addr, start_addr);
  __ mtctr (lines);
  __ bind (loop1);
  __ dcbf (0, addr);
  __ addi (addr, addr, ICache::line_size);
  __ bdnz (loop1);

  __ sync ();
  
  __ mr (addr, start_addr);
  __ mtctr (lines);
  __ bind (loop2);
  __ icbi (0, addr);
  __ addi (addr, addr, ICache::line_size);
  __ bdnz (loop2);

  __ isync ();

  __ mr (r3, magic);
  __ blr ();

  // Check we got the stub size right
  assert(__ pc() - start == ICache::stub_size, "wrong stub size");
  
  // Must be set here so StubCodeMark destructor can call the flush stub.
  *flush_icache_stub = (ICache::flush_icache_stub_t)start;
#else
  *flush_icache_stub = (ICache::flush_icache_stub_t)UnimplementedStub();
#endif // PPC
}
