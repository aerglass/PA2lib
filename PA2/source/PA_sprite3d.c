// Includes devKitPro
#include <nds.h>

// Includes C
#include <stdio.h>
#include <string.h>

// Includes propios
#include "PA_basic.h"
#include "PA_3d.h"
#include "PA_sprite3d.h"
#include "PA_sprite256.h"

//////////////////////////////////
// Defines y variables globales //
//////////////////////////////////

// Estructura de control de los sprites 3d
PA_TYPE_3DSPRITE_IPAO PA_3DSPRITE[PA_3DSPRITES];

// Estructura de control de las texturas en VRAM
PA_TYPE_TEX256VRAM_IPAO PA_TEX256VRAM[PA_3DSPRITES];

// Estructura de control de las paletas en VRAM
PA_TYPE_3DSPRPALSLOT_IPAO PA_TEXPALSLOT[32];

// Estructura de control de la VRAM de texturas
PA_TYPE_TEXVRAM_IPAO PA_TEXVRAM;

// Define la estructura de control de los sprites 3d creados
PA_TYPE_CREATED_3DSPRITE_IPAO PA_CREATED_3DSPRITE;





// Funcion PA_Init3dSpriteSys();
void PA_Init3dSpriteSys(void) {

	// Variables locales
	u16 n = 0;

	// Inicializaciones
	for (n = 0; n < PA_3DSPRITES; n ++) {

		// Inicializa las estructuras de control de los sprites
		PA_3DSPRITE[n].x = 0;				// Coordenada X
		PA_3DSPRITE[n].y = 0;				// Coordenada Y
		PA_3DSPRITE[n].z = 0;				// Coordenada Z
		PA_3DSPRITE[n].rx = 0;				// Rotacion Eje X
		PA_3DSPRITE[n].ry = 0;				// Rotacion Eje Y
		PA_3DSPRITE[n].rz = 0;				// Rotacion Eje Z
		PA_3DSPRITE[n].rot = false;			// Rotacion en uso
		PA_3DSPRITE[n].sx = (64 << 6);		// Escala X
		PA_3DSPRITE[n].sy = (64 << 6);		// Escala X
		PA_3DSPRITE[n].scale = false;		// Escalado en uso
		PA_3DSPRITE[n].width = 0;			// Ancho del sprite
		PA_3DSPRITE[n].height = 0;			// Altura del sprite
		PA_3DSPRITE[n].inuse = false;		// Esta este sprite en uso?
		PA_3DSPRITE[n].show = false;		// Debe mostrarse?
		PA_3DSPRITE[n].gfx_tex_format = 0;	// Guarda el formato de la textura
		PA_3DSPRITE[n].gfx = 0;				// Direccion en VRAM del GFX usado
		PA_3DSPRITE[n].gfxid = 0;			// Numero de Gfx usado
		PA_3DSPRITE[n].frame = 0;			// Frame actual
		PA_3DSPRITE[n].newframe = 0;		// Frame al que cambiar
		PA_3DSPRITE[n].framesize = 0;		// Tamaño del frame (en bytes)
		PA_3DSPRITE[n].lastframe = 0;		// Ultimo frame
		PA_3DSPRITE[n].gfx_pal_format = 0;	// Guarda el formato de la paleta
		PA_3DSPRITE[n].pal = 0;				// Direccion en VRAM de la paleta usada
		PA_3DSPRITE[n].palid = 0;			// Numero de paleta usada
		PA_3DSPRITE[n].prio = 0;			// Prioridad del Sprtie
		PA_3DSPRITE[n].poly_id = 0;			// Identificador unico para el Alpha (0 por defecto, 63 prohibido)
		PA_3DSPRITE[n].alpha = 31;			// Nivel de alpha (0 - 31) (31 por defecto)

		// Inicializa las estructuras de control de la VRAM de texturas
		PA_TEX256VRAM[n].size = 0;				// Tamaño (en bytes) del Gfx
		PA_TEX256VRAM[n].width = 0;				// Ancho del Gfx
		PA_TEX256VRAM[n].height = 0;			// Altura del Gfx
		PA_TEX256VRAM[n].address = 0;			// Posicion en la VRAM
		PA_TEX256VRAM[n].ramid = 0;				// Numero de Slot en RAM del que provienes
		PA_TEX256VRAM[n].framesize = 0;			// Tamaño del frame (en bytes)
		PA_TEX256VRAM[n].lastframe = 0;			// Ultimo frame
		PA_TEX256VRAM[n].keepframes = false;	// Si es un Sprite animado, debes de mantener los frames en RAM ?
		PA_TEX256VRAM[n].inuse = false;			// Esta en uso ?

		// Inicializa las esctructuras de control de los sprites creados
		PA_CREATED_3DSPRITE.id[n] = 0;
		PA_CREATED_3DSPRITE.bck[n] = 0;

	}

	// Inicializa la estructura de datos de la VRAM de Sprites
	PA_TEXVRAM.free = 131072;		// Memoria VRAM libre (128kb)
	PA_TEXVRAM.last = 0;			// Ultima posicion usada
	PA_TEXVRAM.deleted = 0;			// Ningun Gfx borrado
	PA_TEXVRAM.fragmented = 0;		// Memoria VRAM fragmentada
	PA_TEXVRAM.inarow = 131072;		// Memoria VRAM contigua
	for (n = 0; n < PA_3DSPRITES; n ++) {
		PA_TEXVRAM.pos[n] = 0;		// Posicion en VRAM para reusar despues de un borrado
		PA_TEXVRAM.size[n] = 0;		// Tamaño del bloque libre para reusar
	}

	// Inicializa los datos de las paletas
	for (n = 0; n < 32; n ++) {
		PA_TEXPALSLOT[n].inuse = false;
		PA_TEXPALSLOT[n].ramslot = 0;
	}

	// Inicializa el numero de sprites creados
	PA_CREATED_3DSPRITE.total = 0;

	// VRAM para TEXTURAS en el Banco B
	vramSetBankB(VRAM_B_LCD);					// Bloquea el banco para la escritura
	memset((void*)0x06820000, 0, 131072);		// Borra el contenido del banco B
	PA_TEXVRAM.next = (0x06820000);				// Guarda la primera posicion de VRAM para Gfx
	vramSetBankB(VRAM_B_TEXTURE_SLOT1);			// Banco B de la VRAM para Texturas (128kb)

	// VRAM para PALETAS
	vramSetBankF(VRAM_F_LCD);					// Bloquea el banco para la escritura
	memset((void*)0x06890000, 0, 16384);		// Borra el contenido del banco F
	vramSetBankF(VRAM_F_TEX_PALETTE);			// Banco F de la VRAM para paletas extendidas (Texturas) (16kb)

	// Inicializa el OpenGL
	PA_InitOpenGL();

}



