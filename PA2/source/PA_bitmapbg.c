#include <nds.h>
#include <filesystem.h>
#include <fat.h>

// Includes C
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Includes propios
#include "PA_bitmapbg.h"
#include "PA_basic.h"
#include "PA_2d.h"


// Define los Buffers para almacenar datos de 16 bits
PA_TYPE_BG16B_IPAO PA_BG16B[PA_SLOTS_BG16B];		// Fondos RAW de 16 bits
// Backbuffer de 16 bits de cada pantalla
u16* PA_16BITS_BACKBUFFER[2];
// Define los Buffers para almacenar datos de 8 bits
PA_TYPE_BG8B_IPAO PA_BG8B[PA_SLOTS_BG8B];	// Fondos indexados de 8 bits
// Backbuffer de 8 bits de cada pantalla
PA_TYPE_BB8B_IPAO PA_8BITS_BACKBUFFER[2];

// Funcion PA_Init16bitsBgBuffers();
void PA_Init16bitsBgBuffers(void) {
	// Variables locales
	u8 n = 0;
	for (n = 0; n < PA_SLOTS_BG16B; n ++) {
		PA_BG16B[n].buffer = NULL;
		PA_BG16B[n].size = 0;
		PA_BG16B[n].inuse = false;
		PA_BG16B[n].width = 0;
		PA_BG16B[n].height = 0;
	}
}

// Funcion PA_Reset16bitsBgBuffers();
void PA_Reset16bitsBgBuffers(void) {
	// Variables locales
	u8 n = 0;
	// Libera la RAM
	for (n = 0; n < PA_SLOTS_BG16B; n ++) free(PA_BG16B[n].buffer);
	// Reinicia los datos
	PA_Init16bitsBgBuffers();
}

// Funcion PA_Init16bitsBackBuffer();
void PA_Init16bitsBackBuffer(u8 screen) {
	u8 scr = screen;
	if (scr > 1) scr = 1; 
	PA_16BITS_BACKBUFFER[scr] = NULL;
}

// Funcion PA_Enable16bitsBackBuffer();
void PA_Enable16bitsBackBuffer(u8 screen) {
	u8 scr = screen;
	if (scr > 1) scr = 1; 
	// Resetea el buffer
	free(PA_16BITS_BACKBUFFER[scr]);
	PA_16BITS_BACKBUFFER[scr] = NULL;
	// Asignale 128kb de memoria
	PA_16BITS_BACKBUFFER[scr] = (u16*) calloc(65536, sizeof(u16));
	// Devuelve error si no hay suficiente memoria
	if (PA_16BITS_BACKBUFFER[scr] == NULL) PA_Error(102, NULL, 131072);
}

// Funcion PA_Disble16bitsBackBuffer();
void PA_Disble16bitsBackBuffer(u8 screen) {
	u8 scr = screen;
	if (scr > 1) scr = 1; 
	// Resetea el buffer
	free(PA_16BITS_BACKBUFFER[scr]);
	PA_16BITS_BACKBUFFER[scr] = NULL;
}



// Funcion PA_Flip16bitsBackBuffer();
void PA_Flip16bitsBackBuffer(u8 screen) {
	// Copia el contenido del Backbuffer a la VRAM
	// de la pantalla solicitada
	if (screen == 0) PA_DmaMemCopy((void*)0x06000000, PA_16BITS_BACKBUFFER[0], 131072);
	else PA_DmaMemCopy((void*)0x06200000, PA_16BITS_BACKBUFFER[1], 131072);
}

