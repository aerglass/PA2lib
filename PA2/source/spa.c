#include <stdio.h>
#include <nds.h>
#include <filesystem.h>
#include <fat.h>
#include <string.h>
#include <unistd.h>
#include <PA_lib.h>

//SPA_Init Function
void SPA_Init(){
        consoleDemoInit(); //initializes the text
        consoleClear(); //clears the text
        setBrightness(3,0); //sets brightness
        
        PA_Init2D(0, 0); //inits 2D on bottom screen
        PA_Init2D(1, 0); //inits 2D On top screen
        PA_SetRoot("NITROFS"); //Sets root in NITROFS
        PA_InitBgBuffers(); //inits bg buffers
        PA_InitBg(0); //inits bg on bottom screen
        PA_InitBg(1); //inits bg on top screen
        PA_InitSpriteBuffers(); //inits sprite buffers
        PA_InitSprite(0); //inits sprite on bottom screen
        PA_InitSprite(1);//inits sprite on top screen
        PA_InitText(0); //inits text on bottom screen
        PA_InitText(1);//inits text on top screen
        PA_InitMapBuffers(); //inits map buffers
        PA_InitSoundBuffers(); //inits sound buffers
}
//SPA_WaitFor Function
void SPA_WaitFor(int sec){
        int i = 0; //inits a variable.
        //run a loop for 100 frames
        for (i = 0; i < sec * 100; i++) swiWaitForVBlank(); //Waits for 60 frames...
}
//SPA_LoadSprite function
    void SPA_LoadSprite(int screen, int id, int width, int height, const char *dir, bool transflag) {
    PA_LoadSpriteGfx(dir, id, width, height); //loads sprite gfx
    PA_LoadSpritePal(dir, id); //loads sprite pal
    PA_VramSpriteGfx(screen, id, id, transflag); //loads sprite gfx on VRAM
    PA_VramSpritePal(screen, id, id); //loads sprite pal on VRAM
}

//SPA_LoadBg
void SPA_LoadBg(const char *dir, int width, int height, int screen, int layer) {
    PA_LoadBg(dir, dir, width, height); //loads background
    PA_CreateBg(screen, layer, dir); //creates background
}
//SPA_UnlodBg
void SPA_UnloadBg(int screen, int layer, const char *name) {
    PA_DeleteBg(screen, layer); //delete bg
    PA_UnloadBg(name); //unload bg
}
// unload gfx and pallete in ram and vram
void SPA_UnloadSprite(int screen, int id) {
    // ram
    PA_UnloadSpriteGfx(id); //unload sprite gfx
    PA_UnloadSpritePal(id);//unload sprite pal

    PA_FreeSpriteGfx(screen, id);//frees sprite gfx
}
//SPA_Flush
void SPA_Flush() {
    PA_SpriteOamSet(1);//set sprite oam to 1
    oamUpdate(&oamSub);//updates oam

    PA_SpriteOamSet(0);//set sprite oam to 0
    oamUpdate(&oamMain);//update oam
    PA_UpdateTextLayers();//update text layers
    swiWaitForVBlank(); //waits for vblank
}
//SPA_SetSpriteX
void SPA_SetSpriteX(int screen, int id, float x){
PA_MoveSprite(screen, id, x, false); //move sprite
}
//SPA_SetSpriteY
void SPA_SetSpriteY(int screen, int id, float y){
        PA_MoveSprite(screen, id, false ,y); //move sprite
}
//SPA_SetSpriteXY
void SPA_SetSpriteXY(int screen, int id, float x, float y){
        PA_MoveSprite(screen, id, x, y);//move sprite
}
//SPA_FlipSprite
void SPA_FlipSprite(int screen, int id, int hflip, int vflip){
        PA_HflipSprite(screen, id, hflip);//hflips sprite
        PA_VflipSprite(screen, id, vflip);//vflips sprite
}
