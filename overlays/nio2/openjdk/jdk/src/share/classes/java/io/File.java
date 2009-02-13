/*
 * Copyright 1994-2008 Sun Microsystems, Inc.  All Rights Reserved.
 * Copyright 2009 Red Hat, Inc.
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

package org.classpath.icedtea.java.io;

import java.beans.ConstructorProperties;
import java.net.URI;
import java.net.URL;
import java.net.MalformedURLException;
import java.net.URISyntaxException;

import java.io.IOException;
import java.io.IOError;

import java.util.*;
import java.util.concurrent.atomic.AtomicInteger;
import java.security.AccessController;
import java.security.PrivilegedAction;
import sun.security.action.GetPropertyAction;

import org.classpath.icedtea.java.nio.file.FileAlreadyExistsException;
import org.classpath.icedtea.java.nio.file.FileSystem;
import org.classpath.icedtea.java.nio.file.FileSystems;
import org.classpath.icedtea.java.nio.file.InvalidPathException;
import org.classpath.icedtea.java.nio.file.Path;
import org.classpath.icedtea.java.nio.file.Paths;

import org.classpath.icedtea.java.nio.file.attribute.FileAttribute;
import org.classpath.icedtea.java.nio.file.attribute.FileAttributeView;
import org.classpath.icedtea.java.nio.file.attribute.PosixFilePermission;
import org.classpath.icedtea.java.nio.file.attribute.PosixFilePermissions;

import org.classpath.icedtea.misc.SharedSecrets;

/**
 * An abstract representation of file and directory pathnames.
 *
 * <p> User interfaces and operating systems use system-dependent <em>pathname
 * strings</em> to name files and directories.  This class presents an
 * abstract, system-independent view of hierarchical pathnames.  An
 * <em>abstract pathname</em> has two components:
 *
 * <ol>
 * <li> An optional system-dependent <em>prefix</em> string,
 *      such as a disk-drive specifier, <code>"/"</code>&nbsp;for the UNIX root
 *      directory, or <code>"\\\\"</code>&nbsp;for a Microsoft Windows UNC pathname, and
 * <li> A sequence of zero or more string <em>names</em>.
 * </ol>
 *
 * The first name in an abstract pathname may be a directory name or, in the
 * case of Microsoft Windows UNC pathnames, a hostname.  Each subsequent name
 * in an abstract pathname denotes a directory; the last name may denote
 * either a directory or a file.  The <em>empty</em> abstract pathname has no
 * prefix and an empty name sequence.
 *
 * <p> The conversion of a pathname string to or from an abstract pathname is
 * inherently system-dependent.  When an abstract pathname is converted into a
 * pathname string, each name is separated from the next by a single copy of
 * the default <em>separator character</em>.  The default name-separator
 * character is defined by the system property <code>file.separator</code>, and
 * is made available in the public static fields <code>{@link
 * #separator}</code> and <code>{@link #separatorChar}</code> of this class.
 * When a pathname string is converted into an abstract pathname, the names
 * within it may be separated by the default name-separator character or by any
 * other name-separator character that is supported by the underlying system.
 *
 * <p> A pathname, whether abstract or in string form, may be either
 * <em>absolute</em> or <em>relative</em>.  An absolute pathname is complete in
 * that no other information is required in order to locate the file that it
 * denotes.  A relative pathname, in contrast, must be interpreted in terms of
 * information taken from some other pathname.  By default the classes in the
 * <code>java.io</code> package always resolve relative pathnames against the
 * current user directory.  This directory is named by the system property
 * <code>user.dir</code>, and is typically the directory in which the Java
 * virtual machine was invoked.
 *
 * <p> The <em>parent</em> of an abstract pathname may be obtained by invoking
 * the {@link #getParent} method of this class and consists of the pathname's
 * prefix and each name in the pathname's name sequence except for the last.
 * Each directory's absolute pathname is an ancestor of any <tt>File</tt>
 * object with an absolute abstract pathname which begins with the directory's
 * absolute pathname.  For example, the directory denoted by the abstract
 * pathname <tt>"/usr"</tt> is an ancestor of the directory denoted by the
 * pathname <tt>"/usr/local/bin"</tt>.
 *
 * <p> The prefix concept is used to handle root directories on UNIX platforms,
 * and drive specifiers, root directories and UNC pathnames on Microsoft Windows platforms,
 * as follows:
 *
 * <ul>
 *
 * <li> For UNIX platforms, the prefix of an absolute pathname is always
 * <code>"/"</code>.  Relative pathnames have no prefix.  The abstract pathname
 * denoting the root directory has the prefix <code>"/"</code> and an empty
 * name sequence.
 *
 * <li> For Microsoft Windows platforms, the prefix of a pathname that contains a drive
 * specifier consists of the drive letter followed by <code>":"</code> and
 * possibly followed by <code>"\\"</code> if the pathname is absolute.  The
 * prefix of a UNC pathname is <code>"\\\\"</code>; the hostname and the share
 * name are the first two names in the name sequence.  A relative pathname that
 * does not specify a drive has no prefix.
 *
 * </ul>
 *
 * <p> Instances of this class may or may not denote an actual file-system
 * object such as a file or a directory.  If it does denote such an object
 * then that object resides in a <i>partition</i>.  A partition is an
 * operating system-specific portion of storage for a file system.  A single
 * storage device (e.g. a physical disk-drive, flash memory, CD-ROM) may
 * contain multiple partitions.  The object, if any, will reside on the
 * partition <a name="partName">named</a> by some ancestor of the absolute
 * form of this pathname.
 *
 * <p> A file system may implement restrictions to certain operations on the
 * actual file-system object, such as reading, writing, and executing.  These
 * restrictions are collectively known as <i>access permissions</i>.  The file
 * system may have multiple sets of access permissions on a single object.
 * For example, one set may apply to the object's <i>owner</i>, and another
 * may apply to all other users.  The access permissions on an object may
 * cause some methods in this class to fail.
 *
 * <p> Instances of the <code>File</code> class are immutable; that is, once
 * created, the abstract pathname represented by a <code>File</code> object
 * will never change.
 *
 * <h4>Interoperability with {@code java.nio.file} package</h4>
 *
 * <p> {@note new}
 * The <a href="../../java/nio/file/package-summary.html">{@code java.nio.file}</a>
 * package defines interfaces and classes for the Java virtual machine to access
 * files, file attributes, and file systems. This API may be used to overcome
 * many of the limitations of the {@code java.io.File} class.
 * The {@link #toPath toPath} method may be used to obtain a {@link
 * Path} that uses the abstract path represented by a {@code File} object to
 * locate a file. The resulting {@code Path} provides more efficient and
 * extensive access to file attributes, additional file operations, and I/O
 * exceptions to help diagnose errors when an operation on a file fails.
 *
 * @author  unascribed
 * @since   JDK1.0
 */

