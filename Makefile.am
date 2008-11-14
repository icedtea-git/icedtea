OPENJDK_DATE = 06_nov_2008
OPENJDK_MD5SUM = 57062779a3c844621865faef8d423ad9
OPENJDK_VERSION = b39

CACAO_VERSION = 0.99.3
CACAO_MD5SUM = 80de3ad344c1a20c086ec5f1390bd1b8

NETBEANS_BASIC_CLUSTER = a7ea855f475fa3a982f094a45c155ab4
NETBEANS_PROFILER_MD5SUM = ff8e8abc42df6c6749e6b02bcf7bb0a5
VISUALVM_MD5SUM = 4b55bc623418818793392bb233da2927

if ENABLE_GCJWEBPLUGIN
GCJWEBPLUGIN_CLEAN = clean-gcjwebplugin
GCJWEBPLUGIN_TARGET = gcjwebplugin.so
else
GCJWEBPLUGIN_CLEAN =
GCJWEBPLUGIN_TARGET =
if ENABLE_PLUGIN
ICEDTEAPLUGIN_CLEAN = clean-IcedTeaPlugin
ICEDTEAPLUGIN_TARGET = IcedTeaPlugin.so
PLUGIN_PATCH = patches/icedtea-liveconnect.patch
LIVECONNECT_DIR = -C lib/rt netscape
else
ICEDTEAPLUGIN_CLEAN =
ICEDTEAPLUGIN_TARGET =
PLUGIN_PATCH = patches/icedtea-plugin.patch
LIVECONNECT_DIR =
endif
endif

if ENABLE_PULSE_JAVA
# include the makefile in pulseaudio subdir
PULSE_JAVA_DIR = $(abs_top_srcdir)/pulseaudio
PULSE_JAVA_NATIVE_SRCDIR = $(PULSE_JAVA_DIR)/src/native
PULSE_JAVA_JAVA_SRCDIR = $(PULSE_JAVA_DIR)/src/java
PULSE_JAVA_CLASS_DIR = $(abs_top_builddir)/pulseaudio
else 
PULSE_JAVA_DIR =
PULSE_JAVA_NATIVE_SRCDIR =
PULSE_JAVA_JAVA_SRCDIR =
PULSE_JAVA_CLASS_DIR =
endif

if WITH_VISUALVM
VISUALVM_PATCH = patches/icedtea-visualvm.patch
else
VISUALVM_PATCH = 
endif

# Top-Level Targets
# =================

all-local: icedtea-against-icedtea

distclean-local: clean-copy clean-jtreg clean-jtreg-reports clean-pulse-java
	rm -rf stamps
	rm -f rt-source-files.txt \
	  hotspot-tools-source-files.txt \
	  extra-source-files.txt
	rm -rf bootstrap
	rm -rf lib
	rm -rf extra-lib
	rm -rf openjdk-ecj
	rm -rf openjdk
	rm -rf hotspot-tools
	rm -rf rt/netscape
if ENABLE_PLUGIN
	rm -f IcedTeaPlugin.o IcedTeaPlugin.so
else
if ENABLE_GCJWEBPLUGIN
	rm -f gcjwebplugin.so
endif
endif
	rm -rf cacao
	rm -rf visualvm
	rm -rf netbeans
	rm -f jni-common.o

install:

.PHONY: \
	env env-ecj snapshot clean-clone clean-patch \
	clean-clone-ecj clean-patch-ecj clean-bootstrap-directory \
	clean-bootstrap-directory-symlink clean-bootstrap-directory-ecj \
	clean-bootstrap-directory-symlink-ecj icedtea icedtea-debug \
	clean-icedtea icedtea-against-icedtea clean-icedtea-ecj clean-plugs \
	clean-tools-jar clean-shared-objects \
	clean-copy clean-hotspot-tools clean-rt $(GCJWEBPLUGIN_CLEAN) \
	$(ICEDTEAPLUGIN_CLEAN) hotspot hotspot-helper clean-extra \
	clean-jtreg clean-jtreg-reports clean-visualvm clean-nbplatform

EXTRA_DIST = generated $(abs_top_srcdir)/patches/icedtea-*.patch \
	gcjwebplugin.cc tools-copy contrib ports \
	extra overlays \
	javaws.png javaws.desktop visualvm.desktop \
	jconsole.desktop policytool.desktop \
	test/jtreg \
	IcedTeaPlugin.cc \
	HACKING pulseaudio fsg.sh \
	plugin

# The Binary plugs directory is called jdk1.7.0 for historical reasons. The
# name is completely irrelevant; only contains the plugs to build IcedTea.

# OpenJDK build environment.
LINUX_DIR = linux-$(BUILD_ARCH_DIR)
BUILD_OUTPUT_DIR = openjdk/build/$(LINUX_DIR)
ECJ_BUILD_OUTPUT_DIR = openjdk-ecj/build/$(LINUX_DIR)
ICEDTEA_BUILD_DIR = \
	$(abs_top_builddir)/$(BUILD_OUTPUT_DIR)
ICEDTEA_RT = \
	$(abs_top_builddir)/bootstrap/jdk1.7.0/jre/lib/rt-closed.jar
ICEDTEA_CLS_DIR = \
	$(abs_top_builddir)/$(BUILD_OUTPUT_DIR)/classes
ICEDTEA_BOOT_DIR= \
	$(abs_top_builddir)/bootstrap/jdk1.6.0
ICEDTEA_ENDORSED_DIR = \
	$(ICEDTEA_BOOT_DIR)/lib/endorsed
JDK_UPDATE_VERSION = 0
COMBINED_VERSION = $(JDK_UPDATE_VERSION)-$(OPENJDK_VERSION)
if CORE_BUILD
  ICEDTEA_CORE_BUILD = yes
else
  ICEDTEA_CORE_BUILD =
endif
if ZERO_BUILD
  ICEDTEA_ZERO_BUILD = yes
else
  ICEDTEA_ZERO_BUILD =
endif
if SHARK_BUILD
  ICEDTEA_SHARK_BUILD = yes
else
  ICEDTEA_SHARK_BUILD =
endif
ICEDTEA_ENV = \
	"ALT_JDK_IMPORT_PATH=$(ICEDTEA_BOOT_DIR)" \
	"ANT_HOME=$(SYSTEM_ANT_DIR)" \
        "BUILD_NUMBER=$(OPENJDK_VERSION)" \
	"JDK_UPDATE_VERSION=$(JDK_UPDATE_VERSION)" \
        "JRE_RELEASE_VERSION=1.7.0_$(COMBINED_VERSION)" \
	"LANG=C" \
	"PATH=$(abs_top_builddir)/bootstrap/jdk1.6.0/bin:/usr/bin:/bin:/usr/sbin:/sbin" \
	"ALT_BOOTDIR=$(ICEDTEA_BOOT_DIR)" \
	"ALT_BINARY_PLUGS_PATH=$(abs_top_builddir)/bootstrap/jdk1.7.0" \
	"BUILD_ARCH_DIR=$(BUILD_ARCH_DIR)" \
	"ICEDTEA_RT=$(ICEDTEA_RT)" \
	"ICEDTEA_BUILD_DIR=$(ICEDTEA_BUILD_DIR)" \
	"ICEDTEA_CLS_DIR=$(ICEDTEA_CLS_DIR)" \
	"ICEDTEA_ENDORSED_DIR=$(ICEDTEA_ENDORSED_DIR)" \
	"ENDORSED=-Djava.endorsed.dirs=$(ICEDTEA_ENDORSED_DIR)" \
	"CLASSPATH=" \
	"LD_LIBRARY_PATH=" \
	"ICEDTEA_CORE_BUILD=$(ICEDTEA_CORE_BUILD)" \
	"ICEDTEA_ZERO_BUILD=$(ICEDTEA_ZERO_BUILD)" \
	"ICEDTEA_SHARK_BUILD=$(ICEDTEA_SHARK_BUILD)" \
	"ZERO_LIBARCH=$(ZERO_LIBARCH)" \
	"ZERO_BITSPERWORD=$(ZERO_BITSPERWORD)" \
	"ZERO_ENDIANNESS=$(ZERO_ENDIANNESS)" \
	"ZERO_ARCHDEF=$(ZERO_ARCHDEF)" \
	"ZERO_ARCHFLAG=$(ZERO_ARCHFLAG)" \
	"LIBFFI_CFLAGS=$(LIBFFI_CFLAGS)" \
	"LIBFFI_LIBS=$(LIBFFI_LIBS)" \
	"LLVM_CFLAGS=$(LLVM_CFLAGS)" \
	"LLVM_LDFLAGS=$(LLVM_LDFLAGS)" \
	"LLVM_LIBS=$(LLVM_LIBS)" \
	"FREETYPE2_HEADERS=$(FREETYPE2_CFLAGS)" \
	"FT2_LIB=$(FREETYPE2_LIBS)" \
	"ALT_PARALLEL_COMPILE_JOBS=$(PARALLEL_JOBS)" \
	"HOTSPOT_BUILD_JOBS=$(PARALLEL_JOBS)" \
	"JAVAC=" \
	"RHINO_JAR=$(RHINO_JAR)" \
	"DISTRIBUTION_ID=$(DIST_ID)"

if WITH_CACAO
ICEDTEA_ENV += \
	"ALT_HOTSPOT_IMPORT_PATH=$(CACAO_IMPORT_PATH)"
endif

if !ENABLE_OPTIMIZATIONS
ICEDTEA_ENV += \
	"NO_OPTIMIZATIONS=true" \
	"CC_NO_OPT=-O0 -g"
endif

if !ENABLE_DOCS
ICEDTEA_ENV += \
	"NO_DOCS=true"
endif

if USE_ALT_JAR
ICEDTEA_ENV += \
	"ALT_JAR_CMD=$(ALT_JAR_CMD)"
endif

env:
	@echo 'unset JAVA_HOME'
	@echo 'export $(ICEDTEA_ENV)'

# OpenJDK ecj build environment.
ICEDTEA_BUILD_DIR_ECJ = \
	$(abs_top_builddir)/$(ECJ_BUILD_OUTPUT_DIR)/
ICEDTEA_CLS_DIR_ECJ = \
	$(abs_top_builddir)/$(ECJ_BUILD_OUTPUT_DIR)/classes
