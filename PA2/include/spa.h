#ifdef __cplusplus
extern "C" {
#endif

#ifndef __SPA_H__
#define __SPA_H__

#define PA_UPDATEPAD(pad, var) do{ \
        Pad.pad.A = (var & KEY_A) != 0; \
        Pad.pad.B = (var & KEY_B) != 0; \
        Pad.pad.X = (var & KEY_X) != 0; \
        Pad.pad.Y = (var & KEY_Y) != 0; \
        Pad.pad.L = (var & KEY_L) != 0; \
        Pad.pad.R = (var & KEY_R) != 0; \
        Pad.pad.Select = (var & KEY_SELECT) != 0; \
        Pad.pad.Start = (var & KEY_START) != 0; \
        Pad.pad.Up = (var & KEY_UP) != 0; \
        Pad.pad.Down = (var & KEY_DOWN) != 0; \
        Pad.pad.Left = (var & KEY_LEFT) != 0; \
        Pad.pad.Right = (var & KEY_RIGHT) != 0; \
        Pad.pad.Anykey = (var & 0xFFF) != 0; \
    }while(0)
  // Includes devKitPro
  #include <nds.h>//include libnds
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


extern Pads Pad;
extern PA_Pad* PadPointer;
  void PA_Init();//function init
  void PA_InitText(u8 screen, u8 layer);
  void PA_OutputText(u8 screen, int x, int y, const char *text);
  void PA_ClearTextBg(u8 screen);
  void PA_InitVBL();
  void PA_OutputSimpleText(u8 screen, int x, int y, const char *text);
  void PA_ClearTextBg(u8 screen);
  void PA_WaitFor(int sec);//function wait for
  void PA_LoadSprite(u8 screen, int id, int size, const char *dir);
  void PA_LoadBackground(u8 screen, u8 layer, const char *dir);
  void PA_UnloadBackground(int screen, int layer, const char *name);//unloads bg
  void PA_UnloadSprite(int screen, int id);//unload sprite
  void PA_WaitForVBL();//flush
  void PA_SetSpriteX(int screen, int id, s32 x);//sets sprite x
  void PA_SetSpriteY(int screen, int id, s32 y);//sets sprite y
  void PA_SetSpriteXY(int screen, int id, s32 x, s32 y);//sets sprite xy
  void PA_FlipSprite(int screen, int id, int hflip, int vflip);//flip sprite
  #endif

#ifdef __cplusplus
}
#endif
