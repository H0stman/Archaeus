#pragma once
#pragma warning(push, 0)
#include <windows.h>
#include <stdio.h>
#include <hidusage.h>
#include <strsafe.h>
#include <crtdbg.h>
#pragma warning(pop);

typedef enum
{
   KEY_UP,
   KEY_DOWN,
   KEY_PRESSED,
   KEY_RELEASED
}KeyState;

struct KeyboardState
{
   KeyState w;
   KeyState a;
   KeyState s;
   KeyState d;
   KeyState r;
   KeyState space;
   KeyState lshift;
};

void ProcessKeyboardMessage(HWND hndl, UINT message, WPARAM wParam, LPARAM lParam);
void InitKeyboard(HWND window);
void ResetKeyboardState(void);