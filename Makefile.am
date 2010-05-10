# Dependencies

OPENJDK_VERSION = b89

CORBA_CHANGESET = e805b4155d76
HOTSPOT_CHANGESET = e431f9da7dc9
JAXP_CHANGESET  = 826bafcb6c4a
JAXWS_CHANGESET = 1661166c82dc
JDK_CHANGESET = c1cee45daf4d
LANGTOOLS_CHANGESET = a0499f48ec96
OPENJDK_CHANGESET = 195fcceefddc

CORBA_SHA256SUM = 870e2eac993a4eef48197ed7309cab176b56a994add1de215a026599052627cb
HOTSPOT_SHA256SUM = 4783b1b85239ac60b16eac7d184e51a9350670fec6f54a2e986761b7ded92994
JAXP_SHA256SUM = 42d004e51a1f01d146ad230ce48996ddf8da3719fe571a41653f431d6b2e8a7b
JAXWS_SHA256SUM = 4523909cd46fd61b5c6670bf932099d5899236a37a4058e7b2bcb855e503a0e4
JDK_SHA256SUM = b6d70bc41bbc7d02977ca4b0455bef765a3e8670dfa2f413a5b8bd91e77ee32a
LANGTOOLS_SHA256SUM = 96f6be5d89195353f55ef775e0dec9bd9a5da7f869aba6ed428e4a2cac806991
OPENJDK_SHA256SUM = 93c536e6bc4e962050a00321e88e694fc4e0000e2ad887b8de22830bfda2649f

CACAO_VERSION = 0.99.4
CACAO_SHA256SUM = c6090c1d2ed3c60acda0076b62563598267be8dab270ce49ee1035002a2510cf
CACAO_BASE_URL = http://www.complang.tuwien.ac.at
CACAO_URL = $(CACAO_BASE_URL)/cacaojvm/download/cacao-$(CACAO_VERSION)/
CACAO_SRC_ZIP = cacao-$(CACAO_VERSION).tar.bz2

NETBEANS_PROFILER_URL = https://visualvm.dev.java.net/files/documents/7163/145168/
NETBEANS_PROFILER_SRC_ZIP = netbeans-profiler-visualvm_release67.tar.gz
NETBEANS_PROFILER_SHA256SUM = 611736b4e6dac2f7ddb669a23a474fb052f4141c04dc2ba5888b9e9db61fe59b

VISUALVM_URL = https://visualvm.dev.java.net/files/documents/7163/145167/
VISUALVM_SRC_ZIP = visualvm_121-src.tar.gz
VISUALVM_SHA256SUM = b561207d85da4916e809cc4c104098e1ca979b30192c207b9d6087a31c91b8b0

JAXWS_DROP_URL = http://kenai.com/projects/jdk7-drops/downloads/download
JAXWS_DROP_ZIP = jdk7-jaxws-2009_09_28.zip
JAXWS_DROP_SHA256SUM = 272756b999e9ae2435da8d58e259562e61126e1c5b1f9421fe65423217cef8fd
JAF_DROP_ZIP = jdk7-jaf-2009_08_28.zip
JAF_DROP_SHA256SUM = 8c81940b3017c0058367d7e7453123442f2cdd77c2bfe74acd9035f97e4db8e8
JAXP_DROP_URL = https://jaxp.dev.java.net/files/documents/913/147490
JAXP_DROP_ZIP = jdk7-jaxp-m6.zip
JAXP_DROP_SHA256SUM = 3d9eb7ae71f5365e2a786ba270413c6cb7dc8d1137f56995c087908b3df8446c

ICEDTEA_HG_URL = http://hg.openjdk.java.net/icedtea/jdk7
OPENJDK_HG_URL = http://hg.openjdk.java.net/jdk7/jdk7
CVMI_HG_URL = http://hg.openjdk.java.net/cvmi/cvmi/
CLOSURES_HG_URL = http://hg.openjdk.java.net/closures/closures/
CACIOCAVALLO_HG_URL = http://hg.openjdk.java.net/caciocavallo/jdk7/
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

OS_DIR = $(BUILD_OS_DIR)-$(BUILD_ARCH_DIR)
BUILD_OUTPUT_DIR = $(abs_top_builddir)/openjdk/build/$(OS_DIR)
BOOT_BUILD_OUTPUT_DIR = openjdk-boot/build/$(OS_DIR)
ICEDTEA_BUILD_DIR = \
	$(abs_top_builddir)/$(BUILD_OUTPUT_DIR)
ICEDTEA_BOOT_DIR= \
	$(abs_top_builddir)/bootstrap/jdk1.6.0
ICEDTEA_RT = \
	$(ICEDTEA_BOOT_DIR)/jre/lib/rt.jar
NETX_CLASSES = \
	$(abs_top_builddir)/netx.build
NETX_JAR = \
	$(NETX_CLASSES)/lib/classes.jar
NETX_SRC = \
	$(NETX_CLASSES)/lib/src.zip
ICEDTEA_ENDORSED_DIR = \
	$(ICEDTEA_BOOT_DIR)/lib/endorsed
ICEDTEA_BUILD_DIR_BOOT = \
	$(abs_top_builddir)/$(BOOT_BUILD_OUTPUT_DIR)/
ICEDTEA_CLS_DIR_BOOT = \
	$(abs_top_builddir)/$(BOOT_BUILD_OUTPUT_DIR)/classes
SERVER_DIR = hotspot/import/jre/lib/$(INSTALL_ARCH_DIR)/server
BUILD_JRE_ARCH_DIR = $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
BUILD_DEBUG_JRE_ARCH_DIR = $(BUILD_OUTPUT_DIR)-debug/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)

# Source directories

SHARE = openjdk-boot/jdk/src/share/classes
SOLARIS = openjdk-boot/jdk/src/solaris/classes
LANGTOOLS = openjdk-boot/langtools/src/share/classes
CORBA = openjdk-boot/corba/src/share/classes
JAXWS = $(BUILD_OUTPUT_DIR)/jaxws/drop/jaxws_src/src

NETX_SRCDIR = $(abs_top_srcdir)/netx
NETX_RESOURCE_DIR=$(NETX_SRCDIR)/net/sourceforge/jnlp/resources

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

SOURCEPATH_DIRS = $(abs_top_builddir)/generated:$(OPENJDK_SOURCEPATH_DIRS)

# Sources used from OpenJDK.
if LACKS_SUN_AWT_TOOLKIT
ICEDTEA_BOOTSTRAP_DIRS = \
	$(SHARE)/com/sun/jdi \
	$(SHARE)/com/sun/tools/jdi \
	$(SHARE)/com/sun/net/httpserver \
	$(SHARE)/java/io \
	$(SHARE)/java/security/cert \
	$(SHARE)/java/util \
	$(SHARE)/java/rmi \
	$(SHARE)/javax/net/ssl \
	$(SHARE)/javax/rmi/ssl \
	$(SHARE)/javax/script \
	$(SHARE)/javax/security/auth/kerberos \
	$(SHARE)/javax/security/sasl \
	$(SHARE)/javax/swing/plaf/basic \
	$(SHARE)/sun/awt/ \
	$(SHARE)/sun/rmi/rmic \
	$(SHARE)/sun/tools/java \
	$(LANGTOOLS)/com/sun/mirror \
	$(LANGTOOLS)/com/sun/tools/apt

ICEDTEA_BOOTSTRAP_RESOURCES = \
	$(LANGTOOLS)/com/sun/tools/javac/resources

endif

# Settings for javac

IT_LANGUAGE_SOURCE_VERSION=5
IT_CLASS_TARGET_VERSION=5
IT_JAVAC_SETTINGS=-g -encoding utf-8 $(JAVACFLAGS) $(MEMORY_LIMIT) $(PREFER_SOURCE)
IT_JAVACFLAGS=$(IT_JAVAC_SETTINGS) -source $(IT_LANGUAGE_SOURCE_VERSION) -target $(IT_CLASS_TARGET_VERSION)

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

if ENABLE_NPPLUGIN
PLUGIN_SRCDIR=$(abs_top_srcdir)/plugin/icedteanp
PLUGIN_BUILD_DIR=$(abs_top_builddir)/plugin.build/icedteanp
ICEDTEANPPLUGIN_TARGET = $(PLUGIN_BUILD_DIR)/IcedTeaNPPlugin.so
ICEDTEAPLUGIN_CLEAN = clean-IcedTeaNPPlugin
PLUGIN_PATCH = patches/icedtea-liveconnect.patch
LIVECONNECT_DIR = sun/applet netscape
JNLP_ABOUT_TARGET = extra-lib/about.jar
LIVECONNECT_CLASSES = $(abs_top_builddir)/liveconnect
LIVECONNECT_JAR = $(LIVECONNECT_CLASSES)/lib/classes.jar
LIVECONNECT_SRC = $(LIVECONNECT_CLASSES)/lib/src.zip
else
if ENABLE_PLUGIN
PLUGIN_SRCDIR=$(abs_top_srcdir)/plugin/icedtea
PLUGIN_BUILD_DIR=$(abs_top_builddir)/plugin.build/icedtea
ICEDTEAPLUGIN_TARGET = $(PLUGIN_BUILD_DIR)/IcedTeaPlugin.so
ICEDTEAPLUGIN_CLEAN = clean-IcedTeaPlugin
PLUGIN_PATCH = patches/icedtea-liveconnect.patch
LIVECONNECT_DIR = sun/applet netscape
JNLP_ABOUT_TARGET = extra-lib/about.jar
LIVECONNECT_CLASSES = $(abs_top_builddir)/liveconnect
LIVECONNECT_JAR = $(LIVECONNECT_CLASSES)/lib/classes.jar
LIVECONNECT_SRC = $(LIVECONNECT_CLASSES)/lib/src.zip
else
PLUGIN_PATCH = patches/icedtea-plugin.patch
endif
endif

if ENABLE_PULSE_JAVA
# include the makefile in pulseaudio subdir
PULSE_JAVA_DIR = $(abs_top_srcdir)/pulseaudio
PULSE_JAVA_NATIVE_SRCDIR = $(PULSE_JAVA_DIR)/src/native
PULSE_JAVA_BUILDDIR = $(abs_top_builddir)/pulseaudio
PULSE_JAVA_NATIVE_BUILDDIR = $(PULSE_JAVA_BUILDDIR)/native
PULSE_JAVA_JAVA_SRCDIR = $(PULSE_JAVA_DIR)/src/java
PULSE_JAVA_CLASS_DIR = $(PULSE_JAVA_BUILDDIR)/classes
else 
PULSE_JAVA_DIR =
PULSE_JAVA_NATIVE_SRCDIR =
PULSE_JAVA_NATIVE_BUILDDIR =
PULSE_JAVA_JAVA_SRCDIR =
PULSE_JAVA_CLASS_DIR =
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

if WITH_CACAO
  ICEDTEA_BUILD_TARGET=jdk_only
  ICEDTEA_DEBUG_BUILD_TARGET=jdk_fastdebug_only
else
  ICEDTEA_BUILD_TARGET=
  ICEDTEA_DEBUG_BUILD_TARGET=debug_build
endif

