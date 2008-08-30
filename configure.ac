AC_PREREQ([2.61])
AC_INIT([icedtea], [1.8-pre], [distro-pkg-dev@openjdk.java.net])
AM_INIT_AUTOMAKE([tar-pax])
AC_CONFIG_FILES([Makefile])
AC_CANONICAL_HOST

AC_PREFIX_DEFAULT([bootstrap])

AC_PROG_CC
AC_PROG_CXX

FIND_TOOL([MAKE], [make])
FIND_TOOL([GZIP], [gzip])
FIND_TOOL([ANT], [ant])
FIND_TOOL([FIND], [find])
FIND_TOOL([PATCH], [patch])
FIND_TOOL([TAR], [tar])
FIND_TOOL([CHMOD], [chmod])
FIND_TOOL([MD5SUM], [md5sum])
FIND_TOOL([WGET], [wget])
FIND_TOOL([ZIP], [zip])
FIND_TOOL([UNZIP], [unzip])
FIND_TOOL([CPIO], [cpio])
FIND_TOOL([FILE], [file])
AC_CHECK_TOOLS([FASTJAR], [fastjar jar])
if test "x$FASTJAR" = x; then
	AC_MSG_ERROR([Can't find fastjar or jar])
fi
AC_SUBST([FASTJAR])
dnl OpenJDK's README-builds.html lists gawk as a build dependency so we
dnl check for it explicitly rather than using AC_PROG_AWK.
FIND_TOOL([GAWK], [gawk])

AC_ARG_WITH([gcj-home],
	    [AS_HELP_STRING([--with-gcj-home],
                            [gcj home directory \
                             (default is /usr/lib/jvm/java-gcj)])],
            [
              if test "x${withval}" == x
              then
                SYSTEM_GCJ_DIR=/usr/lib/jvm/java-gcj
              else
                SYSTEM_GCJ_DIR=${withval}
              fi
            ],
            [
              SYSTEM_GCJ_DIR=/usr/lib/jvm/java-gcj
            ])
AC_SUBST(SYSTEM_GCJ_DIR)

AC_ARG_WITH([openjdk-home],
            [AS_HELP_STRING([--with-openjdk-home],
                            [OpenJDK home directory \
                             (default is /usr/lib/jvm/java-openjdk)])],
            [
              if test "x${withval}" == x
              then
                SYSTEM_OPENJDK_DIR=/usr/lib/jvm/java-openjdk
              else
                SYSTEM_OPENJDK_DIR=${withval}
              fi
            ],
            [
              SYSTEM_OPENJDK_DIR=/usr/lib/jvm/java-openjdk
            ])
AC_SUBST(SYSTEM_OPENJDK_DIR)

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
                            [Ant home directory (default is /usr/share/ant)])],
            [
              if test "x${withval}" == x
              then
                SYSTEM_ANT_DIR=/usr/share/ant
              else
                SYSTEM_ANT_DIR=${withval}
              fi
            ],
            [
              SYSTEM_ANT_DIR=/usr/share/ant
            ])
AC_SUBST(SYSTEM_ANT_DIR)

AC_ARG_ENABLE([gcjwebplugin],
              [AS_HELP_STRING([--disable-gcjwebplugin],
                              [Disable compilation of browser plugin])],
              [enable_plugin="$val"], [enable_plugin="yes"])
AM_CONDITIONAL(ENABLE_PLUGIN, test "x${enable_plugin}" = "xyes")

AC_ARG_ENABLE([liveconnect],
              [AS_HELP_STRING([--enable-liveconnect],
                              [Enable experimental LiveConnect plugin])],
              [enable_liveconnect="yes"], [enable_liveconnect="no"])
AM_CONDITIONAL(ENABLE_LIVECONNECT, test "x${enable_liveconnect}" = "xyes")

AC_ARG_ENABLE([docs],
	      [AS_HELP_STRING([--disable-docs],
	      		      [Disable generation of documentation])],
	      [ENABLE_DOCS="${enableval}"], [ENABLE_DOCS='yes'])
AM_CONDITIONAL([ENABLE_DOCS], [test x$ENABLE_DOCS = xyes])

AC_ARG_WITH([icedtea],
        [AS_HELP_STRING([--with-icedtea],
                        [build IcedTea with system-installed IcedTea])],
        [
          if test "x${withval}" != xno
          then
            with_icedtea=true
          else
            with_icedtea=false
          fi
        ],
        [
          with_icedtea=false
        ])
AM_CONDITIONAL(WITH_ICEDTEA, test "${with_icedtea}" == true)

AC_ARG_WITH([openjdk],
        [AS_HELP_STRING([--with-openjdk],
                        [build IcedTea with system-installed OpenJDK])],
        [
          if test "x${withval}" != xno
          then
            with_openjdk=true
          else
            with_openjdk=false
          fi
        ],
        [
          with_openjdk=false
        ])
