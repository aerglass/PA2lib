
// NightFox LIB - Funciones de Textos de 16 pixeles
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
#include "PA_2d.h"
#include "PA_tiledbg.h"
#include "PA_text.h"
#include "PA_text16.h"





// Funcion PA_LoadTextFont16();
void PA_LoadFont16(const char* file, const char* name, u16 width, u16 height, u8 rotation) {

	// Variable temporal del tamaño de la paleta
	u32 pal_size = 0;

	// Busca un slot libre
	u16 n = 0;
	u8 slot = 255;
	for (n = 0; n < PA_SLOTS_TBG; n ++) {		// Busca en todos los slots
		if (PA_TILEDBG[n].available) {			// Si esta libre
			PA_TILEDBG[n].available = false;	// Marcalo como en uso
			slot = n;							// Guarda el slot a usar
			n = PA_SLOTS_TBG;					// Deja de buscar
		}
	}
	// Si no hay ningun slot libre, error
	if (slot == 255) {
		PA_Error(103, "Tiled Bg", PA_SLOTS_TBG);
	}

	// Verifica que el fondo sea multiplo de 256px (32 tiles)
	if (((width % 256) != 0) || ((height % 256) != 0)) {
		PA_Error(115, file, 0);
	}

	// Vacia los buffers que se usaran
	free(PA_BUFFER_BGMAP[slot]);		// Buffer para los mapas
	PA_BUFFER_BGMAP[slot] = NULL;
	free(PA_BUFFER_BGTILES[slot]);		// Buffer para los tiles
	PA_BUFFER_BGTILES[slot] = NULL;
	free(PA_BUFFER_BGPAL[slot]);		// Buffer para los paletas
	PA_BUFFER_BGPAL[slot] = NULL;

	// Declara los punteros a los ficheros
	FILE* file_id;

	// Variable para almacenar el path al archivo
	char filename[256];

	// Carga el archivo .fnt
	sprintf(filename, "%s/%s.fnt", PA_ROOTFOLDER, file);
	file_id = fopen(filename, "rb");
	if (file_id) {	// Si el archivo existe...
		// Obten el tamaño del archivo
		PA_TILEDBG[slot].tilesize = (PA_TEXT_FONT_CHARS_16 << 7);	// 1 letra 128 bytes (letras * 128)
		// Reserva el espacio en RAM
		PA_BUFFER_BGTILES[slot] = (char*) calloc (PA_TILEDBG[slot].tilesize, sizeof(char));
		if (PA_BUFFER_BGTILES[slot] == NULL) {		// Si no hay suficiente RAM libre
			PA_Error(102, NULL, PA_TILEDBG[slot].tilesize);
		}
		// Lee el archivo y ponlo en la RAM
		fread(PA_BUFFER_BGTILES[slot], 1, PA_TILEDBG[slot].tilesize, file_id);
	} else {	// Si el archivo no existe...
		PA_Error(101, filename, 0);
	}
	fclose(file_id);		// Cierra el archivo

	// Rota los Gfx de los tiles si es necesario
	if (rotation > 0) {
		for (n = 0; n < (PA_TEXT_FONT_CHARS_16 << 1); n ++) {
			PA_RotateTileGfx(slot, n, rotation);
		}
	}

	// Crea un archivo .MAP vacio en RAM 
	// ((ancho / 8) * (alto / 8)) * 2
	PA_TILEDBG[slot].mapsize = (((width >> 3) * (height >> 3)) << 1);
	// Reserva el espacio en RAM
	PA_BUFFER_BGMAP[slot] = (char*) calloc (PA_TILEDBG[slot].mapsize, sizeof(char));
	if (PA_BUFFER_BGMAP[slot] == NULL) {		// Si no hay suficiente RAM libre
		PA_Error(102, NULL, PA_TILEDBG[slot].mapsize);
	}
	// Y ponlo a 0
	memset(PA_BUFFER_BGMAP[slot], 0, PA_TILEDBG[slot].mapsize);

	// Carga el archivo .PAL
	sprintf(filename, "%s/%s.pal", PA_ROOTFOLDER, file);
	file_id = fopen(filename, "rb");
	if (file_id) {	// Si el archivo existe...
		// Obten el tamaño del archivo
		fseek(file_id, 0, SEEK_END);
		pal_size = ftell(file_id);
		PA_TILEDBG[slot].palsize = pal_size;
		rewind(file_id);
		// Si el tamaño es iPAerior a 512 bytes, ajustalo
		if (PA_TILEDBG[slot].palsize < 512) PA_TILEDBG[slot].palsize = 512;
		// Reserva el espacio en RAM
		PA_BUFFER_BGPAL[slot] = (char*) calloc (PA_TILEDBG[slot].palsize, sizeof(char));
		if (PA_BUFFER_BGPAL[slot] == NULL) {		// Si no hay suficiente RAM libre
			PA_Error(102, NULL, PA_TILEDBG[slot].palsize);
		}
		// Lee el archivo y ponlo en la RAM
		fread(PA_BUFFER_BGPAL[slot], 1, pal_size, file_id);
	} else {	// Si el archivo no existe...
		PA_Error(101, filename, 0);
	}
	fclose(file_id);		// Cierra el archivo

	// Guarda el nombre del Fondo
	sprintf(PA_TILEDBG[slot].name, "%s", name);

	// Y las medidas
	PA_TILEDBG[slot].width = width;
	PA_TILEDBG[slot].height = height;

}





