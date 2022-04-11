/*
PA2lib Template.
This is ready to use, no need for installation or anything else.
Enjoy making games

You need to convert textures via GRIT, next you put them into the nitrofs folder

By: Aerglass
Date: 17/01/22
*/
#include <stdio.h>				// Include C
#include <nds.h>				//include Libnds
#include <PA_lib.h>			 	//include PA2lib

//main block of the program.
int main(int argc, char **argv) {
	int y = 192; //a variable for the ballon.
	SPA_Init(1);				//inits everything
	SPA_LoadBg("PA2lib", "bg", 256, 256, 1, 3); //loads background
	SPA_LoadSprite(1,0,32,32,"ball",false); //Load the ball.
	SPA_LoadSprite(1,1,32,32,"ballon",false); //Load the ballon.
	PA_CreateSprite(1,0,0,0,0,0);//Create the ball
	PA_CreateSprite(1,1,1,1,0,64);//Create the ballon

	//loop for ever.
	while(1) {
		scanKeys();			//scan keys
		int Held = keysHeld();		//keys variable
		if(Held & KEY_START) return 0; 	//if the start button is held, turn off the console.
		if(y < 0) y = 192;
		y --; //make the ballon go up.
		SPA_SetSpriteXY(1,1,90,y); //set the ballon XY.
		SPA_Flush();			//update everything (oam, text, swiwaitforvblank)
	}
	return 0; 				//turn off
}
