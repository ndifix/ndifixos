#pragma once

#include <stdint.h>

enum PixelFormat { kPixelRGBResv8BitPerColor, kPixelBGResv8BitPerColor };

struct FrameBufferConfig {
  uint8_t* frame_buffer_base;
  uint32_t frame_buffer_size;
  uint32_t pixels_per_scan_line;
  uint32_t h_resolution;
  uint32_t v_resolution;
  enum PixelFormat pixel_format;
};