// Funcion PA_CreateTextLayer16();
void PA_CreateTextLayer16(u8 screen, u8 layer, u8 rotation, const char* name) {

	u8 n = 0;			// Bucle
	u8 slot = 255;		// Slot seleccionado
	char bg[32];		// Nombre

	// Crea un  fondo para usarlo como capa de texto
	PA_CreateTiledBg(screen, layer, name);

	// Busca el numero de slot donde esta cargada la fuente
	sprintf(bg, "%s", name);							// Obten el nombre del fondo a buscar
	for (n = 0; n < PA_SLOTS_TBG; n ++) {				// Busca en todos los slots
		if (strcmp(bg, PA_TILEDBG[n].name) == 0) {		// Si lo encuentras
			slot = n;									// Guarda el slot a usar
			n = PA_SLOTS_TBG;							// Deja de buscar
		}
	}

	// Guarda si el texto debe ser rotado
	PA_TEXT[screen][layer].rotation = rotation;

	// Guarda las medidas del fondo en tiles (ultimo numero de tile)
	switch (rotation) {
		case 0: // Sin rotacion
			PA_TEXT[screen][layer].width = ((PA_TILEDBG[slot].width >> 3) - 1);
			PA_TEXT[screen][layer].height = ((PA_TILEDBG[slot].height >> 4) - 1);
			break;
		case 1:	// 90º derecha
			PA_TEXT[screen][layer].width = ((PA_TILEDBG[slot].width >> 4) - 1);
			PA_TEXT[screen][layer].height = ((PA_TILEDBG[slot].height >> 3) - 1);
			break;
		case 2:	// 90º izquierda
			PA_TEXT[screen][layer].width = ((PA_TILEDBG[slot].width >> 4) - 1);
			PA_TEXT[screen][layer].height = ((PA_TILEDBG[slot].height >> 3) - 1);
			break;
	}

	// Almacena el slot donde esta cargada la fuente
	PA_TEXT[screen][layer].slot = slot;

	// Y marcalo como creado
	PA_TEXT[screen][layer].exist = true;

}





