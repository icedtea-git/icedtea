AC_INIT([icedtea], [16.0.0pre00], [distro-pkg-dev@openjdk.java.net])
AM_INIT_AUTOMAKE([1.9 tar-pax foreign])
AM_MAINTAINER_MODE([enable])
AC_CONFIG_FILES([Makefile])

# Older automake doesn't generate these correctly
abs_top_builddir=`pwd -P`
AC_SUBST(abs_top_builddir)
abs_top_srcdir=`dirname $0`
cd $abs_top_srcdir
abs_top_srcdir=`pwd`
cd $abs_top_builddir
AC_SUBST(abs_top_srcdir)

IT_DETERMINE_VERSION
AC_CONFIG_FILES([hotspot.map fsg.sh])
AC_CONFIG_FILES([jconsole.desktop])
AC_CONFIG_FILES([policytool.desktop])

AC_CANONICAL_HOST

AC_PROG_CC
AC_PROG_CXX

IT_FIND_TOOLS([MAKE], [gmake make])
IT_FIND_TOOL([GZIP], [gzip])
IT_FIND_TOOLS([FIND], [gfind find])
IT_FIND_TOOLS([PATCH], [gpatch patch])
IT_FIND_TOOLS([TAR], [gtar tar])
IT_FIND_TOOL([CHMOD], [chmod])
IT_FIND_TOOLS([SHA256SUM], [gsha256sum sha256sum])
IT_FIND_TOOL([WGET], [wget])
IT_FIND_TOOL([ZIP], [zip])
IT_FIND_TOOL([UNZIP], [unzip])
IT_FIND_TOOL([CPIO], [cpio])
IT_FIND_TOOL([FILE], [file])
IT_FIND_TOOL([READLINK], [readlink])
IT_FIND_TOOLS([FASTJAR], [fastjar jar])
IT_FIND_TOOLS([SED],[gsed sed])
AC_CHECK_TOOL([LDD], [ldd])
dnl OpenJDK's README-builds.html lists gawk as a build dependency so we
dnl check for it explicitly rather than using AC_PROG_AWK.
IT_FIND_TOOL([GAWK], [gawk])
IT_CHECK_FOR_MERCURIAL
IT_OBTAIN_HG_REVISIONS
AC_PATH_TOOL([LSB_RELEASE],[lsb_release])
IT_CHECK_FOR_RMDIR
IT_WITH_HOTSPOT_BUILD
AC_PATH_TOOL([LINUX32],[linux32])
IT_CHECK_GCC_VERSION
IT_FIND_NUMBER_OF_PROCESSORS
IT_CP_SUPPORTS_REFLINK
IT_CAN_HARDLINK_TO_SOURCE_TREE
IT_DISABLE_TESTS
IT_DISABLE_HOTSPOT_TESTS
IT_DISABLE_LANGTOOLS_TESTS
IT_DISABLE_JDK_TESTS
IT_DISABLE_SYSTEMTAP_TESTS
IT_LOCATE_NSS

# Use xvfb-run if found to run gui tests (check-jdk).
AC_CHECK_PROG(XVFB_RUN_CMD, xvfb-run, [xvfb-run -a -e xvfb-errors], [])
AC_SUBST(XVFB_RUN_CMD)

AC_MSG_CHECKING([whether to build documentation])
AC_ARG_ENABLE([docs],
	      [AS_HELP_STRING([--disable-docs],
	      		      [Disable generation of documentation])],
	      [ENABLE_DOCS="${enableval}"], [ENABLE_DOCS='yes'])
AM_CONDITIONAL([ENABLE_DOCS], [test x$ENABLE_DOCS = xyes])
AC_MSG_RESULT(${ENABLE_DOCS})

IT_GET_PKGVERSION
IT_GET_LSB_DATA

IT_SET_ARCH_SETTINGS
IT_SET_OS_DIRS
OPENJDK_BUILD_DIR=openjdk.build
AC_SUBST(OPENJDK_BUILD_DIR)

IT_CHECK_NUMBER_OF_PARALLEL_JOBS
IT_CHECK_IF_BOOTSTRAPPING
IT_CHECK_IF_DOWNLOADING
IT_CHECK_FOR_JDK
IT_FIND_JAVA
IT_FIND_COMPILER
IT_FIND_JAVAH
IT_FIND_JAR
IT_FIND_RMIC
IT_FIND_NATIVE2ASCII
IT_USING_CACAO
if test "x$enable_bootstrap" = "xyes"; then
  IT_FIND_ECJ_JAR
fi
AC_CONFIG_FILES([javac], [chmod +x javac])
AC_CONFIG_FILES([javap], [chmod +x javap])
AC_CONFIG_FILES([javah], [chmod +x javah])

IT_CHECK_ENABLE_WARNINGS
IT_DIAMOND_CHECK

IT_ENABLE_OPENJDK_CHECKSUM
IT_ENABLE_HOTSPOT_CHECKSUM
IT_ENABLE_CORBA_CHECKSUM
IT_ENABLE_JAXP_CHECKSUM
IT_ENABLE_JAXWS_CHECKSUM
IT_ENABLE_JDK_CHECKSUM
IT_ENABLE_LANGTOOLS_CHECKSUM
IT_ENABLE_NASHORN_CHECKSUM
IT_WITH_ALT_JAR_BINARY

