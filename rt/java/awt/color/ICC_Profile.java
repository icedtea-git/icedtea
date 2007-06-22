/* ICC_Profile.java -- color space profiling
   Copyright (C) 2000, 2002, 2004 Free Software Foundation

This file is part of GNU Classpath.

GNU Classpath is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Classpath is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Classpath; see the file COPYING.  If not, write to the
Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
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
exception statement from your version. */


package java.awt.color;

import gnu.java.awt.color.ProfileHeader;
import gnu.java.awt.color.TagEntry;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.ObjectStreamException;
import java.io.OutputStream;
import java.io.Serializable;
import java.io.UnsupportedEncodingException;
import java.nio.ByteBuffer;
import java.util.Enumeration;
import java.util.Hashtable;

import sun.java2d.cmm.PCMM;
import sun.java2d.cmm.CMSManager;

/**
 * ICC Profile - represents an ICC Color profile.
 * The ICC profile format is a standard file format which maps the transform
 * from a device color space to a standard Profile Color Space (PCS), which
 * can either be CIE L*a*b or CIE XYZ.
 * (With the exception of device link profiles which map from one device space
 * to another)
 *
 * ICC profiles calibrated to specific input/output devices are used when color
 * fidelity is of importance.
 *
 * An instance of ICC_Profile can be created using the getInstance() methods,
 * either using one of the predefined color spaces enumerated in ColorSpace,
 * or from an ICC profile file, or from an input stream.
 *
 * An ICC_ColorSpace object can then be created to transform color values
 * through the profile.
 *
 * The ICC_Profile class implements the version 2 format specified by
 * International Color Consortium Specification ICC.1:1998-09,
 * and its addendum ICC.1A:1999-04, April 1999
 * (available at www.color.org)
 *
 * @author Sven de Marothy
 * @author Rolf W. Rasmussen (rolfwr@ii.uib.no)
 * @since 1.2
 */
public class ICC_Profile implements Serializable
{
  /**
   * Compatible with JDK 1.2+.
   */
  private static final long serialVersionUID = -3938515861990936766L;

  /**
   * ICC Profile classes
   */
  public static final int CLASS_INPUT = 0;
  public static final int CLASS_DISPLAY = 1;
  public static final int CLASS_OUTPUT = 2;
  public static final int CLASS_DEVICELINK = 3;
  public static final int CLASS_COLORSPACECONVERSION = 4;
  public static final int CLASS_ABSTRACT = 5;
  public static final int CLASS_NAMEDCOLOR = 6;

  /**
   * ICC Profile class signatures
   */
  public static final int icSigInputClass = 0x73636e72; // 'scnr'
  public static final int icSigDisplayClass = 0x6d6e7472; // 'mntr'
  public static final int icSigOutputClass = 0x70727472; // 'prtr'
  public static final int icSigLinkClass = 0x6c696e6b; // 'link'
  public static final int icSigColorSpaceClass = 0x73706163; // 'spac'
  public static final int icSigAbstractClass = 0x61627374; // 'abst'
  public static final int icSigNamedColorClass = 0x6e6d636c; // 'nmcl'

  /**
   * Color space signatures
   */
  public static final int icSigXYZData = 0x58595A20; // 'XYZ ' 
  public static final int icSigLabData = 0x4C616220; // 'Lab '
  public static final int icSigLuvData = 0x4C757620; // 'Luv '
  public static final int icSigYCbCrData = 0x59436272; // 'YCbr'
  public static final int icSigYxyData = 0x59787920; // 'Yxy '
  public static final int icSigRgbData = 0x52474220; // 'RGB '
  public static final int icSigGrayData = 0x47524159; // 'GRAY'
  public static final int icSigHsvData = 0x48535620; // 'HSV '
  public static final int icSigHlsData = 0x484C5320; // 'HLS '
  public static final int icSigCmykData = 0x434D594B; // 'CMYK'
  public static final int icSigCmyData = 0x434D5920; // 'CMY '
  public static final int icSigSpace2CLR = 0x32434C52; // '2CLR'
  public static final int icSigSpace3CLR = 0x33434C52; // '3CLR'
  public static final int icSigSpace4CLR = 0x34434C52; // '4CLR'
  public static final int icSigSpace5CLR = 0x35434C52; // '5CLR'
  public static final int icSigSpace6CLR = 0x36434C52; // '6CLR'
  public static final int icSigSpace7CLR = 0x37434C52; // '7CLR'
  public static final int icSigSpace8CLR = 0x38434C52; // '8CLR'
  public static final int icSigSpace9CLR = 0x39434C52; // '9CLR'
  public static final int icSigSpaceACLR = 0x41434C52; // 'ACLR'
  public static final int icSigSpaceBCLR = 0x42434C52; // 'BCLR'
  public static final int icSigSpaceCCLR = 0x43434C52; // 'CCLR'
  public static final int icSigSpaceDCLR = 0x44434C52; // 'DCLR'
  public static final int icSigSpaceECLR = 0x45434C52; // 'ECLR'
  public static final int icSigSpaceFCLR = 0x46434C52; // 'FCLR'

  /**
   * Rendering intents
   */
  public static final int icPerceptual = 0;
  public static final int icRelativeColorimetric = 1;
  public static final int icSaturation = 2;
  public static final int icAbsoluteColorimetric = 3;

