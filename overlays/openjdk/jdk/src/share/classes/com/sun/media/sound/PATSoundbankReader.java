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

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;
import java.util.StringTokenizer;

import javax.sound.midi.InvalidMidiDataException;
import javax.sound.midi.Patch;
import javax.sound.midi.Soundbank;
import javax.sound.midi.spi.SoundbankReader;

/**
 * GUS-compatible GF1 Patch reader.
 * 
 * @version %I%, %E%
 * @author Karl Helgason
 */

public class PATSoundbankReader extends SoundbankReader {

	public Soundbank getSoundbank(URL url) throws InvalidMidiDataException,
			IOException {
		try {
			PATInstrument ins = new PATInstrument(url);
			SimpleSoundbank snk = new SimpleSoundbank();
			snk.addInstrument(ins);
			return snk;
		} catch (InvalidFormatException e) {
			return null;
		} catch (IOException ioe) {
			return null;
		}
	}

	public Soundbank getSoundbank(InputStream stream)
			throws InvalidMidiDataException, IOException {
		try {
			stream.mark(512);
			PATInstrument ins = new PATInstrument(stream);
			SimpleSoundbank snk = new SimpleSoundbank();
			snk.addInstrument(ins);
			return snk;
		} catch (InvalidFormatException e) {
			stream.reset();
			return null;
		}
	}

	
	public Soundbank getSoundbank(File file) throws InvalidMidiDataException, IOException {
		if (file.getPath().toLowerCase().endsWith(".cfg"))
			return readConfigFile(file);
		if (!file.getPath().toLowerCase().endsWith(".pat"))
			return null;
		try {
			PATInstrument ins = new PATInstrument(file);
			SimpleSoundbank snk = new SimpleSoundbank();
			snk.addInstrument(ins);
			return snk;
		} catch (InvalidFormatException e) {
			return null;
		}
	}	
	
	private ModelPerformer[] processDrumKit(PATInstrument ins)
	{
		ModelPerformer[] performers = ins.getPerformers();
		
		for (ModelPerformer performer : performers) {
			double pc = -((ModelWavetable) performer
					.getOscillators().get(0))
					.getPitchcorrection() / 100.0;
			performer
					.getConnectionBlocks()
					.add(
							new ModelConnectionBlock(
									pc / 128.0,
									new ModelDestination(
											ModelDestination.DESTINATION_KEYNUMBER)));
			
			if(((ModelWavetable) performer.getOscillators().get(0)).getLoopType() == ModelWavetable.LOOP_TYPE_OFF)
			{
				performer
				.getConnectionBlocks()
				.add(
						new ModelConnectionBlock(
								12000,
								new ModelDestination(
										ModelDestination.DESTINATION_EG1_RELEASE)));
											
			}
		}
		return performers;
	}

