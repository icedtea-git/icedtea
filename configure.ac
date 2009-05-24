AC_INIT([icedtea], [1.10-pre], [distro-pkg-dev@openjdk.java.net])
AM_INIT_AUTOMAKE([1.9 tar-pax foreign])
AC_CONFIG_FILES([Makefile])

# Older automake doesn't generate these correctly
abs_top_builddir=`pwd -P`
AC_SUBST(abs_top_builddir)
abs_top_srcdir=`dirname $0`
cd $abs_top_srcdir
abs_top_srcdir=`pwd`
cd $abs_top_builddir
AC_SUBST(abs_top_srcdir)

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
AC_PATH_TOOL([HG],[hg])
AC_SUBST([HG])
AC_PATH_TOOL([LSB_RELEASE],[lsb_release])
AC_CHECK_WITH_GCJ
AC_CHECK_WITH_HOTSPOT_BUILD
AC_PATH_TOOL([LINUX32],[linux32])
AC_CHECK_GCC_VERSION

AC_MSG_CHECKING([whether to build using an existing installation of IcedTea])
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
AM_CONDITIONAL(WITH_ICEDTEA, test "${with_icedtea}" = true)
AC_MSG_RESULT(${with_icedtea})

AC_MSG_CHECKING([for a NetBeans installation])
AC_ARG_WITH([netbeans-home],
           [AS_HELP_STRING([--with-netbeans-home],
                            [NetBeans home directory (default is /usr/share/netbeans)])],
            [
              if test "x${withval}" = x
              then
                SYSTEM_NETBEANS_DIR=/usr/share/netbeans
              else
                SYSTEM_NETBEANS_DIR=${withval}
              fi
            ],
            [
              SYSTEM_NETBEANS_DIR=/usr/share/netbeans
            ])
AC_MSG_RESULT([${SYSTEM_NETBEANS_DIR}])
AC_SUBST(SYSTEM_NETBEANS_DIR)

AC_MSG_CHECKING([for a NetBeans installation])
AC_ARG_WITH([netbeans-home],
           [AS_HELP_STRING([--with-netbeans-home],
                            [NetBeans home directory (default is /usr/share/netbeans)])],
            [
              if test "x${withval}" = x
              then
                SYSTEM_NETBEANS_DIR=/usr/share/netbeans
              else
                SYSTEM_NETBEANS_DIR=${withval}
              fi
            ],
            [
              SYSTEM_NETBEANS_DIR=/usr/share/netbeans
            ])
AC_MSG_RESULT([${SYSTEM_NETBEANS_DIR}])
AC_SUBST(SYSTEM_NETBEANS_DIR)

AC_MSG_CHECKING([whether to build using an existing installation of OpenJDK])
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
AM_CONDITIONAL(WITH_OPENJDK, test "${with_openjdk}" = true)
AC_MSG_RESULT(${with_openjdk})

AC_MSG_CHECKING(for Ant home directory)
AC_ARG_WITH([ant-home],
            [AS_HELP_STRING([--with-ant-home],
                            [Ant home directory (default is /usr/share/ant)])],
            [
              if test "x${withval}" = x
              then
                SYSTEM_ANT_DIR=/usr/share/ant
              else
                SYSTEM_ANT_DIR=${withval}
              fi
            ],
            [
              SYSTEM_ANT_DIR=/usr/share/ant
            ])
AC_MSG_RESULT(${SYSTEM_ANT_DIR})
AC_SUBST(SYSTEM_ANT_DIR)

AC_MSG_CHECKING(whether to build VisualVM)
AC_ARG_ENABLE([visualvm],
              [AS_HELP_STRING([--enable-visualvm],
                                         [Enable compilation of visualvm.])],
              [enable_visualvm="${enableval}"], [enable_visualvm="no"])
AM_CONDITIONAL(WITH_VISUALVM, test "x${enable_visualvm}" = "xyes")
AC_MSG_RESULT(${enable_visualvm})

