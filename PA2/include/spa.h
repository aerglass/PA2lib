#ifdef __cplusplus
extern "C" {
#endif

#ifndef __PA_2D_H__
#define __PA_2D_H__

// Includes devKitPro
#include <nds.h>

void SPA_BasicSetup();
void SPA_WaitFor(int sec);
void SPA_LoadSprite(int screen, int id, int width, int height, const char *dir, const char *dir2, bool transflag);
void SPA_LoadBackground(const char *dir, const char *name, int width, int height, int screen, int layer);
void SPA_unloadbg(int screen, int layer, const char *name);
void SPA_unloadsprite(int screen, int id);
void SPA_Flush();
void SPA_PrintText(u8 screen, u8 layer, u16 x, u16 y, const char* text);
#endif

#ifdef __cplusplus
}
#endif
