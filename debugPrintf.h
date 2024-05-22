#ifndef _DEBUG_PRINTF_H_
#define _DEBUG_PRINTF_H_

#include <windows.h>
#include <stdio.h>

// �f�o�b�N�pprintf...VisualStudio�̏o�̓E�B���h�E���o�͂����
inline void DebugPrintf(const char* pFormat, ...)
{
	// ����ȕ��ɏ�����Release�r���h���ɂ̓r���h����Ȃ��͈͂�ݒ�ł���
#if defined(_DEBUG) || defined(DEBUG)
	va_list argp;
	char buf[256];// �o�b�t�@�͑傫�߂Ɏ��܂��傤
	va_start(argp, pFormat);
	vsprintf_s(buf, 256, pFormat, argp);
	va_end(argp);
	OutputDebugStringA(buf);
#endif // _DEBUG || DEBUG
}
#endif // _DEBUG_PRINTF_H_