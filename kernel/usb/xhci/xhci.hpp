#pragma once

#include <cstdint>

namespace ndifixos {
namespace usb {
namespace xhci {

class Controller {
 private:
  const uintptr_t mmio_base;

 public:
  Controller(uintptr_t mmio_base) : mmio_base{mmio_base} {}
};

}  // namespace xhci
}  // namespace usb
}  // namespace ndifixos
