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

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;

import javax.sound.midi.Patch;
import javax.sound.sampled.AudioFormat;

/**
 * GUS Instrument.
 * 
 * @version %I%, %E%
 * @author Karl Helgason
 */

public class PATInstrument extends ModelInstrument {
	
	private class OffsetInputStream extends InputStream
	{
		
		private InputStream is;
		private int filepointer = 0;
		
		public OffsetInputStream(InputStream is)
		{
			this.is = is;
		}
		
		public long getFilePointer() throws IOException {
			return filepointer;
		}		
		
		public void close() throws IOException {
			is.close();
		}

		public int read(byte[] b, int off, int len) throws IOException {
			int ret = is.read(b, off, len);
			if(ret != -1) filepointer+=ret;
			return ret;			
		}

		public long skip(long n) throws IOException {
			long ret = is.skip(n);
			if(ret != -1) filepointer+=ret;
			return ret;
		}

		public int read() throws IOException {
			int ret = is.read();
			if(ret != -1) filepointer++;
			return ret;
		}
		
	}

	protected int preset = 0;

	protected int bank = 0;

	protected boolean percussion = false;

	protected String name = "";

	protected String description = "";

	protected String format = "GF1PATCH110";

	protected String vendor = "ID#000002";

	protected PATSample[] samples = null;

	protected int volume;
	
	private boolean largeFormat = false;
	private File sampleFile;		
	private OffsetInputStream ois;

	public PATInstrument() {
		super(null, null, null, null);
	}

	public PATInstrument(URL url) throws IOException {
		super(null, null, null, null);

		InputStream is = url.openStream();
		try {
			readPatch(is);
		} finally {
			is.close();
		}
	}

	public PATInstrument(File file) throws IOException {
		super(null, null, null, null);
		largeFormat = true;
		sampleFile = file;
		InputStream is = new FileInputStream(file);
		ois = new OffsetInputStream(is);
		is = ois;
		try {
			readPatch(is);
		} finally {
			is.close();
		}
	}	

	public PATInstrument(InputStream inputstream) throws IOException {
		super(null, null, null, null);
		readPatch(inputstream);
	}

	private String readString(InputStream is, int len) throws IOException {
		byte[] buffer = new byte[len];
		is.read(buffer);
		for (int i = 0; i < buffer.length; i++) {
			if (buffer[i] == (byte) 0) {
				return new String(buffer, 0, i, "ASCII");
			}
		}
		return new String(buffer, "ASCII");
	}

	private void readPatch(InputStream inputstream) throws IOException {
		format = readString(inputstream, 12);
		vendor = readString(inputstream, 10);
		if (!(format.equals("GF1PATCH110") || format.equals("GF1PATCH100")))
			throw new InvalidFormatException();
		if (!(vendor.equals("ID#000002")))
			throw new InvalidFormatException();
		description = readString(inputstream, 60);

		int instrument_count = inputstream.read();
		if (instrument_count > 1)
			throw new InvalidDataException("Invalid instrument count.");
		@SuppressWarnings("unused")
		int voices = inputstream.read();
		int channels = inputstream.read();
		if (channels > 1)
			throw new InvalidDataException("Invalid channels count.");
		@SuppressWarnings("unused")
		int waveforms = inputstream.read() + (inputstream.read() << 8);
		volume = inputstream.read() + (inputstream.read() << 8);
		@SuppressWarnings("unused")
		int datasize = inputstream.read() + (inputstream.read() << 8)
				+ (inputstream.read() << 16) + (inputstream.read() << 24);
		byte[] reserve1 = new byte[36];
		inputstream.read(reserve1);
		@SuppressWarnings("unused")
		int instrumentid = inputstream.read() + (inputstream.read() << 8);
		name = readString(inputstream, 16);
		@SuppressWarnings("unused")
		int instrumentsize = inputstream.read() + (inputstream.read() << 8)
				+ (inputstream.read() << 16) + (inputstream.read() << 24);
		int layers = inputstream.read();
		if (layers > 1)
			throw new InvalidDataException("Invalid layers count.");
		byte[] reserve2 = new byte[40];
		inputstream.read(reserve2);
		@SuppressWarnings("unused")
		int layerdupl = inputstream.read();
		@SuppressWarnings("unused")
		int layer = inputstream.read();
		@SuppressWarnings("unused")
		int layersize = inputstream.read() + (inputstream.read() << 8)
				+ (inputstream.read() << 16) + (inputstream.read() << 24);
		int samples = inputstream.read();
		byte[] reserve3 = new byte[40];
		inputstream.read(reserve3);

		this.samples = new PATSample[samples];
		for (int i = 0; i < samples; i++) {
			this.samples[i] = readSample(inputstream);
		}
	}