ICEDTEA_ENV_ECJ = \
	"ALT_JDK_IMPORT_PATH=$(ICEDTEA_BOOT_DIR)" \
	"ANT_HOME=$(SYSTEM_ANT_DIR)" \
	"BUILD_NUMBER=$(OPENJDK_VERSION)" \
	"JDK_UPDATE_VERSION=$(JDK_UPDATE_VERSION)" \
        "JRE_RELEASE_VERSION=1.7.0_$(COMBINED_VERSION)" \
	"LANG=C" \
	"PATH=$(abs_top_builddir)/bootstrap/jdk1.6.0/bin:/usr/bin:/bin:/usr/sbin:/sbin" \
	"ALT_BOOTDIR=$(ICEDTEA_BOOT_DIR)" \
	"ALT_BINARY_PLUGS_PATH=$(abs_top_builddir)/bootstrap/jdk1.7.0" \
	"BUILD_ARCH_DIR=$(BUILD_ARCH_DIR)" \
	"LIBGCJ_JAR=$(LIBGCJ_JAR)" \
	"ICEDTEA_RT=$(ICEDTEA_RT)" \
	"ICEDTEA_BUILD_DIR=$(ICEDTEA_BUILD_DIR_ECJ)" \
	"ICEDTEA_CLS_DIR=$(ICEDTEA_CLS_DIR_ECJ)" \
	"ICEDTEA_ENDORSED_DIR=$(ICEDTEA_ENDORSED_DIR)" \
	"ENDORSED=-Djava.endorsed.dirs=$(ICEDTEA_ENDORSED_DIR)" \
	"BOOTCLASSPATH_CLS_RT=-bootclasspath \
	  $(ICEDTEA_CLS_DIR_ECJ):$(ICEDTEA_RT)" \
	"BOOTCLASSPATH_CLS=-bootclasspath $(ICEDTEA_CLS_DIR_ECJ)" \
	"BOOTCLASSPATH_RT_LIBGCJ=-bootclasspath $(ICEDTEA_RT):$(LIBGCJ_JAR)" \
	"CLASSPATH=" \
	"LD_LIBRARY_PATH=" \
	"GENSRCDIR=$(abs_top_builddir)/generated" \
	"ICEDTEA_CORE_BUILD=$(ICEDTEA_CORE_BUILD)" \
	"ICEDTEA_ZERO_BUILD=$(ICEDTEA_ZERO_BUILD)" \
	"ICEDTEA_SHARK_BUILD=$(ICEDTEA_SHARK_BUILD)" \
	"ZERO_LIBARCH=$(ZERO_LIBARCH)" \
	"ZERO_BITSPERWORD=$(ZERO_BITSPERWORD)" \
	"ZERO_ENDIANNESS=$(ZERO_ENDIANNESS)" \
	"ZERO_ARCHDEF=$(ZERO_ARCHDEF)" \
	"ZERO_ARCHFLAG=$(ZERO_ARCHFLAG)" \
	"LIBFFI_CFLAGS=$(LIBFFI_CFLAGS)" \
	"LIBFFI_LIBS=$(LIBFFI_LIBS)" \
	"LLVM_CFLAGS=$(LLVM_CFLAGS)" \
	"LLVM_LDFLAGS=$(LLVM_LDFLAGS)" \
	"LLVM_LIBS=$(LLVM_LIBS)" \
	"FREETYPE2_HEADERS=$(FREETYPE2_CFLAGS)" \
	"FT2_LIB=$(FREETYPE2_LIBS)" \
	"ALT_PARALLEL_COMPILE_JOBS=$(PARALLEL_JOBS)" \
	"HOTSPOT_BUILD_JOBS=$(PARALLEL_JOBS)" \
	"JAVAC=" \
	"RHINO_JAR=$(RHINO_JAR)" \
	"JAR_KNOWS_ATFILE=$(JAR_KNOWS_ATFILE)" \
	"JAR_KNOWS_J_OPTIONS=$(JAR_KNOWS_J_OPTIONS)" \
	"JAR_ACCEPTS_STDIN_LIST=$(JAR_ACCEPTS_STDIN_LIST)"

if WITH_CACAO
ICEDTEA_ENV_ECJ += \
	"ALT_HOTSPOT_IMPORT_PATH=$(CACAO_IMPORT_PATH)"
endif

if !ENABLE_OPTIMIZATIONS
ICEDTEA_ENV_ECJ += \
	"NO_OPTIMIZATIONS=true" \
	"CC_NO_OPT=-O0 -g"
endif

if !ENABLE_DOCS
ICEDTEA_ENV_ECJ += \
	"NO_DOCS=true"
endif

env-ecj:
	@echo 'unset JAVA_HOME'
	@echo 'export $(ICEDTEA_ENV_ECJ)'

# Mercurial snapshot.
snapshot: dist
	mv $(DIST_ARCHIVES) $(distdir)-`$(HG) tip --template '{node}'`.tar.gz

# Creates archive of openjdk.
dist-openjdk: stamps/extract-cacao.stamp
	find openjdk/ -name \\.hg* | xargs rm -rf	
	$(ZIP) -r openjdk-$(OPENJDK_VERSION) openjdk/

# OpenJDK Source Preparation Targets
# ==================================

# Download OpenJDK sources.

OPENJDK_HG_URL = http://hg.openjdk.java.net/jdk7/jdk7
CVMI_HG_URL = http://hg.openjdk.java.net/cvmi/cvmi/
CLOSURES_HG_URL = http://hg.openjdk.java.net/closures/closures/
CACIOCAVALLO_HG_URL = http://hg.openjdk.java.net/caciocavallo/jdk7/
BSD_HG_URL = http://hg.openjdk.java.net/bsd-port/bsd-port

OPENJDK_URL = http://www.java.net/download/openjdk/jdk7/promoted/$(OPENJDK_VERSION)/
if USE_ALT_OPENJDK_SRC_ZIP
  OPENJDK_SRC_ZIP = $(ALT_OPENJDK_SRC_ZIP)
else
if USE_HG
  OPENJDK_SRC_ZIP = 
else
  OPENJDK_SRC_ZIP = openjdk-7-ea-src-$(OPENJDK_VERSION)-$(OPENJDK_DATE).zip
endif
endif

CACAO_BASE_URL = http://www.complang.tuwien.ac.at
CACAO_URL = $(CACAO_BASE_URL)/cacaojvm/download/cacao-$(CACAO_VERSION)/

if USE_ALT_CACAO_SRC_ZIP
  CACAO_SRC_ZIP = $(ALT_CACAO_SRC_ZIP)
else
  CACAO_SRC_ZIP = cacao-$(CACAO_VERSION).tar.gz
endif

NETBEANS_BASIC_CLUSTER_URL = http://nbi.netbeans.org/files/documents/210/2056/

if USE_ALT_NETBEANS_BASIC_CLUSTER_SRC_ZIP
  NETBEANS_BASIC_CLUSTER_SRC_ZIP = $(ALT_NETBEANS_BASIC_CLUSTER_SRC_ZIP)
else
  NETBEANS_BASIC_CLUSTER_SRC_ZIP = netbeans-6.1-200805300101-basic_cluster-src.zip 
endif

NETBEANS_PROFILER_URL = http://icedtea.classpath.org/visualvm/

if USE_ALT_NETBEANS_PROFILER_SRC_ZIP
  NETBEANS_PROFILER_SRC_ZIP = $(ALT_NETBEANS_PROFILER_SRC_ZIP)
else
  NETBEANS_PROFILER_SRC_ZIP = netbeans-profiler-visualvm_preview2.tar.gz
endif

VISUALVM_URL = http://icedtea.classpath.org/visualvm/

if USE_ALT_VISUALVM_SRC_ZIP
  VISUALVM_SRC_ZIP = $(ALT_VISUALVM_SRC_ZIP)
else
  VISUALVM_SRC_ZIP = visualvm-20080728-src.tar.gz
endif

stamps/download.stamp: stamps/hgforest.stamp
if !OPENJDK_SRC_DIR_FOUND
if USE_ALT_OPENJDK_SRC_ZIP
else
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
if USE_HG
	$(HG) fclone -r jdk7-$(OPENJDK_VERSION) $(OPENJDK_HG_URL) openjdk;
else
	if ! echo "$(OPENJDK_MD5SUM)  $(OPENJDK_SRC_ZIP)" \
	  | $(MD5SUM) --check ; \
	then \
	  if [ $(OPENJDK_SRC_ZIP) ] ; \
	  then \
	    mv $(OPENJDK_SRC_ZIP) $(OPENJDK_SRC_ZIP).old ; \
	  fi ; \
	  $(WGET) $(OPENJDK_URL)$(OPENJDK_SRC_ZIP) -O $(OPENJDK_SRC_ZIP); \
	fi
endif
endif
endif
endif
endif
endif
endif
if WITH_CACAO
if !USE_SYSTEM_CACAO
if USE_ALT_CACAO_SRC_ZIP
else
	if ! echo "$(CACAO_MD5SUM)  $(CACAO_SRC_ZIP)" \
	 | $(MD5SUM) --check ; \
	then \
	 if [ $(CACAO_SRC_ZIP) ] ; \
	 then \
	   mv $(CACAO_SRC_ZIP) $(CACAO_SRC_ZIP).old ; \
	 fi ; \
	 $(WGET) $(CACAO_URL)$(CACAO_SRC_ZIP) -O $(CACAO_SRC_ZIP); \
	fi
endif
endif
endif
if WITH_VISUALVM
if USE_ALT_NETBEANS_BASIC_CLUSTER_SRC_ZIP
else
	if ! echo "$(NETBEANS_BASIC_CLUSTER_MD5SUM) $(NETBEANS_BASIC_CLUSTER_SRC_ZIP)" \
	  | $(MD5SUM) --check ; \
	then \
	  if [ $(NETBEANS_BASIC_CLUSTER_SRC_ZIP) ] ; \
	  then \
	    mv $(NETBEANS_BASIC_CLUSTER_SRC_ZIP) $(NETBEANS_BASIC_CLUSTER_SRC_ZIP).old ; \
	  fi ; \
	$(WGET) $(NETBEANS_BASIC_CLUSTER_URL)$(NETBEANS_BASIC_CLUSTER_SRC_ZIP) \
	  -O $(NETBEANS_BASIC_CLUSTER_SRC_ZIP) ; \
	fi
