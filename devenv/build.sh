ROOTDIR=$(realpath $(cd $(dirname $0); pwd)/..)
DEVENV=${ROOTDIR}/devenv

DSCFILE=${ROOTDIR}/ndifixLoader/ndifixLoaderPkg.dsc
EDK_LOGFILE=${ROOTDIR}/ndifixLoader/build.log
KERNEL_LOGFILE=${ROOTDIR}/kernel/build.log
touch $EDK_LOGFILE $KERNEL_LOGFILE

EDKDIR=~/edk2

echo $ROOTDIR
source ${DEVENV}/var.sh
cd $EDKDIR
source edksetup.sh
build --quiet --platform=$DSCFILE --log=$EDK_LOGFILE

cd ${ROOTDIR}/kernel
make > $KERNEL_LOGFILE
