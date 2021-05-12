#pragma once
#pragma warning(push, 0)
#include <d3d11_4.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <assert.h>
#pragma warning(pop);


ID3D11Device *device_ptr;
ID3D11DeviceContext *device_context_ptr;
IDXGISwapChain *swap_chain_ptr;
ID3D11RenderTargetView *render_target_view_ptr;
ID3D11Texture2D *framebuffer;
ID3D11InputLayout *input_layout_ptr;
ID3D11Buffer *vertex_buffer_ptr;
ID3D11VertexShader *vertex_shader_ptr;
ID3D11PixelShader *pixel_shader_ptr;
ID3DBlob *vs_blob_ptr, *ps_blob_ptr, *error_blob;


void D3D11Initialize(HWND hndl);
void Release(void);
