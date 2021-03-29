#include "console.hpp"

#include <cstdio>
#include <cstring>

#include "font.hpp"
#include "graphics.hpp"

namespace ndifixos_console {
void Console::NewLine() {
  cursor_C = 0;
  if (cursor_R == Rows - 1) {
    // 画面を塗りつぶしてから
    // 全ての文字を上に一段上げる。
    for (int x = 0; x < (char_width + 1) * Collumns; ++x) {
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

void Console::Print(const char* s) {
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

int Console::Write(const char* format, ...) {
  va_list ap;
  int result;
  char s[1024];

  va_start(ap, format);
  result = vsprintf(s, format, ap);
  va_end(ap);

  Print(s);
  return result;
}

}  // namespace ndifixos_console
