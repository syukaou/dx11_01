//　==================================================================
//  main.cpp
//  Windowの表示
//　==================================================================
#include <Windows.h>
#include "game.h"
#include "configuration.h"
#include <sstream>
#pragma comment (lib, "winmm.lib")
// ===================================================================
// 定数定義
// ===================================================================
#define CLASS_NAME	"GameWindow"		//ウィンドウクラスの名前



// ===================================================================
// プロトタイプ宣言
// ===================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static DWORD g_CountFPS = 0;



// メイン
int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance,_In_ LPSTR lpCmdLine,_In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;							//ウィンドウプロシージャーの指定
	wc.lpszClassName = CLASS_NAME;						//クラス名の設定
	wc.hInstance = hInstance;							//インスタンスハンドルの設定
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			//マウスカーソルの指定
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);	//ウインドウのクライアント領域の背景色を設定

	//クラス登録
	RegisterClass(&wc);


	// ウィンドウが伸び縮みしないようにするためにスタイルを変更する一文
	//DWORD style = (WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME) ^ WS_MAXIMIZEBOX;
	DWORD window_style = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);


	//基本矩形座標
	RECT window_rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	//指定したクライアント領域を確保するために新たな矩形座標を計算
	AdjustWindowRect(&window_rect, window_style, FALSE);

	//新たなWindowの矩形座標から幅と高さを算出
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;


	//ウインドウの生成
	HWND hWnd = CreateWindow(
		CLASS_NAME,		//ウィンドウ　クラス
		WINDOW_CAPTION,	//ウィンドウ　テキスト
		window_style,	//ウィンドウ　スタイル　←今日これ触る

		//サイズと位置
		100, 100,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,

		NULL,			//親ウィンドウハンドル
		NULL,			//メニューハンドル
		hInstance,		//インスタンスハンドル
		NULL			//追加のアプリケーションデータ
	);

	//指定のウィンドウハンドルのウィンドウを指定の方法で表示
	ShowWindow(hWnd, nCmdShow);

	//ここがゲームの初期化
	GameInitialize(hWnd);
	//
	DWORD dwExecLastTime = 0;
	DWORD dwFPSLastTime = 0;
	DWORD dwCurrentTime = 0;
	DWORD dwFrameCount = 0; 

	timeBeginPeriod(1); // 分解能を設定 
	dwExecLastTime = dwFPSLastTime = timeGetTime(); // システム時刻をミリ秒単位で取得 
	dwCurrentTime = dwFrameCount = 0;
	// Windowsゲーム用メインループ
	MSG msg = {};// msg.message == WM_NULL
	while (WM_QUIT != msg.message)
	{
		
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//メッセージがある場合はメッセージ処理を優先
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 1000)
			{
              #ifdef _DEBUG 
				g_CountFPS = dwFrameCount;
              #endif 
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;

			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60)) {
				dwExecLastTime = dwCurrentTime;
#ifdef _DEBUG
				std::stringstream caption;
				caption << WINDOW_CAPTION << " FPS:" << g_CountFPS;
				SetWindowText(hWnd, caption.str().c_str());
#endif
				//ここがゲームの処理
				GameUpdate();
				//ここがゲームの描画
				GameDraw();
				dwFrameCount++;
			}
			else {
				Sleep(0);
			}
		
		}
	}
	
	//ここがゲームの終了処理
	GameFinalize();


	return (int)msg.wParam;
}

//ウィンドウプロシージャ（コールバック関数）
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}
		break;

	case WM_CLOSE:
		if (MessageBox(hWnd, "本当に終了してよろしいですか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
			DestroyWindow(hWnd);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
