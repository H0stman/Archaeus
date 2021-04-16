/*
 *        __    _
 *       / /   (_)___  ___  ____ ______
 *      / /   / / __ \/ _ \/ __ `/ ___/
 *     / /___/ / / / /  __/ /_/ / /
 *    /_____/_/_/ /_/\___/\__,_/_/
 *
 */

///TODO: Make cross platform.
///TODO: Refactoring.
///TODO: Add comments and cleanup.

#pragma once
#define _USE_MATH_DEFINES
#pragma warning(push, 0)
#include <immintrin.h>
#include <math.h>
#include <assert.h>
#pragma warning(pop);

#define vectorcall __vectorcall

#define LN_1DIV2PI 0.159154943f
#define LN_PI 3.141592654f

#define toRad(Degrees) ((Degrees) * LN_PI / 180.0f)
#define toDeg(Radians) ((Radians) * 180.0f / LN_PI)

typedef __m128 Vector4;

typedef struct
{
	Vector4 row[4];
} Matrix4;

static const Vector4 Vec1DIV2PI = { LN_1DIV2PI, LN_1DIV2PI, LN_1DIV2PI, LN_1DIV2PI };
static const Vector4 Vec2PI = { (float)M_PI * 2.0f, (float)M_PI * 2.0f, (float)M_PI * 2.0f, (float)M_PI * 2.0f };

/// @brief Computes the dot product of two vectors.
/// @param a Vector a.
/// @param b Vector b.
/// @returns A float set to the result of the dot product.
float vectorcall DotProduct(const Vector4 a, const Vector4 b);

/// @brief Computes the cross product of two vectors.
/// @param a Vector a.
/// @param b Vector b.
/// @returns The resulting vector from the cross-product of vector a and b.
Vector4 vectorcall CrossProduct(Vector4 a, Vector4 b);

/// @brief Computes the sum of two vectors.
/// @param a Vector a.
/// @param b Vector b.
/// @returns The resulting vector of the addition.
Vector4 vectorcall VectorAdd(const Vector4 a, const Vector4 b);

Vector4 vectorcall VectorScale(const Vector4 a, const float factor);

/// @brief Computes the product of two matrices.
/// @param a Matrix a.
/// @param b Matrix b.
/// @returns The product of the two matrices.
Matrix4 vectorcall Multiply(const Matrix4 a, const Matrix4 b);

/// @brief Transposes a matrix.
/// @param a Matrix a.
/// @returns The transpose of matrix a.
Matrix4 vectorcall Transpose(Matrix4 a);

/// @brief Computes a look at matrix based on a position, a focus point and an up vector.
/// @param eve The position of the eyes or the camera.
/// @param focus The point of focus to which the eyes are directed at.
/// @param up A vector representing which way is up.
/// @returns The look at view matrix.
Matrix4 vectorcall LookAt(Vector4 eye, Vector4 focus, Vector4 up);

/// @brief Computes a look to matrix based on a position, a direction and an up vector.
/// @param eve The position of the eyes or the camera.
/// @param direction The direction in which the camera will point.
/// @param up A vector representing which way is up.
/// @returns The look to view matrix.
Matrix4 vectorcall LookTo(Vector4 eye, Vector4 direction, Vector4 up);

Matrix4 Perspective(float fov, float aspectratio, float nearplane, float farplane);

Matrix4 RotationX(float angle);

Matrix4 RotationY(float angle);

Matrix4 RotationZ(float angle);

Matrix4 vectorcall RotationNormal(Vector4 normaxis, float angle);

Matrix4 vectorcall RotationAxis(Vector4 axis, float angle);

Matrix4 Identity(void);

Vector4 vectorcall Normalize(Vector4 a);

Vector4 vectorcall NormalizeEst(Vector4 a);

Vector4 vectorcall Negate(Vector4 a);

Vector4 vectorcall VectorSelect(Vector4 a, Vector4 b, Vector4 control);

Vector4 VectorSelectControl(unsigned int index0, unsigned int index1, unsigned int index2, unsigned int index3);

