#include <stdio.h>	// Include C
#include <nds.h>	//include Libnds
#include <PA_lib.h> //include PA2lib

//main block of the program.
int main(int argc, char **argv) {
	consoleDemoInit();		// Inicializa la consola de texto
	consoleClear();			// Borra la pantalla
	setBrightness(3, 0);	// Restaura el brillo
	
	iprintf("\n Hello World!"); //print hello world
	//loop for ever.
	while(1) {
		scanKeys();
		int Held = keysHeld();
		if(Held & KEY_START) return 0; //if the start button is held, turn off the console.
		SPA_Flush();
	}
	return 0;
}