  /**
   * Tag signatures
   */
  public static final int icSigAToB0Tag = 0x41324230; // 'A2B0' 
  public static final int icSigAToB1Tag = 0x41324231; // 'A2B1' 
  public static final int icSigAToB2Tag = 0x41324232; // 'A2B2' 
  public static final int icSigBlueColorantTag = 0x6258595A; // 'bXYZ' 
  public static final int icSigBlueTRCTag = 0x62545243; // 'bTRC' 
  public static final int icSigBToA0Tag = 0x42324130; // 'B2A0' 
  public static final int icSigBToA1Tag = 0x42324131; // 'B2A1' 
  public static final int icSigBToA2Tag = 0x42324132; // 'B2A2' 
  public static final int icSigCalibrationDateTimeTag = 0x63616C74; // 'calt' 
  public static final int icSigCharTargetTag = 0x74617267; // 'targ' 
  public static final int icSigCopyrightTag = 0x63707274; // 'cprt' 
  public static final int icSigCrdInfoTag = 0x63726469; // 'crdi' 
  public static final int icSigDeviceMfgDescTag = 0x646D6E64; // 'dmnd' 
  public static final int icSigDeviceModelDescTag = 0x646D6464; // 'dmdd' 
  public static final int icSigDeviceSettingsTag = 0x64657673; // 'devs' 
  public static final int icSigGamutTag = 0x67616D74; // 'gamt' 
  public static final int icSigGrayTRCTag = 0x6b545243; // 'kTRC' 
  public static final int icSigGreenColorantTag = 0x6758595A; // 'gXYZ' 
  public static final int icSigGreenTRCTag = 0x67545243; // 'gTRC' 
  public static final int icSigLuminanceTag = 0x6C756d69; // 'lumi' 
  public static final int icSigMeasurementTag = 0x6D656173; // 'meas' 
  public static final int icSigMediaBlackPointTag = 0x626B7074; // 'bkpt' 
  public static final int icSigMediaWhitePointTag = 0x77747074; // 'wtpt' 
  public static final int icSigNamedColor2Tag = 0x6E636C32; // 'ncl2' 
  public static final int icSigOutputResponseTag = 0x72657370; // 'resp' 
  public static final int icSigPreview0Tag = 0x70726530; // 'pre0' 
  public static final int icSigPreview1Tag = 0x70726531; // 'pre1' 
  public static final int icSigPreview2Tag = 0x70726532; // 'pre2' 
  public static final int icSigProfileDescriptionTag = 0x64657363; // 'desc' 
  public static final int icSigProfileSequenceDescTag = 0x70736571; // 'pseq' 
  public static final int icSigPs2CRD0Tag = 0x70736430; // 'psd0' 
  public static final int icSigPs2CRD1Tag = 0x70736431; // 'psd1' 
  public static final int icSigPs2CRD2Tag = 0x70736432; // 'psd2' 
  public static final int icSigPs2CRD3Tag = 0x70736433; // 'psd3' 
  public static final int icSigPs2CSATag = 0x70733273; // 'ps2s' 
  public static final int icSigPs2RenderingIntentTag = 0x70733269; // 'ps2i' 
  public static final int icSigRedColorantTag = 0x7258595A; // 'rXYZ' 
  public static final int icSigRedTRCTag = 0x72545243; // 'rTRC' 
  public static final int icSigScreeningDescTag = 0x73637264; // 'scrd' 
  public static final int icSigScreeningTag = 0x7363726E; // 'scrn' 
  public static final int icSigTechnologyTag = 0x74656368; // 'tech' 
  public static final int icSigUcrBgTag = 0x62666420; // 'bfd ' 
  public static final int icSigViewingCondDescTag = 0x76756564; // 'vued' 
  public static final int icSigViewingConditionsTag = 0x76696577; // 'view' 
  public static final int icSigChromaticityTag = 0x6368726D; // 'chrm'

  /**
   * Non-ICC tag 'head' for use in retrieving the header with getData()
   */
  public static final int icSigHead = 0x68656164;

  /**
   * Header offsets
   */
  public static final int icHdrSize = 0;
  public static final int icHdrCmmId = 4;
  public static final int icHdrVersion = 8;
  public static final int icHdrDeviceClass = 12;
  public static final int icHdrColorSpace = 16;
  public static final int icHdrPcs = 20;
  public static final int icHdrDate = 24;
  public static final int icHdrMagic = 36;
  public static final int icHdrPlatform = 40;
  public static final int icHdrFlags = 44;
  public static final int icHdrManufacturer = 48;
  public static final int icHdrModel = 52;
  public static final int icHdrAttributes = 56;
  public static final int icHdrRenderingIntent = 64;
  public static final int icHdrIlluminant = 68;
  public static final int icHdrCreator = 80;

  /**
   *
   */
  public static final int icTagType = 0;
  public static final int icTagReserved = 4;
  public static final int icCurveCount = 8;
  public static final int icCurveData = 12;
  public static final int icXYZNumberX = 8;

  /**
   * offset of the Tag table
   */
  private static final int tagTableOffset = 128;

  /**
   * @serial
   */
  private static final int iccProfileSerializedDataVersion = 1;

  /**
   * Constants related to generating profiles for
   * built-in colorspace profiles
   */
  /**
   * Copyright notice to stick into built-in-profile files.
   */
  private static final String copyrightNotice = "Generated by GNU Classpath.";

  /**
   * Resolution of the TRC to use for predefined profiles.
   * 1024 should suffice.
   */
  private static final int TRC_POINTS = 1024;

  /**
   * CIE 1931 D50 white point (in Lab coordinates)
   */
  private static final float[] D50 = { 0.96422f, 1.00f, 0.82521f };

  /**
   * Color space profile ID - this is the address of the native littleCMS
   * profile that backs this profile.
   */
  private transient long profileID;

  /**
   * The profile header data
   */
  private transient ProfileHeader header;

  /**
   * The ColorSpace used, or -1 if not one of the built-in types.
   */
  private int colorSpace = -1;

