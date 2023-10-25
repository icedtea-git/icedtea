AC_DEFUN([IT_SET_ARCH_SETTINGS],
[
  case "${target_cpu}" in
    x86_64)
      BUILD_ARCH_DIR=amd64
      INSTALL_ARCH_DIR=amd64
      JRE_ARCH_DIR=amd64
      RPM_ARCH=x86_64
      SYSTEMTAP_ARCH_DIR=x86_64
      ARCHFLAG="-m64"
      ;;
    i?86)
      BUILD_ARCH_DIR=i586
      INSTALL_ARCH_DIR=i386
      JRE_ARCH_DIR=i386
      RPM_ARCH=i686
      SYSTEMTAP_ARCH_DIR=i386
      ARCH_PREFIX=${LINUX32}
      ARCHFLAG="-m32"
      ;;
    alpha*)
      BUILD_ARCH_DIR=alpha
      INSTALL_ARCH_DIR=alpha
      JRE_ARCH_DIR=alpha
      SYSTEMTAP_ARCH_DIR=alpha
      ;;
    arm64|aarch64)
      BUILD_ARCH_DIR=aarch64
      INSTALL_ARCH_DIR=aarch64
      JRE_ARCH_DIR=aarch64
      RPM_ARCH=aarch64
      SYSTEMTAP_ARCH_DIR=arm64
      ARCHFLAG="-D_LITTLE_ENDIAN"
      ;;
    arm*)
      BUILD_ARCH_DIR=aarch32
      INSTALL_ARCH_DIR=aarch32
      JRE_ARCH_DIR=aarch32
      RPM_ARCH=armv7hl
      SYSTEMTAP_ARCH_DIR=arm
      ARCHFLAG="-D_LITTLE_ENDIAN"
      ;;
    mips)
      BUILD_ARCH_DIR=mips
      INSTALL_ARCH_DIR=mips
      JRE_ARCH_DIR=mips
      SYSTEMTAP_ARCH_DIR=mips
       ;;
    mipsel)
      BUILD_ARCH_DIR=mipsel
      INSTALL_ARCH_DIR=mipsel
      JRE_ARCH_DIR=mipsel
      SYSTEMTAP_ARCH_DIR=mips
       ;;
    powerpc)
      BUILD_ARCH_DIR=ppc
      INSTALL_ARCH_DIR=ppc
      JRE_ARCH_DIR=ppc
      RPM_ARCH=ppc
      SYSTEMTAP_ARCH_DIR=powerpc
      ARCH_PREFIX=${LINUX32}
      ARCHFLAG="-m32"
      ;;
    powerpc64)
      BUILD_ARCH_DIR=ppc64
      INSTALL_ARCH_DIR=ppc64
      JRE_ARCH_DIR=ppc64
      RPM_ARCH=ppc64
      SYSTEMTAP_ARCH_DIR=powerpc
      ARCHFLAG="-m64"
       ;;
    powerpc64le)
      BUILD_ARCH_DIR=ppc64le
      INSTALL_ARCH_DIR=ppc64le
      JRE_ARCH_DIR=ppc64le
      RPM_ARCH=ppc64le
      SYSTEMTAP_ARCH_DIR=powerpc
      ARCHFLAG="-m64"
       ;;
    sparc)
      BUILD_ARCH_DIR=sparc
      INSTALL_ARCH_DIR=sparc
      JRE_ARCH_DIR=sparc
      SYSTEMTAP_ARCH_DIR=sparc
      ARCH_PREFIX=${LINUX32}
      ARCHFLAG="-m32"
       ;;
    sparc64)
      BUILD_ARCH_DIR=sparcv9
      INSTALL_ARCH_DIR=sparcv9
      JRE_ARCH_DIR=sparc64
      SYSTEMTAP_ARCH_DIR=sparc
      ARCHFLAG="-m64"
       ;;
    s390)
      BUILD_ARCH_DIR=s390
      INSTALL_ARCH_DIR=s390
      JRE_ARCH_DIR=s390
      RPM_ARCH=s390
      SYSTEMTAP_ARCH_DIR=s390
      ARCH_PREFIX=${LINUX32}
      ARCHFLAG="-m31"
       ;;
    s390x)
      BUILD_ARCH_DIR=s390x
      INSTALL_ARCH_DIR=s390x
      JRE_ARCH_DIR=s390x
      RPM_ARCH=s390x
      SYSTEMTAP_ARCH_DIR=s390
      ARCHFLAG="-m64"
      ;;
    sh*)
      BUILD_ARCH_DIR=sh
      INSTALL_ARCH_DIR=sh
      JRE_ARCH_DIR=sh
      SYSTEMTAP_ARCH_DIR=sh
      ;;
    *)
      BUILD_ARCH_DIR=`uname -m`
      INSTALL_ARCH_DIR=$BUILD_ARCH_DIR
      JRE_ARCH_DIR=$INSTALL_ARCH_DIR
      SYSTEMTAP_ARCH_DIR=$INSTALL_ARCH_DIR
      ;;
  esac
  AC_SUBST(BUILD_ARCH_DIR)
  AC_SUBST(INSTALL_ARCH_DIR)
  AC_SUBST(JRE_ARCH_DIR)
  AC_SUBST(SYSTEMTAP_ARCH_DIR)
  AC_SUBST(ARCH_PREFIX)
  AC_SUBST(ARCHFLAG)
])

AC_DEFUN([IT_SET_OS_DIRS],
[
  case "${target_os}" in
    *linux*)
      BUILD_OS_DIR=linux
      OS_PATH=
      ;;
    *solaris*)
      BUILD_OS_DIR=solaris
      OS_PATH=/opt/SunStudioExpress/bin:/opt/SUNWpro/bin:/usr/gnu/bin
      ;;
    *darwin*|*bsd*)
      BUILD_OS_DIR=bsd
      OS_PATH=
      ;;
    *)
      AC_MSG_ERROR([unsupported operating system ${target_os}])
      ;;
  esac
  AC_SUBST(BUILD_OS_DIR)
  AC_SUBST(OS_PATH)
])

AC_DEFUN([IT_FIND_COMPILER],
[
  IT_FIND_JAVAC
  IT_FIND_ECJ
  IT_USING_ECJ

  AC_SUBST(ECJ)
  AC_SUBST(JAVAC)
])

AC_DEFUN_ONCE([IT_FIND_ECJ],
[
  ECJ_DEFAULT=/usr/bin/ecj
  AC_MSG_CHECKING([if an ecj binary was specified])
  AC_ARG_WITH([ecj],
	      [AS_HELP_STRING(--with-ecj,bytecode compilation with ecj)],
  [
    if test "x${withval}" = "xyes"; then
      ECJ=no
    else
      ECJ="${withval}"
    fi
  ],
  [ 
    ECJ=no
  ])
  AC_MSG_RESULT(${ECJ})
  if test "x${ECJ}" = "xno"; then
    ECJ=${ECJ_DEFAULT}
  fi
  AC_MSG_CHECKING([if $ECJ is a valid executable file])
  if test -x "${ECJ}" && test -f "${ECJ}"; then
    AC_MSG_RESULT([yes])
  else
    ECJ=""
    AC_PATH_PROG(ECJ, "ecj")
    if test -z "${ECJ}"; then
      AC_PATH_PROG(ECJ, "ecj-3.1")
    fi
    if test -z "${ECJ}"; then
      AC_PATH_PROG(ECJ, "ecj-3.2")
    fi
    if test -z "${ECJ}"; then
      AC_PATH_PROG(ECJ, "ecj-3.3")
    fi
    if test -z "${ECJ}"; then
      AC_PATH_PROG(ECJ, "ecj-3.4")
    fi
  fi
])

AC_DEFUN_ONCE([IT_FIND_JAVAC],
[
  JAVAC_DEFAULT=${SYSTEM_JDK_DIR}/bin/javac
  AC_MSG_CHECKING([if a javac binary was specified])
  AC_ARG_WITH([javac],
	      [AS_HELP_STRING([--with-javac[[=PATH]]],the path to a javac binary)],
  [
    if test "x${withval}" = "xyes"; then
      JAVAC=no
    else
      JAVAC="${withval}"
    fi
  ],
  [
    JAVAC=no
  ])
  AC_MSG_RESULT(${JAVAC})
  if test "x${JAVAC}" = "xno"; then
    JAVAC=${JAVAC_DEFAULT}
  fi
  AC_MSG_CHECKING([if $JAVAC is a valid executable file])
  if test -x "${JAVAC}" && test -f "${JAVAC}"; then
    AC_MSG_RESULT([yes])
  else
    AC_MSG_RESULT([no])
    JAVAC=""
    AC_PATH_PROG(JAVAC, "javac")
  fi
  AC_SUBST(JAVAC)
  ])
])

AC_DEFUN_ONCE([IT_FIND_JAVA],
[
  JAVA_DEFAULT=${SYSTEM_JDK_DIR}/bin/java
  AC_MSG_CHECKING([if a java binary was specified])
  AC_ARG_WITH([java],
              [AS_HELP_STRING([--with-java[[=PATH]]],specify location of a 1.5 Java VM)],
  [
    if test "x${withval}" = "xyes"; then
      JAVA=no
    else
      JAVA="${withval}"
    fi
  ],
  [
    JAVA=no
  ])
  AC_MSG_RESULT(${JAVA})
  if test "x${JAVA}" = "xno"; then
    JAVA=${JAVA_DEFAULT}
  fi
  AC_MSG_CHECKING([if $JAVA is a valid executable file])
  if test -x "${JAVA}" && test -f "${JAVA}"; then
    AC_MSG_RESULT([yes])
  else
    AC_MSG_RESULT([no])
    JAVA=""
    AC_PATH_PROG(JAVA, "java")
    if test -z "${JAVA}"; then
      AC_PATH_PROG(JAVA, "gij")
    fi
    if test -z "${JAVA}"; then
      AC_PATH_PROG(JAVA, "cacao")
    fi
    if test -z "${JAVA}"; then
      AC_MSG_ERROR("A 1.5-compatible Java VM is required.")
    fi
  fi
  AC_SUBST(JAVA)
])

AC_DEFUN_ONCE([IT_CP_SUPPORTS_REFLINK],
[
  AC_CACHE_CHECK([if cp supports --reflink], it_cv_reflink, [
    touch tmp.$$
    if cp --reflink=auto tmp.$$ tmp2.$$ >&AS_MESSAGE_LOG_FD 2>&1; then
      it_cv_reflink=yes;
    else
      it_cv_reflink=no;
    fi
    rm -f tmp.$$ tmp2.$$
  ])
  AM_CONDITIONAL([CP_SUPPORTS_REFLINK], test x"${it_cv_reflink}" = "xyes")
])

AC_DEFUN_ONCE([IT_WITH_OPENJDK_SRC_DIR],
[
  DEFAULT_SRC_DIR=${abs_top_builddir}/openjdk
  AC_MSG_CHECKING([for an OpenJDK source directory])
  AC_ARG_WITH([openjdk-src-dir],
              [AS_HELP_STRING([--with-openjdk-src-dir=DIR],specify the location of the OpenJDK source tree)],
  [
    OPENJDK_SRC_DIR=${withval}
    with_external_src_dir=true
  ],
  [ 
    OPENJDK_SRC_DIR=${DEFAULT_SRC_DIR}
    with_external_src_dir=false
  ])
  AC_MSG_RESULT(${OPENJDK_SRC_DIR})
  AC_SUBST(OPENJDK_SRC_DIR)
  if test "x${with_external_src_dir}" = "xtrue"; then
    AC_MSG_CHECKING([if ${OPENJDK_SRC_DIR}/README exists])
    if test -f ${OPENJDK_SRC_DIR}/README; then
      openjdk_src_dir_valid=yes;
    else
      openjdk_src_dir_valid="no, resetting to ${DEFAULT_SRC_DIR}";
      OPENJDK_SRC_DIR=${DEFAULT_SRC_DIR}
      with_external_src_dir=false
    fi
    AC_MSG_RESULT(${openjdk_src_dir_valid})
    if test "x${openjdk_src_dir_valid}" = "xyes"; then
      AC_MSG_CHECKING([if we can hard link rather than copy the OpenJDK source directory])
      if cp -l ${OPENJDK_SRC_DIR}/README tmp.$$ >&AS_MESSAGE_LOG_FD 2>&1; then
        openjdk_src_dir_hardlinkable=yes;
      else
        openjdk_src_dir_hardlinkable=no;
      fi
      AC_MSG_RESULT(${openjdk_src_dir_hardlinkable})
      rm -f tmp.$$
    fi
  fi
  AM_CONDITIONAL(OPENJDK_SRC_DIR_FOUND, test "x${with_external_src_dir}" = "xtrue")
  AM_CONDITIONAL(OPENJDK_SRC_DIR_HARDLINKABLE, test "x${openjdk_src_dir_hardlinkable}" = "xyes")
])

AC_DEFUN_ONCE([IT_CAN_HARDLINK_TO_SOURCE_TREE],
[
  AC_CACHE_CHECK([if we can hard link rather than copy from ${abs_top_srcdir}], it_cv_hardlink_src, [
    if cp -l ${abs_top_srcdir}/README tmp.$$ >&AS_MESSAGE_LOG_FD 2>&1; then
      it_cv_hardlink_src=yes;
    else
      it_cv_hardlink_src=no;
    fi
    rm -f tmp.$$
  ])
  AM_CONDITIONAL([SRC_DIR_HARDLINKABLE], test x"${it_cv_hardlink_src}" = "xyes")
])