endif
if USE_ALT_NETBEANS_PROFILER_SRC_ZIP
else
	if ! echo "$(NETBEANS_PROFILER_MD5SUM) $(NETBEANS_PROFILER_SRC_ZIP)" \
	  | $(MD5SUM) --check ; \
	then \
	  if [ $(NETBEANS_PROFILER_SRC_ZIP) ] ; \
	  then \
	    mv $(NETBEANS_PROFILER_SRC_ZIP) $(NETBEANS_PROFILER_SRC_ZIP).old ; \
	  fi ; \
	  $(WGET) $(NETBEANS_PROFILER_URL)$(NETBEANS_PROFILER_SRC_ZIP) \
	    -O $(NETBEANS_PROFILER_SRC_ZIP) ; \
	fi
endif
if USE_ALT_VISUALVM_SRC_ZIP
else
	if ! echo "$(VISUALVM_MD5SUM) $(VISUALVM_SRC_ZIP)" \
	  | $(MD5SUM) --check ; \
	then \
	  if [ $(VISUALVM_SRC_ZIP) ] ; \
	  then \
	    mv $(VISUALVM_SRC_ZIP) $(VISUALVM_SRC_ZIP).old ; \
	  fi ; \
	  $(WGET) $(VISUALVM_URL)$(VISUALVM_SRC_ZIP) -O $(VISUALVM_SRC_ZIP) ; \
	fi
endif
endif
	mkdir -p stamps
	touch stamps/download.stamp

stamps/hgforest.stamp:
if OPENJDK_SRC_DIR_FOUND
else
if USE_ALT_OPENJDK_SRC_ZIP
else
if !USE_HG
else
	if ! $(HG) fclone -h; \
          then \
          echo "No forest extension found. Please refer to" \
            "http://selenic.com/mercurial/wiki/index.cgi/ForestExtension"; \
          exit 1; \
          fi;
endif
endif
endif
	mkdir -p stamps 
	touch stamps/hgforest.stamp

clean-hgforest:
	rm -f stamps/hgforest.stamp

clean-extract:
	rm -f stamps/extract.stamp
	rm -rf openjdk

clean-download:
	rm -f stamps/download.stamp
	rm -f $(OPENJDK_SRC_ZIP)
if WITH_CACAO
	rm -f $(CACAO_SRC_ZIP)
endif
if WITH_VISUALVM
	rm -f $(VISUALVM_SRC_ZIP) 
	rm -f $(NETBEANS_PROFILER_SRC_ZIP)
	rm -rf $(NETBEANS_BASIC_CLUSTER_SRC_ZIP)
endif