  /**
   * Store the color manager for easy access.
   */
  private PCMM pcmm = CMSManager.getModule();

  /**
   * Contructor for predefined colorspaces
   */
  ICC_Profile(int colorSpace)
  {
    this.colorSpace = colorSpace;
    createProfile(colorSpace);
  }

  /**
   * Constructs an ICC_Profile from a byte array of data.
   */
  ICC_Profile(byte[] data) throws IllegalArgumentException
  {
    // get header and verify it
    header = new ProfileHeader(data);
    header.verifyHeader(data.length);

    profileID = pcmm.loadProfile(data);
  }

  /**
   * Free up the used memory.
   */
  protected void finalize()
  {
    pcmm.freeProfile(profileID);
  }

  /**
   * Returns an ICC_Profile instance from a byte array of profile data.
   *
   * An instance of the specialized classes ICC_ProfileRGB or ICC_ProfileGray
   * may be returned if appropriate.
   *
   * @param data - the profile data
   * @return An ICC_Profile object
   *
   * @throws IllegalArgumentException if the profile data is an invalid
   * v2 profile.
   */
  public static ICC_Profile getInstance(byte[] data)
  {
    ProfileHeader header = new ProfileHeader(data);

    // get header and verify it
    header = new ProfileHeader(data);
    header.verifyHeader(data.length);

    if (isRGBProfile(header))
      return new ICC_ProfileRGB(data);
    if (isGrayProfile(header))
      return new ICC_ProfileGray(data);

    return new ICC_Profile(data);
  }

  /**
   * Returns an predefined ICC_Profile instance.
   *
   * This will construct an ICC_Profile instance from one of the predefined
   * color spaces in the ColorSpace class. (e.g. CS_sRGB, CS_GRAY, etc)
   *
   * An instance of the specialized classes ICC_ProfileRGB or ICC_ProfileGray
   * may be returned if appropriate.
   *
   * @return An ICC_Profile object
   */
  public static ICC_Profile getInstance(int cspace)
  {
    if (cspace == ColorSpace.CS_sRGB || cspace == ColorSpace.CS_LINEAR_RGB)
      return new ICC_ProfileRGB(cspace);
    if (cspace == ColorSpace.CS_GRAY)
      return new ICC_ProfileGray(cspace);
    return new ICC_Profile(cspace);
  }

  /**
   * Returns an ICC_Profile instance from an ICC Profile file.
   *
   * An instance of the specialized classes ICC_ProfileRGB or ICC_ProfileGray
   * may be returned if appropriate.
   *
   * @param filename - the file name of the profile file.
   * @return An ICC_Profile object
   *
   * @throws IllegalArgumentException if the profile data is an invalid
   * v2 profile.
   * @throws IOException if the file could not be read.
   */
  public static ICC_Profile getInstance(String filename)
                                 throws IOException
  {
    return getInstance(new FileInputStream(filename));
  }

  /**
   * Returns an ICC_Profile instance from an InputStream.
   *
   * This method can be used for reading ICC profiles embedded in files
   * which support this. (JPEG and SVG for instance).
   *
   * The stream is treated in the following way: The profile header
   * (128 bytes) is read first, and the header is validated. If the profile
   * header is valid, it will then attempt to read the rest of the profile
   * from the stream. The stream is not closed after reading.
   *
   * An instance of the specialized classes ICC_ProfileRGB or ICC_ProfileGray
   * may be returned if appropriate.
   *
   * @param in - the input stream to read the profile from.
   * @return An ICC_Profile object
   *
   * @throws IllegalArgumentException if the profile data is an invalid
   * v2 profile.
   * @throws IOException if the stream could not be read.
   */
  public static ICC_Profile getInstance(InputStream in)
                                 throws IOException
  {
    // read the header
    byte[] headerData = new byte[ProfileHeader.HEADERSIZE];
    if (in.read(headerData) != ProfileHeader.HEADERSIZE)
      throw new IllegalArgumentException("Invalid profile header");

    ProfileHeader header = new ProfileHeader(headerData);

    // verify it as a correct ICC header, but do not verify the
    // size as we are reading from a stream.
    header.verifyHeader(-1);

    // get the size
    byte[] data = new byte[header.getSize()];
    System.arraycopy(headerData, 0, data, 0, ProfileHeader.HEADERSIZE);

    // read the rest
    if (in.read(data, ProfileHeader.HEADERSIZE,
                header.getSize() - ProfileHeader.HEADERSIZE) != header.getSize()
        - ProfileHeader.HEADERSIZE)
      throw new IOException("Incorrect profile size");

    return getInstance(data);
  }

  /**
   * Returns the major version number
   */
  public int getMajorVersion()
  {
    return header.getMajorVersion();
  }

  /**
   * Returns the minor version number.
   *
   * Only the least-significant byte contains data, in BCD form:
   * the least-significant nibble is the BCD bug fix revision,
   * the most-significant nibble is the BCD minor revision number.
   *
   * (E.g. For a v2.1.0 profile this will return <code>0x10</code>)
   */
  public int getMinorVersion()
  {
    return header.getMinorVersion();
  }

  /**
   * Returns the device class of this profile,
   *
   * (E.g. CLASS_INPUT for a scanner profile,
   * CLASS_OUTPUT for a printer)
   */
  public int getProfileClass()
  {
    return header.getProfileClass();
  }

  /**
   * Returns the color space of this profile, in terms
   * of the color space constants defined in ColorSpace.
   * (For example, it may be a ColorSpace.TYPE_RGB)
   */
  public int getColorSpaceType()
  {
    return header.getColorSpace();
  }

