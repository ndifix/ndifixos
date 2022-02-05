ROOTDIR=$(realpath $(dirname $0)/..)
DEVENV=${ROOTDIR}/devenv

EDK_LOGFILE=${ROOTDIR}/ndifixLoader/build.log
KERNEL_LOGFILE=${ROOTDIR}/kernel/build.log
touch $EDK_LOGFILE $KERNEL_LOGFILE

source ${DEVENV}/var.sh
cd $EDKDIR
source edksetup.sh
build --quiet --platform=$DSCFILE --log=$EDK_LOGFILE

cd ${ROOTDIR}/kernel
make > $KERNEL_LOGFILE
