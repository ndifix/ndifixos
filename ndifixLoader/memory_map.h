#pragma once

#include <stdint.h>

struct MemoryMap {
  UINTN buffer_size;
  VOID* buffer;
  UINTN map_size;
  UINTN map_key;
  UINTN descriptor_size;
  UINT32 descriptor_version;
};

// メモリマップを取得します
EFI_STATUS GetMemoryMap(struct MemoryMap* map) {
  if (map->buffer == NULL) {
    return EFI_BUFFER_TOO_SMALL;
  }

  map->map_size = map->buffer_size;

  return gBS->GetMemoryMap(&map->map_size, (EFI_MEMORY_DESCRIPTOR*)map->buffer,
                           &map->map_key, &map->descriptor_size,
                           &map->descriptor_version);
}
