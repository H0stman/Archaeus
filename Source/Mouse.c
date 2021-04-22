#include <Mouse.h>

struct State mousestate;

HWND windowHandle;
RAWINPUTDEVICE Rid;
BYTE lpb[sizeof(RAWINPUT)];
UINT dwSize = sizeof(RAWINPUT);

void ProcessMouseMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INPUT:
	{
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
		RAWINPUT* raw = (RAWINPUT*)lpb;
		if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			mousestate.x = raw->data.mouse.lLastX;
			mousestate.y = raw->data.mouse.lLastY;
			mousestate.leftButton = (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) == RI_MOUSE_LEFT_BUTTON_DOWN ? TRUE : FALSE;
			mousestate.rightButton = (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) == RI_MOUSE_RIGHT_BUTTON_DOWN ? TRUE : FALSE;
			mousestate.middleButton = (raw->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN) == RI_MOUSE_MIDDLE_BUTTON_DOWN ? TRUE : FALSE;
			if ((raw->data.mouse.usButtonFlags & RI_MOUSE_WHEEL) == RI_MOUSE_WHEEL)
				mousestate.scrollWheelDelta = (short)raw->data.mouse.usButtonData / WHEEL_DELTA;
		}
		break;
	}	
	case WM_ACTIVATEAPP:
		break;

	case WM_MOUSEMOVE:
		mousestate.x = GET_X_LPARAM(lParam);
		mousestate.y = GET_Y_LPARAM(lParam);
		break;

	case WM_LBUTTONDOWN:
		mousestate.leftButton = TRUE;
		break;

	case WM_LBUTTONUP:
		mousestate.leftButton = FALSE;
		break;

	case WM_RBUTTONDOWN:
		mousestate.rightButton = TRUE;
		break;

	case WM_RBUTTONUP:
		mousestate.rightButton = FALSE;
		break;

	case WM_MBUTTONDOWN:
		mousestate.middleButton = TRUE;
		break;

	case WM_MBUTTONUP:
		mousestate.middleButton = FALSE;
		break;

	case WM_MOUSEWHEEL:
		mousestate.scrollWheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
	default:
		DefWindowProc(hwnd, message, wParam, lParam);
		break;
	}


}

void SetMouseMode(Mode mode)
{
	if (mode == MODE_RELATIVE)
	{
		Rid.dwFlags = RIDEV_NOLEGACY;
		Rid.hwndTarget = windowHandle;
		if (RegisterRawInputDevices(&Rid, 1u, sizeof(RAWINPUTDEVICE)) == FALSE)
		{
			OutputDebugString("Error registering raw input device!");
			return;
		}
		ShowCursor(FALSE);
		mousestate.positionMode = mode;
	}
	else if (mode == MODE_ABSOLUTE)
	{
		Rid.dwFlags = RIDEV_REMOVE;
		Rid.hwndTarget = NULL;
		if (RegisterRawInputDevices(&Rid, 1u, sizeof(RAWINPUTDEVICE)) == FALSE)
		{
			OutputDebugString("Error removing raw input device!");
			return;
		}
		ShowCursor(TRUE);
		mousestate.positionMode = mode;
	}
}

void InitMouse(HWND window, Mode mode)
{
	if (mode == MODE_RELATIVE)
	{
		Rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
		Rid.usUsage = HID_USAGE_GENERIC_MOUSE;
		Rid.dwFlags = RIDEV_NOLEGACY | RIDEV_CAPTUREMOUSE; //adds mouse and also ignores legacy mouse messages
		Rid.hwndTarget = window;
		windowHandle = window;
		if (RegisterRawInputDevices(&Rid, 1u, sizeof(RAWINPUTDEVICE)) == FALSE)
		{
			OutputDebugString("Error registering raw input device!");
			return;
		}
		ShowCursor(FALSE);
	}
	mousestate.positionMode = mode;
}