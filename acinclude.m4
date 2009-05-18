 AC_DEFUN([SET_ARCH_DIRS],
[
  case "${host}" in
    x86_64-*-*)
      BUILD_ARCH_DIR=amd64
      INSTALL_ARCH_DIR=amd64
      JRE_ARCH_DIR=amd64
      ;;
    i?86-*-*)
      BUILD_ARCH_DIR=i586
      INSTALL_ARCH_DIR=i386
      JRE_ARCH_DIR=i386
      ARCH_PREFIX=${LINUX32}
      ;;
    alpha*-*-*)
      BUILD_ARCH_DIR=alpha
      INSTALL_ARCH_DIR=alpha
      JRE_ARCH_DIR=alpha
      ;;
    arm*-*-*)
      BUILD_ARCH_DIR=arm
      INSTALL_ARCH_DIR=arm
      JRE_ARCH_DIR=arm
      ;;
    mips-*-*)
      BUILD_ARCH_DIR=mips
      INSTALL_ARCH_DIR=mips
      JRE_ARCH_DIR=mips
       ;;
    mipsel-*-*)
      BUILD_ARCH_DIR=mipsel
      INSTALL_ARCH_DIR=mipsel
      JRE_ARCH_DIR=mipsel
       ;;
    powerpc-*-*)
      BUILD_ARCH_DIR=ppc
      INSTALL_ARCH_DIR=ppc
      JRE_ARCH_DIR=ppc
      ARCH_PREFIX=${LINUX32}
       ;;
    powerpc64-*-*)
      BUILD_ARCH_DIR=ppc64
      INSTALL_ARCH_DIR=ppc64
      JRE_ARCH_DIR=ppc64
       ;;
    sparc64-*-*)
      BUILD_ARCH_DIR=sparcv9
      INSTALL_ARCH_DIR=sparcv9
      JRE_ARCH_DIR=sparc64
       ;;
    s390-*-*)
      BUILD_ARCH_DIR=s390
      INSTALL_ARCH_DIR=s390
      JRE_ARCH_DIR=s390
      ARCH_PREFIX=${LINUX32}
       ;;
    *)
      BUILD_ARCH_DIR=`uname -m`
      INSTALL_ARCH_DIR=$BUILD_ARCH_DIR
      JRE_ARCH_DIR=$INSTALL_ARCH_DIR
      ;;
  esac
  AC_SUBST(BUILD_ARCH_DIR)
  AC_SUBST(INSTALL_ARCH_DIR)
  AC_SUBST(JRE_ARCH_DIR)
  AC_SUBST(ARCH_PREFIX)
])

AC_DEFUN([SET_OS_DIRS],
[
  case "${host_os}" in
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
      AC_MSG_ERROR([unsupported operating system ${host_os}])
      ;;
  esac
  AC_SUBST(BUILD_OS_DIR)
  AC_SUBST(OS_PATH)
])

AC_DEFUN([FIND_JAVAC],
[
  user_specified_javac=

  CLASSPATH_WITH_ECJ
  CLASSPATH_WITH_JAVAC

  if test "x${ECJ}" = x && test "x${JAVAC}" = x && test "x${user_specified_javac}" != xecj; then
      AC_MSG_ERROR([cannot find javac, try --with-ecj])
  fi
])

AC_DEFUN([CLASSPATH_WITH_ECJ],
[
  AC_ARG_WITH([ecj],
	      [AS_HELP_STRING(--with-ecj,bytecode compilation with ecj)],
  [
    if test "x${withval}" != x && test "x${withval}" != xyes && test "x${withval}" != xno; then
      CLASSPATH_CHECK_ECJ(${withval})
    else
      if test "x${withval}" != xno; then
        CLASSPATH_CHECK_ECJ
      fi
    fi
    user_specified_javac=ecj
  ],
  [ 
    CLASSPATH_CHECK_ECJ
  ])
  JAVAC="${ECJ} -nowarn"
  AC_SUBST(JAVAC)
])

AC_DEFUN([CLASSPATH_CHECK_ECJ],
[
  if test "x$1" != x; then
    if test -f "$1"; then
      ECJ="$1"
    else
      AC_PATH_PROG(ECJ, "$1")
    fi
  else
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
  fi
])

AC_DEFUN([CLASSPATH_WITH_JAVAC],
[
  AC_ARG_WITH([javac],
	      [AS_HELP_STRING(--with-javac,bytecode compilation with javac)],
  [
    if test "x${withval}" != x && test "x${withval}" != xyes && test "x${withval}" != xno; then
      CLASSPATH_CHECK_JAVAC(${withval})
    else
      if test "x${withval}" != xno; then
        CLASSPATH_CHECK_JAVAC
      fi
    fi
    user_specified_javac=javac
  ],
  [ 
    CLASSPATH_CHECK_JAVAC
  ])
  AC_SUBST(JAVAC)
])

AC_DEFUN([CLASSPATH_CHECK_JAVAC],
[
  if test "x$1" != x; then
    if test -f "$1"; then
      JAVAC="$1"
    else
      AC_PATH_PROG(JAVAC, "$1")
    fi
  else
    AC_PATH_PROG(JAVAC, "javac")
  fi
])

AC_DEFUN([FIND_JAVA],
[
  AC_ARG_WITH([java],
              [AS_HELP_STRING(--with-java,specify location of the 1.5 java vm)],
  [
    if test -f "${withval}"; then
      AC_MSG_CHECKING(java)
      JAVA="${withval}"
      AC_MSG_RESULT(${withval})
    else
      AC_PATH_PROG(JAVA, "${withval}")
    fi
  ],
  [
    JAVA=
  ])
  if test -z "${JAVA}"; then
    AC_PATH_PROG(JAVA, "gij")
  fi
  if test -z "${JAVA}"; then
    AC_PATH_PROG(JAVA, "java")
  fi
  if test -z "${JAVA}"; then
    AC_MSG_ERROR("A 1.5-compatible Java VM is required.")
  fi
  AC_SUBST(JAVA)
])

