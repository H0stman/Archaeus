#include "window.h"

/// @brief Handles the window procedure.
/// @param hwnd A handle to the window.
/// @param uMsg The message.
/// @param wParam Additional message information. The contents of this parameter depend on the value of the uMsg parameter.
/// @param lParam Additional message information. The contents of this parameter depend on the value of the uMsg parameter.
/// @return The return value is the result of the message processing and depends on the message sent.
static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND MakeWindow(HINSTANCE hInstance)
{
	// Register the window class.

	WNDCLASS wc =
	{
		.lpfnWndProc = WindowProc,
		.hInstance = hInstance,
		.lpszClassName = "Window class",
		.hIcon = LoadIcon(hInstance, IDI_APPLICATION),
		.style = CS_HREDRAW | CS_VREDRAW,
		.hCursor = LoadCursor(hInstance, IDC_ARROW)
	};

	RegisterClass(&wc);

	// Create the window.

	return CreateWindowEx(
		0,												// Optional window styles.
		"Window class",							// Window class
		"Archaeus",									// Window text
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,	// Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,			// Parent window
		NULL,			// Menu
		hInstance, 	// Instance handle
		NULL			// Additional application data
	);
}

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ResetKeyboardState();
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		{
			if (MessageBox(hwnd, "Quit?", "Exit", MB_YESNO) == IDYES)
				DestroyWindow(hwnd);
			else
				return 0;
		}
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW));
			EndPaint(hwnd, &ps);
			break;
		}
	case WM_ACTIVATEAPP:
		ProcessMouseMessage(hwnd, uMsg, wParam, lParam);
		ProcessKeyboardMessage(hwnd, uMsg, wParam, lParam);
		break;

	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		ProcessMouseMessage(hwnd, uMsg, wParam, lParam);
		break;

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
	case WM_SYSKEYDOWN:
		ProcessKeyboardMessage(hwnd, uMsg, wParam, lParam);
		if (wParam == VK_ESCAPE)
			PostQuitMessage(EXIT_SUCCESS);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}