// Funcion PA_Vram3dSpriteGfx();
void PA_Vram3dSpriteGfx(u16 ram, u16 vram, bool keepframes) {

	// Verifica el rango de Id's de RAM
	if ((ram < 0) || (ram >= PA_SLOTS_SPR256GFX)) {
		PA_Error(106, "Sprite GFX", (PA_SLOTS_SPR256GFX - 1));
	}

	// Verifica si slot de RAM esta vacio
	if (PA_SPR256GFX[ram].available) {
		PA_Error(110, "Sprite GFX", ram);
	}

	// Verifica el rango de Id's de VRAM
	if ((vram < 0) || (vram >= PA_3DSPRITES)) {
		PA_Error(106, "VRAM GFX", (PA_3DSPRITES - 1));
	}

	// Verifica si el slot de VRAM esta libre
	if (PA_TEX256VRAM[vram].inuse) {
		PA_Error(109, "VRAM", vram);
	}

	// Verifica que la textura tengo un tamaño valido
	if (
		(PA_GetTextureSize(PA_SPR256GFX[ram].width) == 255)
		||
		(PA_GetTextureSize(PA_SPR256GFX[ram].height) == 255)
		) {
			PA_Error(119, NULL, ram);
	}

	// Bloquea el banco de VRAM (modo LCD) para permitir la escritura
	vramSetBankB(VRAM_B_LCD);

	// Variables de uso general
	s16 n = 0;				// General
	s16 id = 255;			// Id del posible bloque libre
	s16 last_reuse = 0;		// Nº del ultimo bloque reusable
	u32 gfxsize = 0;		// Tamaño de los datos que se copiaran
	u32 size = 0;			// Diferencia de tamaños entre bloque libre y datos
	u16 width = 0;			// Calculo de las medidas
	u16 height = 0;
	bool organize = true;	// Se debe de reorganizar el array de bloques libres ?

	// Auto calcula el tamaño de 1 frame
	width = (PA_SPR256GFX[ram].width >> 3);		// (width / 8)
	height = (PA_SPR256GFX[ram].height >> 3);	// (height / 8)
	PA_TEX256VRAM[vram].framesize = ((width * height) << 6);	// ((width * height) * 64)
	// Auto calcula el ultimo frame de la animacion
	PA_TEX256VRAM[vram].lastframe = ((int)(PA_SPR256GFX[ram].size / PA_TEX256VRAM[vram].framesize)) - 1;
	PA_TEX256VRAM[vram].inuse = true;						// Slot ocupado

	// Calcula el tamaño del grafico a copiar segun si debes o no copiar todos los frames
	if (keepframes) {	// Si debes de mantener los frames en RAM, solo copia el primero
		gfxsize = PA_TEX256VRAM[vram].framesize;
	} else {			// Si no, copialos todos
		gfxsize = PA_SPR256GFX[ram].size;
	}

	// Actualiza la VRAM disponible
	PA_TEXVRAM.free -= gfxsize;

	// Si no hay suficiente VRAM, error
	if (PA_TEXVRAM.free < 0) {
		PA_Error(113, "Sprites", gfxsize);
	}

	// Si hay que aprovechar algun bloque borrado... (tamaño identico, preferente)
	if (PA_TEXVRAM.deleted > 0) {
		// Busca un bloque vacio del tamaño identico
		for (n = 0; n < PA_TEXVRAM.deleted; n ++) {
			if (PA_TEXVRAM.size[n] == gfxsize) {		// Si el bloque tiene el tamaño suficiente
				id = n;		// Guarda la Id
				n = PA_TEXVRAM.deleted;	// y sal
			}
		}
		// Si no habia ningun bloque de tamaño identico, busca el mas parecido (produce fragmentacion)
		if (id != 255) {
			for (n = 0; n < PA_TEXVRAM.deleted; n ++) {
				if (PA_TEXVRAM.size[n] > gfxsize) {		// Si el bloque tiene el tamaño suficiente
					id = n;		// Guarda la Id
					n = PA_TEXVRAM.deleted;	// y sal
				}
			}
		}
	} 
	
	// Si hay algun bloque borrado libre del tamaño suficiente...
	if (id != 255) {

		// Transfiere el grafico a la VRAM
		PA_DmaMemCopy((void*)PA_TEXVRAM.pos[id], PA_BUFFER_SPR256GFX[ram], gfxsize);
		// Guarda el puntero donde lo has almacenado
		PA_TEX256VRAM[vram].address = PA_TEXVRAM.pos[id];

		// Si no has usado todo el tamaño, deja constancia
		if (gfxsize < PA_TEXVRAM.size[id]) {

			// Calcula el tamaño del nuevo bloque libre
			size = PA_TEXVRAM.size[id] - gfxsize;
			// Actualiza los datos
			PA_TEXVRAM.pos[id] += gfxsize;			// Nueva direccion
			PA_TEXVRAM.size[id] = size;				// Nuevo tamaño
			PA_TEXVRAM.fragmented -= gfxsize;		// Actualiza el contador de VRAM fragmentada
			organize = false;	// No se debe de reorganizar el array de bloques

		} else {	// Si has usado todo el tamaño, deja constancia

			PA_TEXVRAM.fragmented -= PA_TEXVRAM.size[id];	// Actualiza el contador de VRAM fragmentada

		}

		// Se tiene que reorganizar el array de bloques libres ?
		if (organize) {
			last_reuse = (PA_TEXVRAM.deleted - 1);
			if (
			(last_reuse > 0)	// Si hay mas de un bloque borrado
			&&
			(id != last_reuse)	// Y no es la ultima posicion
			) {
				// Coloca los valores de la ultima posicion en esta
				PA_TEXVRAM.pos[id] = PA_TEXVRAM.pos[last_reuse];		// Nueva direccion
				PA_TEXVRAM.size[id] = PA_TEXVRAM.size[last_reuse];		// Nuevo tamaño
			}
			PA_TEXVRAM.deleted --;		// Actualiza el contador de bloques libres, borrando el ultimo registro
		}

	} else {	// Si no habia ningun bloque borrado o con el tamaño suficiente, colacalo al final de la VRAM ocupada

		// Actualiza la VRAM contigua disponible (mayor bloque libre al final)
		PA_TEXVRAM.inarow -= gfxsize;

		// Si no hay suficiente VRAM (contigua), error
		if (PA_TEXVRAM.inarow < 0) {
			PA_Error(113, "Sprites", gfxsize);
		}

		// Transfiere el grafico a la VRAM
		PA_DmaMemCopy((void*)PA_TEXVRAM.next, PA_BUFFER_SPR256GFX[ram], gfxsize);
		// Guarda el puntero donde lo has almacenado
		PA_TEX256VRAM[vram].address = PA_TEXVRAM.next;
		// Guarda la direccion actual como la ultima usada
		PA_TEXVRAM.last = PA_TEXVRAM.next;
		// Calcula la siguiente posicion libre
		PA_TEXVRAM.next += gfxsize;

	}

	// Guarda los datos del Gfx que se copiara a la VRAM.
	PA_TEX256VRAM[vram].size = gfxsize;						// Tamaño en bytes de los datos copiados
	PA_TEX256VRAM[vram].width = PA_SPR256GFX[ram].width;	// Alto (px)
	PA_TEX256VRAM[vram].height = PA_SPR256GFX[ram].height;	// Ancho (px)
	PA_TEX256VRAM[vram].ramid = ram;						// Slot RAM de origen
	PA_TEX256VRAM[vram].keepframes = keepframes;			// Debes guardar los frames en RAM o copiarlos a la VRAM?

	// Restaura el banco de VRAM en modo Textura
	vramSetBankB(VRAM_B_TEXTURE_SLOT1);			// Banco B de la VRAM para Texturas (128kb)

}