AC_MSG_CHECKING(whether to build VisualVM)
AC_ARG_ENABLE([visualvm],
              [AS_HELP_STRING([--enable-visualvm],
                                         [Enable compilation of visualvm.])],
              [enable_visualvm="${enableval}"], [enable_visualvm="no"])
AM_CONDITIONAL(WITH_VISUALVM, test "x${enable_visualvm}" = "xyes")
AC_MSG_RESULT(${enable_visualvm})

AC_MSG_CHECKING([whether to build the LiveConnect plugin])
AC_ARG_ENABLE([liveconnect],
              [AS_HELP_STRING([--disable-liveconnect],
                              [Disable compilation of browser plugin])],
              [enable_plugin="${enableval}"], [enable_plugin="yes"])
AM_CONDITIONAL(ENABLE_PLUGIN, test "x${enable_plugin}" = "xyes")
AC_MSG_RESULT(${enable_plugin})

AC_MSG_CHECKING([whether to build the new experimental browser plugin based on npruntime])
AC_ARG_ENABLE([npplugin],
              [AS_HELP_STRING([--enable-npplugin],
                              [Enable compilation of browser plugin (automatically disables default plugin)])],
              [enable_npplugin="${enableval}"], [enable_npplugin="no"])
AM_CONDITIONAL(ENABLE_NPPLUGIN, test "x${enable_npplugin}" = "xyes")
AC_MSG_RESULT(${enable_npplugin})

AM_CONDITIONAL(JNLP_ABOUT_NEEDED, test "x${enable_npplugin}" = "xyes" -o "x${enable_plugin}" = "xyes")

AC_MSG_CHECKING([whether to include PulseAudio support])
AC_ARG_ENABLE([pulse-java],
              [AS_HELP_STRING([--enable-pulse-java],
                              [Enable pulse-java - an audio mixer spi that uses PulseAudio])],
              [enable_pulse_java="${enableval}"], [enable_pulse_java="no"])
AM_CONDITIONAL(ENABLE_PULSE_JAVA, test "x${enable_pulse_java}" = "xyes")
AC_MSG_RESULT(${enable_pulse_java})

AC_MSG_CHECKING([whether to build documentation])
AC_ARG_ENABLE([docs],
	      [AS_HELP_STRING([--disable-docs],
	      		      [Disable generation of documentation])],
	      [ENABLE_DOCS="${enableval}"], [ENABLE_DOCS='yes'])
AM_CONDITIONAL([ENABLE_DOCS], [test x$ENABLE_DOCS = xyes])
AC_MSG_RESULT(${ENABLE_DOCS})

AC_MSG_CHECKING([whether to include the XRender pipeline])
AC_ARG_ENABLE([xrender],
	      [AS_HELP_STRING([--disable-xrender],
	      		      [Disable inclusion of xrender pipeline])],
	      [ENABLE_XRENDER="${enableval}"], [ENABLE_XRENDER='yes'])
AM_CONDITIONAL([ENABLE_XRENDER], [test x$ENABLE_XRENDER = xyes])
AC_MSG_RESULT(${ENABLE_XRENDER})

AC_MSG_CHECKING(whether to include NIO2 support)
AC_ARG_ENABLE([nio2],
	      [AS_HELP_STRING([--disable-nio2],
	      		      [Disable inclusion of backported NIO2])],
	      [ENABLE_NIO2="${enableval}"], [ENABLE_NIO2='yes'])
AM_CONDITIONAL([ENABLE_NIO2], [test x$ENABLE_NIO2 = xyes])
AC_MSG_RESULT(${ENABLE_NIO2})

AC_MSG_CHECKING([whether to include SystemTap tracing support])
AC_ARG_ENABLE([systemtap],
	      [AS_HELP_STRING([--enable-systemtap],
	      		      [Enable inclusion of SystemTap trace support])],
	      [ENABLE_SYSTEMTAP="${enableval}"], [ENABLE_SYSTEMTAP='no'])
AM_CONDITIONAL([ENABLE_SYSTEMTAP], [test x$ENABLE_SYSTEMTAP = xyes])
AC_MSG_RESULT(${ENABLE_SYSTEMTAP})

