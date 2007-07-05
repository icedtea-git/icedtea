OPENJDK_VERSION = b14
OPENJDK_DATE = 21_jun_2007
OPENJDK_MD5SUM = c126a32966e4bdb6a60a5724d9447966

OPENJDK_URL = \
	http://download.java.net/openjdk/jdk7/promoted/$(OPENJDK_VERSION)/

EXTRA_DIST = rt jce generated $(ICEDTEA_PATCHES) $(ICEDTEA_ECJ_PATCH)

if USE_CCACHE
  CCACHE_PATCH = patches/icedtea-ccache.patch
else
  CCACHE_PATCH = 
endif

if FAST_BUILD
  FAST_BUILD_PATCH = patches/icedtea-speed.patch
else
  FAST_BUILD_PATCH =
endif

if USE_ALT_OPENJDK_SRC_ZIP
  OPENJDK_SRC_ZIP = $(ALT_OPENJDK_SRC_ZIP)
else
  OPENJDK_SRC_ZIP = openjdk-7-ea-src-$(OPENJDK_VERSION)-$(OPENJDK_DATE).zip
endif

ICEDTEA_PATCHES = \
	patches/icedtea-copy-plugs.patch \
	patches/icedtea-lesstif.patch \
	patches/icedtea-version.patch \
	patches/icedtea-text-relocations.patch \
	patches/icedtea-graphics.patch \
	patches/icedtea-gcc.patch \
	patches/icedtea-debuginfo.patch \
	patches/icedtea-java.security.patch \
	$(FAST_BUILD_PATCH) \
	$(CCACHE_PATCH)

ICEDTEA_ECJ_PATCH = \
	patches/icedtea-ecj-bootstrap.patch

ICEDTEA_BUILD_DIR = \
	$(abs_top_srcdir)/openjdk/control/build/linux-$(BUILD_ARCH_DIR)/

ICEDTEA_JCE = \
	$(abs_top_srcdir)/openjdk/control/build/linux-$(BUILD_ARCH_DIR)/lib/jce.jar

ICEDTEA_RT = \
	$(abs_top_srcdir)/bootstrap/jdk1.7.0/jre/lib/rt.jar

ICEDTEA_CLS_DIR = \
	$(abs_top_srcdir)/openjdk/control/build/linux-$(BUILD_ARCH_DIR)/classes

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
	"LD_LIBRARY_PATH= "

ICEDTEA_COPY_DIRS = \
	rt/com/sun/jdi \
	rt/com/sun/jdi/connect \
	rt/com/sun/jdi/connect/spi \
	rt/com/sun/jdi/event \
	rt/com/sun/jdi/request \
	rt/com/sun/tools \
	rt/com/sun/tools/jdi \
	rt/java/util \
	rt/com/sun/jmx/snmp/agent/ \
	rt/javax/net/ssl/ \
	rt/java/security/cert/ \
	rt/javax/security/sasl/ \
	rt/javax/security/auth/ \
	rt/javax/security/auth/callback/

