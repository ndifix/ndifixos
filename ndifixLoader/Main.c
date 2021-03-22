#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include "memory_map.h"

// エントリーポイント
EFI_STATUS EFIAPI UefiMain(EFI_HANDLE image_handle,
                           EFI_SYSTEM_TABLE* system_table) {
  Print(L"Hello ndifix os\n");
  while (1)
    ;

  return EFI_SUCCESS;
}
