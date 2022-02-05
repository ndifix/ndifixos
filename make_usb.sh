./build.sh
sudo mkdir -p /mnt/usbmem
sudo mount -t drvfs D: /mnt/usbmem
sudo mkdir -p /mnt/usbmem/EFI/BOOT
sudo cp ~/edk2/Build/ndifixLoaderX64/DEBUG_CLANG38/X64/Loader.efi /mnt/usbmem/EFI/BOOT/BOOTX64.EFI
sudo cp ~/ndifixos/kernel/kernel.elf /mnt/usbmem
sudo umount /mnt/usbmem
