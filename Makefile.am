# Dependencies

JDK_UPDATE_VERSION = 131
BUILD_VERSION = b11
COMBINED_VERSION = $(JDK_UPDATE_VERSION)-$(BUILD_VERSION)

CORBA_CHANGESET = 22ed32f45405
JAXP_CHANGESET = fdc2a6442d2f
JAXWS_CHANGESET = c1bfc2395c57
JDK_CHANGESET = cfc292a2c1c6
LANGTOOLS_CHANGESET = 4ef0ee927940
OPENJDK_CHANGESET = ed5ee0ac7111
NASHORN_CHANGESET = f2d9bca28d0e

CORBA_SHA256SUM = 47210b6c69dcc6193b9bf0a3d61b75b48f4fa56e8ca348e40200cfa14eca3fd1
JAXP_SHA256SUM = c74a8a27f1d2dfeaabfce3b5b46623e367fb0fbd5938a3aca8fcd23eb2ce1d53
JAXWS_SHA256SUM = 90642e9131f4c8922576305224278fcae72d8363956b76d4cdbf813027836cac
JDK_SHA256SUM = ec71e37b98b4baa6831c5cb30bcc1ab18cd95993744dbc4d37a28b2dc5049896
LANGTOOLS_SHA256SUM = 489799c6a86fbfb7da2f2c0ec48e44970a152ea38b97bb40cc04bc09155ab39f
OPENJDK_SHA256SUM = f1eb8c8e45965adcaa1e9cc70df043a825d52409e96712d266167994ff88456d
NASHORN_SHA256SUM = 3f3861e7268a3986fa8d5c940b85a0de1003f7ebb212df157a9b421ac621d6ae

HS_TYPE = "`$(AWK) 'version==$$1 {print $$2}' version=$(HSBUILD) $(abs_top_builddir)/hotspot.map`"
HS_URL = "`$(AWK) 'version==$$1 {print $$3}' version=$(HSBUILD) $(abs_top_builddir)/hotspot.map`"
HS_CHANGESET = "`$(AWK) 'version==$$1 {print $$4}' version=$(HSBUILD) $(abs_top_builddir)/hotspot.map`"
HS_SHA256SUM = "`$(AWK) 'version==$$1 {print $$5}' version=$(HSBUILD) $(abs_top_builddir)/hotspot.map`"

DROP_URL = http://icedtea.classpath.org/download/drops

# Drop is a snapshot of the changeset specified by CACAO_VERSION taken
# from the repository at https://bitbucket.org/gnu_andrew/cacao-staging
CACAO_VERSION = 900db2220376
CACAO_SHA256SUM = eccda2d8bdf6a60347d5479b42143209f19c3731ef93666a3d12185f759165d8
CACAO_BASE_URL = $(DROP_URL)/cacao
CACAO_SRC_ZIP = cacao-$(CACAO_VERSION).tar.xz
CACAO_URL = $(CACAO_BASE_URL)/$(CACAO_SRC_ZIP)

JAMVM_VERSION = ec18fb9e49e62dce16c5094ef1527eed619463aa
JAMVM_SHA256SUM = 31810266666c23822942aac62a78019c2c4589e1c5ee48329cbf42652d4437bc
JAMVM_BASE_URL = $(DROP_URL)/jamvm
JAMVM_URL = $(JAMVM_BASE_URL)/jamvm-$(JAMVM_VERSION).tar.gz
JAMVM_SRC_ZIP = jamvm-$(JAMVM_VERSION).tar.gz

ICEDTEA_MAJOR = icedtea8
ICEDTEA_PREFIX = $(ICEDTEA_MAJOR)-forest
ICEDTEA_HG_URL = http://icedtea.classpath.org/hg/$(ICEDTEA_PREFIX)
ICEDTEA_URL = $(DROP_URL)/$(ICEDTEA_MAJOR)/$(ICEDTEA_RELEASE)

# Build directories

BUILD_DIR = @OPENJDK_BUILD_DIR@
BUILD_OUTPUT_DIR = $(abs_top_builddir)/$(BUILD_DIR)
DEBUG_BUILD_OUTPUT_DIR = $(BUILD_OUTPUT_DIR)-debug
BOOT_BUILD_OUTPUT_DIR = $(BUILD_OUTPUT_DIR)-boot
BOOT_DIR = $(abs_top_builddir)/bootstrap/jdk1.7.0
RUNTIME = $(BOOT_DIR)/jre/lib/rt.jar
CLS_DIR_BOOT = $(BOOT_BUILD_OUTPUT_DIR)/classes
SERVER_DIR = hotspot/dist/jre/lib/$(INSTALL_ARCH_DIR)/server
BUILD_IMAGE_DIR = $(BUILD_OUTPUT_DIR)/images
BUILD_SDK_DIR = $(BUILD_IMAGE_DIR)/j2sdk-image
BUILD_JRE_DIR = $(BUILD_IMAGE_DIR)/j2re-image
BUILD_DEBUGINFO_DIR = $(BUILD_IMAGE_DIR)/j2sdk-image.stripped
BUILD_JRE_ARCH_DIR = $(BUILD_SDK_DIR)/jre/lib/$(INSTALL_ARCH_DIR)
BUILD_DEBUG_IMAGE_DIR = $(DEBUG_BUILD_OUTPUT_DIR)/images
BUILD_DEBUG_SDK_DIR = $(BUILD_DEBUG_IMAGE_DIR)/j2sdk-image
BUILD_DEBUG_JRE_DIR = $(BUILD_DEBUG_IMAGE_DIR)/j2re-image
BUILD_DEBUG_DEBUGINFO_DIR = $(BUILD_DEBUG_IMAGE_DIR)/j2sdk-image.stripped
BUILD_DEBUG_JRE_ARCH_DIR = $(BUILD_DEBUG_SDK_DIR)/jre/lib/$(INSTALL_ARCH_DIR)
BUILD_BOOT_IMAGE_DIR = $(BOOT_BUILD_OUTPUT_DIR)/images
BUILD_BOOT_SDK_DIR = $(BUILD_BOOT_IMAGE_DIR)/j2sdk-image
BUILD_BOOT_JRE_DIR = $(BUILD_BOOT_IMAGE_DIR)/j2re-image
BUILD_BOOT_DEBUGINFO_DIR = $(BUILD_BOOT_IMAGE_DIR)/j2sdk-image.stripped
STAGE1_BOOT_DIR = $(abs_top_builddir)/bootstrap/boot
STAGE2_BOOT_DIR = $(abs_top_builddir)/bootstrap/icedtea
JAMVM_IMPORT_PATH = $(abs_top_builddir)/jamvm/install/hotspot
CRYPTO_CHECK_BUILD_DIR = $(abs_top_builddir)/test/cryptocheck.build
ECC_CHECK_BUILD_DIR = $(abs_top_builddir)/test/ecccheck.build

# Installation directories

DEBUG_PREFIX = $(libdir)/debug

# Source directories

SHARE = openjdk-boot/jdk/src/share/classes
SOLARIS = openjdk-boot/jdk/src/solaris/classes
LANGTOOLS = openjdk-boot/langtools/src/share/classes
CORBA = openjdk-boot/corba/src/share/classes
JAXWS = openjdk-boot/jaxws/src/share/jaxws_classes

OPENJDK_SOURCEPATH_DIRS = \
        $(SHARE):$(SOLARIS):$(LANGTOOLS):$(CORBA):$(JAXWS)

SOURCEPATH_DIRS = $(OPENJDK_SOURCEPATH_DIRS)

VERSION_SETTINGS_FILE = openjdk/jdk/make/ProfileNames.gmk

VERSION_SETTINGS_FILE_BACKUP = $$(basename $(VERSION_SETTINGS_FILE)).bak

CACERTS_FILE = @ALT_CACERTS_FILE@

# Settings for javac

IT_LANGUAGE_SOURCE_VERSION=6
IT_CLASS_TARGET_VERSION=6
IT_JAVAC_SETTINGS=-g -encoding utf-8 $(JAVACFLAGS) $(MEMORY_LIMIT) $(PREFER_SOURCE)
IT_JAVACFLAGS=$(IT_JAVAC_SETTINGS) -source $(IT_LANGUAGE_SOURCE_VERSION) -target $(IT_CLASS_TARGET_VERSION)

# Flags
IT_CFLAGS=$(CFLAGS) $(ARCHFLAG)
ICEDTEA_MAKE=$(subst -j,JOBS=,$(MAKE))

# Conditional defintions

TAR_SUFFIX = tar.xz
if USE_HG
  OPENJDK_SRC_ZIP = 
  CORBA_SRC_ZIP =
  JAXP_SRC_ZIP =
  JAXWS_SRC_ZIP =
  JDK_SRC_ZIP =
  LANGTOOLS_SRC_ZIP =
  HOTSPOT_SRC_ZIP =
  NASHORN_SRC_ZIP =
else
  OPENJDK_SRC_ZIP = openjdk.$(TAR_SUFFIX)
  CORBA_SRC_ZIP = corba.$(TAR_SUFFIX)
  JAXP_SRC_ZIP = jaxp.$(TAR_SUFFIX)
  JAXWS_SRC_ZIP = jaxws.$(TAR_SUFFIX)
  JDK_SRC_ZIP = jdk.$(TAR_SUFFIX)
  LANGTOOLS_SRC_ZIP = langtools.$(TAR_SUFFIX)
  HOTSPOT_SRC_ZIP = hotspot.$(TAR_SUFFIX)
  NASHORN_SRC_ZIP = nashorn.$(TAR_SUFFIX)
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

if DISABLE_NASHORN_CHECKSUM
NASHORN_SHA256SUM_BIN = true
else
NASHORN_SHA256SUM_BIN = $(SHA256SUM)
endif

if WITH_NASHORN_CHECKSUM
NASHORN_SUM = @NASHORN_CHECKSUM@
else
NASHORN_SUM = $(NASHORN_SHA256SUM)
endif

if WITH_HGREV
REV_ARG = -r $(HGREV)
endif

# This should not depend on bootstrapping
# but on whether MEMORY_LIMIT is accepted
# as an argument to javac
if BOOTSTRAPPING
  MEMORY_LIMIT =
  PREFER_SOURCE = 
else
  MEMORY_LIMIT = -J-Xmx1024m
  PREFER_SOURCE = -Xprefer:source
