// Includes devKitPro
#include <nds.h>
#include <filesystem.h>
#include <fat.h>

// Includes C
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

// Includes propios
#include "PA_basic.h"
#include "PA_2d.h"
#include "PA_sprite256.h"

// Define los Buffers para almacenar los Sprites
char* PA_BUFFER_SPR256GFX[PA_SLOTS_SPR256GFX];
char* PA_BUFFER_SPR256PAL[PA_SLOTS_SPR256PAL];

// Define la estructura de datos de los Graficos de los Sprites
PA_TYPE_SPR256GFX_IPAO PA_SPR256GFX[PA_SLOTS_SPR256GFX];
// Define la estructura de datos de las Paletas de los Sprites
PA_TYPE_SPR256PAL_IPAO PA_SPR256PAL[PA_SLOTS_SPR256PAL];

// Define la estructura de Gfx en VRAM
PA_TYPE_SPR256VRAM_IPAO PA_SPR256VRAM[2][128];
// Datos de paletas de Sprites en VRAM (en uso, slot en ram, etc)
PA_TYPE_SPRPALSLOT_IPAO PA_SPRPALSLOT[2][16];

// Define la estructura de datos del OAM (Sprites)
PA_TYPE_SPRITEOAM_IPAO PA_SPRITEOAM[2][128];		// 2 pantallas, 128 sprites

// Define la esturctura de control de la VRAM para Sprites
PA_TYPE_SPRVRAM_IPAO PA_SPRVRAM[2];		// IPAormacion VRAM de Sprites en ambas pantallas


// Funcion PA_InitSpriteBuffers()
void PA_InitSpriteBuffers(void) {

	u16 n = 0;	// Variable comun

	// Inicializa Buffers de GFX
	for (n = 0; n < PA_SLOTS_SPR256GFX; n ++) {
		PA_BUFFER_SPR256GFX[n] = NULL;			// Inicializa puntero
		PA_SPR256GFX[n].size = 0;				// Tama�o (en bytes) del grafico (GFX)
		PA_SPR256GFX[n].width = 0;				// Ancho del Gfx
		PA_SPR256GFX[n].height = 0;				// Altura del Gfx
		PA_SPR256GFX[n].available = true;		// Disponibilidat del Slot
	}

	// Inicializa Buffers de PAL
	for (n = 0; n < PA_SLOTS_SPR256PAL; n ++) {
		PA_BUFFER_SPR256PAL[n] = NULL;		// Inicializa puntero
		PA_SPR256PAL[n].size = 0;			// Tama�o (en bytes) de la paleta (PAL)
		PA_SPR256PAL[n].available = true;	// Disponibilidat del Slot
	}

}



// Funcion PA_ResetSpriteBuffers()
void PA_ResetSpriteBuffers(void) {

	u16 n = 0;	// Variable comun

	// Borra los Buffers de GFX
	for (n = 0; n < PA_SLOTS_SPR256GFX; n ++) {
		free(PA_BUFFER_SPR256GFX[n]);
	}

	// Borra los Buffers de PAL
	for (n = 0; n < PA_SLOTS_SPR256PAL; n ++) {
		free(PA_BUFFER_SPR256PAL[n]);
	}

	// Reinicia el sistema de Sprites
	PA_InitSpriteBuffers();

}



