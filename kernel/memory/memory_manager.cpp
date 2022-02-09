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
      memoryManager->SetAvailable(desc->physical_start, desc->number_of_pages);
    }
  }
}

namespace {
MemoryManager::MemoryManager()
  : physMemAvailable{},
  firstGroup{physMemAvailable[1]},
  lastGroup{physMemAvailable.back()} {}

void MemoryManager::SetAvailable(uintptr_t physStartAddr, uint64_t uefiPages) {
  bool isFrameBoundary = (physStartAddr % BytesPerFrame == 0 ? true : false);

  // これらは UEFIPageSize < BytesPerFrame の場合にのみ実行されうる。
  if (!isFrameBoundary) {
    SetAvailable(physStartAddr + UEFIPageSize, uefiPages - 1);
  }
  if (uefiPages * UEFIPageSize < BytesPerFrame) { return; }

  auto SetFrame = [this](uintptr_t frameStartAddr) {
    size_t group = frameStartAddr / BytesPerFrameGroup;
    uint32_t index = (frameStartAddr / BytesPerFrame) % FramesPerGroup;
    physMemAvailable[group] |= ((FrameGroup)1 << index);
  };

  if (UEFIPageSize <= BytesPerFrame) {
    SetFrame(physStartAddr);
    SetAvailable(physStartAddr + BytesPerFrame, uefiPages - (BytesPerFrame / UEFIPageSize));
  }
  if (UEFIPageSize > BytesPerFrame) {
    for (uintptr_t offset = 0; offset < UEFIPageSize; offset += BytesPerFrame) {
      SetFrame(physStartAddr + offset);
    }
    SetAvailable(physStartAddr + UEFIPageSize, uefiPages - 1);
  }
}
}

}  // namespace memory
}  // namespace ndifixos