  /**
   * Returns the color space of this profile's Profile Connection Space (OCS)
   *
   * In terms of the color space constants defined in ColorSpace.
   * This may be TYPE_XYZ or TYPE_Lab
   */
  public int getPCSType()
  {
    return header.getProfileColorSpace();
  }

  /**
   * Writes the profile data to an ICC profile file.
   * @param filename - The name of the file to write
   * @throws IOException if the write failed.
   */
  public void write(String filename) throws IOException
  {
    FileOutputStream out = new FileOutputStream(filename);
    write(out);
    out.flush();
    out.close();
  }

  /**
   * Writes the profile data in ICC profile file-format to a stream.
   * This is useful for embedding ICC profiles in file formats which
   * support this (such as JPEG and SVG).
   *
   * The stream is not closed after writing.
   * @param out - The outputstream to which the profile data should be written
   * @throws IOException if the write failed.
   */
  public void write(OutputStream out) throws IOException
  {
    out.write(getData());
  }

  /**
   * Returns the data corresponding to this ICC_Profile as a byte array.
   *
   * @return The data in a byte array,
   * where the first element corresponds to first byte of the profile file.
   */
  public byte[] getData()
  {
    byte[] data = new byte[pcmm.getProfileSize(profileID)];
    pcmm.getProfileData(profileID, data);
    return data;
  }

  /**
   * Returns the ICC profile tag data
   * The non ICC-tag icSigHead is also permitted to request the header data.
   *
   * @param tagSignature The ICC signature of the requested tag
   * @return A byte array containing the tag data
   */
  public byte[] getData(int tagSignature)
  {
    if (tagSignature == icSigHead)
      return header.getData(getSize());

    byte[] data = new byte[pcmm.getTagSize(profileID, tagSignature)];
    pcmm.getTagData(profileID, tagSignature, data);
    return data;
  }

  /**
   * Sets the ICC profile tag data.
   *
   * Note that an ICC profile can only contain one tag of each type, if
   * a tag already exists with the given signature, it is replaced.
   *
   * @param tagSignature - The signature of the tag to set
   * @param data - A byte array containing the tag data
   */
  public void setData(int tagSignature, byte[] data)
  {
    if (tagSignature == icSigHead)
      header = new ProfileHeader(data);

    pcmm.setTagData(profileID, tagSignature, data);
  }

  /**
   * Get the number of components in the profile's device color space.
   */
  public int getNumComponents()
  {
    int[] lookup = 
                   {
                     ColorSpace.TYPE_RGB, 3, ColorSpace.TYPE_CMY, 3,
                     ColorSpace.TYPE_CMYK, 4, ColorSpace.TYPE_GRAY, 1,
                     ColorSpace.TYPE_YCbCr, 3, ColorSpace.TYPE_XYZ, 3,
                     ColorSpace.TYPE_Lab, 3, ColorSpace.TYPE_HSV, 3,
                     ColorSpace.TYPE_2CLR, 2, ColorSpace.TYPE_Luv, 3,
                     ColorSpace.TYPE_Yxy, 3, ColorSpace.TYPE_HLS, 3,
                     ColorSpace.TYPE_3CLR, 3, ColorSpace.TYPE_4CLR, 4,
                     ColorSpace.TYPE_5CLR, 5, ColorSpace.TYPE_6CLR, 6,
                     ColorSpace.TYPE_7CLR, 7, ColorSpace.TYPE_8CLR, 8,
                     ColorSpace.TYPE_9CLR, 9, ColorSpace.TYPE_ACLR, 10,
                     ColorSpace.TYPE_BCLR, 11, ColorSpace.TYPE_CCLR, 12,
                     ColorSpace.TYPE_DCLR, 13, ColorSpace.TYPE_ECLR, 14,
                     ColorSpace.TYPE_FCLR, 15
                   };
    for (int i = 0; i < lookup.length; i += 2)
      if (header.getColorSpace() == lookup[i])
	return lookup[i + 1];
    return 3; // should never happen.
  }

  /**
   * After deserializing we must determine if the class we want
   * is really one of the more specialized ICC_ProfileRGB or
   * ICC_ProfileGray classes.
   */
  protected Object readResolve() throws ObjectStreamException
  {
    if (isRGBProfile(header))
      return new ICC_ProfileRGB(getData());
    if (isGrayProfile(header))
      return new ICC_ProfileGray(getData());
    return this;
  }

  /**
   * Deserializes an instance
   */
  private void readObject(ObjectInputStream s)
                   throws IOException, ClassNotFoundException
  {
    s.defaultReadObject();
    String predef = (String) s.readObject();
    byte[] data = (byte[]) s.readObject();

    if (data != null)
      {
	header = new ProfileHeader(data);
        profileID = pcmm.loadProfile(data);
      }

    if (predef != null)
      {
	predef = predef.intern();
	if (predef.equals("CS_sRGB"))
	  createProfile(ColorSpace.CS_sRGB);
	if (predef.equals("CS_LINEAR_RGB"))
	  createProfile(ColorSpace.CS_LINEAR_RGB);
	if (predef.equals("CS_CIEXYZ"))
	  createProfile(ColorSpace.CS_CIEXYZ);
	if (predef.equals("CS_GRAY"))
	  createProfile(ColorSpace.CS_GRAY);
	if (predef.equals("CS_PYCC"))
	  createProfile(ColorSpace.CS_PYCC);
      }
  }

