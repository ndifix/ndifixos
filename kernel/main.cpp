#include <cstdint>

#include "frame_buffer_config.hpp"

void PrintScreen(const ndifixos_FrameBuffer::FrameBufferConfig& config) {
  for (uint64_t i = 0; i < config.frame_buffer_size; ++i) {
    config.frame_buffer_base[i] = i % 0x100;
  }
}

extern "C" void KernelMain(
    const ndifixos_FrameBuffer::FrameBufferConfig& config) {
  PrintScreen(config);
  while (1) __asm__("hlt");
}
