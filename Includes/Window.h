#pragma once
#pragma warning(push, 0)
#include <windows.h>
#pragma warning(pop);
#include "mouse.h"
#include "keyboard.h"

/// @brief Makes a window of default size.
/// @param hInstance The instance of the program.
/// @return The handle to the window.
HWND MakeWindow(HINSTANCE hInstance);