/*
PA2lib Template.
This is ready to use, no need for installation or anything else.
Enjoy making games
*/
#include <stdio.h>				// Include C
#include <nds.h>				//include Libnds
#include <PA_lib.h>			 	//include PA2lib

//main block of the program.
int main(int argc, char **argv) {
	SPA_Init();				//inits everything
	SPA_LoadBackground("PA2lib", "bg", 256, 256, 0, 3); //load and display background
	//loop for ever.
	while(1) {
		scanKeys();			//scan keys
		int Held = keysHeld();		//keys variable
		if(Held & KEY_START) return 0; 	//if the start button is held, turn off the console.
		SPA_Flush();			//update everything (oam, text, swiwaitforvblank)
	}
	return 0; 				//turn off
}