endif

ICEDTEA_BUILD_TARGET=images

if ENABLE_DOCS
  ICEDTEA_BUILD_TARGET += docs
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

if !DISABLE_SYSTEMTAP_TESTS
SYSTEMTAP_TEST_SUITE = check-tapset
endif

# Target to ensure a patched OpenJDK tree containing Zero & Shark
# and any overlays is available in $(abs_top_builddir)/openjdk
OPENJDK_TREE = stamps/overlay.stamp

# Target to ensure a patched OpenJDK bootstrap tree containing
# Zero & Shark, any other overlays and the bootstrapping patches
# is available in $(abs_top_builddir)/openjdk-boot
OPENJDK_BOOT_TREE = stamps/patch-boot.stamp

# Sources list

CRYPTO_CHECK_SRCS = $(top_srcdir)/test/standalone/TestCryptoLevel.java

ECC_CHECK_SRCS = $(top_srcdir)/test/standalone/TestEllipticCurveCryptoSupport.java \
	$(top_srcdir)/test/standalone/TestECDSA.java

# Patch list

ICEDTEA_PATCHES = \
	patches/override-redirect-metacity.patch

# Conditional patches

if WITH_ALT_HSBUILD
ICEDTEA_PATCHES += \
	patches/hotspot/$(HSBUILD)/8038392-rh1064383.patch \
	patches/hotspot/$(HSBUILD)/8041658.patch \
	patches/hotspot/$(HSBUILD)/8087218-pr2740.patch \
	patches/hotspot/$(HSBUILD)/8145096-pr2854.patch \
	patches/hotspot/$(HSBUILD)/8158260-pr2991-rh1341258.patch \
	patches/hotspot/$(HSBUILD)/crash_dump.patch \
	patches/hotspot/$(HSBUILD)/no_devirt.patch \
	patches/hotspot/$(HSBUILD)/pr1368.patch \
	patches/hotspot/$(HSBUILD)/pr1748.patch \
	patches/hotspot/$(HSBUILD)/pr1869.patch \
	patches/hotspot/$(HSBUILD)/pr1942.patch \
	patches/hotspot/$(HSBUILD)/pr1975.patch \
	patches/hotspot/$(HSBUILD)/pr2777.patch \
	patches/hotspot/$(HSBUILD)/werror.patch \
	patches/hotspot/$(HSBUILD)/8162384-pr3122.patch \
	patches/hotspot/$(HSBUILD)/8078628-pr3208.patch \
	patches/hotspot/$(HSBUILD)/8141491-pr3159-g592292.patch \
	patches/hotspot/$(HSBUILD)/8043780-pr3368.patch \
	patches/hotspot/$(HSBUILD)/8173941-pr3326.patch \
	patches/hotspot/$(HSBUILD)/8174164-pr3334-rh1417266.patch \
	patches/hotspot/$(HSBUILD)/8175097-pr3334-rh1417266.patch

# AArch32 already has 6515172 from upstream 8u131
if !WITH_AARCH32_HSBUILD
ICEDTEA_PATCHES += \
	patches/hotspot/$(HSBUILD)/6515172-pr3346.patch
endif

endif

if WITH_AARCH32_HSBUILD
ICEDTEA_PATCHES += \
	patches/aarch32-conf.patch \
	patches/aarch32-jdk.patch
endif

if BUILD_CACAO
ICEDTEA_PATCHES += \
	patches/cacao/launcher.patch
else
if USING_CACAO
ICEDTEA_PATCHES +=
endif
endif

if ENABLE_CACAO
ICEDTEA_PATCHES += \
	patches/cacao/ignore-tests.patch
endif

if BUILD_JAMVM
ICEDTEA_PATCHES += \
	patches/jamvm/find_class_from_caller.patch \
	patches/jamvm/pr2034-tempdir.patch \
	patches/jamvm/noexecstack.patch \
	patches/jamvm/pr2775-cache_api.patch
endif

if !ENABLE_SUNEC
ICEDTEA_PATCHES += \
	patches/disable-intree-ec.patch
endif

if !USE_NON_NSS_CURVES
ICEDTEA_PATCHES += patches/rh1022017.patch
endif

if ENABLE_NSS
if ENABLE_SUNEC
ICEDTEA_PATCHES += patches/nss-config-with-sunec.patch
else
ICEDTEA_PATCHES += patches/nss-config.patch
endif
else
if ENABLE_SUNEC
ICEDTEA_PATCHES += patches/nss-not-enabled-config-with-sunec.patch
else
ICEDTEA_PATCHES += patches/nss-not-enabled-config.patch
endif
endif

ICEDTEA_PATCHES += $(DISTRIBUTION_PATCHES)

# Bootstrapping patches

ICEDTEA_BOOT_PATCHES =

ICEDTEA_BOOT_PATCHES += $(DISTRIBUTION_BOOT_PATCHES)

# OpenJDK build environment.
ICEDTEA_NAME = IcedTea
if HAS_ICEDTEA_REVISION
ICEDTEA_REV = +${ICEDTEA_REVISION}
endif
if HAS_PKGVERSION
ICEDTEA_PKG = $(EMPTY) (${PKGVERSION})
endif

ICEDTEA_COMMON_ENV = \
	LANG="C" \
	PATH="$(BOOT_DIR)/bin:$(OS_PATH):$(NO_CCACHE_PATH)" \
	CLASSPATH="" \
	LD_LIBRARY_PATH=""

ICEDTEA_CONFIGURE = \
	  --build=$(build_triplet) \
	  --host=$(host_triplet) \
	  --enable-unlimited-crypto \
	  --with-stdc++lib=dynamic \
	  --with-boot-jdk=$(BOOT_DIR) \
	  --with-update-version=$(JDK_UPDATE_VERSION) \
	  --with-build-number=$(BUILD_VERSION) \
	  --with-milestone="fcs" \
	  --with-extra-cflags="$(CFLAGS)" \
	  --with-extra-cxxflags="$(CXXFLAGS)" \
	  --with-extra-ldflags="$(LDFLAGS)"

ICEDTEA_CONFIGURE_ENV = \
	${ICEDTEA_COMMON_ENV} \
	FREETYPE_CFLAGS="${FREETYPE2_CFLAGS}" \
	FREETYPE_LIBS="${FREETYPE2_LIBS}"

if ENABLE_CACAO
ICEDTEA_CONFIGURE += \
	--with-import-hotspot="$(CACAO_IMPORT_PATH)"
endif

if ENABLE_JAMVM
ICEDTEA_CONFIGURE += \
	--with-import-hotspot="$(JAMVM_IMPORT_PATH)"
endif

if USE_SYSTEM_ZLIB
ICEDTEA_CONFIGURE += \
	--with-zlib=system
else
ICEDTEA_CONFIGURE += \
	--with-zlib=bundled
endif

if USE_SYSTEM_LCMS
ICEDTEA_CONFIGURE += \
	--with-lcms=system
ICEDTEA_CONFIGURE_ENV += \
	LCMS_LIBS="${LCMS2_LIBS}" \
	LCMS_CFLAGS="${LCMS2_CFLAGS}"
else
ICEDTEA_CONFIGURE += \
	--with-lcms=bundled
endif

if USE_SYSTEM_JPEG
ICEDTEA_CONFIGURE += \
	--with-libjpeg=system
else
ICEDTEA_CONFIGURE += \
	--with-libjpeg=bundled
endif

if USE_SYSTEM_PNG
ICEDTEA_CONFIGURE += \
	--with-libpng=system
ICEDTEA_CONFIGURE_ENV += \
	PNG_LIBS="${PNG_LIBS}" \
	PNG_CFLAGS="${PNG_CFLAGS}"
else
ICEDTEA_CONFIGURE += \
	--with-libpng=bundled
endif

if USE_SYSTEM_GIF
ICEDTEA_CONFIGURE += \
	--with-giflib=system
else
ICEDTEA_CONFIGURE += \
	--with-giflib=bundled
endif

if ENABLE_SUNEC
ICEDTEA_CONFIGURE_ENV += \
	NSS_LIBS="${SUNEC_LIBS}" \
	NSS_CFLAGS="${SUNEC_CFLAGS}"
ICEDTEA_CONFIGURE += \
	--enable-system-nss
else
ICEDTEA_CONFIGURE += \
	--disable-system-nss
endif

if USE_SYSTEM_KERBEROS
ICEDTEA_CONFIGURE_ENV += \
	KRB5_LIBS="${KRB5_LIBS}" \
	KRB5_CFLAGS="${KRB5_CFLAGS}"
ICEDTEA_CONFIGURE += \
	--enable-system-kerberos
else
ICEDTEA_CONFIGURE += \
	--disable-system-kerberos
endif

if USE_SYSTEM_PCSC
ICEDTEA_CONFIGURE_ENV += \
	PCSC_LIBS="${PCSC_LIBS}" \
	PCSC_CFLAGS="${PCSC_CFLAGS}"
ICEDTEA_CONFIGURE += \
	--enable-system-pcsc
else
ICEDTEA_CONFIGURE += \
	--disable-system-pcsc
endif

if USE_SYSTEM_SCTP
ICEDTEA_CONFIGURE_ENV += \
	SCTP_LIBS="${SCTP_LIBS}" \
	SCTP_CFLAGS="${SCTP_CFLAGS}"
ICEDTEA_CONFIGURE += \
	--enable-system-sctp
else
ICEDTEA_CONFIGURE += \
	--disable-system-sctp
endif

if ZERO_BUILD
ICEDTEA_CONFIGURE += \
	--with-jvm-variants=zero
endif

if SHARK_BUILD
ICEDTEA_CONFIGURE += \
	--with-jvm-variants=zeroshark
endif

if ENABLE_WERROR
ICEDTEA_CONFIGURE += \
	--enable-warnings-as-errors
else
ICEDTEA_CONFIGURE += \
	--disable-warnings-as-errors
endif

if USE_ALT_CACERTS_FILE
ICEDTEA_CONFIGURE += \
	--with-cacerts-file=$(CACERTS_FILE)
endif

if ENABLE_IMPROVED_FONT_RENDERING
ICEDTEA_CONFIGURE += \
	--enable-improved-font-rendering
