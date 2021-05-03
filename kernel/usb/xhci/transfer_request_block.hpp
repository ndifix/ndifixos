#pragma once

#include <array>
#include <cstdint>

namespace ndifixos::usb::xhci {
// 実装はtransfer_request_block.cpp
extern const std::array<const char*, 37> TRBCompletionCodeToName;
extern const std::array<const char*, 64> TRBTypeToName;

static const unsigned int LinkTRBType = 6;

union TRB {
  std::array<uint32_t, 4> data{};
  struct {
    uint64_t parameter;
    uint32_t status;
    uint32_t cycle_bit : 1;
    uint32_t evaluate_next_trb : 1;
    uint32_t : 8;
    uint32_t trb_type : 6;
    uint32_t control : 16;
  } __attribute__((packed)) bits;
};

union LinkTRB {
  static const unsigned int Type = LinkTRBType;
  std::array<uint32_t, 4> data{};
  struct {
    uint64_t : 4;
    uint64_t ring_segment_pointer : 60;

    uint32_t : 22;
    uint32_t interrupter_target : 10;

    uint32_t cycle_bit : 1;
    uint32_t toggle_cycle : 1;
    uint32_t : 2;
    uint32_t chain_bit : 1;
    uint32_t interrupt_on_completion : 1;
    uint32_t : 4;
    uint32_t trb_type : 6;
    uint32_t : 16;
  } __attribute__((packed)) bits;

  LinkTRB(const TRB* ring_segment_pointer) {
    bits.trb_type = Type;
    SetPointer(ring_segment_pointer);
  }

  TRB* Pointer() const {
    return reinterpret_cast<TRB*>(bits.ring_segment_pointer << 4);
  }

  void SetPointer(const TRB* p) {
    bits.ring_segment_pointer = reinterpret_cast<uint64_t>(p) >> 4;
  }
};

}  // namespace ndifixos::usb::xhci
