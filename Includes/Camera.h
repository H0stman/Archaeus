#pragma once
#include "Linear.h"
#include "Mouse.h"
#include "Keyboard.h"

static const Vector4 UP = {0.0f, 1.0f, 0.0f, 0.0f};

struct Camera
{
   Vector4 position, focus, direction;
   Matrix4 view, projection;
};

void UpdateCamera(void);
void InitializeCamera(void);