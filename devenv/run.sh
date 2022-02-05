ROOTDIR=$(realpath $(cd $(dirname $0); pwd)/..)
DEVENV=${ROOTDIR}/devenv

${DEVENV}/build.sh
~/osbook/devenv/run_qemu.sh ~/edk2/Build/ndifixLoaderX64/DEBUG_CLANG38/X64/Loader.efi ~/ndifixos/kernel/kernel.elf
