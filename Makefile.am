# Dependencies

CORBA_CHANGESET = 4493de9f3f0b
JAXP_CHANGESET = 5e4ceb999239
JAXWS_CHANGESET = b24931dd91c6
JDK_CHANGESET = 89324d11814d
LANGTOOLS_CHANGESET = 480d3ab02c36
OPENJDK_CHANGESET = b04b11ab5066

CORBA_SHA256SUM = 81e3ddac83b981400c062788bcd3005587d5984f18def588d8cd232b9b5b24ef
JAXP_SHA256SUM = 21772ab65388c1890ff33f9a5dc2fd22f69fd0f0f0d336e1f8f64d920b308b90
JAXWS_SHA256SUM = 3e27f2e6b92d0b15d544e06b897cf2b072bece695a0a8a79de6cd609f1a216ef
JDK_SHA256SUM = 56aacc5bfeab0688a4042f9ffdc971b5764f99c9fdea0080f41e2bfaa27a08b0
LANGTOOLS_SHA256SUM = 8057da4c87ecea91ec16b4b180ab006ae12be60a2f8387dfe2a7b18c8e303379
OPENJDK_SHA256SUM = f1e430150f95073a3dc1f4f32bdfbf8cc6aa4b23cb940128f0224b7da90516aa

DROP_URL = https://icedtea.classpath.org/download/drops

CACAO_VERSION = c182f119eaad
CACAO_SHA256SUM = eade43f6c5da8b3b5a64e6b206b624eaca88539f80101d7164d6aa426eb590b1
CACAO_BASE_URL = $(DROP_URL)/cacao
CACAO_SRC_ZIP = cacao-$(CACAO_VERSION).tar.gz
CACAO_URL = $(CACAO_BASE_URL)/$(CACAO_SRC_ZIP)

JAMVM_VERSION = ec18fb9e49e62dce16c5094ef1527eed619463aa
JAMVM_SHA256SUM = 31810266666c23822942aac62a78019c2c4589e1c5ee48329cbf42652d4437bc
JAMVM_BASE_URL = $(DROP_URL)/jamvm
JAMVM_URL = $(JAMVM_BASE_URL)/jamvm-$(JAMVM_VERSION).tar.gz
JAMVM_SRC_ZIP = jamvm-$(JAMVM_VERSION).tar.gz

ICEDTEA_MAJOR = icedtea7
ICEDTEA_BRANCH = 2.6
ICEDTEA_PREFIX = $(ICEDTEA_MAJOR)-forest-$(ICEDTEA_BRANCH)
ICEDTEA_HG_URL = https://icedtea.classpath.org/hg/release/$(ICEDTEA_PREFIX)
ICEDTEA_URL = $(DROP_URL)/$(ICEDTEA_MAJOR)/$(ICEDTEA_RELEASE)

HS_TYPE = "`$(AWK) 'version==$$1 {print $$2}' version=$(HSBUILD) $(abs_top_builddir)/hotspot.map`"
HS_URL = "`$(AWK) 'version==$$1 {print $$3}' version=$(HSBUILD) $(abs_top_builddir)/hotspot.map`"
HS_CHANGESET = "`$(AWK) 'version==$$1 {print $$4}' version=$(HSBUILD) $(abs_top_builddir)/hotspot.map`"
HS_SHA256SUM = "`$(AWK) 'version==$$1 {print $$5}' version=$(HSBUILD) $(abs_top_builddir)/hotspot.map`"

# Build directories

BUILD_DIR = @OPENJDK_BUILD_DIR@
BUILD_OUTPUT_DIR = $(abs_top_builddir)/$(BUILD_DIR)
DEBUG_BUILD_OUTPUT_DIR = $(BUILD_OUTPUT_DIR)-debug
BOOT_BUILD_OUTPUT_DIR = $(BUILD_OUTPUT_DIR)-boot
BOOT_DIR = $(abs_top_builddir)/bootstrap/jdk1.6.0
RUNTIME = $(BOOT_DIR)/jre/lib/rt.jar
CLS_DIR_BOOT = $(BOOT_BUILD_OUTPUT_DIR)/classes
SERVER_DIR = hotspot/import/jre/lib/$(INSTALL_ARCH_DIR)/server
BUILD_SDK_DIR = $(BUILD_OUTPUT_DIR)/j2sdk-image
BUILD_JRE_DIR = $(BUILD_OUTPUT_DIR)/j2re-image
BUILD_DEBUGINFO_DIR = $(BUILD_OUTPUT_DIR)/j2sdk-image.stripped
BUILD_JRE_ARCH_DIR = $(BUILD_SDK_DIR)/jre/lib/$(INSTALL_ARCH_DIR)
BUILD_DEBUG_SDK_DIR = $(DEBUG_BUILD_OUTPUT_DIR)/j2sdk-image
BUILD_DEBUG_JRE_DIR = $(DEBUG_BUILD_OUTPUT_DIR)/j2re-image
BUILD_DEBUG_DEBUGINFO_DIR = $(DEBUG_BUILD_OUTPUT_DIR)/j2sdk-image.stripped
BUILD_DEBUG_JRE_ARCH_DIR = $(BUILD_DEBUG_SDK_DIR)/jre/lib/$(INSTALL_ARCH_DIR)
BUILD_BOOT_SDK_DIR = $(BOOT_BUILD_OUTPUT_DIR)/j2sdk-image
BUILD_BOOT_JRE_DIR = $(BOOT_BUILD_OUTPUT_DIR)/j2re-image
BUILD_BOOT_DEBUGINFO_DIR = $(BOOT_BUILD_OUTPUT_DIR)/j2sdk-image.stripped
BUILD_BOOT_JRE_ARCH_DIR = $(BUILD_BOOT_SDK_DIR)/jre/lib/$(INSTALL_ARCH_DIR)
REWRITER_BUILD_DIR = $(abs_top_builddir)/rewriter.build
STAGE1_BOOT_DIR = $(abs_top_builddir)/bootstrap/boot
STAGE2_BOOT_DIR = $(abs_top_builddir)/bootstrap/icedtea
JAMVM_IMPORT_PATH = $(abs_top_builddir)/jamvm/install/hotspot
CRYPTO_CHECK_BUILD_DIR = $(abs_top_builddir)/test/cryptocheck.build
ECC_CHECK_BUILD_DIR = $(abs_top_builddir)/ecccheck.build
MIME_TYPE_CHECK_BUILD_DIR = $(abs_top_builddir)/test/mimetypecheck.build
STAGE1_BOOT_RUNTIME = $(STAGE1_BOOT_DIR)/jre/lib/rt.jar
STAGE2_BOOT_RUNTIME = $(STAGE2_BOOT_DIR)/jre/lib/rt.jar
FONTCONFIG_PATH = openjdk/jdk/src/solaris/classes/sun/awt/fontconfigs

# Installation directories

DEBUG_PREFIX = $(libdir)/debug

# Source directories

SHARE = openjdk-boot/jdk/src/share/classes
SOLARIS = openjdk-boot/jdk/src/solaris/classes
LANGTOOLS = openjdk-boot/langtools/src/share/classes
CORBA = openjdk-boot/corba/src/share/classes
JAXWS = openjdk-boot/jaxws/src/share/jaxws_classes:openjdk-boot/jaxws/src/share/jaf_classes

OPENJDK_SOURCEPATH_DIRS = \
        $(SHARE):$(SOLARIS):$(LANGTOOLS):$(CORBA):$(JAXWS)

SOURCEPATH_DIRS = $(abs_top_srcdir)/generated:$(OPENJDK_SOURCEPATH_DIRS)

CACERTS_FILE = @ALT_CACERTS_FILE@

# Sources used from OpenJDK.
ICEDTEA_BOOTSTRAP_CLASSES =

#PR43148 - javac fails due to missing java.util.regex.Matcher.quoteReplacement
if LACKS_JAVA_UTIL_REGEX_MATCHER_QUOTEREPLACEMENT
ICEDTEA_BOOTSTRAP_CLASSES += \
	$(SHARE)/java/util/regex/Matcher.java
endif

#PR48033 - Missing javax.management.remote.JMXServiceURL
if LACKS_JAVAX_MANAGEMENT_REMOTE_JMXSERVICEURL
ICEDTEA_BOOTSTRAP_CLASSES += \
	$(SHARE)/javax/management/remote/JMXServiceURL.java
endif

#PR48034 - javax.management.modelmbean.ModelMBeanInfo
if LACKS_JAVAX_MANAGEMENT_MODELMBEAN_MODELMBEANINFO
ICEDTEA_BOOTSTRAP_CLASSES += \
	$(SHARE)/javax/management/modelmbean/ModelMBeanInfo.java
endif

#PR42003 - Missing javax.swing.plaf.basic.BasicDirectoryModel methods cause OpenJDK build failure 
if LACKS_JAVAX_SWING_PLAF_BASIC_BASICDIRECTORYMODEL_ADDPROPERTYCHANGELISTENER
ICEDTEA_BOOTSTRAP_CLASSES += \
	$(SHARE)/javax/swing/plaf/basic/BasicDirectoryModel.java
endif

#PR56553 - SSLParameters support missing
if LACKS_JAVAX_NET_SSL_SSLCONTEXT_GETDEFAULTSSLPARAMETERS
ICEDTEA_BOOTSTRAP_CLASSES += \
	$(SHARE)/javax/net/ssl/SSLContext.java
endif
if LACKS_JAVAX_NET_SSL_SSLENGINE_SETSSLPARAMETERS
ICEDTEA_BOOTSTRAP_CLASSES += \
	$(SHARE)/javax/net/ssl/SSLEngine.java
endif

#PR57008 - Add missing SslRMIServerSocketFactory constructor from 7
if LACKS_JAVAX_RMI_SSL_SSLRMISERVERSOCKETFACTORY_7
ICEDTEA_BOOTSTRAP_CLASSES += \
	$(SHARE)/javax/rmi/ssl/SslRMIServerSocketFactory.java
endif

# Settings for javac

if NO_BYTECODE7
IT_LANGUAGE_SOURCE_VERSION=6
IT_CLASS_TARGET_VERSION=6
else
IT_LANGUAGE_SOURCE_VERSION=7
IT_CLASS_TARGET_VERSION=7
endif

IT_JAVAC_SETTINGS=-g -encoding utf-8 $(JAVACFLAGS) $(MEMORY_LIMIT) $(PREFER_SOURCE)
IT_JAVACFLAGS=$(IT_JAVAC_SETTINGS) -source $(IT_LANGUAGE_SOURCE_VERSION) -target $(IT_CLASS_TARGET_VERSION)

# Flags
IT_CFLAGS=$(CFLAGS) $(ARCHFLAG)

# Conditional defintions

TAR_SUFFIX = tar.bz2
if USE_HG
  OPENJDK_SRC_ZIP = 
  CORBA_SRC_ZIP =
  JAXP_SRC_ZIP =
  JAXWS_SRC_ZIP =
  JDK_SRC_ZIP =
  LANGTOOLS_SRC_ZIP =
  HOTSPOT_SRC_ZIP =
else
  OPENJDK_SRC_ZIP = openjdk.$(TAR_SUFFIX)
  CORBA_SRC_ZIP = corba.$(TAR_SUFFIX)
  JAXP_SRC_ZIP = jaxp.$(TAR_SUFFIX)
  JAXWS_SRC_ZIP = jaxws.$(TAR_SUFFIX)
  JDK_SRC_ZIP = jdk.$(TAR_SUFFIX)
  LANGTOOLS_SRC_ZIP = langtools.$(TAR_SUFFIX)
  HOTSPOT_SRC_ZIP = hotspot.$(TAR_SUFFIX)
endif

if DISABLE_OPENJDK_CHECKSUM
OPENJDK_SHA256SUM_BIN = true
else
OPENJDK_SHA256SUM_BIN = $(SHA256SUM)
endif

if WITH_OPENJDK_CHECKSUM
OPENJDK_SUM = @OPENJDK_CHECKSUM@
else
OPENJDK_SUM = $(OPENJDK_SHA256SUM)
endif

if DISABLE_CORBA_CHECKSUM
CORBA_SHA256SUM_BIN = true
else
CORBA_SHA256SUM_BIN = $(SHA256SUM)
endif

if WITH_CORBA_CHECKSUM
CORBA_SUM = @CORBA_CHECKSUM@
else
CORBA_SUM = $(CORBA_SHA256SUM)
endif

if DISABLE_JAXP_CHECKSUM
JAXP_SHA256SUM_BIN = true
else
JAXP_SHA256SUM_BIN = $(SHA256SUM)
endif

if WITH_JAXP_CHECKSUM
JAXP_SUM = @JAXP_CHECKSUM@
else
JAXP_SUM = $(JAXP_SHA256SUM)
endif

if DISABLE_JAXWS_CHECKSUM
JAXWS_SHA256SUM_BIN = true
else
JAXWS_SHA256SUM_BIN = $(SHA256SUM)
endif

if WITH_JAXWS_CHECKSUM
JAXWS_SUM = @JAXWS_CHECKSUM@
else
JAXWS_SUM = $(JAXWS_SHA256SUM)
endif

if DISABLE_JDK_CHECKSUM
JDK_SHA256SUM_BIN = true
else
JDK_SHA256SUM_BIN = $(SHA256SUM)
endif

if WITH_JDK_CHECKSUM
JDK_SUM = @JDK_CHECKSUM@
else
JDK_SUM = $(JDK_SHA256SUM)
endif

if DISABLE_LANGTOOLS_CHECKSUM
LANGTOOLS_SHA256SUM_BIN = true
else
LANGTOOLS_SHA256SUM_BIN = $(SHA256SUM)
endif

if WITH_LANGTOOLS_CHECKSUM
LANGTOOLS_SUM = @LANGTOOLS_CHECKSUM@
else
LANGTOOLS_SUM = $(LANGTOOLS_SHA256SUM)
endif

if DISABLE_CACAO_CHECKSUM
CACAO_SHA256SUM_BIN = true
else
CACAO_SHA256SUM_BIN = $(SHA256SUM)
endif

if WITH_CACAO_CHECKSUM
CACAO_SUM = @CACAO_CHECKSUM@
else
CACAO_SUM = $(CACAO_SHA256SUM)
endif

if DISABLE_JAMVM_CHECKSUM
JAMVM_SHA256SUM_BIN = true
else
JAMVM_SHA256SUM_BIN = $(SHA256SUM)
endif

if WITH_JAMVM_CHECKSUM
JAMVM_SUM = @JAMVM_CHECKSUM@
else
JAMVM_SUM = $(JAMVM_SHA256SUM)
endif

if DISABLE_HOTSPOT_CHECKSUM
HOTSPOT_SHA256SUM_BIN = true
else
HOTSPOT_SHA256SUM_BIN = $(SHA256SUM)
endif

if WITH_HOTSPOT_CHECKSUM
HOTSPOT_SUM = @HOTSPOT_CHECKSUM@
else
HOTSPOT_SUM = $(HS_SHA256SUM)
endif

if WITH_HGREV
REV_ARG = -r $(HGREV)
endif

if COMPILER_SUPPORTS_XPREFERSOURCE
  PREFER_SOURCE = -Xprefer:source
else
  PREFER_SOURCE =
endif

if COMPILER_SUPPORTS_MAX_HEAP_SIZE
  MEMORY_LIMIT = -J-Xmx1024m
else
  MEMORY_LIMIT =
endif

if ENABLE_CACAO
  ICEDTEA_BUILD_TARGET = jdk_only
  ICEDTEA_DEBUG_BUILD_TARGET = jdk_fastdebug_only
else
if ENABLE_JAMVM
  ICEDTEA_BUILD_TARGET=jdk_only
  ICEDTEA_DEBUG_BUILD_TARGET = jdk_fastdebug_only
else
  ICEDTEA_DEBUG_BUILD_TARGET = debug_build
endif
endif

if BOOTSTRAPPING
  BOOTSTRAP_DIRECTORY_STAMP = stamps/bootstrap-directory-stage2.stamp
  ICEDTEA_HOME = $(STAGE2_BOOT_DIR)
  INITIAL_BOOTSTRAP_LINK_STAMP = stamps/bootstrap-directory-symlink-stage1.stamp
else
  BOOTSTRAP_DIRECTORY_STAMP =
  ICEDTEA_HOME = $(SYSTEM_JDK_DIR)
  INITIAL_BOOTSTRAP_LINK_STAMP = stamps/bootstrap-directory-symlink-stage2.stamp
endif

if CP_SUPPORTS_REFLINK
  REFLINK = --reflink=auto
endif

if OPENJDK_SRC_DIR_HARDLINKABLE
  OPENJDK_SRC_DIR_LINK = -l
else
  OPENJDK_SRC_DIR_LINK = $(REFLINK)
endif

if SRC_DIR_HARDLINKABLE
  SRC_DIR_LINK = -l
else
  SRC_DIR_LINK = $(REFLINK)
endif

if !DISABLE_HOTSPOT_TESTS
HOTSPOT_TEST_SUITE = hotspot
endif

if !DISABLE_JDK_TESTS
JDK_TEST_SUITE = jdk
endif

if !DISABLE_LANGTOOLS_TESTS
LANGTOOLS_TEST_SUITE = langtools
endif

if !DISABLE_TESTS
TEST_SUITES = $(HOTSPOT_TEST_SUITE) $(JDK_TEST_SUITE) $(LANGTOOLS_TEST_SUITE)
TESTS_TO_RUN = jtreg $(addprefix check-,$(TEST_SUITES))
endif

# Target to ensure a patched OpenJDK tree containing Zero & Shark
# and any overlays is available in $(abs_top_builddir)/openjdk
OPENJDK_TREE = stamps/overlay.stamp stamps/add-nss.stamp

# Target to ensure a patched OpenJDK bootstrap tree containing
# Zero & Shark, any other overlays and the bootstrapping patches
# is available in $(abs_top_builddir)/openjdk-boot
OPENJDK_BOOT_TREE = stamps/patch-boot.stamp

# Sources list

REWRITER_SRCS = $(top_srcdir)/rewriter/com/redhat/rewriter/ClassRewriter.java
CRYPTO_CHECK_SRCS = $(top_srcdir)/test/standalone/TestCryptoLevel.java
ECC_CHECK_SRCS = $(top_srcdir)/test/standalone/TestEllipticCurveCryptoSupport.java \
	$(top_srcdir)/test/standalone/TestECDSA.java
MIME_TYPE_CHECK_SRCS = $(top_srcdir)/test/standalone/RH1195203.java

# Patch list

ICEDTEA_PATCHES = patches/8232643-pr3761.patch

# Conditional patches

if WITH_RHINO
ICEDTEA_PATCHES += \
	patches/rhino.patch
endif

if BUILD_CACAO
ICEDTEA_PATCHES += \
	patches/cacao/launcher.patch \
	patches/cacao/memory.patch \
	patches/cacao/pr2032.patch \
	patches/cacao/pr2520-tempdir.patch \
	patches/cacao/pr2652-classloader.patch \
	patches/cacao/pr2781-refactoring_error.patch \
	patches/cacao/pr3136-unsafe_methods.patch
else
if USING_CACAO
ICEDTEA_PATCHES += \
	patches/cacao/memory.patch
endif
endif

if ENABLE_CACAO
ICEDTEA_PATCHES += \
	patches/cacao/ignore-tests.patch
endif

if BUILD_JAMVM
ICEDTEA_PATCHES += \
	patches/jamvm/find_class_from_caller.patch \
	patches/jamvm/pr2172-tempdir.patch \
	patches/jamvm/noexecstack.patch \
	patches/jamvm/pr2665.patch \
	patches/jamvm/pr3134-unsafe_methods.patch
endif

if ENABLE_NSS
ICEDTEA_PATCHES += patches/nss-config.patch
endif

ICEDTEA_PATCHES += $(DISTRIBUTION_PATCHES)

# Bootstrapping patches

ICEDTEA_BOOT_PATCHES = \
	patches/boot/javafiles.patch \
	patches/boot/corba-idlj.patch \
	patches/boot/corba-no-gen.patch \
	patches/boot/corba-orb.patch \
	patches/boot/demos.patch \
	patches/boot/fphexconstants.patch \
	patches/boot/fontconfig.patch \
	patches/boot/generated-comments.patch \
	patches/boot/icedteart.patch \
	patches/boot/jar.patch \
	patches/boot/symbols.patch \
	patches/boot/tobin.patch \
	patches/boot/tools.jar.patch \
	patches/boot/jopt.patch \
	patches/boot/genstubs-nofork.patch \
	patches/boot/getannotation-cast.patch \
	patches/boot/break-processbuilder-dependency.patch \
	patches/boot/revert-6973616.patch \
	patches/boot/revert-6941137.patch \
	patches/boot/ecj-stringswitch.patch \
	patches/boot/langtools-force-old-jdk.patch \
	patches/boot/dependencies.patch \
	patches/boot/hotspot-jdk-dependency.patch \
	patches/boot/ecj-multicatch.patch \
	patches/boot/ecj-trywithresources.patch \
	patches/boot/ecj-autoboxing.patch \
	patches/boot/xsltproc.patch \
	patches/boot/ecj-odd.patch

if !DISABLE_BOOTSTRAP_TOOLS
ICEDTEA_BOOT_PATCHES += \
	patches/boot/bootstrap-tools.patch \
	patches/boot/ant-javac.patch
endif

if LACKS_COM_SUN_MIRROR_TYPE_TYPEMIRROR
ICEDTEA_BOOT_PATCHES += \
	patches/boot/jaxws-langtools-dependency.patch
endif

if LACKS_COM_SUN_NET_HTTPSERVER_HEADERS
ICEDTEA_BOOT_PATCHES += \
	patches/boot/jaxws-jdk-dependency.patch
endif

if LACKS_SUN_TOOLS_JAVA_CLASSDEFINITION
ICEDTEA_BOOT_PATCHES += \
	patches/boot/corba-dependencies.patch
endif

if !VM_SUPPORTS_XBOOTCLASSPATH
ICEDTEA_BOOT_PATCHES += \
	patches/boot/xbootclasspath.patch
endif

if CP39408_JAVAH
ICEDTEA_BOOT_PATCHES += patches/boot/pr39408.patch
endif

if CP40188_JAVAH
ICEDTEA_BOOT_PATCHES += patches/boot/pr40188.patch \
	patches/boot/javah.patch
endif

if CP45526_JAVAH
ICEDTEA_BOOT_PATCHES += patches/boot/explicit-inner-class-headers.patch
endif

if CP40616
ICEDTEA_BOOT_PATCHES += patches/boot/pr40616.patch
endif

