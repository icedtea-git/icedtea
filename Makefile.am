OPENJDK_VERSION = b18
OPENJDK_DATE = 16_aug_2007
OPENJDK_MD5SUM = 475985960afd082e2aebac4e878a18fb

# Top-Level Targets
# =================

all-local: icedtea-against-icedtea

distclean-local: clean-copy
	rm -f download.stamp extract.stamp patch.stamp extract-ecj.stamp \
	  patch-ecj.stamp bootstrap-directory.stamp \
	  bootstrap-directory-symlink.stamp bootstrap-directory-ecj.stamp \
	  bootstrap-directory-symlink-ecj.stamp icedtea-ecj.stamp \
	  icedtea-against-ecj.stamp plugs.stamp copy-source-files.stamp \
	  rt-class-files.stamp jce-class-files.stamp
	rm -f rt-source-files.txt jce-source-files.txt
	rm -rf bootstrap
	rm -rf lib
	rm -rf openjdk-ecj
	rm -rf openjdk
	rm -f gcjwebplugin.so

install:

.PHONY: \
	env env-ecj snapshot clean-download clean-extract clean-patch \
	clean-extract-ecj clean-patch-ecj clean-bootstrap-directory \
	clean-bootstrap-directory-symlink clean-bootstrap-directory-ecj \
	clean-bootstrap-directory-symlink-ecj icedtea icedtea-debug \
	clean-icedtea icedtea-against-icedtea clean-icedtea-ecj clean-plugs \
	clean-shared-objects clean-security-jars clean-copy clean-rt clean-jce \
	clean-gcjwebplugin

EXTRA_DIST = rt jce generated $(ICEDTEA_PATCHES) $(ICEDTEA_ECJ_PATCH) \
	gcjwebplugin.cc patches/icedtea-speed.patch

# OpenJDK build environment.
LINUX_DIR = linux-$(BUILD_ARCH_DIR)
ICEDTEA_BUILD_DIR = \
	$(abs_top_srcdir)/openjdk/control/build/$(LINUX_DIR)/
ICEDTEA_JCE = \
	$(abs_top_srcdir)/openjdk/control/build/$(LINUX_DIR)/lib/jce.jar
ICEDTEA_RT = \
	$(abs_top_srcdir)/bootstrap/jdk1.7.0/jre/lib/rt.jar
ICEDTEA_CLS_DIR = \
	$(abs_top_srcdir)/openjdk/control/build/$(LINUX_DIR)/classes
ICEDTEA_ENDORSED_DIR = \
	$(abs_top_srcdir)/bootstrap/jdk1.6.0/lib/endorsed
ICEDTEA_ENV = \
	"LANG=C" \
	"PATH=/usr/bin:/bin:/usr/sbin:/sbin" \
	"ALT_BOOTDIR=$(abs_top_srcdir)/bootstrap/jdk1.6.0" \
	"ALT_CLOSED_JDK_IMPORT_PATH=$(abs_top_srcdir)/bootstrap/jdk1.7.0" \
	"BUILD_ARCH_DIR=$(BUILD_ARCH_DIR)" \
	"LIBGCJ_JAR=$(LIBGCJ_JAR)" \
	"ICEDTEA_JCE=$(ICEDTEA_JCE)" \
	"ICEDTEA_RT=$(ICEDTEA_RT)" \
	"ICEDTEA_BUILD_DIR=$(ICEDTEA_BUILD_DIR)" \
	"ICEDTEA_CLS_DIR=$(ICEDTEA_CLS_DIR)" \
	"ICEDTEA_ENDORSED_DIR=$(ICEDTEA_ENDORSED_DIR)" \
	"ENDORSED=-Djava.endorsed.dirs=$(ICEDTEA_ENDORSED_DIR)" \
	"BOOTCLASSPATH_CLS_RT=-bootclasspath $(ICEDTEA_CLS_DIR):$(ICEDTEA_RT)" \
	"BOOTCLASSPATH_CLS=-bootclasspath $(ICEDTEA_CLS_DIR)" \
	"BOOTCLASSPATH_RT_LIBGCJ=-bootclasspath $(ICEDTEA_RT):$(LIBGCJ_JAR)" \
	"CLASSPATH= " \
	"JAVA_HOME= " \
	"LD_LIBRARY_PATH= " \
	"FREETYPE2_INC_DIR=$(FREETYPE2_INC_DIR)"

env:
	@echo 'export $(ICEDTEA_ENV)'

# OpenJDK ecj build environment.
ICEDTEA_BUILD_DIR_ECJ = \
	$(abs_top_srcdir)/openjdk-ecj/control/build/$(LINUX_DIR)/
ICEDTEA_JCE_ECJ = \
	$(abs_top_srcdir)/openjdk-ecj/control/build/$(LINUX_DIR)/lib/jce.jar
ICEDTEA_CLS_DIR_ECJ = \
	$(abs_top_srcdir)/openjdk-ecj/control/build/$(LINUX_DIR)/classes
