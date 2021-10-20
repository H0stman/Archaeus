<<<<<<< HEAD
#include "window.h"
#include "error.h"
#include "core.h"
=======
#include <Window.h>
#include <Error.h>
#include <Core.h>
#pragma warning(push, 0)
#define CGLTF_IMPLEMENTATION
#include "cgltf.h"
#pragma warning(pop);
>>>>>>> parent of a1becee (Depth stencil.)

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nShowCmd);

	HWND wndhndl = MakeWindow(hInstance);

	cgltf_options options = { 0 };
	cgltf_data* data = NULL;
	cgltf_result result = cgltf_parse_file(&options, "..\\Resources\\Duck.gltf", &data);
	data->buffers[1];
	
	if (result == cgltf_result_success)
	{
		/* TODO make awesome stuff */
		cgltf_free(data);
	}
	
	Initialize(wndhndl);

	MSG msg = {0};
	BOOL run = TRUE;

	while (run) 
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				run = FALSE;
		}
		Update();
		Render();
	}

	Release();
	
	// ID3D11Debug *debug;
	// ID3D11Device_QueryInterface(device_ptr, &IID_ID3D11Debug, &debug);
	// ID3D11Debug_ReportLiveDeviceObjects(debug, D3D11_RLDO_DETAIL);

	// ID3D11Debug_Release(debug);
	
	return EXIT_SUCCESS;
}