# Link ports sources into tree
stamps/ports.stamp: stamps/extract.stamp
	for target in $(abs_top_srcdir)/ports/hotspot/make/*/makefiles/* \
		      $(abs_top_srcdir)/ports/hotspot/src/*cpu/* \
		      $(abs_top_srcdir)/ports/hotspot/src/share/vm/*; do \
	  link=$$(dirname $$target | sed 's/^.*ports/openjdk/'); \
	  ln -sf $$target $$link; \
	done
	ln -sf $(abs_top_builddir)/platform_zero openjdk/hotspot/make/linux
	if ! test "x$(ICEDTEA_CORE_BUILD)$(ICEDTEA_SHARK_BUILD)$(WITH_CACAO)" \
	  = "xno" ; then \
	  mkdir -p openjdk/jdk/src/solaris/bin/$(BUILD_ARCH_DIR) ; \
	  ln -sf $(abs_top_builddir)/jvm.cfg \
	    openjdk/jdk/src/solaris/bin/$(BUILD_ARCH_DIR)/jvm.cfg ; \
	fi
	if ! test "x$(ICEDTEA_ZERO_BUILD)$(WITH_CACAO)" = "xno"; then \
	  ln -sf $(abs_top_builddir)/ergo.c \
	    openjdk/jdk/src/solaris/bin/ergo_$(BUILD_ARCH_DIR).c; \
	fi
	mkdir -p stamps
	touch stamps/ports.stamp

# Patches from the other OpenJDK trees (should be removed
# when in an official bxx release)
OPENJDK_PATCHES = 

# If ZERO_BUILD is set then we are building zero.
ZERO_PATCHES = \
	patches/icedtea-hotspot7-build-fixes.patch \
	patches/icedtea-bytecodeInterpreter.patch \
	patches/icedtea-signature-iterator.patch \
	patches/icedtea-test-atomic-operations.patch \
	patches/icedtea-zero.patch \
	patches/icedtea-ia64-bugfix.patch 

# Patches needed when not using the newer OpenJDK 7 HotSpot for zero.
NON_ZERO_PATCHES = 

if ZERO_BUILD
  ZERO_PATCHES_COND = $(ZERO_PATCHES)
else
  ZERO_PATCHES_COND = 
endif

# If Shark is required, apply shark patch
if SHARK_BUILD
  SHARK_PATCH = patches/icedtea-shark.patch
else
  SHARK_PATCH =
endif

ICEDTEA_FSG_PATCHES = \
	patches/icedtea-idl.patch \
	patches/icedtea-jscheme.patch

ICEDTEA_PATCHES = \
	patches/icedtea-copy-plugs.patch \
	patches/icedtea-version.patch \
	patches/icedtea-text-relocations.patch \
	patches/icedtea-graphics.patch \
	patches/icedtea-debuginfo.patch \
	patches/icedtea-ssl.patch \
	$(PLUGIN_PATCH) \
	patches/icedtea-webstart.patch \
	patches/icedtea-webstart-umask.patch \
	patches/icedtea-rmi_amd64.patch \
	patches/icedtea-tools.patch \
	patches/icedtea-demos.patch \
	patches/icedtea-use-system-tzdata.patch \
	patches/icedtea-headers.patch \
	patches/icedtea-ant.patch \
	patches/icedtea-gcc-suffix.patch \
	patches/icedtea-bytebuffer-compact.patch \
	patches/icedtea-float-double-trailing-zeros.patch \
	patches/icedtea-memory-limits.patch \
	patches/icedtea-sunsrc.patch \
	patches/icedtea-libraries.patch \
	patches/icedtea-core-build.patch \
	patches/icedtea-linker-options.patch \
	patches/icedtea-static-libstdc++.patch \
	patches/icedtea-lib64.patch \
	patches/icedtea-zero-build.patch \
	patches/icedtea-print-lsb-release.patch \
	patches/icedtea-jpegclasses.patch \
	patches/icedtea-uname.patch \
	patches/icedtea-ia64-fdlibm.patch \
	patches/icedtea-fonts.patch \
	patches/icedtea-directaudio-close-trick.patch \
	patches/icedtea-sparc64-linux.patch \
	patches/icedtea-sparc-ptracefix.patch \
	patches/icedtea-sparc-trapsfix.patch \
	patches/icedtea-override-redirect-metacity.patch \
	patches/icedtea-no-bcopy.patch \
	patches/icedtea-shark-build.patch \
	patches/icedtea-toolkit.patch \
	patches/icedtea-security-updates.patch \
	patches/icedtea-jsoundhs.patch \
	patches/icedtea-jdk-docs-target.patch \
	patches/icedtea-mbeanintrospector.patch \
	patches/icedtea-hotspot-gcc-pr36917.patch \
	patches/icedtea-alpha-fixes.patch \
	patches/icedtea-alt-jar.patch \
	patches/icedtea-enum-bug-181.patch \
	patches/icedtea-jdk-use-ssize_t.patch \
	patches/icedtea-hotspot-use-idx_t.patch \
	patches/icedtea-hotspot-params-cast-size_t.patch \
	patches/icedtea-clean-crypto.patch \
	$(SHARK_PATCH) \
	$(GCC_PATCH) \
	patches/icedtea-arch.patch \
	patches/icedtea-signed-types.patch \
	patches/icedtea-lc_ctype.patch \
	patches/icedtea-messageutils.patch \
	$(VISUALVM_PATCH) \
	patches/icedtea-javac-debuginfo.patch \
	patches/icedtea-xjc.patch \
	patches/icedtea-renderer-crossing.patch \
	patches/icedtea-snmp.patch \
	patches/icedtea-sound.patch \
	$(ZERO_PATCHES_COND)

if WITH_RHINO
ICEDTEA_PATCHES += \
	patches/icedtea-rhino.patch
endif

if WITH_CACAO
ICEDTEA_PATCHES += \
	patches/icedtea-cacao.patch \
	patches/icedtea-cacao-ignore-jdi-tests.patch \
	patches/icedtea-cacao-ignore-tests.patch
endif

if ENABLE_PULSE_JAVA
ICEDTEA_PATCHES += \
	patches/icedtea-pulse-soundproperties.patch
endif

if !USE_CVMI
ICEDTEA_PATCHES += \
	patches/icedtea-javafiles.patch
endif

ICEDTEA_PATCHES += \
	$(DISTRIBUTION_PATCHES)

stamps/extract.stamp: stamps/download.stamp
if OPENJDK_SRC_DIR_FOUND
	cp -a $(OPENJDK_SRC_DIR) openjdk
else
	if ! test -d openjdk ; then \
	  if [ ! -z $(OPENJDK_SRC_ZIP) ] ; then \
	    $(UNZIP) -q $(OPENJDK_SRC_ZIP) ; \
	  else \
	    echo "ERROR: Couldn't extract OpenJDK"; \
	    false; \
	  fi; \
	fi; 
endif
	chmod -R ug+w openjdk 
	sh $(srcdir)/fsg.sh ; 
	mkdir -p stamps 
	touch stamps/extract.stamp

stamps/extract-cacao.stamp: stamps/extract.stamp
if WITH_CACAO
if !USE_SYSTEM_CACAO
	if ! test -d cacao ; \
	then \
	  mkdir cacao ; \
	  $(TAR) xf $(CACAO_SRC_ZIP) -C cacao ; \
	  dir=$$(basename cacao/cacao-*) ; \
	  ln -s $$dir cacao/cacao ; \
	fi
endif
endif
if WITH_VISUALVM
	if ! test -d visualvm ; \
	then \
	  $(TAR) xf $(VISUALVM_SRC_ZIP) ; \
	fi
	if ! test -d netbeans ; \
	then \
	  mkdir netbeans ; \
	  $(TAR) xf $(NETBEANS_PROFILER_SRC_ZIP) -C netbeans ; \
	  $(UNZIP) -q $(NETBEANS_BASIC_CLUSTER_SRC_ZIP) nbbuild/* -d netbeans/ ; \
	  mv netbeans/profiler/ netbeans/profiler.cvs/ ; \
	  $(UNZIP) -q $(NETBEANS_BASIC_CLUSTER_SRC_ZIP) \
	  api.java/* api.progress/* apisupport.harness/* editor.mimelookup/* \
	  o.jdesktop.layout/* openide.actions/* openide.awt/* \
	  openide.dialogs/* openide.execution/* openide.explorer/* \
	  openide.filesystems/* openide.io/* openide.loaders/* \
	  openide.modules/* openide.nodes/* openide.text/* openide.util/* \
	  openide.windows/* options.api/* projectapi/* queries/* java.platform/* \
	  -d netbeans/ ; \
	fi
endif
	touch stamps/extract-cacao.stamp

stamps/generated.stamp: stamps/extract-cacao.stamp
	if [ ! -e $(abs_top_builddir)/generated ]; then \
	  cp -a $(abs_top_srcdir)/generated $(abs_top_builddir); \
	  find $(abs_top_builddir)/generated -type f -exec chmod 640 '{}' ';' \
	    -o -type d -exec chmod 750 '{}' ';'; \
	fi
	touch stamps/generated.stamp

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
	mv stamps/patch.stamp.tmp stamps/patch.stamp ; \
	if ! test x$${all_patches_ok} = "xyes"; then \
	  echo ERROR patch $${all_patches_ok} FAILED! ; \
	  echo WARNING make clean-patch before retrying a fix ; \
	  exit 2; \
	fi

	if [ -e $(abs_top_srcdir)/.hg ] && which $(HG) >/dev/null; then \
	  revision="-r`(cd $(abs_top_srcdir); $(HG) tip --template '{node|short}')`" ; \
	fi ; \
	icedtea_version="$(PACKAGE_VERSION)$${revision}" ; \
	if [ -n "$(PKGVERSION)" ]; then \
	  icedtea_version="$${icedtea_version} ($(PKGVERSION))" ; \
	fi; \
	sed -i "s#IcedTea6#IcedTea6 $${icedtea_version}#" openjdk/jdk/make/common/shared/Defs.gmk

if ENABLE_PLUGIN
	cp -a $(abs_top_srcdir)/plugin/icedtea/sun/applet/*java openjdk/jdk/src/share/classes/sun/applet/
	mkdir -p rt
	cp -a $(abs_top_srcdir)/plugin/icedtea/netscape rt/
endif

clean-patch:
	rm -f stamps/patch.stamp
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
	fi
	rm -rf rt/netscape
	for file in plugin/icedtea/sun/applet/*java ; \
	do \
		rm -f openjdk/jdk/src/share/classes/sun/applet/`basename $file` ; \
	done ;

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

clean-patch-fsg:
	rm -f stamps/patch-fsg.stamp
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
	fi

stamps/native-ecj.stamp: stamps/patch.stamp
	mkdir -p stamps ; \
	if test "x${GCJ}" != x; then \
	  ${GCJ} ${CFLAGS} -Wl,-Bsymbolic -o native-ecj \
	    --main=org.eclipse.jdt.internal.compiler.batch.Main ${ECJ_JAR} ; \
	fi ; \
	touch stamps/native-ecj.stamp

stamps/overlay.stamp: 
	if [ -e $(abs_top_srcdir)/.hg ] && which $(HG) >/dev/null; then \
	  revision="-r`(cd $(abs_top_srcdir); $(HG) tip --template '{rev}')`" ; \
	fi ; \
	icedtea_version="$(PACKAGE_VERSION)$${revision}" ; \
	if [ -n "$(PKGVERSION)" ]; then \
	  icedtea_version="$${icedtea_version} ($(PKGVERSION))" ; \
	fi; \
	if test x"$(PROJECT_NAME)" != "xjdk7"; then \
	  proj_suffix="-$(PROJECT_NAME)"; \
	fi ; \
	if test x"$(VERSION_SUFFIX)" != "x"; then \
	  ver_suffix="-$(VERSION_SUFFIX)"; \
	fi ; \
	sed -i "s#IcedTea#IcedTea $${icedtea_version}#" openjdk/jdk/make/common/shared/Defs.gmk ; \
	sed -i "s#BUILD_VARIANT_RELEASE)#BUILD_VARIANT_RELEASE)$${proj_suffix}$${ver_suffix}#" \
	  openjdk/jdk/make/common/shared/Defs.gmk; \
	find openjdk -name '*.java' -exec sed -i 's/^[[:space:]]*@Override//' {} + ; \
	cp -r $(abs_top_srcdir)/overlays/openjdk/* openjdk/ \
	  && touch stamps/overlay.stamp

# OpenJDK ecj Source Preparation Targets
# ======================================

# Copy over OpenJDK sources for ecj.
stamps/clone-ecj.stamp: stamps/patch.stamp
	if ! test -d openjdk-ecj ; then \
	    cp -pPRl openjdk openjdk-ecj ; \
	fi
	mkdir -p stamps
	touch stamps/clone-ecj.stamp

clean-clone-ecj:
	rm -f stamps/clone-ecj.stamp
	rm -rf openjdk-ecj

# Link ports sources into tree
stamps/ports-ecj.stamp: stamps/clone-ecj.stamp
	for target in $(abs_top_srcdir)/ports/hotspot/make/*/makefiles/* \
		      $(abs_top_srcdir)/ports/hotspot/src/*cpu/* \
		      $(abs_top_srcdir)/ports/hotspot/src/share/vm/*; do \
	  link=$$(dirname $$target | sed 's/^.*ports/openjdk-ecj/'); \
	  ln -sf $$target $$link; \
	done
	ln -sf $(abs_top_builddir)/platform_zero openjdk-ecj/hotspot/make/linux
	if ! test "x$(ICEDTEA_CORE_BUILD)$(ICEDTEA_SHARK_BUILD)$(WITH_CACAO)" \
	  = "xno"; then \
	  mkdir -p openjdk-ecj/jdk/src/solaris/bin/$(BUILD_ARCH_DIR); \
	  ln -sf $(abs_top_builddir)/jvm.cfg \
	    openjdk-ecj/jdk/src/solaris/bin/$(BUILD_ARCH_DIR)/jvm.cfg; \
	fi
	if ! test "x$(ICEDTEA_ZERO_BUILD)$(WITH_CACAO)" = "xno"; then \
	  ln -sf $(abs_top_builddir)/ergo.c \
	    openjdk-ecj/jdk/src/solaris/bin/ergo_$(BUILD_ARCH_DIR).c; \
	fi
	mkdir -p stamps
	touch stamps/ports-ecj.stamp

# Patch OpenJDK for plug replacements and ecj.
ICEDTEA_ECJ_PATCHES = patches/icedtea-ecj.patch \
	patches/icedtea-ecj-spp.patch \
	patches/icedtea-ecj-jopt.patch

stamps/patch-ecj.stamp: stamps/clone-ecj.stamp
	mkdir -p stamps; \
	rm -f stamps/patch-ecj.stamp.tmp ; \
	touch stamps/patch-ecj.stamp.tmp ; \
	all_patches_ok=yes; \
	for p in $(ICEDTEA_ECJ_PATCHES) ; \
	do \
	  if test x$${all_patches_ok} = "xyes" \
	    && echo Checking $$p \
	    && $(PATCH) -l -p0 --dry-run -s -t -f \
	      -F 0 < $(abs_top_srcdir)/$$p ; \
	  then \
	    echo Applying $$p ; \
	    $(PATCH) -l -p0 < $(abs_top_srcdir)/$$p ; \
	    if ! grep "^\* $$(basename $$p)" $(abs_top_srcdir)/HACKING \
	      >> stamps/patch-ecj.stamp.tmp ; \
	    then \
	      echo "* $$(basename $$p): UNDOCUMENTED" \
	        >> stamps/patch-ecj.stamp.tmp ; \
	    fi ; \
	  else \
	    test x$${all_patches_ok} = "xyes" && all_patches_ok=$$p ; \
	  fi ; \
	done ; \
	mv stamps/patch-ecj.stamp.tmp stamps/patch-ecj.stamp ; \
	if ! test x$${all_patches_ok} = "xyes"; then \
	  echo ERROR patch $${all_patches_ok} FAILED! ; \
	  echo WARNING make clean-patch before retrying a fix ; \
	  exit 2; \
	fi

clean-patch-ecj:
	rm -f stamps/patch-ecj.stamp
	all_patches_ok=yes; \
	for p in $(ICEDTEA_ECJ_PATCHES) ; \
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
	fi

# Bootstrap Directory Targets
# ===========================

# bootstrap/icedtea.
stamps/bootstrap-directory.stamp: stamps/icedtea-against-ecj.stamp
	rm -rf bootstrap/icedtea
	ln -s \
	  $(abs_top_builddir)/$(ECJ_BUILD_OUTPUT_DIR)/j2sdk-image/ \
	  bootstrap/icedtea
	if ! test -d bootstrap/icedtea/include; then \
	  ln -sf $(ICEDTEA_BUILD_DIR_ECJ)/include bootstrap/icedtea/include; \
	fi
	mkdir -p stamps
	touch stamps/bootstrap-directory.stamp

clean-bootstrap-directory:
	rm -f stamps/bootstrap-directory.stamp
	rm -rf bootstrap/icedtea

if WITH_OPENJDK
  BOOTSTRAP_DIRECTORY_STAMP =
  ICEDTEA_HOME = $(SYSTEM_OPENJDK_DIR)
else
if WITH_ICEDTEA
  BOOTSTRAP_DIRECTORY_STAMP =
  ICEDTEA_HOME = $(SYSTEM_ICEDTEA_DIR)
else
  BOOTSTRAP_DIRECTORY_STAMP = stamps/bootstrap-directory.stamp
  ICEDTEA_HOME = $(abs_top_builddir)/bootstrap/icedtea
endif
endif

# bootstrap/jdk1.6.0 to bootstrap/icedtea symlink.
stamps/bootstrap-directory-symlink.stamp: $(BOOTSTRAP_DIRECTORY_STAMP)
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
	touch stamps/bootstrap-directory-symlink.stamp

clean-bootstrap-directory-symlink:
	rm -f stamps/bootstrap-directory-symlink.stamp
	rm -f bootstrap/jdk1.6.0

# Bootstrap ecj Directory Targets
# ===============================

# bootstrap/ecj.
stamps/bootstrap-directory-ecj.stamp: stamps/native-ecj.stamp
	mkdir -p bootstrap/ecj/bin stamps/
	ln -sf $(JAVA) bootstrap/ecj/bin/java
	ln -sf $(JAVAH) bootstrap/ecj/bin/javah
	ln -sf $(RMIC) bootstrap/ecj/bin/rmic
	ln -sf $(JAR) bootstrap/ecj/bin/jar
	ln -sf ../../../javac bootstrap/ecj/bin/javac
	ln -sf ../../../javap bootstrap/ecj/bin/javap
	mkdir -p bootstrap/ecj/lib/endorsed
	ln -sf $(XALAN2_JAR) bootstrap/ecj/lib/endorsed/xalan-j2.jar
	ln -sf $(XALAN2_SERIALIZER_JAR) \
	  bootstrap/ecj/lib/endorsed/xalan-j2-serializer.jar
	ln -sf $(XERCES2_JAR) bootstrap/ecj/lib/endorsed/xerces-j2.jar
	if test -d $(SYSTEM_GCJ_DIR); \
	then \
	  mkdir -p bootstrap/ecj/jre/lib; \
	  ln -sf $(SYSTEM_GCJ_DIR)/jre/lib/rt.jar \
	    bootstrap/ecj/jre/lib/rt-system.jar; \
	  ln -sf $(SYSTEM_GCJ_DIR)/jre/lib/$(JRE_ARCH_DIR) \
	    bootstrap/ecj/jre/lib/; \
	  if ! test -d bootstrap/ecj/jre/lib/$(INSTALL_ARCH_DIR); \
	  then \
	    ln -sf ./$(JRE_ARCH_DIR) \
	      bootstrap/ecj/jre/lib/$(INSTALL_ARCH_DIR); \
	  fi; \
	  mkdir -p bootstrap/ecj/include; \
	  for i in $(SYSTEM_GCJ_DIR)/include/*; do \
	    test -r $$i | continue; \
	    i=`basename $$i`; \
	    rm -f bootstrap/ecj/include/$$i; \
	    ln -s $(SYSTEM_GCJ_DIR)/include/$$i bootstrap/ecj/include/$$i; \
	  done; \
	else \
	  echo $(SYSTEM_GCJ_DIR) " cannot be found. Try reconfiguring with " \
	    --with-gcj-home=/path/to/java-gcj-devel ; \
          exit 1; \
	fi
	if test -f $(abs_top_builddir)/bootstrap/jdk1.7.0/jre/lib/tools.jar ; \
	then \
	  mkdir -p $(abs_top_builddir)/bootstrap/ecj/lib ; \
	  ln -sf $(abs_top_builddir)/bootstrap/jdk1.7.0/jre/lib/tools.jar \
	    $(abs_top_builddir)/bootstrap/ecj/lib/tools.jar ; \
	fi
	if test -f \
	  $(abs_top_builddir)/bootstrap/jdk1.7.0/jre/lib/rt-closed.jar ; \
	then \
	  cp $(abs_top_builddir)/bootstrap/jdk1.7.0/jre/lib/rt-closed.jar \
	    $(abs_top_builddir)/bootstrap/ecj/jre/lib/rt.jar ; \
	fi
	mkdir -p stamps
	touch stamps/bootstrap-directory-ecj.stamp

clean-bootstrap-directory-ecj:
	rm -f stamps/bootstrap-directory-ecj.stamp
	rm -rf bootstrap/ecj

# bootstrap/jdk1.6.0 to bootstrap/ecj symlink.
stamps/bootstrap-directory-symlink-ecj.stamp: \
	stamps/bootstrap-directory-ecj.stamp
	rm -f bootstrap/jdk1.6.0
	ln -sf ecj bootstrap/jdk1.6.0
	mkdir -p stamps
	touch stamps/bootstrap-directory-symlink-ecj.stamp

clean-bootstrap-directory-symlink-ecj:
	rm -f stamps/bootstrap-directory-symlink-ecj.stamp
	rm -f bootstrap/jdk1.6.0


# OpenJDK Targets
# ===============

if WITH_ICEDTEA
  MEMORY_LIMIT = -J-Xmx1024m
else
if WITH_OPENJDK
  MEMORY_LIMIT = -J-Xmx512m
else
  MEMORY_LIMIT =
endif
endif

if WITH_CACAO
  ICEDTEA_BUILD_TARGET=j2se_only
  ICEDTEA_DEBUG_BUILD_TARGET=j2se_fastdebug_only
else
  ICEDTEA_BUILD_TARGET=
  ICEDTEA_DEBUG_BUILD_TARGET=debug_build
endif

NETX_RESOURCE_DIR=openjdk/jdk/src/share/classes/net/sourceforge/jnlp/resources

# If you change anything here in the icedtea target, please make sure
# you change it in the icedtea-debug target as well.
stamps/icedtea.stamp: stamps/bootstrap-directory-symlink.stamp \
	stamps/hotspot-tools.stamp stamps/plugs.stamp \
	stamps/ports.stamp stamps/patch.stamp stamps/overlay.stamp \
	$(GCJWEBPLUGIN_TARGET) $(ICEDTEAPLUGIN_TARGET) \
	extra-lib/about.jar stamps/cacao.stamp stamps/visualvm.stamp \
	stamps/pulse-java.stamp
	$(MAKE) \
	  $(ICEDTEA_ENV) \
	  -C openjdk/ \
	  $(ICEDTEA_BUILD_TARGET)
if ENABLE_GCJWEBPLUGIN
	cp -pPRf gcjwebplugin.so \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	cp -pPRf gcjwebplugin.so \
	  $(BUILD_OUTPUT_DIR)/j2re-image/lib/$(INSTALL_ARCH_DIR)
else
if ENABLE_PLUGIN
	cp -pPRf IcedTeaPlugin.so \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	cp -pPRf IcedTeaPlugin.so \
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
if WITH_VISUALVM
	mkdir -p $(BUILD_OUTPUT_DIR)/j2sdk-image/lib/visualvm/etc ; \
	mkdir -p $(BUILD_OUTPUT_DIR)/j2sdk-image/lib/visualvm/visualvm ; \
	sed "s/APPNAME=\`basename.*\`/APPNAME=visualvm/" \
	  visualvm/visualvm/launcher/visualvm >> \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/bin/jvisualvm ; \
	chmod a+x $(BUILD_OUTPUT_DIR)/j2sdk-image/bin/jvisualvm ; \
	cp visualvm/visualvm/launcher/visualvm.conf \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/lib/visualvm/etc ; \
	echo visualvm >> \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/lib/visualvm/etc/visualvm.clusters ; \
	echo profiler3 >> \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/lib/visualvm/etc/visualvm.clusters ; \
	cp -r visualvm/visualvm/build/cluster/* \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/lib/visualvm/visualvm ; \
	cp -r netbeans/nbbuild/netbeans/platform8 \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/lib/visualvm ; \
	cp -r netbeans/nbbuild/netbeans/profiler3 \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/lib/visualvm
endif
	cp $(NETX_RESOURCE_DIR)/about.jnlp \
	  extra-lib/about.jar \
	  $(BUILD_OUTPUT_DIR)/j2re-image/lib
	cp $(NETX_RESOURCE_DIR)/about.jnlp \
	  extra-lib/about.jar \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib
	@echo "IcedTea is served:" $(BUILD_OUTPUT_DIR)
	mkdir -p stamps
	touch stamps/icedtea.stamp

stamps/icedtea-debug.stamp: stamps/bootstrap-directory-symlink.stamp \
	stamps/hotspot-tools.stamp stamps/plugs.stamp \
	stamps/ports.stamp stamps/patch.stamp stamps/overlay.stamp \
	$(GCJWEBPLUGIN_TARGET) $(ICEDTEAPLUGIN_TARGET) \
	extra-lib/about.jar stamps/cacao.stamp \
	stamps/pulse-java.stamp
	$(MAKE) \
	  $(ICEDTEA_ENV) \
	  -C openjdk/ \
	  $(ICEDTEA_DEBUG_BUILD_TARGET)
if ENABLE_GCJWEBPLUGIN
	cp -pPRf gcjwebplugin.so \
	  $(BUILD_OUTPUT_DIR)-debug/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	cp -pPRf gcjwebplugin.so \
	  $(BUILD_OUTPUT_DIR)-debug/j2re-image/lib/$(INSTALL_ARCH_DIR)
else
if ENABLE_PLUGIN
	cp -pPRf IcedTeaPlugin.so \
	  $(BUILD_OUTPUT_DIR)-debug/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	cp -pPRf IcedTeaPlugin.so \
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
if WITH_VISUALVM
	mkdir -p $(BUILD_OUTPUT_DIR)/j2sdk-image/lib/visualvm/etc ; \
	mkdir -p $(BUILD_OUTPUT_DIR)/j2sdk-image/lib/visualvm/visualvm ; \
	sed "s/APPNAME=\`basename.*\`/APPNAME=visualvm/" \
	  visualvm/visualvm/launcher/visualvm >> \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/bin/jvisualvm ; \
	chmod a+x $(BUILD_OUTPUT_DIR)/j2sdk-image/bin/jvisualvm ; \
	cp visualvm/visualvm/launcher/visualvm.conf \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/lib/visualvm/etc ; \
	echo visualvm >> \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/lib/visualvm/etc/visualvm.clusters ; \
	echo profiler3 >> \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/lib/visualvm/etc/visualvm.clusters ; \
	cp -r visualvm/visualvm/build/cluster/* \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/lib/visualvm/visualvm ; \
	cp -r netbeans/nbbuild/netbeans/platform8 \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/lib/visualvm ; \
	cp -r netbeans/nbbuild/netbeans/profiler3 \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/lib/visualvm
endif
	cp $(NETX_RESOURCE_DIR)/default.jnlp \
	  extra-lib/about.jar \
	  $(BUILD_OUTPUT_DIR)-debug/j2re-image/lib
	cp $(NETX_RESOURCE_DIR)/default.jnlp \
	  extra-lib/about.jar \
	  $(BUILD_OUTPUT_DIR)-debug/j2sdk-image/jre/lib
	@echo "IcedTea (debug build) is served:" \
	  $(BUILD_OUTPUT_DIR)-debug
	mkdir -p stamps
	touch stamps/icedtea-debug.stamp

clean-icedtea: stamps/download.stamp
	$(MAKE) \
	  $(ICEDTEA_ENV) \
	  -C openjdk/make clobber
	rm -f stamps/icedtea.stamp stamps/icedtea-debug.stamp

icedtea-against-icedtea: \
	stamps/bootstrap-directory-symlink.stamp \
	stamps/icedtea.stamp

icedtea-debug-against-icedtea: \
	stamps/bootstrap-directory-symlink.stamp \
	stamps/icedtea-debug.stamp

# OpenJDK ecj Targets
# ===================

stamps/native-ecj.stamp: 
	mkdir -p stamps ; \
	if test "x${GCJ}" != "xno"; then \
	  ${GCJ} ${CFLAGS} -Wl,-Bsymbolic -o native-ecj \
	    --main=org.eclipse.jdt.internal.compiler.batch.Main ${ECJ_JAR} ; \
	fi ; \
	touch stamps/native-ecj.stamp

stamps/icedtea-ecj.stamp: stamps/bootstrap-directory-symlink-ecj.stamp \
	stamps/hotspot-tools.stamp stamps/plugs.stamp \
	stamps/ports-ecj.stamp stamps/patch-ecj.stamp stamps/cacao.stamp
	$(MAKE) \
	  $(ICEDTEA_ENV_ECJ) \
	  -C openjdk-ecj \
	  $(ICEDTEA_BUILD_TARGET)
	@echo "ecj-poured IcedTea is served:" \
	  $(ECJ_BUILD_OUTPUT_DIR)
	mkdir -p stamps
	touch stamps/icedtea-ecj.stamp

SERVER_DIR = hotspot/import/jre/lib/$(BUILD_ARCH_DIR)/server
hotspot:
	rm -f stamps/bootstrap-directory-symlink-ecj.stamp
	rm -f stamps/icedtea-ecj.stamp
	rm -f stamps/icedtea-against-ecj.stamp
	$(MAKE) hotspot-helper
	cp $(ECJ_BUILD_OUTPUT_DIR)/$(SERVER_DIR)/libjvm.so \
	   $(ECJ_BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(BUILD_ARCH_DIR)/server
	@echo "Hotspot is served:" $(ECJ_BUILD_OUTPUT_DIR)/j2sdk-image

hotspot-helper: stamps/bootstrap-directory-symlink-ecj.stamp \
	stamps/hotspot-tools.stamp stamps/plugs.stamp \
	stamps/ports-ecj.stamp stamps/patch-ecj.stamp
	$(MAKE) \
	  $(ICEDTEA_ENV_ECJ) \
	  -C openjdk-ecj hotspot

clean-icedtea-ecj: stamps/clone-ecj.stamp
	rm -f stamps/icedtea-ecj.stamp
	$(MAKE) \
	  $(ICEDTEA_ENV_ECJ) \
	  -C openjdk-ecj clobber

stamps/icedtea-against-ecj.stamp: stamps/icedtea-ecj.stamp
	mkdir -p stamps
	touch stamps/icedtea-against-ecj.stamp

# Binary Plugs and Tools Targets
# ==============================

stamps/plugs.stamp: bootstrap/jdk1.7.0/jre/lib/rt-closed.jar
	mkdir -p stamps
	touch stamps/plugs.stamp

stamps/hotspot-tools.stamp: bootstrap/jdk1.7.0/jre/lib/tools.jar
	mkdir -p stamps
	touch stamps/hotspot-tools.stamp

clean-plugs: clean-plugs clean-shared-objects \
	clean-copy clean-rt
	rm -f stamps/plugs.stamp
	rm -f bootstrap/jdk1.7.0/jre/lib/rt-closed.jar

clean-tools-jar: clean-hotspot-tools
	rm -f stamps/hotspot-tools.stamp
	rm -f bootstrap/jdk1.7.0/jre/lib/tools.jar

# Sources copied from OpenJDK.
ICEDTEA_COPY_DIRS = \
	com/sun/jdi \
	com/sun/jdi/connect \
	com/sun/jdi/connect/spi \
	com/sun/jdi/event \
	com/sun/jdi/request \
	com/sun/tools/jdi \
	com/sun/net/httpserver \
	java/io \
	java/util \
	java/rmi \
	sun/awt/ \
	javax/net/ssl

stamps/copy-source-files.stamp: stamps/patch.stamp
	for copy_dir in $(ICEDTEA_COPY_DIRS) ; \
	do \
	  mkdir -p rt/$$copy_dir ; \
	  cp -pPRf openjdk/jdk/src/share/classes/$$copy_dir/*.java \
	    rt/$$copy_dir ; \
	done
	mkdir -p stamps
	touch stamps/copy-source-files.stamp

clean-copy:
	rm -f stamps/copy-source-files.stamp
	for copy_dir in $(ICEDTEA_COPY_DIRS) ; \
	do \
	  rm -rf rt/$$copy_dir ; \
	done

SHARE = openjdk/jdk/src/share/classes
SOLARIS = openjdk/jdk/src/solaris/classes
LANGTOOLS = openjdk/langtools/src/share/classes
JAXP = openjdk/jaxp/src/share/classes
CORBA = openjdk/corba/src/share/classes
JAXWS = openjdk/jaxws/src/share/classes

OPENJDK_SOURCEPATH_DIRS = \
        $(SHARE):$(SOLARIS):$(LANGTOOLS):$(JAXP):$(CORBA):$(JAXWS)

# tools.jar class files.
stamps/hotspot-tools-copy-source-files.stamp: stamps/patch.stamp \
	stamps/overlay.stamp
	for copy_dir in \
	  `cat $(abs_top_srcdir)/tools-copy/tools-jdk-copy-files.txt` ; \
	do \
	  mkdir -p hotspot-tools/$$copy_dir ; \
	  cp -pPRf openjdk/jdk/src/share/classes/$$copy_dir/* \
	    hotspot-tools/$$copy_dir ; \
	done
	for copy_dir in \
	  `cat $(abs_top_srcdir)/tools-copy/tools-langtools-copy-files.txt` ; \
	do \
	  mkdir -p hotspot-tools/$$copy_dir ; \
	  cp -pPRf openjdk/langtools/src/share/classes/$$copy_dir/* \
	    hotspot-tools/$$copy_dir ; \
	done
	for copy_dir in \
	  `cat $(abs_top_srcdir)/tools-copy/tools-corba-copy-files.txt` ; \
	do \
	  mkdir -p hotspot-tools/$$copy_dir ; \
	  cp -pPRf openjdk/corba/src/share/classes/$$copy_dir/* \
	    hotspot-tools/$$copy_dir ; \
	done
	for copy_dir in \
	  `cat $(abs_top_srcdir)/tools-copy/tools-jaxws-copy-files.txt` ; \
	do \
	  mkdir -p hotspot-tools/$$copy_dir ; \
	  cp -pPRf openjdk/jaxws/src/share/classes/$$copy_dir/* \
	    hotspot-tools/$$copy_dir ; \
	done
	mkdir -p stamps
	touch stamps/hotspot-tools-copy-source-files.stamp

hotspot-tools-source-files.txt: stamps/generated.stamp \
	stamps/hotspot-tools-copy-source-files.stamp
	find hotspot-tools -name '*.java' | sort > $@
	mkdir -p lib/hotspot-tools

ABS_SOURCE_DIRS = $(abs_top_builddir)/generated
stamps/hotspot-tools-class-files.stamp: hotspot-tools-source-files.txt
	if ! test -d $(ICEDTEA_BOOT_DIR) ; \
	then \
	  $(JAVAC) $(MEMORY_LIMIT) -g -d lib/hotspot-tools \
            -source 1.5 \
            -sourcepath \
	      'hotspot-tools:$(OPENJDK_SOURCEPATH_DIRS):$(ABS_SOURCE_DIRS)' \
            -bootclasspath '' @$< ; \
	else \
	  $(ICEDTEA_BOOT_DIR)/bin/javac $(MEMORY_LIMIT) -g \
	    -d lib/hotspot-tools \
	    -source 1.5 \
	    -sourcepath \
	      'hotspot-tools:$(OPENJDK_SOURCEPATH_DIRS):$(ABS_SOURCE_DIRS)' \
	    -bootclasspath '' @$< ; \
	fi
	mkdir -p stamps
	touch stamps/hotspot-tools-class-files.stamp

clean-hotspot-tools:
	rm -rf lib/hotspot-tools
	rm -f stamps/hotspot-tools-class-files.stamp
	rm -f hotspot-tools-source-files.txt
	rm -f stamps/hotspot-tools-copy-source-files.stamp

# tools.jar
bootstrap/jdk1.7.0/jre/lib/tools.jar: stamps/hotspot-tools-class-files.stamp
	mkdir -p bootstrap/jdk1.7.0/jre/lib/
	if ! test -d $(ICEDTEA_BOOT_DIR) ; \
        then \
	  $(JAR) cf $@ -C lib/hotspot-tools com -C lib/hotspot-tools sun \
	    -C lib/hotspot-tools org -C lib/hotspot-tools java \
	    -C lib/hotspot-tools javax ; \
	else \
	  $(ICEDTEA_BOOT_DIR)/bin/jar cf $@ -C lib/hotspot-tools com \
	    -C lib/hotspot-tools sun \
            -C lib/hotspot-tools org -C lib/hotspot-tools java \
            -C lib/hotspot-tools javax ; \
	fi
	if test -d bootstrap/ecj ; \
	then \
	  mkdir -p bootstrap/ecj/lib/; \
	  ln -sf $(abs_top_builddir)/bootstrap/jdk1.7.0/jre/lib/tools.jar \
	    $(abs_top_builddir)/bootstrap/ecj/lib/tools.jar; \
	fi

# rt-closed.jar class files.
rt-source-files.txt: stamps/extract.stamp stamps/copy-source-files.stamp
	find $(abs_top_builddir)/rt -name '*.java' \
	  | sort -u > $@

stamps/rt-class-files.stamp: rt-source-files.txt
	mkdir -p lib/rt
	if ! test -d $(ICEDTEA_BOOT_DIR) ; \
        then \
	  $(JAVAC) $(MEMORY_LIMIT) -g -d lib/rt \
	    -source 1.5 \
	    -sourcepath \
	      '$(OPENJDK_SOURCEPATH_DIRS):$(abs_top_builddir)/generated' \
	    -bootclasspath '' @$< ; \
        else \
	  $(ICEDTEA_BOOT_DIR)/bin/javac $(MEMORY_LIMIT) -g -d lib/rt \
            -source 1.5 \
            -sourcepath \
	      '$(OPENJDK_SOURCEPATH_DIRS):$(abs_top_builddir)/generated' \
            -bootclasspath '' @$< ; \
        fi
	mkdir -p stamps
	touch stamps/rt-class-files.stamp

clean-rt:
	rm -rf lib/rt
	rm -f stamps/rt-class-files.stamp
	rm -f rt-source-files.txt

# rt-closed.jar.
bootstrap/jdk1.7.0/jre/lib/rt-closed.jar: stamps/rt-class-files.stamp
	mkdir -p bootstrap/jdk1.7.0/jre/lib
	if ! test -d $(ICEDTEA_BOOT_DIR) ; \
	then \
	  $(JAR) cf $@ -C lib/rt com -C lib/rt java \
	    -C lib/rt javax $(LIVECONNECT_DIR) -C lib/rt sun ; \
	else \
	  $(ICEDTEA_BOOT_DIR)/bin/jar cf $@ -C lib/rt com -C lib/rt java \
	    -C lib/rt javax $(LIVECONNECT_DIR) -C lib/rt sun ; \
	fi
	if test -d bootstrap/ecj/jre/lib ; \
	then \
	  cp bootstrap/jdk1.7.0/jre/lib/rt-closed.jar \
	    bootstrap/ecj/jre/lib/rt.jar; \
	fi

# extras -- used to create about.jar for javaws.
extra-source-files.txt:
	find $(abs_top_srcdir)/extra -name '*.java' | sort > $@

stamps/extra-class-files.stamp: extra-source-files.txt \
	bootstrap/jdk1.7.0/jre/lib/rt-closed.jar
	mkdir -p extra-lib
	if ! test -d $(ICEDTEA_BOOT_DIR) ; \
	then \
	  $(JAVAC) $(MEMORY_LIMIT) -g -d extra-lib \
	    -source 1.5 \
	    -sourcepath extra -cp $(ICEDTEA_BOOT_DIR)/jre/lib/rt.jar \
	    -bootclasspath '' @extra-source-files.txt ; \
	else \
	  $(ICEDTEA_BOOT_DIR)/bin/javac $(MEMORY_LIMIT) -g -d extra-lib \
            -source 1.5 \
            -sourcepath extra -cp $(ICEDTEA_BOOT_DIR)/jre/lib/rt.jar \
            -bootclasspath '' @extra-source-files.txt ; \
	fi
	cp -r $(abs_top_srcdir)/extra/net/sourceforge/jnlp/about/resources \
	  extra-lib/net/sourceforge/jnlp/about
	mkdir -p stamps
	touch $@

clean-extra:
	rm -rf extra-lib
	rm -f stamps/extra-class-files.stamp
	rm -f extra-source-files.txt

extra-lib/about.jar: stamps/extra-class-files.stamp
	if ! test -d $(ICEDTEA_BOOT_DIR) ; \
	then \
	  $(JAR) cf $@ -C extra-lib net ; \
	else \
	  $(ICEDTEA_BOOT_DIR)/bin/jar cf $@ -C extra-lib net ; \
	fi

stamps/cacao.stamp: stamps/extract.stamp stamps/rt-class-files.stamp
if WITH_CACAO
if !USE_SYSTEM_CACAO
	cd cacao/cacao && \
	./configure \
	  --prefix=$(abs_top_builddir)/cacao/install \
	  --with-java-runtime-library=openjdk \
	  --with-java-runtime-library-prefix=$(abs_top_builddir)/openjdk \
	  --with-java-runtime-library-classes=$(abs_top_builddir)/lib/rt \
	  --enable-jre-layout $(CACAO_CONFIGURE_ARGS); \
	make install
endif
endif
	mkdir -p stamps
	touch $@

clean-cacao:
	rm -rf cacao
	rm -f stamps/cacao.stamp

if ENABLE_GCJWEBPLUGIN
# gcjwebplugin.so.
gcjwebplugin.so: gcjwebplugin.cc
	if [ -e $(abs_top_srcdir)/.hg ] && which $(HG) >/dev/null; then \
	  revision="-r`(cd $(abs_top_srcdir); $(HG) tip --template '{rev}')`" ; \
	fi ; \
	if [ -n "$(PKGVERSION)" ]; then plugin_version=" ($(PKGVERSION))"; fi; \
	plugin_version=" $(PACKAGE_VERSION)$$revision$$plugin_version"; \
	$(CXX) $(CXXFLAGS) $(MOZILLA_CFLAGS) \
	  $(MOZILLA_LIBS) $(GLIB_CFLAGS) $(GLIB_LIBS) \
	  $(GTK_CFLAGS) $(GTK_LIBS) \
	  -DPACKAGE_VERSION="\"$(PACKAGE_VERSION)\"" \
	  -DPLUGIN_VERSION="\"$$plugin_version\"" \
	  -fPIC -shared -o $@ $<

clean-gcjwebplugin:
	rm -f gcjwebplugin.so
else
if ENABLE_PLUGIN
# IcedTeaPlugin.so.
# Separate compile and link invocations to ensure intermediate object
# is listed before -l options.  See:
# http://developer.mozilla.org/en/docs/XPCOM_Glue
IcedTeaPlugin.o: IcedTeaPlugin.cc
	if [ -e $(abs_top_srcdir)/.hg ] && which $(HG) >/dev/null; then \
	  revision="-r`(cd $(abs_top_srcdir); $(HG) tip --template '{rev}')`" ; \
	fi ; \
	if [ -n "$(PKGVERSION)" ]; then plugin_version=" ($(PKGVERSION))"; fi; \
	plugin_version=" $(PACKAGE_VERSION)$$revision$$plugin_version"; \
	$(CXX) $(CXXFLAGS) \
	  -DPACKAGE_VERSION="\"$(PACKAGE_VERSION)\"" \
	  -DPLUGIN_VERSION="\"$$plugin_version\"" \
	  $(GTK_CFLAGS) \
	  $(XULRUNNER_CFLAGS) \
	  -fPIC -c -o $@ $<
IcedTeaPlugin.so: IcedTeaPlugin.o
	$(CXX) $(CXXFLAGS) \
	  $< \
	  $(GTK_LIBS) \
	  $(XULRUNNER_LIBS) \
	  -shared -o $@

clean-IcedTeaPlugin:
	rm -f IcedTeaPlugin.o
	rm -f IcedTeaPlugin.so
endif
endif


# PulseAudio based mixer
# (pulse-java)

stamps/pulse-java.stamp: stamps/pulse-java-jar.stamp stamps/pulse-java-headers.stamp
if ENABLE_PULSE_JAVA
	$(CC) $(CFLAGS) -fPIC -c -I$(ICEDTEA_BOOT_DIR)/include/linux -I$(ICEDTEA_BOOT_DIR)/include $(PULSE_JAVA_NATIVE_SRCDIR)/jni-common.c
	$(CC) $(LIBPULSE_CFLAGS) $(CFLAGS) -fPIC -c -I$(ICEDTEA_BOOT_DIR)/include/linux -I$(ICEDTEA_BOOT_DIR)/include $(PULSE_JAVA_NATIVE_SRCDIR)/org_classpath_icedtea_pulseaudio_EventLoop.c
	$(CC) $(LIBPULSE_CFLAGS) $(CFLAGS) -fPIC -c -I$(ICEDTEA_BOOT_DIR)/include/linux -I$(ICEDTEA_BOOT_DIR)/include $(PULSE_JAVA_NATIVE_SRCDIR)/org_classpath_icedtea_pulseaudio_Operation.c
	$(CC) $(LIBPULSE_CFLAGS) $(CFLAGS) -fPIC -c -I$(ICEDTEA_BOOT_DIR)/include/linux -I$(ICEDTEA_BOOT_DIR)/include $(PULSE_JAVA_NATIVE_SRCDIR)/org_classpath_icedtea_pulseaudio_Stream.c
	$(CC) $(LIBPULSE_CFLAGS) $(CFLAGS) -fPIC -c -I$(ICEDTEA_BOOT_DIR)/include/linux -I$(ICEDTEA_BOOT_DIR)/include $(PULSE_JAVA_NATIVE_SRCDIR)/org_classpath_icedtea_pulseaudio_PulseAudioSourcePort.c 
	$(CC) $(LIBPULSE_CFLAGS) $(CFLAGS) -fPIC -c -I$(ICEDTEA_BOOT_DIR)/include/linux -I$(ICEDTEA_BOOT_DIR)/include $(PULSE_JAVA_NATIVE_SRCDIR)/org_classpath_icedtea_pulseaudio_PulseAudioTargetPort.c
	$(CC) $(LDFLAGS) -shared $(LIBPULSE_LIBS) -o libpulse-java.so org_*pulseaudio*.o jni-common.o
	mv org_classpath_icedtea_pulseaudio_*.o $(PULSE_JAVA_CLASS_DIR)
	mv jni-common.o $(PULSE_JAVA_CLASS_DIR)
endif
	mkdir -p stamps
	touch stamps/pulse-java.stamp

stamps/pulse-java-jar.stamp: stamps/pulse-java-class.stamp
if ENABLE_PULSE_JAVA
	mkdir -p $(PULSE_JAVA_CLASS_DIR);
	if ! test -d $(ICEDTEA_BOOT_DIR) ; \
	then \
	  $(JAR) cf pulse-java.jar -C $(PULSE_JAVA_CLASS_DIR) .; \
	else \
	  $(ICEDTEA_BOOT_DIR)/bin/jar cf pulse-java.jar -C $(PULSE_JAVA_CLASS_DIR) .; \
	fi
endif
	mkdir -p stamps
	touch stamps/pulse-java-jar.stamp

stamps/pulse-java-class.stamp: 
if ENABLE_PULSE_JAVA
	mkdir -p $(PULSE_JAVA_CLASS_DIR)
	if ! test -d $(ICEDTEA_BOOT_DIR) ; \
	then \
		(cd $(PULSE_JAVA_JAVA_SRCDIR); \
		 $(JAVAC) -g -d $(PULSE_JAVA_CLASS_DIR) \
		-bootclasspath  \
	        '$(OPENJDK_SOURCEPATH_DIRS):$(abs_top_builddir)/generated' \
		org/classpath/icedtea/pulseaudio/*.java\
		) \
        else \
		(cd $(PULSE_JAVA_JAVA_SRCDIR); \
		$(ICEDTEA_BOOT_DIR)/bin/javac -g -d $(PULSE_JAVA_CLASS_DIR)\
		-bootclasspath \
	        '$(ICEDTEA_BOOT_DIR)/jre/lib/rt.jar' \
		org/classpath/icedtea/pulseaudio/*.java\
		) \
        fi
	cp  -r $(PULSE_JAVA_JAVA_SRCDIR)/META-INF $(PULSE_JAVA_CLASS_DIR)
endif
	mkdir -p stamps
	touch stamps/pulse-java-class.stamp


stamps/pulse-java-headers.stamp: stamps/pulse-java-class.stamp
if ENABLE_PULSE_JAVA
	if ! test -d $(ICEDTEA_BOOT_DIR) ; \
	then \
	  $(JAVAH) -d $(PULSE_JAVA_NATIVE_SRCDIR) -classpath $(PULSE_JAVA_CLASS_DIR) org.classpath.icedtea.pulseaudio.EventLoop ; \
	  $(JAVAH) -d $(PULSE_JAVA_NATIVE_SRCDIR) -classpath $(PULSE_JAVA_CLASS_DIR) org.classpath.icedtea.pulseaudio.Stream ; \
	  $(JAVAH) -d $(PULSE_JAVA_NATIVE_SRCDIR) -classpath $(PULSE_JAVA_CLASS_DIR) org.classpath.icedtea.pulseaudio.Operation; \
	  $(JAVAH) -d $(PULSE_JAVA_NATIVE_SRCDIR) -classpath $(PULSE_JAVA_CLASS_DIR) org.classpath.icedtea.pulseaudio.PulseAudioSourcePort ; \
	  $(JAVAH) -d $(PULSE_JAVA_NATIVE_SRCDIR) -classpath $(PULSE_JAVA_CLASS_DIR) org.classpath.icedtea.pulseaudio.PulseAudioTargetPort ; \
	else \
	  $(ICEDTEA_BOOT_DIR)/bin/javah -d $(PULSE_JAVA_NATIVE_SRCDIR) -classpath $(PULSE_JAVA_CLASS_DIR) org.classpath.icedtea.pulseaudio.EventLoop ; \
	  $(ICEDTEA_BOOT_DIR)/bin/javah -d $(PULSE_JAVA_NATIVE_SRCDIR) -classpath $(PULSE_JAVA_CLASS_DIR) org.classpath.icedtea.pulseaudio.Stream ; \
	  $(ICEDTEA_BOOT_DIR)/bin/javah -d $(PULSE_JAVA_NATIVE_SRCDIR) -classpath $(PULSE_JAVA_CLASS_DIR) org.classpath.icedtea.pulseaudio.Operation; \
	  $(ICEDTEA_BOOT_DIR)/bin/javah -d $(PULSE_JAVA_NATIVE_SRCDIR) -classpath $(PULSE_JAVA_CLASS_DIR) org.classpath.icedtea.pulseaudio.PulseAudioSourcePort ; \
	  $(ICEDTEA_BOOT_DIR)/bin/javah -d $(PULSE_JAVA_NATIVE_SRCDIR) -classpath $(PULSE_JAVA_CLASS_DIR) org.classpath.icedtea.pulseaudio.PulseAudioTargetPort ; \
	fi
endif
	mkdir -p stamps
	touch stamps/pulse-java-headers.stamp


clean-pulse-java:
if ENABLE_PULSE_JAVA
	[ -z "$(PULSE_JAVA_CLASS_DIR)" ] || find $(PULSE_JAVA_CLASS_DIR) -name '*.class' | xargs -r rm -f
	[ -z "$(PULSE_JAVA_NATIVE_SRCDIR)" ] || rm -f $(PULSE_JAVA_NATIVE_SRCDIR)/org_*.h $(PULSE_JAVA_NATIVE_SRCDIR)/*.o
	rm -f stamps/pulse-java*.stamp
	rm -f pulse-java.jar
	rm -f libpulse-java.so
endif

# end of pulse-java

# jtreg

stamps/jtreg.stamp: stamps/icedtea.stamp
	rm -rf test/jtreg/classes
	mkdir -p test/jtreg/classes
	$(ICEDTEA_BOOT_DIR)/bin/javac -g -d test/jtreg/classes -source 1.5 \
	   -encoding iso-8859-1 `find $(abs_top_srcdir)/test/jtreg/com -name '*.java'`
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
		`pwd`/openjdk/hotspot/test \
	    | tee test/$@.log

check-langtools: stamps/jtreg.stamp
	mkdir -p test/langtools/JTwork test/langtools/JTreport
	$(ICEDTEA_BOOT_DIR)/bin/java -jar test/jtreg.jar -v1 -a -ignore:quiet \
		-w:test/langtools/JTwork -r:test/langtools/JTreport \
		-jdk:`pwd`/$(BUILD_OUTPUT_DIR)/j2sdk-image \
		`pwd`/openjdk/langtools/test \
	    | tee test/$@.log

check-jdk: stamps/jtreg.stamp
	mkdir -p test/jdk/JTwork test/jdk/JTreport
	$(ICEDTEA_BOOT_DIR)/bin/java -jar test/jtreg.jar -v1 -a -ignore:quiet \
		-w:test/jdk/JTwork -r:test/jdk/JTreport \
		-jdk:`pwd`/$(BUILD_OUTPUT_DIR)/j2sdk-image \
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
	  egrep -v '^(Passed:|Directory)' test/check-$$i.log; \
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

check-local: jtregcheck

# fsg-tarball

# Creates archive of openjdk that is compliant with Free Software guidelines.
dist-openjdk-fsg: stamps/patch-fsg.stamp
	$(ZIP) -r openjdk-fsg-$(OPENJDK_VERSION) openjdk/

# VisualVM

stamps/visualvm.stamp: $(BOOTSTRAP_DIRECTORY_STAMP) stamps/download.stamp \
	stamps/extract.stamp stamps/nbplatform.stamp
if WITH_VISUALVM
	cd visualvm/visualvm ; \
	ln -s $(abs_top_srcdir)/netbeans/nbbuild/netbeans . ; \
	rm netbeans/profiler3 && mv netbeans/extra netbeans/profiler3 ; \
	JAVA_HOME=$(ICEDTEA_HOME) $(ANT) build
endif
	mkdir -p stamps
	touch $@

clean-visualvm:
	rm -rf visualvm
	rm -f stamps/visualvm.stamp

# Netbeans platform and profiler

stamps/nbplatform.stamp: $(BOOTSTRAP_DIRECTORY_STAMP) stamps/extract.stamp \
	stamps/download.stamp
if WITH_VISUALVM
	cd netbeans ; \
	ln -s $(abs_top_srcdir)/netbeans/profiler.cvs/libs/jfluid/ lib.profiler ; \
	ln -s $(abs_top_srcdir)/netbeans/profiler.cvs/libs/common/ lib.profiler.common ; \
	ln -s $(abs_top_srcdir)/netbeans/profiler.cvs/libs/jfluid-ui/ lib.profiler.ui ; \
	ln -s $(abs_top_srcdir)/netbeans/profiler.cvs/modules/nbmodule/ profiler.nbmodule ; \
	mkdir -p ./nbbuild/netbeans/ ; \
	ln -s $(SYSTEM_NETBEANS_DIR)/platform8/ ./nbbuild/netbeans/platform8 ; \
	ln -s $(SYSTEM_NETBEANS_DIR)/apisupport1/ ./nbbuild/netbeans/apisupport1 ; \
	ln -s $(SYSTEM_NETBEANS_DIR)/harness/ ./nbbuild/netbeans/harness ; \
	ln -s $(SYSTEM_NETBEANS_DIR)/java2/ ./nbbuild/netbeans/java2 ; \
	ln -s $(SYSTEM_NETBEANS_DIR)/ide9/ ./nbbuild/netbeans/ide9 ; \
	cd profiler.cvs/libs/jfluid/native/build ; \
	if ! (uname -a | grep x86_64) ; then \
	  buildscript=./buildnative-linux-15.sh ; \
	else \
	  buildscript=./buildnative-linux64-15.sh ; \
	fi ; \
	JAVA_HOME_15=$(ICEDTEA_HOME) JAVA_HOME_16=$(ICEDTEA_HOME) \
	  $$buildscript ; \
	cd ../../../../.. ; \
	ln -s $(abs_top_srcdir)/netbeans/nbbuild/netbeans/extra nbbuild/netbeans/profiler3 ; \
	ln -s $(abs_top_srcdir)/netbeans/profiler.cvs/modules/main/ profiler ; \
	for dir in jfluid jfluid-ui common ; do \
	  JAVA_HOME=$(ICEDTEA_HOME) $(ANT) -Dpermit.jdk6.builds=true \
	    -f profiler.cvs/libs/$$dir/build.xml ; \
	done ; \
	JAVA_HOME=$(ICEDTEA_HOME) $(ANT) -Dpermit.jdk6.builds=true \
	  -f profiler.cvs/modules/main/build.xml ;
endif
	mkdir -p stamps
	touch $@

clean-nbplatform:
	rm -rf netbeans
	rm -f stamps/nbplatform.stamp

# Target Aliases
# ===============

bootstrap-directory-ecj: stamps/bootstrap-directory-ecj.stamp

bootstrap-directory: stamps/bootstrap-directory.stamp

bootstrap-directory-symlink-ecj: stamps/bootstrap-directory-symlink-ecj.stamp

bootstrap-directory-symlink: stamps/bootstrap-directory-symlink.stamp

cacao: stamps/cacao.stamp

visualvm: stamps/visualvm.stamp

nbplatform: stamps/nbplatform.stamp

clone-ecj: stamps/clone-ecj.stamp

copy-source-files: stamps/copy-source-files.stamp

download: stamps/download.stamp

extract: stamps/extract.stamp

extract-cacao: stamps/extract-cacao.stamp

generated: stamps/generated.stamp

icedtea: stamps/icedtea.stamp

icedtea-against-ecj: stamps/icedtea-against-ecj.stamp

icedtea-debug: stamps/icedtea-debug.stamp

icedtea-ecj: stamps/icedtea-ecj.stamp

native-ecj: stamps/native-ecj.stamp

patch-ecj: stamps/patch-ecj.stamp

patch: stamps/patch.stamp

patch-fsg: stamps/patch-fsg.stamp

overlay: stamps/overlay.stamp

plugs: stamps/plugs.stamp

rt-class-files: stamps/rt-class-files.stamp

hotspot-tools-class-files: stamps/hotspot-tools-class-files.stamp

hotspot-tools-copy-source-files: stamps/hotspot-tools-copy-source-files.stamp

hotspot-tools: stamps/hotspot-tools.stamp

hotspot-ports: stamps/ports.stamp

tools-class-files: stamps/tools-class-files.stamp

hotspot-ports: stamps/ports.stamp

hotspot-ports-ecj: stamps/ports-ecj.stamp

clean: distclean-local

jtreg: stamps/jtreg.stamp
