# **************************************************************************** #
#                                                              _               #
#                                                  __   ___.--'_\`.            #
#    config.sh                                    ( _\`.' -   'o\` )           #
#                                                 _\\.'_'      _.-'            #
#    By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`             #
#                                                 \\_'-`---'\\__,              #
#    Created: 2024/08/04 01:32:58 by mathroy0310   \`        `-\\              #
#    Updated: 2024/08/04 19:58:29 by mathroy0310    `                          #
#                                                                              #
# **************************************************************************** #

SYSTEM_HEADER_PROJECTS="libc FROG kernel"
PROJECTS="libc FROG kernel"
 
export MAKE=${MAKE:-make}
export HOST=${HOST:-$(./default-host.sh)}
 
export AR=${HOST}-ar
export AS=${HOST}-as
export CC=${HOST}-gcc
export CXX=${HOST}-g++

export PREFIX=/usr
export EXEC_PREFIX=$PREFIX
export BOOTDIR=/boot
export LIBDIR=$EXEC_PREFIX/lib
export INCLUDEDIR=$PREFIX/include
 
export CFLAGS='-O2 -g'
export CPPFLAGS=''
 
# Configure the cross-compiler to use the desired system root.
export SYSROOT="$(pwd)/sysroot"
export CC="$CC --sysroot=$SYSROOT"
export CXX="$CXX --sysroot=$SYSROOT"

# Work around that the -elf gcc targets doesn't have a system include directory
# because it was configured with --without-headers rather than --with-sysroot.
if echo "$HOST" | grep -Eq -- '-elf($|-)'; then
 	export CC="$CC -isystem=$INCLUDEDIR"
	export CXX="$CXX -isystem=$INCLUDEDIR"
fi
