/*==========================================
テクスチャー管理「texture.h」


							Author:SYU KAO
							Date:2024/06/04
---------------------------------------------

=============================================*/
#ifndef TEXTURE_H
#define TEXTURE_H
#include<d3d11.h>
#include<string>
#include <Windows.h>


void Texturelnitialize(ID3D11Device* device);
int TextureLoad(const std::wstring& texture_filename);
void TextureFinalize(void);
int TextureGetW(int id);
int TextureGetH(int id);
ID3D11ShaderResourceView* GetTexture(int id);

#endif // !TEXTURE_H