AC_MSG_CHECKING([how many parallel build jobs to execute])
AC_ARG_WITH([parallel-jobs],
	[AS_HELP_STRING([--with-parallel-jobs],
			[build IcedTea using the specified number of parallel jobs])],
	[
          if test "x${withval}" = x
          then
            PARALLEL_JOBS=2
          else
            PARALLEL_JOBS=${withval}
          fi
        ],
        [
          PARALLEL_JOBS=2
        ])
AC_MSG_RESULT(${PARALLEL_JOBS})
AC_SUBST(PARALLEL_JOBS)

AC_ARG_WITH([pkgversion],
        [AS_HELP_STRING([--with-pkgversion=PKG],
                        [Use PKG in the version string in addition to "IcedTea"])],
        [case "$withval" in
          yes) AC_MSG_ERROR([package version not specified]) ;;
          no)  PKGVERSION= ;;
          *)   PKGVERSION="$withval" ;;
         esac],
        [PKGVERSION=])
AC_SUBST(PKGVERSION)

if test -n "$LSB_RELEASE"; then
  lsb_info="$($LSB_RELEASE -ds | sed 's/^"//;s/"$//')"
  if test -n "$PKGVERSION"; then
    DIST_ID="$lsb_info, package $PKGVERSION"
  else
    DIST_ID="Built on $lsb_info"
  fi
else
  DIST_ID="Custom build ($(date))"
fi
AC_SUBST(DIST_ID)

SET_ARCH_DIRS
SET_OS_DIRS

if test "${with_openjdk}" = true
then
  AC_CHECK_FOR_OPENJDK
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
if test "${with_icedtea}" = true
then
  AC_CHECK_FOR_ICEDTEA
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
  AC_CHECK_FOR_GCJ_JDK
  FIND_JAVA
  FIND_JAVAC
  FIND_JAVAH
  FIND_JAR
  FIND_RMIC
  FIND_ECJ_JAR
  FIND_XALAN2_JAR
  FIND_XALAN2_SERIALIZER_JAR
  FIND_XERCES2_JAR
  AC_CONFIG_FILES([javac], [chmod +x javac])
  AC_CONFIG_FILES([javap], [chmod +x javap])
fi
fi
IT_JAVAH
FIND_RHINO_JAR
WITH_OPENJDK_SRC_ZIP
WITH_HOTSPOT_SRC_ZIP
WITH_CORBA_SRC_ZIP
WITH_JAXP_SRC_ZIP
WITH_JAXWS_SRC_ZIP
WITH_JDK_SRC_ZIP
WITH_LANGTOOLS_SRC_ZIP
WITH_OPENJDK_SRC_DIR
WITH_ALT_JAR_BINARY
AC_CHECK_ENABLE_CACAO
AC_CHECK_WITH_CACAO_HOME
AC_CHECK_WITH_CACAO_SRC_ZIP
AC_CHECK_ENABLE_NIMBUS
AC_CHECK_ENABLE_NIMBUS_GENERATION
ENABLE_OPTIMIZATIONS
SET_SHARK_BUILD
ENABLE_ZERO_BUILD
WITH_VERSION_SUFFIX
WITH_PROJECT
ENABLE_HG

AC_PATH_TOOL([HG],[hg])
if test "x${enable_hg}" = "xyes"; then
  if test "x${HG}" = x; then
    AC_MSG_ERROR([Mercurial not found; this is required for a hg build])
  fi
fi
AC_SUBST([HG])
AC_CHECK_WITH_HG_REVISION

AC_PATH_TOOL([GCJ],[gcj])
AC_SUBST([GCJ])

if test "x${enable_visualvm}" = "xyes"
then
  FIND_NETBEANS
fi

if test "x${enable_visualvm}" = "xyes"
then
  FIND_NETBEANS
fi

if test "x${enable_pulse_java}" = "xyes"
then
  FIND_PULSEAUDIO
fi

