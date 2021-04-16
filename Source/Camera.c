#include <Camera.h>

struct Camera camera;

extern struct State mousestate;
extern struct KeyboardState keystate;

const float rotation = 0.7f;
const float speed = 0.03f;

void UpdateCamera(void)
{
	if (mousestate.positionMode == MODE_RELATIVE)
	{
		///TODO: Work on making camera movement smoother.
		
		//Orientation
		camera.direction = VectorTransform(camera.direction, RotationAxis(CrossProduct(UP, camera.direction), toRad(mousestate.y * rotation * 1.2f)));
		camera.direction = VectorTransform(camera.direction, RotationY(toRad(mousestate.x * rotation)));
		camera.direction = NormalizeEst(camera.direction);

		//Movment
		if (keystate.w)
			camera.position = VectorAdd(camera.position, VectorScale(camera.direction, speed));
		if (keystate.s)
			camera.position = Subtract(camera.position, VectorScale(camera.direction, speed));
		if (keystate.a)
			camera.position = VectorAdd(camera.position, VectorScale(CrossProduct(camera.direction, UP), speed));
		if (keystate.d)
			camera.position = Subtract(camera.position, VectorScale(CrossProduct(camera.direction, UP), speed));
		if (keystate.space)
			camera.position = VectorAdd(camera.position, VectorScale(UP, speed));
		if (keystate.lshift)
			camera.position = Subtract(camera.position, VectorScale(UP, speed));

		//Reset mouse state since we are only intrested in deltas.
		mousestate.x = mousestate.y = 0;
	}
	camera.view = LookTo(camera.position, camera.direction, UP);
}

void InitializeCamera(void)
{
	camera.position = (Vector4){ 0.0f,0.0f,-1.0f,1.0f };
	camera.focus = (Vector4){ 0.0f,0.0f,0.0f,1.0f };
	camera.direction = (Vector4){ 0.0f,0.0f,1.0f,0.0f };

	///NOTE: Transposing is required because the shaders are column-major. It is now done in the shader.
	camera.view = LookTo(camera.position, camera.direction, UP);
	camera.projection = Perspective(toRad(70.0f), 1424.f / 728.f, 0.1f, 100.0f);
}