// Funcion PA_InitBitmapBgSys();
void PA_InitBitmapBgSys(u8 screen, u8 mode) {
	// Habilita la capa 3 de la pantalla indicada en modo BITMAP
	// Variables locales
	u8 n = 0;
	// Inicializa la VRAM
	if (screen == 0) {
		vramSetBankA(VRAM_A_MAIN_BG);				// Banco A de la VRAM para fondos (128kb)
		memset((void*)0x06000000, 0, 131072);		// Borra el contenido del banco A
		// Oculta todas las capas
		for (n = 0; n < 4; n ++) PA_HideBg(0, n);// Oculta todas las 4 capas
	} else {
		vramSetBankC(VRAM_C_SUB_BG);					// Banco C de la VRAM para fondos (128kb)
		memset((void*)0x06200000, 0, 131072);			// Borra el contenido del banco C
		// Oculta todas las capas
		for (n = 0; n < 4; n ++) PA_HideBg(1, n);// Oculta todas las 4 capas
	}

	// Inicializa la capa de dibujado
	if (screen == 0) {
		if (mode == 0) {	// Modo 8 bits (Capas 1 y 3)
			REG_BG3CNT = BG_PRIORITY_3 | BG_BMP_BASE(4) | BG_BMP8_256x256;
			REG_BG2CNT = BG_PRIORITY_2 | BG_BMP_BASE(0) | BG_BMP8_256x256;
		} else REG_BG3CNT = BG_PRIORITY_3 | BG_BMP_BASE(0) | BG_BMP16_256x256;	// Modo 16 bits
		
		// Resetea los registros de RotScale (Capa 3)
		REG_BG3PA = (1 << 8);
		REG_BG3PB = 0;
		REG_BG3PC = 0;
		REG_BG3PD = (1 << 8);
		PA_ScrollBg(0, 3, 0, 0);					// Posicionala en 0, 0
		PA_ShowBg(0, 3);							// Muestra la capa 3
		// Resetea los registros de RotScale (Capa 2)
		if (mode == 0) {
			REG_BG2PA = (1 << 8);
			REG_BG2PB = 0;
			REG_BG2PC = 0;
			REG_BG2PD = (1 << 8);
			PA_ScrollBg(0, 2, 0, 0);				// Posicionala en 0, 0
			PA_ShowBg(0, 2);						// Muestra la capa 2
		}
	} else {
		if (mode == 0) {	// Modo 8 bits (Capas 2 y 3)
			REG_BG3CNT_SUB = BG_PRIORITY_3 | BG_BMP_BASE(4) | BG_BMP8_256x256;
			REG_BG2CNT_SUB = BG_PRIORITY_2 | BG_BMP_BASE(0) | BG_BMP8_256x256;
		} else REG_BG3CNT_SUB = BG_PRIORITY_3 | BG_BMP_BASE(0) | BG_BMP16_256x256; // Modo 16 bits
		
		// Resetea los registros de RotScale (Capa 3)
		REG_BG3PA_SUB = (1 << 8);
		REG_BG3PB_SUB = 0;
		REG_BG3PC_SUB = 0;
		REG_BG3PD_SUB = (1 << 8);
		PA_ScrollBg(1, 3, 0, 0);					// Posicionala en 0, 0
		PA_ShowBg(1, 3);							// Muestra la capa 3
		// Resetea los registros de RotScale (Capa 2)
		if (mode == 0) {
			REG_BG2PA_SUB = (1 << 8);
			REG_BG2PB_SUB = 0;
			REG_BG2PC_SUB = 0;
			REG_BG2PD_SUB = (1 << 8);
			PA_ScrollBg(1, 2, 0, 0);				// Posicionala en 0, 0
			PA_ShowBg(1, 2);						// Muestra la capa 2
		}
	}
}

// Funcion PA_Load16bitsBg();
void PA_Load16bitsBg(const char* file, u8 slot) {
	// Llama a la funcion de carga de datos de imagen de 16bits
	PA_Load16bImgData(file, slot, 256, 256, 0);
}



// Funcion PA_Load16bitsImage();
void PA_Load16bitsImage(const char* file, u8 slot, u16 size_x, u16 size_y) {
	// Llama a la funcion de carga de datos de imagen de 16bits
	PA_Load16bImgData(file, slot, size_x, size_y, 1);
}



// Funcion PA_Load16bImgData();
void PA_Load16bImgData(const char* file, u8 slot, u16 x, u16 y, u8 type) {
	
	// Verifica el rango de Id's
	if ((slot < 0) || (slot >= PA_SLOTS_BG16B)) {
		if (type == 0) PA_Error(106, "16 Bits Bg's", PA_SLOTS_BG16B);
		 
		else PA_Error(106, "16 Bits Image", PA_SLOTS_BG16B);
	}

	// Vacia los buffers que se usaran
	free(PA_BG16B[slot].buffer);
	PA_BG16B[slot].buffer = NULL;

	// Declara los punteros a los ficheros
	FILE* file_id;

	// Variable para almacenar el path al archivo
	char filename[256];

	// Variable para el tamaño de archivo
	u32 size = 0;

	// Carga el archivo .IMG
	sprintf(filename, "%s/%s.img", PA_ROOTFOLDER, file);
	file_id = fopen(filename, "rb");
	if (file_id) {	// Si el archivo existe...
		// Obten el tamaño del archivo
		fseek(file_id, 0, SEEK_END);
		size = ftell(file_id);
		rewind(file_id);
		// Si excede del tamaño maximo (128kb), error
		if (size > 131072) PA_Error(116, filename, 131072);
		// Reserva el espacio en RAM
		PA_BG16B[slot].buffer = (u16*) calloc ((size >> 1), sizeof(u16));
		if (PA_BG16B[slot].buffer == NULL) {		// Si no hay suficiente RAM libre
			PA_Error(102, NULL, size);
		}
		// Lee el archivo y ponlo en la RAM
		fread(PA_BG16B[slot].buffer, 1, size, file_id);
	} else PA_Error(101, filename, 0);	// Si el archivo no existe...
	
	fclose(file_id);		// Cierra el archivo

	// Asegurate que el alpha bit (BIT 15) esta marcado
	u32 n = 0;
	for (n = 0; n < (size >> 1); n ++) PA_BG16B[slot].buffer[n] |= BIT(15);
	
	// Guarda los parametros del fondo
	PA_BG16B[slot].size = size;		// Guarda el tamaño
	PA_BG16B[slot].width = x;		// Ancho del fondo
	PA_BG16B[slot].height = y;		// Altura del fondo
	PA_BG16B[slot].inuse = true;	// Marca que esta en uso

}

