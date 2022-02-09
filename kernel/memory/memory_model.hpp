#pragma once
#include <cstdint>

namespace ndifixos {
namespace memory {
const uint64_t KiByte = 1024;
const uint64_t MiByte = 1024 * KiByte;
const uint64_t GiByte = 1024 * MiByte;
const uint64_t BytesPerFrame = 4 * KiByte;

// フレームを32個ずつ管理する
// 各bit でフレームの使用状況を管理する
// 0 は割り当て不可、1 は割り当て可
using FrameGroup = uint32_t;
const uint64_t FramesPerGroup = 32;
const uint64_t BytesPerFrameGroup = BytesPerFrame * FramesPerGroup;

}
}  // namespace ndifixos
