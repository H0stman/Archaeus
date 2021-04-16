#pragma once
#pragma warning(push, 0)
#include <windows.h>
#include <string.h>
#include <stdio.h>
#pragma warning(pop);


/// @brief Prints the HR code if it is considered an error.
/// @param errorcode The error code to be evalueted.
void HandleHR(HRESULT errorcode);