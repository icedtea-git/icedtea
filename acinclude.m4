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
      ;;
    arm*-*-*)
      BUILD_ARCH_DIR=arm
      INSTALL_ARCH_DIR=arm
      JRE_ARCH_DIR=arm
      ;;
    powerpc-*-*)
      BUILD_ARCH_DIR=ppc
      INSTALL_ARCH_DIR=ppc
      JRE_ARCH_DIR=ppc
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
    *)
      BUILD_ARCH_DIR=`uname -m`
      INSTALL_ARCH_DIR=$BUILD_ARCH_DIR
      JRE_ARCH_DIR=$INSTALL_ARCH_DIR
      ;;
  esac
  AC_SUBST(BUILD_ARCH_DIR)
  AC_SUBST(INSTALL_ARCH_DIR)
  AC_SUBST(JRE_ARCH_DIR)
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
  AC_MSG_CHECKING(openjdk sources)
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
      AC_MSG_CHECKING(ecj jar)
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

AC_DEFUN([FIND_LIBGCJ_JAR],
[
  AM_CONDITIONAL(GCC_OLD, test x != x)
  AC_ARG_WITH([libgcj-jar],
              [AS_HELP_STRING(--with-libgcj-jar,specify location of the libgcj 4.3.0 jar)],
  [
    if test -f "${withval}"; then
      AC_MSG_CHECKING(libgcj jar)
      LIBGCJ_JAR="${withval}"
      AC_MSG_RESULT(${withval})
    fi
  ],
  [
    LIBGCJ_JAR=
  ])
  if test -z "${LIBGCJ_JAR}"; then
    AC_MSG_CHECKING(for libgcj-4.3.0.jar or libgcj-4.1.2.jar)
    if test -e "/usr/share/java/libgcj-4.3.0.jar"; then
      LIBGCJ_JAR=/usr/share/java/libgcj-4.3.0.jar
      AC_MSG_RESULT(${LIBGCJ_JAR})
    else
      if test -e "/usr/share/java/libgcj-4.3.jar"; then
        LIBGCJ_JAR=/usr/share/java/libgcj-4.3.jar
	AC_MSG_RESULT(${LIBGCJ_JAR})
      else
	AM_CONDITIONAL(GCC_OLD, test x = x)
        if test -e "/usr/share/java/libgcj-4.1.2.jar"; then
          LIBGCJ_JAR=/usr/share/java/libgcj-4.1.2.jar
          AC_MSG_RESULT(${LIBGCJ_JAR})
	else
	  if test -e "/usr/share/java/libgcj-4.1.jar"; then
            LIBGCJ_JAR=/usr/share/java/libgcj-4.1.jar
            AC_MSG_RESULT(${LIBGCJ_JAR})
	  else
	    AC_MSG_RESULT(no)
	  fi
	fi
      fi
    fi
  fi
  if test -z "${LIBGCJ_JAR}"; then
    AC_MSG_ERROR("A LIBGCJ jar was not found.")
  fi
  AC_SUBST(LIBGCJ_JAR)
])

