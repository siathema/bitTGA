#pragma once

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float r32;
typedef double r64;

typedef u8 b8;


namespace TGA_Lib
{
  struct TGA_Color {
    union {
      struct {
        u8 b,g,r,a;
      };
      u32 value;
      u8 raw[4];
    };
    TGA_Color() : value(0) {}
    inline TGA_Color(u8 r, u8 b, u8 g, u8 a) : r(r), b(b), g(g), a(a) {}
    inline TGA_Color(u32 value) : value(value) {}
    TGA_Color(r32 r, r32 b, r32 g, r32 a=1.0f);
    TGA_Color(TGA_Color& c);
  };

#pragma pack(push, 1) // NOTE(matthias): Let's do some ugo stuff
  struct TGA_Header {
    u8 Id_Length;
    u8 Color_Map_Type;
    u8 Image_Type;
    u8 Color_Map_Spec_A;
    u32 Color_Map_Spec_B;
    u16 Image_Spec_X_Origin;
    u16 Image_Spec_Y_Origin;
    u16 Image_Spec_Width;
    u16 Image_Spec_Height;
    u8 Image_Spec_Pixel_Depth;
    u8 Image_Spec_Image_Desc;
  };
#pragma pack(pop)

  struct TGA_Data {
    u8* Image_ID;
    u8* Color_Map_Data;
    u8* Image_Data;
  };

  struct TGA_File {
    TGA_Header Header;
    TGA_Data data;
  };

TGA_File* TGA_Generate_File(i32 width, i32 height);

void TGA_Put_Pixel(TGA_File* file, i32 x, i32 y, TGA_Color color);

void TGA_Write_To_File(TGA_File* file, const char* path);

}