ICEDTEA_CONFIGURE_ENV += \
	FONTCONFIG_LIBS="${FONTCONFIG_LIBS}" \
	FONTCONFIG_CFLAGS="${FONTCONFIG_CFLAGS}"
else
ICEDTEA_CONFIGURE += \
	--disable-improved-font-rendering
endif

if BUILD_HEADLESS
ICEDTEA_CONFIGURE += \
	--disable-headful
endif

if USE_CCACHE
ICEDTEA_CONFIGURE += \
	--enable-ccache
else
ICEDTEA_CONFIGURE += \
	--disable-ccache
endif

if ENABLE_JAVA_DEBUGINFO
ICEDTEA_CONFIGURE += \
	--with-java-debug-symbols=yes
else
ICEDTEA_CONFIGURE += \
	--with-java-debug-symbols=no
endif

if DISABLE_PRECOMPILED_HEADERS
ICEDTEA_CONFIGURE += \
	--disable-precompiled-headers
else
ICEDTEA_CONFIGURE += \
	--enable-precompiled-headers
endif

ICEDTEA_UNSET = \
	JAVAC= \
	JAVA_HOME= \
	JDK_HOME= \
	OS=

ICEDTEA_ENV = \
	${ICEDTEA_COMMON_ENV} \
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
	QUIETLY="" \
	ANT_RESPECT_JAVA_HOME="TRUE" \
	DISTRIBUTION_ID="$(DIST_ID)" \
	DERIVATIVE_ID="$(ICEDTEA_NAME) $(PACKAGE_VERSION)$(ICEDTEA_REV)" \
	LOG="debug" \
	POST_STRIP_CMD= STRIP_POLICY="no_strip" \
	JOBS="$(PARALLEL_JOBS)" \
	INSTALL_LOCATION="${prefix}"

if USE_ALT_JAR
ICEDTEA_ENV += \
	ALT_JAR_CMD="$(ALT_JAR_CMD)"
endif

if HAS_HOTSPOT_REVISION
ICEDTEA_ENV += \
	HOTSPOT_BUILD_VERSION="$(HOTSPOT_REVISION)"
endif

if USE_SYSTEM_ZLIB
ICEDTEA_ENV += \
	LIBZ="${ZLIB_LIBS}" \
	ZLIB_CPPFLAGS="${ZLIB_CFLAGS}"
endif

if ENABLE_JAR_COMPRESSION
ICEDTEA_ENV += \
	COMPRESS_JARS="true"
endif

if USE_SYSTEM_JPEG
ICEDTEA_ENV += \
	LIBJPEG_LDFLAGS="${JPEG_LIBS}" \
	LIBJPEG_CFLAGS="${JPEG_CFLAGS}"
endif

if USE_SYSTEM_GIF
ICEDTEA_ENV += \
	GIF_LDFLAGS="${GIF_LIBS}" \
	GIF_CFLAGS="${GIF_CFLAGS}"
endif

if ENABLE_NATIVE_DEBUGINFO
ICEDTEA_ENV += \
	DEBUG_BINARIES="true"
endif

# OpenJDK boot build environment.
ICEDTEA_CONFIGURE_BOOT = $(ICEDTEA_CONFIGURE)
ICEDTEA_ENV_BOOT = $(ICEDTEA_ENV) \
	JAR_KNOWS_ATFILE="$(JAR_KNOWS_ATFILE)" \
	JAR_KNOWS_J_OPTIONS="$(JAR_KNOWS_J_OPTIONS)" \
	JAR_ACCEPTS_STDIN_LIST="$(JAR_ACCEPTS_STDIN_LIST)" \
	DISABLE_NIMBUS="true"

# Source files

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

EXTRA_DIST = $(top_srcdir)/patches/*.patch \
	$(top_srcdir)/patches/cacao/*.patch \
	$(top_srcdir)/patches/jamvm/*.patch \
	$(top_srcdir)/patches/hotspot/*/*.patch \
	tools-copy contrib overlays \
	$(JTREG_SRCS) HACKING autogen.sh \
	tapset/hotspot.stp.in \
	tapset/hotspot_jni.stp.in \
	tapset/jstack.stp.in \
	tapset/hotspot_gc.stp.in \
	scripts/jni_create_stap.c \
	scripts/jni_desc \
	THANKYOU test/tapset \
	$(CRYPTO_CHECK_SRCS) \
	$(ECC_CHECK_SRCS)

# Top-Level Targets
# =================

all-local: icedtea-stage2 $(DESKTOP_FILES)

check-local: jtregcheck $(SYSTEMTAP_TEST_SUITE) stamps/check-ecc.stamp \
 stamps/check-java-debug.stamp stamps/check-java-src.stamp

clean-tests: clean-jtreg clean-tapset-report clean-jtreg-reports clean-check-ecc \
 clean-check-java-debug clean-check-java-src

clean-local: clean-tests \
 clean-icedtea clean-icedtea-debug clean-icedtea-boot clean-clone clean-clone-boot \
 clean-bootstrap-directory-stage1 clean-bootstrap-directory-stage2 \
 clean-bootstrap-directory-symlink-stage1 clean-bootstrap-directory-symlink-stage2 \
 clean-extract clean-icedtea-stage2 clean-icedtea-debug-stage2 clean-icedtea-stage1 \
 clean-add-zero clean-add-zero-debug clean-add-cacao clean-add-cacao-debug \
 clean-cryptocheck clean-ecccheck clean-policytool-@JAVA_VER@.desktop \
 clean-jconsole-@JAVA_VER@.desktop
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
	hotspot hotspot-helper clean-jtreg clean-jtreg-reports \
	jtregcheck clean-remove-intree-libraries clean-check-java-src \
	clean-jamvm clean-extract-jamvm clean-add-jamvm clean-add-jamvm-debug \
	clean-extract-hotspot clean-sanitise-openjdk clean-icedtea-debug \
	clean-download-nashorn clean-extract-nashorn clean-download-hotspot \
	clean-download-corba clean-download-jaxp clean-download-jaxws \
	clean-download-langtools clean-download-jdk clean-download-openjdk \
	clean-extract-corba clean-extract-jaxp clean-extract-jaxws clean-extract-jdk \
	clean-extract-langtools clean-check-crypto clean-check-crypto-debug clean-check-crypto-boot \
	clean-add-archive clean-add-archive-debug clean-add-archive-boot clean-cryptocheck \
	clean-tests clean-tapset-report clean-ecccheck clean-check-ecc clean-check-java-debug \
	check-jdk check-hotspot check-langtools jtregcheck check-tapset check-tapset-probes \
	check-tapset-jstack clean-policytool-@JAVA_VER@.desktop clean-jconsole-@JAVA_VER@.desktop \
	clean-split-debuginfo clean-split-debuginfo-debug clean-split-debuginfo-boot

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
	if test "x${ZIP}" = "x"; then \
	  echo "ERROR: zip not found"; exit 1; \
	fi
	find openjdk/ -name \\.hg* | xargs rm -rf	
	$(ZIP) -r openjdk-$(COMBINED_VERSION) openjdk/

# Creates archive of openjdk that is compliant with Free Software guidelines.
dist-openjdk-fsg: stamps/patch-fsg.stamp
	if test "x${ZIP}" = "x"; then \
	  echo "ERROR: zip not found"; exit 1; \
	fi
	$(ZIP) -r openjdk-fsg-$(COMBINED_VERSION) openjdk/

dist-openjdk-fsg-xz: stamps/patch-fsg.stamp
	tardir=openjdk/ && $(am__tar) | XZ_OPT=$${XZ_OPT--e} xz -c \
	  >openjdk-fsg-$(COMBINED_VERSION).tar.xz

# OpenJDK Source Preparation Targets
# ==================================

# Download OpenJDK sources.

stamps/download.stamp: stamps/download-openjdk.stamp stamps/download-corba.stamp \
 stamps/download-jaxp.stamp stamps/download-jaxws.stamp stamps/download-jdk.stamp \
 stamps/download-cacao.stamp stamps/download-jamvm.stamp stamps/download-hotspot.stamp \
 stamps/download-langtools.stamp stamps/download-nashorn.stamp
	mkdir -p stamps
	touch $@

clean-download: clean-download-openjdk clean-download-cacao clean-download-jamvm \
 clean-download-hotspot clean-download-corba clean-download-jaxp clean-download-jaxws \
 clean-download-jdk clean-download-langtools clean-download-nashorn
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

stamps/download-nashorn.stamp: stamps/download-openjdk.stamp
if OPENJDK_SRC_DIR_FOUND
else
if USE_HG
	$(HG) clone $(REV_ARG) $(ICEDTEA_HG_URL)/nashorn openjdk/nashorn
else
if USE_ALT_NASHORN_SRC_ZIP
	ln -sf $(ALT_NASHORN_SRC_ZIP) $(NASHORN_SRC_ZIP)
endif
	if ! echo "$(NASHORN_SUM)  $(NASHORN_SRC_ZIP)" \
	  | $(NASHORN_SHA256SUM_BIN) --check ; \
	then \
	  if test "x$(enable_downloading)" = "xyes"; then \
	    if [ -e $(NASHORN_SRC_ZIP) ] ; then \
	      mv $(NASHORN_SRC_ZIP) $(NASHORN_SRC_ZIP).old ; \
	    fi ; \
	    $(WGET) $(ICEDTEA_URL)/$(NASHORN_SRC_ZIP); \
	    if ! echo "$(NASHORN_SUM)  $(NASHORN_SRC_ZIP)" \
	      | $(SHA256SUM) --check ; then \
	      echo "ERROR: Bad download of NASHORN zip"; false; \
	    fi; \
	  else \
	    echo "ERROR: No up-to-date OpenJDK Nashorn zip available"; exit -1; \
	  fi ; \
	fi
endif
endif
	mkdir -p stamps
	touch $@

clean-download-nashorn:
	if [ ! -z $(NASHORN_SRC_ZIP) ] ; then \
	  rm -f $(NASHORN_SRC_ZIP) ; \
	fi
	rm -f stamps/download-nashorn.stamp

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
 stamps/extract-cacao.stamp stamps/remove-intree-libraries.stamp \
 stamps/extract-langtools.stamp stamps/extract-jamvm.stamp \
 stamps/extract-hotspot.stamp stamps/sanitise-openjdk.stamp \
 stamps/extract-nashorn.stamp
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
	    mv *-$(OPENJDK_CHANGESET) openjdk ; \
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
 clean-extract-corba clean-extract-langtools clean-extract-nashorn
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