if BOOTSTRAPPING
  BOOTSTRAP_DIRECTORY_STAMP = stamps/bootstrap-directory-stage2.stamp
  ICEDTEA_HOME = $(abs_top_builddir)/bootstrap/icedtea
  INITIAL_BOOTSTRAP_LINK_STAMP = stamps/bootstrap-directory-symlink-stage1.stamp
else
  BOOTSTRAP_DIRECTORY_STAMP =
  ICEDTEA_HOME = $(SYSTEM_JDK_DIR)
  INITIAL_BOOTSTRAP_LINK_STAMP = stamps/bootstrap-directory-symlink-stage2.stamp
endif

# Target to ensure a patched OpenJDK tree containing Zero & Shark
# and any overlays is available in $(abs_top_builddir)/openjdk
OPENJDK_TREE = stamps/overlay.stamp

# Target to ensure a patched OpenJDK bootstrap tree containing
# Zero & Shark, any other overlays and the bootstrapping patches
# is available in $(abs_top_builddir)/openjdk-boot
OPENJDK_BOOT_TREE = stamps/patch-boot.stamp

# Patch list

ICEDTEA_PATCHES = \
	patches/hotspot/$(HSBUILD)/icedtea-version.patch \
	patches/icedtea-copy-plugs.patch \
	patches/hotspot/$(HSBUILD)/icedtea-text-relocations.patch \
	patches/icedtea-ssl.patch \
	$(PLUGIN_PATCH) \
	patches/icedtea-webstart.patch \
	patches/icedtea-webstart-umask.patch \
	patches/icedtea-rmi_amd64.patch \
	patches/icedtea-tools.patch \
	patches/icedtea-demos.patch \
	patches/icedtea-headers.patch \
	patches/hotspot/$(HSBUILD)/icedtea-headers.patch \
	patches/icedtea-gcc-suffix.patch \
	patches/hotspot/$(HSBUILD)/icedtea-gcc-suffix.patch \
	patches/icedtea-bytebuffer-compact.patch \
	patches/hotspot/$(HSBUILD)/icedtea-memory-limits.patch \
	patches/icedtea-sunsrc.patch \
	patches/icedtea-libraries.patch \
	patches/icedtea-print-lsb-release.patch \
	patches/icedtea-jpegclasses.patch \
	patches/debian/icedtea-uname.patch \
	patches/icedtea-freetypeversion.patch \
	patches/hotspot/$(HSBUILD)/icedtea-sparc-ptracefix.patch \
	patches/hotspot/$(HSBUILD)/icedtea-sparc-trapsfix.patch \
	patches/icedtea-override-redirect-metacity.patch \
	patches/icedtea-shark-build.patch \
	patches/hotspot/$(HSBUILD)/icedtea-shark-build.patch \
	patches/icedtea-security-updates.patch \
	patches/icedtea-hotspot-gcc-pr36917.patch \
	patches/icedtea-alpha-fixes.patch \
	patches/icedtea-alt-jar.patch \
	patches/icedtea-jdk-use-ssize_t.patch \
	patches/hotspot/$(HSBUILD)/icedtea-use-idx_t.patch \
	patches/hotspot/$(HSBUILD)/icedtea-params-cast-size_t.patch \
	patches/icedtea-clean-crypto.patch \
	patches/hotspot/$(HSBUILD)/icedtea-shark.patch \
	$(GCC_PATCH) \
	patches/cacao/arch.patch \
	patches/icedtea-signed-types.patch \
	patches/icedtea-lc_ctype.patch \
	patches/icedtea-tests-jdk.patch \
	patches/icedtea-stroker-finish.patch \
	patches/icedtea-linker-libs-order.patch \
	patches/icedtea-testenv.patch \
	patches/icedtea-samejvm-safe.patch \
	patches/icedtea-pr261.patch \
	patches/icedtea-doc-headers.patch \
	patches/hotspot/$(HSBUILD)/icedtea-parisc-opt.patch \
        patches/security/icedtea-6592792.patch \
        patches/security/icedtea-6733959.patch \
        patches/security/icedtea-6755943.patch \
	patches/hotspot/$(HSBUILD)/icedtea-includedb.patch \
	patches/icedtea-demo-swingapplet.patch \
	patches/icedtea-jvmtiEnv.patch \
	patches/icedtea-netx.patch \
	patches/icedtea-disable-intree-ec.patch \
	patches/icedtea-sources.patch \
	patches/no-precompiled.patch

# Conditional patches

if WITH_RHINO
ICEDTEA_PATCHES += \
	patches/icedtea-rhino.patch
endif

if BUILD_CACAO
ICEDTEA_PATCHES += \
	patches/cacao/launcher.patch \
	patches/cacao/no-mmap-first-page.patch \
	patches/cacao/native-resolve.patch \
	patches/cacao/xxoption-warning.patch \
	patches/cacao/no-strict-aliasing.patch \
	patches/cacao/openjdk7.patch \
	patches/cacao/version.patch \
	patches/cacao/versioninfo.patch
endif

if WITH_CACAO
ICEDTEA_PATCHES += \
	patches/cacao/ignore-tests.patch
endif

if ENABLE_PULSE_JAVA
ICEDTEA_PATCHES += \
	patches/icedtea-pulse-soundproperties.patch
endif

if ENABLE_XRENDER
# match against $(abs_top_srcdir) and then remove it again,
# because stamps/patch.stamp target will also add it
XRENDER_PATCHES = patches/xrender/icedtea-???.patch
XRENDER_PATCH_FILES = $(sort $(wildcard $(abs_top_srcdir)/$(XRENDER_PATCHES)))
ICEDTEA_PATCHES += $(sort $(subst $(abs_top_srcdir)/,,$(XRENDER_PATCH_FILES)))
endif

if !USE_CVMI
ICEDTEA_PATCHES += \
	patches/icedtea-javafiles.patch
endif

if ENABLE_SYSTEMTAP
ICEDTEA_PATCHES += patches/icedtea-systemtap.patch
endif

if ENABLE_PLUGIN
ICEDTEA_PATCHES += patches/icedtea-liveconnect-dist.patch
else
if ENABLE_NPPLUGIN
ICEDTEA_PATCHES += patches/icedtea-liveconnect-dist.patch
endif
endif

if ENABLE_NSS
ICEDTEA_PATCHES += patches/icedtea-nss-config.patch
endif

ICEDTEA_PATCHES += $(DISTRIBUTION_PATCHES)

# Bootstrapping patches

ICEDTEA_BOOT_PATCHES = patches/boot/ant-javac.patch \
	patches/boot/corba-defs.patch \
	patches/boot/corba-idlj.patch \
	patches/boot/corba-libgcj.patch \
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
	patches/boot/javah.patch \
	patches/boot/symbols.patch \
	patches/boot/tobin.patch \
	patches/boot/icedtea-hotspot-$(HSBUILD).patch \
	patches/boot/icedtea-jopt.patch \
	patches/boot/icedtea-dyn.patch \
	patches/boot/icedtea-jaxp-dependency.patch \
	patches/boot/genstubs-nofork.patch

if CP39408_JAVAH
ICEDTEA_BOOT_PATCHES += patches/boot/icedtea-pr39408.patch
endif

if CP40188_JAVAH
ICEDTEA_BOOT_PATCHES += patches/boot/icedtea-pr40188.patch
endif

if CP40616
ICEDTEA_BOOT_PATCHES += patches/boot/icedtea-pr40616.patch
endif

# Patches for when java.util.Scanner is missing
# or has the bug #40630 (all these classes rely
# on Scanner#hasNextLine() looping behaviour)
SCANNER_PATCHES = patches/boot/icedtea-spp.patch \
	patches/boot/icedtea-sbcs.patch \
	patches/boot/icedtea-dbcs.patch

if LACKS_JAVA_UTIL_SCANNER
ICEDTEA_BOOT_PATCHES += $(SCANNER_PATCHES)
else
if CP40630
ICEDTEA_BOOT_PATCHES += $(SCANNER_PATCHES)
endif
endif

if JAVAC_LACKS_DIAMOND
ICEDTEA_BOOT_PATCHES += patches/boot/ecj-diamond.patch
endif

# Patches for JAXWS

JAXWS_PATCHES = patches/icedtea-jaxws-getdtdtype.patch

# OpenJDK build environment.
if ZERO_BUILD
  ICEDTEA_ZERO_BUILD = true
else
  ICEDTEA_ZERO_BUILD =
endif
if SHARK_BUILD
  ICEDTEA_SHARK_BUILD = yes
else
  ICEDTEA_SHARK_BUILD =
endif

JDK_UPDATE_VERSION = $(shell echo $(OPENJDK_VERSION) | sed -e "s/^b//")
COMBINED_VERSION = $(JDK_UPDATE_VERSION)-$(OPENJDK_VERSION)

ICEDTEA_ENV = \
	ALT_JDK_IMPORT_PATH="$(ICEDTEA_BOOT_DIR)" \
	ANT="$(ANT)" \
        BUILD_NUMBER="$(OPENJDK_VERSION)" \
	JDK_UPDATE_VERSION="$(JDK_UPDATE_VERSION)" \
        JRE_RELEASE_VERSION="1.7.0_$(COMBINED_VERSION)" \
	MILESTONE="fcs" \
	LANG="C" \
	PATH="$(abs_top_builddir)/bootstrap/jdk1.6.0/bin:$(OS_PATH):$$PATH" \
	ALT_BOOTDIR="$(ICEDTEA_BOOT_DIR)" \
	BUILD_ARCH_DIR="$(BUILD_ARCH_DIR)" \
	ICEDTEA_RT="$(ICEDTEA_RT)" \
	ICEDTEA_ENDORSED_DIR="$(ICEDTEA_ENDORSED_DIR)" \
	ENDORSED="-Djava.endorsed.dirs=$(ICEDTEA_ENDORSED_DIR)" \
	CLASSPATH="" \
	LD_LIBRARY_PATH="" \
	ZERO_BUILD="$(ICEDTEA_ZERO_BUILD)" \
	ICEDTEA_SHARK_BUILD="$(ICEDTEA_SHARK_BUILD)" \
	ZERO_LIBARCH="$(ZERO_LIBARCH)" \
	ARCH_DATA_MODEL="$(ZERO_BITSPERWORD)" \
	ZERO_ENDIANNESS="$(ZERO_ENDIANNESS)" \
	ZERO_ARCHDEF="$(ZERO_ARCHDEF)" \
	ZERO_ARCHFLAG="$(ZERO_ARCHFLAG)" \
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
	RHINO_JAR="$(RHINO_JAR)" \
	DISTRIBUTION_ID="$(DIST_ID)" \
	ALT_JIBX_LIBS_PATH="$(JIBX_DEPS_DIR)" \
	ALT_NETX_DIST="$(NETX_CLASSES)" \
	ALT_LIVECONNECT_DIST="$(LIVECONNECT_CLASSES)" \
	DEBUG_CLASSFILES="true" \
	DEBUG_BINARIES="true" \
	DISABLE_INTREE_EC="true" \
	ALT_DROPS_DIR="$(abs_top_builddir)/drops"

if WITH_CACAO
ICEDTEA_ENV += \
	ALT_HOTSPOT_IMPORT_PATH="$(CACAO_IMPORT_PATH)"
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

