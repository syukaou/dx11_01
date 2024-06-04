/*==============================================================================

   ƒ|ƒŠƒSƒ“‚Ì•\Ž¦ [polygon.h]
														 Author : Youhei Sato
														 Date   : 2024/05/14
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef POLYGON_H
#define POLYGON_H

#include <d3d11.h>


static int g_TextureId [256];
void PolygonInitialize(ID3D11Device* device, ID3D11DeviceContext* device_context);
void PolygonFinalize(void);
void PolygonUpdate(void);
//void SpriteDraw(int Texture_id, float x, float y, float w, float h, float angele);
void SpriteDraw(int Texture_id, float x, float y, float w, float h, 
	int tx,int ty,int tw,int th,float angele);
//void PolygonDraw(void);

#endif // POLYGON_H