stamps/extract-nashorn.stamp: stamps/extract-openjdk.stamp stamps/download-nashorn.stamp
if OPENJDK_SRC_DIR_FOUND
else
if USE_HG
	$(HG) clone $(REV_ARG) $(ICEDTEA_HG_URL)/nashorn openjdk/nashorn
else
	set -e ; \
	if [ ! -z $(NASHORN_SRC_ZIP) ] ; then \
	  if test -e ${NASHORN_SRC_ZIP} ; \
	  then \
	    if ! test -d openjdk/nashorn ; \
	    then \
	      case "$(NASHORN_SRC_ZIP)" in \
	        *.zip) $(UNZIP) -q $(NASHORN_SRC_ZIP) ;; \
	        *.tar.*) $(TAR) xf $(NASHORN_SRC_ZIP) ;; \
	      esac; \
	      mv nashorn-$(NASHORN_CHANGESET) openjdk/nashorn ; \
	    fi ; \
	  fi ; \
	fi
endif
endif
	touch $@

clean-extract-nashorn:
	rm -rf openjdk/nashorn
	rm -f stamps/extract-nashorn.stamp

stamps/sanitise-openjdk.stamp: stamps/extract-openjdk.stamp \
 stamps/extract-hotspot.stamp stamps/extract-jdk.stamp \
 stamps/extract-langtools.stamp stamps/extract-corba.stamp \
 stamps/extract-jaxp.stamp stamps/extract-jaxws.stamp \
 stamps/extract-nashorn.stamp
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
	  $(TAR) xf $(CACAO_SRC_ZIP) -C cacao ; \
	  dir=$$(basename cacao/gnu*) ; \
	  mv -v cacao/$$dir cacao/cacao ; \
	  ln -s cacao cacao/$$dir ; \
	fi
endif
endif
endif
	mkdir -p stamps
	touch $@

clean-extract-cacao: clean-cacao
	rm -rf cacao
	rm -f stamps/extract-cacao.stamp

stamps/extract-jamvm.stamp: stamps/download-jamvm.stamp
if BUILD_JAMVM
	set -e ; \
	if ! test -d jamvm ; \
	then \
	  mkdir jamvm ; \
	  $(TAR) xzf $(JAMVM_SRC_ZIP) -C jamvm ; \
	  dir=$$(basename jamvm/jamvm-*) ; \
	  mv -v jamvm/$$dir jamvm/jamvm ; \
	  ln -s jamvm jamvm/$$dir ; \
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

stamps/patch-fsg.stamp: stamps/extract.stamp
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
	mv $(VERSION_SETTINGS_FILE) $(VERSION_SETTINGS_FILE_BACKUP)
	cat $(VERSION_SETTINGS_FILE_BACKUP) > $(VERSION_SETTINGS_FILE)
	icedtea_version="$(PACKAGE_VERSION)$(ICEDTEA_REV)" ; \
	if test "x$(ENABLE_CACAO)" = "xyes" || \
	   test "x$(ENABLE_JAMVM)" = "xyes"; then \
	  echo "JDK_DERIVATIVE_NAME=$${icedtea_version}" \
	    >>$(VERSION_SETTINGS_FILE) ; \
	  echo "PRODUCT_NAME=$(ICEDTEA_NAME)" \
	    >>$(VERSION_SETTINGS_FILE) ; \
	else \
	  echo "JDK_DERIVATIVE_NAME=$(ICEDTEA_NAME) $${icedtea_version}" \
	    >>$(VERSION_SETTINGS_FILE) ; \
	fi ; 

if HAS_JDK_REVISION
	echo "JDK_REVID=$(JDK_REVISION)" >>$(VERSION_SETTINGS_FILE) ;
endif

	echo "DISTRO_NAME=$(DIST_NAME)" >>$(VERSION_SETTINGS_FILE) ;
if HAS_PKGVERSION
	  echo "DISTRO_PACKAGE_VERSION=$(PKGVERSION)" \
	    >>$(VERSION_SETTINGS_FILE) ;
endif
	if test x"$(PROJECT_NAME)" != "xjdk8"; then \
	  proj_suffix="-$(PROJECT_NAME)"; \
	fi ; \
	if test x"$(VERSION_SUFFIX)" != "x"; then \
	  ver_suffix="-$(VERSION_SUFFIX)"; \
	fi ; \
	$(SED) -i "s#BUILD_VARIANT_RELEASE)#BUILD_VARIANT_RELEASE)$${proj_suffix}$${ver_suffix}#" \
	  $(VERSION_SETTINGS_FILE);
	mkdir -p stamps
	touch $@

clean-versioning:
	if [ -e $(VERSION_SETTINGS_FILE_BACKUP) ] ; then \
	  mv $(VERSION_SETTINGS_FILE_BACKUP) $(VERSION_SETTINGS_FILE) ; \
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

# bootstrap/stage1
stamps/bootstrap-directory-stage1.stamp:
	mkdir -p $(STAGE1_BOOT_DIR)/bin stamps/
	ln -sf $(JAVA) $(STAGE1_BOOT_DIR)/bin/java
	ln -sf ../../../javah $(STAGE1_BOOT_DIR)/bin/javah
	ln -sf $(RMIC) $(STAGE1_BOOT_DIR)/bin/rmic
	ln -sf $(JAR) $(STAGE1_BOOT_DIR)/bin/jar
	ln -sf $(NATIVE2ASCII) $(STAGE1_BOOT_DIR)/bin/native2ascii
	ln -sf ../../../javac $(STAGE1_BOOT_DIR)/bin/javac
	ln -sf ../../../javap $(STAGE1_BOOT_DIR)/bin/javap
	mkdir -p $(STAGE1_BOOT_DIR)/lib/modules
	mkdir -p $(STAGE1_BOOT_DIR)/jre/lib && \
	cp $(SYSTEM_JDK_DIR)/jre/lib/rt.jar \
	  $(STAGE1_BOOT_DIR)/jre/lib/rt.jar && \
	chmod u+w $(STAGE1_BOOT_DIR)/jre/lib/rt.jar
	mkdir -p $(STAGE1_BOOT_DIR)/lib && \
	if [ -e $(SYSTEM_JDK_DIR)/lib/tools.jar ] ; then \
	  ln -sf $(SYSTEM_JDK_DIR)/lib/tools.jar $(STAGE1_BOOT_DIR)/lib/tools.jar ; \
	else \
	  ln -sf $(STAGE1_BOOT_DIR)/jre/lib/rt.jar $(STAGE1_BOOT_DIR)/lib/tools.jar ; \
	fi
	ln -sf $(SYSTEM_JDK_DIR)/jre/lib/$(JRE_ARCH_DIR) \
	  $(STAGE1_BOOT_DIR)/jre/lib/ && \
	if ! test -d $(STAGE1_BOOT_DIR)/jre/lib/$(INSTALL_ARCH_DIR); \
	  then \
	  ln -sf ./$(JRE_ARCH_DIR) \
	    $(STAGE1_BOOT_DIR)/jre/lib/$(INSTALL_ARCH_DIR); \
	fi
	mkdir -p $(STAGE1_BOOT_DIR)/include && \
	for i in $(SYSTEM_JDK_DIR)/include/*; do \
	  test -r $$i | continue; \
	  i=`basename $$i`; \
	  rm -f $(STAGE1_BOOT_DIR)/include/$$i; \
	  ln -s $(SYSTEM_JDK_DIR)/include/$$i $(STAGE1_BOOT_DIR)/include/$$i; \
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
stamps/icedtea-configure.stamp: stamps/bootstrap-directory-symlink-stage2.stamp \
 stamps/download.stamp stamps/extract.stamp $(OPENJDK_TREE) \
 stamps/cacao.stamp stamps/jamvm.stamp
	mkdir -p $(BUILD_OUTPUT_DIR)
	cd $(BUILD_OUTPUT_DIR) && \
          $(ICEDTEA_CONFIGURE_ENV) $(SHELL) $(abs_top_builddir)/openjdk/configure \
          $(ICEDTEA_CONFIGURE)
	mkdir -p stamps
	touch $@

stamps/icedtea.stamp: stamps/icedtea-configure.stamp
	$(ICEDTEA_UNSET) $(ARCH_PREFIX) $(ICEDTEA_MAKE) \
	  $(ICEDTEA_ENV) \
	  -C $(BUILD_OUTPUT_DIR) \
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
	cp $(abs_top_builddir)/nss.cfg \
	  $(BUILD_SDK_DIR)/jre/lib/security;
if ENABLE_JAMVM
	printf -- '-jamvm ALIASED_TO -server\n' >> $(BUILD_JRE_ARCH_DIR)/jvm.cfg
endif
	@echo "IcedTea is served:" $(BUILD_SDK_DIR)
	mkdir -p stamps
	touch $@

stamps/icedtea-debug-configure.stamp: stamps/bootstrap-directory-symlink-stage2.stamp \
 stamps/download.stamp stamps/extract.stamp $(OPENJDK_TREE) \
 stamps/cacao.stamp stamps/jamvm.stamp
	mkdir -p $(DEBUG_BUILD_OUTPUT_DIR)
	cd $(DEBUG_BUILD_OUTPUT_DIR) && \
          $(ICEDTEA_CONFIGURE_ENV) $(SHELL) $(abs_top_builddir)/openjdk/configure \
          $(ICEDTEA_CONFIGURE) --with-debug-level=slowdebug
	mkdir -p stamps
	touch $@

stamps/icedtea-debug.stamp: stamps/icedtea-debug-configure.stamp
	$(ICEDTEA_UNSET) $(ARCH_PREFIX) $(ICEDTEA_MAKE) \
	  $(ICEDTEA_ENV) \
	  -C $(DEBUG_BUILD_OUTPUT_DIR) \
	  $(ICEDTEA_BUILD_TARGET)
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
	cp $(abs_top_builddir)/nss.cfg \
	  $(BUILD_DEBUG_SDK_DIR)/jre/lib/security;
if ENABLE_JAMVM
	printf -- '-jamvm ALIASED_TO -server\n' >> $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg
endif
	@echo "IcedTea (debug build) is served:" $(BUILD_DEBUG_SDK_DIR)
	mkdir -p stamps
	touch $@

clean-icedtea: 
	rm -rf $(BUILD_OUTPUT_DIR)
	rm -f stamps/icedtea-configure.stamp stamps/icedtea.stamp

clean-icedtea-debug:
	rm -rf $(DEBUG_BUILD_OUTPUT_DIR)
	rm -f stamps/icedtea-debug-configure.stamp
	rm -f stamps/icedtea-debug.stamp

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
	  if test "x$(INSTALL_ARCH_DIR)" != "xppc64" -a \
	  "x$(INSTALL_ARCH_DIR)" != "xppc64le" -a \
	  "x$(INSTALL_ARCH_DIR)" != "xarm" ; then \
	    $(BUILD_SDK_DIR)/bin/java -Xshare:dump ; \
	  fi ; \
	fi
endif
endif
endif
	touch stamps/add-archive.stamp

clean-add-archive:
	rm -vf $(BUILD_SDK_DIR)/jre/lib/$(INSTALL_ARCH_DIR)/*/*.jsa
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
	  if test "x$(INSTALL_ARCH_DIR)" != "xppc64" -a \
	  "x$(INSTALL_ARCH_DIR)" != "xppc64le" -a \
	  "x$(INSTALL_ARCH_DIR)" != "xarm" ; then \
	    $(BUILD_DEBUG_SDK_DIR)/bin/java -Xshare:dump ; \
	  fi ; \
	fi