AC_DEFUN([WITH_OPENJDK_SRC_DIR],
[
  AC_MSG_CHECKING(for an OpenJDK source directory)
  AC_ARG_WITH([openjdk-src-dir],
              [AS_HELP_STRING(--with-openjdk-src-dir,specify the location of the openjdk sources)],
  [
    OPENJDK_SRC_DIR=${withval}
    AC_MSG_RESULT(${withval})
    conditional_with_openjdk_sources=true
  ],
  [ 
    conditional_with_openjdk_sources=false
    OPENJDK_SRC_DIR=`pwd`/openjdk
    AC_MSG_RESULT(${OPENJDK_SRC_DIR})
  ])
  AC_SUBST(OPENJDK_SRC_DIR)
  AM_CONDITIONAL(OPENJDK_SRC_DIR_FOUND, test "x${conditional_with_openjdk_sources}" = xtrue)
])

AC_DEFUN([FIND_ECJ_JAR],
[
  AC_ARG_WITH([ecj-jar],
              [AS_HELP_STRING(--with-ecj-jar,specify location of the ECJ jar)],
  [
    if test -f "${withval}"; then
      AC_MSG_CHECKING(for an ecj jar)
      ECJ_JAR="${withval}"
      AC_MSG_RESULT(${withval})
    fi
  ],
  [
    ECJ_JAR=
  ])
  if test -z "${ECJ_JAR}"; then
    AC_MSG_CHECKING(for eclipse-ecj.jar)
    if test -e "/usr/share/java/eclipse-ecj.jar"; then
      ECJ_JAR=/usr/share/java/eclipse-ecj.jar
      AC_MSG_RESULT(${ECJ_JAR})
    elif test -e "/usr/share/java/ecj.jar"; then
      ECJ_JAR=/usr/share/java/ecj.jar
      AC_MSG_RESULT(${ECJ_JAR})
    elif test -e "/usr/share/eclipse-ecj-3.3/lib/ecj.jar"; then
      ECJ_JAR=/usr/share/eclipse-ecj-3.3/lib/ecj.jar
      AC_MSG_RESULT(${ECJ_JAR})
    elif test -e "/usr/share/eclipse-ecj-3.2/lib/ecj.jar"; then
      ECJ_JAR=/usr/share/eclipse-ecj-3.2/lib/ecj.jar
      AC_MSG_RESULT(${ECJ_JAR})
    elif test -e "/usr/share/eclipse-ecj-3.1/lib/ecj.jar"; then
      ECJ_JAR=/usr/share/eclipse-ecj-3.1/lib/ecj.jar
      AC_MSG_RESULT(${ECJ_JAR})
    else
      AC_MSG_RESULT(no)
    fi
  fi
  if test -z "${ECJ_JAR}"; then
    AC_MSG_ERROR("A ECJ jar was not found.")
  fi
  AC_SUBST(ECJ_JAR)
])

AC_DEFUN([AC_CHECK_GCC_VERSION],
[
  AC_MSG_CHECKING([version of GCC])
  gcc_ver=`${CC} -dumpversion`
  gcc_major_ver=`echo ${gcc_ver}|cut -d'.' -f1`
  gcc_minor_ver=`echo ${gcc_ver}|cut -d'.' -f2`
  AM_CONDITIONAL(GCC_OLD, test ! ${gcc_major_ver} -ge 4 -a ${gcc_minor_ver} -ge 3)
  AC_MSG_RESULT([${gcc_ver} (major version ${gcc_major_ver}, minor version ${gcc_minor_ver})])
])

AC_DEFUN([FIND_JAVAH],
[
  AC_ARG_WITH([javah],
              [AS_HELP_STRING(--with-javah,specify location of the javah)],
  [
    if test -f "${withval}"; then
      AC_MSG_CHECKING(for javah)
      JAVAH="${withval}"
      AC_MSG_RESULT(${withval})
    else
      AC_PATH_PROG(JAVAH, "${withval}")
    fi
  ],
  [
    JAVAH=
  ])
  if test -z "${JAVAH}"; then
    AC_PATH_PROG(JAVAH, "gjavah")
  fi
  if test -z "${JAVAH}"; then
    AC_PATH_PROG(JAVAH, "javah")
  fi
  if test -z "${JAVAH}"; then
    AC_MSG_ERROR("javah was not found.")
  fi
  AC_SUBST(JAVAH)
])

