OPENJDK_VERSION = b21
OPENJDK_DATE = 27_sep_2007
OPENJDK_MD5SUM = 64ac356cd24af5e52cf9fd1217e234da

# Top-Level Targets
# =================

all-local: icedtea-against-icedtea

distclean-local: clean-copy
	rm -rf stamps
	rm -f rt-source-files.txt tools-source-files.txt
	rm -rf bootstrap
	rm -rf lib
	rm -rf openjdk
	rm -rf tools
	rm -f gcjwebplugin.so

install:

.PHONY: \
	env snapshot clean-download clean-extract clean-patch \
	clean-bootstrap-directory clean-bootstrap-directory-symlink \
	icedtea icedtea-debug clean-icedtea icedtea-against-icedtea clean-plugs \
	clean-tools-jar clean-tools clean-shared-objects \
	clean-copy clean-rt clean-gcjwebplugin

EXTRA_DIST = rt generated $(ICEDTEA_PATCHES) \
	gcjwebplugin.cc patches/icedtea-speed.patch tools-copy-files.txt

# OpenJDK build environment.
LINUX_DIR = linux-$(BUILD_ARCH_DIR)
ICEDTEA_BUILD_DIR = \
	$(abs_top_srcdir)/openjdk/control/build/$(LINUX_DIR)/
ICEDTEA_RT = \
	$(abs_top_srcdir)/bootstrap/jdk1.7.0/jre/lib/rt-closed.jar
ICEDTEA_CLS_DIR = \
	$(abs_top_srcdir)/openjdk/control/build/$(LINUX_DIR)/classes
ICEDTEA_ENDORSED_DIR = \
	$(abs_top_srcdir)/bootstrap/jdk1.6.0/lib/endorsed
ICEDTEA_ENV = \
	"ANT_HOME=@SYSTEM_ANT_DIR@" \
	"BUILD_NUMBER=b21" \
	"JRE_RELEASE_VERSION=1.7.0-b21" \
	"HOTSPOT_RELEASE_VERSION=1.7.0-b21" \
	"LANG=C" \
	"PATH=/usr/bin:/bin:/usr/sbin:/sbin" \
	"ALT_BOOTDIR=$(abs_top_srcdir)/bootstrap/jdk1.6.0" \
	"ALT_BINARY_PLUGS_PATH=$(abs_top_srcdir)/bootstrap/jdk1.7.0" \
	"BUILD_ARCH_DIR=$(BUILD_ARCH_DIR)" \
	"ICEDTEA_RT=$(ICEDTEA_RT)" \
	"ICEDTEA_BUILD_DIR=$(ICEDTEA_BUILD_DIR)" \
	"ICEDTEA_CLS_DIR=$(ICEDTEA_CLS_DIR)" \
	"ICEDTEA_ENDORSED_DIR=$(ICEDTEA_ENDORSED_DIR)" \
	"ENDORSED=-Djava.endorsed.dirs=$(ICEDTEA_ENDORSED_DIR)" \
	"CLASSPATH=" \
	"LD_LIBRARY_PATH=" \
	"FREETYPE2_INC_DIR=$(FREETYPE2_INC_DIR)"

if WITH_CACAO
ICEDTEA_ENV += \
	"ALT_HOTSPOT_IMPORT_PATH=$(CACAO)" \
	"ALT_JDK_IMPORT_PATH=$(CACAO)"
endif

env:
	@echo 'unset JAVA_HOME'
	@echo 'export $(ICEDTEA_ENV)'

# Mercurial snapshot.
snapshot: dist
	mv $(DIST_ARCHIVES) $(distdir)-`hg tip --template '{node}'`.tar.gz

# OpenJDK Source Preparation Targets
# ==================================

# Download OpenJDK sources.
OPENJDK_URL = http://download.java.net/openjdk/jdk7/promoted/$(OPENJDK_VERSION)/
OPENJDK_SRC_ZIP_BASE = openjdk-7-ea-src-$(OPENJDK_VERSION)-$(OPENJDK_DATE).zip

if USE_ALT_OPENJDK_SRC_ZIP
  OPENJDK_SRC_ZIP = $(ALT_OPENJDK_SRC_ZIP)
else
  OPENJDK_SRC_ZIP = $(OPENJDK_SRC_ZIP_BASE)
endif