// Funcion PA_Free3dSpriteGfx();
void PA_Free3dSpriteGfx(u16 id) {

	// Verifica si hay un grafico cargado en esa Id.
	if (!PA_TEX256VRAM[id].inuse) {
		PA_Error(110, "Sprite Gfx", id);
	}

	// Bloquea el banco de VRAM (modo LCD) para permitir la escritura
	vramSetBankB(VRAM_B_LCD);

	// Borra el Gfx de la VRAM (pon a 0 todos los Bytes)
	memset((void*)PA_TEX256VRAM[id].address, 0, PA_TEX256VRAM[id].size);

	// Actualiza la cantidad de VRAM disponible
	PA_TEXVRAM.free += PA_TEX256VRAM[id].size;

	// Guarda la posicion y tamaño del bloque borrado para su reutilizacion
	PA_TEXVRAM.pos[PA_TEXVRAM.deleted] = PA_TEX256VRAM[id].address;
	PA_TEXVRAM.size[PA_TEXVRAM.deleted] = PA_TEX256VRAM[id].size;

	// Incrementa en contador de bloques borrados
	PA_TEXVRAM.deleted ++;

	// Incrementa el contador de memoria fragmentada
	PA_TEXVRAM.fragmented += PA_TEX256VRAM[id].size;

	// Reinicia los datos de esta Id. de gfx
	PA_TEX256VRAM[id].size = 0;			// Tamaño en bytes
	PA_TEX256VRAM[id].width = 0;		// Alto (px)
	PA_TEX256VRAM[id].height = 0;		// Ancho (px)
	PA_TEX256VRAM[id].address = 0;		// Puntero en VRAM
	PA_TEX256VRAM[id].framesize = 0;	// Tamaño del frame (en bytes)
	PA_TEX256VRAM[id].lastframe = 0;	// Ultimo frame
	PA_TEX256VRAM[id].inuse = false;

	// Debes desfragmentar la VRAM
	if (PA_TEXVRAM.fragmented >= (PA_TEXVRAM.inarow >> 1)) PA_Vram3dSpriteGfxDefrag();

	// Restaura el banco de VRAM en modo Textura
	vramSetBankB(VRAM_B_TEXTURE_SLOT1);			// Banco B de la VRAM para Texturas (128kb)

}



