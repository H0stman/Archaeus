#pragma once
#include "mouse.h"
#include "keyboard.h"
#include "linear.h"

extern struct KeyboardState keystate;
extern struct State mousestate;

struct Camera
{
   Vec4 position, focus, direction;
   Mat4 view, projection;
};

void UpdateCamera(float delta);
void InitializeCamera(void);