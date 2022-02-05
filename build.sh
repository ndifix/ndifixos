SCRIPTDIR=$(cd $(dirname $0); pwd)
DSCFILE=${SCRIPTDIR}/ndifixLoader/ndifixLoaderPkg.dsc
EDK_LOGFILE=${SCRIPTDIR}/ndifixLoader/build.log
KERNEL_LOGFILE=${SCRIPTDIR}/kernel/build.log
touch $EDK_LOGFILE $KERNEL_LOGFILE

EDKDIR=~/edk2


source var.sh
cd $EDKDIR
source edksetup.sh
build --quiet --platform=$DSCFILE --log=$EDK_LOGFILE

cd ${SCRIPTDIR}/kernel
make > $KERNEL_LOGFILE
