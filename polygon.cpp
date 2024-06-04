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
#include"texture.h"
#include "polygon.h"
#define NUM_VERTEX 4	//必要な頂点の数
//これを増やせば増える

static void SetVertex(void);
static void SetVertex(int x,int y,int w,int h,int texture_w,int texture_h);
void SetMatrix( float x, float y,float w,float h, float angele);
static ID3D11Buffer* g_VertexBuffer = NULL;			// 線用の頂点情報
// 注意！初期化で外部から設定されるもの。Release不要。
static ID3D11Device* g_Device = NULL;
static ID3D11DeviceContext* g_DeviceContext = NULL;

// 頂点構造体
struct Vertex {
	XMFLOAT3 pos;	// 座標
	XMFLOAT4 color;	// 頂点カラー
	XMFLOAT2 texcoord; // テクスチャ座標(UV値とも⾔う) 
};


void PolygonInitialize(ID3D11Device* device, ID3D11DeviceContext* device_context)
{
	// 取得するdeviceとdevice_contextのどちらかがNULLだったら正しく初期化できないので、エラー処理した方が良い
	if (!device || !device_context) {
		// MessageBoxなどでエラー処理すべし
	}

	// デバイスとデバイスコンテキストの保存
	DirectXGetDevice();
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
	g_TextureId[1] = TextureLoad(L"asset/texture/12345.png");
	
	
}

void PolygonFinalize(void)
{
	SAFE_RELEASE(g_VertexBuffer);
}

void PolygonUpdate(void)
{
}

/*void PolygonDraw(void)
{
	static float r = 0.0f;
	r += 0.00f;
	// 頂点バッファへ頂点データの書き込み
	//SpriteDraw(g_TextureId, 128.0f, 256.0f, 200, 200, r);
}*/

// これが頂点バッファ
void SetVertex(void)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	g_DeviceContext->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	Vertex* v = (Vertex*)msr.pData;

	static float a = 0.0f;
	// 左上
	v[0].pos.x = -0.5f;
	v[0].pos.y = -0.5f;;
	v[0].pos.z = 0.0f;
	v[0].color = { 1.0f, 1.0f, 1.0f, 1.0f };
	v[0].texcoord = { 0.0f,32.0f*3/512.0f };
	// 右上
	v[1].pos.x =0.5f;
	v[1].pos.y = -0.5;
	v[1].pos.z = 0.0f;
	v[1].color = { 1.0f, 1.0f, 1.0f, 1.0f };
	v[1].texcoord = {32.0f/512.0f,32.0f * 3 / 512.0f };
	// 左下
	v[2].pos.x = -0.5f;
	v[2].pos.y =0.5f;
	v[2].pos.z = 0.0f;
	v[2].color = { 1.0f, 1.0f, 1.0f, 1.0f };
	v[2].texcoord = {0.0f,32.0f * 4 / 512.0f };
	// 右下
	v[3].pos.x =0.5f;
	v[3].pos.y = 0.5f;
	v[3].pos.z = 0.0f;
	v[3].color = { 1.0f, 1.0f, 1.0f, 1.0f };
	v[3].texcoord = { 32.0f/512.0f,32.0f*4/512.0f };
	

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

void SetMatrix(float x, float y, float w, float h, float angele)
{

	// マトリクス設定
	XMMATRIX Projection;

	XMMATRIX scale = XMMatrixScaling(w, h, 1.0f);
	//XMMatrixRotationZはZ回転
	XMMATRIX rotation = XMMatrixRotationZ(XMConvertToRadians(angele));//XMConvertToRadiansはangleのfloatから度数へ変わる
	XMMATRIX trancelation = XMMatrixTranslation(x, y, 0.0f);

	Projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	Projection = XMMatrixTranspose(scale*rotation*trancelation*Projection);

	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, Projection);
	// 頂点シェーダの固定バッファに行列を送信
	ShaderVertexSetMatrix(&matrix);
}

static void SpriteDraw(int Texture_id, float x, float y, float w, float h,float angele ) {
	// 頂点バッファへ頂点データの書き込み
	SetVertex();
	// 2D描画用行列の設定
	SetMatrix(x,  y, w,  h, angele);

	// テクスチャ設定	
	ID3D11ShaderResourceView* srv = GetTexture(Texture_id);
	g_DeviceContext->PSSetShaderResources(0, 1, &srv);
	// ポリゴン描画
	g_DeviceContext->Draw(NUM_VERTEX, 0);
}

void SetVertex(int x, int y, int w, int h, int texture_w, int texture_h)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	g_DeviceContext->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	Vertex* v = (Vertex*)msr.pData;

	static float a = 0.0f;
	float u0 = (float)x / texture_w;
	float v0 = (float)y / texture_h;
	float u1 = (float)(x + w) / texture_w;
	float v1 = (float)(y + h) / texture_h;
	// 左上
	v[0].pos.x = -0.5f;
	v[0].pos.y = -0.5f;;
	v[0].pos.z = 0.0f;
	v[0].color = { 1.0f, 1.0f, 1.0f, 1.0f };
	v[0].texcoord = { u0,v0};
	// 右上
	v[1].pos.x = 0.5f;
	v[1].pos.y = -0.5;
	v[1].pos.z = 0.0f;
	v[1].color = { 1.0f, 1.0f, 1.0f, 1.0f };
	v[1].texcoord = { u1,v0};
	// 左下
	v[2].pos.x = -0.5f;
	v[2].pos.y = 0.5f;
	v[2].pos.z = 0.0f;
	v[2].color = { 1.0f, 1.0f, 1.0f, 1.0f };
	v[2].texcoord = {u0,v1};
	// 右下
	v[3].pos.x = 0.5f;
	v[3].pos.y = 0.5f;
	v[3].pos.z = 0.0f;
	v[3].color = { 1.0f, 1.0f, 1.0f, 1.0f };
	v[3].texcoord = {u1,v1 };


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

static void SpriteDraw(int Texture_id, float x, float y, float w, float h,
	int tx, int ty, int tw, int th, float angele) {
	
	SetVertex(tx,ty,tw,th,TextureGetW(Texture_id),TextureGetH(Texture_id));

	SetMatrix(x, y, w, h, angele);

	// テクスチャ設定	
	ID3D11ShaderResourceView* srv = GetTexture(Texture_id);
	g_DeviceContext->PSSetShaderResources(0, 1, &srv);
	// ポリゴン描画
	g_DeviceContext->Draw(NUM_VERTEX, 0);
}