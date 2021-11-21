#ifdef __cplusplus
extern "C" {
#endif

#ifndef __PA_TILEDBG_H__
#define __PA_TILEDBG_H__

// Includes devKitPro
#include <nds.h>


// Define los slots maximos para los fondos
#define PA_SLOTS_TBG 64			// Datos de los fondos
#define PA_SLOTS_EXBGPAL 128	// Paletas extendidas (maximo 16 paletas por fondo)

// Define el numero maximo de bancos para tiles y mapas
#define PA_MAX_BANKS_TILES 8
#define PA_MAX_BANKS_MAPS 16

// Define el numero de bancos de Mapas y Tiles
extern u8 PA_BANKS_TILES[2];	// (1 banks = 16kb)	Cada banco de tiles puede alvergar 8 bancos de Mapas
extern u8 PA_BANKS_MAPS[2];		// (1 bank = 2kb)	Usar multiplos de 8. Cada set de 8 bancos consume 1 banco de tiles

// Define los Buffers para almacenar los fondos
extern char* PA_BUFFER_BGTILES[PA_SLOTS_TBG];
extern char* PA_BUFFER_BGMAP[PA_SLOTS_TBG];
extern char* PA_BUFFER_BGPAL[PA_SLOTS_TBG];


// Define estructura para almacenar la iPAo de los fondos
typedef struct {
	char name[32];		// Nombre del fondo
	u32 tilesize;		// Tama�o del Tileset
	u32 mapsize;		// Tama�o del Map
	u32 palsize;		// Tama�o de la Paleta
	u16 width;			// Ancho del fondo
	u16 height;			// Altura del fondo
	bool available;		// Disponibilidat del Slot
} PA_TYPE_TBG_IPAO;
extern PA_TYPE_TBG_IPAO PA_TILEDBG[PA_SLOTS_TBG];	// Datos de los fondos

// Define la estructura para almacenar la iPAo y datos de las paletas extendidas
typedef struct {
	char* buffer;	// Buffer para almacenar la paleta
	u32 palsize;	// Tama�o de la paleta
	bool inuse;		// Slot libre o en uso
} PA_TYPE_EXBGPAL_IPAO;
extern PA_TYPE_EXBGPAL_IPAO PA_EXBGPAL[PA_SLOTS_EXBGPAL];	// Datos de las paletas extendidas

// Define estructura para almacenar la iPAo de los fondos en pantalla
typedef struct {
	u8 tilebase;		// Bloque de inicio en VRAM del Tileset
	u8 tileblocks;		// Bloques usados por el Tileset
	u8 mapbase;			// Bloque de inicio en VRAM del Map
	u8 mapblocks;		// Bloques usados por el Map
	u16 bgwidth;		// Ancho del fondo
	u16 bgheight;		// Altura del fondo
	u16 mapwidth;		// Ancho del mapa
	u16 mapheight;		// Altura del mapa
	u8 bgtype;			// Tipo de mapa
	u8 bgslot;			// Buffer de graficos usado (PA_BUFFER_BGMAP)
	u8 blockx;			// Bloque de mapa (horizontal)
	u8 blocky;			// bloque de mapa (vertical)
	bool created;		// Flag de si esta creado
} PA_TYPE_TBGLAYERS_IPAO;
// El hardware de la DS no permite mapas mayores de 512x512
// Asi que iPAormaremos si nuestor mapa lo gestionara el hardware si es menor o 
// igual a 512x512, o usaremos nuestro motor de Tile Swaping
// bgtype 0: Normal (maximo 512 x 512)
// bgtype 1: >512 x 256
// bgtype 2: 256 x >512
// bgtype 3: >512 x >512
extern PA_TYPE_TBGLAYERS_IPAO PA_TILEDBG_LAYERS[2][4];	//[screen][layer]

// Define el array de bloques libres
extern u8 PA_TILEBLOCKS[2][PA_MAX_BANKS_TILES];
extern u8 PA_MAPBLOCKS[2][PA_MAX_BANKS_MAPS];

// Funcion PA_InitTiledBgBuffers();
void PA_InitBgBuffers(void);
// Inicializa los buffers y estructuras de control para usar los fondos "tileados"
// Se debe usar antes de cargar o usar cualquier fondo
// No uses esta funcion mas de una vez en tu codigo

// Funcion PA_ResetTiledBgBuffers();
void PA_ResetBgBuffers(void);
// Borra todos los buffers y reinicia las estructuras de fondos "tileados"
// Usala para los cambios de nivel y similares

// Funcion PA_InitTiledBgSys();
void PA_InitBg(u8 screen);
// Inicializa las variables de control de tiles, mapas y paletas
// Asigna 128kb de RAM para fondos tileados
// Se debe especificar la pantalla (0 o 1)

// Funcion PA_LoadTiledBg();
void PA_LoadBg(const char* file, const char* name, u16 width, u16 height);
// Carga un fondo tileado desde FAT
// Debes de especificar el archivo que se cargara (sin extension) y el nombre
// que le quieres dar y las medidas en pixeles

// Funcion PA_LoadTilesForBg();
void PA_LoadTilesForBg(const char* file, const char* name, u16 width, u16 height, u16 tile_start, u16 tile_end);
// Carga desde la FAT los tiles especificados y su paleta
// Ademas, crea un mapa vacio de la medida especificada
// Esta funcion es util para cargar varios tiles y despues generar fondos
// modificando el MAP desde cogido (Generador de escenarios, animaciones, etc)

// Funcion PA_UnloadTiledBg();
void PA_UnloadBg(const char* name);
// Borra de la RAM un fondo cargado con PA_LoadTiledBg();
// Debes especificar el nombre que le diste al fondo

// Funcion PA_CreateTiledBg();
void PA_CreateBg(u8 screen, u8 layer, const char* name);
// Crea un fondo con los parametros dados, indicale la pantalla, capa y nombre

// Funcion PA_DeleteTiledBg();
void PA_DeleteBg(u8 screen, u8 layer);
// Borra un fondo de la memoria VRAM
// Debes especificar la pantalla y numero de capa

// Funcion PA_GetTileMapAddress();
extern u32 PA_GetTileMapAddress(u8 screen, u8 layer, u16 tile_x, u16 tile_y);
// Funcion de uso interno de la libreria
// Devuelve la direccion en el buffer de un tile concreto

// Funcion PA_GetTileOfMap();
extern u16 PA_GetTileOfMap(u8 screen, u8 layer, u16 tile_x, u16 tile_y);
// Obten el valor del tile del mapa indicado en las coordenadas (en tiles) indicadas.

// Funcion PA_SetTileOfMap();
void PA_SetTileOfMap(u8 screen, u8 layer, u16 tile_x, u16 tile_y, u16 tile);
// Cambia el valor del tile del mapa indicado en las coordenadas (en tiles) indicadas.

// Funcion PA_UpdateVramMap();
void PA_UpdateVramMap(u8 screen, u8 layer);
// Actualiza en VRAM el contenido del mapa seleccionado.

// Funcion PA_BgSetPalColor();
void PA_BgSetPalColor(u8 screen, u8 layer, u8 number, u8 r, u8 g, u8 b);
// Cambia al momento el valor de un color de la paleta
// Cuidado! Funcion Muy lenta, usar solo para 2 o 3 colores por ciclo
// Cambia el color directamente en la VRAM

// Funcion PA_BgEditPalColor();
void PA_BgEditPalColor(u8 screen, u8 layer, u8 number, u8 r, u8 g, u8 b);
// Edita un color de la paleta seleccionada.
// El color se edita en el buffer de RAM, para que sea efectivo,
// mandala a la VRAM con PA_UpdatePalette();

// Funcion 	PA_BgUpdatePalette();
void PA_BgUpdatePalette(u8 screen, u8 layer);
// Actualiza la paleta en VRAM con la que se encuentra en el buffer de RAM

// Funcion PA_BgGetPalColor();
void PA_BgGetPalColor(u8 screen, u8 layer, u8 number, u8* r, u8* g, u8* b);
// Obtiene el valor de un color de la paleta que se encuentra en la RAM

// Funcion PA_GetTilePal();
extern u8 PA_GetTilePal(u8 screen, u8 layer, u16 tile_x, u16 tile_y);
// Devuelve que numero de paleta (0 - 15) esta usando el tile del fondo especificado.
// Por defecto, todos los tiles usan la paleta del Slot n�0
// Los datos se obtienen de la compia en RAM del mapa del fondo.

// Funcion PA_SetTilePal();
void PA_SetTilePal(u8 screen, u8 layer, u16 tile_x, u16 tile_y, u8 pal);
// Cambia el numero de paleta (0 - 15) que usara el tile del fondo especificado.
// Por defecto, todos los tiles usan la paleta del Slot n�0
// Los datos se escriben de la compia en RAM del mapa del fondo, por lo que no seran
// visibles hasta que ejecutes la funcion PA_UpdateVramMap();

// Funcion PA_LoadExBgPal();
void PA_LoadExBgPal(const char* file, u8 slot);
// Carga en el buffer de RAM correspondiente una paleta de fondos, para poderla usar
// mas tarde como paleta extendida.

// Funcion PA_UnloadExBgPal();
void PA_UnloadExBgPal(u8 slot);
// Borra de la RAM la paleta del slot especificado.
	
// Funcion PA_VramExBgPal();
void PA_VramExBgPal(u8 screen, u8 layer, u8 id, u8 slot);
// Transfiere a la VRAM una paleta extendida en el slot de la pantalla y
// fondo especificados.

// Funcion PA_SetExBgPal();
void PA_SetExBgPal(u8 screen, u8 layer, u8 pal);
// Cambia la paleta extendida que usara un fondo.
// La paleta debe de estar transferida en la VRAM previamente

// Funcion PA_SetTileHflip();
void PA_SetTileHflip(u8 screen, u8 layer, u16 tile_x, u16 tile_y);
// Invierte horizontalmente el estado actual del tile seleccionado
// Los cambios no seran visibles hasta que actualices el mapa
// con la funcion PA_UpdateVramMap();

// Funcion PA_SetTileVflip();
void PA_SetTileVflip(u8 screen, u8 layer, u16 tile_x, u16 tile_y);
// Invierte verticalmente el estado actual del tile seleccionado
// Los cambios no seran visibles hasta que actualices el mapa
// con la funcion PA_UpdateVramMap();

// Funcion PA_RotateTileGfx();
void PA_RotateTileGfx(u8 slot, u16 tile, u8 rotation);
// Rota el grafico de un tile especificado. Rota el tile almacenado en un buffer de fondos
// que se encuentra en RAM. Debes especificar el SLOT del buffer, numero de tile en el buffer
// y el angulo de la rotacion.
// 1 - 90� a la derecha
// 2 - 90� a la izquierda
// 3 - 180�


#endif

#ifdef __cplusplus
}
#endif