ICEDTEA_COPY_SRC = \
	com/sun/jmx/snmp/agent/SnmpMib.java \
	com/sun/jdi/AbsentInformationException.java \
	com/sun/jdi/Accessible.java \
	com/sun/jdi/ArrayReference.java \
	com/sun/jdi/ArrayType.java \
	com/sun/jdi/BooleanType.java \
	com/sun/jdi/BooleanValue.java \
	com/sun/jdi/Bootstrap.java \
	com/sun/jdi/ByteType.java \
	com/sun/jdi/ByteValue.java \
	com/sun/jdi/CharType.java \
	com/sun/jdi/CharValue.java \
	com/sun/jdi/ClassLoaderReference.java \
	com/sun/jdi/ClassNotLoadedException.java \
	com/sun/jdi/ClassNotPreparedException.java \
	com/sun/jdi/ClassObjectReference.java \
	com/sun/jdi/ClassType.java \
	com/sun/jdi/connect/AttachingConnector.java \
	com/sun/jdi/connect/Connector.java \
	com/sun/jdi/connect/IllegalConnectorArgumentsException.java \
	com/sun/jdi/connect/LaunchingConnector.java \
	com/sun/jdi/connect/ListeningConnector.java \
	com/sun/jdi/connect/spi/ClosedConnectionException.java \
	com/sun/jdi/connect/spi/Connection.java \
	com/sun/jdi/connect/spi/TransportService.java \
	com/sun/jdi/connect/Transport.java \
	com/sun/jdi/connect/TransportTimeoutException.java \
	com/sun/jdi/connect/VMStartException.java \
	com/sun/jdi/DoubleType.java \
	com/sun/jdi/DoubleValue.java \
	com/sun/jdi/event/AccessWatchpointEvent.java \
	com/sun/jdi/event/BreakpointEvent.java \
	com/sun/jdi/event/ClassPrepareEvent.java \
	com/sun/jdi/event/ClassUnloadEvent.java \
	com/sun/jdi/event/EventIterator.java \
	com/sun/jdi/event/Event.java \
	com/sun/jdi/event/EventQueue.java \
	com/sun/jdi/event/EventSet.java \
	com/sun/jdi/event/ExceptionEvent.java \
	com/sun/jdi/event/LocatableEvent.java \
	com/sun/jdi/event/MethodEntryEvent.java \
	com/sun/jdi/event/MethodExitEvent.java \
	com/sun/jdi/event/ModificationWatchpointEvent.java \
	com/sun/jdi/event/MonitorContendedEnteredEvent.java \
	com/sun/jdi/event/MonitorContendedEnterEvent.java \
	com/sun/jdi/event/MonitorWaitedEvent.java \
	com/sun/jdi/event/MonitorWaitEvent.java \
	com/sun/jdi/event/StepEvent.java \
	com/sun/jdi/event/ThreadDeathEvent.java \
	com/sun/jdi/event/ThreadStartEvent.java \
	com/sun/jdi/event/VMDeathEvent.java \
	com/sun/jdi/event/VMDisconnectEvent.java \
	com/sun/jdi/event/VMStartEvent.java \
	com/sun/jdi/event/WatchpointEvent.java \
	com/sun/jdi/Field.java \
	com/sun/jdi/FloatType.java \
	com/sun/jdi/FloatValue.java \
	com/sun/jdi/IncompatibleThreadStateException.java \
	com/sun/jdi/InconsistentDebugInfoException.java \
	com/sun/jdi/IntegerType.java \
	com/sun/jdi/IntegerValue.java \
	com/sun/jdi/InterfaceType.java \
	com/sun/jdi/InternalException.java \
	com/sun/jdi/InvalidCodeIndexException.java \
	com/sun/jdi/InvalidLineNumberException.java \
	com/sun/jdi/InvalidStackFrameException.java \
	com/sun/jdi/InvalidTypeException.java \
	com/sun/jdi/InvocationException.java \
	com/sun/jdi/JDIPermission.java \
	com/sun/jdi/LocalVariable.java \
	com/sun/jdi/Locatable.java \
	com/sun/jdi/Location.java \
	com/sun/jdi/LongType.java \
	com/sun/jdi/LongValue.java \
	com/sun/jdi/Method.java \
	com/sun/jdi/Mirror.java \
	com/sun/jdi/MonitorInfo.java \
	com/sun/jdi/NativeMethodException.java \
	com/sun/jdi/ObjectCollectedException.java \
	com/sun/jdi/ObjectReference.java \
	com/sun/jdi/PathSearchingVirtualMachine.java \
	com/sun/jdi/PrimitiveType.java \
	com/sun/jdi/PrimitiveValue.java \
	com/sun/jdi/ReferenceType.java \
	com/sun/jdi/request/AccessWatchpointRequest.java \
	com/sun/jdi/request/BreakpointRequest.java \
	com/sun/jdi/request/ClassPrepareRequest.java \
	com/sun/jdi/request/ClassUnloadRequest.java \
	com/sun/jdi/request/DuplicateRequestException.java \
	com/sun/jdi/request/EventRequest.java \
	com/sun/jdi/request/EventRequestManager.java \
	com/sun/jdi/request/ExceptionRequest.java \
	com/sun/jdi/request/InvalidRequestStateException.java \
	com/sun/jdi/request/MethodEntryRequest.java \
	com/sun/jdi/request/MethodExitRequest.java \
	com/sun/jdi/request/ModificationWatchpointRequest.java \
	com/sun/jdi/request/MonitorContendedEnteredRequest.java \
	com/sun/jdi/request/MonitorContendedEnterRequest.java \
	com/sun/jdi/request/MonitorWaitedRequest.java \
	com/sun/jdi/request/MonitorWaitRequest.java \
	com/sun/jdi/request/StepRequest.java \
	com/sun/jdi/request/ThreadDeathRequest.java \
	com/sun/jdi/request/ThreadStartRequest.java \
	com/sun/jdi/request/VMDeathRequest.java \
	com/sun/jdi/request/WatchpointRequest.java \
	com/sun/jdi/ShortType.java \
	com/sun/jdi/ShortValue.java \
	com/sun/jdi/StackFrame.java \
	com/sun/jdi/StringReference.java \
	com/sun/jdi/ThreadGroupReference.java \
	com/sun/jdi/ThreadReference.java \
	com/sun/jdi/TypeComponent.java \
	com/sun/jdi/Type.java \
	com/sun/jdi/Value.java \
	com/sun/jdi/VirtualMachine.java \
	com/sun/jdi/VirtualMachineManager.java \
	com/sun/jdi/VMCannotBeModifiedException.java \
	com/sun/jdi/VMDisconnectedException.java \
	com/sun/jdi/VMMismatchException.java \
	com/sun/jdi/VMOutOfMemoryException.java \
	com/sun/jdi/VoidType.java \
	com/sun/jdi/VoidValue.java \
	com/sun/tools/jdi/LinkedHashMap.java \
	java/util/Observer.java \
	java/util/Timer.java \
	java/security/cert/CertPathValidator.java \
	javax/security/sasl/SaslException.java \
	javax/security/sasl/AuthenticationException.java \
	javax/security/auth/callback/NameCallback.java \
	javax/security/sasl/Sasl.java \
	javax/security/sasl/SaslServer.java \
	javax/security/sasl/SaslClient.java \
	javax/security/sasl/SaslClientFactory.java \
	javax/security/sasl/SaslServerFactory.java \
	javax/security/auth/callback/ChoiceCallback.java \
	javax/security/auth/callback/ConfirmationCallback.java \
	javax/security/auth/callback/LanguageCallback.java \
	javax/security/auth/callback/TextInputCallback.java \
	javax/security/auth/callback/TextOutputCallback.java \
	javax/net/ssl/ManagerFactoryParameters.java \
	javax/net/ssl/SSLSocket.java \
	javax/net/ssl/SSLServerSocketFactory.java \
	javax/net/ssl/SSLContextSpi.java \
	javax/net/ssl/TrustManagerFactorySpi.java \
	javax/net/ssl/SSLSocketFactory.java \
	javax/net/ssl/SSLSessionBindingEvent.java \
	javax/net/ssl/SSLSessionBindingListener.java \
	javax/net/ssl/X509TrustManager.java \
	javax/net/ssl/SSLServerSocket.java \
	javax/net/ssl/HostnameVerifier.java \
	javax/net/ssl/X509ExtendedKeyManager.java \
	javax/net/ssl/KeyManagerFactorySpi.java \
	javax/net/ssl/SSLException.java \
	javax/net/ssl/SSLProtocolException.java \
	javax/net/ssl/KeyManagerFactory.java \
	javax/net/ssl/TrustManagerFactory.java

