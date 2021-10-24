#ifdef __cplusplus
extern "C" {
#endif

#ifndef __PA_2D_H__
#define __PA_2D_H__





// NightFox LIB - Include de funciones 2D comunes
// Requiere DevkitARM
// Codigo por Cesar Rincon "NightFox"
// http://www.nightfoxandco.com/
// Version 20140413



// Includes devKitPro
#include <nds.h>





// Funcion PA_Set2D();
void PA_Init2D(u8 screen, u8 mode);
// Inicia el modo 2D seleccionado en la pantalla deseada



// Funcion PA_ShowBg();
void PA_ShowBg(u8 screen, u8 layer);
// Haz visible el fondo de la capa y pantalla dados



// Funcion PA_HideBg();
void PA_HideBg(u8 screen, u8 layer);
// Oculta el fondo de la capa y pantalla dados



// Funcion PA_ScrollBg();
void PA_ScrollBg(u8 screen, u8 layer, s16 x, s16 y);
// Mueve el fondo a las coordenadas especificadas.
// Debes de indicar la pantalla, capa, y coordenadas X e Y
// Si el mapa es mayor de 512 en alguna medida, debes de mantener el fondo en RAM



// Funcion PA_MoveSprite();
void PA_MoveSprite(u8 screen, u8 id, s16 x, s16 y);
// Mueve el Sprite a las coordenadas especificadas
// Debes de indicar la pantalla, id de sprite y coordenadas



// Funcion PA_SpriteLayer();
void PA_SpriteLayer(u8 screen, u8 id, u8 layer);
// Define la capa sobre la que el sprite sera dibujado
// Debes de indicar la pantalla, id del sprite y capa



// Funcion PA_ShowSprite();
void PA_ShowSprite(u8 screen, u8 id, bool show);
// Muestra o oculta el sprite
// Debes especificar la pantalla, id del sprite y el estado (true, false)



// Funcion PA_HflipSprite();
void PA_HflipSprite(u8 screen, u8 id, bool hflip);
// Voltea el Sprite horizontalmente
// Debes especificar la pantalla, id del sprite y el estado (true, false)



// Funcion PA_GetSpriteHflip();
extern bool PA_GetSpriteHflip(u8 screen, u8 id);
// Devuelve el estado del volteado horizontal de un sprite
// Debes especificar la pantalla y la id del sprite



// Funcion PA_VflipSprite();
void PA_VflipSprite(u8 screen, u8 id, bool vflip);
// Voltea el Sprite verticalmente
// Debes especificar la pantalla, id del sprite y el estado (true, false)



// Funcion PA_GetSpriteVflip();
extern bool PA_GetSpriteVflip(u8 screen, u8 id);
// Devuelve el estado del volteado vertical de un sprite
// Debes especificar la pantalla y la id del sprite



// Funcion PA_SpriteFrame();
void PA_SpriteFrame(u8 screen, u8 id, u16 frame);
// Cambia el frame de un Sprite
// Debes especificar la pantalla, el Id del sprite y el frame



// Funcion PA_EnableSpriteRotScale();
void PA_EnableSpriteRotScale(u8 screen, u8 sprite, u8 id, bool doublesize);
// Habilita el sprite como rotable y escalable
// Debes especificar la pantalla, n� de sprite, id de rotacion y si
// tiene que activarse el doublesize (tama�o x2)



// Funcion PA_DisableSpriteRotScale();
void PA_DisableSpriteRotScale(u8 screen, u8 sprite);
// Deshabilita un sprite como rotable y escalable
// Debes especificar la pantalla y el n� de sprite




// Funcion PA_SpriteRotScale();
void PA_SpriteRotScale(u8 screen, u8 id, s16 angle, u16 sx, u16 sy);
// Define el angulo de rotacion y escala de los Sprites asociados a la Id.
// El rango del angulo es desde -512 a 512, siendo 0 el valor central
// El rango de escalado va de 0 a 512, siendo 256 el valor normal (100%)





#endif

#ifdef __cplusplus
}
#endif
