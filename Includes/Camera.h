#pragma once
#include "mouse.h"
#include "keyboard.h"
#include "import/HandmadeMath.h"

extern struct KeyboardState keystate;
extern struct State mousestate;

struct Camera
{
   hmm_vec4 position, focus, direction;
   hmm_mat4 view, projection;
};

void UpdateCamera(float delta);
void InitializeCamera(void);