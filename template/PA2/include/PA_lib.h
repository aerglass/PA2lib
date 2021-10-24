#ifdef __cplusplus
extern "C" {
#endif

#ifndef __PA_LIB_H__
#define __PA_LIB_H__





// NightFox LIB - Include General
// Requiere DevkitARM
// Codigo por Cesar Rincon "NightFox"
// http://www.nightfoxandco.com/
// Version 20140413



/*

Notas sobre BITSHIFT

(n >> x)	Divide			n / x
(n << x)	Multiplica		n * x

Valores de X
2	  =	1
4	  =	2
8	  =	3
16	  =	4
32	  =	5
64	  =	6
128	  =	7
256	  =	8
512	  =	9
1024  =	10
2048  =	11
4096  =	12
8192  =	13
16384 =	14
32768 =	15
65536 =	16

Dado que la DS no tiene unidad de coma flotante, siempre que dividas o
multipliques por numeros de base 2, usa el bitshift
Por ejemplo:
a = (512 / 8);
seria equivalente a
a = (512 >> 3);
Multiplicando
b = (3 * 2048);
seria con bitshift
b = (3 << 11);

*/




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
