#pragma once

#include "../../register.hpp"

// xHCI のレジスタ定義
namespace ndifixos::usb::xhci {

union IMAN_Bitmap {
  uint32_t data[1];
  struct {
    uint32_t interrupt_pending : 1;
    uint32_t interrupt_enable : 1;
    uint32_t : 30;
  } __attribute__((packed)) bits;
} __attribute__((packed));

union IMOD_Bitmap {
  uint32_t data[1];
  struct {
    uint32_t interrupt_moderation_interval : 16;
    uint32_t interrupt_moderation_counter : 16;
  } __attribute__((packed)) bits;
} __attribute__((packed));

union ERSTSZ_Bitmap {
  uint32_t data[1];
  struct {
    uint32_t event_ring_segment_table_size : 16;
    uint32_t : 16;
  } __attribute__((packed)) bits;

  uint16_t Size() const { return bits.event_ring_segment_table_size; }

  void SetSize(uint16_t value) { bits.event_ring_segment_table_size = value; }
} __attribute__((packed));

union ERSTBA_Bitmap {
  uint64_t data[1];
  struct {
    uint64_t : 6;
    uint64_t event_ring_segment_table_base_address : 58;
  } __attribute__((packed)) bits;

  uint64_t Pointer() const {
    return bits.event_ring_segment_table_base_address << 6;
  }

  void SetPointer(uint64_t value) {
    bits.event_ring_segment_table_base_address = value >> 6;
  }
} __attribute__((packed));

union ERDP_Bitmap {
  uint64_t data[1];
  struct {
    uint64_t dequeue_erst_segment_index : 3;
    uint64_t event_handler_busy : 1;
    uint64_t event_ring_dequeue_pointer : 60;
  } __attribute__((packed)) bits;

  uint64_t Pointer() const { return bits.event_ring_dequeue_pointer << 4; }

  void SetPointer(uint64_t value) {
    bits.event_ring_dequeue_pointer = value >> 4;
  }
} __attribute__((packed));

struct InterrupterRegisterSet {
  MemMapRegister<IMAN_Bitmap> IMAN;
  MemMapRegister<IMOD_Bitmap> IMOD;
  MemMapRegister<ERSTSZ_Bitmap> ERSTSZ;
  uint32_t reserved;
  MemMapRegister<ERSTBA_Bitmap> ERSTBA;
  MemMapRegister<ERDP_Bitmap> ERDP;
} __attribute__((packed));

}  // namespace ndifixos::usb::xhci