// Funcion PA_Unload16bitsBg();
void PA_Unload16bitsBg(u8 slot) {
	// Verifica si el buffer contiene datos
	if (!PA_BG16B[slot].inuse) PA_Error(110, "16 Bits Bg", slot);

	// Vacia los buffers que se usaran
	free(PA_BG16B[slot].buffer);
	PA_BG16B[slot].buffer = NULL;

	PA_BG16B[slot].size = 0;		// Tamaño a 0
	PA_BG16B[slot].inuse = false;	// Marca que esta libre
}

// Funcion PA_Copy16bitsBuffer();
void PA_Copy16bitsBuffer(u8 screen, u8 destination, u8 slot) {
	// Verifica si el buffer contiene datos
	if (!PA_BG16B[slot].inuse) PA_Error(110, "16 Bits Bg", slot);

	if (destination == 0) {		// Si el destino es la VRAM
		// Dependiendo de la pantalla
		if (screen == 0) PA_DmaMemCopy((void*)0x06000000, PA_BG16B[slot].buffer, PA_BG16B[slot].size);
		else PA_DmaMemCopy((void*)0x06200000, PA_BG16B[slot].buffer, PA_BG16B[slot].size);
	} else {					// Si el destino es el BackBuffer
		// Dependiendo de la pantalla
		if (screen == 0) memcpy(PA_16BITS_BACKBUFFER[0], PA_BG16B[slot].buffer, PA_BG16B[slot].size);
		 else memcpy(PA_16BITS_BACKBUFFER[1], PA_BG16B[slot].buffer, PA_BG16B[slot].size);
	}
}

// Funcion PA_Draw16bitsImage();
void PA_Draw16bitsImage(u8 screen, u8 slot, s16 x, s16 y, bool alpha) {
	// Verifica si el buffer contiene datos
	if (!PA_BG16B[slot].inuse) PA_Error(110, "16 Bits Image", slot);

	// Variables locales
	u16 img_x = 0;
	u16 img_y = 0;
	s16 buff_x = 0;
	s16 buff_y = 0;
	u32 buff_idx = 0;
	u16 data = 0;

	// Filtro de pantalla
	u8 scr = screen;
	if (scr > 1) scr = 1;

	// Si el destino es el BackBuffer
	for (img_y = 0; img_y < PA_BG16B[slot].height; img_y ++) {
		for (img_x = 0; img_x < PA_BG16B[slot].width; img_x ++ ) {
			// Calcula donde se escribira el pixel
			buff_x = (img_x + x);
			buff_y = (img_y + y);
			// Si esta dentro de la pantalla, dibujalo
			if (
				(buff_x >= 0)
				&&
				(buff_x <= 255)
				&&
				(buff_y >= 0)
				&&
				(buff_y <= 255)
				) {
				// Calcula el offset dentro del buffer
				buff_idx = ((buff_y << 8) + buff_x);
				// Valor del Pixel
				data = PA_BG16B[slot].buffer[((img_y * PA_BG16B[slot].width) + img_x)]; 
				// Si el pixel NO es magenta !(RGB15(31, 0, 31) | BIT(15))
				if ((data != 0xFC1F) || (!alpha)) {*(PA_16BITS_BACKBUFFER[scr] + buff_idx) = data; // Escribe el pixel en el BackBuffer
				}
			}
		}
	}

}