if test "x${ENABLE_NIMBUS}" = "xyes"
then
  if test "x${ENABLE_NIMBUS_GENERATION}" = "xyes"
  then
    FIND_BCEL_JAR
    FIND_XPP3_JAR
    FIND_JIBX_DIR
  fi
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

if test "x${ENABLE_XRENDER}" = "xyes"
then
  PKG_CHECK_MODULES(XRENDER, xrender, [XRENDER_FOUND=yes], [XRENDER_FOUND=no])
  if test "x${XRENDER_FOUND}" = xno
  then
    AC_MSG_ERROR([Could not find Xrender extension - \
Try installing libXrender-devel or configure --disable-xrender.])
  fi
fi

if test "x${ENABLE_SYSTEMTAP}" = xyes; then
AC_CHECK_HEADER([sys/sdt.h], [SDT_H_FOUND='yes'],
                [SDT_H_FOUND='no';
                   AC_MSG_ERROR([systemtap support needs sys/sdt.h header])])

  AC_MSG_CHECKING([for absolute java home install dir])
  AC_ARG_WITH([abs-install-dir],
	      [AS_HELP_STRING([--with-abs-install-dir], 
			      [The absolute path where the j2sdk-image dir will be installed])],
	      [if test "x${withval}" = x; then
		 ABS_JAVA_HOME_DIR="`pwd`/openjdk/build/${BUILD_OS_DIR}-${BUILD_ARCH_DIR}/j2sdk-image"
	       else
		 ABS_JAVA_HOME_DIR="${withval}"
	       fi], [ABS_JAVA_HOME_DIR="`pwd`/openjdk/build/${BUILD_OS_DIR}-${BUILD_ARCH_DIR}/j2sdk-image"])
  AC_MSG_RESULT([${ABS_JAVA_HOME_DIR}])
  ABS_CLIENT_LIBJVM_SO="${ABS_JAVA_HOME_DIR}/jre/lib/${INSTALL_ARCH_DIR}/client/libjvm.so"
  ABS_SERVER_LIBJVM_SO="${ABS_JAVA_HOME_DIR}/jre/lib/${INSTALL_ARCH_DIR}/server/libjvm.so"
  AC_SUBST(ABS_JAVA_HOME_DIR)
  AC_SUBST(ABS_CLIENT_LIBJVM_SO)
  AC_SUBST(ABS_SERVER_LIBJVM_SO)
  AC_CONFIG_FILES([tapset/hotspot.stp])
fi

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

dnl Check for alsa headers and libraries (only required for Linux).
if test "x${BUILD_OS_DIR}" = "xlinux"
then
  PKG_CHECK_MODULES(ALSA, alsa,[ALSA_FOUND=yes],[ALSA_FOUND=no])
  if test "x${ALSA_FOUND}" = xno
  then
    AC_MSG_ERROR([Could not find alsa - \
    Try installing alsa-lib-devel.])
  fi
AC_SUBST(ALSA_CFLAGS)
AC_SUBST(ALSA_LIBS)
fi

if test "x${enable_pulse_java}" = "xyes"
then
  dnl Check for pulseaudio libraries.
  PKG_CHECK_MODULES(LIBPULSE,[libpulse >= 0.9.11],[LIBPULSE_FOUND=yes]
    ,[LIBPULSE_FOUND=no])
  if test "x${LIBPULSE_FOUND}" = xno
  then
    AC_MSG_ERROR([Could not find pulseaudio>=0.9.11 libraries - \
    Try installing pulseaudio-libs-devel>=0.9.11.])
  fi
  AC_SUBST(LIBPULSE_CFLAGS)
  AC_SUBST(LIBPULSE_LIBS)
fi

dnl Check for plugin support headers and libraries.
dnl FIXME: use unstable
if test "x${enable_plugin}" = "xyes"
then
  PKG_CHECK_MODULES(XULRUNNER, \
    nspr mozilla-js mozilla-plugin libxul-unstable >= 1.9)
  PKG_CHECK_MODULES(GTK, gtk+-2.0)
  IT_XULRUNNER_VERSION
