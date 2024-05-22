/*==============================================================================

   ƒ|ƒŠƒSƒ“‚Ì•\Ž¦ [polygon.h]
														 Author : Youhei Sato
														 Date   : 2024/05/14
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef POLYGON_H
#define POLYGON_H

#include <d3d11.h>

void PolygonInitialize(ID3D11Device* device, ID3D11DeviceContext* device_context);
void PolygonFinalize(void);
void PolygonUpdate(void);
void PolygonDraw(void);


#endif // POLYGON_H