// Funcion PA_Init8bitsBgBuffers();
void PA_Init8bitsBgBuffers(void) {
	// Variables locales
	u8 n = 0;
	for (n = 0; n < PA_SLOTS_BG8B; n ++) {
		PA_BG8B[n].data = NULL;
		PA_BG8B[n].pal = NULL;
		PA_BG8B[n].data_size = 0;
		PA_BG8B[n].pal_size = 0;
		PA_BG8B[n].inuse = false;
	}
}

// Funcion PA_Reset8bitsBgBuffers();
void PA_Reset8bitsBgBuffers(void) {
	// Variables locales
	u8 n = 0;
	// Libera la RAM usada
	for (n = 0; n < PA_SLOTS_BG8B; n ++) {
		free(PA_BG8B[n].data);
		free(PA_BG8B[n].pal);
	}
	// Reinicia los datos
	PA_Init8bitsBgBuffers();
}
// Funcion PA_Load8bitsBg();
void PA_Load8bitsBg(const char* file, u8 slot) {
	
	// Verifica el rango de Id's
	if ((slot < 0) || (slot >= PA_SLOTS_BG8B)) PA_Error(106, "8 Bits Bg's", PA_SLOTS_BG8B);

	// Vacia los buffers que se usaran
	free(PA_BG8B[slot].data);
	PA_BG8B[slot].data = NULL;
	free(PA_BG8B[slot].pal);
	PA_BG8B[slot].pal = NULL;

	// Declara los punteros a los ficheros
	FILE* file_id;

	// Variable para almacenar el path al archivo
	char filename[256];

	// Variable para el tamaño de archivo
	u32 size = 0;

	// Carga el archivo .IMG
	sprintf(filename, "%s/%s.img", PA_ROOTFOLDER, file);
	file_id = fopen(filename, "rb");
	if (file_id) {	// Si el archivo existe...
		// Obten el tamaño del archivo
		fseek(file_id, 0, SEEK_END);
		size = ftell(file_id);
		rewind(file_id);
		// Si excede del tamaño maximo (64kb), error
		if (size > 65536) PA_Error(116, filename, 65536);
		// Reserva el espacio en RAM
		PA_BG8B[slot].data = (u8*) calloc (size, sizeof(u8));
		if (PA_BG8B[slot].data == NULL) PA_Error(102, NULL, size); // Si no hay suficiente RAM libre
		
		// Lee el archivo y ponlo en la RAM
		fread(PA_BG8B[slot].data, 1, size, file_id);
	}
	else PA_Error(101, filename, 0); // Si el archivo no existe...
	
	fclose(file_id);		// Cierra el archivo
	PA_BG8B[slot].data_size = size;		// Guarda el tamaño del buffer

	// Carga el archivo .PAL
	sprintf(filename, "%s/%s.pal", PA_ROOTFOLDER, file);
	file_id = fopen(filename, "rb");
	if (file_id) {	// Si el archivo existe...
		// Obten el tamaño del archivo
		fseek(file_id, 0, SEEK_END);
		size = ftell(file_id);
		rewind(file_id);
		// Si la paleta tiene un tamaño iPAerior a 512, ajusta el tamaño
		if (size < 512) size = 512;
		// Reserva el espacio en RAM
		PA_BG8B[slot].pal = (u16*) calloc ((size >> 1), sizeof(u16));
		if (PA_BG8B[slot].pal == NULL) {PA_Error(102, NULL, size);// Si no hay suficiente RAM libre
		
		// Lee el archivo y ponlo en la RAM
		fread(PA_BG8B[slot].pal, 1, size, file_id);
	} else PA_Error(101, filename, 0);// Si el archivo no existe...
	
	fclose(file_id);		// Cierra el archivo
	PA_BG8B[slot].pal_size = size;		// Guarda el tamaño del buffer

	// Marca el slot como que esta en uso
	PA_BG8B[slot].inuse = true;
}

// Funcion PA_Unload8bitsBg();
void PA_Unload8bitsBg(u8 slot) {
	// Verifica si el buffer contiene datos
	if (!PA_BG8B[slot].inuse) PA_Error(110, "8 Bits Bg", slot);

	// Vacia los buffers que se usaran
	free(PA_BG8B[slot].data);
	PA_BG8B[slot].data = NULL;
	PA_BG8B[slot].data_size = 0;
	free(PA_BG8B[slot].pal);
	PA_BG8B[slot].pal = NULL;
	PA_BG8B[slot].pal_size = 0;

	PA_BG8B[slot].inuse = false;	// Marca que esta libre
}

// Funcion PA_Copy8bitsBuffer();
void PA_Copy8bitsBuffer(u8 screen, u8 destination, u8 slot) {
	// Verifica si el buffer contiene datos
	if (!PA_BG8B[slot].inuse) PA_Error(110, "8 Bits Bg", slot);

	u8 scr = screen;
	if (scr > 1) scr = 1;

	// Si el destino es la VRAM
	if (destination < 2) {
		// Segun la pantalla...
		u32 data = 0;
		u32 pal = 0;
		if (screen == 0) {
			data = (0x06000000);	// Direccion en VRAM para los datos
			pal = (0x05000000);		// Direccion en VRAM para la paleta
		} else {
			data = (0x06200000);	// Direccion en VRAM para los datos
			pal = (0x05000400);		// Direccion en VRAM para la paleta
		}
		// Segun la capa
		if (destination == 1) data += 65536;
		// Copia los datos a la VRAM
		PA_DmaMemCopy((void*)data, PA_BG8B[slot].data, PA_BG8B[slot].data_size);
		PA_DmaMemCopy((void*)pal, PA_BG8B[slot].pal, PA_BG8B[slot].pal_size);

	} else {
		// Copia los datos al BackBuffer
		memcpy(PA_8BITS_BACKBUFFER[scr].data, PA_BG8B[slot].data, PA_BG8B[slot].data_size);
		memcpy(PA_8BITS_BACKBUFFER[scr].pal, PA_BG8B[slot].pal, PA_BG8B[slot].pal_size);
	}
}

// Funcion PA_Init8bitsBackBuffer();
void PA_Init8bitsBackBuffer(u8 screen) {
	u8 scr = screen;
	if (scr > 1) scr = 1; 
	PA_8BITS_BACKBUFFER[scr].data = NULL;
	PA_8BITS_BACKBUFFER[scr].pal = NULL;
}

// Funcion PA_Enable8bitsBackBuffer();
void PA_Enable8bitsBackBuffer(u8 screen) {
	u8 scr = screen;
	if (scr > 1) scr = 1; 
	// Resetea el buffer
	free(PA_8BITS_BACKBUFFER[scr].data);
	free(PA_8BITS_BACKBUFFER[scr].pal);
	PA_8BITS_BACKBUFFER[scr].data = NULL;
	PA_8BITS_BACKBUFFER[scr].pal = NULL;
	// Asignale 64kb de memoria para datos
	PA_8BITS_BACKBUFFER[scr].data = (u8*) calloc(65536, sizeof(u8));
	if (PA_8BITS_BACKBUFFER[scr].data == NULL) PA_Error(102, NULL, 65536);
	// Asignale 512 bytes de memoria para paletas
	PA_8BITS_BACKBUFFER[scr].pal = (u16*) calloc(256, sizeof(u16));
	if (PA_8BITS_BACKBUFFER[scr].pal == NULL) PA_Error(102, NULL, 512);
}

// Funcion PA_Disble8bitsBackBuffer();
void PA_Disble8bitsBackBuffer(u8 screen) {
	u8 scr = screen;
	if (scr > 1) scr = 1; 
	// Resetea el buffer
	free(PA_8BITS_BACKBUFFER[scr].data);
	free(PA_8BITS_BACKBUFFER[scr].pal);
	PA_8BITS_BACKBUFFER[scr].data = NULL;
	PA_8BITS_BACKBUFFER[scr].pal = NULL;
}

// Funcion PA_Flip8bitsBackBuffer();
void PA_Flip8bitsBackBuffer(u8 screen, u8 destination) {
	// Copia el contenido del Backbuffer a la VRAM
	// de la pantalla solicitada
	u8 scr = screen;
	if (scr > 1) scr = 1; 

	// Segun la pantalla...
	u32 data = 0;
	u32 pal = 0;
	if (scr == 0) {
		data = (0x06000000);			// Direccion en VRAM para los datos
		pal = (0x05000000);				// Direccion en VRAM para la paleta
	} else {
		data = (0x06200000);			// Direccion en VRAM para los datos
		pal = (0x05000400);				// Direccion en VRAM para la paleta
	}

	// Segun la capa
	if (destination == 1) data += 65536;
	// Copia los datos a la VRAM
	PA_DmaMemCopy((void*)data, PA_8BITS_BACKBUFFER[scr].data, 65536);
	PA_DmaMemCopy((void*)pal, PA_8BITS_BACKBUFFER[scr].pal, 512);
}
