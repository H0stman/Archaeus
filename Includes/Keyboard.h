#pragma once
#pragma warning(push, 0)
#include <windows.h>
#include <stdio.h>
#pragma warning(pop);

struct KeyboardState
{
   BOOL w;
   BOOL a;
   BOOL s;
   BOOL d;
   BOOL r;
   BOOL space;
   BOOL lshift;
};

void ProcessKeyboardMessage(HWND hndl, UINT message, WPARAM wParam, LPARAM lParam);