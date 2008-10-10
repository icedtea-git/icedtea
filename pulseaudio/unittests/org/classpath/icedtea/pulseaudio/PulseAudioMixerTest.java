/* PulseAudioMixerTest.java
   Copyright (C) 2008 Red Hat, Inc.

This file is part of IcedTea.

IcedTea is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 2.

IcedTea is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with IcedTea; see the file COPYING.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301 USA.

Linking this library statically or dynamically with other modules is
making a combined work based on this library.  Thus, the terms and
conditions of the GNU General Public License cover the whole
combination.

As a special exception, the copyright holders of this library give you
permission to link this library with independent modules to produce an
executable, regardless of the license terms of these independent
modules, and to copy and distribute the resulting executable under
terms of your choice, provided that you also meet, for each linked
independent module, the terms and conditions of the license of that
module.  An independent module is a module which is not derived from
or based on this library.  If you modify this library, you may extend
this exception to your version of the library, but you are not
obligated to do so.  If you do not wish to do so, delete this
exception statement from your version.
 */

package org.classpath.icedtea.pulseaudio;

import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.DataLine;
import javax.sound.sampled.Line;
import javax.sound.sampled.LineEvent;
import javax.sound.sampled.LineListener;
import javax.sound.sampled.LineUnavailableException;
import javax.sound.sampled.Mixer;
import javax.sound.sampled.Port;
import javax.sound.sampled.SourceDataLine;
import javax.sound.sampled.TargetDataLine;

import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;

public class PulseAudioMixerTest {

	Mixer selectedMixer;

	AudioFormat aSupportedFormat = new AudioFormat(
			AudioFormat.Encoding.PCM_UNSIGNED, 44100f, 8, 1, 1, 44100f, true);
	AudioFormat aNotSupportedFormat = new AudioFormat(
			AudioFormat.Encoding.ULAW, 44100, 32, 10, 10, 44100f, true);

	@Before
	public void setUp() throws Exception {
		Mixer.Info mixerInfos[] = AudioSystem.getMixerInfo();
		Mixer.Info selectedMixerInfo = null;
		// int i = 0;
		for (Mixer.Info info : mixerInfos) {
			// System.out.println("Mixer Line " + i++ + ": " + info.getName() +
			// " " + info.getDescription());
			if (info.getName().contains("PulseAudio")) {
				selectedMixerInfo = info;
			}
		}
		Assert.assertNotNull(selectedMixerInfo);
		selectedMixer = AudioSystem.getMixer(selectedMixerInfo);
		Assert.assertNotNull(selectedMixer);
		if (selectedMixer.isOpen()) {
			selectedMixer.close();
		}

	}

	@Test
	public void testOpenClose() throws LineUnavailableException {
		selectedMixer.open();
		selectedMixer.close();

	}

	@Test
	public void testSourceLinesExist() throws LineUnavailableException {
		System.out.println("This tests that source lines exist");
		selectedMixer.open();
		Line.Info allLineInfo[] = selectedMixer.getSourceLineInfo();
		Assert.assertNotNull(allLineInfo);
		Assert.assertTrue(allLineInfo.length > 0);

		boolean foundSourceDataLine = false;
		boolean foundClip = false;
		boolean foundPort = false;

		int j = 0;
		for (Line.Info lineInfo : allLineInfo) {
			System.out.println("Source Line " + j++ + ": "
					+ lineInfo.getLineClass());
			if (lineInfo.getLineClass().toString().contains("SourceDataLine")) {
				foundSourceDataLine = true;
			} else if (lineInfo.getLineClass().toString().contains("Clip")) {
				foundClip = true;
			} else if (lineInfo.getLineClass().toString().contains("Port")) {
				foundPort = true;
			} else {
				Assert.assertFalse("Found a new type of Line", true);
			}
			Line sourceLine = (Line) selectedMixer.getLine(lineInfo);
			Assert.assertNotNull(sourceLine);

		}

		Assert.assertTrue("Couldnt find a SourceDataLine", foundSourceDataLine);
		Assert.assertTrue("Couldnt find a Clip", foundClip);
		Assert.assertTrue("Couldnt find a Port", foundPort);

	}

