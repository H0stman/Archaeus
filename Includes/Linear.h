/*
 *        __    _
 *       / /   (_)___  ___  ____ ______
 *      / /   / / __ \/ _ \/ __ `/ ___/
 *     / /___/ / / / /  __/ /_/ / /
 *    /_____/_/_/ /_/\___/\__,_/_/
 *
 */

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

#define LN_1DIV2PI 0.1591549f
#define LN_PI 3.1415927f
#define LN_2PI 6.2831853f
#define LN_PIDIV2 1.5707963f
#define LN_E 2.7182818f

#define toRad(Degrees) ((Degrees) * LN_PI / 180.0f)
#define toDeg(Radians) ((Radians) * 180.0f / LN_PI)

typedef __m128 Vec4;

typedef struct { Vec4 row[4]; } Mat4;

static const Vec4 Vec1DIV2PI = { LN_1DIV2PI, LN_1DIV2PI, LN_1DIV2PI, LN_1DIV2PI };
static const Vec4 Vec2PI = { LN_PI * 2.0f, LN_PI * 2.0f, LN_PI * 2.0f, LN_PI * 2.0f };

/// @brief Computes the dot product of two vectors.
/// @param a Vector a.
/// @param b Vector b.
/// @returns A float set to the result of the dot product.
float vectorcall Dot(const Vec4 a, const Vec4 b);

/// @brief Computes the cross product of two vectors.
/// @param a Vector a.
/// @param b Vector b.
/// @returns The resulting vector from the cross-product of vector a and b.
Vec4 vectorcall Cross(Vec4 a, Vec4 b);

/// @brief Computes the sum of two vectors.
/// @param a Vector a.
/// @param b Vector b.
/// @returns The resulting vector of the addition.
Vec4 vectorcall VecAdd(const Vec4 a, const Vec4 b);

/// @brief Scales a vector by a constant factor.
/// @param a Vector a.
/// @param factor The factor with which the vector is scaled by.
/// @returns The vector scaled by the factor.
Vec4 vectorcall VecScale(const Vec4 a, const float factor);

/// @brief Computes the product of two matrices.
/// @param a Matrix a.
/// @param b Matrix b.
/// @returns The product of matrix a and b.
Mat4 vectorcall Mul(const Mat4 a, const Mat4 b);

/// @brief Computes the Transpose of a matrix.
/// @param a Matrix a.
/// @returns The transpose of matrix a.
Mat4 vectorcall Transp(Mat4 a);

/// @brief Computes a look at matrix based on a position, a focus point and an up vector.
/// @param eve The position of the eyes or the camera.
/// @param focus The point of focus to which the eyes are directed at.
/// @param up A vector representing which way is up.
/// @returns The look at view matrix.
Mat4 vectorcall LookAt(Vec4 eye, Vec4 focus, Vec4 up);

/// @brief Computes a look to matrix based on a position, a direction and an up vector.
/// @param eve The position of the eyes or the camera.
/// @param direction The direction in which the camera will point.
/// @param up A vector representing which way is up.
/// @returns The look to view matrix.
Mat4 vectorcall LookTo(Vec4 eye, Vec4 direction, Vec4 up);

Mat4 Persp(float fov, float aspectratio, float nearplane, float farplane);

Mat4 RotX(float angle);

Mat4 RotY(float angle);

Mat4 RotZ(float angle);

Mat4 vectorcall RotNorm(Vec4 normaxis, float angle);

Mat4 vectorcall RotAxis(Vec4 axis, float angle);

Mat4 Identity(void);

Vec4 vectorcall VecNorm(Vec4 a);

Vec4 vectorcall VecNormEst(Vec4 a);

Vec4 vectorcall VecNeg(Vec4 a);

Vec4 vectorcall VecSelect(Vec4 a, Vec4 b, Vec4 control);

Vec4 VecSelectCtrl(unsigned int index0, unsigned int index1, unsigned int index2, unsigned int index3);

Vec4 vectorcall VecSub(Vec4 a, Vec4 b);

Vec4 vectorcall VecTransf(Vec4 a, Mat4 b);

Vec4 vectorcall VecMul(Vec4 a, Vec4 b);

Vec4 vectorcall VecModAng(Vec4 a);

Vec4 vectorcall VecRound(Vec4 a);

Vec4 VecSet(float x, float y, float z, float w);

