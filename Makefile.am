OPENJDK_VERSION = b26

if NETX_PLUGIN
NETX_PATCH = patches/icedtea-netx-plugin.patch
else
NETX_PATCH =
endif

# Top-Level Targets
# =================

all-local: icedtea-against-icedtea

distclean-local: clean-copy clean-jtreg clean-jtreg-reports
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
	rm -f gcjwebplugin.so

install:

.PHONY: \
	env env-ecj snapshot clean-clone clean-patch \
	clean-clone-ecj clean-patch-ecj clean-bootstrap-directory \
	clean-bootstrap-directory-symlink clean-bootstrap-directory-ecj \
	clean-bootstrap-directory-symlink-ecj icedtea icedtea-debug \
	clean-icedtea icedtea-against-icedtea clean-icedtea-ecj clean-plugs \
	clean-tools-jar clean-shared-objects \
	clean-copy clean-hotspot-tools clean-rt clean-gcjwebplugin \
	hotspot hotspot-helper clean-extra clean-jtreg clean-jtreg-reports

EXTRA_DIST = rt generated $(ICEDTEA_PATCHES) $(ICEDTEA_ECJ_PATCH) \
	gcjwebplugin.cc tools-copy contrib ports \
	patches/icedtea-gcc-4.3.patch extra $(ZERO_PATCHES) \
	patches/icedtea-cacao.patch $(OPENJDK_PATCHES) \
	javaws.png javaws.desktop patches/icedtea-netx-plugin.patch \
	overlays extra jconsole.desktop policytool.desktop
	test/jtreg

# The Binary plugs directory is called jdk1.7.0 for historical reasons. The
# name is completely irrelevant; only contains the plugs to build IcedTea.

# OpenJDK build environment.
LINUX_DIR = linux-$(BUILD_ARCH_DIR)
BUILD_OUTPUT_DIR = openjdk/build/$(LINUX_DIR)
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
ICEDTEA_ENV = \
	"ALT_JDK_IMPORT_PATH=$(ICEDTEA_BOOT_DIR)" \
	"ANT_HOME=$(SYSTEM_ANT_DIR)" \
        "BUILD_NUMBER=$(OPENJDK_VERSION)" \
        "JRE_RELEASE_VERSION=1.7.0-$(OPENJDK_VERSION)" \
        "HOTSPOT_RELEASE_VERSION=1.7.0-$(OPENJDK_VERSION)" \
	"LANG=C" \
	"PATH=/usr/bin:/bin:/usr/sbin:/sbin" \
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
	"ZERO_LIBARCH=$(ZERO_LIBARCH)" \
	"ZERO_BITSPERWORD=$(ZERO_BITSPERWORD)" \
	"ZERO_ENDIANNESS=$(ZERO_ENDIANNESS)" \
	"ZERO_ARCHDEF=$(ZERO_ARCHDEF)" \
	"ZERO_ARCHFLAG=$(ZERO_ARCHFLAG)" \
	"LIBFFI_CFLAGS=$(LIBFFI_CFLAGS)" \
	"LIBFFI_LIBS=$(LIBFFI_LIBS)" \
	"FREETYPE2_HEADERS=$(FREETYPE2_CFLAGS)" \
	"FT2_LIB=$(FREETYPE2_LIBS)" \
	"ALT_PARALLEL_COMPILE_JOBS=$(PARALLEL_JOBS)" \
	"HOTSPOT_BUILD_JOBS=$(PARALLEL_JOBS)" \
	"JAVAC="

if WITH_CACAO
ICEDTEA_ENV += \
	"ALT_HOTSPOT_IMPORT_PATH=$(CACAO)"
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

env:
	@echo 'unset JAVA_HOME'
	@echo 'export $(ICEDTEA_ENV)'

# OpenJDK ecj build environment.
ICEDTEA_BUILD_DIR_ECJ = \
	$(abs_top_builddir)/openjdk-ecj/build/$(LINUX_DIR)/
ICEDTEA_CLS_DIR_ECJ = \
	$(abs_top_builddir)/openjdk-ecj/build/$(LINUX_DIR)/classes
