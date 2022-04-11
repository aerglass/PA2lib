#ifdef __cplusplus
extern "C" {
#endif

#ifndef __SPA_H__
#define __SPA_H__

// Includes devKitPro
#include <nds.h>//include libnds
#include <PA_lib.h>//include PA2lib
  
void SPA_Init(u8 screen);//function init
void SPA_InitDefault(u8 screen); //function init default
void SPA_WaitFor(int sec);//function wait for
void SPA_LoadSprite(int screen, int id, int width, int height, const char *dir, bool transflag);//function load sprite
void SPA_LoadBg(const char *dir, const char *name, int width, int height, int screen, int layer);//function load background
void SPA_UnloadBg(int screen, int layer, const char *name);//unloads bg
void SPA_UnloadSprite(int screen, int id);//unload sprite
void SPA_Flush();//flush
void SPA_SetSpriteX(int screen, int id, float x);//sets sprite x
void SPA_SetSpriteY(int screen, int id, float y);//sets sprite y
void SPA_SetSpriteXY(int screen, int id, float x, float y);//sets sprite xy
void SPA_FlipSprite(int screen, int id, int hflip, int vflip);//flip sprite
  #endif

#ifdef __cplusplus
}
#endif
