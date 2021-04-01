#pragma once

#include <stdint.h>

namespace ndifixos {
namespace memory {

struct MemoryMap {
  unsigned long long buffer_size;
  void* buffer;
  unsigned long long map_size;
  unsigned long long map_key;
  unsigned long long descriptor_size;
  uint32_t descriptor_version;
};

struct MemoryDescriptor {
  uint32_t type;
  uintptr_t physical_start;
  uintptr_t virtual_start;
  uint64_t number_of_pages;
  uint64_t attribute;
};

const int UEFIPageSize = 4096;

#ifdef __cplusplus

enum class MemoryType {
  EfiReservedMemoryType,
  EfiLoaderCode,
  EfiLoaderData,
  EfiBootServicesCode,
  EfiBootServicesData,
  EfiRuntimeServicesCode,
  EfiRuntimeServicesData,
  EfiConventionalMemory,
  EfiUnusableMemory,
  EfiACPIReclaimMemory,
  EfiACPIMemoryNVS,
  EfiMemoryMappedIO,
  EfiMemoryMappedIOPortSpace,
  EfiPalCode,
  EfiPersistentMemory,
  EfiMaxMemoryType,
};

bool operator==(uint32_t t, MemoryType type) {
  return t == static_cast<uint32_t>(type);
}

bool operator==(MemoryType type, uint32_t t) {
  return t == static_cast<uint32_t>(type);
}

bool IsAvailableMemoryType(uint32_t type) {
  return type == MemoryType::EfiBootServicesCode ||
         type == MemoryType::EfiBootServicesData ||
         type == MemoryType::EfiConventionalMemory;
}

const char MemoryTypeName[][27] = {
    "EfiReservedMemoryType",
    "EfiLoaderCode",
    "EfiLoaderData",
    "EfiBootServicesCode",
    "EfiBootServicesData",
    "EfiRuntimeServicesCode",
    "EfiRuntimeServicesData",
    "EfiConventionalMemory",
    "EfiUnusableMemory",
    "EfiACPIReclaimMemory",
    "EfiACPIMemoryNVS",
    "EfiMemoryMappedIO",
    "EfiMemoryMappedIOPortSpace",
    "EfiPalCode",
    "EfiPersistentMemory",
    "EfiMaxMemoryType",
};

#endif

}  // namespace memory
}  // namespace ndifixos
