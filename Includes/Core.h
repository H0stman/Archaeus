#pragma once
#include "render.h"
#include "camera.h"
#include "time.h"

///@brief Initializes the engine.
void Initialize(HWND windowhandle);

///@brief Updates the game state.
void Update(void);

///@brief Executes rendering pipeline.
void Render(void);