endif
endif
endif
	touch stamps/add-archive-debug.stamp

clean-add-archive-debug:
	rm -vf $(BUILD_DEBUG_SDK_DIR)/jre/lib/$(INSTALL_ARCH_DIR)/*/*.jsa
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

stamps/icedtea-stage2.stamp: stamps/icedtea.stamp stamps/add-cacao.stamp \
 stamps/add-zero.stamp stamps/add-jamvm.stamp stamps/check-crypto.stamp \
 stamps/add-archive.stamp stamps/split-debuginfo.stamp
	mkdir -p stamps
	touch $@

clean-icedtea-stage2: clean-add-jamvm clean-check-crypto clean-add-archive \
 clean-split-debuginfo
	rm -f stamps/icedtea-stage2.stamp

stamps/icedtea-debug-stage2.stamp: stamps/icedtea-debug.stamp \
 stamps/add-cacao-debug.stamp stamps/add-zero-debug.stamp stamps/add-jamvm-debug.stamp \
 stamps/check-crypto-debug.stamp stamps/add-archive-debug.stamp stamps/split-debuginfo-debug.stamp
	mkdir -p stamps
	touch $@

clean-icedtea-debug-stage2: clean-add-jamvm-debug clean-check-crypto-debug \
 clean-add-archive-debug clean-split-debuginfo-debug
	rm -f stamps/icedtea-debug-stage2.stamp

# OpenJDK boot Targets
# ===================

stamps/icedtea-boot-configure.stamp: stamps/bootstrap-directory-symlink-stage1.stamp \
 stamps/download.stamp $(OPENJDK_BOOT_TREE) stamps/cacao.stamp stamps/jamvm.stamp
	mkdir -p $(BOOT_BUILD_OUTPUT_DIR)
	cd $(BOOT_BUILD_OUTPUT_DIR) && \
          $(ICEDTEA_CONFIGURE_ENV) $(SHELL) $(abs_top_builddir)/openjdk-boot/configure \
          $(ICEDTEA_CONFIGURE_BOOT)
	mkdir -p stamps
	touch $@

stamps/icedtea-boot.stamp: stamps/icedtea-boot-configure.stamp
	$(ICEDTEA_UNSET) $(ARCH_PREFIX) $(ICEDTEA_MAKE) \
	  $(ICEDTEA_ENV_BOOT) \
	  -C $(BOOT_BUILD_OUTPUT_DIR) \
	  images
	$(abs_top_builddir)/pax-mark-vm $(BUILD_BOOT_SDK_DIR) true
	@echo "Bootstrapped IcedTea is served:" $(BUILD_BOOT_SDK_DIR)
	mkdir -p stamps
	touch $@

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
	cp $(BOOT_BUILD_OUTPUT_DIR)/$(SERVER_DIR)/libjvm.so \
	   $(BUILD_SDK_DIR)/jre/lib/$(INSTALL_ARCH_DIR)/server
	@echo "HotSpot is served:" $(BUILD_SDK_DIR)
endif

clean-icedtea-boot:
	rm -rf $(BOOT_BUILD_OUTPUT_DIR)
	rm -f stamps/icedtea-boot-configure.stamp
	rm -f stamps/icedtea-boot.stamp

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
	  if test "x$(INSTALL_ARCH_DIR)" != "xppc64" -a \
	  "x$(INSTALL_ARCH_DIR)" != "xppc64le" -a \
	  "x$(INSTALL_ARCH_DIR)" != "xarm" ; then \
	    $(BUILD_BOOT_SDK_DIR)/bin/java -Xshare:dump ; \
	  fi ; \
	fi
endif
endif
endif
	touch $@

clean-add-archive-boot:
	rm -vf $(BUILD_BOOT_SDK_DIR)/jre/lib/$(INSTALL_ARCH_DIR)/*/*.jsa
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

stamps/icedtea-stage1.stamp: stamps/icedtea-boot.stamp \
 stamps/check-crypto-boot.stamp stamps/add-archive-boot.stamp \
 stamps/split-debuginfo-boot.stamp
	mkdir -p stamps
	touch $@

clean-icedtea-stage1: clean-check-crypto-boot clean-add-archive-boot \
 clean-split-debuginfo-boot
	rm -f stamps/icedtea-stage1.stamp

# JamVM

stamps/jamvm.stamp: stamps/jamvm-configure.stamp stamps/jamvm-build.stamp stamps/jamvm-install.stamp
	mkdir -p stamps
	touch stamps/jamvm.stamp

stamps/jamvm-configure.stamp: $(OPENJDK_TREE)
if BUILD_JAMVM
	cd jamvm/jamvm && \
	./autogen.sh --with-java-runtime-library=openjdk8 \
	  --prefix=$(abs_top_builddir)/jamvm/install
endif
	mkdir -p stamps
	touch $@

stamps/jamvm-build.stamp: stamps/jamvm-configure.stamp
if BUILD_JAMVM
	cd jamvm/jamvm && \
	$(ARCH_PREFIX) $(MAKE) -j$(PARALLEL_JOBS)
endif
	mkdir -p stamps
	touch $@

stamps/jamvm-install.stamp: stamps/jamvm-build.stamp
if BUILD_JAMVM
	cd jamvm/jamvm && \
	$(ARCH_PREFIX) $(MAKE) -j$(PARALLEL_JOBS) install
	mkdir -p $(abs_top_builddir)/jamvm/install/hotspot/lib
	touch $(abs_top_builddir)/jamvm/install/hotspot/lib/sa-jdi.jar
	mkdir -p $(abs_top_builddir)/jamvm/install/hotspot/jre/lib/$(INSTALL_ARCH_DIR)/server
	cp $(abs_top_builddir)/jamvm/install/lib/libjvm.so \
	  $(abs_top_builddir)/jamvm/install/hotspot/jre/lib/$(INSTALL_ARCH_DIR)/server
	ln -sf server $(abs_top_builddir)/jamvm/install/hotspot/jre/lib/$(INSTALL_ARCH_DIR)/client
	touch $(abs_top_builddir)/jamvm/install/hotspot/jre/lib/$(INSTALL_ARCH_DIR)/server/Xusage.txt
	ln -sf libjvm.so $(abs_top_builddir)/jamvm/install/hotspot/jre/lib/$(INSTALL_ARCH_DIR)/client/libjsig.so
endif
	mkdir -p stamps
	touch $@

clean-jamvm:
	rm -rf jamvm
	rm -f stamps/jamvm-configure.stamp stamps/jamvm-build.stamp stamps/jamvm-install.stamp
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

stamps/cacao.stamp: stamps/cacao-configure.stamp stamps/cacao-build.stamp stamps/cacao-install.stamp
	mkdir -p stamps
	touch $@

stamps/cacao-configure.stamp: $(OPENJDK_TREE)
if BUILD_CACAO
if !USE_SYSTEM_CACAO
	cd cacao/cacao && \
	./autogen.sh && \
	mkdir ../cacao.build && \
	cd ../cacao.build && \
	$(ARCH_PREFIX) ../cacao/configure \
	  --host=$(host_alias) \
	  --build=$(build_alias) \
	  --target=$(target_alias) \
	  --prefix=$(abs_top_builddir)/cacao/install \
	  --with-java-runtime-library=openjdk8 \
	  --with-java-runtime-library-prefix=$(abs_top_builddir)/openjdk \
	  --with-java-runtime-library-classes=$(RUNTIME) \
	  --with-libatomic-ops=no \
	  --enable-jre-layout $(CACAO_CONFIGURE_ARGS);
endif
endif
	mkdir -p stamps
	touch $@

stamps/cacao-build.stamp: stamps/cacao-configure.stamp
if BUILD_CACAO
if !USE_SYSTEM_CACAO
	cd cacao/cacao.build && \
	$(ARCH_PREFIX) $(MAKE) -j$(PARALLEL_JOBS)
endif
endif
	mkdir -p stamps
	touch $@

stamps/cacao-install.stamp: stamps/cacao-build.stamp
if BUILD_CACAO
if !USE_SYSTEM_CACAO
	cd cacao/cacao.build && \
	$(ARCH_PREFIX) $(MAKE) -j$(PARALLEL_JOBS) install
	mkdir -p $(abs_top_builddir)/cacao/install/hotspot/lib
	touch $(abs_top_builddir)/cacao/install/hotspot/lib/sa-jdi.jar
	mkdir -p $(abs_top_builddir)/cacao/install/hotspot/jre/lib/$(INSTALL_ARCH_DIR)/server
	cp $(abs_top_builddir)/cacao/install/lib/libjvm.so \
	  $(abs_top_builddir)/cacao/install/hotspot/jre/lib/$(INSTALL_ARCH_DIR)/server
	ln -sf server $(abs_top_builddir)/cacao/install/hotspot/jre/lib/$(INSTALL_ARCH_DIR)/client
	touch $(abs_top_builddir)/cacao/install/hotspot/jre/lib/$(INSTALL_ARCH_DIR)/server/Xusage.txt
	ln -sf libjvm.so $(abs_top_builddir)/cacao/install/hotspot/jre/lib/$(INSTALL_ARCH_DIR)/client/libjsig.so
