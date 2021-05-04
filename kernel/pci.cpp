#include "pci.hpp"

// PCI バス制御のプログラムを集めたファイル
namespace ndifixos {
namespace pci {

// ベンダ ID レジスタを読み取る (全ヘッダタイプ共通)
uint16_t PCIio::ReadVendorId(uint8_t bus, uint8_t device, uint8_t function) {
  SetAddress(MakeAddress(bus, device, function, 0x00));
  return ReadData() & 0xffffu;
}

// デバイス ID レジスタを読み取る (全ヘッダタイプ共通)
uint16_t PCIio::ReadDeviceId(uint8_t bus, uint8_t device, uint8_t function) {
  SetAddress(MakeAddress(bus, device, function, 0x00));
  return ReadData() >> 16;
}

//クラスコードレジスタを読み取る (全ヘッダタイプ共通)
/* ______________________________________________
 * |31      24|23     16|15      8|7          0|
 *  base class sub class interface  revision id
 */
uint32_t PCIio::ReadClassCode(uint8_t bus, uint8_t device, uint8_t function) {
  SetAddress(MakeAddress(bus, device, function, 0x08));
  return ReadData();
}

// ヘッダタイプレジスタを読み取る（全ヘッダタイプ共通）
uint8_t PCIio::ReadHeaderType(uint8_t bus, uint8_t device, uint8_t function) {
  SetAddress(MakeAddress(bus, device, function, 0x0c));
  return (ReadData() >> 16) & 0xffu;
}

//  バス番号レジスタを読み取る（ヘッダタイプ 1 用）
/* ______________________________________________________________
 * |31  24|23　　　 　　　　  16|15                 8|7           0|
 *         subordinate bus num  secoundary path num  revision num
 */
uint32_t PCIio::ReadBusNumbers(uint8_t bus, uint8_t device, uint8_t function) {
  SetAddress(MakeAddress(bus, device, function, 0x18));
  return ReadData();
}

uint32_t PCIio::ReadConfReg(const Device& dev, uint8_t reg_addr) {
  SetAddress(
      MakeAddress(dev.Bus(), dev.Device_num(), dev.Function(), reg_addr));
  return ReadData();
}

using Status = status::Status;
// devices[num_device] に情報を書き込み num_device をインクリメントする
Status PCIManager::AddDevice(uint8_t bus, uint8_t device, uint8_t function,
                             uint8_t header_type) {
  if (num_device == devices.size()) {
    return Status::Full;
  }

  uint16_t device_id = pci_io.ReadDeviceId(bus, device, function);
  uint16_t vender_id = pci_io.ReadVendorId(bus, device, function);
  uint32_t code = pci_io.ReadClassCode(bus, device, function);
  ClassCode class_code((code & 0xff000000) >> 24, (code & 0x00ff0000) >> 16,
                       (code & 0x0000ff00) >> 8);
  Device newdevice(bus, device, function, header_type, device_id, vender_id,
                   class_code);
  devices[num_device] = newdevice;
  ++num_device;
  return Status::Success;
}

// 指定のファンクションを devices に追加する．
// もし PCI-PCI ブリッジなら，セカンダリバスに対し ScanBus を実行する
Status PCIManager::ScanFunction(uint8_t bus, uint8_t device, uint8_t function) {
  auto header_type = pci_io.ReadHeaderType(bus, device, function);
  Status status = AddDevice(bus, device, function, header_type);
  if (!status.isSuccess()) {
    return status;
  }

  auto class_code = pci_io.ReadClassCode(bus, device, function);
  uint8_t base = (class_code >> 24) & 0xffu;
  uint8_t sub = (class_code >> 16) & 0xffu;

  if (base == 0x06u && sub == 0x04u) {
    // standard PCI-PCI bridge
    auto bus_numbers = pci_io.ReadBusNumbers(bus, device, function);
    uint8_t secondary_bus = (bus_numbers >> 8) & 0xffu;
    return ScanBus(secondary_bus);
  }

  return Status::Success;
}

/** @brief 指定のデバイス番号の各ファンクションをスキャンする．
 * 有効なファンクションを見つけたら ScanFunction を実行する．
 */
Status PCIManager::ScanDevice(uint8_t bus, uint8_t device) {
  Status status = ScanFunction(bus, device, 0);
  if (!status.isSuccess()) {
    return status;
  }

  if (pci_io.IsSingleFunctionDevice(pci_io.ReadHeaderType(bus, device, 0))) {
    return Status::Success;
  }

  for (uint8_t function = 1; function < 8; ++function) {
    if (pci_io.ReadVendorId(bus, device, function) == 0xffffu) {
      continue;
    }
    status = ScanFunction(bus, device, function);
    if (!status.isSuccess()) {
      return status;
    }
  }
  return Status::Success;
}

// 指定のバス番号の各デバイスをスキャンする.
// 有効なデバイスを見つけたら ScanDevice を実行する.
Status PCIManager::ScanBus(uint8_t bus) {
  for (uint8_t device = 0; device < 32; ++device) {
    if (pci_io.ReadVendorId(bus, device, 0) == 0xffffu) {
      continue;
    }
    Status status = ScanDevice(bus, device);
    if (!status.isSuccess()) {
      return status;
    }
  }
  return Status::Success;
}

/*
 * PCI デバイスをすべて探索し devices に格納する
 * バス 0 から再帰的に PCI デバイスを探索し devices の先頭から詰めて書き込む．
 * 発見したデバイスの数を num_devices に設定する．
 */
Status PCIManager::ScanAllBus() {
  num_device = 0;

  // ホストブリッジの確認
  auto header_type = pci_io.ReadHeaderType(0, 0, 0);
  if (pci_io.IsSingleFunctionDevice(header_type)) {
    return ScanBus(0);
  }

  for (uint8_t function = 1; function < 8; ++function) {
    if (pci_io.ReadVendorId(0, 0, function) == 0xffffu) {
      continue;
    }
    Status status = ScanBus(function);
    if (!status.isSuccess()) {
      return status;
    }
  }
  return Status::Success;
}

// Base Address Register n を読みます
PCIManager::ValWithStatus PCIManager::ReadBar(Device& device,
                                              unsigned int bar_index) {
  if (bar_index >= 6) {
    return {0, Status::OutOfRange};
  }

  auto CalcBarAddress = [](unsigned int b) { return 0x10 + 4 * b; };
  const auto addr = CalcBarAddress(bar_index);
  const auto bar = pci_io.ReadConfReg(device, addr);

  // 32 bit address
  if ((bar & 4u) == 0) {
    return {bar, Status::Success};
  }

  // 64 bit address
  if (bar_index >= 5) {
    return {0, Status::OutOfRange};
  }

  const auto bar_upper = pci_io.ReadConfReg(device, addr + 4);
  return {bar | (static_cast<uint64_t>(bar_upper) << 32), Status::Success};
}

}  // namespace pci
}  // namespace ndifixos
