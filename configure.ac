AC_PREREQ([2.61])
AC_INIT([icedtea], [1.4], [fitzsim@redhat.com])
AM_INIT_AUTOMAKE([tar-pax])
AC_CONFIG_FILES([Makefile])
AC_CANONICAL_HOST

AC_PREFIX_DEFAULT([bootstrap])

AC_PROG_CC
AC_PROG_CXX

FIND_TOOL([FIND], [find])
FIND_TOOL([MD5SUM], [md5sum])
FIND_TOOL([PATCH], [patch])
FIND_TOOL([UNZIP], [unzip])
FIND_TOOL([WGET], [wget])
FIND_TOOL([CHMOD], [chmod])
dnl OpenJDK's README-builds.html lists gawk as a build dependency so we
dnl check for it explicitly rather than using AC_PROG_AWK.
FIND_TOOL([GAWK], [gawk])

AC_ARG_WITH([icedtea-home],
            [AS_HELP_STRING([--with-icedtea-home],
                            [IcedTea home directory \
                             (default is /usr/lib/jvm/java-icedtea)])],
            [
              if test "x${withval}" == x
              then
                SYSTEM_ICEDTEA_DIR=/usr/lib/jvm/java-icedtea
              else
                SYSTEM_ICEDTEA_DIR=${withval}
              fi
            ],
            [
              SYSTEM_ICEDTEA_DIR=/usr/lib/jvm/java-icedtea
            ])
AC_SUBST(SYSTEM_ICEDTEA_DIR)

AC_ARG_WITH([ant-home],
            [AS_HELP_STRING([--with-ant-home],
                            [Ant home directory (default is /usr)])],
            [
              if test "x${withval}" == x
              then
                SYSTEM_ANT_DIR=/usr
              else
                SYSTEM_ANT_DIR=${withval}
              fi
            ],
            [
              SYSTEM_ANT_DIR=/usr
            ])
AC_SUBST(SYSTEM_ANT_DIR)

SET_ARCH_DIRS
JAVA=$SYSTEM_ICEDTEA_DIR/bin/java
AC_SUBST(JAVA)
JAVAC=${SYSTEM_ICEDTEA_DIR}/bin/javac
AC_SUBST(JAVAC)
JAVAH=${SYSTEM_ICEDTEA_DIR}/bin/javah
AC_SUBST(JAVAH)
JAR=${SYSTEM_ICEDTEA_DIR}/bin/jar
AC_SUBST(JAR)
RMIC=${SYSTEM_ICEDTEA_DIR}/bin/rmic
AC_SUBST(RMIC)
FIND_FREETYPE
WITH_OPENJDK_SRC_ZIP
WITH_OPENJDK_SRC_DIR
AC_CHECK_WITH_CACAO
ENABLE_FAST_BUILD

CHECK_HEADERS

AC_CHECK_LIB(Xtst, XTestQueryExtension,
	, [AC_MSG_ERROR("libXtst not found - try installing libXtst-devel")])

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
