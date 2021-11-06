#ifdef __cplusplus
extern "C" {
#endif

#ifndef __SPA_H__
#define __SPA_H__

// Includes devKitPro
#include <nds.h>
#include <PA_lib.h>
  
void SPA_BasicSetup();
void SPA_Init();
void SPA_WaitFor(int sec);
void SPA_LoadSprite(int screen, int id, int width, int height, const char *dir, bool transflag);
void SPA_LoadBackground(const char *dir, const char *name, int width, int height, int screen, int layer);
void SPA_unloadbg(int screen, int layer, const char *name);
void SPA_unloadsprite(int screen, int id);
void SPA_Flush();
void SPA_SetSpriteX(int screen, int id, float x);
void SPA_SetSpriteY(int screen, int id, float y);
void SPA_SetSpriteXY(int screen, int id, float x, float y);
#endif

#ifdef __cplusplus
}
#endif
