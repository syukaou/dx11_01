#pragma once
#include <windows.h>
#ifndef GAME_H
#define GAME_H

void GameInitialize(HWND hWnd);
void GameUpdate(void);
void GameDraw(void);
void GameFinalize(void);


#endif //GAME_H