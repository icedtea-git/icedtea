OPENJDK_VERSION = b59

OPENJDK_CHANGESET = 0d76c4da605f
CORBA_CHANGESET = e9ba2b962ddf
JAXP_CHANGESET = 748976d69503
JAXWS_CHANGESET = faac46e2e60f
JDK_CHANGESET = 567b20871c5d
LANGTOOLS_CHANGESET = 0f653be1a42f
HOTSPOT_CHANGESET = aa0c48844632

OPENJDK_MD5SUM = 51beb96e5d6bc4b875b4316f4ef724d7
CORBA_MD5SUM = 13b64a402845a469cae31e04ffc5ee80
JAXP_MD5SUM = 18c922319545d7e907a2d6cbb265865d
JAXWS_MD5SUM = b2c0a779e349f82427a78485ef53321b
JDK_MD5SUM = cdd6f2d448f65e92a4349e2f83873278
LANGTOOLS_MD5SUM = ccad721f71ab49a61bbf684039b4d55d
HOTSPOT_MD5SUM = c40c25a481a1c33ac15a89812a0b9f8c

CACAO_VERSION = 0.99.4
CACAO_MD5SUM = 63220327925ace13756ae334c55a3baa

NETBEANS_PROFILER_MD5SUM = a6756a62291ebea5c0e2431389b31380
VISUALVM_MD5SUM = a289739f4a5bcdb9a2c642cfcc1e83fe

if ENABLE_NPPLUGIN
ICEDTEAPLUGIN_CLEAN = clean-IcedTeaNPPlugin
ICEDTEAPLUGIN_TARGET = IcedTeaNPPlugin.so
PLUGIN_PATCH = patches/icedtea-liveconnect.patch
LIVECONNECT_DIR = -C lib/rt netscape -C lib/rt net
else
if ENABLE_PLUGIN
ICEDTEAPLUGIN_CLEAN = clean-IcedTeaPlugin
ICEDTEAPLUGIN_TARGET = IcedTeaPlugin.so
PLUGIN_PATCH = patches/icedtea-liveconnect.patch
LIVECONNECT_DIR = -C lib/rt netscape -C lib/rt net
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
PULSE_JAVA_NATIVE_BUILDDIR = $(abs_top_builddir)/pulseaudio
PULSE_JAVA_JAVA_SRCDIR = $(PULSE_JAVA_DIR)/src/java
PULSE_JAVA_CLASS_DIR = $(abs_top_builddir)/pulseaudio
else 
PULSE_JAVA_DIR =
PULSE_JAVA_NATIVE_SRCDIR =
PULSE_JAVA_NATIVE_BUILDDIR = 
PULSE_JAVA_JAVA_SRCDIR =
PULSE_JAVA_CLASS_DIR =
endif

if ENABLE_NIMBUS
JIBX_DEPS_DIR=$(abs_top_builddir)/jibx
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
	rm -rf visualvm
	rm -rf netbeans
if ENABLE_PLUGIN
	rm -f IcedTeaPlugin.o IcedTeaPlugin.so
endif
if ENABLE_NPPLUGIN
	rm -f IcedTeaNPPlugin.o IcedTeaNPPlugin.so
endif
if BUILD_CACAO
	rm -rf cacao
endif
	rm -f jni-common.o
if ADD_ZERO_BUILD
	rm -rf zerovm
endif
if BUILD_NATIVE_ECJ
	rm -f native-ecj
endif
	if [ $(abs_top_srcdir) != $(abs_top_builddir) ]; then \
	  rm -rf $(abs_top_builddir)/generated ; \
	fi

install:

.PHONY: \
	env env-ecj snapshot clean-clone clean-patch \
	clean-clone-ecj clean-patch-ecj clean-bootstrap-directory \
	clean-bootstrap-directory-symlink clean-bootstrap-directory-ecj \
	clean-bootstrap-directory-symlink-ecj icedtea icedtea-debug \
	clean-icedtea icedtea-against-icedtea clean-icedtea-ecj clean-plugs \
	clean-tools-jar clean-shared-objects clean-visualvm clean-nbplatform \
	clean-copy clean-hotspot-tools clean-rt $(ICEDTEAPLUGIN_CLEAN) \
	$(ICEDTEANPPLUGIN_CLEAN) hotspot hotspot-helper clean-extra clean-jtreg \
	clean-jtreg-reports 

