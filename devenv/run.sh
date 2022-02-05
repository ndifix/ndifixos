ROOTDIR=$(realpath $(dirname $0)/..)
DEVENV=${ROOTDIR}/devenv

source ${DEVENV}/var.sh
${DEVENV}/build.sh
~/osbook/devenv/run_qemu.sh $EDK_OUT $KERNEL_OUT
