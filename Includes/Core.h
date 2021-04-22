#pragma once
#include "Renderer.h"
#include "Camera.h"
#include "Clock.h"


///@brief Initializes the engine.
void Initialize(HWND windowhandle);

///@brief Updates the game state.
void Update(void);

///@brief Renders the game state.
void Render(void);