# Patches for when java.util.Scanner is missing
# or has the bug #40630 (all these classes rely
# on Scanner#hasNextLine() looping behaviour)
SCANNER_PATCHES = patches/boot/scanner-spp.patch \
	patches/boot/scanner-sbcs.patch \
	patches/boot/scanner-dbcs.patch \
	patches/boot/scanner-charsets.patch

if LACKS_JAVA_UTIL_SCANNER
ICEDTEA_BOOT_PATCHES += $(SCANNER_PATCHES)
else
if CP40630
ICEDTEA_BOOT_PATCHES += $(SCANNER_PATCHES)
endif
endif

if JAVAC_LACKS_DIAMOND
ICEDTEA_BOOT_PATCHES += patches/boot/ecj-diamond.patch
else
if USING_CACAO
ICEDTEA_BOOT_PATCHES += patches/boot/compile-for-7-cacao-mem.patch
else
if BUILD_CACAO
ICEDTEA_BOOT_PATCHES += patches/boot/compile-for-7-cacao-mem.patch
else
ICEDTEA_BOOT_PATCHES += patches/boot/compile-for-7.patch
endif
endif
endif

if JAVAC_LACKS_UNDERSCORED_LITERALS
ICEDTEA_BOOT_PATCHES += patches/boot/ecj-underscored_literals.patch
endif

if DTDTYPE_QNAME
ICEDTEA_BOOT_PATCHES += patches/boot/jaxws-getdtdtype.patch
endif

if NO_BYTECODE7
ICEDTEA_BOOT_PATCHES +=	patches/boot/use_target_6_for_bootstrap_classes.patch 
endif

ICEDTEA_BOOT_PATCHES += $(DISTRIBUTION_BOOT_PATCHES)

# OpenJDK build environment.
if ZERO_BUILD
  ICEDTEA_ZERO_BUILD = true
endif
if SHARK_BUILD
  ICEDTEA_SHARK_BUILD = true
endif

ICEDTEA_NAME = IcedTea
if HAS_ICEDTEA_REVISION
ICEDTEA_REV = +${ICEDTEA_REVISION}
endif
if HAS_PKGVERSION
ICEDTEA_PKG = $(EMPTY) (${PKGVERSION})
endif

if ENABLE_WERROR
WERROR_STATUS=true
else
WERROR_STATUS=false
endif

if ENABLE_ARM32JIT
ARM32JIT_STATUS=true
else
ARM32JIT_STATUS=false
endif

if ENABLE_QUEENS
TEST_IN_BUILD=true
else
TEST_IN_BUILD=false
endif

ICEDTEA_UNSET = \
	JAVAC= \
	JAVA_HOME= \
	JDK_HOME= \
	OS=

ICEDTEA_ENV = \
	ALT_JDK_IMPORT_PATH="$(BOOT_DIR)" \
	ANT="$(ANT)" \
        BUILD_NUMBER="@BUILD_VERSION@" \
	JDK_UPDATE_VERSION="@JDK_UPDATE_VERSION@" \
        JRE_RELEASE_VERSION="@JAVA_VER@_@COMBINED_VERSION@" \
	MILESTONE="@MILESTONE@" \
	LANG="C" \
	PATH="$(BOOT_DIR)/bin:$(OS_PATH):$$PATH" \
	ALT_BOOTDIR="$(BOOT_DIR)" \
	BUILD_ARCH_DIR="$(BUILD_ARCH_DIR)" \
	ICEDTEA_RT="$(RUNTIME)" \
	CLASSPATH="" \
	LD_LIBRARY_PATH="" \
	ZERO_BUILD="$(ICEDTEA_ZERO_BUILD)" \
	SHARK_BUILD="$(ICEDTEA_SHARK_BUILD)" \
	ZERO_LIBARCH="$(ZERO_LIBARCH)" \
	ARCH_DATA_MODEL="$(ZERO_BITSPERWORD)" \
	ZERO_ENDIANNESS="$(ZERO_ENDIANNESS)" \
	ZERO_ARCHDEF="$(ZERO_ARCHDEF)" \
	ZERO_ARCHFLAG="$(ARCHFLAG)" \
	LIBFFI_CFLAGS="$(LIBFFI_CFLAGS)" \
	LIBFFI_LIBS="$(LIBFFI_LIBS)" \
	LLVM_CFLAGS="$(LLVM_CFLAGS)" \
	LLVM_LDFLAGS="$(LLVM_LDFLAGS)" \
	LLVM_LIBS="$(LLVM_LIBS)" \
	FT2_CFLAGS="$(FREETYPE2_CFLAGS)" \
	FT2_LIBS="$(FREETYPE2_LIBS)" \
	ALT_PARALLEL_COMPILE_JOBS="$(PARALLEL_JOBS)" \
	HOTSPOT_BUILD_JOBS="$(PARALLEL_JOBS)" \
	QUIETLY="" \
	ANT_RESPECT_JAVA_HOME="TRUE" \
	DISTRIBUTION_ID="$(DIST_ID)" \
	DERIVATIVE_ID="$(ICEDTEA_NAME) $(PACKAGE_VERSION)$(ICEDTEA_REV)" \
	ALT_JIBX_LIBS_PATH="$(JIBX_DEPS_DIR)" \
	ALT_OUTPUTDIR="$(BUILD_OUTPUT_DIR)" \
	VERBOSE="$(VERBOSE)" \
	STATIC_CXX="false" \
	CC="$(CC)" \
	CXX="$(CXX)" \
	SYSTEM_CUPS="${ENABLE_SYSTEM_CUPS}" \
	CUPS_LIBS="${CUPS_LIBS}" \
	CUPS_CFLAGS="${CUPS_CFLAGS}" \
	STRIP_POLICY=no_strip \
	JAVAC_WARNINGS_FATAL="$(WERROR_STATUS)" \
	COMPILER_WARNINGS_FATAL="$(WERROR_STATUS)" \
	UNLIMITED_CRYPTO="true" \
	SYSTEM_ZLIB="${ENABLE_SYSTEM_ZLIB}" \
	ZLIB_LIBS="${ZLIB_LIBS}" \
	ZLIB_CFLAGS="${ZLIB_CFLAGS}" \
	SYSTEM_LCMS="${ENABLE_SYSTEM_LCMS}" \
	LCMS_LIBS="${LCMS2_LIBS}" \
	LCMS_CFLAGS="${LCMS2_CFLAGS}" \
	SYSTEM_JPEG="${ENABLE_SYSTEM_JPEG}" \
	JPEG_LIBS="${JPEG_LIBS}" \
	JPEG_CFLAGS="${JPEG_CFLAGS}" \
	SYSTEM_PNG="${ENABLE_SYSTEM_PNG}" \
	PNG_LIBS="${PNG_LIBS}" \
	PNG_CFLAGS="${PNG_CFLAGS}" \
	SYSTEM_GIF="${ENABLE_SYSTEM_GIF}" \
	GIF_LIBS="${GIF_LIBS}" \
	GIF_CFLAGS="${GIF_CFLAGS}" \
	SYSTEM_GTK="${ENABLE_SYSTEM_GTK}" \
	GTK_LIBS="${GTK_LIBS}" \
	GTK_CFLAGS="${GTK_CFLAGS}" \
	SYSTEM_GIO="${ENABLE_SYSTEM_GIO}" \
	SYSTEM_GSETTINGS="${ENABLE_SYSTEM_GSETTINGS}" \
	GIO_LIBS="${GIO_LIBS}" \
	GIO_CFLAGS="${GIO_CFLAGS}" \
	SYSTEM_GCONF="${ENABLE_SYSTEM_GCONF}" \
	GCONF_LIBS="${GCONF_LIBS}" \
	GCONF_CFLAGS="${GCONF_CFLAGS}" \
	SYSTEM_FONTCONFIG="${ENABLE_SYSTEM_FONTCONFIG}" \
	FONTCONFIG_LIBS="${FONTCONFIG_LIBS}" \
	FONTCONFIG_CFLAGS="${FONTCONFIG_CFLAGS}" \
	COMPILE_AGAINST_SYSCALLS="${ENABLE_SYSCALL_COMPILATION}" \
	SYSTEM_KRB5="${ENABLE_SYSTEM_KERBEROS}" \
	KRB5_LIBS="${KRB5_LIBS}" \
	KRB5_CFLAGS="${KRB5_CFLAGS}" \
	SYSTEM_PCSC="${ENABLE_SYSTEM_PCSC}" \
	PCSC_LIBS="${PCSC_LIBS}" \
	PCSC_CFLAGS="${PCSC_CFLAGS}" \
	SYSTEM_SCTP="${ENABLE_SYSTEM_SCTP}" \
	SCTP_LIBS="${SCTP_LIBS}" \
	SCTP_CFLAGS="${SCTP_CFLAGS}" \
	COMPRESS_JARS="${ENABLE_JAR_COMPRESSION}" \
	ARM32JIT="${ARM32JIT_STATUS}" \
	INSTALL_LOCATION="${prefix}" \
	TEST_IN_BUILD="${TEST_IN_BUILD}"

if ENABLE_CACAO
ICEDTEA_ENV += \
	ALT_HOTSPOT_IMPORT_PATH="$(CACAO_IMPORT_PATH)"
endif

if ENABLE_JAMVM
ICEDTEA_ENV += \
	ALT_HOTSPOT_IMPORT_PATH="$(JAMVM_IMPORT_PATH)"
endif

if DISABLE_OPTIMIZATIONS
ICEDTEA_ENV += \
	NO_OPTIMIZATIONS="true" \
	CC_NO_OPT="-O0 -g"
endif

if !ENABLE_DOCS
ICEDTEA_ENV += \
	NO_DOCS="true"
endif

if USE_ALT_JAR
ICEDTEA_ENV += \
	ALT_JAR_CMD="$(ALT_JAR_CMD)"
endif

if HAS_HOTSPOT_REVISION
ICEDTEA_ENV += \
	HOTSPOT_BUILD_VERSION="$(HOTSPOT_REVISION)"
endif

if WITH_RHINO
ICEDTEA_ENV += \
	RHINO_JAR="$(abs_top_builddir)/rhino/rhino.jar"
endif

if WITH_PAX
ICEDTEA_ENV += \
	PAX_COMMAND="${PAX_COMMAND}"
endif

if !DISABLE_SYSTEMTAP_TESTS
SYSTEMTAP_TEST_SUITE = check-tapset
endif

if ENABLE_SUNEC
ICEDTEA_ENV += \
	SYSTEM_NSS="true" \
	NSS_LIBS="${SUNEC_LIBS}" \
	NSS_CFLAGS="${SUNEC_CFLAGS}" \
	ECC_JUST_SUITE_B="true"
else
ICEDTEA_ENV += \
	SYSTEM_NSS="false" \
	DISABLE_INTREE_EC="true"
endif

if ENABLE_NATIVE_DEBUGINFO
ICEDTEA_ENV += \
	DEBUG_BINARIES="true"
endif

if ENABLE_JAVA_DEBUGINFO
ICEDTEA_ENV += \
	DEBUG_CLASSFILES="true"
endif

if ENABLE_IMPROVED_FONT_RENDERING
ICEDTEA_ENV += \
	IMPROVED_FONT_RENDERING="true"
endif

if USE_ALT_CACERTS_FILE
ICEDTEA_ENV += \
	ALT_CACERTS_FILE="${CACERTS_FILE}"
endif

# OpenJDK boot build environment.
ICEDTEA_ENV_BOOT = $(ICEDTEA_ENV) \
	BOOTCLASSPATH_CLS_RT="-bootclasspath $(CLS_DIR_BOOT):$(RUNTIME)" \
	BOOTCLASSPATH_CLS="-bootclasspath $(CLS_DIR_BOOT)" \
	BOOTCLASSPATH_RT_LIBGCJ="-bootclasspath $(RUNTIME)" \
	GENSRCDIR="$(abs_top_builddir)/generated.build" \
	JAR_KNOWS_ATFILE="$(JAR_KNOWS_ATFILE)" \
	JAR_KNOWS_J_OPTIONS="$(JAR_KNOWS_J_OPTIONS)" \
	JAR_ACCEPTS_STDIN_LIST="$(JAR_ACCEPTS_STDIN_LIST)" \
	DISABLE_NIMBUS="true" \
	NO_DOCS="true" \
	ALT_OUTPUTDIR="$(BOOT_BUILD_OUTPUT_DIR)" \
	XSLT="$(XSLTPROC)" \
	TEST_IN_BUILD="false"

# Source files

