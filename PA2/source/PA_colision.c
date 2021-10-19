
// NightFox LIB - Funciones de Colisiones
// Requiere DevkitARM
// Codigo por Cesar Rincon "NightFox"
// http://www.nightfoxandco.com/
// Version 20140413



// Includes devKitPro
#include <nds.h>
#include <filesystem.h>
#include <fat.h>

// Includes C
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Includes propios
#include "PA_basic.h"
#include "PA_colision.h"




// Define los buffers y estructuras de control de los mapas de colision
PA_TYPE_CMAP_IPAO PA_CMAP[PA_SLOTS_CMAP];



// Funcion PA_InitCmapBuffers();
void PA_InitMapBuffers(void) {
	u8 n = 0;
	for (n = 0; n < PA_SLOTS_CMAP; n ++) {
		PA_CMAP[n].tiles = NULL;		// Inicializa los punteros de los buffers
		PA_CMAP[n].map = NULL;
		PA_CMAP[n].tiles_size = 0;		// Tamaño de archivo
		PA_CMAP[n].map_size = 0;
		PA_CMAP[n].width = 0;			// Ancho del mapa
		PA_CMAP[n].height = 0;			// Alto del mapa
		PA_CMAP[n].inuse = false;		// Esta en uso el slot?
	}
}



// Funcion PA_ResetCmapBuffers();
void PA_ResetMapBuffers(void) {
	u8 n = 0;
	for (n = 0; n < PA_SLOTS_CMAP; n ++) {
		free(PA_CMAP[n].tiles);		// Vacia los buffers
		free(PA_CMAP[n].map);
	}
	PA_InitMapBuffers();			// Y reinicia todas las variables
}



// Funcion PA_LoadColisionMap();
void PA_LoadMap(const char* file, u8 id, u16 width, u16 height) {

	// Verifica el rango de Id's
	if ((id < 0) || (id >= PA_SLOTS_CMAP)) {
		PA_Error(106, "Colision Map", PA_SLOTS_CMAP);
	}

	// Verifica si la Id esta libre
	if (PA_CMAP[id].inuse) {
		PA_Error(109, "Colision Map", id);
	}

	// Vacia los buffers que se usaran
	free(PA_CMAP[id].map);
	PA_CMAP[id].map = NULL;

	// Declara los punteros a los ficheros
	FILE* file_id;

	// Variable para almacenar el path al archivo
	char filename[256];
	
	// Carga el archivo .CMP
	sprintf(filename, "%s/%s.cmp", PA_ROOTFOLDER, file);
	file_id = fopen(filename, "rb");
	if (file_id) {	// Si el archivo existe...
		// Obten el tamaño del archivo
		fseek(file_id, 0, SEEK_END);
		PA_CMAP[id].map_size = ftell(file_id);
		rewind(file_id);
		// Reserva el espacio en RAM
		PA_CMAP[id].map = (char*) calloc (PA_CMAP[id].map_size, sizeof(char));
		if (PA_CMAP[id].map == NULL) {		// Si no hay suficiente RAM libre
			PA_Error(102, NULL, PA_CMAP[id].map_size);
		}
		// Lee el archivo y ponlo en la RAM
		fread(PA_CMAP[id].map, 1, PA_CMAP[id].map_size, file_id);
	} else {	// Si el archivo no existe...
		PA_Error(101, filename, 0);
	}
	fclose(file_id);		// Cierra el archivo

	// Guarda las medidas
	PA_CMAP[id].width = width;
	PA_CMAP[id].height = height;

	// Y marca esta ID como usada
	PA_CMAP[id].inuse = true;

}



// Funcion PA_UnloadColisionMap();
void PA_UnloadMap(u8 id) {

	// Verifica el rango de Id's
	if ((id < 0) || (id >= PA_SLOTS_CMAP)) {
		PA_Error(106, "Colision Map", PA_SLOTS_CMAP);
	}

	// Verifica si la Id esta libre
	if (!PA_CMAP[id].inuse) {
		PA_Error(110, "Colision Map", id);
	}

	// Vacia los buffers que se usaran
	free(PA_CMAP[id].map);
	PA_CMAP[id].map = NULL;

	// Y marca esta ID como usada
	PA_CMAP[id].inuse = false;

}



