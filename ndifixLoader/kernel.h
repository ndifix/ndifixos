#pragma once

#include <Guid/FileInfo.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

// カーネルを読み込みます
EFI_STATUS ReadKernel(EFI_FILE_PROTOCOL* dir, CHAR16* file_name,
                      EFI_PHYSICAL_ADDRESS kernel_base_addr) {
  EFI_STATUS status;
  EFI_FILE_PROTOCOL* kernel_file;
  dir->Open(dir, &kernel_file, file_name, EFI_FILE_MODE_READ, 0);

  UINTN file_info_size = sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * 12;
  UINT8 file_info_buffer[file_info_size];
  kernel_file->GetInfo(kernel_file, &gEfiFileInfoGuid, &file_info_size,
                       file_info_buffer);

  EFI_FILE_INFO* file_info = (EFI_FILE_INFO*)file_info_buffer;
  UINTN kernel_file_size = file_info->FileSize;

  status = gBS->AllocatePages(AllocateAddress, EfiLoaderData,
                              (kernel_file_size + 0xfff) / 0x1000,
                              &kernel_base_addr);
  if (EFI_ERROR(status)) {
    return status;
  }

  kernel_file->Read(kernel_file, &kernel_file_size, (VOID*)kernel_base_addr);

  Print(L"Kernel: 0x%0lx (%lu bytes)\n", kernel_base_addr, kernel_file_size);
  return EFI_SUCCESS;
}

// カーネルを起動します
void CallKernel(EFI_PHYSICAL_ADDRESS kernel_base_addr,
                EFI_GRAPHICS_OUTPUT_PROTOCOL* gop) {
  typedef void EntryPointType(UINT64, UINT64);
  UINT64 entry_addr = *(UINT64*)(kernel_base_addr + 24);
  EntryPointType* entry_point = (EntryPointType*)entry_addr;
  entry_point(gop->Mode->FrameBufferBase, gop->Mode->FrameBufferSize);
}