if FOUND_ECJ
  JCOMPILER = $(ECJ) -nowarn
else
if FOUND_JAVAC
  JCOMPILER = $(JAVAC)
endif
endif

copy: extract
	mkdir -p $(ICEDTEA_COPY_DIRS)
	for f in $(ICEDTEA_COPY_SRC) ; \
	do \
	  cp -rf openjdk/j2se/src/share/classes/$$f rt/$$f ; \
	done

uncopy:
	for f in $(ICEDTEA_COPY_SRC) ; \
	do \
	  rm -f rt/$$f ; \
	done
	for d in $(ICEDTEA_COPY_DIRS) ; \
	do \
	  if test -d $$d ; \
	  then \
	    rmdir --ignore-fail-on-non-empty -p $(ICEDTEA_COPY_DIRS) ; \
	  fi ; \
	done

patch: extract
	for p in $(ICEDTEA_PATCHES) ; \
	do \
	  echo Checking $$p ; \
	  if $(PATCH) -p0 --dry-run -s -t -f < $$p ; \
	  then \
	    echo Applying $$p ; \
	    $(PATCH) -p0 < $$p ; \
	  else \
	    : ; \
	  fi ; \
	done

unpatch: unpatchecj
	for p in $(ICEDTEA_PATCHES) ; \
	do \
	  echo Checking $$p ; \
	  if $(PATCH) -p0 -R --dry-run -s -t -f < $$p ; \
	  then \
	    echo Reverting $$p ; \
	    $(PATCH) -p0 -R < $$p ; \
	  else \
	    : ; \
	  fi ; \
	done