// Funcion PA_Vram3dSpriteGfxDefrag();
void PA_Vram3dSpriteGfxDefrag(void) {

	// Bloquea el banco de VRAM (modo LCD) para permitir la escritura
	vramSetBankB(VRAM_B_LCD);

	// Calcula la VRAM en uso y crea un buffer para guardarla
	u32 used_vram = ((131072 - PA_TEXVRAM.free) + 1);
	char* buffer;
	buffer = (char*) calloc (used_vram, sizeof(char));
	if (buffer == NULL) {		// Si no hay suficiente RAM libre
		PA_Error(102, NULL, used_vram);
	}

	char* address[PA_3DSPRITES];	// Guarda la direccion en RAM
	u32 size[PA_3DSPRITES];			// Guarda el tamaño
	u32 ram = 0;					// Puntero inicial de RAM
	u16 n = 0;						// Variable General
	u16 x_size = 0;					// Formato de la textura
	u16 y_size = 0;
	u32 gfx_address = 0;

	// Copia los datos de la VRAM a la RAM
	for (n = 0; n < PA_3DSPRITES; n ++) {
		// Si esta en uso
		if (PA_TEX256VRAM[n].inuse) {
			// Copia el Gfx a la RAM
			address[n] = (buffer + ram);		// Calcula el puntero
			size[n] = PA_TEX256VRAM[n].size;		// Almacena el tamaño
			PA_DmaMemCopy(address[n], (void*)PA_TEX256VRAM[n].address, size[n]);	// Copialo a la VRAM
			ram += size[n];		// Siguiente posicion en RAM (relativa)
		}
	}

	// Inicializa la estructura de datos de la VRAM de Sprites
	PA_TEXVRAM.free = 131072;		// Memoria VRAM libre (128kb)
	PA_TEXVRAM.last = 0;			// Ultima posicion usada
	PA_TEXVRAM.deleted = 0;			// Ningun Gfx borrado
	PA_TEXVRAM.fragmented = 0;		// Memoria VRAM fragmentada
	PA_TEXVRAM.inarow = 131072;		// Memoria VRAM contigua
	for (n = 0; n < PA_3DSPRITES; n ++) {
		PA_TEXVRAM.pos[n] = 0;		// Posicion en VRAM para reusar despues de un borrado
		PA_TEXVRAM.size[n] = 0;		// Tamaño del bloque libre para reusar
	}
	// Aplica la direccion de inicio de la VRAM
	PA_TEXVRAM.next = (0x06820000);

	// Ahora, copia de nuevo los datos a la VRAM, pero alineados
	for (n = 0; n < PA_3DSPRITES; n ++) {
		// Si esta en uso
		if (PA_TEX256VRAM[n].inuse) {
			PA_DmaMemCopy((void*)PA_TEXVRAM.next, address[n], size[n]);		// Vuelve a colocar la el Gfx en VRAM
			PA_TEX256VRAM[n].address = PA_TEXVRAM.next;		// Guarda la nueva posicion en VRAM
			PA_TEXVRAM.free -= size[n];		// Ram libre
			PA_TEXVRAM.inarow -= size[n];	// Ram libre en bloque
			PA_TEXVRAM.last = PA_TEXVRAM.next;	// Guarda la posicion como ultima usada
			PA_TEXVRAM.next += size[n];		// Y calcula la siguiente posicion a escribir
		}
	}

	// Realinea los Sprites con sus graficos
	for (n = 0; n < PA_3DSPRITES; n ++) {
		if (PA_3DSPRITE[n].inuse) {
			// Asigna la nueva direccion de memoria
			PA_3DSPRITE[n].gfx = PA_TEX256VRAM[PA_3DSPRITE[n].gfxid].address;
			if (PA_TEX256VRAM[PA_3DSPRITE[n].gfxid].keepframes) {
				gfx_address = PA_3DSPRITE[n].gfx;
			} else {
				gfx_address = (PA_3DSPRITE[n].gfx + (PA_3DSPRITE[n].framesize * PA_3DSPRITE[n].frame));
			}
			// Recalcula el formato de la textura
			x_size = PA_GetTextureSize(PA_3DSPRITE[n].width);
			y_size = PA_GetTextureSize(PA_3DSPRITE[n].height);
			PA_3DSPRITE[n].gfx_tex_format = (((gfx_address >> 3) & 0xFFFF) | (x_size << 20) | (y_size << 23) | (GL_RGB256 << 26) | GL_TEXTURE_COLOR0_TRANSPARENT | TEXGEN_OFF);
		}
	}

	// Vacia el buffer
	free(buffer);
	buffer = NULL;

	// Restaura el banco de VRAM en modo Textura
	vramSetBankB(VRAM_B_TEXTURE_SLOT1);			// Banco B de la VRAM para Texturas (128kb)

}



// Funcion PA_Vram3dSpritePal();
void PA_Vram3dSpritePal(u8 id, u8 slot) {

	// Verifica el rango de Id's
	if ((id < 0) || (id >= PA_SLOTS_SPR256PAL)) {
		PA_Error(106, "Sprite PAL", PA_SLOTS_SPR256PAL);
	}

	// Verifica si la Id esta libre
	if (PA_SPR256PAL[id].available) {
		PA_Error(110, "Sprite PAL", id);
	}

	// Verifica si te has salido de rango (Paleta)
	if ((slot < 0) || (slot > 31)) {
		PA_Error(106, "Sprite Palette Slot", 31);
	}

	// Copia la paleta a la VRAM, segun el Slot
	u32 address = (0x06890000) + (slot << 9);			// Calcula donde guardaras la paleta
	vramSetBankF(VRAM_F_LCD);			// Bloquea el banco F para escribir las paletas
	PA_DmaMemCopy((void*)address, PA_BUFFER_SPR256PAL[id], PA_SPR256PAL[id].size);	// Copia la paleta al banco F
	vramSetBankF(VRAM_F_TEX_PALETTE);	// Banco F de la VRAM para paletas extendidas (Texturas)

	// Guarda los parametros
	PA_TEXPALSLOT[slot].inuse = true;			// Marca el SLOT de paleta como en uso
	PA_TEXPALSLOT[slot].ramslot = id;			// Guarda el slot de RAM donde esta la paleta original

}



// Funcion PA_Create3dSprite();
void PA_Create3dSprite(u16 id, u16 gfx, u16 pal, s16 x, s16 y) {

	// Verifica el rango de Id's de Sprites
	if ((id < 0) || (id > (PA_3DSPRITES - 1))) {
		PA_Error(106, "3D Sprite", (PA_3DSPRITES - 1));
	}

	// Verifica el rango de Id's de Gfx
	if ((gfx < 0) || (gfx > (PA_3DSPRITES - 1))) {
		PA_Error(106, "3D Sprite GFX", (PA_3DSPRITES - 1));
	}

	// Verifica si esta el Gfx en VRAM
	if (!PA_TEX256VRAM[gfx].inuse) {
		PA_Error(111, "3D Sprite GFX", gfx);
	}

	// Verifica el rango de slots de paletas
	if ((pal < 0) || (pal > 31)) {
		PA_Error(106, "3D Sprite Palette Slot", 31);
	}

	// Verifica si esta la paleta en VRAM
	if (!PA_TEXPALSLOT[pal].inuse) {
		PA_Error(111, "3D Sprite PAL", pal);
	}

	// Calcula la direccion de la paleta y almacenala en la estructura del sprite
	u32 pal_address = (pal << 9);
	PA_3DSPRITE[id].pal = pal_address;	// Direccion en VRAM de la paleta usada (relativa a VRAM_F)
	PA_3DSPRITE[id].palid = pal;		// Numero de paleta usada
	PA_3DSPRITE[id].gfx_pal_format = (pal_address >> 4) & 0x1FFF;	// Formato de la paleta

	// Calcula la direccion de la textura y almacenala en la estructura del sprite
	u32 gfx_address = PA_TEX256VRAM[gfx].address;
	PA_3DSPRITE[id].gfx = gfx_address;		// Direccion en VRAM del GFX usado
	PA_3DSPRITE[id].gfxid = gfx;			// Numero de Gfx usado
	u16 x_size = PA_GetTextureSize(PA_TEX256VRAM[gfx].width);
	u16 y_size = PA_GetTextureSize(PA_TEX256VRAM[gfx].height);
	PA_3DSPRITE[id].gfx_tex_format = (((gfx_address >> 3) & 0xFFFF) | (x_size << 20) | (y_size << 23) | (GL_RGB256 << 26) | GL_TEXTURE_COLOR0_TRANSPARENT | TEXGEN_OFF);

	// Guarda los demas parametros del sprite
	PA_3DSPRITE[id].x = x;
	PA_3DSPRITE[id].y = y;
	PA_3DSPRITE[id].z = 0;
	PA_3DSPRITE[id].width = PA_TEX256VRAM[gfx].width;
	PA_3DSPRITE[id].height = PA_TEX256VRAM[gfx].height;
	PA_3DSPRITE[id].framesize = PA_TEX256VRAM[gfx].framesize;
	PA_3DSPRITE[id].lastframe = PA_TEX256VRAM[gfx].lastframe;
	PA_3DSPRITE[id].inuse = true;
	PA_3DSPRITE[id].show = true;
	PA_3DSPRITE[id].prio = PA_CREATED_3DSPRITE.total;
	PA_3DSPRITE[id].poly_id = 0;
	PA_3DSPRITE[id].alpha = 31;

	// Ahora registra su creacion
	PA_CREATED_3DSPRITE.id[PA_CREATED_3DSPRITE.total] = id;
	PA_CREATED_3DSPRITE.total ++;
	
}



