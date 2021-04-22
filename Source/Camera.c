#include <Camera.h>

struct Camera camera;

extern struct State mousestate;
extern struct KeyboardState keystate;

const float rotation = 100.0f, speed = 5.5f;

void UpdateCamera(float delta)
{
	//INT64 t1 = GetCount();
	if (mousestate.positionMode == MODE_RELATIVE)
	{
		///TODO: Work on making camera movement smoother.

		delta = fminf(1.0f, fmaxf(delta, 0.00015f));
		//Orientation
		camera.direction = VectorTransform(camera.direction, RotationAxis(CrossProduct(UP, camera.direction), toRad(mousestate.y * rotation * delta)));
		camera.direction = VectorTransform(camera.direction, RotationY(toRad(mousestate.x * rotation * delta)));
		camera.direction = Normalize(camera.direction);

		//Movment
		if (keystate.w)
			camera.position = VectorAdd(camera.position, VectorScale(camera.direction, speed * delta));
		if (keystate.s)
			camera.position = Subtract(camera.position, VectorScale(camera.direction, speed * delta));
		if (keystate.a)
			camera.position = VectorAdd(camera.position, VectorScale(CrossProduct(camera.direction, UP), speed * delta));
		if (keystate.d)
			camera.position = Subtract(camera.position, VectorScale(CrossProduct(camera.direction, UP), speed * delta));
		if (keystate.space)
			camera.position = VectorAdd(camera.position, VectorScale(UP, speed * delta));
		if (keystate.lshift)
			camera.position = Subtract(camera.position, VectorScale(UP, speed * delta));

		//Reset mouse state since we are only intrested in deltas.
		mousestate.x = mousestate.y = 0;
		_RPT1(0, "%.7f\n", delta);
	}
	camera.view = LookTo(camera.position, camera.direction, UP);
	// double d = DeltaTime(t1, GetCount());
	
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