GENERATED_FILES = $(top_srcdir)/generated/com/sun/java/swing/plaf/gtk/resources/gtk*.java \
	$(top_srcdir)/generated/com/sun/java/swing/plaf/motif/resources/motif*.java \
	$(top_srcdir)/generated/com/sun/java/swing/plaf/windows/resources/windows*.java \
	$(top_srcdir)/generated/com/sun/corba/se/spi/activation/*.java \
	$(top_srcdir)/generated/com/sun/corba/se/spi/activation/LocatorPackage/*.java \
	$(top_srcdir)/generated/com/sun/corba/se/spi/activation/InitialNameServicePackage/*.java \
	$(top_srcdir)/generated/com/sun/corba/se/spi/activation/RepositoryPackage/*.java \
	$(top_srcdir)/generated/com/sun/corba/se/impl/logging/*.resource \
	$(top_srcdir)/generated/com/sun/corba/se/impl/logging/*.java \
	$(top_srcdir)/generated/com/sun/corba/se/impl/logging/*.properties \
	$(top_srcdir)/generated/com/sun/corba/se/PortableActivationIDL/*.java \
	$(top_srcdir)/generated/com/sun/corba/se/PortableActivationIDL/LocatorPackage/*.java \
	$(top_srcdir)/generated/com/sun/corba/se/PortableActivationIDL/InitialNameServicePackage/*.java \
	$(top_srcdir)/generated/com/sun/corba/se/PortableActivationIDL/RepositoryPackage/*.java \
	$(top_srcdir)/generated/com/sun/swing/internal/plaf/basic/resources/basic*.java \
	$(top_srcdir)/generated/com/sun/swing/internal/plaf/metal/resources/metal*.java \
	$(top_srcdir)/generated/com/sun/swing/internal/plaf/synth/resources/synth*.java \
	$(top_srcdir)/generated/com/sun/tools/apt/resources/apt*.java \
	$(top_srcdir)/generated/com/sun/tools/jdi/*.java \
	$(top_srcdir)/generated/com/sun/tools/jdi/resources/jdi*.java \
	$(top_srcdir)/generated/com/sun/tools/javac/resources/*.java \
	$(top_srcdir)/generated/com/sun/tools/doclets/formats/html/resources/standard*.java \
	$(top_srcdir)/generated/com/sun/tools/doclets/internal/toolkit/resources/doclets*.java \
	$(top_srcdir)/generated/com/sun/tools/javadoc/resources/javadoc*.java \
	$(top_srcdir)/generated/com/sun/accessibility/internal/resources/accessibility*.java \
	$(top_srcdir)/generated/org/omg/IOP/*.java \
	$(top_srcdir)/generated/org/omg/IOP/CodecPackage/*.java \
	$(top_srcdir)/generated/org/omg/IOP/CodecFactoryPackage/*.java \
	$(top_srcdir)/generated/org/omg/CORBA/*.java \
	$(top_srcdir)/generated/org/omg/PortableInterceptor/*.java \
	$(top_srcdir)/generated/org/omg/PortableInterceptor/ORBInitInfoPackage/*.java \
	$(top_srcdir)/generated/org/omg/CosNaming/*.java \
	$(top_srcdir)/generated/org/omg/CosNaming/NamingContextPackage/*.java \
	$(top_srcdir)/generated/org/omg/CosNaming/NamingContextExtPackage/*.java \
	$(top_srcdir)/generated/org/omg/Messaging/*.java \
	$(top_srcdir)/generated/org/omg/Dynamic/*.java \
	$(top_srcdir)/generated/org/omg/DynamicAny/*.java \
	$(top_srcdir)/generated/org/omg/DynamicAny/DynAnyPackage/*.java \
	$(top_srcdir)/generated/org/omg/DynamicAny/DynAnyFactoryPackage/*.java \
	$(top_srcdir)/generated/org/omg/PortableServer/*.java \
	$(top_srcdir)/generated/org/omg/PortableServer/CurrentPackage/*.java \
	$(top_srcdir)/generated/org/omg/PortableServer/POAManagerPackage/*.java \
	$(top_srcdir)/generated/org/omg/PortableServer/POAPackage/*.java \
	$(top_srcdir)/generated/sun/awt/X11/*.java \
	$(top_srcdir)/generated/sun/awt/resources/awt*.java \
	$(top_srcdir)/generated/sun/nio/ch/*.java \
	$(top_srcdir)/generated/sun/nio/cs/*.java \
	$(top_srcdir)/generated/sun/nio/cs/ext/*.java \
	$(top_srcdir)/generated/sun/nio/fs/*.java \
	$(top_srcdir)/generated/sun/misc/*.java \
	$(top_srcdir)/generated/sun/util/*.java \
	$(top_srcdir)/generated/sun/util/logging/resources/logging*.java \
	$(top_srcdir)/generated/sun/util/resources/*.java \
	$(top_srcdir)/generated/sun/print/resources/serviceui*.java \
	$(top_srcdir)/generated/sun/tools/jar/resources/jar*.java \
	$(top_srcdir)/generated/sun/management/resources/agent*.java \
	$(top_srcdir)/generated/java/nio/*.java \
	$(top_srcdir)/generated/java/nio/channels/*.java \
	$(top_srcdir)/generated/java/lang/*.java \
	$(top_srcdir)/generated/java/util/*.java \
	$(top_srcdir)/generated/javax/management/remote/rmi/*.java \
	$(top_srcdir)/generated/fontconfig/fontconfig*.bfc

JTREG_SRCS = $(top_srcdir)/test/jtreg/com/sun/javatest/cof/i18n.properties \
	$(top_srcdir)/test/jtreg/com/sun/javatest/cof/COF2_0_2.xsd \
	$(top_srcdir)/test/jtreg/com/sun/javatest/cof/*.java \
	$(top_srcdir)/test/jtreg/com/sun/javatest/lib/*.java \
	$(top_srcdir)/test/jtreg/com/sun/javatest/diff/*.java \
	$(top_srcdir)/test/jtreg/com/sun/javatest/diff/i18n.properties \
	$(top_srcdir)/test/jtreg/com/sun/javatest/exec/*.java \
	$(top_srcdir)/test/jtreg/com/sun/javatest/exec/i18n.properties \
	$(top_srcdir)/test/jtreg/com/sun/javatest/exec/images/*.gif \
	$(top_srcdir)/test/jtreg/com/sun/javatest/mrep/i18n.properties \
	$(top_srcdir)/test/jtreg/com/sun/javatest/mrep/*.java \
	$(top_srcdir)/test/jtreg/com/sun/javatest/mrep/images/dotdotdot.gif \
	$(top_srcdir)/test/jtreg/com/sun/javatest/mrep/images/Forward16.gif \
	$(top_srcdir)/test/jtreg/com/sun/javatest/mrep/images/Back16.gif \
	$(top_srcdir)/test/jtreg/com/sun/javatest/mrep/images/Home16.gif \
	$(top_srcdir)/test/jtreg/com/sun/javatest/tool/*.java \
	$(top_srcdir)/test/jtreg/com/sun/javatest/tool/i18n.properties \
	$(top_srcdir)/test/jtreg/com/sun/javatest/tool/images/*.gif \
	$(top_srcdir)/test/jtreg/com/sun/javatest/tool/package.html \
	$(top_srcdir)/test/jtreg/com/sun/javatest/util/*.java \
	$(top_srcdir)/test/jtreg/com/sun/javatest/util/i18n.properties \
	$(top_srcdir)/test/jtreg/com/sun/javatest/util/package.html \
	$(top_srcdir)/test/jtreg/com/sun/javatest/*.java \
	$(top_srcdir)/test/jtreg/com/sun/javatest/i18n.properties \
	$(top_srcdir)/test/jtreg/com/sun/javatest/regtest/*.java \
	$(top_srcdir)/test/jtreg/com/sun/javatest/regtest/i18n.properties \
	$(top_srcdir)/test/jtreg/com/sun/javatest/agent/*.java \
	$(top_srcdir)/test/jtreg/com/sun/javatest/agent/i18n.properties \
	$(top_srcdir)/test/jtreg/com/sun/javatest/agent/jticon.gif \
	$(top_srcdir)/test/jtreg/com/sun/javatest/agent/package.html \
	$(top_srcdir)/test/jtreg/com/sun/javatest/batch/i18n.properties \
	$(top_srcdir)/test/jtreg/com/sun/javatest/batch/*.java \
	$(top_srcdir)/test/jtreg/com/sun/javatest/batch/package.html \
	$(top_srcdir)/test/jtreg/com/sun/javatest/audit/i18n.properties \
	$(top_srcdir)/test/jtreg/com/sun/javatest/audit/*.java \
	$(top_srcdir)/test/jtreg/com/sun/javatest/audit/images/dotdotdot.gif \
	$(top_srcdir)/test/jtreg/com/sun/javatest/audit/package.html \
	$(top_srcdir)/test/jtreg/com/sun/javatest/httpd/*.java \
	$(top_srcdir)/test/jtreg/com/sun/javatest/httpd/i18n.properties \
	$(top_srcdir)/test/jtreg/com/sun/javatest/stylesheet.css \
	$(top_srcdir)/test/jtreg/com/sun/javatest/logging/i18n.properties \
	$(top_srcdir)/test/jtreg/com/sun/javatest/logging/*.java \
	$(top_srcdir)/test/jtreg/com/sun/javatest/finder/i18n.properties \
	$(top_srcdir)/test/jtreg/com/sun/javatest/finder/*.java \
	$(top_srcdir)/test/jtreg/com/sun/javatest/finder/package.html \
	$(top_srcdir)/test/jtreg/com/sun/javatest/report/i18n.properties \
	$(top_srcdir)/test/jtreg/com/sun/javatest/report/package.html \
	$(top_srcdir)/test/jtreg/com/sun/javatest/report/*.java \
	$(top_srcdir)/test/jtreg/com/sun/javatest/interview/i18n.properties \
	$(top_srcdir)/test/jtreg/com/sun/javatest/interview/*.java \
	$(top_srcdir)/test/jtreg/com/sun/javatest/interview/package.html \
	$(top_srcdir)/test/jtreg/com/sun/javatest/package.html \
	$(top_srcdir)/test/jtreg/com/sun/interview/i18n.properties \
	$(top_srcdir)/test/jtreg/com/sun/interview/*.java \
	$(top_srcdir)/test/jtreg/com/sun/interview/wizard/*.java \
	$(top_srcdir)/test/jtreg/com/sun/interview/wizard/i18n.properties \
	$(top_srcdir)/test/jtreg/com/sun/interview/wizard/done.gif \
	$(top_srcdir)/test/jtreg/com/sun/interview/wizard/blank.gif \
	$(top_srcdir)/test/jtreg/com/sun/interview/wizard/history.gif \
	$(top_srcdir)/test/jtreg/com/sun/interview/wizard/find.gif \
	$(top_srcdir)/test/jtreg/com/sun/interview/wizard/check.gif \
	$(top_srcdir)/test/jtreg/com/sun/interview/wizard/arrow.gif \
	$(top_srcdir)/test/jtreg/com/sun/interview/wizard/back.gif \
	$(top_srcdir)/test/jtreg/com/sun/interview/wizard/next.gif \
	$(top_srcdir)/test/jtreg/com/sun/interview/wizard/info.gif \
	$(top_srcdir)/test/jtreg/com/sun/interview/wizard/package.html \
	$(top_srcdir)/test/jtreg/com/sun/interview/wizard/cancel.gif \
	$(top_srcdir)/test/jtreg/com/sun/interview/package.html \
	$(top_srcdir)/test/jtreg/JavaTest.toolMgrs.lst \
	$(top_srcdir)/test/jtreg/legal/license.txt \
	$(top_srcdir)/test/jtreg/legal/copyright.txt \
	$(top_srcdir)/test/jtreg/README \
	$(top_srcdir)/test/jtreg/META-INF/MANIFEST.MF \
	$(top_srcdir)/test/jtreg/excludelist.jdk.jtx \
	$(top_srcdir)/test/jtreg/excludelist.jdk.cacao.jtx \
	$(top_srcdir)/test/jtreg/JavaTest.cmdMgrs.lst \
	$(top_srcdir)/test/jtreg/excludelist.langtools.jtx 

DESKTOP_FILES = jconsole-@JAVA_VER@.desktop policytool-@JAVA_VER@.desktop

EXTRA_DIST = $(GENERATED_FILES) $(top_srcdir)/patches/*.patch \
	$(top_srcdir)/patches/boot/*.patch \
	$(top_srcdir)/patches/cacao/*.patch \
	$(top_srcdir)/patches/jamvm/*.patch \
	tools-copy contrib overlays \
	$(JTREG_SRCS) HACKING autogen.sh \
	tz.properties.in \
	scripts/jni_create_stap.c \
	scripts/jni_desc \
	agpl-3.0.txt \
	$(REWRITER_SRCS) \
	test/tapset \
	$(CRYPTO_CHECK_SRCS) \
	$(ECC_CHECK_SRCS) \
	$(MIME_TYPE_CHECK_SRCS)

# Top-Level Targets
# =================

all-local: icedtea-stage2 $(DESKTOP_FILES)

check-local: jtregcheck $(SYSTEMTAP_TEST_SUITE) check-ecc check-mimetype \
 check-java-debug check-java-src

clean-tests: clean-jtreg clean-tapset-report clean-jtreg-reports \
 clean-check-ecc clean-check-mimetype clean-check-java-debug \
 clean-check-java-src

clean-local: clean-tests \
 clean-icedtea clean-icedtea-boot clean-clone clean-clone-boot \
 clean-bootstrap-directory-stage1 clean-bootstrap-directory-stage2 \
 clean-bootstrap-directory-symlink-stage1 clean-bootstrap-directory-symlink-stage2 \
 clean-extract clean-generated clean-native-ecj clean-icedtea-stage2 \
 clean-icedtea-debug-stage2 clean-icedtea-stage1 clean-add-zero clean-add-zero-debug \
 clean-add-cacao clean-add-cacao-debug clean-rt clean-rewrite-rhino clean-rewriter \
 clean-add-systemtap clean-add-systemtap-debug clean-add-nss clean-add-tzdata-support \
 clean-add-tzdata-support-debug clean-cryptocheck clean-policytool-@JAVA_VER@.desktop \
 clean-jconsole-@JAVA_VER@.desktop clean-ecccheck clean-mimetypecheck clean-tz.properties
	if [ -e bootstrap ]; then \
	  rmdir bootstrap ; \
	fi
	if [ -e lib ]; then \
	  rmdir lib ; \
	fi

distclean-local: clean-local clean-download
	if [ $(abs_top_srcdir) != $(abs_top_builddir) ]; then \
	  if [ -e tapset ] ; then \
	    rmdir tapset ; \
	  fi ; \
	fi
	if [ -e stamps ] ; then \
	  rmdir stamps ; \
	fi

.PHONY: \
	env env-boot snapshot clean-clone clean-patch \
	clean-clone-boot clean-patch-boot clean-bootstrap-directory-stage2 \
	clean-bootstrap-directory-symlink-stage2 clean-bootstrap-directory-stage1 \
	clean-bootstrap-directory-symlink-stage1 icedtea icedtea-debug \
	clean-icedtea icedtea-stage2 clean-icedtea-boot \
	clean-rt hotspot hotspot-helper clean-jtreg clean-jtreg-reports \
	jtregcheck clean-remove-intree-libraries clean-check-java-src \
	clean-jamvm clean-extract-jamvm clean-add-jamvm clean-add-jamvm-debug \
	clean-extract-hotspot clean-sanitise-openjdk \
	clean-tests clean-tapset-report clean-add-systemtap \
	clean-add-systemtap-debug clean-add-systemtap-boot \
	clean-add-nss clean-add-tzdata-support clean-add-tzdata-support-debug \
	clean-add-tzdata-support-boot clean-check-mimetype clean-mimetypecheck \
	clean-check-crypto clean-check-crypto-debug clean-check-crypto-boot \
	clean-add-archive clean-add-archive-debug clean-add-archive-boot clean-cryptocheck \
	clean-download-hotspot clean-download-corba clean-download-jaxp \
	clean-download-jaxws clean-download-langtools clean-download-jdk clean-download-openjdk \
	clean-extract-corba clean-extract-jaxp clean-extract-jaxws clean-extract-jdk \
	clean-extract-langtools clean-split-debuginfo clean-split-debuginfo-debug \
	clean-split-debuginfo-boot clean-policytool-@JAVA_VER@.desktop clean-jconsole-@JAVA_VER@.desktop \
	clean-fonts clean-add-mime-types-file clean-add-mime-types-file-debug clean-add-mime-types-file-boot \
	clean-tests clean-tapset-report clean-ecccheck clean-check-ecc clean-tz.properties \
	check-jdk check-hotspot check-langtools jtregcheck check-tapset check-tapset-probes \
	check-tapset-jstack clean-check-java-debug

env:
	@echo 'unset JAVA_HOME'
	@echo 'export $(ICEDTEA_ENV)'

env-boot:
	@echo 'unset JAVA_HOME'
	@echo 'export $(ICEDTEA_ENV_BOOT)'

# Mercurial snapshot.
snapshot: dist
	mv $(DIST_ARCHIVES) $(distdir)-`$(HG) tip --template '{node}'`.tar.gz

# Creates archive of openjdk.
dist-openjdk: stamps/extract-cacao.stamp
	find openjdk/ -name \\.hg* | xargs rm -rf	
	$(ZIP) -r openjdk-@COMBINED_VERSION@ openjdk/

# Creates archive of openjdk that is compliant with Free Software guidelines.
dist-openjdk-fsg: stamps/patch-fsg.stamp
	$(ZIP) -r openjdk-fsg-@COMBINED_VERSION@ openjdk/

dist-openjdk-fsg-xz: stamps/patch-fsg.stamp
	tardir=openjdk/ && $(am__tar) | XZ_OPT=$${XZ_OPT--e} xz -c \
	  >openjdk-fsg-@COMBINED_VERSION@.tar.xz

# OpenJDK Source Preparation Targets
# ==================================

# Download OpenJDK sources.

stamps/download.stamp: stamps/download-openjdk.stamp stamps/download-corba.stamp \
 stamps/download-jaxp.stamp stamps/download-jaxws.stamp stamps/download-jdk.stamp \
 stamps/download-cacao.stamp stamps/download-jamvm.stamp stamps/download-hotspot.stamp \
 stamps/download-langtools.stamp
	mkdir -p stamps
	touch $@

clean-download: clean-download-openjdk clean-download-cacao clean-download-jamvm \
 clean-download-hotspot clean-download-corba clean-download-jaxp clean-download-jaxws \
 clean-download-jdk clean-download-langtools
	rm -f stamps/download.stamp

stamps/download-openjdk.stamp:
if OPENJDK_SRC_DIR_FOUND
else
if USE_HG
else
if USE_ALT_OPENJDK_SRC_ZIP
	ln -sf $(ALT_OPENJDK_SRC_ZIP) $(OPENJDK_SRC_ZIP)
endif
	if ! echo "$(OPENJDK_SUM)  $(OPENJDK_SRC_ZIP)" \
	  | $(OPENJDK_SHA256SUM_BIN) --check ; \
	then \
	  if test "x$(enable_downloading)" = "xyes"; then \
	    if [ -e $(OPENJDK_SRC_ZIP) ] ; then \
	      mv $(OPENJDK_SRC_ZIP) $(OPENJDK_SRC_ZIP).old ; \
	    fi ; \
	    $(WGET) $(ICEDTEA_URL)/$(OPENJDK_SRC_ZIP); \
	    if ! echo "$(OPENJDK_SUM)  $(OPENJDK_SRC_ZIP)" \
	      | $(SHA256SUM) --check ; then \
	      echo "ERROR: Bad download of OpenJDK root zip"; false; \
	    fi; \
	  else \
	    echo "ERROR: No up-to-date OpenJDK root zip available"; exit -1; \
	  fi ; \
	fi ;
endif
endif
	mkdir -p stamps
	touch $@

stamps/download-corba.stamp:
if OPENJDK_SRC_DIR_FOUND
else
if USE_HG
else
if USE_ALT_CORBA_SRC_ZIP
	ln -sf $(ALT_CORBA_SRC_ZIP) $(CORBA_SRC_ZIP)
endif
	if ! echo "$(CORBA_SUM)  $(CORBA_SRC_ZIP)" \
	  | $(CORBA_SHA256SUM_BIN) --check ; \
	then \
	  if test "x$(enable_downloading)" = "xyes"; then \
	    if [ $(CORBA_SRC_ZIP) ] ; then \
	      mv $(CORBA_SRC_ZIP) $(CORBA_SRC_ZIP).old ; \
	    fi ; \
	    $(WGET) $(ICEDTEA_URL)/$(CORBA_SRC_ZIP); \
	    if ! echo "$(CORBA_SUM)  $(CORBA_SRC_ZIP)" \
	      | $(SHA256SUM) --check ; then \
	      echo "ERROR: Bad download of CORBA zip"; false; \
	    fi; \
	  else \
	    echo "ERROR: No up-to-date OpenJDK CORBA zip available"; exit -1; \
	  fi ; \
	fi ;
endif
endif
	mkdir -p stamps
	touch $@

stamps/download-jaxp.stamp:
if OPENJDK_SRC_DIR_FOUND
else
if USE_HG
else
if USE_ALT_JAXP_SRC_ZIP
	ln -sf $(ALT_JAXP_SRC_ZIP) $(JAXP_SRC_ZIP)
endif
	if ! echo "$(JAXP_SUM)  $(JAXP_SRC_ZIP)" \
	  | $(JAXP_SHA256SUM_BIN) --check ; \
	then \
	  if test "x$(enable_downloading)" = "xyes"; then \
	    if [ $(JAXP_SRC_ZIP) ] ; then \
	      mv $(JAXP_SRC_ZIP) $(JAXP_SRC_ZIP).old ; \
	    fi ; \
	    $(WGET) $(ICEDTEA_URL)/$(JAXP_SRC_ZIP); \
	    if ! echo "$(JAXP_SUM)  $(JAXP_SRC_ZIP)" \
	      | $(SHA256SUM) --check ; then \
	      echo "ERROR: Bad download of JAXP zip"; false; \
	    fi; \
	  else \
	    echo "ERROR: No up-to-date JAXP root zip available"; exit -1; \
	  fi ; \
	fi ;
endif
endif
	mkdir -p stamps
	touch $@

stamps/download-jaxws.stamp:
if OPENJDK_SRC_DIR_FOUND
else
if USE_HG
else
if USE_ALT_JAXWS_SRC_ZIP
	ln -sf $(ALT_JAXWS_SRC_ZIP) $(JAXWS_SRC_ZIP)
endif
	if ! echo "$(JAXWS_SUM)  $(JAXWS_SRC_ZIP)" \
	  | $(JAXWS_SHA256SUM_BIN) --check ; \
	then \
	  if test "x$(enable_downloading)" = "xyes"; then \
	    if [ $(JAXWS_SRC_ZIP) ] ; then \
	      mv $(JAXWS_SRC_ZIP) $(JAXWS_SRC_ZIP).old ; \
	    fi ; \
	    $(WGET) $(ICEDTEA_URL)/$(JAXWS_SRC_ZIP); \
	    if ! echo "$(JAXWS_SUM)  $(JAXWS_SRC_ZIP)" \
	      | $(SHA256SUM) --check ; then \
	      echo "ERROR: Bad download of JAXWS zip"; false; \
	    fi; \
	  else \
	    echo "ERROR: No up-to-date OpenJDK JAXWS zip available"; exit -1; \
	  fi ; \
	fi ;
endif
endif
	mkdir -p stamps
	touch $@

stamps/download-jdk.stamp:
if OPENJDK_SRC_DIR_FOUND
else
if USE_HG
else
if USE_ALT_JDK_SRC_ZIP
	ln -sf $(ALT_JDK_SRC_ZIP) $(JDK_SRC_ZIP)
endif
	if ! echo "$(JDK_SUM)  $(JDK_SRC_ZIP)" \
	  | $(JDK_SHA256SUM_BIN) --check ; \
	then \
	  if test "x$(enable_downloading)" = "xyes"; then \
	    if [ $(JDK_SRC_ZIP) ] ; then \
	      mv $(JDK_SRC_ZIP) $(JDK_SRC_ZIP).old ; \
	    fi ; \
	    $(WGET) $(ICEDTEA_URL)/$(JDK_SRC_ZIP); \
	    if ! echo "$(JDK_SUM)  $(JDK_SRC_ZIP)" \
	      | $(SHA256SUM) --check ; then \
	      echo "ERROR: Bad download of JDK zip"; false; \
	    fi; \
	  else \
	    echo "ERROR: No up-to-date OpenJDK JDK zip available"; exit -1; \
	  fi ; \
	fi ;
endif
endif
	mkdir -p stamps
	touch $@

stamps/download-langtools.stamp:
if OPENJDK_SRC_DIR_FOUND
else
if USE_HG
else
if USE_ALT_LANGTOOLS_SRC_ZIP
	ln -sf $(ALT_LANGTOOLS_SRC_ZIP) $(LANGTOOLS_SRC_ZIP)
endif
	if ! echo "$(LANGTOOLS_SUM)  $(LANGTOOLS_SRC_ZIP)" \
	  | $(LANGTOOLS_SHA256SUM_BIN) --check ; \
	then \
	  if test "x$(enable_downloading)" = "xyes"; then \
	    if [ $(LANGTOOLS_SRC_ZIP) ] ; then \
	      mv $(LANGTOOLS_SRC_ZIP) $(LANGTOOLS_SRC_ZIP).old ; \
	    fi ; \
	    $(WGET) $(ICEDTEA_URL)/$(LANGTOOLS_SRC_ZIP); \
	    if ! echo "$(LANGTOOLS_SUM)  $(LANGTOOLS_SRC_ZIP)" \
	      | $(SHA256SUM) --check ; then \
	      echo "ERROR: Bad download of langtools zip"; false; \
	    fi; \
	  else \
	    echo "ERROR: No up-to-date OpenJDK langtools zip available"; exit -1; \
	  fi ; \
	fi ;
endif
endif
	mkdir -p stamps
	touch $@

clean-download-openjdk:
	if [ ! -z $(OPENJDK_SRC_ZIP) ] ; then \
	  rm -f $(OPENJDK_SRC_ZIP) ; \
	fi
	rm -f stamps/download-openjdk.stamp

clean-download-corba:
	if [ ! -z $(CORBA_SRC_ZIP) ] ; then \
	  rm -f $(CORBA_SRC_ZIP) ; \
	fi
	rm -f stamps/download-corba.stamp

clean-download-jaxp:
	if [ ! -z $(JAXP_SRC_ZIP) ] ; then \
	  rm -f $(JAXP_SRC_ZIP) ; \
	fi
	rm -f stamps/download-jaxp.stamp

clean-download-jaxws:
	if [ ! -z $(JAXWS_SRC_ZIP) ] ; then \
	  rm -f $(JAXWS_SRC_ZIP) ; \
	fi
	rm -f stamps/download-jaxws.stamp

clean-download-jdk:
	if [ ! -z $(JDK_SRC_ZIP) ] ; then \
	  rm -f $(JDK_SRC_ZIP) ; \
	fi
	rm -f stamps/download-jdk.stamp

clean-download-langtools:
	if [ ! -z $(LANGTOOLS_SRC_ZIP) ] ; then \
	  rm -f $(LANGTOOLS_SRC_ZIP) ; \
	fi
	rm -f stamps/download-langtools.stamp

stamps/download-cacao.stamp:
if BUILD_CACAO
if !USE_SYSTEM_CACAO
if USE_ALT_CACAO_SRC_DIR
else
if USE_ALT_CACAO_SRC_ZIP
	ln -sf $(ALT_CACAO_SRC_ZIP) $(CACAO_SRC_ZIP)
endif
	if ! echo "$(CACAO_SUM)  $(CACAO_SRC_ZIP)" \
	 | $(CACAO_SHA256SUM_BIN) --check ; \
	then \
	 if test "x$(enable_downloading)" = "xyes"; then \
	   if [ -e $(CACAO_SRC_ZIP) ] ; then \
	     mv $(CACAO_SRC_ZIP) $(CACAO_SRC_ZIP).old ; \
	   fi ; \
	   $(WGET) $(CACAO_URL) -O $(CACAO_SRC_ZIP); \
	   if ! echo "$(CACAO_SUM)  $(CACAO_SRC_ZIP)" \
	     | $(SHA256SUM) --check ; then \
	     echo "ERROR: Bad download of CACAO zip"; false; \
	   fi; \
	 else \
	   echo "ERROR: No up-to-date CACAO zip available"; exit -1; \
	 fi ; \
	fi
endif
endif
endif
	mkdir -p stamps
	touch $@

clean-download-cacao:
	rm -f $(CACAO_SRC_ZIP)
	rm -f stamps/download-cacao.stamp

stamps/download-jamvm.stamp:
if BUILD_JAMVM
if USE_ALT_JAMVM_SRC_ZIP
	ln -sf $(ALT_JAMVM_SRC_ZIP) $(JAMVM_SRC_ZIP)
endif
	if ! echo "$(JAMVM_SUM)  $(JAMVM_SRC_ZIP)" \
	 | $(JAMVM_SHA256SUM_BIN) --check ; \
	then \
	  if test "x$(enable_downloading)" = "xyes"; then \
	    if [ -e $(JAMVM_SRC_ZIP) ] ; then \
	      mv $(JAMVM_SRC_ZIP) $(JAMVM_SRC_ZIP).old ; \
	    fi ; \
	    $(WGET) $(JAMVM_URL) -O $(JAMVM_SRC_ZIP); \
	    if ! echo "$(JAMVM_SUM)  $(JAMVM_SRC_ZIP)" \
	      | $(SHA256SUM) --check ; then \
	      echo "ERROR: Bad download of JamVM zip"; false; \
	    fi; \
	  else \
	    echo "ERROR: No up-to-date JamVM zip available"; exit -1; \
	  fi ; \
	fi
endif
	mkdir -p stamps
	touch $@

clean-download-jamvm:
	rm -f $(JAMVM_SRC_ZIP)
	rm -f stamps/download-jamvm.stamp

stamps/download-hotspot.stamp:
if OPENJDK_SRC_DIR_FOUND
else
if USE_HG
else
if USE_ALT_HOTSPOT_SRC_ZIP
	ln -sf $(ALT_HOTSPOT_SRC_ZIP) $(HOTSPOT_SRC_ZIP)
endif
	echo "Using HotSpot build: ${HSBUILD}"
	if ! echo "$(HOTSPOT_SUM)  $(HOTSPOT_SRC_ZIP)" \
	  | $(HOTSPOT_SHA256SUM_BIN) --check ; \
	then \
	  if test "x$(enable_downloading)" = "xyes"; then \
	    if [ -e $(HOTSPOT_SRC_ZIP) ] ; then \
	      mv $(HOTSPOT_SRC_ZIP) $(HOTSPOT_SRC_ZIP).old ; \
	    fi ; \
	    if test "x$(HS_TYPE)" = "xhg"; then \
	      $(WGET) $(HS_URL)/archive/$(HS_CHANGESET).$(TAR_SUFFIX) -O $(HOTSPOT_SRC_ZIP) ; \
	    else \
		if test "x$(HSBUILD)" = "xdefault"; then \
	          $(WGET) $(HS_URL)/$(HOTSPOT_SRC_ZIP) ; \
		else \
		  $(WGET) $(HS_URL)/$(HSBUILD).$(TAR_SUFFIX) -O $(HOTSPOT_SRC_ZIP) ; \
		fi ; \
            fi ; \
	    if ! echo "$(HOTSPOT_SUM)  $(HOTSPOT_SRC_ZIP)" \
	      | $(SHA256SUM) --check ; then \
	      echo "ERROR: Bad download of HotSpot zip"; false; \
	    fi; \
	  else \
	    echo "ERROR: No up-to-date OpenJDK HotSpot zip available"; exit -1; \
	  fi ; \
	fi
endif
endif
	mkdir -p stamps
	touch $@

clean-download-hotspot:
	if [ ! -z $(HOTSPOT_SRC_ZIP) ] ; then \
	  rm -f $(HOTSPOT_SRC_ZIP) ; \
	fi
	rm -f stamps/download-hotspot.stamp

stamps/extract.stamp: stamps/extract-openjdk.stamp stamps/extract-corba.stamp \
 stamps/extract-jaxp.stamp stamps/extract-jaxws.stamp stamps/extract-jdk.stamp \
 stamps/extract-cacao.stamp stamps/remove-intree-libraries.stamp stamps/extract-langtools.stamp \
 stamps/extract-jamvm.stamp stamps/extract-hotspot.stamp stamps/sanitise-openjdk.stamp
	mkdir -p stamps
	touch $@

clean-extract: clean-extract-openjdk clean-extract-cacao clean-extract-jamvm
	rm -f stamps/extract.stamp

stamps/extract-openjdk.stamp: stamps/download-openjdk.stamp
if OPENJDK_SRC_DIR_FOUND
	cp -pPR $(OPENJDK_SRC_DIR_LINK) $(OPENJDK_SRC_DIR) openjdk
else
if USE_HG
	$(HG) clone $(REV_ARG) $(ICEDTEA_HG_URL) openjdk;
else
	set -e ; \
	if ! test -d openjdk ; then \
	  if [ ! -z $(OPENJDK_SRC_ZIP) ] ; then \
	    case "$(OPENJDK_SRC_ZIP)" in \
	      *.zip) $(UNZIP) -q $(OPENJDK_SRC_ZIP) ;; \
	      *.tar.*) $(TAR) xf $(OPENJDK_SRC_ZIP) ;; \
	    esac; \
	    mv `echo $(ICEDTEA_PREFIX)|$(SED) 's#\.#-#'`-$(OPENJDK_CHANGESET) openjdk ; \
	  else \
	    echo "ERROR: Couldn't extract OpenJDK"; \
	    false; \
	  fi; \
	fi; 
endif
endif
	touch $@

clean-extract-openjdk: clean-overlay clean-versioning \
 clean-patch-fsg clean-remove-intree-libraries \
 clean-sanitise-openjdk clean-extract-hotspot \
 clean-extract-jdk clean-extract-jaxp clean-extract-jaxws \
 clean-extract-corba clean-extract-langtools clean-fonts
	rm -rf openjdk
	rm -f stamps/extract-openjdk.stamp

stamps/extract-corba.stamp: stamps/extract-openjdk.stamp stamps/download-corba.stamp
if OPENJDK_SRC_DIR_FOUND
else
if USE_HG
	$(HG) clone $(REV_ARG) $(ICEDTEA_HG_URL)/corba openjdk/corba
else
	set -e ; \
	if [ ! -z $(CORBA_SRC_ZIP) ] ; then \
	  if test -e ${CORBA_SRC_ZIP} ; \
	  then \
	    if ! test -d openjdk/corba ; \
	    then \
	      case "$(CORBA_SRC_ZIP)" in \
	        *.zip) $(UNZIP) -q $(CORBA_SRC_ZIP) ;; \
	        *.tar.*) $(TAR) xf $(CORBA_SRC_ZIP) ;; \
	      esac; \
	      mv corba-$(CORBA_CHANGESET) openjdk/corba ; \
	    fi ; \
	  fi ; \
	fi
endif
endif
	touch $@

clean-extract-corba:
	rm -rf openjdk/corba
	rm -f stamps/extract-corba.stamp

stamps/extract-jaxp.stamp: stamps/extract-openjdk.stamp stamps/download-jaxp.stamp
if OPENJDK_SRC_DIR_FOUND
else
if USE_HG
	$(HG) clone $(REV_ARG) $(ICEDTEA_HG_URL)/jaxp openjdk/jaxp
else
	set -e ; \
	if [ ! -z $(JAXP_SRC_ZIP) ] ; then \
	  if test -e ${JAXP_SRC_ZIP} ; \
	  then \
	    if ! test -d openjdk/jaxp ; \
	    then \
	      case "$(JAXP_SRC_ZIP)" in \
	        *.zip) $(UNZIP) -q $(JAXP_SRC_ZIP) ;; \
	        *.tar.*) $(TAR) xf $(JAXP_SRC_ZIP) ;; \
	      esac; \
	      mv jaxp-$(JAXP_CHANGESET) openjdk/jaxp ; \
	    fi ; \
	  fi ; \
	fi
endif
endif
	touch $@

clean-extract-jaxp:
	rm -rf openjdk/jaxp
	rm -f stamps/extract-jaxp.stamp

stamps/extract-jaxws.stamp: stamps/extract-openjdk.stamp stamps/download-jaxws.stamp
if OPENJDK_SRC_DIR_FOUND
else
if USE_HG
	$(HG) clone $(REV_ARG) $(ICEDTEA_HG_URL)/jaxws openjdk/jaxws
else
	set -e ; \
	if [ ! -z $(JAXWS_SRC_ZIP) ] ; then \
	  if test -e ${JAXWS_SRC_ZIP} ; \
	  then \
	    if ! test -d openjdk/jaxws ; \
	    then \
	      case "$(JAXWS_SRC_ZIP)" in \
	        *.zip) $(UNZIP) -q $(JAXWS_SRC_ZIP) ;; \
	        *.tar.*) $(TAR) xf $(JAXWS_SRC_ZIP) ;; \
	      esac; \
	      mv jaxws-$(JAXWS_CHANGESET) openjdk/jaxws ; \
	    fi ; \
	  fi ; \
	fi
endif
endif
	touch $@

clean-extract-jaxws:
	rm -rf openjdk/jaxws
	rm -f stamps/extract-jaxws.stamp

stamps/extract-jdk.stamp: stamps/extract-openjdk.stamp stamps/download-jdk.stamp
if OPENJDK_SRC_DIR_FOUND
else
if USE_HG
	$(HG) clone $(REV_ARG) $(ICEDTEA_HG_URL)/jdk openjdk/jdk
else
	set -e ; \
	if [ ! -z $(JDK_SRC_ZIP) ] ; then \
	  if test -e ${JDK_SRC_ZIP} ; \
	  then \
	    if ! test -d openjdk/jdk ; \
	    then \
	      case "$(JDK_SRC_ZIP)" in \
	        *.zip) $(UNZIP) -q $(JDK_SRC_ZIP) ;; \
	        *.tar.*) $(TAR) xf $(JDK_SRC_ZIP) ;; \
	      esac; \
	      mv jdk-$(JDK_CHANGESET) openjdk/jdk ; \
	    fi ; \
	  fi ; \
	fi
endif
endif
	touch $@

clean-extract-jdk:
	rm -rf openjdk/jdk
	rm -f stamps/extract-jdk.stamp

stamps/extract-langtools.stamp: stamps/extract-openjdk.stamp stamps/download-langtools.stamp
if OPENJDK_SRC_DIR_FOUND
else
if USE_HG
	$(HG) clone $(REV_ARG) $(ICEDTEA_HG_URL)/langtools openjdk/langtools
else
	set -e ; \
	if [ ! -z $(LANGTOOLS_SRC_ZIP) ] ; then \
	  if test -e ${LANGTOOLS_SRC_ZIP} ; \
	  then \
	    if ! test -d openjdk/langtools ; \
	    then \
	      case "$(LANGTOOLS_SRC_ZIP)" in \
	        *.zip) $(UNZIP) -q $(LANGTOOLS_SRC_ZIP) ;; \
	        *.tar.*) $(TAR) xf $(LANGTOOLS_SRC_ZIP) ;; \
	      esac; \
	      mv langtools-$(LANGTOOLS_CHANGESET) openjdk/langtools ; \
	    fi ; \
	  fi ; \
	fi
endif
endif
	touch $@

clean-extract-langtools:
	rm -rf openjdk/langtools
	rm -f stamps/extract-langtools.stamp

stamps/extract-hotspot.stamp: stamps/extract-openjdk.stamp stamps/download-hotspot.stamp
if OPENJDK_SRC_DIR_FOUND
else
if USE_HG
	$(HG) clone $(REV_ARG) $(HS_URL) openjdk/hotspot
else
	set -e ; \
	if [ ! -z $(HOTSPOT_SRC_ZIP) ] ; then \
	  if test -e ${HOTSPOT_SRC_ZIP} ; \
	  then \
	    case "$(HOTSPOT_SRC_ZIP)" in \
	      *.zip) $(UNZIP) -q $(HOTSPOT_SRC_ZIP) ;; \
	      *.tar.*) $(TAR) xf $(HOTSPOT_SRC_ZIP) ;; \
	    esac; \
	    if test "x${HSBUILD}" != "xdefault"; then \
	      rm -rf openjdk/hotspot ; \
	    fi ; \
	    if ! test -d openjdk/hotspot ; \
	    then \
	      mv hotspot-$(HS_CHANGESET) openjdk/hotspot ; \
	    else \
	      rm -rf hotspot-$(HS_CHANGESET) ; \
	    fi ; \
	  fi ; \
	fi
endif
endif
	touch $@

clean-extract-hotspot:
	rm -rf openjdk/hotspot
	rm -f stamps/extract-hotspot.stamp

stamps/sanitise-openjdk.stamp: stamps/extract-openjdk.stamp \
 stamps/extract-hotspot.stamp stamps/extract-jdk.stamp \
 stamps/extract-langtools.stamp stamps/extract-corba.stamp \
 stamps/extract-jaxp.stamp stamps/extract-jaxws.stamp
	chmod -R ug+w openjdk
	chmod +x $(builddir)/fsg.sh
	sh $(builddir)/fsg.sh
	touch stamps/sanitise-openjdk.stamp

clean-sanitise-openjdk:
	rm -f stamps/sanitise-openjdk.stamp

stamps/extract-cacao.stamp: stamps/download-cacao.stamp
if BUILD_CACAO
if !USE_SYSTEM_CACAO
if USE_ALT_CACAO_SRC_DIR
	if ! test -d cacao ; \
	then \
	  mkdir -p cacao/cacao ; \
	  cp -r $(ALT_CACAO_SRC_DIR)/* cacao/cacao/ ; \
	fi
else
	set -e ; \
	if ! test -d cacao/cacao ; \
	then \
	  mkdir cacao ; \
	  $(TAR) xzf $(CACAO_SRC_ZIP) -C cacao ; \
	  dir=$$(basename cacao/cacao-*) ; \
	  ln -s $$dir cacao/cacao ; \
	  cd cacao/cacao ; \
	  cd .. ; \
	fi
endif
endif
endif
	mkdir -p stamps
	touch $@

clean-extract-cacao: clean-cacao
	rm -f stamps/extract-cacao.stamp

stamps/extract-jamvm.stamp: stamps/download-jamvm.stamp
if BUILD_JAMVM
	set -e ; \
	if ! test -d jamvm ; \
	then \
	  mkdir jamvm ; \
	  $(TAR) xzf $(JAMVM_SRC_ZIP) -C jamvm ; \
	  dir=$$(basename jamvm/jamvm-*) ; \
	  ln -s $$dir jamvm/jamvm ; \
	fi
endif
	mkdir -p stamps
	touch stamps/extract-jamvm.stamp

clean-extract-jamvm: clean-jamvm
	rm -f stamps/extract-jamvm.stamp

stamps/remove-intree-libraries.stamp: stamps/sanitise-openjdk.stamp
	sh $(abs_top_builddir)/remove-intree-libraries.sh
	touch stamps/remove-intree-libraries.stamp

clean-remove-intree-libraries:
	rm -f stamps/remove-intree-libraries.stamp

stamps/generated.stamp:
	set -e ; \
	if [ ! -e $(abs_top_builddir)/generated.build ]; then \
	  cp -pPR $(abs_top_srcdir)/generated $(abs_top_builddir)/generated.build; \
	  chmod -R u+w $(abs_top_builddir)/generated.build ; \
	fi
	mkdir -p stamps
	touch $@

clean-generated:
	rm -rf $(abs_top_builddir)/generated.build
	rm -f stamps/generated.stamp

stamps/fonts.stamp: stamps/extract.stamp
	cp $(FONTCONFIG_PATH)/linux.fontconfig.Gentoo.properties{,.old}
	cp linux.fontconfig.Gentoo.properties $(FONTCONFIG_PATH)
	mkdir -p stamps
	touch $@

clean-fonts:
	rm -f $(FONTCONFIG_PATH)/linux.fontconfig.Gentoo.properties
	if [ -e $(FONTCONFIG_PATH)/linux.fontconfig.Gentoo.properties.old ] ; then \
	  mv $(FONTCONFIG_PATH)/linux.fontconfig.Gentoo.properties{.old,} ; \
	fi
	rm -f stamps/fonts.stamp

stamps/patch-fsg.stamp: stamps/extract.stamp stamps/fonts.stamp
	mkdir -p stamps ; \
	rm -f stamps/patch-fsg.stamp.tmp ; \
	touch stamps/patch-fsg.stamp.tmp ; \
	all_patches_ok=yes; \
	for p in $(ICEDTEA_FSG_PATCHES) ; \
	do \
	  if test x$${all_patches_ok} = "xyes" \
	    && echo Checking $$p \
	    && $(PATCH) -l -p0 --dry-run -s -t -f \
	      -F 0 < $(abs_top_srcdir)/$$p ; \
	  then \
	    echo Applying $$p ; \
	    $(PATCH) -l -p0 < $(abs_top_srcdir)/$$p ; \
	    if ! grep "^\* $$(basename $$p)" $(abs_top_srcdir)/HACKING \
	      >> stamps/patch-fsg.stamp.tmp ; \
	    then \
	      echo "* $$(basename $$p): UNDOCUMENTED" \
	        >> stamps/patch-fsg.stamp.tmp ; \
	    fi ; \
	  else \
	    test x$${all_patches_ok} = "xyes" && all_patches_ok=$$p ; \
	  fi ; \
	done ; \
	mv stamps/patch-fsg.stamp.tmp stamps/patch-fsg.stamp ; \
	if ! test x$${all_patches_ok} = "xyes"; then \
	  echo ERROR patch $${all_patches_ok} FAILED! ; \
	  echo WARNING make clean-patch before retrying a fix ; \
	  exit 2; \
	fi ; 

clean-patch-fsg: clean-patch
	if [ -e stamps/patch-fsg.stamp ] ; then \
	  all_patches_ok=yes; \
	  for p in $(ICEDTEA_FSG_PATCHES) ; \
	  do \
	    echo Checking $$p ; \
	    if $(PATCH) -l -p0 -R --dry-run -s -t -f < $(abs_top_srcdir)/$$p ; \
	    then \
	      echo Reverting $$p ; \
	      $(PATCH) -l -p0 -R < $(abs_top_srcdir)/$$p ; \
	    else \
	      all_patches_ok=no ; \
	    fi ; \
	  done ; \
	  if ! test x$${all_patches_ok} = "xyes" ; then \
	    echo "WARNING Not all patches reverted cleanly" ; \
	  fi ; \
	  rm -f stamps/patch-fsg.stamp ; \
	fi

stamps/patch.stamp: stamps/patch-fsg.stamp
	mkdir -p stamps ; \
	rm -f stamps/patch.stamp.tmp ; \
	touch stamps/patch.stamp.tmp ; \
	all_patches_ok=yes; \
	cd $(abs_top_builddir)/openjdk/jdk; \
	for p in $(OPENJDK_PATCHES) ; \
	do \
	  if test x$${all_patches_ok} == "xyes" \
	     && echo Checking $$p \
	     && $(PATCH) -l -p1 --dry-run -s -t -f -F 0 < $(abs_top_srcdir)/$$p ; \
	  then \
	    echo Applying $$p ; \
	    $(PATCH) -l -p1 < $(abs_top_srcdir)/$$p ; \
	  else \
	    test x$${all_patches_ok} == "xyes" && all_patches_ok=$$p ; \
	  fi ; \
	done ; \
	cd $(abs_top_builddir) ; \
	for p in $(ICEDTEA_PATCHES) ; \
	do \
	  if test x$${all_patches_ok} = "xyes" \
	    && echo Checking $$p \
	    && $(PATCH) -l -p0 --dry-run -s -t -f \
	      -F 0 < $(abs_top_srcdir)/$$p ; \
	  then \
	    echo Applying $$p ; \
	    $(PATCH) -l -p0 < $(abs_top_srcdir)/$$p ; \
	    if ! grep "^\* $$(basename $$p)" $(abs_top_srcdir)/HACKING \
	      >> stamps/patch.stamp.tmp ; \
	    then \
	      echo "* $$(basename $$p): UNDOCUMENTED" \
	        >> stamps/patch.stamp.tmp ; \
	    fi ; \
	  else \
	    test x$${all_patches_ok} = "xyes" && all_patches_ok=$$p ; \
	  fi ; \
	done ; \
	if ! test x$${all_patches_ok} = "xyes"; then \
	  echo ERROR patch $${all_patches_ok} FAILED! ; \
	  echo WARNING make clean-patch before retrying a fix ; \
	  exit 2; \
	fi ; \
	mv stamps/patch.stamp.tmp stamps/patch.stamp 

clean-patch:
	if [ -e stamps/patch.stamp ] ; then \
	  all_patches_ok=yes; \
	  for p in $(ICEDTEA_PATCHES) ; \
	  do \
	    echo Checking $$p ; \
	    if $(PATCH) -l -p0 -R --dry-run -s -t -f < $(abs_top_srcdir)/$$p ; \
	    then \
	      echo Reverting $$p ; \
	      $(PATCH) -l -p0 -R < $(abs_top_srcdir)/$$p ; \
	    else \
	      all_patches_ok=no ; \
	    fi ; \
	  done ; \
	  cd $(abs_top_builddir)/openjdk/jdk; \
	  for p in $(OPENJDK_PATCHES) ; \
	  do \
	    echo Checking $$p ; \
	    if $(PATCH) -l -p1 -R --dry-run -s -t -f < $(abs_top_srcdir)/$$p ; \
	    then \
	      echo Reverting $$p ; \
	      $(PATCH) -l -p1 -R < $(abs_top_srcdir)/$$p ; \
	    else \
	      all_patches_ok=no ; \
	    fi ; \
	  done ; \
	  cd $(abs_top_builddir) ; \
	  if ! test x$${all_patches_ok} = "xyes" ; then \
	    echo "WARNING Not all patches reverted cleanly" ; \
	  fi ; \
	  rm -f stamps/patch.stamp ; \
	fi

stamps/versioning.stamp: stamps/patch.stamp
	mv openjdk/jdk/make/common/shared/Defs.gmk Defs.gmk.bak
	cat Defs.gmk.bak > openjdk/jdk/make/common/shared/Defs.gmk
	icedtea_version="$(PACKAGE_VERSION)$(ICEDTEA_REV)" ; \
	if test "x$(ENABLE_CACAO)" = "xyes" || \
	   test "x$(ENABLE_JAMVM)" = "xyes"; then \
	  echo "JDK_DERIVATIVE_NAME=$${icedtea_version}" \
	    >>openjdk/jdk/make/common/shared/Defs.gmk ; \
	  echo "PRODUCT_NAME=$(ICEDTEA_NAME)" \
	    >>openjdk/jdk/make/common/shared/Defs.gmk ; \
	else \
	  echo "JDK_DERIVATIVE_NAME=$(ICEDTEA_NAME) $${icedtea_version}" \
	    >>openjdk/jdk/make/common/shared/Defs.gmk ; \
	fi ; 

if HAS_JDK_REVISION
	echo "JDK_REVID=$(JDK_REVISION)" >>openjdk/jdk/make/common/shared/Defs.gmk ;
endif

	echo "DISTRO_NAME=$(DIST_NAME)" >>openjdk/jdk/make/common/shared/Defs.gmk ;
if HAS_PKGVERSION
	  echo "DISTRO_PACKAGE_VERSION=$(PKGVERSION)" \
	    >>openjdk/jdk/make/common/shared/Defs.gmk ;
endif
	if test x"$(VERSION_SUFFIX)" != "x"; then \
	  ver_suffix="-$(VERSION_SUFFIX)"; \
	fi ; \
	$(SED) -i "s#BUILD_VARIANT_RELEASE)#BUILD_VARIANT_RELEASE)$${ver_suffix}#" \
	  openjdk/jdk/make/common/shared/Defs.gmk;
	mkdir -p stamps
	touch $@

clean-versioning:
	if [ -e Defs.gmk.bak ] ; then \
	  mv Defs.gmk.bak openjdk/jdk/make/common/shared/Defs.gmk ; \
	fi
	rm -f stamps/versioning.stamp

stamps/overlay.stamp: stamps/versioning.stamp
	cp -r $(abs_top_srcdir)/overlays/openjdk/* openjdk/
	chmod -R ug+w openjdk
	mkdir -p stamps
	touch $@

clean-overlay:
	rm -f stamps/overlay.stamp

# OpenJDK boot Source Preparation Targets
# ======================================

# Copy over OpenJDK sources for bootstrap.
stamps/clone-boot.stamp: $(OPENJDK_TREE)
	if ! test -d openjdk-boot ; then \
	    cp -pPRl openjdk openjdk-boot ; \
	    if [ -e $(BUILD_OUTPUT_DIR) ] ; then \
	      cp -pPRl $(BUILD_OUTPUT_DIR) $(BOOT_BUILD_OUTPUT_DIR) ; \
	    fi ; \
	fi
	mkdir -p stamps
	touch $@

clean-clone-boot: 
	rm -rf openjdk-boot
	rm -f stamps/patch-boot.stamp
	rm -f stamps/clone-boot.stamp

stamps/patch-boot.stamp: stamps/clone-boot.stamp
	mkdir -p stamps; \
	rm -f stamps/patch-boot.stamp.tmp ; \
	touch stamps/patch-boot.stamp.tmp ; \
	all_patches_ok=yes; \
	for p in $(ICEDTEA_BOOT_PATCHES) ; \
	do \
	  if test x$${all_patches_ok} = "xyes" \
	    && echo Checking $$p \
	    && $(PATCH) -l -p0 --dry-run -s -t -f \
	      -F 0 < $(abs_top_srcdir)/$$p ; \
	  then \
	    echo Applying $$p ; \
	    $(PATCH) -l -p0 < $(abs_top_srcdir)/$$p ; \
	    if ! grep "^\* $$(basename $$p)" $(abs_top_srcdir)/HACKING \
	      >> stamps/patch-boot.stamp.tmp ; \
	    then \
	      echo "* $$(basename $$p): UNDOCUMENTED" \
	        >> stamps/patch-boot.stamp.tmp ; \
	    fi ; \
	  else \
	    test x$${all_patches_ok} = "xyes" && all_patches_ok=$$p ; \
	  fi ; \
	done ; \
	mv stamps/patch-boot.stamp.tmp stamps/patch-boot.stamp ; \
	if ! test x$${all_patches_ok} = "xyes"; then \
	  echo ERROR patch $${all_patches_ok} FAILED! ; \
	  echo WARNING make clean-patch before retrying a fix ; \
	  exit 2; \
	fi

clean-patch-boot:
	if [ -e stamps/patch-boot.stamp ] ; then \
	  all_patches_ok=yes; \
	  for p in $(ICEDTEA_BOOT_PATCHES) ; \
	  do \
	    echo Checking $$p ; \
	    if $(PATCH) -l -p0 -R --dry-run -s -t -f < $(abs_top_srcdir)/$$p ; \
	    then \
	      echo Reverting $$p ; \
	      $(PATCH) -l -p0 -R < $(abs_top_srcdir)/$$p ; \
	    else \
	      all_patches_ok=no ; \
	    fi ; \
	  done ; \
	  if ! test x$${all_patches_ok} = "xyes" ; then \
	    echo "WARNING Not all patches reverted cleanly" ; \
	  fi ; \
	  rm -f stamps/patch-boot.stamp ; \
	fi

# Bootstrap Directory Targets
# ===========================

# Optional native ecj
stamps/native-ecj.stamp:
	mkdir -p stamps 
if BUILD_NATIVE_ECJ
	${GCJ} $(IT_CFLAGS) -Wl,-Bsymbolic -findirect-dispatch -o native-ecj \
	    --main=org.eclipse.jdt.internal.compiler.batch.Main -lgcj ${ECJ_JAR}
endif
	touch $@

clean-native-ecj:
	rm -f native-ecj
	rm -rf stamps/native-ecj.stamp

# bootstrap/stage1
stamps/bootstrap-directory-stage1.stamp: stamps/native-ecj.stamp
	mkdir -p $(STAGE1_BOOT_DIR)/bin stamps/
	ln -sfv $(JAVA) $(STAGE1_BOOT_DIR)/bin/java
if JAVAH_SUPPORTS_X_OPTIONS
	ln -sfv $(JAVAH) $(STAGE1_BOOT_DIR)/bin/javah
else
	ln -sfv ../../../javah $(STAGE1_BOOT_DIR)/bin/javah
endif
	ln -sfv $(RMIC) $(STAGE1_BOOT_DIR)/bin/rmic
	ln -sfv $(JAR) $(STAGE1_BOOT_DIR)/bin/jar
	ln -sfv $(NATIVE2ASCII) $(STAGE1_BOOT_DIR)/bin/native2ascii
	ln -sfv ../../../javac $(STAGE1_BOOT_DIR)/bin/javac
	ln -sfv ../../../javap $(STAGE1_BOOT_DIR)/bin/javap
	mkdir -p $(STAGE1_BOOT_DIR)/lib/modules
	mkdir -p $(STAGE1_BOOT_DIR)/jre/lib && \
	cp $(SYSTEM_JDK_DIR)/jre/lib/rt.jar $(STAGE1_BOOT_RUNTIME) && \
	chmod u+w $(STAGE1_BOOT_RUNTIME)
	mkdir -p $(STAGE1_BOOT_DIR)/lib && \
	if [ -e $(SYSTEM_JDK_DIR)/lib/tools.jar ] ; then \
	  ln -sfv $(SYSTEM_JDK_DIR)/lib/tools.jar $(STAGE1_BOOT_DIR)/lib/tools.jar ; \
	else \
	  ln -sfv $(STAGE1_BOOT_RUNTIME) $(STAGE1_BOOT_DIR)/lib/tools.jar ; \
	fi
# Workaround some older ppc64le builds installing to 'ppc64' or 'ppc64el' rather than 'ppc64le'
	if test -d $(SYSTEM_JDK_DIR)/jre/lib/ppc64 ; then \
	  ln -sfv $(SYSTEM_JDK_DIR)/jre/lib/ppc64 \
	    $(STAGE1_BOOT_DIR)/jre/lib/$(JRE_ARCH_DIR) ; \
	elif test -d $(SYSTEM_JDK_DIR)/jre/lib/ppc64el ; then \
	  ln -sfv $(SYSTEM_JDK_DIR)/jre/lib/ppc64el \
	    $(STAGE1_BOOT_DIR)/jre/lib/$(JRE_ARCH_DIR) ; \
	else \
	  ln -sfv $(SYSTEM_JDK_DIR)/jre/lib/$(JRE_ARCH_DIR) \
	    $(STAGE1_BOOT_DIR)/jre/lib/ ; \
	fi
	if ! test -d $(STAGE1_BOOT_DIR)/jre/lib/$(INSTALL_ARCH_DIR); \
	  then \
	  ln -sfv ./$(JRE_ARCH_DIR) \
	    $(STAGE1_BOOT_DIR)/jre/lib/$(INSTALL_ARCH_DIR); \
	fi
	mkdir -p $(STAGE1_BOOT_DIR)/include && \
	for i in $(SYSTEM_JDK_DIR)/include/*; do \
	  test -r $$i | continue; \
	  i=`basename $$i`; \
	  rm -f $(STAGE1_BOOT_DIR)/include/$$i; \
	  ln -sv $(SYSTEM_JDK_DIR)/include/$$i $(STAGE1_BOOT_DIR)/include/$$i; \
	done;
	mkdir -p stamps
	touch $@

clean-bootstrap-directory-stage1:
	rm -rf $(STAGE1_BOOT_DIR)
	rm -f stamps/bootstrap-directory-stage1.stamp

# bootstrap/stage2.
stamps/bootstrap-directory-stage2.stamp: stamps/icedtea-stage1.stamp
	rm -rf $(STAGE2_BOOT_DIR)
	ln -s $(BUILD_BOOT_SDK_DIR)/ $(STAGE2_BOOT_DIR)
	if ! test -d $(STAGE2_BOOT_DIR)/include; then \
	  ln -sf $(BOOT_BUILD_OUTPUT_DIR)/include $(STAGE2_BOOT_DIR)/include; \
	fi
	mkdir -p stamps
	touch $@

clean-bootstrap-directory-stage2:
	rm -rf $(STAGE2_BOOT_DIR)
	rm -f stamps/bootstrap-directory-stage2.stamp

# stage 1 symlink.
stamps/bootstrap-directory-symlink-stage1.stamp: stamps/bootstrap-directory-stage1.stamp
	rm -f $(BOOT_DIR)
	ln -sf $(STAGE1_BOOT_DIR) $(BOOT_DIR)
	mkdir -p stamps
	touch $@

clean-bootstrap-directory-symlink-stage1:
	rm -f $(BOOT_DIR)
	rm -f stamps/bootstrap-directory-symlink-stage1.stamp

# stage 2 symlink.
stamps/bootstrap-directory-symlink-stage2.stamp: $(BOOTSTRAP_DIRECTORY_STAMP)
	mkdir -p bootstrap
	rm -f $(BOOT_DIR)
	ln -sf $(ICEDTEA_HOME) $(BOOT_DIR); \
	if ! $(BOOT_DIR)/bin/java -version ; \
         then \
           echo "$(BOOT_DIR)/bin/java" \
               "cannot be found or is corrupted." ; \
           exit 1; \
        fi
	mkdir -p stamps
	touch $@

clean-bootstrap-directory-symlink-stage2:
	rm -f $(BOOT_DIR)
	rm -f stamps/bootstrap-directory-symlink-stage2.stamp

# OpenJDK Targets
# ===============

# If you change anything here in the icedtea target, please make sure
# you change it in the icedtea-debug target as well.
stamps/icedtea.stamp: stamps/bootstrap-directory-symlink-stage2.stamp \
 stamps/download.stamp stamps/extract.stamp $(OPENJDK_TREE) \
 stamps/cacao.stamp stamps/rewrite-rhino.stamp stamps/jamvm.stamp
	$(ICEDTEA_UNSET) $(ARCH_PREFIX) $(MAKE) -j1 \
	  $(ICEDTEA_ENV) \
	  -C openjdk/ \
	  $(ICEDTEA_BUILD_TARGET)
	$(abs_top_builddir)/pax-mark-vm $(BUILD_SDK_DIR) true
	mkdir -p $(BUILD_SDK_DIR)/jre/lib/$(INSTALL_ARCH_DIR)
	mkdir -p $(BUILD_JRE_DIR)/lib/$(INSTALL_ARCH_DIR)
	mkdir -p $(BUILD_SDK_DIR)/jre/lib/ext
	mkdir -p $(BUILD_JRE_DIR)/lib/ext 
if ZERO_BUILD
	printf -- '-zero ALIASED_TO -server\n' >> $(BUILD_JRE_ARCH_DIR)/jvm.cfg
endif
if SHARK_BUILD
	printf -- '-shark ALIASED_TO -server\n' >> $(BUILD_JRE_ARCH_DIR)/jvm.cfg
endif
if ENABLE_CACAO
	printf -- '-cacao ALIASED_TO -server\n' >> $(BUILD_JRE_ARCH_DIR)/jvm.cfg
endif
if ENABLE_JAMVM
	printf -- '-jamvm ALIASED_TO -server\n' >> $(BUILD_JRE_ARCH_DIR)/jvm.cfg
endif
# Fix permissions on JARs updated during build if PR1437 bug is present
if PR1437_PRESENT
	if [ -e $(BUILD_SDK_DIR)/lib/sa-jdi.jar ] ; then \
	  chmod g+r,o+r $(BUILD_SDK_DIR)/lib/sa-jdi.jar ; \
	fi
	chmod g+r,o+r $(BUILD_JRE_DIR)/lib/resources.jar
	chmod g+r,o+r $(BUILD_SDK_DIR)/jre/lib/resources.jar
endif
	@echo "IcedTea is served:" $(BUILD_SDK_DIR)
	mkdir -p stamps
	touch $@

stamps/add-systemtap.stamp: stamps/icedtea.stamp
if ENABLE_SYSTEMTAP
	mkdir -p $(BUILD_SDK_DIR)/tapset ; \
	grep "client IGNORE" $(BUILD_JRE_ARCH_DIR)/jvm.cfg; \
	if test $$? -eq 0; then \
	  $(SED) -e '/\/client\/libjvm.so/d' \
	    < $(abs_top_builddir)/tapset/hotspot.stp \
	    > $(BUILD_SDK_DIR)/tapset/hotspot.stp; \
	  $(SED) -e '/\/client\/libjvm.so/d' \
	    < $(abs_top_builddir)/tapset/hotspot_jni.stp \
	    > $(BUILD_SDK_DIR)/tapset/hotspot_jni.stp; \
	  $(SED) -e '/\/client\/libjvm.so/d' \
	    < $(abs_top_builddir)/tapset/hotspot_gc.stp \
	    > $(BUILD_SDK_DIR)/tapset/hotspot_gc.stp; \
	else \
	  cp $(abs_top_builddir)/tapset/hotspot.stp \
	    $(BUILD_SDK_DIR)/tapset/hotspot.stp; \
	  cp $(abs_top_builddir)/tapset/hotspot_jni.stp \
	    $(BUILD_SDK_DIR)/tapset/hotspot_jni.stp; \
	  cp $(abs_top_builddir)/tapset/hotspot_gc.stp \
	    $(BUILD_SDK_DIR)/tapset/hotspot_gc.stp; \
	fi; \
	cp $(abs_top_builddir)/tapset/jstack.stp \
	  $(BUILD_SDK_DIR)/tapset/jstack.stp
endif
	touch stamps/add-systemtap.stamp

clean-add-systemtap:
	rm -rf $(BUILD_SDK_DIR)/tapset
	rm -f stamps/add-systemtap.stamp

stamps/add-nss.stamp: stamps/extract-jdk.stamp
	cp -v $(abs_top_builddir)/nss.cfg openjdk/jdk/src/share/lib/security;
	touch stamps/add-nss.stamp

clean-add-nss:
	rm -f openjdk/jdk/src/share/lib/security/nss.cfg
	rm -f stamps/add-nss.stamp

stamps/add-tzdata-support.stamp: stamps/icedtea.stamp tz.properties
if WITH_TZDATA_DIR
	if [ -e $(BUILD_SDK_DIR)/jre/lib ] ; then \
	  cp -v $(abs_top_builddir)/tz.properties \
	    $(BUILD_SDK_DIR)/jre/lib ; \
	fi
endif
	touch stamps/add-tzdata-support.stamp

clean-add-tzdata-support:
	if [ -e $(BUILD_SDK_DIR)/jre/lib/tz.properties ] ; then \
	  rm -vf $(BUILD_SDK_DIR)/jre/lib/tz.properties ; \
	fi
	rm -f stamps/add-tzdata-support.stamp

stamps/check-crypto.stamp: stamps/cryptocheck.stamp stamps/icedtea.stamp
	if [ -e $(BUILD_SDK_DIR)/bin/java ] ; then \
	  $(BUILD_SDK_DIR)/bin/java -cp $(CRYPTO_CHECK_BUILD_DIR) TestCryptoLevel ; \
	fi
	mkdir -p stamps
	touch $@

clean-check-crypto:
	rm -f stamps/check-crypto.stamp

stamps/add-archive.stamp: stamps/icedtea.stamp
if !ENABLE_JAMVM
if !ENABLE_CACAO
if !ZERO_BUILD
	if [ -e $(BUILD_SDK_DIR)/bin/java ] ; then \
	  if test "x$(INSTALL_ARCH_DIR)" != "xppc64" -a "x$(INSTALL_ARCH_DIR)" != "xppc64le" ; then \
	    $(BUILD_SDK_DIR)/bin/java -Xshare:dump ; \
	  fi ; \
	fi
endif
endif
endif
	touch stamps/add-archive.stamp

clean-add-archive:
	rm -vf $(BUILD_JRE_ARCH_DIR)/*/*.jsa
	rm -f stamps/add-archive.stamp

