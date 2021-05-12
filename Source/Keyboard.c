#include "Keyboard.h"

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

void ResetKeyboardState(void)
{
	keystate.w = KEY_UP;
	keystate.a = KEY_UP;
	keystate.s = KEY_UP;
	keystate.d = KEY_UP;
	keystate.lshift = KEY_UP;
	keystate.space = KEY_UP;
}