#ifdef __cplusplus
extern "C" {
#endif

#ifndef __PA_SPRITE3D_H__
#define __PA_SPRITE3D_H__

// Includes devKitPro
#include <nds.h>

// Numero maximo de sprites en pantalla
#define PA_3DSPRITES 256

// Estructura de control de los sprites 3d
typedef struct {
	s16 x;					// Coordenada X
	s16 y;					// Coordenada Y
	s16 z;					// Coordenada Z
	s16 rx;					// Rotacion Eje X (-512/0/512) << 6
	s16 ry;					// Rotacion Eje Y (-512/0/512) << 6
	s16 rz;					// Rotacion Eje Z (-512/0/512) << 6
	bool rot;				// Rotacion en uso
	u16 sx;					// Escala X (0/64/512) << 6
	u16 sy;					// Escala Y (0/64/512) << 6
	bool scale;				// Escalado en uso
	s16 width;				// Ancho del sprite
	s16 height;				// Alto del sprite
	bool inuse;				// Esta en uso?
	bool show;				// Debe mostrarse el sprite?
	u32 gfx_tex_format;		// Guarda el formato de la textura
	u32 gfx;				// Direccion donde esta almacenado el grafico en VRAM
	u16 gfxid;				// Id de Gfx usado
	u16 frame;				// Frame actual
	u16 newframe;			// Frame al que cambiar
	u16 framesize;			// Tamaño del frame (en bytes)
	u16 lastframe;			// Ultimo frame
	u32 gfx_pal_format;		// Guarda el formato de la paleta
	u32 pal;				// Direccion donde esta almacenada la paleta en VRAM
	u16 palid;				// Id de la paleta usada
	u16 prio;				// Prioridad de dibujado del sprite
	u8 poly_id;				// Identificador unico para el Alpha (0 por defecto, 63 prohibido)
	u8 alpha;				// Nivel de alpha (0 - 31) (31 por defecto)
} PA_TYPE_3DSPRITE_IPAO;
extern PA_TYPE_3DSPRITE_IPAO PA_3DSPRITE[PA_3DSPRITES];

// Estructura de control Texturas en VRAM
typedef struct {
	u32 size;			// Tamaño (en bytes) del Gfx
	u16 width;			// Ancho del Gfx
	u16 height;			// Altura del Gfx
	u32 address;		// Posicion en la VRAM
	u16 ramid;			// Numero de Slot en RAM del que provienes
	u16 framesize;		// Tamaño del frame (en bytes)
	u16 lastframe;		// Ultimo frame
	bool keepframes;	// Si es un Sprite animado, debes de mantener los frames en RAM ?
	bool inuse;			// Disponibilidat del Slot
} PA_TYPE_TEX256VRAM_IPAO;
extern PA_TYPE_TEX256VRAM_IPAO PA_TEX256VRAM[PA_3DSPRITES];

// Estructura de control de las paletas en VRAM
typedef struct {
	bool inuse;		// Slot en uso
	u8 ramslot;		// Paleta original en RAM
} PA_TYPE_3DSPRPALSLOT_IPAO;
extern PA_TYPE_3DSPRPALSLOT_IPAO PA_TEXPALSLOT[32];

// Define la esturctura de control de la VRAM para Sprites 3d
typedef struct {
	s32 free;					// Memoria VRAM libre
	u32 next;					// Siguiente posicion libre
	u32 last;					// Ultima posicion usada
	u32 pos[PA_3DSPRITES];		// Posicion en VRAM para reusar despues de un borrado
	u32 size[PA_3DSPRITES];		// Tamaño del bloque libre para reusar
	u16 deleted;				// Numero de bloques borrados
	s32 fragmented;				// Memoria VRAM fragmentada
	s32 inarow;					// Memoria VRAM contigua
} PA_TYPE_TEXVRAM_IPAO;
extern PA_TYPE_TEXVRAM_IPAO PA_TEXVRAM;		// IPAormacion VRAM de texturas


// Define la estructura de control de los sprites 3d creados
typedef struct {
	s16 total;					// Numero de sprites creados
	u16 id[PA_3DSPRITES];		// ID del Sprite
	u16 bck[PA_3DSPRITES];		// Backup del ID
} PA_TYPE_CREATED_3DSPRITE_IPAO;
extern PA_TYPE_CREATED_3DSPRITE_IPAO PA_CREATED_3DSPRITE;

// Funcion PA_Init3dSpriteSys();
void PA_Init3dSpriteSys(void);
// Inicializa el sistema de Sprites en 3D

// Funcion PA_Vram3dSpriteGfx();
void PA_Vram3dSpriteGfx(u16 ram, u16 vram, bool keepframes);
// Transfiere un grafico de la RAM a la VRAM

// Funcion PA_Free3dSpriteGfx();
void PA_Free3dSpriteGfx(u16 id);
// Elimina de la VRAM un grafico de texturas y desfragmenta la VRAM si es necesario

// Funcion PA_Vram3dSpriteGfxDefrag();
void PA_Vram3dSpriteGfxDefrag(void);
// Desfragmenta la VRAM usada para texturas

// Funcion PA_Vram3dSpritePal();
void PA_Vram3dSpritePal(u8 id, u8 slot);
// Copia una paleta a la VRAM

// Funcion PA_Create3dSprite();
void PA_Create3dSprite(u16 id, u16 gfx, u16 pal, s16 x, s16 y);
// Crea un Sprite 3D en las coordenadas indicadas

// Funcion PA_Delete3dSprite();
void PA_Delete3dSprite(u16 id);
// Borra el Sprite con la ID indicada

// Funcion PA_Sort3dSprites();
void PA_Sort3dSprites(void);
// Reordena la cola de Sprites 3D creados de menor a mayor segun su ID
// Los Sprites con numeros mas bajos tienen prioridad.

// Funcion PA_Set3dSpritePriority();
void PA_Set3dSpritePriority(u16 id, u16 prio);
// Cambia la prioridad del Sprite

// Funcion PA_Swap3dSpritePriority();
void PA_Swap3dSpritePriority(u16 id_a, u16 id_b);
// Intercambia la prioridad de dos Sprites

// Funcion PA_Move3dSprite();
void PA_Move3dSprite(u16 id, s16 x, s16 y);
// Mueve el Sprite seleccionado a las coordenadas dadas

// Funcion PA_Show3dSprite();
void PA_Show3dSprite(u16 id, bool show);
// Muestra u oculta el sprite con la id indicada

// Funcion PA_Set3dSpriteFrame();
void PA_Set3dSpriteFrame(u16 id, u16 frame);
// Cambia el frame visible del sprite indicado

// Funcion PA_Draw3dSprites();
void PA_Draw3dSprites(void);
// Dibuja en pantalla todos los sprites creados

// Funcion PA_Update3dSpritesGfx();
void PA_Update3dSpritesGfx(void);
// Actualiza si es necesario las texturas de los sprites animados

// Funcion PA_Rotate3dSprite();
void PA_Rotate3dSprite(u16 id, s16 x, s16 y, s16 z);
// Rota el Sprite sobre los ejes indicados (-512/0/512)
	
// Funcion PA_Scale3dSprite();
void PA_Scale3dSprite(u16 id, u16 x, u16 y);
// Escala el sprite al tamaño indicado (0/64/512)

// Funcion PA_Blend3dSprite();
void PA_Blend3dSprite(u8 sprite, u8 poly_id, u8 alpha);
/
// Funcion PA_3dSpritesLayer();
void PA_3dSpritesLayer(u8 layer);

// Funcion PA_3dSpriteEditPalColor();
void PA_3dSpriteEditPalColor(u8 pal, u8 number, u8 r, u8 g, u8 b);

// Funcion 	PA_3dSpriteUpdatePalette();
void PA_3dSpriteUpdatePalette(u8 pal);
// Actualiza la paleta en VRAM con la copia que se encuentra en la RAM


// Funcion PA_3dSpriteGetPalColor();
void PA_3dSpriteGetPalColor(u8 pal, u8 number, u8* r, u8* g, u8* b);
// Obtiene el valor de un color de la paleta que se encuentra en la RAM


// Funcion PA_3dSpriteSetDeep();
void PA_3dSpriteSetDeep(u8 id, s16 z);

#endif

#ifdef __cplusplus
}
#endif
