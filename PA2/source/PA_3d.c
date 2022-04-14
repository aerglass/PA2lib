//PA2Lib code
//thanks to knightfox75
//because its useful code


// Includes devKitPro
#include <nds.h>

// Includes C
#include <stdio.h>
#include <string.h>

// Includes propios
#include "PA_basic.h"
#include "PA_3d.h"
#include "PA_2d.h"





// Funcion PA_Set3D();
void PA_Init3D(u8 screen, u8 mode) {

	// Especifica en que pantalla estara el main engine (unico que puede usar 3D)
	if (screen == 0) {
		lcdMainOnTop();
	} else {
		lcdMainOnBottom();
	}

	// Selecciona modo 3D
	switch (mode) {
		case 0:		
			videoSetMode(MODE_0_3D);
			break;
		case 2:		
			videoSetMode(MODE_2_3D);
			break;
		case 5:
			videoSetMode(MODE_5_3D);
			break;
	}

}



// Funcion PA_InitOpenGL();
void PA_InitOpenGL(void) {

	// Inicializa el OpenGL de Libnds
	glInit();

	// Define el tamaño de la ventana 3D (toda la pantalla)
	glViewport(0, 0, 255, 191);

	// CoPAigura la matriz de proyeccion
	glMatrixMode(GL_PROJECTION);	// Selecciona la matriz
	glLoadIdentity();				// Y reseteala

	// Ajusta OpenGL para proyeccion Ortografica
	glOrthof32(0, 256, 192, 0, 1024, -1024);

	// CoPAigura la matriz de visualizacion de modelos
	glMatrixMode(GL_MODELVIEW);		// Selecciona la matriz
	glLoadIdentity();				// Y reseteala

	// Por defecto, todos los poligonos son opacos
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);

	// CoPAigura el fondo
	glClearColor(0, 0, 0, 0);		// Fondo transparente
	glClearDepth(0x7FFF);			// Define la distancia de vision

	// CoPAigura la iluminacion global
	glColor(RGB15(31, 31, 31));

	// Habilita las texturas
	glEnable(GL_TEXTURE_2D | GL_BLEND);

	// Habilita la capa de dibujado
	PA_ShowBg(0, 0);

}



// Funcion PA_GetTextureSize();
u16 PA_GetTextureSize(u16 textel) {

	// Variables
	u16 size = 0;

	// Devuelve el tamaño del textel, segun su base2
	switch (textel) {
		case 8:
			size = 0;
			break;
		case 16:
			size = 1;
			break;
		case 32:
			size = 2;
			break;
		case 64:
			size = 3;
			break;
		case 128:
			size = 4;
			break;
		case 256:
			size = 5;
			break;
		case 512:
			size = 6;
			break;
		case 1024:
			size = 7;
			break;
		default:
			size = 255;
			break;
	}

	// Devuelve el valor
	return size;

}
