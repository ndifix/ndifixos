#pragma once

#include <cstdint>

#include "../../status.hpp"
#include "context.hpp"
#include "device.hpp"

namespace ndifixos::usb::xhci {
// USB デバイスの管理機能

class DeviceManager {
 private:
  // device_context_pointers_ can be used as DCBAAP's value.
  // The number of elements is num_devices_ + 1.
  DeviceContext** device_context_pointers;
  Device* devices;
  size_t num_devices;

 public:
  status::Status Initialize(size_t num_devices);
  DeviceContext** DeviceContexts() const;
  Device* FindByPort(uint8_t port_num, uint32_t route_string) const;
  Device* FindByState(enum Device::State state) const;
  Device* FindBySlot(uint8_t slot_id) const;
  status::ValWithStatus<Device*> Get(uint8_t device_id) const;
  status::Status AssignSlot(Device* dev, uint8_t slot_id);
  status::Status Remove(Device* dev);
};
}  // namespace ndifixos::usb::xhci
