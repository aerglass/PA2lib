#ifdef __cplusplus
extern "C" {
#endif

#ifndef __PA_3D_H__
#define __PA_3D_H__

// Includes devKitPro
#include <nds.h>


// Funcion PA_Set3D();
void PA_Set3D(u8 screen, u8 mode);
// Inicia el modo 3D seleccionado en la pantalla deseada


// Funcion PA_InitOpenGL();
void PA_InitOpenGL(void);
// Inicializa el OpenGL para la libreria



// Funcion PA_GetTextureSize();
extern u16 PA_GetTextureSize(u16 textel);
// Devuelve el tamaño del textel


#endif

#ifdef __cplusplus
}
#endif
