#include <stdio.h>
#include <nds.h>
#include <filesystem.h>
#include <fat.h>
#include <string.h>
#include <unistd.h>
#include <PA_lib.h>

//do simplepalib things
void SPA_BasicSetup(){
        PA_Init2D(0, 0);
        PA_Init2D(1, 0);
        PA_SetRoot("NITROFS");
        PA_InitBgBuffers();
        PA_InitBg(0);		
        PA_InitBg(1);
        
        PA_InitSpriteBuffers();
        PA_InitSprite(0);
        PA_InitSprite(1);
        PA_InitText(0);
        PA_InitText(1);
}
//do simplepalib things
void SPA_Init(){
        PA_Init2D(0, 0);
        PA_Init2D(1, 0);
        PA_SetRoot("NITROFS");
        PA_InitBgBuffers();
        PA_InitBg(0);		
        PA_InitBg(1);
        PA_InitSpriteBuffers();
        PA_InitSprite(0);
        PA_InitSprite(1);
        PA_InitText(0);
        PA_InitText(1);
        PA_InitMapBuffers();
        PA_InitSoundBuffers();
}
void SPA_WaitFor(int sec){
        for (int i = 0; i < sec * 40000; i++){
        printf(" ");
        }
}
    void SPA_LoadSprite(int screen, int id, int width, int height, const char *dir, bool transflag) {
    PA_LoadSpriteGfx(dir, id, width, height);
    PA_LoadSpritePal(dir, id);
    PA_VramSpriteGfx(screen, id, id, transflag);
    PA_VramSpritePal(screen, id, id);
}

void SPA_LoadBackground(const char *dir, const char *name, int width, int height, int screen, int layer) {
    PA_LoadBg(dir, name, width, height);
    PA_CreateBg(screen, layer, name);
}
void SPA_unloadbg(int screen, int layer, const char *name) {
    PA_DeleteBg(screen, layer);
    PA_UnloadBg(name);
}
// unload gfx and pallete in ram and vram
void SPA_unloadsprite(int screen, int id) {
    // ram
    PA_UnloadSpriteGfx(id);
    PA_UnloadSpritePal(id);

    PA_FreeSpriteGfx(screen, id);
}
void SPA_Flush() {
    PA_SpriteOamSet(1);
    oamUpdate(&oamSub);

    PA_SpriteOamSet(0);
    oamUpdate(&oamMain);
    PA_UpdateTextLayers();
    swiWaitForVBlank();
}
void SPA_SetSpriteX(int screen, int id, float x){
PA_MoveSprite(scren, id, x, false);
}
void SPA_SetSpriteY(int screen, int id, float y){
        PA_MoveSprite(screen, id, false ,y); 
}
void SPA_SetSpriteXY(int screen, int id, float x, float y){
        PA_MoveSprite(screen, id, x, y);
}
void SPA_AnimateSprite(int screen, int id, int speed, int frame, int last_frame, int variable){
        variable ++;
        if(variable > speed){
                variable = 0;
                PA_SetSpriteFrame(screen, id, frame);
                frame ++;
        }
        if(frame >= last_frame) frame = 0;
}
