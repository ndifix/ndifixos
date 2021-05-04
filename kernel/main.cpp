#include <cstdint>

#include "console.hpp"
#include "frame_buffer_config.hpp"
#include "graphics.hpp"
#include "pci.hpp"
#include "status.hpp"
#include "usb/xhci/xhci.hpp"

ndifixos::pci::Device* FindXhc(ndifixos::pci::PCIManager&,
                               ndifixos::console::Console&);

void Halt() {
  while (1) __asm__("hlt");
}

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

  // 見つからなかった場合停止する
  if (xhc_dev == nullptr) {
    Halt();
  }

  const auto xhc_bar = pci.ReadBar(*xhc_dev, 0);
  const uintptr_t xhc_MMIO_base = xhc_bar.val & ~static_cast<uint64_t>(0xf);
  console.Write("Read BAR: %s\n", xhc_bar.status.isSuccess() ? "ok" : "error");
  console.Write("xHC MMIO base: %08lx\n", xhc_MMIO_base);

  ndifixos::usb::xhci::Controller controller(xhc_MMIO_base);
  ndifixos::status::Status stat = controller.Initialize();
  console.Write("xHC initialize: %s\n", stat.Name());
  console.Write("xHC starting\n");
  controller.Run();

  Halt();
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