	private Soundbank readConfigFile(File file)
			throws InvalidMidiDataException, IOException {
		File root = file.getParentFile();
		SimpleSoundbank soundbank = new SimpleSoundbank();
		FileInputStream in = new FileInputStream(file);
		try {
			try {

				int bank = -2;
				int drumset = -1;
				SimpleInstrument drumsetins = null;
				
				HashMap<Integer, HashMap<Integer, ModelPerformer[]>> multipatchmaps
					= new HashMap<Integer, HashMap<Integer, ModelPerformer[]>>();

				Reader r = new InputStreamReader(in, "ASCII");
				BufferedReader br = new BufferedReader(r);
				String line = null;
				while ((line = br.readLine()) != null) {
					int li = line.indexOf('#');
					if (li != -1)
						line = line.substring(0, li);
					line = line.trim();
					if (line.length() == 0)
						continue;
					StringTokenizer st = new StringTokenizer(line, " ");
					if (!st.hasMoreTokens())
						continue;
					String token = st.nextToken();
					if (token.equalsIgnoreCase("drumset")) {
						if (!st.hasMoreTokens())
							continue;
						token = st.nextToken();
						drumset = Integer.parseInt(token);
						bank = -1;
						drumsetins = new SimpleInstrument();
						drumsetins.setPatch(new ModelPatch(0, drumset, true));
						soundbank.addInstrument(drumsetins);
					} else if (token.equalsIgnoreCase("bank")) {
						if (!st.hasMoreTokens())
							continue;
						token = st.nextToken();
						drumset = -1;
						bank = Integer.parseInt(token);
					} else
					if(bank == -2)
					{				
						int prg_from = 0;
						int prg_to = 0;
						int li2 = token.indexOf('-');
						if(li2 == -1)
						{
							prg_from = Integer.parseInt(token)-1;
							prg_to = Integer.parseInt(token)-1;
						}
						else
						{
							prg_from = Integer.parseInt(token.substring(0, li2))-1;
							prg_to = Integer.parseInt(token.substring(li2+1))-1;							
						}
						if(prg_to < 0) continue;
						if(prg_to < prg_from) continue;
						if(prg_from < 0) prg_from = 0; 
						
						if(!st.hasMoreTokens()) continue;						
						token = st.nextToken();
						if(token.equals("begin_multipatch") || token.equals("override_patch"))
						{

							HashMap<Integer, ModelPerformer[]> multipatchmap = new HashMap<Integer, ModelPerformer[]>();
							if(token.equals("override_patch"))
							{
								HashMap<Integer, ModelPerformer[]> omap = multipatchmaps.get(prg_from);
								if(omap != null)
									multipatchmap.putAll(omap);
							}
							for (int prg = prg_from; prg <= prg_to; prg++) 
								multipatchmaps.put(prg, multipatchmap);

							
							while ((line = br.readLine()) != null) {
								if(line.equals("end_multipatch")) break;
								
								StringTokenizer st2 = new StringTokenizer(line, " ");
								if(!st2.hasMoreTokens()) continue;
								int key = Integer.parseInt(st2.nextToken().trim());
								if(!st2.hasMoreTokens()) continue;
								token = st2.nextToken();
								File patfile = new File(root, token);
								if (!patfile.exists()) 
									patfile = new File(root, token + ".pat");
								
								multipatchmap.put(key, processDrumKit(new PATInstrument(patfile)));
							}
						}
						else
						{							
							File patfile = new File(root, token);
							if (!patfile.exists()) 
								patfile = new File(root, token + ".pat");							
							
							for (int prg = prg_from; prg <= prg_to; prg++) {
								PATInstrument ins = new PATInstrument(patfile);
								if(prg != 0)
								{
									if(prg >= 128)
										ins.setPatch(new ModelPatch(0, prg-128, true));
									else
										ins.setPatch(new Patch(0, prg));
								}
								soundbank.addInstrument(ins);
							}
														
						}						
					}
					else
					{
						int id = Integer.parseInt(token);
						if (!st.hasMoreTokens())
							continue;
						token = st.nextToken();
						File patfile = new File(root, token);
						if (!patfile.exists()) {
							patfile = new File(root, token + ".pat");
						}
						PATInstrument ins = new PATInstrument(patfile);

						if (bank != -1) {
							ins.setPatch(new Patch(bank, id));
							soundbank.addInstrument(ins);
						} else if (drumset != -1) {							
							drumsetins.add(processDrumKit(ins), id, id);
						}
					}

				}
				
				for(Map.Entry<Integer, HashMap<Integer, ModelPerformer[]>> entry :
					multipatchmaps.entrySet())
				{
					
					int prg = entry.getKey();										
					drumsetins = new SimpleInstrument();
					if(prg >= 128)
						drumsetins.setPatch(new ModelPatch(0, prg-128, true));
					else
						drumsetins.setPatch(new Patch(0, prg));
					
					drumsetins.add(entry.getValue().get(36));					
					for(Map.Entry<Integer, ModelPerformer[]> entry2 : entry.getValue().entrySet())
						drumsetins.add(entry2.getValue(), entry2.getKey(), entry2.getKey());
					
					soundbank.addInstrument(drumsetins);
					
				}
				
			} finally {
				in.close();
			}
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
		return soundbank;
	}


}
