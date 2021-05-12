#pragma once
#include "Linear.h"
#include "Mouse.h"
#include "Keyboard.h"
#include <crtdbg.h>

static const Vec4 UP = {0.0f, 1.0f, 0.0f, 0.0f};

struct Camera
{
   Vec4 position, focus, direction;
   Mat4 view, projection;
};

void UpdateCamera(float delta);
void InitializeCamera(void);