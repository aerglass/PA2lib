#ifdef __cplusplus
extern "C" {
#endif

#ifndef __PA_LIB_H__
#define __PA_LIB_H__

// Definiciones comunes
#include "PA_defines.h"

// Libreria de funciones basicas y comunes
#include "PA_basic.h"

// Libreria de funciones 2D comunes
#include "PA_2d.h"

// Libreria de fondos con Tiles
#include "PA_tiledbg.h"

// Libreria de fondos Affine
#include "PA_affinebg.h"

// Libreria de fondos en modo Bitmap
#include "PA_bitmapbg.h"

// Libreria de fondos en modo mixto (Tiled / Bitmap 8 bits)
#include "PA_mixedbg.h"

// Libreria de sprites de 256 colores
#include "PA_sprite256.h"

// Libreria de textos
#include "PA_text.h"

// Libreria de textos de 16 pixeles
#include "PA_text16.h"

// Libreria de colisiones
#include "PA_colision.h"

// Libreria de sonido
#include "PA_sound.h"

// Libreria de archivos multimedia
#include "PA_media.h"

// Libreria 3D, funciones comunes
#include "PA_3d.h"

// Libreria 3D, Sprites
#include "PA_sprite3d.h"

//do simplepalib things
void SPA_BasicSetup(){
        PA_InitBgBuffers();
        PA_InitBg(0);		
        PA_InitBg(1);
        
        PA_InitSpriteBuffers();
        PA_InitSprite(0);
        PA_InitSprite(1);
        PA_InitTextSys(0);
        PA_InitTextSys(1);
}
void SPA_WaitFor(int sec){
        for (int i = 0; i < sec * 40000; i++){
        printf(" ");
        }
}
    void SPA_LoadSprite(int screen, int ramslot, int width, int height, const char *dir, const char *dir2, bool transflag) {
    PA_LoadSpriteGfx(dir, ramslot, width, height);
    PA_LoadSpritePal(dir2, ramslot);
    PA_VramSpriteGfx(screen, ramslot, ramslot, transflag);
    PA_VramSpritePal(screen, ramslot, ramslot);
}

void SPA_LoadBackground(const char *dir, const char *name, int width, int height, int screen, int layer) {
    PA_LoadBg(dir, name, width, height);
    PA_CreateBg(screen, layer, name);
}
void SPA_unloadbg(int screen, int layer, const char *name) {
    PA_DeleteBg(screen, layer);
    PA_UnloadBg(name);
}
// unload gfx and pallete in ram and vram
void SPA_unloadsprite(int screen, int ramslot, int vramslot) {
    // ram
    PA_UnloadSpriteGfx(ramslot);
    PA_UnloadSpritePal(ramslot);

    // vram
    PA_FreeSpriteGfx(screen, vramslot);
}
void SPA_Flush() {
    PA_SpriteOamSet(1);
    oamUpdate(&oamSub);

    PA_SpriteOamSet(0);
    oamUpdate(&oamMain);
    PA_UpdateTextLayers();
    swiWaitForVBlank();
}
    
#endif

#ifdef __cplusplus
}
#endif