stamps/split-debuginfo.stamp: stamps/icedtea.stamp
if SPLIT_DEBUGINFO
	( cd $(BUILD_SDK_DIR) ; \
	  for files in $$($(FIND) . -type f) ; do \
	    if $(FILE) $${files} | $(GREP) 'ELF' > /dev/null ; then \
	      mkdir -p $(BUILD_DEBUGINFO_DIR)/$$(dirname $${files}) ; \
	      $(OBJCOPY) --only-keep-debug $${files} $(BUILD_DEBUGINFO_DIR)/$${files}.debug ; \
	      $(OBJCOPY) --strip-debug $${files} $(BUILD_DEBUGINFO_DIR)/$${files}.stripped ; \
	      $(OBJCOPY) --add-gnu-debuglink=$(BUILD_DEBUGINFO_DIR)/$${files}.debug \
	        $(BUILD_DEBUGINFO_DIR)/$${files}.stripped ; \
	    fi ; \
	  done ; \
	)
endif
	touch $@

clean-split-debuginfo:
	rm -rvf $(BUILD_DEBUGINFO_DIR)
	rm -f stamps/split-debuginfo.stamp

stamps/add-mime-types-file.stamp: stamps/icedtea.stamp
if MIME_TYPES_FILE_FOUND
	if [ -e $(BUILD_SDK_DIR)/jre/lib ] ; then \
	  ln -sf $(MIME_TYPES_FILE) $(BUILD_SDK_DIR)/jre/lib/mime.types ; \
	fi
