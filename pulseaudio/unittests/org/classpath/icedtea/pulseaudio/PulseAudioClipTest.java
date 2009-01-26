/* PulseAudioClipTest.java
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
import javax.sound.sampled.Clip;
import javax.sound.sampled.Control;
import javax.sound.sampled.DataLine;
import javax.sound.sampled.Line;
import javax.sound.sampled.LineEvent;
import javax.sound.sampled.LineListener;
import javax.sound.sampled.LineUnavailableException;
import javax.sound.sampled.Mixer;
import javax.sound.sampled.UnsupportedAudioFileException;

import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

public class PulseAudioClipTest {

	Mixer mixer;
	AudioFormat aSupportedFormat = new AudioFormat(
			AudioFormat.Encoding.PCM_UNSIGNED, 44100f, 8, 1, 1, 44100f, true);

	int started = 0;
	int stopped = 0;
	int opened = 0;
	int closed = 0;

	@Before
	public void setUp() throws LineUnavailableException {
		Mixer.Info wantedMixerInfo = null;
		Mixer.Info[] mixerInfos = AudioSystem.getMixerInfo();
		for (Mixer.Info mixerInfo : mixerInfos) {
			if (mixerInfo.getName().contains("PulseAudio")) {
				wantedMixerInfo = mixerInfo;
				break;
			}
		}
		assert (wantedMixerInfo != null);
		mixer = AudioSystem.getMixer(wantedMixerInfo);
		mixer.open();

		started = 0;
		stopped = 0;
		opened = 0;
		closed = 0;
	}

	@Test
	public void testObtainingAClip() throws LineUnavailableException {
		System.out
				.println("This tests if a clip can be obtained from the mixer");
		Clip clip = (Clip) mixer.getLine(new Line.Info(Clip.class));
		Assert.assertNotNull(clip);
	}

	@Test(expected = IllegalArgumentException.class)
	public void testClipOpenWrongUse() throws LineUnavailableException {
		System.out.println("This test checks ");
		Clip clip = (Clip) mixer.getLine(new Line.Info(Clip.class));
		clip.open();
	}

	@Test
	public void testClipOpens() throws LineUnavailableException,
			UnsupportedAudioFileException, IOException {
		Clip clip = (Clip) mixer.getLine(new Line.Info(Clip.class));
		File soundFile = new File("testsounds/startup.wav");
		AudioInputStream audioInputStream = AudioSystem
				.getAudioInputStream(soundFile);
		clip.open(audioInputStream);
		clip.close();
	}

	@Test
	public void testLoop4Times() throws LineUnavailableException, IOException,
			UnsupportedAudioFileException {
		System.out
				.println("This tests loop(4) on the Clip. "
						+ "You should hear a certain part of the clip play back 5 time");

		Clip clip = (Clip) mixer.getLine(new Line.Info(Clip.class));
		File soundFile = new File("testsounds/startup.wav");
		AudioInputStream audioInputStream = AudioSystem
				.getAudioInputStream(soundFile);
		clip.open(audioInputStream);

		clip.setLoopPoints((int) (clip.getFrameLength() / 4), (int) (clip
				.getFrameLength() / 2));
		clip.loop(4);

		clip.drain();

		clip.stop();

		clip.close();
	}
	
	
	@Test
	public void testLoopContinuously() throws LineUnavailableException,
			IOException, UnsupportedAudioFileException, InterruptedException {
		System.out
				.println("This tests loop(LOOP_CONTINUOUSLY) on the Clip");
		final Clip clip = (Clip) mixer.getLine(new Line.Info(Clip.class));
		File soundFile = new File("testsounds/error.wav");
		AudioInputStream audioInputStream = AudioSystem
				.getAudioInputStream(soundFile);
		clip.open(audioInputStream);

		clip.setLoopPoints((int) (clip.getFrameLength() / 4), (int) (clip
				.getFrameLength() / 2));
		clip.loop(Clip.LOOP_CONTINUOUSLY);

		Runnable blocker = new Runnable() {

			@Override
			public void run() {
				clip.drain();
			}

		};

		Thread th = new Thread(blocker);
		th.start();
		th.join(10000);

		if (!th.isAlive()) {
			clip.close();
			Assert.fail("LOOP_CONTINUOUSLY doesnt seem to work");
		}
		
		clip.stop();
		th.join(500);
		if ( th.isAlive()) {
			clip.close();
			Assert.fail("stopping LOOP_CONTINUOSLY failed");
		}
		
		clip.close();
	}

	@Test
	public void testIsActiveAndIsOpen() throws LineUnavailableException,
			UnsupportedAudioFileException, IOException {

		Clip clip = (Clip) mixer.getLine(new Line.Info(Clip.class));
		File soundFile = new File("testsounds/startup.wav");
		AudioInputStream audioInputStream = AudioSystem
				.getAudioInputStream(soundFile);

		Assert.assertFalse(clip.isActive());
		Assert.assertFalse(clip.isOpen());
		clip.open(audioInputStream);
		Assert.assertTrue(clip.isOpen());
		Assert.assertFalse(clip.isActive());
		clip.start();
		Assert.assertTrue(clip.isOpen());
		Assert.assertTrue(clip.isActive());
		clip.stop();
		Assert.assertTrue(clip.isOpen());
		Assert.assertFalse(clip.isActive());
		clip.close();
		Assert.assertFalse(clip.isOpen());
		Assert.assertFalse(clip.isActive());

	}

	@Test
	public void testOpenEvent() throws LineUnavailableException,
			UnsupportedAudioFileException, IOException {
		System.out
				.println("This tests the OPEN event. You should see an OPEN on the next line");
		Clip clip = (Clip) mixer.getLine(new Line.Info(Clip.class));
		File soundFile = new File("testsounds/startup.wav");
		AudioInputStream audioInputStream = AudioSystem
				.getAudioInputStream(soundFile);

		opened = 0;

		LineListener openListener = new LineListener() {

			@Override
			public void update(LineEvent event) {
				if (event.getType() == LineEvent.Type.OPEN) {
					System.out.println("OPEN");
					opened++;
				}
			}

		};

		clip.addLineListener(openListener);
		clip.open(audioInputStream);
		clip.close();
		clip.removeLineListener(openListener);

		Assert.assertEquals(1, opened);

	}

	@Test
	public void testCloseEvent() throws LineUnavailableException,
			UnsupportedAudioFileException, IOException {

		System.out.println("This tests the CLOSE event");

		Clip clip = (Clip) mixer.getLine(new Line.Info(Clip.class));
		File soundFile = new File("testsounds/startup.wav");
		AudioInputStream audioInputStream = AudioSystem
				.getAudioInputStream(soundFile);

		closed = 0;

		LineListener closeListener = new LineListener() {

			@Override
			public void update(LineEvent event) {
				if (event.getType() == LineEvent.Type.CLOSE) {
					System.out.println("CLOSE");
					closed++;
				}
			}

		};

		clip.addLineListener(closeListener);
		clip.open(audioInputStream);
		clip.close();

		clip.removeLineListener(closeListener);

		Assert.assertEquals(1, closed);

	}

	@Test
	public void testStartedStopped() throws LineUnavailableException,
			UnsupportedAudioFileException, IOException, InterruptedException {

		File soundFile = new File("testsounds/startup.wav");
		AudioInputStream audioInputStream = AudioSystem
				.getAudioInputStream(soundFile);
		AudioFormat audioFormat = audioInputStream.getFormat();

		Clip clip;
		clip = (Clip) mixer.getLine(new DataLine.Info(Clip.class, audioFormat));
		Assert.assertNotNull(clip);

		clip.open(audioInputStream);

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

		clip.addLineListener(startStopListener);

		clip.start();
		clip.drain();
		clip.stop();
		clip.close();

		Assert.assertEquals(1, started);
		Assert.assertEquals(1, stopped);

	}

	@Test(timeout = 1000)
	public void testDrainWithoutStart() throws UnsupportedAudioFileException,
			IOException, LineUnavailableException {

		File soundFile = new File("testsounds/startup.wav");
		AudioInputStream audioInputStream = AudioSystem
				.getAudioInputStream(soundFile);
		AudioFormat audioFormat = audioInputStream.getFormat();

		Clip clip;
		clip = (Clip) mixer.getLine(new DataLine.Info(Clip.class, audioFormat));
		Assert.assertNotNull(clip);

		clip.open(audioInputStream);
		clip.drain();
		clip.close();

	}

	@Test
	public void testDrainBlocksWhilePlaying()
			throws UnsupportedAudioFileException, IOException,
			LineUnavailableException {

		String fileName = "testsounds/startup.wav";
		File soundFile = new File(fileName);
		AudioInputStream audioInputStream = AudioSystem
				.getAudioInputStream(soundFile);
		AudioFormat audioFormat = audioInputStream.getFormat();

		Clip clip;
		clip = (Clip) mixer.getLine(new DataLine.Info(Clip.class, audioFormat));
		Assert.assertNotNull(clip);

		long startTime = System.currentTimeMillis();

		clip.open(audioInputStream);
		clip.start();
		clip.drain();
		clip.stop();
		clip.close();

		long endTime = System.currentTimeMillis();

		Assert.assertTrue(endTime - startTime > 3000);
		System.out.println("Playback of " + fileName + " completed in "
				+ (endTime - startTime) + " milliseconds");
	}

	@Test
	public void testLoop0InterruptsPlayback() throws LineUnavailableException,
			IOException, UnsupportedAudioFileException {
		Clip clip = (Clip) mixer.getLine(new Line.Info(Clip.class));
		File soundFile = new File("testsounds/startup.wav");
		AudioInputStream audioInputStream = AudioSystem
				.getAudioInputStream(soundFile);
		clip.open(audioInputStream);

		clip.setLoopPoints((int) (clip.getFrameLength() / 4), (int) (clip
				.getFrameLength() / 2));
		clip.loop(4);
		try {
			Thread.sleep(2000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		clip.loop(0);
		clip.close();
	}

	@Test(expected = IllegalArgumentException.class)
	public void testOpenWrongUse() throws LineUnavailableException {
		Clip clip = (Clip) mixer.getLine(new Line.Info(Clip.class));
		clip.open();

	}

	/*
	 * 
	 * modified version of the sample code at
	 * http://icedtea.classpath.org/bugzilla/show_bug.cgi?id=173
	 * 
	 */

	@Test
	public void testPlayTwoClips() throws LineUnavailableException,
			UnsupportedAudioFileException, IOException {
		Clip clip1 = (Clip) mixer.getLine(new Line.Info(Clip.class));
		File soundFile1 = new File("testsounds/startup.wav");
		AudioInputStream audioInputStream1 = AudioSystem
				.getAudioInputStream(soundFile1);
		clip1.open(audioInputStream1);

		Clip clip2 = (Clip) mixer.getLine(new Line.Info(Clip.class));
		File soundFile2 = new File("testsounds/logout.wav");
		AudioInputStream audioInputStream2 = AudioSystem
				.getAudioInputStream(soundFile2);
		clip2.open(audioInputStream2);

		clip1.start();
		clip2.start();

		clip1.drain();
		clip2.drain();

		clip1.close();
		clip2.close();

		Assert.assertFalse(clip1.isOpen());
		Assert.assertFalse(clip2.isOpen());

	}

	@Test
	public void testSupportedControls() throws LineUnavailableException,
			UnsupportedAudioFileException, IOException {
		Clip clip = (Clip) mixer.getLine(new Line.Info(Clip.class));
		File soundFile1 = new File("testsounds/startup.wav");
		AudioInputStream audioInputStream1 = AudioSystem
				.getAudioInputStream(soundFile1);
		clip.open(audioInputStream1);

		Control[] controls = clip.getControls();
		Assert.assertNotNull(controls);
		Assert.assertTrue(controls.length >= 1);
		for (Control control : controls) {
			Assert.assertNotNull(control);
		}

		clip.close();
	}

	@Test
	public void testFramePositionBeforeClose() throws LineUnavailableException,
			UnsupportedAudioFileException, IOException {
		System.out
				.println("This tests if the Clip provides the correct frame position");
		Clip clip = (Clip) mixer.getLine(new Line.Info(Clip.class));
		String fileName = "testsounds/logout.wav";
		File soundFile1 = new File(fileName);
		AudioInputStream audioInputStream1 = AudioSystem
				.getAudioInputStream(soundFile1);
		clip.open(audioInputStream1);

		clip.start();

		clip.drain();

		long pos = clip.getFramePosition();

		clip.close();

		long expected = 136703;
		long granularity = 5;
		System.out.println("Frames in " + fileName + ": " + expected);
		System.out.println("Frame position in clip :" + pos);
		Assert.assertTrue("Expected: " + expected + " got " + pos, Math
				.abs(expected - pos) < granularity);

	}

	@Test
	public void testFramePositionWithStartStop()
			throws LineUnavailableException, UnsupportedAudioFileException,
			IOException, InterruptedException {

		System.out
				.println("This tests if the Clip provides the correct frame position");
		Clip clip = (Clip) mixer.getLine(new Line.Info(Clip.class));
		String fileName = "testsounds/logout.wav";
		File soundFile1 = new File(fileName);
		AudioInputStream audioInputStream1 = AudioSystem
				.getAudioInputStream(soundFile1);
		clip.open(audioInputStream1);

		clip.start();

		Thread.sleep(500);
		clip.stop();
		Thread.sleep(5000);
		clip.start();

		clip.drain();

		long pos = clip.getFramePosition();

		clip.close();

		long expected = 136703;
		long granularity = 5;
		System.out.println("Frames in " + fileName + ": " + expected);
		System.out.println("Frame position in clip :" + pos);
		Assert.assertTrue("Expected: " + expected + " got " + pos, Math
				.abs(expected - pos) < granularity);

	}

	@Test
	public void testFramePositionWithLoop() throws LineUnavailableException,
			UnsupportedAudioFileException, IOException, InterruptedException {
		System.out.println("This tests if the Clip provides the correct frame "
				+ "position with a bit of looping in the clip");
		Clip clip = (Clip) mixer.getLine(new Line.Info(Clip.class));
		String fileName = "testsounds/logout.wav";
		File soundFile1 = new File(fileName);
		AudioInputStream audioInputStream1 = AudioSystem
				.getAudioInputStream(soundFile1);
		clip.open(audioInputStream1);

		clip.setLoopPoints(0, -1);
		clip.loop(1);
		Thread.sleep(500);
		clip.stop();
		Thread.sleep(2000);
		clip.start();
		Thread.sleep(100);
		clip.stop();
		Thread.sleep(4000);
		clip.start();
		Thread.sleep(100);
		clip.stop();
		Thread.sleep(2000);
		clip.start();
		Thread.sleep(100);
		clip.stop();
		Thread.sleep(3000);
		clip.start();

		clip.drain();

		long pos = clip.getFramePosition();

		clip.close();

		long expected = 136703 * 1;
		long granularity = 5;
		System.out.println("Frames in " + fileName + ": " + expected);
		System.out.println("Frame position in clip :" + pos);
		Assert.assertTrue("Expected: " + expected + " got " + pos, Math
				.abs(expected - pos) < granularity);

	}

	@Test
	public void testFramePositionAfterLooping()
			throws LineUnavailableException, UnsupportedAudioFileException,
			IOException {
		System.out
				.println("This tests if the Clip provides the correct frame position");
		Clip clip = (Clip) mixer.getLine(new Line.Info(Clip.class));
		String fileName = "testsounds/logout.wav";
		File soundFile1 = new File(fileName);
		AudioInputStream audioInputStream1 = AudioSystem
				.getAudioInputStream(soundFile1);
		clip.open(audioInputStream1);

		clip.setLoopPoints(0, -1);
		clip.loop(1);

		clip.drain();

		long pos = clip.getFramePosition();

		clip.close();

		long expected = 136703 * 2;
		long granularity = 5;
		System.out.println("Frames in " + fileName + ": " + expected);
		System.out.println("Frame position in clip :" + pos);
		Assert.assertTrue("Expected: " + expected + " got " + pos, Math
				.abs(expected - pos) < granularity);

	}

	@Test
	public void testMixerKnowsAboutOpenClips() throws LineUnavailableException,
			UnsupportedAudioFileException, IOException {
		Clip clip = (Clip) mixer.getLine(new Line.Info(Clip.class));
		File soundFile1 = new File("testsounds/startup.wav");
		AudioInputStream audioInputStream1 = AudioSystem
				.getAudioInputStream(soundFile1);

		int initiallyOpenClips = mixer.getSourceLines().length;
		Assert.assertEquals(initiallyOpenClips, mixer.getSourceLines().length);
		clip.open(audioInputStream1);
		Assert.assertEquals(initiallyOpenClips + 1, mixer.getSourceLines().length);
		Assert.assertEquals(clip, mixer.getSourceLines()[initiallyOpenClips]);
		clip.close();
		Assert.assertEquals(initiallyOpenClips, mixer.getSourceLines().length);

	}

	@After
	public void tearDown() {
		mixer.close();

	}

}