ICEDTEA_ENV_ECJ = \
	"ALT_JDK_IMPORT_PATH=$(ICEDTEA_BOOT_DIR)" \
	"ANT_HOME=$(SYSTEM_ANT_DIR)" \
	"BUILD_NUMBER=$(OPENJDK_VERSION)" \
        "JRE_RELEASE_VERSION=1.7.0-$(OPENJDK_VERSION)" \
        "HOTSPOT_RELEASE_VERSION=1.7.0-$(OPENJDK_VERSION)" \
	"LANG=C" \
	"PATH=/usr/bin:/bin:/usr/sbin:/sbin" \
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
	"GENSRCDIR=$(abs_top_srcdir)/generated" \
	"ICEDTEA_CORE_BUILD=$(ICEDTEA_CORE_BUILD)" \
	"ICEDTEA_ZERO_BUILD=$(ICEDTEA_ZERO_BUILD)" \
	"ZERO_LIBARCH=$(ZERO_LIBARCH)" \
	"ZERO_BITSPERWORD=$(ZERO_BITSPERWORD)" \
	"ZERO_ENDIANNESS=$(ZERO_ENDIANNESS)" \
	"ZERO_ARCHDEF=$(ZERO_ARCHDEF)" \
	"ZERO_ARCHFLAG=$(ZERO_ARCHFLAG)" \
	"LIBFFI_CFLAGS=$(LIBFFI_CFLAGS)" \
	"LIBFFI_LIBS=$(LIBFFI_LIBS)" \
	"FREETYPE2_HEADERS=$(FREETYPE2_CFLAGS)" \
	"FT2_LIB=$(FREETYPE2_LIBS)" \
	"ALT_PARALLEL_COMPILE_JOBS=$(PARALLEL_JOBS)" \
	"HOTSPOT_BUILD_JOBS=$(PARALLEL_JOBS)" \
	"JAVAC="

if WITH_CACAO
ICEDTEA_ENV_ECJ += \
	"ALT_HOTSPOT_IMPORT_PATH=$(CACAO)"
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
	for target in ${abs_top_srcdir}/ports/hotspot/build/*/makefiles/* \
		      ${abs_top_srcdir}/ports/hotspot/src/*/*; do \
	  link=$$(dirname $$target | sed 's/^.*ports/openjdk/'); \
	  ln -sf $$target $$link; \
	done
	ln -sf ${abs_top_builddir}/platform_zero openjdk/hotspot/build/linux
	if ! test "x$(ICEDTEA_CORE_BUILD)$(CACAO)" = "xno"; then \
	  mkdir -p openjdk/jdk/src/solaris/bin/$(BUILD_ARCH_DIR); \
	  ln -sf ${abs_top_builddir}/jvm.cfg openjdk/jdk/src/solaris/bin/$(BUILD_ARCH_DIR)/jvm.cfg; \
	fi
	if ! test "x$(ICEDTEA_ZERO_BUILD)$(CACAO)" = "xno"; then \
	  ln -sf ${abs_top_builddir}/ergo.c openjdk/jdk/src/solaris/bin/ergo_$(BUILD_ARCH_DIR).c; \
	fi
	mkdir -p stamps
	touch stamps/ports.stamp

# If new gcc is installed, apply 4.3 patch
if GCC_OLD
  GCC_PATCH =
else
  GCC_PATCH = patches/icedtea-gcc-4.3.patch
endif

# Patches from the other OpenJDK trees (should be removed
# when in an official bxx release)
OPENJDK_PATCHES = \
	patches/openjdk-color.patch

# If ZERO_BUILD is set then we are building zero and need
# to patch up to OpenJDK 7 HotSpot for the C++ interpreter.
ZERO_PATCHES = \
	patches/icedtea-hotspot7-build-fixes.patch \
	patches/icedtea-bytecodeInterpreter.patch \
	patches/icedtea-signature-iterator.patch \
	patches/icedtea-signed-types.patch \
	patches/icedtea-test-atomic-operations.patch \
	patches/icedtea-zero.patch \
	patches/icedtea-ia64-bugfix.patch

