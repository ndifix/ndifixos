#include <cstdint>

#include "console.hpp"
#include "frame_buffer_config.hpp"
#include "graphics.hpp"

extern "C" void KernelMain(
    const ndifixos_FrameBuffer::FrameBufferConfig& config) {
  const ndifixos_graphics::PixelWriter writer(config);
  writer.PrintScreen({238, 210, 238});
  ndifixos_console::Console console(writer);
  console.Write("ndifix os\n");

  while (1) __asm__("hlt");
}