AM_CONDITIONAL(WITH_OPENJDK, test "${with_openjdk}" == true)

AC_ARG_WITH([parallel-jobs],
	[AS_HELP_STRING([--with-parallel-jobs],
			[build IcedTea using the specified number of parallel jobs])],
	[
          if test "x${withval}" == x
          then
            PARALLEL_JOBS=2
          else
            PARALLEL_JOBS=${withval}
          fi
        ],
        [
          PARALLEL_JOBS=2
        ])
AC_SUBST(PARALLEL_JOBS)

SET_ARCH_DIRS
if test "${with_openjdk}" == true
then
  JAVA=$SYSTEM_OPENJDK_DIR/bin/java
  AC_SUBST(JAVA)
  JAVAC=${SYSTEM_OPENJDK_DIR}/bin/javac
  AC_SUBST(JAVAC)
  JAVAH=${SYSTEM_OPENJDK_DIR}/bin/javah
  AC_SUBST(JAVAH)
  JAR=${SYSTEM_OPENJDK_DIR}/bin/jar
  AC_SUBST(JAR)
  RMIC=${SYSTEM_OPENJDK_DIR}/bin/rmic
  AC_SUBST(RMIC)
else
if test "${with_icedtea}" == true
then
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
else
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
  AC_CONFIG_FILES([javac], [chmod +x javac])
  AC_CONFIG_FILES([javap], [chmod +x javap])
fi
fi
FIND_RHINO_JAR
WITH_OPENJDK_SRC_ZIP
WITH_OPENJDK_SRC_DIR
AC_CHECK_WITH_CACAO
AC_CHECK_WITH_CACAO_HOME
AC_CHECK_WITH_CACAO_SRC_ZIP
ENABLE_OPTIMIZATIONS
ENABLE_ZERO_BUILD
SET_CORE_OR_SHARK_BUILD
ENABLE_HG
WITH_PROJECT

if test "x${enable_hg}" = "xyes"; then
  FIND_TOOL([MERCURIAL],[hg])
fi