ICEDTEA_ENV_ECJ = \
	"LANG=C" \
	"PATH=/usr/bin:/bin:/usr/sbin:/sbin" \
	"ALT_BOOTDIR=$(abs_top_srcdir)/bootstrap/jdk1.6.0" \
	"ALT_CLOSED_JDK_IMPORT_PATH=$(abs_top_srcdir)/bootstrap/jdk1.7.0" \
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
	"CLASSPATH= " \
	"JAVA_HOME= " \
	"LD_LIBRARY_PATH= " \
	"FREETYPE2_INC_DIR=$(FREETYPE2_INC_DIR)"

env-ecj:
	@echo 'export $(ICEDTEA_ENV_ECJ)'

# Mercurial snapshot.
snapshot: dist
	mv $(DIST_ARCHIVES) $(distdir)-`hg tip --template '{node}'`.tar.gz

# OpenJDK Source Preparation Targets
# ==================================

# Download OpenJDK sources.
OPENJDK_URL = http://download.java.net/openjdk/jdk7/promoted/$(OPENJDK_VERSION)/

if USE_ALT_OPENJDK_SRC_ZIP
  OPENJDK_SRC_ZIP = $(ALT_OPENJDK_SRC_ZIP)
else
  OPENJDK_SRC_ZIP = openjdk-7-ea-src-$(OPENJDK_VERSION)-$(OPENJDK_DATE).zip
endif

download.stamp:
	rm -f download.stamp
	if ! echo "$(OPENJDK_MD5SUM)  $(OPENJDK_SRC_ZIP)" \
	  | $(MD5SUM) --check ; \
	then \
	  $(WGET) $(OPENJDK_URL)$(OPENJDK_SRC_ZIP) ; \
	fi
	touch download.stamp

clean-download:
	rm -f download.stamp
	rm -f $(OPENJDK_SRC_ZIP)

# Extract OpenJDK sources.
extract.stamp: download.stamp
	if ! test -d openjdk ; \
	then \
	  $(UNZIP) -q $(OPENJDK_SRC_ZIP) ; \
	fi
	touch extract.stamp

clean-extract:
	rm -f extract.stamp
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
	$(FAST_BUILD_PATCH)

patch.stamp: extract.stamp
	for p in $(ICEDTEA_PATCHES) ; \
	do \
	  echo Checking $$p ; \
	  if $(PATCH) -p0 --dry-run -s -t -f -F 0 < $$p ; \
	  then \
	    echo Applying $$p ; \
	    $(PATCH) -p0 < $$p ; \
	  fi ; \
	done
	touch patch.stamp

clean-patch:
	rm -f patch.stamp
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
extract-ecj.stamp: download.stamp
	if ! test -d openjdk-ecj ; \
	then \
	  mkdir tmp-ecj ; \
	  $(UNZIP) -q -d tmp-ecj $(OPENJDK_SRC_ZIP) ; \
	  mv tmp-ecj/openjdk openjdk-ecj ; \
	  rmdir tmp-ecj ; \
	fi
	touch extract-ecj.stamp

clean-extract-ecj:
	rm -f extract-ecj.stamp
	rm -rf openjdk-ecj

# Patch OpenJDK for plug replacements and ecj.
ICEDTEA_ECJ_PATCH = patches/icedtea-ecj-bootstrap.patch

patch-ecj.stamp: extract-ecj.stamp
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
	touch patch-ecj.stamp

clean-patch-ecj:
	rm -f patch-ecj.stamp
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
bootstrap-directory.stamp: icedtea-against-ecj.stamp
	rm -rf bootstrap/icedtea
	cp -a openjdk-ecj/control/build/$(LINUX_DIR)/j2sdk-image \
	  bootstrap/icedtea
	touch bootstrap-directory.stamp

clean-bootstrap-directory:
	rm -f bootstrap-directory.stamp
	rm -rf bootstrap/icedtea

# bootstrap/jdk1.6.0 to bootstrap/icedtea symlink.
bootstrap-directory-symlink.stamp: bootstrap-directory.stamp
	rm -f bootstrap/jdk1.6.0
	ln -sf icedtea bootstrap/jdk1.6.0
	touch bootstrap-directory-symlink.stamp

clean-bootstrap-directory-symlink:
	rm -f bootstrap-directory-symlink.stamp
	rm -f bootstrap/jdk1.6.0

# Bootstrap ecj Directory Targets
# ===============================

# bootstrap/ecj.
bootstrap-directory-ecj.stamp:
	mkdir -p bootstrap/ecj/bin
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
	touch bootstrap-directory-ecj.stamp

clean-bootstrap-directory-ecj:
	rm -f bootstrap-directory-ecj.stamp
	rm -rf bootstrap/ecj

# bootstrap/jdk1.6.0 to bootstrap/ecj symlink.
bootstrap-directory-symlink-ecj.stamp: bootstrap-directory-ecj.stamp
	rm -f bootstrap/jdk1.6.0
	ln -sf ecj bootstrap/jdk1.6.0
	touch bootstrap-directory-symlink-ecj.stamp

clean-bootstrap-directory-symlink-ecj:
	rm -f bootstrap-directory-symlink-ecj.stamp
	rm -f bootstrap/jdk1.6.0


