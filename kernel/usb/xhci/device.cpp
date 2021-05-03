#include "device.hpp"

#include "../memory.hpp"
#include "ring.hpp"

namespace ndifixos::usb::xhci {

status::Status Device::Initialize() {
  state = State::Blank;
  for (size_t i = 0; i < 31; ++i) {
    const DeviceContextIndex dci(i + 1);
  }
  return status::Status::Success;
}

void Device::SelectForSlotAssignment() { state = State::SlotAssigning; }

void Device::AssignSlot(uint8_t slot_id) {
  this->slot_id = slot_id;
  state = State::SlotAssigned;
}

status::Status Device::AllocTransferRing(DeviceContextIndex index,
                                         size_t buf_size) {
  auto tr = AllocArray<Ring>(1, 64, 4096);
  tr->Initialize(buf_size);
  return status::Status::Success;
}
}  // namespace ndifixos::usb::xhci