// Funcion PA_InitSpriteSys();
void PA_InitSprite(int screen, ...) {

	// Analiza los parametros variables de la funcion
	va_list options;
	va_start(options, screen);
	u8 mode = va_arg(options, int);
	va_end(options);


	// Variables
	u8 n = 0;	// Uso comun

	// Inicializa la estructura de Gfx en VRAM
	// y la estructura de datos del OAM (Sprites)
	for (n = 0; n < 128; n ++) {	// 128 sprites
		// Gfx en la VRAM (128 Gfx x pantalla)
		PA_SPR256VRAM[screen][n].size = 0;				// Tama�o (en bytes) del Gfx
		PA_SPR256VRAM[screen][n].width = 0;				// Ancho del Gfx
		PA_SPR256VRAM[screen][n].height = 0;			// Altura del Gfx
		PA_SPR256VRAM[screen][n].address = 0;			// Posicion en la VRAM
		PA_SPR256VRAM[screen][n].ramid = 0;				// Numero de Slot en RAM del que provienes
		PA_SPR256VRAM[screen][n].framesize = 0;			// Tama�o del frame (en bytes)
		PA_SPR256VRAM[screen][n].lastframe = 0;			// Ultimo frame
		PA_SPR256VRAM[screen][n].keepframes = false;	// Si es un Sprite animado, debes de mantener los frames en RAM ?
		PA_SPR256VRAM[screen][n].inuse = false;			// Esta en uso ?
		// OAM (128 Sprites x pantalla)
		PA_SPRITEOAM[screen][n].index = n;			// Numero de Sprite (Index = N)
		PA_SPRITEOAM[screen][n].x = 0;				// Coordenada X del Sprite (0 por defecto)
		PA_SPRITEOAM[screen][n].y = 0;				// Coordenada Y del Sprite (0 por defecto)
		PA_SPRITEOAM[screen][n].layer = 0;			// Prioridad en las capas (0 por defecto)
		PA_SPRITEOAM[screen][n].pal = 0;			// Paleta que usaras (0 por defecto)
		PA_SPRITEOAM[screen][n].size = SpriteSize_8x8;					// Tama�o del Sprite (macro) (8x8 por defecto)
		PA_SPRITEOAM[screen][n].color = SpriteColorFormat_256Color;		// Modo de color (macro) (256 colores)
		PA_SPRITEOAM[screen][n].gfx = NULL;				// Puntero al grafico usado
		PA_SPRITEOAM[screen][n].rot = -1;				// Id de rotacion (-1 por defecto) (0 - 31 Id de rotacion)
		PA_SPRITEOAM[screen][n].doublesize = false;		// Usar el "double size" al rotar ? ("NO" por defecto)
		PA_SPRITEOAM[screen][n].hide = true;			// Ocultar el Sprite ("SI" por defecto)
		PA_SPRITEOAM[screen][n].hflip = false;			// Volteado Horizontal ("NO" por defecto)
		PA_SPRITEOAM[screen][n].vflip = false;			// Volteado Vertical ("NO" por defecto)
		PA_SPRITEOAM[screen][n].mosaic = false;			// Mosaico ("NO" por defecto)
		PA_SPRITEOAM[screen][n].gfxid = 0;				// Numero de Gfx usado
		PA_SPRITEOAM[screen][n].frame = 0;				// Frame actual
		PA_SPRITEOAM[screen][n].framesize = 0;			// Tama�o del frame (en bytes)
		PA_SPRITEOAM[screen][n].lastframe = 0;			// Ultimo frame
		PA_SPRITEOAM[screen][n].created = false;		// Esta creado este sprite ?
	}

	// Inicializa la estructura de datos de la VRAM de Sprites
	if (mode == 128) {
		PA_SPRVRAM[screen].max = 131072;
	} else {
		PA_SPRVRAM[screen].max = 65536;
	}
	PA_SPRVRAM[screen].free = PA_SPRVRAM[screen].max;		// Memoria VRAM libre (64kb/128kb)
	PA_SPRVRAM[screen].last = 0;							// Ultima posicion usada
	PA_SPRVRAM[screen].deleted = 0;							// Ningun Gfx borrado
	PA_SPRVRAM[screen].fragmented = 0;						// Memoria VRAM fragmentada
	PA_SPRVRAM[screen].inarow = PA_SPRVRAM[screen].max;		// Memoria VRAM contigua
	for (n = 0; n < 128; n ++) {
		PA_SPRVRAM[screen].pos[n] = 0;		// Posicion en VRAM para reusar despues de un borrado
		PA_SPRVRAM[screen].size[n] = 0;		// Tama�o del bloque libre para reusar
	}

	// Inicializa los datos de las paletas
	for (n = 0; n < 16; n ++) {
		PA_SPRPALSLOT[screen][n].inuse = false;
		PA_SPRPALSLOT[screen][n].ramslot = 0;
	}

	// CoPAigura el Motor 2D y VRAM segun la pantalla de destino
	if (screen == 0) {

		// CoPAigura la pantalla 0
		REG_DISPCNT |= (DISPLAY_SPR_ACTIVE);			// Activa los Sprites en la pantalla superior
		vramSetBankB(VRAM_B_MAIN_SPRITE_0x06400000);	// Banco B de la VRAM para Sprites (128kb)
		memset((void*)0x06400000, 0, 131072);			// Borra el contenido del banco B
		PA_SPRVRAM[screen].next = (0x06400000);			// Guarda la primera posicion de VRAM para Gfx
		vramSetBankF(VRAM_F_LCD);						// Banco F de la VRAM para paletas extendidas (Sprites) (8kb de 16kb)
		memset((void*)0x06890000, 0, 8192);				// Borra el contenido del banco F
		if (mode == 128) {
			oamInit(&oamMain, SpriteMapping_1D_128, true);	// Inicializa el OAM (Mapeado de 128 bytes, Paletas extendidas)
		} else {
			oamInit(&oamMain, SpriteMapping_1D_64, true);	// Inicializa el OAM (Mapeado de 64 bytes, Paletas extendidas)
		}

	} else {

		// CoPAigura la pantalla 1
		REG_DISPCNT_SUB |= (DISPLAY_SPR_ACTIVE);		// Activa los Sprites en la pantalla iPAerior
		vramSetBankD(VRAM_D_SUB_SPRITE);				// Banco D de la VRAM para Sprites (128kb)
		memset((void*)0x06600000, 0, 131072);			// Borra el contenido del banco D
		PA_SPRVRAM[screen].next = (0x06600000);			// Guarda la primera posicion de VRAM para Gfx
		vramSetBankI(VRAM_I_LCD);						// Banco I de la VRAM para paletas extendidas (Sprites) (8kb de 16kb)
		memset((void*)0x068A0000, 0, 8192);				// Borra el contenido del banco I
		if (mode == 128) {
			oamInit(&oamSub, SpriteMapping_1D_128, true);	// Inicializa el OAM (Mapeado de 128 bytes, Paletas extendidas)
		} else {
			oamInit(&oamSub, SpriteMapping_1D_64, true);	// Inicializa el OAM (Mapeado de 64 bytes, Paletas extendidas)
		}

	}

}



// Funcion PA_LoadSpriteGfx();
void PA_LoadSpriteGfx(const char* file, u16 id,  u16 width, u16 height) {

	// Verifica el rango de Id's
	if ((id < 0) || (id >= PA_SLOTS_SPR256GFX)) {
		PA_Error(106, "Sprite GFX", PA_SLOTS_SPR256GFX);
	}

	// Verifica si la Id esta libre
	if (!PA_SPR256GFX[id].available) {
		PA_Error(109, "Sprite GFX", id);
	}

	// Vacia los buffers que se usaran
	free(PA_BUFFER_SPR256GFX[id]);
	PA_BUFFER_SPR256GFX[id] = NULL;

	// Declara los punteros a los ficheros
	FILE* file_id;

	// Variable para almacenar el path al archivo
	char filename[256];

	// Carga el archivo .IMG
	sprintf(filename, "%s/%s.img", PA_ROOTFOLDER, file);
	file_id = fopen(filename, "rb");
	if (file_id) {	// Si el archivo existe...
		// Obten el tama�o del archivo
		fseek(file_id, 0, SEEK_END);
		PA_SPR256GFX[id].size = ftell(file_id);
		rewind(file_id);
		// Reserva el espacio en RAM
		PA_BUFFER_SPR256GFX[id] = (char*) calloc (PA_SPR256GFX[id].size, sizeof(char));
		if (PA_BUFFER_SPR256GFX[id] == NULL) {		// Si no hay suficiente RAM libre
			PA_Error(102, NULL, PA_SPR256GFX[id].size);
		}
		// Lee el archivo y ponlo en la RAM
		fread(PA_BUFFER_SPR256GFX[id], 1, PA_SPR256GFX[id].size, file_id);
	} else {	// Si el archivo no existe...
		PA_Error(101, filename, 0);
	}
	fclose(file_id);		// Cierra el archivo

	// Guarda las medidas del grafico
	PA_SPR256GFX[id].width = width;		// Ancho del Gfx
	PA_SPR256GFX[id].height = height;	// Altura del Gfx

	// Y marca esta ID como usada
	PA_SPR256GFX[id].available = false;

}



