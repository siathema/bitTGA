#ifndef _BIT_TGA
#define _BIT_TGA

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


  typedef struct TGA_Color {
    union {
      struct {
        u8 b,g,r,a;
      };
      u32 value;
      u8 raw[4];
    };
    }TGA_Color;

#pragma pack(push, 1)
  typedef struct TGA_Header {
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
  }TGA_Header;
#pragma pack(pop)

  typedef struct TGA_Data {
    u8* Image_ID;
    u8* Color_Map_Data;
    u8* Image_Data;
  }TGA_Data;

  typedef struct TGA_File {
    TGA_Header Header;
    TGA_Data data;
  }TGA_File;

#ifdef __cplusplus
extern "C" {
#endif

  TGA_Color TGA_Colorb( u8 r=0, u8 g=0, u8 b=0, u8 a=0);

  TGA_Color TGA_Colorf( r32 r=0, r32 g=0, r32 b=0, r32 a=0);

  TGA_File* TGA_File_Generate(i32 width, i32 height);

  void TGA_Image_Clear_To_Color(TGA_File* file, TGA_Color color);

  void TGA_Put_Pixel(TGA_File* file, i32 x, i32 y, TGA_Color color);

  void TGA_Image_Flip_Vertical(TGA_File* file);

  void TGA_File_Write(TGA_File* file, const char* path);

  void TGA_File_Destroy(TGA_File* file);

#ifdef __cplusplus
}
#endif

#endif