	private PATSample readSample(InputStream inputstream) throws IOException {
		PATSample patsample = new PATSample();

		patsample.wavename = readString(inputstream, 7);
		patsample.fractions = inputstream.read();
		int sampledatasize = inputstream.read() + (inputstream.read() << 8)
				+ (inputstream.read() << 16) + (inputstream.read() << 24);
		patsample.loopstart = inputstream.read() + (inputstream.read() << 8)
				+ (inputstream.read() << 16) + (inputstream.read() << 24);
		patsample.loopend = inputstream.read() + (inputstream.read() << 8)
				+ (inputstream.read() << 16) + (inputstream.read() << 24);
		patsample.samplerate = inputstream.read() + (inputstream.read() << 8);
		patsample.lowfreq = inputstream.read() + (inputstream.read() << 8)
				+ (inputstream.read() << 16) + (inputstream.read() << 24);
		patsample.hifreq = inputstream.read() + (inputstream.read() << 8)
				+ (inputstream.read() << 16) + (inputstream.read() << 24);
		patsample.rootfreq = inputstream.read() + (inputstream.read() << 8)
				+ (inputstream.read() << 16) + (inputstream.read() << 24);
		patsample.tune = inputstream.read() + (inputstream.read() << 8);
		patsample.pan = inputstream.read();

		patsample.env_attack_rate = inputstream.read();
		patsample.env_decay_rate = inputstream.read();
		patsample.env_sustain_rate = inputstream.read();
		patsample.env_release1_rate = inputstream.read();
		patsample.env_release2_rate = inputstream.read();
		patsample.env_release3_rate = inputstream.read();

		patsample.env_attack_offset = inputstream.read();
		patsample.env_decay_offset = inputstream.read();
		patsample.env_sustain_offset = inputstream.read();
		patsample.env_release1_offset = inputstream.read();
		patsample.env_release2_offset = inputstream.read();
		patsample.env_release3_offset = inputstream.read();

		patsample.tremolo_sweep = inputstream.read();
		patsample.tremolo_rate = inputstream.read();
		patsample.tremolo_depth = inputstream.read();
		patsample.vibrato_sweep = inputstream.read();
		patsample.vibrato_rate = inputstream.read();
		patsample.vibrato_depth = inputstream.read();
		patsample.sampling_mode = inputstream.read();
		patsample.scalefreq = inputstream.read() + (inputstream.read() << 8);
		patsample.scalefactor = inputstream.read() + (inputstream.read() << 8);
		byte[] reserve1 = new byte[36];
		inputstream.read(reserve1);
		
		if(largeFormat)
		{
			long offset = ois.getFilePointer();
			inputstream.skip(sampledatasize);
			patsample.sampledata = new ModelByteBuffer(sampleFile, offset, sampledatasize);
		}
		else
		{		
			byte[] buff = new byte[sampledatasize];
			inputstream.read(buff);
			patsample.sampledata = new ModelByteBuffer(buff);
		}
		
		return patsample;

	}

	public Object getData() {
		return null;
	}

