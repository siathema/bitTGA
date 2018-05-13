#include <stdlib.h>
#include <stdio.h>

#include "mAssert.h"
#include "TGA_Lib.h"

namespace TGA_Lib
{

#define nColorToByte(n) (n >= 1.0f ? 0xff : ( n <= 0.0f ? 0.0f : (u8)(n*0xFF)))

  TGA_Color::TGA_Color(r32 r, r32 b, r32 g, r32 a) {
    this->r = nColorToByte(r);
    this->g = nColorToByte(b);
    this->b = nColorToByte(g);
    this->a = nColorToByte(a);
  }

  TGA_Color::TGA_Color(TGA_Color& c) {
    value = c.value;
  }

  TGA_File* TGA_Generate_File(i32 width, i32 height) {
    TGA_File* file = (TGA_File*)calloc(1, sizeof(TGA_File));
    file->Header.Image_Type = 2;
    file->Header.Image_Spec_Width = width;
    file->Header.Image_Spec_Height = height;
    file->Header.Image_Spec_Pixel_Depth = 32;
    file->Header.Image_Spec_Image_Desc = 0x10;

    i32 dataSize = width*height*4;
    file->data.Image_Data = (u8*)malloc(dataSize);

    for(i32 i=0; i<width*height; i++) {
      file->data.Image_Data[i*4] = 0xff;//blue
      file->data.Image_Data[i*4+1] = 0xff;//green
      file->data.Image_Data[i*4+2] = 0xff;//red
      file->data.Image_Data[i*4+3] = 0xff;//alpha
    }

    return file;
  }

  void TGA_Put_Pixel(TGA_File* file, i32 x, i32 y, TGA_Color color) {
    i32 img_width = file->Header.Image_Spec_Width;
    i32 img_height = file->Header.Image_Spec_Height;
    m_assert(x < img_width && y < img_height);

    i32 index = (((img_width*img_height)-1)*4)-(((y*img_width)+x)*4);
    u8* pixels = file->data.Image_Data;

    pixels[index] = color.b;
    pixels[index+1] = color.g;
    pixels[index+2] = color.r;
    pixels[index+3] = color.a;

  }

  void TGA_Write_To_File(TGA_File* file, const char* path) {
    FILE* outStream = fopen(path, "w");
    if(outStream == nullptr)
      printf("Could not create file!\n");

    fwrite(&file->Header, sizeof(TGA_Header), 1, outStream);
    i32 imageDataSize = file->Header.Image_Spec_Width * file->Header.Image_Spec_Height * (file->Header.Image_Spec_Pixel_Depth/8);
    fwrite(file->data.Image_Data, imageDataSize, 1, outStream);

    fclose(outStream);
  }

}