Vec4 vectorcall VecSetX(Vec4 a, float x);

Vec4 vectorcall VecSetY(Vec4 a, float y);

Vec4 vectorcall VecSetZ(Vec4 a, float z);

Vec4 vectorcall VecSetW(Vec4 a, float w);

float vectorcall VecGetX(Vec4 a);

float vectorcall VecGetY(Vec4 a);

float vectorcall VecGetZ(Vec4 a);

float vectorcall VecGetW(Vec4 a);

void ScalarSinCos(float* sin, float* cos, float value);

inline float vectorcall Dot(const Vec4 a, const Vec4 b)
{
	float total;
	_mm_store_ps(&total, _mm_dp_ps(a, b, 0xff));
	return total;
}

inline Vec4 vectorcall Cross(Vec4 a, Vec4 b)
{
	Vec4 temp1 = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1));
	Vec4 temp2 = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 1, 0, 2));
	Vec4 result = _mm_mul_ps(temp1, temp2);
	temp1 = _mm_shuffle_ps(temp1, temp1, _MM_SHUFFLE(3, 0, 2, 1));
	temp2 = _mm_shuffle_ps(temp2, temp2, _MM_SHUFFLE(3, 1, 0, 2));
	return _mm_sub_ps((result), _mm_mul_ps((temp1), (temp2)));
}

inline Vec4 vectorcall VecAdd(const Vec4 a, const Vec4 b)
{
	return _mm_add_ps(a, b);
}

inline Vec4 vectorcall VecScale(const Vec4 a, const float factor)
{
	__m128 result = _mm_set_ps1(factor);
	return _mm_mul_ps(result, a);
}

