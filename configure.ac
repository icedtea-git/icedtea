AC_PREREQ([2.61])
AC_INIT([icedtea], [1.5], [fitzsim@redhat.com])
AM_INIT_AUTOMAKE([tar-pax])
AC_CONFIG_FILES([Makefile])
AC_CANONICAL_HOST

AC_PREFIX_DEFAULT([bootstrap])

AC_PROG_CC
AC_PROG_CXX

FIND_TOOL([FIND], [find])
FIND_TOOL([PATCH], [patch])
FIND_TOOL([ZIP], [zip])
FIND_TOOL([UNZIP], [unzip])
FIND_TOOL([CHMOD], [chmod])
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
	      [ENABLE_PLUGIN="$val"], [ENABLE_PLUGIN='yes'])
AC_SUBST(ENABLE_PLUGIN)

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

SET_ARCH_DIRS
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
  AM_CONDITIONAL(GCC_OLD, test x != x)
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
FIND_FREETYPE
WITH_OPENJDK_SRC_ZIP
WITH_OPENJDK_SRC_DIR
AC_CHECK_WITH_CACAO
ENABLE_FAST_BUILD

if test "$ALT_OPENJDK_SRC_ZIP" = "not specified"; then
  FIND_TOOL([MERCURIAL], [hg])
fi

CHECK_HEADERS

AC_CHECK_LIB(Xtst, XTestQueryExtension,
	, [AC_MSG_ERROR("libXtst not found - try installing libXtst-devel")])
AC_CHECK_LIB(Xinerama, XineramaQueryExtension,
	, [AC_MSG_ERROR("libXinerama not found - try installing libXinerama-devel")])
AC_CHECK_LIB(png, main,
        , [AC_MSG_ERROR("libpng not found - try installing libpng-devel")])
AC_CHECK_LIB(jpeg, main,
        , [AC_MSG_ERROR("libjpeg not found - try installing libjpeg-devel")])
AC_CHECK_LIB(gif, main,
        , [AC_MSG_ERROR("giflib not found - try installing giflib-devel")])
AC_CHECK_LIB(z, main,
	, [AC_MSG_ERROR("zlib not found - try installing zlib-devel")])

dnl Check for plugin support headers and libraries.
if test "$ENABLE_PLUGIN" = "yes"; then
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
fi

AC_OUTPUT
