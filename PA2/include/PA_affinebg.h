#ifdef __cplusplus
extern "C" {
#endif

#ifndef __PA_AFFINEBG_H__
#define __PA_AFFINEBG_H__

// Includes devKitPro
#include <nds.h>


// Estructura para almacenar los parametros de los fondos Affine
typedef struct {
	s32 x;			// Posicion X
	s32 y;			// Posicion Y
	s32 x_center;	// Centro X
	s32 y_center;	// Centro Y
	s32 x_scale;	// Valor Zoom X (PA)
	s32 y_scale;	// Valor Zoom Y (PD)
	s32 x_tilt;		// Valor Inclinacion X (PB)
	s32 y_tilt;		// Valor Inclinacion Y (PC)
	s32 angle;		// Valor de la rotacion
} PA_TYPE_AFFINE_BG;
extern PA_TYPE_AFFINE_BG PA_AFFINE_BG[2][4];





// Funcion PA_InitTiledBgSys();
void PA_InitAffineBgSys(u8 screen);
// Inicializa el sistema de fondos Affine. Solo puede usarse en las capas 2 y 3, los fondos en
// la misma pantalla deben de compartir la paleta y no pueden tener mas de 256 tiles.



// Funcion PA_LoadAffineBg();
void PA_LoadAffineBg(const char* file, const char* name, u16 width, u16 height);
// Carga un fondo tileado desde FAT
// Debes de especificar el archivo que se cargara (sin extension) y el nombre
// que le quieres dar y las medidas en pixeles
// Los buffers para fondos tileados deben estar inicializados antes de usar esta funcion



// Funcion PA_UnloadAffineBg();
void PA_UnloadAffineBg(const char* name);
// Borra de la RAM el fondo affine especificado. Es una simple llamada a la funcion PA_UnloadTiledBg();



// Funcion PA_CreateAffineBg();
void PA_CreateAffineBg(u8 screen, u8 layer, const char* name, u8 wrap);
// Crea un fondo con los parametros dados, indicale la pantalla, capa, nombre y si se activa la opcion
// de WRAP arround (0 desactivado, 1 activado).



// Funcion PA_DeleteAffineBg();
void PA_DeleteAffineBg(u8 screen, u8 layer);
// Borra el fondo Affine especificado



// Funcion PA_AffineBgTransform();
void PA_AffineBgTransform(u8 screen, u8 layer, s32 x_scale, s32 y_scale, s32 x_tilt, s32 y_tilt);
// Modifica los parametros de la matriz de transformacion del fondo affine



// Funcion PA_AffineBgMove();
void PA_AffineBgMove(u8 screen, u8 layer, s32 x, s32 y, s32 angle);
// Desplaza el fondo affine y rotalo (-2048 a 2048)



// Funcion PA_AffineBgCenter();
void PA_AffineBgCenter(u8 screen, u8 layer, s32 x, s32 y);
// Define el centro de rotacion de un fondo affine

#endif

#ifdef __cplusplus
}
#endif
