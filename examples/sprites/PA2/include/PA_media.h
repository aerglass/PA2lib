#ifdef __cplusplus
extern "C" {
#endif

#ifndef __PA_MEDIA_H__
#define __PA_MEDIA_H__

// Includes devKitPro
#include <nds.h>


// Funcion PA_LoadBMP();
void PA_LoadBMP(const char* file, u8 slot);
// Carga un archivo BMP de 8, 16 o 24 bits en un slot de imagenes de 16 bits.
// Debes inicializar el modo 16 bits, el backbuffer y usar la funcion PA_Draw16bitsImage();
// para mostrarlo.

#endif

#ifdef __cplusplus
}
#endif
