#include <cstdint>

#include "console.hpp"
#include "frame_buffer_config.hpp"
#include "graphics.hpp"
#include "memory/memory_map.hpp"

extern "C" void KernelMain(
    const ndifixos::frameBuffer::FrameBufferConfig& config,
    const ndifixos::memory::MemoryMap& memmap) {
  const ndifixos::graphics::PixelWriter writer(config);
  writer.PrintScreen({238, 210, 238});
  ndifixos::console::Console console(writer);
  console.Write("ndifix os\n");

  for (uintptr_t iter = reinterpret_cast<uintptr_t>(memmap.buffer);
       iter < reinterpret_cast<uintptr_t>(memmap.buffer) + memmap.map_size;
       iter += memmap.descriptor_size) {
    auto desc = reinterpret_cast<ndifixos::memory::MemoryDescriptor*>(iter);
    if (ndifixos::memory::IsAvailableMemoryType(desc->type)) {
      console.Write(
          "type=%s, phys=%08lx-%08lx, pages=%05x, attr=%06lx\n",
          ndifixos::memory::MemoryTypeName[desc->type], desc->physical_start,
          desc->physical_start +
              desc->number_of_pages * ndifixos::memory::UEFIPageSize - 1,
          desc->number_of_pages, desc->attribute);
    }
  }

  while (1) __asm__("hlt");
}