AC_DEFUN([IT_FIND_ECJ_JAR],
[
  AC_MSG_CHECKING([for an ecj JAR file])
  AC_ARG_WITH([ecj-jar],
              [AS_HELP_STRING([--with-ecj-jar[[=PATH]]],specify location of an ECJ JAR file)],
  [
    if test -f "${withval}"; then
      ECJ_JAR="${withval}"
    fi
  ],
  [
    ECJ_JAR=
  ])
  if test -z "${ECJ_JAR}"; then
    for jar in /usr/share/java/eclipse-ecj.jar \
      /usr/share/java/ecj.jar \
      /usr/share/eclipse-ecj-3.{2,3,4,5}/lib/ecj.jar; do
        if test -e $jar; then
          ECJ_JAR=$jar
	  break
        fi
      done
      if test -z "${ECJ_JAR}"; then
        ECJ_JAR=no
      fi
  fi
  AC_MSG_RESULT(${ECJ_JAR})
  if test "x${ECJ_JAR}" = "xno"; then
    if test "x${JAVAC}" = "x"; then
      AC_MSG_ERROR("No compiler or ecj JAR file was found.")
    fi
  fi
  AC_SUBST(ECJ_JAR)
])

AC_DEFUN([IT_CHECK_GCC_VERSION],
[
  AC_MSG_CHECKING([version of GCC])
  gcc_ver=`${CC} -dumpversion`
  gcc_major_ver=`echo ${gcc_ver}|cut -d'.' -f1`
  gcc_minor_ver=`echo ${gcc_ver}|cut -d'.' -f2`
  AM_CONDITIONAL(GCC_OLD, test ! ${gcc_major_ver} -ge 4 -a ${gcc_minor_ver} -ge 3)
  AC_MSG_RESULT([${gcc_ver} (major version ${gcc_major_ver}, minor version ${gcc_minor_ver})])
])

AC_DEFUN([IT_FIND_JAVAH],
[
  JAVAH_DEFAULT=${SYSTEM_JDK_DIR}/bin/javah
  AC_MSG_CHECKING([if a javah executable is specified])
  AC_ARG_WITH([javah],
              [AS_HELP_STRING([--with-javah[[=PATH]]],specify location of javah)],
  [
    if test "x${withval}" = "xyes"; then
      JAVAH=no
    else
      JAVAH="${withval}"
    fi
  ],
  [
    JAVAH=no
  ])
  AC_MSG_RESULT(${JAVAH})
  if test "x${JAVAH}" == "xno"; then
    JAVAH=${JAVAH_DEFAULT}
  fi
  AC_MSG_CHECKING([if $JAVAH is a valid executable file])
  if test -x "${JAVAH}" && test -f "${JAVAH}"; then
    AC_MSG_RESULT([yes])
  else
    AC_MSG_RESULT([no])
    JAVAH=""
    AC_PATH_PROG(JAVAH, "javah")
    if test -z "${JAVAH}"; then
      AC_PATH_PROG(JAVAH, "gjavah")
    fi
    if test -z "${JAVAH}"; then
      AC_MSG_ERROR("A Java header generator was not found.")
    fi
  fi
  AC_SUBST(JAVAH)
])

AC_DEFUN([IT_FIND_JAR],
[
  JAR_DEFAULT=${SYSTEM_JDK_DIR}/bin/jar
  AC_MSG_CHECKING([if a jar executable is specified])
  AC_ARG_WITH([jar],
              [AS_HELP_STRING([--with-jar[[=PATH]]],specify location of jar)],
  [
    if test "x${withval}" = "xyes"; then
      JAR=no
    else
      JAR="${withval}"
     fi
  ],
  [
    JAR=no
  ])
  AC_MSG_RESULT(${JAR})
  if test "x${JAR}" == "xno"; then
    JAR=${JAR_DEFAULT}
  fi
  AC_MSG_CHECKING([if $JAR is a valid executable file])
  if test -x "${JAR}" && test -f "${JAR}"; then
    AC_MSG_RESULT([yes])
  else
    AC_MSG_RESULT([no])
    JAR=""
    AC_PATH_PROG(JAR, "jar")
    if test -z "${JAR}"; then
      AC_PATH_PROG(JAR, "gjar")
    fi
    if test -z "${JAR}"; then
      AC_MSG_ERROR("No Java archive tool was found.")
    fi
  fi
  AC_MSG_CHECKING([whether jar supports @<file> argument])
  touch _config.txt
  cat >_config.list <<EOF
_config.txt
EOF
  if $JAR cf _config.jar @_config.list >&AS_MESSAGE_LOG_FD 2>&1; then
    JAR_KNOWS_ATFILE=1
    AC_MSG_RESULT(yes)
  else
    JAR_KNOWS_ATFILE=
    AC_MSG_RESULT(no)
  fi
  AC_MSG_CHECKING([whether jar supports stdin file arguments])
  if cat _config.list | $JAR cf@ _config.jar >&AS_MESSAGE_LOG_FD 2>&1; then
    JAR_ACCEPTS_STDIN_LIST=1
    AC_MSG_RESULT(yes)
  else
    JAR_ACCEPTS_STDIN_LIST=
    AC_MSG_RESULT(no)
  fi
  rm -f _config.list _config.jar
  AC_MSG_CHECKING([whether jar supports -J options at the end])
  if $JAR cf _config.jar _config.txt -J-Xmx896m >&AS_MESSAGE_LOG_FD 2>&1; then
    JAR_KNOWS_J_OPTIONS=1
    AC_MSG_RESULT(yes)
  else
    JAR_KNOWS_J_OPTIONS=
    AC_MSG_RESULT(no)
  fi
  rm -f _config.txt _config.jar
  AC_SUBST(JAR)
  AC_SUBST(JAR_KNOWS_ATFILE)
  AC_SUBST(JAR_ACCEPTS_STDIN_LIST)
  AC_SUBST(JAR_KNOWS_J_OPTIONS)
])

AC_DEFUN([IT_FIND_RMIC],
[
  RMIC_DEFAULT=${SYSTEM_JDK_DIR}/bin/rmic
  AC_MSG_CHECKING(if an rmic executable is specified)
  AC_ARG_WITH([rmic],
              [AS_HELP_STRING([--with-rmic[[=PATH]]],specify location of rmic)],
  [
    if test "x${withval}" = "xyes"; then
      RMIC=no
    else
      RMIC="${withval}"
    fi
  ],
  [
    RMIC=no
  ])
  AC_MSG_RESULT(${RMIC})
  if test "x${RMIC}" = "xno"; then
    RMIC=${RMIC_DEFAULT}
  fi
  AC_MSG_CHECKING([if $RMIC is a valid executable file])
  if test -x "${RMIC}" && test -f "${RMIC}"; then
    AC_MSG_RESULT([yes])
  else
    AC_MSG_RESULT([no])
    RMIC=""
    AC_PATH_PROG(RMIC, "rmic")
    if test -z "${RMIC}"; then
      AC_PATH_PROG(RMIC, "grmic")
    fi
    if test -z "${RMIC}"; then
      AC_MSG_ERROR("An RMI compiler was not found.")
    fi
  fi
  AC_SUBST(RMIC)
])

AC_DEFUN([IT_FIND_NATIVE2ASCII],
[
  NATIVE2ASCII_DEFAULT=${SYSTEM_JDK_DIR}/bin/native2ascii
  AC_MSG_CHECKING([if a native2ascii binary was specified])
  AC_ARG_WITH([native2ascii],
              [AS_HELP_STRING(--with-native2ascii,specify location of the native2ascii converter)],
  [
    if test "x${withval}" = "xyes"; then
      NATIVE2ASCII=no
    else
      NATIVE2ASCII="${withval}"
   fi
  ],
  [
    NATIVE2ASCII=no
  ])
  AC_MSG_RESULT(${NATIVE2ASCII})
  if test "x${NATIVE2ASCII}" = "xno"; then
    NATIVE2ASCII=${NATIVE2ASCII_DEFAULT}
  fi
  AC_MSG_CHECKING([if $NATIVE2ASCII is a valid executable file])
  if test -x "${NATIVE2ASCII}" && test -f "${NATIVE2ASCII}"; then
    AC_MSG_RESULT([yes])
  else
    AC_MSG_RESULT([no])
    NATIVE2ASCII=""
    AC_PATH_PROG(NATIVE2ASCII, "native2ascii")
    if test -z "${NATIVE2ASCII}"; then
      AC_PATH_PROG(NATIVE2ASCII, "gnative2ascii")
    fi
    if test -z "${NATIVE2ASCII}"; then
      AC_MSG_ERROR("A native2ascii converter was not found.")
    fi
  fi
  AC_SUBST([NATIVE2ASCII])
])

AC_DEFUN_ONCE([IT_WITH_OPENJDK_SRC_ZIP],
[
  AC_MSG_CHECKING([for an OpenJDK source zip])
  AC_ARG_WITH([openjdk-src-zip],
              [AS_HELP_STRING([--with-openjdk-src-zip[[=PATH]]],specify the location of the OpenJDK source zip)],
  [
    ALT_OPENJDK_SRC_ZIP=${withval}
    if test "x${ALT_OPENJDK_SRC_ZIP}" = "xno"; then
      ALT_OPENJDK_SRC_ZIP="not specified"
    elif ! test -f ${ALT_OPENJDK_SRC_ZIP} ; then
      AC_MSG_ERROR([Invalid OpenJDK source zip specified: ${ALT_OPENJDK_SRC_ZIP}])
    fi
  ],
  [ 
    ALT_OPENJDK_SRC_ZIP="not specified"
  ])
  AM_CONDITIONAL(USE_ALT_OPENJDK_SRC_ZIP, test "x${ALT_OPENJDK_SRC_ZIP}" != "xnot specified")
  AC_MSG_RESULT(${ALT_OPENJDK_SRC_ZIP})
  AC_SUBST(ALT_OPENJDK_SRC_ZIP)
])

AC_DEFUN([IT_WITH_ALT_JAR_BINARY],
[
  AC_MSG_CHECKING([for an alternate jar command])
  AC_ARG_WITH([alt-jar],
              [AS_HELP_STRING(--with-alt-jar=PATH, specify the location of an alternate jar binary to use for building)],
  [
    ALT_JAR_CMD=${withval}
    AM_CONDITIONAL(USE_ALT_JAR, test x = x)
  ],
  [ 
    ALT_JAR_CMD="not specified"
    AM_CONDITIONAL(USE_ALT_JAR, test x != x)
  ])
  AC_MSG_RESULT(${ALT_JAR_CMD})
  AC_SUBST(ALT_JAR_CMD)
])

AC_DEFUN([IT_FIND_TOOL],
[AC_PATH_TOOL([$1],[$2])
 if test x"$$1" = x ; then
   AC_MSG_ERROR([The following program was not found on the PATH: $2])
 fi
 AC_SUBST([$1])
])

AC_DEFUN([IT_FIND_TOOLS],
[AC_PATH_PROGS([$1],[$2])
 if test x"$$1" = x ; then
   AC_MSG_ERROR([None of the following programs could be found on the PATH: $2])
 fi
 AC_SUBST([$1])
])

AC_DEFUN_ONCE([IT_ENABLE_ZERO_BUILD],
[
  AC_REQUIRE([IT_SET_ARCH_SETTINGS])
  AC_REQUIRE([IT_ENABLE_CACAO])
  AC_REQUIRE([IT_ENABLE_JAMVM])
  AC_REQUIRE([IT_ENABLE_SHARK])
  AC_REQUIRE([IT_ARCH_HAS_NATIVE_HOTSPOT_PORT])
  AC_MSG_CHECKING([whether to use the zero-assembler port])
  use_zero=no
  AC_ARG_ENABLE([zero],
                [AS_HELP_STRING(--enable-zero,
                               use zero-assembler port on non-zero platforms)],
  [
    case "${enableval}" in
      no)
        use_zero=no
        ;;
      *)
        use_zero=yes
        ;;
    esac
  ],
  [
    if test "x${use_shark}" = "xyes"; then
      use_zero=yes;
    else if test "x$has_native_hotspot_port" = "xno"; then
      if test "x${ENABLE_CACAO}" = xyes || \
         test "x${ENABLE_JAMVM}" = xyes; then
           use_zero=no
      else
           use_zero=yes
      fi
    fi; fi
  ])
  AC_MSG_RESULT($use_zero)
  AM_CONDITIONAL(ZERO_BUILD, test "x${use_zero}" = xyes)

  ZERO_LIBARCH="${INSTALL_ARCH_DIR}"
  dnl can't use AC_CHECK_SIZEOF on multilib
  case "${ZERO_LIBARCH}" in
    arm|i386|ppc|s390|sh|sparc)
      ZERO_BITSPERWORD=32
      ;;
    aarch64|alpha|amd64|ia64|ppc64|ppc64le|s390x|sparcv9)
      ZERO_BITSPERWORD=64
      ;;
    *)
      AC_CHECK_SIZEOF(void *)
      ZERO_BITSPERWORD=`expr "${ac_cv_sizeof_void_p}" "*" 8`
  esac
  AC_C_BIGENDIAN([ZERO_ENDIANNESS="big"], [ZERO_ENDIANNESS="little"])
  case "${ZERO_LIBARCH}" in
    i386)
      ZERO_ARCHDEF="IA32"
      ;;
    ppc*)
      ZERO_ARCHDEF="PPC"
      ;;
    s390*)
      ZERO_ARCHDEF="S390"
      ;;
    sparc*)
      ZERO_ARCHDEF="SPARC"
      ;;
    *)
      ZERO_ARCHDEF=`echo ${ZERO_LIBARCH} | tr a-z A-Z`
  esac
  AC_SUBST(ZERO_LIBARCH)
  AC_SUBST(ZERO_BITSPERWORD)
  AC_SUBST(ZERO_ENDIANNESS)
  AC_SUBST(ZERO_ARCHDEF)
])

AC_DEFUN_ONCE([IT_ENABLE_SHARK],
[
  AC_MSG_CHECKING([whether to use the Shark JIT])
  AC_ARG_ENABLE([shark], [AS_HELP_STRING(--enable-shark, use Shark JIT)],
  [
    use_shark="${enableval}"
  ],
  [
    use_shark=no
  ])

  AC_MSG_RESULT($use_shark)
  AM_CONDITIONAL(SHARK_BUILD, test "x${use_shark}" = xyes)
  AC_SUBST(ENABLE_SHARK)
])

