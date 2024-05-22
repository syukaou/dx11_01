#ifndef _DEBUG_PRINTF_H_
#define _DEBUG_PRINTF_H_

#include <windows.h>
#include <stdio.h>

// デバック用printf...VisualStudioの出力ウィンドウが出力される
inline void DebugPrintf(const char* pFormat, ...)
{
	// こんな風に書くとReleaseビルド時にはビルドされない範囲を設定できる
#if defined(_DEBUG) || defined(DEBUG)
	va_list argp;
	char buf[256];// バッファは大きめに取りましょう
	va_start(argp, pFormat);
	vsprintf_s(buf, 256, pFormat, argp);
	va_end(argp);
	OutputDebugStringA(buf);
#endif // _DEBUG || DEBUG
}
#endif // _DEBUG_PRINTF_H_