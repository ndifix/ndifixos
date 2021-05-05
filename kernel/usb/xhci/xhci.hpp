#pragma once

#include <cstdint>

#include "../../status.hpp"
#include "context.hpp"
#include "device_manager.hpp"
#include "port.hpp"
#include "registers.hpp"
#include "ring.hpp"

namespace ndifixos {
namespace usb {
namespace xhci {

class Controller {
 private:
  static const size_t deviceSize = 8;

  const uintptr_t mmio_base;
  CapabilityRegisters* const cap_reg;
  OperationalRegisters* const ope_reg;
  const uint8_t max_ports;

  class DeviceManager device_manager;
  Ring ring;
  EventRing event_ring;

  InterrupterRegisterSetArray InterrupterRegisterSets() const {
    return {mmio_base + cap_reg->RTSOFF.Read().Offset() + 0x20u, 1024};
  }

  PortRegisterSetArray PortRegisterSets() const {
    return {reinterpret_cast<uintptr_t>(ope_reg) + 0x400u, max_ports};
  }

  DoorbellRegisterArray DoorbellRegisters() const {
    return {mmio_base + cap_reg->DBOFF.Read().Offset(), 256};
  }

  status::Status ResetPort(Port& port);
  status::Status EnableSlot(Port& port);
  status::Status OnEvent(PortStatusChangeEventTRB& trb);

 public:
  Controller(uintptr_t mmio_base);
  status::Status Initialize();
  status::Status Run();
  Ring* CommandRing() { return &ring; }
  EventRing* PrimaryEventRing() { return &event_ring; }
  DoorbellRegister* DoorbellRegisterAt(uint8_t index);
  Port PortAt(uint8_t port_num) {
    return Port{port_num, PortRegisterSets()[port_num]};
  }
  uint8_t MaxPorts() const { return max_ports; }
  DeviceManager* DeviceManager() { return &device_manager; }

  status::Status ConfigurePort(Port& port);
  status::Status ProcessEvent();
};

}  // namespace xhci
}  // namespace usb
}  // namespace ndifixos