AC_DEFUN([FIND_JAR],
[
  AC_ARG_WITH([jar],
              [AS_HELP_STRING(--with-jar,specify location of the jar)],
  [
    if test -f "${withval}"; then
      AC_MSG_CHECKING(for jar)
      JAR="${withval}"
      AC_MSG_RESULT(${withval})
    else
      AC_PATH_PROG(JAR, "${withval}")
    fi
  ],
  [
    JAR=
  ])
  if test -z "${JAR}"; then
    AC_PATH_PROG(JAR, "gjar")
  fi
  if test -z "${JAR}"; then
    AC_PATH_PROG(JAR, "jar")
  fi
  if test -z "${JAR}"; then
    AC_MSG_ERROR("jar was not found.")
  fi
  AC_MSG_CHECKING([whether jar supports @<file> argument])
  touch _config.txt
  cat >_config.list <<EOF
_config.txt
EOF
  if $JAR cf _config.jar @_config.list 2>/dev/null; then
    JAR_KNOWS_ATFILE=1
    AC_MSG_RESULT(yes)
  else
    JAR_KNOWS_ATFILE=
    AC_MSG_RESULT(no)
  fi
  AC_MSG_CHECKING([whether jar supports stdin file arguments])
  if cat _config.list | $JAR cf@ _config.jar 2>/dev/null; then
    JAR_ACCEPTS_STDIN_LIST=1
    AC_MSG_RESULT(yes)
  else
    JAR_ACCEPTS_STDIN_LIST=
    AC_MSG_RESULT(no)
  fi
  rm -f _config.list _config.jar
  AC_MSG_CHECKING([whether jar supports -J options at the end])
  if $JAR cf _config.jar _config.txt -J-Xmx896m 2>/dev/null; then
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

AC_DEFUN([FIND_RMIC],
[
  AC_ARG_WITH([rmic],
              [AS_HELP_STRING(--with-rmic,specify location of the rmic)],
  [
    if test -f "${withval}"; then
      AC_MSG_CHECKING(for rmic)
      RMIC="${withval}"
      AC_MSG_RESULT(${withval})
    else
      AC_PATH_PROG(RMIC, "${withval}")
    fi
  ],
  [
    RMIC=
  ])
  if test -z "${RMIC}"; then
    AC_PATH_PROG(RMIC, "grmic")
  fi
  if test -z "${RMIC}"; then
    AC_PATH_PROG(RMIC, "rmic")
  fi
  if test -z "${RMIC}"; then
    AC_MSG_ERROR("rmic was not found.")
  fi
  AC_SUBST(RMIC)
])

AC_DEFUN([FIND_ENDORSED_JARS],
[
  AC_MSG_CHECKING(for endorsed jars dir)
  AC_ARG_WITH([endorsed-dir],
              [AS_HELP_STRING(--with-endorsed-dir,specify directory of endorsed jars (xalan-j2.jar, xalan-j2-serializer.jar, xerces-j2.jar))],
  [
    if test "x${withval}" = "xno"; then
        ENDORSED_JARS="${withval}"
        AC_MSG_RESULT(${withval})
    else if test -f "${withval}/xalan-j2.jar"; then
      if test -f "${withval}/xalan-j2-serializer.jar"; then
        if test -f "${withval}/xerces-j2.jar"; then
          ENDORSED_JARS="${withval}"
          AC_MSG_RESULT(${withval})
        fi
      fi
    fi
  ],
  [
    ENDORSED_JARS=
  ])
  if test -z "${ENDORSED_JARS}"; then
    if test -f "/usr/share/java/xalan-j2.jar"; then
      if test -f "/usr/share/java/xalan-j2-serializer.jar"; then
        if test -f "/usr/share/java/xerces-j2.jar"; then
          ENDORSED_JARS="/usr/share/java/xalan-j2.jar /usr/share/java/xalan-j2-serializer.jar /usr/share/java/xerces-j2.jar"
          AC_MSG_RESULT(/usr/share/java)
        fi
      fi
    fi
    if test -z "${ENDORSED_JARS}"; then
      AC_MSG_RESULT(missing)
    fi
  fi
  if test -z "${ENDORSED_JARS}"; then
    AC_MSG_ERROR("A directory containing required jars (xalan-j2.jar, xalan-j2-serializer.jar, xerces-j2.jar) was not found.")
  fi
  AC_SUBST(ENDORSED_JARS)
])

AC_DEFUN([WITH_OPENJDK_SRC_ZIP],
[
  AC_MSG_CHECKING(for an OpenJDK source zip)
  AC_ARG_WITH([openjdk-src-zip],
              [AS_HELP_STRING(--with-openjdk-src-zip,specify the location of the openjdk source zip)],
  [
    ALT_OPENJDK_SRC_ZIP=${withval}
    AM_CONDITIONAL(USE_ALT_OPENJDK_SRC_ZIP, test x = x)
    AC_SUBST(ALT_OPENJDK_SRC_ZIP)
  ],
  [ 
    ALT_OPENJDK_SRC_ZIP="not specified"
    AM_CONDITIONAL(USE_ALT_OPENJDK_SRC_ZIP, test x != x)
  ])
  AC_MSG_RESULT(${ALT_OPENJDK_SRC_ZIP})
  AC_SUBST(ALT_OPENJDK_SRC_ZIP)
])

AC_DEFUN([WITH_ALT_JAR_BINARY],
[
  AC_MSG_CHECKING(for an alternate jar command)
  AC_ARG_WITH([alt-jar],
              [AS_HELP_STRING(--with-alt-jar, specify the location of an alternate jar binary to use for building)],
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

AC_DEFUN([FIND_XALAN2_JAR],
[
  AC_MSG_CHECKING(xalan2 jar)
  AC_ARG_WITH([xalan2-jar],
              [AS_HELP_STRING(--with-xalan2-jar,specify location of the xalan2 jar)],
  [
    if test -f "${withval}" ; then
      XALAN2_JAR="${withval}"
    fi
  ],
  [
    XALAN2_JAR=
  ])
  if test -z "${XALAN2_JAR}"; then
    if test -e "/usr/share/java/xalan-j2.jar"; then
      XALAN2_JAR=/usr/share/java/xalan-j2.jar
    elif test -e "/usr/share/java/xalan2.jar"; then
      XALAN2_JAR=/usr/share/java/xalan2.jar
    elif test -e "/usr/share/xalan/lib/xalan.jar"; then
      XALAN2_JAR=/usr/share/xalan/lib/xalan.jar
    else
      AC_MSG_RESULT(no)
    fi
  fi
  if test -z "${XALAN2_JAR}"; then
    AC_MSG_ERROR("A xalan2 jar was not found.")
  fi
  AC_MSG_RESULT(${XALAN2_JAR})
  AC_SUBST(XALAN2_JAR)
])

AC_DEFUN([FIND_XALAN2_SERIALIZER_JAR],
[
  AC_MSG_CHECKING(for xalan2 serializer jar)
  AC_ARG_WITH([xalan2-serializer-jar],
              [AS_HELP_STRING(--with-xalan2-serializer-jar,specify location of the xalan2-serializer jar)],
  [
    if test -f "${withval}" ; then
      XALAN2_SERIALIZER_JAR="${withval}"
    fi
  ],
  [
    XALAN2_SERIALIZER_JAR=
  ])
  if test -z "${XALAN2_SERIALIZER_JAR}"; then
    if test -e "/usr/share/java/xalan-j2-serializer.jar"; then
      XALAN2_SERIALIZER_JAR=/usr/share/java/xalan-j2-serializer.jar
    elif test -e "/usr/share/xalan-serializer/lib/serializer.jar"; then
      XALAN2_SERIALIZER_JAR=/usr/share/xalan-serializer/lib/serializer.jar
    elif test -e "/usr/share/java/serializer.jar"; then
      XALAN2_SERIALIZER_JAR=/usr/share/java/serializer.jar
    else
      AC_MSG_RESULT(no)
    fi
  fi
  if test -z "${XALAN2_SERIALIZER_JAR}"; then
    AC_MSG_ERROR("A xalan2-serializer jar was not found.")
  fi
  AC_MSG_RESULT(${XALAN2_SERIALIZER_JAR})
  AC_SUBST(XALAN2_SERIALIZER_JAR)
])

AC_DEFUN([FIND_XERCES2_JAR],
[
  AC_MSG_CHECKING(for xerces2 jar)
  AC_ARG_WITH([xerces2-jar],
              [AS_HELP_STRING(--with-xerces2-jar,specify location of the xerces2 jar)],
  [
    if test -f "${withval}" ; then
      XERCES2_JAR="${withval}"
    fi
  ],
  [
    XERCES2_JAR=
  ])
  if test -z "${XERCES2_JAR}"; then
    if test -e "/usr/share/java/xerces-j2.jar"; then
      XERCES2_JAR=/usr/share/java/xerces-j2.jar
    elif test -e "/usr/share/java/xerces2.jar"; then
      XERCES2_JAR=/usr/share/java/xerces2.jar
    elif test -e "/usr/share/xerces-2/lib/xercesImpl.jar"; then
      XERCES2_JAR=/usr/share/xerces-2/lib/xercesImpl.jar
    elif test -e "/usr/share/java/xercesImpl.jar"; then
      XERCES2_JAR=/usr/share/java/xercesImpl.jar
    else
      AC_MSG_RESULT(no)
    fi
  fi
  if test -z "${XERCES2_JAR}"; then
    AC_MSG_ERROR("A xerces2 jar was not found.")
  fi
  AC_MSG_RESULT(${XERCES2_JAR})
  AC_SUBST(XERCES2_JAR)
])

AC_DEFUN([FIND_NETBEANS],
[
  AC_ARG_WITH([netbeans],
              [AS_HELP_STRING(--with-netbeans,specify location of netbeans)],
  [
    if test -f "${withval}"; then
      AC_MSG_CHECKING(netbeans)
      NETBEANS="${withval}"
      AC_MSG_RESULT(${withval})
    else
      AC_PATH_PROG(NETBEANS, "${withval}")
    fi
  ],
  [
    NETBEANS=
  ])
  if test -z "${NETBEANS}"; then
    AC_PATH_PROG(NETBEANS, "netbeans")
  fi
  if test -z "${NETBEANS}"; then
    AC_MSG_ERROR("NetBeans was not found.")
  fi
  AC_SUBST(NETBEANS)
])

AC_DEFUN([FIND_NETBEANS],
[
  AC_ARG_WITH([netbeans],
              [AS_HELP_STRING(--with-netbeans,specify location of netbeans)],
  [
    if test -f "${withval}"; then
      AC_MSG_CHECKING(netbeans)
      NETBEANS="${withval}"
      AC_MSG_RESULT(${withval})
    else
      AC_PATH_PROG(NETBEANS, "${withval}")
    fi
  ],
  [
    NETBEANS=
  ])
  if test -z "${NETBEANS}"; then
    AC_PATH_PROG(NETBEANS, "netbeans")
  fi
  if test -z "${NETBEANS}"; then
    AC_MSG_ERROR("NetBeans was not found.")
  fi
  AC_SUBST(NETBEANS)
])

AC_DEFUN([FIND_RHINO_JAR],
[
  AC_MSG_CHECKING(whether to include Javascript support via Rhino)
  AC_ARG_WITH([rhino],
              [AS_HELP_STRING(--with-rhino,specify location of the rhino jar)],
  [
    case "${withval}" in
      yes)
	RHINO_JAR=yes
        ;;
      no)
        RHINO_JAR=no
        ;;
      *)
    	if test -f "${withval}"; then
          RHINO_JAR="${withval}"
        else
	  AC_MSG_RESULT([not found])
          AC_MSG_ERROR("The rhino jar ${withval} was not found.")
        fi
	;;
     esac
  ],
  [
    RHINO_JAR=yes
  ])
  if test x"${RHINO_JAR}" = "xyes"; then
    if test -e "/usr/share/java/rhino.jar"; then
      RHINO_JAR=/usr/share/java/rhino.jar
    elif test -e "/usr/share/java/js.jar"; then
      RHINO_JAR=/usr/share/java/js.jar
    elif test -e "/usr/share/rhino-1.6/lib/js.jar"; then
      RHINO_JAR=/usr/share/rhino-1.6/lib/js.jar
    fi
    if test x"${RHINO_JAR}" = "xyes"; then
      AC_MSG_RESULT([not found])
      AC_MSG_ERROR("A rhino jar was not found in /usr/share/java as either rhino.jar or js.jar.")
    fi
  fi
  AC_MSG_RESULT(${RHINO_JAR})
  AM_CONDITIONAL(WITH_RHINO, test x"${RHINO_JAR}" != "xno")
  AC_SUBST(RHINO_JAR)
])

AC_DEFUN([FIND_PULSEAUDIO],
[
  AC_PATH_PROG(PULSEAUDIO_BIN, "pulseaudio")
  if test -z "${PULSEAUDIO_BIN}"; then
    AC_MSG_ERROR("pulseaudio was not found.")
  fi
  AC_SUBST(PULSEAUDIO_BIN)
])

AC_DEFUN([ENABLE_OPTIMIZATIONS],
[
  AC_MSG_CHECKING(whether to disable optimizations)
  AC_ARG_ENABLE([optimizations],
                [AS_HELP_STRING(--disable-optimizations,build with -O0 -g [[default=no]])],
  [
    case "${enableval}" in
      no)
        AC_MSG_RESULT([yes, building with -O0 -g])
        enable_optimizations=no
        ;;
      *)
        AC_MSG_RESULT([no])
        enable_optimizations=yes
        ;;
    esac
  ],
  [
    enable_optimizations=yes
  ])
  AM_CONDITIONAL([ENABLE_OPTIMIZATIONS], test x"${enable_optimizations}" = "xyes")
])

