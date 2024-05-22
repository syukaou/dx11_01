
#include <Windows.h>
#include "game.h"
#include "directx.h"

void GameInitialize(HWND hWnd)
{
	// main.cpp‚©‚ç hWnd ‚ðŽó‚¯“n‚µ
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