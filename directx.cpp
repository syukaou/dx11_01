

#include <Windows.h>
#include <d3d11.h>
#include "directx.h"
#include "configuration.h"
#pragma comment (lib, "d3d11.lib")
#if _DEBUG 
#pragma comment(lib, "DirectXTex_Debug.lib") 
#else 
#pragma comment(lib, "DirectXTex_Release.lib") 
#endif 

static D3D_FEATURE_LEVEL g_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

// ===================================================================
// グローバル変数
// ===================================================================
static ID3D11Device* g_Device = NULL;
static ID3D11DeviceContext* g_DeviceContext = NULL;
static IDXGISwapChain* g_SwapChain = NULL;
static ID3D11Texture2D* g_DepthStencilTexture = NULL;
static ID3D11RenderTargetView* g_RenderTargetView = NULL;
static ID3D11DepthStencilView* g_DepthStencilView = NULL;

static ID3D11RasterizerState* g_RasterizerState = NULL;
static ID3D11BlendState* g_BlendState = NULL;
static ID3D11DepthStencilState* g_DepthStenCilStateDepthEnable = NULL;
static ID3D11DepthStencilState* g_DepthStenCilStateDepthDisable = NULL;



void DirectXInitialize(HWND hWnd)
{
	// デバイス、スワップチェイン、コンテキスト生成
	DXGI_SWAP_CHAIN_DESC sd;

	ZeroMemory(&sd, sizeof(sd));

	sd.BufferCount = 1;
	sd.BufferDesc.Width = SCREEN_WIDTH;
	sd.BufferDesc.Height = SCREEN_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// main.cppから game.cpp経由 hWnd を受け渡し
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;

	HRESULT hr = D3D11CreateDeviceAndSwapChain
	(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&g_SwapChain,
		&g_Device,
		&feature_level,
		&g_DeviceContext
	);


	if (FAILED(hr)) {
		// メッセージボックスなどによるエラー表?


		return;
	}

	//レンダーターゲットビュー生成
	ID3D11Texture2D* pBackBuffer = NULL;
	g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	g_Device->CreateRenderTargetView(pBackBuffer, NULL, &g_RenderTargetView);
	pBackBuffer->Release();


	// デプスステンシル?テクスチャー作成
	D3D11_TEXTURE2D_DESC td = {};
	td.Width = sd.BufferDesc.Width;
	td.Height = sd.BufferDesc.Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc = sd.SampleDesc;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	g_Device->CreateTexture2D(&td, NULL, &g_DepthStencilTexture);

	// デプスステンシルターゲット作成
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags = 0;
	g_Device->CreateDepthStencilView
	(
		g_DepthStencilTexture,
		&dsvd,
		&g_DepthStencilView
	);

	// デバイスコンテキストへレンダーターゲットビューとデプスステンシルビューを設定
	g_DeviceContext->OMSetRenderTargets(1, &g_RenderTargetView, g_DepthStencilView);

	//ビューポイント設定
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SCREEN_WIDTH;
	vp.Height = (FLOAT)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_DeviceContext->RSSetViewports(1, &vp);

	//ラスタライザステートの作成
	D3D11_RASTERIZER_DESC rd = {};
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_BACK;
	rd.DepthClipEnable = TRUE;
	rd.MultisampleEnable = FALSE;

	g_Device->CreateRasterizerState(&rd, &g_RasterizerState);

	//デバイスコンテキストにラスタライザーステートを設定
	g_DeviceContext->RSSetState(g_RasterizerState);

	//ブレンドステート作成
	D3D11_BLEND_DESC bd = {};
	bd.AlphaToCoverageEnable = FALSE;
	bd.IndependentBlendEnable = FALSE;
	bd.RenderTarget[0].BlendEnable = TRUE;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_DEST_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	float blend_factor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	g_Device->CreateBlendState(&bd, &g_BlendState);
	g_DeviceContext->OMSetBlendState(g_BlendState, blend_factor, 0xffffffff);

	//深度ステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC dsd = {};

	//深度有効ステート
	dsd.DepthEnable = TRUE;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS;
	dsd.StencilEnable = FALSE;

	g_Device->CreateDepthStencilState(&dsd, &g_DepthStenCilStateDepthEnable);
	//深度無効ステート
	dsd.DepthEnable = FALSE;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	g_Device->CreateDepthStencilState(&dsd, &g_DepthStenCilStateDepthDisable);

	//とりあえず深度無効にしておく
	g_DeviceContext->OMSetDepthStencilState(g_DepthStenCilStateDepthDisable, NULL);

}




void DirectXFinalize(void)
{
	if (g_DepthStencilView) {
		g_DepthStencilView->Release();
		g_DepthStencilView = NULL;
	}
	if (g_DepthStencilTexture) {
		g_DepthStencilTexture->Release();
		g_DepthStencilTexture = NULL;
	}
	if (g_RenderTargetView) {
		g_RenderTargetView->Release();
		g_RenderTargetView = NULL;
	}
	if (g_SwapChain) {
		g_SwapChain->Release();
		g_SwapChain = NULL;
	}
	if (g_DeviceContext) {
		g_DeviceContext->Release();
		g_DeviceContext = NULL;
	}
	if (g_Device) {
		g_Device->Release();
		g_Device = NULL;
	}


}

ID3D11Device* DirectXGetDevice(void)
{
	return g_Device;
}
ID3D11DeviceContext* DirectXGetDeviceContext(void)
{
	return g_DeviceContext;
}



// ===================================================================
// バッファクリア
// ===================================================================
void Clear(void)
{
	// バックバッファクリア

	//clear画面の色
	float clear_color[4] = { 160.0f / 255, 216.0f / 255, 239.0f / 255, 1.0f };
	g_DeviceContext->ClearRenderTargetView(g_RenderTargetView, clear_color);
	g_DeviceContext->ClearDepthStencilView(g_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

}

void Present(void)
{
	// バックバッファとフロントバッファの交換
	g_SwapChain->Present(0, 0);
}