  /**
   * Serializes an instance
   * The format is a String and a byte array,
   * The string is non-null if the instance is one of the built-in profiles.
   * Otherwise the byte array is non-null and represents the profile data.
   */
  private void writeObject(ObjectOutputStream s) throws IOException
  {
    s.defaultWriteObject();

    int colorSpaceID = getColorSpaceType();
    if (colorSpaceID == ColorSpace.CS_sRGB)
      s.writeObject("CS_sRGB");
    else if (colorSpaceID == ColorSpace.CS_LINEAR_RGB)
      s.writeObject("CS_LINEAR_RGB");
    else if (colorSpaceID == ColorSpace.CS_CIEXYZ)
      s.writeObject("CS_CIEXYZ");
    else if (colorSpaceID == ColorSpace.CS_GRAY)
      s.writeObject("CS_GRAY");
    else if (colorSpaceID == ColorSpace.CS_PYCC)
      s.writeObject("CS_PYCC");
    else
      {
	s.writeObject(null); // null string
	s.writeObject(getData()); // data
	return;
      }
    s.writeObject(null); // null data
  }

  /**
   * Returns the total size of the padded, stored data
   * Note: Tags must be stored on 4-byte aligned offsets.
   */
  private int getSize()
  {
    return pcmm.getProfileSize(profileID);
  }

  /**
   * Returns if the criteria for an ICC_ProfileRGB are met.
   * This means:
   * Color space is TYPE_RGB
   * (r,g,b)ColorantTags included
   * (r,g,b)TRCTags included
   * mediaWhitePointTag included
   */
  private static boolean isRGBProfile(ProfileHeader header)
  {
    if (header.getColorSpace() != ColorSpace.TYPE_RGB)
      return false;
    return true;

    /*
    // TODO: implement pcmm.tagExists()

    if (tags.get(TagEntry.tagHashKey(icSigRedColorantTag)) == null)
      return false;
    if (tags.get(TagEntry.tagHashKey(icSigGreenColorantTag)) == null)
      return false;
    if (tags.get(TagEntry.tagHashKey(icSigBlueColorantTag)) == null)
      return false;
    if (tags.get(TagEntry.tagHashKey(icSigRedTRCTag)) == null)
      return false;
    if (tags.get(TagEntry.tagHashKey(icSigGreenTRCTag)) == null)
      return false;
    if (tags.get(TagEntry.tagHashKey(icSigBlueTRCTag)) == null)
      return false;
    return (tags.get(TagEntry.tagHashKey(icSigMediaWhitePointTag)) != null);
    */
  }

  /**
   * Returns if the criteria for an ICC_ProfileGray are met.
   * This means:
   * Colorspace is TYPE_GRAY
   * grayTRCTag included
   * mediaWhitePointTag included
   */
  private static boolean isGrayProfile(ProfileHeader header)
  {
    if (header.getColorSpace() != ColorSpace.TYPE_GRAY)
      return false;
    return true;

    /*
    // TODO: implement pcmm.tagExists()

    if (tags.get(TagEntry.tagHashKey(icSigGrayTRCTag)) == null)
      return false;
    return (tags.get(TagEntry.tagHashKey(icSigMediaWhitePointTag)) != null);
    */
  }

  /**
   * Returns curve data for a 'curv'-type tag
   * If it's a gamma curve, a single entry will be returned with the
   * gamma value (including 1.0 for linear response)
   * Otherwise the TRC table is returned.
   *
   * (Package private - used by ICC_ProfileRGB and ICC_ProfileGray)
   */
  short[] getCurve(int signature)
  {
    byte[] data = getData(signature);
    short[] curve;

    // can't find tag?
    if (data == null)
      return null;

    // not an curve type tag?
    ByteBuffer buf = ByteBuffer.wrap(data);
    if (buf.getInt(0) != 0x63757276) // 'curv' type
      return null;
    int count = buf.getInt(8);
    if (count == 0)
      {
	curve = new short[1];
	curve[0] = 0x0100; // 1.00 in u8fixed8
	return curve;
      }
    if (count == 1)
      {
	curve = new short[1];
	curve[0] = buf.getShort(12); // other u8fixed8 gamma
	return curve;
      }
    curve = new short[count];
    for (int i = 0; i < count; i++)
      curve[i] = buf.getShort(12 + i * 2);
    return curve;
  }

  /**
   * Returns XYZ tristimulus values for an 'XYZ ' type tag
   * @return the XYZ values, or null if the tag was not an 'XYZ ' type tag.
   *
   * (Package private - used by ICC_ProfileXYZ and ICC_ProfileGray)
   */
  float[] getXYZData(int signature)
  {
    byte[] data = getData(signature);

    // can't find tag?
    if (data == null)
      return null;

    // not an XYZData type tag?
    ByteBuffer buf = ByteBuffer.wrap(data);
    if (buf.getInt(0) != icSigXYZData) // 'XYZ ' type
      return null;

    float[] point = new float[3];

    // get the X,Y,Z tristimulus values
    point[0] = ((float) buf.getInt(8)) / 65536f;
    point[1] = ((float) buf.getInt(12)) / 65536f;
    point[2] = ((float) buf.getInt(16)) / 65536f;
    return point;
  }

  /**
   * Returns the profile ID if it's a predefined profile
   * Or -1 for a profile loaded from an ICC profile
   *
   * (Package private - used by ICC_ColorSpace)
   */
  int isPredefined()
  {
    return colorSpace;
  }

  /**
   * Creates a tag of XYZ-value type.
   */
  private byte[] makeXYZData(float[] values)
  {
    ByteBuffer buf = ByteBuffer.allocate(20);
    buf.putInt(0, icSigXYZData); // 'XYZ '
    buf.putInt(4, 0);
    buf.putInt(8, (int) (values[0] * 65536.0));
    buf.putInt(12, (int) (values[1] * 65536.0));
    buf.putInt(16, (int) (values[2] * 65536.0));
    return buf.array();
  }

