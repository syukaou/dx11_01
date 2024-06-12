//�@==================================================================
//  game.cpp
// 
//
//  
//�@==================================================================

#include <Windows.h>
#include "game.h"
#include "directx.h"
#include "shader.h"
#include "polygon.h"
#include"texture.h"

void GameInitialize(HWND hWnd)
{
	// main.cpp���� hWnd ���󂯓n��
	DirectXInitialize(hWnd);
	Texturelnitialize(DirectXGetDevice());
	ShaderInitialize(DirectXGetDevice(), DirectXGetDeviceContext());
	PolygonInitialize(DirectXGetDevice(), DirectXGetDeviceContext());
}


void GameUpdate(void)
{
}

void GameDraw(void)
{

	Clear();

	PolygonDraw();
	//SpriteAnimeDraw(300,300);
	Present();

}

void GameFinalize(void)
{
	DirectXFinalize();
	ShaderFinalize();
	PolygonFinalize();
}