	@Test
	public void testTargetLinesExist() throws LineUnavailableException {
		System.out.println("This tests if target Lines exist");
		selectedMixer.open();
		Line.Info allLineInfo[] = selectedMixer.getTargetLineInfo();
		Assert.assertNotNull(allLineInfo);
		Assert.assertTrue(allLineInfo.length > 0);

		boolean foundTargetDataLine = false;
		boolean foundPort = false;

		int j = 0;
		for (Line.Info lineInfo : allLineInfo) {
			System.out.println("Target Line " + j++ + ": "
					+ lineInfo.getLineClass());
			if (lineInfo.getLineClass().toString().contains("TargetDataLine")) {
				foundTargetDataLine = true;
			} else if (lineInfo.getLineClass().toString().contains("Port")) {
				foundPort = true;
			} else {
				Assert.assertTrue("Found invalid type of target line", true);
			}
			Line targetLine = (Line) selectedMixer.getLine(lineInfo);
			Assert.assertNotNull(targetLine);

		}

		Assert.assertTrue("Couldnt find a TargetDataLine", foundTargetDataLine);
		Assert.assertTrue("Couldnt find a target Port", foundPort);

	}

	@Ignore
	@Test
	public void testHeadphonePortExists() throws LineUnavailableException {
		selectedMixer.open();
		selectedMixer.getLine(Port.Info.HEADPHONE);
	}

	@Ignore
	@Test
	public void testSpeakerPortExists() throws LineUnavailableException {
		selectedMixer.open();
		selectedMixer.getLine(Port.Info.SPEAKER);
	}

	@Ignore
	@Test
	public void testLineInPortExists() throws LineUnavailableException {
		selectedMixer.open();
		selectedMixer.getLine(Port.Info.LINE_IN);
	}

	@Ignore
	@Test
	public void testCdPortExists() throws LineUnavailableException {
		selectedMixer.open();
		selectedMixer.getLine(Port.Info.COMPACT_DISC);
	}

	@Ignore
	@Test
	public void testLineOutPortExists() throws LineUnavailableException {
		selectedMixer.open();
		selectedMixer.getLine(Port.Info.LINE_OUT);
	}

	@Ignore
	@Test
	public void testMicrophonePortExists() throws LineUnavailableException {
		selectedMixer.open();
		selectedMixer.getLine(Port.Info.MICROPHONE);
	}

	@Test
	public void testSaneNumberOfPorts() throws LineUnavailableException {
		System.out
				.println("This test checks that a sane number of ports are detected");
		selectedMixer.open();
		Line.Info[] lineInfos = selectedMixer.getSourceLineInfo();
		Assert.assertNotNull(lineInfos);

		int ports = 0;
		for (Line.Info info : lineInfos) {
			if (info instanceof Port.Info) {
				ports++;
			}
		}
		Assert.assertTrue("Too few Source ports", ports > 0);
		Assert.assertTrue("Too many Source ports... this looks wrong",
				ports < 5);

		lineInfos = selectedMixer.getTargetLineInfo();
		ports = 0;
		for (Line.Info info : lineInfos) {
			if (info instanceof Port.Info) {
				ports++;
			}
		}
		Assert.assertTrue("Too few Target ports", ports > 0);
		Assert.assertTrue("Too many Target ports... this looks wrong",
				ports < 5);

	}

	@Test
	public void testGetTargetPortInfo() throws LineUnavailableException {
		System.out.println("This test checks target ports");
		selectedMixer.open();
		Line.Info[] lineInfos = selectedMixer.getTargetLineInfo();
		int i = 0;
		for (Line.Info info : lineInfos) {
			if (info instanceof Port.Info) {
				Port.Info portInfo = (Port.Info) info;
				Assert.assertTrue(portInfo.isSource() == false);
				Assert.assertTrue(portInfo.getLineClass() == Port.class);
				System.out.println("Port " + ++i + ": " + portInfo.getName()
						+ " - " + portInfo.getLineClass());
			}
		}

	}

	@Test
	public void testGetSourcePortInfo() throws LineUnavailableException {
		System.out.println("This test checks source ports");
		selectedMixer.open();
		Line.Info[] lineInfos = selectedMixer.getSourceLineInfo();
		int i = 0;
		for (Line.Info info : lineInfos) {
			if (info instanceof Port.Info) {
				Port.Info portInfo = (Port.Info) info;
				Assert.assertTrue(portInfo.isSource() == true);
				Assert.assertTrue(portInfo.getLineClass() == Port.class);
				System.out.println("Port " + ++i + ": " + portInfo.getName()
						+ " - " + portInfo.getLineClass());
			}
		}

	}

