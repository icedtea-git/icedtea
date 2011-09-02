# Dependencies

OPENJDK_VERSION = b00

CORBA_CHANGESET = 635ebbe236a9
HOTSPOT_CHANGESET = dc6631a3d175
JAXP_CHANGESET = bf4b076e4556
JAXWS_CHANGESET = 8ff499dc157f
JDK_CHANGESET = dc5df74132b6
LANGTOOLS_CHANGESET = 8caec3672381
OPENJDK_CHANGESET = 5057eaa7e88a

CORBA_SHA256SUM = edd7cb8ae6a9d9b0e8f9748ecdf31a0f4decc8a06b418baa7b2ef1e7a9d3474a
HOTSPOT_SHA256SUM = dd59a52bed76f01b382c0c3b44905390bd5e7a77a54b3c8f6de71001376e4b99
JAXP_SHA256SUM = 4daf673635e1a09339695ec246afbc154c5fc31715a4748a5c6d6ae7332ffdfb
JAXWS_SHA256SUM = b1a54b1f295451d449683b04de8dc8b6c82014e647f1b41543798d401d011ca3
JDK_SHA256SUM = cd06344728aff01027875757d26d5ec62b6406095af24e50fcaf72a5dbf8a10c
LANGTOOLS_SHA256SUM = 495b601afd6b12bee9a8ccc3e1d067bb14789d4f3c0a9d21388b1708d946e3fb
OPENJDK_SHA256SUM = 6dada5c4c04640ef4b95b7ceeff20baeb69ff4cd64e2591eb64bb33c6035b5cf

CACAO_VERSION = d6264eb66506
CACAO_SHA256SUM = 94ea7899e806ccbc33a732b5113a8f969d8b1f4ce7ffd27cf04577054f65f63c
CACAO_BASE_URL = http://mips.complang.tuwien.ac.at/hg/cacao/archive
CACAO_URL = $(CACAO_BASE_URL)/$(CACAO_VERSION).tar.bz2
CACAO_SRC_ZIP = cacao-$(CACAO_VERSION).tar.bz2

JAMVM_VERSION = 6a5776faa95649adae498e349da7a93b8b9fc355
JAMVM_SHA256SUM = 1482dbd4d1ee44b6c50350904ae44debeaee72fb2fafeca6862db9fa00a04171
JAMVM_BASE_URL = http://icedtea.classpath.org/download/drops/jamvm
JAMVM_URL = $(JAMVM_BASE_URL)/jamvm-$(JAMVM_VERSION).tar.gz
JAMVM_SRC_ZIP = jamvm-$(JAMVM_VERSION).tar.gz

JAXWS_DROP_URL = http://icedtea.classpath.org/download/drops
JAXWS_DROP_ZIP = jdk7-jaxws2_2_4-b03-2011_05_27.zip
JAXWS_DROP_SHA256SUM = 05ae7259b75d0f2307276d61ece1887fcd437cb4ecda42fab8c22d4b537effd6
JAF_DROP_URL = http://icedtea.classpath.org/download/drops
JAF_DROP_ZIP = jdk7-jaf-2010_08_19.zip
JAF_DROP_SHA256SUM = e6aefedfdbb4673e8019583d1344fb162b94e1b10382c362364dbbfd5889c09e
JAXP_DROP_URL = http://icedtea.classpath.org/download/drops
JAXP_DROP_ZIP = jaxp145_01.zip
JAXP_DROP_SHA256SUM = c5924c6188988e2e8397ee5143bd8eaa062569d052567afd2ac9326a54a848cb

ICEDTEA_PREFIX = icedtea8-forest
ICEDTEA_HG_URL = http://icedtea.classpath.org/hg/$(ICEDTEA_PREFIX)
OPENJDK_HG_URL = http://hg.openjdk.java.net/jdk8/jdk8
CVMI_HG_URL = http://hg.openjdk.java.net/cvmi/cvmi/
CLOSURES_HG_URL = http://hg.openjdk.java.net/closures/closures/
CACIOCAVALLO_HG_URL = http://hg.openjdk.java.net/caciocavallo/jdk8/
BSD_HG_URL = http://hg.openjdk.java.net/bsd-port/bsd-port
NIO2_HG_URL = http://hg.openjdk.java.net/nio/nio/

OPENJDK_URL = $(ICEDTEA_HG_URL)
CORBA_URL = $(ICEDTEA_HG_URL)/corba
JAXP_URL = $(ICEDTEA_HG_URL)/jaxp
JAXWS_URL = $(ICEDTEA_HG_URL)/jaxws
JDK_URL = $(ICEDTEA_HG_URL)/jdk
LANGTOOLS_URL = $(ICEDTEA_HG_URL)/langtools
HOTSPOT_URL = $(ICEDTEA_HG_URL)/hotspot

# Build directories