IT_ENABLE_CACAO
IT_WITH_CACAO_HOME
IT_WITH_CACAO_SRC_DIR
IT_ENABLE_CACAO_CHECKSUM
IT_ENABLE_JAMVM
IT_ENABLE_JAMVM_CHECKSUM

IT_DISABLE_OPTIMIZATIONS
IT_ENABLE_WERROR
IT_ENABLE_JAR_COMPRESSION
IT_ENABLE_ZERO_BUILD
IT_CHECK_ADDITIONAL_VMS
IT_ENABLE_NON_NSS_CURVES

IT_WITH_VERSION_SUFFIX
IT_ENABLE_HG
IT_WITH_CACERTS_FILE

IT_WITH_PAX
AC_CONFIG_FILES([pax-mark-vm], [chmod +x pax-mark-vm])

AC_PATH_TOOL([HG],[hg])
if test "x${enable_hg}" = "xyes"; then
  if test "x${HG}" = x; then
    AC_MSG_ERROR([Mercurial not found; this is required for a hg build])
  fi
fi
AC_SUBST([HG])
IT_WITH_HG_REVISION

dnl pkgconfig cannot be used to find these headers and libraries.
AC_CHECK_HEADERS([cups/cups.h cups/ppd.h],[]
	,[AC_MSG_ERROR("CUPS headers were not found -
	try installing cups-devel.")])
AC_CHECK_HEADERS([X11/X.h],[]
	,[AC_MSG_ERROR("xorg headers were not found -
	try installing xorg-x11-proto-devel.")])

IT_CHECK_FOR_ZLIB
IT_CHECK_FOR_JPEG
IT_CHECK_FOR_PNG
IT_CHECK_FOR_GIF
IT_CHECK_FOR_LCMS
AC_CONFIG_FILES([remove-intree-libraries.sh])

dnl Check for Xproto headers and libraries.
PKG_CHECK_MODULES(XPROTO, xproto,[XPROTO_FOUND=yes],[XPROTO_FOUND=no])
if test "x${XPROTO_FOUND}" = xno
then
  AC_MSG_ERROR([Could not find Xproto headers - \
  Try installing xorg-x11-proto-devel.])
fi
AC_SUBST(XPROTO_CFLAGS)
AC_SUBST(XPROTO_LIBS)

dnl Check for libXt headers and libraries.
PKG_CHECK_MODULES(XT, xt,[XT_FOUND=yes],[XT_FOUND=no])
if test "x${XT_FOUND}" = xno
then
  AC_MSG_ERROR([Could not find Xt - \
  Try installing libXt-devel.])
fi
AC_SUBST(XT_CFLAGS)
AC_SUBST(XT_LIBS)

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

PKG_CHECK_MODULES(XRENDER, xrender, [XRENDER_FOUND=yes], [XRENDER_FOUND=no])
if test "x${XRENDER_FOUND}" = xno
then
  AC_MSG_ERROR([Could not find Xrender extension - \
Try installing libXrender-devel.])
fi

PKG_CHECK_MODULES(XCOMPOSITE, xcomposite, [XCOMPOSITE_FOUND=yes], [XCOMPOSITE_FOUND=no])
if test "x${XCOMPOSITE_FOUND}" = xno
then AC_MSG_ERROR([Could not find Xcomposite extension.  Try installing libXcomposite-devel.])
fi
AC_SUBST(XCOMPOSITE_CFLAGS)
AC_SUBST(XCOMPOSITE_LIBS)

AC_CHECK_HEADER([sys/sdt.h], [SDT_H_FOUND='yes'],
                [SDT_H_FOUND='no';
                   AC_MSG_WARN([systemtap support needs sys/sdt.h header])])

AC_MSG_CHECKING([working sys/sdt.h and g++ support])
AC_LANG_PUSH([C++])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#include <sys/sdt.h>
class ProbeClass
{
private:
  int& ref;
  const char *name;

public:
  ProbeClass(int& v, const char *n) : ref(v), name(n)
  {
    DTRACE_PROBE2(_test_, cons, name, ref);
  }

  void method(int min)
  {
    DTRACE_PROBE3(_test_, meth, name, ref, min);
    ref -= min;
  }

  ~ProbeClass()
  {
    DTRACE_PROBE2(_test_, dest, name, ref);
  }
};
]],
[[
  int i = 64;
  DTRACE_PROBE1(_test_, call, i);
  ProbeClass inst = ProbeClass(i, "call");
  inst.method(24);
]])], [AC_MSG_RESULT([yes])], [SDT_H_FOUND='no'; AC_MSG_WARN([systemtap sdt.h or g++ too old])])
AC_LANG_POP([C++])
if test "x${SDT_H_FOUND}" = "xyes"; then
 AC_PATH_TOOL([PERL],[perl])
 if test x"${PERL}" = x ; then
   AC_MSG_WARN([Perl not found in PATH; SystemTap tests will not be able to run])
 fi
 AC_SUBST(PERL)
 AC_PATH_TOOL([STAP],[stap])
 if test x"${STAP}" = x ; then
   AC_MSG_WARN([stap not found in PATH; SystemTap tests will not be able to run])
 fi
 AC_SUBST(STAP)
fi

AM_CONDITIONAL([ENABLE_SYSTEMTAP], [test x$SDT_H_FOUND = xyes])

ABS_CLIENT_LIBJVM_SO="${prefix}/jre/lib/${INSTALL_ARCH_DIR}/client/libjvm.so"
ABS_SERVER_LIBJVM_SO="${prefix}/jre/lib/${INSTALL_ARCH_DIR}/server/libjvm.so"
AC_SUBST(ABS_CLIENT_LIBJVM_SO)
AC_SUBST(ABS_SERVER_LIBJVM_SO)
AC_CONFIG_FILES([tapset/hotspot.stp])
AC_CONFIG_FILES([tapset/hotspot_jni.stp])
AC_CONFIG_FILES([tapset/jstack.stp])
AC_CONFIG_FILES([tapset/hotspot_gc.stp])
AC_CONFIG_FILES([test/tapset/jstaptest.pl])

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

AC_ARG_WITH([llvm-config],
            [AS_HELP_STRING([--with-llvm-config],
                            [llvm-config binary (default is llvm-config)])],
            [
              if test "x${withval}" != x && test "x${withval}" != xyes && test "x${withval}" != xno; then
	        if which ${withval} >/dev/null 2>&1 && ${withval} --version >/dev/null 2>&1; then
		  LLVM_CONFIG=${withval}
		else
		  AC_MSG_ERROR([invalid argument for --with-llvm-config ($withval)])
		fi
              fi
            ])

if test "x${SHARK_BUILD_TRUE}" = x || test "x${ADD_SHARK_BUILD_TRUE}" = x; then
  if test "x${LLVM_CONFIG}" = x; then
    IT_FIND_TOOL([LLVM_CONFIG], [llvm-config])
  fi
  llvm_components="jit engine nativecodegen"
  LLVM_VERSION="`$LLVM_CONFIG --version`"
  dnl LLVM_CFLAGS
  LLVM_CFLAGS=
  for flag in $($LLVM_CONFIG --cxxflags $llvm_components); do
    if echo "$flag" | grep -q '^-[[ID]]'; then
      if test "$flag" != "-D_DEBUG"; then
        if test "x$LLVM_CFLAGS" != "x"; then
          LLVM_CFLAGS="$LLVM_CFLAGS "
        fi
        LLVM_CFLAGS="$LLVM_CFLAGS$flag"
      fi
    fi
  done
  LLVM_CFLAGS="$LLVM_CFLAGS -DSHARK_LLVM_VERSION=`echo $LLVM_VERSION | sed 's/\.//;s/svn.*//'`"
  dnl LLVM_LDFLAGS
  LLVM_LDFLAGS=
  for flag in $($LLVM_CONFIG --ldflags $llvm_components); do
    if echo "$flag" | grep -q '^-L'; then
      if test "x$LLVM_LDFLAGS" != "x"; then
        LLVM_LDFLAGS="$LLVM_LDFLAGS "
      fi
      LLVM_LDFLAGS="$LLVM_LDFLAGS$flag"
    fi
  done
  dnl LLVM_LIBS
  LIBS_SAVED=$LIBS
  LDFLAGS_SAVED=$LDFLAGS
  LDFLAGS=$LLVM_LDFLAGS
  AC_CHECK_LIB(LLVM-$LLVM_VERSION, LLVMGetNextInstruction,LLVM_SHARED_LIB=" -lLLVM-$LLVM_VERSION")
  LIBS=$LIBS_SAVED
  LDFLAGS=$LDFLAGS_SAVED
  LLVM_LIBS=
  for flag in $($LLVM_CONFIG --libs $llvm_components); do
    if test "x$LLVM_SHARED_LIB" != x && echo "$flag" | grep -q '^-lLLVM'; then
      continue
    fi
    if echo "$flag" | grep -q '^-l'; then
      if test "x$LLVM_LIBS" != "x"; then
        LLVM_LIBS="$LLVM_LIBS "
      fi
      LLVM_LIBS="$LLVM_LIBS$flag"
    fi
  done
  LLVM_LIBS="$LLVM_LIBS$LLVM_SHARED_LIB"
fi
AC_SUBST(LLVM_CFLAGS)
AC_SUBST(LLVM_LDFLAGS)
AC_SUBST(LLVM_LIBS)

# Arguments passed to configure.
AC_SUBST(CONFIGURE_ARGS)
CONFIGURE_ARGS="$ac_configure_args"

# Sanity check; make sure we have a JVM
if test "x$has_native_hotspot_port" = "xno" ; then
  if test "x${use_zero}" = "xno" ; then
    if test "x${ENABLE_CACAO}" = "xno" -a "x${ENABLE_JAMVM}" = "xno"; then
      AC_MSG_ERROR([No native HotSpot port available for ${host_cpu} and all alternatives disabled.])
    fi
  fi
fi

AC_OUTPUT
