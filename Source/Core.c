#include "core.h"
#include "linear.h"

//Extern variables
extern struct Camera camera;
extern ID3D11Device* device_ptr;
extern ID3D11DeviceContext* device_context_ptr;

float background_colour[] = { 0.047f, 0.235f, 0.650f, 1.0f };
static ID3D11Buffer* matbuff;
static D3D11_SUBRESOURCE_DATA data;
static D3D11_MAPPED_SUBRESOURCE updatemat;
static Mat4 matrices[3];
static float angle = 0.0f;


void Initialize(HWND windowhandle)
{
   D3D11Initialize(windowhandle);
   InitMouse(windowhandle, MODE_ABSOLUTE);
   InitializeCamera();
   StartClock();

   //Bind camera matrices to GPU buffers.
   D3D11_BUFFER_DESC matrixbufferdesc =
   {
      .ByteWidth = sizeof(Mat4) * ARRAYSIZE(matrices),
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
   ResetMouseState();
}

