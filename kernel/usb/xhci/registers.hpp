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

union PORTSC_Bitmap {
  uint32_t data[1];
  struct {
    uint32_t current_connect_status : 1;
    uint32_t port_enabled_disabled : 1;
    uint32_t : 1;
    uint32_t over_current_active : 1;
    uint32_t port_reset : 1;
    uint32_t port_link_state : 4;
    uint32_t port_power : 1;
    uint32_t port_speed : 4;
    uint32_t port_indicator_control : 2;
    uint32_t port_link_state_write_strobe : 1;
    uint32_t connect_status_change : 1;
    uint32_t port_enabled_disabled_change : 1;
    uint32_t warm_port_reset_change : 1;
    uint32_t over_current_change : 1;
    uint32_t port_reset_change : 1;
    uint32_t port_link_state_change : 1;
    uint32_t port_config_error_change : 1;
    uint32_t cold_attach_status : 1;
    uint32_t wake_on_connect_enable : 1;
    uint32_t wake_on_disconnect_enable : 1;
    uint32_t wake_on_over_current_enable : 1;
    uint32_t : 2;
    uint32_t device_removable : 1;
    uint32_t warm_port_reset : 1;
  } __attribute__((packed)) bits;
} __attribute__((packed));

union PORTPMSC_Bitmap {
  uint32_t data[1];
  struct {  // definition for USB3 protocol
    uint32_t u1_timeout : 8;
    uint32_t u2_timeout : 8;
    uint32_t force_link_pm_accept : 1;
    uint32_t : 15;
  } __attribute__((packed)) bits_usb3;
} __attribute__((packed));

union PORTLI_Bitmap {
  uint32_t data[1];
  struct {  // definition for USB3 protocol
    uint32_t link_error_count : 16;
    uint32_t rx_lane_count : 4;
    uint32_t tx_lane_count : 4;
    uint32_t : 8;
  } __attribute__((packed)) bits_usb3;
} __attribute__((packed));

union PORTHLPMC_Bitmap {
  uint32_t data[1];
  struct {  // definition for USB2 protocol
    uint32_t host_initiated_resume_duration_mode : 2;
    uint32_t l1_timeout : 8;
    uint32_t best_effort_service_latency_deep : 4;
    uint32_t : 18;
  } __attribute__((packed)) bits_usb2;
} __attribute__((packed));

struct PortRegisterSet {
  MemMapRegister<PORTSC_Bitmap> PORTSC;
  MemMapRegister<PORTPMSC_Bitmap> PORTPMSC;
  MemMapRegister<PORTLI_Bitmap> PORTLI;
  MemMapRegister<PORTHLPMC_Bitmap> PORTHLPMC;
} __attribute__((packed));

}  // namespace ndifixos::usb::xhci