  /**
   * Creates a tag of text type
   */
  private byte[] makeTextTag(String text)
  {
    int length = text.length();
    ByteBuffer buf = ByteBuffer.allocate(8 + length + 1);
    byte[] data;
    try
      {
	data = text.getBytes("US-ASCII");
      }
    catch (UnsupportedEncodingException e)
      {
	data = new byte[length]; // shouldn't happen
      }

    buf.putInt(0, (int) 0x74657874); // 'text'
    buf.putInt(4, 0);
    for (int i = 0; i < length; i++)
      buf.put(8 + i, data[i]);
    buf.put(8 + length, (byte) 0); // null-terminate
    return buf.array();
  }

  /**
   * Creates a tag of textDescriptionType
   */
  private byte[] makeDescTag(String text)
  {
    int length = text.length();
    ByteBuffer buf = ByteBuffer.allocate(90 + length + 1);
    buf.putInt(0, (int) 0x64657363); // 'desc'
    buf.putInt(4, 0); // reserved 
    buf.putInt(8, length + 1); // ASCII length, including null termination
    byte[] data;

    try
      {
	data = text.getBytes("US-ASCII");
      }
    catch (UnsupportedEncodingException e)
      {
	data = new byte[length]; // shouldn't happen
      }

    for (int i = 0; i < length; i++)
      buf.put(12 + i, data[i]);
    buf.put(12 + length, (byte) 0); // null-terminate

    for (int i = 0; i < 39; i++)
      buf.putShort(13 + length + (i * 2), (short) 0); // 78 bytes we can ignore

    return buf.array();
  }

  /**
   * Creates a tag of TRC type (linear curve)
   */
  private byte[] makeTRC()
  {
    ByteBuffer buf = ByteBuffer.allocate(12);
    buf.putInt(0, 0x63757276); // 'curv' type
    buf.putInt(4, 0); // reserved
    buf.putInt(8, 0);
    return buf.array();
  }

  /**
   * Creates a tag of TRC type (single gamma value)
   */
  private byte[] makeTRC(float gamma)
  {
    short gammaValue = (short) (gamma * 256f);
    ByteBuffer buf = ByteBuffer.allocate(14);
    buf.putInt(0, 0x63757276); // 'curv' type
    buf.putInt(4, 0); // reserved
    buf.putInt(8, 1);
    buf.putShort(12, gammaValue); // 1.00 in u8fixed8
    return buf.array();
  }

  /**
   * Creates a tag of TRC type (TRC curve points)
   */
  private byte[] makeTRC(float[] trc)
  {
    ByteBuffer buf = ByteBuffer.allocate(12 + 2 * trc.length);
    buf.putInt(0, 0x63757276); // 'curv' type
    buf.putInt(4, 0); // reserved
    buf.putInt(8, trc.length); // number of points

    // put the curve values 
    for (int i = 0; i < trc.length; i++)
      buf.putShort(12 + i * 2, (short) (trc[i] * 65535f));

    return buf.array();
  }

  /**
   * Creates an identity color lookup table.
   */
  private byte[] makeIdentityClut()
  {
    final int nIn = 3;
    final int nOut = 3;
    final int nInEntries = 256;
    final int nOutEntries = 256;
    final int gridpoints = 16;

    // gridpoints**nIn
    final int clutSize = 2 * nOut * gridpoints * gridpoints * gridpoints;
    final int totalSize = clutSize + 2 * nInEntries * nIn
                          + 2 * nOutEntries * nOut + 52;

    ByteBuffer buf = ByteBuffer.allocate(totalSize);
    buf.putInt(0, 0x6D667432); // 'mft2'
    buf.putInt(4, 0); // reserved
    buf.put(8, (byte) nIn); // number input channels
    buf.put(9, (byte) nOut); // number output channels
    buf.put(10, (byte) gridpoints); // number gridpoints
    buf.put(11, (byte) 0); // padding

    // identity matrix
    buf.putInt(12, 65536); // = 1 in s15.16 fixed point
    buf.putInt(16, 0);
    buf.putInt(20, 0);
    buf.putInt(24, 0);
    buf.putInt(28, 65536);
    buf.putInt(32, 0);
    buf.putInt(36, 0);
    buf.putInt(40, 0);
    buf.putInt(44, 65536);

    buf.putShort(48, (short) nInEntries); // input table entries
    buf.putShort(50, (short) nOutEntries); // output table entries

    // write the linear input channels, unsigned 16.16 fixed point,
    // from 0.0 to FF.FF
    for (int channel = 0; channel < 3; channel++)
      for (int i = 0; i < nInEntries; i++)
        {
	  short n = (short) ((i << 8) | i); // assumes 256 entries
	  buf.putShort(52 + (channel * nInEntries + i) * 2, n);
        }
    int clutOffset = 52 + nInEntries * nIn * 2;

    for (int x = 0; x < gridpoints; x++)
      for (int y = 0; y < gridpoints; y++)
	for (int z = 0; z < gridpoints; z++)
	  {
	    int offset = clutOffset + z * 2 * nOut + y * gridpoints * 2 * nOut
	                 + x * gridpoints * gridpoints * 2 * nOut;
	    double xf = ((double) x) / ((double) gridpoints - 1.0);
	    double yf = ((double) y) / ((double) gridpoints - 1.0);
	    double zf = ((double) z) / ((double) gridpoints - 1.0);
	    buf.putShort(offset, (short) (xf * 65535.0));
	    buf.putShort(offset + 2, (short) (yf * 65535.0));
	    buf.putShort(offset + 4, (short) (zf * 65535.0));
	  }

    for (int channel = 0; channel < 3; channel++)
      for (int i = 0; i < nOutEntries; i++)
        {
	  short n = (short) ((i << 8) | i); // assumes 256 entries
	  buf.putShort(clutOffset + clutSize + (channel * nOutEntries + i) * 2,
	               n);
        }

    return buf.array();
  }

