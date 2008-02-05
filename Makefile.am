OPENJDK_VERSION = b24

# Top-Level Targets
# =================

all-local: icedtea-against-icedtea

distclean-local: clean-copy
	rm -rf stamps
	rm -f rt-source-files.txt tools-source-files.txt \
	  hotspot-tools-source-files.txt
	rm -rf bootstrap
	rm -rf lib
	rm -rf openjdk-ecj
	rm -rf openjdk
	rm -rf hotspot-tools
	rm -f gcjwebplugin.so

install:

.PHONY: \
	env env-ecj snapshot clean-clone clean-patch \
	clean-clone-ecj clean-patch-ecj clean-bootstrap-directory \
	clean-bootstrap-directory-symlink clean-bootstrap-directory-ecj \
	clean-bootstrap-directory-symlink-ecj icedtea icedtea-debug \
	clean-icedtea icedtea-against-icedtea clean-icedtea-ecj clean-plugs \
	clean-tools-jar clean-tools clean-shared-objects \
	clean-copy clean-hotspot-tools clean-rt clean-gcjwebplugin \
	hotspot hotspot-helper

EXTRA_DIST = rt generated $(ICEDTEA_PATCHES) $(ICEDTEA_ECJ_PATCH) \
	gcjwebplugin.cc patches/icedtea-speed.patch tools-copy contrib ports

# OpenJDK build environment.
LINUX_DIR = linux-$(BUILD_ARCH_DIR)
BUILD_OUTPUT_DIR = openjdk/build/$(LINUX_DIR)
ICEDTEA_BUILD_DIR = \
	$(abs_top_srcdir)/$(BUILD_OUTPUT_DIR)
ICEDTEA_RT = \
	$(abs_top_srcdir)/bootstrap/jdk1.7.0/jre/lib/rt-closed.jar
ICEDTEA_CLS_DIR = \
	$(abs_top_srcdir)/$(BUILD_OUTPUT_DIR)/classes
ICEDTEA_BOOT_DIR= \
	$(abs_top_srcdir)/bootstrap/jdk1.6.0
ICEDTEA_ENDORSED_DIR = \
	$(ICEDTEA_BOOT_DIR)/lib/endorsed
ICEDTEA_ENV = \
	"ALT_JDK_IMPORT_PATH=$(ICEDTEA_BOOT_DIR)" \
	"ANT_HOME=$(SYSTEM_ANT_DIR)" \
        "BUILD_NUMBER=$(OPENJDK_VERSION)" \
        "JRE_RELEASE_VERSION=1.7.0-$(OPENJDK_VERSION)" \
        "HOTSPOT_RELEASE_VERSION=1.7.0-$(OPENJDK_VERSION)" \
	"LANG=C" \
	"PATH=/usr/bin:/bin:/usr/sbin:/sbin" \
	"ALT_BOOTDIR=$(ICEDTEA_BOOT_DIR)" \
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
	"ALT_HOTSPOT_IMPORT_PATH=$(CACAO)"
endif

env:
	@echo 'unset JAVA_HOME'
	@echo 'export $(ICEDTEA_ENV)'

# OpenJDK ecj build environment.
ICEDTEA_BUILD_DIR_ECJ = \
	$(abs_top_srcdir)/openjdk-ecj/build/$(LINUX_DIR)/
ICEDTEA_CLS_DIR_ECJ = \
	$(abs_top_srcdir)/openjdk-ecj/build/$(LINUX_DIR)/classes
ICEDTEA_ENV_ECJ = \
	"ALT_JDK_IMPORT_PATH=$(ICEDTEA_BOOT_DIR)" \
	"ANT_HOME=$(SYSTEM_ANT_DIR)" \
	"BUILD_NUMBER=$(OPENJDK_VERSION)" \
        "JRE_RELEASE_VERSION=1.7.0-$(OPENJDK_VERSION)" \
        "HOTSPOT_RELEASE_VERSION=1.7.0-$(OPENJDK_VERSION)" \
	"LANG=C" \
	"PATH=/usr/bin:/bin:/usr/sbin:/sbin" \
	"ALT_BOOTDIR=$(ICEDTEA_BOOT_DIR)" \
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

if WITH_CACAO
ICEDTEA_ENV_ECJ += \
	"ALT_HOTSPOT_IMPORT_PATH=$(CACAO)"
endif

