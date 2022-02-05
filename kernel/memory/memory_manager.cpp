#include <cstdint>
#include <new>

#include "memory_manager.hpp"
#include "memory_map.hpp"

namespace ndifixos {
namespace memory {
MemoryManager* memoryManager = nullptr;

namespace {
char memoryManagerBuf[sizeof(MemoryManager)];
}

void InitMemoryManager(const MemoryMap& memory_map) {
  memoryManager = new (memoryManagerBuf) MemoryManager;
  const uintptr_t memmap_base = reinterpret_cast<uintptr_t>(memory_map.buffer);

  for (uintptr_t iter = memmap_base;
      iter < memmap_base + memory_map.map_size;
      iter += memory_map.descriptor_size) {
    auto desc = reinterpret_cast<MemoryDescriptor*>(iter);
    if (IsAvailableMemoryType(desc->type)) {
    }
  }
}

}  // namespace memory
}  // namespace ndifixos