inline Mat4 vectorcall Mul(const Mat4 a, const Mat4 b)
{
	Mat4 result;

	//Splat the component X,Y,Z then W
	// Use vW to hold the original row
	Vec4 vW = a.row[0];
	Vec4 vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
	Vec4 vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
	Vec4 vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
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

inline Mat4 vectorcall Transp(Mat4 a)
{
	_MM_TRANSPOSE4_PS(a.row[0], a.row[1], a.row[2], a.row[3]);
	return a;
}

inline Mat4 vectorcall LookAt(Vec4 eye, Vec4 focus, Vec4 up)
{
	Vec4 NegEyeDirection = VecSub(focus, eye);
	return LookTo(eye, NegEyeDirection, up);
}

inline Mat4 vectorcall LookTo(Vec4 eye, Vec4 direction, Vec4 up)
{
	Vec4 r2 = VecNorm(direction);

	Vec4 r0 = Cross(up, r2);
	r0 = VecNorm(r0);

	Vec4 r1 = Cross(r2, r0);

	Vec4 NegEyePosition = VecNeg(eye);

	Vec4 d0 = _mm_dp_ps(r0, NegEyePosition, 0x7f);
	Vec4 d1 = _mm_dp_ps(r1, NegEyePosition, 0x7f);
	Vec4 d2 = _mm_dp_ps(r2, NegEyePosition, 0x7f);

	Vec4 control = VecSelectCtrl(1, 1, 1, 0);
	Vec4 xyzW = { 0.0f, 0.0f, 0.0f, 1.0f };

	Mat4 m;

	m.row[0] = VecSelect(d0, r0, control);
	m.row[1] = VecSelect(d1, r1, control);
	m.row[2] = VecSelect(d2, r2, control);
	m.row[3] = xyzW;

	m = Transp(m);

	return m;
}

inline Mat4 Persp(float fov, float aspectratio, float nearplane, float farplane)
{
	float sinfov;
	float cosfov;
	ScalarSinCos(&sinfov, &cosfov, 0.5f * fov);

	float range = farplane / (farplane - nearplane);

	float height = cosfov / sinfov;
	Vec4 rMem = {
		 height / aspectratio,
		 height,
		 range,
		 -range * nearplane };

	Vec4 values = rMem;
	Vec4 temp = _mm_setzero_ps();

	temp = _mm_move_ss(temp, values);

	Mat4 m;
	m.row[0] = temp;

	temp = values;
	const Vec4 MaskY = VecSelectCtrl(0, 1, 0, 0);
	temp = _mm_and_ps(temp, MaskY);
	m.row[1] = temp;
	temp = _mm_setzero_ps();

	const Vec4 IdentityR3 = { 0.0f, 0.0f, 0.0f, 1.0f };
	values = _mm_shuffle_ps(values, IdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
	temp = _mm_shuffle_ps(temp, values, _MM_SHUFFLE(3, 0, 0, 0));
	m.row[2] = temp;
	temp = _mm_shuffle_ps(temp, values, _MM_SHUFFLE(2, 1, 0, 0));
	m.row[3] = temp;
	return m;
}

inline Mat4 RotX(float angle)
{
	float SinAngle;
	float CosAngle;
	ScalarSinCos(&SinAngle, &CosAngle, angle);

	__m128 vSin = _mm_set_ss(SinAngle);
	__m128 vCos = _mm_set_ss(CosAngle);
	// x = 0,y = cos,z = sin, w = 0
	vCos = _mm_shuffle_ps(vCos, vSin, _MM_SHUFFLE(3, 0, 0, 3));
	Mat4 m;
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

inline Mat4 RotY(float angle)
{
	float SinAngle;
	float CosAngle;
	ScalarSinCos(&SinAngle, &CosAngle, angle);
	__m128 vSin = _mm_set_ss(SinAngle);
	__m128 vCos = _mm_set_ss(CosAngle);
	// x = sin,y = 0,z = cos, w = 0
	vSin = _mm_shuffle_ps(vSin, vCos, _MM_SHUFFLE(3, 0, 3, 0));
	Mat4 m;
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

inline Mat4 RotZ(float angle)
{
	float SinAngle;
	float CosAngle;
	ScalarSinCos(&SinAngle, &CosAngle, angle);

	__m128 vSin = _mm_set_ss(SinAngle);
	__m128 vCos = _mm_set_ss(CosAngle);
	// x = cos,y = sin,z = 0, w = 0
	vCos = _mm_unpacklo_ps(vCos, vSin);
	Mat4 m;
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

inline Mat4 vectorcall RotNorm(Vec4 normaxis, float angle)
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

	__m128 mask = VecSelectCtrl(1, 1, 1, 0);
	v0 = _mm_and_ps(r0, mask);
	__m128 v1 = _mm_shuffle_ps(r1, r2, _MM_SHUFFLE(2, 1, 2, 0));
	v1 = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(0, 3, 2, 1));
	__m128 v2 = _mm_shuffle_ps(r1, r2, _MM_SHUFFLE(0, 0, 1, 1));
	v2 = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(2, 0, 2, 0));

	r2 = _mm_shuffle_ps(v0, v1, _MM_SHUFFLE(1, 0, 3, 0));
	r2 = _mm_shuffle_ps(r2, r2, _MM_SHUFFLE(1, 3, 2, 0));

	Mat4 m;
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

inline Mat4 vectorcall RotAxis(Vec4 axis, float angle)
{
	Vec4 Normal = VecNorm(axis);
	return RotNorm(Normal, angle);
}

inline Mat4 Identity(void)
{
	Mat4 m = { 1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f };
	return m;
}

inline Vec4 vectorcall VecNorm(Vec4 a)
{
	Vec4 lenghtsq = _mm_dp_ps(a, a, 0xff);
	// Prepare for the division
	Vec4 result = _mm_sqrt_ps(lenghtsq);
	// Create zero with a single instruction
	Vec4 zeromask = _mm_setzero_ps();
	// Test for a divide by zero (Must be FP to detect -0.0)
	zeromask = _mm_cmpneq_ps(zeromask, result);
	// Failsafe on zero (Or epsilon) length planes
	// If the length is infinity, set the elements to zero
	Vec4 inf = { 0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000 };
	lenghtsq = _mm_cmpneq_ps(lenghtsq, inf);
	// Divide to perform the normalization
	result = _mm_div_ps(a, result);
	// Any that are infinity, set to zero
	result = _mm_and_ps(result, zeromask);
	// Select qnan or result based on infinite length
	Vec4 nan = { 0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000 };
	Vec4 temp1 = _mm_andnot_ps(lenghtsq, nan);
	Vec4 temp2 = _mm_and_ps(result, lenghtsq);
	result = _mm_or_ps(temp1, temp2);
	return result;
}

inline Vec4 vectorcall VecNormEst(Vec4 a)
{
	__m128 temp = _mm_dp_ps(a, a, 0xff);
	__m128 result = _mm_rsqrt_ps(temp);
	return _mm_mul_ps(result, a);
}

inline Vec4 vectorcall VecNeg(Vec4 a)
{
	Vec4 z;
	z = _mm_setzero_ps();
	return _mm_sub_ps(z, a);
}

inline Vec4 vectorcall VecSelect(Vec4 a, Vec4 b, Vec4 control)
{
	Vec4 temp1 = _mm_andnot_ps(control, a);
	Vec4 temp2 = _mm_and_ps(b, control);
	return _mm_or_ps(temp1, temp2);
}

inline Vec4 VecSelectCtrl(unsigned int index0, unsigned int index1, unsigned int index2, unsigned int index3)
{
	// x=Index0,y=Index1,z=Index2,w=Index3
	__m128i temp = _mm_set_epi32((int)index3, (int)index2, (int)index1, (int)index0);
	// Any non-zero entries become 0xFFFFFFFF else 0
	__m128i zero = { 0.0f, 0.0f, 0.0f, 0.0f };
	temp = _mm_cmpgt_epi32(temp, zero);
	return _mm_castsi128_ps(temp);
}

inline Vec4 vectorcall VecSub(Vec4 a, Vec4 b)
{
	return _mm_sub_ps(a, b);
}

inline Vec4 vectorcall VecTransf(Vec4 a, Mat4 b)
{
	Vec4 vResult = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 3, 3, 3)); // W
	vResult = _mm_mul_ps(vResult, b.row[3]);
	Vec4 vTemp = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 2, 2, 2)); // Z
	vResult = _mm_add_ps(_mm_mul_ps(vTemp, b.row[2]), vResult);
	vTemp = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 1, 1, 1)); // Y
	vResult = _mm_add_ps(_mm_mul_ps(vTemp, b.row[1]), vResult);
	vTemp = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 0)); // X
	vResult = _mm_add_ps(_mm_mul_ps(vTemp, b.row[0]), vResult);
	return vResult;
}