	@Test(expected = IllegalStateException.class)
	public void testOpeningAgain() throws LineUnavailableException {
		selectedMixer.open();
		selectedMixer.open();
	}

	@Test(expected = IllegalStateException.class)
	public void testClosingAgain() throws LineUnavailableException,
			IllegalStateException {
		selectedMixer.close();
		selectedMixer.close();
	}

	@Test
	public void testSourceLinesOpenAndClose() throws LineUnavailableException {
		System.out.println("This test checks if source lines open and close");
		selectedMixer.open();

		Line.Info allLineInfo[] = selectedMixer.getSourceLineInfo();
		for (Line.Info lineInfo : allLineInfo) {
			try {
				Line sourceLine = selectedMixer.getLine(lineInfo);
				sourceLine.open();
				sourceLine.close();
			} catch (IllegalArgumentException e) {
				// ignore this
			}
		}
	}

	@Test
	public void testTargetLinesOpenAndClose() throws LineUnavailableException {
		System.out.println("This test checks if source lines open and close");
		selectedMixer.open();

		Line.Info allLineInfo[] = selectedMixer.getTargetLineInfo();
		for (Line.Info lineInfo : allLineInfo) {
			try {
				TargetDataLine targetLine = (TargetDataLine) selectedMixer
						.getLine(lineInfo);
				Assert.assertNotNull(targetLine);
				targetLine.open(aSupportedFormat);
				targetLine.close();
			} catch (ClassCastException cce) {
				Port targetLine = (Port) selectedMixer.getLine(lineInfo);
				Assert.assertNotNull(targetLine);
				targetLine.open();
				targetLine.close();
			}

		}
	}

	@Test
	public void testOpenEvent() throws LineUnavailableException {
		LineListener listener = new LineListener() {
			private int called = 0;

			@Override
			public void update(LineEvent event) {
				assert (event.getType() == LineEvent.Type.OPEN);
				called++;
				// assert listener is called exactly once
				Assert.assertEquals(1, called);
			}
		};

		selectedMixer.addLineListener(listener);
		selectedMixer.open();
		selectedMixer.removeLineListener(listener);
		selectedMixer.close();

	}

	@Test
	public void testCloseEvent() throws LineUnavailableException {
		LineListener listener = new LineListener() {
			private int count = 0;

			@Override
			public void update(LineEvent event) {
				Assert.assertTrue(event.getType() == LineEvent.Type.CLOSE);
				count++;
				// assert listener is called exactly once
				Assert.assertEquals(1, count);

			}
		};

		selectedMixer.open();
		selectedMixer.addLineListener(listener);
		selectedMixer.close();
		selectedMixer.removeLineListener(listener);

	}

	@Test
	public void testLineSupportedWorksWithoutOpeningMixer() {

		Assert.assertFalse(selectedMixer.isOpen());

		Assert.assertFalse(selectedMixer.isLineSupported(new DataLine.Info(
				SourceDataLine.class, aNotSupportedFormat)));

		Assert.assertTrue(selectedMixer.isLineSupported(new DataLine.Info(
				SourceDataLine.class, aSupportedFormat)));

	}

	@Test
	public void testSynchronizationNotSupported()
			throws LineUnavailableException {
		selectedMixer.open();

		SourceDataLine line1 = (SourceDataLine) selectedMixer
				.getLine(new Line.Info(SourceDataLine.class));
		SourceDataLine line2 = (SourceDataLine) selectedMixer
				.getLine(new Line.Info(SourceDataLine.class));
		Line[] lines = { line1, line2 };

		Assert.assertFalse(selectedMixer
				.isSynchronizationSupported(lines, true));

		Assert.assertFalse(selectedMixer.isSynchronizationSupported(lines,
				false));

		try {
			selectedMixer.synchronize(lines, true);
			Assert.fail("mixer shouldnt be able to synchronize lines");
		} catch (IllegalArgumentException e) {

		}

		try {
			selectedMixer.synchronize(lines, false);
			Assert.fail("mixer shouldnt be able to synchronize lines");
		} catch (IllegalArgumentException e) {

		}

		selectedMixer.close();

	}

	@Ignore
	@Test
	public void testLongWait() throws LineUnavailableException {
		selectedMixer.open();
		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			System.out.println("thread interrupted");
		}

	}

	@After
	public void tearDown() throws Exception {
		if (selectedMixer.isOpen())
			selectedMixer.close();
	}

}
