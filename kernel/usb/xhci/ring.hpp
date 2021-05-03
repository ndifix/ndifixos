#pragma once

#include <cstdint>

#include "../../status.hpp"
#include "../memory.hpp"
#include "registers.hpp"
#include "transfer_request_block.hpp"

// Event Ring, Command Ring, Transfer Ring のクラスや関連機能
namespace ndifixos::usb::xhci {

// Command/Transfer Ring
class Ring {
 private:
  TRB* buf = nullptr;
  size_t buf_size = 0;

  // プロデューサ・サイクル・ステートを表すビット
  bool cycle_bit;
  // リング上で次に書き込む位置
  size_t write_index;

  /** @brief TRB に cycle bit を設定した上でリング末尾に書き込む．
   *
   * write_index は変化させない．
   */
  void CopyToLast(const std::array<uint32_t, 4>& data);

  /** @brief TRB に cycle bit を設定した上でリング末尾に追加する．
   *
   * write_index_ をインクリメントする．その結果 write_index がリング末尾
   * に達したら LinkTRB を適切に配置して write_index を 0 に戻し，
   * cycle bit を反転させる．
   */
  void Push(const std::array<uint32_t, 4>& data);

 public:
  Ring() = default;
  Ring(const Ring&) = delete;
  ~Ring();
  Ring& operator=(const Ring&) = delete;

  // リングのメモリ領域を割り当て，メンバを初期化する
  status::Status Initialize(size_t buf_size);

  // TRB に cycle bit を設定した上でリング末尾に追加する
  template <typename TRBType>
  void Push(const TRBType& trb) {
    Push(trb.data);
  }

  TRB* Buffer() const { return buf; }
};

union EventRingSegmentTableEntry {
  std::array<uint32_t, 4> data;
  struct {
    uint64_t ring_segment_base_address;  // 64 バイトアライメント

    uint32_t ring_segment_size : 16;
    uint32_t : 16;

    uint32_t : 32;
  } __attribute__((packed)) bits;
};

class EventRing {
 private:
  TRB* buf;
  size_t buf_size;

  bool cycle_bit;
  EventRingSegmentTableEntry* erst;
  InterrupterRegisterSet* interrupter;

 public:
  status::Status Initialize(size_t buf_size,
                            InterrupterRegisterSet* interrupter);

  TRB* ReadDequeuePointer() const {
    return reinterpret_cast<TRB*>(interrupter->ERDP.Read().Pointer());
  }

  void WriteDequeuePointer(TRB* p);

  bool HasFront() const { return Front()->bits.cycle_bit == cycle_bit; }

  TRB* Front() const { return ReadDequeuePointer(); }

  void Pop();
};
}  // namespace ndifixos::usb::xhci
