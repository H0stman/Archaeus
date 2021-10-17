#pragma once
#pragma warning(push, 0)
#include <windows.h>
#include <stdio.h>
#include <hidusage.h>
#include <strsafe.h>
#pragma warning(pop);

typedef enum
{
   KEY_UP,
   KEY_DOWN,
}KeyState;

struct KeyboardState
{
   KeyState w;
   KeyState a;
   KeyState s;
   KeyState d;
   KeyState r;
   KeyState space;
   KeyState shift;
};

void ProcessKeyboardMessage(HWND hndl, UINT message, WPARAM wParam, LPARAM lParam);
void InitKeyboard(HWND window);