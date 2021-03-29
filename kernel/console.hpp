#pragma once

#include <cstdio>

#include "graphics.hpp"

namespace ndifixos_console {
class Console {
 private:
  // 文字サイズ
  static const int char_height = 16, char_width = 8;
  // 表示する文字数
  static const int Rows = 25, Collumns = 80;
  ndifixos_graphics::PixelWriter writer;
  ndifixos_graphics::PixelColor back_c, char_c;
  char buffer[Rows][Collumns + 1];
  int cursor_R, cursor_C;

  void NewLine();

  void Print(const char* s);

 public:
  Console(const ndifixos_graphics::PixelWriter& writer,
          ndifixos_graphics::PixelColor back_color = {0, 0, 0},
          ndifixos_graphics::PixelColor char_color = {0xff, 0xff, 0xff})
      : buffer{} {
    this->writer = writer;
    back_c = back_color;
    char_c = char_color;
    cursor_R = cursor_C = 0;
  }

  int Write(const char* format, ...);
};
}  // namespace ndifixos_console
