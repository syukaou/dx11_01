
#include <Windows.h>
#include "game.h"
#include "directx.h"

void GameInitialize(HWND hWnd)
{
	// main.cppから hWnd を受け渡し
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