if ZERO_BUILD
  ZERO_PATCHES_COND = $(ZERO_PATCHES)
else
  ZERO_PATCHES_COND = 
endif

ICEDTEA_PATCHES = \
	patches/icedtea-copy-plugs.patch \
	patches/icedtea-version.patch \
	patches/icedtea-text-relocations.patch \
	patches/icedtea-graphics.patch \
	patches/icedtea-debuginfo.patch \
	patches/icedtea-ssl.patch \
	patches/icedtea-license-headers.patch \
	patches/icedtea-webservices.patch \
	$(NETX_PATCH) \
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
	patches/icedtea-corba.patch \
	patches/icedtea-core-build.patch \
	patches/icedtea-linker-options.patch \
	patches/icedtea-static-libstdc++.patch \
	patches/icedtea-lcms-leak.patch \
	patches/icedtea-timerqueue.patch \
	patches/icedtea-generated.patch \
	patches/icedtea-lib64.patch \
	patches/icedtea-zero-build.patch \
	patches/icedtea-print-lsb-release.patch \
	patches/icedtea-jpegclasses.patch \
	patches/icedtea-xinerama.patch \
	patches/icedtea-arm-uname.patch \
	patches/icedtea-ia64-fdlibm.patch \
	patches/icedtea-LCMS-setTagData.patch \
	patches/icedtea-color-createcontext.patch \
	patches/icedtea-color-profiles.patch \
	patches/icedtea-fonts.patch \
	patches/icedtea-gervill.patch \
	patches/icedtea-directaudio-close-trick.patch \
	patches/icedtea-hat-spl-gpl.patch \
	patches/icedtea-sparc.patch \
	$(ZERO_PATCHES_COND) \
	$(GCC_PATCH) \
	$(DISTRIBUTION_PATCHES) \
	patches/icedtea-xslfix.patch \
	patches/icedtea-override.patch

if WITH_CACAO
ICEDTEA_PATCHES += \
	patches/icedtea-cacao.patch
endif

stamps/patch.stamp: stamps/clone.stamp
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
	  if test x$${all_patches_ok} == "xyes" \
	     && echo Checking $$p \
	     && $(PATCH) -l -p0 --dry-run -s -t -f -F 0 < $(abs_top_srcdir)/$$p ; \
	  then \
	    echo Applying $$p ; \
	    $(PATCH) -l -p0 < $(abs_top_srcdir)/$$p ; \
	  else \
	    test x$${all_patches_ok} == "xyes" && all_patches_ok=$$p ; \
	  fi ; \
	done ; \
	mkdir -p stamps ; \
	touch stamps/patch.stamp ; \
	if ! test x$${all_patches_ok} == "xyes"; then \
	  echo ERROR patch $${all_patches_ok} FAILED! ; \
	  echo WARNING make clean-patch before retrying a fix ; \
	  false; \
	fi

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
	if ! test x$${all_patches_ok} == "xyes" ; then \
	  echo "WARNING Not all patches reverted cleanly" ; \
	fi

stamps/overlay.stamp: stamps/patch.stamp
	cp -r $(abs_top_srcdir)/overlays/openjdk/* openjdk/ && touch stamps/overlay.stamp

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
	for target in ${abs_top_srcdir}/ports/hotspot/build/*/makefiles/* \
		      ${abs_top_srcdir}/ports/hotspot/src/*/*; do \
	  link=$$(dirname $$target | sed 's/^.*ports/openjdk-ecj/'); \
	  ln -sf $$target $$link; \
	done
	ln -sf ${abs_top_builddir}/platform_zero openjdk-ecj/hotspot/build/linux
	if ! test "x$(ICEDTEA_CORE_BUILD)$(CACAO)" = "xno"; then \
	  mkdir -p openjdk-ecj/jdk/src/solaris/bin/$(BUILD_ARCH_DIR); \
	  ln -sf ${abs_top_builddir}/jvm.cfg openjdk-ecj/jdk/src/solaris/bin/$(BUILD_ARCH_DIR)/jvm.cfg; \
	fi
	if ! test "x$(ICEDTEA_ZERO_BUILD)$(CACAO)" = "xno"; then \
	  ln -sf ${abs_top_builddir}/ergo.c openjdk-ecj/jdk/src/solaris/bin/ergo_$(BUILD_ARCH_DIR).c; \
	fi
	mkdir -p stamps
	touch stamps/ports-ecj.stamp

