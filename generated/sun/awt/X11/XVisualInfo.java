// This file is an automatically generated file, please do not edit this file, modify the WrapperGenerator.java file instead !

package sun.awt.X11;

import sun.misc.*;

import java.util.logging.*;
public class XVisualInfo extends XWrapperBase { 
	private Unsafe unsafe = XlibWrapper.unsafe; 
	private final boolean should_free_memory;
	public static int getSize() { return 40; }
	public int getDataSize() { return getSize(); }

	long pData;

	public long getPData() { return pData; }


	XVisualInfo(long addr) {
		log.finest("Creating");
		pData=addr;
		should_free_memory = false;
	}


	XVisualInfo() {
		log.finest("Creating");
		pData = unsafe.allocateMemory(getSize());
		should_free_memory = true;
	}


	public void dispose() {
		log.finest("Disposing");
		if (should_free_memory) {
			log.finest("freeing memory");
			unsafe.freeMemory(pData); 
	}
		}
	public long get_visual(int index) { log.finest(""); return Native.getLong(pData+0)+index*Native.getLongSize(); }
	public long get_visual() { log.finest("");return Native.getLong(pData+0); }
	public void set_visual(long v) { log.finest(""); Native.putLong(pData + 0, v); }
	public long get_visualid() { log.finest("");return (Native.getLong(pData+4)); }
	public void set_visualid(long v) { log.finest(""); Native.putLong(pData+4, v); }
	public int get_screen() { log.finest("");return (Native.getInt(pData+8)); }
	public void set_screen(int v) { log.finest(""); Native.putInt(pData+8, v); }
	public int get_depth() { log.finest("");return (Native.getInt(pData+12)); }
	public void set_depth(int v) { log.finest(""); Native.putInt(pData+12, v); }
	public int get_class() { log.finest("");return (Native.getInt(pData+16)); }
	public void set_class(int v) { log.finest(""); Native.putInt(pData+16, v); }
	public long get_red_mask() { log.finest("");return (Native.getLong(pData+20)); }
	public void set_red_mask(long v) { log.finest(""); Native.putLong(pData+20, v); }
	public long get_green_mask() { log.finest("");return (Native.getLong(pData+24)); }
	public void set_green_mask(long v) { log.finest(""); Native.putLong(pData+24, v); }
	public long get_blue_mask() { log.finest("");return (Native.getLong(pData+28)); }
	public void set_blue_mask(long v) { log.finest(""); Native.putLong(pData+28, v); }
	public int get_colormap_size() { log.finest("");return (Native.getInt(pData+32)); }
	public void set_colormap_size(int v) { log.finest(""); Native.putInt(pData+32, v); }
	public int get_bits_per_rgb() { log.finest("");return (Native.getInt(pData+36)); }
	public void set_bits_per_rgb(int v) { log.finest(""); Native.putInt(pData+36, v); }


	String getName() {
		return "XVisualInfo"; 
	}


	String getFieldsAsString() {
		String ret="";

		ret += ""+"visual = " + get_visual() +", ";
		ret += ""+"visualid = " + get_visualid() +", ";
		ret += ""+"screen = " + get_screen() +", ";
		ret += ""+"depth = " + get_depth() +", ";
		ret += ""+"class = " + get_class() +", ";
		ret += ""+"red_mask = " + get_red_mask() +", ";
		ret += ""+"green_mask = " + get_green_mask() +", ";
		ret += ""+"blue_mask = " + get_blue_mask() +", ";
		ret += ""+"colormap_size = " + get_colormap_size() +", ";
		ret += ""+"bits_per_rgb = " + get_bits_per_rgb() +", ";
		return ret;
	}


}