fi
if test "x${enable_npplugin}" = "xyes"
then
  PKG_CHECK_MODULES(MOZILLA, \
    mozilla-plugin libxul-unstable)
  PKG_CHECK_MODULES(GTK, gtk+-2.0)
  PKG_CHECK_MODULES(GLIB, glib-2.0)

  AC_SUBST(MOZILLA_CFLAGS)
  AC_SUBST(MOZILLA_LIBS)
  AC_SUBST(GLIB_CFLAGS)
  AC_SUBST(GLIB_LIBS)
  AC_SUBST(GTK_CFLAGS)
  AC_SUBST(GTK_LIBS)
fi


AC_MSG_CHECKING(for --with-additional-vms)
AC_ARG_WITH(additional-vms,
            AC_HELP_STRING([--with-additional-vms=vm-list], [build additional virtual machines. Valid value is a comma separated string with the backend names `cacao', `zero' and `shark'.]),
[
if test x$with_additional_vms != x
then
  for vm in `echo $with_additional_vms | sed 's/,/ /g'`; do
    case "x$vm" in
      xcacao) add_vm_cacao=yes;;
      xzero)  add_vm_zero=yes;;
      xshark) add_vm_shark=yes;;
      *) AC_MSG_ERROR([proper usage is --with-additional-vms=vm1,vm2,...])
    esac
  done
fi])
AC_MSG_RESULT($with_additional_vms)

AM_CONDITIONAL(ADD_CACAO_BUILD, test x$add_vm_cacao != x)
AM_CONDITIONAL(ADD_ZERO_BUILD,  test x$add_vm_zero  != x || test x$add_vm_shark != x)
AM_CONDITIONAL(ADD_SHARK_BUILD, test x$add_vm_shark != x)
AM_CONDITIONAL(BUILD_CACAO, test x$add_vm_cacao != x || test "x${WITH_CACAO}" = xyes)

if test "x${WITH_CACAO}" = xyes && test "x${ADD_CACAO_BUILD_TRUE}" = x; then
  AC_MSG_ERROR([additional vm is the default vm])
fi
if test "x${ZERO_BUILD_TRUE}" = x && test "x${ADD_ZERO_BUILD_TRUE}" = x; then
  AC_MSG_ERROR([additional vm is the default vm])
fi
if test "x${USE_SYSTEM_CACAO_TRUE}" = x; then
  AC_MSG_ERROR([cannot build with system cacao as additional vm])
fi
if test "x${ADD_ZERO_BUILD_TRUE}" = x && test "x${abs_top_builddir}" = "x${abs_top_srcdir}"; then
  AC_MSG_ERROR([build of additional zero/shark VM requires build with srcdir != builddir])
fi

if test "x${ZERO_BUILD_TRUE}" = x || test "x${ADD_ZERO_BUILD_TRUE}" = x; then
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

if test "x${SHARK_BUILD_TRUE}" = x || test "x${ADD_SHARK_BUILD_TRUE}" = x; then
  FIND_TOOL([LLVM_CONFIG], [llvm-config])
  llvm_components="engine nativecodegen"
  LLVM_CFLAGS=`$LLVM_CONFIG --cflags $llvm_components | \
    sed -e 's/-O.//g' | sed -e 's/-fomit-frame-pointer//g' | \
    sed -e 's/-D_DEBUG//g'`
  LLVM_CFLAGS="$LLVM_CFLAGS -DSHARK_LLVM_VERSION=`$LLVM_CONFIG --version | sed 's/\.//;s/svn.*//'`"
  LLVM_LDFLAGS=`$LLVM_CONFIG --ldflags $llvm_components`
  LLVM_LIBS=`$LLVM_CONFIG --libs $llvm_components`
fi
AC_SUBST(LLVM_CFLAGS)
AC_SUBST(LLVM_LDFLAGS)
AC_SUBST(LLVM_LIBS)

# Arguments passed to configure.
AC_SUBST(CONFIGURE_ARGS)
CONFIGURE_ARGS="$ac_configure_args"

AC_OUTPUT
