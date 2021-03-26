#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/BlockIo.h>
#include <Protocol/DiskIo2.h>
#include <Protocol/LoadedImage.h>

#include "frame_buffer_config.h"
#include "graphics.h"
#include "kernel.h"
#include "memory_map.h"

void Halt(void) {
  while (1) __asm__("hlt");
}

// root ディレクトリを取得します
EFI_STATUS OpenRootDir(EFI_HANDLE image_handle, EFI_FILE_PROTOCOL** root) {
  EFI_LOADED_IMAGE_PROTOCOL* loaded_image;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* fs;

  gBS->OpenProtocol(image_handle, &gEfiLoadedImageProtocolGuid,
                    (VOID**)&loaded_image, image_handle, NULL,
                    EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);

  gBS->OpenProtocol(loaded_image->DeviceHandle,
                    &gEfiSimpleFileSystemProtocolGuid, (VOID**)&fs,
                    image_handle, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);

  fs->OpenVolume(fs, root);

  return EFI_SUCCESS;
}

// メモリマップを取得してファイルに保存します
EFI_STATUS SaveMemoryMapToFile(struct MemoryMap* memmap, EFI_FILE_PROTOCOL* dir,
                               CHAR16* file_name) {
  GetMemoryMap(memmap);

  EFI_FILE_PROTOCOL* memmap_file;
  dir->Open(dir, &memmap_file, file_name,
            EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);

  SaveMemoryMap(memmap, memmap_file);
  memmap_file->Close(memmap_file);

  return EFI_SUCCESS;
}

// ブートサービスを停止します
void StopBootService(EFI_HANDLE image_handle, struct MemoryMap* memmap) {
  EFI_STATUS status = GetMemoryMap(memmap);
  if (EFI_ERROR(status)) {
    Print(L"failed to get memory map: %r\n", status);
    Halt();
  }

  status = gBS->ExitBootServices(image_handle, memmap->map_key);
  if (EFI_ERROR(status)) {
    Print(L"could not exit boot service: %r\n", status);
    Halt();
  }
}

// エントリーポイント
EFI_STATUS EFIAPI UefiMain(EFI_HANDLE image_handle,
                           EFI_SYSTEM_TABLE* system_table) {
  Print(L"Hello ndifix os\n");

  EFI_STATUS status;
  EFI_FILE_PROTOCOL* root_dir;
  OpenRootDir(image_handle, &root_dir);

  // save memory map
  CHAR8 memmap_buf[1024 * 16];
  struct MemoryMap memmap = {sizeof(memmap_buf), memmap_buf, 0, 0, 0, 0};
  SaveMemoryMapToFile(&memmap, root_dir, L"memmap");

  // get GOP
  EFI_GRAPHICS_OUTPUT_PROTOCOL* gop = NULL;
  struct FrameBufferConfig fb_config;
  OpenGOP(image_handle, &gop);
  status = SetConfig(gop, &fb_config);
  if (EFI_ERROR(status)) {
    Print(L"unsupported pixel format\n");
    Halt();
  }
  Print(L"Resolution: %ux%u\n", gop->Mode->Info->HorizontalResolution,
        gop->Mode->Info->VerticalResolution);
  Print(L"Pixel Format: %s\n",
        GetPixelFormatUnicode(gop->Mode->Info->PixelFormat));
  Print(L"%u pixels/line\n", gop->Mode->Info->PixelsPerScanLine);
  Print(L"Frame Buffer: 0x%0lx - 0x%0lx, Size: %lu bytes\n",
        gop->Mode->FrameBufferBase,
        gop->Mode->FrameBufferBase + gop->Mode->FrameBufferSize,
        gop->Mode->FrameBufferSize);

  // read kernel
  EFI_PHYSICAL_ADDRESS kernel_base_addr = 0x100000;
  status = ReadKernel(root_dir, L"kernel.elf", kernel_base_addr);
  if (EFI_ERROR(status)) {
    Print(L"error occured while reading kernel\n");
    Halt();
  }
  Print(L"kernel read\n");

  // stop boot service
  StopBootService(image_handle, &memmap);

  // call kernel
  CallKernel(kernel_base_addr, &fb_config);

  Halt();

  return EFI_SUCCESS;
}
