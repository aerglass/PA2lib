#ifdef __cplusplus
extern "C" {
#endif

#ifndef __PA_MIXEDBG_H__
#define __PA_MIXEDBG_H__





// NightFox LIB - Include de Fondos mixtos (Tiled / Bitmap 8 bits)
// Requiere DevkitARM
// Codigo por Cesar Rincon "NightFox"
// http://www.nightfoxandco.com/
// Version 20140413





// Includes devKitPro
#include <nds.h>



// Funcion PA_InitMixedBgSys();
void PA_InitMixedBgSys(u8 screen);
// Inicializa el modo mixto para fondo (Tiled BG + Bitmap 8 bits)
// Capas 0 a 2 - Tiled
// Capa 3 - Bitmap 8 bits





#endif

#ifdef __cplusplus
}
#endif
