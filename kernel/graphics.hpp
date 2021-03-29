#pragma once

#include <cstdint>

#include "frame_buffer_config.hpp"

namespace ndifixos {
namespace graphics {

struct PixelColor {
  uint8_t R, G, B;
};

// ピクセル毎に描画を管理します
class PixelWriter {
 private:
  frameBuffer::FrameBufferConfig config;

 protected:
  uint8_t* PixelAt(int x, int y) const {
    return config.frame_buffer_base + 4 * (config.pixels_per_scan_line * y + x);
  }

 public:
  PixelWriter() {}
  PixelWriter(const frameBuffer::FrameBufferConfig& c) : config(c) {}
  ~PixelWriter() = default;

  // 指定された1ピクセルを指定した色で描画します
  void Write(int x, int y, PixelColor c) const {
    auto p = PixelAt(x, y);
    switch (config.pixel_format) {
      case frameBuffer::kPixelRGBResv8BitPerColor:
        p[0] = c.R;
        p[1] = c.G;
        p[2] = c.B;
        break;
      case frameBuffer::kPixelBGResv8BitPerColor:
        p[0] = c.B;
        p[1] = c.G;
        p[2] = c.R;
        break;
    }
  }

  void PrintScreen(PixelColor c) const {
    for (int x = 0; x < config.h_resolution; ++x) {
      for (int y = 0; y < config.v_resolution; ++y) {
        Write(x, y, c);
      }
    }
  }
};

}  // namespace graphics
}  // namespace ndifixos
