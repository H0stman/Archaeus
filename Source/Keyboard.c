#include "Keyboard.h"

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
			keystate.w = keystate.w == KEY_DOWN ? KEY_DOWN : KEY_PRESSED;
			break;
		case 0x41: //A key.
			keystate.a = keystate.a == KEY_DOWN ? KEY_DOWN : KEY_PRESSED;
			break;
		case 0x53: //S key.
			keystate.s = keystate.s == KEY_DOWN ? KEY_DOWN : KEY_PRESSED;
			break;
		case 0x44: //D key.
			keystate.d = keystate.d == KEY_DOWN ? KEY_DOWN : KEY_PRESSED;
			break;
		case VK_SHIFT:
			keystate.shift = keystate.shift == KEY_DOWN ? KEY_DOWN : KEY_PRESSED;
			break;
		case VK_SPACE:
			keystate.space = keystate.space == KEY_DOWN ? KEY_DOWN : KEY_PRESSED;
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
			keystate.w = KEY_RELEASED;
			break;
		case 0x41: //A key.
			keystate.a = KEY_RELEASED;
			break;
		case 0x53: //S key.
			keystate.s = KEY_RELEASED;
			break;
		case 0x44: //D key.
			keystate.d = KEY_RELEASED;
			break;
		case VK_SHIFT:
			keystate.shift = KEY_RELEASED;
			break;
		case VK_SPACE:
			keystate.space = KEY_RELEASED;
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

void ResetKeyboardState(void)
{
	if (keystate.w == KEY_PRESSED)
		keystate.w = KEY_DOWN;
	if (keystate.a == KEY_PRESSED)
		keystate.a = KEY_DOWN;
	if (keystate.s == KEY_PRESSED)
		keystate.s = KEY_DOWN;
	if (keystate.d == KEY_PRESSED)
		keystate.d = KEY_DOWN;
	if (keystate.shift == KEY_PRESSED)
		keystate.shift = KEY_DOWN;
	if (keystate.space == KEY_PRESSED)
		keystate.space = KEY_DOWN;
	if (keystate.w == KEY_RELEASED)
		keystate.w = KEY_UP;
	if (keystate.a == KEY_RELEASED)
		keystate.a = KEY_UP;
	if (keystate.s == KEY_RELEASED)
		keystate.s = KEY_UP;
	if (keystate.d == KEY_RELEASED)
		keystate.d = KEY_UP;
	if (keystate.shift == KEY_RELEASED)
		keystate.shift = KEY_UP;
	if (keystate.space == KEY_RELEASED)
		keystate.space = KEY_UP;
}