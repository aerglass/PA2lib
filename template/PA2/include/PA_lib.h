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

#include "spa.h"
    
    
#endif

#ifdef __cplusplus
}
#endif
