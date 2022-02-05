ROOTDIR=$(realpath $(dirname $0)/..)
DEVENV=${ROOTDIR}/devenv

MOUNT_POINT=/mnt/usbmem

source ${DEVENV}/var.sh
${DEVENV}/build.sh
sudo mkdir -p $MOUNT_POINT
sudo mount -t drvfs D: $MOUNT_POINT
sudo mkdir -p ${MOUNT_POINT}/EFI/BOOT
sudo cp $EDK_OUT ${MOUNT_POINT}/EFI/BOOT/BOOTX64.EFI
sudo cp ~/ndifixos/kernel/kernel.elf $MOUNT_POINT
sudo umount $MOUNT_POINT