Vector4 vectorcall Subtract(Vector4 a, Vector4 b);

Vector4 vectorcall VectorTransform(Vector4 a, Matrix4 b);

Vector4 vectorcall VectorMultiply(Vector4 a, Vector4 b);

Vector4 vectorcall VectorModAngles(Vector4 a);

Vector4 vectorcall VectorRound(Vector4 a);

Vector4 VectorSet(float x, float y, float z, float w);

Vector4 vectorcall VectorSetX(Vector4 a, float x);

Vector4 vectorcall VectorSetY(Vector4 a, float y);

Vector4 vectorcall VectorSetZ(Vector4 a, float z);

Vector4 vectorcall VectorSetW(Vector4 a, float w);

float vectorcall VectorGetX(Vector4 a);

float vectorcall VectorGetY(Vector4 a);

float vectorcall VectorGetZ(Vector4 a);

float vectorcall VectorGetW(Vector4 a);

void ScalarSinCos(float* sin, float* cos, float value);

inline float vectorcall DotProduct(const Vector4 a, const Vector4 b)
{
	float total;
	_mm_store_ps(&total, _mm_dp_ps(a, b, 0xff));
	return total;
}

inline Vector4 vectorcall CrossProduct(Vector4 a, Vector4 b)
{
	Vector4 temp1 = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1));
	Vector4 temp2 = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 1, 0, 2));
	Vector4 result = _mm_mul_ps(temp1, temp2);
	temp1 = _mm_shuffle_ps(temp1, temp1, _MM_SHUFFLE(3, 0, 2, 1));
	temp2 = _mm_shuffle_ps(temp2, temp2, _MM_SHUFFLE(3, 1, 0, 2));
	return _mm_sub_ps((result), _mm_mul_ps((temp1), (temp2)));
}

inline Vector4 vectorcall VectorAdd(const Vector4 a, const Vector4 b)
{
	return _mm_add_ps(a, b);
}

inline Vector4 vectorcall VectorScale(const Vector4 a, const float factor)
{
	__m128 result = _mm_set_ps1(factor);
	return _mm_mul_ps(result, a);
}

inline Matrix4 vectorcall Multiply(const Matrix4 a, const Matrix4 b)
{
	Matrix4 result;
	//Splat the component X,Y,Z then W

	// Use vW to hold the original row
	Vector4 vW = a.row[0];
	Vector4 vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
	Vector4 vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
	Vector4 vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
	vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));

	// Perform the operation on the first row
	vX = _mm_mul_ps(vX, b.row[0]);
	vY = _mm_mul_ps(vY, b.row[1]);
	vZ = _mm_mul_ps(vZ, b.row[2]);
	vW = _mm_mul_ps(vW, b.row[3]);
	// Perform a binary add to reduce cumulative errors
	vX = _mm_add_ps(vX, vZ);
	vY = _mm_add_ps(vY, vW);
	vX = _mm_add_ps(vX, vY);
	result.row[0] = vX;

	// Repeat for the other 3 rows
	vW = a.row[1];
	vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
	vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
	vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
	vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));

	vX = _mm_mul_ps(vX, b.row[0]);
	vY = _mm_mul_ps(vY, b.row[1]);
	vZ = _mm_mul_ps(vZ, b.row[2]);
	vW = _mm_mul_ps(vW, b.row[3]);
	vX = _mm_add_ps(vX, vZ);
	vY = _mm_add_ps(vY, vW);
	vX = _mm_add_ps(vX, vY);
	result.row[1] = vX;

	vW = a.row[2];
	vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
	vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
	vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
	vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));

	vX = _mm_mul_ps(vX, b.row[0]);
	vY = _mm_mul_ps(vY, b.row[1]);
	vZ = _mm_mul_ps(vZ, b.row[2]);
	vW = _mm_mul_ps(vW, b.row[3]);
	vX = _mm_add_ps(vX, vZ);
	vY = _mm_add_ps(vY, vW);
	vX = _mm_add_ps(vX, vY);
	result.row[2] = vX;

	vW = a.row[3];
	vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
	vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
	vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
	vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));

	vX = _mm_mul_ps(vX, b.row[0]);
	vY = _mm_mul_ps(vY, b.row[1]);
	vZ = _mm_mul_ps(vZ, b.row[2]);
	vW = _mm_mul_ps(vW, b.row[3]);
	vX = _mm_add_ps(vX, vZ);
	vY = _mm_add_ps(vY, vW);
	vX = _mm_add_ps(vX, vY);
	result.row[3] = vX;

	return result;
}