# Patch OpenJDK for plug replacements and ecj.
ICEDTEA_ECJ_PATCH = $(abs_top_srcdir)/patches/icedtea-ecj.patch

stamps/patch-ecj.stamp: stamps/clone-ecj.stamp
	echo Checking $(ICEDTEA_ECJ_PATCH) ; \
	if $(PATCH) -p0 -l --dry-run -s -t -f < $(ICEDTEA_ECJ_PATCH) ; \
	then \
	  echo Applying $(ICEDTEA_ECJ_PATCH) ; \
	  $(PATCH) -p0 -l < $(ICEDTEA_ECJ_PATCH) ; \
	  mkdir -p stamps; \
	  touch stamps/patch-ecj.stamp; \
	else \
	  false; \
	fi

clean-patch-ecj:
	rm -f stamps/patch-ecj.stamp
	echo Checking $(ICEDTEA_ECJ_PATCH) ; \
	if $(PATCH) -l -p0 -R --dry-run -s -t -f < $(ICEDTEA_ECJ_PATCH) ; \
	then \
	  echo Reverting $(ICEDTEA_ECJ_PATCH) ; \
	  $(PATCH) -p0 -l -R < $(ICEDTEA_ECJ_PATCH) ; \
	else \
          false; \
	fi

# Bootstrap Directory Targets
# ===========================

# bootstrap/icedtea.
stamps/bootstrap-directory.stamp: stamps/icedtea-against-ecj.stamp
	rm -rf bootstrap/icedtea
	ln -s $(abs_top_builddir)/openjdk-ecj/build/$(LINUX_DIR)/j2sdk-image/ bootstrap/icedtea
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
  ICEDTEA_HOME = icedtea
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
	if test -f $(abs_top_builddir)/bootstrap/jdk1.7.0/jre/lib/tools.jar ; \
	then \
		mkdir -p $(abs_top_builddir)/bootstrap/ecj/lib ; \
		ln -sf $(abs_top_builddir)/bootstrap/jdk1.7.0/jre/lib/tools.jar \
	            $(abs_top_builddir)/bootstrap/ecj/lib/tools.jar; \
	fi
	if test -f $(abs_top_builddir)/bootstrap/jdk1.7.0/jre/lib/rt-closed.jar ; \
	then \
		cp $(abs_top_builddir)/bootstrap/jdk1.7.0/jre/lib/rt-closed.jar \
			$(abs_top_builddir)/bootstrap/ecj/jre/lib/rt.jar; \
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

if WITH_CACAO
  ICEDTEA_BUILD_TARGET=j2se_only
  ICEDTEA_DEBUG_BUILD_TARGET=j2se_fastdebug_only
else
  ICEDTEA_BUILD_TARGET=
  ICEDTEA_DEBUG_BUILD_TARGET=debug_build
endif

# If you change anything here in the icedtea target, please make sure
# you change it in the icedtea-debug target as well.
icedtea: stamps/bootstrap-directory-symlink.stamp \
	stamps/hotspot-tools.stamp stamps/plugs.stamp \
	stamps/ports.stamp stamps/patch.stamp \
	gcjwebplugin.so extra-lib/about.jar env
	$(MAKE) \
	  $(ICEDTEA_ENV) \
	  -C openjdk/ \
	  $(ICEDTEA_BUILD_TARGET)
	if test "$(ENABLE_PLUGIN)" == "yes" ; \
	  then \
	  cp -pPRf gcjwebplugin.so \
	    $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR) ; \
	  cp -pPRf gcjwebplugin.so \
	    $(BUILD_OUTPUT_DIR)/j2re-image/lib/$(INSTALL_ARCH_DIR) ; \
	fi
	cp $(abs_top_srcdir)/rt/net/sourceforge/jnlp/resources/about.jnlp \
	  extra-lib/about.jar \
	  $(BUILD_OUTPUT_DIR)/j2re-image/lib
	cp $(abs_top_srcdir)/rt/net/sourceforge/jnlp/resources/about.jnlp \
	  extra-lib/about.jar \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib
	@echo "IcedTea is served:" $(BUILD_OUTPUT_DIR)

