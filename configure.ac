AC_PREREQ([2.61])
AC_INIT([icedtea], [1.4], [fitzsim@redhat.com])
AM_INIT_AUTOMAKE([tar-pax])
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([javac], [chmod +x javac])
AC_CONFIG_FILES([javap], [chmod +x javap])
AC_CANONICAL_HOST

AC_PREFIX_DEFAULT([bootstrap])

AC_PROG_CC
AC_PROG_CXX

FIND_TOOL([FIND], [find])
FIND_TOOL([MD5SUM], [md5sum])
FIND_TOOL([PATCH], [patch])
FIND_TOOL([UNZIP], [unzip])
FIND_TOOL([WGET], [wget])
dnl OpenJDK's README-builds.html lists gawk as a build dependency so we
dnl check for it explicitly rather than using AC_PROG_AWK.
FIND_TOOL([GAWK], [gawk])

SET_ARCH_DIRS
FIND_JAVA
FIND_JAVAC
FIND_JAVAH
FIND_JAR
FIND_RMIC
FIND_ECJ_JAR
FIND_LIBGCJ_JAR
FIND_XALAN2_JAR
FIND_XALAN2_SERIALIZER_JAR
FIND_XERCES2_JAR
FIND_FREETYPE
WITH_OPENJDK_SRC_ZIP
WITH_OPENJDK_SRC_DIR
ENABLE_FAST_BUILD

CHECK_HEADERS

dnl Check for plugin support headers and libraries.
PKG_CHECK_MODULES(MOZILLA, mozilla-plugin, [MOZILLA_FOUND=yes], \
  [MOZILLA_FOUND=no])
if test "x${MOZILLA_FOUND}" = xno
then
  PKG_CHECK_MODULES(MOZILLA, firefox-plugin firefox-xpcom, \
    [MOZILLA_FOUND=yes], [MOZILLA_FOUND=no])
fi
if test "x${MOZILLA_FOUND}" = xno
then
  PKG_CHECK_MODULES(MOZILLA, xulrunner-plugin xulrunner-xpcom, \
    [MOZILLA_FOUND=yes], [MOZILLA_FOUND=no])
fi
if test "x${MOZILLA_FOUND}" = xno
then
  PKG_CHECK_MODULES(MOZILLA, mozilla-firefox-plugin mozilla-firefox-xpcom, \
    [MOZILLA_FOUND=yes], [MOZILLA_FOUND=no])
fi
if test "x${MOZILLA_FOUND}" = xno
then
  PKG_CHECK_MODULES(MOZILLA, seamonkey-plugin seamonkey-xpcom, \
    [MOZILLA_FOUND=yes], [MOZILLA_FOUND=no])
fi
if test "x${MOZILLA_FOUND}" = xno
then
  PKG_CHECK_MODULES(MOZILLA, iceape-plugin iceape-xpcom, \
    [MOZILLA_FOUND=yes], [MOZILLA_FOUND=no])
fi
if test "x${MOZILLA_FOUND}" = xno
then
  AC_MSG_ERROR([Could not find plugin support headers and libraries.])
fi

PKG_CHECK_MODULES(GLIB, glib-2.0)
PKG_CHECK_MODULES(GTK, gtk+-2.0 gthread-2.0 gdk-pixbuf-2.0)

AC_SUBST(MOZILLA_CFLAGS)
AC_SUBST(MOZILLA_LIBS)
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

AC_OUTPUT