// Funcion PA_GetTile();
u16 PA_GetTile(u8 slot, s32 x, s32 y) {

	// Si la coordenada esta fuera de rango, devuelve 0
	if (
		(x < 0)
		||
		(y < 0)
		||
		(x >= PA_CMAP[slot].width)
		||
		(y >= PA_CMAP[slot].height)
		) {
			// Devuelve 0
			return 0;

	} else {	// Si la coordenada esta dentro del rango...

		// Calcula el ancho en tiles del mapa
		u16 columns = (PA_CMAP[slot].width >> 3);		// (width / 8);

		// Calcula los tiles de posicion	(x / 8); (y / 8);
		u16 tile_x = (x >> 3);
		u16 tile_y = (y >> 3) + 1;		// +1, por que la primera fila se reserva para la referencia de tiles

		// Calcula el nº de tile
		u32 address = (((tile_y * columns) + tile_x) << 1);

		// Obten los bytes
		u8 lobyte = *(PA_CMAP[slot].map + address);
		u8 hibyte = *(PA_CMAP[slot].map + (address + 1));

		// Devuelve el valor del tile
		return ((hibyte << 8) | lobyte);

	}

}



// Funcion PA_SetTile();
void PA_SetTile(u8 slot, s32 x, s32 y, u16 value) {

	// Si la coordenada esta dentro del rango...
	if (
		(x >= 0)
		&&
		(y >= 0)
		&&
		(x < PA_CMAP[slot].width)
		&&
		(y < PA_CMAP[slot].height)
		) {

		// Calcula el ancho en tiles del mapa
		u16 columns = (PA_CMAP[slot].width >> 3);		// (width / 8);

		// Calcula los tiles de posicion	(x / 8); (y / 8);
		u16 tile_x = (x >> 3);
		u16 tile_y = (y >> 3) + 1;		// +1, por que la primera fila se reserva para la referencia de tiles

		// Calcula el nº de tile
		u32 address = (((tile_y * columns) + tile_x) << 1);
		// nº de tile x2, dado que el mapa es de 16 bits (2 bytes por dato) y el buffer
		// es de 8 bits, se lee el 2do byte, por eso se multiplica por 2.

		// Calcula los valores de los bytes
		u8 hibyte = ((value >> 8) & 0xff);		// HI Byte
		u8 lobyte = (value & 0xff);				// LO Byte

		// Escribe el nuevo valor en el mapa de colisiones
		*(PA_CMAP[slot].map + address) = lobyte;
		*(PA_CMAP[slot].map + (address + 1)) = hibyte;

	}

}



