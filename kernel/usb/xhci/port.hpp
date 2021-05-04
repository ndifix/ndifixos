#pragma once

#include <cstdint>

#include "../../status.hpp"

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
  int Speed() const;
  status::Status Reset();
  Device* Initialize();
};
}  // namespace ndifixos::usb::xhci
