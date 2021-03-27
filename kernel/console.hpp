#pragma once

#include <cstring>

#include "font.hpp"
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

  void NewLine() {
    cursor_C = 0;
    if (cursor_R == Rows - 1) {
      // 画面を塗りつぶしてから
      // 全ての文字を上に一段上げる。
      for (int x = 0; x < char_width * Collumns; ++x) {
        for (int y = 0; y < char_height * Rows; ++y) {
          writer.Write(x, y, back_c);
        }
      }

      for (int row = 0; row < Rows - 1; ++row) {
        memcpy(buffer[row], buffer[row + 1], Collumns + 1);
        ndifixos_font::WriteString(writer, 0, char_height * row, buffer[row],
                                   char_c);
      }
      memset(buffer[Rows - 1], 0, Collumns + 1);
    } else {
      ++cursor_R;
    }
  }

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

  void Write(const char* s) {
    while (*s) {
      if (*s == '\n' || cursor_C == Collumns) {
        NewLine();
      } else {
        ndifixos_font::WriteAscii(writer, (char_width + 1) * cursor_C,
                                  char_height * cursor_R, *s, char_c);
        buffer[cursor_R][cursor_C] = *s;
        ++cursor_C;
      }
      ++s;
    }
  }
};
}  // namespace ndifixos_console
