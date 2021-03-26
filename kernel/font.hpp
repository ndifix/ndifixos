#pragma once

#include <cstdint>

#include "graphics.hpp"

extern const uint8_t _binary_ascii_bin_start;
extern const uint8_t _binary_ascii_bin_end;
extern const uint8_t _binary_ascii_bin_size;

namespace ndifixos_font {
// ascii.o を用いて16*8px のイメージを取得します
const uint8_t* GetFont(char c) {
  auto index = 16 * static_cast<unsigned int>(c);
  if (index >= reinterpret_cast<uintptr_t>(&_binary_ascii_bin_size)) {
    return nullptr;
  }
  return &_binary_ascii_bin_start + index;
}

void WriteAscii(ndifixos_graphics::PixelWriter& writer, int x, int y, char c,
                const ndifixos_graphics::PixelColor& color = {0xff, 0xff,
                                                              0xff}) {
  const uint8_t* font = GetFont(c);

  for (int dy = 0; dy < 16; ++dy) {
    for (int dx = 0; dx < 8; ++dx) {
      if (font[dy] << dx & 0x80u) {
        writer.Write(x + dx, y + dy, color);
      }
    }
  }
}

void WriteString(ndifixos_graphics::PixelWriter& writer, int x, int y, char* c,
                 const ndifixos_graphics::PixelColor& color = {0xff, 0xff,
                                                               0xff}) {
  for (char* pc = c; *pc != '\0'; pc++) {
    WriteAscii(writer, x, y, *pc, color);
    x += 9;
  }
}
}  // namespace ndifixos_font
