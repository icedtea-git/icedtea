/* PulseAudioMixerRawTest.java
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

import java.net.UnknownHostException;

import javax.sound.sampled.LineUnavailableException;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

public class PulseAudioMixerRawTest {

	PulseAudioMixer mixer = null;

	@Before
	public void setUp() {
		mixer = PulseAudioMixer.getInstance();
		if (mixer.isOpen()) {
			mixer.close();
		}
	}

	@Test
	public void testLocalOpen() throws LineUnavailableException {
		System.out.println("This test tries to open to the local system");
		mixer.openLocal();
	}

	@Test
	public void testLocalOpenAppName() throws LineUnavailableException {
		System.out
				.println("This test tries to connect to the local system while using an application name");
		mixer.openLocal("JunitTest");

	}

	@Test(expected = LineUnavailableException.class)
	public void testRemoteOpenWithInvalidPort() throws UnknownHostException,
			LineUnavailableException {
		System.out
				.println("this test tries to connect to an invalid remote system");
		mixer.openRemote("JUnitTest", "128.0.0.1", 10);

	}

	/*
	 * This test assumes a computer named 'town' is in the network with
	 * pulseaudio listening on port 4173
	 */
	@Test
	public void testRemoteOpenWithValidPort() throws UnknownHostException,
			LineUnavailableException {
		System.out.println("This test tries to connect a valid remote system");
		mixer.openRemote("JUnitTest", "town", 4713);
		mixer.close();
	}

	/*
	 * This test assumes a computer named 'town' is in the network with
	 * pulseaudio listening
	 */
	@Test
	public void testRemoteOpen() throws UnknownHostException,
			LineUnavailableException {
		mixer.openRemote("JUnitTest", "town");
		mixer.close();
	}

	@Test(expected = LineUnavailableException.class)
	public void testInvalidRemoteOpen() throws UnknownHostException,
			LineUnavailableException {
		mixer.openRemote("JUnitTest", "127.0.0.1");
		mixer.close();
	}

	@After
	public void tearDown() {
		if (mixer.isOpen()) {
			mixer.close();
		}
	}

}