AC_DEFUN([FIND_TOOL],
[AC_PATH_TOOL([$1],[$2])
 if test x"$$1" = x ; then
   AC_MSG_ERROR([$2 program not found in PATH])
 fi
 AC_SUBST([$1])
])

AC_DEFUN([ENABLE_ZERO_BUILD],
[
  AC_MSG_CHECKING(whether to use the zero-assembler port)
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
    else
      case "${host}" in
        i?86-*-*) ;;
        sparc*-*-*) ;;
        x86_64-*-*) ;;
        *)
          if test "x${WITH_CACAO}" != xno; then
            use_zero=no
          else
            use_zero=yes
          fi
          ;;
      esac
    fi
  ])
  AC_MSG_RESULT($use_zero)
  AM_CONDITIONAL(ZERO_BUILD, test "x${use_zero}" = xyes)

  use_core=no
  if test "x${WITH_CACAO}" != "xno"; then
    use_core=yes;
  elif test "x${use_zero}" = "xyes"; then
    if test "x${use_shark}" = "xno"; then
      use_core=yes;
    fi
  fi
  AM_CONDITIONAL(CORE_BUILD, test "x${use_core}" = xyes)

  ZERO_LIBARCH=
  ZERO_BITSPERWORD=
  ZERO_ENDIANNESS=
  ZERO_ARCHDEF=
  ZERO_ARCHFLAG=
  if test "x${use_zero}" = xyes; then
    ZERO_LIBARCH="${INSTALL_ARCH_DIR}"
    dnl can't use AC_CHECK_SIZEOF on multilib
    case "${ZERO_LIBARCH}" in
      i386|ppc|s390|sparc)
        ZERO_BITSPERWORD=32
        ;;
      amd64|ppc64|s390x|sparc64)
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
    dnl multilib machines need telling which mode to build for
    case "${ZERO_LIBARCH}" in
      i386|ppc|sparc)
        ZERO_ARCHFLAG="-m32"
        ;;
      s390)
        ZERO_ARCHFLAG="-m31"
        ;;
      amd64|ppc64|s390x|sparc64)
        ZERO_ARCHFLAG="-m64"
        ;;
    esac
  fi
  AC_SUBST(ZERO_LIBARCH)
  AC_SUBST(ZERO_BITSPERWORD)
  AC_SUBST(ZERO_ENDIANNESS)
  AC_SUBST(ZERO_ARCHDEF)
  AC_SUBST(ZERO_ARCHFLAG)
  AC_CONFIG_FILES([platform_zero])
  AC_CONFIG_FILES([jvm.cfg])
  AC_CONFIG_FILES([ergo.c])
])

