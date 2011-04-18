#!/bin/sh

ZLIB_VERSION=1.2.3
ZIP_SRC=openjdk/jdk/src/share/native/java/util/zip/zlib-${ZLIB_VERSION}
JPEG_SRC=openjdk/jdk/src/share/native/sun/awt/image/jpeg
GIF_SRC=openjdk/jdk/src/share/native/sun/awt/giflib
PNG_SRC=openjdk/jdk/src/share/native/sun/awt/libpng

echo "Removing zlib"
rm -rvf ${ZIP_SRC}
echo "Removing libjpeg"
rm -vf ${JPEG_SRC}/jcomapi.c
rm -vf ${JPEG_SRC}/jdapimin.c
rm -vf ${JPEG_SRC}/jdapistd.c
rm -vf ${JPEG_SRC}/jdcoefct.c
rm -vf ${JPEG_SRC}/jdcolor.c
rm -vf ${JPEG_SRC}/jddctmgr.c
rm -vf ${JPEG_SRC}/jdhuff.c
rm -vf ${JPEG_SRC}/jdinput.c
rm -vf ${JPEG_SRC}/jdmainct.
rm -vf ${JPEG_SRC}/jdmarker.c
rm -vf ${JPEG_SRC}/jdmaster.c
rm -vf ${JPEG_SRC}/jdmerge.c
rm -vf ${JPEG_SRC}/jdphuff.c
rm -vf ${JPEG_SRC}/jdpostct.c
rm -vf ${JPEG_SRC}/jdsample.c
rm -vf ${JPEG_SRC}/jerror.c
rm -vf ${JPEG_SRC}/jidctflt.c
rm -vf ${JPEG_SRC}/jidctfst.c
rm -vf ${JPEG_SRC}/jidctint.c
rm -vf ${JPEG_SRC}/jidctred.c
rm -vf ${JPEG_SRC}/jmemmgr.c
rm -vf ${JPEG_SRC}/jmemnobs.c
rm -vf ${JPEG_SRC}/jquant1.c
rm -vf ${JPEG_SRC}/jquant2.c
rm -vf ${JPEG_SRC}/jutils.c
rm -vf ${JPEG_SRC}/jcapimin.c
rm -vf ${JPEG_SRC}/jcapistd.c
rm -vf ${JPEG_SRC}/jccoefct.c
rm -vf ${JPEG_SRC}/jccolor.c
rm -vf ${JPEG_SRC}/jcdctmgr.c
rm -vf ${JPEG_SRC}/jchuff.c
rm -vf ${JPEG_SRC}/jcinit.c
rm -vf ${JPEG_SRC}/jcmainct.c
rm -vf ${JPEG_SRC}/jcmarker.c
rm -vf ${JPEG_SRC}/jcmaster.c
rm -vf ${JPEG_SRC}/jcparam.c
rm -vf ${JPEG_SRC}/jcphuff.c
rm -vf ${JPEG_SRC}/jcprepct.c
rm -vf ${JPEG_SRC}/jcsample.c
rm -vf ${JPEG_SRC}/jctrans.c
rm -vf ${JPEG_SRC}/jdtrans.c
rm -vf ${JPEG_SRC}/jfdctflt.c
rm -vf ${JPEG_SRC}/jfdctfst.c
rm -vf ${JPEG_SRC}/jfdctint.c
rm -vf ${JPEG_SRC}/README
echo "Removing giflib"
rm -rvf ${GIF_SRC}
echo "Removing libpng"
rm -rvf ${PNG_SRC}