// Funcion PA_UnloadSpriteGfx();
void PA_UnloadSpriteGfx(u16 id) {

	// Verifica el rango de Id's
	if ((id < 0) || (id >= PA_SLOTS_SPR256GFX)) {
		PA_Error(106, "Sprite GFX", PA_SLOTS_SPR256GFX);
	}

	// Verifica si la Id esta libre
	if (PA_SPR256GFX[id].available) {
		PA_Error(110, "Sprite GFX", id);
	}

	// Vacia el buffer
	free(PA_BUFFER_SPR256GFX[id]);

	// Y reinicia las variables
	PA_BUFFER_SPR256GFX[id] = NULL;			// Inicializa puntero
	PA_SPR256GFX[id].size = 0;				// Tama�o (en bytes) del grafico (GFX)
	PA_SPR256GFX[id].width = 0;				// Ancho del Gfx
	PA_SPR256GFX[id].height = 0;			// Altura del Gfx
	PA_SPR256GFX[id].available = true;		// Disponibilidat del Slot

}



// Funcion PA_LoadSpritePal();
void PA_LoadSpritePal(const char* file, u8 id) {

	// Variable temporal del tama�o de la paleta
	u32 pal_size = 0;

	// Verifica el rango de Id's
	if ((id < 0) || (id >= PA_SLOTS_SPR256PAL)) {
		PA_Error(106, "Sprite PAL", PA_SLOTS_SPR256PAL);
	}

	// Verifica si la Id esta libre
	if (!PA_SPR256PAL[id].available) {
		PA_Error(109, "Sprite PAL", id);
	}

	// Vacia los buffers que se usaran
	free(PA_BUFFER_SPR256PAL[id]);
	PA_BUFFER_SPR256PAL[id] = NULL;

	// Declara los punteros a los ficheros
	FILE* file_id;

	// Variable para almacenar el path al archivo
	char filename[256];

	// Carga el archivo .PAL
	sprintf(filename, "%s/%s.pal", PA_ROOTFOLDER, file);
	file_id = fopen(filename, "rb");
	if (file_id) {	// Si el archivo existe...
		// Obten el tama�o del archivo
		fseek(file_id, 0, SEEK_END);
		pal_size = ftell(file_id);
		PA_SPR256PAL[id].size = pal_size;
		rewind(file_id);
		// Si el tama�o es iPAerior a 512 bytes, ajustalo
		if (PA_SPR256PAL[id].size < 512) PA_SPR256PAL[id].size = 512;
		// Reserva el espacio en RAM
		PA_BUFFER_SPR256PAL[id] = (char*) calloc (PA_SPR256PAL[id].size, sizeof(char));
		if (PA_BUFFER_SPR256PAL[id] == NULL) {		// Si no hay suficiente RAM libre
			PA_Error(102, NULL, PA_SPR256PAL[id].size);
		}
		// Lee el archivo y ponlo en la RAM
		fread(PA_BUFFER_SPR256PAL[id], 1, pal_size, file_id);
	} else {	// Si el archivo no existe...
		PA_Error(101, filename, 0);
	}
	fclose(file_id);		// Cierra el archivo

	// Y marca esta ID como usada
	PA_SPR256PAL[id].available = false;

}



// Funcion PA_UnloadSpritePal();
void PA_UnloadSpritePal(u8 id) {

	// Verifica el rango de Id's
	if ((id < 0) || (id >= PA_SLOTS_SPR256PAL)) {
		PA_Error(106, "Sprite PAL", PA_SLOTS_SPR256PAL);
	}

	// Verifica si la Id esta libre
	if (PA_SPR256PAL[id].available) {
		PA_Error(110, "Sprite PAL", id);
	}

	// Vacia el buffer
	free(PA_BUFFER_SPR256PAL[id]);

	// Y reinicia las variables
	PA_BUFFER_SPR256PAL[id] = NULL;		// Inicializa puntero
	PA_SPR256PAL[id].size = 0;			// Tama�o (en bytes) de la paleta (PAL)
	PA_SPR256PAL[id].available = true;	// Disponibilidat del Slot

}