AC_DEFUN([SET_SHARK_BUILD],
[
  AC_MSG_CHECKING(whether to use the Shark JIT)
  shark_selected=no
  AC_ARG_ENABLE([shark], [AS_HELP_STRING(--enable-shark, use Shark JIT)],
  [
    case "${enableval}" in
      no)
        ;;
      *)
        shark_selected=yes
        ;;
    esac
  ])

  use_shark=no
  if test "x${shark_selected}" = "xyes"; then
      use_shark=yes
  fi
  AC_MSG_RESULT($use_shark)

  AM_CONDITIONAL(SHARK_BUILD, test "x${use_shark}" = xyes)
])

AC_DEFUN([AC_CHECK_ENABLE_CACAO],
[
  AC_MSG_CHECKING(whether to use CACAO as VM)
  AC_ARG_ENABLE([cacao],
	      [AS_HELP_STRING(--enable-cacao,use CACAO as VM [[default=no]])],
  [
    WITH_CACAO="${enableval}"
  ],
  [
    WITH_CACAO=no
  ])

  AC_MSG_RESULT(${WITH_CACAO})
  AM_CONDITIONAL(WITH_CACAO, test x"${WITH_CACAO}" = "xyes")
  AC_SUBST(WITH_CACAO)
])

AC_DEFUN([AC_CHECK_WITH_CACAO_HOME],
[
  AC_MSG_CHECKING(for CACAO home directory)
  AC_ARG_WITH([cacao-home],
              [AS_HELP_STRING([--with-cacao-home],
                              [CACAO home directory [[default=/usr/local/cacao]]])],
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
                CACAO_IMPORT_PATH="\$(abs_top_builddir)/cacao/install"
                AM_CONDITIONAL(USE_SYSTEM_CACAO, false)
              ])
  AC_MSG_RESULT(${CACAO_IMPORT_PATH})
  AC_SUBST(CACAO_IMPORT_PATH)
])

AC_DEFUN([AC_CHECK_WITH_CACAO_SRC_ZIP],
[
  AC_MSG_CHECKING(for a CACAO source zip)
  AC_ARG_WITH([cacao-src-zip],
              [AS_HELP_STRING(--with-cacao-src-zip,specify the location of the CACAO source zip)],
  [
    ALT_CACAO_SRC_ZIP=${withval}
    AM_CONDITIONAL(USE_ALT_CACAO_SRC_ZIP, test x = x)
  ],
  [ 
    ALT_CACAO_SRC_ZIP="not specified"
    AM_CONDITIONAL(USE_ALT_CACAO_SRC_ZIP, test x != x)
  ])
  AC_MSG_RESULT(${ALT_CACAO_SRC_ZIP})
  AC_SUBST(ALT_CACAO_SRC_ZIP)
])

AC_DEFUN([ENABLE_HG],
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
    case "${project}" in
      jdk7)
        enable_hg=no
        ;;
      *)
        enable_hg=yes
        ;;
    esac
  ])
  AC_MSG_RESULT([${enable_hg}])
  AM_CONDITIONAL([USE_HG], test x"${enable_hg}" = "xyes")
])

