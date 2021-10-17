#include "keyboard.h"

static RAWINPUTDEVICE Rid;
static BYTE lpb[sizeof(RAWINPUT)];
static UINT dwSize = sizeof(RAWINPUT);

struct KeyboardState keystate;

void ProcessKeyboardMessage(HWND hndl, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_KEYDOWN)
	{
		switch (wParam)
		{
		case 0x57: //W key.
			keystate.w = KEY_DOWN;
			break;
		case 0x41: //A key.
			keystate.a = KEY_DOWN;
			break;
		case 0x53: //S key.
			keystate.s = KEY_DOWN;
			break;
		case 0x44: //D key.
			keystate.d = KEY_DOWN;
			break;
		case VK_SHIFT:
			keystate.shift = KEY_DOWN;
			break;
		case VK_SPACE:
			keystate.space = KEY_DOWN;
			break;
		default:
			DefWindowProc(hndl, message, wParam, lParam);
			break;
		}
	}
	else if (message == WM_KEYUP)
	{
		switch (wParam)
		{
		case 0x57: //W key.
			keystate.w = KEY_UP;
			break;
		case 0x41: //A key.
			keystate.a = KEY_UP;
			break;
		case 0x53: //S key.
			keystate.s = KEY_UP;
			break;
		case 0x44: //D key.
			keystate.d = KEY_UP;
			break;
		case VK_SHIFT:
			keystate.shift = KEY_UP;
			break;
		case VK_SPACE:
			keystate.space = KEY_UP;
			break;
		default:
			DefWindowProc(hndl, message, wParam, lParam);
			break;
		}
	}
	else
		DefWindowProc(hndl, message, wParam, lParam);
}

void InitKeyboard(HWND window)
{
	Rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
	Rid.usUsage = HID_USAGE_GENERIC_KEYBOARD;
	Rid.dwFlags = RIDEV_NOLEGACY;
	Rid.hwndTarget = window;

	if (RegisterRawInputDevices(&Rid, 1u, sizeof(RAWINPUTDEVICE)) == FALSE)
	{
		OutputDebugString("Error registering raw input device!");
		return;
	}
}