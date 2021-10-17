#pragma once
#pragma warning(push, 0)
#include <d3d11_4.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <assert.h>
#pragma warning(pop);


void D3D11Initialize(HWND hndl);
void Render(void);
void Release(void);
