#!/usr/bin/env bash

SRC="src"
BUILD="build"
INSTALL="avr-gcc" # tar.bz2 needs a prefix of 'avr-gcc'
DOWNLOAD="download"
mkdir ${SRC}
mkdir ${BUILD}
mkdir ${INSTALL}
root=$(pwd)

# Get sources
wget -q   "https://raw.githubusercontent.com/osx-cross/homebrew-avr/master/avr-binutils-size.patch" &
wget -qO- "http://ftp.gnu.org/gnu/binutils/binutils-2.30.tar.bz2" | tar xj --directory ${SRC} &
wget -qO- "https://gcc.gnu.org/pub/gcc/releases/gcc-7.3.0/gcc-7.3.0.tar.xz" | tar xJ --directory ${SRC} &
wget -qO- "https://download.savannah.gnu.org/releases/avr-libc/avr-libc-2.0.0.tar.bz2" | tar xj --directory ${SRC} &
wait

# Build binutils first
cd ${SRC}/binutils-2.30
# patch size file
patch -g 0 -f -p0 -i ../../avr-binutils-size.patch
mkdir build && cd build
# configure and make
../configure --prefix=${root}/${INSTALL}/avr-binutils/ --target=avr --disable-nls --disable-werror
make -j16
make install

# prepend path of newly compiled avr-gcc
export PATH=${root}/${INSTALL}/avr-binutils/bin:$PATH

cd ${root}
cd ${SRC}/gcc-7.3.0
mkdir build && cd build
../configure --target=avr --prefix=${root}/${INSTALL}/avr-gcc/ \
        --with-ld=${root}/${INSTALL}/avr-binutils/bin/avr-ld \
        --with-as=${root}/${INSTALL}/avr-binutils/bin/avr-as \
        --enable-languages=c,c++ --with-dwarf2 \
        --disable-nls --disable-libssp --disable-shared \
        --disable-threads --disable-libgomp --disable-bootstrap
make -j16
make install

# prepend path of newly compiled avr-gcc
export PATH=${root}/${INSTALL}/avr-gcc/bin:$PATH

cd ${root}
cd ${SRC}/avr-libc-2.0.0
build=`./config.guess`
./configure --build=${build} --prefix=${root}/${INSTALL}/avr-gcc --host=avr
make install -j16

cd ${root}
rm -r build src avr-binutils-size.patch