BUILD_DIR = @OPENJDK_BUILD_DIR@
BUILD_OUTPUT_DIR = $(abs_top_builddir)/$(BUILD_DIR)
DEBUG_BUILD_OUTPUT_DIR = $(BUILD_OUTPUT_DIR)-debug
BOOT_BUILD_OUTPUT_DIR = $(BUILD_OUTPUT_DIR)-boot
BOOT_DIR = $(abs_top_builddir)/bootstrap/jdk1.6.0
RUNTIME = $(BOOT_DIR)/jre/lib/rt.jar
CLS_DIR_BOOT = $(BOOT_BUILD_OUTPUT_DIR)/classes
SERVER_DIR = hotspot/import/jre/lib/$(INSTALL_ARCH_DIR)/server
BUILD_JRE_ARCH_DIR = $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
BUILD_DEBUG_JRE_ARCH_DIR = $(DEBUG_BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
REWRITER_BUILD_DIR = $(abs_top_builddir)/rewriter.build
STAGE1_BOOT_DIR = $(abs_top_builddir)/bootstrap/boot
STAGE2_BOOT_DIR = $(abs_top_builddir)/bootstrap/icedtea
JAMVM_IMPORT_PATH = $(abs_top_builddir)/jamvm/install/hotspot

# Source directories

SHARE = openjdk-boot/jdk/src/share/classes
SOLARIS = openjdk-boot/jdk/src/solaris/classes
LANGTOOLS = openjdk-boot/langtools/src/share/classes
CORBA = openjdk-boot/corba/src/share/classes
JAXWS = $(BUILD_OUTPUT_DIR)/jaxws/drop/jaxws_src/src

PULSE_JAVA_DIR = $(abs_top_srcdir)/pulseaudio
PULSE_JAVA_NATIVE_SRCDIR = $(PULSE_JAVA_DIR)/src/native
PULSE_JAVA_NATIVE_SRCS = $(wildcard $(PULSE_JAVA_NATIVE_SRCDIR)/*.c)
PULSE_JAVA_BUILDDIR = $(abs_top_builddir)/pulseaudio.build
PULSE_JAVA_NATIVE_BUILDDIR = $(PULSE_JAVA_BUILDDIR)/native
PULSE_JAVA_NATIVE_OBJECTS = \
	$(subst $(PULSE_JAVA_NATIVE_SRCDIR),$(PULSE_JAVA_NATIVE_BUILDDIR),$(patsubst %.c,%.o,$(PULSE_JAVA_NATIVE_SRCS)))
PULSE_JAVA_JAVA_SRCDIR = $(PULSE_JAVA_DIR)/src/java
PULSE_JAVA_CLASS_DIR = $(PULSE_JAVA_BUILDDIR)/classes

if DTDTYPE_QNAME
NEED_JAXWS_SRC = true
JAXWS_COND = $(JAXWS)
endif

if LACKS_JAVAX_ANNOTATION_RESOURCE
NEED_JAXWS_SRC = true
JAXWS_COND = $(JAXWS)
endif

OPENJDK_SOURCEPATH_DIRS = \
        $(SHARE):$(SOLARIS):$(LANGTOOLS):$(CORBA):$(JAXWS_COND)

SOURCEPATH_DIRS = $(abs_top_srcdir)/generated:$(OPENJDK_SOURCEPATH_DIRS)

# Sources used from OpenJDK.
if LACKS_SUN_AWT_TOOLKIT
#PR43148 - javac fails due to missing java.util.regex.Matcher.quoteReplacement
#PR48033 - Missing javax.management.remote.JMXServiceURL
#PR48034 - javax.management.modelmbean.ModelMBeanInfo
#PR42003 - Missing javax.swing.plaf.basic.BasicDirectoryModel methods cause OpenJDK build failure 
ICEDTEA_BOOTSTRAP_CLASSES = \
	$(SHARE)/java/util/regex/Matcher.java \
	$(SHARE)/javax/management/remote/JMXServiceURL.java \
	$(SHARE)/javax/management/modelmbean/ModelMBeanInfo.java \
	$(SHARE)/javax/swing/plaf/basic/BasicDirectoryModel.java
endif

# Settings for javac

IT_LANGUAGE_SOURCE_VERSION=6
IT_CLASS_TARGET_VERSION=6
IT_JAVAC_SETTINGS=-g -encoding utf-8 $(JAVACFLAGS) $(MEMORY_LIMIT) $(PREFER_SOURCE)
IT_JAVACFLAGS=$(IT_JAVAC_SETTINGS) -source $(IT_LANGUAGE_SOURCE_VERSION) -target $(IT_CLASS_TARGET_VERSION)

# Flags
IT_CFLAGS=$(CFLAGS) $(ARCHFLAG)

# Conditional defintions

if USE_HG
  OPENJDK_SRC_ZIP = 
  CORBA_SRC_ZIP =
  JAXP_SRC_ZIP =
  JAXWS_SRC_ZIP =
  JDK_SRC_ZIP =
  LANGTOOLS_SRC_ZIP =
  HOTSPOT_SRC_ZIP =
else
  OPENJDK_SRC_ZIP = openjdk.tar.gz
  CORBA_SRC_ZIP = corba.tar.gz
  JAXP_SRC_ZIP = jaxp.tar.gz
  JAXWS_SRC_ZIP = jaxws.tar.gz
  JDK_SRC_ZIP = jdk.tar.gz
  LANGTOOLS_SRC_ZIP = langtools.tar.gz
  HOTSPOT_SRC_ZIP = hotspot.tar.gz
endif

if WITH_ALT_HSBUILD
  HS_SHA256SUM = "`$(AWK) 'version==$$1 {print $$4}' version=$(HSBUILD) $(abs_top_srcdir)/hotspot.map`"
  HS_CHANGESET = "`$(AWK) 'version==$$1 {print $$3}' version=$(HSBUILD) $(abs_top_srcdir)/hotspot.map`"
  HS_URL = "`$(AWK) 'version==$$1 {print $$2}' version=$(HSBUILD) $(abs_top_srcdir)/hotspot.map`"
else
  HS_SHA256SUM = $(HOTSPOT_SHA256SUM)
  HS_CHANGESET = $(HOTSPOT_CHANGESET)
  HS_URL = $(HOTSPOT_URL)
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
OPENJDK_TREE = stamps/overlay.stamp

# Target to ensure a patched OpenJDK bootstrap tree containing
# Zero & Shark, any other overlays and the bootstrapping patches
# is available in $(abs_top_builddir)/openjdk-boot
OPENJDK_BOOT_TREE = stamps/patch-jaxws.stamp

# Sources list

REWRITER_SRCS = $(top_srcdir)/rewriter/com/redhat/rewriter/ClassRewriter.java

# Patch list

ICEDTEA_PATCHES = \
	patches/text-relocations.patch \
	patches/demos.patch \
	patches/headers.patch \
	patches/gcc-suffix.patch \
	patches/memory-limits.patch \
	patches/sunsrc.patch \
	patches/libraries.patch \
	patches/print-lsb-release.patch \
	patches/jpegclasses.patch \
	patches/debian/uname.patch \
	patches/sparc-ptracefix.patch \
	patches/sparc-trapsfix.patch \
	patches/override-redirect-metacity.patch \
	patches/security-updates.patch \
	patches/hotspot-gcc-pr36917.patch \
	patches/alpha-fixes.patch \
	patches/alt-jar.patch \
	patches/use-idx_t.patch \
	patches/params-cast-size_t.patch \
	patches/clean-crypto.patch \
	patches/cacao/arch.patch \
	patches/signed-types.patch \
	patches/lc_ctype.patch \
	patches/tests-jdk.patch \
	patches/linker-libs-order.patch \
	patches/samejvm-safe.patch \
	patches/pr261.patch \
	patches/parisc-opt.patch \
        patches/security/6592792.patch \
        patches/security/6733959.patch \
        patches/security/6755943.patch \
	patches/jvmtiEnv.patch \
	patches/disable-intree-ec.patch \
	patches/sources.patch \
	patches/parisc.patch \
	patches/sh4-support.patch \
	patches/jtreg-httpTest.patch \
	patches/update-bootclasspath.patch

# Conditional patches

if WITH_RHINO
ICEDTEA_PATCHES += \
	patches/rhino.patch
endif

if BUILD_CACAO
ICEDTEA_PATCHES += \
	patches/cacao/launcher.patch \
	patches/cacao/version.patch \
	patches/cacao/jsig.patch \
	patches/cacao/6714758.patch \
	patches/cacao/memory.patch
endif

if ENABLE_CACAO
ICEDTEA_PATCHES += \
	patches/cacao/ignore-tests.patch
endif

if BUILD_JAMVM
ICEDTEA_PATCHES += \
	patches/jamvm/remove-sun.misc.Perf-debug-code.patch \
	patches/jamvm/jmm_GetLongAttribute_201.patch \
	patches/jamvm/ignore-assertions-and-verify-options.patch
endif

if ENABLE_PULSE_JAVA
ICEDTEA_PATCHES += \
	patches/pulse-soundproperties.patch
endif

if !USE_CVMI
ICEDTEA_PATCHES += \
	patches/javafiles.patch
endif

if ENABLE_SYSTEMTAP
ICEDTEA_PATCHES += patches/systemtap.patch \
	patches/systemtap-gcc-4.5.patch \
	patches/systemtap-alloc-size-workaround.patch
endif

if ENABLE_NSS
ICEDTEA_PATCHES += patches/nss-config.patch
else
ICEDTEA_PATCHES += patches/nss-not-enabled-config.patch
endif

ICEDTEA_PATCHES += $(DISTRIBUTION_PATCHES)

# Bootstrapping patches

ICEDTEA_BOOT_PATCHES = patches/boot/ant-javac.patch \
	patches/boot/corba-defs.patch \
	patches/boot/corba-idlj.patch \
	patches/boot/corba-no-gen.patch \
	patches/boot/corba-orb.patch \
	patches/boot/demos.patch \
	patches/boot/ecj-fphexconstants.patch \
	patches/boot/ecj-opts.patch \
	patches/boot/fontconfig.patch \
	patches/boot/generated-comments.patch \
	patches/boot/xbootclasspath.patch \
	patches/boot/icedteart.patch \
	patches/boot/jar.patch \
	patches/boot/symbols.patch \
	patches/boot/tobin.patch \
	patches/boot/test_gamma.patch \
	patches/boot/tools.jar.patch \
	patches/boot/jopt.patch \
	patches/boot/jaxp-dependency.patch \
	patches/boot/genstubs-nofork.patch \
	patches/boot/getannotation-cast.patch \
	patches/boot/break-processbuilder-dependency.patch \
	patches/boot/revert-6973616.patch \
	patches/boot/revert-6941137.patch \
	patches/boot/ecj-stringswitch.patch \
	patches/boot/langtools-force-old-jdk.patch \
	patches/boot/corba-dependencies.patch \
	patches/boot/jaxws-langtools-dependency.patch \
	patches/boot/jaxws-jdk-dependency.patch \
	patches/boot/hotspot-jdk-dependency.patch \
	patches/boot/ecj-multicatch.patch \
	patches/boot/ecj-trywithresources.patch \
	patches/boot/ecj-autoboxing.patch \
	patches/boot/xsltproc.patch

if CP39408_JAVAH
ICEDTEA_BOOT_PATCHES += patches/boot/pr39408.patch
endif

if CP40188_JAVAH
ICEDTEA_BOOT_PATCHES += patches/boot/pr40188.patch \
	patches/boot/javah.patch
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
ICEDTEA_BOOT_PATCHES += patches/boot/compile-for-7.patch
endif

ICEDTEA_BOOT_PATCHES += $(DISTRIBUTION_BOOT_PATCHES)

# Patches for JAXWS

JAXWS_PATCHES = patches/boot/jaxws-getdtdtype.patch

# OpenJDK build environment.
if ZERO_BUILD
  ICEDTEA_ZERO_BUILD = true
endif
if SHARK_BUILD
  ICEDTEA_SHARK_BUILD = true
endif

JDK_UPDATE_VERSION = $(shell echo $(OPENJDK_VERSION) | sed -e "s/^b//")
COMBINED_VERSION = $(JDK_UPDATE_VERSION)-$(OPENJDK_VERSION)

ICEDTEA_NAME = IcedTea8
if HAS_ICEDTEA_REVISION
ICEDTEA_REV = +${ICEDTEA_REVISION}
endif
if HAS_PKGVERSION
ICEDTEA_PKG = $(EMPTY) (${PKGVERSION})
endif

ICEDTEA_ENV = \
	ALT_JDK_IMPORT_PATH="$(BOOT_DIR)" \
	ANT="$(ANT)" \
        BUILD_NUMBER="$(OPENJDK_VERSION)" \
	JDK_UPDATE_VERSION="$(JDK_UPDATE_VERSION)" \
        JRE_RELEASE_VERSION="1.7.0_$(COMBINED_VERSION)" \
	MILESTONE="fcs" \
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
	FREETYPE2_HEADERS="$(FREETYPE2_CFLAGS)" \
	FT2_LIB="$(FREETYPE2_LIBS)" \
	ALT_PARALLEL_COMPILE_JOBS="$(PARALLEL_JOBS)" \
	HOTSPOT_BUILD_JOBS="$(PARALLEL_JOBS)" \
	JAVAC="" \
	JAVA_HOME="" \
	JDK_HOME="" \
	QUIETLY="" \
	ANT_RESPECT_JAVA_HOME="TRUE" \
	DISTRIBUTION_ID="$(DIST_ID)" \
	DERIVATIVE_ID="$(ICEDTEA_NAME) $(PACKAGE_VERSION)$(ICEDTEA_REV)" \
	ALT_JIBX_LIBS_PATH="$(JIBX_DEPS_DIR)" \
	DEBUG_CLASSFILES="true" \
	DEBUG_BINARIES="true" \
	DISABLE_INTREE_EC="true" \
	ALT_DROPS_DIR="$(abs_top_builddir)/drops" \
	ALT_OUTPUTDIR="$(BUILD_OUTPUT_DIR)" \
	VERBOSE="$(VERBOSE)" \
	STATIC_CXX="false"

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

if USE_SYSTEM_LCMS
ICEDTEA_ENV += \
	USE_SYSTEM_LCMS="true" \
	LCMS_LIBS="${LCMS2_LIBS}" \
	LCMS_CFLAGS="${LCMS2_CFLAGS}"
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
	XSLT="$(XSLTPROC)"

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
	$(top_srcdir)/generated/sun/tools/jconsole/*.java \
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

EXTRA_DIST = $(GENERATED_FILES) $(top_srcdir)/patches/*.patch \
	$(top_srcdir)/patches/boot/*.patch \
	$(top_srcdir)/patches/cacao/*.patch \
	$(top_srcdir)/patches/debian/*.patch \
	$(top_srcdir)/patches/jamvm/*.patch \
	$(top_srcdir)/patches/security/*.patch \
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
 clean-icedtea clean-icedtea-boot clean-clone clean-clone-boot \
 clean-bootstrap-directory-stage1 clean-bootstrap-directory-stage2 \
 clean-bootstrap-directory-symlink-stage1 clean-bootstrap-directory-symlink-stage2 \
 clean-extract clean-generated clean-native-ecj clean-hgforest clean-icedtea-stage2 \
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
	clean-drops jtregcheck clean-remove-intree-libraries \
	clean-jamvm clean-extract-jamvm clean-add-jamvm clean-add-jamvm-debug \
	clean-extract-hotspot clean-sanitise-openjdk

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
	$(ZIP) -r openjdk-$(OPENJDK_VERSION) openjdk/

# Creates archive of openjdk that is compliant with Free Software guidelines.
dist-openjdk-fsg: stamps/patch-fsg.stamp
	$(ZIP) -r openjdk-fsg-$(OPENJDK_VERSION) openjdk/

# OpenJDK Source Preparation Targets
# ==================================

# Check for forest support

stamps/hgforest.stamp:
if USE_HG
	if ! $(HG) fclone -h; \
          then \
          echo "No forest extension found. Please refer to" \
            "http://selenic.com/mercurial/wiki/index.cgi/ForestExtension"; \
          exit 1; \
        fi;
endif
	mkdir -p stamps 
	touch $@

clean-hgforest:
	rm -f stamps/hgforest.stamp

# Download OpenJDK sources.

stamps/download.stamp: stamps/download-drops.stamp stamps/download-openjdk.stamp \
 stamps/download-cacao.stamp stamps/download-jamvm.stamp
	mkdir -p stamps
	touch $@

clean-download: clean-drops clean-download-openjdk clean-download-cacao clean-download-jamvm
	rm -f stamps/download.stamp

stamps/download-jaxp-drop.stamp:
	mkdir -p drops
if USE_ALT_JAXP_DROP_ZIP
	ln -sf $(ALT_JAXP_DROP_ZIP) drops/$(JAXP_DROP_ZIP)
endif
	if ! echo "$(JAXP_DROP_SHA256SUM)  drops/$(JAXP_DROP_ZIP)" \
	  | $(SHA256SUM) --check ; \
	then \
	  if [ -f drops/$(JAXP_DROP_ZIP) ] ; \
	  then \
	    mv drops/$(JAXP_DROP_ZIP) drops/$(JAXP_DROP_ZIP).old ; \
	  fi ; \
	  $(WGET) $(JAXP_DROP_URL)/$(JAXP_DROP_ZIP) -O drops/$(JAXP_DROP_ZIP); \
	  if ! echo "$(JAXP_DROP_SHA256SUM)  drops/$(JAXP_DROP_ZIP)" \
	    | $(SHA256SUM) --check ; \
	  then echo "ERROR: Bad download of jaxp drop zip"; false; \
	  fi; \
	fi ;
	mkdir -p stamps
	touch $@

clean-download-jaxp-drop:
	rm -f drops/$(JAXP_DROP_ZIP)
	rm -f stamps/download-jaxp-drop.stamp

stamps/download-jaf-drop.stamp:
	mkdir -p drops
if USE_ALT_JAF_DROP_ZIP
	ln -sf $(ALT_JAF_DROP_ZIP) drops/$(JAF_DROP_ZIP)
endif
	if ! echo "$(JAF_DROP_SHA256SUM)  drops/$(JAF_DROP_ZIP)" \
	  | $(SHA256SUM) --check ; \
	then \
	  if [ -f drops/$(JAF_DROP_ZIP) ] ; \
	  then \
	    mv drops/$(JAF_DROP_ZIP) drops/$(JAF_DROP_ZIP).old ; \
	  fi ; \
	  $(WGET) $(JAF_DROP_URL)/$(JAF_DROP_ZIP) -O drops/$(JAF_DROP_ZIP); \
	  if ! echo "$(JAF_DROP_SHA256SUM)  drops/$(JAF_DROP_ZIP)" \
	    | $(SHA256SUM) --check ; \
	  then echo "ERROR: Bad download of jaf drop zip"; false; \
	  fi; \
	fi ;
	mkdir -p stamps
	touch $@

clean-download-jaf-drop:
	rm -f drops/$(JAF_DROP_ZIP)
	rm -f stamps/download-jaf-drop.stamp

stamps/download-jaxws-drop.stamp:
	mkdir -p drops
if USE_ALT_JAXWS_DROP_ZIP
	ln -sf $(ALT_JAXWS_DROP_ZIP) drops/$(JAXWS_DROP_ZIP)
endif
	if ! echo "$(JAXWS_DROP_SHA256SUM)  drops/$(JAXWS_DROP_ZIP)" \
	  | $(SHA256SUM) --check ; \
	then \
	  if [ -f drops/$(JAXWS_DROP_ZIP) ] ; \
	  then \
	    mv drops/$(JAXWS_DROP_ZIP) drops/$(JAXWS_DROP_ZIP).old ; \
	  fi ; \
	  $(WGET) $(JAXWS_DROP_URL)/$(JAXWS_DROP_ZIP) -O drops/$(JAXWS_DROP_ZIP); \
	  if ! echo "$(JAXWS_DROP_SHA256SUM)  drops/$(JAXWS_DROP_ZIP)" \
	    | $(SHA256SUM) --check ; \
	  then echo "ERROR: Bad download of jaxws drop zip"; false; \
	  fi; \
	fi ;
	mkdir -p stamps
	touch $@

clean-download-jaxws-drop:
	rm -f drops/$(JAXWS_DROP_ZIP)
	rm -f stamps/download-jaxws-drop.stamp

stamps/download-drops.stamp: stamps/download-jaxp-drop.stamp \
 stamps/download-jaxws-drop.stamp stamps/download-jaf-drop.stamp
	mkdir -p stamps
	touch $@

clean-drops: clean-download-jaxp-drop clean-download-jaf-drop \
 clean-download-jaxws-drop
	if [ -e drops ] ; then \
	  rmdir drops ; \
	fi
	rm -f stamps/download-drops.stamp

stamps/download-openjdk.stamp: stamps/hgforest.stamp
if !OPENJDK_SRC_DIR_FOUND
if USE_CLOSURES
	$(HG) fclone $(CLOSURES_HG_URL) openjdk; 
else
if USE_CVMI
	$(HG) fclone $(CVMI_HG_URL) openjdk; 
else
if USE_CACIOCAVALLO
	$(HG) fclone $(CACIOCAVALLO_HG_URL) openjdk;
else
if USE_BSD
	$(HG) fclone $(BSD_HG_URL) openjdk;
else
if USE_NIO2
	hg fclone $(NIO2_HG_URL) openjdk;
else
if USE_JDK7
	hg fclone $(OPENJDK_HG_URL) openjdk;
else
if USE_HG
if WITH_HGREV
	$(HG) fclone -r $(HGREV) $(ICEDTEA_HG_URL) openjdk;
else
	$(HG) fclone $(ICEDTEA_HG_URL) openjdk;
endif
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
	  $(WGET) $(OPENJDK_URL)/archive/$(OPENJDK_CHANGESET).tar.gz -O $(OPENJDK_SRC_ZIP); \
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
	  $(WGET) $(CORBA_URL)/archive/$(CORBA_CHANGESET).tar.gz -O $(CORBA_SRC_ZIP); \
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
	  $(WGET) $(JAXP_URL)/archive/$(JAXP_CHANGESET).tar.gz -O $(JAXP_SRC_ZIP); \
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
	  $(WGET) $(JAXWS_URL)/archive/$(JAXWS_CHANGESET).tar.gz -O $(JAXWS_SRC_ZIP); \
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
	  $(WGET) $(JDK_URL)/archive/$(JDK_CHANGESET).tar.gz -O $(JDK_SRC_ZIP); \
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
	  $(WGET) $(LANGTOOLS_URL)/archive/$(LANGTOOLS_CHANGESET).tar.gz -O $(LANGTOOLS_SRC_ZIP); \
	  if ! echo "$(LANGTOOLS_SHA256SUM)  $(LANGTOOLS_SRC_ZIP)" \
	    | $(SHA256SUM) --check ; \
	  then echo "ERROR: Bad download of langtools zip"; false; \
	  fi; \
	fi ;
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
	  $(WGET) $(HS_URL)/archive/$(HS_CHANGESET).tar.gz -O $(HOTSPOT_SRC_ZIP) ; \
	  if ! echo "$(HOTSPOT_SHA256SUM)  $(HOTSPOT_SRC_ZIP)" \
	    | $(SHA256SUM) --check ; \
	  then echo "ERROR: Bad download of HotSpot zip"; false; \
	  fi; \
	fi
endif
endif
endif
endif
endif
endif
endif
endif
	mkdir -p stamps
	touch $@

clean-download-openjdk:
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
	if [ ! -z $(HOTSPOT_SRC_ZIP) ] ; then \
	  rm -f $(HOTSPOT_SRC_ZIP) ; \
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

stamps/extract.stamp: stamps/extract-openjdk.stamp \
 stamps/extract-cacao.stamp stamps/extract-jaxws.stamp \
 stamps/remove-intree-libraries.stamp stamps/extract-jamvm.stamp \
 stamps/extract-hotspot.stamp stamps/sanitise-openjdk.stamp
	mkdir -p stamps
	touch $@

clean-extract: clean-extract-openjdk clean-extract-cacao clean-extract-jaxws \
 clean-extract-jamvm
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
	    mv $(ICEDTEA_PREFIX)-$(OPENJDK_CHANGESET) openjdk ; \
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
	touch stamps/extract-openjdk.stamp

clean-extract-openjdk: clean-overlay clean-versioning \
 clean-patch-fsg clean-remove-intree-libraries \
 clean-sanitise-openjdk clean-extract-hotspot
	rm -rf openjdk
	rm -f stamps/extract-openjdk.stamp

stamps/extract-hotspot.stamp: stamps/extract-openjdk.stamp
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

stamps/sanitise-openjdk.stamp: stamps/extract-openjdk.stamp \
 stamps/extract-hotspot.stamp
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

stamps/extract-jaxws.stamp: stamps/download-jaxws-drop.stamp \
 stamps/download-jaf-drop.stamp stamps/sanitise-openjdk.stamp
	set -e ; \
	if test "x$(NEED_JAXWS_SRC)" = "xtrue"; then \
	  (cd openjdk/jaxws ; \
	   $(ANT) -Ddrops.dir=$(abs_top_builddir)/drops \
	   -Doutput.dir=$(BUILD_OUTPUT_DIR)/jaxws \
	   source ; ) \
	fi
	mkdir -p stamps
	touch $@

clean-extract-jaxws:
	if [ -d $(BUILD_OUTPUT_DIR)/jaxws ] ; then \
	  (cd openjdk/jaxws ; \
	   $(ANT) -Doutput.dir=$(BUILD_OUTPUT_DIR)/jaxws clobber ; \
	   cd ../../); \
        fi
	mkdir -p stamps
	rm -f stamps/extract-jaxws.stamp

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
	if test x"$(PROJECT_NAME)" != "xjdk7"; then \
	  proj_suffix="-$(PROJECT_NAME)"; \
	fi ; \
	if test x"$(VERSION_SUFFIX)" != "x"; then \
	  ver_suffix="-$(VERSION_SUFFIX)"; \
	fi ; \
	sed -i "s#BUILD_VARIANT_RELEASE)#BUILD_VARIANT_RELEASE)$${proj_suffix}$${ver_suffix}#" \
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
	rm -f stamps/patch-jaxws.stamp
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

stamps/patch-jaxws.stamp: stamps/patch-boot.stamp
	mkdir -p stamps
	rm -f stamps/patch-jaxws.stamp.tmp
	touch stamps/patch-jaxws.stamp.tmp
if DTDTYPE_QNAME
	set -e ; \
	(cd $(BOOT_BUILD_OUTPUT_DIR)/jaxws; \
	 all_patches_ok=yes; \
	 for p in $(JAXWS_PATCHES) ; \
	 do \
	   if test x$${all_patches_ok} = "xyes" \
	     && echo Checking $$p \
	     && $(PATCH) -l -p0 --dry-run -s -t -f \
	       -F 0 < $(abs_top_srcdir)/$$p ; \
	   then \
	    echo Applying $$p ; \
	    $(PATCH) -l -p0 < $(abs_top_srcdir)/$$p ; \
	    if ! grep "^\* $$(basename $$p)" $(abs_top_srcdir)/HACKING \
	      >> $(abs_top_builddir)/stamps/patch-jaxws.stamp.tmp ; \
	    then \
	      echo "* $$(basename $$p): UNDOCUMENTED" \
	        >> $(abs_top_builddir)/stamps/patch-jaxws.stamp.tmp ; \
	    fi ; \
	   else \
	     test x$${all_patches_ok} = "xyes" && all_patches_ok=$$p ; \
	   fi ; \
	 done ; \
	 if ! test x$${all_patches_ok} = "xyes"; then \
	   echo ERROR patch $${all_patches_ok} FAILED! ; \
	   echo WARNING make clean-patch-jaxws before retrying a fix ; \
	   exit 2; \
	 fi )
endif
	mv stamps/patch-jaxws.stamp.tmp stamps/patch-jaxws.stamp

clean-patch-jaxws:
if DTDTYPE_QNAME
	if [ -e stamps/patch-jaxws.stamp ] ; then \
	  all_patches_ok=yes; \
	  cd $(BOOT_BUILD_OUTPUT_DIR)/jaxws; \
	  for p in $(JAXWS_PATCHES) ; \
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
	fi
endif
	rm -f stamps/patch-jaxws.stamp

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
	ln -s $(BOOT_BUILD_OUTPUT_DIR)/j2sdk-image/ $(STAGE2_BOOT_DIR)
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
 stamps/cacao.stamp $(PULSE_JAVA_TARGET) stamps/rewrite-rhino.stamp \
 stamps/jamvm.stamp
	$(ARCH_PREFIX) $(MAKE) \
	  $(ICEDTEA_ENV) \
	  -C openjdk/ \
	  $(ICEDTEA_BUILD_TARGET)
	mkdir -p $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	mkdir -p $(BUILD_OUTPUT_DIR)/j2re-image/lib/$(INSTALL_ARCH_DIR)
	mkdir -p $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/ext
	mkdir -p $(BUILD_OUTPUT_DIR)/j2re-image/lib/ext 
if ENABLE_PULSE_JAVA
	cp -pPRf $(PULSE_JAVA_NATIVE_BUILDDIR)/libpulse-java.so \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	cp -pPRf $(PULSE_JAVA_NATIVE_BUILDDIR)/libpulse-java.so \
	  $(BUILD_OUTPUT_DIR)/j2re-image/lib/$(INSTALL_ARCH_DIR)
	cp -pPRf pulse-java.jar \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/ext
	cp -pPRf pulse-java.jar \
	  $(BUILD_OUTPUT_DIR)/j2re-image/lib/ext
	(cd $(PULSE_JAVA_JAVA_SRCDIR) && \
	   $(ZIP) -qur $(BUILD_OUTPUT_DIR)/j2sdk-image/src.zip org )
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
	mkdir -p $(BUILD_OUTPUT_DIR)/j2sdk-image/tapset ; \
	grep "client IGNORE" $(BUILD_JRE_ARCH_DIR)/jvm.cfg; \
	if test $$? -eq 0; then \
	  sed -e '/\/client\/libjvm.so/d' \
	    < $(abs_top_builddir)/tapset/hotspot.stp \
	    > $(BUILD_OUTPUT_DIR)/j2sdk-image/tapset/hotspot.stp; \
	  sed -e '/\/client\/libjvm.so/d' \
	    < $(abs_top_builddir)/tapset/hotspot_jni.stp \
	    > $(BUILD_OUTPUT_DIR)/j2sdk-image/tapset/hotspot_jni.stp; \
	else \
	  cp $(abs_top_builddir)/tapset/hotspot.stp \
	    $(BUILD_OUTPUT_DIR)/j2sdk-image/tapset/hotspot.stp; \
	  cp $(abs_top_builddir)/tapset/hotspot_jni.stp \
	    $(BUILD_OUTPUT_DIR)/j2sdk-image/tapset/hotspot_jni.stp; \
	fi; \
	cp $(abs_top_builddir)/tapset/jstack.stp \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/tapset/jstack.stp
endif
	cp $(abs_top_builddir)/nss.cfg \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/security;
if WITH_TZDATA_DIR
	cp $(abs_top_builddir)/tz.properties \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib;
endif
if ENABLE_JAMVM
	printf -- '-jamvm ALIASED_TO -server\n' >> $(BUILD_JRE_ARCH_DIR)/jvm.cfg
endif
	@echo "IcedTea is served:" $(BUILD_OUTPUT_DIR)
	mkdir -p stamps
	touch $@

stamps/icedtea-debug.stamp: stamps/bootstrap-directory-symlink-stage2.stamp \
 stamps/download.stamp stamps/extract.stamp $(OPENJDK_TREE) \
 stamps/cacao.stamp $(PULSE_JAVA_TARGET) stamps/rewrite-rhino.stamp \
 stamps/jamvm.stamp
	$(ARCH_PREFIX) $(MAKE) \
	  $(ICEDTEA_ENV) \
	  -C openjdk/ \
	  $(ICEDTEA_DEBUG_BUILD_TARGET)
	mkdir -p $(DEBUG_BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	mkdir -p $(DEBUG_BUILD_OUTPUT_DIR)/j2re-image/lib/$(INSTALL_ARCH_DIR)
	mkdir -p $(DEBUG_BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/ext
	mkdir -p $(DEBUG_BUILD_OUTPUT_DIR)/j2re-image/lib/ext 
if ENABLE_PULSE_JAVA
	cp -pPRf $(PULSE_JAVA_NATIVE_BUILDDIR)/libpulse-java.so \
	  $(DEBUG_BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	cp -pPRf $(PULSE_JAVA_NATIVE_BUILDDIR)/libpulse-java.so \
	  $(DEBUG_BUILD_OUTPUT_DIR)/j2re-image/lib/$(INSTALL_ARCH_DIR)
	cp -pPRf pulse-java.jar \
	  $(DEBUG_BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/ext
	cp -pPRf pulse-java.jar \
	  $(DEBUG_BUILD_OUTPUT_DIR)/j2re-image/lib/ext
	(cd $(PULSE_JAVA_JAVA_SRCDIR) && \
	   $(ZIP) -qur $(DEBUG_BUILD_OUTPUT_DIR)/j2sdk-image/src.zip org )
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
	mkdir -p $(DEBUG_BUILD_OUTPUT_DIR)/j2sdk-image/tapset ; \
	grep "client IGNORE" $(BUILD_JRE_ARCH_DIR)/jvm.cfg; \
	if test $$? -eq 0; then \
	  sed -e '/\/client\/libjvm.so/d' \
	    < $(abs_top_builddir)/tapset/hotspot.stp \
	    > $(DEBUG_BUILD_OUTPUT_DIR)/j2sdk-image/tapset/hotspot.stp; \
	  sed -e '/\/client\/libjvm.so/d' \
	    < $(abs_top_builddir)/tapset/hotspot_jni.stp \
	    > $(DEBUG_BUILD_OUTPUT_DIR)/j2sdk-image/tapset/hotspot_jni.stp; \
	else \
	  cp $(abs_top_builddir)/tapset/hotspot.stp \
	    $(DEBUG_BUILD_OUTPUT_DIR)/j2sdk-image/tapset/hotspot.stp; \
	  cp $(abs_top_builddir)/tapset/hotspot_jni.stp \
	    $(DEBUG_BUILD_OUTPUT_DIR)/j2sdk-image/tapset/hotspot_jni.stp; \
	fi; \
	cp $(abs_top_builddir)/tapset/jstack.stp \
	  $(DEBUG_BUILD_OUTPUT_DIR)/j2sdk-image/tapset/jstack.stp
endif
	cp $(abs_top_builddir)/nss.cfg \
	  $(DEBUG_BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/security;
if WITH_TZDATA_DIR
	cp $(abs_top_builddir)/tz.properties \
	  $(DEBUG_BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib;
endif
if ENABLE_JAMVM
	printf -- '-jamvm ALIASED_TO -server\n' >> $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg
endif
	@echo "IcedTea (debug build) is served:" \
	  $(DEBUG_BUILD_OUTPUT_DIR)
	mkdir -p stamps
	touch $@

clean-icedtea: 
	rm -rf $(BUILD_OUTPUT_DIR)
	rm -f stamps/icedtea.stamp stamps/icedtea-debug.stamp

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

stamps/icedtea-boot.stamp: stamps/bootstrap-directory-symlink-stage1.stamp \
 stamps/download.stamp $(OPENJDK_BOOT_TREE) stamps/cacao.stamp \
 stamps/rewrite-rhino.stamp stamps/jamvm.stamp stamps/generated.stamp
	$(ARCH_PREFIX) $(MAKE) \
	  $(ICEDTEA_ENV_BOOT) \
	  -C openjdk-boot \
	  $(ICEDTEA_BUILD_TARGET)
	@echo "Bootstrapped IcedTea is served:" \
	  $(BOOT_BUILD_OUTPUT_DIR)
	mkdir -p stamps
	touch $@

# Rebuild targets

rebuild:
if BOOTSTRAPPING
	rm -f stamps/icedtea-boot.stamp
	rm -f stamps/icedtea-stage1.stamp
	$(ARCH_PREFIX) $(MAKE) icedtea-stage1
	@echo "bootstrap-poured IcedTea is served (again):" $(BOOT_BUILD_OUTPUT_DIR)/j2sdk-image
else
	rm -f stamps/icedtea.stamp
	rm -f stamps/icedtea-stage2.stamp
	$(ARCH_PREFIX) $(MAKE) icedtea-stage2
	@echo "IcedTea is served (again):" $(BUILD_OUTPUT_DIR)/j2sdk-image
endif

hotspot:
if BOOTSTRAPPING
	rm -f stamps/icedtea-boot.stamp
	rm -f stamps/icedtea-stage1.stamp
	$(ARCH_PREFIX) $(MAKE) ICEDTEA_BUILD_TARGET=hotspot icedtea-stage1
	mkdir -p \
	   $(BOOT_BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)/server
	cp $(BOOT_BUILD_OUTPUT_DIR)/$(SERVER_DIR)/libjvm.so \
	   $(BOOT_BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)/server
	@echo "HotSpot is served:" $(BOOT_BUILD_OUTPUT_DIR)/j2sdk-image
else
	rm -f stamps/icedtea.stamp
	rm -f stamps/icedtea-stage2.stamp
	$(ARCH_PREFIX) $(MAKE) ICEDTEA_BUILD_TARGET=hotspot icedtea-stage2
	mkdir -p \
	   $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)/server
	cp $(BUILD_OUTPUT_DIR)/$(SERVER_DIR)/libjvm.so \
	   $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)/server
	@echo "HotSpot is served:" $(BUILD_OUTPUT_DIR)/j2sdk-image
endif

clean-icedtea-boot:
	rm -rf $(BOOT_BUILD_OUTPUT_DIR)
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
	./autogen.sh --with-java-runtime-library=openjdk \
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
	  --with-java-runtime-library=openjdk \
	  --with-java-runtime-library-prefix=$(abs_top_builddir)/openjdk \
	  --with-java-runtime-library-classes=$(RUNTIME) \
	  --enable-jre-layout $(CACAO_CONFIGURE_ARGS); \
	$(ARCH_PREFIX) $(MAKE) -j$(PARALLEL_JOBS) install
	ln -s server $(abs_top_builddir)/cacao/install/jre/lib/$(INSTALL_ARCH_DIR)/client
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
	--with-jdk-home=$(BUILD_OUTPUT_DIR)/j2sdk-image \
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
	  $(if $(findstring --with-$(i)-src-zip=, $(CONFIGURE_ARGS)),, --with-$(i)-src-zip=$(abs_top_builddir)/$(i).tar.gz))

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
		-jdk:$(BUILD_OUTPUT_DIR)/j2sdk-image \
		$(ICEDTEA_JTREG_OPTIONS) \
		`pwd`/openjdk/hotspot/test \
	    | tee test/$@.log

check-langtools: stamps/jtreg.stamp
	mkdir -p test/langtools/JTwork test/langtools/JTreport
	$(BOOT_DIR)/bin/java -jar test/jtreg.jar -v1 -a -ignore:quiet \
		-w:test/langtools/JTwork -r:test/langtools/JTreport \
		$${ICEDTEA_JTREG_OTHERVM:--samevm} \
		-jdk:$(BUILD_OUTPUT_DIR)/j2sdk-image \
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
		-jdk:$(BUILD_OUTPUT_DIR)/j2sdk-image \
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
	for dir in $(ICEDTEA_BOOTSTRAP_DIRS) ; \
	do \
	$(FIND) $(abs_top_builddir)/$$dir -name '*.java' >> $@; \
	done
	for files in $(ICEDTEA_BOOTSTRAP_CLASSES) ; \
	do \
	  echo $$files >> $@ ; \
	done

stamps/rt-class-files.stamp: $(INITIAL_BOOTSTRAP_LINK_STAMP) rt-source-files.txt
	mkdir -p lib/rt
if LACKS_SUN_AWT_TOOLKIT
	$(BOOT_DIR)/bin/javac $(IT_JAVACFLAGS) -d lib/rt \
	  -classpath $(SYSTEM_JDK_DIR)/jre/lib/rt.jar \
          -sourcepath '$(SOURCEPATH_DIRS)' \
          -bootclasspath \'\' @rt-source-files.txt ;
endif
	mkdir -p stamps
	touch $@

stamps/rt.stamp: stamps/rt-class-files.stamp
if LACKS_SUN_AWT_TOOLKIT
	(set -e ; \
	 cd lib/rt ; \
	 for dirs in $(ICEDTEA_BOOTSTRAP_RESOURCES) ; \
	   do \
	   destpath=`echo $$dirs|sed -e 's#$(LANGTOOLS)/##' -e 's#$(SHARE)/##'` ; \
	   mkdir -p `dirname $$destpath` ; \
	   cp -a ../../$$dirs $$destpath ; \
	 done ; \
	 $(ZIP) -qur $(STAGE1_BOOT_DIR)/jre/lib/rt.jar \
	 com java javax sun );
endif
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

download-drops: stamps/download-drops.stamp

download-jamvm: stamps/download-jamvm.stamp

download-jaf-drop: stamps/download-jaf-drop.stamp

download-jaxp-drop: stamps/download-jaxp-drop.stamp

download-jaxws-drop: stamps/download-jaxws-drop.stamp

download-openjdk: stamps/download-openjdk.stamp

extract: stamps/extract.stamp

extract-cacao: stamps/extract-cacao.stamp

extract-openjdk: stamps/extract-openjdk.stamp

extract-hotspot: stamps/extract-hotspot.stamp

generated: stamps/generated.stamp

icedtea: stamps/icedtea.stamp

icedtea-boot: stamps/icedtea-boot.stamp

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

patch-jaxws: stamps/patch-jaxws.stamp

pulse-java: $(PULSE_JAVA_TARGET)

rewriter: stamps/rewriter.stamp

rewrite-rhino: stamps/rewrite-rhino.stamp

rt: stamps/rt.stamp

rt-class-files: stamps/rt-class-files.stamp

sanitise-openjdk: stamps/sanitise-openjdk.stamp
