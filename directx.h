#ifndef DIRECTX_H
#define DIRECTX_H


#include <Windows.h>
#include<d3d11.h>

void DirectXInitialize(HWND hWnd);
void DirectXFinalize(void);

#define SAFE_RELEASE(o) if (o) { (o)->Release(); o = NULL; }

ID3D11Device* DirectXGetDevice(void);
ID3D11DeviceContext* DirectXGetDeviceContext(void);

void Clear(void);
void Present(void);

#endif // DIRECTX_H

