/* OtherSoundProvidersAvailableTest.java
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

import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.Line;
import javax.sound.sampled.LineUnavailableException;
import javax.sound.sampled.Mixer;
import javax.sound.sampled.SourceDataLine;

import org.junit.Test;

public class OtherSoundProvidersAvailableTest {

	@Test
	public void testOtherSoundProviders() {
		System.out.println("This tests if alsa mixers are still available");

		Mixer.Info mixerInfos[] = AudioSystem.getMixerInfo();
		Mixer.Info selectedMixerInfo = null;
		Mixer mixer;

		boolean selected = false;
		int i = 0;
		System.out.println("Available Mixers:");
		// use 0,0 or the default
		for (Mixer.Info info : mixerInfos) {
			System.out.println("Mixer Line " + i++ + ": " + info.getName()
					+ " " + info.getDescription());
			if (info.getName().contains("0,0") && !selected) {
				System.out.println("^ selecting as the mixer to use");
				selectedMixerInfo = info;
				selected = true;
			}
		}

		if (selectedMixerInfo != null) {
			System.out.println(selectedMixerInfo.toString());
		}

		System.out.print("Selected mixer is of class: ");

		mixer = AudioSystem.getMixer(selectedMixerInfo);
		System.out.println(mixer.getClass().toString());
		try {
			Line.Info sourceDataLineInfo = null;

			mixer.open(); // initialize the mixer

			Line.Info allLineInfo[] = mixer.getSourceLineInfo();
			System.out.println("Source lines supported by mixer: ");
			int j = 0;
			for (Line.Info lineInfo : allLineInfo) {
				System.out.println("Source Line " + j++ + ": "
						+ lineInfo.getLineClass());
				if (lineInfo.toString().contains("SourceDataLine")) {
					sourceDataLineInfo = lineInfo;
				}

			}

			if (sourceDataLineInfo == null) {
				System.out.println("Mixer supports no SourceDataLines");
			} else {
				SourceDataLine sourceDataLine = (SourceDataLine) mixer
						.getLine(sourceDataLineInfo);

				sourceDataLine.open();
				// sourceDataLine.write('a', 0, 2);
				sourceDataLine.close();
			}
		} catch (LineUnavailableException e) {
			System.out.println("Line unavailable");
		} finally {
			mixer.close();
		}

	}
}