// Funcion PA_Delete3dSprite();
void PA_Delete3dSprite(u16 id) {

	// Verifica el rango de Id's de Sprites
	if ((id < 0) || (id > (PA_3DSPRITES - 1))) {
		PA_Error(106, "3D Sprite", (PA_3DSPRITES - 1));
	}

	// Verifica si el Sprite esta creado
	if (!PA_3DSPRITE[id].inuse) {
		PA_Error(112, "3D", id);
	}

	// Resetea los parametros del Sprite dado
	PA_3DSPRITE[id].x = 0;				// Coordenada X
	PA_3DSPRITE[id].y = 0;				// Coordenada Y
	PA_3DSPRITE[id].z = 0;				// Coordenada Z
	PA_3DSPRITE[id].rx = 0;				// Rotacion Eje X
	PA_3DSPRITE[id].ry = 0;				// Rotacion Eje Y
	PA_3DSPRITE[id].rz = 0;				// Rotacion Eje Z
	PA_3DSPRITE[id].rot = false;		// Rotacion en uso
	PA_3DSPRITE[id].sx = (64 << 6);		// Escala X
	PA_3DSPRITE[id].sy = (64 << 6);		// Escala X
	PA_3DSPRITE[id].scale = false;		// Escalado en uso
	PA_3DSPRITE[id].width = 0;			// Ancho del sprite
	PA_3DSPRITE[id].height = 0;			// Altura del sprite
	PA_3DSPRITE[id].inuse = false;		// Esta este sprite en uso?
	PA_3DSPRITE[id].show = false;		// Debe mostrarse?
	PA_3DSPRITE[id].gfx_tex_format = 0;	// Guarda el formato de la textura
	PA_3DSPRITE[id].gfx = 0;			// Direccion en VRAM del GFX usado
	PA_3DSPRITE[id].gfxid = 0;			// Numero de Gfx usado
	PA_3DSPRITE[id].frame = 0;			// Frame actual
	PA_3DSPRITE[id].newframe = 0;		// Frame al que cambiar
	PA_3DSPRITE[id].framesize = 0;		// Tamaño del frame (en bytes)
	PA_3DSPRITE[id].lastframe = 0;		// Ultimo frame
	PA_3DSPRITE[id].gfx_pal_format = 0;	// Guarda el formato de la paleta
	PA_3DSPRITE[id].pal = 0;			// Direccion en VRAM de la paleta usada
	PA_3DSPRITE[id].palid = 0;			// Numero de paleta usada
	PA_3DSPRITE[id].prio = 0;			// Prioridad del Sprtie
	PA_3DSPRITE[id].poly_id = 0;		// Identificador unico para el Alpha (0 por defecto, 63 prohibido)
	PA_3DSPRITE[id].alpha = 31;			// Nivel de alpha (0 - 31) (31 por defecto)

	// Ahora vamos a eliminar de la cola es sprite seleccionado
	u16 n1 = 0;
	u16 n2 = 0;
	// Si hay mas de 1 Sprite creado...
	if (PA_CREATED_3DSPRITE.total > 1) {
		for (n1 = 0; n1 < PA_CREATED_3DSPRITE.total; n1 ++) {
			// Si el Sprite no es el seleccionado, copialo al array temporal
			if (PA_CREATED_3DSPRITE.id[n1] != id) {
				PA_CREATED_3DSPRITE.bck[n2] = PA_CREATED_3DSPRITE.id[n1];
				n2 ++;
			}
		}
		// Marca que hay un sprite menos
		PA_CREATED_3DSPRITE.total --;

		// Ahora, copia el array modificado al principal
		for (n1 = 0; n1 < PA_CREATED_3DSPRITE.total; n1 ++) {
			PA_CREATED_3DSPRITE.id[n1] = PA_CREATED_3DSPRITE.bck[n1];
		}
	} else {
		// Si era el ultimo Sprite de la cola...
		PA_CREATED_3DSPRITE.id[0] = 0;
		PA_CREATED_3DSPRITE.total = 0;
	}
	
}