endif
	touch $@

clean-add-mime-types-file:
	if [ -e $(BUILD_SDK_DIR)/jre/lib/mime.types ] ; then \
	  rm -vf $(BUILD_SDK_DIR)/jre/lib/mime.types ; \
	fi
	rm -f stamps/add-mime-types-file.stamp

stamps/icedtea-debug.stamp: stamps/bootstrap-directory-symlink-stage2.stamp \
 stamps/download.stamp stamps/extract.stamp $(OPENJDK_TREE) \
 stamps/cacao.stamp stamps/rewrite-rhino.stamp stamps/jamvm.stamp
	$(ICEDTEA_UNSET) $(ARCH_PREFIX) $(MAKE) -j1 \
	  $(ICEDTEA_ENV) \
	  -C openjdk/ \
	  $(ICEDTEA_DEBUG_BUILD_TARGET)
	$(abs_top_builddir)/pax-mark-vm $(BUILD_DEBUG_SDK_DIR) true
	mkdir -p $(BUILD_DEBUG_SDK_DIR)/jre/lib/$(INSTALL_ARCH_DIR)
	mkdir -p $(BUILD_DEBUG_JRE_DIR)/lib/$(INSTALL_ARCH_DIR)
	mkdir -p $(BUILD_DEBUG_SDK_DIR)/jre/lib/ext
	mkdir -p $(BUILD_DEBUG_JRE_DIR)/lib/ext 
if ZERO_BUILD
	printf -- '-zero ALIASED_TO -server\n' >> $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg
endif
if SHARK_BUILD
	printf -- '-shark ALIASED_TO -server\n' >> $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg
endif
if ENABLE_CACAO
	printf -- '-cacao ALIASED_TO -server\n' >> $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg
endif
if ENABLE_JAMVM
	printf -- '-jamvm ALIASED_TO -server\n' >> $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg
endif
# Fix permissions on JARs updated during build if PR1437 bug is present
if PR1437_PRESENT
	if [ -e $(BUILD_DEBUG_SDK_DIR)/lib/sa-jdi.jar ] ; then \
	  chmod g+r,o+r $(BUILD_DEBUG_SDK_DIR)/lib/sa-jdi.jar ; \
	fi
	chmod g+r,o+r $(BUILD_DEBUG_JRE_DIR)/lib/resources.jar
	chmod g+r,o+r $(BUILD_DEBUG_SDK_DIR)/jre/lib/resources.jar
endif
	@echo "IcedTea (debug build) is served:" $(BUILD_DEBUG_SDK_DIR)
	mkdir -p stamps
	touch $@

clean-icedtea: 
	rm -rf $(BUILD_OUTPUT_DIR)
	rm -f stamps/icedtea.stamp stamps/icedtea-debug.stamp

stamps/add-systemtap-debug.stamp: stamps/icedtea-debug.stamp
if ENABLE_SYSTEMTAP
	mkdir -p $(BUILD_DEBUG_SDK_DIR)/tapset ; \
	grep "client IGNORE" $(BUILD_JRE_ARCH_DIR)/jvm.cfg; \
	if test $$? -eq 0; then \
	  $(SED) -e '/\/client\/libjvm.so/d' \
	    < $(abs_top_builddir)/tapset/hotspot.stp \
	    > $(BUILD_DEBUG_SDK_DIR)/tapset/hotspot.stp; \
	  $(SED) -e '/\/client\/libjvm.so/d' \
	    < $(abs_top_builddir)/tapset/hotspot_jni.stp \
	    > $(BUILD_DEBUG_SDK_DIR)/tapset/hotspot_jni.stp; \
	  $(SED) -e '/\/client\/libjvm.so/d' \
	    < $(abs_top_builddir)/tapset/hotspot_gc.stp \
	    > $(BUILD_DEBUG_SDK_DIR)/tapset/hotspot_gc.stp; \
	else \
	  cp $(abs_top_builddir)/tapset/hotspot.stp \
	    $(BUILD_DEBUG_SDK_DIR)/tapset/hotspot.stp; \
	  cp $(abs_top_builddir)/tapset/hotspot_jni.stp \
	    $(BUILD_DEBUG_SDK_DIR)/tapset/hotspot_jni.stp; \
	  cp $(abs_top_builddir)/tapset/hotspot_gc.stp \
	    $(BUILD_DEBUG_SDK_DIR)/tapset/hotspot_gc.stp; \
	fi; \
	cp $(abs_top_builddir)/tapset/jstack.stp \
	  $(BUILD_DEBUG_SDK_DIR)/tapset/jstack.stp
endif
	touch stamps/add-systemtap-debug.stamp

clean-add-systemtap-debug:
	rm -rf $(BUILD_DEBUG_SDK_DIR)/tapset
	rm -f stamps/add-systemtap-debug.stamp

stamps/add-tzdata-support-debug.stamp: stamps/icedtea-debug.stamp tz.properties
if WITH_TZDATA_DIR
	if [ -e $(BUILD_DEBUG_SDK_DIR)/jre/lib ] ; then \
	  cp -v $(abs_top_builddir)/tz.properties \
	    $(BUILD_DEBUG_SDK_DIR)/jre/lib; \
	fi
endif
	touch stamps/add-tzdata-support-debug.stamp

clean-add-tzdata-support-debug:
	if [ -e $(BUILD_DEBUG_SDK_DIR)/jre/lib/tz.properties ] ; then \
	  rm -vf $(BUILD_DEBUG_SDK_DIR)/jre/lib/tz.properties ; \
	fi
	rm -f stamps/add-tzdata-support-debug.stamp

stamps/check-crypto-debug.stamp: stamps/cryptocheck.stamp stamps/icedtea-debug.stamp
	if [ -e $(BUILD_DEBUG_SDK_DIR)/bin/java ] ; then \
	  $(BUILD_DEBUG_SDK_DIR)/bin/java -cp $(CRYPTO_CHECK_BUILD_DIR) TestCryptoLevel ; \
	fi
	mkdir -p stamps
	touch $@

clean-check-crypto-debug:
	rm -f stamps/check-crypto-debug.stamp

stamps/add-archive-debug.stamp: stamps/icedtea-debug.stamp
if !ENABLE_JAMVM
if !ENABLE_CACAO
if !ZERO_BUILD
	if [ -e $(BUILD_DEBUG_SDK_DIR)/bin/java ] ; then \
	  if test "x$(INSTALL_ARCH_DIR)" != "xppc64" -a "x$(INSTALL_ARCH_DIR)" != "xppc64le" ; then \
	    $(BUILD_DEBUG_SDK_DIR)/bin/java -Xshare:dump ; \
	  fi ; \
	fi
endif
endif
endif
	touch stamps/add-archive-debug.stamp

