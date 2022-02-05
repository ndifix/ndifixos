#!/bin/sh -ex

if [ $# -lt 1 ]
then
    echo "Usage: $0 <.efi file> [kernel file]"
    exit 1
fi

DEVENV_DIR=$(dirname "$0")
EFI_FILE=$1
KERNEL_FILE=$2
DISK_IMG=./disk.img
MOUNT_POINT=./mnt

$DEVENV_DIR/make_image.sh $DISK_IMG $MOUNT_POINT $EFI_FILE $KERNEL_FILE
$DEVENV_DIR/run_image.sh $DISK_IMG