inline Matrix4 vectorcall Transpose(Matrix4 a)
{
	_MM_TRANSPOSE4_PS(a.row[0], a.row[1], a.row[2], a.row[3]);
	return a;
}

inline Matrix4 vectorcall LookAt(Vector4 eye, Vector4 focus, Vector4 up)
{
	Vector4 NegEyeDirection = Subtract(focus, eye);
	return LookTo(eye, NegEyeDirection, up);
}

inline Matrix4 vectorcall LookTo(Vector4 eye, Vector4 direction, Vector4 up)
{
	Vector4 r2 = Normalize(direction);

	Vector4 r0 = CrossProduct(up, r2);
	r0 = Normalize(r0);

	Vector4 r1 = CrossProduct(r2, r0);

	Vector4 NegEyePosition = Negate(eye);

	Vector4 d0 = _mm_dp_ps(r0, NegEyePosition, 0x7f);
	Vector4 d1 = _mm_dp_ps(r1, NegEyePosition, 0x7f);
	Vector4 d2 = _mm_dp_ps(r2, NegEyePosition, 0x7f);

	Vector4 control = VectorSelectControl(1,1,1,0);
	Vector4 xyzW = { 0.0f, 0.0f, 0.0f, 1.0f };

	Matrix4 m;
	///FIXME: Vector select is not working properly. 
	m.row[0] = VectorSelect(d0, r0, control);
	m.row[1] = VectorSelect(d1, r1, control);
	m.row[2] = VectorSelect(d2, r2, control);
	m.row[3] = xyzW;

	m = Transpose(m);

	return m;
}