stamps/download.stamp:
	if ! echo "$(OPENJDK_MD5SUM)  $(OPENJDK_SRC_ZIP)" \
	  | $(MD5SUM) --check ; \
	then \
	  if [ $(OPENJDK_SRC_ZIP) ] ; \
	  then \
            mv $(OPENJDK_SRC_ZIP) $(OPENJDK_SRC_ZIP).old ; \
          fi ; \
	  $(WGET) $(OPENJDK_URL)$(OPENJDK_SRC_ZIP_BASE) -O $(OPENJDK_SRC_ZIP); \
	fi
	mkdir -p stamps
	touch stamps/download.stamp

clean-download:
	rm -f stamps/download.stamp
	rm -f $(OPENJDK_SRC_ZIP)

# Extract OpenJDK sources.
stamps/extract.stamp: stamps/download.stamp
	if ! test -d openjdk ; \
	then \
	  $(UNZIP) -q $(OPENJDK_SRC_ZIP) ; \
	  chmod ug+w `find openjdk/j2se/src/share/demo/jvmti \
	     -name sample.makefile.txt` ; \
	fi
	mkdir -p stamps
	touch stamps/extract.stamp

clean-extract:
	rm -f stamps/extract.stamp
	rm -rf openjdk

# Patch OpenJDK sources for plug replacements.
if FAST_BUILD
  FAST_BUILD_PATCH = patches/icedtea-speed.patch
else
  FAST_BUILD_PATCH =
endif

ICEDTEA_PATCHES = \
	patches/icedtea-copy-plugs.patch \
	patches/icedtea-version.patch \
	patches/icedtea-text-relocations.patch \
	patches/icedtea-graphics.patch \
	patches/icedtea-antialias.patch \
	patches/icedtea-paths.patch \
	patches/icedtea-debuginfo.patch \
	patches/icedtea-ssl.patch \
	patches/icedtea-license-headers.patch \
	patches/icedtea-libpng.patch \
	patches/icedtea-plugin.patch \
	patches/icedtea-rmi_amd64.patch \
	patches/icedtea-tools.patch \
	patches/icedtea-certbundle.patch \
	patches/icedtea-demos.patch \
	patches/icedtea-use-system-tzdata.patch \
	patches/icedtea-headers.patch \
	patches/icedtea-ant.patch \
	patches/icedtea-gcc-suffix.patch \
	patches/icedtea-bytebuffer-compact.patch \
	patches/icedtea-float-double-trailing-zeros.patch \
	$(FAST_BUILD_PATCH) \
	$(DISTRIBUTION_PATCHES)

if WITH_CACAO
ICEDTEA_PATCHES += \
	patches/icedtea-cacao-Compiler-gcc-gmk.patch \
	patches/icedtea-cacao-Defs-linux-gmk.patch \
	patches/icedtea-cacao-Platform-gmk.patch \
	patches/icedtea-cacao-Program-gmk.patch \
	patches/icedtea-cacao-docs-Makefile.patch \
	patches/icedtea-cacao-instrument-Makefile.patch \
	patches/icedtea-cacao-java-c.patch \
	patches/icedtea-cacao-jvm-cfg.patch
endif

stamps/patch.stamp: stamps/extract.stamp
	for p in $(ICEDTEA_PATCHES) ; \
	do \
	  echo Checking $$p ; \
	  if $(PATCH) -p0 --dry-run -s -t -f -F 0 < $$p ; \
	  then \
	    echo Applying $$p ; \
	    $(PATCH) -p0 < $$p ; \
	  fi ; \
	done
	mkdir -p stamps
	touch stamps/patch.stamp

clean-patch:
	rm -f stamps/patch.stamp
	for p in $(ICEDTEA_PATCHES) ; \
	do \
	  echo Checking $$p ; \
	  if $(PATCH) -p0 -R --dry-run -s -t -f < $$p ; \
	  then \
	    echo Reverting $$p ; \
	    $(PATCH) -p0 -R < $$p ; \
	  fi ; \
	done

# Bootstrap Directory Targets
# ===========================

MEMORY_LIMIT = -J-Xmx512m

# bootstrap/jdk1.6.0 to bootstrap/icedtea symlink.
stamps/bootstrap-directory-symlink.stamp:
	mkdir -p bootstrap
	rm -f bootstrap/jdk1.6.0
	ln -sf $(SYSTEM_ICEDTEA_DIR) bootstrap/jdk1.6.0
	mkdir -p stamps
	touch stamps/bootstrap-directory-symlink.stamp