patchecj: patch
	echo Checking $$p ; \
	if $(PATCH) -p0 --dry-run -s -t -f < $(ICEDTEA_ECJ_PATCH) ; \
	then \
	  echo Applying $$p ; \
	  $(PATCH) -p0 < $(ICEDTEA_ECJ_PATCH) ; \
	fi

unpatchecj:
	echo Checking $$p ; \
	if $(PATCH) -p0 -R --dry-run -s -t -f < $(ICEDTEA_ECJ_PATCH) ; \
	then \
	  echo Reverting $$p ; \
	  $(PATCH) -p0 -R < $(ICEDTEA_ECJ_PATCH) ; \
	fi

extract: download
	if test -d openjdk; \
	then \
	  : ; \
	else \
	  $(UNZIP) -q $(OPENJDK_SRC_ZIP) ; \
	fi

unextract:
	rm -rf openjdk

download:
	if echo "$(OPENJDK_MD5SUM)  $(OPENJDK_SRC_ZIP)" | $(MD5SUM) --check; \
	then \
	  : ; \
	else \
	  rm -f $(OPENJDK_SRC_ZIP) ; \
	  $(WGET) $(OPENJDK_URL)$(OPENJDK_SRC_ZIP) ; \
	fi

undownload:
	rm -f $(OPENJDK_SRC_ZIP)

clobber: extract
	$(MAKE) \
	  $(ICEDTEA_ENV) \
	  -C openjdk/control/make clobber

env:
	@echo 'export $(ICEDTEA_ENV)'

