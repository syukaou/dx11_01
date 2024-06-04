#include "texture.h"
#include "DirectXTex.h"
#include "directx.h"

using namespace DirectX;
#define TEXTURE_MAX (256)

struct TexturData {
	ID3D11ShaderResourceView* shader_resource_view;
	int w;
	int h;
};
static TexturData g_Textures[TEXTURE_MAX] = {};
static int g_TextureCount = 0;
static ID3D11Device* g_Device = 0;
void Texturelnitialize(ID3D11Device* device)
{
	for (size_t i = 0; i < TEXTURE_MAX; i++)
	{
		g_Textures[i].shader_resource_view = NULL;
	}
	g_TextureCount = 0;
	g_Device = device;
}

int TextureLoad(const std::wstring& texture_filename)
{
	// テクスチャ読み込み 
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(texture_filename.c_str(), WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(g_Device, image.GetImages(), image.GetImageCount(),metadata, &g_Textures[g_TextureCount].shader_resource_view);
	g_Textures[g_TextureCount].w = metadata.width;
	g_Textures[g_TextureCount].h = metadata.height;
	
	if (!g_Textures[g_TextureCount].shader_resource_view) {
		//MessageBoxなどでテクスチャが読み込めなかったことを警告する 
		MessageBoxW(NULL, L"NO File", texture_filename.c_str(),MB_ICONEXCLAMATION|MB_OK);
		return - 1;
	}
	int ret = g_TextureCount;
	g_TextureCount++;
	return  ret;
}

void TextureFinalize(void)
{
	for (size_t i = 0; i < g_TextureCount; i++)
	{
		SAFE_RELEASE(g_Textures[i].shader_resource_view);
	}
}

int TextureGetW(int id)
{
	return g_Textures[id].w ;
}

int TextureGetH(int id)
{
	return g_Textures[id].h;
}

ID3D11ShaderResourceView* GetTexture(int id)
{
	if (id < 0 || id >= g_TextureCount) {
		return NULL;
	};
	return g_Textures[id].shader_resource_view;
}
