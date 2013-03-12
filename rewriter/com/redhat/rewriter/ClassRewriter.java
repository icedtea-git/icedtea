/* ClassRewriter -- Rewrite package name used in a class.
   Copyright (C) 2010  Red Hat

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

package com.redhat.rewriter;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.IOException;

import java.nio.charset.Charset;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Executors;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;

import java.util.logging.ConsoleHandler;
import java.util.logging.Level;
import java.util.logging.Logger;

public class ClassRewriter
  implements Callable<Void>
{

  /**
   * The {@link Logger} instance for messages.
   */
  private static final Logger logger =
    Logger.getLogger(ClassRewriter.class.getPackage().getName());;

  /**
   * Set this to true to turn on debug messages.
   */
  private static final boolean DEBUG = false;

  /**
   * The executor for submitting rewriting jobs.
   */
  private static final ExecutorService executor = 
    Executors.newSingleThreadExecutor();

  /**
   * The source directory, set once by main.
   */
  private static File srcDir;

  /**
   * The list of tasks submitted to the executor.
   */
  private static List<Future<Void>> tasks = new ArrayList<Future<Void>>();

  public static void main(String[] args)
    throws ExecutionException, InterruptedException
  {
    if (args.length < 4)
      {
        System.err.println("ClassRewriter <srcdir> <destdir> <oldpkg> <newpkg>");
        System.exit(-1);
      }
    Level level = DEBUG ? Level.FINE : Level.INFO;
    logger.setUseParentHandlers(false);
    logger.setLevel(level);
    ConsoleHandler handler = new ConsoleHandler();
    handler.setLevel(level);
    logger.addHandler(handler);
    srcDir = new File(args[0]);
    processFile(srcDir, args[1], args[2], args[3]);
    logger.info("Waiting for rewrites to complete...");
    executor.shutdown();
    executor.awaitTermination(1, TimeUnit.MINUTES);
    logger.info("Checking for errors...");
    // Check for exceptions
    for (Future<Void> f : tasks)
      f.get();
    logger.info("Rewriting completed successfully.");
  }

  private static void processFile(File srcDir, String destDir,
                                  String oldPkg, String newPkg)
  {
    if (srcDir.isDirectory())
      {
        logger.fine("Recursing into " + srcDir);
        for (File f : srcDir.listFiles())
          processFile(f, destDir, oldPkg, newPkg);
      }
    else if (srcDir.getName().endsWith(".class"))
      {
        logger.info("Processing class " + srcDir);
        tasks.add(executor.submit(new ClassRewriter(srcDir, destDir,
                                                    oldPkg, newPkg)));
      }
    else
      logger.fine("Skipping " + srcDir);
  }

  /**
   * The class file to alter.
   */
  private final File classFile;

  /**
   * The destination directory.
   */
  private final String destDir;

  /**
   * The old package name.
   */
  private final String oldPackage;

  /**
   * The new package name.
   */
  private final String newPackage;

  public ClassRewriter(File classFile, String destDir,
                       String oldPackage, String newPackage)
  {
    this.classFile = classFile;
    this.oldPackage = oldPackage;
    this.newPackage = newPackage;
    this.destDir = destDir;
  }

  public Void call()
    throws IOException
  {
    String slashedOldPackage = oldPackage.replace(".", "/");
    String slashedNewPackage = newPackage.replace(".", "/");
    String dollaredOldPackage = oldPackage.replace(".", "$");
    String dollaredNewPackage = newPackage.replace(".", "$");

    File outClass =
      new File(classFile.getPath().replace(srcDir.getPath() + File.separator,
                                           destDir + File.separator).replace(slashedOldPackage, slashedNewPackage));
    File targetDir = outClass.getParentFile();
    while (!targetDir.exists())
      outClass.getParentFile().mkdirs();

    DataInputStream is = new DataInputStream(new FileInputStream(classFile));
    DataOutputStream os = new DataOutputStream(new FileOutputStream(outClass));

    /* Check magic 0xCAFEBABE is present */
    byte[] magic = new byte[4];
    int count = is.read(magic);
    if (count != 4 || !Arrays.equals(magic, new byte[] { (byte) 0xCA,
                                                         (byte) 0xFE,
                                                         (byte) 0xBA,
                                                         (byte) 0xBE }))
      throw new IOException(classFile + " is not a class file.");
    os.write(magic);

    /* Copy version number */
    byte[] version = new byte[4];
    count = is.read(version);
    if (count != 4)
      throw new IOException("Could not read version number.");
    os.write(version);

    int cpCount = is.readUnsignedShort();
    os.writeShort(cpCount);
    logger.fine("Constant pool has " + cpCount + " items.");

    for (int a = 1; a < cpCount ; ++a)
      {
        byte tag = is.readByte();
        String prefix = "At index " + a + ", tag " + tag + ": ";
        os.write(tag);
        switch (tag)
          {
          case 1:
            /* CONSTANT_Utf8_Info */
            String data = is.readUTF();
            logger.fine(prefix + "String " + data);
            if (data.contains(oldPackage))
              {
                logger.fine(String.format("%s: Found %s\n", outClass.toString(), data));
                data = data.replace(oldPackage, newPackage);
                logger.fine(String.format("%s: Rewriting to %s\n", outClass.toString(), data));
              }
            else if (data.contains(slashedOldPackage))
              {
                logger.fine(String.format("%s: Found %s\n", outClass.toString(), data));
                data = data.replace(slashedOldPackage, slashedNewPackage);
                logger.fine(String.format("%s: Rewriting to %s\n", outClass.toString(), data));
              }
            else if (data.contains(dollaredOldPackage))
              {
                logger.fine(String.format("%s: Found %s\n", outClass.toString(), data));
                data = data.replace(dollaredOldPackage, dollaredNewPackage);
                logger.fine(String.format("%s: Rewriting to %s\n", outClass.toString(), data));
              }
            os.writeUTF(data);
            break;
          case 3:
            /* CONSTANT_Integer_Info */
            int intBytes = is.readInt();
            logger.fine(prefix + "Integer " + intBytes);
            os.writeInt(intBytes);
            break;
          case 4:
            /* CONSTANT_Float_Info */
            float floatBytes = is.readFloat();
            logger.fine(prefix + "Float " + floatBytes);
            os.writeFloat(floatBytes);
            break;
          case 5:
            /* CONSTANT_Long_Info */
            long longBytes = is.readLong();
            logger.fine(prefix + "Long " + longBytes);
            os.writeLong(longBytes);
            ++a; // longs count as two entries
            break;
          case 6:
            /* CONSTANT_Double_Info */
            double doubleBytes = is.readDouble();
            logger.fine(prefix + "Double " + doubleBytes);
            os.writeDouble(doubleBytes);
            ++a; // doubles count as two entries
            break;
          case 7:
          case 8:
            /* CONSTANT_Class_Info and CONSTANT_String_Info */
            short nameIndex = is.readShort();
            if (tag == 7)
              logger.fine(prefix + "Class at index " + nameIndex);
            else
              logger.fine(prefix + "String at index " + nameIndex);
            os.writeShort(nameIndex);
            break;
          case 9:
          case 10:
          case 11:
            /* CONSTANT_Fieldref_Info, CONSTANT_Methodref_Info,
               CONSTANT_InterfaceMethodrefInfo */
            short classIndex = is.readShort();
            short nameAndTypeIndex = is.readShort();
            if (tag == 9)
              logger.fine(prefix + "Field with class at index " + classIndex +
                                 " with name and type at " + nameAndTypeIndex);
            else if (tag == 10)
              logger.fine(prefix + "Method with class at index " + classIndex +
                                 " with name and type at " + nameAndTypeIndex);
            else
              logger.fine(prefix + "Interface with class at index " + classIndex +
                                 " with name and type at " + nameAndTypeIndex);
            os.writeShort(classIndex);
            os.writeShort(nameAndTypeIndex);
            break;
          case 12:
            /* CONSTANT_NameAndTypeInfo */
            short nIndex = is.readShort();
            short descriptorIndex = is.readShort();
            logger.fine(prefix + "Name at index " + nIndex +
                               " with descriptor at index " + descriptorIndex);
            os.writeShort(nIndex);
            os.writeShort(descriptorIndex);
            break;
          }
      }
    for (int nextByte = is.read(); nextByte != -1; nextByte = is.read())
      os.write(nextByte);
    is.close();
    os.close();
    return null;
  }

}