boot:
	mkdir -p bootstrap/jdk1.6.0/bin
	mkdir -p bootstrap/jdk1.6.0/lib/endorsed	
	if ! test -f bootstrap/jdk1.6.0/bin/java; \
	then \
		ln -s $(JAVA) bootstrap/jdk1.6.0/bin/java; \
	fi; \
	if ! test -f bootstrap/jdk1.6.0/bin/javah; \
	then \
		ln -s $(JAVAH) bootstrap/jdk1.6.0/bin/javah; \
	fi; \
	if ! test -f bootstrap/jdk1.6.0/bin/rmic; \
	then \
		ln -s $(RMIC) bootstrap/jdk1.6.0/bin/rmic; \
	fi; \
	if ! test -f bootstrap/jdk1.6.0/bin/jar; \
	then \
		ln -s $(JAR) bootstrap/jdk1.6.0/bin/jar; \
	fi; \
	if ! test -f bootstrap/jdk1.6.0/bin/javac; \
	then \
		cp javac bootstrap/jdk1.6.0/bin/javac; \
	fi; \
	if ! test -f bootstrap/jdk1.6.0/bin/javap; \
	then \
		cp javap bootstrap/jdk1.6.0/bin/javap; \
	fi;
	ln -sf $(XALAN2_JAR) bootstrap/jdk1.6.0/lib/endorsed/xalan-j2.jar
	ln -sf $(XALAN2_SERIALIZER_JAR) bootstrap/jdk1.6.0/lib/endorsed/xalan-j2-serializer.jar
	ln -sf $(XERCES2_JAR) bootstrap/jdk1.6.0/lib/endorsed/xerces-j2.jar

icedteaboot: all unboot
	cp -a openjdk/control/build/linux-$(BUILD_ARCH_DIR)/j2sdk-image \
	  bootstrap/jdk1.6.0

unboot:
	rm -rf bootstrap/jdk1.6.0

plugs: lib/rt/rt.jar lib/jce/jce.jar
	mkdir -p bootstrap/jdk1.7.0/jre/lib/$(INSTALL_ARCH_DIR)
	for l in libdcpr.so libjsoundalsa.so libjsound.so libt2k.so ; \
	do \
	  echo | $(CC) -shared -fPIC \
	    -o bootstrap/jdk1.7.0/jre/lib/$(INSTALL_ARCH_DIR)/$$l \
	    -Wl,-soname,libjawt.so -xc - ; \
	done
	mkdir -p bootstrap/jdk1.7.0/jre/lib/ext
	touch empty_file
	$(JAR) cf bootstrap/jdk1.7.0/jre/lib/ext/sunjce_provider.jar empty_file
	mkdir -p bootstrap/jdk1.7.0/jre/lib/security
	$(JAR) cf bootstrap/jdk1.7.0/jre/lib/security/local_policy.jar empty_file
	$(JAR) cf bootstrap/jdk1.7.0/jre/lib/security/US_export_policy.jar empty_file
	rm empty_file
	cp -f lib/rt/rt.jar bootstrap/jdk1.7.0/jre/lib
	cp -f lib/jce/jce.jar bootstrap/jdk1.7.0/jre/lib

unplugs:
	rm -rf bootstrap/jdk1.7.0

icedtea:
	$(MAKE) \
	  $(ICEDTEA_ENV) \
	  -C openjdk/control/make
	@echo "IcedTea is served:" openjdk/control/build/linux-$(BUILD_ARCH_DIR)

bootstrap: icedteaboot unpatchecj clobber
	$(MAKE) \
	  $(ICEDTEA_ENV) \
	  -C openjdk/control/make
	@echo "IcedTea re-served:" openjdk/control/build/linux-$(BUILD_ARCH_DIR)

all-local: boot plugs patch patchecj icedtea