// Funcion PA_LoadColisionBg();
void PA_LoadColisionBg(const char* file, u8 id, u16 width, u16 height) {

	// Verifica el rango de Id's
	if ((id < 0) || (id >= PA_SLOTS_CMAP)) {
		PA_Error(106, "Colision Map", PA_SLOTS_CMAP);
	}

	// Verifica si la Id esta libre
	if (PA_CMAP[id].inuse) {
		PA_Error(109, "Colision Map", id);
	}

	// Vacia los buffers que se usaran
	free(PA_CMAP[id].tiles);
	PA_CMAP[id].tiles = NULL;
	free(PA_CMAP[id].map);
	PA_CMAP[id].map = NULL;

	// Declara los punteros a los ficheros
	FILE* file_id;

	// Variable para almacenar el path al archivo
	char filename[256];
	
	// Carga el archivo .DAT (TILES)
	sprintf(filename, "%s/%s.dat", PA_ROOTFOLDER, file);
	file_id = fopen(filename, "rb");
	if (file_id) {	// Si el archivo existe...
		// Obten el tamaño del archivo
		fseek(file_id, 0, SEEK_END);
		PA_CMAP[id].tiles_size = ftell(file_id);
		rewind(file_id);
		// Reserva el espacio en RAM
		PA_CMAP[id].tiles = (char*) calloc (PA_CMAP[id].tiles_size, sizeof(char));
		if (PA_CMAP[id].tiles == NULL) {		// Si no hay suficiente RAM libre
			PA_Error(102, NULL, PA_CMAP[id].tiles_size);
		}
		// Lee el archivo y ponlo en la RAM
		fread(PA_CMAP[id].tiles, 1, PA_CMAP[id].tiles_size, file_id);
	} else {	// Si el archivo no existe...
		PA_Error(101, filename, 0);
	}
	fclose(file_id);		// Cierra el archivo

	// Carga el archivo .CMP
	sprintf(filename, "%s/%s.cmp", PA_ROOTFOLDER, file);
	file_id = fopen(filename, "rb");
	if (file_id) {	// Si el archivo existe...
		// Obten el tamaño del archivo
		fseek(file_id, 0, SEEK_END);
		PA_CMAP[id].map_size = ftell(file_id);
		rewind(file_id);
		// Reserva el espacio en RAM
		PA_CMAP[id].map = (char*) calloc (PA_CMAP[id].map_size, sizeof(char));
		if (PA_CMAP[id].map == NULL) {		// Si no hay suficiente RAM libre
			PA_Error(102, NULL, PA_CMAP[id].map_size);
		}
		// Lee el archivo y ponlo en la RAM
		fread(PA_CMAP[id].map, 1, PA_CMAP[id].map_size, file_id);
	} else {	// Si el archivo no existe...
		PA_Error(101, filename, 0);
	}
	fclose(file_id);		// Cierra el archivo

	// Guarda las medidas
	PA_CMAP[id].width = width;
	PA_CMAP[id].height = height;

	// Y marca esta ID como usada
	PA_CMAP[id].inuse = true;

}



// Funcion PA_UnloadColisionBg();
void PA_UnloadColisionBg(u8 id) {

	// Verifica el rango de Id's
	if ((id < 0) || (id >= PA_SLOTS_CMAP)) {
		PA_Error(106, "Colision Map", PA_SLOTS_CMAP);
	}

	// Verifica si la Id esta libre
	if (!PA_CMAP[id].inuse) {
		PA_Error(110, "Colision Map", id);
	}

	// Vacia los buffers que se usaran
	free(PA_CMAP[id].tiles);
	PA_CMAP[id].tiles = NULL;
	free(PA_CMAP[id].map);
	PA_CMAP[id].map = NULL;

	// Y marca esta ID como usada
	PA_CMAP[id].inuse = false;

}



// Funcion PA_GetPoint();
u8 PA_GetPoint(u8 slot, s32 x, s32 y) {

	// Si la coordenada esta fuera de rango, devuelve 0
	if (
		(x < 0)
		||
		(y < 0)
		||
		(x >= PA_CMAP[slot].width)
		||
		(y >= PA_CMAP[slot].height)
		) {
			// Devuelve 0
			return 0;

	} else {	// Si la coordenada esta dentro del rango...

		// Calcula el ancho en tiles del mapa
		u16 columns = (PA_CMAP[slot].width >> 3);		// (width / 8);

		// Calcula los tiles de posicion	(x / 8); (y / 8);
		u16 tile_x = (x >> 3);
		u16 tile_y = (y >> 3) + 1;			// +1, por que la primera fila se reserva para la referencia de tiles

		// Calcula los pixeles relativos
		u16 pixel_x = x - (tile_x << 3);
		u16 pixel_y = (y + 8) - (tile_y << 3); 

		// Calcula la posicion de tile dentro del archivo de mapa
		s32 address = (((tile_y * columns) + tile_x) << 1);
		u8 lobyte = *(PA_CMAP[slot].map + address);
		u8 hibyte = *(PA_CMAP[slot].map + (address + 1));
		u16 tile = ((hibyte << 8) | lobyte);

		// Obten el valor del pixel leyendola del archivo de tiles
		address = ((tile << 6) + (pixel_y << 3) + pixel_x);	// (tile * 64) + (y * 8) + x
		lobyte = *(PA_CMAP[slot].tiles + address);

		// Devuelve el valor del pixel
		return lobyte;

	}

}
