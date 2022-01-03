#!/usr/bin/env bash

SRC="src"
BUILD="build"
INSTALL="/opt/avr-gcc" # tar.bz2 needs a prefix of 'avr-gcc'
DOWNLOAD="download"
mkdir ${SRC}
mkdir ${BUILD}
mkdir ${INSTALL}
root=$(pwd)
cores=8
#VERSION_BINUTILS="2.37"
VERSION_GCC="11.2.0"
VERSION_LIBC="2.0.0"

# Get sources
wget -q   "https://raw.githubusercontent.com/archlinux/svntogit-community/c3efadcb76f4d8b1a3784015e7c472f59dbfa7de/avr-binutils/repos/community-x86_64/avr-size.patch" &
wget -q   "https://raw.githubusercontent.com/osx-cross/homebrew-avr/d2e2566b06b90355952ed996707a0a1a24673cd3/Patch/avr-libc-add-mcu-atmega168pb.patch" &
#wget -q   "https://raw.githubusercontent.com/osx-cross/homebrew-avr/18d50ba2a168a3b90a25c96e4bc4c053df77d7dc/Patch/avr-binutils-elf-bfd-gdb-fix.patch" &
wget -qO- "https://ftp.gnu.org/gnu/binutils/binutils-2.37.tar.bz2" | tar xj --directory ${SRC} &
wget -qO- "https://ftp.gnu.org/gnu/gcc/gcc-${VERSION_GCC}/gcc-${VERSION_GCC}.tar.xz" | tar xJ --directory ${SRC} &
wget -qO- "https://download.savannah.gnu.org/releases/avr-libc/avr-libc-${VERSION_LIBC}.tar.bz2" | tar xj --directory ${SRC} &
wait

# Build binutils first
cd ${SRC}/binutils-2.37
# patch size file
patch -g 0 -f -p0 -i ../../avr-size.patch
mkdir build && cd build
# configure and make
../configure \
        --prefix=${INSTALL}/avr-binutils/ \
        --target=avr \
        --disable-nls \
        --disable-werror
make -j${cores}
make install

# prepend path of newly compiled avr-binutils
export PATH=${INSTALL}/avr-binutils/bin:$PATH

cd ${root}
cd ${SRC}/gcc-${VERSION_GCC}
mkdir build && cd build
../configure \
        --target=avr \
        --prefix=${INSTALL}/avr-gcc/ \
        --with-ld=${INSTALL}/avr-binutils/bin/avr-ld \
        --with-as=${INSTALL}/avr-binutils/bin/avr-as \
        --libdir=${INSTALL}/avr-gcc/avr/lib \
        --enable-languages=c,c++ \
        --disable-nls \
        --disable-libssp \
        --disable-shared \
        --disable-threads \
        --disable-libgomp \
        --with-dwarf2 \
        --with-avrlibc \
        #--with-system-zlib \
        --disable-bootstrap
make -j${cores}
make install

# prepend path of newly compiled avr-gcc
export PATH=${INSTALL}/avr-gcc/bin:$PATH

cd ${root}
cd ${SRC}/avr-libc-${VERSION_LIBC}
patch -g 0 -f -p0 -i ../../avr-libc-add-mcu-atmega168pb.patch
build=`./config.guess`
./configure \
        --build=${build} \
        --prefix=${INSTALL}/avr-gcc \
        --host=avr
make install -j${cores}

cd ${root}
rm -r build src
rm avr-size.patch
rm avr-libc-add-mcu-atmega168pb.patch