inline Vec4 vectorcall VecMul(Vec4 a, Vec4 b)
{
	return _mm_mul_ps(a, b);
}

inline Vec4 vectorcall VecModAng(Vec4 a)
{
	// Modulo the range of the given angles such that -XM_PI <= Angles < XM_PI
	Vec4 result = _mm_mul_ps(a, Vec1DIV2PI);
	// Use the inline function due to complexity for rounding
	result = VecRound(result);
	return _mm_add_ps(_mm_mul_ps(result, Vec2PI), a);
}

inline Vec4 vectorcall VecRound(Vec4 a)
{
	return _mm_round_ps(a, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
}

inline Vec4 VecSet(float x, float y, float z, float w)
{
	return _mm_set_ps(w, z, y, x);
}

inline Vec4 vectorcall VecSetX(Vec4 a, float x)
{
	Vec4 result = _mm_set_ss(x);
	result = _mm_move_ss(a, result);
	return result;
}

inline Vec4 vectorcall VecSetY(Vec4 a, float y)
{
	Vec4 result = _mm_set_ss(y);
	result = _mm_insert_ps(a, result, 0x10);
	return result;
}

inline Vec4 vectorcall VecSetZ(Vec4 a, float z)
{
	Vec4 result = _mm_set_ss(z);
	result = _mm_insert_ps(a, result, 0x20);
	return result;
}

inline Vec4 vectorcall VecSetW(Vec4 a, float w)
{
	Vec4 result = _mm_set_ss(w);
	result = _mm_insert_ps(a, result, 0x20);
	return result;
}

inline float vectorcall VecGetX(Vec4 a)
{
	return _mm_cvtss_f32(a);
}

inline float vectorcall VecGetY(Vec4 a)
{
	__m128 temp = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 1, 1, 1));
	return _mm_cvtss_f32(temp);
}

inline float vectorcall VecGetZ(Vec4 a)
{
	__m128 temp = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 2, 2, 2));
	return _mm_cvtss_f32(temp);
}

inline float vectorcall VecGetW(Vec4 a)
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

	float y = value - 2 * LN_PI * quotient;

	// Map y to [-pi/2,pi/2] with sin(y) = sin(value).
	float sign;
	if (y > LN_PIDIV2)
	{
		y = LN_PI - y;
		sign = -1.0f;
	}
	else if (y < -LN_PIDIV2)
	{
		y = -LN_PI - y;
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