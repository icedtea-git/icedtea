OPENJDK_VERSION = b23
OPENJDK_DATE = 30_oct_2007
OPENJDK_MD5SUM = 7eed137c0321e0b5b75ff2d646ce0d0d

# Top-Level Targets
# =================

all-local: icedtea-against-icedtea

distclean-local: clean-copy
	rm -rf stamps
	rm -f rt-source-files.txt tools-source-files.txt
	rm -rf bootstrap
	rm -rf lib
	rm -rf openjdk-ecj
	rm -rf openjdk
	rm -rf tools
	rm -f gcjwebplugin.so

install:

.PHONY: \
	env env-ecj snapshot clean-download clean-extract clean-patch \
	clean-extract-ecj clean-patch-ecj clean-bootstrap-directory \
	clean-bootstrap-directory-symlink clean-bootstrap-directory-ecj \
	clean-bootstrap-directory-symlink-ecj icedtea icedtea-debug \
	clean-icedtea icedtea-against-icedtea clean-icedtea-ecj clean-plugs \
	clean-tools-jar clean-tools clean-shared-objects \
	clean-copy clean-rt clean-gcjwebplugin

EXTRA_DIST = rt generated $(ICEDTEA_PATCHES) $(ICEDTEA_ECJ_PATCH) \
	gcjwebplugin.cc patches/icedtea-speed.patch tools-copy contrib ports

# OpenJDK build environment.
LINUX_DIR = linux-$(BUILD_ARCH_DIR)
BUILD_OUTPUT_DIR = openjdk/control/build/$(LINUX_DIR)
ICEDTEA_BUILD_DIR = \
	$(abs_top_srcdir)/$(BUILD_OUTPUT_DIR)
ICEDTEA_RT = \
	$(abs_top_srcdir)/bootstrap/jdk1.7.0/jre/lib/rt-closed.jar
ICEDTEA_CLS_DIR = \
	$(abs_top_srcdir)/$(BUILD_OUTPUT_DIR)/classes
ICEDTEA_ENDORSED_DIR = \
	$(abs_top_srcdir)/bootstrap/jdk1.6.0/lib/endorsed
ICEDTEA_ENV = \
	"ALT_JDK_IMPORT_PATH=$(abs_top_srcdir)/bootstrap/jdk1.6.0/" \
	"ANT_HOME=$(SYSTEM_ANT_DIR)" \
        "BUILD_NUMBER=$(OPENJDK_VERSION)" \
        "JRE_RELEASE_VERSION=1.7.0-$(OPENJDK_VERSION)" \
        "HOTSPOT_RELEASE_VERSION=1.7.0-$(OPENJDK_VERSION)" \
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

env:
	@echo 'unset JAVA_HOME'
	@echo 'export $(ICEDTEA_ENV)'

# OpenJDK ecj build environment.
ICEDTEA_BUILD_DIR_ECJ = \
	$(abs_top_srcdir)/openjdk-ecj/control/build/$(LINUX_DIR)/
ICEDTEA_CLS_DIR_ECJ = \
	$(abs_top_srcdir)/openjdk-ecj/control/build/$(LINUX_DIR)/classes
ICEDTEA_ENV_ECJ = \
	"ALT_JDK_IMPORT_PATH=$(abs_top_srcdir)/bootstrap/jdk1.6.0/" \
	"ANT_HOME=$(SYSTEM_ANT_DIR)" \
	"BUILD_NUMBER=$(OPENJDK_VERSION)" \
        "JRE_RELEASE_VERSION=1.7.0-$(OPENJDK_VERSION)" \
        "HOTSPOT_RELEASE_VERSION=1.7.0-$(OPENJDK_VERSION)" \
	"LANG=C" \
	"PATH=/usr/bin:/bin:/usr/sbin:/sbin" \
	"ALT_BOOTDIR=$(abs_top_srcdir)/bootstrap/jdk1.6.0" \
	"ALT_BINARY_PLUGS_PATH=$(abs_top_srcdir)/bootstrap/jdk1.7.0" \
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
	"GENSRCDIR=$(abs_top_srcdir)/generated" \
	"FREETYPE2_INC_DIR=$(FREETYPE2_INC_DIR)"