clean-bootstrap-directory-symlink:
	rm -f stamps/bootstrap-directory-symlink.stamp
	rm -f bootstrap/jdk1.6.0

# If you change anything here in the icedtea target, please make sure
# you change it in the icedtea-debug target as well.
icedtea: stamps/bootstrap-directory-symlink.stamp stamps/tools.stamp stamps/plugs.stamp stamps/extract.stamp \
	stamps/patch.stamp gcjwebplugin.so
	$(MAKE) \
	  $(ICEDTEA_ENV) \
	  -C openjdk/control/make j2se_only
	cp -af gcjwebplugin.so \
	  openjdk/control/build/$(LINUX_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	cp -af gcjwebplugin.so \
	  openjdk/control/build/$(LINUX_DIR)/j2re-image/lib/$(INSTALL_ARCH_DIR)
	@echo "IcedTea is served:" openjdk/control/build/$(LINUX_DIR)

icedtea-debug: stamps/bootstrap-directory-symlink.stamp \
	stamps/tools.stamp stamps/plugs.stamp stamps/extract.stamp \
	stamps/patch.stamp gcjwebplugin.so
	$(MAKE) \
	  $(ICEDTEA_ENV) \
	  -C openjdk/control/make \
	  j2se_fastdebug_only
	cp -af gcjwebplugin.so \
	  openjdk/control/build/$(LINUX_DIR)-debug/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	cp -af gcjwebplugin.so \
	  openjdk/control/build/$(LINUX_DIR)-debug/j2re-image/lib/$(INSTALL_ARCH_DIR)
	@echo "IcedTea (debug build) is served:" \
	  openjdk/control/build/$(LINUX_DIR)-debug

clean-icedtea: stamps/extract.stamp
	$(MAKE) \
	  $(ICEDTEA_ENV) \
	  -C openjdk/control/make clobber

icedtea-against-icedtea: \
	stamps/bootstrap-directory-symlink.stamp \
	icedtea

# Binary Plugs and Tools Targets
# ==============================

stamps/plugs.stamp: $(SHARED_OBJECT_FILES) \
	bootstrap/jdk1.7.0/jre/lib/rt-closed.jar
	mkdir -p stamps
	touch stamps/plugs.stamp

stamps/tools.stamp: bootstrap/jdk1.7.0/jre/lib/tools.jar
	mkdir -p stamps
	touch stamps/tools.stamp

clean-plugs: clean-plugs clean-shared-objects clean-copy clean-rt
	rm -f stamps/plugs.stamp
	rm -f bootstrap/jdk1.7.0/jre/lib/rt-closed.jar

clean-tools-jar: clean-tools-jar clean-tools
	rm -f stamps/tools.stamp
	rm -rf tools/
	rm -f bootstrap/jdk1.7.0/jre/lib/tools.jar

# Shared objects.
SHARED_OBJECT_FILES = \
	bootstrap/jdk1.7.0/jre/lib/$(INSTALL_ARCH_DIR)/libdcpr.so \
	bootstrap/jdk1.7.0/jre/lib/$(INSTALL_ARCH_DIR)/libjsoundalsa.so \
	bootstrap/jdk1.7.0/jre/lib/$(INSTALL_ARCH_DIR)/libjsound.so \
	bootstrap/jdk1.7.0/jre/lib/$(INSTALL_ARCH_DIR)/libjsoundhs.so \
	bootstrap/jdk1.7.0/jre/lib/$(INSTALL_ARCH_DIR)/libt2k.so

$(SHARED_OBJECT_FILES):
	mkdir -p bootstrap/jdk1.7.0/jre/lib/$(INSTALL_ARCH_DIR)
	for shared_object in $(SHARED_OBJECT_FILES) ; \
	do \
	  echo | $(CC) $(CFLAGS) -fpic -shared \
	    -o $$shared_object \
	    -Wl,-soname,$$shared_object -xc - ; \
	done

clean-shared-objects:
	rm -f $(SHARED_OBJECT_FILES)

# Sources copied from OpenJDK.
ICEDTEA_COPY_DIRS = \
	com/sun/jdi \
	com/sun/jdi/connect \
	com/sun/jdi/connect/spi \
	com/sun/jdi/event \
	com/sun/jdi/request \
	com/sun/jmx/snmp/agent \
	com/sun/tools/jdi \
	java/util

stamps/copy-source-files.stamp: stamps/extract.stamp stamps/patch.stamp
	for copy_dir in $(ICEDTEA_COPY_DIRS) ; \
	do \
	  mkdir -p rt/$$copy_dir ; \
	  cp -af openjdk/j2se/src/share/classes/$$copy_dir/*.java \
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

OPENJDK_SOURCEPATH_DIRS = \
        openjdk/j2se/src/share/classes:openjdk/j2se/src/solaris/classes:openjdk/langtools/src/share/classes

# tools.jar class files.
stamps/tools-copy-source-files.stamp: stamps/extract.stamp stamps/patch.stamp
	for copy_dir in `cat tools-copy-files.txt` ; \
        do \
          mkdir -p tools/$$copy_dir ; \
	  cp -arf openjdk/j2se/src/share/classes/$$copy_dir/* \
            tools/$$copy_dir ; \
        done
	mkdir -p stamps
	touch stamps/tools-copy-source-files.stamp

tools-source-files.txt: stamps/extract.stamp \
	stamps/tools-copy-source-files.stamp
	find tools -name '*.java' | sort > $@
	mkdir -p lib/tools

stamps/tools-class-files.stamp: tools-source-files.txt
	$(JAVAC) $(MEMORY_LIMIT) -g -d lib/tools -bootclasspath '' -source 1.6 \
	  -sourcepath rt:tools:$(OPENJDK_SOURCEPATH_DIRS):generated \
	  @$<
	mkdir -p stamps
	touch stamps/tools-class-files.stamp

clean-tools:
	rm -rf lib/tools
	rm -f stamps/tools-class-files.stamp
	rm -f tools-source-files.txt
	rm -f stamps/tools-copy-source-files.stamp

# tools.jar
bootstrap/jdk1.7.0/jre/lib/tools.jar: stamps/tools-class-files.stamp
	mkdir -p bootstrap/jdk1.7.0/jre/lib/
	$(JAR) cf $@ -C lib/tools com -C lib/tools sun \
	  -C lib/tools org

# rt-closed.jar class files.
rt-source-files.txt: stamps/extract.stamp stamps/copy-source-files.stamp
	find rt -name '*.java' | sort > $@

stamps/rt-class-files.stamp: rt-source-files.txt
	mkdir -p lib/rt
	$(JAVAC) $(MEMORY_LIMIT) -g -d lib/rt -bootclasspath '' -source 1.6 \
	  -sourcepath rt:$(OPENJDK_SOURCEPATH_DIRS):generated \
	  @$<
	mkdir -p stamps
	touch stamps/rt-class-files.stamp

clean-rt:
	rm -rf lib/rt
	rm -f stamps/rt-class-files.stamp
	rm -f rt-source-files.txt

# rt-closed.jar.
bootstrap/jdk1.7.0/jre/lib/rt-closed.jar: stamps/rt-class-files.stamp
	mkdir -p bootstrap/jdk1.7.0/jre/lib/
	$(JAR) cf $@ -C lib/rt com -C lib/rt gnu -C lib/rt java \
	  -C lib/rt javax -C lib/rt sun

# gcjwebplugin.so.
gcjwebplugin.so: gcjwebplugin.cc
	$(CXX) $(CXXFLAGS) $(MOZILLA_CFLAGS) \
	  $(MOZILLA_LIBS) $(GLIB_CFLAGS) $(GLIB_LIBS) \
	  $(GTK_CFLAGS) $(GTK_LIBS) \
	  -DPACKAGE_VERSION="\"$(PACKAGE_VERSION)\"" \
	  -fpic -shared -o $@ $<

clean-gcjwebplugin:
	rm -f gcjwebplugin.so

# All Stamped Targets
# ===================
bootstrap-directory: stamps/bootstrap-directory.stamp

bootstrap-directory-symlink: stamps/bootstrap-directory-symlink.stamp

copy-source-files: stamps/copy-source-files.stamp

download: stamps/download.stamp

extract: stamps/extract.stamp

patch: stamps/patch.stamp

plugs: stamps/plugs.stamp

rt-class-files: stamps/rt-class-files.stamp

tools-class-files: stamps/tools-class-files.stamp

tools-copy-source-files: stamps/tools-copy-source-files.stamp

tools: stamps/tools.stamp