# OpenJDK boot build environment.
ICEDTEA_ENV_BOOT = $(ICEDTEA_ENV) \
	BOOTCLASSPATH_CLS_RT="-bootclasspath $(ICEDTEA_CLS_DIR_BOOT):$(ICEDTEA_RT)" \
	BOOTCLASSPATH_CLS="-bootclasspath $(ICEDTEA_CLS_DIR_BOOT)" \
	BOOTCLASSPATH_RT_LIBGCJ="-bootclasspath $(ICEDTEA_RT):$(SYSTEM_JDK_DIR)/jre/lib/rt.jar" \
	GENSRCDIR="$(abs_top_builddir)/generated" \
	JAR_KNOWS_ATFILE="$(JAR_KNOWS_ATFILE)" \
	JAR_KNOWS_J_OPTIONS="$(JAR_KNOWS_J_OPTIONS)" \
	JAR_ACCEPTS_STDIN_LIST="$(JAR_ACCEPTS_STDIN_LIST)" \
	DISABLE_NIMBUS="true" \
	NO_DOCS="true"

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
	$(top_srcdir)/pulseaudio/AUTHORS \
	$(top_srcdir)/pulseaudio/COPYING

EXTRA_DIST = $(GENERATED_FILES) $(top_srcdir)/patches/*.patch \
	$(top_srcdir)/patches/hotspot/default/*.patch \
	$(top_srcdir)/patches/boot/*.patch \
	$(top_srcdir)/patches/cacao/*.patch \
	$(top_srcdir)/patches/security/*.patch \
	$(top_srcdir)/patches/xrender/*.patch \
	tools-copy contrib ports extra overlays \
	javaws.png javaws.desktop visualvm.desktop \
	jconsole.desktop policytool.desktop \
	$(JTREG_SRCS) \
	IcedTeaPlugin.cc \
	HACKING $(PULSEAUDIO_SRCS) fsg.sh \
	$(abs_top_srcdir)/plugin $(top_srcdir)/netx \
	hotspot.map autogen.sh \
	tapset/hotspot.stp.in \
	tapset/hotspot_jni.stp.in \
	scripts/jni_create_stap.c \
	scripts/jni_desc

# Top-Level Targets
# =================

all-local: icedtea-stage2

check-local: jtregcheck

clean-local: clean-jtreg clean-jtreg-reports clean-pulse-java \
 clean-icedtea clean-icedtea-boot clean-clone clean-clone-boot \
 clean-plugs clean-extra clean-netx clean-visualvm clean-nbplatform \
 clean-bootstrap-directory-stage1 clean-bootstrap-directory-stage2 \
 clean-bootstrap-directory-symlink-stage1 clean-bootstrap-directory-symlink-stage2 \
 clean-extract clean-generated clean-plugin clean-liveconnect clean-native-ecj \
 clean-hgforest clean-icedtea-stage2 clean-icedtea-debug-stage2 clean-icedtea-stage1 \
 clean-add-zero clean-add-zero-debug clean-add-cacao clean-add-cacao-debug
	if [ -e bootstrap ]; then \
	  rmdir bootstrap ; \
	fi
	if [ -e lib ]; then \
	  rmdir lib ; \
	fi
	rm -f jni-common.o

distclean-local: clean-local clean-download
	if [ -e stamps ] ; then \
	  rmdir stamps ; \
	fi

install:

.PHONY: \
	env env-boot snapshot clean-clone clean-patch \
	clean-clone-boot clean-patch-boot clean-bootstrap-directory-stage2 \
	clean-bootstrap-directory-symlink-stage2 clean-bootstrap-directory-stage1 \
	clean-bootstrap-directory-symlink-stage1 icedtea icedtea-debug \
	clean-icedtea icedtea-stage2 clean-icedtea-boot clean-plugs \
	clean-tools-jar clean-visualvm clean-nbplatform \
	clean-rt clean-plugin hotspot hotspot-helper clean-extra clean-jtreg \
	clean-jtreg-reports clean-netx clean-drops

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
 stamps/download-cacao.stamp stamps/download-visualvm.stamp
	mkdir -p stamps
	touch $@

clean-download: clean-drops clean-download-openjdk clean-download-cacao \
 clean-download-visualvm
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
	  $(WGET) $(JAXWS_DROP_URL)/$(JAF_DROP_ZIP) -O drops/$(JAF_DROP_ZIP); \
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
	  if [ $(OPENJDK_SRC_ZIP) ] ; \
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
	  if [ $(HOTSPOT_SRC_ZIP) ] ; \
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
	 if [ $(CACAO_SRC_ZIP) ] ; \
	 then \
	   mv $(CACAO_SRC_ZIP) $(CACAO_SRC_ZIP).old ; \
	 fi ; \
	 $(WGET) $(CACAO_URL)$(CACAO_SRC_ZIP) -O $(CACAO_SRC_ZIP); \
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

stamps/download-visualvm.stamp:
if WITH_VISUALVM
if USE_ALT_NETBEANS_PROFILER_ZIP
	ln -sf $(ALT_NETBEANS_PROFILER_ZIP) $(NETBEANS_PROFILER_SRC_ZIP)
endif
	if ! echo "$(NETBEANS_PROFILER_SHA256SUM)  $(NETBEANS_PROFILER_SRC_ZIP)" \
	 | $(SHA256SUM) --check ; \
	then \
	 if [ $(NETBEANS_PROFILER_SRC_ZIP) ] ; \
	 then \
	  mv $(NETBEANS_PROFILER_SRC_ZIP) $(NETBEANS_PROFILER_SRC_ZIP).old ; \
	 fi ; \
	 $(WGET) $(NETBEANS_PROFILER_URL)$(NETBEANS_PROFILER_SRC_ZIP) \
	  -O $(NETBEANS_PROFILER_SRC_ZIP) ; \
	 if ! echo "$(NETBEANS_PROFILER_SHA256SUM)  $(NETBEANS_PROFILER_SRC_ZIP)" \
	   | $(SHA256SUM) --check ; \
	 then echo "ERROR: Bad download of NetBeans profiler zip"; false; \
	 fi; \
	fi

if USE_ALT_VISUALVM_ZIP
	ln -sf $(ALT_VISUALVM_ZIP) $(VISUALVM_SRC_ZIP)
endif
	if ! echo "$(VISUALVM_SHA256SUM)  $(VISUALVM_SRC_ZIP)" \
         | $(SHA256SUM) --check ; \
        then \
         if [ $(VISUALVM_SRC_ZIP) ] ; \
         then \
          mv $(VISUALVM_SRC_ZIP) $(VISUALVM_SRC_ZIP).old ; \
         fi ; \
         $(WGET) $(VISUALVM_URL)$(VISUALVM_SRC_ZIP) \
          -O $(VISUALVM_SRC_ZIP) ; \
	 if ! echo "$(VISUALVM_SHA256SUM)  $(VISUALVM_SRC_ZIP)" \
	   | $(SHA256SUM) --check ; \
	 then echo "ERROR: Bad download of VisualVM zip"; false; \
	 fi; \
        fi
endif
	mkdir -p stamps
	touch $@

clean-download-visualvm:
	rm -f $(VISUALVM_SRC_ZIP)
	rm -f $(NETBEANS_PROFILER_SRC_ZIP)
	rm -f stamps/download-visualvm.stamp

stamps/extract.stamp: stamps/extract-openjdk.stamp \
 stamps/extract-visualvm.stamp stamps/extract-cacao.stamp \
 stamps/extract-jaxws.stamp
	mkdir -p stamps
	touch $@

clean-extract: clean-extract-openjdk clean-extract-visualvm \
 clean-extract-cacao clean-extract-jaxws
	rm -f stamps/extract.stamp

stamps/extract-openjdk.stamp: stamps/download-openjdk.stamp
if OPENJDK_SRC_DIR_FOUND
	cp -a $(OPENJDK_SRC_DIR) openjdk
else
	if ! test -d openjdk ; then \
	  if [ ! -z $(OPENJDK_SRC_ZIP) ] ; then \
	    case "$(OPENJDK_SRC_ZIP)" in \
	      *.zip) $(UNZIP) -q $(OPENJDK_SRC_ZIP) ;; \
	      *.tar.*) $(TAR) xf $(OPENJDK_SRC_ZIP) ;; \
	    esac; \
	    mv jdk7-$(OPENJDK_CHANGESET) openjdk ; \
	    if test "x${HSBUILD}" != "xdefault"; then \
	      rm -rf openjdk/hotspot ; \
	    fi ; \
	  else \
	    echo "ERROR: Couldn't extract OpenJDK"; \
	    false; \
	  fi; \
	fi; 
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
	if [ ! -z $(LANGTOOLS_SRC_ZIP) ] ; then \
	  if test -e ${LANGTOOLS_SRC_ZIP} ; \
	  then \
	    if ! test -d openjdk/hotspot ; \
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
	if [ ! -z $(HOTSPOT_SRC_ZIP) ] ; then \
	  if test -e ${HOTSPOT_SRC_ZIP} ; \
	  then \
	    if ! test -d openjdk/hotspot ; \
	    then \
	      case "$(HOTSPOT_SRC_ZIP)" in \
	        *.zip) $(UNZIP) -q $(HOTSPOT_SRC_ZIP) ;; \
	        *.tar.*) $(TAR) xf $(HOTSPOT_SRC_ZIP) ;; \
	      esac; \
	      mv hotspot-$(HS_CHANGESET) openjdk/hotspot ; \
	    fi ; \
	  fi ; \
	fi
	chmod -R ug+w openjdk 
	sh $(srcdir)/fsg.sh ; 
	find openjdk -name '*.java' \
	  -exec sed -i 's/^[[:space:]]*@Override//' {} + ; \
	mkdir -p stamps 
	touch $@

clean-extract-openjdk: clean-overlay clean-versioning \
 clean-ports clean-patch-fsg
	rm -rf openjdk
	rm -f stamps/extract-openjdk.stamp

stamps/extract-visualvm.stamp: stamps/download-visualvm.stamp
if WITH_VISUALVM
	if ! test -d netbeans ; \
	then \
	  mkdir netbeans ; \
	  $(TAR) xf $(NETBEANS_PROFILER_SRC_ZIP) -C netbeans ; \
	fi

	if ! test -d visualvm ; \
	then \
	  $(TAR) xf $(VISUALVM_SRC_ZIP) ; \
	fi
endif
	mkdir -p stamps
	touch $@

clean-extract-visualvm:
if WITH_VISUALVM
	rm -rf netbeans visualvm
endif
	rm -f stamps/extract-visualvm.stamp

stamps/extract-cacao.stamp: stamps/download-cacao.stamp
if BUILD_CACAO
if !USE_SYSTEM_CACAO
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

stamps/extract-jaxws.stamp: stamps/extract-openjdk.stamp \
  stamps/download-jaxws-drop.stamp stamps/download-jaf-drop.stamp
	if test "x$(NEED_JAXWS_SRC)" = "xtrue"; then \
	  (cd openjdk/jaxws ; \
	   $(ANT) -Ddrops.dir=$(abs_top_builddir)/drops \
	   -Doutput.dir=$(BUILD_OUTPUT_DIR)/jaxws \
	   source ; \
	   cd $(abs_top_builddir)) ; \
	fi
if DTDTYPE_QNAME
	(cd $(BUILD_OUTPUT_DIR)/jaxws; \
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
	      >> stamps/extract-jaxws.stamp.tmp ; \
	    then \
	      echo "* $$(basename $$p): UNDOCUMENTED" \
	        >> stamps/extract-jaxws.stamp.tmp ; \
	    fi ; \
	   else \
	     test x$${all_patches_ok} = "xyes" && all_patches_ok=$$p ; \
	   fi ; \
	 done ; \
	 if ! test x$${all_patches_ok} = "xyes"; then \
	   echo ERROR patch $${all_patches_ok} FAILED! ; \
	   echo WARNING make clean-extract-jaxws before retrying a fix ; \
	   exit 2; \
	 fi ; \
	 cd $(abs_top_builddir)) ;
endif
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

# Link ports sources into tree
stamps/ports.stamp: stamps/extract.stamp
	for target in $(abs_top_srcdir)/ports/hotspot/make/*/platform_zero.in \
		      $(abs_top_srcdir)/ports/hotspot/make/*/makefiles/* \
		      $(abs_top_srcdir)/ports/hotspot/src/cpu/zero/vm/* \
		      $(abs_top_srcdir)/ports/hotspot/src/share/vm/*; do \
	  link=$$(dirname $$target | sed 's/^.*ports/openjdk/'|sed "s#hotspot/build#hotspot/make#"); \
	  ln -sfv $$target $$link; \
	done
	if ! test "x$(WITH_CACAO)" = "xno" ; then \
	  mkdir -p openjdk/jdk/src/solaris/bin/$(BUILD_ARCH_DIR) ; \
	  ln -sf $(abs_top_builddir)/jvm.cfg \
	    openjdk/jdk/src/solaris/bin/$(BUILD_ARCH_DIR)/jvm.cfg ; \
	fi
	if ! test "x$(WITH_CACAO)" = "xno"; then \
	  ln -sf $(abs_top_builddir)/ergo.c \
	    openjdk/jdk/src/solaris/bin/ergo_$(BUILD_ARCH_DIR).c; \
	fi
	mkdir -p stamps
	touch $@

clean-ports:
	for target in $(abs_top_srcdir)/ports/hotspot/make/*/makefiles/* \
		      $(abs_top_srcdir)/ports/hotspot/src/cpu/zero/vm/* \
		      $(abs_top_srcdir)/ports/hotspot/src/share/vm/*; do \
	  link=$$(echo $$target | sed 's/^.*ports/openjdk/'|sed "s#hotspot/build#hotspot/make#"); \
	  rm -rf $$link; \
	done
	rm -f openjdk/jdk/src/solaris/bin/$(BUILD_ARCH_DIR)/jvm.cfg
	rm -f openjdk/jdk/src/solaris/bin/ergo_$(BUILD_ARCH_DIR).c 
	rm -f stamps/ports.stamp

stamps/generated.stamp: stamps/ports.stamp
	if [ ! -e $(abs_top_builddir)/generated ]; then \
	  cp -a $(abs_top_srcdir)/generated $(abs_top_builddir); \
	  find $(abs_top_builddir)/generated -type f -exec chmod 640 '{}' ';' \
	    -o -type d -exec chmod 750 '{}' ';'; \
	fi
	mkdir -p stamps
	touch $@

clean-generated:
	if [ $(abs_top_srcdir) != $(abs_top_builddir) ]; then \
	  rm -rf $(abs_top_builddir)/generated ; \
	fi
	rm -f stamps/generated.stamp

stamps/patch-fsg.stamp: stamps/generated.stamp
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
	cp openjdk/jdk/make/common/shared/Defs.gmk Defs.gmk.bak
	if which $(HG) >/dev/null; then \
	  if [ -e $(abs_top_srcdir)/.hg ] ; then \
	    revision="+r`(cd $(abs_top_srcdir); $(HG) tip --template '{node|short}')`" ; \
	  fi ; \
	  if [ -e openjdk/jdk/.hg ] ; then \
	    jdk_rev="r`(cd openjdk/jdk; $(HG) tip --template '{node|short}')`" ; \
	  fi ; \
	  if [ -e openjdk/hotspot/.hg ] ; then \
	    hotspot_rev="r`(cd openjdk/hotspot; $(HG) tip --template '{node|short}')`" ; \
	  fi ; \
	fi ; \
	icedtea_version="$(PACKAGE_VERSION)$${revision}" ; \
	if ! test "x$(WITH_CACAO)" = "xno"; then \
	  echo "JDK_DERIVATIVE_NAME=$${icedtea_version}" \
	    >>openjdk/jdk/make/common/shared/Defs.gmk ; \
	  echo "PRODUCT_NAME=IcedTea7" \
	    >>openjdk/jdk/make/common/shared/Defs.gmk ; \
	else \
	  echo "JDK_DERIVATIVE_NAME=IcedTea7 $${icedtea_version}" \
	    >>openjdk/jdk/make/common/shared/Defs.gmk ; \
	fi ; \
	echo "JDK_REVID=$${jdk_rev}" >>openjdk/jdk/make/common/shared/Defs.gmk ; \
	echo "HOTSPOT_REVID=$${hotspot_rev}" >>openjdk/jdk/make/common/shared/Defs.gmk ; \
	echo "DISTRO_NAME=$(DIST_NAME)" >>openjdk/jdk/make/common/shared/Defs.gmk ; \
	if [ -n "$(PKGVERSION)" ]; then \
	  echo "DISTRO_PACKAGE_VERSION=$(PKGVERSION)" \
	    >>openjdk/jdk/make/common/shared/Defs.gmk ; \
	fi; \
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
	fi
	mkdir -p stamps
	touch $@

clean-clone-boot: clean-patch-boot
	rm -rf openjdk-boot
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
	${GCJ} ${CFLAGS} -Wl,-Bsymbolic -findirect-dispatch -o native-ecj \
	    --main=org.eclipse.jdt.internal.compiler.batch.Main ${ECJ_JAR}
endif
	touch $@

clean-native-ecj:
	rm -f native-ecj
	rm -rf stamps/native-ecj.stamp

# bootstrap/stage1
stamps/bootstrap-directory-stage1.stamp: stamps/native-ecj.stamp
	mkdir -p bootstrap/boot/bin stamps/
	ln -sf $(JAVA) bootstrap/boot/bin/java
	ln -sf $(JAVAH) bootstrap/boot/bin/javah
	ln -sf $(RMIC) bootstrap/boot/bin/rmic
	ln -sf $(JAR) bootstrap/boot/bin/jar
	ln -sf ../../../javac bootstrap/boot/bin/javac
	ln -sf ../../../javap bootstrap/boot/bin/javap
	mkdir -p bootstrap/boot/lib/endorsed
	ln -sf $(XALAN2_JAR) bootstrap/boot/lib/endorsed/xalan-j2.jar
	ln -sf $(XALAN2_SERIALIZER_JAR) \
	  bootstrap/boot/lib/endorsed/xalan-j2-serializer.jar
	ln -sf $(XERCES2_JAR) bootstrap/boot/lib/endorsed/xerces-j2.jar
	mkdir -p bootstrap/boot/jre/lib; \
	cp $(SYSTEM_JDK_DIR)/jre/lib/rt.jar \
	  bootstrap/boot/jre/lib/rt.jar; \
	chmod u+w bootstrap/boot/jre/lib/rt.jar; \
	ln -sf $(SYSTEM_JDK_DIR)/jre/lib/$(JRE_ARCH_DIR) \
	  bootstrap/boot/jre/lib/; \
	if ! test -d bootstrap/boot/jre/lib/$(INSTALL_ARCH_DIR); \
	  then \
	  ln -sf ./$(JRE_ARCH_DIR) \
	    bootstrap/boot/jre/lib/$(INSTALL_ARCH_DIR); \
	fi; \
	mkdir -p bootstrap/boot/include; \
	for i in $(SYSTEM_JDK_DIR)/include/*; do \
	  test -r $$i | continue; \
	  i=`basename $$i`; \
	  rm -f bootstrap/boot/include/$$i; \
	  ln -s $(SYSTEM_JDK_DIR)/include/$$i bootstrap/boot/include/$$i; \
	done; \
	mkdir -p stamps
	touch $@

clean-bootstrap-directory-stage1:
	rm -f stamps/bootstrap-directory-stage1.stamp
	rm -rf bootstrap/boot

# bootstrap/stage2.
stamps/bootstrap-directory-stage2.stamp: stamps/icedtea-stage1.stamp
	rm -rf bootstrap/icedtea
	ln -s \
	  $(abs_top_builddir)/$(BOOT_BUILD_OUTPUT_DIR)/j2sdk-image/ \
	  bootstrap/icedtea
	if ! test -d bootstrap/icedtea/include; then \
	  ln -sf $(ICEDTEA_BUILD_DIR_BOOT)/include bootstrap/icedtea/include; \
	fi
	mkdir -p stamps
	touch $@

clean-bootstrap-directory-stage2:
	rm -f stamps/bootstrap-directory-stage2.stamp
	rm -rf bootstrap/icedtea

# stage 1 symlink.
stamps/bootstrap-directory-symlink-stage1.stamp: stamps/bootstrap-directory-stage1.stamp
	rm -f bootstrap/jdk1.6.0
	ln -sf boot bootstrap/jdk1.6.0
	mkdir -p stamps
	touch $@

clean-bootstrap-directory-symlink-stage1:
	rm -f stamps/bootstrap-directory-symlink-stage1.stamp
	rm -f bootstrap/jdk1.6.0

# stage 2 symlink.
stamps/bootstrap-directory-symlink-stage2.stamp: $(BOOTSTRAP_DIRECTORY_STAMP)
	mkdir -p bootstrap
	rm -f bootstrap/jdk1.6.0
	ln -sf $(ICEDTEA_HOME) bootstrap/jdk1.6.0; \
	if ! bootstrap/jdk1.6.0/bin/java -version ; \
         then \
           echo "bootstrap/jdk1.6.0/bin/java" \
               "cannot be found or is corrupted." ; \
           exit 1; \
        fi
	mkdir -p stamps
	touch $@

clean-bootstrap-directory-symlink-stage2:
	rm -f stamps/bootstrap-directory-symlink-stage2.stamp
	rm -f bootstrap/jdk1.6.0

# OpenJDK Targets
# ===============

# If you change anything here in the icedtea target, please make sure
# you change it in the icedtea-debug target as well.
stamps/icedtea.stamp: stamps/bootstrap-directory-symlink-stage2.stamp \
 stamps/download.stamp stamps/extract.stamp $(OPENJDK_TREE) \
 stamps/plugin.stamp $(JNLP_ABOUT_TARGET) stamps/cacao.stamp \
 stamps/netx-dist.stamp stamps/liveconnect-dist.stamp stamps/pulse-java.stamp
	$(ARCH_PREFIX) $(MAKE) \
	  $(ICEDTEA_ENV) \
	  -C openjdk/ \
	  $(ICEDTEA_BUILD_TARGET)
	mkdir -p $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	mkdir -p $(BUILD_OUTPUT_DIR)/j2re-image/lib/$(INSTALL_ARCH_DIR)
	mkdir -p $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/ext
	mkdir -p $(BUILD_OUTPUT_DIR)/j2re-image/lib/ext 
if ENABLE_NPPLUGIN
	cp -pPRf $(PLUGIN_BUILD_DIR)/IcedTeaNPPlugin.so \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	cp -pPRf $(PLUGIN_BUILD_DIR)/IcedTeaNPPlugin.so \
	  $(BUILD_OUTPUT_DIR)/j2re-image/lib/$(INSTALL_ARCH_DIR)
else
if ENABLE_PLUGIN
	cp -pPRf $(PLUGIN_BUILD_DIR)/IcedTeaPlugin.so \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	cp -pPRf $(PLUGIN_BUILD_DIR)/IcedTeaPlugin.so \
	  $(BUILD_OUTPUT_DIR)/j2re-image/lib/$(INSTALL_ARCH_DIR)
endif
endif
if ENABLE_PULSE_JAVA
	cp -pPRf libpulse-java.so \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	cp -pPRf libpulse-java.so \
	  $(BUILD_OUTPUT_DIR)/j2re-image/lib/$(INSTALL_ARCH_DIR)
	cp -pPRf pulse-java.jar \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/ext
	cp -pPRf pulse-java.jar \
	  $(BUILD_OUTPUT_DIR)/j2re-image/lib/ext
endif
if JNLP_ABOUT_NEEDED
	cp $(NETX_RESOURCE_DIR)/about.jnlp \
	  extra-lib/about.jar \
	  $(BUILD_OUTPUT_DIR)/j2re-image/lib
	cp $(NETX_RESOURCE_DIR)/about.jnlp \
	  extra-lib/about.jar \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib
endif
if ZERO_BUILD
	printf -- '-zero ALIASED_TO -server\n' >> $(BUILD_JRE_ARCH_DIR)/jvm.cfg
endif
if SHARK_BUILD
	printf -- '-shark ALIASED_TO -server\n' >> $(BUILD_JRE_ARCH_DIR)/jvm.cfg
endif
if WITH_CACAO
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
	fi
endif
if ENABLE_NSS
	cp $(abs_top_builddir)/nss.cfg \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/security;
endif
if WITH_TZDATA_DIR
	cp $(abs_top_builddir)/tz.properties \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib;
endif
	@echo "IcedTea is served:" $(BUILD_OUTPUT_DIR)
	mkdir -p stamps
	touch $@

stamps/icedtea-debug.stamp: stamps/bootstrap-directory-symlink-stage2.stamp \
 stamps/download.stamp stamps/extract.stamp $(OPENJDK_TREE) \
 stamps/plugin.stamp $(JNLP_ABOUT_TARGET) stamps/cacao.stamp \
 stamps/netx-dist.stamp stamps/liveconnect-dist.stamp stamps/pulse-java.stamp
	$(ARCH_PREFIX) $(MAKE) \
	  $(ICEDTEA_ENV) \
	  -C openjdk/ \
	  $(ICEDTEA_DEBUG_BUILD_TARGET)
	mkdir -p $(BUILD_OUTPUT_DIR)-debug/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	mkdir -p $(BUILD_OUTPUT_DIR)-debug/j2re-image/lib/$(INSTALL_ARCH_DIR)
	mkdir -p $(BUILD_OUTPUT_DIR)-debug/j2sdk-image/jre/lib/ext
	mkdir -p $(BUILD_OUTPUT_DIR)-debug/j2re-image/lib/ext 
if ENABLE_NPPLUGIN
	cp -pPRf $(PLUGIN_BUILD_DIR)/IcedTeaNPPlugin.so \
	  $(BUILD_OUTPUT_DIR)-debug/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	cp -pPRf $(PLUGIN_BUILD_DIR)/IcedTeaNPPlugin.so \
	  $(BUILD_OUTPUT_DIR)-debug/j2re-image/lib/$(INSTALL_ARCH_DIR)
else
if ENABLE_PLUGIN
	cp -pPRf $(PLUGIN_BUILD_DIR)/IcedTeaPlugin.so \
	  $(BUILD_OUTPUT_DIR)-debug/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	cp -pPRf $(PLUGIN_BUILD_DIR)/IcedTeaPlugin.so \
	  $(BUILD_OUTPUT_DIR)-debug/j2re-image/lib/$(INSTALL_ARCH_DIR)
endif
endif
if ENABLE_PULSE_JAVA
	cp -pPRf libpulse-java.so \
	  $(BUILD_OUTPUT_DIR)-debug/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	cp -pPRf libpulse-java.so \
	  $(BUILD_OUTPUT_DIR)-debug/j2re-image/lib/$(INSTALL_ARCH_DIR)
	cp -pPRf pulse-java.jar \
	  $(BUILD_OUTPUT_DIR)-debug/j2sdk-image/jre/lib/ext
	cp -pPRf pulse-java.jar \
	  $(BUILD_OUTPUT_DIR)-debug/j2re-image/lib/ext
endif
if JNLP_ABOUT_NEEDED
	cp $(NETX_RESOURCE_DIR)/default.jnlp \
	  extra-lib/about.jar \
	  $(BUILD_OUTPUT_DIR)-debug/j2re-image/lib
	cp $(NETX_RESOURCE_DIR)/default.jnlp \
	  extra-lib/about.jar \
	  $(BUILD_OUTPUT_DIR)-debug/j2sdk-image/jre/lib
endif
if ZERO_BUILD
	printf -- '-zero ALIASED_TO -server\n' >> $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg
endif
if SHARK_BUILD
	printf -- '-shark ALIASED_TO -server\n' >> $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg
endif
if WITH_CACAO
	printf -- '-cacao ALIASED_TO -server\n' >> $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg
endif
if ENABLE_SYSTEMTAP
	mkdir -p $(BUILD_OUTPUT_DIR)-debug/j2sdk-image/tapset ; \
	grep "client IGNORE" $(BUILD_JRE_ARCH_DIR)/jvm.cfg; \
	if test $$? -eq 0; then \
	  sed -e '/\/client\/libjvm.so/d' \
	    < $(abs_top_builddir)/tapset/hotspot.stp \
	    > $(BUILD_OUTPUT_DIR)-debug/j2sdk-image/tapset/hotspot.stp; \
	  sed -e '/\/client\/libjvm.so/d' \
	    < $(abs_top_builddir)/tapset/hotspot_jni.stp \
	    > $(BUILD_OUTPUT_DIR)/j2sdk-image/tapset/hotspot_jni.stp; \
	else \
	  cp $(abs_top_builddir)/tapset/hotspot.stp \
	    $(BUILD_OUTPUT_DIR)-debug/j2sdk-image/tapset/hotspot.stp; \
	  cp $(abs_top_builddir)/tapset/hotspot_jni.stp \
	    $(BUILD_OUTPUT_DIR)/j2sdk-image/tapset/hotspot_jni.stp; \
	fi
endif
if ENABLE_NSS
	cp $(abs_top_builddir)/nss.cfg \
	  $(BUILD_OUTPUT_DIR)-debug/j2sdk-image/jre/lib/security;
endif
if WITH_TZDATA_DIR
	cp $(abs_top_builddir)/tz.properties \
	  $(BUILD_OUTPUT_DIR)-debug/j2sdk-image/jre/lib;
endif
	@echo "IcedTea (debug build) is served:" \
	  $(BUILD_OUTPUT_DIR)-debug
	mkdir -p stamps
	touch $@

clean-icedtea: 
	if [ -e $(abs_top_builddir)/openjdk ]; then \
	  $(ARCH_PREFIX) $(MAKE) \
	    $(ICEDTEA_ENV) \
	    -C openjdk clobber ; \
	fi
	rm -f stamps/icedtea.stamp stamps/icedtea-debug.stamp

stamps/icedtea-stage2.stamp: stamps/icedtea.stamp stamps/add-cacao.stamp \
 stamps/add-zero.stamp stamps/visualvm.stamp
	mkdir -p stamps
	touch $@

clean-icedtea-stage2:
	rm -f stamps/icedtea-stage2.stamp

stamps/icedtea-debug-stage2.stamp: stamps/icedtea-debug.stamp \
 stamps/add-cacao-debug.stamp stamps/add-zero-debug.stamp \
 stamps/visualvm.stamp
	mkdir -p stamps
	touch $@

clean-icedtea-debug-stage2:
	rm -f stamps/icedtea-debug-stage2.stamp

# OpenJDK boot Targets
# ===================

stamps/icedtea-boot.stamp: stamps/bootstrap-directory-symlink-stage1.stamp \
 stamps/download.stamp $(OPENJDK_BOOT_TREE) stamps/plugs.stamp \
 stamps/netx-dist.stamp stamps/liveconnect-dist.stamp stamps/cacao.stamp
	$(ARCH_PREFIX) $(MAKE) \
	  $(ICEDTEA_ENV_BOOT) \
	  -C openjdk-boot \
	  $(ICEDTEA_BUILD_TARGET)
	@echo "Bootstrapped IcedTea is served:" \
	  $(BOOT_BUILD_OUTPUT_DIR)
	mkdir -p stamps
	touch $@

hotspot:
	rm -f stamps/bootstrap-directory-symlink-stage1.stamp
	rm -f stamps/icedtea-boot.stamp
	rm -f stamps/icedtea-stage1.stamp
	$(ARCH_PREFIX) $(MAKE) hotspot-helper
	mkdir -p \
	   $(BOOT_BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)/server
	cp $(BOOT_BUILD_OUTPUT_DIR)/$(SERVER_DIR)/libjvm.so \
	   $(BOOT_BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)/server
	@echo "Hotspot is served:" $(BOOT_BUILD_OUTPUT_DIR)/j2sdk-image

hotspot-helper: stamps/bootstrap-directory-symlink-stage1.stamp \
	stamps/plugs.stamp stamps/ports.stamp stamps/patch-boot.stamp
	$(ARCH_PREFIX) $(MAKE) \
	  $(ICEDTEA_ENV_BOOT) \
	  -C openjdk-boot hotspot

clean-icedtea-boot: 
	if [ -e $(abs_top_builddir)/openjdk-boot ]; then \
	  $(ARCH_PREFIX) $(MAKE) \
	    $(ICEDTEA_ENV_BOOT) \
	    -C openjdk-boot clobber ; \
	fi 
	rm -f stamps/icedtea-boot.stamp

stamps/icedtea-stage1.stamp: stamps/icedtea-boot.stamp
	mkdir -p stamps
	touch $@

clean-icedtea-stage1:
	rm -f stamps/icedtea-stage1.stamp

# Plugin

if ENABLE_NPPLUGIN
# IcedTeaNPPlugin.so.
# Separate compile and link invocations to ensure intermediate object
# is listed before -l options.  See:
# http://developer.mozilla.org/en/docs/XPCOM_Glue
NPPLUGIN_SRC=IcedTeaNPPlugin.cc IcedTeaScriptablePluginObject.cc \
	IcedTeaJavaRequestProcessor.cc IcedTeaPluginRequestProcessor.cc \
	IcedTeaPluginUtils.cc

NPPLUGIN_OBJECTS=IcedTeaNPPlugin.o IcedTeaScriptablePluginObject.o \
	IcedTeaJavaRequestProcessor.o IcedTeaPluginRequestProcessor.o \
	IcedTeaPluginUtils.o

$(PLUGIN_BUILD_DIR)/%.o: $(PLUGIN_SRCDIR)/%.cc
	mkdir -p $(PLUGIN_BUILD_DIR) && \
	$(CXX) $(CXXFLAGS) \
	  -DPACKAGE_VERSION="\"$(PACKAGE_VERSION)\"" \
	  -DPLUGIN_VERSION="\"$(PACKAGE_VERSION)$(ICEDTEA_REV)\"" \
	  -DMOZILLA_VERSION_COLLAPSED="$(MOZILLA_VERSION_COLLAPSED)" \
	  $(GLIB_CFLAGS) \
	  $(GTK_CFLAGS) \
	  $(MOZILLA_CFLAGS) \
	  -fPIC -o $@ -c $<

$(PLUGIN_BUILD_DIR)/IcedTeaNPPlugin.so: $(addprefix $(PLUGIN_BUILD_DIR)/,$(NPPLUGIN_OBJECTS))
	$(CXX) $(CXXFLAGS) \
	  $(addprefix $(PLUGIN_BUILD_DIR)/,$(NPPLUGIN_OBJECTS)) \
	  $(GLIB_LIBS) \
	  $(GTK_LIBS) \
	  $(MOZILLA_LIBS)\
	  -shared -o $@

clean-IcedTeaNPPlugin:
	rm -f $(addprefix $(PLUGIN_BUILD_DIR)/,$(NPPLUGIN_OBJECTS))
	rm -f $(PLUGIN_BUILD_DIR)/IcedTeaNPPlugin.so
endif

if ENABLE_PLUGIN
# IcedTeaPlugin.so.
# Separate compile and link invocations to ensure intermediate object
# is listed before -l options.  See:
# http://developer.mozilla.org/en/docs/XPCOM_Glue
$(PLUGIN_BUILD_DIR)/IcedTeaPlugin.o: IcedTeaPlugin.cc
	if [ -e $(abs_top_srcdir)/.hg ] && which $(HG) >/dev/null; then \
	  revision="-r`(cd $(abs_top_srcdir); $(HG) tip --template '{rev}')`" ; \
	fi ; \
	if [ -n "$(PKGVERSION)" ]; then plugin_version=" ($(PKGVERSION))"; fi; \
	plugin_version=" $(PACKAGE_VERSION)$$revision$$plugin_version"; \
	mkdir -p $(PLUGIN_BUILD_DIR) && \
	$(CXX) $(CXXFLAGS) \
	  -DPACKAGE_VERSION="\"$(PACKAGE_VERSION)\"" \
	  -DPLUGIN_VERSION="\"$$plugin_version\"" \
	  -DMOZILLA_VERSION_COLLAPSED="$(MOZILLA_VERSION_COLLAPSED)" \
	  $(GTK_CFLAGS) \
	  $(MOZILLA_CFLAGS) \
	  -fPIC -c -o $@ $<

$(PLUGIN_BUILD_DIR)/IcedTeaPlugin.so: $(PLUGIN_BUILD_DIR)/IcedTeaPlugin.o
	$(CXX) $(CXXFLAGS) \
	  $< \
	  $(GTK_LIBS) \
	  $(XULRUNNER_LIBS) \
	  -shared -o $@

clean-IcedTeaPlugin:
	rm -f $(PLUGIN_BUILD_DIR)/IcedTeaPlugin.o
	rm -f $(PLUGIN_BUILD_DIR)/IcedTeaPlugin.so
endif

stamps/plugin.stamp: $(ICEDTEAPLUGIN_TARGET) $(ICEDTEANPPLUGIN_TARGET)
	mkdir -p stamps
	touch $@

clean-plugin: $(ICEDTEAPLUGIN_CLEAN) $(ICEDTEANPPLUGIN_CLEAN)
	if [ -e $(PLUGIN_BUILD_DIR) ] ; then \
	  rmdir $(PLUGIN_BUILD_DIR) ; \
	fi
	rm -f stamps/plugin.stamp

liveconnect-source-files.txt:
	if test "x${LIVECONNECT_DIR}" != x; then \
	  find $(PLUGIN_SRCDIR)/java -name '*.java' | sort > $@ ; \
	fi
	touch $@

stamps/liveconnect.stamp: liveconnect-source-files.txt stamps/netx.stamp
	if test "x${LIVECONNECT_DIR}" != x; then \
	  mkdir -p $(LIVECONNECT_CLASSES) ; \
	  $(ICEDTEA_BOOT_DIR)/bin/javac $(IT_JAVACFLAGS) \
	      -d $(LIVECONNECT_CLASSES) \
	      -classpath $(ICEDTEA_RT):$(NETX_CLASSES) \
	      -sourcepath $(SOURCEPATH_DIRS) \
	      -bootclasspath \'\' \
	      @liveconnect-source-files.txt ; \
	fi
	mkdir -p stamps
	touch $@

stamps/liveconnect-dist.stamp: stamps/liveconnect.stamp
	if test "x${LIVECONNECT_DIR}" != x; then \
	  (cd $(LIVECONNECT_CLASSES) ; \
	   mkdir -p lib ; \
	   $(ICEDTEA_BOOT_DIR)/bin/jar cf $(LIVECONNECT_JAR) $(LIVECONNECT_DIR) ; \
	   cp -a $(PLUGIN_SRCDIR)/java src; \
	   find src -type f -exec chmod 640 '{}' ';' -o -type d -exec chmod 750 '{}' ';'; \
	   cd src ; \
	   $(ZIP) -qr $(LIVECONNECT_SRC) ${LIVECONNECT_DIR} ) ; \
	fi
	mkdir -p stamps
	touch $@

clean-liveconnect:
	rm -rf $(LIVECONNECT_CLASSES)
	rm -f stamps/liveconnect-dist.stamp
	rm -f liveconnect-source-files.txt
	rm -f stamps/liveconnect.stamp

# NetX
# requires availability of OpenJDK source code including
# a patch applied to sun.plugin.AppletViewerPanel and generated sources

netx-source-files.txt:
	find $(NETX_SRCDIR) -name '*.java' | sort > $@

stamps/netx.stamp: netx-source-files.txt stamps/rt.stamp
	mkdir -p $(NETX_CLASSES)
	$(ICEDTEA_BOOT_DIR)/bin/javac $(IT_JAVACFLAGS) \
	    -d $(NETX_CLASSES) \
	    -classpath $(ICEDTEA_RT) \
	    -sourcepath $(NETX_SRCDIR):$(SOURCEPATH_DIRS) \
	    -bootclasspath \'\' \
	    @netx-source-files.txt ;
	mkdir -p stamps
	touch $@

stamps/netx-dist.stamp: stamps/netx.stamp
	(cd $(NETX_CLASSES) ; \
	 mkdir -p lib ; \
	 $(ICEDTEA_BOOT_DIR)/bin/jar cf $(NETX_JAR) javax/jnlp net ; \
	 cp -a $(NETX_SRCDIR) src; \
	 find src -type f -exec chmod 640 '{}' ';' -o -type d -exec chmod 750 '{}' ';'; \
	 cd src ; \
	 $(ICEDTEA_BOOT_DIR)/bin/jar uf $(NETX_JAR) \
	  `find . -type f -not -name '*.java'` ; \
	 $(ZIP) -qr $(NETX_SRC) javax net )
	mkdir -p stamps
	touch $@

clean-netx:
	rm -rf $(NETX_CLASSES)
	rm -f stamps/netx-dist.stamp
	rm -f netx-source-files.txt
	rm -f stamps/netx.stamp

extra-source-files.txt:
	find $(abs_top_srcdir)/extra -name '*.java' | sort > $@

stamps/extra-class-files.stamp: extra-source-files.txt stamps/netx.stamp
	mkdir -p extra-lib
	$(ICEDTEA_BOOT_DIR)/bin/javac $(IT_JAVACFLAGS) -d extra-lib \
	  -classpath $(ICEDTEA_RT):$(NETX_CLASSES) \
	  -sourcepath $(abs_top_srcdir)/extra:$(SOURCEPATH_DIRS) \
	  -bootclasspath \'\' \
	  @extra-source-files.txt
	cp -r $(abs_top_srcdir)/extra/net/sourceforge/jnlp/about/resources \
	  extra-lib/net/sourceforge/jnlp/about	
	find extra-lib/net/sourceforge/jnlp/about -type f -exec chmod 640 '{}' ';' \
	  -o -type d -exec chmod 750 '{}' ';'
	mkdir -p stamps
	touch $@

clean-extra:
	rm -rf extra-lib
	rm -f stamps/extra-class-files.stamp
	rm -f extra-source-files.txt

extra-lib/about.jar: stamps/extra-class-files.stamp
	$(ICEDTEA_BOOT_DIR)/bin/jar cf $@ -C extra-lib net

# PulseAudio based mixer
# (pulse-java)

stamps/pulse-java.stamp: stamps/pulse-java-jar.stamp stamps/pulse-java-native-code.stamp
	mkdir -p stamps
	touch $@

stamps/pulse-java-class.stamp: $(INITIAL_BOOTSTRAP_LINK_STAMP)
if ENABLE_PULSE_JAVA
	mkdir -p $(PULSE_JAVA_CLASS_DIR)
	(cd $(PULSE_JAVA_JAVA_SRCDIR); \
	 $(ICEDTEA_BOOT_DIR)/bin/javac $(IT_JAVACFLAGS) -d $(PULSE_JAVA_CLASS_DIR)\
	 -bootclasspath '$(ICEDTEA_BOOT_DIR)/jre/lib/rt.jar' \
	 org/classpath/icedtea/pulseaudio/*.java\
	)
	cp  -r $(PULSE_JAVA_JAVA_SRCDIR)/META-INF $(PULSE_JAVA_CLASS_DIR)
	chmod -R ug+w $(PULSE_JAVA_CLASS_DIR)/META-INF
endif
	mkdir -p stamps
	touch $@

stamps/pulse-java-jar.stamp: stamps/pulse-java-class.stamp
if ENABLE_PULSE_JAVA
	mkdir -p $(PULSE_JAVA_CLASS_DIR);
	$(ICEDTEA_BOOT_DIR)/bin/jar cf pulse-java.jar -C $(PULSE_JAVA_CLASS_DIR) .;
endif
	mkdir -p stamps
	touch $@

stamps/pulse-java-headers.stamp: stamps/pulse-java-class.stamp
if ENABLE_PULSE_JAVA
	if ! test -d $(ICEDTEA_BOOT_DIR) ; \
	then \
	  $(JAVAH) -d $(PULSE_JAVA_NATIVE_BUILDDIR) -classpath $(PULSE_JAVA_CLASS_DIR) org.classpath.icedtea.pulseaudio.EventLoop ; \
	  $(JAVAH) -d $(PULSE_JAVA_NATIVE_BUILDDIR) -classpath $(PULSE_JAVA_CLASS_DIR) org.classpath.icedtea.pulseaudio.Stream ; \
	  $(JAVAH) -d $(PULSE_JAVA_NATIVE_BUILDDIR) -classpath $(PULSE_JAVA_CLASS_DIR) org.classpath.icedtea.pulseaudio.Operation; \
	  $(JAVAH) -d $(PULSE_JAVA_NATIVE_BUILDDIR) -classpath $(PULSE_JAVA_CLASS_DIR) org.classpath.icedtea.pulseaudio.PulseAudioSourcePort ; \
	  $(JAVAH) -d $(PULSE_JAVA_NATIVE_BUILDDIR) -classpath $(PULSE_JAVA_CLASS_DIR) org.classpath.icedtea.pulseaudio.PulseAudioTargetPort ; \
	else \
	  $(ICEDTEA_BOOT_DIR)/bin/javah -d $(PULSE_JAVA_NATIVE_BUILDDIR) -classpath $(PULSE_JAVA_CLASS_DIR) org.classpath.icedtea.pulseaudio.EventLoop ; \
	  $(ICEDTEA_BOOT_DIR)/bin/javah -d $(PULSE_JAVA_NATIVE_BUILDDIR) -classpath $(PULSE_JAVA_CLASS_DIR) org.classpath.icedtea.pulseaudio.Stream ; \
	  $(ICEDTEA_BOOT_DIR)/bin/javah -d $(PULSE_JAVA_NATIVE_BUILDDIR) -classpath $(PULSE_JAVA_CLASS_DIR) org.classpath.icedtea.pulseaudio.Operation; \
	  $(ICEDTEA_BOOT_DIR)/bin/javah -d $(PULSE_JAVA_NATIVE_BUILDDIR) -classpath $(PULSE_JAVA_CLASS_DIR) org.classpath.icedtea.pulseaudio.PulseAudioSourcePort ; \
	  $(ICEDTEA_BOOT_DIR)/bin/javah -d $(PULSE_JAVA_NATIVE_BUILDDIR) -classpath $(PULSE_JAVA_CLASS_DIR) org.classpath.icedtea.pulseaudio.PulseAudioTargetPort ; \
	fi
endif
	mkdir -p stamps
	touch $@

stamps/pulse-java-native-code.stamp: stamps/pulse-java-headers.stamp
if ENABLE_PULSE_JAVA
	mkdir -p $(PULSE_JAVA_NATIVE_BUILDDIR)
	$(CC) $(CFLAGS) -fPIC -c -I$(ICEDTEA_BOOT_DIR)/include/linux -I$(ICEDTEA_BOOT_DIR)/include $(PULSE_JAVA_NATIVE_SRCDIR)/jni-common.c -o $(PULSE_JAVA_NATIVE_BUILDDIR)/jni-common.o
	$(CC) $(LIBPULSE_CFLAGS) $(CFLAGS) -fPIC -c -I$(ICEDTEA_BOOT_DIR)/include/linux -I$(ICEDTEA_BOOT_DIR)/include -I$(PULSE_JAVA_NATIVE_BUILDDIR) $(PULSE_JAVA_NATIVE_SRCDIR)/org_classpath_icedtea_pulseaudio_EventLoop.c -o $(PULSE_JAVA_NATIVE_BUILDDIR)/org_classpath_icedtea_pulseaudio_EventLoop.o
	$(CC) $(LIBPULSE_CFLAGS) $(CFLAGS) -fPIC -c -I$(ICEDTEA_BOOT_DIR)/include/linux -I$(ICEDTEA_BOOT_DIR)/include -I$(PULSE_JAVA_NATIVE_BUILDDIR) $(PULSE_JAVA_NATIVE_SRCDIR)/org_classpath_icedtea_pulseaudio_Operation.c -o $(PULSE_JAVA_NATIVE_BUILDDIR)/org_classpath_icedtea_pulseaudio_Operation.o
	$(CC) $(LIBPULSE_CFLAGS) $(CFLAGS) -fPIC -c -I$(ICEDTEA_BOOT_DIR)/include/linux -I$(ICEDTEA_BOOT_DIR)/include -I$(PULSE_JAVA_NATIVE_BUILDDIR) $(PULSE_JAVA_NATIVE_SRCDIR)/org_classpath_icedtea_pulseaudio_Stream.c -o $(PULSE_JAVA_NATIVE_BUILDDIR)/org_classpath_icedtea_pulseaudio_Stream.o
	$(CC) $(LIBPULSE_CFLAGS) $(CFLAGS) -fPIC -c -I$(ICEDTEA_BOOT_DIR)/include/linux -I$(ICEDTEA_BOOT_DIR)/include -I$(PULSE_JAVA_NATIVE_BUILDDIR) $(PULSE_JAVA_NATIVE_SRCDIR)/org_classpath_icedtea_pulseaudio_PulseAudioTargetPort.c -o $(PULSE_JAVA_NATIVE_BUILDDIR)/org_classpath_icedtea_pulseaudio_PulseAudioTargetPort.o
	$(CC) $(LDFLAGS) -shared $(PULSE_JAVA_NATIVE_BUILDDIR)/org_*pulseaudio*.o $(PULSE_JAVA_NATIVE_BUILDDIR)/jni-common.o $(LIBPULSE_LIBS) -o libpulse-java.so
endif
	mkdir -p stamps
	touch $@

clean-pulse-java:
if ENABLE_PULSE_JAVA
	[ -z "$(PULSE_JAVA_NATIVE_BUILDDIR)" ] || rm -rf $(PULSE_JAVA_NATIVE_BUILDDIR)
	[ -z "$(PULSE_JAVA_CLASS_DIR)" ] || rm -rf $(PULSE_JAVA_CLASS_DIR)
	rm -f pulse-java.jar
	rm -f libpulse-java.so
	if [ $(abs_top_srcdir) != $(abs_top_builddir) ]; then \
	  rmdir $(abs_top_builddir)/pulseaudio ; \
	fi
endif
	rm -f stamps/pulse-java*.stamp

# end of pulse-java

# VisualVM

stamps/nbplatform.stamp: $(INITIAL_BOOTSTRAP_LINK_STAMP) stamps/extract-visualvm.stamp
if WITH_VISUALVM
	cd netbeans ; \
        rm nbbuild/external/binaries-list ; \
	mkdir -p ./nbbuild/netbeans_visualvm/ ; \
	ln -s $(SYSTEM_NETBEANS_DIR)/platform9/ ./nbbuild/netbeans_visualvm/platform9 ; \
	ln -s $(SYSTEM_NETBEANS_DIR)/apisupport1/ ./nbbuild/netbeans_visualvm/apisupport1 ; \
	ln -s $(SYSTEM_NETBEANS_DIR)/harness/ ./nbbuild/netbeans_visualvm/harness ; \
	if ! (uname -a | grep x86_64) ; then \
	  mkdir -p lib.profiler/release/lib/deployed/jdk15/linux ; \
	  mkdir -p lib.profiler/release/lib/deployed/jdk16/linux ; \
	  buildscript=./buildnative-linux-15.sh ; \
	else \
	  mkdir -p lib.profiler/release/lib/deployed/jdk15/linux-amd64 ; \
	  mkdir -p lib.profiler/release/lib/deployed/jdk16/linux-amd64 ; \
	  buildscript=./buildnative-linux64-15.sh ; \
	fi ; \
	cd lib.profiler/native/build ; \
	JAVA_HOME_15=$(ICEDTEA_HOME) JAVA_HOME_16=$(ICEDTEA_HOME) \
	  $$buildscript ; \
	cd ../../.. ; \
        JAVA_HOME=$(ICEDTEA_HOME) $(ANT) -Dpermit.jdk6.builds=true \
            -f nbbuild/build.xml bootstrap ; \
	for dir in lib.profiler lib.profiler.charts lib.profiler.ui lib.profiler.common profiler ; do \
	  JAVA_HOME=$(ICEDTEA_HOME) $(ANT) -Dpermit.jdk6.builds=true -Dbuildnumber=091020 \
            -Dnetbeans.dest.dir=$(abs_top_builddir)/netbeans/nbbuild/netbeans_visualvm \
	    -f $$dir/build.xml ; \
	done 
endif
	mkdir -p stamps
	touch $@

clean-nbplatform:
	rm -rf netbeans
	rm -f stamps/nbplatform.stamp

stamps/visualvm.stamp: stamps/icedtea.stamp stamps/nbplatform.stamp
if WITH_VISUALVM
	cd visualvm ; \
	ln -s $(abs_top_srcdir)/netbeans/nbbuild/netbeans_visualvm netbeans ; \
	JAVA_HOME=$(ICEDTEA_HOME) $(ANT) build
	mkdir -p $(BUILD_OUTPUT_DIR)/j2sdk-image/lib/visualvm/etc ; \
	mkdir -p $(BUILD_OUTPUT_DIR)/j2sdk-image/lib/visualvm/visualvm ; \
	sed "s/APPNAME=\`basename.*\`/APPNAME=visualvm/" \
	  visualvm/launcher/visualvm >> \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/bin/jvisualvm ; \
	chmod a+x $(BUILD_OUTPUT_DIR)/j2sdk-image/bin/jvisualvm ; \
	cp visualvm/launcher/visualvm.conf \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/lib/visualvm/etc ; \
	echo visualvm >> \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/lib/visualvm/etc/visualvm.clusters ; \
	echo profiler3 >> \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/lib/visualvm/etc/visualvm.clusters ; \
	cp -r visualvm/build/cluster/* \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/lib/visualvm/visualvm ; \
	cp -r netbeans/nbbuild/netbeans_visualvm/platform9 \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/lib/visualvm ; \
	cp -r netbeans/nbbuild/netbeans_visualvm/profiler3 \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/lib/visualvm	
endif
	mkdir -p stamps
	touch $@

clean-visualvm:
	rm -rf visualvm
	rm -f stamps/visualvm.stamp

# CACAO

stamps/cacao.stamp: stamps/rt-class-files.stamp
if BUILD_CACAO
if !USE_SYSTEM_CACAO
	cd cacao/cacao && \
	$(ARCH_PREFIX) ./configure \
	  --host=$(host_alias) \
	  --build=$(build_alias) \
	  --target=$(target_alias) \
	  --prefix=$(abs_top_builddir)/cacao/install \
	  --with-java-runtime-library=openjdk \
	  --with-java-runtime-library-prefix=$(abs_top_builddir)/openjdk \
	  --with-java-runtime-library-classes=$(abs_top_builddir)/lib/rt \
	  --enable-jre-layout $(CACAO_CONFIGURE_ARGS); \
	$(ARCH_PREFIX) $(MAKE) -j$(PARALLEL_JOBS) install
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
	install -m 644 cacao/install/jre/lib/$(INSTALL_ARCH_DIR)/server/libjsig.so \
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
	sed -i 's#-cacao KNOWN#-cacao ERROR#' jvm.cfg
	rm -f stamps/add-cacao.stamp

stamps/add-cacao-debug.stamp: stamps/icedtea-debug.stamp stamps/cacao.stamp
if ADD_CACAO_BUILD
	mkdir -p $(BUILD_DEBUG_JRE_ARCH_DIR)/cacao
	install -m 644 cacao/install/lib/libjvm.so \
		$(BUILD_DEBUG_JRE_ARCH_DIR)/cacao/
	install -m 644 cacao/install/jre/lib/$(INSTALL_ARCH_DIR)/server/libjsig.so \
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
	sed -i 's#-cacao KNOWN#-cacao ERROR#' jvm.cfg
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
			'--with-rmic=% '--with-additional-vms=% \
			'--disable-bootstrap% '--enable-bootstrap% , \
		$(CONFIGURE_ARGS)) \
	$(foreach i, openjdk hotspot corba jaxp jaxws jdk langtools, \
	  $(if $(findstring --with-$(i)-src-zip=, $(CONFIGURE_ARGS)),, --with-$(i)-src-zip=$(abs_top_builddir)/$(i).tar.gz))

ADD_ZERO_EXTRA_BUILD_ENV = \
	BUILD_LANGTOOLS=false ALT_LANGTOOLS_DIST=$(ICEDTEA_BUILD_DIR)/langtools/dist \
	BUILD_JAXP=false      ALT_JAXP_DIST=$(ICEDTEA_BUILD_DIR)/jaxp/dist \
	BUILD_JAXWS=false     ALT_JAXWS_DIST=$(ICEDTEA_BUILD_DIR)/jaxws/dist \
	BUILD_CORBA=false     ALT_CORBA_DIST=$(ICEDTEA_BUILD_DIR)/corba/dist \
	BUILD_JDK=false \
	DISTRIBUTION_PATCHES='$(foreach p,$(DISTRIBUTION_PATCHES),$(if $(findstring cacao,$(p)),,$(p)))'

stamps/add-zero.stamp: stamps/icedtea.stamp
	mkdir -p stamps
if ADD_ZERO_BUILD
	mkdir -p zerovm

	cd zerovm && \
	    $(ADD_ZERO_EXTRA_BUILD_ENV) \
		$(ARCH_PREFIX) $(abs_top_srcdir)/configure $(ADD_ZERO_CONFIGURE_ARGS)

	$(ADD_ZERO_EXTRA_BUILD_ENV) \
		$(ARCH_PREFIX) $(MAKE) -C zerovm $(ICEDTEA_BUILD_TARGET)

if ZERO_BUILD
	mkdir -p $(BUILD_JRE_ARCH_DIR)/shark
	cp -a zerovm/openjdk/build/$(OS_DIR)/hotspot/import/jre/lib/$(INSTALL_ARCH_DIR)/server/* \
		$(BUILD_JRE_ARCH_DIR)/shark/
	printf -- '-shark KNOWN\n' >> $(BUILD_JRE_ARCH_DIR)/jvm.cfg
	printf -- '-zero ERROR\n' >> $(BUILD_JRE_ARCH_DIR)/jvm.cfg
else
	mkdir -p $(BUILD_JRE_ARCH_DIR)/zero
	cp -a zerovm/openjdk/build/$(OS_DIR)/hotspot/import/jre/lib/$(INSTALL_ARCH_DIR)/server/* \
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
	sed -i 's#-zero KNOWN#-zero ERROR#' jvm.cfg
	sed -i 's#-shark KNOWN#-shark ERROR#' jvm.cfg
if ADD_ZERO_BUILD
	rm -rf zerovm
endif
	rm -f stamps/add-zero.stamp

stamps/add-zero-debug.stamp: stamps/icedtea-debug.stamp
	mkdir -p stamps
if ADD_ZERO_BUILD
	mkdir -p zerovm

	cd zerovm && \
	    $(ADD_ZERO_EXTRA_BUILD_ENV) \
		$(ARCH_PREFIX) $(abs_top_srcdir)/configure $(ADD_ZERO_CONFIGURE_ARGS)

	$(ADD_ZERO_EXTRA_BUILD_ENV) \
		$(ARCH_PREFIX) $(MAKE) -C zerovm $(ICEDTEA_DEBUG_BUILD_TARGET)

if ZERO_BUILD
	mkdir -p $(BUILD_DEBUG_JRE_ARCH_DIR)/shark
	cp -a zerovm/openjdk/build/$(OS_DIR)/hotspot/import/jre/lib/$(INSTALL_ARCH_DIR)/server/* \
		$(BUILD_DEBUG_JRE_ARCH_DIR)/shark/
	printf -- '-shark KNOWN\n' >> $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg
else
	mkdir -p $(BUILD_DEBUG_JRE_ARCH_DIR)/zero
	cp -a zerovm/openjdk/build/$(OS_DIR)/hotspot/import/jre/lib/$(INSTALL_ARCH_DIR)/server/* \
		$(BUILD_DEBUG_JRE_ARCH_DIR)/zero/
	printf -- '-zero KNOWN\n' >> $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg
endif
else
	printf -- '-zero ERROR\n' >> $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg
	printf -- '-shark ERROR\n' >> $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg
endif
	touch $@

clean-add-zero-debug:
	rm -rf $(BUILD_JRE_ARCH_DIR)/zero
	sed -i 's#-zero KNOWN#-zero ERROR#' jvm.cfg
	sed -i 's#-shark KNOWN#-shark ERROR#' jvm.cfg
	rm -f stamps/add-zero-debug.stamp

# end additional VMs

# jtreg

stamps/jtreg.stamp: stamps/icedtea.stamp
	rm -rf test/jtreg/classes
	mkdir -p test/jtreg/classes
	$(ICEDTEA_BOOT_DIR)/bin/javac $(IT_JAVACFLAGS) -d test/jtreg/classes \
	   `find $(abs_top_srcdir)/test/jtreg/com -name '*.java'`
	(cd $(abs_top_srcdir)/test/jtreg; \
	 $(ICEDTEA_BOOT_DIR)/bin/jar cfm $(abs_top_builddir)/test/jtreg.jar \
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
	$(ICEDTEA_BOOT_DIR)/bin/java -jar test/jtreg.jar -v1 -a -ignore:quiet \
		-w:test/hotspot/JTwork -r:test/hotspot/JTreport \
		-jdk:`pwd`/$(BUILD_OUTPUT_DIR)/j2sdk-image \
		$(ICEDTEA_JTREG_OPTIONS) \
		`pwd`/openjdk/hotspot/test \
	    | tee test/$@.log

check-langtools: stamps/jtreg.stamp
	mkdir -p test/langtools/JTwork test/langtools/JTreport
	$(ICEDTEA_BOOT_DIR)/bin/java -jar test/jtreg.jar -v1 -a -ignore:quiet \
		-w:test/langtools/JTwork -r:test/langtools/JTreport \
		-s -jdk:`pwd`/$(BUILD_OUTPUT_DIR)/j2sdk-image \
		$(ICEDTEA_JTREG_OPTIONS) \
		-exclude:$(abs_top_srcdir)/test/jtreg/excludelist.langtools.jtx \
		`pwd`/openjdk/langtools/test \
	    | tee test/$@.log

check-jdk: stamps/jtreg.stamp
	mkdir -p test/jdk/JTwork test/jdk/JTreport
if WITH_CACAO
	CACAO_EXCLUDE="-exclude:$(abs_top_srcdir)/test/jtreg/excludelist.jdk.cacao.jtx"
endif
	$(ICEDTEA_BOOT_DIR)/bin/java -jar test/jtreg.jar -v1 -a -ignore:quiet \
		-w:test/jdk/JTwork -r:test/jdk/JTreport \
		-s -jdk:`pwd`/$(BUILD_OUTPUT_DIR)/j2sdk-image \
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

jtregcheck: jtreg check-hotspot check-langtools check-jdk
	for i in hotspot langtools jdk; do \
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
	$(FIND) $(abs_top_builddir)/rt -name '*.java' \
	  | sort -u > $@
	for dir in $(ICEDTEA_BOOTSTRAP_DIRS) ; \
	do \
	$(FIND) $(abs_top_builddir)/$$dir -name '*.java' >> $@; \
	done

stamps/rt-class-files.stamp: $(INITIAL_BOOTSTRAP_LINK_STAMP) rt-source-files.txt
	mkdir -p lib/rt
if LACKS_SUN_AWT_TOOLKIT
	$(ICEDTEA_BOOT_DIR)/bin/javac $(IT_JAVACFLAGS) -d lib/rt \
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
	 $(ZIP) -qur $(abs_top_builddir)/bootstrap/boot/jre/lib/rt.jar \
	 com java javax sun );
endif
	mkdir -p stamps
	touch $@

clean-rt:
	rm -rf lib/rt
	rm -f stamps/rt-class-files.stamp stamps/rt.stamp
	rm -f rt-source-files.txt

# tools.jar
bootstrap/jdk1.7.0/lib/tools.jar: stamps/rt.stamp
	mkdir -p bootstrap/boot/lib
	ln -sf $(abs_top_builddir)/bootstrap/boot/jre/lib/rt.jar \
	  $(abs_top_builddir)/bootstrap/boot/lib/tools.jar
	mkdir -p bootstrap/jdk1.7.0/lib
	ln -sf $(abs_top_builddir)/bootstrap/boot/lib/tools.jar \
	  $(abs_top_builddir)/bootstrap/jdk1.7.0/lib/tools.jar

clean-tools-jar:
	rm -f bootstrap/jdk1.7.0/lib/tools.jar
	rm -f bootstrap/boot/lib/tools.jar

# rt-closed.jar.
bootstrap/jdk1.7.0/jre/lib/rt-closed.jar: stamps/rt.stamp
	mkdir -p bootstrap/jdk1.7.0/jre/lib
	ln -sf ${abs_top_builddir}/bootstrap/boot/jre/lib/rt.jar \
		bootstrap/jdk1.7.0/jre/lib/rt-closed.jar

stamps/plugs.stamp: bootstrap/jdk1.7.0/jre/lib/rt-closed.jar \
	bootstrap/jdk1.7.0/lib/tools.jar
	mkdir -p stamps
	touch $@

clean-plugs: clean-tools-jar clean-rt
	rm -f stamps/plugs.stamp
	rm -f bootstrap/jdk1.7.0/jre/lib/rt-closed.jar
	rm -rf bootstrap/jdk1.7.0

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

download-jaf-drop: stamps/download-jaf-drop.stamp

download-jaxp-drop: stamps/download-jaxp-drop.stamp

download-jaxws-drop: stamps/download-jaxws-drop.stamp

download-openjdk: stamps/download-openjdk.stamp

download-visualvm: stamps/download-visualvm.stamp

extract: stamps/extract.stamp

extract-cacao: stamps/extract-cacao.stamp

extract-openjdk: stamps/extract-openjdk.stamp

extract-visualvm: stamps/extract-visualvm.stamp

generated: stamps/generated.stamp

hotspot-ports: stamps/ports.stamp

icedtea: stamps/icedtea.stamp

icedtea-boot: stamps/icedtea-boot.stamp

icedtea-debug: stamps/icedtea-debug.stamp

icedtea-debug-stage2: stamps/icedtea-debug-stage2.stamp

icedtea-stage1: stamps/icedtea-stage1.stamp

icedtea-stage2: stamps/icedtea-stage2.stamp

jtreg: stamps/jtreg.stamp

liveconnect: stamps/liveconnect.stamp

liveconnect-dist: stamps/liveconnect-dist.stamp

native-ecj: stamps/native-ecj.stamp

nbplatform: stamps/nbplatform.stamp

netx-dist: stamps/netx-dist.stamp

overlay: stamps/overlay.stamp

patch: stamps/patch.stamp

patch-boot: stamps/patch-boot.stamp

patch-fsg: stamps/patch-fsg.stamp

plugin: stamps/plugin.stamp

plugs: stamps/plugs.stamp

pulse-java: stamps/pulse-java.stamp

rt: stamps/rt.stamp

rt-class-files: stamps/rt-class-files.stamp

visualvm: stamps/visualvm.stamp
