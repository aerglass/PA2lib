#include <stdio.h>
#include <nds.h>
#include <PA_lib.h>
u8 T_BG_L = 0;
u8 S_BG_L = 0;

#define PA_UPDATEPAD(pad, var) do{ \
        Pad.pad.A = (var & KEY_A) != 0; \
        Pad.pad.B = (var & KEY_B) != 0; \
        Pad.pad.X = (var & KEY_X) != 0; \
        Pad.pad.Y = (var & KEY_Y) != 0; \
        Pad.pad.L = (var & KEY_L) != 0; \
        Pad.pad.R = (var & KEY_R) != 0; \
        Pad.pad.Select = (var & KEY_SELECT) != 0; \
        Pad.pad.Start = (var & KEY_START) != 0; \
        Pad.pad.Up = (var & KEY_UP) != 0; \
        Pad.pad.Down = (var & KEY_DOWN) != 0; \
        Pad.pad.Left = (var & KEY_LEFT) != 0; \
        Pad.pad.Right = (var & KEY_RIGHT) != 0; \
        Pad.pad.Anykey = (var & 0xFFF) != 0; \
    }while(0)

//SPA_Init Function
void PA_Init(){
        consoleDemoInit(); //initializes the text
        consoleClear(); //clears the text
        setBrightness(3,0); //sets brightness
        
        PA_Init2D(0, 0); //inits screen
        PA_Init2D(1, 0);
        PA_SetRoot("NITROFS"); //Sets root in NITROFS
        PA_InitBgBuffers(); //inits bg buffers
        PA_InitBg(0); //inits bg on bottom screen
        PA_InitBg(1); //inits bg on bottom screen
        PA_InitSpriteBuffers(); //inits sprite buffers
        PA_InitSprite(0); //inits sprite on bottom screen
        PA_InitSprite(1);
        PA_InitMapBuffers(); //inits map buffers
        PA_InitSoundBuffers(); //inits sound buffers
}
void PA_InitVBL(){
    //... Do nothing because PA_InitVBL(); is useless. I may give a use to this after all lol.
    if(0 == 0) while(1) break;
}
void PA_InitText(u8 screen, u8 layer){
    switch(screen){
    case 0:
    NF_InitText(1);
    PA_LoadFont("default", "default", 256, 256, 1);
    PA_CreateTextLayer(1, layer, 0, "default");
    T_BG_L = layer;
    break;
    case 1:
    S_BG_L = layer;
    NF_InitText(0);
    PA_LoadFont("default", "default", 256, 256, 0);
    PA_CreateTextLayer(0, layer, 0, "default");
    break;
    }
}
void PA_ClearTextBg(u8 screen){
    switch(screen){
        case 1: PA_ClearTextBgSys(0,T_BG_L); break;
        case 0: PA_ClearTextBgSys(1,S_BG_L); break;
    }
}
void PA_OutputText(u8 screen, int x, int y, const char *text){
    switch(screen){
        case 0: PA_WriteText(1,T_BG_L,x,y,text);
        break;
        case 1: PA_WriteText(0,S_BG_L,x,y,text);
        break;
    }
}
void PA_OutputSimpleText(u8 screen, int x, int y, const char *text){
    switch(screen){
        case 0: PA_WriteText(1,T_BG_L,x,y,text);
        break;
        case 1: PA_WriteText(0,S_BG_L,x,y,text);
        break;
    }
}
//SPA_WaitFor Function
void PA_WaitFor(int sec){
        int i = 0; //inits a variable.
        //run a loop for 100 frames
        for (i = 0; i < sec * 100; i++) swiWaitForVBlank(); //Waits for 60 frames...
}
//SPA_LoadSprite function
//    void PA_LoadSprite(int screen, int id, int width, int height, const char *dir, bool transflag) {
void PA_LoadSprite(u8 screen, int id, int size, const char *dir){
        switch(size){
            case 88: 
        PA_LoadSpriteGfx(dir, id, 8, 8); //loads sprite gfx
        PA_LoadSpritePal(dir, id); //loads sprite pal
        PA_VramSpriteGfx(screen, id, id, false); //loads sprite gfx on VRAM
        PA_VramSpritePal(screen, id, id); //loads sprite pal on VRAM
        break;
            case 816:
            PA_LoadSpriteGfx(dir, id, 8, 16); //loads sprite gfx
            PA_LoadSpritePal(dir, id); //loads sprite pal
            PA_VramSpriteGfx(screen, id, id, false); //loads sprite gfx on VRAM
            PA_VramSpritePal(screen, id, id); //loads sprite pal on VRAM
            break;
            case 832:
            PA_LoadSpriteGfx(dir, id, 8, 32); //loads sprite gfx
            PA_LoadSpritePal(dir, id); //loads sprite pal
            PA_VramSpriteGfx(screen, id, id, false); //loads sprite gfx on VRAM
            PA_VramSpritePal(screen, id, id); //loads sprite pal on VRAM
            break;
            case 168:
            PA_LoadSpriteGfx(dir, id, 16, 8); //loads sprite gfx
            PA_LoadSpritePal(dir, id); //loads sprite pal
            PA_VramSpriteGfx(screen, id, id, false); //loads sprite gfx on VRAM
            PA_VramSpritePal(screen, id, id); //loads sprite pal on VRAM
            break;
            case 1616:
            PA_LoadSpriteGfx(dir, id, 16, 16); //loads sprite gfx
            PA_LoadSpritePal(dir, id); //loads sprite pal
            PA_VramSpriteGfx(screen, id, id, false); //loads sprite gfx on VRAM
            PA_VramSpritePal(screen, id, id); //loads sprite pal on VRAM
            break;
            case 1632:
            PA_LoadSpriteGfx(dir, id, 8, 32); //loads sprite gfx
            PA_LoadSpritePal(dir, id); //loads sprite pal
            PA_VramSpriteGfx(screen, id, id, false); //loads sprite gfx on VRAM
            PA_VramSpritePal(screen, id, id); //loads sprite pal on VRAM
            break;
            case 328:
            PA_LoadSpriteGfx(dir, id, 32, 8); //loads sprite gfx
            PA_LoadSpritePal(dir, id); //loads sprite pal
            PA_VramSpriteGfx(screen, id, id, false); //loads sprite gfx on VRAM
            PA_VramSpritePal(screen, id, id); //loads sprite pal on VRAM
            break;
            case 3216:
            PA_LoadSpriteGfx(dir, id, 32, 16); //loads sprite gfx
            PA_LoadSpritePal(dir, id); //loads sprite pal
            PA_VramSpriteGfx(screen, id, id, false); //loads sprite gfx on VRAM
            PA_VramSpritePal(screen, id, id); //loads sprite pal on VRAM
            break;
            case 3232:
            PA_LoadSpriteGfx(dir, id, 32, 32); //loads sprite gfx
            PA_LoadSpritePal(dir, id); //loads sprite pal
            PA_VramSpriteGfx(screen, id, id, false); //loads sprite gfx on VRAM
            PA_VramSpritePal(screen, id, id); //loads sprite pal on VRAM
            break;
            case 3264:
            PA_LoadSpriteGfx(dir, id, 32, 64); //loads sprite gfx
            PA_LoadSpritePal(dir, id); //loads sprite pal
            PA_VramSpriteGfx(screen, id, id, false); //loads sprite gfx on VRAM
            PA_VramSpritePal(screen, id, id); //loads sprite pal on VRAM
            break;
            case 6432:
            PA_LoadSpriteGfx(dir, id, 64, 32); //loads sprite gfx
            PA_LoadSpritePal(dir, id); //loads sprite pal
            PA_VramSpriteGfx(screen, id, id, false); //loads sprite gfx on VRAM
            PA_VramSpritePal(screen, id, id); //loads sprite pal on VRAM
            break;
            case 6464:
            PA_LoadSpriteGfx(dir, id, 64, 64); //loads sprite gfx
            PA_LoadSpritePal(dir, id); //loads sprite pal
            PA_VramSpriteGfx(screen, id, id, false); //loads sprite gfx on VRAM
            PA_VramSpritePal(screen, id, id); //loads sprite pal on VRAM
            break;
        }
}
void PA_LoadBackground(u8 screen, u8 layer, const char *dir){
    PA_LoadBg(dir, dir, 512, 512); //loads background
    PA_CreateBg(screen, layer, dir); //creates background
}
//SPA_UnlodBg
void PA_UnloadBackground(int screen, int layer, const char *name) {
    PA_DeleteBg(screen, layer); //delete bg
    PA_UnloadBg(name); //unload bg
}
// unload gfx and pallete in ram and vram
void PA_UnloadSprite(int screen, int id) {
    // ram
    PA_UnloadSpriteGfx(id); //unload sprite gfx
    PA_UnloadSpritePal(id);//unload sprite pal

    PA_FreeSpriteGfx(screen, id);//frees sprite gfx
}
//SPA_Flush
void PA_WaitForVBL() {
    scanKeys();
    PA_SpriteOamSet(1);//set sprite oam to 1
    oamUpdate(&oamSub);//updates oam

    PA_SpriteOamSet(0);//set sprite oam to 0
    oamUpdate(&oamMain);//update oam
        
    PA_UpdateTextLayers();//update text layers
    swiWaitForVBlank(); //waits for vblank
    PA_UPDATEPAD(Held, keysHeld());
    PA_UPDATEPAD(Newpress, keysDown());
    PA_UPDATEPAD(Released, keysUp());
}
//SPA_SetSpriteX
void PA_SetSpriteX(int screen, int id, s32 x){
PA_MoveSprite(screen, id, x, false); //move sprite
}
//SPA_SetSpriteY
void PA_SetSpriteY(int screen, int id, s32 y){
        PA_MoveSprite(screen, id, false ,y); //move sprite
}
//SPA_SetSpriteXY
void PA_SetSpriteXY(int screen, int id, s32 x, s32 y){
        PA_MoveSprite(screen, id, x, y);//move sprite
}
//SPA_FlipSprite
void PA_FlipSprite(int screen, int id, int hflip, int vflip){
        PA_HflipSprite(screen, id, hflip);//hflips sprite
        PA_VflipSprite(screen, id, vflip);//vflips sprite
}
