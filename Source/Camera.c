#include <Camera.h>

static const hmm_vec3 UP = { 0.0f, 1.0f, 0.0f };
static const float rotation = 0.6f, speed = 6.0f;

struct Camera camera;

void UpdateCamera(float delta)
{
	if (mousestate.positionMode == MODE_RELATIVE)
	{
		//Orientation
		camera.direction = HMM_MultiplyMat4ByVec4(HMM_Rotate(mousestate.y * rotation, HMM_Cross(UP, camera.direction.XYZ)), camera.direction);
		camera.direction = HMM_MultiplyMat4ByVec4(HMM_Rotate(mousestate.x * rotation, UP), camera.direction);
		camera.direction = HMM_FastNormalizeVec4(camera.direction);

		//Movment
		if (keystate.w == KEY_DOWN)
			camera.position = HMM_AddVec4(camera.position, HMM_MultiplyVec4f(camera.direction, speed * delta));
		if (keystate.s == KEY_DOWN)
			camera.position = HMM_SubtractVec4(camera.position, HMM_MultiplyVec4f(camera.direction, speed * delta));
		if (keystate.a == KEY_DOWN)
			camera.position = HMM_AddVec4(camera.position, HMM_MultiplyVec4f(HMM_Vec4v(HMM_Cross(UP, camera.direction.XYZ), 1.0f), speed * delta));
		if (keystate.d == KEY_DOWN)
			camera.position = HMM_SubtractVec4(camera.position, HMM_MultiplyVec4f(HMM_Vec4v(HMM_Cross(UP, camera.direction.XYZ), 1.0f), speed * delta));
		if (keystate.space == KEY_DOWN)
			camera.position = HMM_AddVec4(camera.position, HMM_MultiplyVec4f(HMM_Vec4v(UP, 1.0f), speed * delta));
		if (keystate.shift == KEY_DOWN)
			camera.position = HMM_SubtractVec4(camera.position, HMM_MultiplyVec4f(HMM_Vec4v(UP, 1.0f), speed * delta));
	}
	camera.view = HMM_LookAt(camera.position.XYZ, HMM_AddVec3(camera.direction.XYZ, camera.position.XYZ), UP);
}

void InitializeCamera(void)
{
	camera.position = HMM_Vec4(0.0f, 0.0f, -5.0f, 0.0f);
	camera.focus = HMM_Vec4(0.0f, 0.0f, 0.0f, 1.0f);
	camera.direction = HMM_Vec4(0.0f, 0.0f, 1.0f, 0.0f);

	///NOTE: Transposing is required since the shaders handle matrices in column-major. It is now done in the shader.
	camera.view = LookTo(camera.position, camera.direction, UP);
	camera.projection = Persp(toRad(70.0f), 1424.f / 728.f, 0.1f, 100.0f);
}