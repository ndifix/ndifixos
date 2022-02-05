ROOTDIR=$(realpath $(dirname $0)/..)
DEVENV=${ROOTDIR}/devenv
KERNEL=${ROOTDIR}/kernel

export EDKDIR=~/edk2
export DSCFILE=${ROOTDIR}/ndifixLoader/ndifixLoaderPkg.dsc
export EDK_OUT=${EDKDIR}/Build/ndifixLoaderX64/DEBUG_CLANG38/X64/Loader.efi
export KERNEL_OUT=${KERNEL}/kernel.elf

BASEDIR=${DEVENV}/x86_64-elf
export CPPFLAGS="-I$BASEDIR/include/c++/v1 -I$BASEDIR/include -I$BASEDIR/include/freetype2 -nostdlibinc -D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS"
export LDFLAGS="-L$BASEDIR/lib"