clean-local:
	rm -rf lib
	rm -f rt-classes
	rm -f jce-classes
	rm -f generated/java/nio/*Exception.java
	rm -f generated/java/nio/channels/*Exception.java
	rm -f generated/java/nio/charset/*Exception.java
	rm -f generated/sun/nio/ch/*Exception.java
	rm -f generated/sun/util/resources/*
	rm -f generated/gen-srcs.lst

distclean-local: uncopy unplugs unboot unextract

lib/rt/rt.jar: rt-generate-sources rt-compile-classes copy
	cd lib/rt ; \
	  $(JAR) cf rt.jar com gnu java javax sun

rt-classes: copy rt-generate-sources
	cd rt ; \
	  find . -name '*.java' -print > ../rt-classes

rt-compile-classes: rt-classes
	mkdir -p lib/rt
	cd rt ; \
	  $(JCOMPILER) -g -d ../lib/rt -bootclasspath '' -source 1.6 \
	    -sourcepath $(OPENJDK_SRC_DIR)/j2se/src/share/classes:$(OPENJDK_SRC_DIR)/j2se/src/solaris/classes:../generated:../jce \
	    @../rt-classes

rt-generate-sources: patch
	@if test -e $(abs_top_srcdir)/generated/gen-srcs.lst; then \
	  while read line; do \
	    if ! test -e $$line; then \
        echo $$line not FOUND!; \
	      generate=true; \
	    fi; \
	  done < $(abs_top_srcdir)/generated/gen-srcs.lst; \
	else \
	  generate=true; \
	fi; \
	if ! test -z $$generate; then	\
		echo Regenerating sources...; \
	  mkdir -p generated/java/nio/channels; \
		mkdir -p generated/java/nio/charset; \
	  mkdir -p generated/sun/nio/ch; \
	  $(SHELL) $(OPENJDK_SRC_DIR)/j2se/make/java/nio/genExceptions.sh \
	    $(OPENJDK_SRC_DIR)/j2se/src/share/classes/java/nio/channels/exceptions \
	    generated/java/nio/channels; \
	  $(SHELL) $(OPENJDK_SRC_DIR)/j2se/make/java/nio/genExceptions.sh \
	    $(OPENJDK_SRC_DIR)/j2se/src/share/classes/java/nio/charset/exceptions \
	    generated/java/nio/charset; \
	  $(SHELL) $(OPENJDK_SRC_DIR)/j2se/make/java/nio/genExceptions.sh \
	    $(OPENJDK_SRC_DIR)/j2se/src/share/classes/java/nio/exceptions \
	    generated/java/nio; \
	  $(SHELL) $(OPENJDK_SRC_DIR)/j2se/make/java/nio/genExceptions.sh \
	    $(OPENJDK_SRC_DIR)/j2se/src/share/classes/sun/nio/ch/exceptions \
	    generated/sun/nio/ch; \
	  mkdir -p $(abs_top_srcdir)/lib; \
	  cd $(OPENJDK_SRC_DIR)/j2se/make/tools/CompileProperties; \
	    $(JCOMPILER) -g -d ../../../../../lib -target 1.5 -source 1.5 CompileProperties.java; \
	  mkdir -p $(abs_top_srcdir)/generated/sun/util/resources; \
	  cd $(abs_top_srcdir)/lib ; \
	    for FILE in `ls $(OPENJDK_SRC_DIR)/j2se/src/share/classes/sun/util/resources/*.properties`; do \
	      export TMPVAR=$${FILE/%.properties/.java} ; \
	      $(JAVA) -classpath . CompileProperties $$FILE \
	        ../generated/sun/util/resources/$${TMPVAR##*/} \
					LocaleNamesBundle ; \
	    done; \
	  find $(abs_top_srcdir)/generated -name *.java > \
		$(abs_top_srcdir)/generated/gen-srcs.lst; \
	else echo Generated sources found, continuing...; \
	fi;

jce-classes:
	cd jce ; \
	  find . -name '*.java' -print > ../jce-classes

jce-compile-classes: jce-classes rt-classes
	mkdir -p lib/jce
	cd jce ; \
	  $(JCOMPILER) -g -d ../lib/jce -source 1.6 -bootclasspath '' \
	    -classpath ../lib/rt @../jce-classes

lib/jce/jce.jar: jce-compile-classes
	cd lib/jce ; \
	  $(JAR) cf jce.jar gnu javax sun

install:

.PHONY: \
	copy uncopy patch unpatch patchecj unpatchecj extract unextract \
	download undownload clobber env boot icedteaboot unboot plugs unplugs \
	icedtea bootstrap rt-compile-classes rt-generate-sources \
	jce-compile-classes
