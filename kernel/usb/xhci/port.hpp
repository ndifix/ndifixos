#pragma once

#include <cstdint>

#include "../../status.hpp"
#include "registers.hpp"

namespace ndifixos::usb::xhci {
struct PortRegisterSet;
class Device;

// xHCI の各ポートを表すクラスと周辺機能
class Port {
 private:
  const uint8_t port_num;
  PortRegisterSet& port_reg_set;

 public:
  Port(uint8_t port_num, PortRegisterSet& port_reg_set)
      : port_num{port_num}, port_reg_set{port_reg_set} {}

  uint8_t Number() const;
  bool IsConnected() const;
  bool IsEnabled() const;
  bool IsConnectStatusChanged() const;
  bool IsPortResetChanged() const;
  int Speed() const;
  status::Status Reset();
  Device* Initialize();

  void ClearPortResetChange() const {
    PORTSC_Bitmap portsc = port_reg_set.PORTSC.Read();
    portsc.data[0] &= 0x0e01c3e0u;
    portsc.bits.port_reset_change = 1;
    port_reg_set.PORTSC.Write(portsc);
  }
};
}  // namespace ndifixos::usb::xhci
