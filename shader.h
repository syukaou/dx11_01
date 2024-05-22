/*==============================================================================

   シェーダーの利用 [shader.h]
														 Author : Youhei Sato
														 Date   : 2024/05/14
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef SHADER_H
#define SHADER_H

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

void ShaderInitialize(ID3D11Device* device, ID3D11DeviceContext* device_context);
void ShaderFinalize(void);

void ShaderVertexSetMatrix(const XMFLOAT4X4* matrix);

#endif // SHADER_H