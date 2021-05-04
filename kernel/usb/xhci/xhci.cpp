#include "xhci.hpp"

namespace {
using namespace ndifixos;
using namespace ndifixos::status;
using namespace ndifixos::usb::xhci;

Status RegisterCommandRing(Ring* ring, MemMapRegister<CRCR_Bitmap>* crcr) {
  CRCR_Bitmap value{};
  value.bits.ring_cycle_state = true;
  value.SetPointer(reinterpret_cast<uint64_t>(ring->Buffer()));
  crcr->Write(value);
  return Status::Success;
}
}  // namespace

namespace ndifixos::usb::xhci {

Controller::Controller(uintptr_t mmio_base)
    : mmio_base{mmio_base},
      cap_reg{reinterpret_cast<CapabilityRegisters*>(mmio_base)},
      ope_reg{reinterpret_cast<OperationalRegisters*>(
          mmio_base + cap_reg->CAPLENGTH.Read())},
      max_ports{
          static_cast<uint8_t>(cap_reg->HCSPARAMS1.Read().bits.max_ports)} {}

status::Status Controller::Initialize() {
  status::Status status = device_manager.Initialize(deviceSize);
  if (!status.isSuccess()) {
    return status;
  }

  // Host controller must be halted.
  if (!ope_reg->USBSTS.Read().bits.host_controller_halted) {
    return status::Status::HostControllerNotHalted;
  }

  // Reset controller
  auto usbcmd = ope_reg->USBCMD.Read();
  usbcmd.bits.host_controller_reset = true;
  ope_reg->USBCMD.Write(usbcmd);
  while (ope_reg->USBCMD.Read().bits.host_controller_reset)
    ;
  while (ope_reg->USBSTS.Read().bits.controller_not_ready)
    ;

  // printk("MaxSlots: %u\n", cap_->HCSPARAMS1.Read().bits.max_device_slots);
  // Set "Max Slots Enabled" field in CONFIG.
  auto config = ope_reg->CONFIG.Read();
  config.bits.max_device_slots_enabled = deviceSize;
  ope_reg->CONFIG.Write(config);

  DCBAAP_Bitmap dcbaap{};
  dcbaap.SetPointer(
      reinterpret_cast<uint64_t>(device_manager.DeviceContexts()));
  ope_reg->DCBAAP.Write(dcbaap);

  auto primary_interrupter = &InterrupterRegisterSets()[0];

  status = ring.Initialize(32);
  if (!status.isSuccess()) {
    return status;
  }
  status = RegisterCommandRing(&ring, &ope_reg->CRCR);
  if (!status.isSuccess()) {
    return status;
  }
  status = event_ring.Initialize(32, primary_interrupter);
  if (!status.isSuccess()) {
    return status;
  }

  // Enable interrupt for the primary interrupter
  auto iman = primary_interrupter->IMAN.Read();
  iman.bits.interrupt_pending = true;
  iman.bits.interrupt_enable = true;
  primary_interrupter->IMAN.Write(iman);

  // Enable interrupt for the controller
  usbcmd = ope_reg->USBCMD.Read();
  usbcmd.bits.interrupter_enable = true;
  ope_reg->USBCMD.Write(usbcmd);

  return status::Status::Success;
}

status::Status Controller::Run() {
  // Run the controller
  auto usbcmd = ope_reg->USBCMD.Read();
  usbcmd.bits.run_stop = true;
  ope_reg->USBCMD.Write(usbcmd);
  ope_reg->USBCMD.Read();

  while (ope_reg->USBSTS.Read().bits.host_controller_halted)
    ;

  return status::Status::Success;
}

DoorbellRegister* Controller::DoorbellRegisterAt(uint8_t index) {
  return &DoorbellRegisters()[index];
}
}  // namespace ndifixos::usb::xhci