// Funcion PA_Sort3dSprites();
void PA_Sort3dSprites(void) {

	// Variables
	u16 n1 = 0;
	u16 n2 = 0;
	u16 id1 = 0;
	u16 id2 = 0;

	// Si hay mas de 1 item...
	if (PA_CREATED_3DSPRITE.total > 1) {

		// Ordena la cola de sprites creados
		for (n1 = 0; n1 < (PA_CREATED_3DSPRITE.total - 1); n1 ++) {
			for (n2 = (n1 + 1); n2 < PA_CREATED_3DSPRITE.total; n2 ++) {
				// Si es menor, reordenalo
				if (PA_CREATED_3DSPRITE.id[n2] < PA_CREATED_3DSPRITE.id[n1]) {
					id2 = PA_CREATED_3DSPRITE.id[n2];
					id1 = PA_CREATED_3DSPRITE.id[n1];
					// Guarda las IDs de ordenadas en la cola
					PA_CREATED_3DSPRITE.id[n1] = id2;
					PA_CREATED_3DSPRITE.id[n2] = id1;
					// Actualiza la iPAormacion de prioridades del sprite
					PA_3DSPRITE[id2].prio = n1;
					PA_3DSPRITE[id1].prio = n2;
				}
			}
		}

	}

}



// Funcion PA_Set3dSpritePriority();
void PA_Set3dSpritePriority(u16 id, u16 prio) {

	// Variables
	u16 n = 0;
	u16 a = 0;
	u16 b = 0;

	// Realiza una copia de seguridad de la cola
	for (n = 0; n < PA_CREATED_3DSPRITE.total; n ++) {
		PA_CREATED_3DSPRITE.bck[n] = PA_CREATED_3DSPRITE.id[n];
	}

	// Coloca el Sprite en la prioridad indicada
	PA_CREATED_3DSPRITE.id[prio] = id;
	PA_3DSPRITE[id].prio = prio;
	for (n = 0; n < PA_CREATED_3DSPRITE.total; n ++) {
		// Si no coinciden ni Id ni prioridad...
		if ((a != prio) && (PA_CREATED_3DSPRITE.bck[b] != id)) {
			PA_CREATED_3DSPRITE.id[a] = PA_CREATED_3DSPRITE.bck[b];
			PA_3DSPRITE[PA_CREATED_3DSPRITE.id[a]].prio = a;
			a ++;
			b ++;
		} else {
			// Si coincide la prioridad, saltatela
			if (a == prio) {
				a ++;
			}
			// Si coincide la ID, saltatela
			if (PA_CREATED_3DSPRITE.bck[b] == id) {
				b ++;
			}
		}
	}

}



// Funcion PA_Swap3dSpritePriority();
void PA_Swap3dSpritePriority(u16 id_a, u16 id_b) {

	// Variables, obten las prioridades a cambiar
	u16 prio_a = PA_3DSPRITE[id_b].prio;
	u16 prio_b = PA_3DSPRITE[id_a].prio;

	// Asignalas en su nuevo orden
	PA_3DSPRITE[id_a].prio = prio_a;
	PA_3DSPRITE[id_b].prio = prio_b;

	// Y por ultimo, modifica la cola de sprites
	PA_CREATED_3DSPRITE.id[prio_a] = id_a;
	PA_CREATED_3DSPRITE.id[prio_b] = id_b;

}



// Funcion PA_Move3dSprite();
void PA_Move3dSprite(u16 id, s16 x, s16 y) {

	// Actualiza las coordenadas del Sprite
	PA_3DSPRITE[id].x = x;
	PA_3DSPRITE[id].y = y;

}



// Funcion PA_Show3dSprite();
void PA_Show3dSprite(u16 id, bool show) {

	// Actualiza el flag de visibilidad
	PA_3DSPRITE[id].show = show;

}



// Funcion PA_Set3dSpriteFrame();
void PA_Set3dSpriteFrame(u16 id, u16 frame) {

	// Verifica el rango de Id's de Sprites
	if ((id < 0) || (id > (PA_3DSPRITES - 1))) {
		PA_Error(106, "3D Sprite", (PA_3DSPRITES - 1));
	}

	// Verifica si el Sprite esta creado
	if (!PA_3DSPRITE[id].inuse) {
		PA_Error(112, "3D", id);
	}

	// Verifica el rango de frames del Sprite
	if (frame > PA_3DSPRITE[id].lastframe) {
		PA_Error(106, "Sprite frame", PA_3DSPRITE[id].lastframe);
	}

	// Si debes de copiar el nuevo frame desde la RAM a la VRAM...
	if (PA_TEX256VRAM[PA_3DSPRITE[id].gfxid].keepframes) {

		// Marca para que se copie la nueva textura a la VRAM durante la actualizacion de los Sprites 3D
		PA_3DSPRITE[id].newframe = frame;

	} else {	// Si todos los frames ya estan en VRAM...

		// Calcula la direccion del Gfx del frame
		u32 gfx_address = (PA_3DSPRITE[id].gfx + (PA_3DSPRITE[id].framesize * frame));
		u16 x_size = PA_GetTextureSize(PA_3DSPRITE[id].width);
		u16 y_size = PA_GetTextureSize(PA_3DSPRITE[id].height);
		PA_3DSPRITE[id].gfx_tex_format = (((gfx_address >> 3) & 0xFFFF) | (x_size << 20) | (y_size << 23) | (GL_RGB256 << 26) | GL_TEXTURE_COLOR0_TRANSPARENT | TEXGEN_OFF);
		// Guarda el numero de frame actual
		PA_3DSPRITE[id].frame = frame;
		PA_3DSPRITE[id].newframe = frame;

	}

}