endif
endif
	mkdir -p stamps
	touch $@

clean-cacao:
	rm -rf cacao/cacao.build cacao/install
	rm -f stamps/cacao-configure.stamp stamps/cacao-build.stamp stamps/cacao-install.stamp
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
	  $(if $(findstring --with-$(i)-src-zip=, $(CONFIGURE_ARGS)),, --with-$(i)-src-zip=$(abs_top_builddir)/$(i).tar.$(TAR_SUFFIX)))

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
		$(ARCH_PREFIX) $(MAKE) -C zerovm ICEDTEA_BUILD_TARGET=hotspot icedtea-stage2

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

# jtreg

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
	  -B $(BUILD_IMAGE_DIR) -A $(BUILD_ARCH_DIR) \
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
	  -B $(BUILD_IMAGE_DIR) -A $(BUILD_ARCH_DIR) \
	  -S $(abs_top_srcdir)/test/tapset \
	  -a test/check-stap.log -j
endif

check-tapset: check-tapset-probes check-tapset-jstack

clean-tapset-report:
if ENABLE_SYSTEMTAP
	rm -f test/check-stap.log
endif

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
	cat $< | $(SED) "s#_BINDIR_#$(bindir)#" > $@

clean-policytool-@JAVA_VER@.desktop:
	rm -f policytool-@JAVA_VER@.desktop

jconsole-@JAVA_VER@.desktop: jconsole.desktop
	cat $< | $(SED) "s#_BINDIR_#$(bindir)#" > $@

clean-jconsole-@JAVA_VER@.desktop:
	rm -f jconsole-@JAVA_VER@.desktop

# Installation Targets
# ====================

