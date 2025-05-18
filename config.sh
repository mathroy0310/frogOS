#!/bin/sh
echo "===== Starting Configuration Script ====="
SYSTEM_HEADER_PROJECTS="libc FROG kernel"
echo "  SYSTEM_HEADER_PROJECTS: $SYSTEM_HEADER_PROJECTS"

echo "Setting main projects list..."
PROJECTS="libc FROG kernel"
echo "  PROJECTS: $PROJECTS"

echo "Configuring build tools..."
export MAKE=${MAKE:-make}
echo "  MAKE: $MAKE"

echo "Detecting host system..."
export HOST=${HOST:-$(./default-host.sh)}
echo "  HOST: $HOST" 

echo "Setting up cross-compiler toolchain..."
export AR=${HOST}-ar
echo "  AR: $AR"
export AS=${HOST}-as
echo "  AS: $AS"
export CC=${HOST}-gcc
echo "  CC: $CC"
export CXX=${HOST}-g++
echo "  CXX: $CXX"

echo "Configuring installation directories..."
export PREFIX=/usr
echo "  PREFIX: $PREFIX"
export EXEC_PREFIX=$PREFIX
echo "  EXEC_PREFIX: $EXEC_PREFIX"
export BOOTDIR=/boot
echo "  BOOTDIR: $BOOTDIR"
export LIBDIR=$EXEC_PREFIX/lib
echo "  LIBDIR: $LIBDIR"
export INCLUDEDIR=$PREFIX/include
echo "  INCLUDEDIR: $INCLUDEDIR"
 
echo "Setting compilation flags..."
export CFLAGS='-O2 -g'
echo "  CFLAGS: $CFLAGS"
export CPPFLAGS='--std=c++20 -Wno-literal-suffix'
echo "  CPPFLAGS: $CPPFLAGS"
export UBSAN=1
echo "  UBSAN: $UBSAN"

echo "Configuring system root..."
# Configure the cross-compiler to use the desired system root.
export PWD=$(pwd)
echo "  PWD: $PWD"
export SYSROOT="$PWD/sysroot"
echo "  SYSROOT: $SYSROOT"

echo "Updating compiler to use sysroot..."
export CC="$CC --sysroot=$SYSROOT"
echo "  CC: $CC"
export CXX="$CXX --sysroot=$SYSROOT"
echo "  CXX: $CXX"

# Work around that the -elf gcc targets doesn't have a system include directory
# because it was configured with --without-headers rather than --with-sysroot.
if echo "$HOST" | grep -Eq -- '-elf($|-)'; then
 	export CC="$CC -isystem=$INCLUDEDIR"
	export CXX="$CXX -isystem=$INCLUDEDIR"
fi

echo "===== Configuration Complete ====="