// Funcion PA_VramSpriteGfx();
void PA_VramSpriteGfx(u8 screen, u16 ram, u16 vram, bool keepframes) {

	// Verifica el rango de Id's de RAM
	if ((ram < 0) || (ram >= PA_SLOTS_SPR256GFX)) {
		PA_Error(106, "Sprite GFX", (PA_SLOTS_SPR256GFX - 1));
	}

	// Verifica si slot de RAM esta vacio
	if (PA_SPR256GFX[ram].available) {
		PA_Error(110, "Sprite GFX", ram);
	}

	// Verifica el rango de Id's de VRAM
	if ((vram < 0) || (vram > 127)) {
		PA_Error(106, "VRAM GFX", 127);
	}

	// Verifica si el slot de VRAM esta libre
	if (PA_SPR256VRAM[screen][vram].inuse) {
		PA_Error(109, "VRAM", vram);
	}

	// Variables de uso general
	s16 n = 0;				// General
	s16 id = 255;			// Id del posible bloque libre
	s16 last_reuse = 0;		// N� del ultimo bloque reusable
	u32 gfxsize = 0;		// Tama�o de los datos que se copiaran
	u32 size = 0;			// Diferencia de tama�os entre bloque libre y datos
	u8 width = 0;			// Calculo de las medidas
	u8 height = 0;
	bool organize = true;	// Se debe de reorganizar el array de bloques libres ?

	// Auto calcula el tama�o de 1 frame
	width = (PA_SPR256GFX[ram].width >> 3);		// (width / 8)
	height = (PA_SPR256GFX[ram].height >> 3);	// (height / 8)
	PA_SPR256VRAM[screen][vram].framesize = ((width * height) << 6);	// ((width * height) * 64)
	// Auto calcula el ultimo frame de la animacion
	PA_SPR256VRAM[screen][vram].lastframe = ((int)(PA_SPR256GFX[ram].size / PA_SPR256VRAM[screen][vram].framesize)) - 1;
	PA_SPR256VRAM[screen][vram].inuse = true;						// Slot ocupado

	// Calcula el tama�o del grafico a copiar segun si debes o no copiar todos los frames
	if (keepframes) {	// Si debes de mantener los frames en RAM, solo copia el primero
		gfxsize = PA_SPR256VRAM[screen][vram].framesize;
	} else {			// Si no, copialos todos
		gfxsize = PA_SPR256GFX[ram].size;
	}

	// Actualiza la VRAM disponible
	PA_SPRVRAM[screen].free -= gfxsize;

	// Si no hay suficiente VRAM, error
	if (PA_SPRVRAM[screen].free < 0) {
		PA_Error(113, "Sprites", gfxsize);
	}

	// Si hay que aprovechar algun bloque borrado... (tama�o identico, preferente)
	if (PA_SPRVRAM[screen].deleted > 0) {
		// Busca un bloque vacio del tama�o identico
		for (n = 0; n < PA_SPRVRAM[screen].deleted; n ++) {
			if (PA_SPRVRAM[screen].size[n] == gfxsize) {		// Si el bloque tiene el tama�o suficiente
				id = n;		// Guarda la Id
				n = PA_SPRVRAM[screen].deleted;	// y sal
			}
		}
		// Si no habia ningun bloque de tama�o identico, busca el mas parecido (produce fragmentacion)
		if (id == 255) {
			// Busca un bloque vacio del tama�o suficiente
			for (n = 0; n < PA_SPRVRAM[screen].deleted; n ++) {
				if (PA_SPRVRAM[screen].size[n] > gfxsize) {		// Si el bloque tiene el tama�o suficiente
					id = n;		// Guarda la Id
					n = PA_SPRVRAM[screen].deleted;	// y sal
				}
			}
		}
	}
	
	// Si hay algun bloque borrado libre del tama�o suficiente...
	if (id != 255) {

		// Transfiere el grafico a la VRAM
		PA_DmaMemCopy((void*)PA_SPRVRAM[screen].pos[id], PA_BUFFER_SPR256GFX[ram], gfxsize);
		// Guarda el puntero donde lo has almacenado
		PA_SPR256VRAM[screen][vram].address = PA_SPRVRAM[screen].pos[id];

		// Si no has usado todo el tama�o, deja constancia
		if (gfxsize < PA_SPRVRAM[screen].size[id]) {

			// Calcula el tama�o del nuevo bloque libre
			size = (PA_SPRVRAM[screen].size[id] - gfxsize);
			// Actualiza los datos
			PA_SPRVRAM[screen].pos[id] += gfxsize;			// Nueva direccion
			PA_SPRVRAM[screen].size[id] = size;				// Nuevo tama�o
			PA_SPRVRAM[screen].fragmented -= gfxsize;		// Actualiza el contador de VRAM fragmentada
			organize = false;								// No se debe de reorganizar el array de bloques

		} else {	// Si has usado todo el tama�o, deja constancia

			PA_SPRVRAM[screen].fragmented -= PA_SPRVRAM[screen].size[id];	// Actualiza el contador de VRAM fragmentada

		}

		// Se tiene que reorganizar el array de bloques libres ?
		if (organize) {
			last_reuse = (PA_SPRVRAM[screen].deleted - 1);
			if (
			(last_reuse > 0)	// Si hay mas de un bloque borrado
			&&
			(id != last_reuse)	// Y no es la ultima posicion
			) {
				// Coloca los valores de la ultima posicion en esta
				PA_SPRVRAM[screen].pos[id] = PA_SPRVRAM[screen].pos[last_reuse];		// Nueva direccion
				PA_SPRVRAM[screen].size[id] = PA_SPRVRAM[screen].size[last_reuse];		// Nuevo tama�o
			}
			PA_SPRVRAM[screen].deleted --;		// Actualiza el contador de bloques libres, borrando el ultimo registro
		}

	} else {	// Si no habia ningun bloque borrado o con el tama�o suficiente, colacalo al final de la VRAM ocupada

		// Actualiza la VRAM contigua disponible (mayor bloque libre al final)
		PA_SPRVRAM[screen].inarow -= gfxsize;

		// Si no hay suficiente VRAM (contigua), error
		if (PA_SPRVRAM[screen].inarow < 0) {
			PA_Error(113, "Sprites", gfxsize);
		}

		// Transfiere el grafico a la VRAM
		PA_DmaMemCopy((void*)PA_SPRVRAM[screen].next, PA_BUFFER_SPR256GFX[ram], gfxsize);
		// Guarda el puntero donde lo has almacenado
		PA_SPR256VRAM[screen][vram].address = PA_SPRVRAM[screen].next;
		// Guarda la direccion actual como la ultima usada
		PA_SPRVRAM[screen].last = PA_SPRVRAM[screen].next;
		// Calcula la siguiente posicion libre
		PA_SPRVRAM[screen].next += gfxsize;

	}

	// Guarda los datos del Gfx que se copiara a la VRAM.
	PA_SPR256VRAM[screen][vram].size = gfxsize;						// Tama�o en bytes de los datos copiados
	PA_SPR256VRAM[screen][vram].width = PA_SPR256GFX[ram].width;	// Alto (px)
	PA_SPR256VRAM[screen][vram].height = PA_SPR256GFX[ram].height;	// Ancho (px)
	PA_SPR256VRAM[screen][vram].ramid = ram;						// Slot RAM de origen
	PA_SPR256VRAM[screen][vram].keepframes = keepframes;			// Debes guardar los frames en RAM o copiarlos a la VRAM?

}



