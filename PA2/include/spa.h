#ifdef __cplusplus
extern "C" {
#endif

#ifndef __SPA_H__
#define __SPA_H__

// Includes devKitPro
#include <nds.h>
#include <PA_lib.h>
  
void SPA_BasicSetup();
void SPA_WaitFor(int sec);
void SPA_LoadSprite(int screen, int id, int width, int height, const char *dir, const char *dir2, bool transflag);
void SPA_LoadBackground(const char *dir, const char *name, int width, int height, int screen, int layer);
void SPA_unloadbg(int screen, int layer, const char *name);
void SPA_unloadsprite(int screen, int id);
void SPA_Flush();
#endif

#ifdef __cplusplus
}
#endif
