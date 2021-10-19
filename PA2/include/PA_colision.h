#ifdef __cplusplus
extern "C" {
#endif

#ifndef __PA_COLISION_H__
#define __PA_COLISION_H__





// NightFox LIB - Include de Colisiones
// Requiere DevkitARM
// Codigo por Cesar Rincon "NightFox"
// http://www.nightfoxandco.com/
// Version 20140413





// Includes devKitPro
#include <nds.h>





// Define los slots maximos para los mapas de colisiones
#define PA_SLOTS_CMAP 32

// Define la estructura de control de mapas de colisiones
typedef struct {
	char* tiles;		// Buffer para almacenar los tiles
	char* map;			// Buffer para almacenar el mapa
	char* pal;			// Buffer para almacenar la paleta
	u32 tiles_size;		// Tamaño de los archivos
	u32 map_size;
	u32 pal_size;
	u16 width;			// Ancho del mapa (en pixeles)
	u16 height;			// Alto del mapa (en pixeles)
	bool inuse;			// Esta en uso el slot?
} PA_TYPE_CMAP_IPAO;
extern PA_TYPE_CMAP_IPAO PA_CMAP[PA_SLOTS_CMAP];		// Datos de los mapas de colision




// Funcion PA_InitCmapBuffers();
void PA_InitCmapBuffers(void);
// Inicializa los buffers que almacenaran los mapas de colision
// Debes usar esta funcion una unica vez antes de cargar ningun mapa de colision



// Funcion PA_ResetCmapBuffers();
void PA_ResetCmapBuffers(void);
// Reinicia los buffers y variables de los mapas de colisiones.



// Funcion PA_LoadColisionMap();
void PA_LoadColisionMap(const char* file, u8 id, u16 width, u16 height);
// Carga un mapa de colisiones en el slot indicado.



// Funcion PA_UnloadColisionMap();
void PA_UnloadColisionMap(u8 id);
// Borra de la RAM el mapa de colisiones con el nº de slot indicado.



// Funcion PA_GetTile();
extern u16 PA_GetTile(u8 slot, s32 x, s32 y);
// Devuelve el numero de tile de la posicion especificada.



// Funcion PA_SetTile();
void PA_SetTile(u8 slot, s32 x, s32 y, u16 value);
// Cambia el valor del tile en la posicion especificada.



// Funcion PA_LoadColisionBg();
void PA_LoadColisionBg(const char* file, u8 id, u16 width, u16 height);
// Carga un fondo de colisiones para pixel perfect



// Funcion PA_UnloadColisionBg();
void PA_UnloadColisionBg(u8 id);
// Descarga un fondo de colisiones para pixel perfect



// Funcion PA_GetPoint();
extern u8 PA_GetPoint(u8 slot, s32 x, s32 y);
// Obten el color del pixel de las coordenadas dadas





#endif

#ifdef __cplusplus
}
#endif