clean-add-archive-debug:
	rm -vf $(BUILD_DEBUG_JRE_ARCH_DIR)/*/*.jsa
	rm -f stamps/add-archive-debug.stamp

stamps/split-debuginfo-debug.stamp: stamps/icedtea-debug.stamp
if SPLIT_DEBUGINFO
	( cd $(BUILD_DEBUG_SDK_DIR) ; \
	  for files in $$($(FIND) . -type f) ; do \
	    if $(FILE) $${files} | $(GREP) 'ELF' > /dev/null ; then \
	      mkdir -p $(BUILD_DEBUG_DEBUGINFO_DIR)/$$(dirname $${files}) ; \
	      $(OBJCOPY) --only-keep-debug $${files} $(BUILD_DEBUG_DEBUGINFO_DIR)/$${files}.debug ; \
	      $(OBJCOPY) --strip-debug $${files} $(BUILD_DEBUG_DEBUGINFO_DIR)/$${files}.stripped ; \
	      $(OBJCOPY) --add-gnu-debuglink=$(BUILD_DEBUG_DEBUGINFO_DIR)/$${files}.debug \
	        $(BUILD_DEBUG_DEBUGINFO_DIR)/$${files}.stripped ; \
	    fi ; \
	  done ; \
	)
endif
	touch $@

clean-split-debuginfo-debug:
	rm -rvf $(BUILD_DEBUG_DEBUGINFO_DIR)
	rm -f stamps/split-debuginfo-debug.stamp

stamps/add-mime-types-file-debug.stamp: stamps/icedtea-debug.stamp
if MIME_TYPES_FILE_FOUND
	if [ -e $(BUILD_DEBUG_SDK_DIR)/jre/lib ] ; then \
	  ln -sf $(MIME_TYPES_FILE) $(BUILD_DEBUG_SDK_DIR)/jre/lib/mime.types ; \
	fi
endif
	touch $@

clean-add-mime-types-file-debug:
	if [ -e $(BUILD_DEBUG_SDK_DIR)/jre/lib/mime.types ] ; then \
	  rm -vf $(BUILD_DEBUG_SDK_DIR)/jre/lib/mime.types ; \
	fi
	rm -f stamps/add-mime-types-file-debug.stamp

stamps/icedtea-stage2.stamp: stamps/icedtea.stamp stamps/add-cacao.stamp \
 stamps/add-zero.stamp stamps/add-jamvm.stamp stamps/add-systemtap.stamp \
 stamps/add-tzdata-support.stamp stamps/check-crypto.stamp stamps/add-archive.stamp \
 stamps/split-debuginfo.stamp stamps/add-mime-types-file.stamp
	mkdir -p stamps
	touch $@

clean-icedtea-stage2: clean-add-jamvm clean-check-crypto clean-add-archive clean-split-debuginfo \
 clean-add-mime-types-file
	rm -f stamps/icedtea-stage2.stamp

stamps/icedtea-debug-stage2.stamp: stamps/icedtea-debug.stamp \
 stamps/add-cacao-debug.stamp stamps/add-zero-debug.stamp stamps/add-jamvm-debug.stamp \
 stamps/add-systemtap-debug.stamp stamps/add-tzdata-support-debug.stamp \
 stamps/check-crypto-debug.stamp stamps/add-archive-debug.stamp stamps/split-debuginfo-debug.stamp \
 stamps/add-mime-types-file-debug.stamp
	mkdir -p stamps
	touch $@

clean-icedtea-debug-stage2: clean-add-jamvm-debug clean-check-crypto-debug \
 clean-add-archive-debug clean-split-debuginfo-debug clean-add-mime-types-file-debug
	rm -f stamps/icedtea-debug-stage2.stamp

# OpenJDK boot Targets
# ===================

stamps/icedtea-boot.stamp: stamps/bootstrap-directory-symlink-stage1.stamp \
 stamps/download.stamp $(OPENJDK_BOOT_TREE) stamps/cacao.stamp \
 stamps/rewrite-rhino.stamp stamps/jamvm.stamp stamps/generated.stamp
	$(ICEDTEA_UNSET) $(ARCH_PREFIX) $(MAKE) -j1 \
	  $(ICEDTEA_ENV_BOOT) \
	  -C openjdk-boot \
	  $(ICEDTEA_BUILD_TARGET)
	$(abs_top_builddir)/pax-mark-vm $(BUILD_BOOT_SDK_DIR) true
# Fix permissions on JARs updated during build if PR1437 bug is present
if PR1437_PRESENT
	if [ -e $(BUILD_BOOT_SDK_DIR)/lib/sa-jdi.jar ] ; then \
	  chmod g+r,o+r $(BUILD_BOOT_SDK_DIR)/lib/sa-jdi.jar ; \
	fi
	chmod g+r,o+r $(BUILD_BOOT_JRE_DIR)/lib/resources.jar
	chmod g+r,o+r $(BUILD_BOOT_SDK_DIR)/jre/lib/resources.jar
endif
	@echo "Bootstrapped IcedTea is served:" $(BUILD_BOOT_SDK_DIR)
	mkdir -p stamps
	touch $@

clean-icedtea-boot:
	rm -rf $(BOOT_BUILD_OUTPUT_DIR)
	rm -f stamps/icedtea-boot.stamp

stamps/add-systemtap-boot.stamp: stamps/icedtea-boot.stamp
if ENABLE_SYSTEMTAP
	mkdir -p $(BUILD_BOOT_SDK_DIR)/tapset ; \
	grep "client IGNORE" $(BUILD_BOOT_JRE_ARCH_DIR)/jvm.cfg; \
	if test $$? -eq 0; then \
	  $(SED) -e '/\/client\/libjvm.so/d' \
	    < $(abs_top_builddir)/tapset/hotspot.stp \
	    > $(BUILD_BOOT_SDK_DIR)/tapset/hotspot.stp; \
	  $(SED) -e '/\/client\/libjvm.so/d' \
	    < $(abs_top_builddir)/tapset/hotspot_jni.stp \
	    > $(BUILD_BOOT_SDK_DIR)/tapset/hotspot_jni.stp; \
	  $(SED) -e '/\/client\/libjvm.so/d' \
	    < $(abs_top_builddir)/tapset/hotspot_gc.stp \
	    > $(BUILD_BOOT_SDK_DIR)/tapset/hotspot_gc.stp; \
	else \
	  cp $(abs_top_builddir)/tapset/hotspot.stp \
	    $(BUILD_BOOT_SDK_DIR)/tapset/hotspot.stp; \
	  cp $(abs_top_builddir)/tapset/hotspot_jni.stp \
	    $(BUILD_BOOT_SDK_DIR)/tapset/hotspot_jni.stp; \
	  cp $(abs_top_builddir)/tapset/hotspot_gc.stp \
	    $(BUILD_BOOT_SDK_DIR)/tapset/hotspot_gc.stp; \
	fi; \
	cp $(abs_top_builddir)/tapset/jstack.stp \
	  $(BUILD_BOOT_SDK_DIR)/tapset/jstack.stp
endif
	touch stamps/add-systemtap-boot.stamp

clean-add-systemtap-boot:
	rm -rf $(BUILD_BOOT_SDK_DIR)/tapset
	rm -f stamps/add-systemtap-boot.stamp

stamps/add-tzdata-support-boot.stamp: stamps/icedtea-boot.stamp tz.properties
if WITH_TZDATA_DIR
	if [ -e $(BUILD_BOOT_SDK_DIR)/jre/lib ] ; then \
	  cp -v $(abs_top_builddir)/tz.properties \
	    $(BUILD_BOOT_SDK_DIR)/jre/lib; \
	fi
endif
	touch stamps/add-tzdata-support-boot.stamp

clean-add-tzdata-support-boot:
	if [ -e $(BUILD_BOOT_SDK_DIR)/jre/lib/tz.properties ] ; then \
	  rm -vf $(BUILD_BOOT_SDK_DIR)/jre/lib/tz.properties ; \
	fi
	rm -f stamps/add-tzdata-support-boot.stamp

stamps/check-crypto-boot.stamp: stamps/cryptocheck.stamp stamps/icedtea-boot.stamp
	if [ -e $(BUILD_BOOT_SDK_DIR)/bin/java ] ; then \
	  $(BUILD_BOOT_SDK_DIR)/bin/java -cp $(CRYPTO_CHECK_BUILD_DIR) TestCryptoLevel ; \
	fi
	mkdir -p stamps
	touch $@

clean-check-crypto-boot:
	rm -f stamps/check-crypto-boot.stamp

stamps/add-archive-boot.stamp: stamps/icedtea-boot.stamp
if !ENABLE_JAMVM
if !ENABLE_CACAO
if !ZERO_BUILD
	if [ -e $(BUILD_BOOT_SDK_DIR)/bin/java ] ; then \
	  if test "x$(INSTALL_ARCH_DIR)" != "xppc64" -a "x$(INSTALL_ARCH_DIR)" != "xppc64le" ; then \
	    $(BUILD_BOOT_SDK_DIR)/bin/java -Xshare:dump ; \
	  fi ; \
	fi
endif
endif
endif
	touch $@

clean-add-archive-boot:
	rm -vf $(BUILD_BOOT_JRE_ARCH_DIR)/*/*.jsa
	rm -f stamps/add-archive-boot.stamp

stamps/split-debuginfo-boot.stamp: stamps/icedtea-boot.stamp
if SPLIT_DEBUGINFO
	( cd $(BUILD_BOOT_SDK_DIR) ; \
	  for files in $$($(FIND) . -type f) ; do \
	    if $(FILE) $${files} | $(GREP) 'ELF' > /dev/null ; then \
	      mkdir -p $(BUILD_BOOT_DEBUGINFO_DIR)/$$(dirname $${files}) ; \
	      $(OBJCOPY) --only-keep-debug $${files} $(BUILD_BOOT_DEBUGINFO_DIR)/$${files}.debug ; \
	      $(OBJCOPY) --strip-debug $${files} $(BUILD_BOOT_DEBUGINFO_DIR)/$${files}.stripped ; \
	      $(OBJCOPY) --add-gnu-debuglink=$(BUILD_BOOT_DEBUGINFO_DIR)/$${files}.debug \
	        $(BUILD_BOOT_DEBUGINFO_DIR)/$${files}.stripped ; \
	    fi ; \
	  done ; \
	)
endif
	touch $@

clean-split-debuginfo-boot:
	rm -rvf $(BUILD_BOOT_DEBUGINFO_DIR)
	rm -f stamps/split-debuginfo-boot.stamp

stamps/add-mime-types-file-boot.stamp: stamps/icedtea-boot.stamp
if MIME_TYPES_FILE_FOUND
	if [ -e $(BUILD_BOOT_SDK_DIR)/jre/lib ] ; then \
	  ln -sf $(MIME_TYPES_FILE) $(BUILD_BOOT_SDK_DIR)/jre/lib/mime.types ; \
	fi
endif
	touch $@

clean-add-mime-types-file-boot:
	if [ -e $(BUILD_BOOT_SDK_DIR)/jre/lib/mime.types ] ; then \
	  rm -vf $(BUILD_BOOT_SDK_DIR)/jre/lib/mime.types ; \
	fi
	rm -f stamps/add-mime-types-file-boot.stamp

stamps/icedtea-stage1.stamp: stamps/icedtea-boot.stamp \
 stamps/add-systemtap-boot.stamp stamps/add-tzdata-support-boot.stamp \
 stamps/check-crypto-boot.stamp stamps/add-archive-boot.stamp \
 stamps/split-debuginfo-boot.stamp stamps/add-mime-types-file-boot.stamp
	mkdir -p stamps
	touch $@

clean-icedtea-stage1: clean-add-systemtap-boot clean-add-tzdata-support-boot \
 clean-check-crypto-boot clean-add-archive-boot clean-split-debuginfo-boot \
 clean-add-mime-types-file-boot
	rm -f stamps/icedtea-stage1.stamp

# Rebuild targets

rebuild:
if BOOTSTRAPPING
	rm -f stamps/icedtea-boot.stamp
	rm -f stamps/icedtea-stage1.stamp
	$(ARCH_PREFIX) $(MAKE) icedtea-stage1
	@echo "bootstrap-poured IcedTea is served (again):" $(BUILD_BOOT_SDK_DIR)
else
	rm -f stamps/icedtea.stamp
	rm -f stamps/icedtea-stage2.stamp
	$(ARCH_PREFIX) $(MAKE) icedtea-stage2
	@echo "IcedTea is served (again):" $(BUILD_SDK_DIR)
endif

hotspot:
if BOOTSTRAPPING
	rm -f stamps/icedtea-boot.stamp
	rm -f stamps/icedtea-stage1.stamp
	$(ARCH_PREFIX) $(MAKE) ICEDTEA_BUILD_TARGET=hotspot icedtea-stage1
	mkdir -p \
	   $(BUILD_BOOT_SDK_DIR)/jre/lib/$(INSTALL_ARCH_DIR)/server
	cp $(BOOT_BUILD_OUTPUT_DIR)/$(SERVER_DIR)/libjvm.so \
	   $(BUILD_BOOT_SDK_DIR)/jre/lib/$(INSTALL_ARCH_DIR)/server
	@echo "HotSpot is served:" $(BUILD_BOOT_SDK_DIR)
else
	rm -f stamps/icedtea.stamp
	rm -f stamps/icedtea-stage2.stamp
	$(ARCH_PREFIX) $(MAKE) ICEDTEA_BUILD_TARGET=hotspot icedtea-stage2
	mkdir -p \
	   $(BUILD_SDK_DIR)/jre/lib/$(INSTALL_ARCH_DIR)/server
	cp $(BUILD_OUTPUT_DIR)/$(SERVER_DIR)/libjvm.so \
	   $(BUILD_SDK_DIR)/jre/lib/$(INSTALL_ARCH_DIR)/server
	@echo "HotSpot is served:" $(BUILD_SDK_DIR)
endif

# Rhino support

stamps/rewriter.stamp: $(INITIAL_BOOTSTRAP_LINK_STAMP)
if WITH_RHINO
	mkdir -p $(REWRITER_BUILD_DIR)
	$(BOOT_DIR)/bin/javac $(IT_JAVACFLAGS) \
	 -d $(REWRITER_BUILD_DIR) $(REWRITER_SRCS)
endif
	mkdir -p stamps
	touch stamps/rewriter.stamp

clean-rewriter:
	rm -rf $(REWRITER_BUILD_DIR)
	rm -f stamps/rewriter.stamp

stamps/rewrite-rhino.stamp: stamps/rewriter.stamp $(RHINO_JAR)
if WITH_RHINO
	mkdir -p rhino/rhino.{old,new} && \
	(cd rhino/rhino.old && $(BOOT_DIR)/bin/jar xf $(RHINO_JAR)) && \
	$(BOOT_DIR)/bin/java -cp $(REWRITER_BUILD_DIR) \
	  com.redhat.rewriter.ClassRewriter \
	  $(abs_top_builddir)/rhino/rhino.old $(abs_top_builddir)/rhino/rhino.new \
	  org.mozilla sun.org.mozilla && \
	(cd rhino/rhino.old && \
	 for files in `find -type f -not -name '*.class'` ; do \
	   new_file=../rhino.new/`echo $$files|$(SED) -e 's#org#sun/org#'` ; \
	   mkdir -p `dirname $$new_file` ; \
	   cp -v $$files $$new_file ; \
	   $(SED) -ie 's#org\.mozilla#sun.org.mozilla#g' $$new_file ; \
	 done \
	) && \
	(cd rhino/rhino.new && \
	 $(BOOT_DIR)/bin/jar cfm ../rhino.jar META-INF/MANIFEST.MF sun )
endif
	mkdir -p stamps
	touch stamps/rewrite-rhino.stamp

clean-rewrite-rhino:
	rm -rf rhino/rhino.old
	rm -rf rhino/rhino.new
	rm -f rhino/rhino.jar
	if [ -e rhino ] ; then \
	  rmdir rhino ; \
	fi
	rm -f stamps/rewrite-rhino.stamp

# JamVM

stamps/jamvm.stamp: $(OPENJDK_TREE) stamps/rt.stamp
if BUILD_JAMVM
	cd jamvm/jamvm && \
	./autogen.sh --with-java-runtime-library=openjdk7 \
	  --prefix=$(abs_top_builddir)/jamvm/install ; \
	$(MAKE) ; \
	$(MAKE) install
	mkdir -p $(abs_top_builddir)/jamvm/install/hotspot/jre/lib/$(INSTALL_ARCH_DIR)/server
	cp $(abs_top_builddir)/jamvm/install/lib/libjvm.so $(abs_top_builddir)/jamvm/install/hotspot/jre/lib/$(INSTALL_ARCH_DIR)/server
	ln -sf server $(abs_top_builddir)/jamvm/install/hotspot/jre/lib/$(INSTALL_ARCH_DIR)/client
	touch $(abs_top_builddir)/jamvm/install/hotspot/jre/lib/$(INSTALL_ARCH_DIR)/server/Xusage.txt
	ln -sf client/libjvm.so $(abs_top_builddir)/jamvm/install/hotspot/jre/lib/$(INSTALL_ARCH_DIR)/libjsig.so
endif
	mkdir -p stamps
	touch stamps/jamvm.stamp

clean-jamvm:
	rm -rf jamvm
	rm -f stamps/jamvm.stamp

# Targets for additional VMs

stamps/add-jamvm.stamp: stamps/icedtea.stamp stamps/jamvm.stamp
if ADD_JAMVM_BUILD
	mkdir -p $(BUILD_JRE_ARCH_DIR)/jamvm
	install -m 644 jamvm/install/lib/libjvm.so \
		$(BUILD_JRE_ARCH_DIR)/jamvm/
	printf -- '-jamvm KNOWN\n' >> $(BUILD_JRE_ARCH_DIR)/jvm.cfg
else
	printf -- '-jamvm ERROR\n' >> $(BUILD_JRE_ARCH_DIR)/jvm.cfg
endif
	touch $@

clean-add-jamvm:
	rm -rf $(BUILD_JRE_ARCH_DIR)/jamvm
	if [ -e $(BUILD_JRE_ARCH_DIR)/jvm.cfg ] ; then \
	  $(SED) -i 's#-jamvm KNOWN#-jamvm ERROR#' $(BUILD_JRE_ARCH_DIR)/jvm.cfg ; \
	fi
	rm -f stamps/add-jamvm.stamp

stamps/add-jamvm-debug.stamp: stamps/icedtea-debug.stamp stamps/jamvm.stamp
if ADD_JAMVM_BUILD
	mkdir -p $(BUILD_DEBUG_JRE_ARCH_DIR)/jamvm
	install -m 644 jamvm/install/lib/libjvm.so \
		$(BUILD_DEBUG_JRE_ARCH_DIR)/jamvm/
	printf -- '-jamvm KNOWN\n' >> $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg
else
	printf -- '-jamvm ERROR\n' >> $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg
endif
	touch $@

clean-add-jamvm-debug:
	rm -rf $(BUILD_DEBUG_JRE_ARCH_DIR)/jamvm
	if [ -e $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg ] ; then \
	  $(SED) -i 's#-jamvm KNOWN#-jamvm ERROR#' $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg ; \
	fi
	rm -f stamps/add-jamvm-debug.stamp

# CACAO

stamps/cacao.stamp: stamps/rt.stamp
if BUILD_CACAO
if !USE_SYSTEM_CACAO
	cd cacao/cacao && \
	./autogen.sh && \
	$(ARCH_PREFIX) ./configure \
	  --host=$(host_alias) \
	  --build=$(build_alias) \
	  --target=$(target_alias) \
	  --prefix=$(abs_top_builddir)/cacao/install \
	  --with-java-runtime-library=openjdk7 \
	  --with-java-runtime-library-prefix=$(abs_top_builddir)/openjdk \
	  --with-java-runtime-library-classes=$(RUNTIME) \
	  --with-libatomic-ops=no \
	  --enable-jre-layout $(CACAO_CONFIGURE_ARGS); \
	$(ARCH_PREFIX) $(MAKE) -j$(PARALLEL_JOBS) install
	ln -sf server $(abs_top_builddir)/cacao/install/jre/lib/$(INSTALL_ARCH_DIR)/client
	touch $(abs_top_builddir)/cacao/install/jre/lib/$(INSTALL_ARCH_DIR)/server/Xusage.txt
	ln -sf client/libjvm.so $(abs_top_builddir)/cacao/install/jre/lib/$(INSTALL_ARCH_DIR)/libjsig.so
endif
endif
	mkdir -p stamps
	touch $@

clean-cacao:
	rm -rf cacao
	rm -f stamps/cacao.stamp

# targets for additional VMs

stamps/add-cacao.stamp: stamps/icedtea.stamp stamps/cacao.stamp
if ADD_CACAO_BUILD
	mkdir -p $(BUILD_JRE_ARCH_DIR)/cacao
	install -m 644 cacao/install/lib/libjvm.so \
		$(BUILD_JRE_ARCH_DIR)/cacao/
	install -m 644 cacao/install/jre/lib/$(INSTALL_ARCH_DIR)/libjsig.so \
		$(BUILD_JRE_ARCH_DIR)/cacao/
	install -m 644 cacao/install/jre/lib/$(INSTALL_ARCH_DIR)/server/Xusage.txt \
		$(BUILD_JRE_ARCH_DIR)/cacao/
	printf -- '-cacao KNOWN\n' >> $(BUILD_JRE_ARCH_DIR)/jvm.cfg
else
	printf -- '-cacao ERROR\n' >> $(BUILD_JRE_ARCH_DIR)/jvm.cfg
endif
	touch $@

clean-add-cacao:
	rm -rf $(BUILD_JRE_ARCH_DIR)/cacao
	if [ -e $(BUILD_JRE_ARCH_DIR)/jvm.cfg ] ; then \
	  $(SED) -i 's#-cacao KNOWN#-cacao ERROR#' $(BUILD_JRE_ARCH_DIR)/jvm.cfg ; \
	fi
	rm -f stamps/add-cacao.stamp

stamps/add-cacao-debug.stamp: stamps/icedtea-debug.stamp stamps/cacao.stamp
if ADD_CACAO_BUILD
	mkdir -p $(BUILD_DEBUG_JRE_ARCH_DIR)/cacao
	install -m 644 cacao/install/lib/libjvm.so \
		$(BUILD_DEBUG_JRE_ARCH_DIR)/cacao/
	install -m 644 cacao/install/jre/lib/$(INSTALL_ARCH_DIR)/libjsig.so \
		$(BUILD_DEBUG_JRE_ARCH_DIR)/cacao/
	install -m 644 cacao/install/jre/lib/$(INSTALL_ARCH_DIR)/server/Xusage.txt \
		$(BUILD_DEBUG_JRE_ARCH_DIR)/cacao/
	printf -- '-cacao KNOWN\n' >> $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg
else
	printf -- '-cacao ERROR\n' >> $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg
endif
	touch $@

clean-add-cacao-debug:
	rm -rf $(BUILD_DEBUG_JRE_ARCH_DIR)/cacao
	if [ -e $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg ] ; then \
	  $(SED) -i 's#-cacao KNOWN#-cacao ERROR#' $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg ; \
	fi
	rm -f stamps/add-cacao-debug.stamp

# configure script arguments, quoted in single quotes
CONFIGURE_ARGS = @CONFIGURE_ARGS@
ADD_ZERO_CONFIGURE_ARGS = \
	--with-jdk-home=$(BUILD_SDK_DIR) \
	--disable-bootstrap --enable-zero
if ADD_SHARK_BUILD
ADD_ZERO_CONFIGURE_ARGS += \
	--enable-shark
