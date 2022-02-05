#pragma once

#include "memory_map.hpp"

namespace ndifixos {
namespace memory {
using ull = unsigned long long;

const ull KiByte = 1024;
const ull MiByte = 1024 * KiByte;
const ull GiByte = 1024 * MiByte;
const ull BytesPerFrame = 4 * KiByte;

void InitMemoryManager(const MemoryMap& memory_map);

class MemoryManager {};

extern MemoryManager* memoryManager;
}  // namespace memory
}  // namespace ndifixos
