#include <cstdint>

#include "console.hpp"
#include "frame_buffer_config.hpp"
#include "graphics.hpp"
#include "pci.hpp"

ndifixos::pci::Device* FindXhc(ndifixos::pci::PCIManager&,
                               ndifixos::console::Console&);

extern "C" void KernelMain(
    const ndifixos::frameBuffer::FrameBufferConfig& config) {
  const ndifixos::graphics::PixelWriter writer(config);
  writer.PrintScreen({238, 210, 238});
  ndifixos::console::Console console(writer);
  console.Write("ndifix os\n");

  ndifixos::pci::PCIManager pci;
  pci.ScanAllBus();
  for (int i = 0; i < pci.DeviceCount(); i++) {
    console.Write("bus:%3d device:%2d func:%1d head:%3d\n", pci.device(i).Bus(),
                  pci.device(i).Device_num(), pci.device(i).Function(),
                  pci.device(i).Header_type());
  }

  ndifixos::pci::Device* xhc_dev = FindXhc(pci, console);

  while (1) __asm__("hlt");
}

// xHC コントローラを探す。なければ nullptr を返す。
ndifixos::pci::Device* FindXhc(ndifixos::pci::PCIManager& pci,
                               ndifixos::console::Console& console) {
  pci.ScanAllBus();

  ndifixos::pci::Device* xhc_dev = nullptr;
  for (int i = 0; i < pci.DeviceCount(); i++) {
    if (pci.device(i).Class_code().isxHC()) {
      auto tmp = pci.device(i);
      xhc_dev = &tmp;

      if (pci.device(i).isIntel()) {
        break;
      }
    }
  }

  if (xhc_dev != nullptr) {
    console.Write("\nxHC found\n");
    console.Write("bus:%d device:%d func:%d head:%d\n", xhc_dev->Bus(),
                  xhc_dev->Device_num(), xhc_dev->Function(),
                  xhc_dev->Header_type());
  } else {
    console.Write("xHC not found\n");
  }
  return xhc_dev;
}
