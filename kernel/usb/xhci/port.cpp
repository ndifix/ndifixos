#include "port.hpp"

#include "../../status.hpp"
#include "registers.hpp"
#include "xhci.hpp"

namespace ndifixos::usb::xhci {
uint8_t Port::Number() const { return port_num; }

bool Port::IsConnected() const {
  return port_reg_set.PORTSC.Read().bits.current_connect_status;
}

bool Port::IsEnabled() const {
  return port_reg_set.PORTSC.Read().bits.port_enabled_disabled;
}

bool Port::IsConnectStatusChanged() const {
  return port_reg_set.PORTSC.Read().bits.connect_status_change;
}

int Port::Speed() const { return port_reg_set.PORTSC.Read().bits.port_speed; }

status::Status Port::Reset() {
  auto portsc = port_reg_set.PORTSC.Read();
  portsc.data[0] &= 0x0e00c3e0u;
  portsc.data[0] |= 0x00020010u;  // Write 1 to PR and CSC
  port_reg_set.PORTSC.Write(portsc);
  while (port_reg_set.PORTSC.Read().bits.port_reset)
    ;
  return status::Status::Success;
}

Device* Port::Initialize() { return nullptr; }
}  // namespace ndifixos::usb::xhci
