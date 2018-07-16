#include "TGA_Lib.h"
#include <stdlib.h>
#include <stdio.h>

int test() {
  TGA_File* file = TGA_File_Generate(10, 10);
  TGA_Color color = TGA_Colorf(1.0f, 0.f, 0.f, 1.f);
  TGA_Put_Pixel(file, 9, 9, color);
  TGA_File_Write(file, "test.tga");

  color = TGA_Colorf(0.4f, 1.0f, 0.123f, 1.0f);
  TGA_Image_Clear_To_Color(file, color);
  TGA_Put_Pixel(file, 1, 3, {0,255,0,255});
  TGA_Image_Flip_Vertical(file);
  TGA_File_Write(file, "test01.tga");

  printf("color: r:%d, g:%d, b:%d, a:%d\n", color.r, color.g, color.b, color.a);

  TGA_File_Destroy(file);
  return 0;
}

int main(int argc, char** argv) {
  return test();
}