env-ecj:
	@echo 'unset JAVA_HOME'
	@echo 'export $(ICEDTEA_ENV_ECJ)'

# Mercurial snapshot.
snapshot: dist
	mv $(DIST_ARCHIVES) $(distdir)-`hg tip --template '{node}'`.tar.gz

# Creates archive of openjdk.
dist-openjdk:
	if ! hg; \
	then \
	  echo "Mercurial cannot be found. Please install it and try again."; \
	  exit 1; \
	else \
	  if ! hg fclone -h; \
	  then \
	    echo "No forest extension found. Please refer to" \
              "http://selenic.com/mercurial/wiki/index.cgi/ForestExtension"; \
            exit 1; \
	  fi; \
	fi
	hg fclone -r jdk7-$(OPENJDK_VERSION) $(OPENJDK_URL) openjdk-dist/openjdk
	find -name \\.hg* | xargs rm -rf	
	cd openjdk-dist && $(ZIP) -r openjdk-$(OPENJDK_VERSION) openjdk/
	mv openjdk-dist/openjdk-$(OPENJDK_VERSION).zip . 
	rm -rf openjdk-dist

# OpenJDK Source Preparation Targets
# ==================================

# Download OpenJDK sources.
OPENJDK_URL = http://hg.openjdk.java.net/jdk7/jdk7

if USE_ALT_OPENJDK_SRC_ZIP
  OPENJDK_SRC_ZIP = $(ALT_OPENJDK_SRC_ZIP)
else
  OPENJDK_SRC_ZIP =
endif

stamps/clone.stamp:
	if ! test -d openjdk ; then \
	  if [ ! -z $(OPENJDK_SRC_ZIP) ] ; then \
	    $(UNZIP) -q $(OPENJDK_SRC_ZIP) ; \
	  else \
	    if ! hg fclone -h; \
            then \
              echo "No forest extension found. Please refer to" \
              "http://selenic.com/mercurial/wiki/index.cgi/ForestExtension"; \
              exit 1; \
            fi; \
	    hg fclone -r jdk7-$(OPENJDK_VERSION) $(OPENJDK_URL) openjdk; \
	  fi ; \
	fi
	chmod -R ug+w openjdk
	mkdir -p stamps
	touch stamps/clone.stamp

clean-clone:
	rm -f stamps/clone.stamp
	rm -rf openjdk

# Link ports sources into tree
stamps/ports.stamp: stamps/clone.stamp
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

# If new libgcj is installed, apply 4.3 patch
if LIBGCJ_OLD
  GCC_PATCH =
else
  GCC_PATCH = patches/icedtea-gcc-4.3.patch
endif

ICEDTEA_PATCHES = \
	patches/icedtea-copy-plugs.patch \
	patches/icedtea-version.patch \
	patches/icedtea-text-relocations.patch \
	patches/icedtea-graphics.patch \
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
	patches/icedtea-timerqueue.patch \
	patches/icedtea-jnlp-launcher.patch \
	patches/icedtea-generated.patch \
	patches/icedtea-lib64.patch \
	patches/icedtea-zero.patch \
	$(GCC_PATCH) \
	$(FAST_BUILD_PATCH) \
	$(DISTRIBUTION_PATCHES)

if WITH_CACAO
ICEDTEA_PATCHES += \
	patches/icedtea-cacao.patch
endif

stamps/patch.stamp: stamps/clone.stamp
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

# Copy over OpenJDK sources for ecj.
stamps/clone-ecj.stamp: stamps/patch.stamp
	if ! test -d openjdk-ecj ; then \
	    cp -al openjdk openjdk-ecj ; \
	fi
	mkdir -p stamps
	touch stamps/clone-ecj.stamp

clean-clone-ecj:
	rm -f stamps/clone-ecj.stamp
	rm -rf openjdk-ecj

# Link ports sources into tree
stamps/ports-ecj.stamp: stamps/clone-ecj.stamp
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

stamps/patch-ecj.stamp: stamps/clone-ecj.stamp
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

# Bootstrap Directory Targets
# ===========================