public class File
  extends java.io.File
{

    /**
     * Creates a new <code>File</code> instance by converting the given
     * pathname string into an abstract pathname.  If the given string is
     * the empty string, then the result is the empty abstract pathname.
     *
     * @param   pathname  A pathname string
     * @throws  NullPointerException
     *          If the <code>pathname</code> argument is <code>null</code>
     */
    @ConstructorProperties("path")
    public File(String pathname) {
      super(pathname);
    }

    /* Note: The two-argument File constructors do not interpret an empty
       parent abstract pathname as the current user directory.  An empty parent
       instead causes the child to be resolved against the system-dependent
       directory defined by the FileSystem.getDefaultParent method.  On Unix
       this default is "/", while on Microsoft Windows it is "\\".  This is required for
       compatibility with the original behavior of this class. */

    /**
     * Creates a new <code>File</code> instance from a parent pathname string
     * and a child pathname string.
     *
     * <p> If <code>parent</code> is <code>null</code> then the new
     * <code>File</code> instance is created as if by invoking the
     * single-argument <code>File</code> constructor on the given
     * <code>child</code> pathname string.
     *
     * <p> Otherwise the <code>parent</code> pathname string is taken to denote
     * a directory, and the <code>child</code> pathname string is taken to
     * denote either a directory or a file.  If the <code>child</code> pathname
     * string is absolute then it is converted into a relative pathname in a
     * system-dependent way.  If <code>parent</code> is the empty string then
     * the new <code>File</code> instance is created by converting
     * <code>child</code> into an abstract pathname and resolving the result
     * against a system-dependent default directory.  Otherwise each pathname
     * string is converted into an abstract pathname and the child abstract
     * pathname is resolved against the parent.
     *
     * @param   parent  The parent pathname string
     * @param   child   The child pathname string
     * @throws  NullPointerException
     *          If <code>child</code> is <code>null</code>
     */
    public File(String parent, String child) {
      super(parent, child);
    }

    /**
     * Creates a new <code>File</code> instance from a parent abstract
     * pathname and a child pathname string.
     *
     * <p> If <code>parent</code> is <code>null</code> then the new
     * <code>File</code> instance is created as if by invoking the
     * single-argument <code>File</code> constructor on the given
     * <code>child</code> pathname string.
     *
     * <p> Otherwise the <code>parent</code> abstract pathname is taken to
     * denote a directory, and the <code>child</code> pathname string is taken
     * to denote either a directory or a file.  If the <code>child</code>
     * pathname string is absolute then it is converted into a relative
     * pathname in a system-dependent way.  If <code>parent</code> is the empty
     * abstract pathname then the new <code>File</code> instance is created by
     * converting <code>child</code> into an abstract pathname and resolving
     * the result against a system-dependent default directory.  Otherwise each
     * pathname string is converted into an abstract pathname and the child
     * abstract pathname is resolved against the parent.
     *
     * @param   parent  The parent abstract pathname
     * @param   child   The child pathname string
     * @throws  NullPointerException
     *          If <code>child</code> is <code>null</code>
     */
    public File(File parent, String child) {
      super(parent, child);
    }



    /* -- Temporary files -- */

    private static class TemporaryDirectory {
        private TemporaryDirectory() { }

        static final File valueAsFile =
            new File(AccessController.doPrivileged(new GetPropertyAction("java.io.tmpdir")));

        // file name generation
        private static final AtomicInteger counter =
            new AtomicInteger(new Random().nextInt() & 0xffff);
        static File generateFile(String prefix, String suffix, File dir) {
            int n = counter.getAndIncrement();
            return new File(dir, prefix + Integer.toString(n) + suffix);
        }

        // default file permissions
        static final FileAttribute<Set<PosixFilePermission>> defaultPosixFilePermissions =
            PosixFilePermissions.asFileAttribute(EnumSet
                .of(PosixFilePermission.OWNER_READ, PosixFilePermission.OWNER_WRITE));
        static final boolean isPosix = isPosix();
        static boolean isPosix() {
            return AccessController.doPrivileged(
                new PrivilegedAction<Boolean>() {
                    public Boolean run() {
                        try {
			  return FileSystems.getDefault().getPath(valueAsFile.getPath())
                                .getFileStore().supportsFileAttributeView("posix");
                        } catch (IOException e) {
                            throw new IOError(e);
                        }
                    }
                });
        }
    }

    /**
     * {@note new}
     * Creates an empty file in the default temporary-file directory, using
     * the given prefix and suffix to generate its name. This method is
     * equivalent to invoking the {@link #createTempFile(String,String)
     * createTempFile(prefix,&nbsp;suffix)} method with the addition that the
     * resulting pathname may be requested to be deleted when the Java virtual
     * machine terminates, and the initial file attributes to set atomically
     * when creating the file may be specified.
     *
     * <p> When the value of the {@code deleteOnExit} method is {@code true}
     * then the resulting file is requested to be deleted when the Java virtual
     * machine terminates as if by invoking the {@link #deleteOnExit deleteOnExit}
     * method.
     *
     * <p> The {@code attrs} parameter is an optional array of {@link FileAttribute
     * attributes} to set atomically when creating the file. Each attribute is
     * identified by its {@link FileAttribute#name name}. If more than one attribute
     * of the same name is included in the array then all but the last occurrence
     * is ignored.
     *
     * @param   prefix
     *          The prefix string to be used in generating the file's
     *          name; must be at least three characters long
     * @param   suffix
     *          The suffix string to be used in generating the file's
     *          name; may be {@code null}, in which case the suffix
     *          {@code ".tmp"} will be used
     * @param   deleteOnExit
     *          {@code true} if the file denoted by resulting pathname be
     *          deleted when the Java virtual machine terminates
     * @param   attrs
     *          An optional list of file attributes to set atomically when creating
     *          the file
     *
     * @return  An abstract pathname denoting a newly-created empty file
     *
     * @throws  IllegalArgumentException
     *          If the <code>prefix</code> argument contains fewer than three
     *          characters
     * @throws  UnsupportedOperationException
     *          If the array contains an attribute that cannot be set atomically
     *          when creating the file
     * @throws  IOException
     *          If a file could not be created
     * @throws  SecurityException
     *          If a security manager exists and its <code>{@link
     *          java.lang.SecurityManager#checkWrite(java.lang.String)}</code>
     *          method does not allow a file to be created. When the {@code
     *          deleteOnExit} parameter has the value {@code true} then the
     *          security manager's {@link
     *          java.lang.SecurityManager#checkDelete(java.lang.String)} is
     *          invoked to check delete access to the file.
     * @since 1.7
     */
    public static File createTempFile(String prefix,
                                      String suffix,
                                      boolean deleteOnExit,
                                      FileAttribute<?>... attrs)
        throws IOException
    {
        if (prefix.length() < 3)
            throw new IllegalArgumentException("Prefix string too short");
        suffix = (suffix == null) ? ".tmp" : suffix;

        // special case POSIX environments so that 0600 is used as the file mode
        if (TemporaryDirectory.isPosix) {
            if (attrs.length == 0) {
                // no attributes so use default permissions
                attrs = new FileAttribute<?>[1];
                attrs[0] = TemporaryDirectory.defaultPosixFilePermissions;
            } else {
                // check if posix permissions given; if not use default
                boolean hasPermissions = false;
                for (int i=0; i<attrs.length; i++) {
                    if (attrs[i].name().equals("posix:permissions")) {
                        hasPermissions = true;
                        break;
                    }
                }
                if (!hasPermissions) {
                    FileAttribute<?>[] copy = new FileAttribute<?>[attrs.length+1];
                    System.arraycopy(attrs, 0, copy, 0, attrs.length);
                    attrs = copy;
                    attrs[attrs.length-1] =
                        TemporaryDirectory.defaultPosixFilePermissions;
                }
            }
        }

        // use Path#createFile to create file
        SecurityManager sm = System.getSecurityManager();
        for (;;) {
            File f = TemporaryDirectory
                .generateFile(prefix, suffix, TemporaryDirectory.valueAsFile);
            if (sm != null && deleteOnExit)
                sm.checkDelete(f.getPath());
            try {
                f.toPath().createFile(attrs);
                if (deleteOnExit)
		  SharedSecrets.getJavaIODeleteOnExitAccess().add(f.getPath());
                return f;
            } catch (InvalidPathException e) {
                // don't reveal temporary directory location
                if (sm != null)
                    throw new IllegalArgumentException("Invalid prefix or suffix");
                throw e;
            } catch (SecurityException e) {
                // don't reveal temporary directory location
                if (sm != null)
                    throw new SecurityException("Unable to create temporary file");
                throw e;
            } catch (FileAlreadyExistsException e) {
                // ignore
            }
        }
    }

    // -- Integration with java.nio.file --

    private volatile transient Path filePath;

    /**
     * {@note new}
     * Returns a {@link Path java.nio.file.Path} object constructed from the
     * this abstract path. The first invocation of this method works as if
     * invoking it were equivalent to evaluating the expression:
     * <blockquote><pre>
     * {@link FileSystems#getDefault FileSystems.getDefault}().{@link FileSystem#getPath getPath}(this.{@link #getPath getPath}());
     * </pre></blockquote>
     * Subsequent invocations of this method return the same {@code Path}.
     *
     * <p> If this abstract pathname is the empty abstract pathname then this
     * method returns a {@code Path} that may be used to access to the current
     * user directory.
     *
     * @return  A {@code Path} constructed from this abstract path. The resulting
     *          {@code Path} is associated with the {@link FileSystems#getDefault
     *          default-filesystem}.
     *
     * @throws  InvalidPathException
     *          If a {@code Path} object cannot be constructed from the abstract
     *          path (see {@link java.nio.file.FileSystem#getPath FileSystem.getPath})
     *
     * @since   1.7
     */
    public Path toPath() {
        if (filePath == null) {
            synchronized (this) {
                if (filePath == null) {
		    String path = getPath();
                    if (path.length() == 0) {
                        // assume default file system treats "." as current directory
                        filePath = Paths.get(".");
                    } else {
                        filePath = Paths.get(path);
                    }
                }
            }
        }
        return filePath;
    }
}
