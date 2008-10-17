/* PulseAudioSourceDataLineRawTest.java
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
import javax.sound.sampled.BooleanControl;
import javax.sound.sampled.DataLine;
import javax.sound.sampled.FloatControl;
import javax.sound.sampled.Line;
import javax.sound.sampled.LineEvent;
import javax.sound.sampled.LineListener;
import javax.sound.sampled.LineUnavailableException;
import javax.sound.sampled.Mixer;
import javax.sound.sampled.SourceDataLine;
import javax.sound.sampled.UnsupportedAudioFileException;

import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

public class PulseAudioSourceDataLineRawTest {

	PulseAudioMixer mixer = null;

	int started = 0;
	int stopped = 0;

	AudioFormat aSupportedFormat = new AudioFormat(
			AudioFormat.Encoding.PCM_UNSIGNED, 44100f, 8, 1, 1, 44100f, true);

	class ThreadWriter extends Thread {
		PulseAudioSourceDataLine line;
		AudioInputStream stream;

		public ThreadWriter(AudioInputStream stream,
				PulseAudioSourceDataLine line) throws LineUnavailableException {

			this.line = line;
			this.stream = stream;

			if (line.isOpen()) {
				line.close();
			}

		}

		@Override
		public void run() {
			try {
				AudioFormat audioFormat = stream.getFormat();

				line.open(audioFormat);

				byte[] abData = new byte[1000];
				int bytesRead = 0;

				line.start();

				while (bytesRead >= 0) {
					bytesRead = stream.read(abData, 0, abData.length);
					// System.out.println("read data");
					if (bytesRead > 0) {
						// System.out.println("about to write data");
						line.write(abData, 0, bytesRead);
						// System.out.println("wrote data");
					}
				}

				line.drain();
				line.close();

			} catch (LineUnavailableException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	@Before
	public void setUp() throws LineUnavailableException {

		mixer = PulseAudioMixer.getInstance();
		if (mixer.isOpen()) {
			mixer.close();
		}

		mixer.open();

		started = 0;
		stopped = 0;

	}

	@Test
	public void testStartAndStopEventsOnCork()
			throws UnsupportedAudioFileException, IOException,
			LineUnavailableException, InterruptedException {

		System.out
				.println("This test checks if START and STOP notifications appear on corking");

		File soundFile = new File("testsounds/startup.wav");
		AudioInputStream audioInputStream = AudioSystem
				.getAudioInputStream(soundFile);
		AudioFormat audioFormat = audioInputStream.getFormat();

		PulseAudioSourceDataLine line;
		line = (PulseAudioSourceDataLine) mixer.getLine(new DataLine.Info(
				SourceDataLine.class, audioFormat));
		Assert.assertNotNull(line);

		LineListener startStopListener = new LineListener() {

			@Override
			public void update(LineEvent event) {
				if (event.getType() == LineEvent.Type.START) {
					System.out.println("START");
					started++;
				}

				if (event.getType() == LineEvent.Type.STOP) {
					System.out.println("STOP");
					stopped++;
				}
			}

		};

		line.addLineListener(startStopListener);
		System.out.println("Launching threadWriter");
		ThreadWriter writer = new ThreadWriter(audioInputStream, line);
		writer.start();
		// System.out.println("started");

		Thread.sleep(1000);

		// CORK
		line.stop();

		Thread.sleep(1000);

		// UNCORK
		line.start();

		Thread.sleep(1000);

		// System.out.println("waiting for thread to finish");
		writer.join();

		Assert.assertEquals(2, started);
		Assert.assertEquals(2, stopped);

	}

	@Test
	public void testVolumeAndMute() throws Exception {

		Mixer selectedMixer = mixer;
		SourceDataLine line = (SourceDataLine) selectedMixer
				.getLine(new Line.Info(SourceDataLine.class));

		File soundFile = new File(new java.io.File(".").getCanonicalPath()
				+ "/testsounds/logout.wav");
		AudioInputStream audioInputStream = AudioSystem
				.getAudioInputStream(soundFile);
		AudioFormat audioFormat = audioInputStream.getFormat();

		line.open(audioFormat);
		line.start();
		PulseAudioVolumeControl volume = (PulseAudioVolumeControl) line
				.getControl(FloatControl.Type.VOLUME);
		PulseAudioMuteControl mute = (PulseAudioMuteControl) line
				.getControl(BooleanControl.Type.MUTE);

		mute.setValue(true);
		volume.setValue(PulseAudioVolumeControl.MAX_VOLUME);

		mute.setValue(false);

		byte[] abData = new byte[1000];
		int bytesRead = 0;

		while (bytesRead >= 0) {
			bytesRead = audioInputStream.read(abData, 0, abData.length);
			if (bytesRead > 0) {
				line.write(abData, 0, bytesRead);
			}
		}

		line.drain();
		line.close();
		selectedMixer.close();

	}

	@Test
	public void testSettingStreamName() throws LineUnavailableException,
			UnsupportedAudioFileException, IOException {
		File soundFile = new File("testsounds/logout.wav");
		AudioInputStream audioInputStream = AudioSystem
				.getAudioInputStream(soundFile);
		AudioFormat audioFormat = audioInputStream.getFormat();

		PulseAudioSourceDataLine line;
		line = (PulseAudioSourceDataLine) mixer.getLine(new DataLine.Info(
				SourceDataLine.class, audioFormat));

		String name = "Knights Who Say ... Oh my god, i am so sorry, i didnt mean it...";
		line.setName(name);

		line.open(audioFormat);
		line.start();

		byte[] abData = new byte[1000];
		int bytesRead = 0;

		while (bytesRead >= 0) {
			bytesRead = audioInputStream.read(abData, 0, abData.length);
			if (bytesRead > 0) {
				line.write(abData, 0, bytesRead);
			}
		}

		Assert.assertTrue(line.getName() == name);
		/*
		 * FIXME test that PulseAudio also knows this correctly using
		 * introspection
		 */

		line.drain();
		line.stop();
		line.close();

	}

	@Test
	public void messWithStreams() throws LineUnavailableException {
		System.out
				.println("This test tries to unCork a stream which hasnt been corked");

		PulseAudioSourceDataLine line = (PulseAudioSourceDataLine) mixer
				.getLine(new DataLine.Info(SourceDataLine.class,
						aSupportedFormat, 1000));

		line.open();
		line.start();
		Stream s = line.getStream();
		Operation o;
		synchronized (EventLoop.getEventLoop().threadLock) {
			o = s.unCork();
		}
		o.waitForCompletion();
		o.releaseReference();
		line.stop();
		line.close();
	}

	@After
	public void tearDown() {

	}

}
