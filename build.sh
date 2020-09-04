#!/usr/bin/env bash

SRC="src"
BUILD="build"
INSTALL="/opt/avr-gcc" # tar.bz2 needs a prefix of 'avr-gcc'
DOWNLOAD="download"
mkdir ${SRC}
mkdir ${BUILD}
mkdir ${INSTALL}
root=$(pwd)
cores=4
#VERSION_BINUTILS="2.33"
VERSION_GCC="10.2.0"
VERSION_LIBC="2.0.0"

# Get sources
wget -q   "https://dl.bintray.com/osx-cross/avr-patches/avr-binutils-2.33-size.patch" &
wget -q   "https://dl.bintray.com/osx-cross/avr-patches/avr-libc-${VERSION_LIBC}-atmega168pb.patch" &
wget -qO- "https://ftp.gnu.org/gnu/binutils/binutils-2.33.1.tar.bz2" | tar xj --directory ${SRC} &
wget -qO- "https://ftp.gnu.org/gnu/gcc/gcc-${VERSION_GCC}/gcc-${VERSION_GCC}.tar.xz" | tar xJ --directory ${SRC} &
wget -qO- "https://download.savannah.gnu.org/releases/avr-libc/avr-libc-${VERSION_LIBC}.tar.bz2" | tar xj --directory ${SRC} &
wait

# Build binutils first
cd ${SRC}/binutils-2.33.1
# patch size file
patch -g 0 -f -p0 -i ../../avr-binutils-2.33-size.patch
mkdir build && cd build
# configure and make
../configure --prefix=${INSTALL}/avr-binutils/ --target=avr --disable-nls --disable-werror
make -j${cores}
make install

# prepend path of newly compiled avr-binutils
export PATH=${INSTALL}/avr-binutils/bin:$PATH

cd ${root}
cd ${SRC}/gcc-${VERSION_GCC}
mkdir build && cd build
../configure --target=avr --prefix=${INSTALL}/avr-gcc/ \
        --with-ld=${INSTALL}/avr-binutils/bin/avr-ld \
        --with-as=${INSTALL}/avr-binutils/bin/avr-as \
        --enable-languages=c,c++ --with-dwarf2 \
        --disable-nls --disable-libssp --disable-shared \
        --disable-threads --disable-libgomp --disable-bootstrap
make -j${cores}
make install

# prepend path of newly compiled avr-gcc
export PATH=${INSTALL}/avr-gcc/bin:$PATH

cd ${root}
cd ${SRC}/avr-libc-${VERSION_LIBC}
patch -g 0 -f -p0 -i ../../avr-libc-${VERSION_LIBC}-atmega168pb.patch
build=`./config.guess`
./configure --build=${build} --prefix=${INSTALL}/avr-gcc --host=avr
make install -j${cores}

cd ${root}
rm -r build src
rm avr-binutils-2.33-size.patch
rm avr-libc-${VERSION_LIBC}-atmega168pb.patch