# bootstrap/icedtea.
stamps/bootstrap-directory.stamp: stamps/icedtea-against-ecj.stamp
	rm -rf bootstrap/icedtea
	ln -s $(abs_top_srcdir)/openjdk-ecj/build/$(LINUX_DIR)/j2sdk-image/ bootstrap/icedtea
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
	if test -d $(SYSTEM_GCJ_DIR); \
	then \
	  mkdir -p bootstrap/ecj/jre/lib; \
	  ln -sf $(SYSTEM_GCJ_DIR)/jre/lib/$(INSTALL_ARCH_DIR) \
	    bootstrap/ecj/jre/lib/; \
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
if WITH_CACAO
  MEMORY_LIMIT = -J-Xmx1024m
else
  MEMORY_LIMIT = -J-Xmx512m
endif
else
  MEMORY_LIMIT =
endif

# If you change anything here in the icedtea target, please make sure
# you change it in the icedtea-debug target as well.
icedtea: stamps/bootstrap-directory-symlink.stamp \
	stamps/hotspot-tools.stamp stamps/plugs.stamp \
	stamps/ports.stamp stamps/patch.stamp gcjwebplugin.so \
	stamps/tools.stamp
	$(MAKE) \
	  $(ICEDTEA_ENV) \
	  -C openjdk/ j2se_only
	if test "$(ENABLE_PLUGIN)" == "yes" ; \
	  then \
	  cp -af gcjwebplugin.so \
	    $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR) ; \
	  cp -af gcjwebplugin.so \
	    $(BUILD_OUTPUT_DIR)/j2re-image/lib/$(INSTALL_ARCH_DIR) ; \
	fi
	$(FASTJAR) uf $(BUILD_OUTPUT_DIR)/j2sdk-image/lib/tools.jar \
	  -C lib/tools .
	@echo "IcedTea is served:" $(BUILD_OUTPUT_DIR)

icedtea-debug: stamps/bootstrap-directory-symlink.stamp \
	stamps/hotspot-tools.stamp stamps/plugs.stamp \
	stamps/ports.stamp stamps/patch.stamp gcjwebplugin.so \
	stamps/tools.stamp
	$(MAKE) \
	  $(ICEDTEA_ENV) \
	  -C openjdk/ \
	  j2se_fastdebug_only
	if test "$(ENABLE_PLUGIN)" == "yes" ; \
	  then \
	  cp -af gcjwebplugin.so \
	    $(BUILD_OUTPUT_DIR)-debug/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR) ; \
	  cp -af gcjwebplugin.so \
	    $(BUILD_OUTPUT_DIR)-debug/j2re-image/lib/$(INSTALL_ARCH_DIR) ; \
	fi
	$(FASTJAR) uf $(BUILD_OUTPUT_DIR)-debug/j2sdk-image/lib/tools.jar \
	  -C lib/tools .
	@echo "IcedTea (debug build) is served:" \
	  $(BUILD_OUTPUT_DIR)-debug

clean-icedtea: stamps/clone.stamp
	$(MAKE) \
	  $(ICEDTEA_ENV) \
	  -C openjdk/ clobber

icedtea-against-icedtea: \
	stamps/bootstrap-directory-symlink.stamp \
	icedtea

# OpenJDK ecj Targets
# ===================

stamps/icedtea-ecj.stamp: stamps/hotspot-tools.stamp stamps/plugs.stamp \
	stamps/ports-ecj.stamp stamps/patch-ecj.stamp
	$(MAKE) \
	  $(ICEDTEA_ENV_ECJ) \
	  -C openjdk-ecj/ j2se_only
	@echo "ecj-poured IcedTea is served:" \
	  openjdk-ecj/build/$(LINUX_DIR)
	mkdir -p stamps
	touch stamps/icedtea-ecj.stamp

hotspot:
	rm -f stamps/bootstrap-directory-symlink-ecj.stamp
	rm -f stamps/icedtea-ecj.stamp
	rm -f stamps/icedtea-against-ecj.stamp
	$(MAKE) hotspot-helper
	cp openjdk-ecj/build/$(LINUX_DIR)/hotspot/import/jre/lib/$(BUILD_ARCH_DIR)/client/libjvm.so \
	   openjdk-ecj/build/$(LINUX_DIR)/j2sdk-image/jre/lib/$(BUILD_ARCH_DIR)/client
	@echo "Hotspot is served:" openjdk-ecj/build/$(LINUX_DIR)/j2sdk-image

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
	  -C openjdk-ecj/ clobber

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

stamps/hotspot-tools.stamp: bootstrap/jdk1.7.0/jre/lib/tools.jar
	mkdir -p stamps
	touch stamps/hotspot-tools.stamp