// Funcion PA_WriteText16();
void PA_Print16(u8 screen, u8 layer, u16 x, u16 y, const char* text) {

	// Verifica si la capa de texto de destino existe
	if (!PA_TEXT[screen][layer].exist) {
		PA_Error(114, NULL, screen);
	}

	u16 n = 0;						// Variable de uso general

	s16 value = 0;					// Valor

	u16 tsize = strlen(text);		// Calcula el numero de letras del texto

	u8* string;					// Buffer temporal
	string = NULL;
	string = (u8*) calloc (tsize, sizeof(u8));

	// Almacena en el buffer temporal el valor de los caracteres
	for (n = 0; n < tsize; n ++) {
		value = ((int)(text[n])) - 32;	// Resta 32 al valor entrado
		if (value < 0) value = 0;
		string[n] = value;
		// Si es un caracter especial...
		if (string[n] > 95) {
			// Resetea el caracter...
			string[n] = 0;
			// Verifica caracteres especiales
			switch (((int)(text[n]))) {
				// Salto de linea
				case 10:	// \n
					string[n] = 200;
					break;
				// Caracteres especiales
				case 199:	// Ç
					string[n] = 96;
					break;
				case 231:	// ç
					string[n] = 97;
					break;
				case 209:	// Ñ
					string[n] = 98;
					break;
				case 241:	// ñ
					string[n] = 99;
					break;
				// Acentos Mayusculas
				case 193:	// Á
					string[n] = 100;
					break;
				case 201:	// É
					string[n] = 101;
					break;
				case 205:	// Í
					string[n] = 102;
					break;
				case 211:	// Ó
					string[n] = 103;
					break;
				case 218:	// Ú
					string[n] = 104;
					break;
				// Acentos Minusculas
				case 225:	// á
					string[n] = 105;
					break;
				case 233:	// é
					string[n] = 106;
					break;
				case 237:	// í
					string[n] = 107;
					break;
				case 243:	// ó
					string[n] = 108;
					break;
				case 250:	// ú
					string[n] = 109;
					break;
				// Dieresis
				case 239:	// ï
					string[n] = 110;
					break;
				case 252:	// ü
					string[n] = 111;
					break;
				// Admiracion e interrogante (Español)
				case 161:	// ¡
					string[n] = 112;
					break;
				case 191:	// ¿
					string[n] = 113;
					break;
				// Caracter invalido
				default:	
					string[n] = 0;
					break;
			}
		}
	}

	// Variable para calcular la posicion del texto
	s16 pos_x = 0;	// Posicion X real en tiles
	s16 pos_y = 0;	// Posicion Y real en tiles
	s16 tx = 0;		// Posicion X del texto
	s16 ty = 0;		// Posicion Y del texto

	switch (PA_TEXT[screen][layer].rotation) {

		case 0:		// Sin rotacion
			// Traspasa las coordenadas
			tx = x;
			ty = y;
			// Copia el texto al buffer letra a letra
			for (n = 0; n < tsize; n ++) {
				// Si es un caracter valido
				if (string[n] <= PA_TEXT_FONT_LAST_VALID_CHAR_16) {
					// Calcula la posicion en el tilemap
					pos_x = tx;
					pos_y = (ty << 1);
					// Escribe la letra correspondiente
					value = ((((int)(string[n] >> 5)) << 5) + string[n]);
					PA_SetTileOfMap(screen,layer, pos_x, pos_y, ((PA_TEXT[screen][layer].pal << 12) + value));
					PA_SetTileOfMap(screen,layer, pos_x, (pos_y + 1), ((PA_TEXT[screen][layer].pal << 12) + (value + 32)));
					// Siguiente letra
					tx ++;
				}
				if ((tx > PA_TEXT[screen][layer].width) || (string[n] == 200)) {		// Si llegas al final de linea,
					tx = 0;			// salto de linea
					ty ++;
					if (ty > PA_TEXT[screen][layer].height) {	// Si estas en la ultima linea,
						ty = 0;		// vuelve a la primera
					}
				}
			}
			break;

		case 1:		// 90º derecha
			// Traspasa las coordenadas
			tx = (PA_TEXT[screen][layer].width - y);
			ty = x;
			// Copia el texto al buffer letra a letra
			for (n = 0; n < tsize; n ++) {
				// Si es un caracter valido
				if (string[n] <= PA_TEXT_FONT_LAST_VALID_CHAR_16) {
					// Calcula la posicion en el tilemap
					pos_x = (tx << 1);
					pos_y = ty;
					// Escribe la letra correspondiente
					value = ((((int)(string[n] >> 5)) << 5) + string[n]);
					PA_SetTileOfMap(screen,layer, pos_x, pos_y, ((PA_TEXT[screen][layer].pal << 12) + value));
					PA_SetTileOfMap(screen,layer, (pos_x - 1), pos_y, ((PA_TEXT[screen][layer].pal << 12) + (value + 32)));
					// Siguiente letra
					ty ++;
				}
				if ((ty > PA_TEXT[screen][layer].height) || (string[n] == 200)) {		// Si llegas al final de linea,
					ty = 0;			// salto de linea
					tx --;
					if (tx < 0) {	// Si estas en la ultima linea,
						tx = PA_TEXT[screen][layer].width;	// vuelve a la primera
					}
				}
			}
			break;

		case 2:		// 90º izquierda
			// Traspasa las coordenadas
			tx = y;
			ty = (PA_TEXT[screen][layer].height - x);
			// Copia el texto al buffer letra a letra
			for (n = 0; n < tsize; n ++) {
				// Si es un caracter valido
				if (string[n] <= PA_TEXT_FONT_LAST_VALID_CHAR_16) {
					// Calcula la posicion en el tilemap
					pos_x = (tx << 1);
					pos_y = ty;
					// Escribe la letra correspondiente
					value = ((((int)(string[n] >> 5)) << 5) + string[n]);
					PA_SetTileOfMap(screen,layer, pos_x, pos_y, ((PA_TEXT[screen][layer].pal << 12) + value));
					PA_SetTileOfMap(screen,layer, (pos_x + 1), pos_y, ((PA_TEXT[screen][layer].pal << 12) + (value + 32)));
					// Siguiente letra
					ty --;
				}
				if ((ty < 0) || (string[n] == 200)) {		// Si llegas al final de linea,
					ty = PA_TEXT[screen][layer].height;		// Salto de linea
					tx ++;
					if (tx > PA_TEXT[screen][layer].width) {	// Si llegas a la ultima linea
						tx = 0;		// vuelve a la primera
					}
				}
			}
			break;

		default:
			break;

	}

	// Marca esta capa de texto para actualizar
	PA_TEXT[screen][layer].update = true;

	// Libera el buffer
	free(string);

}





// Funcion PA_ClearTextLayer16();
void PA_ClearTextLayer16(u8 screen, u8 layer) {

	// Verifica si la capa de texto de destino existe
	if (!PA_TEXT[screen][layer].exist) {
		PA_Error(114, NULL, screen);
	}

	// Calcula el tamaño del buffer (segun la rotacion)
	u32 size = 0;
	switch (PA_TEXT[screen][layer].rotation) {
		case 0:	// Sin rotacion
			size = (((PA_TEXT[screen][layer].width + 1) * ((PA_TEXT[screen][layer].height + 1) << 1)) << 1);
			break;
		case 1:	// 90º a la derecha
			size = ((((PA_TEXT[screen][layer].width + 1) << 1) * (PA_TEXT[screen][layer].height + 1)) << 1);
			break;
		case 2:	// 90º a la izquierda
			size = ((((PA_TEXT[screen][layer].width + 1) << 1) * (PA_TEXT[screen][layer].height + 1)) << 1);
			break;
	}

	// Pon a 0 todos los bytes del mapa de la capa de texto
	memset(PA_BUFFER_BGMAP[PA_TEXT[screen][layer].slot], 0, size);

	// Marca esta capa de texto para actualizar
	PA_TEXT[screen][layer].update = true;

}
