#pragma once

#include <cstdint>

#include "../../status.hpp"
#include "context.hpp"
#include "transfer_request_block.hpp"

namespace ndifixos::usb::xhci {
// USB デバイスを表すクラスと関連機能．
class Device {
 public:
  enum class State { Invalid, Blank, SlotAssigning, SlotAssigned };

  status::Status Initialize();

  DeviceContext* DeviceContext() { return &device_ctx; }
  InputContext* InputContext() { return &input_ctx; }

  State State() const { return state; }
  uint8_t SlotID() const { return slot_id; }

  void SelectForSlotAssignment();
  void AssignSlot(uint8_t slot_id);
  status::Status AllocTransferRing(DeviceContextIndex index, size_t buf_size);

 private:
  alignas(64) struct DeviceContext device_ctx;
  alignas(64) struct InputContext input_ctx;

  enum State state;
  uint8_t slot_id;
};
}  // namespace ndifixos::usb::xhci