  /**
   * Creates profile data corresponding to the built-in colorspaces.
   */
  private void createProfile(int colorSpace) throws IllegalArgumentException
  {
    header = new ProfileHeader();

    switch (colorSpace)
      {
      case ColorSpace.CS_sRGB:
	createRGBProfile();
	return;
      case ColorSpace.CS_LINEAR_RGB:
	createLinearRGBProfile();
	return;
      case ColorSpace.CS_CIEXYZ:
	createCIEProfile();
	return;
      case ColorSpace.CS_GRAY:
	createGrayProfile();
	return;
      case ColorSpace.CS_PYCC:
	createPyccProfile();
	return;
      default:
	throw new IllegalArgumentException("Not a predefined color space!");
      }
  }

  /**
   * Creates an ICC_Profile representing the sRGB color space
   */
  private void createRGBProfile()
  {
    header.setColorSpace( ColorSpace.TYPE_RGB );
    header.setProfileColorSpace( ColorSpace.TYPE_XYZ );
    ICC_ColorSpace cs = new ICC_ColorSpace(this);

    float[] r = { 1f, 0f, 0f };
    float[] g = { 0f, 1f, 0f };
    float[] b = { 0f, 0f, 1f };
    float[] black = { 0f, 0f, 0f };

    // CIE 1931 D50 white point (in Lab coordinates)
    float[] white = D50;

    // Get tristimulus values (matrix elements)
    r = cs.toCIEXYZ(r);
    g = cs.toCIEXYZ(g);
    b = cs.toCIEXYZ(b);

    // Generate the sRGB TRC curve, this is the linear->nonlinear
    // RGB transform.
    cs = new ICC_ColorSpace(getInstance(ICC_ColorSpace.CS_LINEAR_RGB));
    float[] points = new float[TRC_POINTS];
    float[] in = new float[3];
    for (int i = 0; i < TRC_POINTS; i++)
      {
	in[0] = in[1] = in[2] = ((float) i) / ((float) TRC_POINTS - 1);
	in = cs.fromRGB(in);
	// Note this value is the same for all components.
	points[i] = in[0];
      }

    Hashtable<String, TagEntry> tags = new Hashtable<String, TagEntry>();

    TagEntry t = new TagEntry(icSigRedColorantTag, makeXYZData(r));
    tags.put(t.hashKey(), t);
    t = new TagEntry(icSigGreenColorantTag, makeXYZData(g));
    tags.put(t.hashKey(), t);
    t = new TagEntry(icSigBlueColorantTag, makeXYZData(b));
    tags.put(t.hashKey(), t);
    t = new TagEntry(icSigMediaWhitePointTag, makeXYZData(white));
    tags.put(t.hashKey(), t);
    t = new TagEntry(icSigMediaBlackPointTag, makeXYZData(black));
    tags.put(t.hashKey(), t);
    t = new TagEntry(icSigRedTRCTag, makeTRC(points));
    tags.put(t.hashKey(), t);
    t = new TagEntry(icSigGreenTRCTag, makeTRC(points));
    tags.put(t.hashKey(), t);
    t = new TagEntry(icSigBlueTRCTag, makeTRC(points));
    tags.put(t.hashKey(), t);
    t = new TagEntry(icSigCopyrightTag, makeTextTag(copyrightNotice));
    tags.put(t.hashKey(), t);
    t = new TagEntry(icSigProfileDescriptionTag, makeDescTag("Generic sRGB"));
    tags.put(t.hashKey(), t);

    profileID = pcmm.loadProfile(getDataFromTags(header, tags));
  }

  /**
   * Creates an linear sRGB profile
   */
  private void createLinearRGBProfile()
  {
    header.setColorSpace(ColorSpace.TYPE_RGB);
    header.setProfileColorSpace(ColorSpace.TYPE_XYZ);
    ICC_ColorSpace cs = new ICC_ColorSpace(this);

    float[] r = { 1f, 0f, 0f };
    float[] g = { 0f, 1f, 0f };
    float[] b = { 0f, 0f, 1f };
    float[] black = { 0f, 0f, 0f };

    float[] white = D50;

    // Get tristimulus values (matrix elements)
    r = cs.toCIEXYZ(r);
    g = cs.toCIEXYZ(g);
    b = cs.toCIEXYZ(b);

    Hashtable<String, TagEntry> tags = new Hashtable<String, TagEntry>();

    TagEntry t = new TagEntry(icSigRedColorantTag, makeXYZData(r));
    tags.put(t.hashKey(), t);
    t = new TagEntry(icSigGreenColorantTag, makeXYZData(g));
    tags.put(t.hashKey(), t);
    t = new TagEntry(icSigBlueColorantTag, makeXYZData(b));
    tags.put(t.hashKey(), t);

    t = new TagEntry(icSigMediaWhitePointTag, makeXYZData(white));
    tags.put(t.hashKey(), t);
    t = new TagEntry(icSigMediaBlackPointTag, makeXYZData(black));
    tags.put(t.hashKey(), t);

    t = new TagEntry(icSigRedTRCTag, makeTRC());
    tags.put(t.hashKey(), t);
    t = new TagEntry(icSigGreenTRCTag, makeTRC());
    tags.put(t.hashKey(), t);
    t = new TagEntry(icSigBlueTRCTag, makeTRC());
    tags.put(t.hashKey(), t);
    t = new TagEntry(icSigCopyrightTag, makeTextTag(copyrightNotice));
    tags.put(t.hashKey(), t);
    t = new TagEntry(icSigProfileDescriptionTag, makeDescTag("Linear RGB"));
    tags.put(t.hashKey(), t);

    profileID = pcmm.loadProfile(getDataFromTags(header, tags));
  }