AC_DEFUN([WITH_VERSION_SUFFIX],
[
  AC_MSG_CHECKING(if a version suffix has been specified)
  AC_ARG_WITH([version-suffix],
              [AS_HELP_STRING(--with-version-suffix,appends the given text to the JDK version)],
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

AC_DEFUN([WITH_PROJECT],
[
  AC_MSG_CHECKING(which OpenJDK project is being used)
  AC_ARG_WITH([project],
              [AS_HELP_STRING(--with-project,choose the OpenJDK project to use: jdk7 closures cvmi caciocavallo bsd nio2 [[default=jdk7]])],
  [
    case "${withval}" in
      yes)
	project=jdk7
        ;;
      no)
	project=jdk7
	;;
      *)
        project=${withval}
        ;;
    esac
  ],
  [
    project=jdk7
  ])
  AC_MSG_RESULT([${project}])
  AC_SUBST(PROJECT_NAME, $project)
  AM_CONDITIONAL([USE_CLOSURES], test x"${project}" = "xclosures")
  AM_CONDITIONAL([USE_CVMI], test x"${project}" = "xcvmi")
  AM_CONDITIONAL([USE_CACIOCAVALLO], test x"${project}" = "xcaciocavallo")
  AM_CONDITIONAL([USE_BSD], test x"${project}" = "xbsd")
  AM_CONDITIONAL([USE_NIO2], test x"${project}" = "xnio2")
])

AC_DEFUN([AC_CHECK_WITH_GCJ],
[
  AC_MSG_CHECKING([whether to compile ecj natively])
  AC_ARG_WITH([gcj],
	      [AS_HELP_STRING(--with-gcj,location of gcj for natively compiling ecj)],
  [
    GCJ="${withval}"
  ],
  [ 
    GCJ="no"
  ])
  AC_MSG_RESULT([${GCJ}])
  if test "x${GCJ}" = xyes; then
    AC_PATH_TOOL([GCJ],[gcj])
  fi
  AM_CONDITIONAL([BUILD_NATIVE_ECJ], test x"${GCJ}" != xno)
  AC_SUBST([GCJ])
])