AC_DEFUN([IT_ENABLE_CACAO],
[
  AC_MSG_CHECKING(whether to use CACAO as VM)
  AC_ARG_ENABLE([cacao],
	      [AS_HELP_STRING(--enable-cacao,use CACAO as VM [[default=no]])],
  [
    case "${enableval}" in
      yes)
        ENABLE_CACAO=yes
        ;;
      *)
        ENABLE_CACAO=no
        ;;
    esac
  ],
  [
    ENABLE_CACAO=no
  ])

  AC_MSG_RESULT(${ENABLE_CACAO})
  AM_CONDITIONAL(ENABLE_CACAO, test x"${ENABLE_CACAO}" = "xyes")
  AC_SUBST(ENABLE_CACAO)
])

AC_DEFUN([IT_WITH_CACAO_HOME],
[
  AC_MSG_CHECKING([for a CACAO home directory])
  AC_ARG_WITH([cacao-home],
              [AS_HELP_STRING([--with-cacao-home[[=PATH]]],
                              [CACAO home directory [[PATH=/usr/local/cacao]]])],
              [
                case "${withval}" in
                yes)
                  CACAO_IMPORT_PATH=/usr/local/cacao
                  ;;
                *)
                  CACAO_IMPORT_PATH=${withval}
                  ;;
                esac
                AM_CONDITIONAL(USE_SYSTEM_CACAO, true)
              ],
              [
                CACAO_IMPORT_PATH="\$(abs_top_builddir)/cacao/install/hotspot"
                AM_CONDITIONAL(USE_SYSTEM_CACAO, false)
              ])
  AC_MSG_RESULT(${CACAO_IMPORT_PATH})
  AC_SUBST(CACAO_IMPORT_PATH)
])

AC_DEFUN_ONCE([IT_WITH_CACAO_SRC_ZIP],
[
  AC_MSG_CHECKING([for a CACAO source zip])
  AC_ARG_WITH([cacao-src-zip],
              [AS_HELP_STRING(--with-cacao-src-zip=PATH,specify the location of the CACAO source zip)],
  [
    ALT_CACAO_SRC_ZIP=${withval}
    if test "x${ALT_CACAO_SRC_ZIP}" = "xno"; then
      ALT_CACAO_SRC_ZIP="not specified"
    elif ! test -f ${ALT_CACAO_SRC_ZIP} ; then
      AC_MSG_ERROR([Invalid CACAO source zip specified: ${ALT_CACAO_SRC_ZIP}])
    fi
  ],
  [ 
    ALT_CACAO_SRC_ZIP="not specified"
  ])
  AM_CONDITIONAL(USE_ALT_CACAO_SRC_ZIP, test "x${ALT_CACAO_SRC_ZIP}" != "xnot specified")
  AC_MSG_RESULT(${ALT_CACAO_SRC_ZIP})
  AC_SUBST(ALT_CACAO_SRC_ZIP)
])

AC_DEFUN([IT_WITH_CACAO_SRC_DIR],
[
  AC_MSG_CHECKING([for a CACAO source directory])
  AC_ARG_WITH([cacao-src-dir],
              [AS_HELP_STRING(--with-cacao-src-dir,specify the location of the Cacao sources)],
  [
    ALT_CACAO_SRC_DIR=${withval}
    AM_CONDITIONAL(USE_ALT_CACAO_SRC_DIR, test x = x)
  ],
  [
    ALT_CACAO_SRC_DIR="not specified"
    AM_CONDITIONAL(USE_ALT_CACAO_SRC_DIR, test x != x)
  ])
  AC_MSG_RESULT(${ALT_CACAO_SRC_DIR})
  AC_SUBST(ALT_CACAO_SRC_DIR)
])

AC_DEFUN([IT_ENABLE_HG],
[
  AC_MSG_CHECKING(whether to retrieve the source code from Mercurial)
  AC_ARG_ENABLE([hg],
                [AS_HELP_STRING(--enable-hg,download source code from Mercurial [[default=no]])],
  [
    case "${enableval}" in
      no)
	enable_hg=no
        ;;
      *)
        enable_hg=yes
        ;;
    esac
  ],
  [
    enable_hg=no
  ])
  AC_MSG_RESULT([${enable_hg}])
  AM_CONDITIONAL([USE_HG], test x"${enable_hg}" = "xyes")
])

AC_DEFUN([IT_WITH_VERSION_SUFFIX],
[
  AC_MSG_CHECKING(if a version suffix has been specified)
  AC_ARG_WITH([version-suffix],
              [AS_HELP_STRING(--with-version-suffix=TEXT,appends the given text to the JDK version)],
  [
    case "${withval}" in
      yes)
	version_suffix=
	AC_MSG_RESULT([no])
        ;;
      no)
	version_suffix=
	AC_MSG_RESULT([no])
	;;
      *)
        version_suffix=${withval}
	AC_MSG_RESULT([${version_suffix}])
        ;;
    esac
  ],
  [
    version_suffix=
    AC_MSG_RESULT([no])
  ])
  AC_SUBST(VERSION_SUFFIX, $version_suffix)
])

AC_DEFUN_ONCE([IT_WITH_HOTSPOT_BUILD],
[
  case "${host_cpu}" in
    arm64) DEFAULT_BUILD="default" ;;
    arm*) DEFAULT_BUILD="aarch32" ;;
    *) DEFAULT_BUILD="default" ;;
  esac
  AC_MSG_NOTICE([Default HotSpot build on this architecture is ${DEFAULT_BUILD}])
  AC_MSG_CHECKING([which HotSpot build to use])
  AC_ARG_WITH([hotspot-build],
	      [AS_HELP_STRING(--with-hotspot-build=BUILD,the HotSpot build to use [[BUILD=default]])],
  [
    HSBUILD="${withval}"
  ],
  [ 
    HSBUILD="${DEFAULT_BUILD}"
  ])
  if test "x${HSBUILD}" = xyes; then
	HSBUILD="${DEFAULT_BUILD}"
  elif test "x${HSBUILD}" = xno; then
	HSBUILD="default"
  fi
  AC_MSG_RESULT([${HSBUILD}])
  AC_SUBST([HSBUILD])
  AM_CONDITIONAL(WITH_ALT_HSBUILD, test "x${HSBUILD}" != "xdefault")
  AM_CONDITIONAL(WITH_AARCH32_HSBUILD, test "x${HSBUILD}" = "xaarch32")
  AM_CONDITIONAL(WITH_SHENANDOAH_HSBUILD, test "x${HSBUILD}" = "xshenandoah")
])

AC_DEFUN_ONCE([IT_WITH_HOTSPOT_SRC_ZIP],
[
  AC_MSG_CHECKING([for a HotSpot source zip])
  AC_ARG_WITH([hotspot-src-zip],
              [AS_HELP_STRING(--with-hotspot-src-zip=PATH,specify the location of the HotSpot source zip)],
  [
    ALT_HOTSPOT_SRC_ZIP=${withval}
    if test "x${ALT_HOTSPOT_SRC_ZIP}" = "xno"; then
      ALT_HOTSPOT_SRC_ZIP="not specified"
    elif ! test -f ${ALT_HOTSPOT_SRC_ZIP} ; then
      AC_MSG_ERROR([Invalid HotSpot source zip specified: ${ALT_HOTSPOT_SRC_ZIP}])
    fi
  ],
  [ 
    ALT_HOTSPOT_SRC_ZIP="not specified"
  ])
  AM_CONDITIONAL(USE_ALT_HOTSPOT_SRC_ZIP, test "x${ALT_HOTSPOT_SRC_ZIP}" != "xnot specified")
  AC_MSG_RESULT(${ALT_HOTSPOT_SRC_ZIP})
  AC_SUBST(ALT_HOTSPOT_SRC_ZIP)
])

AC_DEFUN([IT_WITH_HG_REVISION],
[
  AC_MSG_CHECKING([which Mercurial revision to use])
  AC_ARG_WITH([hg-revision],
	      [AS_HELP_STRING(--with-hg-revision=REV,the Mercurial revision to use [[REV=tip]])],
  [
    HGREV="${withval}"
    AC_MSG_RESULT([${HGREV}])
  ],
  [ 
    HGREV=""
    AC_MSG_RESULT([tip])
  ])
  AC_SUBST([HGREV])
  AM_CONDITIONAL(WITH_HGREV, test "x${HGREV}" != "x")
])

AC_DEFUN([IT_CHECK_IF_BOOTSTRAPPING],
[
  AC_MSG_CHECKING([whether to build a bootstrap version first])
  AC_ARG_ENABLE([bootstrap],
                [AS_HELP_STRING(--disable-bootstrap, don't build a bootstrap version [[default=no]])],
  [
    case "${enableval}" in
      no)
	enable_bootstrap=no
        ;;
      *)
        enable_bootstrap=yes
        ;;
    esac
  ],
  [
        enable_bootstrap=yes
  ])
  AC_MSG_RESULT([${enable_bootstrap}])
  AM_CONDITIONAL([BOOTSTRAPPING], test x"${enable_bootstrap}" = "xyes")
])

AC_DEFUN([IT_CHECK_FOR_JDK],
[
  AC_REQUIRE([IT_SET_ARCH_SETTINGS])
  AC_MSG_CHECKING([for a JDK home directory])
  AC_ARG_WITH([jdk-home],
	      [AS_HELP_STRING([--with-jdk-home[[=PATH]]],
                              [jdk home directory (default is first predefined JDK found)])],
              [
                if test "x${withval}" = xyes
                then
                  SYSTEM_JDK_DIR=
                elif test "x${withval}" = xno
                then
	          SYSTEM_JDK_DIR=
	        else
                  SYSTEM_JDK_DIR=${withval}
                fi
              ],
              [
	        SYSTEM_JDK_DIR=
              ])
  if test -z "${SYSTEM_JDK_DIR}"; then
    AC_MSG_RESULT([not specified])
    if test "x${enable_bootstrap}" = "xyes"; then
      BOOTSTRAP_VMS="/usr/lib/jvm/cacao";
    fi
    ICEDTEA7_VMS="/usr/lib/jvm/icedtea-7 /usr/lib/jvm/icedtea7 /usr/lib/jvm/java-1.7.0-openjdk
    		  /usr/lib/jvm/java-1.7.0-openjdk.${RPM_ARCH} /usr/lib64/jvm/java-1.7.0-openjdk
		  /usr/lib/jvm/java-1.7.0 /usr/lib/jvm/java-7-openjdk"
    ICEDTEA8_VMS="/usr/lib/jvm/icedtea-8 /usr/lib/jvm/java-1.8.0-openjdk
    		  /usr/lib/jvm/java-1.8.0-openjdk.${RPM_ARCH} /usr/lib64/jvm/java-1.8.0-openjdk
		  /usr/lib/jvm/java-1.8.0 /usr/lib/jvm/java-8-openjdk"
    for dir in ${ICEDTEA8_VMS} ${ICEDTEA7_VMS} ${BOOTSTRAP_VMS} \
    	       /usr/lib/jvm/java-openjdk /usr/lib/jvm/openjdk /usr/lib/jvm/java-icedtea \
	       /etc/alternatives/java_sdk_openjdk ; do
       AC_MSG_CHECKING([for ${dir}]);
       if test -d $dir; then
         SYSTEM_JDK_DIR=$dir ;
	 AC_MSG_RESULT([found]) ;
	 break ;
       else
         AC_MSG_RESULT([not found]) ;
       fi
    done
  else
    AC_MSG_RESULT(${SYSTEM_JDK_DIR})
  fi
  if ! test -d "${SYSTEM_JDK_DIR}"; then
    AC_MSG_ERROR("A JDK home directory could not be found.")
  fi
  AC_SUBST(SYSTEM_JDK_DIR)
])