endif
ADD_ZERO_CONFIGURE_ARGS += \
	--disable-docs \
	$(filter-out '--with-jdk-home=% '--with-ecj=% \
			'--with-java=% '--with-javah=% \
			'--with-javac=% '--with-javac=% \
			'--with-rmic=% '--with-additional-vms=% \
			'--disable-bootstrap% '--enable-bootstrap% , \
		$(CONFIGURE_ARGS)) \
	$(foreach i, openjdk hotspot corba jaxp jaxws jdk langtools, \
	  $(if $(findstring --with-$(i)-src-zip=, $(CONFIGURE_ARGS)),, --with-$(i)-src-zip=$(abs_top_builddir)/$(i).$(TAR_SUFFIX)))

ADD_ZERO_EXTRA_BUILD_ENV = \
	BUILD_LANGTOOLS=false ALT_LANGTOOLS_DIST=$(BUILD_OUTPUT_DIR)/langtools/dist \
	BUILD_JAXP=false      ALT_JAXP_DIST=$(BUILD_OUTPUT_DIR)/jaxp/dist \
	BUILD_JAXWS=false     ALT_JAXWS_DIST=$(BUILD_OUTPUT_DIR)/jaxws/dist \
	BUILD_CORBA=false     ALT_CORBA_DIST=$(BUILD_OUTPUT_DIR)/corba/dist \
	BUILD_JDK=false \
	DISTRIBUTION_PATCHES='$(foreach p,$(DISTRIBUTION_PATCHES),$(if $(findstring cacao,$(p)),,$(if $(findstring jamvm,$(p)),,$(p))))'

stamps/add-zero.stamp: stamps/icedtea.stamp
	mkdir -p stamps
if ADD_ZERO_BUILD
	mkdir -p zerovm

	cd zerovm && \
	    $(ADD_ZERO_EXTRA_BUILD_ENV) \
		$(ARCH_PREFIX) $(abs_top_srcdir)/configure $(ADD_ZERO_CONFIGURE_ARGS)

	$(ADD_ZERO_EXTRA_BUILD_ENV) \
		$(ARCH_PREFIX) $(MAKE) -C zerovm ICEDTEA_BUILD_TARGET=hotspot icedtea-stage2