AC_DEFUN([AC_CHECK_WITH_HOTSPOT_BUILD],
[
  DEFAULT_BUILD="default"
  AC_MSG_CHECKING([which HotSpot build to use])
  AC_ARG_WITH([hotspot-build],
	      [AS_HELP_STRING(--with-hotspot-build,the HotSpot build to use)],
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
])

AC_DEFUN([WITH_HOTSPOT_SRC_ZIP],
[
  AC_MSG_CHECKING(for a HotSpot source zip)
  AC_ARG_WITH([hotspot-src-zip],
              [AS_HELP_STRING(--with-hotspot-src-zip,specify the location of the hotspot source zip)],
  [
    ALT_HOTSPOT_SRC_ZIP=${withval}
    AM_CONDITIONAL(USE_ALT_HOTSPOT_SRC_ZIP, test x = x)
  ],
  [ 
    ALT_HOTSPOT_SRC_ZIP="not specified"
    AM_CONDITIONAL(USE_ALT_HOTSPOT_SRC_ZIP, test x != x)
  ])
  AC_MSG_RESULT(${ALT_HOTSPOT_SRC_ZIP})
  AC_SUBST(ALT_HOTSPOT_SRC_ZIP)
])

AC_DEFUN([WITH_CORBA_SRC_ZIP],
[
  AC_MSG_CHECKING(for a CORBA source zip)
  AC_ARG_WITH([corba-src-zip],
              [AS_HELP_STRING(--with-corba-src-zip,specify the location of the corba source zip)],
  [
    ALT_CORBA_SRC_ZIP=${withval}
    AM_CONDITIONAL(USE_ALT_CORBA_SRC_ZIP, test x = x)
  ],
  [ 
    ALT_CORBA_SRC_ZIP="not specified"
    AM_CONDITIONAL(USE_ALT_CORBA_SRC_ZIP, test x != x)
  ])
  AC_MSG_RESULT(${ALT_CORBA_SRC_ZIP})
  AC_SUBST(ALT_CORBA_SRC_ZIP)
])

AC_DEFUN([WITH_JAXP_SRC_ZIP],
[
  AC_MSG_CHECKING(for a JAXP source zip)
  AC_ARG_WITH([jaxp-src-zip],
              [AS_HELP_STRING(--with-jaxp-src-zip,specify the location of the jaxp source zip)],
  [
    ALT_JAXP_SRC_ZIP=${withval}
    AM_CONDITIONAL(USE_ALT_JAXP_SRC_ZIP, test x = x)
  ],
  [ 
    ALT_JAXP_SRC_ZIP="not specified"
    AM_CONDITIONAL(USE_ALT_JAXP_SRC_ZIP, test x != x)
  ])
  AC_MSG_RESULT(${ALT_JAXP_SRC_ZIP})
  AC_SUBST(ALT_JAXP_SRC_ZIP)
])

AC_DEFUN([WITH_JAXWS_SRC_ZIP],
[
  AC_MSG_CHECKING(for a JAXWS source zip)
  AC_ARG_WITH([jaxws-src-zip],
              [AS_HELP_STRING(--with-jaxws-src-zip,specify the location of the jaxws source zip)],
  [
    ALT_JAXWS_SRC_ZIP=${withval}
    AM_CONDITIONAL(USE_ALT_JAXWS_SRC_ZIP, test x = x)
  ],
  [ 
    ALT_JAXWS_SRC_ZIP="not specified"
    AM_CONDITIONAL(USE_ALT_JAXWS_SRC_ZIP, test x != x)
  ])
  AC_MSG_RESULT(${ALT_JAXWS_SRC_ZIP})
  AC_SUBST(ALT_JAXWS_SRC_ZIP)
])

AC_DEFUN([WITH_JDK_SRC_ZIP],
[
  AC_MSG_CHECKING(for a JDK source zip)
  AC_ARG_WITH([jdk-src-zip],
              [AS_HELP_STRING(--with-jdk-src-zip,specify the location of the jdk source zip)],
  [
    ALT_JDK_SRC_ZIP=${withval}
    AM_CONDITIONAL(USE_ALT_JDK_SRC_ZIP, test x = x)
  ],
  [ 
    ALT_JDK_SRC_ZIP="not specified"
    AM_CONDITIONAL(USE_ALT_JDK_SRC_ZIP, test x != x)
  ])
  AC_MSG_RESULT(${ALT_JDK_SRC_ZIP})
  AC_SUBST(ALT_JDK_SRC_ZIP)
])

AC_DEFUN([WITH_LANGTOOLS_SRC_ZIP],
[
  AC_MSG_CHECKING(for a langtools source zip)
  AC_ARG_WITH([langtools-src-zip],
              [AS_HELP_STRING(--with-langtools-src-zip,specify the location of the langtools source zip)],
  [
    ALT_LANGTOOLS_SRC_ZIP=${withval}
    AM_CONDITIONAL(USE_ALT_LANGTOOLS_SRC_ZIP, test x = x)
  ],
  [ 
    ALT_LANGTOOLS_SRC_ZIP="not specified"
    AM_CONDITIONAL(USE_ALT_LANGTOOLS_SRC_ZIP, test x != x)
  ])
  AC_MSG_RESULT(${ALT_LANGTOOLS_SRC_ZIP})
  AC_SUBST(ALT_LANGTOOLS_SRC_ZIP)
])

AC_DEFUN([ENABLE_HG],
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

AC_DEFUN([AC_CHECK_WITH_HG_REVISION],
[
  AC_MSG_CHECKING([which Mercurial revision to use])
  AC_ARG_WITH([hg-revision],
	      [AS_HELP_STRING(--with-hg-revision,the Mercurial revision to use)],
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

AC_DEFUN([AC_CHECK_FOR_GCJ_JDK],
[
  AC_MSG_CHECKING([for a GCJ JDK home directory])
  AC_ARG_WITH([gcj-home],
	      [AS_HELP_STRING([--with-gcj-home],
                              [gcj home directory \
                               (default is /usr/lib/jvm/java-gcj or /usr/lib/jvm/gcj-jdk)])],
              [
                if test "x${withval}" = xyes
                then
                  SYSTEM_GCJ_DIR=
                elif test "x${withval}" = xno
                then
	          SYSTEM_GCJ_DIR=
	        else
                  SYSTEM_GCJ_DIR=${withval}
                fi
              ],
              [
	        SYSTEM_GCJ_DIR=
              ])
  if test -z "${SYSTEM_GCJ_DIR}"; then
    for dir in /usr/lib/jvm/java-gcj /usr/lib/jvm/gcj-jdk /usr/lib/jvm/cacao ; do
       if test -d $dir; then
         SYSTEM_GCJ_DIR=$dir
	 break
       fi
    done
  fi
  AC_MSG_RESULT(${SYSTEM_GCJ_DIR})
  if ! test -d "${SYSTEM_GCJ_DIR}"; then
    AC_MSG_ERROR("A GCJ JDK home directory could not be found.")
  fi
  AC_SUBST(SYSTEM_GCJ_DIR)
])

AC_DEFUN([AC_CHECK_FOR_OPENJDK],
[
  AC_MSG_CHECKING([for an existing OpenJDK installation])
  AC_ARG_WITH([openjdk-home],
              [AS_HELP_STRING([--with-openjdk-home],
                              [OpenJDK home directory \
                               (default is /usr/lib/jvm/java-openjdk)])],
              [
                if test "x${withval}" = xyes
                then
                  SYSTEM_OPENJDK_DIR=
                elif test "x${withval}" = xno
                then
	          SYSTEM_OPENJDK_DIR=
	        else
                  SYSTEM_OPENJDK_DIR=${withval}
                fi
              ],
              [
                SYSTEM_OPENJDK_DIR=
              ])
  if test -z "${SYSTEM_OPENJDK_DIR}"; then
    for dir in /usr/lib/jvm/java-openjdk /usr/lib/jvm/openjdk ; do
       if test -d $dir; then
         SYSTEM_OPENJDK_DIR=$dir
	 break
       fi
    done
  fi
  AC_MSG_RESULT(${SYSTEM_OPENJDK_DIR})
  if ! test -d "${SYSTEM_OPENJDK_DIR}"; then
    AC_MSG_ERROR("An OpenJDK home directory could not be found.")
  fi
  AC_SUBST(SYSTEM_OPENJDK_DIR)
])

AC_DEFUN([AC_CHECK_FOR_ICEDTEA],
[
  AC_MSG_CHECKING(for an existing IcedTea installation)
  AC_ARG_WITH([icedtea-home],
              [AS_HELP_STRING([--with-icedtea-home],
                              [IcedTea home directory \
                               (default is /usr/lib/jvm/java-icedtea)])],
              [
                if test "x${withval}" = xyes
                then
                  SYSTEM_ICEDTEA_DIR=
                elif test "x${withval}" = xno
                then
	          SYSTEM_ICEDTEA_DIR=
	        else
                  SYSTEM_ICEDTEA_DIR=${withval}
                fi
              ],
              [
                SYSTEM_ICEDTEA_DIR=
              ])
  if test -z "${SYSTEM_ICEDTEA_DIR}"; then
    for dir in /usr/lib/jvm/java-icedtea /usr/lib/jvm/java-openjdk \
               /usr/lib/jvm/icedtea6 /usr/lib/jvm/java-6-openjdk ; do
       if test -d $dir; then
         SYSTEM_ICEDTEA_DIR=$dir
	 break
       fi
    done
  fi
  AC_MSG_RESULT(${SYSTEM_ICEDTEA_DIR})
  if ! test -d "${SYSTEM_ICEDTEA_DIR}"; then
    AC_MSG_ERROR("An IcedTea home directory could not be found.")
  fi
  AC_SUBST(SYSTEM_ICEDTEA_DIR)
])

AC_DEFUN([IT_JAVAH],[
AC_CACHE_CHECK([if $JAVAH exhibits Classpath bug 39408], it_cv_cp39408_javah, [
SUPERCLASS=Test.java
SUBCLASS=TestImpl.java
SUB=$(echo $SUBCLASS|sed 's#\.java##')
SUBHEADER=$(echo $SUBCLASS|sed 's#\.java#.h#')
mkdir tmp.$$
cd tmp.$$
cat << \EOF > $SUPERCLASS
/* [#]line __oline__ "configure" */
public class Test 
{
  public static final int POTATO = 0;
  public static final int CABBAGE = 1;
}
EOF
cat << \EOF > $SUBCLASS
/* [#]line __oline__ "configure" */
public class TestImpl
  extends Test
{
  public native void doStuff();
}
EOF
if $JAVAC -cp . $JAVACFLAGS $SUBCLASS >/dev/null 2>&1; then
  if $JAVAH -classpath . $SUB > /dev/null 2>&1; then
    if cat $SUBHEADER | grep POTATO > /dev/null 2>&1; then
      it_cv_cp39408_javah=no;
    else
      it_cv_cp39408_javah=yes;
    fi
  else
    AC_MSG_ERROR([The Java header generator $JAVAH failed])
    echo "configure: failed program was:" >&AC_FD_CC
    cat $SUBCLASS >&AC_FD_CC
  fi
else
  AC_MSG_ERROR([The Java compiler $JAVAC failed])
  echo "configure: failed program was:" >&AC_FD_CC
  cat $SUBCLASS >&AC_FD_CC
fi
])
AC_CACHE_CHECK([if $JAVAH exhibits Classpath bug 40188], it_cv_cp40188_javah, [
  if test -e $SUBHEADER ; then
    if cat $SUBHEADER | grep TestImpl_POTATO > /dev/null 2>&1; then
      it_cv_cp40188_javah=no;
    else
      it_cv_cp40188_javah=yes;
    fi
  fi
])
rm -f $SUBCLASS $SUPERCLASS $SUBHEADER *.class
cd ..
rmdir tmp.$$
AM_CONDITIONAL([CP39408_JAVAH], test x"${it_cv_cp39408_javah}" = "xyes")
AM_CONDITIONAL([CP40188_JAVAH], test x"${it_cv_cp40188_javah}" = "xyes")
AC_PROVIDE([$0])dnl
])

AC_DEFUN([FIND_BCEL_JAR],
[
  AC_MSG_CHECKING([for bytecode engineering library (BCEL)])
  AC_ARG_WITH([bcel],
              [AS_HELP_STRING(--with-bcel,specify location of the bcel jar)],
  [
    case "${withval}" in
      yes)
	BCEL_JAR=yes
        ;;
      no)
        BCEL_JAR=no
        ;;
      *)
        BCEL_JAR="${withval}"
	;;
     esac
  ],
  [
    BCEL_JAR=yes
  ])
  if test x"${BCEL_JAR}" = "xyes"; then
    if test -e "/usr/share/bcel/lib/bcel.jar"; then
      BCEL_JAR="/usr/share/bcel/lib/bcel.jar"
    elif test -e "/usr/share/java/bcel.jar"; then
      BCEL_JAR="/usr/share/java/bcel.jar"
    fi
  fi
  if ! test -f "${BCEL_JAR}"; then
      AC_MSG_RESULT([not found])
      AC_MSG_ERROR("A BCEL jar ${BCEL_JAR} was not found.")
  fi
  AC_MSG_RESULT(${BCEL_JAR})
  AC_SUBST(BCEL_JAR)
])

AC_DEFUN([FIND_XPP3_JAR],
[
  AC_MSG_CHECKING([for XML Pull Parser 3 (XPP3)])
  AC_ARG_WITH([xpp3],
              [AS_HELP_STRING(--with-xpp3,specify location of the xpp3 jar)],
  [
    case "${withval}" in
      yes)
	XPP3_JAR=yes
        ;;
      no)
        XPP3_JAR=no
        ;;
      *)
        XPP3_JAR="${withval}"
	;;
     esac
  ],
  [
    XPP3_JAR=yes
  ])
  if test x"${XPP3_JAR}" = "xyes"; then
    if test -e "/usr/share/xpp3/lib/xpp3.jar"; then
      XPP3_JAR="/usr/share/xpp3/lib/xpp3.jar"
    elif test -e "/usr/share/java/xpp3.jar"; then
      XPP3_JAR="/usr/share/java/xpp3.jar"
    fi
  fi
  if ! test -f "${XPP3_JAR}"; then
      AC_MSG_RESULT([not found])
      AC_MSG_ERROR("A XPP3 jar ${XPP3_JAR} was not found.")
  fi
  AC_MSG_RESULT(${XPP3_JAR})
  AC_SUBST(XPP3_JAR)
])

