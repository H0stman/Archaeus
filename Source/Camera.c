#include <camera.h>

static const Vec4 UP = { 0.0f, 1.0f, 0.0f, 1.0f };
static const float rotation = 0.6f, speed = 6.0f;

struct Camera camera;

void UpdateCamera(float delta)
{
	if (mousestate.positionMode == MODE_RELATIVE)
	{
		//Orientation
		camera.direction = VecTransf(camera.direction, RotAxis(Cross(camera.direction, UP), mousestate.y * rotation));
		camera.direction = VecTransf(camera.direction, RotY(mousestate.x * rotation));
		camera.direction = VecNormEst(camera.direction);

		//Movment
		if (keystate.w == KEY_DOWN)
			camera.position = VecAdd(camera.position, VecScale(camera.direction, speed * delta));
		if (keystate.s == KEY_DOWN)
			camera.position = VecSub(camera.position, VecScale(camera.direction, speed * delta));
		if (keystate.a == KEY_DOWN)
			camera.position = VecAdd(camera.position, VecScale(Cross(camera.direction, UP), speed * delta));
		if (keystate.d == KEY_DOWN)
			camera.position = VecSub(camera.position, VecScale(Cross(camera.direction, UP), speed * delta));
		if (keystate.space == KEY_DOWN)
			camera.position = VecAdd(camera.position, VecScale(UP, speed * delta));
		if (keystate.shift == KEY_DOWN)
			camera.position = VecSub(camera.position, VecScale(UP, speed * delta));
	}
	camera.view = LookTo(camera.position, camera.direction, UP);
}

void InitializeCamera(void)
{
	camera.position = VecSet(0.0f, 0.0f, -5.0f, 0.0f);
	camera.focus = VecSet(0.0f, 0.0f, 0.0f, 1.0f);
	camera.direction = VecSet(0.0f, 0.0f, 1.0f, 0.0f);

	///NOTE: Transposing is required since the shaders handle matrices in column-major. It is now done in the shader.
	camera.view = LookTo(camera.position, camera.direction, UP);
	camera.projection = Persp(toRad(70.0f), 1424.f / 728.f, 0.1f, 100.0f);
}