AC_DEFUN([FIND_JAVAH],
[
  AC_ARG_WITH([javah],
              [AS_HELP_STRING(--with-javah,specify location of the javah)],
  [
    if test -f "${withval}"; then
      AC_MSG_CHECKING(javah)
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
      AC_MSG_CHECKING(jar)
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
  AC_MSG_CHECKING([wether jar supports @<file> argument])
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
  rm -f _config.txt _config.list _config.jar
  AC_SUBST(JAR)
  AC_SUBST(JAR_KNOWS_ATFILE)
])

AC_DEFUN([FIND_RMIC],
[
  AC_ARG_WITH([rmic],
              [AS_HELP_STRING(--with-rmic,specify location of the rmic)],
  [
    if test -f "${withval}"; then
      AC_MSG_CHECKING(rmic)
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
  AC_ARG_WITH([endorsed-dir],
              [AS_HELP_STRING(--with-endorsed-dir,specify directory of endorsed jars (xalan-j2.jar, xalan-j2-serializer.jar, xerces-j2.jar))],
  [
    if test -f "${withval}/xalan-j2.jar"; then
      if test -f "${withval}/xalan-j2-serializer.jar"; then
        if test -f "${withval}/xerces-j2.jar"; then
          AC_MSG_CHECKING(endorsed jars dir)
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
    AC_MSG_CHECKING(for endorsed jars dir)
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
  AC_MSG_CHECKING(openjdk source zip)
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
])

AC_DEFUN([FIND_XALAN2_JAR],
[
  AC_ARG_WITH([xalan2-jar],
              [AS_HELP_STRING(--with-xalan2-jar,specify location of the xalan2 jar)],
  [
    if test -f "${withval}"; then
      AC_MSG_CHECKING(xalan2 jar)
      XALAN2_JAR="${withval}"
      AC_MSG_RESULT(${withval})
    fi
  ],
  [
    XALAN2_JAR=
  ])
  if test -z "${XALAN2_JAR}"; then
    AC_MSG_CHECKING(for xalan2 jar)
    if test -e "/usr/share/java/xalan-j2.jar"; then
      XALAN2_JAR=/usr/share/java/xalan-j2.jar
      AC_MSG_RESULT(${XALAN2_JAR})
    elif test -e "/usr/share/java/xalan2.jar"; then
      XALAN2_JAR=/usr/share/java/xalan2.jar
      AC_MSG_RESULT(${XALAN2_JAR})
    elif test -e "/usr/share/xalan/lib/xalan.jar"; then
      XALAN2_JAR=/usr/share/xalan/lib/xalan.jar
      AC_MSG_RESULT(${XALAN2_JAR})
    else
      AC_MSG_RESULT(no)
    fi
  fi
  if test -z "${XALAN2_JAR}"; then
    AC_MSG_ERROR("A xalan2 jar was not found.")
  fi
  AC_SUBST(XALAN2_JAR)
])

AC_DEFUN([FIND_XALAN2_SERIALIZER_JAR],
[
  AC_ARG_WITH([xalan2-serializer-jar],
              [AS_HELP_STRING(--with-xalan2-serializer-jar,specify location of the xalan2-serializer jar)],
  [
    if test -f "${withval}"; then
      AC_MSG_CHECKING(xalan2 serializer jar)
      XALAN2_SERIALIZER_JAR="${withval}"
      AC_MSG_RESULT(${withval})
    fi
  ],
  [
    XALAN2_SERIALIZER_JAR=
  ])
  if test -z "${XALAN2_SERIALIZER_JAR}"; then
    AC_MSG_CHECKING(for xalan2-serializer jar)
    if test -e "/usr/share/java/xalan-j2-serializer.jar"; then
      XALAN2_SERIALIZER_JAR=/usr/share/java/xalan-j2-serializer.jar
      AC_MSG_RESULT(${XALAN2_SERIALIZER_JAR})
    elif test -e "/usr/share/xalan-serializer/lib/serializer.jar"; then
      XALAN2_SERIALIZER_JAR=/usr/share/xalan-serializer/lib/serializer.jar
      AC_MSG_RESULT(${XALAN2_SERIALIZER_JAR})
    elif test -e "/usr/share/java/serializer.jar"; then
      XALAN2_SERIALIZER_JAR=/usr/share/java/serializer.jar
      AC_MSG_RESULT(${XALAN2_SERIALIZER_JAR})
    else
      AC_MSG_RESULT(no)
    fi
  fi
  if test -z "${XALAN2_SERIALIZER_JAR}"; then
    AC_MSG_ERROR("A xalan2-serializer jar was not found.")
  fi
  AC_SUBST(XALAN2_SERIALIZER_JAR)
])

AC_DEFUN([FIND_XERCES2_JAR],
[
  AC_ARG_WITH([xerces2-jar],
              [AS_HELP_STRING(--with-xerces2-jar,specify location of the xerces2 jar)],
  [
    if test -f "${withval}"; then
      AC_MSG_CHECKING(xerces2 jar)
      XERCES2_JAR="${withval}"
      AC_MSG_RESULT(${withval})
    fi
  ],
  [
    XERCES2_JAR=
  ])
  if test -z "${XERCES2_JAR}"; then
    AC_MSG_CHECKING(for xerces2 jar)
    if test -e "/usr/share/java/xerces-j2.jar"; then
      XERCES2_JAR=/usr/share/java/xerces-j2.jar
      AC_MSG_RESULT(${XERCES2_JAR})
    elif test -e "/usr/share/java/xerces2.jar"; then
      XERCES2_JAR=/usr/share/java/xerces2.jar
      AC_MSG_RESULT(${XERCES2_JAR})
    elif test -e "/usr/share/xerces-2/lib/xercesImpl.jar"; then
      XERCES2_JAR=/usr/share/xerces-2/lib/xercesImpl.jar
      AC_MSG_RESULT(${XERCES2_JAR})
    elif test -e "/usr/share/java/xercesImpl.jar"; then
      XERCES2_JAR=/usr/share/java/xercesImpl.jar
      AC_MSG_RESULT(${XERCES2_JAR})
    else
      AC_MSG_RESULT(no)
    fi
  fi
  if test -z "${XERCES2_JAR}"; then
    AC_MSG_ERROR("A xerces2 jar was not found.")
  fi
  AC_SUBST(XERCES2_JAR)
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
  ])
  AC_MSG_RESULT($use_zero)
  AM_CONDITIONAL(ZERO_BUILD, test "x${use_zero}" = xyes)

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

AC_DEFUN([SET_CORE_OR_SHARK_BUILD],
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

  use_core=no
  use_shark=no
  if test "x${WITH_CACAO}" != "xno"; then
    use_core=yes
  elif test "x${use_zero}" = "xyes"; then
    if test "x${shark_selected}" = "xyes"; then
      use_shark=yes
    else
      use_core=yes
    fi
  fi
  AC_MSG_RESULT($use_shark)

  AM_CONDITIONAL(CORE_BUILD, test "x${use_core}" = xyes)
  AM_CONDITIONAL(SHARK_BUILD, test "x${use_shark}" = xyes)
])

AC_DEFUN([AC_CHECK_WITH_CACAO],
[
  AC_MSG_CHECKING(whether to use CACAO as VM)
  AC_ARG_WITH([cacao],
	      [AS_HELP_STRING(--with-cacao,use CACAO as VM [[default=no]])],
  [
    WITH_CACAO=yes
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
  AC_MSG_CHECKING(CACAO home directory)
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
  AC_MSG_CHECKING(CACAO source zip)
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
    enable_hg=no
  ])
  AC_MSG_RESULT([${enable_hg}])
  AM_CONDITIONAL([USE_HG], test x"${enable_hg}" = "xyes")
])

AC_DEFUN([ENABLE_EXTRA_PROJECTS],
[
  AC_MSG_CHECKING(whether to use code from the Closures project)
  AC_ARG_ENABLE([closures],
                [AS_HELP_STRING(--enable-closures,use Closures source code [[default=no]])],
  [
    case "${enableval}" in
      no)
	enable_closures=no
        ;;
      *)
        enable_closures=yes
        ;;
    esac
  ],
  [
    enable_closures=no
  ])
  AC_MSG_RESULT([${enable_closures}])
  AM_CONDITIONAL([USE_CLOSURES], test x"${enable_closures}" = "xyes")
  AC_MSG_CHECKING(whether to use code from the CVMI project)
  AC_ARG_ENABLE([cvmi],
                [AS_HELP_STRING(--enable-cvmi,use CVMI source code [[default=no]])],
  [
    case "${enableval}" in
      no)
	enable_cvmi=no
        ;;
      *)
        enable_cvmi=yes
        ;;
    esac
  ],
  [
    enable_cvmi=no
  ])
  AC_MSG_RESULT([${enable_cvmi}])
  AM_CONDITIONAL([USE_CVMI], test x"${enable_cvmi}" = "xyes")
])