// Funcion PA_FreeSpriteGfx();
void PA_FreeSpriteGfx(u8 screen, u16 id) {

	// Verifica si hay un grafico cargado en esa Id.
	if (!PA_SPR256VRAM[screen][id].inuse) {
		PA_Error(110, "Sprite Gfx", id);
	}

	// Borra el Gfx de la VRAM (pon a 0 todos los Bytes)
	memset((void*)PA_SPR256VRAM[screen][id].address, 0, PA_SPR256VRAM[screen][id].size);

	// Actualiza la cantidad de VRAM disponible
	PA_SPRVRAM[screen].free += PA_SPR256VRAM[screen][id].size;

	// Guarda la posicion y tama�o del bloque borrado para su reutilizacion
	PA_SPRVRAM[screen].pos[PA_SPRVRAM[screen].deleted] = PA_SPR256VRAM[screen][id].address;
	PA_SPRVRAM[screen].size[PA_SPRVRAM[screen].deleted] = PA_SPR256VRAM[screen][id].size;

	// Incrementa en contador de bloques borrados
	PA_SPRVRAM[screen].deleted ++;

	// Incrementa el contador de memoria fragmentada
	PA_SPRVRAM[screen].fragmented += PA_SPR256VRAM[screen][id].size;

	// Reinicia los datos de esta Id. de gfx
	PA_SPR256VRAM[screen][id].size = 0;			// Tama�o en bytes
	PA_SPR256VRAM[screen][id].width = 0;		// Alto (px)
	PA_SPR256VRAM[screen][id].height = 0;		// Ancho (px)
	PA_SPR256VRAM[screen][id].address = 0;		// Puntero en VRAM
	PA_SPR256VRAM[screen][id].framesize = 0;	// Tama�o del frame (en bytes)
	PA_SPR256VRAM[screen][id].lastframe = 0;	// Ultimo frame
	PA_SPR256VRAM[screen][id].inuse = false;

	// Debes desfragmentar la VRAM
	if (PA_SPRVRAM[screen].fragmented >= (PA_SPRVRAM[screen].inarow >> 1)) {
		PA_VramSpriteGfxDefrag(screen);
	}

}



// Funcion PA_VramSpriteGfxDefrag();
void PA_VramSpriteGfxDefrag(u8 screen) {

	// Calcula la VRAM en uso y crea un buffer para guardarla
	u32 used_vram = ((PA_SPRVRAM[screen].max - PA_SPRVRAM[screen].free) + 1);
	char* buffer;
	buffer = (char*) calloc (used_vram, sizeof(char));
	if (buffer == NULL) {		// Si no hay suficiente RAM libre
		PA_Error(102, NULL, used_vram);
	}

	char* address[128];		// Guarda la direccion en RAM
	u32 size[128];			// Guarda el tama�o
	u32 ram = 0;			// Puntero inicial de RAM
	u8 n = 0;				// Variable General
	u32 frame_address = 0;	// Guarda la direccion de VRAM del frame


	// Copia los datos de la VRAM a la RAM
	for (n = 0; n < 128; n ++) {
		// Si esta en uso
		if (PA_SPR256VRAM[screen][n].inuse) {
			// Copia el Gfx a la RAM
			address[n] = (buffer + ram);		// Calcula el puntero
			size[n] = PA_SPR256VRAM[screen][n].size;		// Almacena el tama�o
			PA_DmaMemCopy(address[n], (void*)PA_SPR256VRAM[screen][n].address, size[n]);	// Copialo a la VRAM
			ram += size[n];		// Siguiente posicion en RAM (relativa)
		}
	}

	// Inicializa la estructura de datos de la VRAM de Sprites
	PA_SPRVRAM[screen].free = PA_SPRVRAM[screen].max;		// Memoria VRAM libre (128kb)
	PA_SPRVRAM[screen].last = 0;							// Ultima posicion usada
	PA_SPRVRAM[screen].deleted = 0;							// Ningun Gfx borrado
	PA_SPRVRAM[screen].fragmented = 0;						// Memoria VRAM fragmentada
	PA_SPRVRAM[screen].inarow = PA_SPRVRAM[screen].max;		// Memoria VRAM contigua
	for (n = 0; n < 128; n ++) {
		PA_SPRVRAM[screen].pos[n] = 0;		// Posicion en VRAM para reusar despues de un borrado
		PA_SPRVRAM[screen].size[n] = 0;		// Tama�o del bloque libre para reusar
	}
	// Aplica la direccion de inicio de la VRAM
	if (screen == 0) {
		PA_SPRVRAM[screen].next = (0x06400000);
	} else {
		PA_SPRVRAM[screen].next = (0x06600000);
	}

	// Ahora, copia de nuevo los datos a la VRAM, pero alineados
	for (n = 0; n < 128; n ++) {
		// Si esta en uso
		if (PA_SPR256VRAM[screen][n].inuse) {
			PA_DmaMemCopy((void*)PA_SPRVRAM[screen].next, address[n], size[n]);		// Vuelve a colocar la el Gfx en VRAM
			PA_SPR256VRAM[screen][n].address = PA_SPRVRAM[screen].next;				// Guarda la nueva posicion en VRAM
			PA_SPRVRAM[screen].free -= size[n];		// Ram libre
			PA_SPRVRAM[screen].inarow -= size[n];	// Ram libre en bloque
			PA_SPRVRAM[screen].last = PA_SPRVRAM[screen].next;	// Guarda la posicion como ultima usada
			PA_SPRVRAM[screen].next += size[n];		// Y calcula la siguiente posicion a escribir
		}
	}

	// Reasigna a los sprites las nuevas posiciones de los graficos que usan
	for (n = 0; n < 128; n ++) {
		if (PA_SPRITEOAM[screen][n].created) {
			if (PA_SPR256VRAM[screen][PA_SPRITEOAM[screen][n].gfxid].keepframes) {
				// Si la opcion de animacion KEEP FRAMES esta activada,
				// simplemente asigna la nueva direccion en VRAM del grafico.
				PA_SPRITEOAM[screen][n].gfx = (u32*)PA_SPR256VRAM[screen][PA_SPRITEOAM[screen][n].gfxid].address;
			} else {
				// Si la opcion KEEP FRAMES esta desactivada,
				// calcula el desplazamiento dentro de la nueva direccion asignada.
				frame_address = (PA_SPR256VRAM[screen][PA_SPRITEOAM[screen][n].gfxid].address + (PA_SPRITEOAM[screen][n].framesize * PA_SPRITEOAM[screen][n].frame));
				PA_SPRITEOAM[screen][n].gfx = (u32*)frame_address;
			}
		}
	}

	// Vacia el buffer
	free(buffer);
	buffer = NULL;

}