// Funcion PA_Draw3dSprites();
void PA_Draw3dSprites(void) {

	// Variables
	u16 n = 0;		// Uso general
	s16 x1 = 0;		// Vertices
	s16 x2 = 0;
	s16 y1 = 0;
	s16 y2 = 0;
	s16 x = 0;
	s16 y = 0;
	s16 z = 0;
	u16 id = 0;

	// Si hay Sprites 3D que dibujar...
	if (PA_CREATED_3DSPRITE.total > 0) {
		// Dibuja todos los sprites creados
		for (n = 0; n < PA_CREATED_3DSPRITE.total; n ++) {
			// Obten la ID del sprite actual
			id = PA_CREATED_3DSPRITE.id[n];
			// Si el sprite es visible...
			if (PA_3DSPRITE[id].show) {
				// Calcula la Z actual
				z = (n + PA_3DSPRITE[id].z);
				// Aplicale el alpha indicado
				glPolyFmt(POLY_ALPHA(PA_3DSPRITE[id].alpha) | POLY_ID(PA_3DSPRITE[id].poly_id) | POLY_CULL_NONE);
				// Hay que aplicarle rotacion o escalado?
				if (PA_3DSPRITE[id].rot || PA_3DSPRITE[id].scale) {
					// Guarda la matriz
					glPushMatrix();
					// Trasladate al centro del Sprite
					x = (PA_3DSPRITE[id].x + (PA_3DSPRITE[id].width >> 1));
					y = (PA_3DSPRITE[id].y + (PA_3DSPRITE[id].height >> 1));
					glTranslatef32(x, y, z);
					// Aplica la rotacion
					if (PA_3DSPRITE[id].rot) {
						glRotateXi(PA_3DSPRITE[id].rx);
						glRotateYi(PA_3DSPRITE[id].ry);
						glRotateZi(PA_3DSPRITE[id].rz);
					}
					// Aplica el escalado
					if (PA_3DSPRITE[id].scale) {
						glScalef32(PA_3DSPRITE[id].sx, PA_3DSPRITE[id].sy, 0);
					}
					// Vuelve a la posicion original
					glTranslatef32(-x, -y, -z);
				}
				// Aplica la textura
				GFX_PAL_FORMAT = PA_3DSPRITE[id].gfx_pal_format;
				GFX_TEX_FORMAT = PA_3DSPRITE[id].gfx_tex_format;
				// Calcula los vertices
				x1 = PA_3DSPRITE[id].x;
				x2 = (PA_3DSPRITE[id].x + PA_3DSPRITE[id].width);
				y1 = PA_3DSPRITE[id].y;
				y2 = (PA_3DSPRITE[id].y + PA_3DSPRITE[id].height);
				// Dibuja el poligono en la pantalla
				glBegin(GL_QUAD);
					// Vertice 1 (arriba, izquierda)
					glTexCoord2t16(inttot16(0), inttot16(0));
					glVertex3v16(x1, y1, z);
					// Vertice 2 (abajo, izquierda)
					glTexCoord2t16(inttot16(0), inttot16(PA_3DSPRITE[id].height));
					glVertex3v16(x1, y2, z);
					// Vertice 3 (abajo, derecha)
					glTexCoord2t16(inttot16(PA_3DSPRITE[id].width), inttot16(PA_3DSPRITE[id].height));
					glVertex3v16(x2, y2, z);
					// Vertice 4 (arriba, derecha)
					glTexCoord2t16(inttot16(PA_3DSPRITE[id].width), inttot16(0));
					glVertex3v16(x2, y1, z);
				// Has aplicado rotacion o escalado?, restaura la matriz
				if (PA_3DSPRITE[id].rot || PA_3DSPRITE[id].scale) {
					// Restaura la matriz
					glPopMatrix(1);
				}
			}
		}

	}

}



// Funcion PA_Update3dSpritesGfx();
void PA_Update3dSpritesGfx(void) {

	// Variables
	u16 n = 0;				// Uso general
	u16 id = 0;
	char* source;			// Puntero de origen
	u32 destination = 0;	// Puntero de destino
	u16 ramid = 0;			// Slot de RAM donde se encuentra el Gfx

	// Si hay Sprites 3D que actualizar...
	if (PA_CREATED_3DSPRITE.total > 0) {

		// Si es necesario, actualiza las texturas de la RAM a la VRAM
		// Bloquea el banco de VRAM (modo LCD) para permitir la escritura
		vramSetBankB(VRAM_B_LCD);

		// Busca los frames a actualizar
		for (n = 0; n < PA_CREATED_3DSPRITE.total; n ++) {
			// Obten la ID del sprite actual
			id = PA_CREATED_3DSPRITE.id[n];
			// Si los frames adicionales estan en RAM y debe de cambiar de frame
			if (
				PA_TEX256VRAM[PA_3DSPRITE[id].gfxid].keepframes
				&&
				(PA_3DSPRITE[id].frame != PA_3DSPRITE[id].newframe)
				) {
				// Calcula el origen y destino del nuevo frame a copiar
				ramid =  PA_TEX256VRAM[PA_3DSPRITE[id].gfxid].ramid;
				source = PA_BUFFER_SPR256GFX[ramid] + (PA_3DSPRITE[id].framesize * PA_3DSPRITE[id].newframe);
				destination = PA_TEX256VRAM[PA_3DSPRITE[id].gfxid].address;
				// Copialo
				PA_DmaMemCopy((void*)destination, source, PA_3DSPRITE[id].framesize);
				// Y actualiza el frame actual
				PA_3DSPRITE[id].frame = PA_3DSPRITE[id].newframe;
			}
		}

		// Restaura el banco de VRAM en modo Textura
		vramSetBankB(VRAM_B_TEXTURE_SLOT1);			// Banco B de la VRAM para Texturas (128kb)

	}

}



// Funcion PA_Rotate3dSprite();
void PA_Rotate3dSprite(u16 id, s16 x, s16 y, s16 z) {

	// Variables locales
	s16 temp = 0;
	s16 rx = 0;
	s16 ry = 0;
	s16 rz = 0;

	// Calcula la rotacion eje X
	if (x < 0) {
		temp = -x;
		rx = (temp << 6);
		rx = -rx;
	} else {
		rx = (x << 6);
	}
	PA_3DSPRITE[id].rx = rx;

	// Calcula la rotacion eje Y
	if (y < 0) {
		temp = -y;
		ry = (temp << 6);
		ry = -ry;
	} else {
		ry = (y << 6);
	}
	PA_3DSPRITE[id].ry = ry;

	// Calcula la rotacion eje Z
	if (z < 0) {
		temp = -z;
		rz = (temp << 6);
		rz = -rz;
	} else {
		rz = (z << 6);
	}
	PA_3DSPRITE[id].rz = rz;

	// Cambia el estado del flag de rotacion
	if ((PA_3DSPRITE[id].rx != 0) || (PA_3DSPRITE[id].ry != 0) || (PA_3DSPRITE[id].rz != 0)) {
		PA_3DSPRITE[id].rot = true;
	} else {
		PA_3DSPRITE[id].rot = false;
	}

}