icedtea-debug: stamps/bootstrap-directory-symlink.stamp \
	stamps/hotspot-tools.stamp stamps/plugs.stamp \
	stamps/ports.stamp stamps/patch.stamp \
	gcjwebplugin.so extra-lib/about.jar
	$(MAKE) \
	  $(ICEDTEA_ENV) \
	  -C openjdk/ \
	  $(ICEDTEA_DEBUG_BUILD_TARGET)
	if test "$(ENABLE_PLUGIN)" == "yes" ; \
	  then \
	  cp -pPRf gcjwebplugin.so \
	    $(BUILD_OUTPUT_DIR)-debug/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR) ; \
	  cp -pPRf gcjwebplugin.so \
	    $(BUILD_OUTPUT_DIR)-debug/j2re-image/lib/$(INSTALL_ARCH_DIR) ; \
	fi
	cp $(abs_top_srcdir)/rt/net/sourceforge/jnlp/resources/default.jnlp \
	  extra-lib/about.jar \
	  $(BUILD_OUTPUT_DIR)-debug/j2re-image/lib
	cp $(abs_top_srcdir)/rt/net/sourceforge/jnlp/resources/default.jnlp \
	  extra-lib/about.jar \
	  $(BUILD_OUTPUT_DIR)-debug/j2sdk-image/jre/lib
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
	stamps/ports-ecj.stamp stamps/patch-ecj.stamp env-ecj
	$(MAKE) \
	  $(ICEDTEA_ENV_ECJ) \
	  -C openjdk-ecj \
	  $(ICEDTEA_BUILD_TARGET)
	@echo "ecj-poured IcedTea is served:" \
	  openjdk-ecj/build/$(LINUX_DIR)
	mkdir -p stamps
	touch stamps/icedtea-ecj.stamp

hotspot:
	rm -f stamps/bootstrap-directory-symlink-ecj.stamp
	rm -f stamps/icedtea-ecj.stamp
	rm -f stamps/icedtea-against-ecj.stamp
	$(MAKE) hotspot-helper
	cp openjdk-ecj/build/$(LINUX_DIR)/hotspot/import/jre/lib/$(BUILD_ARCH_DIR)/core/libjvm.so \
	   openjdk-ecj/build/$(LINUX_DIR)/j2sdk-image/jre/lib/$(BUILD_ARCH_DIR)/core
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
	  -C openjdk-ecj clobber

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

clean-tools-jar: clean-hotspot-tools
	rm -f stamps/hotspot-tools.stamp
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

stamps/generated.stamp:
	mkdir -p $(abs_top_srcdir)/generated/java/lang
	cp $(SOLARIS)/java/lang/UNIXProcess.java.$(BUILD_OS) \
	  $(abs_top_srcdir)/generated/java/lang/UNIXProcess.java ;
	touch stamps/generated.stamp

SHARE = openjdk/jdk/src/share/classes
SOLARIS = openjdk/jdk/src/solaris/classes
LANGTOOLS = openjdk/langtools/src/share/classes
JAXP = openjdk/jaxp/src/share/classes
CORBA = openjdk/corba/src/share/classes
JAXWS = openjdk/jaxws/src/share/classes

OPENJDK_SOURCEPATH_DIRS = \
        $(SHARE):$(SOLARIS):$(LANGTOOLS):$(JAXP):$(CORBA):$(JAXWS)

