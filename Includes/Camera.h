#pragma once
#include "Linear.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Clock.h"
#include <crtdbg.h>

static const Vector4 UP = {0.0f, 1.0f, 0.0f, 0.0f};

struct Camera
{
   Vector4 position, focus, direction;
   Matrix4 view, projection;
};

void UpdateCamera(float delta);
void InitializeCamera(void);