// Funcion PA_VramSpritePal();
void PA_VramSpritePal(u8 screen, u8 id, u8 slot) {

	// Verifica el rango de Id's
	if ((id < 0) || (id >= PA_SLOTS_SPR256PAL)) {
		PA_Error(106, "Sprite PAL", PA_SLOTS_SPR256PAL);
	}

	// Verifica si la Id esta libre
	if (PA_SPR256PAL[id].available) {
		PA_Error(110, "Sprite PAL", id);
	}

	// Verifica si te has salido de rango (Paleta)
	if ((slot < 0) || (slot > 15)) {
		PA_Error(106, "Sprite Palette Slot", 15);
	}

	// Copia la paleta a la VRAM, segun la pantalla y el Slot
	u32 address = 0;
	if (screen == 0) {
		address = (0x06890000) + (slot << 9);			// Calcula donde guardaras la paleta
		vramSetBankF(VRAM_F_LCD);						// Bloquea el banco F para escribir las paletas
		PA_DmaMemCopy((void*)address, PA_BUFFER_SPR256PAL[id], PA_SPR256PAL[id].size);	// Copia la paleta al banco F
		vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);		// Pon el banco F en modo paleta extendida
	} else {
		address = (0x068A0000) + (slot << 9);			// Calcula donde guardaras la paleta
		vramSetBankI(VRAM_I_LCD);						// Bloquea el banco I para escribir las paletas
		PA_DmaMemCopy((void*)address, PA_BUFFER_SPR256PAL[id], PA_SPR256PAL[id].size);	// Copia la paleta al banco I
		vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);	// Pon el banco I en modo paleta extendida
	}

	PA_SPRPALSLOT[screen][slot].inuse = true;			// Marca el SLOT de paleta como en uso
	PA_SPRPALSLOT[screen][slot].ramslot = id;			// Guarda el slot de RAM donde esta la paleta original

}



// Funcion PA_CreateSprite();
void PA_CreateSprite(u8 screen, u8 id, u16 gfx, u8 pal, s16 x, s16 y) {

	// Verifica el rango de Id's de Sprites
	if ((id < 0) || (id > 127)) {
		PA_Error(106, "Sprite", 127);
	}

	// Verifica el rango de Id's de Gfx
	if ((gfx < 0) || (gfx > 127)) {
		PA_Error(106, "Sprite GFX", 127);
	}

	// Verifica si esta el Gfx en VRAM
	if (!PA_SPR256VRAM[screen][gfx].inuse) {
		PA_Error(111, "Sprite GFX", gfx);
	}

	// Verifica el rango de slots de paletas
	if ((pal < 0) || (pal > 15)) {
		PA_Error(106, "Sprite Palette Slot", 15);
	}

	// Verifica si esta la paleta en VRAM
	if (!PA_SPRPALSLOT[screen][pal].inuse) {
		PA_Error(111, "Sprite PAL", pal);
	}

	// // IPAorma al array de OAM del Id
	PA_SPRITEOAM[screen][id].index = id;

	// IPAorma al array de OAM del Gfx a usar
	PA_SPRITEOAM[screen][id].gfx = (u32*)PA_SPR256VRAM[screen][gfx].address;

	// IPAorma al array de OAM de la Paleta a usar
	PA_SPRITEOAM[screen][id].pal = pal;

	// IPAorma al array de OAM de la coordenada X
	PA_SPRITEOAM[screen][id].x = x;

	// IPAorma al array de OAM de la coordenada X
	PA_SPRITEOAM[screen][id].y = y;

	// IPAorma al array de OAM del numero de colores
	PA_SPRITEOAM[screen][id].color = SpriteColorFormat_256Color;

	// IPAorma al array de OAM de que debe mostrar el sprite
	PA_SPRITEOAM[screen][id].hide = false;

	// IPAorma al array de OAM del Id de Gfx usado
	PA_SPRITEOAM[screen][id].gfxid = gfx;

	// IPAorma al array de OAM de que el sprite se ha creado
	PA_SPRITEOAM[screen][id].created = true;

	// IPAorma al array de OAM del tama�o
	if ((PA_SPR256VRAM[screen][gfx].width == 8) && (PA_SPR256VRAM[screen][gfx].height == 8)) {	// 8x8
		if (PA_SPRVRAM[screen].max != 131072) {		// En modo 1D_128, este tama�o es ilegal
			PA_SPRITEOAM[screen][id].size = SpriteSize_8x8;
		} else {
			PA_Error(120, NULL, id);
		}
	}
	if ((PA_SPR256VRAM[screen][gfx].width == 16) && (PA_SPR256VRAM[screen][gfx].height == 16)) {	// 16x16
		PA_SPRITEOAM[screen][id].size = SpriteSize_16x16;
	}
	if ((PA_SPR256VRAM[screen][gfx].width == 32) && (PA_SPR256VRAM[screen][gfx].height == 32)) {	// 32x32
		PA_SPRITEOAM[screen][id].size = SpriteSize_32x32;
	}
	if ((PA_SPR256VRAM[screen][gfx].width == 64) && (PA_SPR256VRAM[screen][gfx].height == 64)) {	// 64x64
		PA_SPRITEOAM[screen][id].size = SpriteSize_64x64;
	}
	if ((PA_SPR256VRAM[screen][gfx].width == 16) && (PA_SPR256VRAM[screen][gfx].height == 8)) {	// 16x8
		PA_SPRITEOAM[screen][id].size = SpriteSize_16x8;
	}
	if ((PA_SPR256VRAM[screen][gfx].width == 32) && (PA_SPR256VRAM[screen][gfx].height == 8)) {	// 32x8
		PA_SPRITEOAM[screen][id].size = SpriteSize_32x8;
	}
	if ((PA_SPR256VRAM[screen][gfx].width == 32) && (PA_SPR256VRAM[screen][gfx].height == 16)) {	// 32x16
		PA_SPRITEOAM[screen][id].size = SpriteSize_32x16;
	}
	if ((PA_SPR256VRAM[screen][gfx].width == 64) && (PA_SPR256VRAM[screen][gfx].height == 32)) {	// 64x32
		PA_SPRITEOAM[screen][id].size = SpriteSize_64x32;
	}
	if ((PA_SPR256VRAM[screen][gfx].width == 8) && (PA_SPR256VRAM[screen][gfx].height == 16)) {	// 8x16
		PA_SPRITEOAM[screen][id].size = SpriteSize_8x16;
	}
	if ((PA_SPR256VRAM[screen][gfx].width == 8) && (PA_SPR256VRAM[screen][gfx].height == 32)) {	// 8x32
		PA_SPRITEOAM[screen][id].size = SpriteSize_8x32;
	}
	if ((PA_SPR256VRAM[screen][gfx].width == 16) && (PA_SPR256VRAM[screen][gfx].height == 32)) {	// 16x32
		PA_SPRITEOAM[screen][id].size = SpriteSize_16x32;
	}
	if ((PA_SPR256VRAM[screen][gfx].width == 32) && (PA_SPR256VRAM[screen][gfx].height == 64)) {	// 32x64
		PA_SPRITEOAM[screen][id].size = SpriteSize_32x64;
	}

	// IPAorma al array de OAM del ultimo frame del Sprite
	PA_SPRITEOAM[screen][id].lastframe = PA_SPR256VRAM[screen][gfx].lastframe;

	// IPAorma al array de OAM del tama�o del frame del Sprite (en bytes)
	PA_SPRITEOAM[screen][id].framesize = PA_SPR256VRAM[screen][gfx].framesize;

	// Por defecto, el primer frame (0)
	PA_SPRITEOAM[screen][id].frame = 0;

}



