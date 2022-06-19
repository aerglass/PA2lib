#ifdef __cplusplus
extern "C" {
#endif

#ifndef __SPA_H__
#define __SPA_H__

  // Includes devKitPro
  #include <nds.h>//include libnds
  #include <PA_BgStruct.h>
  #include <PA_lib.h>//include PA2lib
  #define OBJ_SIZE_8X8  88
  #define OBJ_SIZE_8X16  816
  #define OBJ_SIZE_8X32  832

  #define OBJ_SIZE_16X8 168
  #define OBJ_SIZE_16X16 1616
  #define OBJ_SIZE_16X32 1632
  
  #define OBJ_SIZE_32X8  328
  #define OBJ_SIZE_32X16  3216
  #define OBJ_SIZE_32X32  3232
  #define OBJ_SIZE_32X64  3264

  #define OBJ_SIZE_64X32  6432
  #define OBJ_SIZE_64X64  6464

// Stylus and Pad definitions, will be used by the arm7 to auto-update positions...

typedef struct {
  u8 A, B, X, Y, L, R, Up, Down, Right, Left, Start, Select, Anykey;
} PA_Pad;

typedef struct {
  PA_Pad Held, Released, Newpress; // Pressed si on appuye, Held si on garde appuyé, et Released si on relache
} Pads;

typedef struct {
  u8 Held, Released, Newpress, Newpress0;
  s16 X, Y, altX, altY, Pressure, Vx, Vy, oldVx, oldVy, Downtime, Uptime, DblClick;
} PA_Stylus;


  void PA_Init();//function init
  void PA_InitText(u8 screen, u8 layer);
  void PA_OutputText(u8 screen, int x, int y, const char *text);
  void PA_ClearTextBg(u8 screen);
  void PA_InitVBL();
  void PA_OutputSimpleText(u8 screen, int x, int y, const char *text);
  void PA_ClearTextBg(u8 screen);
  void PA_WaitFor(int sec);//function wait for
  void PA_LoadSprite(u8 screen, int id, int size, const char *dir);
  void PA_UnloadBackground(int screen, int layer, const char *name);//unloads bg
  void PA_UnloadSprite(int screen, int id);//unload sprite
  void PA_WaitForVBL();//flush

extern Pads Pad;
extern PA_Pad* PadPointer;
Pads Pad;
  #endif

#ifdef __cplusplus
}
#endif
