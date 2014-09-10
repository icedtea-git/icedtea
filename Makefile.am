# Dependencies

JDK_UPDATE_VERSION = 20
BUILD_VERSION = b23
COMBINED_VERSION = $(JDK_UPDATE_VERSION)-$(BUILD_VERSION)

CORBA_CHANGESET = 83ebbcc0dda5
JAXP_CHANGESET = 888f90c5e7da
JAXWS_CHANGESET = 9be5317def51
JDK_CHANGESET = 03f9102db2c0
LANGTOOLS_CHANGESET = 948daf9c5e22
OPENJDK_CHANGESET = a81a301b0f89
NASHORN_CHANGESET = 2a866ca13bc6

CORBA_SHA256SUM = 023aea7e793e57404380318fc827250dc5c711219dd85bc90fdf46a41497f1b6
JAXP_SHA256SUM = 6436cce532e70421483307aafcc6a9eb6b607812617fefba668c321a6190d4c5
JAXWS_SHA256SUM = 2f696e725176e8b24dad07eb7ab5442ee4601b40ed115c3cabae987ce49a48c3
JDK_SHA256SUM = 88d4b37120269e5e31f8de22660a29e09bfa07beda8b37cc79b32d00b10d19aa
LANGTOOLS_SHA256SUM = 9f9457e40f1e354e305aca01108285d5714cd59c2ab4e84ac4edd2ab418e5ae9
OPENJDK_SHA256SUM = a2d87a9a688ed97b91483a12c9fbd12669f12ea297595788a75b15891267dd11
NASHORN_SHA256SUM = a5053e8698569821278786efd816fed4c4264ad5017f6a95d6ff735331d30116

HS_TYPE = "`$(AWK) 'version==$$1 {print $$2}' version=$(HSBUILD) $(abs_top_srcdir)/hotspot.map`"
HS_URL = "`$(AWK) 'version==$$1 {print $$3}' version=$(HSBUILD) $(abs_top_srcdir)/hotspot.map`"
HS_CHANGESET = "`$(AWK) 'version==$$1 {print $$4}' version=$(HSBUILD) $(abs_top_srcdir)/hotspot.map`"
HS_SHA256SUM = "`$(AWK) 'version==$$1 {print $$5}' version=$(HSBUILD) $(abs_top_srcdir)/hotspot.map`"

DROP_URL = http://icedtea.classpath.org/download/drops

CACAO_VERSION = e215e36be9fc
CACAO_SHA256SUM = 4966514c72ee7ed108b882d9b6e65c3adf8a8f9c2dccb029f971b3c8cb4870ab
CACAO_BASE_URL = $(DROP_URL)/cacao
CACAO_URL = $(CACAO_BASE_URL)/$(CACAO_VERSION).tar.gz
CACAO_SRC_ZIP = cacao-$(CACAO_VERSION).tar.bz2

JAMVM_VERSION = ec18fb9e49e62dce16c5094ef1527eed619463aa
JAMVM_SHA256SUM = 31810266666c23822942aac62a78019c2c4589e1c5ee48329cbf42652d4437bc
JAMVM_BASE_URL = $(DROP_URL)/jamvm
JAMVM_URL = $(JAMVM_BASE_URL)/jamvm-$(JAMVM_VERSION).tar.gz
JAMVM_SRC_ZIP = jamvm-$(JAMVM_VERSION).tar.gz

