//Å@==================================================================
//  game.cpp
// 
//
//  
//Å@==================================================================

#include <Windows.h>
#include "game.h"
#include "directx.h"
#include "shader.h"
#include "polygon.h"

void GameInitialize(HWND hWnd)
{
	// main.cppÇ©ÇÁ hWnd ÇéÛÇØìnÇµ
	DirectXInitialize(hWnd);

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

	Present();

}

void GameFinalize(void)
{
	DirectXFinalize();
	ShaderFinalize();
	PolygonFinalize();
}