dnl pkgconfig cannot be used to find these headers and libraries.
AC_CHECK_HEADERS([cups/cups.h cups/ppd.h],[]
	,[AC_MSG_ERROR("CUPS headers were not found -
	try installing cups-devel.")])
AC_CHECK_HEADERS([X11/X.h],[]
	,[AC_MSG_ERROR("xorg headers were not found -
	try installing xorg-x11-proto-devel.")])
AC_CHECK_LIB(jpeg, main,
        , [AC_MSG_ERROR("libjpeg not found - try installing libjpeg-devel")])
AC_CHECK_LIB(gif, main,
        , [AC_MSG_ERROR("giflib not found - try installing giflib-devel")])
AC_CHECK_LIB(z, main,
	, [AC_MSG_ERROR("zlib not found - try installing zlib-devel")])

dnl Check for Xproto headers and libraries.
PKG_CHECK_MODULES(XPROTO, xproto,[XPROTO_FOUND=yes],[XPROTO_FOUND=no])
if test "x${XPROTO_FOUND}" = xno
then
  AC_MSG_ERROR([Could not find Xproto headers - \
  Try installing xorg-x11-proto-devel.])
fi
AC_SUBST(XT_CFLAGS)
AC_SUBST(XT_LIBS)

dnl Check for libXt headers and libraries.
PKG_CHECK_MODULES(XT, xt,[XT_FOUND=yes],[XT_FOUND=no])
if test "x${XT_FOUND}" = xno
then
  AC_MSG_ERROR([Could not find Xt - \
  Try installing libXt-devel.])
fi
AC_SUBST(XT_CFLAGS)
AC_SUBST(XT_LIBS)

dnl Check for libXp headers and libraries.
PKG_CHECK_MODULES(XP, xp,[XP_FOUND=yes],[XP_FOUND=no])
if test "x${XP_FOUND}" = xno
then
  AC_MSG_ERROR([Could not find Xp - \
  Try installing libXp-devel.])
fi
AC_SUBST(XP_CFLAGS)
AC_SUBST(XP_LIBS)

dnl Check for libX11 headers and libraries.
PKG_CHECK_MODULES(X11, x11,[X11_FOUND=yes],[X11_FOUND=no])
if test "x${X11_FOUND}" = xno
then
  AC_MSG_ERROR([Could not find x11 - \
  Try installing libX11-devel.])
fi
AC_SUBST(X11_CFLAGS)
AC_SUBST(X11_LIBS)

dnl Check for libXinerama headers and libraries.
PKG_CHECK_MODULES(XINERAMA, xinerama,[XINERAMA_FOUND=yes]
	,[XINERAMA_FOUND=no])
if test "x${XINERAMA_FOUND}" = xno
then
  AC_MSG_ERROR([Could not find Xinerama - \
  Try installing libXinerama-devel.])
fi
AC_SUBST(XINERAMA_CFLAGS)
AC_SUBST(XINERAMA_LIBS)

dnl Check for libpng headers and libraries.
PKG_CHECK_MODULES(LIBPNG, libpng,[LIBPNG_FOUND=yes]
	,[LIBPNG_FOUND=no])
if test "x${LIBPNG_FOUND}" = xno
then
  AC_MSG_ERROR([Could not find libpng - \
  Try installing libpng-devel.])
fi
AC_SUBST(LIBPNG_CFLAGS)
AC_SUBST(LIBPNG_LIBS)

dnl Check for libXtst headers and libraries.
PKG_CHECK_MODULES(XTST, xtst,[XTST_FOUND=yes],[XTST_FOUND=no])
if test "x${XTST_FOUND}" = xno
then
  AC_MSG_ERROR([Could not find Xtst - \
  Try installing libXtst-devel.])
fi
AC_SUBST(XTST_CFLAGS)
AC_SUBST(XTST_LIBS)

dnl Check for freetype2 headers and libraries.
PKG_CHECK_MODULES(FREETYPE2, freetype2,[FREETYPE2_FOUND=yes]
	,[FREETYPE2_FOUND=no])
if test "x${FREETYPE2_FOUND}" = xno
then
  AC_MSG_ERROR([Could not find freetype2 - \
  Try installing freetype2-devel.])
fi
AC_SUBST(FREETYPE2_CFLAGS)
AC_SUBST(FREETYPE2_LIBS)

dnl Check for alsa headers and libraries.
PKG_CHECK_MODULES(ALSA, alsa,[ALSA_FOUND=yes],[ALSA_FOUND=no])
if test "x${ALSA_FOUND}" = xno
then
  AC_MSG_ERROR([Could not find alsa - \
  Try installing alsa-lib-devel.])
fi
AC_SUBST(ALSA_CFLAGS)
AC_SUBST(ALSA_LIBS)

dnl Check for plugin support headers and libraries.
dnl FIXME: use unstable
if test "x${enable_liveconnect}" = "xyes"
then
  PKG_CHECK_MODULES(XULRUNNER, \
    nspr mozilla-js mozilla-plugin libxul-unstable >= 1.9)
  PKG_CHECK_MODULES(GTK, gtk+-2.0)
else
  if test "x${enable_plugin}" = "xyes"
  then
    PKG_CHECK_MODULES(MOZILLA, mozilla-plugin libxul-unstable, \
      [MOZILLA_FOUND=yes], [MOZILLA_FOUND=no])
    if test "x${MOZILLA_FOUND}" = xno
    then
      PKG_CHECK_MODULES(MOZILLA, mozilla-plugin, \
        [MOZILLA_FOUND=yes], [MOZILLA_FOUND=no])
    fi
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
  fi
fi

if test "x${ZERO_BUILD_TRUE}" = x; then
  dnl Check for libffi headers and libraries.
  PKG_CHECK_MODULES(LIBFFI, libffi,[LIBFFI_FOUND=yes],[LIBFFI_FOUND=no])
  if test "x${LIBFFI_FOUND}" = xno
  then
    LIBFFI_FOUND=
    AC_CHECK_HEADER([ffi.h],[LIBFFI_CFLAGS=],[LIBFFI_FOUND=no])
    AC_SEARCH_LIBS([ffi_call], [ffi],[LIBFFI_LIBS=-lffi],[LIBFFI_FOUND=no])
    if test "x${LIBFFI_FOUND}" = xno
    then
      AC_MSG_ERROR([Could not find libffi headers - \
      Try installing libffi-devel, libffi-dev or libffi4-dev.])
    fi
  fi
fi
AC_SUBST(LIBFFI_CFLAGS)
AC_SUBST(LIBFFI_LIBS)

if test "x${SHARK_BUILD_TRUE}" = x; then
  FIND_TOOL([LLVM_CONFIG], [llvm-config])
  llvm_components="engine bitwriter"
  LLVM_CFLAGS=`$LLVM_CONFIG --cflags $llvm_components | \
    sed -e 's/-O.//g' | sed -e 's/-fomit-frame-pointer//g'`
  LLVM_LDFLAGS=`$LLVM_CONFIG --ldflags $llvm_components`
  LLVM_LIBS=`$LLVM_CONFIG --libs $llvm_components`
fi
AC_SUBST(LLVM_CFLAGS)
AC_SUBST(LLVM_LDFLAGS)
AC_SUBST(LLVM_LIBS)

AC_OUTPUT
