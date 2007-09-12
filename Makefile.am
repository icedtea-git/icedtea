OPENJDK_VERSION = b19
OPENJDK_DATE = 30_aug_2007
OPENJDK_MD5SUM = 2763a34fe37695fafd09061cd2ca880b

# Top-Level Targets
# =================

all-local: icedtea-against-icedtea

distclean-local: clean-copy
	rm -rf stamps
	rm -f rt-source-files.txt jce-source-files.txt tools-source-files.txt
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
	clean-tools-jar clean-tools clean-shared-objects clean-security-jars \
	clean-copy clean-rt clean-jce clean-gcjwebplugin

EXTRA_DIST = rt jce generated $(ICEDTEA_PATCHES) $(ICEDTEA_ECJ_PATCH) \
	gcjwebplugin.cc patches/icedtea-speed.patch tools-copy-files.txt

# OpenJDK build environment.
LINUX_DIR = linux-$(BUILD_ARCH_DIR)
ICEDTEA_BUILD_DIR = \
	$(abs_top_srcdir)/openjdk/control/build/$(LINUX_DIR)/
ICEDTEA_JCE = \
	$(abs_top_srcdir)/openjdk/control/build/$(LINUX_DIR)/lib/jce.jar
ICEDTEA_RT = \
	$(abs_top_srcdir)/bootstrap/jdk1.7.0/jre/lib/rt-closed.jar
ICEDTEA_CLS_DIR = \
	$(abs_top_srcdir)/openjdk/control/build/$(LINUX_DIR)/classes
ICEDTEA_ENDORSED_DIR = \
	$(abs_top_srcdir)/bootstrap/jdk1.6.0/lib/endorsed
ICEDTEA_ENV = \
	"BUILD_NUMBER=b19" \
	"LANG=C" \
	"PATH=/usr/bin:/bin:/usr/sbin:/sbin" \
	"ALT_BOOTDIR=$(abs_top_srcdir)/bootstrap/jdk1.6.0" \
	"ALT_BINARY_PLUGS_PATH=$(abs_top_srcdir)/bootstrap/jdk1.7.0" \
	"BUILD_ARCH_DIR=$(BUILD_ARCH_DIR)" \
	"ICEDTEA_JCE=$(ICEDTEA_JCE)" \
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
ICEDTEA_JCE_ECJ = \
	$(abs_top_srcdir)/openjdk-ecj/control/build/$(LINUX_DIR)/lib/jce.jar
ICEDTEA_CLS_DIR_ECJ = \
	$(abs_top_srcdir)/openjdk-ecj/control/build/$(LINUX_DIR)/classes
ICEDTEA_ENV_ECJ = \
	"BUILD_NUMBER=b19" \
	"LANG=C" \
	"PATH=/usr/bin:/bin:/usr/sbin:/sbin" \
	"ALT_BOOTDIR=$(abs_top_srcdir)/bootstrap/jdk1.6.0" \
	"ALT_BINARY_PLUGS_PATH=$(abs_top_srcdir)/bootstrap/jdk1.7.0" \
	"BUILD_ARCH_DIR=$(BUILD_ARCH_DIR)" \
	"LIBGCJ_JAR=$(LIBGCJ_JAR)" \
	"ICEDTEA_JCE=$(ICEDTEA_JCE_ECJ)" \
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
	patches/icedtea-java.security.patch \
	patches/icedtea-ssl.patch \
	patches/icedtea-assembler_amd64.patch \
	patches/icedtea-license-headers.patch \
	patches/icedtea-libpng.patch \
	patches/icedtea-plugin.patch \
	patches/icedtea-rmi_amd64.patch \
	patches/icedtea-lesstif_amd64.patch \
	patches/icedtea-tools.patch \
	patches/icedtea-certbundle.patch \
	patches/icedtea-demos.patch \
	$(FAST_BUILD_PATCH)

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
	  chmod ug+w `find openjdk-ecj/j2se/src/share/demo/jvmti \
	     -name sample.makefile.txt` ; \
	fi
	mkdir -p stamps
	touch stamps/extract-ecj.stamp

clean-extract-ecj:
	rm -f stamps/extract-ecj.stamp
	rm -rf openjdk-ecj

# Patch OpenJDK for plug replacements and ecj.
ICEDTEA_ECJ_PATCH = patches/icedtea-ecj-bootstrap.patch

