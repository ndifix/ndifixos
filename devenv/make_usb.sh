ROOTDIR=$(realpath $(dirname $0)/..)
DEVENV=${ROOTDIR}/devenv

source ${DEVENV}/var.sh
${DEVENV}/build.sh
sudo mkdir -p /mnt/usbmem
sudo mount -t drvfs D: /mnt/usbmem
sudo mkdir -p /mnt/usbmem/EFI/BOOT
sudo cp $EDK_OUT /mnt/usbmem/EFI/BOOT/BOOTX64.EFI
sudo cp ~/ndifixos/kernel/kernel.elf /mnt/usbmem
sudo umount /mnt/usbmem
