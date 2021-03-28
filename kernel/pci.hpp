#pragma once

#include <array>
#include <cstdint>

#include "asmfunc.h"
#include "status.hpp"

namespace ndifixos {
namespace pci {
// PCI デバイスを操作するための基礎データを格納する
class Device {
 private:
  uint8_t bus, device_num, function, header_type;
  uint16_t device_id, vender_id;
  uint32_t class_code;

 public:
  Device() {}
  Device(uint8_t bus, uint8_t device_num, uint8_t function, uint8_t header_type,
         uint16_t device_id, uint16_t vender_id, uint32_t class_code) {
    this->bus = bus;
    this->device_num = device_num;
    this->function = function;
    this->header_type = header_type;
    this->device_id = device_id;
    this->vender_id = vender_id;
    this->class_code = class_code;
  }

  inline uint8_t Bus() { return bus; }
  inline uint8_t Device_num() { return device_num; }
  inline uint8_t Function() { return function; }
  inline uint8_t Header_type() { return header_type; }
  inline uint16_t Device_id() { return device_id; }
  inline uint16_t Vender_id() { return vender_id; }
  inline uint32_t Class_code() { return class_code; }
};

class PCIio {
 private:
  // CONFIG_ADDRESS レジスタの IO ポートアドレス
  const uint16_t ConfigAddress = 0x0cf8;
  // CONFIG_DATA レジスタの IO ポートアドレス
  const uint16_t ConfigData = 0x0cfc;

  // CONFIG_ADDRESS 用の 32 ビット整数を生成する
  inline uint32_t MakeAddress(uint8_t bus, uint8_t device, uint8_t function,
                              uint8_t reg_addr) {
    return (1 << 31) | (bus << 16) | (device << 11) | (function << 8) |
           (reg_addr & 0xfcu);
  }

  // CONFIG_ADDRESS, CONFIG_DATA に指定された整数を書き込む
  // asmfunc.h 参照
  inline void SetAddress(uint32_t address) { IoOut32(ConfigAddress, address); }
  inline void WriteData(uint32_t value) { IoOut32(ConfigData, value); }
  // CONFIG_DATA から 32 bit整数を読み込む
  inline uint32_t ReadData() { return IoIn32(ConfigData); }

 public:
  inline bool IsSingleFunctionDevice(uint8_t header_type) {
    return (header_type & 0x80u) == 0;
  }

  // ベンダ ID レジスタを読み取る (全ヘッダタイプ共通)
  uint16_t ReadVendorId(uint8_t bus, uint8_t device, uint8_t function);

  // デバイス ID レジスタを読み取る (全ヘッダタイプ共通)
  uint16_t ReadDeviceId(uint8_t bus, uint8_t device, uint8_t function);

  //クラスコードレジスタを読み取る (全ヘッダタイプ共通)
  /* ______________________________________________
   * |31      24|23     16|15      8|7          0|
   *  base class sub class interface  revision id
   */
  uint32_t ReadClassCode(uint8_t bus, uint8_t device, uint8_t function);

  // ヘッダタイプレジスタを読み取る（全ヘッダタイプ共通）
  uint8_t ReadHeaderType(uint8_t bus, uint8_t device, uint8_t function);

  //  バス番号レジスタを読み取る（ヘッダタイプ 1 用）
  /* ______________________________________________________________
   * |31  24|23　　　 　　　　  16|15                 8|7           0|
   *         subordinate bus num  secoundary path num  revision num
   */
  uint32_t ReadBusNumbers(uint8_t bus, uint8_t device, uint8_t function);
};

class PCIManager {
  using Status = status::Status;

 private:
  static const size_t max_devices = 32;
  // 有効な PCI device
  std::array<Device, max_devices> devices;
  // 有効なdevice数
  int num_device;

  PCIio pci_io;

  // devices[num_device] に情報を書き込み num_device をインクリメントする
  Status AddDevice(uint8_t bus, uint8_t device, uint8_t function,
                   uint8_t header_type);

  // 指定のファンクションを devices に追加する．
  // もし PCI-PCI ブリッジなら，セカンダリバスに対し ScanBus を実行する
  Status ScanFunction(uint8_t bus, uint8_t device, uint8_t function);

  /** @brief 指定のデバイス番号の各ファンクションをスキャンする．
   * 有効なファンクションを見つけたら ScanFunction を実行する．
   */
  Status ScanDevice(uint8_t bus, uint8_t device);

  // 指定のバス番号の各デバイスをスキャンする.
  // 有効なデバイスを見つけたら ScanDevice を実行する.
  Status ScanBus(uint8_t bus);

 public:
  inline int DeviceCount() { return num_device; }
  inline Device device(int i) { return this->devices[i]; }
  /*
   * PCI デバイスをすべて探索し devices に格納する
   * バス 0 から再帰的に PCI デバイスを探索し devices の先頭から詰めて書き込む．
   * 発見したデバイスの数を num_devices に設定する．
   */
  Status ScanAllBus();
};

}  // namespace pci
}  // namespace ndifixos