clean-plugs: clean-plugs clean-shared-objects \
	clean-copy clean-rt
	rm -f stamps/plugs.stamp
	rm -f bootstrap/jdk1.7.0/jre/lib/rt-closed.jar

clean-tools-jar: clean-hotspot-tools clean-tools
	rm -f stamps/hotspot-tools.stamp
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

stamps/copy-source-files.stamp: stamps/patch.stamp
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
stamps/hotspot-tools-copy-source-files.stamp: stamps/patch.stamp
	for copy_dir in `cat tools-copy/tools-jdk-copy-files.txt` ; \
	do \
	  mkdir -p hotspot-tools/$$copy_dir ; \
	  cp -arf openjdk/jdk/src/share/classes/$$copy_dir/* \
	    hotspot-tools/$$copy_dir ; \
	done
	
	for copy_dir in `cat tools-copy/tools-langtools-copy-files.txt` ; \
	do \
	  mkdir -p hotspot-tools/$$copy_dir ; \
	  cp -arf openjdk/langtools/src/share/classes/$$copy_dir/* \
	    hotspot-tools/$$copy_dir ; \
	done
	
	for copy_dir in `cat tools-copy/tools-corba-copy-files.txt` ; \
	do \
	  mkdir -p hotspot-tools/$$copy_dir ; \
	  cp -arf openjdk/corba/src/share/classes/$$copy_dir/* \
	    hotspot-tools/$$copy_dir ; \
	done
	
	for copy_dir in `cat tools-copy/tools-jaxws-copy-files.txt` ; \
	do \
	  mkdir -p hotspot-tools/$$copy_dir ; \
	  cp -arf openjdk/jaxws/src/share/classes/$$copy_dir/* \
	    hotspot-tools/$$copy_dir ; \
	done
	
	mkdir -p stamps
	touch stamps/hotspot-tools-copy-source-files.stamp

hotspot-tools-source-files.txt: stamps/clone.stamp \
	stamps/hotspot-tools-copy-source-files.stamp
	find hotspot-tools -name '*.java' | sort > $@
	mkdir -p lib/hotspot-tools

stamps/hotspot-tools-class-files.stamp: hotspot-tools-source-files.txt
	if ! test -f stamps/icedtea-ecj.stamp ; \
	then \
	  $(JAVAC) $(MEMORY_LIMIT) -g -d lib/hotspot-tools -bootclasspath '' -source 1.6 \
	    -sourcepath 'rt:hotspot-tools:$(OPENJDK_SOURCEPATH_DIRS):generated' \
	    @$< ; \
	else \
	  $(ICEDTEA_BOOT_DIR)/bin/javac $(MEMORY_LIMIT) -g -d lib/hotspot-tools \
	    -bootclasspath '' -source 1.6 \
            -sourcepath 'rt:hotspot-tools:$(OPENJDK_SOURCEPATH_DIRS):generated' \
            @$< ; \
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
	if ! test -f stamps/icedtea-ecj.stamp ; \
        then \
    	  $(JAR) cf $@ -C lib/hotspot-tools com -C lib/hotspot-tools sun \
	    -C lib/hotspot-tools org -C lib/hotspot-tools gnu -C lib/hotspot-tools java \
	    -C lib/hotspot-tools javax ; \
	else \
	  $(ICEDTEA_BOOT_DIR)/bin/jar cf $@ -C lib/hotspot-tools com -C lib/hotspot-tools sun \
            -C lib/hotspot-tools org -C lib/hotspot-tools gnu -C lib/hotspot-tools java \
            -C lib/hotspot-tools javax ; \
	fi
	if test -d bootstrap/ecj ; \
	then \
	  mkdir -p bootstrap/ecj/lib/; \
	  ln -sf $(abs_top_srcdir)/bootstrap/jdk1.7.0/jre/lib/tools.jar \
	    $(abs_top_srcdir)/bootstrap/ecj/lib/tools.jar; \
	fi

# rt-closed.jar class files.
rt-source-files.txt: stamps/clone.stamp stamps/copy-source-files.stamp
	find rt -name '*.java' | sort > $@

stamps/rt-class-files.stamp: rt-source-files.txt
	mkdir -p lib/rt
	if ! test -f stamps/icedtea-ecj.stamp ; \
        then \
	  $(JAVAC) $(MEMORY_LIMIT) -g -d lib/rt -bootclasspath '' -source 1.6 \
          -sourcepath rt:$(OPENJDK_SOURCEPATH_DIRS):generated \
          @$< ; \
        else \
	  $(ICEDTEA_BOOT_DIR)/bin/javac $(MEMORY_LIMIT) -g -d lib/rt \
	  -bootclasspath '' -source 1.6 \
          -sourcepath rt:$(OPENJDK_SOURCEPATH_DIRS):generated \
          @$< ; \
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
	if ! test -f stamps/icedtea-ecj.stamp ; \
        then \
          $(JAR) cf $@ -C lib/rt com -C lib/rt gnu -C lib/rt java \
          -C lib/rt javax -C lib/rt sun -C lib/rt org ; \
	else \
          $(ICEDTEA_BOOT_DIR)/bin/jar cf $@ -C lib/rt com -C lib/rt gnu -C lib/rt java \
          -C lib/rt javax -C lib/rt sun -C lib/rt org ; \
	fi
	if test -d bootstrap/ecj/jre/lib ; \
	then \
	  cp bootstrap/jdk1.7.0/jre/lib/rt-closed.jar bootstrap/ecj/jre/lib/rt.jar; \
	fi

# gcjwebplugin.so.
gcjwebplugin.so: gcjwebplugin.cc
	if test "$(ENABLE_PLUGIN)" == "yes" ; \
	then \
	  $(CXX) $(CXXFLAGS) $(MOZILLA_CFLAGS) \
	    $(MOZILLA_LIBS) $(GLIB_CFLAGS) $(GLIB_LIBS) \
	    $(GTK_CFLAGS) $(GTK_LIBS) \
	    -DPACKAGE_VERSION="\"$(PACKAGE_VERSION)\"" \
	    -fpic -shared -o $@ $< ; \
	fi

clean-gcjwebplugin:
	rm -f gcjwebplugin.so

#extra tools
stamps/tools.stamp: stamps/tools-class-files.stamp
	touch stamps/tools.stamp

tools-source-files.txt: stamps/clone.stamp
	find tools -name '*.java' | sort > $@

stamps/tools-class-files.stamp: tools-source-files.txt \
	stamps/rt-class-files.stamp
	mkdir -p lib/tools
	if ! test -f stamps/icedtea-ecj.stamp ; \
        then \
	  $(JAVAC) $(MEMORY_LIMIT) -g -d lib/tools \
          -source 1.6 \
          -sourcepath tools \
          @$< ; \
        else \
	  $(ICEDTEA_BOOT_DIR)/bin/javac $(MEMORY_LIMIT) -g -d lib/tools \
          -source 1.6 \
          -sourcepath tools \
          @$< ; \
        fi
	cp tools/default.jnlp lib/tools
	cp -r tools/netx/jnlp/resources lib/tools/netx/jnlp
	mkdir -p stamps
	touch stamps/tools-class-files.stamp

clean-tools:
	rm -rf lib/tools
	rm -rf stamps/tools-class-files.stamp
	rm -rf tools-source-files.txt

# Target Aliases
# ===============

bootstrap-directory-ecj: stamps/bootstrap-directory-ecj.stamp

bootstrap-directory: stamps/bootstrap-directory.stamp

bootstrap-directory-symlink-ecj: stamps/bootstrap-directory-symlink-ecj.stamp

bootstrap-directory-symlink: stamps/bootstrap-directory-symlink.stamp

copy-source-files: stamps/copy-source-files.stamp

clone: stamps/clone.stamp

clone-ecj: stamps/clone-ecj.stamp

icedtea-against-ecj: stamps/icedtea-against-ecj.stamp

icedtea-ecj: stamps/icedtea-ecj.stamp

patch-ecj: stamps/patch-ecj.stamp

patch: stamps/patch.stamp

plugs: stamps/plugs.stamp

rt-class-files: stamps/rt-class-files.stamp

hotspot-tools-class-files: stamps/hotspot-tools-class-files.stamp

hotspot-tools-copy-source-files: stamps/hotspot-tools-copy-source-files.stamp

hotspot-tools: stamps/hotspot-tools.stamp

tools: stamps/tools.stamp

tools-class-files: stamps/tools-class-files.stamp

ports: stamps/ports.stamp

ports-ecj: stamps/ports-ecj.stamp

clean: distclean-local