env-ecj:
	@echo 'unset JAVA_HOME'
	@echo 'export $(ICEDTEA_ENV_ECJ)'

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
	  chmod -R ug+w openjdk ; \
	fi
	mkdir -p stamps
	touch stamps/extract.stamp

clean-extract:
	rm -f stamps/extract.stamp
	rm -rf openjdk

# Link ports sources into tree
stamps/ports.stamp: stamps/extract.stamp
	for target in ports/hotspot/build/*/platform_* \
		      ports/hotspot/build/*/makefiles/* \
		      ports/hotspot/src/*/* \
		      ports/jdk/src/*/bin/*; do \
	  link=$$(dirname $$target | sed 's/^ports/openjdk/'); \
	  ln -sf $$PWD/$$target $$link; \
	done
	mkdir -p stamps
	touch stamps/ports.stamp

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
	patches/icedtea-gtkplaf.patch \
	patches/icedtea-memory-limits.patch \
	patches/icedtea-sunsrc.patch \
	patches/icedtea-libraries.patch \
	patches/icedtea-javafiles.patch \
	patches/icedtea-bytecodeInterpreterWithChecks.patch \
	patches/icedtea-corba.patch \
	patches/icedtea-bytecodeInterpreter.patch \
	patches/icedtea-signature-iterator.patch \
	patches/icedtea-signed-types.patch \
	patches/icedtea-test-atomic-operations.patch \
	patches/icedtea-core-build.patch \
	patches/icedtea-linker-options.patch \
	patches/icedtea-ports.patch \
	patches/icedtea-lcms-leak.patch \
	patches/icedtea-c1-unimplemented.patch \
	$(FAST_BUILD_PATCH) \
	$(DISTRIBUTION_PATCHES)

stamps/patch.stamp: stamps/extract.stamp
	for p in $(ICEDTEA_PATCHES) ; \
	do \
	  echo Checking $$p ; \
	  if $(PATCH) -l -p0 --dry-run -s -t -f -F 0 < $$p ; \
	  then \
	    echo Applying $$p ; \
	    $(PATCH) -l -p0 < $$p ; \
	  fi ; \
	done
	mkdir -p stamps
	touch stamps/patch.stamp

clean-patch:
	rm -f stamps/patch.stamp
	for p in $(ICEDTEA_PATCHES) ; \
	do \
	  echo Checking $$p ; \
	  if $(PATCH) -l -p0 -R --dry-run -s -t -f < $$p ; \
	  then \
	    echo Reverting $$p ; \
	    $(PATCH) -l -p0 -R < $$p ; \
	  fi ; \
	done

# OpenJDK ecj Source Preparation Targets
# ======================================

# Extract OpenJDK sources for ecj.
stamps/extract-ecj.stamp: stamps/download.stamp
	if ! test -d openjdk-ecj ; \
	then \
	  mkdir tmp-ecj ; \
	  $(UNZIP) -q -d tmp-ecj $(OPENJDK_SRC_ZIP) ; \
	  mv tmp-ecj/openjdk openjdk-ecj ; \
	  rmdir tmp-ecj ; \
	  chmod -R ug+w openjdk-ecj ; \
	fi
	mkdir -p stamps
	touch stamps/extract-ecj.stamp

clean-extract-ecj:
	rm -f stamps/extract-ecj.stamp
	rm -rf openjdk-ecj

# Link ports sources into tree
stamps/ports-ecj.stamp: stamps/extract.stamp
	for target in ports/hotspot/build/*/platform_* \
		      ports/hotspot/build/*/makefiles/* \
		      ports/hotspot/src/*/* \
		      ports/jdk/src/*/bin/*; do \
	  link=$$(dirname $$target | sed 's/^ports/openjdk-ecj/'); \
	  ln -sf $$PWD/$$target $$link; \
	done
	mkdir -p stamps
	touch stamps/ports-ecj.stamp

# Patch OpenJDK for plug replacements and ecj.
ICEDTEA_ECJ_PATCH = patches/icedtea-ecj.patch

stamps/patch-ecj.stamp: stamps/extract-ecj.stamp
	for p in $(ICEDTEA_PATCHES) ; \
	do \
	  echo Checking $$p ; \
	  if $(PATCH) -l -p1 -d openjdk-ecj --dry-run -s -t -f -F 0 < $$p ; \
	  then \
	    echo Applying $$p ; \
	    $(PATCH) -l -p1 -d openjdk-ecj < $$p ; \
	  fi ; \
	done
	echo Checking $(ICEDTEA_ECJ_PATCH) ; \
	if $(PATCH) -p0 -l --dry-run -s -t -f < $(ICEDTEA_ECJ_PATCH) ; \
	then \
	  echo Applying $(ICEDTEA_ECJ_PATCH) ; \
	  $(PATCH) -p0 -l < $(ICEDTEA_ECJ_PATCH) ; \
	fi
	mkdir -p stamps
	touch stamps/patch-ecj.stamp

clean-patch-ecj:
	rm -f stamps/patch-ecj.stamp
	echo Checking $(ICEDTEA_ECJ_PATCH) ; \
	if $(PATCH) -l -p0 -R --dry-run -s -t -f < $(ICEDTEA_ECJ_PATCH) ; \
	then \
	  echo Reverting $(ICEDTEA_ECJ_PATCH) ; \
	  $(PATCH) -p0 -l -R < $(ICEDTEA_ECJ_PATCH) ; \
	fi
	for p in $(ICEDTEA_PATCHES) ; \
	do \
	  echo Checking $$p ; \
	  if $(PATCH) -p1 -l -d openjdk-ecj -R --dry-run -s -t -f < $$p ; \
	  then \
	    echo Reverting $$p ; \
	    $(PATCH) -p1 -l -d openjdk-ecj -R < $$p ; \
	  fi ; \
	done

# Bootstrap Directory Targets
# ===========================

# bootstrap/icedtea.
stamps/bootstrap-directory.stamp: stamps/icedtea-against-ecj.stamp
	rm -rf bootstrap/icedtea
	ln -s $(abs_top_srcdir)/openjdk-ecj/control/build/$(LINUX_DIR)/j2sdk-image/ bootstrap/icedtea
	if ! test -d bootstrap/icedtea/include; then \
	  ln -sf $(ICEDTEA_BUILD_DIR_ECJ)/include bootstrap/icedtea/include; \
	fi
	mkdir -p stamps
	touch stamps/bootstrap-directory.stamp

clean-bootstrap-directory:
	rm -f stamps/bootstrap-directory.stamp
	rm -rf bootstrap/icedtea

if WITH_ICEDTEA
  BOOTSTRAP_DIRECTORY_STAMP =
  ICEDTEA_HOME = $(SYSTEM_ICEDTEA_DIR)
else
  BOOTSTRAP_DIRECTORY_STAMP = stamps/bootstrap-directory.stamp
  ICEDTEA_HOME = icedtea
endif

# bootstrap/jdk1.6.0 to bootstrap/icedtea symlink.
stamps/bootstrap-directory-symlink.stamp: $(BOOTSTRAP_DIRECTORY_STAMP)
	mkdir -p bootstrap
	rm -f bootstrap/jdk1.6.0
	ln -sf $(ICEDTEA_HOME) bootstrap/jdk1.6.0
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
stamps/bootstrap-directory-ecj.stamp:
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
	mkdir -p bootstrap/ecj/jre/lib
	ln -sf $(SYSTEM_GCJ_DIR)/jre/lib/$(INSTALL_ARCH_DIR) bootstrap/ecj/jre/lib/
	mkdir -p bootstrap/ecj/include
	for i in $(SYSTEM_GCJ_DIR)/include/*; do \
	  test -r $$i | continue; \
	  i=`basename $$i`; \
	  rm -f bootstrap/ecj/include/$$i; \
	  ln -s $(SYSTEM_GCJ_DIR)/include/$$i bootstrap/ecj/include/$$i; \
	done
	if test -f bootstrap/jdk1.7.0/jre/lib/tools.jar ; \
	then \
		mkdir -p bootstrap/ecj/lib ; \
		ln -sf $(abs_top_srcdir)/bootstrap/jdk1.7.0/jre/lib/tools.jar \
	            $(abs_top_srcdir)/bootstrap/ecj/lib/tools.jar; \
	fi
	if test -f bootstrap/jdk1.7.0/jre/lib/rt-closed.jar ; \
	then \
		cp bootstrap/jdk1.7.0/jre/lib/rt-closed.jar \
			bootstrap/ecj/jre/lib/rt.jar; \
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
  MEMORY_LIMIT = -J-Xmx512m
else
  MEMORY_LIMIT =
endif

# If you change anything here in the icedtea target, please make sure
# you change it in the icedtea-debug target as well.
icedtea: stamps/tools.stamp stamps/plugs.stamp stamps/extract.stamp \
	stamps/ports.stamp stamps/patch.stamp gcjwebplugin.so
	$(MAKE) \
	  $(ICEDTEA_ENV) \
	  -C openjdk/control/make
	cp -af gcjwebplugin.so \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	cp -af gcjwebplugin.so \
	  $(BUILD_OUTPUT_DIR)/j2re-image/lib/$(INSTALL_ARCH_DIR)
	@echo "IcedTea is served:" $(BUILD_OUTPUT_DIR)

icedtea-debug: 	stamps/bootstrap-directory-symlink.stamp \
	stamps/tools.stamp stamps/plugs.stamp stamps/extract.stamp \
	stamps/ports.stamp stamps/patch.stamp gcjwebplugin.so
	$(MAKE) \
	  $(ICEDTEA_ENV) \
	  -C openjdk/control/make \
	  debug_build
	cp -af gcjwebplugin.so \
	 $(BUILD_OUTPUT_DIR)-debug/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	cp -af gcjwebplugin.so \
	  $(BUILD_OUTPUT_DIR)-debug/j2re-image/lib/$(INSTALL_ARCH_DIR)
	@echo "IcedTea (debug build) is served:" \
	  $(BUILD_OUTPUT_DIR)-debug

clean-icedtea: stamps/extract.stamp
	$(MAKE) \
	  $(ICEDTEA_ENV) \
	  -C openjdk/control/make clobber

icedtea-against-icedtea: \
	stamps/bootstrap-directory-symlink.stamp \
	icedtea

# OpenJDK ecj Targets
# ===================

stamps/icedtea-ecj.stamp: stamps/tools.stamp stamps/plugs.stamp \
	stamps/extract-ecj.stamp stamps/ports-ecj.stamp stamps/patch-ecj.stamp
	$(MAKE) \
	  $(ICEDTEA_ENV_ECJ) \
	  -C openjdk-ecj/control/make
	@echo "ecj-poured IcedTea is served:" \
	  openjdk-ecj/control/build/$(LINUX_DIR)
	mkdir -p stamps
	touch stamps/icedtea-ecj.stamp

clean-icedtea-ecj: stamps/extract-ecj.stamp
	rm -f stamps/icedtea-ecj.stamp
	$(MAKE) \
	  $(ICEDTEA_ENV_ECJ) \
	  -C openjdk-ecj/control/make clobber

stamps/icedtea-against-ecj.stamp: \
	stamps/bootstrap-directory-symlink-ecj.stamp \
	stamps/icedtea-ecj.stamp
	mkdir -p stamps
	touch stamps/icedtea-against-ecj.stamp

# Binary Plugs and Tools Targets
# ==============================

stamps/plugs.stamp: $(SHARED_OBJECT_FILES) \
	bootstrap/jdk1.7.0/jre/lib/rt-closed.jar
	mkdir -p stamps
	touch stamps/plugs.stamp

stamps/tools.stamp: bootstrap/jdk1.7.0/jre/lib/tools.jar
	mkdir -p stamps
	touch stamps/tools.stamp

clean-plugs: clean-plugs clean-shared-objects \
	clean-copy clean-rt
	rm -f stamps/plugs.stamp
	rm -f bootstrap/jdk1.7.0/jre/lib/rt-closed.jar

clean-tools-jar: clean-tools-jar clean-tools
	rm -f stamps/tools.stamp
	rm -rf tools/
	rm -f bootstrap/jdk1.7.0/jre/lib/tools.jar

# Shared objects.
SHARED_OBJECT_FILES = \
	bootstrap/jdk1.7.0/jre/lib/$(INSTALL_ARCH_DIR)/libdcpr.so \
	bootstrap/jdk1.7.0/jre/lib/$(INSTALL_ARCH_DIR)/libjsoundhs.so

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
	java/util \
	java/rmi \
	javax/net/ssl

stamps/copy-source-files.stamp: stamps/extract.stamp stamps/patch.stamp
	for copy_dir in $(ICEDTEA_COPY_DIRS) ; \
	do \
	  mkdir -p rt/$$copy_dir ; \
	  cp -af openjdk/jdk/src/share/classes/$$copy_dir/*.java \
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
stamps/tools-copy-source-files.stamp: stamps/extract.stamp stamps/patch.stamp
	for copy_dir in `cat tools-copy/tools-jdk-copy-files.txt` ; \
        do \
          mkdir -p tools/$$copy_dir ; \
	  cp -arf openjdk/jdk/src/share/classes/$$copy_dir/* \
            tools/$$copy_dir ; \
        done
	
	for copy_dir in `cat tools-copy/tools-langtools-copy-files.txt` ; \
	do \
	  mkdir -p tools/$$copy_dir ; \
	  cp -arf openjdk/langtools/src/share/classes/$$copy_dir/* \
	    tools/$$copy_dir ; \
	done

	for copy_dir in `cat tools-copy/tools-corba-copy-files.txt` ; \
	do \
	  mkdir -p tools/$$copy_dir ; \
	  cp -arf openjdk/corba/src/share/classes/$$copy_dir/* \
	    tools/$$copy_dir ; \
	done

	for copy_dir in `cat tools-copy/tools-jaxws-copy-files.txt` ; \
	do \
	  mkdir -p tools/$$copy_dir ; \
	  cp -arf openjdk/jaxws/src/share/classes/$$copy_dir/* \
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
	  -sourcepath 'rt:tools:$(OPENJDK_SOURCEPATH_DIRS):generated' \
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
	  -C lib/tools org -C lib/tools gnu -C lib/tools java \
	  -C lib/tools javax
	if test -d bootstrap/ecj ; \
	then \
	  mkdir -p bootstrap/ecj/lib/; \
	  ln -sf $(abs_top_srcdir)/bootstrap/jdk1.7.0/jre/lib/tools.jar \
	    $(abs_top_srcdir)/bootstrap/ecj/lib/tools.jar; \
	fi
 
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
	mkdir -p bootstrap/jdk1.7.0/jre/lib
	$(JAR) cf $@ -C lib/rt com -C lib/rt gnu -C lib/rt java \
	  -C lib/rt javax -C lib/rt sun -C lib/rt org
	if test -d bootstrap/ecj/jre/lib ; \
	then \
	  cp bootstrap/jdk1.7.0/jre/lib/rt-closed.jar bootstrap/ecj/jre/lib/rt.jar; \
	fi

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

bootstrap-directory-ecj: stamps/bootstrap-directory-ecj.stamp

bootstrap-directory: stamps/bootstrap-directory.stamp

bootstrap-directory-symlink-ecj: stamps/bootstrap-directory-symlink-ecj.stamp

bootstrap-directory-symlink: stamps/bootstrap-directory-symlink.stamp

copy-source-files: stamps/copy-source-files.stamp

download: stamps/download.stamp

extract-ecj: stamps/extract-ecj.stamp

extract: stamps/extract.stamp

icedtea-against-ecj: stamps/icedtea-against-ecj.stamp

icedtea-ecj: stamps/icedtea-ecj.stamp

patch-ecj: stamps/patch-ecj.stamp

patch: stamps/patch.stamp

plugs: stamps/plugs.stamp

rt-class-files: stamps/rt-class-files.stamp

tools-class-files: stamps/tools-class-files.stamp

tools-copy-source-files: stamps/tools-copy-source-files.stamp

tools: stamps/tools.stamp