# tools.jar class files.
stamps/hotspot-tools-copy-source-files.stamp: stamps/patch.stamp stamps/overlay.stamp
	for copy_dir in `cat $(abs_top_srcdir)/tools-copy/tools-jdk-copy-files.txt` ; \
	do \
	  mkdir -p hotspot-tools/$$copy_dir ; \
	  cp -pPRf openjdk/jdk/src/share/classes/$$copy_dir/* \
	    hotspot-tools/$$copy_dir ; \
	done
	for copy_dir in `cat $(abs_top_srcdir)/tools-copy/tools-langtools-copy-files.txt` ; \
	do \
	  mkdir -p hotspot-tools/$$copy_dir ; \
	  cp -pPRf openjdk/langtools/src/share/classes/$$copy_dir/* \
	    hotspot-tools/$$copy_dir ; \
	done
	for copy_dir in `cat $(abs_top_srcdir)/tools-copy/tools-corba-copy-files.txt` ; \
	do \
	  mkdir -p hotspot-tools/$$copy_dir ; \
	  cp -pPRf openjdk/corba/src/share/classes/$$copy_dir/* \
	    hotspot-tools/$$copy_dir ; \
	done
	for copy_dir in `cat $(abs_top_srcdir)/tools-copy/tools-jaxws-copy-files.txt` ; \
	do \
	  mkdir -p hotspot-tools/$$copy_dir ; \
	  cp -pPRf openjdk/jaxws/src/share/classes/$$copy_dir/* \
	    hotspot-tools/$$copy_dir ; \
	done
	mkdir -p stamps
	touch stamps/hotspot-tools-copy-source-files.stamp

hotspot-tools-source-files.txt: stamps/clone.stamp \
	stamps/hotspot-tools-copy-source-files.stamp
	find hotspot-tools -name '*.java' | sort > $@
	mkdir -p lib/hotspot-tools

stamps/hotspot-tools-class-files.stamp: hotspot-tools-source-files.txt \
	stamps/generated.stamp
	if ! test -d $(ICEDTEA_BOOT_DIR) ; \
	then \
	  $(JAVAC) $(MEMORY_LIMIT) -g -d lib/hotspot-tools \
            -source 1.5 \
            -sourcepath 'hotspot-tools:$(OPENJDK_SOURCEPATH_DIRS):$(abs_top_srcdir)/generated:$(abs_top_srcdir)/rt' \
            -bootclasspath '' @$< ; \
	else \
	  $(ICEDTEA_BOOT_DIR)/bin/javac $(MEMORY_LIMIT) -g -d lib/hotspot-tools \
	    -source 1.5 \
            -sourcepath 'hotspot-tools:$(OPENJDK_SOURCEPATH_DIRS):$(abs_top_srcdir)/generated:$(abs_top_srcdir)/rt' \
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
	  $(ICEDTEA_BOOT_DIR)/bin/jar cf $@ -C lib/hotspot-tools com -C lib/hotspot-tools sun \
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
rt-source-files.txt: stamps/clone.stamp stamps/copy-source-files.stamp
	find $(abs_top_srcdir)/rt $(abs_top_builddir)/rt -name '*.java' | sort -u > $@

stamps/rt-class-files.stamp: rt-source-files.txt stamps/generated.stamp
	mkdir -p lib/rt
	if ! test -d $(ICEDTEA_BOOT_DIR) ; \
        then \
	  $(JAVAC) $(MEMORY_LIMIT) -g -d lib/rt \
	    -source 1.5 \
            -sourcepath '$(OPENJDK_SOURCEPATH_DIRS):$(abs_top_srcdir)/generated' \
	    -bootclasspath '' @$< ; \
        else \
	  $(ICEDTEA_BOOT_DIR)/bin/javac $(MEMORY_LIMIT) -g -d lib/rt \
            -source 1.5 \
            -sourcepath '$(OPENJDK_SOURCEPATH_DIRS):$(abs_top_srcdir)/generated' \
            -bootclasspath '' @$< ; \
        fi
	cp -r $(abs_top_srcdir)/rt/net/sourceforge/jnlp/resources \
	  lib/rt/net/sourceforge/jnlp/
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
            -C lib/rt javax -C lib/rt net -C lib/rt sun ; \
	else \
	  $(ICEDTEA_BOOT_DIR)/bin/jar cf $@ -C lib/rt com -C lib/rt java \
            -C lib/rt javax -C lib/rt net -C lib/rt sun ; \
	fi
	if test -d bootstrap/ecj/jre/lib ; \
	then \
	  cp bootstrap/jdk1.7.0/jre/lib/rt-closed.jar bootstrap/ecj/jre/lib/rt.jar; \
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
 	    -sourcepath extra -cp bootstrap/jdk1.7.0/jre/lib/rt-closed.jar \
	    -bootclasspath '' @extra-source-files.txt ; \
	else \
	  $(ICEDTEA_BOOT_DIR)/bin/javac $(MEMORY_LIMIT) -g -d extra-lib \
            -source 1.5 \
            -sourcepath extra -cp bootstrap/jdk1.7.0/jre/lib/rt-closed.jar \
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

# jtreg

stamps/jtreg.stamp:
	rm -rf test/jtreg/classes
	mkdir -p test/jtreg/classes
	$(ICEDTEA_BOOT_DIR)/bin/javac -g -d test/jtreg/classes -source 1.5 \
		 -encoding iso-8859-1 `find test/jtreg/com -name '*.java'`
	(cd test/jtreg; \
	 $(ICEDTEA_BOOT_DIR)/bin/jar cfm ../jtreg.jar \
		META-INF/MANIFEST.MF \
		legal  README JavaTest.cmdMgrs.lst JavaTest.toolMgrs.lst \
		`find com -type f -a -not -name '*.java'` \
		-C classes com)
	mkdir -p stamps
	touch $@

clean-jtreg:
	rm -rf test/jtreg/classes
	rm -f test/jtreg.jar
	rm -f stamps/jtreg.stamp

check-hotspot: jtreg
	mkdir -p test/hotspot/JTwork test/hotspot/JTreport
	bootstrap/icedtea/bin/java -jar test/jtreg.jar -v1 -a -ignore:quiet \
		-w:test/hotspot/JTwork -r:test/hotspot/JTreport \
		-jdk:`pwd`/openjdk/control/build/$(LINUX_DIR)/j2sdk-image \
		`pwd`/openjdk/hotspot/test

check-langtools: jtreg
	mkdir -p test/langtools/JTwork test/langtools/JTreport
	bootstrap/icedtea/bin/java -jar test/jtreg.jar -v1 -a -ignore:quiet \
		-w:test/langtools/JTwork -r:test/langtools/JTreport \
		-jdk:`pwd`/openjdk/control/build/$(LINUX_DIR)/j2sdk-image \
		`pwd`/openjdk/langtools/test

check-jdk: jtreg
	mkdir -p test/jdk/JTwork test/jdk/JTreport
	bootstrap/icedtea/bin/java -jar test/jtreg.jar -v1 -a -ignore:quiet \
		-w:test/jdk/JTwork -r:test/jdk/JTreport \
		-jdk:`pwd`/openjdk/control/build/$(LINUX_DIR)/j2sdk-image \
		`pwd`/openjdk/jdk/test

clean-jtreg-reports:
	rm -rf test/hotspot test/langtools test/jdk

jtregcheck: jtreg check-hotspot check-langtools check-jdk

check-local: jtregcheck

# Target Aliases
# ===============

bootstrap-directory-ecj: stamps/bootstrap-directory-ecj.stamp

bootstrap-directory: stamps/bootstrap-directory.stamp

bootstrap-directory-symlink-ecj: stamps/bootstrap-directory-symlink-ecj.stamp

bootstrap-directory-symlink: stamps/bootstrap-directory-symlink.stamp

clone: stamps/clone.stamp

clone-ecj: stamps/clone-ecj.stamp

copy-source-files: stamps/copy-source-files.stamp

download: stamps/download.stamp

generated: stamps/generated.stamp

icedtea-against-ecj: stamps/icedtea-against-ecj.stamp

icedtea-ecj: stamps/icedtea-ecj.stamp

patch-ecj: stamps/patch-ecj.stamp

patch: stamps/patch.stamp

overlay: stamps/overlay.stamp

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

jtreg: stamps/jtreg.stamp