inline Matrix4 Perspective(float fov, float aspectratio, float nearplane, float farplane)
{
	float sinfov;
	float cosfov;
	ScalarSinCos(&sinfov, &cosfov, 0.5f * fov);

	float range = farplane / (farplane - nearplane);

	float height = cosfov / sinfov;
	Vector4 rMem = {
		 height / aspectratio,
		 height,
		 range,
		 -range * nearplane };

	Vector4 values = rMem;
	Vector4 temp = _mm_setzero_ps();

	temp = _mm_move_ss(temp, values);

	Matrix4 m;
	m.row[0] = temp;

	temp = values;
	const Vector4 MaskY = VectorSelectControl(0,1,0,0);
	temp = _mm_and_ps(temp, MaskY);
	m.row[1] = temp;
	temp = _mm_setzero_ps();

	const Vector4 IdentityR3 = { 0.0f, 0.0f, 0.0f, 1.0f };
	values = _mm_shuffle_ps(values, IdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
	temp = _mm_shuffle_ps(temp, values, _MM_SHUFFLE(3, 0, 0, 0));
	m.row[2] = temp;
	temp = _mm_shuffle_ps(temp, values, _MM_SHUFFLE(2, 1, 0, 0));
	m.row[3] = temp;
	return m;
}

inline Matrix4 RotationX(float angle)
{
	float SinAngle;
	float CosAngle;
	ScalarSinCos(&SinAngle, &CosAngle, angle);

	__m128 vSin = _mm_set_ss(SinAngle);
	__m128 vCos = _mm_set_ss(CosAngle);
	// x = 0,y = cos,z = sin, w = 0
	vCos = _mm_shuffle_ps(vCos, vSin, _MM_SHUFFLE(3, 0, 0, 3));
	Matrix4 m;
	__m128 temp = { 1.0f, 0.0f, 0.0f, 0.0f };
	m.row[0] = temp;
	m.row[1] = vCos;
	// x = 0,y = sin,z = cos, w = 0
	vCos = _mm_shuffle_ps(vCos, vCos, _MM_SHUFFLE(3, 1, 2, 0));
	// x = 0,y = -sin,z = cos, w = 0
	__m128 negY = { 1.0f, -1.0f, 1.0f, 1.0f };
	vCos = _mm_mul_ps(vCos, negY);
	m.row[2] = vCos;
	__m128 idenR3 = { 0.0f, 0.0f, 0.0f, 1.0f };
	m.row[3] = idenR3;
	return m;
}

inline Matrix4 RotationY(float angle)
{
	float SinAngle;
	float CosAngle;
	ScalarSinCos(&SinAngle, &CosAngle, angle);
	__m128 vSin = _mm_set_ss(SinAngle);
	__m128 vCos = _mm_set_ss(CosAngle);
	// x = sin,y = 0,z = cos, w = 0
	vSin = _mm_shuffle_ps(vSin, vCos, _MM_SHUFFLE(3, 0, 3, 0));
	Matrix4 m;
	m.row[2] = vSin;
	__m128 temp = { 0.0f, 1.0f, 0.0f, 0.0f };
	m.row[1] = temp;
	// x = cos,y = 0,z = sin, w = 0
	vSin = _mm_shuffle_ps(vSin, vSin, _MM_SHUFFLE(3, 0, 1, 2));
	// x = cos,y = 0,z = -sin, w = 0
	__m128 temp2 = { 1.0f, 1.0f, -1.0f, 1.0f };
	vSin = _mm_mul_ps(vSin, temp2);
	m.row[0] = vSin;
	__m128 temp3 = { 0.0f, 0.0f, 0.0f, 1.0f };
	m.row[3] = temp3;
	return m;
}

inline Matrix4 RotationZ(float angle)
{
	float SinAngle;
	float CosAngle;
	ScalarSinCos(&SinAngle, &CosAngle, angle);

	__m128 vSin = _mm_set_ss(SinAngle);
	__m128 vCos = _mm_set_ss(CosAngle);
	// x = cos,y = sin,z = 0, w = 0
	vCos = _mm_unpacklo_ps(vCos, vSin);
	Matrix4 m;
	m.row[0] = vCos;
	// x = sin,y = cos,z = 0, w = 0
	vCos = _mm_shuffle_ps(vCos, vCos, _MM_SHUFFLE(3, 2, 0, 1));
	// x = cos,y = -sin,z = 0, w = 0
	__m128 negX = { -1.0f, 1.0f, 1.0f, 1.0f };
	vCos = _mm_mul_ps(vCos, negX);
	m.row[1] = vCos;
	__m128 idenR2 = { 0.0f, 0.0f, 1.0f, 0.0f };
	m.row[2] = idenR2;
	__m128 idenR3 = { 0.0f, 0.0f, 0.0f, 1.0f };
	m.row[3] = idenR3;
	return m;
}

inline Matrix4 vectorcall RotationNormal(Vector4 normaxis, float angle)
{
	float fSinAngle;
	float fCosAngle;
	ScalarSinCos(&fSinAngle, &fCosAngle, angle);

	__m128 c2 = _mm_set_ps1(1.0f - fCosAngle);
	__m128 c1 = _mm_set_ps1(fCosAngle);
	__m128 c0 = _mm_set_ps1(fSinAngle);

	__m128 n0 = _mm_shuffle_ps(normaxis, normaxis, _MM_SHUFFLE(3, 0, 2, 1));
	__m128 n1 = _mm_shuffle_ps(normaxis, normaxis, _MM_SHUFFLE(3, 1, 0, 2));

	__m128 v0 = _mm_mul_ps(c2, n0);
	v0 = _mm_mul_ps(v0, n1);

	__m128 r0 = _mm_mul_ps(c2, normaxis);
	r0 = _mm_mul_ps(r0, normaxis);
	r0 = _mm_add_ps(r0, c1);

	__m128 r1 = _mm_mul_ps(c0, normaxis);
	r1 = _mm_add_ps(r1, v0);
	__m128 r2 = _mm_mul_ps(c0, normaxis);
	r2 = _mm_sub_ps(v0, r2);

	__m128 mask = VectorSelectControl(1,1,1,0);
	v0 = _mm_and_ps(r0, mask);
	__m128 v1 = _mm_shuffle_ps(r1, r2, _MM_SHUFFLE(2, 1, 2, 0));
	v1 = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(0, 3, 2, 1));
	__m128 v2 = _mm_shuffle_ps(r1, r2, _MM_SHUFFLE(0, 0, 1, 1));
	v2 = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(2, 0, 2, 0));

	r2 = _mm_shuffle_ps(v0, v1, _MM_SHUFFLE(1, 0, 3, 0));
	r2 = _mm_shuffle_ps(r2, r2, _MM_SHUFFLE(1, 3, 2, 0));

	Matrix4 m;
	m.row[0] = r2;

	r2 = _mm_shuffle_ps(v0, v1, _MM_SHUFFLE(3, 2, 3, 1));
	r2 = _mm_shuffle_ps(r2, r2, _MM_SHUFFLE(1, 3, 0, 2));
	m.row[1] = r2;

	v2 = _mm_shuffle_ps(v2, v0, _MM_SHUFFLE(3, 2, 1, 0));
	m.row[2] = v2;
	__m128 idenR3 = { 0.0f, 0.0f, 0.0f, 1.0f };
	m.row[3] = idenR3;
	return m;
}

