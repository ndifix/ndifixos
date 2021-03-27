#include <cstdint>

#include "console.hpp"
#include "frame_buffer_config.hpp"
#include "graphics.hpp"

extern "C" void KernelMain(
    const ndifixos_FrameBuffer::FrameBufferConfig& config) {
  const ndifixos_graphics::PixelWriter writer(config);
  ndifixos_console::Console console(writer);

  while (1) __asm__("hlt");
}
