/*
 * Copyright 2007 Sun Microsystems, Inc.  All Rights Reserved.
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

package com.sun.media.sound;

/**
 * GUS Sample storage.
 * 
 * @version %I%, %E%
 * @author Karl Helgason
 */

public class PATSample {

	public String wavename = "";

	public int fractions; 

	// bit 0..3: Loop offset start fractions [0/16 .. 15/16]
	// bit 4..7: Loop offset end fractions [0/16 .. 15/16]

	public int loopstart;

	public int loopend;

	public int samplerate;

	public int lowfreq;

	public int hifreq;

	public int rootfreq;

	public int tune; // not used

	public int pan; // 0..15, 7 and 8 are center

	public int env_attack_rate;

	public int env_decay_rate;

	public int env_sustain_rate;

	public int env_release1_rate;

	public int env_release2_rate; // ignored

	public int env_release3_rate; // ignored

	public int env_attack_offset;

	public int env_decay_offset;

	public int env_sustain_offset;

	public int env_release1_offset;

	public int env_release2_offset; // ignored

	public int env_release3_offset; // ignored

	public int tremolo_sweep; // ignored

	public int tremolo_rate;

	public int tremolo_depth;

	public int vibrato_sweep; // ignored

	public int vibrato_rate;

	public int vibrato_depth;

	public int sampling_mode;

	// bit 0: 16-bit (versus 8-bit)
	// bit 1: Unsigned (versus signed)
	// bit 2: Looped
	// bit 3: Pingpong Loop
	// bit 4: Reverse
	// bit 5: Sustain <- Ignored
	// bit 6: Envelope
	// bit 7: Clamped release (6th point of envelope) <- Ignored

	public int scalefreq; // ignored

	public int scalefactor; // ignored

	public ModelByteBuffer sampledata;

}