inline Matrix4 vectorcall RotationAxis(Vector4 axis, float angle)
{
	Vector4 Normal = Normalize(axis);
	return RotationNormal(Normal, angle);
}

inline Matrix4 Identity(void)
{
	Matrix4 m = { 1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f };
	return m;
}

inline Vector4 vectorcall Normalize(Vector4 a)
{
	Vector4 lenghtsq = _mm_dp_ps(a, a, 0xff);
	// Prepare for the division
	Vector4 result = _mm_sqrt_ps(lenghtsq);
	// Create zero with a single instruction
	Vector4 zeromask = _mm_setzero_ps();
	// Test for a divide by zero (Must be FP to detect -0.0)
	zeromask = _mm_cmpneq_ps(zeromask, result);
	// Failsafe on zero (Or epsilon) length planes
	// If the length is infinity, set the elements to zero
	Vector4 inf = { 0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000 };
	lenghtsq = _mm_cmpneq_ps(lenghtsq, inf);
	// Divide to perform the normalization
	result = _mm_div_ps(a, result);
	// Any that are infinity, set to zero
	result = _mm_and_ps(result, zeromask);
	// Select qnan or result based on infinite length
	Vector4 nan = { 0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000 };
	Vector4 temp1 = _mm_andnot_ps(lenghtsq, nan);
	Vector4 temp2 = _mm_and_ps(result, lenghtsq);
	result = _mm_or_ps(temp1, temp2);
	return result;
}

inline Vector4 vectorcall NormalizeEst(Vector4 a)
{
	__m128 temp = _mm_dp_ps(a, a, 0xff);
	__m128 result = _mm_rsqrt_ps(temp);
	return _mm_mul_ps(result, a);
}

inline Vector4 vectorcall Negate(Vector4 a)
{
	Vector4 z;
	z = _mm_setzero_ps();
	return _mm_sub_ps(z, a);
}

inline Vector4 vectorcall VectorSelect(Vector4 a, Vector4 b, Vector4 control)
{
	Vector4 temp1 = _mm_andnot_ps(control, a);
	Vector4 temp2 = _mm_and_ps(b, control);
	return _mm_or_ps(temp1, temp2);
}

inline Vector4 VectorSelectControl(unsigned int index0, unsigned int index1, unsigned int index2, unsigned int index3)
{
	// x=Index0,y=Index1,z=Index2,w=Index3
	__m128i temp = _mm_set_epi32((int)index3, (int)index2, (int)index1, (int)index0);
	// Any non-zero entries become 0xFFFFFFFF else 0
	__m128i zero = { 0.0f, 0.0f, 0.0f, 0.0f };
	temp = _mm_cmpgt_epi32(temp, zero);
	return _mm_castsi128_ps(temp);
}