AC_DEFUN([IT_CHECK_ADDITIONAL_VMS],
[
AC_MSG_CHECKING([for additional virtual machines to build])
AC_ARG_WITH(additional-vms,
            AC_HELP_STRING([--with-additional-vms=VM-LIST],
	    [build additional virtual machines. Valid value is a comma separated string with the backend names `cacao', `jamvm', `zero' and `shark'.]),
[
if test "x${withval}" != x ; then
  with_additional_vms=${withval}
  for vm in `echo ${with_additional_vms} | sed 's/,/ /g'`; do
    case "x$vm" in
      xcacao) add_vm_cacao=yes;;
      xzero)  add_vm_zero=yes;;
      xshark) add_vm_shark=yes;;
      xjamvm) add_vm_jamvm=yes;;
      *) AC_MSG_ERROR([proper usage is --with-additional-vms=vm1,vm2,...])
    esac
  done
fi])

if test "x${with_additional_vms}" = x; then
   with_additional_vms="none";
fi
AC_MSG_RESULT($with_additional_vms)

AM_CONDITIONAL(ADD_JAMVM_BUILD, test x$add_vm_jamvm != x)
AM_CONDITIONAL(ADD_CACAO_BUILD, test x$add_vm_cacao != x)
AM_CONDITIONAL(ADD_ZERO_BUILD,  test x$add_vm_zero  != x || test x$add_vm_shark != x)
AM_CONDITIONAL(ADD_SHARK_BUILD, test x$add_vm_shark != x)
AM_CONDITIONAL(BUILD_CACAO, test x$add_vm_cacao != x || test "x${ENABLE_CACAO}" = xyes)
AM_CONDITIONAL(BUILD_JAMVM, test x$add_vm_jamvm != x || test "x${ENABLE_JAMVM}" = xyes)

if test "x${ENABLE_JAMVM}" = xyes && test "x${ADD_JAMVM_BUILD_TRUE}" = x; then
  AC_MSG_ERROR([additional vm is the default vm])
fi
if test "x${ENABLE_CACAO}" = xyes && test "x${ADD_CACAO_BUILD_TRUE}" = x; then
  AC_MSG_ERROR([additional vm is the default vm])
fi
if test "x${ZERO_BUILD_TRUE}" = x && test "x${ADD_ZERO_BUILD_TRUE}" = x && test "x${ADD_SHARK_BUILD_TRUE}" != x; then
  AC_MSG_ERROR([additional vm is the default vm])
fi
if test "x${SHARK_BUILD_TRUE}" = x && test "x${ADD_SHARK_BUILD_TRUE}" = x; then
  AC_MSG_ERROR([additional vm is the default vm])
fi
if test "x${USE_SYSTEM_CACAO_TRUE}" = x; then
  AC_MSG_ERROR([cannot build with system cacao as additional vm])
fi
if test "x${ADD_ZERO_BUILD_TRUE}" = x && test "x${abs_top_builddir}" = "x${abs_top_srcdir}"; then
  AC_MSG_ERROR([build of additional zero/shark VM requires build with srcdir != builddir])
fi
])

AC_DEFUN([IT_USING_ECJ],[
AC_CACHE_CHECK([if we are using ecj as javac], it_cv_ecj, [
if $JAVAC -version 2>&1| grep '^Eclipse' >&AS_MESSAGE_LOG_FD ; then
  it_cv_ecj=yes;
else
  it_cv_ecj=no;
fi
])
USING_ECJ=$it_cv_ecj
AC_SUBST(USING_ECJ)
AC_PROVIDE([$0])dnl
])

AC_DEFUN([IT_CHECK_ENABLE_WARNINGS],
[
  AC_MSG_CHECKING(whether to enable Java compiler warnings)
  AC_ARG_ENABLE([warnings],
	      [AS_HELP_STRING(--enable-warnings,produce warnings from javac/ecj [[default=no]])],
  [
    case "${enableval}" in
      no)
        ENABLE_WARNINGS=no
        ;;
      *)
        ENABLE_WARNINGS=yes
        ;;
    esac
  ],
  [
    ENABLE_WARNINGS=no
  ])

  AC_MSG_RESULT(${ENABLE_WARNINGS})
  AM_CONDITIONAL(ENABLE_WARNINGS, test x"${ENABLE_WARNINGS}" = "xyes")
  AC_SUBST(ENABLE_WARNINGS)
])

AC_DEFUN([IT_WITH_TZDATA_DIR],
[
  TZDATA_DEFAULT="${datadir}/javazi"
  AC_MSG_CHECKING([which Java timezone data directory to use])
  AC_ARG_WITH([tzdata-dir],
	      [AS_HELP_STRING(--with-tzdata-dir,set the Java timezone data directory [[default=DATAROOTDIR/javazi]])],
  [
    if test "x${withval}" = x || test "x${withval}" = xyes; then
      TZDATA_DIR="${TZDATA_DEFAULT}"
    else
      TZDATA_DIR="${withval}"
    fi
  ],
  [ 
    TZDATA_DIR="${TZDATA_DEFAULT}"
  ])
  if test "x${TZDATA_DIR}" = "xno"; then
    TZDATA_DIR=none
    TZDATA_DIR_SET=no
  else
    TZDATA_DIR_SET=yes
  fi
  AC_MSG_RESULT([${TZDATA_DIR}])
  AC_SUBST([TZDATA_DIR])
  AM_CONDITIONAL(WITH_TZDATA_DIR, test "x${TZDATA_DIR_SET}" = "xyes")
])

dnl check that javac and java work
AC_DEFUN_ONCE([IT_CHECK_JAVA_AND_JAVAC_WORK],[
  AC_REQUIRE([IT_FIND_JAVA])
  AC_REQUIRE([IT_FIND_COMPILER])
  AC_CACHE_CHECK([if the VM and compiler work together], it_cv_jdk_works, [
  CLASS=Test.java
  BYTECODE=$(echo $CLASS|sed 's#\.java##')
  mkdir tmp.$$
  cd tmp.$$
  cat << \EOF > $CLASS
[/* [#]line __oline__ "configure" */

public class Test
{
    public static void main(String[] args)
    {
      System.out.println("Hello World!");
    }
}]
EOF
  if $JAVAC -cp . $JAVACFLAGS -source 5 -target 5 $CLASS >&AS_MESSAGE_LOG_FD 2>&1; then
    if $JAVA -classpath . $BYTECODE >&AS_MESSAGE_LOG_FD 2>&1; then
      it_cv_jdk_works=yes;
    else
      it_cv_jdk_works=no;
      AC_MSG_ERROR([VM failed to run compiled class.])
    fi
  else
    it_cv_jdk_works=no;
    AC_MSG_ERROR([Compiler failed to compile Java code.])
  fi
  rm -f $CLASS *.class
  cd ..
  rmdir tmp.$$
  ])
AC_PROVIDE([$0])dnl
])

dnl Generic macro to check for a Java class
dnl Takes two arguments: the name of the macro
dnl and the name of the class.  The macro name
dnl is usually the name of the class with '.'
dnl replaced by '_' and all letters capitalised.
dnl e.g. IT_CHECK_FOR_CLASS([JAVA_UTIL_SCANNER],[java.util.Scanner])
AC_DEFUN([IT_CHECK_FOR_CLASS],[
AC_REQUIRE([IT_CHECK_JAVA_AND_JAVAC_WORK])
AC_CACHE_CHECK([if $2 is missing], it_cv_$1, [
CLASS=Test.java
BYTECODE=$(echo $CLASS|sed 's#\.java##')
mkdir tmp.$$
cd tmp.$$
cat << \EOF > $CLASS
[/* [#]line __oline__ "configure" */
public class Test
{
  public static void main(String[] args)
  {
    $2.class.toString();
  }
}
]
EOF
if $JAVAC -cp . $JAVACFLAGS -nowarn $CLASS >&AS_MESSAGE_LOG_FD 2>&1; then
  if $JAVA -classpath . $BYTECODE >&AS_MESSAGE_LOG_FD 2>&1; then
      it_cv_$1=no;
  else
      it_cv_$1=yes;
  fi
else
  it_cv_$1=yes;
fi
])
rm -f $CLASS *.class
cd ..
rmdir tmp.$$
AM_CONDITIONAL([LACKS_$1], test x"${it_cv_$1}" = "xyes")
AC_PROVIDE([$0])dnl
])

AC_DEFUN([IT_ENABLE_WERROR],
[
  AC_MSG_CHECKING([whether to enable -Werror])
  AC_ARG_ENABLE([Werror],
                [AS_HELP_STRING(--enable-Werror,build with -Werror [[default=no]])],
  [
    case "${enableval}" in
      yes)
        enable_werror=yes
        ;;
      *)
        enable_werror=no
        ;;
    esac
  ],
  [
    enable_werror=no
  ])
  AC_MSG_RESULT([$enable_werror])
  AM_CONDITIONAL([ENABLE_WERROR], test x"${enable_werror}" = "xyes")
])

AC_DEFUN([IT_FIND_NUMBER_OF_PROCESSORS],[
  FIND_TOOL([GETCONF], [getconf])
  AC_CACHE_CHECK([the number of online processors], it_cv_proc, [
    if number=$($GETCONF _NPROCESSORS_ONLN); then
      it_cv_proc=$number;
    else
      it_cv_proc=2;
    fi
  ])
  AC_PROVIDE([$0])dnl
])

AC_DEFUN_ONCE([IT_GET_PKGVERSION],
[
AC_MSG_CHECKING([for distribution package version])
AC_ARG_WITH([pkgversion],
        [AS_HELP_STRING([--with-pkgversion=PKG],
                        [Use PKG in the version string in addition to "IcedTea"])],
        [case "$withval" in
          yes) AC_MSG_ERROR([package version not specified]) ;;
          no)  PKGVERSION=none ;;
          *)   PKGVERSION="$withval" ;;
         esac],
        [PKGVERSION=none])
AC_MSG_RESULT([${PKGVERSION}])
AM_CONDITIONAL(HAS_PKGVERSION, test "x${PKGVERSION}" != "xnone") 
AC_SUBST(PKGVERSION)
])

AC_DEFUN_ONCE([IT_GET_LSB_DATA],
[
AC_REQUIRE([IT_GET_PKGVERSION])
AC_MSG_CHECKING([build identification])
if test -n "$LSB_RELEASE"; then
  lsb_info="$($LSB_RELEASE -ds | sed 's/^"//;s/"$//')"
  if test "x$PKGVERSION" = "xnone"; then
    DIST_ID="Built on $lsb_info ($(date))"
  else
    DIST_ID="$lsb_info, package $PKGVERSION"
  fi
  DIST_NAME="$($LSB_RELEASE -is | sed 's/^"//;s/"$//')"
else
  DIST_ID="Custom build ($(date))"
  DIST_NAME="$build_os"
fi
AC_MSG_RESULT([${DIST_ID}])
AC_SUBST(DIST_ID)
AC_SUBST(DIST_NAME)
])


AC_DEFUN_ONCE([IT_CHECK_FOR_MERCURIAL],
[
  AC_PATH_TOOL([HG],[hg])
  AC_SUBST([HG])
])

AC_DEFUN_ONCE([IT_OBTAIN_HG_REVISIONS],
[
  AC_REQUIRE([IT_CHECK_FOR_MERCURIAL])
  AC_REQUIRE([IT_WITH_OPENJDK_SRC_DIR])
  ICEDTEA_REVISION="none";
  JDK_REVISION="none";
  HOTSPOT_REVISION="none";
  if which ${HG} >&AS_MESSAGE_LOG_FD 2>&1; then
    AC_MSG_CHECKING([for IcedTea Mercurial revision ID])
    if test -e ${abs_top_srcdir}/.hg ; then 
      ICEDTEA_REVISION="r`(cd ${abs_top_srcdir}; ${HG} id -i)`" ; 
    fi ;
    AC_MSG_RESULT([${ICEDTEA_REVISION}])
    AC_SUBST([ICEDTEA_REVISION])
    AC_MSG_CHECKING([for JDK Mercurial revision ID])
    if test -e ${OPENJDK_SRC_DIR}/jdk/.hg ; then
      JDK_REVISION="r`(cd ${OPENJDK_SRC_DIR}/jdk; ${HG} id -i)`" ;
    fi ;
    AC_MSG_RESULT([${JDK_REVISION}])
    AC_SUBST([JDK_REVISION])
    AC_MSG_CHECKING([for HotSpot Mercurial revision ID])
    if test -e ${OPENJDK_SRC_DIR}/hotspot/.hg ; then \
      HOTSPOT_REVISION="r`(cd ${OPENJDK_SRC_DIR}/hotspot; ${HG} id -i)`" ;
    fi ; 
    AC_MSG_RESULT([${HOTSPOT_REVISION}])
    AC_SUBST([HOTSPOT_REVISION])
  fi;
  AM_CONDITIONAL([HAS_ICEDTEA_REVISION], test "x${ICEDTEA_REVISION}" != xnone)
  AM_CONDITIONAL([HAS_JDK_REVISION], test "x${JDK_REVISION}" != xnone)
  AM_CONDITIONAL([HAS_HOTSPOT_REVISION], test "x${HOTSPOT_REVISION}" != xnone)
])

AC_DEFUN_ONCE([IT_OBTAIN_DEFAULT_LIBDIR],
[
dnl find the system library directory
AC_CACHE_CHECK([for system library directory], [it_cv_default_libdir],
[
if test "x$LDD" = x; then
  it_cv_default_libdir=/usr/lib
else
  AC_LANG_CONFTEST([AC_LANG_PROGRAM([[]], [[]])])
  $CC conftest.c
  syslibdir=`$LDD a.out | sed -n '/libc\.so./s,.*=> */\(@<:@^/@:>@*\)/.*,\1,p'`
  rm -f a.out
  case x${syslibdir} in
    xlib|xlib64|xlib32|xlibn32) NSS_LIBDIR=/usr/${syslibdir};;
    *) it_cv_default_libdir=/usr/lib
  esac
fi
])
AC_SUBST([DEFAULT_LIBDIR], $it_cv_default_libdir)
])

AC_DEFUN_ONCE([IT_LOCATE_NSS],
[
AC_REQUIRE([IT_OBTAIN_DEFAULT_LIBDIR])
AC_MSG_CHECKING([whether to enable the PKCS11 crypto provider using NSS])
AC_ARG_ENABLE([nss],
	      [AS_HELP_STRING([--enable-nss],
	      		      [Enable inclusion of PKCS11 crypto provider using NSS])],
  [
    case "${enableval}" in
      no)
        ENABLE_NSS=no
        ;;
      *)
        ENABLE_NSS=yes
        ;;
    esac
  ], [ENABLE_NSS='no'])
AM_CONDITIONAL([ENABLE_NSS], [test x$ENABLE_NSS = xyes])
if test "x${ENABLE_NSS}" = "xyes"
then
  AC_MSG_RESULT([enabled by default (edit java.security to disable)])
else
  AC_MSG_RESULT([disabled by default (edit java.security to enable)])
fi
PKG_CHECK_MODULES(NSS, nss, [NSS_FOUND=yes], [NSS_FOUND=no])
if test "x${NSS_FOUND}" = xno
then
  if test "x${ENABLE_NSS}" = "xyes"
  then
    AC_MSG_ERROR([Could not find NSS.  Either install it or configure using --disable-nss.])
  else
    AC_MSG_WARN([Could not find NSS; using $DEFAULT_LIBDIR as its location.])
    NSS_LIBDIR=$DEFAULT_LIBDIR
  fi
else
  NSS_LIBDIR=`$PKG_CONFIG --variable=libdir nss`
fi
AC_SUBST(NSS_LIBDIR)
AC_CONFIG_FILES([nss.cfg])
])

AC_DEFUN([IT_DIAMOND_CHECK],[
  AC_CACHE_CHECK([if the Java compiler lacks support for the diamond operator], it_cv_diamond, [
  CLASS=Test.java
  BYTECODE=$(echo $CLASS|sed 's#\.java##')
  mkdir tmp.$$
  cd tmp.$$
  cat << \EOF > $CLASS
[/* [#]line __oline__ "configure" */
import java.util.HashMap;
import java.util.Map;

public class Test
{
    public static void main(String[] args)
    {
      Map<String,String> m = new HashMap<>();
    }
}]
EOF
  if $JAVAC -cp . $JAVACFLAGS -source 7 -target 7 $CLASS >&AS_MESSAGE_LOG_FD 2>&1; then
    it_cv_diamond=no;
  else
    it_cv_diamond=yes;
  fi
  rm -f $CLASS *.class
  cd ..
  rmdir tmp.$$
  ])
if test x"${it_cv_diamond}" = "xyes"; then
  AC_MSG_ERROR([$JAVAC does not support the diamond operator])
fi
AC_PROVIDE([$0])dnl
])

AC_DEFUN([IT_CHECK_IF_DOWNLOADING],
[
  AC_MSG_CHECKING([whether to download tarballs])
  AC_ARG_ENABLE([downloading],
	      [AS_HELP_STRING(--disable-downloading,don't download tarballs [[default=no]])],
  [
    case "${enableval}" in
      no)
	enable_downloading=no
        ;;
      *)
        enable_downloading=yes
        ;;
    esac
  ],
  [
        enable_downloading=yes
  ])
  AC_MSG_RESULT([${enable_downloading}])
  AM_CONDITIONAL([DOWNLOADING], test x"${enable_downloading}" = "xyes")
  AC_SUBST([enable_downloading])
])

# Finds number of available processors using sysconf
AC_DEFUN_ONCE([IT_FIND_NUMBER_OF_PROCESSORS],[
  IT_FIND_TOOL([GETCONF], [getconf])
  AC_CACHE_CHECK([the number of online processors], it_cv_proc, [
    if number=$($GETCONF _NPROCESSORS_ONLN); then
      it_cv_proc=$number;
    else
      it_cv_proc=2;
    fi
  ])
  AC_PROVIDE([$0])dnl
])

# Provides the option --with-parallel-jobs
#  * --with-parallel-jobs; use jobs=processors + 1
#  * --with-parallel-jobs=x; use jobs=x
#  * --without-parallel-jobs (default); use jobs=2
AC_DEFUN_ONCE([IT_CHECK_NUMBER_OF_PARALLEL_JOBS],
[
AC_REQUIRE([IT_FIND_NUMBER_OF_PROCESSORS])
proc_default=$(($it_cv_proc + 1))
AC_MSG_CHECKING([how many parallel build jobs to execute])
AC_ARG_WITH([parallel-jobs],
	[AS_HELP_STRING([--with-parallel-jobs[[=NUM]]],
			[build IcedTea using the specified number of parallel jobs])],
	[
          if test "x${withval}" = xyes; then
            PARALLEL_JOBS=${proc_default}
	  elif test "x${withval}" = xno; then
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
])

AC_DEFUN_ONCE([IT_DISABLE_TESTS],
[
  AC_MSG_CHECKING([whether to disable the execution of the JTReg tests])
  AC_ARG_ENABLE([tests],
                [AS_HELP_STRING(--disable-tests,do not run the JTReg tests via make check [[default=no]])],
  [
    case "${enableval}" in
      no)
        disable_tests=yes
        ;;
      *)
        disable_tests=no
        ;;
    esac
  ],
  [
    disable_tests=no
  ])
  AC_MSG_RESULT([$disable_tests])
  AM_CONDITIONAL([DISABLE_TESTS], test x"${disable_tests}" = "xyes")
])

AC_DEFUN_ONCE([IT_DISABLE_HOTSPOT_TESTS],
[
  AC_MSG_CHECKING([whether to disable the execution of the HotSpot JTReg tests])
  AC_ARG_ENABLE([hotspot-tests],
                [AS_HELP_STRING(--disable-hotspot-tests,do not run the HotSpot JTReg tests via make check-hotspot [[default=no]])],
  [
    case "${enableval}" in
      no)
        disable_hotspot_tests=yes
        ;;
      *)
        disable_hotspot_tests=no
        ;;
    esac
  ],
  [
    disable_hotspot_tests=no
  ])
  AC_MSG_RESULT([$disable_hotspot_tests])
  AM_CONDITIONAL([DISABLE_HOTSPOT_TESTS], test x"${disable_hotspot_tests}" = "xyes")
])

AC_DEFUN_ONCE([IT_DISABLE_LANGTOOLS_TESTS],
[
  AC_MSG_CHECKING([whether to disable the execution of the langtools JTReg tests])
  AC_ARG_ENABLE([langtools-tests],
                [AS_HELP_STRING(--disable-langtools-tests,do not run the langtools JTReg tests via make check-langtools [[default=no]])],
  [
    case "${enableval}" in
      no)
        disable_langtools_tests=yes
        ;;
      *)
        disable_langtools_tests=no
        ;;
    esac
  ],
  [
    disable_langtools_tests=no
  ])
  AC_MSG_RESULT([$disable_langtools_tests])
  AM_CONDITIONAL([DISABLE_LANGTOOLS_TESTS], test x"${disable_langtools_tests}" = "xyes")
])

AC_DEFUN_ONCE([IT_DISABLE_JDK_TESTS],
[
  AC_MSG_CHECKING([whether to disable the execution of the JDK JTReg tests])
  AC_ARG_ENABLE([jdk-tests],
                [AS_HELP_STRING(--disable-jdk-tests,do not run the JDK JTReg tests via make check-jdk [[default=no]])],
  [
    case "${enableval}" in
      no)
        disable_jdk_tests=yes
        ;;
      *)
        disable_jdk_tests=no
        ;;
    esac
  ],
  [
    disable_jdk_tests=no
  ])
  AC_MSG_RESULT([$disable_jdk_tests])
  AM_CONDITIONAL([DISABLE_JDK_TESTS], test x"${disable_jdk_tests}" = "xyes")
])

AC_DEFUN_ONCE([IT_CHECK_FOR_LCMS],
[
  AC_MSG_CHECKING([whether to use the system LCMS install])
  AC_ARG_ENABLE([system-lcms],
	      [AS_HELP_STRING(--enable-system-lcms,use the system LCMS [[default=yes]])],
  [
    case "${enableval}" in
      no)
        ENABLE_SYSTEM_LCMS=no
        ;;
      *)
        ENABLE_SYSTEM_LCMS=yes
        ;;
    esac
  ],
  [
    case "${target_os}" in
     *linux*)
       ENABLE_SYSTEM_LCMS="yes"
       ;;
     *)
       ENABLE_SYSTEM_LCMS="no" ;
       ;;
    esac
  ])
  AC_MSG_RESULT(${ENABLE_SYSTEM_LCMS})
  if test x"${ENABLE_SYSTEM_LCMS}" = "xyes"; then
    dnl Check for LCMS2 headers and libraries.
    PKG_CHECK_MODULES(LCMS2, lcms2,[LCMS2_FOUND=yes],[LCMS2_FOUND=no])
    if test "x${LCMS2_FOUND}" = xno
    then
      AC_MSG_ERROR([Could not find LCMS2; install LCMS2 or build with --disable-system-lcms to use the in-tree copy.])
    fi
    AC_SUBST(LCMS2_CFLAGS)
    AC_SUBST(LCMS2_LIBS)
  fi
  AM_CONDITIONAL(USE_SYSTEM_LCMS, test x"${ENABLE_SYSTEM_LCMS}" = "xyes")
  AC_SUBST(ENABLE_SYSTEM_LCMS)
])

AC_DEFUN_ONCE([IT_CHECK_FOR_ZLIB],
[
  AC_MSG_CHECKING([whether to use the system zlib install])
  AC_ARG_ENABLE([system-zlib],
	      [AS_HELP_STRING(--enable-system-zlib,use the system ZLIB [[default=yes]])],
  [
    case "${enableval}" in
      no)
        ENABLE_SYSTEM_ZLIB=no
        ;;
      *)
        ENABLE_SYSTEM_ZLIB=yes
        ;;
    esac
  ],
  [
    case "${target_os}" in
      *linux*)
        ENABLE_SYSTEM_ZLIB="yes"
	;;
      *)
        ENABLE_SYSTEM_ZLIB="no" ;
	;;
    esac
  ])
  AC_MSG_RESULT(${ENABLE_SYSTEM_ZLIB})
  if test x"${ENABLE_SYSTEM_ZLIB}" = "xyes"; then
    dnl Check for ZLIB headers and libraries.
    PKG_CHECK_MODULES(ZLIB, zlib,[ZLIB_FOUND=yes],[ZLIB_FOUND=no])
    if test "x${ZLIB_FOUND}" = xno
    then
      AC_MSG_ERROR([Could not find ZLIB; install ZLIB or build with --disable-system-zlib to use the in-tree copy.])
    fi
    AC_SUBST(ZLIB_CFLAGS)
    AC_SUBST(ZLIB_LIBS)
  fi
  AM_CONDITIONAL(USE_SYSTEM_ZLIB, test x"${ENABLE_SYSTEM_ZLIB}" = "xyes")
  AC_SUBST(ENABLE_SYSTEM_ZLIB)
])

AC_DEFUN_ONCE([IT_CHECK_FOR_JPEG],
[
  AC_MSG_CHECKING([whether to use the system jpeg install])
  AC_ARG_ENABLE([system-jpeg],
	      [AS_HELP_STRING(--enable-system-jpeg,use the system libjpeg [[default=yes]])],
  [
    case "${enableval}" in
      no)
        ENABLE_SYSTEM_JPEG=no
        ;;
      *)
        ENABLE_SYSTEM_JPEG=yes
        ;;
    esac
  ],
  [
    case "${target_os}" in
      *linux*)
        ENABLE_SYSTEM_JPEG="yes"
	;;
      *)
        ENABLE_SYSTEM_JPEG="no" ;
	;;
    esac
  ])
  AC_MSG_RESULT(${ENABLE_SYSTEM_JPEG})
  if test x"${ENABLE_SYSTEM_JPEG}" = "xyes"; then
    dnl Check for JPEG headers and libraries.
    AC_CHECK_LIB([jpeg], [main],
        , [AC_MSG_ERROR("Could not find JPEG library; install JPEG or build with --disable-system-jpeg to use the in-tree copy.")])
    AC_CHECK_HEADER([jpeglib.h],
        , [AC_MSG_ERROR("Could not find JPEG header; install JPEG or build with --disable-system-jpeg to use the in-tree copy.")])
    JPEG_LIBS="-ljpeg"
    AC_SUBST(JPEG_LIBS)
  fi
  AM_CONDITIONAL(USE_SYSTEM_JPEG, test x"${ENABLE_SYSTEM_JPEG}" = "xyes")
  AC_SUBST(ENABLE_SYSTEM_JPEG)
])

AC_DEFUN_ONCE([IT_CHECK_FOR_PNG],
[
  AC_MSG_CHECKING([whether to use the system libpng install])
  AC_ARG_ENABLE([system-png],
	      [AS_HELP_STRING(--enable-system-png,use the system PNG [[default=yes]])],
  [
    case "${enableval}" in
      no)
        ENABLE_SYSTEM_PNG=no
        ;;
      *)
        ENABLE_SYSTEM_PNG=yes
        ;;
    esac
  ],
  [
    case "${target_os}" in
      *linux*)
        ENABLE_SYSTEM_PNG="yes"
	;;
      *)
        ENABLE_SYSTEM_PNG="no" ;
	;;
    esac
  ])
  AC_MSG_RESULT(${ENABLE_SYSTEM_PNG})
  if test x"${ENABLE_SYSTEM_PNG}" = "xyes"; then
    dnl Check for PNG headers and libraries.
    PKG_CHECK_MODULES(PNG, libpng,[LIBPNG_FOUND=yes],[LIBPNG_FOUND=no])
    if test "x${LIBPNG_FOUND}" = xno
    then
      AC_MSG_ERROR([Could not find libpng; install libpng or build with --disable-system-png to use the in-tree copy.])
    fi
    AC_SUBST(PNG_CFLAGS)
    AC_SUBST(PNG_LIBS)
  fi
  AM_CONDITIONAL(USE_SYSTEM_PNG, test x"${ENABLE_SYSTEM_PNG}" = "xyes")
  AC_SUBST(ENABLE_SYSTEM_PNG)
])

AC_DEFUN_ONCE([IT_CHECK_FOR_GIF],
[
  AC_MSG_CHECKING([whether to use the system giflib install])
  AC_ARG_ENABLE([system-gif],
	      [AS_HELP_STRING(--enable-system-gif,use the system giflib [[default=yes]])],
  [
    case "${enableval}" in
      no)
        ENABLE_SYSTEM_GIF=no
        ;;
      *)
        ENABLE_SYSTEM_GIF=yes
        ;;
    esac
  ],
  [
    case "${target_os}" in
      *linux*)
        ENABLE_SYSTEM_GIF="yes"
	;;
      *)
        ENABLE_SYSTEM_GIF="no" ;
	;;
    esac
  ])
  AC_MSG_RESULT(${ENABLE_SYSTEM_GIF})
  if test x"${ENABLE_SYSTEM_GIF}" = "xyes"; then
    dnl Check for GIF headers and libraries.
    AC_CHECK_LIB([gif], [main],
        , [AC_MSG_ERROR("Could not find GIF library; install GIF or build with --disable-system-gif to use the in-tree copy.")])
    AC_CHECK_HEADER([gif_lib.h],
        , [AC_MSG_ERROR("Could not find GIF header; install GIF or build with --disable-system-gif to use the in-tree copy.")])
    GIF_LIBS="-lgif"
    AC_SUBST(GIF_LIBS)
  fi
  AM_CONDITIONAL(USE_SYSTEM_GIF, test x"${ENABLE_SYSTEM_GIF}" = "xyes")
  AC_SUBST(ENABLE_SYSTEM_GIF)
])

dnl Check for Kerberos library in order to lookup cache location at runtime.
AC_DEFUN_ONCE([IT_CHECK_FOR_KERBEROS],
[
  AC_MSG_CHECKING([whether to use the system Kerberos install])
  AC_ARG_ENABLE([system-kerberos],
	      [AS_HELP_STRING(--enable-system-kerberos,use the system kerberos [[default=yes]])],
  [
    ENABLE_SYSTEM_KERBEROS="${enableval}"
  ],
  [
    ENABLE_SYSTEM_KERBEROS="yes"
  ])
  AC_MSG_RESULT(${ENABLE_SYSTEM_KERBEROS})
  if test x"${ENABLE_SYSTEM_KERBEROS}" = "xyes"; then
    dnl Check for krb5 header and library.
    PKG_CHECK_MODULES(KRB5, krb5, [KRB5_FOUND=yes], [KRB5_FOUND=no])
    if test "x${KRB5_FOUND}" = "xno"; then
      AC_MSG_NOTICE([Could not find Kerberos using pkg-config; trying via krb5.h and krb5 library])
      AC_CHECK_LIB([krb5], [krb5_cc_default],
        , [AC_MSG_ERROR([Could not find Kerberos library; install Kerberos or build with --disable-system-kerberos to use the default cache location.])])
      AC_CHECK_HEADER([krb5.h],
        , [AC_MSG_ERROR([Could not find Kerberos header; install Kerberos or build with --disable-system-kerberos to use the default cache location.])])
      KRB5_LIBS="-lkrb5"
    fi
  fi
  AM_CONDITIONAL(USE_SYSTEM_KERBEROS, test x"${ENABLE_SYSTEM_KERBEROS}" = "xyes")
  AC_SUBST(ENABLE_SYSTEM_KERBEROS)
])

AC_DEFUN_ONCE([IT_CHECK_FOR_PCSC],
[
  AC_MSG_CHECKING([whether to use the system libpcsclite install])
  AC_ARG_ENABLE([system-pcsc],
	      [AS_HELP_STRING(--enable-system-pcsc,use the system PCSC [[default=yes]])],
  [
    ENABLE_SYSTEM_PCSC="${enableval}"
  ],
  [
    ENABLE_SYSTEM_PCSC="yes"
  ])
  AC_MSG_RESULT(${ENABLE_SYSTEM_PCSC})
  if test x"${ENABLE_SYSTEM_PCSC}" = "xyes"; then
    dnl Check for PCSC headers and libraries.
    PKG_CHECK_MODULES(PCSC, libpcsclite,[LIBPCSC_FOUND=yes],[LIBPCSC_FOUND=no])
    if test "x${LIBPCSC_FOUND}" = xno
    then
      AC_MSG_ERROR([Could not find libpcsc; install libpcsc or build with --disable-system-pcsc to use dynamic loading.])
    fi
    AC_SUBST(PCSC_CFLAGS)
    AC_SUBST(PCSC_LIBS)
  fi
  AM_CONDITIONAL(USE_SYSTEM_PCSC, test x"${ENABLE_SYSTEM_PCSC}" = "xyes")
  AC_SUBST(ENABLE_SYSTEM_PCSC)
])

AC_DEFUN([IT_ENABLE_JAMVM],
[
  AC_MSG_CHECKING(whether to use JamVM as VM)
  AC_ARG_ENABLE([jamvm],
	      [AS_HELP_STRING(--enable-jamvm,use JamVM as VM [[default=no]])],
  [
    case "${enableval}" in
      yes)
        ENABLE_JAMVM=yes
        ;;
      *)
        ENABLE_JAMVM=no
        ;;
    esac
  ],
  [
    ENABLE_JAMVM=no
  ])

  AC_MSG_RESULT(${ENABLE_JAMVM})
  AM_CONDITIONAL(ENABLE_JAMVM, test x"${ENABLE_JAMVM}" = "xyes")
  AC_SUBST(ENABLE_JAMVM)
])

AC_DEFUN_ONCE([IT_WITH_JAMVM_SRC_ZIP],
[
  AC_MSG_CHECKING([for a JamVM source zip])
  AC_ARG_WITH([jamvm-src-zip],
	      [AS_HELP_STRING(--with-jamvm-src-zip,specify the location of the JamVM source zip)],
  [
    ALT_JAMVM_SRC_ZIP=${withval}
    if test "x${ALT_JAMVM_SRC_ZIP}" = "xno"; then
      ALT_JAMVM_SRC_ZIP="not specified"
    elif ! test -f ${ALT_JAMVM_SRC_ZIP} ; then
      AC_MSG_ERROR([Invalid JamVM source zip specified: ${ALT_JAMVM_SRC_ZIP}])
    fi
  ],
  [
    ALT_JAMVM_SRC_ZIP="not specified"
  ])
  AM_CONDITIONAL(USE_ALT_JAMVM_SRC_ZIP, test "x${ALT_JAMVM_SRC_ZIP}" != "xnot specified")
  AC_MSG_RESULT(${ALT_JAMVM_SRC_ZIP})
  AC_SUBST(ALT_JAMVM_SRC_ZIP)
])

AC_DEFUN_ONCE([IT_HAS_PAX],
[
  AC_MSG_CHECKING([if a PaX kernel is in use])
  if grep '^PaX' /proc/self/status >&AS_MESSAGE_LOG_FD 2>&1; then
    pax_active=yes;
  else
    pax_active=no;
  fi
  AC_MSG_RESULT([${pax_active}])
  AM_CONDITIONAL([USING_PAX], test x"${pax_active}" = "xyes")
])

AC_DEFUN_ONCE([IT_WITH_PAX],
[
  AC_REQUIRE([IT_HAS_PAX])
  PAX_DEFAULT=/usr/sbin/paxmark.sh
  AC_MSG_CHECKING([if a PaX utility was specified])
  AC_ARG_WITH([pax],
              [AS_HELP_STRING(--with-pax=COMMAND,the command used for pax marking)],
  [
    PAX_COMMAND="${withval}"
  ],
  [ 
    PAX_COMMAND=${pax_active}
  ])
  if test "x${PAX_COMMAND}" == "xyes"; then
    AC_MSG_RESULT([no])
    PAX_COMMAND=${PAX_DEFAULT}
    AC_MSG_NOTICE([PaX enabled but no tool specified; using ${PAX_DEFAULT}])
  else
    AC_MSG_RESULT(${PAX_COMMAND})
  fi
  if test "x${PAX_COMMAND}" != "xno"; then
    AC_MSG_CHECKING([if $PAX_COMMAND is a valid executable file])
    if test -x "${PAX_COMMAND}" && test -f "${PAX_COMMAND}"; then
      AC_MSG_RESULT([yes])
    else
      AC_MSG_RESULT([no])
      PAX_COMMAND=""
      AC_PATH_PROG(PAX_COMMAND, "paxmark.sh")
      if test -z "${PAX_COMMAND}"; then
        AC_PATH_PROG(PAX_COMMAND, "paxctl-ng")
      fi
      if test -z "${PAX_COMMAND}"; then
        AC_PATH_PROG(PAX_COMMAND, "chpax")
      fi
      if test -z "${PAX_COMMAND}"; then
        AC_PATH_PROG(PAX_COMMAND, "paxctl")
      fi
    fi
  else
    PAX_COMMAND=""
  fi
  if test -z "${PAX_COMMAND}"; then
    if test "x${pax_active}" = "xyes"; then
      AC_MSG_ERROR("No PaX utility found and running on a PaX kernel.")
    else
      AC_MSG_WARN("No PaX utility found.")
    fi
  fi
  if test -n "${PAX_COMMAND}"; then
    AC_MSG_CHECKING([which options to pass to ${PAX_COMMAND}])
    case "${host_cpu}" in
      i?86)
        PAX_COMMAND_ARGS="-msp"
        ;;
      *)
        PAX_COMMAND_ARGS="-m"
        ;;
    esac
    AC_MSG_RESULT(${PAX_COMMAND_ARGS})
  fi
  AM_CONDITIONAL(WITH_PAX, test "x${PAX_COMMAND}" != "x")
  AC_SUBST(PAX_COMMAND)
  AC_SUBST(PAX_COMMAND_ARGS)
])

AC_DEFUN([IT_USING_CACAO],[
  AC_REQUIRE([IT_FIND_JAVA])
  AC_CACHE_CHECK([if we are using CACAO as the build VM], it_cv_cacao, [
  if $JAVA -version 2>&1| grep '^CACAO' >&AS_MESSAGE_LOG_FD ; then
    it_cv_cacao=yes;
  else
    it_cv_cacao=no;
  fi
  ])
  USING_CACAO=$it_cv_cacao
  AC_SUBST(USING_CACAO)
  AM_CONDITIONAL(USING_CACAO, test "x${USING_CACAO}" = "xyes")
  AC_PROVIDE([$0])dnl
])

AC_DEFUN([IT_ENABLE_JAR_COMPRESSION],
[
  AC_MSG_CHECKING([whether to enable JAR compression])
  AC_ARG_ENABLE([jar-compression],
                [AS_HELP_STRING(--enable-jar-compression,compress built jars [[default=yes]])],
  [
    case "${enableval}" in
      no)
        enable_jar_compression=no
        ;;
      *)
        enable_jar_compression=yes
        ;;
    esac
  ],
  [
    enable_jar_compression=yes
  ])
  AC_MSG_RESULT([$enable_jar_compression])
  AM_CONDITIONAL([ENABLE_JAR_COMPRESSION], test x"${enable_jar_compression}" = "xyes")
])

AC_DEFUN_ONCE([IT_DETERMINE_VERSION],
[
  AC_MSG_CHECKING([which branch and release of IcedTea is being built])
  JAVA_VER=1.8.0
  dnl JAVA_SPEC_VER is the same for OpenJDK >= 9, but not for earlier versions
  dnl (e.g. 1.8.0 = 8, 1.7.0 = 7, etc.)
  JAVA_SPEC_VER=8
  JAVA_VENDOR=openjdk
  JDK_UPDATE_VERSION=392
  BUILD_VERSION=b08
  MILESTONE=fcs
  if test "x${MILESTONE}" = "xfcs"; then
    COMBINED_VERSION=${JDK_UPDATE_VERSION}-${BUILD_VERSION}
  else
    COMBINED_VERSION=${JDK_UPDATE_VERSION}-${MILESTONE}-${BUILD_VERSION}
  fi
  OPENJDK_VER=${JAVA_VER}_${COMBINED_VERSION}
  ICEDTEA_RELEASE=$(echo ${PACKAGE_VERSION} | sed 's#pre.*##')
  ICEDTEA_BRANCH=$(echo ${ICEDTEA_RELEASE}|sed 's|\.[[0-9]]$||')
  AC_MSG_RESULT([branch ${ICEDTEA_BRANCH}, release ${ICEDTEA_RELEASE} for OpenJDK ${OPENJDK_VER} (specification ${JAVA_SPEC_VER})])
  AC_SUBST([JAVA_VER])
  AC_SUBST([JAVA_SPEC_VER])
  AC_SUBST([JAVA_VENDOR])
  AC_SUBST([JDK_UPDATE_VERSION])
  AC_SUBST([BUILD_VERSION])
  AC_SUBST([MILESTONE])
  AC_SUBST([COMBINED_VERSION])
  AC_SUBST([OPENJDK_VER])
  AC_SUBST([ICEDTEA_RELEASE])
  AC_SUBST([ICEDTEA_BRANCH])
])

AC_DEFUN_ONCE([IT_ENABLE_NATIVE_DEBUGINFO],
[
  AC_MSG_CHECKING([whether to build native code with debugging information])
  AC_ARG_ENABLE([native-debuginfo],
                [AS_HELP_STRING(--enable-native-debuginfo,build with native code debuginfo [[default=yes]])],
  [
    case "${enableval}" in
      yes)
        enable_native_debuginfo=yes
        ;;
      *)
        enable_native_debuginfo=no
        ;;
    esac
  ],
  [
    enable_native_debuginfo=yes
  ])
  AC_MSG_RESULT([$enable_native_debuginfo])
  AM_CONDITIONAL([ENABLE_NATIVE_DEBUGINFO], test x"${enable_native_debuginfo}" = "xyes")
])

AC_DEFUN_ONCE([IT_ENABLE_JAVA_DEBUGINFO],
[
  AC_MSG_CHECKING([whether to build Java bytecode with debugging information])
  AC_ARG_ENABLE([java-debuginfo],
                [AS_HELP_STRING(--enable-java-debuginfo,build with Java bytecode debuginfo [[default=yes]])],
  [
    case "${enableval}" in
      yes)
        enable_java_debuginfo=yes
        ;;
      *)
        enable_java_debuginfo=no
        ;;
    esac
  ],
  [
    enable_java_debuginfo=yes
  ])
  AC_MSG_RESULT([$enable_java_debuginfo])
  AM_CONDITIONAL([ENABLE_JAVA_DEBUGINFO], test x"${enable_java_debuginfo}" = "xyes")
])

AC_DEFUN_ONCE([IT_ENABLE_IMPROVED_FONT_RENDERING],
[
  AC_REQUIRE([IT_CHECK_FOR_FREETYPE])
  AC_MSG_CHECKING([whether to use fontconfig to provide better font rendering])
  AC_ARG_ENABLE([improved-font-rendering],
                [AS_HELP_STRING(--enable-improved-font-rendering,build with fontconfig font rendering [[default=no]])],
  [
    case "${enableval}" in
      yes)
        enable_improved_font_rendering=yes
        ;;
      *)
        enable_improved_font_rendering=no
        ;;
    esac
  ],
  [
    enable_improved_font_rendering=no
  ])
  AC_MSG_RESULT([$enable_improved_font_rendering])
  AM_CONDITIONAL([ENABLE_IMPROVED_FONT_RENDERING], test x"${enable_improved_font_rendering}" = "xyes")
  if test "x${enable_improved_font_rendering}" = "xyes"; then
    dnl Check for Fontconfig+ headers and libraries.
    PKG_CHECK_MODULES(FONTCONFIG, fontconfig,[FONTCONFIG_FOUND=yes],[FONTCONFIG_FOUND=no])
    if test "x${FONTCONFIG_FOUND}" = xno
    then
      AC_MSG_ERROR([Improved font rendering support requires fontconfig. Either install fontconfig or --disable-improved-font-rendering])
    fi
    AC_SUBST(FONTCONFIG_CFLAGS)
    AC_SUBST(FONTCONFIG_LIBS)
  fi
])

AC_DEFUN_ONCE([IT_ARCH_HAS_NATIVE_HOTSPOT_PORT],
[
  AC_REQUIRE([IT_WITH_HOTSPOT_BUILD])
  AC_MSG_CHECKING([if a native HotSpot port is available in the ${HSBUILD} HotSpot build for ${host_cpu}])
  has_native_hotspot_port=yes;
  case "${host_cpu}" in
    aarch64|arm64) if test "x${HSBUILD}" = "xaarch32"; then has_native_hotspot_port=no; fi ;;
    i?86) ;;
    sparc) ;;
    x86_64) ;;
    powerpc64) ;;
    powerpc64le) ;;
    arm*) if test "x${HSBUILD}" != "xaarch32"; then has_native_hotspot_port=no; fi ;;
    *) has_native_hotspot_port=no;
  esac
  AC_MSG_RESULT([$has_native_hotspot_port])
])

AC_DEFUN_ONCE([IT_CHECK_FOR_RMDIR],
[
  IT_FIND_TOOL([RMDIR],[rmdir])
  AC_CACHE_CHECK([if ${RMDIR} supports --ignore-fail-on-non-empty], it_cv_RMDIR, [
    mkdir tmp.$$
    touch tmp.$$/t
    if ${RMDIR} --ignore-fail-on-non-empty tmp.$$ >&AS_MESSAGE_LOG_FD 2>&1; then
       it_cv_RMDIR=yes;
       RMDIR="${RMDIR} --ignore-fail-on-non-empty"
    else
       it_cv_RMDIR=no;
    fi
  ])
  rm -f tmp.$$/t
  ${RMDIR} tmp.$$
])

AC_DEFUN_ONCE([IT_DISABLE_SYSTEMTAP_TESTS],
[
  AC_MSG_CHECKING([whether to disable the execution of the SystemTap tests])
  AC_ARG_ENABLE([systemtap-tests],
                [AS_HELP_STRING(--disable-systemtap-tests,do not run the SystemTap tests via make check [[default=no]])],
  [
    case "${enableval}" in
      no)
        disable_systemtap_tests=yes
        ;;
      *)
        disable_systemtap_tests=no
        ;;
    esac
  ],
  [
    disable_systemtap_tests=no
  ])
  AC_MSG_RESULT([$disable_systemtap_tests])
  AM_CONDITIONAL([DISABLE_SYSTEMTAP_TESTS], test x"${disable_systemtap_tests}" = "xyes")
])

AC_DEFUN_ONCE([IT_JAVAC_OPTIONS_CHECK],
[
  AC_REQUIRE([IT_CHECK_JAVA_AND_JAVAC_WORK])
  AC_CACHE_CHECK([if the Java compiler supports -Xprefer:source], it_cv_xprefersource_works, [
  CLASS=Test.java
  BYTECODE=$(echo $CLASS|sed 's#\.java##')
  mkdir tmp.$$
  cd tmp.$$
  cat << \EOF > $CLASS
[/* [#]line __oline__ "configure" */

public class Test
{
    public static void main(String[] args)
    {
      System.out.println("Hello World!");
    }
}]
EOF
  mkdir build
  if $JAVAC -d build -cp . $JAVACFLAGS -Xprefer:source $CLASS >&AS_MESSAGE_LOG_FD 2>&1; then
    it_cv_xprefersource_works=yes;
  else
    it_cv_xprefersource_works=no;
  fi
  rm -f $CLASS build/*.class
  rmdir build
  cd ..
  rmdir tmp.$$
  ])
  AC_CACHE_CHECK([if the Java compiler supports setting the maximum heap size], it_cv_max_heap_size_works, [
  CLASS=Test.java
  BYTECODE=$(echo $CLASS|sed 's#\.java##')
  mkdir tmp.$$
  cd tmp.$$
  cat << \EOF > $CLASS
[/* [#]line __oline__ "configure" */

public class Test
{
    public static void main(String[] args)
    {
      System.out.println("Hello World!");
    }
}]
EOF
  mkdir build
  if $JAVAC -d build -cp . $JAVACFLAGS -J-Xmx1024m $CLASS >&AS_MESSAGE_LOG_FD 2>&1; then
    it_cv_max_heap_size_works=yes;
  else
    it_cv_max_heap_size_works=no;
  fi
  rm -f $CLASS build/*.class
  rmdir build
  cd ..
  rmdir tmp.$$
  ])
  AC_PROVIDE([$0])dnl
  AM_CONDITIONAL([COMPILER_SUPPORTS_XPREFERSOURCE], test x"${it_cv_xprefersource_works}" = "xyes")
  AM_CONDITIONAL([COMPILER_SUPPORTS_MAX_HEAP_SIZE], test x"${it_cv_max_heap_size_works}" = "xyes")
])

AC_DEFUN_ONCE([IT_DISABLE_PRECOMPILED_HEADERS],
[
  AC_MSG_CHECKING([whether to disable the use of pre-compiled headers])
  AC_ARG_ENABLE([precompiled-headers],
                [AS_HELP_STRING(--disable-precompiled-headers,do not use pre-compiled headers [[default=no]])],
  [
    case "${enableval}" in
      no)
        disable_precompiled_headers=yes
        ;;
      *)
        disable_precompiled_headers=no
        ;;
    esac
  ],
  [
    disable_precompiled_headers=no
  ])
  AC_MSG_RESULT([$disable_precompiled_headers])
  AM_CONDITIONAL([DISABLE_PRECOMPILED_HEADERS], test x"${disable_precompiled_headers}" = "xyes")
])

AC_DEFUN_ONCE([IT_ENABLE_OPENJDK_CHECKSUM],
[
  AC_REQUIRE([IT_WITH_OPENJDK_SRC_ZIP])
  AC_MSG_CHECKING([whether to enable checksumming of the specified OpenJDK tarball])
  AC_ARG_WITH([openjdk-checksum],
	      [AS_HELP_STRING(--with-openjdk-checksum,checksum the specified OpenJDK tarball [[default=yes]])],
  [
    OPENJDK_CHECKSUM=${withval}
  ],
  [
    OPENJDK_CHECKSUM="yes"
  ])
  AC_MSG_RESULT(${OPENJDK_CHECKSUM})
  if test "x${OPENJDK_CHECKSUM}" = "xno" -a "x${ALT_OPENJDK_SRC_ZIP}" = "xnot specified"; then
    AC_MSG_WARN([No OpenJDK source tarball specified; downloaded tarballs are always checksummed.])
  fi
  AM_CONDITIONAL(DISABLE_OPENJDK_CHECKSUM, test x"${OPENJDK_CHECKSUM}" = "xno")
  AM_CONDITIONAL(WITH_OPENJDK_CHECKSUM, test x"${OPENJDK_CHECKSUM}" != "xyes" -a x"${OPENJDK_CHECKSUM}" != "xno")
  AC_SUBST(OPENJDK_CHECKSUM)
])

AC_DEFUN_ONCE([IT_ENABLE_CACAO_CHECKSUM],
[
  AC_REQUIRE([IT_WITH_CACAO_SRC_ZIP])
  AC_MSG_CHECKING([whether to enable checksumming of the specified CACAO tarball])
  AC_ARG_WITH([cacao-checksum],
	      [AS_HELP_STRING(--with-cacao-checksum,checksum the specified CACAO tarball [[default=yes]])],
  [
    CACAO_CHECKSUM=${withval}
  ],
  [
    CACAO_CHECKSUM="yes"
  ])
  AC_MSG_RESULT(${CACAO_CHECKSUM})
  if test "x${CACAO_CHECKSUM}" = "xno" -a "x${ALT_CACAO_SRC_ZIP}" = "xnot specified"; then
    AC_MSG_WARN([No CACAO source tarball specified; downloaded tarballs are always checksummed.])
  fi
  AM_CONDITIONAL(DISABLE_CACAO_CHECKSUM, test x"${CACAO_CHECKSUM}" = "xno")
  AM_CONDITIONAL(WITH_CACAO_CHECKSUM, test x"${CACAO_CHECKSUM}" != "xyes" -a x"${CACAO_CHECKSUM}" != "xno")
  AC_SUBST(CACAO_CHECKSUM)
])

AC_DEFUN_ONCE([IT_ENABLE_JAMVM_CHECKSUM],
[
  AC_REQUIRE([IT_WITH_JAMVM_SRC_ZIP])
  AC_MSG_CHECKING([whether to enable checksumming of the specified JamVM tarball])
  AC_ARG_WITH([jamvm-checksum],
	      [AS_HELP_STRING(--with-jamvm-checksum,checksum the specified JamVM tarball [[default=yes]])],
  [
    JAMVM_CHECKSUM=${withval}
  ],
  [
    JAMVM_CHECKSUM="yes"
  ])
  AC_MSG_RESULT(${JAMVM_CHECKSUM})
  if test "x${JAMVM_CHECKSUM}" = "xno" -a "x${ALT_JAMVM_SRC_ZIP}" = "xnot specified"; then
    AC_MSG_WARN([No JamVM source tarball specified; downloaded tarballs are always checksummed.])
  fi
  AM_CONDITIONAL(DISABLE_JAMVM_CHECKSUM, test x"${JAMVM_CHECKSUM}" = "xno")
  AM_CONDITIONAL(WITH_JAMVM_CHECKSUM, test x"${JAMVM_CHECKSUM}" != "xyes" -a x"${JAMVM_CHECKSUM}" != "xno")
  AC_SUBST(JAMVM_CHECKSUM)
])

AC_DEFUN_ONCE([IT_ENABLE_HOTSPOT_CHECKSUM],
[
  AC_REQUIRE([IT_WITH_HOTSPOT_SRC_ZIP])
  AC_MSG_CHECKING([whether to enable checksumming of the specified HotSpot tarball])
  AC_ARG_WITH([hotspot-checksum],
	      [AS_HELP_STRING(--with-hotspot-checksum,checksum the specified HotSpot tarball [[default=yes]])],
  [
    HOTSPOT_CHECKSUM=${withval}
  ],
  [
    HOTSPOT_CHECKSUM="yes"
  ])
  AC_MSG_RESULT(${HOTSPOT_CHECKSUM})
  if test "x${HOTSPOT_CHECKSUM}" = "xno" -a "x${ALT_HOTSPOT_SRC_ZIP}" = "xnot specified"; then
    AC_MSG_WARN([No HotSpot source tarball specified; downloaded tarballs are always checksummed.])
  fi
  AM_CONDITIONAL(DISABLE_HOTSPOT_CHECKSUM, test x"${HOTSPOT_CHECKSUM}" = "xno")
  AM_CONDITIONAL(WITH_HOTSPOT_CHECKSUM, test x"${HOTSPOT_CHECKSUM}" != "xyes" -a x"${HOTSPOT_CHECKSUM}" != "xno")
  AC_SUBST(HOTSPOT_CHECKSUM)
])

AC_DEFUN_ONCE([IT_WITH_CACERTS_FILE],
[
  CACERTS_DEFAULT=${SYSTEM_JDK_DIR}/jre/lib/security/cacerts
  AC_MSG_CHECKING([whether to copy a certificate authority certificates (cacerts) file])
  AC_ARG_WITH([cacerts-file],
              [AS_HELP_STRING([--with-cacerts-file[[=PATH]]],specify the location of the cacerts file)],
  [
    ALT_CACERTS_FILE=${withval}
  ],
  [ 
    ALT_CACERTS_FILE="yes"
  ])
  AC_MSG_RESULT(${ALT_CACERTS_FILE})
  if test "x${ALT_CACERTS_FILE}" != "xno"; then
    if test "x${ALT_CACERTS_FILE}" = "xyes"; then
      AC_MSG_NOTICE([No cacerts file specified; using ${CACERTS_DEFAULT}])
      ALT_CACERTS_FILE=${CACERTS_DEFAULT} ;
    fi
    if test -h "${ALT_CACERTS_FILE}"; then
       ALT_CACERTS_FILE=$(${READLINK} -e ${ALT_CACERTS_FILE})
       AC_MSG_NOTICE([Resolved cacerts file symlink to ${ALT_CACERTS_FILE}])
    fi
    AC_MSG_CHECKING([if $ALT_CACERTS_FILE is a valid keystore file])
    if test -f "${ALT_CACERTS_FILE}" && \
     ${FILE} ${ALT_CACERTS_FILE} | ${GREP} 'Java KeyStore' >&AS_MESSAGE_LOG_FD 2>&1; then
      AC_MSG_RESULT([yes])
    else
      AC_MSG_RESULT([no])
      AC_MSG_WARN([No valid cacerts file found; one won't be passed to the OpenJDK build])
      ALT_CACERTS_FILE="no"
    fi
  fi
  AM_CONDITIONAL(USE_ALT_CACERTS_FILE, test "x${ALT_CACERTS_FILE}" != "xno")
  AC_SUBST(ALT_CACERTS_FILE)
])

AC_DEFUN_ONCE([IT_CHECK_FOR_FREETYPE],
[
dnl Check for freetype2 headers and libraries.
  PKG_CHECK_MODULES(FREETYPE2, freetype2,[]
	  ,[AC_MSG_ERROR([Could not find freetype2 - \
  Try installing freetype2-devel.])])
  AC_SUBST(FREETYPE2_CFLAGS)
  AC_SUBST(FREETYPE2_LIBS)
])

AC_DEFUN_ONCE([IT_CHECK_FOR_SCTP],
[
  AC_MSG_CHECKING([whether to use the system libsctp install])
  AC_ARG_ENABLE([system-sctp],
	      [AS_HELP_STRING(--enable-system-sctp,use the system SCTP [[default=yes]])],
  [
    ENABLE_SYSTEM_SCTP="${enableval}"
  ],
  [
    ENABLE_SYSTEM_SCTP="yes"
  ])
  AC_MSG_RESULT(${ENABLE_SYSTEM_SCTP})
  if test x"${ENABLE_SYSTEM_SCTP}" = "xyes"; then
    dnl Check for SCTP headers and libraries.
    AC_CHECK_LIB([sctp], [sctp_bindx],
        , [AC_MSG_ERROR([Could not find SCTP library; install SCTP or build with --disable-system-sctp to use the in-tree copy.])])
    AC_CHECK_HEADER([netinet/sctp.h],
        , [AC_MSG_ERROR([Could not find SCTP header; install SCTP or build with --disable-system-sctp to use the in-tree copy.])])
    SCTP_LIBS="-lsctp"
    AC_SUBST(SCTP_LIBS)
    ENABLE_SYSTEM_SCTP=true
  fi
  AM_CONDITIONAL(USE_SYSTEM_SCTP, test x"${ENABLE_SYSTEM_SCTP}" = "xtrue")
  AC_SUBST(ENABLE_SYSTEM_SCTP)
])

AC_DEFUN_ONCE([IT_WITH_CURVES],
[
  CURVE_DEFAULT=nist+
  AC_MSG_CHECKING([which set of elliptic curves to enable])
  AC_ARG_WITH([curves],
	      [AS_HELP_STRING(--with-curves,register the specified number of ECC curves [[default=nist+]])],
  [
    if test "x${withval}" = x || test "x${withval}" = xyes; then
        CURVE_SET=all;
    elif test "x${withval}" = xno; then
    	CURVE_SET="${CURVE_DEFAULT}";
    else
	CURVE_SET="${withval}";
    fi
  ],
  [
    CURVE_SET="${CURVE_DEFAULT}";
  ])
  AC_MSG_RESULT(${CURVE_SET})
  if test "x${CURVE_SET}" = "xnist" || test "x${CURVE_SET}" = "x3"; then
     CURVES=3;
  elif test "x${CURVE_SET}" = "xnist+" || test "x${CURVE_SET}" = "x4"; then
     CURVES=4;
  elif test "x${CURVE_SET}" = "xall"; then
     CURVES=all
  else
     AC_MSG_ERROR([Invalid value specified for curve set: "${CURVE_SET}"])
  fi
  AM_CONDITIONAL(USE_NIST_CURVES, test x"${CURVES}" = "x3")
  AM_CONDITIONAL(USE_NISTPLUS_CURVES, test x"${CURVES}" = "x4")
  AM_CONDITIONAL(USE_ALL_CURVES, test x"${CURVES}" = "xall")
  AC_SUBST(CURVES)
])

AC_DEFUN([IT_ENABLE_SPLIT_DEBUGINFO],
[
  AC_REQUIRE([IT_ENABLE_NATIVE_DEBUGINFO])
  AC_MSG_CHECKING([whether to split debuginfo into separate files])
  AC_ARG_ENABLE([split-debuginfo],
	      [AS_HELP_STRING(--enable-split-debuginfo,split debuginfo into separate files [[default=no]])],
  [
    case "${enableval}" in
      no)
	enable_split_debuginfo=no
        ;;
      *)
        enable_split_debuginfo=yes
        ;;
    esac
  ],
  [
        enable_split_debuginfo=no
  ])
  AC_MSG_RESULT([${enable_split_debuginfo}])
  if test x"${enable_split_debuginfo}" = "xyes"; then
    if test x"${enable_native_debuginfo}" = "xno"; then
      AC_MSG_WARN([disabling split debuginfo as native debuginfo is not enabled])
      enable_split_debuginfo=no
    else
      IT_FIND_TOOL([OBJCOPY], [objcopy])
    fi
  fi
  AM_CONDITIONAL([SPLIT_DEBUGINFO], test x"${enable_split_debuginfo}" = "xyes")
  AC_SUBST([enable_split_debuginfo])
])

AC_DEFUN_ONCE([IT_ENABLE_HEADLESS],
[
  AC_MSG_CHECKING([whether to perform a headless build of OpenJDK])
  AC_ARG_ENABLE([headless],
	      [AS_HELP_STRING(--enable-headless,perform a build of OpenJDK without graphical UI support [[default=no]])],
  [
    case "${enableval}" in
      no)
        ENABLE_HEADLESS=no
        ;;
      *)
        ENABLE_HEADLESS=yes
        ;;
    esac
  ],
  [
    ENABLE_HEADLESS="no"
  ])
  AC_MSG_RESULT(${ENABLE_HEADLESS})
  AM_CONDITIONAL(BUILD_HEADLESS, test x"${ENABLE_HEADLESS}" = "xyes")
  AC_SUBST(ENABLE_HEADLESS)
])

AC_DEFUN_ONCE([IT_ENABLE_CCACHE],
[
  AC_MSG_CHECKING([whether to use ccache to speed up recompilations])
  AC_ARG_ENABLE([ccache],
	      [AS_HELP_STRING(--enable-ccache,use ccache to speed up recompilations [[default=yes if ccache detected]])],
  [
    case "${enableval}" in
      no)
        ENABLE_CCACHE=no
        ;;
      *)
        ENABLE_CCACHE=yes
        ;;
    esac
    AC_MSG_RESULT(${ENABLE_CCACHE})
  ],
  [
    AC_MSG_RESULT([if available])
    AC_PATH_PROG(CCACHE, "ccache")
    if test -z "${CCACHE}"; then
      ENABLE_CCACHE="no"
    else
      ENABLE_CCACHE="yes"
    fi
  ])
  AC_MSG_CHECKING([if there is a ccache gcc wrapper on the PATH])
  ABS_CC=$(${WHICH} ${CC})
  REAL_GCC=$(${READLINK} -e ${ABS_CC})
  if test "x$(basename ${REAL_GCC})" = "xccache"; then
     AC_MSG_RESULT([yes; ${CC} resolves to ${REAL_GCC}])
     NO_CCACHE_PATH=$(sed "s#@<:@^:@:>@*$(dirname ${ABS_CC}):##g" <<< "${PATH}")
     AC_MSG_NOTICE([Using ${NO_CCACHE_PATH} as PATH])
  else
     AC_MSG_RESULT([no; ${CC} resolves to ${REAL_GCC}])
     NO_CCACHE_PATH=${PATH}
  fi
  AM_CONDITIONAL(USE_CCACHE, test x"${ENABLE_CCACHE}" = "xyes")
  AC_SUBST(ENABLE_CCACHE)
  AC_SUBST(NO_CCACHE_PATH)
])

AC_DEFUN_ONCE([IT_HSBUILD_WORKS_ON_THIS_ARCH],
[
  AC_REQUIRE([IT_WITH_HOTSPOT_BUILD])
  AC_MSG_CHECKING([if HotSpot build ${HSBUILD} works on ${host_cpu}])
  has_working_hotspot=yes;
  AC_MSG_RESULT([$has_working_hotspot])
  if test "x$has_working_hotspot" = "xno"; then
    AC_MSG_ERROR([${HSBUILD} is not supported on this platform.])
  fi
])

AC_DEFUN([IT_WITH_TAPSET_DIR],
[
  TAPSET_DEFAULT="${datadir}/systemtap/tapset"
  AC_MSG_CHECKING([which SystemTap tapset directory to use])
  AC_ARG_WITH([tapset-dir],
	      [AS_HELP_STRING(--with-tapset-dir,set the SystemTap tapset directory [[default=DATAROOTDIR/systemtap/tapset]])],
  [
    if test "x${withval}" = x || test "x${withval}" = xyes; then
      TAPSET_DIR="${TAPSET_DEFAULT}"
    else
      TAPSET_DIR="${withval}"
    fi
  ],
  [
    TAPSET_DIR="${TAPSET_DEFAULT}"
  ])
  if test "x${TAPSET_DIR}" = "xno"; then
    TAPSET_DIR=none
    TAPSET_DIR_SET=no
  else
    TAPSET_DIR_SET=yes
  fi
  AC_MSG_RESULT([${TAPSET_DIR}])
  AC_SUBST([TAPSET_DIR])
  AM_CONDITIONAL(WITH_TAPSET_DIR, test "x${TAPSET_DIR_SET}" = "xyes")
])

AC_DEFUN([IT_ARG_WITH],
  [AC_ARG_WITH(m4_translit([[$1]], [_], [-]),
   [AS_HELP_STRING([--with-m4_translit([$1], [_], [-])], [$2 [default=$3]])],
   [$4="${withval}"], [$4=$5])
  ]
)

AC_DEFUN_ONCE([IT_VENDOR_OPTS],
[
  VENDOR_NAME_DEFAULT="IcedTea"
  AC_MSG_CHECKING([what vendor name to use])
  IT_ARG_WITH([vendor_name],
    [Set vendor name. Among others, used to set the 'java.vendor'
     and 'java.vm.vendor' system properties.], IcedTea, VENDOR_NAME,
     ${VENDOR_NAME_DEFAULT})
  AC_MSG_RESULT([${VENDOR_NAME}])
  if test "x${VENDOR_NAME}" = x ||
     test "x${VENDOR_NAME}" = xyes ||
     test "x${VENDOR_NAME}" = xno; then
       AC_MSG_ERROR([--with-vendor-name must have a value])
  else
    case ${VENDOR_NAME} in
      *[![:print:]]*)
    	AC_MSG_ERROR([--with-vendor-name value contains non-printing characters: ${VENDOR_NAME}])
	;;
    esac
  fi
  AC_SUBST(VENDOR_NAME)

  VENDOR_URL_DEFAULT="https://icedtea.classpath.org"
  AC_MSG_CHECKING([what vendor URL to use])
  IT_ARG_WITH([vendor_url],
    [Set the 'java.vendor.url' system property],
    https://icedtea.classpath.org, VENDOR_URL, ${VENDOR_URL_DEFAULT})
  AC_MSG_RESULT([${VENDOR_URL}])
  if test "x${VENDOR_URL}" = x ||
     test "x${VENDOR_URL}" = xyes ||
     test "x${VENDOR_URL}" = xno; then
       AC_MSG_ERROR([--with-vendor-url must have a value])
  else
    case ${VENDOR_URL} in
      *[![:print:]]*)
        AC_MSG_ERROR([--with-vendor-url value contains non-printing characters: ${VENDOR_URL}])
	;;
    esac
  fi
  AC_SUBST(VENDOR_URL)

  VENDOR_BUG_URL_DEFAULT="https://icedtea.classpath.org/bugzilla"
  AC_MSG_CHECKING([what vendor bug URL to use])
  IT_ARG_WITH([vendor_bug_url],
    [Set the 'java.vendor.url.bug' system property],
    https://icedtea.classpath.org/bugzilla, VENDOR_BUG_URL, ${VENDOR_BUG_URL_DEFAULT})
  AC_MSG_RESULT([${VENDOR_BUG_URL}])
  if test "x${VENDOR_BUG_URL}" = x ||
     test "x${VENDOR_BUG_URL}" = xyes ||
     test "x${VENDOR_BUG_URL}" = xno; then
       AC_MSG_ERROR([--with-vendor-bug-url must have a value])
  else
    case ${VENDOR_BUG_URL} in
      *[![:print:]]*)
        AC_MSG_ERROR([--with-vendor-bug-url value contains non-printing characters: ${VENDOR_BUG_URL}])
	;;
    esac
  fi
  AC_SUBST(VENDOR_BUG_URL)

  VENDOR_VM_BUG_URL_DEFAULT=${VENDOR_BUG_URL}
  AC_MSG_CHECKING([what vendor VM bug URL to use])
  IT_ARG_WITH([vendor_vm_bug_url],
    [Sets the bug URL which will be displayed when the VM crashes],
    VENDOR_BUG_URL, VENDOR_VM_BUG_URL, ${VENDOR_VM_BUG_URL_DEFAULT})
  AC_MSG_RESULT([${VENDOR_VM_BUG_URL}])
  if test "x${VENDOR_VM_BUG_URL}" = x ||
     test "x${VENDOR_VM_BUG_URL}" = xyes ||
     test "x${VENDOR_VM_BUG_URL}" = xno; then
       AC_MSG_ERROR([--with-vendor-vm-bug-url must have a value])
  else
    case ${VENDOR_VM_BUG_URL} in
      *[![:print:]]*)
        AC_MSG_ERROR([--with-vendor-vm-bug-url value contains non-printing characters: ${VENDOR_VM_BUG_URL}])
	;;
    esac
  fi
  AC_SUBST(VENDOR_VM_BUG_URL)
])

AC_DEFUN_ONCE([IT_ARCH_HAS_JFR],
[
  AC_REQUIRE([IT_WITH_HOTSPOT_BUILD])
  AC_MSG_CHECKING([if the Java Flight Recorder is available in the ${HSBUILD} HotSpot build for ${host_cpu}])
  supports_jfr=yes;
  case "${host_cpu}" in
    aarch64|arm64) if test "x${HSBUILD}" = "xaarch32"; then supports_jfr=no; fi ;;
    i?86) ;;
    sparc) ;;
    x86_64) ;;
    powerpc64) ;;
    powerpc64le) ;;
    arm*) if test "x${HSBUILD}" != "xaarch32"; then supports_jfr=no; fi ;;
    *) supports_jfr=no;
  esac
  AC_MSG_RESULT([$supports_jfr])
])

AC_DEFUN_ONCE([IT_ENABLE_JFR],
[
  AC_REQUIRE([IT_ARCH_HAS_JFR])
  AC_MSG_CHECKING([whether to build OpenJDK with the Java Flight Recorder])
  AC_ARG_ENABLE([jfr],
	      [AS_HELP_STRING(--enable-jfr,compile OpenJDK with the Java Flight Recorder [[default=yes]])],
  [
    case "${enableval}" in
      no)
        ENABLE_JFR=no
        ;;
      *)
        ENABLE_JFR=yes
        ;;
    esac
  ],
  [
	ENABLE_JFR=yes
  ])
  AC_MSG_RESULT(${ENABLE_JFR})
  if test "x${ENABLE_JFR}" = "xyes" -a "x$supports_jfr" = "xno"; then
    AC_MSG_ERROR([The Java Flight Recorder is not supported on this platform.])
  fi
  AM_CONDITIONAL(USE_JFR, test x"${ENABLE_JFR}" = "xyes")
  AC_SUBST(ENABLE_JFR)
])
