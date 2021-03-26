#include <cstdint>

#include "frame_buffer_config.hpp"
#include "graphics.hpp"

extern "C" void KernelMain(
    const ndifixos_FrameBuffer::FrameBufferConfig& config) {
  PrintScreen(config);
  while (1) __asm__("hlt");
}
