#pragma once
#pragma warning(push, 0)
#include <windows.h>
#pragma warning(pop);
#include "Mouse.h"
#include "Keyboard.h"

/// @brief Makes a window of default size.
/// @return The handle to the window.
HWND MakeWindow(HINSTANCE hInstance);

/// @brief Handles the window procedure.
/// @param hwnd A handle to the window.
/// @param uMsg The message.
/// @param wParam Additional message information. The contents of this parameter depend on the value of the uMsg parameter.
/// @param lParam Additional message information. The contents of this parameter depend on the value of the uMsg parameter.
/// @return The return value is the result of the message processing and depends on the message sent.
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);