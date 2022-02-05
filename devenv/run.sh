ROOTDIR=$(realpath $(dirname $0)/..)
DEVENV=${ROOTDIR}/devenv

source ${DEVENV}/var.sh
${DEVENV}/build.sh
${DEVENV}/run_qemu.sh $EDK_OUT $KERNEL_OUT
