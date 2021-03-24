#include <cstdint>

void PrintScreen(uint64_t frame_buffer_base, uint64_t frame_buffer_size) {
  uint8_t* frame_buffer = reinterpret_cast<uint8_t*>(frame_buffer_base);
  for (uint64_t i = 0; i < frame_buffer_size; ++i) {
    frame_buffer[i] = i % 0x100;
  }
}

extern "C" void KernelMain(uint64_t frame_buffer_base,
                           uint64_t frame_buffer_size) {
  PrintScreen(frame_buffer_base, frame_buffer_size);
  while (1) __asm__("hlt");
}
