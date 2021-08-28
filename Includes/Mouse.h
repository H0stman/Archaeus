#pragma once
#pragma warning(push, 0)
#include <windows.h>
#include <windowsx.h>
#include <strsafe.h>
#include <hidusage.h>
#include <stdlib.h>
#pragma warning(pop);
#include <Error.h>

typedef enum
{
   MODE_RELATIVE,
   MODE_ABSOLUTE
}Mode;

struct State
{
   BOOL leftButton;
   BOOL middleButton;
   BOOL rightButton;
   BOOL xButton1;
   BOOL xButton2;
   long x;
   long y;
   short scrollWheelDelta;
   Mode positionMode;
};

void ProcessMouseMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void SetMouseMode(Mode mode);
void InitMouse(HWND window, Mode mode);
void ResetMouseDelta(void);