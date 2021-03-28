#include <cstdint>

#include "console.hpp"
#include "frame_buffer_config.hpp"
#include "graphics.hpp"
#include "pci.hpp"

extern "C" void KernelMain(
    const ndifixos::frameBuffer::FrameBufferConfig& config) {
  const ndifixos::graphics::PixelWriter writer(config);
  writer.PrintScreen({238, 210, 238});
  ndifixos::console::Console console(writer);
  console.Write("ndifix os\n");

  ndifixos::pci::PCIManager pci;
  pci.ScanAllBus();
  for (int i = 0; i < pci.DeviceCount(); i++) {
    console.Write("bus:%d device:%d func:%d head:%d\n", pci.device(i).Bus(),
                  pci.device(i).Device_num(), pci.device(i).Function(),
                  pci.device(i).Header_type());
  }
  while (1) __asm__("hlt");
}