stamps/patch-ecj.stamp: stamps/extract-ecj.stamp
	for p in $(ICEDTEA_PATCHES) ; \
	do \
	  echo Checking $$p ; \
	  if $(PATCH) -p1 -d openjdk-ecj --dry-run -s -t -f -F 0 < $$p ; \
	  then \
	    echo Applying $$p ; \
	    $(PATCH) -p1 -d openjdk-ecj < $$p ; \
	  fi ; \
	done
	echo Checking $(ICEDTEA_ECJ_PATCH) ; \
	if $(PATCH) -p0 --dry-run -s -t -f < $(ICEDTEA_ECJ_PATCH) ; \
	then \
	  echo Applying $(ICEDTEA_ECJ_PATCH) ; \
	  $(PATCH) -p0 < $(ICEDTEA_ECJ_PATCH) ; \
	fi
	mkdir -p stamps
	touch stamps/patch-ecj.stamp

clean-patch-ecj:
	rm -f stamps/patch-ecj.stamp
	echo Checking $(ICEDTEA_ECJ_PATCH) ; \
	if $(PATCH) -p0 -R --dry-run -s -t -f < $(ICEDTEA_ECJ_PATCH) ; \
	then \
	  echo Reverting $(ICEDTEA_ECJ_PATCH) ; \
	  $(PATCH) -p0 -R < $(ICEDTEA_ECJ_PATCH) ; \
	fi
	for p in $(ICEDTEA_PATCHES) ; \
	do \
	  echo Checking $$p ; \
	  if $(PATCH) -p1 -d openjdk-ecj -R --dry-run -s -t -f < $$p ; \
	  then \
	    echo Reverting $$p ; \
	    $(PATCH) -p1 -d openjdk-ecj -R < $$p ; \
	  fi ; \
	done

# Bootstrap Directory Targets
# ===========================

# bootstrap/icedtea.
stamps/bootstrap-directory.stamp: stamps/icedtea-against-ecj.stamp
	rm -rf bootstrap/icedtea
	cp -a openjdk-ecj/control/build/$(LINUX_DIR)/j2sdk-image \
	  bootstrap/icedtea
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
	mkdir -p stamps
	touch stamps/bootstrap-directory-ecj.stamp

clean-bootstrap-directory-ecj:
	rm -f stamps/ootstrap-directory-ecj.stamp
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

