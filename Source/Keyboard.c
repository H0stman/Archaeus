#include "Keyboard.h"

struct KeyboardState keystate;

void ProcessKeyboardMessage(HWND hndl, UINT message, WPARAM wParam, LPARAM lParam)
{
	///TODO: Handle keyboard input properly.
	if(message == WM_KEYDOWN)
	{
		switch (wParam)
		{
		case 0x57: //W key.
			keystate.w = TRUE;
			break;
		case 0x41: //A key.
			keystate.a = TRUE;
			break;
		case 0x53: //S key.
			keystate.s = TRUE;
			break;
		case 0x44: //D key.
			keystate.d = TRUE;
			break;
		case VK_SHIFT:
			keystate.lshift = TRUE;
			break;
		case VK_SPACE:
			keystate.space = TRUE;
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
			keystate.w = FALSE;
			break;
		case 0x41: //A key.
			keystate.a = FALSE;
			break;
		case 0x53: //S key.
			keystate.s = FALSE;
			break;
		case 0x44: //D key.
			keystate.d = FALSE;
			break;
		case VK_SHIFT:
			keystate.lshift = FALSE;
			break;
		case VK_SPACE:
			keystate.space = FALSE;
			break;
		default:
			DefWindowProc(hndl, message, wParam, lParam);
			break;
		}
	}
	else
		DefWindowProc(hndl, message, wParam, lParam);
}