install-exec-local:
	${mkinstalldirs} $(DESTDIR)$(bindir) $(DESTDIR)$(includedir) $(DESTDIR)${exec_prefix}/lib
	${mkinstalldirs} $(DESTDIR)$(includedir)/$(BUILD_OS_DIR) $(DESTDIR)${exec_prefix}/lib/$(INSTALL_ARCH_DIR)
	${mkinstalldirs} $(DESTDIR)${exec_prefix}/lib/$(INSTALL_ARCH_DIR)/jli
	${mkinstalldirs} $(DESTDIR)${prefix}/jre/bin $(DESTDIR)${prefix}/jre/lib
	${mkinstalldirs} $(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)
	${mkinstalldirs} $(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/jli
	${mkinstalldirs} $(DESTDIR)${prefix}/jre/lib/ext
	${mkinstalldirs} $(DESTDIR)${prefix}/tapset
if SPLIT_DEBUGINFO
	${mkinstalldirs} $(DESTDIR)$(DEBUG_PREFIX)/$(bindir) $(DESTDIR)$(DEBUG_PREFIX)/${exec_prefix}/lib
	${mkinstalldirs} $(DESTDIR)$(DEBUG_PREFIX)/${exec_prefix}/lib/$(INSTALL_ARCH_DIR)
	${mkinstalldirs} $(DESTDIR)$(DEBUG_PREFIX)/${exec_prefix}/lib/$(INSTALL_ARCH_DIR)/jli
	${mkinstalldirs} $(DESTDIR)$(DEBUG_PREFIX)/${prefix}/jre/bin $(DESTDIR)${prefix}/jre/lib
	${mkinstalldirs} $(DESTDIR)$(DEBUG_PREFIX)/${prefix}/jre/lib/$(INSTALL_ARCH_DIR)
	${mkinstalldirs} $(DESTDIR)$(DEBUG_PREFIX)/${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/jli
endif
	for files in $(BUILD_SDK_DIR)/bin/*; do \
	  name=$$(basename $${files}) ; \
	  if test "x$(enable_split_debuginfo)" = "xyes" -a ! -h $${files} -a "x$${name}" != "xjava-rmi.cgi" ; then \
	    dir=$$(echo $$(dirname $${files}) | $(SED) "s#$(BUILD_SDK_DIR)#$(BUILD_DEBUGINFO_DIR)#") ; \
	    $(INSTALL_PROGRAM) $${dir}/$${name}.stripped $(DESTDIR)$(bindir)/$${name}; \
	    $(INSTALL_PROGRAM) $${dir}/$${name}.debug $(DESTDIR)$(DEBUG_PREFIX)/$(bindir); \
	  else \
	    $(INSTALL_PROGRAM) $${files} $(DESTDIR)$(bindir); \
	  fi ; \
	done
	for files in $(BUILD_SDK_DIR)/include/*.h; do \
	  $(INSTALL_DATA) $${files} $(DESTDIR)$(includedir); \
	done
	for files in $(BUILD_SDK_DIR)/include/$(BUILD_OS_DIR)/*.h; do \
	  $(INSTALL_DATA) $${files} $(DESTDIR)$(includedir)/$(BUILD_OS_DIR); \
	done
	for files in $(BUILD_SDK_DIR)/lib/*.jar; do \
	  $(INSTALL_DATA) $${files} $(DESTDIR)${exec_prefix}/lib; \
	done
	for files in $(BUILD_SDK_DIR)/lib/*.idl; do \
	  $(INSTALL_DATA) $${files} $(DESTDIR)${exec_prefix}/lib; \
	done
	if test "x$(enable_split_debuginfo)" = "xyes"; then \
	  $(INSTALL_PROGRAM) $(BUILD_DEBUGINFO_DIR)/lib/jexec.stripped $(DESTDIR)${exec_prefix}/lib/jexec ; \
	  $(INSTALL_PROGRAM) $(BUILD_DEBUGINFO_DIR)/lib/jexec.debug $(DESTDIR)$(DEBUG_PREFIX)/${exec_prefix}/lib ; \
	else \
	  $(INSTALL_PROGRAM) $(BUILD_SDK_DIR)/lib/jexec $(DESTDIR)${exec_prefix}/lib ; \
	fi
	for files in $(BUILD_SDK_DIR)/lib/$(INSTALL_ARCH_DIR)/*.so; do \
	  if test "x$(enable_split_debuginfo)" = "xyes" -a ! -h $${files}; then \
	    name=$$(basename $${files}) ; \
	    dir=$$(echo $$(dirname $${files}) | $(SED) "s#$(BUILD_SDK_DIR)#$(BUILD_DEBUGINFO_DIR)#") ; \
	    $(INSTALL_PROGRAM) $${dir}/$${name}.stripped $(DESTDIR)${exec_prefix}/lib/$(INSTALL_ARCH_DIR)/$${name}; \
	    $(INSTALL_PROGRAM) $${dir}/$${name}.debug $(DESTDIR)$(DEBUG_PREFIX)/${exec_prefix}/lib/$(INSTALL_ARCH_DIR); \
	  else \
	    $(INSTALL_PROGRAM) $${files} $(DESTDIR)${exec_prefix}/lib/$(INSTALL_ARCH_DIR); \
	  fi ; \
	done
	for files in $(BUILD_SDK_DIR)/lib/$(INSTALL_ARCH_DIR)/jli/*.so; do \
	  if test "x$(enable_split_debuginfo)" = "xyes" -a ! -h $${files}; then \
	    name=$$(basename $${files}) ; \
	    dir=$$(echo $$(dirname $${files}) | $(SED) "s#$(BUILD_SDK_DIR)#$(BUILD_DEBUGINFO_DIR)#") ; \
	    $(INSTALL_PROGRAM) $${dir}/$${name}.stripped $(DESTDIR)${exec_prefix}/lib/$(INSTALL_ARCH_DIR)/jli/$${name}; \
	    $(INSTALL_PROGRAM) $${dir}/$${name}.debug $(DESTDIR)$(DEBUG_PREFIX)/${exec_prefix}/lib/$(INSTALL_ARCH_DIR)/jli; \
	  else \
	    $(INSTALL_PROGRAM) $${files} $(DESTDIR)${exec_prefix}/lib/$(INSTALL_ARCH_DIR)/jli; \
	  fi ; \
	done
	for files in $(BUILD_SDK_DIR)/jre/bin/*; do \
	  if test "x$(enable_split_debuginfo)" = "xyes" -a ! -h $${files}; then \
	    name=$$(basename $${files}) ; \
	    dir=$$(echo $$(dirname $${files}) | $(SED) "s#$(BUILD_SDK_DIR)#$(BUILD_DEBUGINFO_DIR)#") ; \
	    $(INSTALL_PROGRAM) $${dir}/$${name}.stripped $(DESTDIR)${prefix}/jre/bin/$${name}; \
	    $(INSTALL_PROGRAM) $${dir}/$${name}.debug $(DESTDIR)$(DEBUG_PREFIX)/${prefix}/jre/bin; \
	  else \
	    $(INSTALL_PROGRAM) $${files} $(DESTDIR)${prefix}/jre/bin; \
	  fi ; \
	done
	for files in $(BUILD_SDK_DIR)/jre/lib/*.jar; do \
	  $(INSTALL_DATA) $${files} $(DESTDIR)${prefix}/jre/lib; \
	done
	if test "x$(enable_split_debuginfo)" = "xyes"; then \
	  $(INSTALL_PROGRAM) $(BUILD_DEBUGINFO_DIR)/jre/lib/jexec.stripped $(DESTDIR)${exec_prefix}/jre/lib/jexec ; \
	  $(INSTALL_PROGRAM) $(BUILD_DEBUGINFO_DIR)/jre/lib/jexec.debug $(DESTDIR)$(DEBUG_PREFIX)/${exec_prefix}/jre/lib ; \
	else \
	  $(INSTALL_PROGRAM) $(BUILD_SDK_DIR)/jre/lib/jexec $(DESTDIR)${exec_prefix}/jre/lib ; \
	fi
	for files in $(BUILD_SDK_DIR)/jre/lib/$(INSTALL_ARCH_DIR)/*.so; do \
	  if test "x$(enable_split_debuginfo)" = "xyes" -a ! -h $${files}; then \
	    name=$$(basename $${files}) ; \
	    dir=$$(echo $$(dirname $${files}) | $(SED) "s#$(BUILD_SDK_DIR)#$(BUILD_DEBUGINFO_DIR)#") ; \
	    $(INSTALL_PROGRAM) $${dir}/$${name}.stripped $(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/$${name}; \
	    $(INSTALL_PROGRAM) $${dir}/$${name}.debug $(DESTDIR)$(DEBUG_PREFIX)/${prefix}/jre/lib/$(INSTALL_ARCH_DIR); \
	  else \
	    $(INSTALL_PROGRAM) $${files} $(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR); \
	  fi ; \
	done
	for files in $(BUILD_SDK_DIR)/jre/lib/$(INSTALL_ARCH_DIR)/jli/*.so; do \
	  if test "x$(enable_split_debuginfo)" = "xyes" -a ! -h $${files} ; then \
	    name=$$(basename $${files}) ; \
	    dir=$$(echo $$(dirname $${files}) | $(SED) "s#$(BUILD_SDK_DIR)#$(BUILD_DEBUGINFO_DIR)#") ; \
	    $(INSTALL_PROGRAM) $${dir}/$${name}.stripped $(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/jli/$${name}; \
	    $(INSTALL_PROGRAM) $${dir}/$${name}.debug $(DESTDIR)$(DEBUG_PREFIX)/${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/jli; \
	  else \
	    $(INSTALL_PROGRAM) $${files} $(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/jli; \
	  fi ; \
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
	        $(INSTALL_PROGRAM) $${files} $(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/$${vms}; \
	      fi ; \
	    done ; \
	  fi ; \
	done
	for files in $(BUILD_SDK_DIR)/jre/lib/ext/*.jar; do \
	  $(INSTALL_DATA) $${files} $(DESTDIR)${prefix}/jre/lib/ext; \
	done
if ENABLE_SYSTEMTAP
	for files in $(BUILD_SDK_DIR)/tapset/*.stp; do \
	  $(INSTALL_DATA) $${files} $(DESTDIR)${prefix}/tapset; \
	done
endif
	$(abs_top_builddir)/pax-mark-vm $(DESTDIR)${prefix} false

install-data-local:
	${mkinstalldirs} $(DESTDIR)${prefix}
	${mkinstalldirs} $(DESTDIR)${prefix}/jre/.systemPrefs
	${mkinstalldirs} $(DESTDIR)${prefix}/jre/lib
	${mkinstalldirs} $(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)
	${mkinstalldirs} $(DESTDIR)${prefix}/jre/lib/{cmm,ext,images/cursors,management,security}
	${mkinstalldirs} $(DESTDIR)$(mandir)/{man1,ja_JP.UTF-8/man1}
	${mkinstalldirs} $(DESTDIR)${prefix}/demo
	${mkinstalldirs} $(DESTDIR)${prefix}/sample
	${mkinstalldirs} $(DESTDIR)$(docdir)
	${mkinstalldirs} $(DESTDIR)$(datadir)/applications
	${mkinstalldirs} $(DESTDIR)$(sysconfdir)/.java/.systemPrefs
	for files in $(BUILD_SDK_DIR)/jre/lib/*.properties; do \
	  $(INSTALL_DATA) $${files} $(DESTDIR)${prefix}/jre/lib; \
	done
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/release $(DESTDIR)${prefix}
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/src.zip $(DESTDIR)${prefix}
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/jre/lib/classlist $(DESTDIR)${prefix}/jre/lib
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/jre/lib/currency.data $(DESTDIR)${prefix}/jre/lib
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/jre/lib/jvm.hprof.txt $(DESTDIR)${prefix}/jre/lib
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/jre/lib/meta-index $(DESTDIR)${prefix}/jre/lib
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/jre/lib/psfont.properties.ja $(DESTDIR)${prefix}/jre/lib
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/jre/lib/tzdb.dat $(DESTDIR)${prefix}/jre/lib
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
	  $(INSTALL_DATA) $${files} $(DESTDIR)${prefix}/jre/lib/cmm; \
	done
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/jre/lib/ext/meta-index $(DESTDIR)${prefix}/jre/lib/ext
	for files in $(BUILD_SDK_DIR)/jre/lib/images/cursors/*.gif; do \
	  $(INSTALL_DATA) $${files} $(DESTDIR)${prefix}/jre/lib/images/cursors; \
	done
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/jre/lib/images/cursors/cursors.properties \
	  $(DESTDIR)${prefix}/jre/lib/images/cursors
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/jre/lib/management/jmxremote.access \
	  $(DESTDIR)${prefix}/jre/lib/management
	$(INSTALL_DATA) $(BUILD_SDK_DIR)/jre/lib/management/management.properties \
	  $(DESTDIR)${prefix}/jre/lib/management
	for files in $(BUILD_SDK_DIR)/jre/lib/management/*.template; do \
	  $(INSTALL_DATA) $${files} $(DESTDIR)${prefix}/jre/lib/management; \
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
	for files in $(BUILD_SDK_DIR)/jre/lib/security/*.jar; do \
	  $(INSTALL_DATA) $${files} $(DESTDIR)${prefix}/jre/lib/security; \
	done
	for files in $(BUILD_SDK_DIR)/man/man1/*.1; do \
	  $(INSTALL_DATA) $${files} $(DESTDIR)$(mandir)/man1; \
	done
	for files in $(BUILD_SDK_DIR)/man/ja_JP.UTF-8/man1/*.1; do \
	  $(INSTALL_DATA) $${files} $(DESTDIR)$(mandir)/ja_JP.UTF-8/man1; \
	done
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
	    $(DESTDIR)$(datadir)/icons/hicolor/$${sizes}x$${sizes}/apps/java-@JAVA_VER@.png ; \
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

uninstall-local:
	for files in $(BUILD_SDK_DIR)/bin/*; do \
	  name=$$(basename $${files}) ; \
	  rm -vf $(DESTDIR)$(bindir)/$${name}; \
	  rm -vf $(DESTDIR)$(DEBUG_PREFIX)/$(bindir)/$${name}.debug; \
	done
	for files in $(BUILD_SDK_DIR)/include/*.h; do \
	  rm -vf $(DESTDIR)$(includedir)/$$(basename $${files}); \
	done
	for files in $(BUILD_SDK_DIR)/include/$(BUILD_OS_DIR)/*.h; do \
	  rm -vf $(DESTDIR)$(includedir)/$(BUILD_OS_DIR)/$$(basename $${files}); \
	done
	for files in $(BUILD_SDK_DIR)/lib/*.jar; do \
	  rm -vf $(DESTDIR)${exec_prefix}/lib/$$(basename $${files}); \
	done
	for files in $(BUILD_SDK_DIR)/lib/*.idl; do \
	  rm -vf $(DESTDIR)${exec_prefix}/lib/$$(basename $${files}); \
	done
	rm -vf $(DESTDIR)${exec_prefix}/lib/jexec
	rm -vf $(DESTDIR)$(DEBUG_PREFIX)/${exec_prefix}/lib/jexec.debug
	for files in $(BUILD_SDK_DIR)/lib/$(INSTALL_ARCH_DIR)/*.so; do \
	  name=$$(basename $${files}) ; \
	  rm -vf $(DESTDIR)${exec_prefix}/lib/$(INSTALL_ARCH_DIR)/$${name}; \
	  rm -vf $(DESTDIR)$(DEBUG_PREFIX)/${exec_prefix}/lib/$(INSTALL_ARCH_DIR)/$${name}.debug; \
	done
	for files in $(BUILD_SDK_DIR)/lib/$(INSTALL_ARCH_DIR)/jli/*.so; do \
	  name=$$(basename $${files}) ; \
	  rm -vf $(DESTDIR)${exec_prefix}/lib/$(INSTALL_ARCH_DIR)/jli/$${name}; \
	  rm -vf $(DESTDIR)$(DEBUG_PREFIX)/${exec_prefix}/lib/$(INSTALL_ARCH_DIR)/jli/$${name}.debug; \
	done
	for files in $(BUILD_SDK_DIR)/jre/bin/*; do \
	  name=$$(basename $${files}) ; \
	  rm -vf $(DESTDIR)${prefix}/jre/bin/$${name}; \
	  rm -vf $(DESTDIR)$(DEBUG_PREFIX)/${prefix}/jre/bin/$${name}.debug; \
	done
	for files in $(BUILD_SDK_DIR)/jre/lib/*.jar; do \
	  rm -vf $(DESTDIR)${prefix}/jre/lib/$$(basename $${files}); \
	done
	rm -vf $(DESTDIR)${prefix}/jre/lib/jexec
	rm -vf $(DESTDIR)$(DEBUG_PREFIX)/${prefix}/jre/lib/jexec.debug
	for files in $(BUILD_SDK_DIR)/jre/lib/$(INSTALL_ARCH_DIR)/*.so; do \
	  name=$$(basename $${files}) ; \
	  rm -vf $(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/$${name}; \
	  rm -vf $(DESTDIR)$(DEBUG_PREFIX)/${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/$${name}.debug; \
	done
	for files in $(BUILD_SDK_DIR)/jre/lib/$(INSTALL_ARCH_DIR)/jli/*.so; do \
	  name=$$(basename $${files}) ; \
	  rm -vf $(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/jli/$${name}; \
	  rm -vf $(DESTDIR)$(DEBUG_PREFIX)/${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/jli/$${name}.debug; \
	done
	for vms in client server ; do \
	  for files in $(BUILD_SDK_DIR)/jre/lib/$(INSTALL_ARCH_DIR)/$${vms}/*.so; do \
	    name=$$(basename $${files}) ; \
	    rm -vf $(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/$${vms}/$${name}; \
	    rm -vf $(DESTDIR)$(DEBUG_PREFIX)/${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/$${vms}/$${name}.debug; \
	  done ; \
	done
	for files in $(BUILD_SDK_DIR)/jre/lib/ext/*.jar; do \
	  rm -vf $(DESTDIR)${prefix}/jre/lib/ext/$$(basename $${files}); \
	done
	for files in $(BUILD_SDK_DIR)/tapset/*.stp; do \
	  rm -vf $(DESTDIR)${prefix}/tapset/$$(basename $${files}); \
	done
	for files in $(BUILD_SDK_DIR)/jre/lib/*.properties; do \
	  rm -vf $(DESTDIR)${prefix}/jre/lib/$$(basename $${files}); \
	done
	rm -vf $(DESTDIR)${prefix}/release
	rm -vf $(DESTDIR)${prefix}/src.zip
	rm -vf $(DESTDIR)${prefix}/jre/lib/classlist
	rm -vf $(DESTDIR)${prefix}/jre/lib/currency.data
	rm -vf $(DESTDIR)${prefix}/jre/lib/jvm.hprof.txt
	rm -vf $(DESTDIR)${prefix}/jre/lib/meta-index
	rm -vf $(DESTDIR)${prefix}/jre/lib/psfont.properties.ja
	rm -vf $(DESTDIR)${prefix}/jre/lib/tzdb.dat
	rm -vf $(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/jvm.cfg
	for vms in client server ; do \
	  rm -vf $(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/$${vms}/Xusage.txt; \
	done
	for files in $(BUILD_SDK_DIR)/jre/lib/cmm/*.pf; do \
	  rm -vf $(DESTDIR)/${prefix}/jre/lib/cmm/$$(basename $${files}); \
	done
	rm -vf $(DESTDIR)${prefix}/jre/lib/ext/meta-index
	for files in $(BUILD_SDK_DIR)/jre/lib/images/cursors/*.gif; do \
	  rm -vf $(DESTDIR)${prefix}/jre/lib/images/cursors/$$(basename $${files}); \
	done
	rm -vf $(DESTDIR)${prefix}/jre/lib/images/cursors/cursors.properties
	rm -vf $(DESTDIR)${prefix}/jre/lib/management/jmxremote.access
	rm -vf $(DESTDIR)${prefix}/jre/lib/management/management.properties
	for files in $(BUILD_SDK_DIR)/jre/lib/management/*.template; do \
	  rm -vf $(DESTDIR)${prefix}/jre/lib/management/$$(basename $${files}); \
	done
	rm -vf $(DESTDIR)${prefix}/jre/lib/security/cacerts
	rm -vf $(DESTDIR)${prefix}/jre/lib/security/java.policy
	rm -vf $(DESTDIR)${prefix}/jre/lib/security/java.security
	rm -vf $(DESTDIR)${prefix}/jre/lib/security/nss.cfg
	rm -vf $(DESTDIR)${prefix}/jre/lib/security/blacklisted.certs
	for files in $(BUILD_SDK_DIR)/jre/lib/security/*.jar; do \
	  rm -vf $(DESTDIR)${prefix}/jre/lib/security/$$(basename $${files}); \
	done
	for files in $(BUILD_SDK_DIR)/man/man1/*.1; do \
	  rm -vf $(DESTDIR)$(mandir)/man1/$$(basename $${files}); \
	done
	for files in $(BUILD_SDK_DIR)/man/ja_JP.UTF-8/man1/*.1; do \
	  rm -vf $(DESTDIR)$(mandir)/ja_JP.UTF-8/man1/$$(basename $${files}); \
	done
	for items in $(BUILD_SDK_DIR)/demo/*; do \
	  rm -rvf $(DESTDIR)${prefix}/demo/$$(basename $${items}); \
	done
	for items in $(BUILD_SDK_DIR)/sample/*; do \
	  rm -rvf $(DESTDIR)${prefix}/sample/$$(basename $${items}); \
	done
	rm -rf $(DESTDIR)$(docdir)/README
	rm -rf $(DESTDIR)$(docdir)/NEWS
	rm -rf $(DESTDIR)$(docdir)/AUTHORS
	rm -rf $(DESTDIR)$(docdir)/HACKING
	rm -rf $(DESTDIR)$(docdir)/COPYING
	rm -rf $(DESTDIR)$(docdir)/ChangeLog
	rm -rf $(DESTDIR)$(docdir)/ASSEMBLY_EXCEPTION
	rm -rf $(DESTDIR)$(docdir)/LICENSE
	rm -rf $(DESTDIR)$(docdir)/THIRD_PARTY_README
	rm -rf $(DESTDIR)$(docdir)/TRADEMARK
	for sizes in 16 24 32 48 ; do \
	  rm -vf $(DESTDIR)$(datadir)/icons/hicolor/$${sizes}x$${sizes}/apps/java-@JAVA_VER@.png; \
	done
	for files in $(DESKTOP_FILES) ; do \
	  rm -vf $(DESTDIR)$(datadir)/applications/$$(basename $${files}); \
	done
	rm -vf $(DESTDIR)${prefix}/jre/.systemPrefs/.system.lock
	rm -vf $(DESTDIR)${prefix}/jre/.systemPrefs/.systemRootModFile
if ENABLE_DOCS
	for items in $(BUILD_OUTPUT_DIR)/docs/*; do \
	  rm -rvf $(DESTDIR)$(htmldir)/$$(basename $${items}); \
	done
endif
	for vms in client server ; do \
	  rm -vf $(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/$${vms}/classes.jsa; \
	  if [ -d $(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/$${vms} ] ; then \
	    $(RMDIR) $(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/$${vms} ; \
	  fi ; \
	done
	$(RMDIR) $(DESTDIR)${exec_prefix}/lib/$(INSTALL_ARCH_DIR)/jli
	$(RMDIR) $(DESTDIR)$(includedir)/$(BUILD_OS_DIR) $(DESTDIR)${exec_prefix}/lib/$(INSTALL_ARCH_DIR)
	$(RMDIR) $(DESTDIR)$(bindir) $(DESTDIR)$(includedir) $(DESTDIR)${exec_prefix}/lib
	$(RMDIR) $(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)/jli
	$(RMDIR) $(DESTDIR)${prefix}/jre/lib/$(INSTALL_ARCH_DIR)
	$(RMDIR) $(DESTDIR)${prefix}/jre/lib/{cmm,ext,images/cursors,management,security}
	$(RMDIR) $(DESTDIR)${prefix}/jre/lib/images
	$(RMDIR) $(DESTDIR)${prefix}/jre/bin $(DESTDIR)${prefix}/jre/lib
	$(RMDIR) $(DESTDIR)${prefix}/jre/.systemPrefs
	$(RMDIR) $(DESTDIR)${prefix}/jre
	$(RMDIR) $(DESTDIR)${prefix}/tapset
	$(RMDIR) $(DESTDIR)${prefix}/demo
	$(RMDIR) $(DESTDIR)${prefix}/sample
	$(RMDIR) $(DESTDIR)$(docdir)
	$(RMDIR) $(DESTDIR)$(datadir)/applications
	$(RMDIR) $(DESTDIR)$(sysconfdir)/.java/.systemPrefs
	$(RMDIR) $(DESTDIR)$(mandir)/{man1,ja_JP.UTF-8/man1}
	for sizes in 16 24 32 48 ; do \
	  $(RMDIR) $(DESTDIR)$(datadir)/icons/hicolor/$${sizes}x$${sizes}/apps ; \
	done
if ENABLE_DOCS
# May have already been removed if docdir == htmldir
	if [ -d $(DESTDIR)$(htmldir) ] ; then \
	  $(RMDIR) $(DESTDIR)$(htmldir) ; \
	fi
endif

# Target Aliases
# ===============

add-archive: stamps/add-archive.stamp

add-archive-boot: stamps/add-archive-ecj.stamp

add-archive-debug: stamps/add-archive-debug.stamp

add-cacao: stamps/add-cacao.stamp

add-cacao-debug: stamps/add-cacao-debug.stamp

add-jamvm: stamps/add-jamvm.stamp

add-jamvm-debug: stamps/add-jamvm-debug.stamp

add-zero: stamps/add-zero.stamp

add-zero-debug: stamps/add-zero-debug.stamp

bootstrap-directory-stage1: stamps/bootstrap-directory-stage1.stamp

bootstrap-directory-stage2: stamps/bootstrap-directory-stage2.stamp

bootstrap-directory-symlink-stage1: stamps/bootstrap-directory-symlink-stage1.stamp

bootstrap-directory-symlink-stage2: stamps/bootstrap-directory-symlink-stage2.stamp

cacao: stamps/cacao.stamp

cacao-build: stamps/cacao-build.stamp

cacao-configure: stamps/cacao-configure.stamp

cacao-install: stamps/cacao-install.stamp

check-crypto: stamps/check-crypto.stamp

check-crypto-boot: stamps/check-crypto-boot.stamp

check-crypto-debug: stamps/check-crypto-debug.stamp

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

download-nashorn: stamps/download-nashorn.stamp

download-openjdk: stamps/download-openjdk.stamp

ecccheck: stamps/ecccheck.stamp

extract: stamps/extract.stamp

extract-cacao: stamps/extract-cacao.stamp

extract-corba: stamps/extract-corba.stamp

extract-hotspot: stamps/extract-hotspot.stamp

extract-jamvm: stamps/extract-jamvm.stamp

extract-jaxp: stamps/extract-jaxp.stamp

extract-jaxws: stamps/extract-jaxws.stamp

extract-jdk: stamps/extract-jdk.stamp

extract-langtools: stamps/extract-langtools.stamp

extract-nashorn: stamps/extract-nashorn.stamp

extract-openjdk: stamps/extract-openjdk.stamp

icedtea-configure: stamps/icedtea-configure.stamp

icedtea: stamps/icedtea.stamp

icedtea-boot-configure: stamps/icedtea-boot-configure.stamp

icedtea-boot: stamps/icedtea-boot.stamp

icedtea-debug-configure: stamps/icedtea-debug-configure.stamp

icedtea-debug: stamps/icedtea-debug.stamp

icedtea-debug-stage2: stamps/icedtea-debug-stage2.stamp

icedtea-stage1: stamps/icedtea-stage1.stamp

icedtea-stage2: stamps/icedtea-stage2.stamp

jamvm: stamps/jamvm.stamp

jamvm-build: stamps/jamvm-build.stamp

jamvm-configure: stamps/jamvm-configure.stamp

jamvm-install: stamps/jamvm-install.stamp

jtreg: stamps/jtreg.stamp

overlay: stamps/overlay.stamp

patch: stamps/patch.stamp

patch-boot: stamps/patch-boot.stamp

patch-fsg: stamps/patch-fsg.stamp

remove-intree-libraries: stamps/remove-intree-libraries.stamp

sanitise-openjdk: stamps/sanitise-openjdk.stamp

versioning: stamps/versioning.stamp
