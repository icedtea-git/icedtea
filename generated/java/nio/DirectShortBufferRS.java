/*
 * Copyright 2000-2005 Sun Microsystems, Inc.  All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Sun designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Sun in the LICENSE file that accompanied this code.
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
 */

// -- This file was mechanically generated: Do not edit! -- //

package java.nio;

import sun.misc.Cleaner;
import sun.misc.Unsafe;
import sun.nio.ch.DirectBuffer;
import sun.nio.ch.FileChannelImpl;


class DirectShortBufferRS



    extends DirectShortBufferS

    implements DirectBuffer
{






















































































































    // For duplicates and slices
    //
    DirectShortBufferRS(DirectBuffer db,         // package-private
                               int mark, int pos, int lim, int cap,
                               int off)
    {








        super(db, mark, pos, lim, cap, off);

    }

    public ShortBuffer slice() {
        int pos = this.position();
        int lim = this.limit();
        assert (pos <= lim);
        int rem = (pos <= lim ? lim - pos : 0);
        int off = (pos << 1);
        assert (off >= 0);
        return new DirectShortBufferRS(this, -1, 0, rem, rem, off);
    }

    public ShortBuffer duplicate() {
        return new DirectShortBufferRS(this,
                                              this.markValue(),
                                              this.position(),
                                              this.limit(),
                                              this.capacity(),
                                              0);
    }

    public ShortBuffer asReadOnlyBuffer() {








        return duplicate();

    }


















































    public ShortBuffer put(short x) {




        throw new ReadOnlyBufferException();

    }

    public ShortBuffer put(int i, short x) {




        throw new ReadOnlyBufferException();

    }

    public ShortBuffer put(ShortBuffer src) {




































        throw new ReadOnlyBufferException();

    }

    public ShortBuffer put(short[] src, int offset, int length) {






















        throw new ReadOnlyBufferException();

    }

    public ShortBuffer compact() {












        throw new ReadOnlyBufferException();

    }

    public boolean isDirect() {
        return true;
    }

    public boolean isReadOnly() {
        return true;
    }













































    public ByteOrder order() {

        return ((ByteOrder.nativeOrder() == ByteOrder.BIG_ENDIAN)
                ? ByteOrder.LITTLE_ENDIAN : ByteOrder.BIG_ENDIAN);





    }


























}