// Funcion PA_DeleteSprite();
void PA_DeleteSprite(u8 screen, u8 id) {

	// Verifica el rango de Id's de Sprites
	if ((id < 0) || (id > 127)) {
		PA_Error(106, "Sprite", 127);
	}

	// Verifica si el Sprite esta creado
	if (!PA_SPRITEOAM[screen][id].created) {
		char text[3];
		sprintf(text, "%d", screen);
		PA_Error(112, text, id);
	}

	// Reinicia todas las variables de ese Sprite
	PA_SPRITEOAM[screen][id].index = id;	// Numero de Sprite
	PA_SPRITEOAM[screen][id].x = 0;			// Coordenada X del Sprite (0 por defecto)
	PA_SPRITEOAM[screen][id].y = 0;			// Coordenada Y del Sprite (0 por defecto)
	PA_SPRITEOAM[screen][id].layer = 0;		// Prioridad en las capas (0 por defecto)
	PA_SPRITEOAM[screen][id].pal = 0;		// Paleta que usaras (0 por defecto)
	PA_SPRITEOAM[screen][id].size = SpriteSize_8x8;					// Tama�o del Sprite (macro) (8x8 por defecto)
	PA_SPRITEOAM[screen][id].color = SpriteColorFormat_256Color;	// Modo de color (macro) (256 colores)
	PA_SPRITEOAM[screen][id].gfx = NULL;			// Puntero al grafico usado
	PA_SPRITEOAM[screen][id].rot = -1;				// Id de rotacion (-1 ninguno) (0 - 31 Id de rotacion)
	PA_SPRITEOAM[screen][id].doublesize = false;	// Usar el "double size" al rotar ? ("NO" por defecto)
	PA_SPRITEOAM[screen][id].hide = true;			// Ocultar el Sprite ("SI" por defecto)
	PA_SPRITEOAM[screen][id].hflip = false;			// Volteado Horizontal ("NO" por defecto)
	PA_SPRITEOAM[screen][id].vflip = false;			// Volteado Vertical ("NO" por defecto)
	PA_SPRITEOAM[screen][id].mosaic = false;		// Mosaico ("NO" por defecto)
	PA_SPRITEOAM[screen][id].gfxid = 0;				// Numero de Gfx usado
	PA_SPRITEOAM[screen][id].frame = 0;				// Frame actual
	PA_SPRITEOAM[screen][id].framesize = 0;			// Tama�o del frame (en bytes)
	PA_SPRITEOAM[screen][id].lastframe = 0;			// Ultimo frame
	PA_SPRITEOAM[screen][id].created = false;		// Esta creado este sprite ?

}