ICEDTEA_MAJOR = icedtea8
ICEDTEA_PREFIX = $(ICEDTEA_MAJOR)-forest
ICEDTEA_HG_URL = http://icedtea.classpath.org/hg/$(ICEDTEA_PREFIX)
ICEDTEA_URL = $(DROP_URL)/$(ICEDTEA_MAJOR)

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
BUILD_JRE_ARCH_DIR = $(BUILD_IMAGE_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
BUILD_DEBUG_IMAGE_DIR = $(DEBUG_BUILD_OUTPUT_DIR)/images
BUILD_DEBUG_JRE_ARCH_DIR = $(BUILD_DEBUG_IMAGE_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
BUILD_BOOT_IMAGE_DIR = $(BOOT_BUILD_OUTPUT_DIR)/images
REWRITER_BUILD_DIR = $(abs_top_builddir)/rewriter.build
STAGE1_BOOT_DIR = $(abs_top_builddir)/bootstrap/boot
STAGE2_BOOT_DIR = $(abs_top_builddir)/bootstrap/icedtea
JAMVM_IMPORT_PATH = $(abs_top_builddir)/jamvm/install/hotspot

# Source directories

SHARE = openjdk-boot/jdk/src/share/classes
SOLARIS = openjdk-boot/jdk/src/solaris/classes
LANGTOOLS = openjdk-boot/langtools/src/share/classes
CORBA = openjdk-boot/corba/src/share/classes
JAXWS = openjdk-boot/jaxws/src/share/jaxws_classes

PULSE_JAVA_DIR = $(abs_top_srcdir)/pulseaudio
PULSE_JAVA_NATIVE_SRCDIR = $(PULSE_JAVA_DIR)/src/native
PULSE_JAVA_NATIVE_SRCS = $(wildcard $(PULSE_JAVA_NATIVE_SRCDIR)/*.c)
PULSE_JAVA_BUILDDIR = $(abs_top_builddir)/pulseaudio.build
PULSE_JAVA_NATIVE_BUILDDIR = $(PULSE_JAVA_BUILDDIR)/native
PULSE_JAVA_NATIVE_OBJECTS = \
	$(subst $(PULSE_JAVA_NATIVE_SRCDIR),$(PULSE_JAVA_NATIVE_BUILDDIR),$(patsubst %.c,%.o,$(PULSE_JAVA_NATIVE_SRCS)))
PULSE_JAVA_JAVA_SRCDIR = $(PULSE_JAVA_DIR)/src/java
PULSE_JAVA_CLASS_DIR = $(PULSE_JAVA_BUILDDIR)/classes

OPENJDK_SOURCEPATH_DIRS = \
        $(SHARE):$(SOLARIS):$(LANGTOOLS):$(CORBA):$(JAXWS)

SOURCEPATH_DIRS = $(OPENJDK_SOURCEPATH_DIRS)

VERSION_SETTINGS_FILE = openjdk/jdk/make/ProfileNames.gmk

# Sources used from OpenJDK.
#PR43148 - javac fails due to missing java.util.regex.Matcher.quoteReplacement
#PR48033 - Missing javax.management.remote.JMXServiceURL
#PR48034 - javax.management.modelmbean.ModelMBeanInfo
#PR42003 - Missing javax.swing.plaf.basic.BasicDirectoryModel methods cause OpenJDK build failure 
ICEDTEA_BOOTSTRAP_CLASSES =

# Settings for javac

IT_LANGUAGE_SOURCE_VERSION=6
IT_CLASS_TARGET_VERSION=6
IT_JAVAC_SETTINGS=-g -encoding utf-8 $(JAVACFLAGS) $(MEMORY_LIMIT) $(PREFER_SOURCE)
IT_JAVACFLAGS=$(IT_JAVAC_SETTINGS) -source $(IT_LANGUAGE_SOURCE_VERSION) -target $(IT_CLASS_TARGET_VERSION)

# Flags
IT_CFLAGS=$(CFLAGS) $(ARCHFLAG)
ICEDTEA_MAKE=$(subst -j,JOBS=,$(MAKE))

# Conditional defintions

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
  OPENJDK_SRC_ZIP = openjdk.tar.xz
  CORBA_SRC_ZIP = corba.tar.xz
  JAXP_SRC_ZIP = jaxp.tar.xz
  JAXWS_SRC_ZIP = jaxws.tar.xz
  JDK_SRC_ZIP = jdk.tar.xz
  LANGTOOLS_SRC_ZIP = langtools.tar.xz
  HOTSPOT_SRC_ZIP = hotspot.tar.xz
  NASHORN_SRC_ZIP = nashorn.tar.xz
endif

if WITH_HGREV
REV_ARG = -r $(HGREV)
endif

if ENABLE_PULSE_JAVA
PULSE_JAVA_TARGET = stamps/pulse-java.stamp
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

# Target to ensure a patched OpenJDK tree containing Zero & Shark
# and any overlays is available in $(abs_top_builddir)/openjdk
OPENJDK_TREE = stamps/overlay.stamp

# Target to ensure a patched OpenJDK bootstrap tree containing
# Zero & Shark, any other overlays and the bootstrapping patches
# is available in $(abs_top_builddir)/openjdk-boot
OPENJDK_BOOT_TREE = stamps/patch-boot.stamp

# Sources list

REWRITER_SRCS = $(top_srcdir)/rewriter/com/redhat/rewriter/ClassRewriter.java

# Patch list

ICEDTEA_PATCHES = \
	patches/memory-limits.patch \
	patches/override-redirect-metacity.patch

# Conditional patches

if BUILD_CACAO
ICEDTEA_PATCHES += \
	patches/cacao/launcher.patch \
	patches/cacao/6714758.patch \
	patches/cacao/memory.patch
endif

if ENABLE_CACAO
ICEDTEA_PATCHES += \
	patches/cacao/ignore-tests.patch
endif

if ENABLE_PULSE_JAVA
ICEDTEA_PATCHES += \
	patches/pulse-soundproperties.patch
endif

if ENABLE_SYSTEMTAP
ICEDTEA_PATCHES += patches/systemtap-gcc-4.5.patch
endif

if ENABLE_NSS
ICEDTEA_PATCHES += patches/nss-config.patch
else
ICEDTEA_PATCHES += patches/nss-not-enabled-config.patch
endif

ICEDTEA_PATCHES += $(DISTRIBUTION_PATCHES)

# Bootstrapping patches

ICEDTEA_BOOT_PATCHES =

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

ICEDTEA_CONFIGURE = \
	  --enable-unlimited-crypto \
	  --with-cacerts-file=$(SYSTEM_JDK_DIR)/jre/lib/security/cacerts \
	  --with-zlib=system --with-stdc++lib=dynamic \
	  --with-boot-jdk=$(BOOT_DIR) \
	  --with-update-version=$(JDK_UPDATE_VERSION) \
	  --with-build-number=$(BUILD_VERSION) \
	  --with-milestone="fcs"

if ENABLE_CACAO
ICEDTEA_CONFIGURE += \
	--with-import-hotspot="$(CACAO_IMPORT_PATH)"
endif

if ENABLE_JAMVM
ICEDTEA_CONFIGURE += \
	--with-import-hotspot="$(JAMVM_IMPORT_PATH)"
endif

if USE_SYSTEM_LCMS
ICEDTEA_CONFIGURE += \
	--with-lcms=system
else
ICEDTEA_CONFIGURE += \
	--with-lcms=bundled
endif

ICEDTEA_ENV = \
	LANG="C" \
	PATH="$(BOOT_DIR)/bin:$(OS_PATH):$$PATH" \
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
	JAVA_HOME="" \
	JDK_HOME="" \
	QUIETLY="" \
	ANT_RESPECT_JAVA_HOME="TRUE" \
	DISTRIBUTION_ID="$(DIST_ID)" \
	DERIVATIVE_ID="$(ICEDTEA_NAME) $(PACKAGE_VERSION)$(ICEDTEA_REV)" \
	DEBUG_CLASSFILES="true" \
	DEBUG_BINARIES="true" \
	DISABLE_INTREE_EC="true" \
	LOG="debug" SCTP_WERROR= \
	POST_STRIP_CMD= STRIP_POLICY="no_strip" \
	JOBS="$(PARALLEL_JOBS)"

if DISABLE_OPTIMIZATIONS
ICEDTEA_ENV += \
	NO_OPTIMIZATIONS="true" \
	CC_NO_OPT="-O0 -g"
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

if USE_SYSTEM_LCMS
ICEDTEA_ENV += \
	LCMS_LIBS="${LCMS2_LIBS}" \
	LCMS_CFLAGS="${LCMS2_CFLAGS}"
endif

# Dropped for new build
#	ALT_JDK_IMPORT_PATH="$(BOOT_DIR)" 
#	ANT="$(ANT)" 
#	ALT_BOOTDIR="$(BOOT_DIR)" 
#	BUILD_ARCH_DIR="$(BUILD_ARCH_DIR)" 
#	ICEDTEA_RT="$(RUNTIME)" 
#	FREETYPE2_HEADERS="$(FREETYPE2_CFLAGS)" 
#	FT2_LIB="$(FREETYPE2_LIBS)" 
#	ALT_PARALLEL_COMPILE_JOBS="$(PARALLEL_JOBS)" 
#	HOTSPOT_BUILD_JOBS="$(PARALLEL_JOBS)" 
#	ALT_OUTPUTDIR="$(BUILD_OUTPUT_DIR)" 
#	ALT_JIBX_LIBS_PATH="$(JIBX_DEPS_DIR)"

# OpenJDK boot build environment.
ICEDTEA_CONFIGURE_BOOT = $(ICEDTEA_CONFIGURE)
ICEDTEA_ENV_BOOT = $(ICEDTEA_ENV) \
	JAR_KNOWS_ATFILE="$(JAR_KNOWS_ATFILE)" \
	JAR_KNOWS_J_OPTIONS="$(JAR_KNOWS_J_OPTIONS)" \
	JAR_ACCEPTS_STDIN_LIST="$(JAR_ACCEPTS_STDIN_LIST)" \
	DISABLE_NIMBUS="true"

# Dropped for new build
#	BOOTCLASSPATH_CLS_RT="-bootclasspath $(CLS_DIR_BOOT):$(RUNTIME)" 
#	BOOTCLASSPATH_CLS="-bootclasspath $(CLS_DIR_BOOT)" 
#	BOOTCLASSPATH_RT_LIBGCJ="-bootclasspath $(RUNTIME)" 
#	ALT_OUTPUTDIR="$(BOOT_BUILD_OUTPUT_DIR)"
#	XSLT="$(XSLTPROC)"

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

PULSEAUDIO_SRCS = $(top_srcdir)/pulseaudio/src/java/org/classpath/icedtea/pulseaudio/*.java \
	$(top_srcdir)/pulseaudio/src/java/META-INF/services/javax.sound.sampled.spi.MixerProvider \
	$(top_srcdir)/pulseaudio/src/native/*.h \
	$(top_srcdir)/pulseaudio/src/native/*.c \
	$(top_srcdir)/pulseaudio/TODO \
	$(top_srcdir)/pulseaudio/README \
	$(top_srcdir)/pulseaudio/testsounds/README \
	$(top_srcdir)/pulseaudio/testsounds/startup.wav \
	$(top_srcdir)/pulseaudio/testsounds/logout.wav \
	$(top_srcdir)/pulseaudio/testsounds/error.wav \
	$(top_srcdir)/pulseaudio/unittests/org/classpath/icedtea/pulseaudio/*.java \
	$(top_srcdir)/pulseaudio/COPYING

EXTRA_DIST = $(top_srcdir)/patches/*.patch \
	$(top_srcdir)/patches/cacao/*.patch \
	tools-copy contrib overlays \
	javaws.png javaws.desktop \
	jconsole.desktop policytool.desktop \
	$(JTREG_SRCS) \
	HACKING $(PULSEAUDIO_SRCS) fsg.sh \
	hotspot.map autogen.sh \
	tapset/hotspot.stp.in \
	tapset/hotspot_jni.stp.in \
	scripts/jni_create_stap.c \
	scripts/jni_desc \
	rewriter/agpl-3.0.txt \
	$(REWRITER_SRCS) \
	THANKYOU

# Top-Level Targets
# =================

all-local: icedtea-stage2

check-local: jtregcheck

clean-local: clean-jtreg clean-jtreg-reports clean-pulse-java \
 clean-icedtea clean-icedtea-debug clean-icedtea-boot clean-clone clean-clone-boot \
 clean-bootstrap-directory-stage1 clean-bootstrap-directory-stage2 \
 clean-bootstrap-directory-symlink-stage1 clean-bootstrap-directory-symlink-stage2 \
 clean-extract clean-native-ecj clean-icedtea-stage2 \
 clean-icedtea-debug-stage2 clean-icedtea-stage1 clean-add-zero clean-add-zero-debug \
 clean-add-cacao clean-add-cacao-debug clean-rt clean-rewrite-rhino clean-rewriter
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

install:

.PHONY: \
	env env-boot snapshot clean-clone clean-patch \
	clean-clone-boot clean-patch-boot clean-bootstrap-directory-stage2 \
	clean-bootstrap-directory-symlink-stage2 clean-bootstrap-directory-stage1 \
	clean-bootstrap-directory-symlink-stage1 icedtea icedtea-debug \
	clean-icedtea icedtea-stage2 clean-icedtea-boot \
	clean-rt hotspot hotspot-helper clean-jtreg clean-jtreg-reports \
	jtregcheck clean-remove-intree-libraries \
	clean-jamvm clean-extract-jamvm clean-add-jamvm clean-add-jamvm-debug \
	clean-extract-hotspot clean-sanitise-openjdk clean-icedtea-debug \
	clean-download-nashorn clean-extract-nashorn clean-download-hotspot

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
	$(ZIP) -r openjdk-$(COMBINED_VERSION) openjdk/

# Creates archive of openjdk that is compliant with Free Software guidelines.
dist-openjdk-fsg: stamps/patch-fsg.stamp
	$(ZIP) -r openjdk-fsg-$(COMBINED_VERSION) openjdk/

dist-openjdk-fsg-xz: stamps/patch-fsg.stamp
	tardir=openjdk/ && $(am__tar) | XZ_OPT=$${XZ_OPT--e} xz -c \
	  >openjdk-fsg-$(COMBINED_VERSION).tar.xz

# OpenJDK Source Preparation Targets
# ==================================

# Download OpenJDK sources.

stamps/download.stamp: stamps/download-openjdk.stamp stamps/download-cacao.stamp stamps/download-jamvm.stamp \
 stamps/download-nashorn.stamp stamps/download-hotspot.stamp
	mkdir -p stamps
	touch $@

clean-download: clean-download-openjdk clean-download-cacao clean-download-jamvm
	rm -f stamps/download.stamp

stamps/download-openjdk.stamp:
if OPENJDK_SRC_DIR_FOUND
else
if USE_HG
	$(HG) clone $(REV_ARG) $(ICEDTEA_HG_URL) openjdk;
	for repos in corba jaxp jaxws langtools jdk ; do \
	  $(HG) clone $(REV_ARG) $(ICEDTEA_HG_URL)/$${repos} openjdk/$${repos}; \
	done
else
if USE_ALT_OPENJDK_SRC_ZIP
	ln -sf $(ALT_OPENJDK_SRC_ZIP) $(OPENJDK_SRC_ZIP)
endif
	if ! echo "$(OPENJDK_SHA256SUM)  $(OPENJDK_SRC_ZIP)" \
	  | $(SHA256SUM) --check ; \
	then \
	  if [ -e $(OPENJDK_SRC_ZIP) ] ; \
	  then \
	    mv $(OPENJDK_SRC_ZIP) $(OPENJDK_SRC_ZIP).old ; \
	  fi ; \
	  $(WGET) $(ICEDTEA_URL)/$(OPENJDK_SRC_ZIP); \
	  if ! echo "$(OPENJDK_SHA256SUM)  $(OPENJDK_SRC_ZIP)" \
	    | $(SHA256SUM) --check ; \
	  then echo "ERROR: Bad download of OpenJDK root zip"; false; \
	  fi; \
	fi ;
if USE_ALT_CORBA_SRC_ZIP
	ln -sf $(ALT_CORBA_SRC_ZIP) $(CORBA_SRC_ZIP)
endif
	if ! echo "$(CORBA_SHA256SUM)  $(CORBA_SRC_ZIP)" \
	  | $(SHA256SUM) --check ; \
	then \
	  if [ $(CORBA_SRC_ZIP) ] ; \
	  then \
	    mv $(CORBA_SRC_ZIP) $(CORBA_SRC_ZIP).old ; \
	  fi ; \
	  $(WGET) $(ICEDTEA_URL)/$(CORBA_SRC_ZIP); \
	  if ! echo "$(CORBA_SHA256SUM)  $(CORBA_SRC_ZIP)" \
	    | $(SHA256SUM) --check ; \
	  then echo "ERROR: Bad download of CORBA zip"; false; \
	  fi; \
	fi ;
if USE_ALT_JAXP_SRC_ZIP
	ln -sf $(ALT_JAXP_SRC_ZIP) $(JAXP_SRC_ZIP)
endif
	if ! echo "$(JAXP_SHA256SUM)  $(JAXP_SRC_ZIP)" \
	  | $(SHA256SUM) --check ; \
	then \
	  if [ $(JAXP_SRC_ZIP) ] ; \
	  then \
	    mv $(JAXP_SRC_ZIP) $(JAXP_SRC_ZIP).old ; \
	  fi ; \
	  $(WGET) $(ICEDTEA_URL)/$(JAXP_SRC_ZIP); \
	  if ! echo "$(JAXP_SHA256SUM)  $(JAXP_SRC_ZIP)" \
	    | $(SHA256SUM) --check ; \
	  then echo "ERROR: Bad download of JAXP zip"; false; \
	  fi; \
	fi ;
if USE_ALT_JAXWS_SRC_ZIP
	ln -sf $(ALT_JAXWS_SRC_ZIP) $(JAXWS_SRC_ZIP)
endif
	if ! echo "$(JAXWS_SHA256SUM)  $(JAXWS_SRC_ZIP)" \
	  | $(SHA256SUM) --check ; \
	then \
	  if [ $(JAXWS_SRC_ZIP) ] ; \
	  then \
	    mv $(JAXWS_SRC_ZIP) $(JAXWS_SRC_ZIP).old ; \
	  fi ; \
	  $(WGET) $(ICEDTEA_URL)/$(JAXWS_SRC_ZIP); \
	  if ! echo "$(JAXWS_SHA256SUM)  $(JAXWS_SRC_ZIP)" \
	    | $(SHA256SUM) --check ; \
	  then echo "ERROR: Bad download of JAXWS zip"; false; \
	  fi; \
	fi ;
if USE_ALT_JDK_SRC_ZIP
	ln -sf $(ALT_JDK_SRC_ZIP) $(JDK_SRC_ZIP)
endif
	if ! echo "$(JDK_SHA256SUM)  $(JDK_SRC_ZIP)" \
	  | $(SHA256SUM) --check ; \
	then \
	  if [ $(JDK_SRC_ZIP) ] ; \
	  then \
	    mv $(JDK_SRC_ZIP) $(JDK_SRC_ZIP).old ; \
	  fi ; \
	  $(WGET) $(ICEDTEA_URL)/$(JDK_SRC_ZIP); \
	  if ! echo "$(JDK_SHA256SUM)  $(JDK_SRC_ZIP)" \
	    | $(SHA256SUM) --check ; \
	  then echo "ERROR: Bad download of JDK zip"; false; \
	  fi; \
	fi ;
if USE_ALT_LANGTOOLS_SRC_ZIP
	ln -sf $(ALT_LANGTOOLS_SRC_ZIP) $(LANGTOOLS_SRC_ZIP)
endif
	if ! echo "$(LANGTOOLS_SHA256SUM)  $(LANGTOOLS_SRC_ZIP)" \
	  | $(SHA256SUM) --check ; \
	then \
	  if [ $(LANGTOOLS_SRC_ZIP) ] ; \
	  then \
	    mv $(LANGTOOLS_SRC_ZIP) $(LANGTOOLS_SRC_ZIP).old ; \
	  fi ; \
	  $(WGET) $(ICEDTEA_URL)/$(LANGTOOLS_SRC_ZIP); \
	  if ! echo "$(LANGTOOLS_SHA256SUM)  $(LANGTOOLS_SRC_ZIP)" \
	    | $(SHA256SUM) --check ; \
	  then echo "ERROR: Bad download of langtools zip"; false; \
	  fi; \
	fi ;
endif
endif
	mkdir -p stamps
	touch $@

clean-download-openjdk: clean-download-nashorn clean-download-hotspot
	if [ ! -z $(OPENJDK_SRC_ZIP) ] ; then \
	  rm -f $(OPENJDK_SRC_ZIP) ; \
	fi
	if [ ! -z $(CORBA_SRC_ZIP) ] ; then \
	  rm -f $(CORBA_SRC_ZIP) ; \
	fi
	if [ ! -z $(JAXP_SRC_ZIP) ] ; then \
	  rm -f $(JAXP_SRC_ZIP) ; \
	fi
	if [ ! -z $(JAXWS_SRC_ZIP) ] ; then \
	  rm -f $(JAXWS_SRC_ZIP) ; \
	fi
	if [ ! -z $(JDK_SRC_ZIP) ] ; then \
	  rm -f $(JDK_SRC_ZIP) ; \
	fi
	if [ ! -z $(LANGTOOLS_SRC_ZIP) ] ; then \
	  rm -f $(LANGTOOLS_SRC_ZIP) ; \
	fi
	rm -f stamps/download-openjdk.stamp

stamps/download-cacao.stamp:
if BUILD_CACAO
if !USE_SYSTEM_CACAO
if USE_ALT_CACAO_SRC_ZIP
	ln -sf $(ALT_CACAO_SRC_ZIP) $(CACAO_SRC_ZIP)
endif
	if ! echo "$(CACAO_SHA256SUM)  $(CACAO_SRC_ZIP)" \
	 | $(SHA256SUM) --check ; \
	then \
	 if [ -e $(CACAO_SRC_ZIP) ] ; \
	 then \
	   mv $(CACAO_SRC_ZIP) $(CACAO_SRC_ZIP).old ; \
	 fi ; \
	 $(WGET) $(CACAO_URL) -O $(CACAO_SRC_ZIP); \
	 if ! echo "$(CACAO_SHA256SUM)  $(CACAO_SRC_ZIP)" \
	   | $(SHA256SUM) --check ; \
	 then echo "ERROR: Bad download of CACAO zip"; false; \
	 fi; \
	fi
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
	if ! echo "$(JAMVM_SHA256SUM)  $(JAMVM_SRC_ZIP)" \
	 | $(SHA256SUM) --check ; \
	then \
	 if [ -e $(JAMVM_SRC_ZIP) ] ; \
	 then \
	   mv $(JAMVM_SRC_ZIP) $(JAMVM_SRC_ZIP).old ; \
	 fi ; \
	 $(WGET) $(JAMVM_URL) -O $(JAMVM_SRC_ZIP); \
	 if ! echo "$(JAMVM_SHA256SUM)  $(JAMVM_SRC_ZIP)" \
	  | $(SHA256SUM) --check ; \
	 then echo "ERROR: Bad download of JamVM zip"; false; \
	 fi; \
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
	if ! echo "$(NASHORN_SHA256SUM)  $(NASHORN_SRC_ZIP)" \
	  | $(SHA256SUM) --check ; \
	then \
	  if [ $(NASHORN_SRC_ZIP) ] ; \
	  then \
	    mv $(NASHORN_SRC_ZIP) $(NASHORN_SRC_ZIP).old ; \
	  fi ; \
	  $(WGET) $(ICEDTEA_URL)/$(NASHORN_SRC_ZIP); \
	  if ! echo "$(NASHORN_SHA256SUM)  $(NASHORN_SRC_ZIP)" \
	    | $(SHA256SUM) --check ; \
	  then echo "ERROR: Bad download of NASHORN zip"; false; \
	  fi; \
	fi ;
endif
endif
	mkdir -p stamps
	touch $@

clean-download-nashorn:
	if [ ! -z $(NASHORN_SRC_ZIP) ] ; then \
	  rm -f $(NASHORN_SRC_ZIP) ; \
	fi
	rm -f stamps/download-nashorn.stamp

stamps/download-hotspot.stamp: stamps/download-openjdk.stamp
if OPENJDK_SRC_DIR_FOUND
else
if USE_HG
	$(HG) clone $(REV_ARG) $(HS_URL) openjdk/hotspot
else
if USE_ALT_HOTSPOT_SRC_ZIP
	ln -sf $(ALT_HOTSPOT_SRC_ZIP) $(HOTSPOT_SRC_ZIP)
endif
	if ! echo "$(HS_SHA256SUM)  $(HOTSPOT_SRC_ZIP)" \
	  | $(SHA256SUM) --check ; \
	then \
	  if [ -e $(HOTSPOT_SRC_ZIP) ] ; \
	  then \
	    mv $(HOTSPOT_SRC_ZIP) $(HOTSPOT_SRC_ZIP).old ; \
	  fi ; \
	  if test "x$(HS_TYPE)" = "xhg"; then \
	    $(WGET) $(HS_URL)/archive/$(HS_CHANGESET).tar.gz -O $(HOTSPOT_SRC_ZIP) ; \
	  else \
	    $(WGET) $(HS_URL)/$(HOTSPOT_SRC_ZIP) ; \
          fi ; \
	  if ! echo "$(HS_SHA256SUM)  $(HOTSPOT_SRC_ZIP)" \
	    | $(SHA256SUM) --check ; \
	  then echo "ERROR: Bad download of HotSpot zip"; false; \
	  fi; \
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

stamps/extract.stamp: stamps/extract-openjdk.stamp \
 stamps/extract-cacao.stamp stamps/remove-intree-libraries.stamp \
 stamps/extract-jamvm.stamp stamps/extract-hotspot.stamp \
 stamps/sanitise-openjdk.stamp stamps/extract-nashorn.stamp
	mkdir -p stamps
	touch $@

clean-extract: clean-extract-openjdk clean-extract-cacao clean-extract-jamvm
	rm -f stamps/extract.stamp

stamps/extract-openjdk.stamp: stamps/download-openjdk.stamp
if OPENJDK_SRC_DIR_FOUND
	cp -pPR $(OPENJDK_SRC_DIR_LINK) $(OPENJDK_SRC_DIR) openjdk
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
# Remove broken javazic tool
	rm -rf openjdk/jdk/make/tools/src/build/tools/javazic
	touch stamps/extract-openjdk.stamp

clean-extract-openjdk: clean-overlay clean-versioning \
 clean-patch-fsg clean-remove-intree-libraries \
 clean-sanitise-openjdk clean-extract-hotspot
	rm -rf openjdk
	rm -f stamps/extract-openjdk.stamp

stamps/extract-hotspot.stamp: stamps/extract-openjdk.stamp \
 stamps/download-hotspot.stamp
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
	touch stamps/extract-hotspot.stamp

clean-extract-hotspot:
	rm -rf openjdk/hotspot
	rm -f stamps/extract-hotspot.stamp

stamps/extract-nashorn.stamp: stamps/extract-openjdk.stamp stamps/download-nashorn.stamp
if !OPENJDK_SRC_DIR_FOUND
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
	mkdir -p stamps
	touch $@

clean-extract-nashorn:
	rm -rf openjdk/nashorn
	rm -f stamps/extract-nashorn.stamp

stamps/sanitise-openjdk.stamp: stamps/extract-openjdk.stamp \
 stamps/extract-hotspot.stamp stamps/extract-nashorn.stamp
	chmod -R ug+w openjdk 
	sh $(srcdir)/fsg.sh
	touch stamps/sanitise-openjdk.stamp

clean-sanitise-openjdk:
	rm -f stamps/sanitise-openjdk.stamp

stamps/extract-cacao.stamp: stamps/download-cacao.stamp
if BUILD_CACAO
if !USE_SYSTEM_CACAO
	set -e ; \
	if ! test -d cacao/cacao ; \
	then \
	  mkdir cacao ; \
	  $(TAR) xjf $(CACAO_SRC_ZIP) -C cacao ; \
	  dir=$$(basename cacao/cacao-*) ; \
	  ln -s $$dir cacao/cacao ; \
	  cd cacao/cacao ; \
	  cd .. ; \
	fi
endif
endif
	mkdir -p stamps
	touch $@

clean-extract-cacao: clean-cacao
	rm -f stamps/extract-cacao.stamp

stamps/extract-jamvm.stamp:
if BUILD_JAMVM
	set -e ; \
	if ! test -d jamvm ; \
	then \
	  mkdir jamvm ; \
	  $(TAR) xf $(JAMVM_SRC_ZIP) -C jamvm ; \
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
	mv $(VERSION_SETTINGS_FILE) ProfileNames.gmk.bak
	cat ProfileNames.gmk.bak > $(VERSION_SETTINGS_FILE)
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
	sed -i "s#BUILD_VARIANT_RELEASE)#BUILD_VARIANT_RELEASE)$${proj_suffix}$${ver_suffix}#" \
	  $(VERSION_SETTINGS_FILE);
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
	    --main=org.eclipse.jdt.internal.compiler.batch.Main ${ECJ_JAR}
endif
	touch $@

clean-native-ecj:
	rm -f native-ecj
	rm -rf stamps/native-ecj.stamp

# bootstrap/stage1
stamps/bootstrap-directory-stage1.stamp: stamps/native-ecj.stamp
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
	ln -s $(BUILD_BOOT_IMAGE_DIR)/j2sdk-image/ $(STAGE2_BOOT_DIR)
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
 stamps/cacao.stamp $(PULSE_JAVA_TARGET) stamps/rewrite-rhino.stamp \
 stamps/jamvm.stamp
	mkdir -p $(BUILD_OUTPUT_DIR)
	cd $(BUILD_OUTPUT_DIR) && \
	$(SHELL) $(abs_top_builddir)/openjdk/configure $(ICEDTEA_CONFIGURE)
	mkdir -p stamps
	touch $@

stamps/icedtea.stamp: stamps/icedtea-configure.stamp
	$(ARCH_PREFIX) $(ICEDTEA_MAKE) \
	  $(ICEDTEA_ENV) \
	  -C $(BUILD_OUTPUT_DIR) \
	  $(ICEDTEA_BUILD_TARGET)
	$(abs_top_builddir)/pax-mark-vm $(BUILD_IMAGE_DIR)/j2sdk-image
	mkdir -p $(BUILD_IMAGE_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	mkdir -p $(BUILD_IMAGE_DIR)/j2re-image/lib/$(INSTALL_ARCH_DIR)
	mkdir -p $(BUILD_IMAGE_DIR)/j2sdk-image/jre/lib/ext
	mkdir -p $(BUILD_IMAGE_DIR)/j2re-image/lib/ext 
if ENABLE_PULSE_JAVA
	cp -pPRf $(PULSE_JAVA_NATIVE_BUILDDIR)/libpulse-java.so \
	  $(BUILD_IMAGE_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	cp -pPRf $(PULSE_JAVA_NATIVE_BUILDDIR)/libpulse-java.so \
	  $(BUILD_IMAGE_DIR)/j2re-image/lib/$(INSTALL_ARCH_DIR)
	cp -pPRf pulse-java.jar \
	  $(BUILD_IMAGE_DIR)/j2sdk-image/jre/lib/ext
	cp -pPRf pulse-java.jar \
	  $(BUILD_IMAGE_DIR)/j2re-image/lib/ext
	(cd $(PULSE_JAVA_JAVA_SRCDIR) && \
	   $(ZIP) -qur $(BUILD_IMAGE_DIR)/j2sdk-image/src.zip org )
endif
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
	mkdir -p $(BUILD_IMAGE_DIR)/j2sdk-image/tapset ; \
	grep "client IGNORE" $(BUILD_JRE_ARCH_DIR)/jvm.cfg; \
	if test $$? -eq 0; then \
	  sed -e '/\/client\/libjvm.so/d' \
	    < $(abs_top_builddir)/tapset/hotspot.stp \
	    > $(BUILD_IMAGE_DIR)/j2sdk-image/tapset/hotspot.stp; \
	  sed -e '/\/client\/libjvm.so/d' \
	    < $(abs_top_builddir)/tapset/hotspot_jni.stp \
	    > $(BUILD_IMAGE_DIR)/j2sdk-image/tapset/hotspot_jni.stp; \
	else \
	  cp $(abs_top_builddir)/tapset/hotspot.stp \
	    $(BUILD_IMAGE_DIR)/j2sdk-image/tapset/hotspot.stp; \
	  cp $(abs_top_builddir)/tapset/hotspot_jni.stp \
	    $(BUILD_IMAGE_DIR)/j2sdk-image/tapset/hotspot_jni.stp; \
	fi; \
	cp $(abs_top_builddir)/tapset/jstack.stp \
	  $(BUILD_IMAGE_DIR)/j2sdk-image/tapset/jstack.stp
endif
	cp $(abs_top_builddir)/nss.cfg \
	  $(BUILD_IMAGE_DIR)/j2sdk-image/jre/lib/security;
if WITH_TZDATA_DIR
	cp $(abs_top_builddir)/tz.properties \
	  $(BUILD_IMAGE_DIR)/j2sdk-image/jre/lib;
endif
if ENABLE_JAMVM
	printf -- '-jamvm ALIASED_TO -server\n' >> $(BUILD_JRE_ARCH_DIR)/jvm.cfg
endif
	@echo "IcedTea is served:" $(BUILD_IMAGE_DIR)/j2sdk-image
	mkdir -p stamps
	touch $@

stamps/icedtea-debug-configure.stamp: stamps/bootstrap-directory-symlink-stage2.stamp \
 stamps/download.stamp stamps/extract.stamp $(OPENJDK_TREE) \
 stamps/cacao.stamp $(PULSE_JAVA_TARGET) stamps/rewrite-rhino.stamp \
 stamps/jamvm.stamp
	mkdir -p $(DEBUG_BUILD_OUTPUT_DIR)
	cd $(DEBUG_BUILD_OUTPUT_DIR) && \
	$(SHELL) $(abs_top_builddir)/openjdk/configure $(ICEDTEA_CONFIGURE) \
	  --with-debug-level=slowdebug
	mkdir -p stamps
	touch $@

stamps/icedtea-debug.stamp: stamps/icedtea-debug-configure.stamp
	$(ARCH_PREFIX) $(ICEDTEA_MAKE) \
	  $(ICEDTEA_ENV) \
	  -C $(DEBUG_BUILD_OUTPUT_DIR) \
	  $(ICEDTEA_BUILD_TARGET)
	$(abs_top_builddir)/pax-mark-vm $(BUILD_DEBUG_IMAGE_DIR)/j2sdk-image
	mkdir -p $(BUILD_DEBUG_IMAGE_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	mkdir -p $(BUILD_DEBUG_IMAGE_DIR)/j2re-image/lib/$(INSTALL_ARCH_DIR)
	mkdir -p $(BUILD_DEBUG_IMAGE_DIR)/j2sdk-image/jre/lib/ext
	mkdir -p $(BUILD_DEBUG_IMAGE_DIR)/j2re-image/lib/ext 
if ENABLE_PULSE_JAVA
	cp -pPRf $(PULSE_JAVA_NATIVE_BUILDDIR)/libpulse-java.so \
	  $(BUILD_DEBUG_IMAGE_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	cp -pPRf $(PULSE_JAVA_NATIVE_BUILDDIR)/libpulse-java.so \
	  $(BUILD_DEBUG_IMAGE_DIR)/j2re-image/lib/$(INSTALL_ARCH_DIR)
	cp -pPRf pulse-java.jar \
	  $(BUILD_DEBUG_IMAGE_DIR)/j2sdk-image/jre/lib/ext
	cp -pPRf pulse-java.jar \
	  $(BUILD_DEBUG_IMAGE_DIR)/j2re-image/lib/ext
	(cd $(PULSE_JAVA_JAVA_SRCDIR) && \
	   $(ZIP) -qur $(BUILD_DEBUG_IMAGE_DIR)/j2sdk-image/src.zip org )
endif
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
	mkdir -p $(BUILD_DEBUG_IMAGE_DIR)/j2sdk-image/tapset ; \
	grep "client IGNORE" $(BUILD_JRE_ARCH_DIR)/jvm.cfg; \
	if test $$? -eq 0; then \
	  sed -e '/\/client\/libjvm.so/d' \
	    < $(abs_top_builddir)/tapset/hotspot.stp \
	    > $(BUILD_DEBUG_IMAGE_DIR)/j2sdk-image/tapset/hotspot.stp; \
	  sed -e '/\/client\/libjvm.so/d' \
	    < $(abs_top_builddir)/tapset/hotspot_jni.stp \
	    > $(BUILD_DEBUG_IMAGE_DIR)/j2sdk-image/tapset/hotspot_jni.stp; \
	else \
	  cp $(abs_top_builddir)/tapset/hotspot.stp \
	    $(BUILD_DEBUG_IMAGE_DIR)/j2sdk-image/tapset/hotspot.stp; \
	  cp $(abs_top_builddir)/tapset/hotspot_jni.stp \
	    $(BUILD_DEBUG_IMAGE_DIR)/j2sdk-image/tapset/hotspot_jni.stp; \
	fi; \
	cp $(abs_top_builddir)/tapset/jstack.stp \
	  $(BUILD_DEBUG_IMAGE_DIR)/j2sdk-image/tapset/jstack.stp
endif
	cp $(abs_top_builddir)/nss.cfg \
	  $(BUILD_DEBUG_IMAGE_DIR)/j2sdk-image/jre/lib/security;
if WITH_TZDATA_DIR
	cp $(abs_top_builddir)/tz.properties \
	  $(BUILD_DEBUG_IMAGE_DIR)/j2sdk-image/jre/lib;
endif
if ENABLE_JAMVM
	printf -- '-jamvm ALIASED_TO -server\n' >> $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg
endif
	@echo "IcedTea (debug build) is served:" $(BUILD_DEBUG_IMAGE_DIR)/j2sdk-image
	mkdir -p stamps
	touch $@

clean-icedtea: 
	rm -rf $(BUILD_OUTPUT_DIR)
	rm -f stamps/icedtea-configure.stamp stamps/icedtea.stamp

clean-icedtea-debug:
	rm -rf $(DEBUG_BUILD_OUTPUT_DIR)
	rm -f stamps/icedtea-debug-configure.stamp
	rm -f stamps/icedtea-debug.stamp

stamps/icedtea-stage2.stamp: stamps/icedtea.stamp stamps/add-cacao.stamp \
 stamps/add-zero.stamp stamps/add-jamvm.stamp
	mkdir -p stamps
	touch $@

clean-icedtea-stage2: clean-add-jamvm
	rm -f stamps/icedtea-stage2.stamp

stamps/icedtea-debug-stage2.stamp: stamps/icedtea-debug.stamp \
 stamps/add-cacao-debug.stamp stamps/add-zero-debug.stamp stamps/add-jamvm-debug.stamp
	mkdir -p stamps
	touch $@

clean-icedtea-debug-stage2: clean-add-jamvm-debug
	rm -f stamps/icedtea-debug-stage2.stamp

# OpenJDK boot Targets
# ===================

stamps/icedtea-boot-configure.stamp: stamps/bootstrap-directory-symlink-stage1.stamp \
 stamps/download.stamp $(OPENJDK_BOOT_TREE) stamps/cacao.stamp \
 stamps/rewrite-rhino.stamp stamps/jamvm.stamp
	mkdir -p $(BOOT_BUILD_OUTPUT_DIR)
	cd $(BOOT_BUILD_OUTPUT_DIR) && \
	$(SHELL) $(abs_top_builddir)/openjdk-boot/configure $(ICEDTEA_CONFIGURE_BOOT)
	mkdir -p stamps
	touch $@

stamps/icedtea-boot.stamp: stamps/icedtea-boot-configure.stamp
	$(ARCH_PREFIX) $(ICEDTEA_MAKE) \
	  $(ICEDTEA_ENV_BOOT) \
	  -C $(BOOT_BUILD_OUTPUT_DIR) \
	  images
	$(abs_top_builddir)/pax-mark-vm $(BUILD_BOOT_IMAGE_DIR)/j2sdk-image
	@echo "Bootstrapped IcedTea is served:" $(BUILD_BOOT_IMAGE_DIR)/j2sdk-image
	mkdir -p stamps
	touch $@

# Rebuild targets

rebuild:
if BOOTSTRAPPING
	rm -f stamps/icedtea-boot.stamp
	rm -f stamps/icedtea-stage1.stamp
	$(ARCH_PREFIX) $(MAKE) icedtea-stage1
	@echo "bootstrap-poured IcedTea is served (again):" $(BUILD_BOOT_IMAGE_DIR)/j2sdk-image
else
	rm -f stamps/icedtea.stamp
	rm -f stamps/icedtea-stage2.stamp
	$(ARCH_PREFIX) $(MAKE) icedtea-stage2
	@echo "IcedTea is served (again):" $(BUILD_IMAGE_DIR)/j2sdk-image
endif

hotspot:
if BOOTSTRAPPING
	rm -f stamps/icedtea-boot.stamp
	rm -f stamps/icedtea-stage1.stamp
	$(ARCH_PREFIX) $(MAKE) ICEDTEA_BUILD_TARGET=hotspot icedtea-stage1
	mkdir -p \
	   $(BUILD_BOOT_IMAGE_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)/server
	cp $(BOOT_BUILD_OUTPUT_DIR)/$(SERVER_DIR)/libjvm.so \
	   $(BUILD_BOOT_IMAGE_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)/server
	@echo "HotSpot is served:" $(BUILD_BOOT_IMAGE_DIR)/j2sdk-image
else
	rm -f stamps/icedtea.stamp
	rm -f stamps/icedtea-stage2.stamp
	$(ARCH_PREFIX) $(MAKE) ICEDTEA_BUILD_TARGET=hotspot icedtea-stage2
	mkdir -p \
	   $(BUILD_IMAGE_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)/server
	cp $(BOOT_BUILD_OUTPUT_DIR)/$(SERVER_DIR)/libjvm.so \
	   $(BUILD_IMAGE_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)/server
	@echo "HotSpot is served:" $(BUILD_IMAGE_DIR)/j2sdk-image
endif

clean-icedtea-boot:
	rm -rf $(BOOT_BUILD_OUTPUT_DIR)
	rm -f stamps/icedtea-boot-configure.stamp
	rm -f stamps/icedtea-boot.stamp

stamps/icedtea-stage1.stamp: stamps/icedtea-boot.stamp
	mkdir -p stamps
	touch $@

clean-icedtea-stage1:
	rm -f stamps/icedtea-stage1.stamp

# PulseAudio based mixer
# (pulse-java)
if ENABLE_PULSE_JAVA

stamps/pulse-java.stamp: stamps/pulse-java-jar.stamp $(PULSE_JAVA_NATIVE_BUILDDIR)/libpulse-java.so
	mkdir -p stamps
	touch $@

stamps/pulse-java-class.stamp: $(INITIAL_BOOTSTRAP_LINK_STAMP)
	mkdir -p $(PULSE_JAVA_CLASS_DIR)
	(cd $(PULSE_JAVA_JAVA_SRCDIR); \
	 $(BOOT_DIR)/bin/javac $(IT_JAVACFLAGS) -d $(PULSE_JAVA_CLASS_DIR) \
	 -bootclasspath $(RUNTIME) org/classpath/icedtea/pulseaudio/*.java \
	)
	cp  -r $(PULSE_JAVA_JAVA_SRCDIR)/META-INF $(PULSE_JAVA_CLASS_DIR)
	chmod -R ug+w $(PULSE_JAVA_CLASS_DIR)/META-INF
	mkdir -p stamps
	touch $@

stamps/pulse-java-jar.stamp: stamps/pulse-java-class.stamp
	$(BOOT_DIR)/bin/jar cf pulse-java.jar -C $(PULSE_JAVA_CLASS_DIR) .;
	mkdir -p stamps
	touch $@

stamps/pulse-java-headers.stamp: stamps/pulse-java-class.stamp
	mkdir -p $(PULSE_JAVA_NATIVE_BUILDDIR)
	$(BOOT_DIR)/bin/javah -d $(PULSE_JAVA_NATIVE_BUILDDIR) -classpath $(PULSE_JAVA_CLASS_DIR) \
	  org.classpath.icedtea.pulseaudio.EventLoop ;
	$(BOOT_DIR)/bin/javah -d $(PULSE_JAVA_NATIVE_BUILDDIR) -classpath $(PULSE_JAVA_CLASS_DIR) \
	  org.classpath.icedtea.pulseaudio.Stream
	$(BOOT_DIR)/bin/javah -d $(PULSE_JAVA_NATIVE_BUILDDIR) -classpath $(PULSE_JAVA_CLASS_DIR) \
	  org.classpath.icedtea.pulseaudio.Operation
	$(BOOT_DIR)/bin/javah -d $(PULSE_JAVA_NATIVE_BUILDDIR) -classpath $(PULSE_JAVA_CLASS_DIR) \
	  org.classpath.icedtea.pulseaudio.PulseAudioSourcePort
	$(BOOT_DIR)/bin/javah -d $(PULSE_JAVA_NATIVE_BUILDDIR) -classpath $(PULSE_JAVA_CLASS_DIR) \
	  org.classpath.icedtea.pulseaudio.PulseAudioTargetPort
	mkdir -p stamps
	touch $@

$(PULSE_JAVA_NATIVE_BUILDDIR)/%.o: $(PULSE_JAVA_NATIVE_SRCDIR)/%.c stamps/pulse-java-headers.stamp
	$(CC) $(IT_CFLAGS) -fPIC -I$(BOOT_DIR)/include/linux -I$(BOOT_DIR)/include \
	 -I$(PULSE_JAVA_NATIVE_BUILDDIR) -o $@ -c $<

$(PULSE_JAVA_NATIVE_BUILDDIR)/libpulse-java.so: $(PULSE_JAVA_NATIVE_OBJECTS)
	$(CC) $(LDFLAGS) -shared $(PULSE_JAVA_NATIVE_OBJECTS) $(LIBPULSE_LIBS) \
	 -o $(PULSE_JAVA_NATIVE_BUILDDIR)/libpulse-java.so

endif

clean-pulse-java:
	rm -rf $(PULSE_JAVA_NATIVE_BUILDDIR)
	rm -rf $(PULSE_JAVA_CLASS_DIR)
	rm -f pulse-java.jar
	if [ -e $(PULSE_JAVA_BUILDDIR) ]; then \
	  rmdir $(PULSE_JAVA_BUILDDIR) ; \
	fi
	rm -f stamps/pulse-java*.stamp

# end of pulse-java

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
	   new_file=../rhino.new/`echo $$files|sed -e 's#org#sun/org#'` ; \
	   mkdir -p `dirname $$new_file` ; \
	   cp -v $$files $$new_file ; \
	   sed -ie 's#org\.mozilla#sun.org.mozilla#g' $$new_file ; \
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
	./autogen.sh --with-java-runtime-library=openjdk8 \
	  --prefix=$(abs_top_builddir)/jamvm/install ; \
	$(MAKE) ; \
	$(MAKE) install
	mkdir -p $(abs_top_builddir)/jamvm/install/hotspot/lib
	touch $(abs_top_builddir)/jamvm/install/hotspot/lib/sa-jdi.jar
	mkdir -p $(abs_top_builddir)/jamvm/install/hotspot/jre/lib/$(INSTALL_ARCH_DIR)/server
	cp $(abs_top_builddir)/jamvm/install/lib/libjvm.so $(abs_top_builddir)/jamvm/install/hotspot/jre/lib/$(INSTALL_ARCH_DIR)/server
	ln -sf server $(abs_top_builddir)/jamvm/install/hotspot/jre/lib/$(INSTALL_ARCH_DIR)/client
	touch $(abs_top_builddir)/jamvm/install/hotspot/jre/lib/$(INSTALL_ARCH_DIR)/server/Xusage.txt
	ln -sf libjvm.so $(abs_top_builddir)/jamvm/install/hotspot/jre/lib/$(INSTALL_ARCH_DIR)/client/libjsig.so
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
	  sed -i 's#-jamvm KNOWN#-jamvm ERROR#' $(BUILD_JRE_ARCH_DIR)/jvm.cfg ; \
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
	  sed -i 's#-jamvm KNOWN#-jamvm ERROR#' $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg ; \
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
	  --enable-jre-layout $(CACAO_CONFIGURE_ARGS); \
	$(ARCH_PREFIX) $(MAKE) -j$(PARALLEL_JOBS) install
	mkdir -p $(abs_top_builddir)/cacao/install/lib
	touch $(abs_top_builddir)/cacao/install/hotspot/lib/sa-jdi.jar
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
	  sed -i 's#-cacao KNOWN#-cacao ERROR#' $(BUILD_JRE_ARCH_DIR)/jvm.cfg ; \
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
	  sed -i 's#-cacao KNOWN#-cacao ERROR#' $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg ; \
	fi
	rm -f stamps/add-cacao-debug.stamp

# configure script arguments, quoted in single quotes
CONFIGURE_ARGS = @CONFIGURE_ARGS@
ADD_ZERO_CONFIGURE_ARGS = \
	--with-jdk-home=$(BUILD_IMAGE_DIR)/j2sdk-image \
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
	  $(if $(findstring --with-$(i)-src-zip=, $(CONFIGURE_ARGS)),, --with-$(i)-src-zip=$(abs_top_builddir)/$(i).tar.xz))

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
	  sed -i 's#-zero KNOWN#-zero ERROR#' $(BUILD_JRE_ARCH_DIR)/jvm.cfg ; \
	  sed -i 's#-shark KNOWN#-shark ERROR#' $(BUILD_JRE_ARCH_DIR)/jvm.cfg ; \
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
	  sed -i 's#-zero KNOWN#-zero ERROR#' $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg ; \
	  sed -i 's#-shark KNOWN#-shark ERROR#' $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg ; \
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
	rm -f test/jtreg.jar
	rm -f stamps/jtreg.stamp

check-hotspot: stamps/jtreg.stamp
	mkdir -p test/hotspot/JTwork test/hotspot/JTreport
	$(BOOT_DIR)/bin/java -jar test/jtreg.jar -v1 -a -ignore:quiet \
		-w:test/hotspot/JTwork -r:test/hotspot/JTreport \
		-jdk:$(BUILD_IMAGE_DIR)/j2sdk-image \
		$(ICEDTEA_JTREG_OPTIONS) \
		`pwd`/openjdk/hotspot/test \
	    | tee test/$@.log

check-langtools: stamps/jtreg.stamp
	mkdir -p test/langtools/JTwork test/langtools/JTreport
	$(BOOT_DIR)/bin/java -jar test/jtreg.jar -v1 -a -ignore:quiet \
		-w:test/langtools/JTwork -r:test/langtools/JTreport \
		$${ICEDTEA_JTREG_OTHERVM:--samevm} \
		-jdk:$(BUILD_IMAGE_DIR)/j2sdk-image \
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
		-jdk:$(BUILD_IMAGE_DIR)/j2sdk-image \
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
	| sed 's,$(CURDIR)/$(sdkimg),<sdkimg>,g;s,$(CURDIR),<pwd>,g'
jtreg_pids = ps x --no-headers -ww -o pid,ppid,args \
	| awk '$$2 == 1 && $$3 ~ /^$(subst /,\/,$(CURDIR)/$(sdkimg))/ {print $$1}'

jtregcheck: $(TESTS_TO_RUN)
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
	     destpath=`echo $$dirs|sed -e 's#$(LANGTOOLS)/##' -e 's#$(SHARE)/##'` ; \
	     mkdir -p `dirname $$destpath` ; \
	     cp -a ../../$$dirs $$destpath ; \
	   done ; \
	   $(ZIP) -qur $(STAGE1_BOOT_DIR)/jre/lib/rt.jar \
	     com java javax sun ); \
	fi
	mkdir -p stamps
	touch $@

clean-rt:
	rm -rf lib/rt
	rm -f stamps/rt-class-files.stamp stamps/rt.stamp
	rm -f rt-source-files.txt
	if [ -e $(STAGE1_BOOT_DIR)/jre/lib ] ; then \
	  cp $(SYSTEM_JDK_DIR)/jre/lib/rt.jar $(STAGE1_BOOT_DIR)/jre/lib ; \
	fi

# Target Aliases
# ===============

add-zero: stamps/add-zero.stamp

add-zero-debug: stamps/add-zero-debug.stamp

bootstrap-directory-stage1: stamps/bootstrap-directory-stage1.stamp

bootstrap-directory-stage2: stamps/bootstrap-directory-stage2.stamp

bootstrap-directory-symlink-stage1: stamps/bootstrap-directory-symlink-stage1.stamp

bootstrap-directory-symlink-stage2: stamps/bootstrap-directory-symlink-stage2.stamp

cacao: stamps/cacao.stamp

clone-boot: stamps/clone-boot.stamp

download: stamps/download.stamp

download-cacao: stamps/download-cacao.stamp

download-jamvm: stamps/download-jamvm.stamp

download-openjdk: stamps/download-openjdk.stamp

extract: stamps/extract.stamp

extract-cacao: stamps/extract-cacao.stamp

extract-openjdk: stamps/extract-openjdk.stamp

extract-hotspot: stamps/extract-hotspot.stamp

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

jtreg: stamps/jtreg.stamp

native-ecj: stamps/native-ecj.stamp

overlay: stamps/overlay.stamp

patch: stamps/patch.stamp

patch-boot: stamps/patch-boot.stamp

patch-fsg: stamps/patch-fsg.stamp

pulse-java: $(PULSE_JAVA_TARGET)

rewriter: stamps/rewriter.stamp

rewrite-rhino: stamps/rewrite-rhino.stamp

rt: stamps/rt.stamp

rt-class-files: stamps/rt-class-files.stamp

sanitise-openjdk: stamps/sanitise-openjdk.stamp
