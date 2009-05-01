/*
 * Copyright 1999-2007 Sun Microsystems, Inc.  All Rights Reserved.
 * Copyright 2009 Red Hat, Inc.
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
#include "incls/_sharkFrameCache.cpp.incl"

using namespace llvm;

SharkFrameCache::SharkFrameCache(SharkFunction *function)
  : _frame_size(function->extended_frame_size())
{
  _values = NEW_RESOURCE_ARRAY(Value*, frame_size());
  memset(_values, 0, frame_size() * sizeof(Value *));
}

SharkFrameCache::SharkFrameCache(const SharkFrameCache* cache)
  : _frame_size(cache->frame_size())
{
  _values = NEW_RESOURCE_ARRAY(Value*, frame_size());
  memcpy(_values, cache->_values, frame_size() * sizeof(Value *));
}

bool SharkFrameCache::equal_to(SharkFrameCache* other)
{
  if (frame_size() != other->frame_size())
    return false;

  for (int i = 0; i < frame_size(); i++) {
    if (value(i) != other->value(i))
      return false;
  }

  return true;
}

void SharkFrameCache::merge(SharkFrameCache* other)
{
  assert(frame_size() == other->frame_size(), "should be");

  for (int i = 0; i < frame_size(); i++) {
    if (value(i) != other->value(i))
      set_value(i, NULL);
  }
}
