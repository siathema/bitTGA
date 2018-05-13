#include "TGA_Lib.h"
#include <stdlib.h>
#include <stdio.h>

namespace TGA_Lib {
  int test() {
    TGA_File* file = TGA_Generate_File(10, 10);
    TGA_Color color(1.0f, 0.f, 0.f, 1.f);
    TGA_Put_Pixel(file, 9, 9, color);
    TGA_Write_To_File(file, "test.tga");

    printf("color: r:%d, g:%d, b:%d, a:%d\n", color.r, color.g, color.b, color.a);

    free(file->data.Image_Data);
    free(file);
    return 0;
  }
}

int main(int argc, char** argv) {
  return TGA_Lib::test();
}