inline Vector4 vectorcall Subtract(Vector4 a, Vector4 b)
{
	return _mm_sub_ps(a, b);
}

inline Vector4 vectorcall VectorTransform(Vector4 a, Matrix4 b)
{
	Vector4 vResult = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 3, 3, 3)); // W
	vResult = _mm_mul_ps(vResult, b.row[3]);
	Vector4 vTemp = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 2, 2, 2)); // Z
	vResult = _mm_add_ps(_mm_mul_ps(vTemp, b.row[2]), vResult);
	vTemp = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 1, 1, 1)); // Y
	vResult = _mm_add_ps(_mm_mul_ps(vTemp, b.row[1]), vResult);
	vTemp = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 0)); // X
	vResult = _mm_add_ps(_mm_mul_ps(vTemp, b.row[0]), vResult);
	return vResult;
}

inline Vector4 vectorcall VectorMultiply(Vector4 a, Vector4 b)
{
	return _mm_mul_ps(a, b);
}

inline Vector4 vectorcall VectorModAngles(Vector4 a)
{
	// Modulo the range of the given angles such that -XM_PI <= Angles < XM_PI
	Vector4 result = _mm_mul_ps(a, Vec1DIV2PI);
	// Use the inline function due to complexity for rounding
	result = VectorRound(result);
	return _mm_add_ps(_mm_mul_ps(result, Vec2PI), a);
}

inline Vector4 vectorcall VectorRound(Vector4 a)
{
	return _mm_round_ps(a, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
}

inline Vector4 VectorSet(float x, float y, float z, float w)
{
	return _mm_set_ps(w, z, y, x);
}

inline Vector4 vectorcall VectorSetX(Vector4 a, float x)
{
	Vector4 result = _mm_set_ss(x);
	result = _mm_move_ss(a, result);
	return result;
}

inline Vector4 vectorcall VectorSetY(Vector4 a, float y)
{
	Vector4 result = _mm_set_ss(y);
	result = _mm_insert_ps(a, result, 0x10);
	return result;
}

inline Vector4 vectorcall VectorSetZ(Vector4 a, float z)
{
	Vector4 result = _mm_set_ss(z);
	result = _mm_insert_ps(a, result, 0x20);
	return result;
}

inline Vector4 vectorcall VectorSetW(Vector4 a, float w)
{
	Vector4 result = _mm_set_ss(w);
	result = _mm_insert_ps(a, result, 0x20);
	return result;
}

inline float vectorcall VectorGetX(Vector4 a)
{
	return _mm_cvtss_f32(a);
}

inline float vectorcall VectorGetY(Vector4 a)
{
	__m128 temp = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 1, 1, 1));
	return _mm_cvtss_f32(temp);
}

inline float vectorcall VectorGetZ(Vector4 a)
{
	__m128 temp = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 2, 2, 2));
	return _mm_cvtss_f32(temp);
}

inline float vectorcall VectorGetW(Vector4 a)
{
	__m128 temp = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 3, 3, 3));
	return _mm_cvtss_f32(temp);
}

inline void ScalarSinCos(float* sin, float* cos, float value)
{
	assert(sin);
	assert(cos);

	// Map value to y in [-pi,pi], x = 2*pi*quotient + remainder.
	float quotient = LN_1DIV2PI * value;
	if (value >= 0.0f)
		quotient = (int)(quotient + 0.5f);
	else
		quotient = (int)(quotient - 0.5f);

	float y = value - 2 * (float)M_PI * quotient;

	// Map y to [-pi/2,pi/2] with sin(y) = sin(value).
	float sign;
	if (y > M_PI_2)
	{
		y = (float)M_PI - y;
		sign = -1.0f;
	}
	else if (y < -M_PI_2)
	{
		y = (float)-M_PI - y;
		sign = -1.0f;
	}
	else
		sign = +1.0f;

	float y2 = y * y;

	// 11-degree minimax approximation
	*sin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

	// 10-degree minimax approximation
	float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
	*cos = sign * p;
}