	public String getName() {
		return this.name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public ModelPatch getPatch() {
		return new ModelPatch(bank, preset, percussion);
	}

	public void setPatch(Patch patch) {
		if (patch instanceof ModelPatch && ((ModelPatch) patch).isPercussion()) {
			percussion = true;
			bank = patch.getBank();
			preset = patch.getProgram();
		} else {
			percussion = false;
			bank = patch.getBank();
			preset = patch.getProgram();
		}
	}

	public String getDescription() {
		return description;
	}

	public void setDescription(String description) {
		this.description = description;
	}

	public String getFormat() {
		return format;
	}

	public void setFormat(String format) {
		this.format = format;
	}

	public String getVendor() {
		return vendor;
	}

	public void setVendor(String vendor) {
		this.vendor = vendor;
	}

	private double convertRate(int rate) {
		return (rate & 63) * Math.pow(1.0 / 2.0, 3 * ((rate >> 6) & 3));
	}

	public ModelPerformer[] getPerformers() {

		ModelPerformer[] performers = new ModelPerformer[samples.length];
		for (int i = 0; i < performers.length; i++) {
			ModelPerformer performer = new ModelPerformer();			
			PATSample patsample = samples[i];
			performer.setName(samples[i].wavename);
			
			double keyfrom = 69 + 12
					* Math.log((patsample.lowfreq / 1000.0) / 440.0)
					/ Math.log(2.0);
			double keyto = 69 + 12
				* Math.log((patsample.hifreq / 1000.0) / 440.0)
				/ Math.log(2.0);
			
			performer.setKeyFrom((int)Math.ceil(keyfrom));
			performer.setKeyTo((int)Math.floor(keyto));
			
			ModelByteBuffer buffer = patsample.sampledata;

			boolean is16bit = (patsample.sampling_mode & 1) != 0;
			boolean isUnsigned = (patsample.sampling_mode & 2) != 0;
			boolean isLooping = (patsample.sampling_mode & 4) != 0;
			boolean isPingPongLoop = (patsample.sampling_mode & 8) != 0;
			boolean isReverseLoop = (patsample.sampling_mode & 16) != 0;
			//boolean isSustain = (patsample.sampling_mode & 32) != 0;
			boolean isEnv = (patsample.sampling_mode & 64) != 0;
			//boolean isClampedRelease = (patsample.sampling_mode & 128) != 0;
			
						
			AudioFormat audioformat = null;
			if (is16bit)
				audioformat = new AudioFormat(patsample.samplerate, 16, 1,
						!isUnsigned, false);
			else
				audioformat = new AudioFormat(patsample.samplerate, 8, 1,
						!isUnsigned, false);

			float pitchcorrection = -(float) (69 + 12
					* Math.log((patsample.rootfreq / 1000.0) / 440.0)
					/ Math.log(2.0));
			pitchcorrection *= 100;			

			ModelByteBufferWavetable osc = new ModelByteBufferWavetable(buffer,
					audioformat, pitchcorrection);
			
			float loopstart = patsample.loopstart + ((patsample.fractions&0xF)/16f);
			float loopend = patsample.loopend + ((patsample.fractions>>4)/16f);
			
			if (isLooping) {
				if (is16bit) {
					osc.setLoopStart(loopstart / 2f);
					osc.setLoopLength(loopend / 2f
							- loopstart / 2f);
				} else {
					osc.setLoopStart(loopstart);
					osc.setLoopLength(loopend - loopstart);
				}				
				osc.setLoopType(ModelWavetable.LOOP_TYPE_FORWARD);
				if(isPingPongLoop)
					osc.setLoopType(ModelWavetable.LOOP_TYPE_PINGPONG);
				if(isReverseLoop)
					osc.setLoopType(ModelWavetable.LOOP_TYPE_REVERSE);
			}
			performer.getOscillators().add(osc);

			double lfo_vib_rate = patsample.vibrato_rate / 42.0;
			double lfo_vol_rate = patsample.tremolo_rate / 42.0;
			double lfo_vib_depth = 400.0 * patsample.vibrato_depth / 256.0;
			
			performer.getConnectionBlocks().add(
					new ModelConnectionBlock(6900 + 1200.0
							* Math.log(lfo_vol_rate / 440.0) / Math.log(2),
							new ModelDestination(
									ModelDestination.DESTINATION_LFO1_FREQ)));
			if(lfo_vib_depth > 0)
				performer.getConnectionBlocks().add(
					new ModelConnectionBlock(6900 + 1200.0
							* Math.log(lfo_vib_rate / 440.0) / Math.log(2),
							new ModelDestination(
									ModelDestination.DESTINATION_LFO2_FREQ)));

			
			if (lfo_vib_depth > 0) {
				ModelIdentifier src = ModelSource.SOURCE_LFO2;
				ModelIdentifier dest = ModelDestination.DESTINATION_PITCH;
				performer.getConnectionBlocks().add(
						new ModelConnectionBlock(new ModelSource(src,
								ModelStandardTransform.DIRECTION_MIN2MAX,
								ModelStandardTransform.POLARITY_BIPOLAR),
								lfo_vib_depth, new ModelDestination(dest)));
			}

			double lfo_vol_depth = -200
					* Math.log(1.0 - patsample.tremolo_depth / 256.0)
					/ Math.log(10);
			if (lfo_vol_depth > 960)
				lfo_vol_depth = 960;
			if (lfo_vol_depth > 0) {
				ModelIdentifier src = ModelSource.SOURCE_LFO1;
				ModelIdentifier dest = ModelDestination.DESTINATION_GAIN;
				performer.getConnectionBlocks().add(
						new ModelConnectionBlock(new ModelSource(src,
								ModelStandardTransform.DIRECTION_MIN2MAX,
								ModelStandardTransform.POLARITY_BIPOLAR),
								lfo_vol_depth, new ModelDestination(dest)));
			}

			double volume = this.volume / 128.0;
			if (isEnv) {
				double env_attack_msec = patsample.env_attack_offset
						/ convertRate(patsample.env_attack_rate);
				double env_decay_msec = 0.5*(patsample.env_attack_offset - patsample.env_decay_offset)
						/ convertRate(patsample.env_decay_rate);			
				env_decay_msec += 0.5*(patsample.env_decay_offset - patsample.env_sustain_offset)
						/ convertRate(patsample.env_sustain_rate); 				
				double env_release_msec = 0.5*(patsample.env_attack_offset - 0)
						/ convertRate(patsample.env_release1_rate);
				/*  patsample.env_release1_offset
				env_release_msec += (patsample.env_release1_offset - patsample.env_release2_offset)
				/ convertRate(patsample.env_release2_rate);
				env_release_msec += (patsample.env_release2_offset - patsample.env_release3_offset)
				/ convertRate(patsample.env_release3_rate);
				 */
				double env_volume_level = patsample.env_attack_offset / 256.0;
				double env_sustain_level = patsample.env_sustain_offset / 256.0;
				if (env_attack_msec < 0)
					env_attack_msec = 0;
				if (env_decay_msec < 0)
					env_decay_msec = 0;
				if (env_release_msec < 0)
					env_release_msec = 0;
				volume *= env_volume_level;
				performer
						.getConnectionBlocks()
						.add(
								new ModelConnectionBlock(
										env_sustain_level * 1000.0,
										new ModelDestination(
												ModelDestination.DESTINATION_EG1_SUSTAIN)));
				performer.getConnectionBlocks().add(
						new ModelConnectionBlock(1200.0
								* Math.log(env_attack_msec / 1000.0)
								/ Math.log(2), new ModelDestination(
								ModelDestination.DESTINATION_EG1_ATTACK)));
				performer.getConnectionBlocks().add(
						new ModelConnectionBlock(1200.0
								* Math.log(env_decay_msec / 1000.0)
								/ Math.log(2), new ModelDestination(
								ModelDestination.DESTINATION_EG1_DECAY)));
				performer.getConnectionBlocks().add(
						new ModelConnectionBlock(1200.0
								* Math.log(env_release_msec / 1000.0)
								/ Math.log(2), new ModelDestination(
								ModelDestination.DESTINATION_EG1_RELEASE)));
			} else {
				if (!isLooping)
					performer
							.getConnectionBlocks()
							.add(
									new ModelConnectionBlock(
											12000.0,
											new ModelDestination(
													ModelDestination.DESTINATION_EG1_RELEASE)));
			}

			// Linear gain to dB
			double gain = ((20.0) / Math.log(10)) * Math.log(volume);
			performer.getConnectionBlocks().add(
					new ModelConnectionBlock(gain, new ModelDestination(
							ModelDestination.DESTINATION_GAIN)));

			double panning = 500;
			if (patsample.pan < 7)
				panning = (patsample.pan / 7.0) * 500.0;
			if (patsample.pan > 8)
				panning += ((patsample.pan - 8) / 7.0) * 500.0;
			performer.getConnectionBlocks().add(
					new ModelConnectionBlock(panning - 500.0,
							new ModelDestination(
									ModelDestination.DESTINATION_PAN)));

			// Modulation wheel should use SOURCE_LFO2 (vib)
			performer.getConnectionBlocks().add(
					new ModelConnectionBlock(new ModelSource(
							ModelSource.SOURCE_LFO2,
							ModelStandardTransform.DIRECTION_MIN2MAX,
							ModelStandardTransform.POLARITY_BIPOLAR,
							ModelStandardTransform.TRANSFORM_LINEAR),
							new ModelSource(new ModelIdentifier("midi_cc", "1",
									0),
									ModelStandardTransform.DIRECTION_MIN2MAX,
									ModelStandardTransform.POLARITY_UNIPOLAR,
									ModelStandardTransform.TRANSFORM_LINEAR),
							50, new ModelDestination(
									ModelDestination.DESTINATION_PITCH)));

			performers[i] = performer;
		}

		return performers;
	}

	public PATSample[] getSamples() {
		return samples;
	}

	public void setSamples(PATSample[] samples) {
		this.samples = samples;
	}

}
