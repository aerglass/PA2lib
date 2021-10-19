
// NightFox LIB - Funciones de de funciones de sonido
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
#include "PA_sound.h"





// Define los Buffers para almacenar los archivos de audio
char* PA_BUFFER_RAWSOUND[PA_SLOTS_RAWSOUND];

// Datos de los sonidos cargado
PA_TYPE_RAWSOUND_IPAO PA_RAWSOUND[PA_SLOTS_RAWSOUND];





// Funcion PA_InitRawSoundBuffers();
void PA_InitSoundBuffers(void) {

	u8 n = 0;	// Variable comun

	// Inicializa Buffers de sonido en RAW
	for (n = 0; n < PA_SLOTS_RAWSOUND; n ++) {
		PA_BUFFER_RAWSOUND[n] = NULL;			// Inicializa puntero
		PA_RAWSOUND[n].available = true;		// Disponibilidad del slot
		PA_RAWSOUND[n].size = 0;				// Tamaño del archivo
		PA_RAWSOUND[n].freq = 0;				// Frecuencia del sample
		PA_RAWSOUND[n].format = 0;				// Formato del sample
	}

}





// Funcion PA_ResetRawSoundBuffers();
void PA_ResetSoundBuffers(void) {

	u8 n = 0;	// Variable comun

	// Borra los datos de los buffers de sonido en RAW
	for (n = 0; n < PA_SLOTS_RAWSOUND; n ++) {
		free(PA_BUFFER_RAWSOUND[n]);			// Borra el contenido puntero
	}

	// Reinicia las estructuras de datos
	PA_InitSoundBuffers();

}





// Funcion PA_LoadRawSound();
void PA_LoadSound(const char* file, u16 id,  u16 freq, u8 format) {

	// Verifica el rango de Id's
	if ((id < 0) || (id >= PA_SLOTS_RAWSOUND)) {
		PA_Error(106, "Raw Sound", PA_SLOTS_RAWSOUND);
	}

	// Verifica si la Id esta libre
	if (!PA_RAWSOUND[id].available) {
		PA_Error(109, "Raw Sound", id);
	}

	// Vacia los buffers que se usaran
	free(PA_BUFFER_RAWSOUND[id]);
	PA_BUFFER_RAWSOUND[id] = NULL;

	// Declara los punteros a los ficheros
	FILE* file_id;

	// Variable para almacenar el path al archivo
	char filename[256];

	// Carga el archivo .RAW
	sprintf(filename, "%s/%s.raw", PA_ROOTFOLDER, file);
	file_id = fopen(filename, "rb");
	if (file_id) {	// Si el archivo existe...
		// Obten el tamaño del archivo
		fseek(file_id, 0, SEEK_END);
		PA_RAWSOUND[id].size = ftell(file_id);
		rewind(file_id);
		// Si excede del tamaño maximo, error
		if (PA_RAWSOUND[id].size > (1 << 18)) PA_Error(116, filename, (1 << 18));
		// Reserva el espacio en RAM
		PA_BUFFER_RAWSOUND[id] = (char*) calloc (PA_RAWSOUND[id].size, sizeof(char));
		if (PA_BUFFER_RAWSOUND[id] == NULL) {		// Si no hay suficiente RAM libre
			PA_Error(102, NULL, PA_RAWSOUND[id].size);
		}
		// Lee el archivo y ponlo en la RAM
		fread(PA_BUFFER_RAWSOUND[id], 1, PA_RAWSOUND[id].size, file_id);
	} else {	// Si el archivo no existe...
		PA_Error(101, filename, 0);
	}
	fclose(file_id);		// Cierra el archivo
	// swiWaitForVBlank();		// Espera al cierre del archivo (Usar en caso de corrupcion de datos)

	// Guarda los parametros del archivo de sonido
	PA_RAWSOUND[id].freq = freq;		// Frequencia del sample (en Hz)
	PA_RAWSOUND[id].format = format;	// Formato del sample (0 - > 8 bits, 1 - > 16 bits, 2 -> ADPCM)

	// Y marca esta ID como usada
	PA_RAWSOUND[id].available = false;

}





// Funcion UnloadRawSound();
void PA_UnloadSound(u8 id) {

	// Verifica el rango de Id's
	if ((id < 0) || (id >= PA_SLOTS_RAWSOUND)) PA_Error(106, "RAW Sound", PA_SLOTS_RAWSOUND); 

	// Verifica si el sonido existe
	if (PA_RAWSOUND[id].available) PA_Error(110, "RAW Sound", id);

	// Vacia los buffers de la Id. seleccionada
	free(PA_BUFFER_RAWSOUND[id]);
	PA_BUFFER_RAWSOUND[id] = NULL;

	// Resetea las variables
	PA_RAWSOUND[id].freq = 0;		// Frequencia del sample (en Hz)
	PA_RAWSOUND[id].format = 0;	// Formato del sample (0 - > 8 bits, 1 - > 16 bits, 2 -> ADPCM)

	// Y marca esta ID como libre
	PA_RAWSOUND[id].available = true;

}





// Funcion PA_PlayRawSound();
u8 PA_PlaySound(u8 id, u8 volume, u8 pan, bool loop, u16 loopfrom) {

	// Verifica el rango de Id's
	if ((id < 0) || (id >= PA_SLOTS_RAWSOUND)) PA_Error(106, "RAW Sound", PA_SLOTS_RAWSOUND); 

	// Verifica si el sonido existe
	if (PA_RAWSOUND[id].available) PA_Error(110, "RAW Sound", id);

	return soundPlaySample(PA_BUFFER_RAWSOUND[id], PA_RAWSOUND[id].format, PA_RAWSOUND[id].size, PA_RAWSOUND[id].freq, volume, pan, loop, loopfrom);

}