if ZERO_BUILD
	mkdir -p $(BUILD_JRE_ARCH_DIR)/shark
	cp -a zerovm/$(BUILD_DIR)/hotspot/import/jre/lib/$(INSTALL_ARCH_DIR)/server/* \
		$(BUILD_JRE_ARCH_DIR)/shark/
	printf -- '-shark KNOWN\n' >> $(BUILD_JRE_ARCH_DIR)/jvm.cfg
	printf -- '-zero ERROR\n' >> $(BUILD_JRE_ARCH_DIR)/jvm.cfg
else
	mkdir -p $(BUILD_JRE_ARCH_DIR)/zero
	cp -a zerovm/$(BUILD_DIR)/hotspot/import/jre/lib/$(INSTALL_ARCH_DIR)/server/* \
		$(BUILD_JRE_ARCH_DIR)/zero/
	printf -- '-zero KNOWN\n' >> $(BUILD_JRE_ARCH_DIR)/jvm.cfg
	printf -- '-shark ERROR\n' >> $(BUILD_JRE_ARCH_DIR)/jvm.cfg
endif
else
	printf -- '-zero ERROR\n' >> $(BUILD_JRE_ARCH_DIR)/jvm.cfg
	printf -- '-shark ERROR\n' >> $(BUILD_JRE_ARCH_DIR)/jvm.cfg
endif
	touch $@

clean-add-zero:
	rm -rf $(BUILD_JRE_ARCH_DIR)/zero
	rm -rf $(BUILD_JRE_ARCH_DIR)/shark
	rm -rf zerovm
	if [ -e $(BUILD_JRE_ARCH_DIR)/jvm.cfg ] ; then \
	  $(SED) -i 's#-zero KNOWN#-zero ERROR#' $(BUILD_JRE_ARCH_DIR)/jvm.cfg ; \
	  $(SED) -i 's#-shark KNOWN#-shark ERROR#' $(BUILD_JRE_ARCH_DIR)/jvm.cfg ; \
	fi
	rm -f stamps/add-zero.stamp

stamps/add-zero-debug.stamp: stamps/icedtea-debug.stamp
	mkdir -p stamps
if ADD_ZERO_BUILD
	mkdir -p zerovm

	cd zerovm && \
	    $(ADD_ZERO_EXTRA_BUILD_ENV) \
		$(ARCH_PREFIX) $(abs_top_srcdir)/configure $(ADD_ZERO_CONFIGURE_ARGS)

	$(ADD_ZERO_EXTRA_BUILD_ENV) \
		$(ARCH_PREFIX) $(MAKE) -C zerovm ICEDTEA_DEBUG_BUILD_TARGET=hotspot icedtea-stage2

if ZERO_BUILD
	mkdir -p $(BUILD_DEBUG_JRE_ARCH_DIR)/shark
	cp -a zerovm/$(BUILD_OUTPUT_DIR)/hotspot/import/jre/lib/$(INSTALL_ARCH_DIR)/server/* \
		$(BUILD_DEBUG_JRE_ARCH_DIR)/shark/
	printf -- '-shark KNOWN\n' >> $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg
else
	mkdir -p $(BUILD_DEBUG_JRE_ARCH_DIR)/zero
	cp -a zerovm/$(BUILD_OUTPUT_DIR)/hotspot/import/jre/lib/$(INSTALL_ARCH_DIR)/server/* \
		$(BUILD_DEBUG_JRE_ARCH_DIR)/zero/
	printf -- '-zero KNOWN\n' >> $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg
endif
else
	printf -- '-zero ERROR\n' >> $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg
	printf -- '-shark ERROR\n' >> $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg
endif
	touch $@

clean-add-zero-debug:
	rm -rf $(BUILD_DEBUG_JRE_ARCH_DIR)/zero
	rm -rf $(BUILD_DEBUG_JRE_ARCH_DIR)/shark
	rm -rf zerovm
	if [ -e $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg ] ; then \
	  $(SED) -i 's#-zero KNOWN#-zero ERROR#' $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg ; \
	  $(SED) -i 's#-shark KNOWN#-shark ERROR#' $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg ; \
	fi
	rm -f stamps/add-zero-debug.stamp

# end additional VMs

# Test cases

stamps/jtreg.stamp: stamps/icedtea-stage2.stamp
	rm -rf test/jtreg/classes
	mkdir -p test/jtreg/classes
	$(BOOT_DIR)/bin/javac $(IT_JAVACFLAGS) -d test/jtreg/classes \
	   `find $(abs_top_srcdir)/test/jtreg/com -name '*.java'`
	(cd $(abs_top_srcdir)/test/jtreg; \
	 $(BOOT_DIR)/bin/jar cfm $(abs_top_builddir)/test/jtreg.jar \
	   META-INF/MANIFEST.MF \
	   legal  README JavaTest.cmdMgrs.lst JavaTest.toolMgrs.lst \
	   `find com -type f -a -not -name '*.java'` \
	   -C $(abs_top_builddir)/test/jtreg/classes com)
	mkdir -p stamps
	touch $@

clean-jtreg:
	rm -rf test/jtreg/classes
	if [ $(abs_top_srcdir) != $(abs_top_builddir) ] ; then \
	  if [ -e test/jtreg ] ; then \
	    rmdir test/jtreg ; \
	  fi ; \
	fi
	rm -f test/jtreg.jar
	rm -f stamps/jtreg.stamp

check-hotspot: stamps/jtreg.stamp
	mkdir -p test/hotspot/JTwork test/hotspot/JTreport
	$(BOOT_DIR)/bin/java -jar test/jtreg.jar -v1 -a -ignore:quiet \
		-w:test/hotspot/JTwork -r:test/hotspot/JTreport \
		-jdk:$(BUILD_SDK_DIR) \
		$(ICEDTEA_JTREG_OPTIONS) \
		`pwd`/openjdk/hotspot/test \
	    | tee test/$@.log

check-langtools: stamps/jtreg.stamp
	mkdir -p test/langtools/JTwork test/langtools/JTreport
	$(BOOT_DIR)/bin/java -jar test/jtreg.jar -v1 -a -ignore:quiet \
		-w:test/langtools/JTwork -r:test/langtools/JTreport \
		$${ICEDTEA_JTREG_OTHERVM:--samevm} \
		-jdk:$(BUILD_SDK_DIR) \
		$(ICEDTEA_JTREG_OPTIONS) \
		-exclude:$(abs_top_srcdir)/test/jtreg/excludelist.langtools.jtx \
		`pwd`/openjdk/langtools/test \
	    | tee test/$@.log

check-jdk: stamps/jtreg.stamp
	mkdir -p test/jdk/JTwork test/jdk/JTreport
if ENABLE_CACAO
	CACAO_EXCLUDE="-exclude:$(abs_top_srcdir)/test/jtreg/excludelist.jdk.cacao.jtx"
endif
	if test "x$${DISPLAY}" = "x"; then XVFB_RUN="@XVFB_RUN_CMD@"; fi; \
	if test "x$${DISPLAY}" = "x" && test "x$${XVFB_RUN}" = "x"; then \
		echo "WARNING: DISPLAY not set and xvfb-run not found"; \
	fi; \
	$${XVFB_RUN} \
	$(BOOT_DIR)/bin/java -jar test/jtreg.jar -v1 -a -ignore:quiet \
		-w:test/jdk/JTwork -r:test/jdk/JTreport \
		$${ICEDTEA_JTREG_OTHERVM:--samevm} \
		-jdk:$(BUILD_SDK_DIR) \
		$(ICEDTEA_JTREG_OPTIONS) $(CACAO_EXCLUDE) \
		-exclude:$(abs_top_srcdir)/test/jtreg/excludelist.jdk.jtx \
		`pwd`/openjdk/jdk/test \
	    | tee test/$@.log

clean-jtreg-reports:
	rm -rf test/hotspot test/langtools test/jdk
	rm -f test/check-*.log test/jtreg-summary.log

# FIXME: this might need some adjustment for other OS than Linux
jtreg_processes = ps x -ww -o pid,ppid,args \
	| awk '$$2 == 1 && $$3 ~ /^$(subst /,\/,/scratch)/' \
	| $(SED) 's,$(CURDIR)/$(sdkimg),<sdkimg>,g;s,$(CURDIR),<pwd>,g'
jtreg_pids = ps x --no-headers -ww -o pid,ppid,args \
	| awk '$$2 == 1 && $$3 ~ /^$(subst /,\/,$(CURDIR)/$(sdkimg))/ {print $$1}'

jtregcheck: $(TESTS_TO_RUN)
	$(MKDIR_P) test
	for i in $(TEST_SUITES); do \
	  echo "--------------- jtreg console summary for $$i ---------------"; \
	  egrep -v '^(Passed:|Directory|Re[a-z]+\ written\ to)' test/check-$$i.log; \
	done | tee test/jtreg-summary.log

	: # kill testsuite processes still hanging
	@pids=$$($(jtreg_pids)); \
	if [ -n "$$pids" ]; then \
	  echo "killing processes..."; \
	  $(jtreg_processes); \
	  kill -1 $$pids; \
	  sleep 2; \
	  pids=$$($(jtreg_pids)); \
	  if [ -n "$$pids" ]; then \
	    echo "try harder..."; \
	    $(jtreg_processes); \
	    kill -9 $$pids; \
	    sleep 2; \
	  fi; \
	else \
	  echo "nothing to cleanup"; \
	fi; \
	pids=$$($(jtreg_pids)); \
	if [ -n "$$pids" ]; then \
	  echo "leftover processes..."; \
	  $(jtreg_processes); \
	fi

check-tapset-probes: 
if ENABLE_SYSTEMTAP
	if test "x${PERL}" = "x"; then \
	  echo "ERROR: Perl not found"; exit -1; \
	fi
	if test "x${STAP}" = "x"; then \
	  echo "ERROR: stap not found"; exit -1; \
	fi
	$(MKDIR_P) test
	${PERL} $(abs_top_builddir)/test/tapset/jstaptest.pl \
	  -B $(BUILD_OUTPUT_DIR) -A $(BUILD_ARCH_DIR) \
	  -S $(abs_top_srcdir)/test/tapset \
	  -a test/check-stap.log -p
endif
check-tapset-jstack: 
if ENABLE_SYSTEMTAP
	if test "x${PERL}" = "x"; then \
	  echo "ERROR: Perl not found"; exit -1; \
	fi
	if test "x${STAP}" = "x"; then \
	  echo "ERROR: stap not found"; exit -1; \
	fi
	$(MKDIR_P) test
	${PERL} $(abs_top_builddir)/test/tapset/jstaptest.pl \
	  -B $(BUILD_OUTPUT_DIR) -A $(BUILD_ARCH_DIR) \
	  -S $(abs_top_srcdir)/test/tapset \
	  -a test/check-stap.log -j
endif
check-tapset: check-tapset-probes check-tapset-jstack

clean-tapset-report:
if ENABLE_SYSTEMTAP
	rm -f test/check-stap.log
endif

stamps/check-mimetype.stamp: stamps/mimetypecheck.stamp stamps/icedtea.stamp
	if [ -e $(BUILD_SDK_DIR)/bin/java ] ; then \
	  echo "<html><head/><body/></html>" > $(MIME_TYPE_CHECK_BUILD_DIR)/test.html ; \
	  expected="text/html" ; \
	  html=$$($(BUILD_SDK_DIR)/bin/java -cp $(MIME_TYPE_CHECK_BUILD_DIR) RH1195203 \
	    $(MIME_TYPE_CHECK_BUILD_DIR)/test.html) ; \
	  if test "x$${html}" != "x$${expected}" ; then \
	    echo "MIME type test failed; expected $${expected}, got $${html}" ; \
	    exit -1; \
	  fi ; \
	fi
if JAVA_SOURCE_SUPPORTED
	if [ -e $(BUILD_SDK_DIR)/bin/java ] ; then \
	  expected="text/x-java-source" ; \
	  java=$$($(BUILD_SDK_DIR)/bin/java -cp $(MIME_TYPE_CHECK_BUILD_DIR) RH1195203 \
	    $(MIME_TYPE_CHECK_SRCS)) ; \
	  if test "x$${java}" != "x$${expected}"; then \
	    echo "MIME type test failed; expected $${expected}, got $${java}" ; \
	    exit -1; \
	  fi ; \
	fi
endif
	mkdir -p stamps
	touch $@

clean-check-mimetype:
	rm -f $(MIME_TYPE_CHECK_BUILD_DIR)/test.html
	rm -f stamps/check-mimetype.stamp

# Support classes for non-OpenJDK bootstraps

rt-source-files.txt: $(OPENJDK_BOOT_TREE)
	rm -f $@
	for files in $(ICEDTEA_BOOTSTRAP_CLASSES) ; \
	do \
	  echo $$files >> $@ ; \
	done
	touch $@

stamps/rt-class-files.stamp: $(INITIAL_BOOTSTRAP_LINK_STAMP) rt-source-files.txt
	mkdir -p lib/rt
	if echo $(ICEDTEA_BOOTSTRAP_CLASSES) | grep '\S' &> /dev/null ; then \
	  $(BOOT_DIR)/bin/javac $(IT_JAVACFLAGS) -d lib/rt \
	    -classpath $(SYSTEM_JDK_DIR)/jre/lib/rt.jar \
            -sourcepath '$(SOURCEPATH_DIRS)' \
            -bootclasspath \'\' @rt-source-files.txt ; \
	fi
	mkdir -p stamps
	touch $@

stamps/rt.stamp: stamps/rt-class-files.stamp
	if echo $(ICEDTEA_BOOTSTRAP_CLASSES) | grep '\S' &> /dev/null ; then \
	  (set -e ; \
	   cd lib/rt ; \
	   for dirs in $(ICEDTEA_BOOTSTRAP_RESOURCES) ; \
	     do \
	     destpath=`echo $$dirs|$(SED) -e 's#$(LANGTOOLS)/##' -e 's#$(SHARE)/##'` ; \
	     mkdir -p `dirname $$destpath` ; \
	     cp -a ../../$$dirs $$destpath ; \
	   done ; \
	   if [ -w $(STAGE1_BOOT_RUNTIME) ] ; then \
	     $(ZIP) -qr $(STAGE1_BOOT_RUNTIME) com java javax sun ; \
	   fi ; ) \
	fi
	mkdir -p stamps
	touch $@

clean-rt:
	rm -rf lib/rt
	rm -f stamps/rt-class-files.stamp stamps/rt.stamp
	rm -f rt-source-files.txt
	if [ -e $(STAGE1_BOOT_DIR)/jre/lib ] ; then \
	  cp $(SYSTEM_JDK_DIR)/jre/lib/rt.jar $(STAGE1_BOOT_RUNTIME) ; \
	  chmod u+w $(STAGE1_BOOT_RUNTIME) ; \
	fi

# Crypto Level Check

stamps/cryptocheck.stamp: $(INITIAL_BOOTSTRAP_LINK_STAMP)
	mkdir -p $(CRYPTO_CHECK_BUILD_DIR)
	$(BOOT_DIR)/bin/javac $(IT_JAVACFLAGS) \
	  -d $(CRYPTO_CHECK_BUILD_DIR) $(CRYPTO_CHECK_SRCS)
	mkdir -p stamps
	touch $@

clean-cryptocheck:
	rm -rf $(CRYPTO_CHECK_BUILD_DIR)
	rm -f stamps/cryptocheck.stamp

# Mime Type Check

stamps/mimetypecheck.stamp: $(INITIAL_BOOTSTRAP_LINK_STAMP)
	mkdir -p $(MIME_TYPE_CHECK_BUILD_DIR)
	$(BOOT_DIR)/bin/javac $(IT_JAVACFLAGS) \
	  -d $(MIME_TYPE_CHECK_BUILD_DIR) $(MIME_TYPE_CHECK_SRCS)
	mkdir -p stamps
	touch $@

clean-mimetypecheck:
	rm -rf $(MIME_TYPE_CHECK_BUILD_DIR)
	rm -f stamps/mimetypecheck.stamp

# Check class files, both from in-tree and generated sources, contain debug info
stamps/check-java-debug.stamp: stamps/icedtea.stamp
if ENABLE_JAVA_DEBUGINFO
	@echo "Checking that Java classes include debug info"
	if [ -e $(BUILD_SDK_DIR)/bin/javap ] ; then \
	  $(BUILD_SDK_DIR)/bin/javap -l java.lang.Object | grep "Compiled from" ; \
	  $(BUILD_SDK_DIR)/bin/javap -l java.lang.Object | grep LineNumberTable ; \
	  $(BUILD_SDK_DIR)/bin/javap -l java.lang.Object | grep LocalVariableTable ; \
	  $(BUILD_SDK_DIR)/bin/javap -l java.nio.ByteBuffer | grep "Compiled from" ; \
	  $(BUILD_SDK_DIR)/bin/javap -l java.nio.ByteBuffer | grep LineNumberTable ; \
	  $(BUILD_SDK_DIR)/bin/javap -l java.nio.ByteBuffer | grep LocalVariableTable ; \
	fi
endif
	mkdir -p stamps
	touch $@

clean-check-java-debug:
	rm -f stamps/check-java-debug.stamp

# Check source files are included in the source zip
stamps/check-java-src.stamp: stamps/icedtea.stamp
	@echo "Checking for Java sources in src.zip"
	$(UNZIP) -l $(BUILD_SDK_DIR)/src.zip | grep 'sun.misc.Unsafe'
	mkdir -p stamps
	touch $@

clean-check-java-src:
	rm -f stamps/check-java-src.stamp

# File substitution

policytool-@JAVA_VER@.desktop: policytool.desktop
	cat $< | $(SED) "s#_JREBINDIR_#$(prefix)/jre/bin#" > $@

clean-policytool-@JAVA_VER@.desktop:
	rm -f policytool-@JAVA_VER@.desktop

jconsole-@JAVA_VER@.desktop: jconsole.desktop
	cat $< | $(SED) "s#_SDKBINDIR_#$(bindir)#" > $@

clean-jconsole-@JAVA_VER@.desktop:
	rm -f jconsole-@JAVA_VER@.desktop

tz.properties: $(top_srcdir)/tz.properties.in
	cat $< | $(SED) "s#_TZDATA_DIR_#$(TZDATA_DIR)#" > $@

clean-tz.properties:
	rm -f tz.properties

# ECC Availability Check

stamps/ecccheck.stamp: $(INITIAL_BOOTSTRAP_LINK_STAMP)
if ENABLE_SUNEC
	mkdir -p $(ECC_CHECK_BUILD_DIR)
	$(BOOT_DIR)/bin/javac $(IT_JAVACFLAGS) \
	  -d $(ECC_CHECK_BUILD_DIR) $(ECC_CHECK_SRCS)
endif
	mkdir -p stamps
	touch $@

clean-ecccheck:
if ENABLE_SUNEC
	rm -rf $(ECC_CHECK_BUILD_DIR)
endif
	rm -f stamps/ecccheck.stamp

stamps/check-ecc.stamp: stamps/ecccheck.stamp stamps/icedtea.stamp
if ENABLE_SUNEC
	set -e ; \
	if [ -e $(BUILD_SDK_DIR)/bin/java ] ; then \
	  $(BUILD_SDK_DIR)/bin/java -cp $(ECC_CHECK_BUILD_DIR) TestEllipticCurveCryptoSupport yes ; \
	  $(BUILD_SDK_DIR)/bin/java -cp $(ECC_CHECK_BUILD_DIR) TestECDSA ; \
	fi
endif
	mkdir -p stamps
	touch $@

clean-check-ecc:
	rm -f stamps/check-ecc.stamp

# Installation Targets
# ====================

# install doesn't work with symlinks
install_file = if [ -h $(1) ] ; then cp -a $(1) $(2) ; else $(3) $(1) $(2) ; fi

install-exec-local:
	${mkinstalldirs} $(DESTDIR)$(bindir) $(DESTDIR)$(includedir) $(DESTDIR)${exec_prefix}/lib
	${mkinstalldirs} $(DESTDIR)$(includedir)/$(BUILD_OS_DIR) $(DESTDIR)${exec_prefix}/lib/$(INSTALL_ARCH_DIR)
	${mkinstalldirs} $(DESTDIR)${exec_prefix}/lib/$(INSTALL_ARCH_DIR)/jli
	${mkinstalldirs} $(DESTDIR)${prefix}/jre/bin $(DESTDIR)${prefix}/jre/lib
	${mkinstalldirs} $(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/{.,headless,jli,xawt}
	${mkinstalldirs} $(DESTDIR)${prefix}/jre/lib/ext
	${mkinstalldirs} $(DESTDIR)${prefix}/tapset
if SPLIT_DEBUGINFO
	${mkinstalldirs} $(DESTDIR)$(DEBUG_PREFIX)/$(bindir) $(DESTDIR)$(DEBUG_PREFIX)/${exec_prefix}/lib
	${mkinstalldirs} $(DESTDIR)$(DEBUG_PREFIX)/${exec_prefix}/lib/$(INSTALL_ARCH_DIR)
	${mkinstalldirs} $(DESTDIR)$(DEBUG_PREFIX)/${exec_prefix}/lib/$(INSTALL_ARCH_DIR)/jli
	${mkinstalldirs} $(DESTDIR)$(DEBUG_PREFIX)/${prefix}/jre/bin $(DESTDIR)${prefix}/jre/lib
	${mkinstalldirs} $(DESTDIR)$(DEBUG_PREFIX)/${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/{.,headless,jli,xawt}
endif
	for files in $(BUILD_SDK_DIR)/bin/*; do \
	  name=$$(basename $${files}) ; \
	  if test "x$(enable_split_debuginfo)" = "xyes" -a ! -h $${files} -a "x$${name}" != "xjava-rmi.cgi" ; then \
	    dir=$$(echo $$(dirname $${files}) | $(SED) "s#$(BUILD_SDK_DIR)#$(BUILD_DEBUGINFO_DIR)#") ; \
	    $(INSTALL_PROGRAM) $${dir}/$${name}.stripped $(DESTDIR)$(bindir)/$${name}; \
	    $(INSTALL_PROGRAM) $${dir}/$${name}.debug $(DESTDIR)$(DEBUG_PREFIX)/$(bindir); \
	  else \
	    $(call install_file,$${files},$(DESTDIR)$(bindir),$(INSTALL_PROGRAM)) ; \
	  fi ; \
	done
	for files in $(BUILD_SDK_DIR)/include/*.h; do \
	  $(call install_file,$${files},$(DESTDIR)$(includedir),$(INSTALL_DATA)) ; \
	done
	for files in $(BUILD_SDK_DIR)/include/$(BUILD_OS_DIR)/*.h; do \
	  $(call install_file,$${files},$(DESTDIR)$(includedir)/$(BUILD_OS_DIR),$(INSTALL_DATA)) ; \
	done
	for files in $(BUILD_SDK_DIR)/lib/*.jar; do \
	  $(call install_file,$${files},$(DESTDIR)${exec_prefix}/lib,$(INSTALL_DATA)) ; \
	done
	for files in $(BUILD_SDK_DIR)/lib/*.idl; do \
	  $(call install_file,$${files},$(DESTDIR)${exec_prefix}/lib,$(INSTALL_DATA)) ; \
	done
	if test "x$(enable_split_debuginfo)" = "xyes"; then \
	  $(INSTALL_PROGRAM) $(BUILD_DEBUGINFO_DIR)/lib/jexec.stripped $(DESTDIR)${exec_prefix}/lib/jexec ; \
	  $(INSTALL_PROGRAM) $(BUILD_DEBUGINFO_DIR)/lib/jexec.debug $(DESTDIR)$(DEBUG_PREFIX)/${exec_prefix}/lib ; \
	else \
	  $(INSTALL_PROGRAM) $(BUILD_SDK_DIR)/lib/jexec $(DESTDIR)${exec_prefix}/lib ; \
	fi
	for files in $(BUILD_SDK_DIR)/lib/$(INSTALL_ARCH_DIR)/jli/*.so; do \
	  if test "x$(enable_split_debuginfo)" = "xyes" -a ! -h $${files}; then \
	    name=$$(basename $${files}) ; \
	    dir=$$(echo $$(dirname $${files}) | $(SED) "s#$(BUILD_SDK_DIR)#$(BUILD_DEBUGINFO_DIR)#") ; \
	    $(INSTALL_PROGRAM) $${dir}/$${name}.stripped $(DESTDIR)${exec_prefix}/lib/$(INSTALL_ARCH_DIR)/jli/$${name}; \
	    $(INSTALL_PROGRAM) $${dir}/$${name}.debug $(DESTDIR)$(DEBUG_PREFIX)/${exec_prefix}/lib/$(INSTALL_ARCH_DIR)/jli; \
	  else \
	    $(call install_file,$${files},$(DESTDIR)${exec_prefix}/lib/$(INSTALL_ARCH_DIR)/jli,$(INSTALL_PROGRAM)) ; \
	  fi ; \
	done
	for files in $(BUILD_SDK_DIR)/jre/bin/*; do \
	  if test "x$(enable_split_debuginfo)" = "xyes" -a ! -h $${files}; then \
	    name=$$(basename $${files}) ; \
	    dir=$$(echo $$(dirname $${files}) | $(SED) "s#$(BUILD_SDK_DIR)#$(BUILD_DEBUGINFO_DIR)#") ; \
	    $(INSTALL_PROGRAM) $${dir}/$${name}.stripped $(DESTDIR)${prefix}/jre/bin/$${name}; \
	    $(INSTALL_PROGRAM) $${dir}/$${name}.debug $(DESTDIR)$(DEBUG_PREFIX)/${prefix}/jre/bin; \
	  else \
	    $(call install_file,$${files},$(DESTDIR)${prefix}/jre/bin,$(INSTALL_PROGRAM)) ; \
	  fi ; \
	done
	for files in $(BUILD_SDK_DIR)/jre/lib/*.jar; do \
	  $(call install_file,$${files},$(DESTDIR)${prefix}/jre/lib,$(INSTALL_DATA)) ; \
	done
	if test "x$(enable_split_debuginfo)" = "xyes"; then \
	  $(INSTALL_PROGRAM) $(BUILD_DEBUGINFO_DIR)/jre/lib/jexec.stripped $(DESTDIR)${exec_prefix}/jre/lib/jexec ; \
	  $(INSTALL_PROGRAM) $(BUILD_DEBUGINFO_DIR)/jre/lib/jexec.debug $(DESTDIR)$(DEBUG_PREFIX)/${exec_prefix}/jre/lib ; \
	else \
	  $(INSTALL_PROGRAM) $(BUILD_SDK_DIR)/jre/lib/jexec $(DESTDIR)${exec_prefix}/jre/lib ; \
	fi
	for subdirs in . headless jli xawt ; do \
	  for files in $(BUILD_SDK_DIR)/jre/lib/$(INSTALL_ARCH_DIR)/$${subdirs}/*.so; do \
	    if test "x$(enable_split_debuginfo)" = "xyes" -a ! -h $${files}; then \
	      name=$$(basename $${files}) ; \
	      dir=$$(echo $$(dirname $${files}) | $(SED) "s#$(BUILD_SDK_DIR)#$(BUILD_DEBUGINFO_DIR)#") ; \
	      $(INSTALL_PROGRAM) $${dir}/$${name}.stripped \
	        $(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/$${subdirs}/$${name}; \
	      $(INSTALL_PROGRAM) $${dir}/$${name}.debug \
	        $(DESTDIR)$(DEBUG_PREFIX)/${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/$${subdirs}; \
	    else \
	      $(call install_file,$${files},$(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/$${subdirs},$(INSTALL_PROGRAM));\
	    fi ; \
	  done ; \
	done
	for vms in client server ; do \
	  if [ -d $(BUILD_SDK_DIR)/jre/lib/$(INSTALL_ARCH_DIR)/$${vms} ] ; then \
	    ${mkinstalldirs} $(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/$${vms} ; \
	    ${mkinstalldirs} $(DESTDIR)$(DEBUG_PREFIX)/${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/$${vms} ; \
	    for files in $(BUILD_SDK_DIR)/jre/lib/$(INSTALL_ARCH_DIR)/$${vms}/*.so; do \
	      if test "x$(enable_split_debuginfo)" = "xyes" -a ! -h $${files} ; then \
	        name=$$(basename $${files}) ; \
	        dir=$$(echo $$(dirname $${files}) | $(SED) "s#$(BUILD_SDK_DIR)#$(BUILD_DEBUGINFO_DIR)#") ; \
	        $(INSTALL_PROGRAM) $${dir}/$${name}.stripped \
	          $(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/$${vms}/$${name}; \
	        $(INSTALL_PROGRAM) $${dir}/$${name}.debug \
	          $(DESTDIR)$(DEBUG_PREFIX)/${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/$${vms}; \
	      else \
	        $(call install_file,$${files},$(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/$${vms},$(INSTALL_PROGRAM));\
	      fi ; \
	    done ; \
	  fi ; \
	done
	for files in $(BUILD_SDK_DIR)/jre/lib/ext/*.jar; do \
	  $(call install_file,$${files},$(DESTDIR)${prefix}/jre/lib/ext,$(INSTALL_DATA));\
	done
if ENABLE_SYSTEMTAP
if WITH_TAPSET_DIR
	stapdest="$(DESTDIR)$(TAPSET_DIR)/$(SYSTEMTAP_ARCH_DIR)" ; \
	for files in $(BUILD_SDK_DIR)/tapset/*.stp; do \
	  $(call install_file,$${files},$(DESTDIR)${prefix}/tapset,$(INSTALL_DATA));\
	  ${mkinstalldirs} $${stapdest} ; \
	  tapname=$$(basename $${files}); \
	  destname=$${tapname/./-@JAVA_VER@.} ; \
	  ln -s ${prefix}/tapset/$${tapname} $${stapdest}/$${destname} ; \
	done
endif
endif
	$(abs_top_builddir)/pax-mark-vm $(DESTDIR)${prefix} false

install-data-local:
	${mkinstalldirs} $(DESTDIR)${prefix}
	${mkinstalldirs} $(DESTDIR)${prefix}/jre/.systemPrefs
	${mkinstalldirs} $(DESTDIR)${prefix}/jre/lib
	${mkinstalldirs} $(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)
	${mkinstalldirs} $(DESTDIR)${prefix}/jre/lib/{cmm,ext,images/cursors,management,security,zi}
	${mkinstalldirs} $(DESTDIR)$(mandir)/{man1,ja_JP.UTF-8/man1}
	${mkinstalldirs} $(DESTDIR)${prefix}/demo
	${mkinstalldirs} $(DESTDIR)${prefix}/sample
	${mkinstalldirs} $(DESTDIR)$(docdir)
	${mkinstalldirs} $(DESTDIR)$(datadir)/applications
	${mkinstalldirs} $(DESTDIR)$(sysconfdir)/.java/.systemPrefs
	for files in $(BUILD_SDK_DIR)/jre/lib/*.properties; do \
	  $(call install_file,$${files},$(DESTDIR)${prefix}/jre/lib,$(INSTALL_DATA)) ; \
	done
	for files in $(BUILD_SDK_DIR)/jre/lib/fontconfig*.{properties.src,bfc}; do \
	  $(call install_file,$${files},$(DESTDIR)${prefix}/jre/lib,$(INSTALL_DATA)) ; \
	done
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/release $(DESTDIR)${prefix}
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/src.zip $(DESTDIR)${prefix}
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/jre/lib/classlist $(DESTDIR)${prefix}/jre/lib
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/jre/lib/currency.data $(DESTDIR)${prefix}/jre/lib
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/jre/lib/jvm.hprof.txt $(DESTDIR)${prefix}/jre/lib
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/jre/lib/meta-index $(DESTDIR)${prefix}/jre/lib
if MIME_TYPES_FILE_FOUND
	$(call install_file,$(BUILD_SDK_DIR)/jre/lib/mime.types,$(DESTDIR)${prefix}/jre/lib,$(INSTALL_DATA))
endif
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/jre/lib/psfont.properties.ja $(DESTDIR)${prefix}/jre/lib
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/jre/lib/$(INSTALL_ARCH_DIR)/jvm.cfg \
	  $(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)
	for vms in client server ; do \
	  if [ -d $(BUILD_SDK_DIR)/jre/lib/$(INSTALL_ARCH_DIR)/$${vms} ] ; then \
	    ${mkinstalldirs} $(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/$${vms} ; \
	    $(INSTALL_DATA) $(BUILD_SDK_DIR)/jre/lib/$(INSTALL_ARCH_DIR)/$${vms}/Xusage.txt \
	      $(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/$${vms} ; \
	  fi ; \
	done
	for files in $(BUILD_SDK_DIR)/jre/lib/cmm/*.pf; do \
	  $(call install_file,$${files},$(DESTDIR)${prefix}/jre/lib/cmm,$(INSTALL_DATA)) ; \
	done
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/jre/lib/ext/meta-index $(DESTDIR)${prefix}/jre/lib/ext
	for files in $(BUILD_SDK_DIR)/jre/lib/images/cursors/*.gif; do \
	  $(call install_file,$${files},$(DESTDIR)${prefix}/jre/lib/images/cursors,$(INSTALL_DATA)) ; \
	done
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/jre/lib/images/cursors/cursors.properties \
	  $(DESTDIR)${prefix}/jre/lib/images/cursors
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/jre/lib/management/jmxremote.access \
	  $(DESTDIR)${prefix}/jre/lib/management
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/jre/lib/management/management.properties \
	  $(DESTDIR)${prefix}/jre/lib/management
	for files in $(BUILD_SDK_DIR)/jre/lib/management/*.template; do \
	  $(call install_file,$${files},$(DESTDIR)${prefix}/jre/lib/management,$(INSTALL_DATA)) ; \
	done
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/jre/lib/security/cacerts \
	  $(DESTDIR)${prefix}/jre/lib/security
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/jre/lib/security/java.policy \
	  $(DESTDIR)${prefix}/jre/lib/security
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/jre/lib/security/java.security \
	  $(DESTDIR)${prefix}/jre/lib/security
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/jre/lib/security/nss.cfg \
	  $(DESTDIR)${prefix}/jre/lib/security
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/jre/lib/security/blacklisted.certs \
	  $(DESTDIR)${prefix}/jre/lib/security
	for policies in limited unlimited; do \
	  ${mkinstalldirs} $(DESTDIR)${prefix}/jre/lib/security/policy/$${policies} ; \
	  for files in $(BUILD_SDK_DIR)/jre/lib/security/policy/$${policies}/*.jar; do \
	    $(call install_file,$${files},$(DESTDIR)${prefix}/jre/lib/security/policy/$${policies},$(INSTALL_DATA)); \
	  done ; \
	done
	for items in $(BUILD_SDK_DIR)/jre/lib/zi/*; do \
	  cp -RP $${items} $(DESTDIR)${prefix}/jre/lib/zi ; \
	done
	for files in $(BUILD_SDK_DIR)/man/man1/*.1; do \
	  $(call install_file,$${files},$(DESTDIR)$(mandir)/man1,$(INSTALL_DATA)) ; \
	done
	for files in $(BUILD_SDK_DIR)/man/ja_JP.UTF-8/man1/*.1; do \
	  $(call install_file,$${files},$(DESTDIR)$(mandir)/ja_JP.UTF-8/man1,$(INSTALL_DATA)) ; \
	done
	cp -a $(BUILD_SDK_DIR)/man/ja $(DESTDIR)$(mandir)
	for items in $(BUILD_SDK_DIR)/demo/*; do \
	  cp -RP $${items} $(DESTDIR)${prefix}/demo ; \
	done
	for items in $(BUILD_SDK_DIR)/sample/*; do \
	  cp -RP $${items} $(DESTDIR)${prefix}/sample ; \
	done
	${INSTALL_DATA} $(srcdir)/README $(DESTDIR)$(docdir)
	${INSTALL_DATA} $(srcdir)/NEWS $(DESTDIR)$(docdir)
	${INSTALL_DATA} $(srcdir)/AUTHORS $(DESTDIR)$(docdir)
	${INSTALL_DATA} $(srcdir)/HACKING $(DESTDIR)$(docdir)
	${INSTALL_DATA} $(srcdir)/COPYING $(DESTDIR)$(docdir)
	${INSTALL_DATA} $(srcdir)/ChangeLog $(DESTDIR)$(docdir)
	${INSTALL_DATA} $(BUILD_SDK_DIR)/ASSEMBLY_EXCEPTION $(DESTDIR)$(docdir)
	${INSTALL_DATA} $(BUILD_SDK_DIR)/LICENSE $(DESTDIR)$(docdir)
	${INSTALL_DATA} $(BUILD_SDK_DIR)/THIRD_PARTY_README $(DESTDIR)$(docdir)
	${INSTALL_DATA} openjdk/jaxp/TRADEMARK $(DESTDIR)$(docdir)
	for sizes in 16 24 32 48 ; do \
	  ${mkinstalldirs} $(DESTDIR)$(datadir)/icons/hicolor/$${sizes}x$${sizes}/apps ; \
	  ${INSTALL_DATA} openjdk/jdk/src/solaris/classes/sun/awt/X11/java-icon$${sizes}.png \
	    $(DESTDIR)$(datadir)/icons/hicolor/$${sizes}x$${sizes}/apps/java-@JAVA_VER@-@JAVA_VENDOR@.png ; \
	done
	for files in $(DESKTOP_FILES) ; do \
	  ${INSTALL_DATA} $(builddir)/$${files} $(DESTDIR)$(datadir)/applications ; \
	done
	touch $(DESTDIR)/${prefix}/jre/.systemPrefs/.system.lock
	touch $(DESTDIR)/${prefix}/jre/.systemPrefs/.systemRootModFile
if ENABLE_DOCS
	${mkinstalldirs} $(DESTDIR)$(htmldir)
	cp -RP $(BUILD_OUTPUT_DIR)/docs/* $(DESTDIR)$(htmldir)
endif
	for vms in client server ; do \
	  if [ -f $(BUILD_SDK_DIR)/jre/lib/$(INSTALL_ARCH_DIR)/$${vms}/classes.jsa ] ; then \
	    ${mkinstalldirs} $(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/$${vms} ; \
	    $(INSTALL_DATA) $(BUILD_SDK_DIR)/jre/lib/$(INSTALL_ARCH_DIR)/$${vms}/classes.jsa \
	      $(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/$${vms} ; \
	  fi ; \
	done
# Target Aliases
# ===============

add-archive: stamps/add-archive.stamp

add-archive-boot: stamps/add-archive-ecj.stamp

add-archive-debug: stamps/add-archive-debug.stamp

add-cacao: stamps/add-cacao.stamp

add-cacao-debug: stamps/add-cacao-debug.stamp

add-jamvm: stamps/add-jamvm.stamp

add-jamvm-debug: stamps/add-jamvm-debug.stamp

add-mime-types-file: stamps/add-mime-types-file.stamp

add-mime-types-file-debug: stamps/add-mime-types-file-debug.stamp

add-mime-types-file-boot: stamps/add-mime-types-file-boot.stamp

add-nss: stamps/add-nss.stamp

add-nss-debug: stamps/add-nss-debug.stamp

add-systemtap: stamps/add-systemtap.stamp

add-systemtap-boot: stamps/add-systemtap-boot.stamp

add-systemtap-debug: stamps/add-systemtap-debug.stamp

add-tzdata-support: stamps/add-tzdata-support.stamp

add-tzdata-support-boot: stamps/add-tzdata-support-boot.stamp

add-tzdata-support-debug: stamps/add-tzdata-support-debug.stamp

add-zero: stamps/add-zero.stamp

add-zero-debug: stamps/add-zero-debug.stamp

bootstrap-directory-stage1: stamps/bootstrap-directory-stage1.stamp

bootstrap-directory-stage2: stamps/bootstrap-directory-stage2.stamp

bootstrap-directory-symlink-stage1: stamps/bootstrap-directory-symlink-stage1.stamp

bootstrap-directory-symlink-stage2: stamps/bootstrap-directory-symlink-stage2.stamp

cacao: stamps/cacao.stamp

check-crypto: stamps/check-crypto.stamp

check-crypto-boot: stamps/check-crypto-boot.stamp

check-crypto-debug: stamps/check-crypto-debug.stamp

check-mimetype: stamps/check-mimetype.stamp

check-ecc: stamps/check-ecc.stamp

check-java-src: stamps/check-java-src.stamp

check-java-debug: stamps/check-java-debug.stamp

clone-boot: stamps/clone-boot.stamp

cryptocheck: stamps/cryptocheck.stamp

download: stamps/download.stamp

download-cacao: stamps/download-cacao.stamp

download-corba: stamps/download-corba.stamp

download-hotspot: stamps/download-hotspot.stamp

download-jamvm: stamps/download-jamvm.stamp

download-jaxp: stamps/download-jaxp.stamp

download-jaxws: stamps/download-jaxws.stamp

download-jdk: stamps/download-jdk.stamp

download-langtools: stamps/download-langtools.stamp

download-openjdk: stamps/download-openjdk.stamp

ecccheck: stamps/ecccheck.stamp

extract: stamps/extract.stamp

extract-cacao: stamps/extract-cacao.stamp

extract-corba: stamps/extract-corba.stamp

extract-openjdk: stamps/extract-openjdk.stamp

extract-hotspot: stamps/extract-hotspot.stamp

extract-jamvm: stamps/extract-jamvm.stamp

extract-jaxp: stamps/extract-jaxp.stamp

extract-jaxws: stamps/extract-jaxws.stamp

extract-jdk: stamps/extract-jdk.stamp

extract-langtools: stamps/extract-langtools.stamp

fonts: stamps/fonts.stamp

generated: stamps/generated.stamp

icedtea: stamps/icedtea.stamp

icedtea-boot: stamps/icedtea-boot.stamp

icedtea-debug: stamps/icedtea-debug.stamp

icedtea-debug-stage2: stamps/icedtea-debug-stage2.stamp

icedtea-stage1: stamps/icedtea-stage1.stamp

icedtea-stage2: stamps/icedtea-stage2.stamp

jamvm: stamps/jamvm.stamp

jtreg: stamps/jtreg.stamp

mimetypecheck: stamps/mimetypecheck.stamp

native-ecj: stamps/native-ecj.stamp

overlay: stamps/overlay.stamp

patch: stamps/patch.stamp

patch-boot: stamps/patch-boot.stamp

patch-fsg: stamps/patch-fsg.stamp

remove-intree-libraries: stamps/remove-intree-libraries.stamp

rewriter: stamps/rewriter.stamp

rewrite-rhino: stamps/rewrite-rhino.stamp

rt: stamps/rt.stamp

rt-class-files: stamps/rt-class-files.stamp

sanitise-openjdk: stamps/sanitise-openjdk.stamp

split-debuginfo: stamps/split-debuginfo.stamp

split-debuginfo-boot: stamps/split-debuginfo-boot.stamp

split-debuginfo-debug: stamps/split-debuginfo-debug.stamp

versioning: stamps/versioning.stamp