// Funcion PA_SpriteOamSet();
void PA_SpriteOamSet(u8 screen) {

	u8 n  = 0;	// Variable de uso general

	if (screen == 0) {

		for (n = 0; n < 128; n ++) {
			oamSet(&oamMain,							// OAM pantalla superior (Main, 0)
				PA_SPRITEOAM[screen][n].index,			// Numero de Sprite
				PA_SPRITEOAM[screen][n].x,				// Coordenada X del Sprite
				PA_SPRITEOAM[screen][n].y,				// Coordenada Y del Sprite
				PA_SPRITEOAM[screen][n].layer,			// Prioridad en las capas
				PA_SPRITEOAM[screen][n].pal,			// Paleta que usaras
				PA_SPRITEOAM[screen][n].size,			// Tama�o del Sprite (macro)
				PA_SPRITEOAM[screen][n].color,			// Modo de color (macro)
				PA_SPRITEOAM[screen][n].gfx,			// Puntero al grafico usado
				PA_SPRITEOAM[screen][n].rot,			// Valor de la rotacion
				PA_SPRITEOAM[screen][n].doublesize,		// Usar el "double size" al rotar ?
				PA_SPRITEOAM[screen][n].hide,			// Ocultar el Sprite
				PA_SPRITEOAM[screen][n].hflip,			// Volteado Horizontal
				PA_SPRITEOAM[screen][n].vflip,			// Volteado Vertical
				PA_SPRITEOAM[screen][n].mosaic);		// Mosaico
		}

	} else {

		for (n = 0; n < 128; n ++) {
			oamSet(&oamSub,								// OAM pantalla superior (Main, 0)
				PA_SPRITEOAM[screen][n].index,			// Numero de Sprite
				PA_SPRITEOAM[screen][n].x,				// Coordenada X del Sprite
				PA_SPRITEOAM[screen][n].y,				// Coordenada Y del Sprite
				PA_SPRITEOAM[screen][n].layer,			// Prioridad en las capas
				PA_SPRITEOAM[screen][n].pal,			// Paleta que usaras
				PA_SPRITEOAM[screen][n].size,			// Tama�o del Sprite (macro)
				PA_SPRITEOAM[screen][n].color,			// Modo de color (macro)
				PA_SPRITEOAM[screen][n].gfx,			// Puntero al grafico usado
				PA_SPRITEOAM[screen][n].rot,			// Valor de la rotacion
				PA_SPRITEOAM[screen][n].doublesize,		// Usar el "double size" al rotar ?
				PA_SPRITEOAM[screen][n].hide,			// Ocultar el Sprite
				PA_SPRITEOAM[screen][n].hflip,			// Volteado Horizontal
				PA_SPRITEOAM[screen][n].vflip,			// Volteado Vertical
				PA_SPRITEOAM[screen][n].mosaic);		// Mosaico
		}

	}

}



// Funcion PA_SpriteSetPalColor();
void PA_SpriteSetPalColor(u8 screen, u8 pal, u8 number, u8 r, u8 g, u8 b) {

	// Verifica si esta la paleta en VRAM
	if (!PA_SPRPALSLOT[screen][pal].inuse) {
		PA_Error(111, "Sprite PAL", pal);
	}

	// Calcula el valor RGB
	u16 rgb = ((r)|((g) << 5)|((b) << 10));
	// Direccion en VRAM
	u32 address = 0;

	// Modifica la paleta
	if (screen == 0) {
		address = (0x06890000) + (pal << 9) + (number << 1);	// Calcula donde guardaras el color de la paleta
		vramSetBankF(VRAM_F_LCD);								// Bloquea el banco F para escribir las paletas
		*((u16*)address) = rgb;									// Cambia el color
		vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);				// Pon el banco F en modo paleta extendida
	} else {
		address = (0x068A0000) + (pal << 9) + (number << 1);	// Calcula donde guardaras el color de la paleta
		vramSetBankI(VRAM_I_LCD);								// Bloquea el banco I para escribir las paletas
		*((u16*)address) = rgb;									// Cambia el color
		vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);			// Pon el banco I en modo paleta extendida
	}

}



// Funcion PA_SpriteEditPalColor();
void PA_SpriteEditPalColor(u8 screen, u8 pal, u8 number, u8 r, u8 g, u8 b) {

	// Verifica si esta la paleta en VRAM
	if (!PA_SPRPALSLOT[screen][pal].inuse) {
		PA_Error(111, "Sprite PAL", pal);
	}

	// Calcula el valor RGB
	u16 rgb = ((r)|((g) << 5)|((b) << 10));

	// Calcula los valores para el HI-Byte y el LO-Byte
	u8 hibyte = ((rgb >> 8) & 0xff);
	u8 lobyte = (rgb & 0xff);

	// Graba los bytes
	*(PA_BUFFER_SPR256PAL[PA_SPRPALSLOT[screen][pal].ramslot] + (number << 1)) = lobyte;
	*(PA_BUFFER_SPR256PAL[PA_SPRPALSLOT[screen][pal].ramslot] + ((number << 1) + 1)) = hibyte;

}



// Funcion 	PA_SpriteUpdatePalette();
void PA_SpriteUpdatePalette(u8 screen, u8 pal) {

	// Verifica si esta la paleta en VRAM
	if (!PA_SPRPALSLOT[screen][pal].inuse) {
		PA_Error(111, "Sprite PAL", pal);
	}

	// Direccion en VRAM
	u32 address = 0;

	// Obten el slot donde esta la paleta en RAM
	u8 slot = PA_SPRPALSLOT[screen][pal].ramslot;

	// Actualiza la paleta en VRAM
	if (screen == 0) {
		address = (0x06890000) + (pal << 9);			// Calcula donde guardaras la paleta
		vramSetBankF(VRAM_F_LCD);					// Bloquea el banco F para escribir las paletas
		PA_DmaMemCopy((void*)address, PA_BUFFER_SPR256PAL[slot], PA_SPR256PAL[slot].size);	// Copia la paleta al banco F
		vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);	// Pon el banco F en modo paleta extendida
	} else {
		address = (0x068A0000) + (pal << 9);			// Calcula donde guardaras la paleta
		vramSetBankI(VRAM_I_LCD);					// Bloquea el banco I para escribir las paletas
		PA_DmaMemCopy((void*)address, PA_BUFFER_SPR256PAL[slot], PA_SPR256PAL[slot].size);	// Copia la paleta al banco I
		vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);	// Pon el banco I en modo paleta extendida
	}

}



// Funcion PA_SpriteGetPalColor();
void PA_SpriteGetPalColor(u8 screen, u8 pal, u8 number, u8* r, u8* g, u8* b) {

	// Verifica si esta la paleta en VRAM
	if (!PA_SPRPALSLOT[screen][pal].inuse) {
		PA_Error(111, "Sprite PAL", pal);
	}

	// Obten los bytes
	u8 lobyte = *(PA_BUFFER_SPR256PAL[PA_SPRPALSLOT[screen][pal].ramslot] + (number << 1));
	u8 hibyte = *(PA_BUFFER_SPR256PAL[PA_SPRPALSLOT[screen][pal].ramslot] + ((number << 1) + 1));

	// Calcula el RGB (compuesto)
	u16 rgb = ((hibyte << 8) | lobyte);

	// Calcula los RGB
	*r = (rgb & 0x1F);
	*g = ((rgb >> 5) & 0x1F);
	*b = ((rgb >> 10) & 0x1F);

}