// Funcion PA_Scale3dSprite();
void PA_Scale3dSprite(u16 id, u16 x, u16 y) {

	// Calcula la rotacion eje X
	PA_3DSPRITE[id].sx = (x << 6);

	// Calcula la rotacion eje Y
	PA_3DSPRITE[id].sy = (y << 6);

	// Cambia el estado del flag de rotacion
	if ((PA_3DSPRITE[id].sx != (64 << 6)) || (PA_3DSPRITE[id].sy != (64 << 6))) {
		PA_3DSPRITE[id].scale = true;
	} else {
		PA_3DSPRITE[id].scale = false;
	}

}



// Funcion PA_Blend3dSprite();
void PA_Blend3dSprite(u8 sprite, u8 poly_id, u8 alpha) {
	// Si el nivel de alpha es opaco o el ID es 0...
	if ((poly_id == 0) || (alpha == 31)) {
		PA_3DSPRITE[sprite].poly_id = 0;
		PA_3DSPRITE[sprite].alpha = 31;
	} else {
		// De lo contrario, asignale el ID y alpha indicados
		PA_3DSPRITE[sprite].poly_id = poly_id;
		PA_3DSPRITE[sprite].alpha = alpha;
	}
}



// Funcion PA_3dSpritesLayer();
void PA_3dSpritesLayer(u8 layer) {

	// Resetea los BITS de control de prioridad en todos los fondos
	REG_BG0CNT &= 0xfffc;		// Pon a 0 los bits 0 y 1 del registro
	REG_BG1CNT &= 0xfffc;
	REG_BG2CNT &= 0xfffc;
	REG_BG3CNT &= 0xfffc;

	// Reordena todas las capas segun la solicitud
	switch (layer) {
		case 0:		// 3D Sprites en la capa 0
			REG_BG0CNT += BG_PRIORITY_0;
			REG_BG1CNT += BG_PRIORITY_1;
			REG_BG2CNT += BG_PRIORITY_2;
			REG_BG3CNT += BG_PRIORITY_3;
			break;
		case 1:		// 3D Sprites en la capa 1
			REG_BG0CNT += BG_PRIORITY_1;
			REG_BG1CNT += BG_PRIORITY_0;
			REG_BG2CNT += BG_PRIORITY_2;
			REG_BG3CNT += BG_PRIORITY_3;
			break;
		case 2:		// 3D Sprites en la capa 2
			REG_BG0CNT += BG_PRIORITY_2;
			REG_BG1CNT += BG_PRIORITY_0;
			REG_BG2CNT += BG_PRIORITY_1;
			REG_BG3CNT += BG_PRIORITY_3;
			break;
		case 3:		// 3D Sprites en la capa 3
			REG_BG0CNT += BG_PRIORITY_3;
			REG_BG1CNT += BG_PRIORITY_0;
			REG_BG2CNT += BG_PRIORITY_1;
			REG_BG3CNT += BG_PRIORITY_2;
			break;
		default:	// 3D Sprites en la capa 0
			REG_BG0CNT += BG_PRIORITY_0;
			REG_BG1CNT += BG_PRIORITY_1;
			REG_BG2CNT += BG_PRIORITY_2;
			REG_BG3CNT += BG_PRIORITY_3;
			break;
	}

}



// Funcion PA_3dSpriteEditPalColor();
void PA_3dSpriteEditPalColor(u8 pal, u8 number, u8 r, u8 g, u8 b) {

	// Verifica si esta la paleta en VRAM
	if (!PA_TEXPALSLOT[pal].inuse) {
		PA_Error(111, "Sprite PAL", pal);
	}

	// Calcula el valor RGB
	u16 rgb = ((r)|((g) << 5)|((b) << 10));

	// Calcula los valores para el HI-Byte y el LO-Byte
	u16 hibyte = (rgb >> 8);
	u16 lobyte = rgb - (hibyte << 8);

	// Graba los bytes
	*(PA_BUFFER_SPR256PAL[PA_TEXPALSLOT[pal].ramslot] + (number << 1)) = lobyte;
	*(PA_BUFFER_SPR256PAL[PA_TEXPALSLOT[pal].ramslot] + ((number << 1) + 1)) = hibyte;

}



// Funcion 	PA_3dSpriteUpdatePalette();
void PA_3dSpriteUpdatePalette(u8 pal) {

	// Verifica si esta la paleta en VRAM
	if (!PA_TEXPALSLOT[pal].inuse) {
		PA_Error(111, "Sprite PAL", pal);
	}

	// Obten el slot donde esta la paleta en RAM
	u8 slot = PA_TEXPALSLOT[pal].ramslot;

	// Actualiza la paleta en VRAM
	u32 address = (0x06890000) + (pal << 9);			// Calcula donde guardaras la paleta
	vramSetBankF(VRAM_F_LCD);			// Bloquea el banco F para escribir las paletas
	PA_DmaMemCopy((void*)address, PA_BUFFER_SPR256PAL[slot], PA_SPR256PAL[slot].size);	// Copia la paleta al banco F
	vramSetBankF(VRAM_F_TEX_PALETTE);	// Banco F de la VRAM para paletas extendidas (Texturas)

}



// Funcion PA_3dSpriteGetPalColor();
void PA_3dSpriteGetPalColor(u8 pal, u8 number, u8* r, u8* g, u8* b) {

	// Verifica si esta la paleta en VRAM
	if (!PA_TEXPALSLOT[pal].inuse) {
		PA_Error(111, "Sprite PAL", pal);
	}

	// Obten los bytes
	u16 lobyte = *(PA_BUFFER_SPR256PAL[PA_TEXPALSLOT[pal].ramslot] + (number << 1));
	u16 hibyte = *(PA_BUFFER_SPR256PAL[PA_TEXPALSLOT[pal].ramslot] + ((number << 1) + 1));

	// Calcula el RGB (compuesto)
	u16 rgb = (hibyte << 8) + lobyte;

	// Calcula los RGB
	*r = (rgb & 0x1F);
	*g = ((rgb >> 5) & 0x1F);
	*b = ((rgb >> 10) & 0x1F);

}
// Funcion PA_3dSpriteSetDeep();
void PA_3dSpriteSetDeep(u8 id, s16 z) {

	// Asigna la profundidad
	PA_3DSPRITE[id].z = z;

	// Verifica los limites
	if (PA_3DSPRITE[id].z < -512) PA_3DSPRITE[id].z = -512;
	if (PA_3DSPRITE[id].z > 512) PA_3DSPRITE[id].z = 512;

}