# OpenJDK Targets
# ===============

icedtea: plugs.stamp extract.stamp patch.stamp gcjwebplugin.so
	$(MAKE) \
	  $(ICEDTEA_ENV) \
	  -C openjdk/control/make
	cp -af gcjwebplugin.so \
	  openjdk/control/build/$(LINUX_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	cp -af gcjwebplugin.so \
	  openjdk/control/build/$(LINUX_DIR)/j2re-image/lib/$(INSTALL_ARCH_DIR)
	@echo "IcedTea is served:" openjdk/control/build/$(LINUX_DIR)

icedtea-debug: plugs.stamp extract.stamp patch.stamp gcjwebplugin.so
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

clean-icedtea: extract.stamp
	$(MAKE) \
	  $(ICEDTEA_ENV) \
	  -C openjdk/control/make clobber

icedtea-against-icedtea: \
	bootstrap-directory-symlink.stamp \
	icedtea

# OpenJDK ecj Targets
# ===================

icedtea-ecj.stamp: plugs.stamp extract-ecj.stamp patch-ecj.stamp
	$(MAKE) \
	  $(ICEDTEA_ENV_ECJ) \
	  -C openjdk-ecj/control/make
	@echo "ecj-poured IcedTea is served:" \
	  openjdk-ecj/control/build/$(LINUX_DIR)
	touch icedtea-ecj.stamp

clean-icedtea-ecj: extract-ecj.stamp
	rm -f icedtea-ecj.stamp
	$(MAKE) \
	  $(ICEDTEA_ENV_ECJ) \
	  -C openjdk-ecj/control/make clobber

icedtea-against-ecj.stamp: \
	bootstrap-directory-symlink-ecj.stamp \
	icedtea-ecj.stamp
	touch icedtea-against-ecj.stamp

# Binary Plugs Targets
# ====================

plugs.stamp: $(SHARED_OBJECT_FILES) $(SECURITY_JAR_FILES) \
	bootstrap/jdk1.7.0/jre/lib/rt.jar \
	bootstrap/jdk1.7.0/jre/lib/jce.jar
	touch plugs.stamp

clean-plugs: clean-plugs clean-shared-objects clean-security-jars \
	clean-copy clean-rt clean-jce
	rm -f plugs.stamp
	rm -f bootstrap/jdk1.7.0/jre/lib/rt.jar
	rm -f bootstrap/jdk1.7.0/jre/lib/jce.jar

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
	for security_jar in $(SECURITY_JAR_FILES) ; \
	do \
	  $(JAR) cf $$security_jar ; \
	done

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

copy-source-files.stamp: extract.stamp patch.stamp
	for copy_dir in $(ICEDTEA_COPY_DIRS) ; \
	do \
	  mkdir -p rt/$$copy_dir ; \
	  cp -af openjdk/j2se/src/share/classes/$$copy_dir/*.java \
	    rt/$$copy_dir ; \
	done
	touch copy-source-files.stamp

clean-copy:
	rm -f copy-source-files.stamp
	for copy_dir in $(ICEDTEA_COPY_DIRS) ; \
	do \
	  rm -rf rt/$$copy_dir ; \
	done

if FOUND_ECJ
  JCOMPILER = $(ECJ) -nowarn
else
if FOUND_JAVAC
  JCOMPILER = $(JAVAC)
endif
endif

# rt.jar class files.
rt-source-files.txt: extract.stamp copy-source-files.stamp
	find rt -name '*.java' | sort > $@

OPENJDK_SOURCEPATH_DIRS = \
	openjdk/j2se/src/share/classes:openjdk/j2se/src/solaris/classes

rt-class-files.stamp: rt-source-files.txt
	$(JCOMPILER) -g -d lib/rt -bootclasspath '' -source 1.6 \
	  -sourcepath rt:$(OPENJDK_SOURCEPATH_DIRS):generated:jce \
	  @rt-source-files.txt
	touch rt-class-files.stamp

clean-rt:
	rm -rf lib/rt
	rm -f rt-class-files.stamp
	rm -f rt-source-files.txt

# rt.jar.
bootstrap/jdk1.7.0/jre/lib/rt.jar: rt-class-files.stamp
	mkdir -p bootstrap/jdk1.7.0/jre/lib
	$(JAR) cf $@ -C lib/rt com -C lib/rt gnu -C lib/rt java \
	  -C lib/rt javax -C lib/rt sun

# jce.jar class files.
jce-source-files.txt:
	find jce -name '*.java' -print | sort > $@

jce-class-files.stamp: jce-source-files.txt rt-class-files.stamp
	$(JCOMPILER) -g -d lib/jce -bootclasspath '' -source 1.6 \
	  -classpath lib/rt @$<
	touch jce-class-files.stamp

clean-jce:
	rm -rf lib/jce
	rm -f jce-class-files.stamp
	rm -f jce-source-files.txt

# jce.jar.
bootstrap/jdk1.7.0/jre/lib/jce.jar: jce-class-files.stamp
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
