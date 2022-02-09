#pragma once

#include <array>
#include <cstdint>
#include <tuple>

#include "memory_map.hpp"
#include "memory_model.hpp"

namespace ndifixos {
namespace memory {

void InitMemoryManager(const MemoryMap& memory_map);

namespace {
// 物理メモリの使用状況を管理する
class MemoryManager {
 private:
  // 対応する物理メモリの最大量
  inline constexpr static uint64_t maxPhysMemSize = 64 * GiByte;
  // 最大メモリに対応する最大フレームグループ数
  inline constexpr static uint64_t maxFrameGroups = maxPhysMemSize / BytesPerFrame / 32;
  // フレームグループが割り当て可能かを表す
  std::array<FrameGroup, maxFrameGroups> physMemAvailable;
  // 割り当て可能なフレームが存在するグループの範囲
  FrameGroup &firstGroup, lastGroup;

 public:
  // インスタンスを初期化する
  MemoryManager();
  // フレームグループに値をセットする
  void SetAvailable(uintptr_t physStartAddr, uint64_t uefiPages);
};
}  // namespace

extern MemoryManager* memoryManager;
}  // namespace memory
}  // namespace ndifixos