AC_DEFUN([FIND_JIBX_DIR],
[
  AC_MSG_CHECKING([for JIBX])
  AC_ARG_WITH([jibx],
              [AS_HELP_STRING(--with-jibx,specify location of the jibx jars)],
  [
    case "${withval}" in
      yes)
	JIBX_DIR=yes
        ;;
      no)
        JIBX_DIR=no
        ;;
      *)
        JIBX_DIR="${withval}"
	;;
     esac
  ],
  [
    JIBX_DIR=yes
  ])
  if test x"${JIBX_DIR}" = "xyes"; then
    if test -e "/usr/share/jibx/lib/jibx-run.jar"; then
      JIBX_DIR=/usr/share/jibx/lib
    elif test -e "/usr/share/java/jibx-run.jar"; then
      JIBX_DIR=/usr/share/java
    fi
  fi
  if ! test -d "${JIBX_DIR}"; then
      AC_MSG_RESULT([not found])
      AC_MSG_ERROR("A JIBX jar directory ${JIBX_JAR} was not found.")
  fi
  AC_MSG_RESULT(${JIBX_DIR})
  AC_SUBST(JIBX_DIR)
])

AC_DEFUN([AC_CHECK_ENABLE_NIMBUS],
[
  AC_MSG_CHECKING(whether to build the Nimbus L'n'F)
  AC_ARG_ENABLE([nimbus],
	      [AS_HELP_STRING(--enable-nimbus,build the Nimbus L'n'F [[default=yes]])],
  [
    ENABLE_NIMBUS="${enableval}"
  ],
  [
    ENABLE_NIMBUS=yes
  ])

  AC_MSG_RESULT(${ENABLE_NIMBUS})
  AM_CONDITIONAL(ENABLE_NIMBUS, test x"${ENABLE_NIMBUS}" = "xyes")
  AC_SUBST(ENABLE_NIMBUS)
])
