#include "Keyboard.h"

static RAWINPUTDEVICE Rid;
static BYTE lpb[sizeof(RAWINPUT)];
static UINT dwSize = sizeof(RAWINPUT);

struct KeyboardState keystate;

void ProcessKeyboardMessage(HWND hndl, UINT message, WPARAM wParam, LPARAM lParam)
{
	// switch (message)
	// {
	// case WM_INPUT:
	// {
	// 	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
	// 	RAWINPUT* raw = (RAWINPUT*)lpb;
	// 	if (raw->header.dwType == RIM_TYPEKEYBOARD)
	// 	{
	// 		//_RPT0(0, "Key!");
	// 		_RPT5(0," Kbd: make=%04x Flags:%04x ExtraInformation:%08x, msg=%04x VK=%04x \n",
	// 			raw->data.keyboard.MakeCode,
	// 			raw->data.keyboard.Flags,
	// 			raw->data.keyboard.ExtraInformation,
	// 			raw->data.keyboard.Message,
	// 			raw->data.keyboard.VKey);
	// 	}
	// 	break;
	// }
	// 	break;

	// default:
	// 	DefWindowProc(hndl, message, wParam, lParam);
	// 	break;
	// }
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
			keystate.lshift = KEY_DOWN;
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
			keystate.lshift = KEY_RELEASED;
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
	keystate.w = KEY_UP;
	keystate.a = KEY_UP;
	keystate.s = KEY_UP;
	keystate.d = KEY_UP;
	keystate.lshift = KEY_UP;
	keystate.space = KEY_UP;
}