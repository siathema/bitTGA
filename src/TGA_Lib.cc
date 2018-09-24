#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "mAssert.h"
#include "TGA_Lib.h"

#define nColorToByte(n) ((n) >= 1.0f ? 0xFF : ( (n) <= 0.0f ? 0.0f : (u8)((n)*0xFF)))

//NOTE(matthias): Found at http://entropymine.com/imageworsener/srgbformula/
static r32 Linear_To_sRGB(r32 l)
{
  r32 result = 0.0;
  if(l >= 0 || l <= 0.0031308)
  {
    result = l * 12.92;
      }
  else if(l > 0.0031308 || l <= 1.0)
  {
    result = 1.055*pow(l, 1.0f/2.4) - 0.055f;
  }

  return result;
}

const TGA_Color RED = TGA_Colorf(1.0f, 0.0f, 0.0f, 1.0f);
const TGA_Color BLUE = TGA_Colorf(0.0f, 1.0f, 0.0f, 1.0f);
const TGA_Color GREEN = TGA_Colorf(0.0f, 0.0f, 1.0f, 1.0f);
const TGA_Color WHITE = TGA_Colorf(1.0f, 1.0f, 1.0f, 1.0f);
const TGA_Color BLACK  = TGA_Colorf(0.0f, 0.0f, 0.0f, 1.0f);

extern "C" TGA_Color TGA_Colorf(r32 r, r32 g, r32 b,  r32 a)
{
  TGA_Color result = {};
  result.r = nColorToByte(Linear_To_sRGB(r));
  result.g = nColorToByte(Linear_To_sRGB(g));
  result.b = nColorToByte(Linear_To_sRGB(b));
  result.a = nColorToByte(a);

  return result;
}

extern "C" TGA_Color TGA_Colorb(u8 r, u8 g, u8 b, u8 a) {
  TGA_Color result = {};
  result.r = r;
  result.g = g;
  result.b = b;
  result.a = a;

    return result;
}

extern "C" TGA_File* TGA_File_Generate(i32 width, i32 height) {
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

extern "C" void TGA_Image_Clear_To_Color(TGA_File* file, TGA_Color color) {
  i32 size = file->Header.Image_Spec_Height * file->Header.Image_Spec_Width;
  u32* pixels = (u32*)file->data.Image_Data;
  for(i32 pixel=0; pixel<size; pixel++)
    pixels[pixel] = color.value;
}

extern "C" void TGA_Put_Pixel(TGA_File* file, i32 x, i32 y, TGA_Color color) {
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

extern "C" void TGA_Image_Flip_Vertical(TGA_File* file) {
  i32 rows = file->Header.Image_Spec_Height;
  i32 cols = file->Header.Image_Spec_Width;
  i32 numPixels = rows * cols;
  u32* tempPixels = (u32*)malloc(sizeof(u32)*numPixels);
  u32* imagePixels = (u32*)file->data.Image_Data;

  for(i32 row=0; row<rows; row++) {
    for(i32 col=0; col<cols; col++) {
      i32 index = (row*cols)+col;
      i32 reverseIndex = ((cols*(rows-(row+1)))+col);
      printf("rIndex: %d, index %d\n", reverseIndex, index);
      tempPixels[reverseIndex] = imagePixels[index];
    }
  }
  free(file->data.Image_Data);
  file->data.Image_Data = (u8*)tempPixels;
}

extern "C" void TGA_File_Write(TGA_File* file, const char* path) {
  FILE* outStream = fopen(path, "w");
  if(outStream == 0)
    printf("Could not create file!\n");

  fwrite(&file->Header, sizeof(TGA_Header), 1, outStream);
  i32 imageDataSize = file->Header.Image_Spec_Width * file->Header.Image_Spec_Height * (file->Header.Image_Spec_Pixel_Depth/8);
  fwrite(file->data.Image_Data, imageDataSize, 1, outStream);

  fclose(outStream);
}

extern "C" void TGA_File_Destroy(TGA_File* file)
{
  if(file)
  {
    free(file->data.Image_Data);
    file->data.Image_Data = 0;
    free(file);
  }
}
