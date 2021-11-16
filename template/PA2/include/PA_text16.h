#ifdef __cplusplus
extern "C" {
#endif

#ifndef __PA_TEXT16_H__
#define __PA_TEXT16_H__





// NightFox LIB - Include de Textos de 16 pixels
// Requiere DevkitARM
// Codigo por Cesar Rincon "NightFox"
// http://www.nightfoxandco.com/
// Version 20140413





// Includes devKitPro
#include <nds.h>



// Define el nº de caracteres que tiene la fuente
#define PA_TEXT_FONT_CHARS_16 127
#define PA_TEXT_FONT_LAST_VALID_CHAR_16 113




// Funcion PA_LoadTextFont16();
void PA_LoadTextFont16(const char* file, const char* name, u16 width, u16 height, u8 rotation);
// Carga una fuente para texto de 16 pixeles de altura



// Funcion PA_CreateTextLayer16();
void PA_CreateTextLayer16(u8 screen, u8 layer, u8 rotation, const char* name);
// Crea una capa de texto para fuentes de 16 pixeles



// Funcion PA_WriteText16();
void PA_WriteText16(u8 screen, u8 layer, u16 x, u16 y, const char* text);
// Escribe un texto en la capa y pantalla especificados



// Funcion PA_ClearTextLayer16();
void PA_ClearTextLayer16(u8 screen, u8 layer);
// Limpia la capa de texto especificada





#endif

#ifdef __cplusplus
}
#endif
