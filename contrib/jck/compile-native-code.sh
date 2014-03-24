#!/bin/sh

if [ $# != 1 ]; then
  echo 1>&2 "usage: `basename $0` JCKDIR"
  exit 1
fi

JCKDIR=$(cd $1 && pwd)
if [ ! -f $JCKDIR/src/share/lib/atr/jckatr.c ]; then
  echo 1>&2 "error: $JCKDIR is not a runtime JCK"
  exit 1
fi

basedir=$(dirname $JCKDIR)
if [ -z $basedir ]; then
  echo 1>&2 "error: don't let me delete /lib"\!
  exit 1
fi

arch=$(uname -m)
case "$arch" in
  arm*)
    MFLAG=
    ;;
  s390)
    MFLAG=-m31
    ;;
  i?86|ppc)
    MFLAG=-m32
    ;;
  x86_64|ppc64|s390x)
    MFLAG=-m64
    ;;
  aarch64)
    MFLAG=-march=armv8-a
    ;;
  *)
    echo 1>&2 "error: unhandled arch '$arch'"
    exit 1
esac

LIBDIR=$basedir/lib
RESDIR=$basedir/resources

for dir in $LIBDIR $RESDIR; do
    rm -Rf $dir
    mkdir -p $dir
done

set -x
cp -r $JCKDIR/tests/api/javax_management/loading/data/* $RESDIR
chmod -R +w  $RESDIR

gcc $MFLAG -fPIC -shared -o $LIBDIR/libjckatr.so -I$JCKDIR \
  $JCKDIR/src/share/lib/atr/jckatr.c   

gcc $MFLAG -fPIC -shared -o $LIBDIR/libjckjni.so -I$JCKDIR \
  -I$JCKDIR/src/share/lib/jni/include \
  -I$JCKDIR/src/share/lib/jni/include/solaris \
  $JCKDIR/src/share/lib/jni/jckjni.c   

gcc $MFLAG -fPIC -shared -o $LIBDIR/libjckjvmti.so -I$JCKDIR \
  -I$JCKDIR/src/share/lib/jvmti/include \
  -I$JCKDIR/src/share/lib/jni/include \
  -I$JCKDIR/src/share/lib/jni/include/solaris \
  $JCKDIR/src/share/lib/jvmti/jckjvmti.c   

gcc $MFLAG -fPIC -shared -o $LIBDIR/libsystemInfo.so \
  -I$JCKDIR/src/share/lib/jni/include \
  -I$JCKDIR/src/share/lib/jni/include/solaris \
  $JCKDIR/tests/api/javax_management/loading/data/archives/src/C/com_sun_management_mbeans_loading_SystemInfoUseNativeLib.c   

gcc $MFLAG -fPIC -shared -o $LIBDIR/libjmxlibid.so \
  -I$JCKDIR/src/share/lib/jni/include \
  -I$JCKDIR/src/share/lib/jni/include/solaris \
  $JCKDIR/tests/api/javax_management/loading/data/archives/src/C/com_sun_management_mbeans_loading_GetLibIdFromNativeLib.c   

gcc $MFLAG -fPIC -shared -o $LIBDIR/libgenrandom.so \
  -I$JCKDIR/src/share/lib/jni/include \
  -I$JCKDIR/src/share/lib/jni/include/solaris \
  $JCKDIR/tests/api/javax_management/loading/data/archives/src/C/com_sun_management_mbeans_loading_RandomGen.c   

cd $LIBDIR
jar uf $RESDIR/archives/MBeanUseNativeLib.jar libsystemInfo.so 
rm -f libsystemInfo.so   
jar cf $RESDIR/archives/OnlyLibs.jar libjmxlibid.so 
rm -f libjmxlibid.so
