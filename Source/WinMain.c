#include "window.h"
#include "error.h"
#include "core.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nShowCmd);

	HWND wndhndl = MakeWindow(hInstance);

	Initialize(wndhndl);

	MSG msg = { 0 };
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

	// ID3D11Debug* debug;
	// ID3D11Device_QueryInterface(device_ptr, &IID_ID3D11Debug, &debug);
	// ID3D11Debug_ReportLiveDeviceObjects(debug, D3D11_RLDO_DETAIL);
	// ID3D11Debug_Release(debug);

	return EXIT_SUCCESS;
}