GENERATED_FILES = generated/com/sun/java/swing/plaf/gtk/resources/gtk_it.java \
	generated/com/sun/java/swing/plaf/gtk/resources/gtk_de.java \
	generated/com/sun/java/swing/plaf/gtk/resources/gtk_zh_CN.java \
	generated/com/sun/java/swing/plaf/gtk/resources/gtk_ko.java \
	generated/com/sun/java/swing/plaf/gtk/resources/gtk_es.java \
	generated/com/sun/java/swing/plaf/gtk/resources/gtk.java \
	generated/com/sun/java/swing/plaf/gtk/resources/gtk_sv.java \
	generated/com/sun/java/swing/plaf/gtk/resources/gtk_fr.java \
	generated/com/sun/java/swing/plaf/gtk/resources/gtk_zh_TW.java \
	generated/com/sun/java/swing/plaf/gtk/resources/gtk_zh_HK.java \
	generated/com/sun/java/swing/plaf/gtk/resources/gtk_ja.java \
	generated/com/sun/java/swing/plaf/motif/resources/motif_es.java \
	generated/com/sun/java/swing/plaf/motif/resources/motif_sv.java \
	generated/com/sun/java/swing/plaf/motif/resources/motif_fr.java \
	generated/com/sun/java/swing/plaf/motif/resources/motif_zh_CN.java \
	generated/com/sun/java/swing/plaf/motif/resources/motif_ja.java \
	generated/com/sun/java/swing/plaf/motif/resources/motif_it.java \
	generated/com/sun/java/swing/plaf/motif/resources/motif_de.java \
	generated/com/sun/java/swing/plaf/motif/resources/motif_zh_TW.java \
	generated/com/sun/java/swing/plaf/motif/resources/motif_ko.java \
	generated/com/sun/java/swing/plaf/motif/resources/motif_zh_HK.java \
	generated/com/sun/java/swing/plaf/motif/resources/motif.java \
	generated/com/sun/java/swing/plaf/windows/resources/windows_it.java \
	generated/com/sun/java/swing/plaf/windows/resources/windows_de.java \
	generated/com/sun/java/swing/plaf/windows/resources/windows_zh_CN.java \
	generated/com/sun/java/swing/plaf/windows/resources/windows.java \
	generated/com/sun/java/swing/plaf/windows/resources/windows_ko.java \
	generated/com/sun/java/swing/plaf/windows/resources/windows_es.java \
	generated/com/sun/java/swing/plaf/windows/resources/windows_sv.java \
	generated/com/sun/java/swing/plaf/windows/resources/windows_fr.java \
	generated/com/sun/java/swing/plaf/windows/resources/windows_zh_TW.java \
	generated/com/sun/java/swing/plaf/windows/resources/windows_zh_HK.java \
	generated/com/sun/java/swing/plaf/windows/resources/windows_ja.java \
	generated/com/sun/corba/se/spi/activation/ORBPortInfoHelper.java \
	generated/com/sun/corba/se/spi/activation/Server.java \
	generated/com/sun/corba/se/spi/activation/ServerIdHelper.java \
	generated/com/sun/corba/se/spi/activation/BadServerDefinition.java \
	generated/com/sun/corba/se/spi/activation/EndpointInfoListHelper.java \
	generated/com/sun/corba/se/spi/activation/ServerIdsHolder.java \
	generated/com/sun/corba/se/spi/activation/ServerNotRegisteredHelper.java \
	generated/com/sun/corba/se/spi/activation/_ServerStub.java \
	generated/com/sun/corba/se/spi/activation/ServerManagerHolder.java \
	generated/com/sun/corba/se/spi/activation/ActivatorOperations.java \
	generated/com/sun/corba/se/spi/activation/LocatorPackage/ServerLocationPerORBHelper.java \
	generated/com/sun/corba/se/spi/activation/LocatorPackage/ServerLocationPerORBHolder.java \
	generated/com/sun/corba/se/spi/activation/LocatorPackage/ServerLocation.java \
	generated/com/sun/corba/se/spi/activation/LocatorPackage/ServerLocationPerORB.java \
	generated/com/sun/corba/se/spi/activation/LocatorPackage/ServerLocationHelper.java \
	generated/com/sun/corba/se/spi/activation/LocatorPackage/ServerLocationHolder.java \
	generated/com/sun/corba/se/spi/activation/ServerAlreadyActiveHelper.java \
	generated/com/sun/corba/se/spi/activation/ServerHelper.java \
	generated/com/sun/corba/se/spi/activation/TCPPortHelper.java \
	generated/com/sun/corba/se/spi/activation/NoSuchEndPoint.java \
	generated/com/sun/corba/se/spi/activation/EndPointInfo.java \
	generated/com/sun/corba/se/spi/activation/_ServerManagerImplBase.java \
	generated/com/sun/corba/se/spi/activation/Repository.java \
	generated/com/sun/corba/se/spi/activation/BadServerDefinitionHolder.java \
	generated/com/sun/corba/se/spi/activation/ORBidListHolder.java \
	generated/com/sun/corba/se/spi/activation/ServerAlreadyInstalledHelper.java \
	generated/com/sun/corba/se/spi/activation/NoSuchEndPointHelper.java \
	generated/com/sun/corba/se/spi/activation/ORBPortInfoHolder.java \
	generated/com/sun/corba/se/spi/activation/ServerHeldDownHelper.java \
	generated/com/sun/corba/se/spi/activation/EndpointInfoListHolder.java \
	generated/com/sun/corba/se/spi/activation/ServerNotRegisteredHolder.java \
	generated/com/sun/corba/se/spi/activation/ORBPortInfoListHelper.java \
	generated/com/sun/corba/se/spi/activation/RepositoryOperations.java \
	generated/com/sun/corba/se/spi/activation/ServerNotActiveHelper.java \
	generated/com/sun/corba/se/spi/activation/_LocatorStub.java \
	generated/com/sun/corba/se/spi/activation/ServerAlreadyActiveHolder.java \
	generated/com/sun/corba/se/spi/activation/ServerHolder.java \
	generated/com/sun/corba/se/spi/activation/_ServerImplBase.java \
	generated/com/sun/corba/se/spi/activation/_InitialNameServiceStub.java \
	generated/com/sun/corba/se/spi/activation/ActivatorHelper.java \
	generated/com/sun/corba/se/spi/activation/ServerAlreadyRegisteredHelper.java \
	generated/com/sun/corba/se/spi/activation/EndPointInfoHelper.java \
	generated/com/sun/corba/se/spi/activation/ServerNotActive.java \
	generated/com/sun/corba/se/spi/activation/InitialNameServicePackage/NameAlreadyBoundHelper.java \
	generated/com/sun/corba/se/spi/activation/InitialNameServicePackage/NameAlreadyBoundHolder.java \
	generated/com/sun/corba/se/spi/activation/InitialNameServicePackage/NameAlreadyBound.java \
	generated/com/sun/corba/se/spi/activation/ServerAlreadyInstalledHolder.java \
	generated/com/sun/corba/se/spi/activation/NoSuchEndPointHolder.java \
	generated/com/sun/corba/se/spi/activation/_InitialNameServiceImplBase.java \
	generated/com/sun/corba/se/spi/activation/LocatorOperations.java \
	generated/com/sun/corba/se/spi/activation/_ActivatorStub.java \
	generated/com/sun/corba/se/spi/activation/ServerHeldDownHolder.java \
	generated/com/sun/corba/se/spi/activation/ServerAlreadyActive.java \
	generated/com/sun/corba/se/spi/activation/IIOP_CLEAR_TEXT.java \
	generated/com/sun/corba/se/spi/activation/ORBPortInfoListHolder.java \
	generated/com/sun/corba/se/spi/activation/ServerManagerOperations.java \
	generated/com/sun/corba/se/spi/activation/ServerNotActiveHolder.java \
	generated/com/sun/corba/se/spi/activation/ServerAlreadyRegistered.java \
	generated/com/sun/corba/se/spi/activation/ServerAlreadyUninstalledHelper.java \
	generated/com/sun/corba/se/spi/activation/_ServerManagerStub.java \
	generated/com/sun/corba/se/spi/activation/RepositoryPackage/StringSeqHelper.java \
	generated/com/sun/corba/se/spi/activation/RepositoryPackage/StringSeqHolder.java \
	generated/com/sun/corba/se/spi/activation/RepositoryPackage/ServerDef.java \
	generated/com/sun/corba/se/spi/activation/RepositoryPackage/ServerDefHelper.java \
	generated/com/sun/corba/se/spi/activation/RepositoryPackage/ServerDefHolder.java \
	generated/com/sun/corba/se/spi/activation/ActivatorHolder.java \
	generated/com/sun/corba/se/spi/activation/ServerAlreadyUninstalled.java \
	generated/com/sun/corba/se/spi/activation/InvalidORBidHelper.java \
	generated/com/sun/corba/se/spi/activation/ServerAlreadyRegisteredHolder.java \
	generated/com/sun/corba/se/spi/activation/EndPointInfoHolder.java \
	generated/com/sun/corba/se/spi/activation/Activator.java \
	generated/com/sun/corba/se/spi/activation/ServerManager.java \
	generated/com/sun/corba/se/spi/activation/ORBidHelper.java \
	generated/com/sun/corba/se/spi/activation/InitialNameServiceHelper.java \
	generated/com/sun/corba/se/spi/activation/ServerOperations.java \
	generated/com/sun/corba/se/spi/activation/RepositoryHelper.java \
	generated/com/sun/corba/se/spi/activation/ORBAlreadyRegisteredHelper.java \
	generated/com/sun/corba/se/spi/activation/LocatorHelper.java \
	generated/com/sun/corba/se/spi/activation/ServerAlreadyUninstalledHolder.java \
	generated/com/sun/corba/se/spi/activation/POANameHelper.java \
	generated/com/sun/corba/se/spi/activation/_RepositoryImplBase.java \
	generated/com/sun/corba/se/spi/activation/ServerIdsHelper.java \
	generated/com/sun/corba/se/spi/activation/_ActivatorImplBase.java \
	generated/com/sun/corba/se/spi/activation/InvalidORBidHolder.java \
	generated/com/sun/corba/se/spi/activation/ServerManagerHelper.java \
	generated/com/sun/corba/se/spi/activation/ServerHeldDown.java \
	generated/com/sun/corba/se/spi/activation/ORBPortInfo.java \
	generated/com/sun/corba/se/spi/activation/InitialNameServiceHolder.java \
	generated/com/sun/corba/se/spi/activation/InvalidORBid.java \
	generated/com/sun/corba/se/spi/activation/ServerAlreadyInstalled.java \
	generated/com/sun/corba/se/spi/activation/_LocatorImplBase.java \
	generated/com/sun/corba/se/spi/activation/InitialNameService.java \
	generated/com/sun/corba/se/spi/activation/ORBAlreadyRegistered.java \
	generated/com/sun/corba/se/spi/activation/RepositoryHolder.java \
	generated/com/sun/corba/se/spi/activation/ORBAlreadyRegisteredHolder.java \
	generated/com/sun/corba/se/spi/activation/LocatorHolder.java \
	generated/com/sun/corba/se/spi/activation/Locator.java \
	generated/com/sun/corba/se/spi/activation/InitialNameServiceOperations.java \
	generated/com/sun/corba/se/spi/activation/_RepositoryStub.java \
	generated/com/sun/corba/se/spi/activation/BadServerDefinitionHelper.java \
	generated/com/sun/corba/se/spi/activation/ORBidListHelper.java \
	generated/com/sun/corba/se/spi/activation/POANameHolder.java \
	generated/com/sun/corba/se/spi/activation/ServerNotRegistered.java \
	generated/com/sun/corba/se/impl/logging/InterceptorsSystemException.resource \
	generated/com/sun/corba/se/impl/logging/ActivationSystemException.java \
	generated/com/sun/corba/se/impl/logging/LogStrings.properties \
	generated/com/sun/corba/se/impl/logging/IORSystemException.resource \
	generated/com/sun/corba/se/impl/logging/UtilSystemException.resource \
	generated/com/sun/corba/se/impl/logging/NamingSystemException.resource \
	generated/com/sun/corba/se/impl/logging/ORBUtilSystemException.java \
	generated/com/sun/corba/se/impl/logging/InterceptorsSystemException.java \
	generated/com/sun/corba/se/impl/logging/ActivationSystemException.resource \
	generated/com/sun/corba/se/impl/logging/POASystemException.java \
	generated/com/sun/corba/se/impl/logging/IORSystemException.java \
	generated/com/sun/corba/se/impl/logging/POASystemException.resource \
	generated/com/sun/corba/se/impl/logging/OMGSystemException.java \
	generated/com/sun/corba/se/impl/logging/ORBUtilSystemException.resource \
	generated/com/sun/corba/se/impl/logging/NamingSystemException.java \
	generated/com/sun/corba/se/impl/logging/UtilSystemException.java \
	generated/com/sun/corba/se/impl/logging/OMGSystemException.resource \
	generated/com/sun/corba/se/PortableActivationIDL/ORBProxyHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/ORBPortInfoHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/BadServerDefinition.java \
	generated/com/sun/corba/se/PortableActivationIDL/EndpointInfoListHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerIdsHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerNotRegisteredHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerManagerHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/ActivatorOperations.java \
	generated/com/sun/corba/se/PortableActivationIDL/LocatorPackage/ServerLocationPerTypeHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/LocatorPackage/ServerLocationPerORBHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/LocatorPackage/ServerLocationPerTypeHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/LocatorPackage/ServerLocationPerORBHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/LocatorPackage/ServerLocationPerType.java \
	generated/com/sun/corba/se/PortableActivationIDL/LocatorPackage/ServerLocationPerORB.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerAlreadyActiveHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/TCPPortHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerProxyOperations.java \
	generated/com/sun/corba/se/PortableActivationIDL/NoSuchEndPoint.java \
	generated/com/sun/corba/se/PortableActivationIDL/EndPointInfo.java \
	generated/com/sun/corba/se/PortableActivationIDL/_ServerManagerImplBase.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerProxyHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/Repository.java \
	generated/com/sun/corba/se/PortableActivationIDL/BadServerDefinitionHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/ORBidListHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerAlreadyInstalledHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/NoSuchEndPointHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/ORBProxyHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/ORBPortInfoHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerHeldDownHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/_ORBProxyStub.java \
	generated/com/sun/corba/se/PortableActivationIDL/EndpointInfoListHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerNotRegisteredHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/ORBPortInfoListHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/RepositoryOperations.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerNotActiveHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/_LocatorStub.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerAlreadyActiveHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/_ServerProxyStub.java \
	generated/com/sun/corba/se/PortableActivationIDL/_InitialNameServiceStub.java \
	generated/com/sun/corba/se/PortableActivationIDL/ActivatorHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerAlreadyRegisteredHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/EndPointInfoHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerNotActive.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerProxyHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/InitialNameServicePackage/NameAlreadyBoundHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/InitialNameServicePackage/NameAlreadyBoundHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/InitialNameServicePackage/NameAlreadyBound.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerAlreadyInstalledHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/NoSuchEndPointHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/_InitialNameServiceImplBase.java \
	generated/com/sun/corba/se/PortableActivationIDL/LocatorOperations.java \
	generated/com/sun/corba/se/PortableActivationIDL/_ActivatorStub.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerHeldDownHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerAlreadyActive.java \
	generated/com/sun/corba/se/PortableActivationIDL/IIOP_CLEAR_TEXT.java \
	generated/com/sun/corba/se/PortableActivationIDL/ORBPortInfoListHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerManagerOperations.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerNotActiveHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerAlreadyRegistered.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerAlreadyUninstalledHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/_ServerManagerStub.java \
	generated/com/sun/corba/se/PortableActivationIDL/_ORBProxyImplBase.java \
	generated/com/sun/corba/se/PortableActivationIDL/RepositoryPackage/ServerDef.java \
	generated/com/sun/corba/se/PortableActivationIDL/RepositoryPackage/ServerDefHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/RepositoryPackage/ServerDefHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/RepositoryPackage/AppNamesHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/RepositoryPackage/AppNamesHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/ActivatorHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerAlreadyUninstalled.java \
	generated/com/sun/corba/se/PortableActivationIDL/InvalidORBidHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerAlreadyRegisteredHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/EndPointInfoHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/Activator.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerManager.java \
	generated/com/sun/corba/se/PortableActivationIDL/InitialNameServiceHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/ORBProxy.java \
	generated/com/sun/corba/se/PortableActivationIDL/ORBProxyOperations.java \
	generated/com/sun/corba/se/PortableActivationIDL/RepositoryHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/ORBAlreadyRegisteredHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/LocatorHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerAlreadyUninstalledHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/_ServerProxyImplBase.java \
	generated/com/sun/corba/se/PortableActivationIDL/_RepositoryImplBase.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerIdsHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/_ActivatorImplBase.java \
	generated/com/sun/corba/se/PortableActivationIDL/InvalidORBidHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerManagerHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerHeldDown.java \
	generated/com/sun/corba/se/PortableActivationIDL/ORBPortInfo.java \
	generated/com/sun/corba/se/PortableActivationIDL/InitialNameServiceHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/InvalidORBid.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerAlreadyInstalled.java \
	generated/com/sun/corba/se/PortableActivationIDL/_LocatorImplBase.java \
	generated/com/sun/corba/se/PortableActivationIDL/InitialNameService.java \
	generated/com/sun/corba/se/PortableActivationIDL/ORBAlreadyRegistered.java \
	generated/com/sun/corba/se/PortableActivationIDL/RepositoryHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/ORBAlreadyRegisteredHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/LocatorHolder.java \
	generated/com/sun/corba/se/PortableActivationIDL/Locator.java \
	generated/com/sun/corba/se/PortableActivationIDL/InitialNameServiceOperations.java \
	generated/com/sun/corba/se/PortableActivationIDL/_RepositoryStub.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerProxy.java \
	generated/com/sun/corba/se/PortableActivationIDL/BadServerDefinitionHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/ORBidListHelper.java \
	generated/com/sun/corba/se/PortableActivationIDL/ServerNotRegistered.java \
	generated/com/sun/swing/internal/plaf/basic/resources/basic_ko.java \
	generated/com/sun/swing/internal/plaf/basic/resources/basic_zh_TW.java \
	generated/com/sun/swing/internal/plaf/basic/resources/basic_es.java \
	generated/com/sun/swing/internal/plaf/basic/resources/basic_zh_HK.java \
	generated/com/sun/swing/internal/plaf/basic/resources/basic_sv.java \
	generated/com/sun/swing/internal/plaf/basic/resources/basic_fr.java \
	generated/com/sun/swing/internal/plaf/basic/resources/basic_zh_CN.java \
	generated/com/sun/swing/internal/plaf/basic/resources/basic.java \
	generated/com/sun/swing/internal/plaf/basic/resources/basic_ja.java \
	generated/com/sun/swing/internal/plaf/basic/resources/basic_it.java \
	generated/com/sun/swing/internal/plaf/basic/resources/basic_de.java \
	generated/com/sun/swing/internal/plaf/metal/resources/metal_ko.java \
	generated/com/sun/swing/internal/plaf/metal/resources/metal.java \
	generated/com/sun/swing/internal/plaf/metal/resources/metal_zh_CN.java \
	generated/com/sun/swing/internal/plaf/metal/resources/metal_es.java \
	generated/com/sun/swing/internal/plaf/metal/resources/metal_sv.java \
	generated/com/sun/swing/internal/plaf/metal/resources/metal_fr.java \
	generated/com/sun/swing/internal/plaf/metal/resources/metal_ja.java \
	generated/com/sun/swing/internal/plaf/metal/resources/metal_zh_TW.java \
	generated/com/sun/swing/internal/plaf/metal/resources/metal_it.java \
	generated/com/sun/swing/internal/plaf/metal/resources/metal_zh_HK.java \
	generated/com/sun/swing/internal/plaf/metal/resources/metal_de.java \
	generated/com/sun/swing/internal/plaf/synth/resources/synth_zh_CN.java \
	generated/com/sun/swing/internal/plaf/synth/resources/synth_ja.java \
	generated/com/sun/swing/internal/plaf/synth/resources/synth.java \
	generated/com/sun/swing/internal/plaf/synth/resources/synth_it.java \
	generated/com/sun/swing/internal/plaf/synth/resources/synth_de.java \
	generated/com/sun/swing/internal/plaf/synth/resources/synth_ko.java \
	generated/com/sun/swing/internal/plaf/synth/resources/synth_zh_TW.java \
	generated/com/sun/swing/internal/plaf/synth/resources/synth_es.java \
	generated/com/sun/swing/internal/plaf/synth/resources/synth_zh_HK.java \
	generated/com/sun/swing/internal/plaf/synth/resources/synth_sv.java \
	generated/com/sun/swing/internal/plaf/synth/resources/synth_fr.java \
	generated/com/sun/tools/apt/resources/apt_zh_CN.java \
	generated/com/sun/tools/apt/resources/apt.java \
	generated/com/sun/tools/apt/resources/apt_ja.java \
	generated/com/sun/tools/jdi/JDWP.java \
	generated/com/sun/tools/jdi/resources/jdi_zh_CN.java \
	generated/com/sun/tools/jdi/resources/jdi.java \
	generated/com/sun/tools/jdi/resources/jdi_ja.java \
	generated/com/sun/tools/javac/resources/version.java \
	generated/com/sun/tools/javac/resources/legacy.java \
	generated/com/sun/tools/javac/resources/javac.java \
	generated/com/sun/tools/javac/resources/compiler_ja.java \
	generated/com/sun/tools/javac/resources/compiler_zh_CN.java \
	generated/com/sun/tools/javac/resources/javac_zh_CN.java \
	generated/com/sun/tools/javac/resources/compiler.java \
	generated/com/sun/tools/javac/resources/javac_ja.java \
	generated/com/sun/tools/doclets/formats/html/resources/standard.java \
	generated/com/sun/tools/doclets/formats/html/resources/standard_ja.java \
	generated/com/sun/tools/doclets/formats/html/resources/standard_zh_CN.java \
	generated/com/sun/tools/doclets/internal/toolkit/resources/doclets.java \
	generated/com/sun/tools/doclets/internal/toolkit/resources/doclets_zh_CN.java \
	generated/com/sun/tools/doclets/internal/toolkit/resources/doclets_ja.java \
	generated/com/sun/tools/javadoc/resources/javadoc_ja.java \
	generated/com/sun/tools/javadoc/resources/javadoc_zh_CN.java \
	generated/com/sun/tools/javadoc/resources/javadoc.java \
	generated/com/sun/accessibility/internal/resources/accessibility_zh_HK.java \
	generated/com/sun/accessibility/internal/resources/accessibility_zh_CN.java \
	generated/com/sun/accessibility/internal/resources/accessibility_ja.java \
	generated/com/sun/accessibility/internal/resources/accessibility_it.java \
	generated/com/sun/accessibility/internal/resources/accessibility_de.java \
	generated/com/sun/accessibility/internal/resources/accessibility_ko.java \
	generated/com/sun/accessibility/internal/resources/accessibility_en.java \
	generated/com/sun/accessibility/internal/resources/accessibility.java \
	generated/com/sun/accessibility/internal/resources/accessibility_es.java \
	generated/com/sun/accessibility/internal/resources/accessibility_zh_TW.java \
	generated/com/sun/accessibility/internal/resources/accessibility_sv.java \
	generated/com/sun/accessibility/internal/resources/accessibility_fr.java \
	generated/org/omg/IOP/TaggedComponent.java \
	generated/org/omg/IOP/Codec.java \
	generated/org/omg/IOP/IORHelper.java \
	generated/org/omg/IOP/MultipleComponentProfileHolder.java \
	generated/org/omg/IOP/CodeSets.java \
	generated/org/omg/IOP/CodecFactory.java \
	generated/org/omg/IOP/TaggedProfileHelper.java \
	generated/org/omg/IOP/TAG_RMI_CUSTOM_MAX_STREAM_FORMAT.java \
	generated/org/omg/IOP/ServiceContextHelper.java \
	generated/org/omg/IOP/CodecFactoryHelper.java \
	generated/org/omg/IOP/CodecOperations.java \
	generated/org/omg/IOP/IORHolder.java \
	generated/org/omg/IOP/Encoding.java \
	generated/org/omg/IOP/ServiceContext.java \
	generated/org/omg/IOP/TAG_MULTIPLE_COMPONENTS.java \
	generated/org/omg/IOP/TAG_CODE_SETS.java \
	generated/org/omg/IOP/TaggedProfileHolder.java \
	generated/org/omg/IOP/ServiceIdHelper.java \
	generated/org/omg/IOP/ServiceContextHolder.java \
	generated/org/omg/IOP/TransactionService.java \
	generated/org/omg/IOP/CodecPackage/InvalidTypeForEncoding.java \
	generated/org/omg/IOP/CodecPackage/FormatMismatch.java \
	generated/org/omg/IOP/CodecPackage/FormatMismatchHelper.java \
	generated/org/omg/IOP/CodecPackage/InvalidTypeForEncodingHelper.java \
	generated/org/omg/IOP/CodecPackage/TypeMismatchHelper.java \
	generated/org/omg/IOP/CodecPackage/TypeMismatch.java \
	generated/org/omg/IOP/TAG_INTERNET_IOP.java \
	generated/org/omg/IOP/TAG_ORB_TYPE.java \
	generated/org/omg/IOP/TaggedComponentHelper.java \
	generated/org/omg/IOP/TAG_JAVA_CODEBASE.java \
	generated/org/omg/IOP/ProfileIdHelper.java \
	generated/org/omg/IOP/IOR.java \
	generated/org/omg/IOP/ServiceContextListHelper.java \
	generated/org/omg/IOP/RMICustomMaxStreamFormat.java \
	generated/org/omg/IOP/CodecFactoryPackage/UnknownEncoding.java \
	generated/org/omg/IOP/CodecFactoryPackage/UnknownEncodingHelper.java \
	generated/org/omg/IOP/TaggedComponentHolder.java \
	generated/org/omg/IOP/MultipleComponentProfileHelper.java \
	generated/org/omg/IOP/ENCODING_CDR_ENCAPS.java \
	generated/org/omg/IOP/TaggedProfile.java \
	generated/org/omg/IOP/TAG_ALTERNATE_IIOP_ADDRESS.java \
	generated/org/omg/IOP/TAG_POLICIES.java \
	generated/org/omg/IOP/ServiceContextListHolder.java \
	generated/org/omg/IOP/ExceptionDetailMessage.java \
	generated/org/omg/IOP/ComponentIdHelper.java \
	generated/org/omg/IOP/CodecFactoryOperations.java \
	generated/org/omg/CORBA/WStringSeqHelper.java \
	generated/org/omg/CORBA/StringSeqHelper.java \
	generated/org/omg/CORBA/WStringSeqHolder.java \
	generated/org/omg/CORBA/StringSeqHolder.java \
	generated/org/omg/CORBA/PolicyErrorHelper.java \
	generated/org/omg/CORBA/ParameterModeHelper.java \
	generated/org/omg/CORBA/ParameterMode.java \
	generated/org/omg/CORBA/PolicyErrorHolder.java \
	generated/org/omg/CORBA/ParameterModeHolder.java \
	generated/org/omg/CORBA/PolicyErrorCodeHelper.java \
	generated/org/omg/PortableInterceptor/ServerIdHelper.java \
	generated/org/omg/PortableInterceptor/IORInterceptor_3_0Operations.java \
	generated/org/omg/PortableInterceptor/PolicyFactoryOperations.java \
	generated/org/omg/PortableInterceptor/ObjectReferenceFactory.java \
	generated/org/omg/PortableInterceptor/ACTIVE.java \
	generated/org/omg/PortableInterceptor/CurrentHelper.java \
	generated/org/omg/PortableInterceptor/IORInfo.java \
	generated/org/omg/PortableInterceptor/ServerRequestInterceptorOperations.java \
	generated/org/omg/PortableInterceptor/AdapterStateHelper.java \
	generated/org/omg/PortableInterceptor/ClientRequestInfoOperations.java \
	generated/org/omg/PortableInterceptor/ORBInitInfoPackage/DuplicateNameHelper.java \
	generated/org/omg/PortableInterceptor/ORBInitInfoPackage/InvalidNameHelper.java \
	generated/org/omg/PortableInterceptor/ORBInitInfoPackage/DuplicateName.java \
	generated/org/omg/PortableInterceptor/ORBInitInfoPackage/ObjectIdHelper.java \
	generated/org/omg/PortableInterceptor/ORBInitInfoPackage/InvalidName.java \
	generated/org/omg/PortableInterceptor/RequestInfo.java \
	generated/org/omg/PortableInterceptor/ForwardRequest.java \
	generated/org/omg/PortableInterceptor/IORInfoOperations.java \
	generated/org/omg/PortableInterceptor/PolicyFactory.java \
	generated/org/omg/PortableInterceptor/ServerRequestInterceptor.java \
	generated/org/omg/PortableInterceptor/CurrentOperations.java \
	generated/org/omg/PortableInterceptor/IORInterceptor.java \
	generated/org/omg/PortableInterceptor/IORInterceptorOperations.java \
	generated/org/omg/PortableInterceptor/HOLDING.java \
	generated/org/omg/PortableInterceptor/ObjectReferenceFactoryHelper.java \
	generated/org/omg/PortableInterceptor/ServerRequestInfoOperations.java \
	generated/org/omg/PortableInterceptor/DISCARDING.java \
	generated/org/omg/PortableInterceptor/ForwardRequestHelper.java \
	generated/org/omg/PortableInterceptor/ORBInitializerOperations.java \
	generated/org/omg/PortableInterceptor/USER_EXCEPTION.java \
	generated/org/omg/PortableInterceptor/ObjectReferenceTemplateSeqHelper.java \
	generated/org/omg/PortableInterceptor/SUCCESSFUL.java \
	generated/org/omg/PortableInterceptor/ORBInitializer.java \
	generated/org/omg/PortableInterceptor/ORBInitInfoOperations.java \
	generated/org/omg/PortableInterceptor/ObjectReferenceTemplateHelper.java \
	generated/org/omg/PortableInterceptor/ClientRequestInterceptor.java \
	generated/org/omg/PortableInterceptor/INACTIVE.java \
	generated/org/omg/PortableInterceptor/ClientRequestInterceptorOperations.java \
	generated/org/omg/PortableInterceptor/ObjectReferenceFactoryHolder.java \
	generated/org/omg/PortableInterceptor/ServerRequestInfo.java \
	generated/org/omg/PortableInterceptor/AdapterNameHelper.java \
	generated/org/omg/PortableInterceptor/SYSTEM_EXCEPTION.java \
	generated/org/omg/PortableInterceptor/LOCATION_FORWARD.java \
	generated/org/omg/PortableInterceptor/IORInterceptor_3_0Helper.java \
	generated/org/omg/PortableInterceptor/AdapterManagerIdHelper.java \
	generated/org/omg/PortableInterceptor/Current.java \
	generated/org/omg/PortableInterceptor/ORBInitInfo.java \
	generated/org/omg/PortableInterceptor/ObjectReferenceTemplateSeqHolder.java \
	generated/org/omg/PortableInterceptor/ClientRequestInfo.java \
	generated/org/omg/PortableInterceptor/ObjectReferenceTemplateHolder.java \
	generated/org/omg/PortableInterceptor/ObjectIdHelper.java \
	generated/org/omg/PortableInterceptor/NON_EXISTENT.java \
	generated/org/omg/PortableInterceptor/Interceptor.java \
	generated/org/omg/PortableInterceptor/InvalidSlot.java \
	generated/org/omg/PortableInterceptor/IORInterceptor_3_0.java \
	generated/org/omg/PortableInterceptor/IORInterceptor_3_0Holder.java \
	generated/org/omg/PortableInterceptor/RequestInfoOperations.java \
	generated/org/omg/PortableInterceptor/UNKNOWN.java \
	generated/org/omg/PortableInterceptor/InterceptorOperations.java \
	generated/org/omg/PortableInterceptor/ORBIdHelper.java \
	generated/org/omg/PortableInterceptor/InvalidSlotHelper.java \
	generated/org/omg/PortableInterceptor/ObjectReferenceTemplate.java \
	generated/org/omg/PortableInterceptor/TRANSPORT_RETRY.java \
	generated/org/omg/CosNaming/BindingListHelper.java \
	generated/org/omg/CosNaming/BindingTypeHolder.java \
	generated/org/omg/CosNaming/NameHolder.java \
	generated/org/omg/CosNaming/NamingContextExt.java \
	generated/org/omg/CosNaming/NamingContextPOA.java \
	generated/org/omg/CosNaming/BindingType.java \
	generated/org/omg/CosNaming/NamingContextExtHelper.java \
	generated/org/omg/CosNaming/_BindingIteratorStub.java \
	generated/org/omg/CosNaming/BindingIterator.java \
	generated/org/omg/CosNaming/BindingListHolder.java \
	generated/org/omg/CosNaming/_NamingContextStub.java \
	generated/org/omg/CosNaming/NamingContextExtHolder.java \
	generated/org/omg/CosNaming/IstringHelper.java \
	generated/org/omg/CosNaming/NameComponentHelper.java \
	generated/org/omg/CosNaming/BindingIteratorPOA.java \
	generated/org/omg/CosNaming/NamingContext.java \
	generated/org/omg/CosNaming/_NamingContextExtStub.java \
	generated/org/omg/CosNaming/NamingContextExtOperations.java \
	generated/org/omg/CosNaming/BindingHelper.java \
	generated/org/omg/CosNaming/BindingIteratorHelper.java \
	generated/org/omg/CosNaming/NamingContextHelper.java \
	generated/org/omg/CosNaming/NamingContextExtPOA.java \
	generated/org/omg/CosNaming/NameComponentHolder.java \
	generated/org/omg/CosNaming/BindingIteratorOperations.java \
	generated/org/omg/CosNaming/BindingTypeHelper.java \
	generated/org/omg/CosNaming/NameHelper.java \
	generated/org/omg/CosNaming/NamingContextPackage/NotFoundReasonHolder.java \
	generated/org/omg/CosNaming/NamingContextPackage/CannotProceed.java \
	generated/org/omg/CosNaming/NamingContextPackage/NotFoundHelper.java \
	generated/org/omg/CosNaming/NamingContextPackage/AlreadyBound.java \
	generated/org/omg/CosNaming/NamingContextPackage/AlreadyBoundHelper.java \
	generated/org/omg/CosNaming/NamingContextPackage/NotFound.java \
	generated/org/omg/CosNaming/NamingContextPackage/NotFoundReason.java \
	generated/org/omg/CosNaming/NamingContextPackage/NotFoundHolder.java \
	generated/org/omg/CosNaming/NamingContextPackage/AlreadyBoundHolder.java \
	generated/org/omg/CosNaming/NamingContextPackage/CannotProceedHelper.java \
	generated/org/omg/CosNaming/NamingContextPackage/NotEmptyHelper.java \
	generated/org/omg/CosNaming/NamingContextPackage/InvalidNameHelper.java \
	generated/org/omg/CosNaming/NamingContextPackage/CannotProceedHolder.java \
	generated/org/omg/CosNaming/NamingContextPackage/InvalidName.java \
	generated/org/omg/CosNaming/NamingContextPackage/NotEmptyHolder.java \
	generated/org/omg/CosNaming/NamingContextPackage/InvalidNameHolder.java \
	generated/org/omg/CosNaming/NamingContextPackage/NotFoundReasonHelper.java \
	generated/org/omg/CosNaming/NamingContextPackage/NotEmpty.java \
	generated/org/omg/CosNaming/NamingContextOperations.java \
	generated/org/omg/CosNaming/Binding.java \
	generated/org/omg/CosNaming/NamingContextExtPackage/AddressHelper.java \
	generated/org/omg/CosNaming/NamingContextExtPackage/InvalidAddressHolder.java \
	generated/org/omg/CosNaming/NamingContextExtPackage/URLStringHelper.java \
	generated/org/omg/CosNaming/NamingContextExtPackage/InvalidAddress.java \
	generated/org/omg/CosNaming/NamingContextExtPackage/StringNameHelper.java \
	generated/org/omg/CosNaming/NamingContextExtPackage/InvalidAddressHelper.java \
	generated/org/omg/CosNaming/BindingHolder.java \
	generated/org/omg/CosNaming/BindingIteratorHolder.java \
	generated/org/omg/CosNaming/NamingContextHolder.java \
	generated/org/omg/CosNaming/NameComponent.java \
	generated/org/omg/Messaging/SYNC_WITH_TRANSPORT.java \
	generated/org/omg/Messaging/SyncScopeHelper.java \
	generated/org/omg/Dynamic/Parameter.java \
	generated/org/omg/DynamicAny/DynArrayHelper.java \
	generated/org/omg/DynamicAny/DynSequenceOperations.java \
	generated/org/omg/DynamicAny/DynStructHelper.java \
	generated/org/omg/DynamicAny/DynEnum.java \
	generated/org/omg/DynamicAny/DynAnySeqHelper.java \
	generated/org/omg/DynamicAny/DynFixed.java \
	generated/org/omg/DynamicAny/DynStruct.java \
	generated/org/omg/DynamicAny/DynArray.java \
	generated/org/omg/DynamicAny/_DynSequenceStub.java \
	generated/org/omg/DynamicAny/DynSequence.java \
	generated/org/omg/DynamicAny/DynFixedHelper.java \
	generated/org/omg/DynamicAny/NameValuePair.java \
	generated/org/omg/DynamicAny/DynAny.java \
	generated/org/omg/DynamicAny/DynEnumOperations.java \
	generated/org/omg/DynamicAny/DynAnyPackage/InvalidValueHelper.java \
	generated/org/omg/DynamicAny/DynAnyPackage/InvalidValue.java \
	generated/org/omg/DynamicAny/DynAnyPackage/TypeMismatchHelper.java \
	generated/org/omg/DynamicAny/DynAnyPackage/TypeMismatch.java \
	generated/org/omg/DynamicAny/DynUnion.java \
	generated/org/omg/DynamicAny/_DynValueStub.java \
	generated/org/omg/DynamicAny/FieldNameHelper.java \
	generated/org/omg/DynamicAny/NameValuePairSeqHelper.java \
	generated/org/omg/DynamicAny/DynValueCommonOperations.java \
	generated/org/omg/DynamicAny/DynUnionHelper.java \
	generated/org/omg/DynamicAny/_DynUnionStub.java \
	generated/org/omg/DynamicAny/NameDynAnyPair.java \
	generated/org/omg/DynamicAny/DynValueBox.java \
	generated/org/omg/DynamicAny/DynSequenceHelper.java \
	generated/org/omg/DynamicAny/_DynFixedStub.java \
	generated/org/omg/DynamicAny/DynArrayOperations.java \
	generated/org/omg/DynamicAny/DynValueCommon.java \
	generated/org/omg/DynamicAny/DynAnyFactoryOperations.java \
	generated/org/omg/DynamicAny/DynValueBoxOperations.java \
	generated/org/omg/DynamicAny/DynAnyFactory.java \
	generated/org/omg/DynamicAny/_DynArrayStub.java \
	generated/org/omg/DynamicAny/NameDynAnyPairHelper.java \
	generated/org/omg/DynamicAny/DynUnionOperations.java \
	generated/org/omg/DynamicAny/DynValueHelper.java \
	generated/org/omg/DynamicAny/DynAnyOperations.java \
	generated/org/omg/DynamicAny/_DynAnyStub.java \
	generated/org/omg/DynamicAny/DynFixedOperations.java \
	generated/org/omg/DynamicAny/DynValue.java \
	generated/org/omg/DynamicAny/_DynStructStub.java \
	generated/org/omg/DynamicAny/NameValuePairHelper.java \
	generated/org/omg/DynamicAny/NameDynAnyPairSeqHelper.java \
	generated/org/omg/DynamicAny/DynAnyFactoryPackage/InconsistentTypeCode.java \
	generated/org/omg/DynamicAny/DynAnyFactoryPackage/InconsistentTypeCodeHelper.java \
	generated/org/omg/DynamicAny/AnySeqHelper.java \
	generated/org/omg/DynamicAny/_DynAnyFactoryStub.java \
	generated/org/omg/DynamicAny/_DynEnumStub.java \
	generated/org/omg/DynamicAny/DynAnyHelper.java \
	generated/org/omg/DynamicAny/DynStructOperations.java \
	generated/org/omg/DynamicAny/DynValueOperations.java \
	generated/org/omg/DynamicAny/DynAnyFactoryHelper.java \
	generated/org/omg/DynamicAny/DynEnumHelper.java \
	generated/org/omg/PortableServer/ThreadPolicy.java \
	generated/org/omg/PortableServer/LifespanPolicyValue.java \
	generated/org/omg/PortableServer/_ServantLocatorStub.java \
	generated/org/omg/PortableServer/AdapterActivator.java \
	generated/org/omg/PortableServer/ForwardRequest.java \
	generated/org/omg/PortableServer/CurrentPackage/NoContext.java \
	generated/org/omg/PortableServer/CurrentPackage/NoContextHelper.java \
	generated/org/omg/PortableServer/ServantManager.java \
	generated/org/omg/PortableServer/IdAssignmentPolicy.java \
	generated/org/omg/PortableServer/_ServantActivatorStub.java \
	generated/org/omg/PortableServer/IdAssignmentPolicyOperations.java \
	generated/org/omg/PortableServer/LifespanPolicyOperations.java \
	generated/org/omg/PortableServer/CurrentOperations.java \
	generated/org/omg/PortableServer/POAManager.java \
	generated/org/omg/PortableServer/ServantLocatorOperations.java \
	generated/org/omg/PortableServer/ForwardRequestHelper.java \
	generated/org/omg/PortableServer/ServantRetentionPolicy.java \
	generated/org/omg/PortableServer/ServantRetentionPolicyValue.java \
	generated/org/omg/PortableServer/RequestProcessingPolicyOperations.java \
	generated/org/omg/PortableServer/ImplicitActivationPolicyValue.java \
	generated/org/omg/PortableServer/ServantManagerOperations.java \
	generated/org/omg/PortableServer/ServantLocatorPOA.java \
	generated/org/omg/PortableServer/ServantActivatorHelper.java \
	generated/org/omg/PortableServer/ImplicitActivationPolicy.java \
	generated/org/omg/PortableServer/SERVANT_RETENTION_POLICY_ID.java \
	generated/org/omg/PortableServer/REQUEST_PROCESSING_POLICY_ID.java \
	generated/org/omg/PortableServer/THREAD_POLICY_ID.java \
	generated/org/omg/PortableServer/Current.java \
	generated/org/omg/PortableServer/ServantActivatorOperations.java \
	generated/org/omg/PortableServer/LIFESPAN_POLICY_ID.java \
	generated/org/omg/PortableServer/POAOperations.java \
	generated/org/omg/PortableServer/POAManagerPackage/AdapterInactiveHelper.java \
	generated/org/omg/PortableServer/POAManagerPackage/State.java \
	generated/org/omg/PortableServer/POAManagerPackage/AdapterInactive.java \
	generated/org/omg/PortableServer/IdAssignmentPolicyValue.java \
	generated/org/omg/PortableServer/ThreadPolicyValue.java \
	generated/org/omg/PortableServer/POA.java \
	generated/org/omg/PortableServer/ServantLocatorHelper.java \
	generated/org/omg/PortableServer/RequestProcessingPolicyValue.java \
	generated/org/omg/PortableServer/ServantActivator.java \
	generated/org/omg/PortableServer/ID_ASSIGNMENT_POLICY_ID.java \
	generated/org/omg/PortableServer/ServantActivatorPOA.java \
	generated/org/omg/PortableServer/RequestProcessingPolicy.java \
	generated/org/omg/PortableServer/AdapterActivatorOperations.java \
	generated/org/omg/PortableServer/ThreadPolicyOperations.java \
	generated/org/omg/PortableServer/ImplicitActivationPolicyOperations.java \
	generated/org/omg/PortableServer/ServantRetentionPolicyOperations.java \
	generated/org/omg/PortableServer/POAPackage/AdapterNonExistentHelper.java \
	generated/org/omg/PortableServer/POAPackage/WrongPolicy.java \
	generated/org/omg/PortableServer/POAPackage/ObjectAlreadyActiveHelper.java \
	generated/org/omg/PortableServer/POAPackage/NoServant.java \
	generated/org/omg/PortableServer/POAPackage/AdapterAlreadyExists.java \
	generated/org/omg/PortableServer/POAPackage/NoServantHelper.java \
	generated/org/omg/PortableServer/POAPackage/ObjectAlreadyActive.java \
	generated/org/omg/PortableServer/POAPackage/AdapterNonExistent.java \
	generated/org/omg/PortableServer/POAPackage/AdapterAlreadyExistsHelper.java \
	generated/org/omg/PortableServer/POAPackage/WrongAdapter.java \
	generated/org/omg/PortableServer/POAPackage/ServantAlreadyActiveHelper.java \
	generated/org/omg/PortableServer/POAPackage/InvalidPolicyHelper.java \
	generated/org/omg/PortableServer/POAPackage/ServantNotActiveHelper.java \
	generated/org/omg/PortableServer/POAPackage/InvalidPolicy.java \
	generated/org/omg/PortableServer/POAPackage/ServantNotActive.java \
	generated/org/omg/PortableServer/POAPackage/ObjectNotActiveHelper.java \
	generated/org/omg/PortableServer/POAPackage/WrongPolicyHelper.java \
	generated/org/omg/PortableServer/POAPackage/ObjectNotActive.java \
	generated/org/omg/PortableServer/POAPackage/ServantAlreadyActive.java \
	generated/org/omg/PortableServer/POAPackage/WrongAdapterHelper.java \
	generated/org/omg/PortableServer/IMPLICIT_ACTIVATION_POLICY_ID.java \
	generated/org/omg/PortableServer/IdUniquenessPolicyValue.java \
	generated/org/omg/PortableServer/POAManagerOperations.java \
	generated/org/omg/PortableServer/LifespanPolicy.java \
	generated/org/omg/PortableServer/ID_UNIQUENESS_POLICY_ID.java \
	generated/org/omg/PortableServer/IdUniquenessPolicyOperations.java \
	generated/org/omg/PortableServer/ServantLocator.java \
	generated/org/omg/PortableServer/IdUniquenessPolicy.java \
	generated/sun/awt/X11/XSelectionEvent.java \
	generated/sun/awt/X11/XWindowAttributes.java \
	generated/sun/awt/X11/XGCValues.java \
	generated/sun/awt/X11/XCreateWindowEvent.java \
	generated/sun/awt/X11/XMapEvent.java \
	generated/sun/awt/X11/XdbeSwapInfo.java \
	generated/sun/awt/X11/XIMCallback.java \
	generated/sun/awt/X11/XPoint.java \
	generated/sun/awt/X11/awtImageData.java \
	generated/sun/awt/X11/XAWTIcon64_java_icon16_png.java \
	generated/sun/awt/X11/XAWTIcon32_java_icon16_png.java \
	generated/sun/awt/X11/XColormapEvent.java \
	generated/sun/awt/X11/XImage.java \
	generated/sun/awt/X11/XNoExposeEvent.java \
	generated/sun/awt/X11/Depth.java \
	generated/sun/awt/X11/XSelectionClearEvent.java \
	generated/sun/awt/X11/XResizeRequestEvent.java \
	generated/sun/awt/X11/XIMText.java \
	generated/sun/awt/X11/XAnyEvent.java \
	generated/sun/awt/X11/XGraphicsExposeEvent.java \
	generated/sun/awt/X11/XReparentEvent.java \
	generated/sun/awt/X11/Screen.java \
	generated/sun/awt/X11/XmbTextItem.java \
	generated/sun/awt/X11/XErrorEvent.java \
	generated/sun/awt/X11/XIMHotKeyTrigger.java \
	generated/sun/awt/X11/XIMPreeditDrawCallbackStruct.java \
	generated/sun/awt/X11/XKeyboardControl.java \
	generated/sun/awt/X11/XPixmapFormatValues.java \
	generated/sun/awt/X11/XIMStyles.java \
	generated/sun/awt/X11/ColorEntry.java \
	generated/sun/awt/X11/XExposeEvent.java \
	generated/sun/awt/X11/XHostAddress.java \
	generated/sun/awt/X11/PropMwmHints.java \
	generated/sun/awt/X11/XCirculateRequestEvent.java \
	generated/sun/awt/X11/XWindowChanges.java \
	generated/sun/awt/X11/XwcTextItem.java \
	generated/sun/awt/X11/XAWTIcon64_java_icon24_png.java \
	generated/sun/awt/X11/XAWTIcon32_java_icon24_png.java \
	generated/sun/awt/X11/XMotionEvent.java \
	generated/sun/awt/X11/ScreenFormat.java \
	generated/sun/awt/X11/XVisualInfo.java \
	generated/sun/awt/X11/XOMCharSetList.java \
	generated/sun/awt/X11/XIMValuesList.java \
	generated/sun/awt/X11/XStandardColormap.java \
	generated/sun/awt/X11/XVisibilityEvent.java \
	generated/sun/awt/X11/XCrossingEvent.java \
	generated/sun/awt/X11/XButtonEvent.java \
	generated/sun/awt/X11/AwtGraphicsConfigData.java \
	generated/sun/awt/X11/XComposeStatus.java \
	generated/sun/awt/X11/XModifierKeymap.java \
	generated/sun/awt/X11/XCharStruct.java \
	generated/sun/awt/X11/XIMPreeditCaretCallbackStruct.java \
	generated/sun/awt/X11/generator/sizer.32.c \
	generated/sun/awt/X11/generator/sizer.64.c \
	generated/sun/awt/X11/generator/sizer.32 \
	generated/sun/awt/X11/generator/sizer.64 \
	generated/sun/awt/X11/generator/sizes.32 \
	generated/sun/awt/X11/generator/sizes.64 \
	generated/sun/awt/X11/AwtScreenData.java \
	generated/sun/awt/X11/XDestroyWindowEvent.java \
	generated/sun/awt/X11/XChar2b.java \
	generated/sun/awt/X11/Visual.java \
	generated/sun/awt/X11/XMapRequestEvent.java \
	generated/sun/awt/X11/XOMOrientation.java \
	generated/sun/awt/X11/ColorData.java \
	generated/sun/awt/X11/XUnmapEvent.java \
	generated/sun/awt/X11/XRectangle.java \
	generated/sun/awt/X11/XAWTIcon64_java_icon32_png.java \
	generated/sun/awt/X11/XExtCodes.java \
	generated/sun/awt/X11/XAWTIcon32_java_icon32_png.java \
	generated/sun/awt/X11/XIMHotKeyTriggers.java \
	generated/sun/awt/X11/XFontStruct.java \
	generated/sun/awt/X11/XConfigureRequestEvent.java \
	generated/sun/awt/X11/XKeyEvent.java \
	generated/sun/awt/X11/XColor.java \
	generated/sun/awt/X11/XFontProp.java \
	generated/sun/awt/X11/XArc.java \
	generated/sun/awt/X11/XFocusChangeEvent.java \
	generated/sun/awt/X11/XKeymapEvent.java \
	generated/sun/awt/X11/XClientMessageEvent.java \
	generated/sun/awt/X11/XExtData.java \
	generated/sun/awt/X11/XConfigureEvent.java \
	generated/sun/awt/X11/XKeyboardState.java \
	generated/sun/awt/X11/XGravityEvent.java \
	generated/sun/awt/X11/XOMFontInfo.java \
	generated/sun/awt/X11/XSizeHints.java \
	generated/sun/awt/X11/XTextItem.java \
	generated/sun/awt/X11/XAWTIcon64_java_icon48_png.java \
	generated/sun/awt/X11/XAWTIcon32_java_icon48_png.java \
	generated/sun/awt/X11/XSegment.java \
	generated/sun/awt/X11/XIMStringConversionText.java \
	generated/sun/awt/X11/XMappingEvent.java \
	generated/sun/awt/X11/XTimeCoord.java \
	generated/sun/awt/X11/XIMStringConversionCallbackStruct.java \
	generated/sun/awt/X11/XTextItem16.java \
	generated/sun/awt/X11/XIMPreeditStateNotifyCallbackStruct.java \
	generated/sun/awt/X11/XClassHint.java \
	generated/sun/awt/X11/XTextProperty.java \
	generated/sun/awt/X11/XIconSize.java \
	generated/sun/awt/X11/XFontSetExtents.java \
	generated/sun/awt/X11/XSetWindowAttributes.java \
	generated/sun/awt/X11/XPropertyEvent.java \
	generated/sun/awt/X11/XEvent.java \
	generated/sun/awt/X11/XSelectionRequestEvent.java \
	generated/sun/awt/X11/XIMStatusDrawCallbackStruct.java \
	generated/sun/awt/X11/XWMHints.java \
	generated/sun/awt/X11/XCirculateEvent.java \
	generated/sun/awt/X11/XAWTIcon32_security_icon_bw16_png.java \
	generated/sun/awt/X11/XAWTIcon32_security_icon_bw24_png.java \
	generated/sun/awt/X11/XAWTIcon32_security_icon_bw32_png.java \
	generated/sun/awt/X11/XAWTIcon32_security_icon_bw48_png.java \
	generated/sun/awt/X11/XAWTIcon32_security_icon_interim16_png.java \
	generated/sun/awt/X11/XAWTIcon32_security_icon_interim24_png.java \
	generated/sun/awt/X11/XAWTIcon32_security_icon_interim32_png.java \
	generated/sun/awt/X11/XAWTIcon32_security_icon_interim48_png.java \
	generated/sun/awt/X11/XAWTIcon32_security_icon_yellow16_png.java \
	generated/sun/awt/X11/XAWTIcon32_security_icon_yellow24_png.java \
	generated/sun/awt/X11/XAWTIcon32_security_icon_yellow32_png.java \
	generated/sun/awt/X11/XAWTIcon32_security_icon_yellow48_png.java \
	generated/sun/awt/X11/XAWTIcon64_security_icon_bw16_png.java \
	generated/sun/awt/X11/XAWTIcon64_security_icon_bw24_png.java \
	generated/sun/awt/X11/XAWTIcon64_security_icon_bw32_png.java \
	generated/sun/awt/X11/XAWTIcon64_security_icon_bw48_png.java \
	generated/sun/awt/X11/XAWTIcon64_security_icon_interim16_png.java \
	generated/sun/awt/X11/XAWTIcon64_security_icon_interim24_png.java \
	generated/sun/awt/X11/XAWTIcon64_security_icon_interim32_png.java \
	generated/sun/awt/X11/XAWTIcon64_security_icon_interim48_png.java \
	generated/sun/awt/X11/XAWTIcon64_security_icon_yellow16_png.java \
	generated/sun/awt/X11/XAWTIcon64_security_icon_yellow24_png.java \
	generated/sun/awt/X11/XAWTIcon64_security_icon_yellow32_png.java \
	generated/sun/awt/X11/XAWTIcon64_security_icon_yellow48_png.java \
	generated/sun/awt/X11/XkbAccessXNotifyEvent.java \
	generated/sun/awt/X11/XkbActionMessageEvent.java \
	generated/sun/awt/X11/XkbAnyEvent.java \
	generated/sun/awt/X11/XkbBellNotifyEvent.java \
	generated/sun/awt/X11/XkbCompatMapNotifyEvent.java \
	generated/sun/awt/X11/XkbControlsNotifyEvent.java \
	generated/sun/awt/X11/XkbEvent.java \
	generated/sun/awt/X11/XkbExtensionDeviceNotifyEvent.java \
	generated/sun/awt/X11/XkbIndicatorNotifyEvent.java \
	generated/sun/awt/X11/XkbMapNotifyEvent.java \
	generated/sun/awt/X11/XkbNamesNotifyEvent.java \
	generated/sun/awt/X11/XkbNewKeyboardNotifyEvent.java \
	generated/sun/awt/X11/XkbStateNotifyEvent.java \
	generated/sun/awt/resources/awt_sv.java \
	generated/sun/awt/resources/awt_fr.java \
	generated/sun/awt/resources/awt.java \
	generated/sun/awt/resources/awt_ja.java \
	generated/sun/awt/resources/awt_zh_TW.java \
	generated/sun/awt/resources/awt_it.java \
	generated/sun/awt/resources/awt_zh_HK.java \
	generated/sun/awt/resources/awt_de.java \
	generated/sun/awt/resources/awt_ko.java \
	generated/sun/awt/resources/awt_zh_CN.java \
	generated/sun/awt/resources/awt_es.java \
	generated/sun/nio/ch/SocketOptionRegistry.java \
	generated/sun/nio/ch/AlreadyBoundException.java \
	generated/sun/nio/cs/StandardCharsets.java \
	generated/sun/misc/Version.java \
	generated/sun/util/LocaleDataMetaInfo.java \
	generated/sun/util/logging/resources/logging_de.java \
	generated/sun/util/logging/resources/logging_ko.java \
	generated/sun/util/logging/resources/logging_es.java \
	generated/sun/util/logging/resources/logging_sv.java \
	generated/sun/util/logging/resources/logging_fr.java \
	generated/sun/util/logging/resources/logging_zh_TW.java \
	generated/sun/util/logging/resources/logging_zh_HK.java \
	generated/sun/util/logging/resources/logging_ja.java \
	generated/sun/util/logging/resources/logging_zh_CN.java \
	generated/sun/util/logging/resources/logging.java \
	generated/sun/util/logging/resources/logging_it.java \
	generated/sun/util/CoreResourceBundleControl.java \
	generated/sun/util/resources/CurrencyNames_es_SV.java \
	generated/sun/util/resources/LocaleNames_ca.java \
	generated/sun/util/resources/LocaleNames_ja.java \
	generated/sun/util/resources/CalendarData_cs.java \
	generated/sun/util/resources/CurrencyNames_ar_JO.java \
	generated/sun/util/resources/CurrencyNames_no_NO.java \
	generated/sun/util/resources/CurrencyNames_sv_SE.java \
	generated/sun/util/resources/LocaleNames_pl.java \
	generated/sun/util/resources/LocaleNames_in.java \
	generated/sun/util/resources/CurrencyNames_en_GB.java \
	generated/sun/util/resources/CurrencyNames_it_CH.java \
	generated/sun/util/resources/CurrencyNames_de_AT.java \
	generated/sun/util/resources/CurrencyNames_fr_CA.java \
	generated/sun/util/resources/CurrencyNames_it_IT.java \
	generated/sun/util/resources/CurrencyNames_ar_YE.java \
	generated/sun/util/resources/CurrencyNames_es_NI.java \
	generated/sun/util/resources/CurrencyNames_da_DK.java \
	generated/sun/util/resources/CurrencyNames_ca_ES.java \
	generated/sun/util/resources/LocaleNames_el_CY.java \
	generated/sun/util/resources/LocaleNames_is.java \
	generated/sun/util/resources/CurrencyNames_zh_TW.java \
	generated/sun/util/resources/CurrencyNames_el_GR.java \
	generated/sun/util/resources/LocaleNames_it.java \
	generated/sun/util/resources/CalendarData_ko.java \
	generated/sun/util/resources/CurrencyNames_ja.java \
	generated/sun/util/resources/LocaleNames_da.java \
	generated/sun/util/resources/CurrencyNames_es_MX.java \
	generated/sun/util/resources/CurrencyNames_ar_LB.java \
	generated/sun/util/resources/LocaleNames_pt.java \
	generated/sun/util/resources/CurrencyNames_ms_MY.java \
	generated/sun/util/resources/CurrencyNames_en_MT.java \
	generated/sun/util/resources/CurrencyNames_ar_SA.java \
	generated/sun/util/resources/CalendarData_ro.java \
	generated/sun/util/resources/LocaleNames_iw.java \
	generated/sun/util/resources/CurrencyNames_fr_CH.java \
	generated/sun/util/resources/CurrencyNames_pl_PL.java \
	generated/sun/util/resources/CurrencyNames_de_CH.java \
	generated/sun/util/resources/CurrencyNames_ar_EG.java \
	generated/sun/util/resources/CurrencyNames_ar_SD.java \
	generated/sun/util/resources/LocaleNames_de.java \
	generated/sun/util/resources/CalendarData_el_CY.java \
	generated/sun/util/resources/CurrencyNames_es_VE.java \
	generated/sun/util/resources/CurrencyNames_es_GT.java \
	generated/sun/util/resources/CurrencyNames_cs_CZ.java \
	generated/sun/util/resources/CalendarData_el.java \
	generated/sun/util/resources/CalendarData_mt_MT.java \
	generated/sun/util/resources/CalendarData_zh.java \
	generated/sun/util/resources/LocaleNames_zh_SG.java \
	generated/sun/util/resources/CurrencyNames_sr_BA.java \
	generated/sun/util/resources/LocaleNames_cs.java \
	generated/sun/util/resources/CalendarData_ru.java \
	generated/sun/util/resources/CurrencyNames_it.java \
	generated/sun/util/resources/CalendarData_en.java \
	generated/sun/util/resources/CurrencyNames_es_US.java \
	generated/sun/util/resources/CalendarData_sk.java \
	generated/sun/util/resources/CurrencyNames_lt_LT.java \
	generated/sun/util/resources/CurrencyNames_hu_HU.java \
	generated/sun/util/resources/CalendarData_sl.java \
	generated/sun/util/resources/CurrencyNames_ar_MA.java \
	generated/sun/util/resources/CurrencyNames_ar_KW.java \
	generated/sun/util/resources/CurrencyNames_es_PA.java \
	generated/sun/util/resources/CurrencyNames_es_HN.java \
	generated/sun/util/resources/CurrencyNames_de_DE.java \
	generated/sun/util/resources/CurrencyNames_ru_RU.java \
	generated/sun/util/resources/CurrencyNames_nl_NL.java \
	generated/sun/util/resources/CurrencyNames_et_EE.java \
	generated/sun/util/resources/CurrencyNames_ar_DZ.java \
	generated/sun/util/resources/CurrencyNames_en_US.java \
	generated/sun/util/resources/CurrencyNames_sl_SI.java \
	generated/sun/util/resources/CalendarData_es.java \
	generated/sun/util/resources/CurrencyNames_de.java \
	generated/sun/util/resources/CurrencyNames_mk_MK.java \
	generated/sun/util/resources/CurrencyNames_es_AR.java \
	generated/sun/util/resources/LocaleNames_pt_BR.java \
	generated/sun/util/resources/CalendarData_fi.java \
	generated/sun/util/resources/CalendarData_et.java \
	generated/sun/util/resources/CurrencyNames_es_UY.java \
	generated/sun/util/resources/CalendarData_sq.java \
	generated/sun/util/resources/LocaleNames_en_SG.java \
	generated/sun/util/resources/CurrencyNames_tr_TR.java \
	generated/sun/util/resources/CurrencyNames_es_PE.java \
	generated/sun/util/resources/CalendarData_sr.java \
	generated/sun/util/resources/CalendarData_lt.java \
	generated/sun/util/resources/LocaleNames_ko.java \
	generated/sun/util/resources/CalendarData_th.java \
	generated/sun/util/resources/CurrencyNames_in_ID.java \
	generated/sun/util/resources/CalendarData_mk.java \
	generated/sun/util/resources/LocaleNames_ro.java \
	generated/sun/util/resources/CalendarData_lv.java \
	generated/sun/util/resources/CurrencyNames_en_IE.java \
	generated/sun/util/resources/CurrencyNames_en_NZ.java \
	generated/sun/util/resources/LocaleNames_pt_PT.java \
	generated/sun/util/resources/CalendarData_sv.java \
	generated/sun/util/resources/CurrencyNames_hi_IN.java \
	generated/sun/util/resources/CurrencyNames_fi_FI.java \
	generated/sun/util/resources/CurrencyNames.java \
	generated/sun/util/resources/CurrencyNames_lv_LV.java \
	generated/sun/util/resources/CurrencyNames_th_TH.java \
	generated/sun/util/resources/CurrencyNames_en_AU.java \
	generated/sun/util/resources/CurrencyNames_es_BO.java \
	generated/sun/util/resources/CurrencyNames_uk_UA.java \
	generated/sun/util/resources/LocaleNames_el.java \
	generated/sun/util/resources/CurrencyNames_en_PH.java \
	generated/sun/util/resources/LocaleNames_zh.java \
	generated/sun/util/resources/CalendarData_fr.java \
	generated/sun/util/resources/CurrencyNames_ar_LY.java \
	generated/sun/util/resources/CurrencyNames_en_CA.java \
	generated/sun/util/resources/LocaleNames_ru.java \
	generated/sun/util/resources/LocaleNames_en.java \
	generated/sun/util/resources/CurrencyNames_nl_BE.java \
	generated/sun/util/resources/LocaleNames_sk.java \
	generated/sun/util/resources/CurrencyNames_ko.java \
	generated/sun/util/resources/CurrencyNames_ar_TN.java \
	generated/sun/util/resources/CurrencyNames_ar_SY.java \
	generated/sun/util/resources/LocaleNames_sl.java \
	generated/sun/util/resources/CurrencyNames_ga_IE.java \
	generated/sun/util/resources/CalendarData_tr.java \
	generated/sun/util/resources/CalendarData_mt.java \
	generated/sun/util/resources/CalendarData_es_ES.java \
	generated/sun/util/resources/CurrencyNames_en_IN.java \
	generated/sun/util/resources/CurrencyNames_is_IS.java \
	generated/sun/util/resources/CalendarData_pt_PT.java \
	generated/sun/util/resources/CurrencyNames_es_PR.java \
	generated/sun/util/resources/CurrencyNames_zh_CN.java \
	generated/sun/util/resources/CurrencyNames_hr_HR.java \
	generated/sun/util/resources/CurrencyNames_ja_JP.java \
	generated/sun/util/resources/LocaleNames_es.java \
	generated/sun/util/resources/CalendarData_nl.java \
	generated/sun/util/resources/CurrencyNames_es_CL.java \
	generated/sun/util/resources/LocaleNames_fi.java \
	generated/sun/util/resources/CalendarData_uk.java \
	generated/sun/util/resources/LocaleNames_et.java \
	generated/sun/util/resources/LocaleNames_sq.java \
	generated/sun/util/resources/LocaleNames_sr.java \
	generated/sun/util/resources/LocaleNames_lt.java \
	generated/sun/util/resources/CalendarData_no.java \
	generated/sun/util/resources/LocaleNames_th.java \
	generated/sun/util/resources/CurrencyNames_es_CO.java \
	generated/sun/util/resources/LocaleNames_ga.java \
	generated/sun/util/resources/LocaleNames_mk.java \
	generated/sun/util/resources/LocaleNames_lv.java \
	generated/sun/util/resources/LocaleNames_zh_TW.java \
	generated/sun/util/resources/CurrencyNames_ar_AE.java \
	generated/sun/util/resources/CurrencyNames_es_PY.java \
	generated/sun/util/resources/LocaleNames_sv.java \
	generated/sun/util/resources/CurrencyNames_es_CR.java \
	generated/sun/util/resources/CalendarData_en_GB.java \
	generated/sun/util/resources/CalendarData_hi.java \
	generated/sun/util/resources/CurrencyNames_sr_CS.java \
	generated/sun/util/resources/LocaleNames_en_MT.java \
	generated/sun/util/resources/CurrencyNames_el_CY.java \
	generated/sun/util/resources/CurrencyNames_fr_LU.java \
	generated/sun/util/resources/CalendarData_fr_CA.java \
	generated/sun/util/resources/CurrencyNames_es.java \
	generated/sun/util/resources/CurrencyNames_mt_MT.java \
	generated/sun/util/resources/CalendarData_vi.java \
	generated/sun/util/resources/LocaleNames_fr.java \
	generated/sun/util/resources/CurrencyNames_de_LU.java \
	generated/sun/util/resources/CurrencyNames_bg_BG.java \
	generated/sun/util/resources/CurrencyNames_sq_AL.java \
	generated/sun/util/resources/CurrencyNames_iw_IL.java \
	generated/sun/util/resources/LocaleNames_ms.java \
	generated/sun/util/resources/CalendarData.java \
	generated/sun/util/resources/CalendarData_be.java \
	generated/sun/util/resources/LocaleNames_tr.java \
	generated/sun/util/resources/LocaleNames_mt.java \
	generated/sun/util/resources/CurrencyNames_ro_RO.java \
	generated/sun/util/resources/CurrencyNames_es_EC.java \
	generated/sun/util/resources/CurrencyNames_es_DO.java \
	generated/sun/util/resources/CalendarData_bg.java \
	generated/sun/util/resources/LocaleNames_es_US.java \
	generated/sun/util/resources/CalendarData_ar.java \
	generated/sun/util/resources/CalendarData_ms_MY.java \
	generated/sun/util/resources/CurrencyNames_ar_OM.java \
	generated/sun/util/resources/CalendarData_en_MT.java \
	generated/sun/util/resources/CurrencyNames_fr_FR.java \
	generated/sun/util/resources/CurrencyNames_sv.java \
	generated/sun/util/resources/CalendarData_hr.java \
	generated/sun/util/resources/LocaleNames_nl.java \
	generated/sun/util/resources/LocaleNames_uk.java \
	generated/sun/util/resources/CurrencyNames_be_BY.java \
	generated/sun/util/resources/CurrencyNames_ar_BH.java \
	generated/sun/util/resources/CurrencyNames_sk_SK.java \
	generated/sun/util/resources/CalendarData_hu.java \
	generated/sun/util/resources/LocaleNames_no.java \
	generated/sun/util/resources/CurrencyNames_fr.java \
	generated/sun/util/resources/CalendarData_ca.java \
	generated/sun/util/resources/CurrencyNames_en_ZA.java \
	generated/sun/util/resources/CalendarData_ja.java \
	generated/sun/util/resources/CalendarData_es_US.java \
	generated/sun/util/resources/CalendarData_pl.java \
	generated/sun/util/resources/LocaleNames_hi.java \
	generated/sun/util/resources/LocaleNames.java \
	generated/sun/util/resources/CurrencyNames_pt_BR.java \
	generated/sun/util/resources/CurrencyNames_vi_VN.java \
	generated/sun/util/resources/CurrencyNames_en_SG.java \
	generated/sun/util/resources/CurrencyNames_ar_QA.java \
	generated/sun/util/resources/LocaleNames_vi.java \
	generated/sun/util/resources/CurrencyNames_sr_ME.java \
	generated/sun/util/resources/CurrencyNames_de_GR.java \
	generated/sun/util/resources/CalendarData_is.java \
	generated/sun/util/resources/CurrencyNames_ko_KR.java \
	generated/sun/util/resources/LocaleNames_be.java \
	generated/sun/util/resources/CurrencyNames_ar_IQ.java \
	generated/sun/util/resources/CalendarData_it.java \
	generated/sun/util/resources/LocaleNames_en_PH.java \
	generated/sun/util/resources/CurrencyNames_es_ES.java \
	generated/sun/util/resources/CurrencyNames_pt_PT.java \
	generated/sun/util/resources/CalendarData_in_ID.java \
	generated/sun/util/resources/CalendarData_da.java \
	generated/sun/util/resources/LocaleNames_bg.java \
	generated/sun/util/resources/LocaleNames_ar.java \
	generated/sun/util/resources/CalendarData_pt.java \
	generated/sun/util/resources/CalendarData_en_IE.java \
	generated/sun/util/resources/CalendarData_iw.java \
	generated/sun/util/resources/LocaleNames_hr.java \
	generated/sun/util/resources/CurrencyNames_fr_BE.java \
	generated/sun/util/resources/CalendarData_de.java \
	generated/sun/util/resources/LocaleNames_hu.java \
	generated/sun/print/resources/serviceui_fr.java \
	generated/sun/print/resources/serviceui_zh_CN.java \
	generated/sun/print/resources/serviceui.java \
	generated/sun/print/resources/serviceui_ja.java \
	generated/sun/print/resources/serviceui_it.java \
	generated/sun/print/resources/serviceui_de.java \
	generated/sun/print/resources/serviceui_ko.java \
	generated/sun/print/resources/serviceui_zh_TW.java \
	generated/sun/print/resources/serviceui_zh_HK.java \
	generated/sun/print/resources/serviceui_es.java \
	generated/sun/print/resources/serviceui_sv.java \
	generated/sun/tools/jar/resources/jar_sv.java \
	generated/sun/tools/jar/resources/jar_zh_CN.java \
	generated/sun/tools/jar/resources/jar_fr.java \
	generated/sun/tools/jar/resources/jar_ja.java \
	generated/sun/tools/jar/resources/jar_it.java \
	generated/sun/tools/jar/resources/jar_de.java \
	generated/sun/tools/jar/resources/jar.java \
	generated/sun/tools/jar/resources/jar_zh_TW.java \
	generated/sun/tools/jar/resources/jar_zh_HK.java \
	generated/sun/tools/jar/resources/jar_ko.java \
	generated/sun/tools/jar/resources/jar_es.java \
	generated/sun/tools/jconsole/Version.java \
	generated/sun/management/resources/agent_it.java \
	generated/sun/management/resources/agent_de.java \
	generated/sun/management/resources/agent_zh_CN.java \
	generated/sun/management/resources/agent.java \
	generated/sun/management/resources/agent_ko.java \
	generated/sun/management/resources/agent_es.java \
	generated/sun/management/resources/agent_sv.java \
	generated/sun/management/resources/agent_fr.java \
	generated/sun/management/resources/agent_zh_TW.java \
	generated/sun/management/resources/agent_zh_HK.java \
	generated/sun/management/resources/agent_ja.java \
	generated/java/nio/HeapCharBufferR.java \
	generated/java/nio/DirectCharBufferS.java \
	generated/java/nio/ByteBufferAsDoubleBufferRB.java \
	generated/java/nio/DirectCharBufferU.java \
	generated/java/nio/ByteBufferAsShortBufferL.java \
	generated/java/nio/HeapByteBuffer.java \
	generated/java/nio/HeapCharBuffer.java \
	generated/java/nio/FloatBuffer.java \
	generated/java/nio/ByteBufferAsDoubleBufferRL.java \
	generated/java/nio/ReadOnlyBufferException.java \
	generated/java/nio/DirectDoubleBufferRS.java \
	generated/java/nio/DirectIntBufferS.java \
	generated/java/nio/DirectDoubleBufferRU.java \
	generated/java/nio/DirectIntBufferU.java \
	generated/java/nio/InvalidMarkException.java \
	generated/java/nio/channels/NonWritableChannelException.java \
	generated/java/nio/channels/AsynchronousCloseException.java \
	generated/java/nio/channels/ConnectionPendingException.java \
	generated/java/nio/channels/AlreadyConnectedException.java \
	generated/java/nio/channels/FileLockInterruptionException.java \
	generated/java/nio/channels/ClosedSelectorException.java \
	generated/java/nio/channels/UnresolvedAddressException.java \
	generated/java/nio/channels/NotYetBoundException.java \
	generated/java/nio/channels/IllegalBlockingModeException.java \
	generated/java/nio/channels/UnsupportedAddressTypeException.java \
	generated/java/nio/channels/NotYetConnectedException.java \
	generated/java/nio/channels/AlreadyBoundException.java \
	generated/java/nio/channels/NonReadableChannelException.java \
	generated/java/nio/channels/CancelledKeyException.java \
	generated/java/nio/channels/NoConnectionPendingException.java \
	generated/java/nio/channels/ClosedChannelException.java \
	generated/java/nio/channels/IllegalSelectorException.java \
	generated/java/nio/channels/OverlappingFileLockException.java \
	generated/java/nio/channels/ClosedByInterruptException.java \
	generated/java/nio/ByteBuffer.java \
	generated/java/nio/HeapLongBuffer.java \
	generated/java/nio/DirectCharBufferRS.java \
	generated/java/nio/HeapDoubleBufferR.java \
	generated/java/nio/CharBuffer.java \
	generated/java/nio/DirectCharBufferRU.java \
	generated/java/nio/HeapDoubleBuffer.java \
	generated/java/nio/ByteBufferAsIntBufferRB.java \
	generated/java/nio/DirectFloatBufferS.java \
	generated/java/nio/DirectFloatBufferU.java \
	generated/java/nio/ByteBufferAsCharBufferRB.java \
	generated/java/nio/DirectByteBufferR.java \
	generated/java/nio/HeapIntBuffer.java \
	generated/java/nio/HeapByteBufferR.java \
	generated/java/nio/ByteBufferAsFloatBufferRB.java \
	generated/java/nio/IntBuffer.java \
	generated/java/nio/ByteBufferAsShortBufferRB.java \
	generated/java/nio/ByteBufferAsIntBufferRL.java \
	generated/java/nio/LongBuffer.java \
	generated/java/nio/DirectLongBufferRS.java \
	generated/java/nio/HeapFloatBufferR.java \
	generated/java/nio/ByteBufferAsFloatBufferB.java \
	generated/java/nio/DirectLongBufferRU.java \
	generated/java/nio/DirectIntBufferRS.java \
	generated/java/nio/HeapShortBuffer.java \
	generated/java/nio/ByteBufferAsDoubleBufferB.java \
	generated/java/nio/ByteBufferAsCharBufferRL.java \
	generated/java/nio/DirectIntBufferRU.java \
	generated/java/nio/ByteBufferAsFloatBufferRL.java \
	generated/java/nio/DirectByteBuffer.java \
	generated/java/nio/ByteBufferAsShortBufferRL.java \
	generated/java/nio/HeapIntBufferR.java \
	generated/java/nio/ByteBufferAsLongBufferRB.java \
	generated/java/nio/BufferUnderflowException.java \
	generated/java/nio/ByteBufferAsFloatBufferL.java \
	generated/java/nio/ByteBufferAsIntBufferB.java \
	generated/java/nio/ByteBufferAsDoubleBufferL.java \
	generated/java/nio/ByteBufferAsLongBufferB.java \
	generated/java/nio/DirectShortBufferS.java \
	generated/java/nio/ByteBufferAsLongBufferRL.java \
	generated/java/nio/DirectFloatBufferRS.java \
	generated/java/nio/DirectShortBufferU.java \
	generated/java/nio/ByteBufferAsCharBufferB.java \
	generated/java/nio/ShortBuffer.java \
	generated/java/nio/DirectFloatBufferRU.java \
	generated/java/nio/DirectShortBufferRS.java \
	generated/java/nio/ByteBufferAsIntBufferL.java \
	generated/java/nio/DirectShortBufferRU.java \
	generated/java/nio/DoubleBuffer.java \
	generated/java/nio/ByteBufferAsLongBufferL.java \
	generated/java/nio/BufferOverflowException.java \
	generated/java/nio/HeapShortBufferR.java \
	generated/java/nio/DirectDoubleBufferS.java \
	generated/java/nio/ByteBufferAsShortBufferB.java \
	generated/java/nio/HeapFloatBuffer.java \
	generated/java/nio/DirectDoubleBufferU.java \
	generated/java/nio/HeapLongBufferR.java \
	generated/java/nio/DirectLongBufferS.java \
	generated/java/nio/charset/CharsetDecoder.java \
	generated/java/nio/charset/IllegalCharsetNameException.java \
	generated/java/nio/charset/CharacterCodingException.java \
	generated/java/nio/charset/CharsetEncoder.java \
	generated/java/nio/charset/UnsupportedCharsetException.java \
	generated/java/nio/ByteBufferAsCharBufferL.java \
	generated/java/nio/DirectLongBufferU.java \
	generated/java/lang/CharacterData00.java \
	generated/java/lang/CharacterData01.java \
	generated/java/lang/CharacterData02.java \
	generated/java/lang/CharacterDataUndefined.java \
	generated/java/lang/CharacterData0E.java \
	generated/java/lang/CharacterDataPrivateUse.java \
	generated/java/lang/CharacterDataLatin1.java \
	generated/java/lang/UNIXProcess.java \
	generated/java/util/CurrencyData.java \
	generated/javax/management/remote/rmi/RMIConnectionImpl_Stub.java \
	generated/javax/management/remote/rmi/RMIServerImpl_Stub.java \
	generated/fontconfig/fontconfig.Fedora.bfc \
	generated/fontconfig/fontconfig.SuSE.bfc \
	generated/fontconfig/fontconfig.Ubuntu.bfc \
	generated/fontconfig/fontconfig.bfc \
	generated/javax/swing/plaf/nimbus/ArrowButtonPainter.java \
	generated/javax/swing/plaf/nimbus/ButtonPainter.java \
	generated/javax/swing/plaf/nimbus/CheckBoxMenuItemPainter.java \
	generated/javax/swing/plaf/nimbus/CheckBoxPainter.java \
	generated/javax/swing/plaf/nimbus/ComboBoxComboBoxArrowButtonEditableState.java \
	generated/javax/swing/plaf/nimbus/ComboBoxComboBoxArrowButtonPainter.java \
	generated/javax/swing/plaf/nimbus/ComboBoxComboBoxTextFieldPainter.java \
	generated/javax/swing/plaf/nimbus/ComboBoxEditableState.java \
	generated/javax/swing/plaf/nimbus/ComboBoxPainter.java \
	generated/javax/swing/plaf/nimbus/DesktopIconPainter.java \
	generated/javax/swing/plaf/nimbus/DesktopPanePainter.java \
	generated/javax/swing/plaf/nimbus/EditorPanePainter.java \
	generated/javax/swing/plaf/nimbus/FileChooserPainter.java \
	generated/javax/swing/plaf/nimbus/FormattedTextFieldPainter.java \
	generated/javax/swing/plaf/nimbus/InternalFrameInternalFrameTitlePaneInternalFrameTitlePaneCloseButtonPainter.java \
	generated/javax/swing/plaf/nimbus/InternalFrameInternalFrameTitlePaneInternalFrameTitlePaneCloseButtonWindowNotFocusedState.java \
	generated/javax/swing/plaf/nimbus/InternalFrameInternalFrameTitlePaneInternalFrameTitlePaneIconifyButtonPainter.java \
	generated/javax/swing/plaf/nimbus/InternalFrameInternalFrameTitlePaneInternalFrameTitlePaneIconifyButtonWindowNotFocusedState.java \
	generated/javax/swing/plaf/nimbus/InternalFrameInternalFrameTitlePaneInternalFrameTitlePaneMaximizeButtonPainter.java \
	generated/javax/swing/plaf/nimbus/InternalFrameInternalFrameTitlePaneInternalFrameTitlePaneMaximizeButtonWindowMaximizedState.java \
	generated/javax/swing/plaf/nimbus/InternalFrameInternalFrameTitlePaneInternalFrameTitlePaneMaximizeButtonWindowNotFocusedState.java \
	generated/javax/swing/plaf/nimbus/InternalFrameInternalFrameTitlePaneInternalFrameTitlePaneMenuButtonPainter.java \
	generated/javax/swing/plaf/nimbus/InternalFrameInternalFrameTitlePaneInternalFrameTitlePaneMenuButtonWindowNotFocusedState.java \
	generated/javax/swing/plaf/nimbus/InternalFrameInternalFrameTitlePanePainter.java \
	generated/javax/swing/plaf/nimbus/InternalFrameInternalFrameTitlePaneWindowFocusedState.java \
	generated/javax/swing/plaf/nimbus/InternalFramePainter.java \
	generated/javax/swing/plaf/nimbus/InternalFrameWindowFocusedState.java \
	generated/javax/swing/plaf/nimbus/MenuBarMenuPainter.java \
	generated/javax/swing/plaf/nimbus/MenuBarPainter.java \
	generated/javax/swing/plaf/nimbus/MenuItemPainter.java \
	generated/javax/swing/plaf/nimbus/MenuPainter.java \
	generated/javax/swing/plaf/nimbus/NimbusDefaults.java \
	generated/javax/swing/plaf/nimbus/OptionPaneOptionPaneMessageAreaOptionPaneLabelPainter.java \
	generated/javax/swing/plaf/nimbus/OptionPaneOptionPaneMessageAreaPainter.java \
	generated/javax/swing/plaf/nimbus/OptionPanePainter.java \
	generated/javax/swing/plaf/nimbus/PasswordFieldPainter.java \
	generated/javax/swing/plaf/nimbus/PopupMenuPainter.java \
	generated/javax/swing/plaf/nimbus/PopupMenuSeparatorPainter.java \
	generated/javax/swing/plaf/nimbus/ProgressBarFinishedState.java \
	generated/javax/swing/plaf/nimbus/ProgressBarIndeterminateState.java \
	generated/javax/swing/plaf/nimbus/ProgressBarPainter.java \
	generated/javax/swing/plaf/nimbus/RadioButtonMenuItemPainter.java \
	generated/javax/swing/plaf/nimbus/RadioButtonPainter.java \
	generated/javax/swing/plaf/nimbus/ScrollBarPainter.java \
	generated/javax/swing/plaf/nimbus/ScrollBarScrollBarButtonPainter.java \
	generated/javax/swing/plaf/nimbus/ScrollBarScrollBarThumbPainter.java \
	generated/javax/swing/plaf/nimbus/ScrollBarScrollBarTrackPainter.java \
	generated/javax/swing/plaf/nimbus/ScrollPanePainter.java \
	generated/javax/swing/plaf/nimbus/SeparatorPainter.java \
	generated/javax/swing/plaf/nimbus/SliderArrowShapeState.java \
	generated/javax/swing/plaf/nimbus/SliderPainter.java \
	generated/javax/swing/plaf/nimbus/SliderSliderThumbArrowShapeState.java \
	generated/javax/swing/plaf/nimbus/SliderSliderThumbPainter.java \
	generated/javax/swing/plaf/nimbus/SliderSliderTrackArrowShapeState.java \
	generated/javax/swing/plaf/nimbus/SliderSliderTrackPainter.java \
	generated/javax/swing/plaf/nimbus/SpinnerPainter.java \
	generated/javax/swing/plaf/nimbus/SpinnerPanelSpinnerFormattedTextFieldPainter.java \
	generated/javax/swing/plaf/nimbus/SpinnerSpinnerNextButtonPainter.java \
	generated/javax/swing/plaf/nimbus/SpinnerSpinnerPreviousButtonPainter.java \
	generated/javax/swing/plaf/nimbus/SplitPanePainter.java \
	generated/javax/swing/plaf/nimbus/SplitPaneSplitPaneDividerPainter.java \
	generated/javax/swing/plaf/nimbus/SplitPaneSplitPaneDividerVerticalState.java \
	generated/javax/swing/plaf/nimbus/SplitPaneVerticalState.java \
	generated/javax/swing/plaf/nimbus/TabbedPanePainter.java \
	generated/javax/swing/plaf/nimbus/TabbedPaneTabbedPaneTabAreaPainter.java \
	generated/javax/swing/plaf/nimbus/TabbedPaneTabbedPaneTabPainter.java \
	generated/javax/swing/plaf/nimbus/TableEditorPainter.java \
	generated/javax/swing/plaf/nimbus/TableHeaderPainter.java \
	generated/javax/swing/plaf/nimbus/TableHeaderTableHeaderRendererPainter.java \
	generated/javax/swing/plaf/nimbus/TableHeaderTableHeaderRendererSortedState.java \
	generated/javax/swing/plaf/nimbus/TextAreaNotInScrollPaneState.java \
	generated/javax/swing/plaf/nimbus/TextAreaPainter.java \
	generated/javax/swing/plaf/nimbus/TextFieldPainter.java \
	generated/javax/swing/plaf/nimbus/TextPanePainter.java \
	generated/javax/swing/plaf/nimbus/ToggleButtonPainter.java \
	generated/javax/swing/plaf/nimbus/ToolBarButtonPainter.java \
	generated/javax/swing/plaf/nimbus/ToolBarEastState.java \
	generated/javax/swing/plaf/nimbus/ToolBarNorthState.java \
	generated/javax/swing/plaf/nimbus/ToolBarPainter.java \
	generated/javax/swing/plaf/nimbus/ToolBarSouthState.java \
	generated/javax/swing/plaf/nimbus/ToolBarToggleButtonPainter.java \
	generated/javax/swing/plaf/nimbus/ToolBarWestState.java \
	generated/javax/swing/plaf/nimbus/ToolTipPainter.java \
	generated/javax/swing/plaf/nimbus/TreeCellEditorPainter.java \
	generated/javax/swing/plaf/nimbus/TreePainter.java \
	generated/javax/swing/plaf/nimbus/TreeTreeCellPainter.java

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
	$(top_srcdir)/patches/ecj/*.patch \
	$(top_srcdir)/patches/cacao/*.patch \
	$(top_srcdir)/patches/security/*.patch \
	$(top_srcdir)/patches/xrender/*.patch \
	tools-copy contrib ports extra overlays \
	javaws.png javaws.desktop visualvm.desktop \
	jconsole.desktop policytool.desktop \
	$(JTREG_SRCS) \
	IcedTeaPlugin.cc \
	IcedTeaNPPlugin.cc \
	HACKING $(PULSEAUDIO_SRCS) fsg.sh \
	plugin hotspot.map autogen.sh

# The Binary plugs directory is called jdk1.7.0 for historical reasons. The
# name is completely irrelevant; only contains the plugs to build IcedTea.

# OpenJDK build environment.
OS_DIR = $(BUILD_OS_DIR)-$(BUILD_ARCH_DIR)
BUILD_OUTPUT_DIR = openjdk/build/$(OS_DIR)
ECJ_BUILD_OUTPUT_DIR = openjdk-ecj/build/$(OS_DIR)
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
	"MILESTONE=fcs" \
	"LANG=C" \
	"PATH=$(abs_top_builddir)/bootstrap/jdk1.6.0/bin:$(OS_PATH):$$PATH" \
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
	"DISTRIBUTION_ID=$(DIST_ID)" \
	"ALT_JIBX_LIBS_PATH=$(JIBX_DEPS_DIR)"

if WITH_CACAO
ICEDTEA_ENV += \
	"ALT_HOTSPOT_IMPORT_PATH=$(CACAO_IMPORT_PATH)"
endif

if !ENABLE_NIMBUS_GENERATION
ICEDTEA_ENV += \
	"DISABLE_NIMBUS=true"
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
	"MILESTONE=fcs" \
	"LANG=C" \
	"PATH=$(abs_top_builddir)/bootstrap/jdk1.6.0/bin:$(OS_PATH):$$PATH" \
	"ALT_BOOTDIR=$(ICEDTEA_BOOT_DIR)" \
	"ALT_BINARY_PLUGS_PATH=$(abs_top_builddir)/bootstrap/jdk1.7.0" \
	"BUILD_ARCH_DIR=$(BUILD_ARCH_DIR)" \
	"ICEDTEA_RT=$(ICEDTEA_RT)" \
	"ICEDTEA_BUILD_DIR=$(ICEDTEA_BUILD_DIR_ECJ)" \
	"ICEDTEA_CLS_DIR=$(ICEDTEA_CLS_DIR_ECJ)" \
	"ICEDTEA_ENDORSED_DIR=$(ICEDTEA_ENDORSED_DIR)" \
	"ENDORSED=-Djava.endorsed.dirs=$(ICEDTEA_ENDORSED_DIR)" \
	"BOOTCLASSPATH_CLS_RT=-bootclasspath \
	  $(ICEDTEA_CLS_DIR_ECJ):$(ICEDTEA_RT)" \
	"BOOTCLASSPATH_CLS=-bootclasspath $(ICEDTEA_CLS_DIR_ECJ)" \
	"BOOTCLASSPATH_RT_LIBGCJ=-bootclasspath $(ICEDTEA_RT):$(SYSTEM_GCJ_DIR)/jre/lib/rt.jar" \
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
	"JAR_ACCEPTS_STDIN_LIST=$(JAR_ACCEPTS_STDIN_LIST)" \
	"DISABLE_NIMBUS=true"

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

OPENJDK_HOTSPOT_URL = http://hg.openjdk.java.net/jdk7/hotspot/hotspot

OPENJDK_HG_URL = http://hg.openjdk.java.net/jdk7/jdk7
CVMI_HG_URL = http://hg.openjdk.java.net/cvmi/cvmi/
CLOSURES_HG_URL = http://hg.openjdk.java.net/closures/closures/
CACIOCAVALLO_HG_URL = http://hg.openjdk.java.net/caciocavallo/jdk7/
BSD_HG_URL = http://hg.openjdk.java.net/bsd-port/bsd-port
NIO2_HG_URL = http://hg.openjdk.java.net/nio/nio/

OPENJDK_URL = http://hg.openjdk.java.net/icedtea/jdk7
CORBA_URL = http://hg.openjdk.java.net/icedtea/jdk7/corba
JAXP_URL = http://hg.openjdk.java.net/icedtea/jdk7/jaxp
JAXWS_URL = http://hg.openjdk.java.net/icedtea/jdk7/jaxws
JDK_URL = http://hg.openjdk.java.net/icedtea/jdk7/jdk
LANGTOOLS_URL = http://hg.openjdk.java.net/icedtea/jdk7/langtools
HOTSPOT_URL = http://hg.openjdk.java.net/icedtea/jdk7/hotspot

if USE_ALT_OPENJDK_SRC_ZIP
  OPENJDK_SRC_ZIP = $(ALT_OPENJDK_SRC_ZIP)
else
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
endif

if USE_ALT_HOTSPOT_SRC_ZIP
  HOTSPOT_SRC_ZIP = $(ALT_HOTSPOT_SRC_ZIP)
endif

if WITH_ALT_HSBUILD
  HS_MD5SUM = "`$(AWK) 'version==$$1 {print $$3}' version=$(HSBUILD) $(abs_top_srcdir)/hotspot.map`"
  HS_CHANGESET = "`$(AWK) 'version==$$1 {print $$2}' version=$(HSBUILD) $(abs_top_srcdir)/hotspot.map`"
  HS_URL = $(OPENJDK_HOTSPOT_URL)
else
  HS_MD5SUM = $(HOTSPOT_MD5SUM)
  HS_CHANGESET = $(HOTSPOT_CHANGESET)
  HS_URL = $(HOTSPOT_URL)
endif

if USE_ALT_CORBA_SRC_ZIP
  CORBA_SRC_ZIP = $(ALT_CORBA_SRC_ZIP)
endif

if USE_ALT_JAXP_SRC_ZIP
  JAXP_SRC_ZIP = $(ALT_JAXP_SRC_ZIP)
endif

if USE_ALT_JAXWS_SRC_ZIP
  JAXWS_SRC_ZIP = $(ALT_JAXWS_SRC_ZIP)
endif

if USE_ALT_JDK_SRC_ZIP
  JDK_SRC_ZIP = $(ALT_JDK_SRC_ZIP)
endif

if USE_ALT_LANGTOOLS_SRC_ZIP
  LANGTOOLS_SRC_ZIP = $(ALT_LANGTOOLS_SRC_ZIP)
endif

CACAO_BASE_URL = http://www.complang.tuwien.ac.at
CACAO_URL = $(CACAO_BASE_URL)/cacaojvm/download/cacao-$(CACAO_VERSION)/

if USE_ALT_CACAO_SRC_ZIP
  CACAO_SRC_ZIP = $(ALT_CACAO_SRC_ZIP)
else
  CACAO_SRC_ZIP = cacao-$(CACAO_VERSION).tar.gz
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
if USE_NIO2
	hg fclone $(NIO2_HG_URL) openjdk;
else
if USE_HG
if WITH_HGREV
	$(HG) fclone -r $(HGREV) $(OPENJDK_HG_URL) openjdk;
else
	$(HG) fclone $(OPENJDK_HG_URL) openjdk;
endif
else
	if ! echo "$(OPENJDK_MD5SUM)  $(OPENJDK_SRC_ZIP)" \
	  | $(MD5SUM) --check ; \
	then \
	  if [ $(OPENJDK_SRC_ZIP) ] ; \
	  then \
	    mv $(OPENJDK_SRC_ZIP) $(OPENJDK_SRC_ZIP).old ; \
	  fi ; \
	  $(WGET) $(OPENJDK_URL)/archive/$(OPENJDK_CHANGESET).tar.gz -O $(OPENJDK_SRC_ZIP); \
	fi ; \
	if ! echo "$(CORBA_MD5SUM)  $(CORBA_SRC_ZIP)" \
	  | $(MD5SUM) --check ; \
	then \
	  if [ $(CORBA_SRC_ZIP) ] ; \
	  then \
	    mv $(CORBA_SRC_ZIP) $(CORBA_SRC_ZIP).old ; \
	  fi ; \
	  $(WGET) $(CORBA_URL)/archive/$(CORBA_CHANGESET).tar.gz -O $(CORBA_SRC_ZIP); \
	fi ; \
	if ! echo "$(JAXP_MD5SUM)  $(JAXP_SRC_ZIP)" \
	  | $(MD5SUM) --check ; \
	then \
	  if [ $(JAXP_SRC_ZIP) ] ; \
	  then \
	    mv $(JAXP_SRC_ZIP) $(JAXP_SRC_ZIP).old ; \
	  fi ; \
	  $(WGET) $(JAXP_URL)/archive/$(JAXP_CHANGESET).tar.gz -O $(JAXP_SRC_ZIP); \
	fi ; \
	if ! echo "$(JAXWS_MD5SUM)  $(JAXWS_SRC_ZIP)" \
	  | $(MD5SUM) --check ; \
	then \
	  if [ $(JAXWS_SRC_ZIP) ] ; \
	  then \
	    mv $(JAXWS_SRC_ZIP) $(JAXWS_SRC_ZIP).old ; \
	  fi ; \
	  $(WGET) $(JAXWS_URL)/archive/$(JAXWS_CHANGESET).tar.gz -O $(JAXWS_SRC_ZIP); \
	fi ; \
	if ! echo "$(JDK_MD5SUM)  $(JDK_SRC_ZIP)" \
	  | $(MD5SUM) --check ; \
	then \
	  if [ $(JDK_SRC_ZIP) ] ; \
	  then \
	    mv $(JDK_SRC_ZIP) $(JDK_SRC_ZIP).old ; \
	  fi ; \
	  $(WGET) $(JDK_URL)/archive/$(JDK_CHANGESET).tar.gz -O $(JDK_SRC_ZIP); \
	fi ; \
	if ! echo "$(LANGTOOLS_MD5SUM)  $(LANGTOOLS_SRC_ZIP)" \
	  | $(MD5SUM) --check ; \
	then \
	  if [ $(LANGTOOLS_SRC_ZIP) ] ; \
	  then \
	    mv $(LANGTOOLS_SRC_ZIP) $(LANGTOOLS_SRC_ZIP).old ; \
	  fi ; \
	  $(WGET) $(LANGTOOLS_URL)/archive/$(LANGTOOLS_CHANGESET).tar.gz -O $(LANGTOOLS_SRC_ZIP); \
	fi ;
endif
endif
endif
endif
endif
endif
endif
	if ! echo "$(HS_MD5SUM)  $(HOTSPOT_SRC_ZIP)" \
	  | $(MD5SUM) --check ; \
	then \
	  if [ $(HOTSPOT_SRC_ZIP) ] ; \
	  then \
	    mv $(HOTSPOT_SRC_ZIP) $(HOTSPOT_SRC_ZIP).old ; \
	  fi ; \
	  $(WGET) $(HS_URL)/archive/$(HS_CHANGESET).tar.gz -O $(HOTSPOT_SRC_ZIP) ; \
	fi
endif
if BUILD_CACAO
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
	if ! echo "$(NETBEANS_PROFILER_MD5SUM)  $(NETBEANS_PROFILER_SRC_ZIP)" \
	 | $(MD5SUM) --check ; \
	then \
	 if [ $(NETBEANS_PROFILER_SRC_ZIP) ] ; \
	 then \
	  mv $(NETBEANS_PROFILER_SRC_ZIP) $(NETBEANS_PROFILER_SRC_ZIP).old ; \
	 fi ; \
	 $(WGET) $(NETBEANS_PROFILER_URL)$(NETBEANS_PROFILER_SRC_ZIP) \
	  -O $(NETBEANS_PROFILER_SRC_ZIP) ; \
	 fi

	if ! echo "$(VISUALVM_MD5SUM)  $(VISUALVM_SRC_ZIP)" \
         | $(MD5SUM) --check ; \
        then \
         if [ $(VISUALVM_SRC_ZIP) ] ; \
         then \
          mv $(VISUALVM_SRC_ZIP) $(VISUALVM_SRC_ZIP).old ; \
         fi ; \
         $(WGET) $(VISUALVM_URL)$(VISUALVM_SRC_ZIP) \
          -O $(VISUALVM_SRC_ZIP) ; \
         fi
endif
	mkdir -p stamps
	touch stamps/download.stamp

# VisualVM sources. Alternatives cannot be used here.
NETBEANS_PROFILER_URL = http://icedtea.classpath.org/visualvm/
NETBEANS_PROFILER_SRC_ZIP = netbeans-profiler-visualvm_release65_mod.tar.gz

VISUALVM_URL = https://visualvm.dev.java.net/files/documents/7163/127170/
VISUALVM_SRC_ZIP = visualvm-111-src.tar.gz

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
if BUILD_CACAO
	rm -f $(CACAO_SRC_ZIP)
endif
if WITH_VISUALVM
	rm -f $(VISUALVM_SRC_ZIP)
	rm -f $(NETBEANS_PROFILER_SRC_ZIP)
endif
if WITH_VISUALVM
	rm -f $(VISUALVM_SRC_ZIP)
	rm -f $(NETBEANS_PROFILER_SRC_ZIP)
endif

# Link ports sources into tree
stamps/ports.stamp: stamps/extract.stamp
	for target in $(abs_top_srcdir)/ports/hotspot/make/*/makefiles/* \
		      $(abs_top_srcdir)/ports/hotspot/src/*cpu/* \
		      $(abs_top_srcdir)/ports/hotspot/src/share/vm/*; do \
	  link=$$(dirname $$target | sed 's/^.*ports/openjdk/'|sed "s#hotspot/build#hotspot/make#"); \
	  ln -sfv $$target $$link; \
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

ICEDTEA_PATCHES = \
	patches/icedtea-version.patch \
	patches/hotspot/$(HSBUILD)/icedtea-version.patch \
	patches/icedtea-copy-plugs.patch \
	patches/hotspot/$(HSBUILD)/icedtea-text-relocations.patch \
	patches/icedtea-graphics.patch \
	patches/icedtea-debuginfo.patch \
	patches/hotspot/$(HSBUILD)/icedtea-debuginfo.patch \
	patches/icedtea-ssl.patch \
	$(PLUGIN_PATCH) \
	patches/icedtea-webstart.patch \
	patches/icedtea-webstart-umask.patch \
	patches/icedtea-rmi_amd64.patch \
	patches/icedtea-tools.patch \
	patches/icedtea-demos.patch \
	patches/icedtea-use-system-tzdata.patch \
	patches/icedtea-headers.patch \
	patches/hotspot/$(HSBUILD)/icedtea-headers.patch \
	patches/icedtea-ant.patch \
	patches/icedtea-gcc-suffix.patch \
	patches/hotspot/$(HSBUILD)/icedtea-gcc-suffix.patch \
	patches/icedtea-bytebuffer-compact.patch \
	patches/icedtea-float-double-trailing-zeros.patch \
	patches/hotspot/$(HSBUILD)/icedtea-memory-limits.patch \
	patches/icedtea-sunsrc.patch \
	patches/icedtea-libraries.patch \
	patches/icedtea-core-build.patch \
	patches/hotspot/$(HSBUILD)/icedtea-core-build.patch \
	patches/icedtea-linker-options.patch \
	patches/hotspot/$(HSBUILD)/icedtea-static-libstdc++.patch \
	patches/icedtea-zero-build.patch \
	patches/hotspot/$(HSBUILD)/icedtea-zero-build.patch \
	patches/icedtea-print-lsb-release.patch \
	patches/icedtea-jpegclasses.patch \
	patches/icedtea-uname.patch \
	patches/icedtea-ia64-fdlibm.patch \
	patches/icedtea-freetypeversion.patch \
	patches/icedtea-fonts.patch \
	patches/hotspot/$(HSBUILD)/icedtea-sparc-ptracefix.patch \
	patches/hotspot/$(HSBUILD)/icedtea-sparc-trapsfix.patch \
	patches/icedtea-override-redirect-metacity.patch \
	patches/icedtea-shark-build.patch \
	patches/hotspot/$(HSBUILD)/icedtea-shark-build.patch \
	patches/icedtea-toolkit.patch \
	patches/icedtea-security-updates.patch \
	patches/icedtea-jdk-docs-target.patch \
	patches/icedtea-mbeanintrospector.patch \
	patches/icedtea-hotspot-gcc-pr36917.patch \
	patches/icedtea-alpha-fixes.patch \
	patches/icedtea-alt-jar.patch \
	patches/icedtea-jdk-use-ssize_t.patch \
	patches/hotspot/$(HSBUILD)/icedtea-use-idx_t.patch \
	patches/hotspot/$(HSBUILD)/icedtea-params-cast-size_t.patch \
	patches/icedtea-clean-crypto.patch \
	patches/hotspot/$(HSBUILD)/icedtea-shark.patch \
	$(GCC_PATCH) \
	patches/icedtea-arch.patch \
	patches/icedtea-signed-types.patch \
	patches/icedtea-lc_ctype.patch \
	patches/icedtea-messageutils.patch \
	patches/icedtea-javac-debuginfo.patch \
	patches/icedtea-xjc.patch \
	patches/icedtea-renderer-crossing.patch \
	patches/icedtea-tests-jdk.patch \
	patches/icedtea-stroker-finish.patch \
	patches/icedtea-alsa-default-device.patch \
	patches/icedtea-linker-libs-order.patch \
	patches/icedtea-testenv.patch \
	patches/icedtea-samejvm-safe.patch \
	patches/icedtea-pr261.patch \
	patches/icedtea-doc-headers.patch \
	patches/hotspot/$(HSBUILD)/icedtea-bytecodeInterpreter.patch \
	patches/icedtea-signature-iterator.patch \
	patches/icedtea-test-atomic-operations.patch \
	patches/icedtea-zero.patch \
	patches/icedtea-ia64-bugfix.patch \
	patches/hotspot/$(HSBUILD)/icedtea-parisc-opt.patch \
        patches/security/icedtea-6592792.patch \
        patches/security/icedtea-6733959.patch \
        patches/security/icedtea-6755943.patch \
	patches/hotspot/$(HSBUILD)/icedtea-includedb.patch \
	patches/icedtea-demo-swingapplet.patch \
	patches/icedtea-jvmtiEnv.patch \
	patches/icedtea-xml-encodinginfo.patch \
	patches/icedtea-cc-interp-backedge.patch \
	$(DISTRIBUTION_PATCHES)

if WITH_RHINO
ICEDTEA_PATCHES += \
	patches/icedtea-rhino.patch
endif

if BUILD_CACAO
ICEDTEA_PATCHES += \
	patches/cacao/launcher.patch
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

if ENABLE_NIO2
ICEDTEA_PATCHES += patches/icedtea-nio2.patch
endif

if ENABLE_SYSTEMTAP
ICEDTEA_PATCHES += patches/icedtea-systemtap.patch
endif

if !ENABLE_NIMBUS_GENERATION
ICEDTEA_PATCHES += patches/icedtea-pregenerated-nimbus.patch
endif

stamps/extract.stamp: stamps/download.stamp
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
	touch stamps/extract.stamp

stamps/extract-cacao.stamp: stamps/extract.stamp
if BUILD_CACAO
if !USE_SYSTEM_CACAO
	if ! test -d cacao ; \
	then \
	  mkdir cacao ; \
	  $(TAR) xf $(CACAO_SRC_ZIP) -C cacao ; \
	  dir=$$(basename cacao/cacao-*) ; \
	  ln -s $$dir cacao/cacao ; \
	  cd cacao/cacao ; \
	  $(PATCH) -l -p0 < $(abs_top_srcdir)/patches/cacao/openjdk7.patch ; \
	  cd .. ; \
	fi
endif
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

if ENABLE_NPPLUGIN
	cp -a $(abs_top_srcdir)/plugin/icedteanp/sun/applet/*java openjdk/jdk/src/share/classes/sun/applet/
	mkdir -p rt
	cp -a $(abs_top_srcdir)/plugin/icedteanp/netscape rt/
endif
if ENABLE_PLUGIN
	cp -a $(abs_top_srcdir)/plugin/icedtea/sun/applet/*java openjdk/jdk/src/share/classes/sun/applet/
	mkdir -p rt
	cp -a $(abs_top_srcdir)/plugin/icedtea/netscape rt/
	chmod -R ug+w rt 
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

stamps/overlay.stamp: stamps/patch.stamp
	if [ -e $(abs_top_srcdir)/.hg ] && which $(HG) >/dev/null; then \
	  revision="-r`(cd $(abs_top_srcdir); $(HG) tip --template '{node|short}')`" ; \
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
	fi
	if [ -n "$(PKGVERSION)" ]; then \
	  icedtea_version="$${icedtea_version} ($(PKGVERSION))" ; \
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
	  openjdk/jdk/make/common/shared/Defs.gmk; \
	cp -r $(abs_top_srcdir)/overlays/openjdk/* openjdk/
if ENABLE_NIO2
	cp -r $(abs_top_srcdir)/overlays/nio2/openjdk/* openjdk/
endif
	chmod -R ug+w openjdk 
	touch stamps/overlay.stamp

# OpenJDK ecj Source Preparation Targets
# ======================================

# Copy over OpenJDK sources for ecj.
stamps/clone-ecj.stamp: stamps/overlay.stamp
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
	  link=$$(dirname $$target | sed 's/^.*ports/openjdk-ecj/'|sed "s#hotspot/build#hotspot/make#"); \
	  ln -sfv $$target $$link; \
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

# Patch OpenJDK for ecj/gcj builds.
ICEDTEA_ECJ_PATCHES = patches/ecj/icedtea.patch \
	patches/ecj/icedtea-hotspot-$(HSBUILD).patch \
	patches/ecj/icedtea-spp.patch \
	patches/ecj/icedtea-jopt.patch \
	patches/ecj/icedtea-sbcs.patch \
	patches/ecj/icedtea-dyn.patch

if CP39408_JAVAH
ICEDTEA_ECJ_PATCHES += patches/ecj/icedtea-pr39408.patch
endif

if CP40188_JAVAH
ICEDTEA_ECJ_PATCHES += patches/ecj/icedtea-pr40188.patch
endif

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
  ICEDTEA_BUILD_TARGET=jdk_only
  ICEDTEA_DEBUG_BUILD_TARGET=jdk_fastdebug_only
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
	$(ICEDTEAPLUGIN_TARGET) $(ICEDTEANPPLUGIN_TARGET) \
	stamps/jibx.stamp stamps/cacao.stamp \
	stamps/pulse-java.stamp stamps/visualvm.stamp
	$(ARCH_PREFIX) $(MAKE) \
	  $(ICEDTEA_ENV) \
	  -C openjdk/ \
	  $(ICEDTEA_BUILD_TARGET)
	mkdir -p $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	mkdir -p $(BUILD_OUTPUT_DIR)/j2re-image/lib/$(INSTALL_ARCH_DIR)
	mkdir -p $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/ext
	mkdir -p $(BUILD_OUTPUT_DIR)/j2re-image/lib/ext 
if ENABLE_NPPLUGIN
	cp -pPRf IcedTeaNPPlugin.so \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	cp -pPRf IcedTeaNPPlugin.so \
	  $(BUILD_OUTPUT_DIR)/j2re-image/lib/$(INSTALL_ARCH_DIR)
endif
if ENABLE_PLUGIN
	cp -pPRf IcedTeaPlugin.so \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	cp -pPRf IcedTeaPlugin.so \
	  $(BUILD_OUTPUT_DIR)/j2re-image/lib/$(INSTALL_ARCH_DIR)
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
	cp $(NETX_RESOURCE_DIR)/about.jnlp \
	  extra-lib/about.jar \
	  $(BUILD_OUTPUT_DIR)/j2re-image/lib
	cp $(NETX_RESOURCE_DIR)/about.jnlp \
	  extra-lib/about.jar \
	  $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib
if ZERO_BUILD
	printf -- '-zero ALIASED_TO -server\n' >> $(BUILD_JRE_ARCH_DIR)/jvm.cfg
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
	else \
	  cp $(abs_top_builddir)/tapset/hotspot.stp \
	    $(BUILD_OUTPUT_DIR)/j2sdk-image/tapset/hotspot.stp; \
	fi
endif
	@echo "IcedTea is served:" $(BUILD_OUTPUT_DIR)
	mkdir -p stamps
	touch stamps/icedtea.stamp

stamps/icedtea-debug.stamp: stamps/bootstrap-directory-symlink.stamp \
	stamps/hotspot-tools.stamp stamps/plugs.stamp \
	stamps/ports.stamp stamps/patch.stamp stamps/overlay.stamp \
	$(ICEDTEAPLUGIN_TARGET) $(ICEDTEANPPLUGIN_TARGET) \
	stamps/jibx.stamp stamps/cacao.stamp \
	stamps/pulse-java.stamp stamps/visualvm.stamp
	$(ARCH_PREFIX) $(MAKE) \
	  $(ICEDTEA_ENV) \
	  -C openjdk/ \
	  $(ICEDTEA_DEBUG_BUILD_TARGET)
	mkdir -p $(BUILD_OUTPUT_DIR)-debug/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	mkdir -p $(BUILD_OUTPUT_DIR)-debug/j2re-image/lib/$(INSTALL_ARCH_DIR)
	mkdir -p $(BUILD_OUTPUT_DIR)-debug/j2sdk-image/jre/lib/ext
	mkdir -p $(BUILD_OUTPUT_DIR)-debug/j2re-image/lib/ext 
if ENABLE_NPPLUGIN
	cp -pPRf IcedTeaNPPlugin.so \
	  $(BUILD_OUTPUT_DIR)-debug/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	cp -pPRf IcedTeaNPPlugin.so \
	  $(BUILD_OUTPUT_DIR)-debug/j2re-image/lib/$(INSTALL_ARCH_DIR)
endif
if ENABLE_PLUGIN
	cp -pPRf IcedTeaPlugin.so \
	  $(BUILD_OUTPUT_DIR)-debug/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
	cp -pPRf IcedTeaPlugin.so \
	  $(BUILD_OUTPUT_DIR)-debug/j2re-image/lib/$(INSTALL_ARCH_DIR)
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
	cp $(NETX_RESOURCE_DIR)/default.jnlp \
	  extra-lib/about.jar \
	  $(BUILD_OUTPUT_DIR)-debug/j2re-image/lib
	cp $(NETX_RESOURCE_DIR)/default.jnlp \
	  extra-lib/about.jar \
	  $(BUILD_OUTPUT_DIR)-debug/j2sdk-image/jre/lib
if ZERO_BUILD
	printf -- '-zero ALIASED_TO -server\n' >> $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg
endif
if WITH_CACAO
	printf -- '-cacao ALIASED_TO -server\n' >> $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg
endif
if ENABLE_SYSTEMTAP
	mkdir -p $(BUILD_OUTPUT_DIR)/j2sdk-image/tapset ; \
	grep "client IGNORE" $(BUILD_JRE_ARCH_DIR)/jvm.cfg; \
	if test $$? -eq 0; then \
	  sed -e '/\/client\/libjvm.so/d' \
	    < $(abs_top_builddir)/tapset/hotspot.stp \
	    > $(BUILD_OUTPUT_DIR)/j2sdk-image/tapset/hotspot.stp; \
	else \
	  cp $(abs_top_builddir)/tapset/hotspot.stp \
	    $(BUILD_OUTPUT_DIR)/j2sdk-image/tapset/hotspot.stp; \
	fi
endif
	@echo "IcedTea (debug build) is served:" \
	  $(BUILD_OUTPUT_DIR)-debug
	mkdir -p stamps
	touch stamps/icedtea-debug.stamp

clean-icedtea: stamps/download.stamp
	$(ARCH_PREFIX) $(MAKE) \
	  $(ICEDTEA_ENV) \
	  -C openjdk/make clobber
	rm -f stamps/icedtea.stamp stamps/icedtea-debug.stamp

icedtea-against-icedtea: \
	stamps/bootstrap-directory-symlink.stamp \
	stamps/icedtea.stamp \
	stamps/add-cacao.stamp \
	stamps/add-zero.stamp

icedtea-debug-against-icedtea: \
	stamps/bootstrap-directory-symlink.stamp \
	stamps/icedtea-debug.stamp \
	stamps/add-cacao-debug.stamp \
	stamps/add-zero-debug.stamp

# OpenJDK ecj Targets
# ===================

stamps/native-ecj.stamp:
	mkdir -p stamps 
if BUILD_NATIVE_ECJ
	${GCJ} ${CFLAGS} -Wl,-Bsymbolic -findirect-dispatch -o native-ecj \
	    --main=org.eclipse.jdt.internal.compiler.batch.Main ${ECJ_JAR}
endif
	touch stamps/native-ecj.stamp

stamps/icedtea-ecj.stamp: stamps/bootstrap-directory-symlink-ecj.stamp \
	stamps/hotspot-tools.stamp stamps/plugs.stamp \
	stamps/ports-ecj.stamp stamps/patch-ecj.stamp stamps/cacao.stamp
	$(ARCH_PREFIX) $(MAKE) \
	  $(ICEDTEA_ENV_ECJ) \
	  -C openjdk-ecj \
	  $(ICEDTEA_BUILD_TARGET)
	@echo "ecj-poured IcedTea is served:" \
	  $(ECJ_BUILD_OUTPUT_DIR)
	mkdir -p stamps
	touch stamps/icedtea-ecj.stamp

SERVER_DIR = hotspot/import/jre/lib/$(INSTALL_ARCH_DIR)/server
hotspot:
	rm -f stamps/bootstrap-directory-symlink-ecj.stamp
	rm -f stamps/icedtea-ecj.stamp
	rm -f stamps/icedtea-against-ecj.stamp
	$(ARCH_PREFIX) $(MAKE) hotspot-helper
	mkdir -p \
	   $(ECJ_BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)/server
	cp $(ECJ_BUILD_OUTPUT_DIR)/$(SERVER_DIR)/libjvm.so \
	   $(ECJ_BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)/server
	@echo "Hotspot is served:" $(ECJ_BUILD_OUTPUT_DIR)/j2sdk-image

hotspot-helper: stamps/bootstrap-directory-symlink-ecj.stamp \
	stamps/hotspot-tools.stamp stamps/plugs.stamp \
	stamps/ports-ecj.stamp stamps/patch-ecj.stamp
	$(ARCH_PREFIX) $(MAKE) \
	  $(ICEDTEA_ENV_ECJ) \
	  -C openjdk-ecj hotspot

clean-icedtea-ecj: stamps/clone-ecj.stamp
	rm -f stamps/icedtea-ecj.stamp
	$(ARCH_PREFIX) $(MAKE) \
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

if WITH_ALT_HSBUILD
ICEDTEA_COPY_DIRS += \
	javax/script
endif

stamps/copy-source-files.stamp: stamps/overlay.stamp
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
stamps/hotspot-tools-copy-source-files.stamp: stamps/overlay.stamp
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
            -bootclasspath \'\' @$< ; \
	else \
	  $(ICEDTEA_BOOT_DIR)/bin/javac $(MEMORY_LIMIT) -g \
	    -d lib/hotspot-tools \
	    -source 1.5 \
	    -sourcepath \
	      'hotspot-tools:$(OPENJDK_SOURCEPATH_DIRS):$(ABS_SOURCE_DIRS)' \
	    -bootclasspath \'\' @$< ; \
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
	      '$(OPENJDK_SOURCEPATH_DIRS):$(ABS_SOURCE_DIRS)' \
	    -bootclasspath \'\' @$< ; \
	else \
	  $(ICEDTEA_BOOT_DIR)/bin/javac $(MEMORY_LIMIT) -g -d lib/rt \
            -source 1.5 \
            -sourcepath \
	      '$(OPENJDK_SOURCEPATH_DIRS):$(ABS_SOURCE_DIRS)' \
            -bootclasspath \'\' @$< ; \
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

stamps/extra-class-files.stamp: extra-source-files.txt stamps/plugs.stamp
	mkdir -p extra-lib
	if ! test -d $(ICEDTEA_BOOT_DIR) ; \
	then \
	  $(JAVAC) $(MEMORY_LIMIT) -g -d extra-lib \
	    -source 1.5 \
	    -sourcepath $(abs_top_srcdir)/extra \
	    -classpath $(ICEDTEA_RT) \
	    -bootclasspath \'\' \
	    @extra-source-files.txt ; \
	else \
	  $(ICEDTEA_BOOT_DIR)/bin/javac $(MEMORY_LIMIT) -g -d extra-lib \
            -source 1.5 \
	    -sourcepath $(abs_top_srcdir)/extra \
	    -classpath $(ICEDTEA_RT) \
	    -bootclasspath \'\' \
	    @extra-source-files.txt ; \
	fi
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
	if ! test -d $(ICEDTEA_BOOT_DIR) ; \
	then \
	  $(JAR) cf $@ -C extra-lib net ; \
	else \
	  $(ICEDTEA_BOOT_DIR)/bin/jar cf $@ -C extra-lib net ; \
	fi

stamps/jibx.stamp: extra-lib/about.jar
if ENABLE_NIMBUS
if ENABLE_NIMBUS_GENERATION
	mkdir $(JIBX_DEPS_DIR)
	ln -s $(BCEL_JAR) $(JIBX_DEPS_DIR)
	ln -s $(XPP3_JAR) $(JIBX_DEPS_DIR)
	ln -s $(JIBX_DIR)/jibx-bind.jar $(JIBX_DEPS_DIR)
	ln -s $(JIBX_DIR)/jibx-run.jar $(JIBX_DEPS_DIR)
else
	mkdir -p $(BUILD_OUTPUT_DIR)/gensrc/javax/swing/plaf
	cp -a $(abs_top_builddir)/generated/javax/swing/plaf/nimbus \
	  $(BUILD_OUTPUT_DIR)/gensrc/javax/swing/plaf
endif
endif
	mkdir -p stamps
	touch $@

stamps/cacao.stamp: stamps/extract.stamp stamps/rt-class-files.stamp
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

if ENABLE_NPPLUGIN
# IcedTeaNPPlugin.so.
# Separate compile and link invocations to ensure intermediate object
# is listed before -l options.  See:
# http://developer.mozilla.org/en/docs/XPCOM_Glue
IcedTeaNPPlugin.o: IcedTeaNPPlugin.cc
	if [ -e $(abs_top_srcdir)/.hg ] && which $(HG) >/dev/null; then \
	  revision="-r`(cd $(abs_top_srcdir); $(HG) tip --template '{rev}')`" ; \
	fi ; \
	if [ -n "$(PKGVERSION)" ]; then plugin_version=" ($(PKGVERSION))"; fi; \
	plugin_version=" $(PACKAGE_VERSION)$$revision$$plugin_version"; \
	$(CXX) $(CXXFLAGS) \
	  -DPACKAGE_VERSION="\"$(PACKAGE_VERSION)\"" \
	  -DPLUGIN_VERSION="\"$$plugin_version\"" \
	  -DMOZILLA_VERSION_COLLAPSED="$(MOZILLA_VERSION_COLLAPSED)" \
	  $(GLIB_CFLAGS) \
	  $(GTK_CFLAGS) \
	  $(MOZILLA_CFLAGS) \
	  -fPIC -c -o $@ $<
IcedTeaNPPlugin.so: IcedTeaNPPlugin.o
	$(CXX) $(CXXFLAGS) \
	  $< \
	  $(GLIB_LIBS) \
	  $(GTK_LIBS) \
	  $(MOZILLA_LIBS)\
	  -shared -o $@

clean-IcedTeaNPPlugin:
	rm -f IcedTeaNPPlugin.o
	rm -f IcedTeaNPPlugin.so
endif
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
	  -DMOZILLA_VERSION_COLLAPSED="$(MOZILLA_VERSION_COLLAPSED)" \
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


# PulseAudio based mixer
# (pulse-java)

stamps/pulse-java.stamp: stamps/pulse-java-jar.stamp stamps/pulse-java-headers.stamp
if ENABLE_PULSE_JAVA
	$(CC) $(CFLAGS) -fPIC -c -I$(ICEDTEA_BOOT_DIR)/include/linux -I$(ICEDTEA_BOOT_DIR)/include $(PULSE_JAVA_NATIVE_SRCDIR)/jni-common.c
	$(CC) $(LIBPULSE_CFLAGS) $(CFLAGS) -fPIC -c -I$(ICEDTEA_BOOT_DIR)/include/linux -I$(ICEDTEA_BOOT_DIR)/include -I$(builddir)/pulseaudio $(PULSE_JAVA_NATIVE_SRCDIR)/org_classpath_icedtea_pulseaudio_EventLoop.c
	$(CC) $(LIBPULSE_CFLAGS) $(CFLAGS) -fPIC -c -I$(ICEDTEA_BOOT_DIR)/include/linux -I$(ICEDTEA_BOOT_DIR)/include -I$(builddir)/pulseaudio $(PULSE_JAVA_NATIVE_SRCDIR)/org_classpath_icedtea_pulseaudio_Operation.c
	$(CC) $(LIBPULSE_CFLAGS) $(CFLAGS) -fPIC -c -I$(ICEDTEA_BOOT_DIR)/include/linux -I$(ICEDTEA_BOOT_DIR)/include -I$(builddir)/pulseaudio $(PULSE_JAVA_NATIVE_SRCDIR)/org_classpath_icedtea_pulseaudio_Stream.c
	$(CC) $(LIBPULSE_CFLAGS) $(CFLAGS) -fPIC -c -I$(ICEDTEA_BOOT_DIR)/include/linux -I$(ICEDTEA_BOOT_DIR)/include -I$(builddir)/pulseaudio $(PULSE_JAVA_NATIVE_SRCDIR)/org_classpath_icedtea_pulseaudio_PulseAudioSourcePort.c 
	$(CC) $(LIBPULSE_CFLAGS) $(CFLAGS) -fPIC -c -I$(ICEDTEA_BOOT_DIR)/include/linux -I$(ICEDTEA_BOOT_DIR)/include -I$(builddir)/pulseaudio $(PULSE_JAVA_NATIVE_SRCDIR)/org_classpath_icedtea_pulseaudio_PulseAudioTargetPort.c
	$(CC) $(LDFLAGS) -shared org_*pulseaudio*.o jni-common.o $(LIBPULSE_LIBS) -o libpulse-java.so 
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
	chmod -R ug+w $(PULSE_JAVA_CLASS_DIR)/META-INF
endif
	mkdir -p stamps
	touch stamps/pulse-java-class.stamp


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
	touch stamps/pulse-java-headers.stamp


clean-pulse-java:
if ENABLE_PULSE_JAVA
	[ -z "$(PULSE_JAVA_CLASS_DIR)" ] || find $(PULSE_JAVA_CLASS_DIR) -name '*.class' | xargs -r rm -f
	[ -z "$(PULSE_JAVA_NATIVE_BUILDDIR)" ] || rm -f $(PULSE_JAVA_NATIVE_BUILDDIR)/org_*.h $(PULSE_JAVA_NATIVE_BUILDDIR)/*.o
	rm -f stamps/pulse-java*.stamp
	rm -f pulse-java.jar
	rm -f libpulse-java.so
	rm -rf $(PULSE_JAVA_CLASS_DIR)/META-INF
endif

# end of pulse-java

# targets for additional VMs

BUILD_JRE_ARCH_DIR = $(BUILD_OUTPUT_DIR)/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)
BUILD_DEBUG_JRE_ARCH_DIR = $(BUILD_OUTPUT_DIR)-debug/j2sdk-image/jre/lib/$(INSTALL_ARCH_DIR)

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


# configure script arguments, quoted in single quotes
CONFIGURE_ARGS = @CONFIGURE_ARGS@
ADD_ZERO_CONFIGURE_ARGS = \
	--with-openjdk \
	--with-openjdk-home=$(abs_top_builddir)/$(BUILD_OUTPUT_DIR)/j2sdk-image \
	--enable-zero
if ADD_SHARK_BUILD
ADD_ZERO_CONFIGURE_ARGS += \
	--enable-shark
endif
ADD_ZERO_CONFIGURE_ARGS += \
	--disable-docs \
	$(filter-out '--with-gcj-home=% '--with-ecj=% \
			'--with-java=% '--with-javah=% \
			'--with-rmic=% '--with-additional-vms=% \
			'--with-icedtea '--with-icedtea-home=% \
			'--with-openjdk '--with-openjdk-home=% , \
		$(CONFIGURE_ARGS))

ADD_ZERO_EXTRA_BUILD_ENV = \
	BUILD_LANGTOOLS=false ALT_LANGTOOLS_DIST=$(ICEDTEA_BUILD_DIR)/langtools/dist \
	BUILD_JAXP=false      ALT_JAXP_DIST=$(ICEDTEA_BUILD_DIR)/jaxp/dist \
	BUILD_JAXWS=false     ALT_JAXWS_DIST=$(ICEDTEA_BUILD_DIR)/jaxws/dist \
	BUILD_CORBA=false     ALT_CORBA_DIST=$(ICEDTEA_BUILD_DIR)/corba/dist \
	BUILD_JDK=false \
	DISTRIBUTION_PATCHES='$(foreach p,$(DISTRIBUTION_PATCHES),$(if $(findstring cacao,$(p)),,$(p)))'

stamps/add-zero.stamp: stamps/icedtea.stamp
if ADD_ZERO_BUILD
	mkdir -p zerovm

	cd zerovm && \
	    $(ADD_ZERO_EXTRA_BUILD_ENV) \
		$(ARCH_PREFIX) $(abs_top_srcdir)/configure $(ADD_ZERO_CONFIGURE_ARGS)

	$(ADD_ZERO_EXTRA_BUILD_ENV) \
		$(ARCH_PREFIX) $(MAKE) -C zerovm $(ICEDTEA_BUILD_TARGET)

	mkdir -p $(BUILD_JRE_ARCH_DIR)/zero
	cp -a zerovm/openjdk/build/$(OS_DIR)/hotspot/import/jre/lib/$(INSTALL_ARCH_DIR)/server/* \
		$(BUILD_JRE_ARCH_DIR)/zero/
	printf -- '-zero KNOWN\n' >> $(BUILD_JRE_ARCH_DIR)/jvm.cfg
else
	printf -- '-zero ERROR\n' >> $(BUILD_JRE_ARCH_DIR)/jvm.cfg
endif
	touch $@

stamps/add-zero-debug.stamp: stamps/icedtea-debug.stamp
if ADD_ZERO_BUILD
	mkdir -p zerovm

	cd zerovm && \
	    $(ADD_ZERO_EXTRA_BUILD_ENV) \
		$(ARCH_PREFIX) $(abs_top_srcdir)/configure $(ADD_ZERO_CONFIGURE_ARGS)

	$(ADD_ZERO_EXTRA_BUILD_ENV) \
		$(ARCH_PREFIX) $(MAKE) -C zerovm $(ICEDTEA_DEBUG_BUILD_TARGET)

	mkdir -p $(BUILD_DEBUG_JRE_ARCH_DIR)/zero
	cp -a zerovm/openjdk/build/$(OS_DIR)/hotspot/import/jre/lib/$(INSTALL_ARCH_DIR)/server/* \
		$(BUILD_DEBUG_JRE_ARCH_DIR)/zero/
	printf -- '-zero KNOWN\n' >> $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg
else
	printf -- '-zero ERROR\n' >> $(BUILD_DEBUG_JRE_ARCH_DIR)/jvm.cfg
endif
	touch $@

# end additional VMs

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

check-local: jtregcheck

# VisualVM

stamps/visualvm.stamp: $(BOOTSTRAP_DIRECTORY_STAMP) stamps/download.stamp \
	stamps/extract.stamp stamps/nbplatform.stamp
if WITH_VISUALVM
	cd visualvm ; \
	ln -s $(abs_top_srcdir)/netbeans/nbbuild/netbeans_visualvm netbeans ; \
	JAVA_HOME=$(ICEDTEA_HOME) $(ANT) build
endif
	mkdir -p stamps
	touch $@

clean-visualvm:
	rm -rf visualvm
	rm -f stamps/visualvm.stamp

stamps/nbplatform.stamp: $(BOOTSTRAP_DIRECTORY_STAMP) stamps/extract-visualvm.stamp
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
	for dir in lib.profiler lib.profiler.ui lib.profiler.common profiler ; do \
	  JAVA_HOME=$(ICEDTEA_HOME) $(ANT) -Dpermit.jdk6.builds=true -Dbuildnumber=081204 \
            -Dnetbeans.dest.dir=../nbbuild/netbeans_visualvm \
	    -f $$dir/build.xml ; \
	done 
endif
	mkdir -p stamps
	touch $@

stamps/extract-visualvm.stamp:
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

clean-nbplatform:
	rm -rf netbeans
	rm -f stamps/nbplatform.stamp

# fsg-tarball

# Creates archive of openjdk that is compliant with Free Software guidelines.
dist-openjdk-fsg: stamps/patch-fsg.stamp
	$(ZIP) -r openjdk-fsg-$(OPENJDK_VERSION) openjdk/

# Target Aliases
# ===============

bootstrap-directory-ecj: stamps/bootstrap-directory-ecj.stamp

bootstrap-directory: stamps/bootstrap-directory.stamp

bootstrap-directory-symlink-ecj: stamps/bootstrap-directory-symlink-ecj.stamp

bootstrap-directory-symlink: stamps/bootstrap-directory-symlink.stamp

cacao: stamps/cacao.stamp

clone-ecj: stamps/clone-ecj.stamp

visualvm: stamps/visualvm.stamp

nbplatform: stamps/nbplatform.stamp

extract-visualvm: stamps/extract-visualvm.stamp

visualvm: stamps/visualvm.stamp

nbplatform: stamps/nbplatform.stamp

copy-source-files: stamps/copy-source-files.stamp

download: stamps/download.stamp

extract: stamps/extract.stamp

extract-cacao: stamps/extract-cacao.stamp

generated: stamps/generated.stamp

icedtea: stamps/icedtea.stamp

icedtea-against-ecj: stamps/icedtea-against-ecj.stamp

icedtea-debug: stamps/icedtea-debug.stamp

icedtea-ecj: stamps/icedtea-ecj.stamp

jibx: stamps/jibx.stamp

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

hotspot-ports-ecj: stamps/ports-ecj.stamp

clean: distclean-local

jtreg: stamps/jtreg.stamp
