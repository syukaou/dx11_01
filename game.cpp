
#include <Windows.h>
#include "game.h"
#include "directx.h"

void GameInitialize(HWND hWnd)
{
	// main.cpp���� hWnd ���󂯓n��
	DirectXInitialize(hWnd);
}


void GameUpdate(void)
{
}

void GameDraw(void)
{


}

void GameFinalize(void)
{
	DirectXFinalize;
}