  /**
   * Creates an CIE XYZ identity profile
   */
  private void createCIEProfile()
  {
    header.setColorSpace( ColorSpace.TYPE_XYZ );
    header.setProfileColorSpace( ColorSpace.TYPE_XYZ );
    header.setProfileClass( CLASS_COLORSPACECONVERSION );
    ICC_ColorSpace cs = new ICC_ColorSpace(this);

    float[] white = D50;

    Hashtable<String, TagEntry> tags = new Hashtable<String, TagEntry>();

    TagEntry t = new TagEntry(icSigMediaWhitePointTag, makeXYZData(white));
    tags.put(t.hashKey(), t);
    t = new TagEntry(icSigAToB0Tag, makeIdentityClut());
    tags.put(t.hashKey(), t);
    t = new TagEntry(icSigBToA0Tag, makeIdentityClut());
    tags.put(t.hashKey(), t);
    t = new TagEntry(icSigCopyrightTag, makeTextTag(copyrightNotice));
    tags.put(t.hashKey(), t);
    t = new TagEntry(icSigProfileDescriptionTag, makeDescTag("CIE XYZ identity profile"));
    tags.put(t.hashKey(), t);

    profileID = pcmm.loadProfile(getDataFromTags(header, tags));
  }

  /**
   * Creates a linear gray ICC_Profile
   */
  private void createGrayProfile()
  {
    header.setColorSpace(ColorSpace.TYPE_GRAY);
    header.setProfileColorSpace(ColorSpace.TYPE_XYZ);

    // CIE 1931 D50 white point (in Lab coordinates)
    float[] white = D50;

    Hashtable<String, TagEntry> tags = new Hashtable<String, TagEntry>();

    TagEntry t = new TagEntry(icSigMediaWhitePointTag, makeXYZData(white));
    tags.put(t.hashKey(), t);
    t = new TagEntry(icSigGrayTRCTag, makeTRC(1.0f));
    tags.put(t.hashKey(), t);
    t = new TagEntry(icSigCopyrightTag, makeTextTag(copyrightNotice));
    tags.put(t.hashKey(), t);
    t = new TagEntry(icSigProfileDescriptionTag, makeDescTag("Linear grayscale"));
    tags.put(t.hashKey(), t);

    profileID = pcmm.loadProfile(getDataFromTags(header, tags));
  }

  /**
   * XXX Implement me
   */
  private void createPyccProfile()
  {
    header.setColorSpace(ColorSpace.TYPE_3CLR);
    header.setProfileColorSpace(ColorSpace.TYPE_XYZ);

    // Create CLUTs here. :-)

    Hashtable<String, TagEntry> tags = new Hashtable<String, TagEntry>();

    TagEntry t = new TagEntry(icSigCopyrightTag, makeTextTag(copyrightNotice));
    tags.put(t.hashKey(), t);
    t = new TagEntry(icSigProfileDescriptionTag, makeDescTag("Photo YCC"));
    tags.put(t.hashKey(), t);

    profileID = pcmm.loadProfile(getDataFromTags(header, tags));
  }

  /**
   * Creates a byte buffer of ICC data from a given header and tags.
   */
  private static byte[] getDataFromTags(ProfileHeader header, Hashtable<String, TagEntry> tags)
  {
    // First calculate size of buffer
    int size = ProfileHeader.HEADERSIZE; // size of header

    int tagTableSize = 4 + tags.size() * TagEntry.entrySize; // size of tag table
    if ((tagTableSize & 0x0003) != 0)
      tagTableSize += 4 - (tagTableSize & 0x0003); // pad
    size += tagTableSize;

    // Also convert tag hashtable to bytes while we're iterating through it
    int offset = 4;
    int tagOffset = ProfileHeader.HEADERSIZE + tagTableSize;
    ByteBuffer tt = ByteBuffer.allocate(tagTableSize);
    tt.putInt(tags.size()); // number of tags

    Enumeration<TagEntry> e = tags.elements();
    while (e.hasMoreElements())
      {
        TagEntry tag = e.nextElement();
        int tagSize = tag.getSize();
        if ((tagSize & 0x0003) != 0)
          tagSize += 4 - (tagSize & 0x0003); // pad
        size += tagSize;

        tt.putInt(offset, tag.getSignature());
        tt.putInt(offset + 4, tagOffset);
        tt.putInt(offset + 8, tag.getSize());
        tag.setOffset(tagOffset);
        tagOffset += tagSize;
        offset += 12;
      }

    byte[] data = new byte[size];

    // Copy header & tags into buffer
    System.arraycopy(header.getData(size), 0, data, 0, ProfileHeader.HEADERSIZE);

    if (tt.hasArray())
    {
      System.arraycopy(tt.array(), 0, data, ProfileHeader.HEADERSIZE, tt.array().length);
    }
    else
    {
      tt.rewind();
      tt.get(data, ProfileHeader.HEADERSIZE, tt.remaining());
    }

    e = tags.elements();
    while (e.hasMoreElements())
      {
        TagEntry tag = e.nextElement();
        System.arraycopy(tag.getData(), 0,
                         data, tag.getOffset(), tag.getSize());
      }
    return data;
  }

} // class ICC_Profile