icedtea: stamps/tools.stamp stamps/plugs.stamp stamps/extract.stamp \
	stamps/patch.stamp gcjwebplugin.so
	$(MAKE) \
	  $(ICEDTEA_ENV) \
	  -C openjdk/control/make
	cp -af gcjwebplugin.so \
	  openjdk/control/build/$(LINUX_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	cp -af gcjwebplugin.so \
	  openjdk/control/build/$(LINUX_DIR)/j2re-image/lib/$(INSTALL_ARCH_DIR)
	@echo "IcedTea is served:" openjdk/control/build/$(LINUX_DIR)

icedtea-debug: stamps/bootstrap-directory-symlink-ecj.stamp stamps/tools.stamp \
	 stamps/plugs.stamp stamps/extract.stamp \
	stamps/patch.stamp gcjwebplugin.so
	$(MAKE) \
	  $(ICEDTEA_ENV) \
	  -C openjdk/control/make \
	  debug_build
	cp -af gcjwebplugin.so \
	  openjdk/control/build/$(LINUX_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	cp -af gcjwebplugin.so \
	  openjdk/control/build/$(LINUX_DIR)/j2re-image/lib/$(INSTALL_ARCH_DIR)
	@echo "Debug-flavoured IcedTea is served:" \
	  openjdk/control/build/$(LINUX_DIR)-debug

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
	stamps/extract-ecj.stamp stamps/patch-ecj.stamp
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

stamps/plugs.stamp: $(SHARED_OBJECT_FILES) $(SECURITY_JAR_FILES) \
	bootstrap/jdk1.7.0/jre/lib/rt-closed.jar \
	bootstrap/jdk1.7.0/jre/lib/jce.jar
	mkdir -p stamps
	touch stamps/plugs.stamp

stamps/tools.stamp: bootstrap/ecj/lib/tools.jar
	mkdir -p stamps
	touch stamps/tools.stamp

clean-plugs: clean-plugs clean-shared-objects clean-security-jars \
	clean-copy clean-rt clean-jce
	rm -f stamps/plugs.stamp
	rm -f bootstrap/jdk1.7.0/jre/lib/rt-closed.jar
	rm -f bootstrap/jdk1.7.0/jre/lib/jce.jar

clean-tools-jar: clean-tools-jar clean-tools
	rm -f stamps/tools.stamp
	rm -rf tools/
	rm -f bootstrap/jdk1.6.0/lib/tools.jar

# Shared objects.
SHARED_OBJECT_FILES = \
	bootstrap/jdk1.7.0/jre/lib/$(INSTALL_ARCH_DIR)/libdcpr.so \
	bootstrap/jdk1.7.0/jre/lib/$(INSTALL_ARCH_DIR)/libjsoundalsa.so \
	bootstrap/jdk1.7.0/jre/lib/$(INSTALL_ARCH_DIR)/libjsound.so \
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

# Security JAR files.
SECURITY_JAR_FILES = \
	bootstrap/jdk1.7.0/jre/lib/ext/sunjce_provider.jar \
	bootstrap/jdk1.7.0/jre/lib/security/local_policy.jar \
	bootstrap/jdk1.7.0/jre/lib/security/US_export_policy.jar

$(SECURITY_JAR_FILES):
	mkdir -p bootstrap/jdk1.7.0/jre/lib/ext
	mkdir -p bootstrap/jdk1.7.0/jre/lib/security
	touch dummy-file.txt
	for security_jar in $(SECURITY_JAR_FILES) ; \
	do \
	  $(JAR) cf $$security_jar dummy-file.txt ; \
	done
	rm -f dummy-file.txt

clean-security-jars:
	rm -f $(SECURITY_JAR_FILES)

# Sources copied from OpenJDK.
ICEDTEA_COPY_DIRS = \
	com/sun/jdi \
	com/sun/jdi/connect \
	com/sun/jdi/connect/spi \
	com/sun/jdi/event \
	com/sun/jdi/request \
	com/sun/jmx/snmp/agent \
	com/sun/tools/jdi \
	java/security/cert \
	java/util \
	javax/security/auth \
	javax/security/auth/callback \
	javax/security/sasl

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
        openjdk/j2se/src/share/classes:openjdk/j2se/src/solaris/classes

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
	  -sourcepath jce:rt:tools:$(OPENJDK_SOURCEPATH_DIRS):generated \
	  @$<
	mkdir -p stamps
	touch stamps/tools-class-files.stamp

clean-tools:
	rm -rf lib/tools
	rm -f stamps/tools-class-files.stamp
	rm -f tools-source-files.txt
	rm -f stamps/tools-copy-source-files.stamp

# tools.jar
bootstrap/ecj/lib/tools.jar: stamps/tools-class-files.stamp
	mkdir -p bootstrap/ecj/lib
	$(JAR) cf $@ -C lib/tools com -C lib/tools sun \
	  -C lib/tools org
 
# rt-closed.jar class files.
rt-source-files.txt: stamps/extract.stamp stamps/copy-source-files.stamp
	find rt -name '*.java' | sort > $@

stamps/rt-class-files.stamp: rt-source-files.txt
	mkdir -p lib/rt
	$(JAVAC) $(MEMORY_LIMIT) -g -d lib/rt -bootclasspath '' -source 1.6 \
	  -sourcepath rt:$(OPENJDK_SOURCEPATH_DIRS):generated:jce \
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
	  -C lib/rt javax -C lib/rt sun

# jce.jar class files.
jce-source-files.txt:
	find jce -name '*.java' -print | sort > $@

stamps/jce-class-files.stamp: jce-source-files.txt stamps/rt-class-files.stamp
	mkdir -p lib/jce
	$(JAVAC) $(MEMORY_LIMIT) -g -d lib/jce -bootclasspath '' -source 1.6 \
	  -classpath lib/rt @$<
	mkdir -p stamps
	touch stamps/jce-class-files.stamp

clean-jce:
	rm -rf lib/jce
	rm -f stamps/jce-class-files.stamp
	rm -f jce-source-files.txt

# jce.jar.
bootstrap/jdk1.7.0/jre/lib/jce.jar: stamps/jce-class-files.stamp
	mkdir -p bootstrap/jdk1.7.0/jre/lib
	$(JAR) cf $@ -C lib/jce gnu -C lib/jce javax -C lib/jce sun

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

icedtea-ecj: stamps/icedtea-ecj

jce-class-files: stamps/jce-class-files.stamp

patch-ecj: stamps/patch-ecj.stamp

patch: stamps/patch.stamp

plugs: stamps/plugs.stamp

rt-class-files: stamps/rt-class-files.stamp

tools-class-files: stamps/tools-class-files.stamp

tools-copy-source-files: stamps/tools-copy-source-files.stamp

tools: stamps/tools.stamp
