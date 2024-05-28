//�@==================================================================
//  main.cpp
//  Window�̕\��
//�@==================================================================
#include <Windows.h>
#include "game.h"
#include "configuration.h"

// ===================================================================
// �萔��`
// ===================================================================
#define CLASS_NAME	"GameWindow"		//�E�B���h�E�N���X�̖��O



// ===================================================================
// �v���g�^�C�v�錾
// ===================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);





// ���C��
int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance,_In_ LPSTR lpCmdLine,_In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;							//�E�B���h�E�v���V�[�W���[�̎w��
	wc.lpszClassName = CLASS_NAME;						//�N���X���̐ݒ�
	wc.hInstance = hInstance;							//�C���X�^���X�n���h���̐ݒ�
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			//�}�E�X�J�[�\���̎w��
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);	//�E�C���h�E�̃N���C�A���g�̈�̔w�i�F��ݒ�

	//�N���X�o�^
	RegisterClass(&wc);


	// �E�B���h�E���L�яk�݂��Ȃ��悤�ɂ��邽�߂ɃX�^�C����ύX����ꕶ
	//DWORD style = (WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME) ^ WS_MAXIMIZEBOX;
	DWORD window_style = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);


	//��{��`���W
	RECT window_rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	//�w�肵���N���C�A���g�̈���m�ۂ��邽�߂ɐV���ȋ�`���W���v�Z
	AdjustWindowRect(&window_rect, window_style, FALSE);

	//�V����Window�̋�`���W���畝�ƍ������Z�o
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;


	//�E�C���h�E�̐���
	HWND hWnd = CreateWindow(
		CLASS_NAME,		//�E�B���h�E�@�N���X
		WINDOW_CAPTION,	//�E�B���h�E�@�e�L�X�g
		window_style,	//�E�B���h�E�@�X�^�C���@����������G��

		//�T�C�Y�ƈʒu
		100, 100,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,

		NULL,			//�e�E�B���h�E�n���h��
		NULL,			//���j���[�n���h��
		hInstance,		//�C���X�^���X�n���h��
		NULL			//�ǉ��̃A�v���P�[�V�����f�[�^
	);

	//�w��̃E�B���h�E�n���h���̃E�B���h�E���w��̕��@�ŕ\��
	ShowWindow(hWnd, nCmdShow);

	//�������Q�[���̏�����
	GameInitialize(hWnd);

	// Windows�Q�[���p���C�����[�v
	MSG msg = {};// msg.message == WM_NULL
	while (WM_QUIT != msg.message)
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//���b�Z�[�W������ꍇ�̓��b�Z�[�W������D��
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//�������Q�[���̏���
			GameUpdate();
			//�������Q�[���̕`��
			GameDraw();
		}
	}

	//�������Q�[���̏I������
	GameFinalize();


	return (int)msg.wParam;
}

//�E�B���h�E�v���V�[�W���i�R�[���o�b�N�֐��j
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
		if (MessageBox(hWnd, "�{���ɏI�����Ă�낵���ł����H", "�m�F", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
			DestroyWindow(hWnd);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}