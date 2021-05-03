#include "device_manager.hpp"

#include "../memory.hpp"

namespace ndifixos::usb::xhci {
status::Status DeviceManager::Initialize(size_t num_devices) {
  this->num_devices = num_devices;

  devices = AllocArray<Device>(num_devices, 0, 0);
  if (devices == nullptr) {
    return status::Status::NoEnoughMemory;
  }

  device_context_pointers = AllocArray<DeviceContext*>(num_devices + 1, 0, 0);
  if (device_context_pointers == nullptr) {
    FreeMem(devices);
    return status::Status::NoEnoughMemory;
  }

  for (size_t i = 0; i < num_devices; ++i) {
    devices[i].Initialize();
    device_context_pointers[i + 1] = nullptr;
  }
  device_context_pointers[0] = nullptr;  // scratch pad buffer

  return status::Status::Success;
}

DeviceContext** DeviceManager::DeviceContexts() const {
  return device_context_pointers;
}

Device* DeviceManager::FindByPort(uint8_t port_num,
                                  uint32_t route_string) const {
  for (size_t i = 0; i < num_devices; ++i) {
    auto dev = &devices[i];
    if (dev->DeviceContext()->slot_context.bits.root_hub_port_num == port_num) {
      return dev;
    }
  }
  return nullptr;
}

Device* DeviceManager::FindByState(enum Device::State state) const {
  for (size_t i = 0; i < num_devices; ++i) {
    auto dev = &devices[i];
    if (dev->State() == state) {
      return dev;
    }
  }
  return nullptr;
}

Device* DeviceManager::FindBySlot(uint8_t slot_id) const {
  for (size_t i = 0; i < num_devices; ++i) {
    auto dev = &devices[i];
    if (dev->SlotID() == slot_id) {
      return dev;
    }
  }
  return nullptr;
}

status::ValWithStatus<Device*> DeviceManager::Get(uint8_t device_id) const {
  if (device_id >= num_devices) {
    return {nullptr, status::Status::InvalidDeviceId};
  }
  return {&devices[device_id], status::Status::Success};
}

status::Status DeviceManager::AssignSlot(Device* dev, uint8_t slot_id) {
  dev->AssignSlot(slot_id);
  device_context_pointers[slot_id] = dev->DeviceContext();

  return status::Status::Success;
}

status::Status DeviceManager::Remove(Device* dev) {
  device_context_pointers[dev->SlotID()] = nullptr;
  dev->Initialize();
  return status::Status::Success;
}
}  // namespace ndifixos::usb::xhci
