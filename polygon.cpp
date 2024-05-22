/*
   ポリゴンの表示 [polygon.cpp]
														 Author : Youhei Sato
														 Date   : 2024/05/14
--------------------------------------------------------------------------------

==============================================================================*/
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

#include "directx.h"
#include "configuration.h"
#include "shader.h"
#include "DirectXTex.h"

#define NUM_VERTEX 4	//必要な頂点の数
//これを増やせば増える

static void SetVertex(float x, float y, float w, float h);
static void SetMatrix(void);

static ID3D11Buffer* g_VertexBuffer = NULL;			// 線用の頂点情報

// 注意！初期化で外部から設定されるもの。Release不要。
static ID3D11Device* g_Device = NULL;
static ID3D11DeviceContext* g_DeviceContext = NULL;
static ID3D11ShaderResourceView* g_Texture = NULL; // シェーダーリソースビュー…つまりテクスチャ

// 頂点構造体
struct Vertex {
	XMFLOAT3 pos;	// 座標
	XMFLOAT4 color;	// 頂点カラー
	XMFLOAT2 texcoord; // テクスチャ座標(UV値とも⾔う) 
};

static void SpriteDraw(ID3D11ShaderResourceView*texture,float x,float y,float w,float h) {
	// 頂点バッファへ頂点データの書き込み
	SetVertex(x, y,w,h);

	// テクスチャ設定 
	g_DeviceContext->PSSetShaderResources(0, 1, &texture);
	// ポリゴン描画
	g_DeviceContext->Draw(NUM_VERTEX, 0);
}
void PolygonInitialize(ID3D11Device* device, ID3D11DeviceContext* device_context)
{
	// 取得するdeviceとdevice_contextのどちらかがNULLだったら正しく初期化できないので、エラー処理した方が良い
	if (!device || !device_context) {
		// MessageBoxなどでエラー処理すべし
	}

	// デバイスとデバイスコンテキストの保存
	g_Device = device;
	g_DeviceContext = device_context;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(Vertex) * NUM_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	g_Device->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	// テクスチャ読み込み 
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\texture\\1.jpeg", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(g_Device, image.GetImages(), image.GetImageCount(),metadata, &g_Texture);
	if (!g_Texture) {
		// MessageBoxなどでテクスチャが読み込めなかったことを警告する 
	}
}

void PolygonFinalize(void)
{
	SAFE_RELEASE(g_VertexBuffer);
	SAFE_RELEASE(g_Texture);
}

void PolygonUpdate(void)
{
}

void PolygonDraw(void)
{
	// 2D描画用行列の設定
	SetMatrix();
	// 頂点バッファへ頂点データの書き込み
	SpriteDraw(g_Texture, SCREEN_WIDTH * 0.5-512, SCREEN_HEIGHT*0.5-384, 1024.0f, 768.0f);

// ポリゴン描画
	g_DeviceContext->Draw(NUM_VERTEX, 0);
	
}

// これが頂点バッファ
void SetVertex(float x ,float y,float w,float h)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	g_DeviceContext->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	Vertex* v = (Vertex*)msr.pData;

	static float a = 0.0f;
	// 左上
	v[0].pos.x = x;
	v[0].pos.y = y;
	v[0].pos.z = 0.0f;
	v[0].color = { 1.0f, 1.0f, 1.0f, 1.0f };
	v[0].texcoord = { 0.0f,0.0f };
	// 右上
	v[1].pos.x = w+x;
	v[1].pos.y = y;
	v[1].pos.z = 0.0f;
	v[1].color = { 1.0f, 1.0f, 1.0f, 1.0f };
	v[1].texcoord = { 1.0f,0.0f };
	// 左下
	v[2].pos.x = x;
	v[2].pos.y = y+h;
	v[2].pos.z = 0.0f;
	v[2].color = { 1.0f, 1.0f, 1.0f, 1.0f };
	v[2].texcoord = { 0.0f,1.0f };
	// 右下
	v[3].pos.x = w + x;
	v[3].pos.y = h + y;
	v[3].pos.z = 0.0f;
	v[3].color = { 1.0f, 1.0f, 1.0f, 1.0f };
	v[3].texcoord = { 1.0f,1.0f };
	

	g_DeviceContext->Unmap(g_VertexBuffer, 0);

	// 頂点バッファ設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_DeviceContext->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);
	// プリミティブトポロジ設定
	g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	//g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


}

void SetMatrix(void)
{
	// マトリクス設定
	XMMATRIX worldViewProjection;

	worldViewProjection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	worldViewProjection = XMMatrixTranspose(worldViewProjection);

	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, worldViewProjection);

	// 頂点シェーダの固定バッファに行列を送信
	ShaderVertexSetMatrix(&matrix);
}

	