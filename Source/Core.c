#include <Core.h>

extern struct Camera camera;
float background_colour[] = { 0.671f, 0.886f, 1.0f, 1.0f };
ID3D11Buffer* matbuff;
D3D11_SUBRESOURCE_DATA data;
D3D11_MAPPED_SUBRESOURCE updatemat;
Matrix4 matrices[3];


void Initialize(HWND windowhandle)
{
   D3D11Initialize(windowhandle);
   InitializeCamera();
   StartClock();

   //Bind camera matrices to GPU buffers.
   D3D11_BUFFER_DESC matrixbufferdesc =
   {
      .ByteWidth = sizeof(Matrix4) * ARRAYSIZE(matrices),
      .Usage = D3D11_USAGE_DYNAMIC,
      .BindFlags = D3D11_BIND_CONSTANT_BUFFER,
      .CPUAccessFlags = D3D11_CPU_ACCESS_WRITE
   };

   matrices[0] = Identity();
   matrices[1] = camera.view;
   matrices[2] = camera.projection;

   data = (D3D11_SUBRESOURCE_DATA){ .pSysMem = matrices };

   HandleHR(ID3D11Device_CreateBuffer(device_ptr, &matrixbufferdesc, &data, &matbuff));
   ID3D11DeviceContext_VSSetConstantBuffers(device_context_ptr, 0u, 1u, &matbuff);
}

void Update(void)
{
   UpdateCamera((float)DeltaTime());
   UpdateClock();
   HRESULT hr = ID3D11DeviceContext_Map(device_context_ptr, (ID3D11Resource*)matbuff, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &updatemat);
   HandleHR(hr);
   Matrix4 *mat = (Matrix4*)updatemat.pData;
   mat[0] = Identity();
   mat[1] = camera.view;
   mat[2] = camera.projection;
   ID3D11DeviceContext_Unmap(device_context_ptr, (ID3D11Resource*)matbuff, 0u);
   ID3D11DeviceContext_VSSetConstantBuffers(device_context_ptr, 0u, 1u, &matbuff);
   
}

void Render(void)
{
   ID3D11DeviceContext_ClearRenderTargetView(device_context_ptr, render_target_view_ptr, background_colour);

   ID3D11DeviceContext_OMSetRenderTargets(device_context_ptr, 1u, &render_target_view_ptr, NULL);
   ID3D11DeviceContext_Draw(device_context_ptr, 3u, 0u);
   IDXGISwapChain_Present(swap_chain_ptr, 0u, 0u);
}