/* PulseAudioTargetDataLineTest.java
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

import java.io.File;
import java.io.IOException;

import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioInputStream;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.DataLine;
import javax.sound.sampled.Line;
import javax.sound.sampled.LineEvent;
import javax.sound.sampled.LineListener;
import javax.sound.sampled.LineUnavailableException;
import javax.sound.sampled.Mixer;
import javax.sound.sampled.TargetDataLine;
import javax.sound.sampled.UnsupportedAudioFileException;

import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;

public class PulseAudioTargetDataLineTest {

	private Mixer mixer;
	private TargetDataLine targetDataLine;

	int started = 0;
	int stopped = 0;

	AudioFormat aSupportedFormat = new AudioFormat(
			AudioFormat.Encoding.PCM_UNSIGNED, 44100f, 8, 1, 1, 44100f, true);

	class ThreadReader extends Thread {
		TargetDataLine line;
		byte[] buffer;

		public ThreadReader(TargetDataLine line, byte[] buffer)
				throws LineUnavailableException {

			this.line = line;
			this.buffer = buffer;

		}

		@Override
		public void run() {
			int bytesRead = 0;

			bytesRead = line.read(buffer, 0, buffer.length);
			// System.out.println("read data");

		}
	}

	@Before
	public void setUp() throws LineUnavailableException {
		Mixer.Info[] mixerInfos = AudioSystem.getMixerInfo();
		Mixer.Info wantedMixerInfo = null;
		for (Mixer.Info mixerInfo : mixerInfos) {
			if (mixerInfo.getName().contains("PulseAudio")) {
				wantedMixerInfo = mixerInfo;
			}
		}
		Assert.assertNotNull(wantedMixerInfo);
		mixer = AudioSystem.getMixer(wantedMixerInfo);
		Assert.assertNotNull(mixer);
		mixer.open();
		targetDataLine = null;
		started = 0;
		stopped = 0;

	}

	@Test
	public void testOpenClose() throws LineUnavailableException {
		targetDataLine = (TargetDataLine) mixer.getLine(new Line.Info(
				TargetDataLine.class));
		Assert.assertNotNull(targetDataLine);
		targetDataLine.open();
		targetDataLine.close();
	}

	@Test
	public void testIsActiveAndIsOpen() throws LineUnavailableException {

		TargetDataLine line = (TargetDataLine) mixer.getLine(new DataLine.Info(
				TargetDataLine.class, aSupportedFormat, 1000));

		Assert.assertFalse(line.isActive());
		Assert.assertFalse(line.isOpen());
		line.open();
		Assert.assertTrue(line.isOpen());
		Assert.assertFalse(line.isActive());
		line.start();
		Assert.assertTrue(line.isOpen());
		Assert.assertTrue(line.isActive());
		line.stop();
		Assert.assertTrue(line.isOpen());
		Assert.assertFalse(line.isActive());
		line.close();
		Assert.assertFalse(line.isOpen());
		Assert.assertFalse(line.isActive());

	}

	@Test
	public void testOpenEvents() throws LineUnavailableException {
		targetDataLine = (TargetDataLine) mixer.getLine(new Line.Info(
				TargetDataLine.class));
		Assert.assertNotNull(targetDataLine);

		LineListener openListener = new LineListener() {
			private int calledCount = 0;

			@Override
			public void update(LineEvent event) {
				Assert.assertEquals(LineEvent.Type.OPEN, event.getType());
				System.out.println("OPEN");
				calledCount++;
				Assert.assertEquals(1, calledCount);
			}

		};

		targetDataLine.addLineListener(openListener);
		targetDataLine.open();
		targetDataLine.removeLineListener(openListener);
		targetDataLine.close();

	}

	@Test
	public void testOpenWithFormat() throws LineUnavailableException {
		System.out.println("This test checks that open(AudioFormat) works");

		targetDataLine = (TargetDataLine) mixer.getLine(new Line.Info(
				TargetDataLine.class));
		Assert.assertNotNull(targetDataLine);
		targetDataLine.open(aSupportedFormat);

	}

	@Test
	public void testRead() throws LineUnavailableException {
		System.out.println("This test checks that read() sort of wroks");

		targetDataLine = (TargetDataLine) mixer.getLine(new Line.Info(
				TargetDataLine.class));
		Assert.assertNotNull(targetDataLine);
		targetDataLine.open(aSupportedFormat);

		byte[] buffer = new byte[1000];
		for (int i = 0; i < buffer.length; i++) {
			buffer[i] = 0;
		}
		targetDataLine.start();

		targetDataLine.read(buffer, 0, buffer.length);
		Assert.assertTrue(buffer[999] != 0);

		buffer = new byte[1000];
		for (int i = 0; i < buffer.length; i++) {
			buffer[i] = 0;
		}

		targetDataLine.read(buffer, 0, buffer.length - 2);
		Assert.assertTrue(buffer[999] == 0);

		targetDataLine.stop();
		targetDataLine.close();

	}

	@Test(expected = IllegalArgumentException.class)
	public void testReadLessThanFrameSize() throws LineUnavailableException {
		System.out.println("This test checks that read() throws an exception "
				+ "when not reading an integral number of frames");

		targetDataLine = (TargetDataLine) mixer.getLine(new Line.Info(
				TargetDataLine.class));
		Assert.assertNotNull(targetDataLine);
		AudioFormat breakingFormat = new AudioFormat(
				AudioFormat.Encoding.PCM_UNSIGNED, 44100f, 8, 2, 2, 44100f,
				true);
		targetDataLine.open(breakingFormat);

		byte[] buffer = new byte[1000];
		for (int i = 0; i < buffer.length; i++) {
			buffer[i] = 0;
		}
		targetDataLine.start();

		targetDataLine.read(buffer, 0, buffer.length - 1);

		targetDataLine.stop();
		targetDataLine.close();

	}

	@Test
	public void testReadAndClose() throws LineUnavailableException,
			InterruptedException {
		System.out.println("This test tries to close a line while "
				+ "read()ing to check that read() returns");
		targetDataLine = (TargetDataLine) mixer.getLine(new Line.Info(
				TargetDataLine.class));
		Assert.assertNotNull(targetDataLine);
		AudioFormat breakingFormat = new AudioFormat(
				AudioFormat.Encoding.PCM_UNSIGNED, 44100f, 8, 2, 2, 44100f,
				true);
		targetDataLine.open(breakingFormat);
		targetDataLine.start();
		byte[] buffer = new byte[1000000];

		ThreadReader reader = new ThreadReader(targetDataLine, buffer);
		reader.start();

		Thread.sleep(100);

		Assert.assertTrue(reader.isAlive());

		targetDataLine.close();

		reader.join(500);

		Assert.assertFalse(reader.isAlive());

	}

	@Test
	public void testReadAndStop() throws LineUnavailableException,
			InterruptedException {
		targetDataLine = (TargetDataLine) mixer.getLine(new Line.Info(
				TargetDataLine.class));
		Assert.assertNotNull(targetDataLine);
		AudioFormat breakingFormat = new AudioFormat(
				AudioFormat.Encoding.PCM_UNSIGNED, 44100f, 8, 2, 2, 44100f,
				true);
		targetDataLine.open(breakingFormat);
		targetDataLine.start();
		byte[] buffer = new byte[10000000];

		ThreadReader reader = new ThreadReader(targetDataLine, buffer);
		reader.start();

		Thread.sleep(100);

		Assert.assertTrue(reader.isAlive());

		targetDataLine.stop();

		Thread.sleep(100);

		Assert.assertFalse(reader.isAlive());

		targetDataLine.close();

	}

	// this is kind of messed up
	// drain should hang on a started data line
	// but read should return
	@Test
	public void testReadAndDrain() throws LineUnavailableException,
			InterruptedException {
		targetDataLine = (TargetDataLine) mixer.getLine(new Line.Info(
				TargetDataLine.class));
		Assert.assertNotNull(targetDataLine);
		AudioFormat breakingFormat = new AudioFormat(
				AudioFormat.Encoding.PCM_UNSIGNED, 44100f, 8, 2, 2, 44100f,
				true);
		targetDataLine.open(breakingFormat);
		targetDataLine.start();
		byte[] buffer = new byte[10000000];

		ThreadReader reader = new ThreadReader(targetDataLine, buffer);
		reader.start();

		Thread.sleep(100);

		Assert.assertTrue(reader.isAlive());

		Thread drainer = new Thread() {

			@Override
			public void run() {
				targetDataLine.drain();

			}

		};

		drainer.start();

		Thread.sleep(100);

		Assert.assertFalse(reader.isAlive());

		targetDataLine.stop();

		Thread.sleep(100);
		Assert.assertFalse(drainer.isAlive());

		targetDataLine.close();
	}

	@Test
	public void testReadAndFlush() throws LineUnavailableException,
			InterruptedException {
		targetDataLine = (TargetDataLine) mixer.getLine(new Line.Info(
				TargetDataLine.class));
		Assert.assertNotNull(targetDataLine);
		AudioFormat breakingFormat = new AudioFormat(
				AudioFormat.Encoding.PCM_UNSIGNED, 44100f, 8, 2, 2, 44100f,
				true);
		targetDataLine.open(breakingFormat);
		targetDataLine.start();
		byte[] buffer = new byte[10000000];

		ThreadReader reader = new ThreadReader(targetDataLine, buffer);
		reader.start();

		Thread.sleep(100);

		Assert.assertTrue(reader.isAlive());

		targetDataLine.flush();

		Thread.sleep(100);

		Assert.assertFalse(reader.isAlive());

		targetDataLine.stop();
		targetDataLine.close();
	}

	@Test
	public void testDrain() throws LineUnavailableException,
			InterruptedException {
		System.out
				.println("This test checks that drain() on a start()ed TargetDataLine hangs");

		targetDataLine = (TargetDataLine) mixer.getLine(new Line.Info(
				TargetDataLine.class));
		Assert.assertNotNull(targetDataLine);

		targetDataLine.open();
		targetDataLine.start();

		Thread th = new Thread(new Runnable() {

			@Override
			public void run() {
				targetDataLine.drain();
			}

		});

		th.start();

		th.join(5000);

		if (!th.isAlive()) {
			targetDataLine.stop();
			th.join();
			targetDataLine.close();
			Assert.fail("drain() on a opened TargetDataLine should hang");
		}
	}

	@Test(expected = IllegalStateException.class)
	public void testDrainWihtoutOpen() throws LineUnavailableException {
		System.out
				.println("This test checks that drain() fails on a line not opened");

		targetDataLine = (TargetDataLine) mixer.getLine(new Line.Info(
				TargetDataLine.class));
		Assert.assertNotNull(targetDataLine);

		targetDataLine.drain();

	}

	@Test
	public void testFlush() throws LineUnavailableException {
		System.out.println("This test checks that flush() wroks");

		targetDataLine = (TargetDataLine) mixer.getLine(new Line.Info(
				TargetDataLine.class));
		Assert.assertNotNull(targetDataLine);
		targetDataLine.open();

		byte[] buffer = new byte[1000];
		for (int i = 0; i < buffer.length; i++) {
			buffer[i] = 0;
		}
		targetDataLine.start();

		targetDataLine.read(buffer, 0, buffer.length);
		targetDataLine.stop();
		targetDataLine.flush();
		targetDataLine.close();
	}

	@Test(expected = IllegalStateException.class)
	public void testFlushWithoutOpen() throws LineUnavailableException {
		System.out
				.println("This test checks that flush() fails on a line not opened");

		targetDataLine = (TargetDataLine) mixer.getLine(new Line.Info(
				TargetDataLine.class));
		Assert.assertNotNull(targetDataLine);

		targetDataLine.flush();
	}

	@Test
	public void testCloseEvents() throws LineUnavailableException {
		targetDataLine = (TargetDataLine) mixer.getLine(new Line.Info(
				TargetDataLine.class));
		Assert.assertNotNull(targetDataLine);

		LineListener closeListener = new LineListener() {
			private int calledCount = 0;

			@Override
			public void update(LineEvent event) {
				Assert.assertEquals(LineEvent.Type.CLOSE, event.getType());
				System.out.println("CLOSE");
				calledCount++;
				Assert.assertEquals(1, calledCount);
			}

		};

		targetDataLine.open();
		targetDataLine.addLineListener(closeListener);
		targetDataLine.close();
		targetDataLine.removeLineListener(closeListener);

	}

	@Test
	public void testStartedStopped() throws LineUnavailableException,
			UnsupportedAudioFileException, IOException, InterruptedException {

		File soundFile = new File("testsounds/startup.wav");
		AudioInputStream audioInputStream = AudioSystem
				.getAudioInputStream(soundFile);
		AudioFormat audioFormat = audioInputStream.getFormat();

		TargetDataLine line;
		line = (TargetDataLine) mixer.getLine(new DataLine.Info(
				TargetDataLine.class, audioFormat));
		Assert.assertNotNull(line);

		started = 0;
		stopped = 0;

		line.open(audioFormat);

		LineListener startStopListener = new LineListener() {

			@Override
			public void update(LineEvent event) {
				if (event.getType() == LineEvent.Type.START) {
					System.out.println("START");
					started++;
					Assert.assertEquals(1, started);
				}

				if (event.getType() == LineEvent.Type.STOP) {
					System.out.println("STOP");
					stopped++;
					Assert.assertEquals(1, stopped);
				}
			}

		};

		line.addLineListener(startStopListener);

		line.start();

		Thread.sleep(100);

		line.stop();
		line.close();

		Assert.assertEquals(1, started);
		Assert.assertEquals(1, stopped);

	}

	@Test
	public void testMixerKnowsAboutOpenLines() throws LineUnavailableException {
		targetDataLine = (TargetDataLine) mixer.getLine(new Line.Info(
				TargetDataLine.class));

		int initiallyOpen = mixer.getTargetLines().length;
		targetDataLine.open();
		Assert.assertEquals(initiallyOpen+1, mixer.getTargetLines().length);
		targetDataLine.close();
		Assert.assertEquals(initiallyOpen, mixer.getTargetLines().length);

	}

	@Test
	public void testAllTargetLinesClosed() {
		Assert.assertEquals(0, mixer.getTargetLines().length);

	}

	@Test
	public void testTargetLineHasNoControls() throws LineUnavailableException {
		targetDataLine = (TargetDataLine) mixer.getLine(new Line.Info(
				TargetDataLine.class));

		targetDataLine.open();

		Assert.assertEquals(0, targetDataLine.getControls().length);

		targetDataLine.close();
	}

	@Test
	public void testFramePosition() throws LineUnavailableException {
		System.out
				.println("This test tests frame position for a target data line");

		final int CHUNCKS = 100;
		final int BUFFER_SIZE = 1000;
		targetDataLine = (TargetDataLine) mixer.getLine(new Line.Info(
				TargetDataLine.class));

		targetDataLine.open();
		targetDataLine.start();
		byte[] data = new byte[BUFFER_SIZE];

		for (int i = 0; i < CHUNCKS; i++) {
			targetDataLine.read(data, 0, data.length);
		}

		targetDataLine.stop();
		long pos = targetDataLine.getLongFramePosition();
		System.out.println("Frames read: " + pos);
		long expected = BUFFER_SIZE * CHUNCKS
				/ targetDataLine.getFormat().getFrameSize();
		System.out.println("Expected: " + expected);
		long granularity = 2;
		Assert.assertTrue(Math.abs(expected - pos) < granularity);
		targetDataLine.close();
	}

	@Test
	public void testFramePositionWithStartAndStop()
			throws LineUnavailableException, InterruptedException {
		System.out
				.println("This test tests frame position for a target data line");

		final int CHUNCKS = 100;
		final int BUFFER_SIZE = 1000;
		targetDataLine = (TargetDataLine) mixer.getLine(new Line.Info(
				TargetDataLine.class));

		targetDataLine.open();
		targetDataLine.start();
		byte[] data = new byte[BUFFER_SIZE];

		for (int i = 0; i < CHUNCKS; i++) {
			if (i == CHUNCKS / 2) {
				targetDataLine.stop();
				Thread.sleep(1000);
				targetDataLine.start();
			}

			targetDataLine.read(data, 0, data.length);
		}

		targetDataLine.stop();
		long pos = targetDataLine.getLongFramePosition();
		System.out.println("Frames read: " + pos);
		long expected = BUFFER_SIZE * CHUNCKS
				/ targetDataLine.getFormat().getFrameSize();
		System.out.println("Expected: " + expected);
		long granularity = 2;
		Assert.assertTrue(Math.abs(expected - pos) < granularity);
		targetDataLine.close();

	}

	@After
	public void tearDown() {
		if (targetDataLine != null) {
			if (targetDataLine.isActive()) {
				targetDataLine.stop();
			}

			if (targetDataLine.isOpen()) {
				targetDataLine.close();
			}
		}

		if (mixer.isOpen()) {
			mixer.close();
		}
	}

}
