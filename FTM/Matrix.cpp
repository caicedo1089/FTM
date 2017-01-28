#include "Matrix.h"

Matrix::Matrix(void)
{
}

Matrix Matrix::operator * (Matrix m)
{
	Matrix result;

	result.m11 = m11 * m.m11 + m12 * m.m21 + m13 * m.m31 +  m14 * m.m41;
	result.m12 = m11 * m.m12 + m12 * m.m22 + m13 * m.m32 +  m14 * m.m42;
	result.m13 = m11 * m.m13 + m12 * m.m23 + m13 * m.m33 +  m14 * m.m43;
	result.m14 = m11 * m.m14 + m12 * m.m24 + m13 * m.m34 +  m14 * m.m44;

	result.m21 = m21 * m.m11 + m22 * m.m21 + m23 * m.m31 +  m24 * m.m41;
	result.m22 = m21 * m.m12 + m22 * m.m22 + m23 * m.m32 +  m24 * m.m42;
	result.m23 = m21 * m.m13 + m22 * m.m23 + m23 * m.m33 +  m24 * m.m43;
	result.m24 = m21 * m.m14 + m12 * m.m24 + m23 * m.m34 +  m24 * m.m44;

	result.m31 = m31 * m.m11 + m32 * m.m21 + m33 * m.m31 +  m34 * m.m41;
	result.m32 = m31 * m.m12 + m32 * m.m22 + m33 * m.m32 +  m34 * m.m42;
	result.m33 = m31 * m.m13 + m32 * m.m23 + m33 * m.m33 +  m34 * m.m43;
	result.m34 = m31 * m.m14 + m32 * m.m24 + m33 * m.m34 +  m34 * m.m44;

	result.m41 = m41 * m.m11 + m42 * m.m21 + m43 * m.m31 +  m44 * m.m41;
	result.m42 = m41 * m.m12 + m42 * m.m22 + m43 * m.m32 +  m44 * m.m42;
	result.m43 = m41 * m.m13 + m42 * m.m23 + m43 * m.m33 +  m44 * m.m43;
	result.m44 = m41 * m.m14 + m42 * m.m24 + m43 * m.m34 +  m44 * m.m44;

	return result;
}

Matrix Matrix::CreateScale(float xScale, float yScale, float zScale)
{
	Matrix result;

	result.m11 = xScale;
	result.m12 = 0;
	result.m13 = 0;
	result.m14 = 0;

	result.m21 = 0;
	result.m22 = yScale;
	result.m23 = 0;
	result.m24 = 0;

	result.m31 = 0;
	result.m32 = 0;
	result.m33 = zScale;
	result.m34 = 0;

	result.m41 = 0;
	result.m42 = 0;
	result.m43 = 0;
	result.m44 = 1;

	return result;
}

Matrix Matrix::CreateTranslation(float xPosition, float yPosition, float zPosition)
{
	Matrix result;

	result.m11 = 1;
	result.m12 = 0;
	result.m13 = 0;
	result.m14 = xPosition;

	result.m21 = 0;
	result.m22 = 1;
	result.m23 = 0;
	result.m24 = yPosition;

	result.m31 = 0;
	result.m32 = 0;
	result.m33 = 1;
	result.m34 = zPosition;

	result.m41 = 0;
	result.m42 = 0;
	result.m43 = 0;
	result.m44 = 1;

	return result;
}

Matrix Matrix::CreateRotationX(float radians)
{
	Matrix result;

	result.m11 = 1;
	result.m12 = 0;
	result.m13 = 0;
	result.m14 = 0;

	result.m21 = 0;
	result.m22 = cosf(radians);
	result.m23 = -sinf(radians);
	result.m24 = 0;

	result.m31 = 0;
	result.m32 = sinf(radians);
	result.m33 = cosf(radians);
	result.m34 = 0;

	result.m41 = 0;
	result.m42 = 0;
	result.m43 = 0;
	result.m44 = 1;

	return result;
}

Matrix Matrix::CreateRotationY(float radians)
{
	Matrix result;

	result.m11 = cosf(radians);
	result.m12 = 0;
	result.m13 = -sinf(radians);
	result.m14 = 0;

	result.m21 = 0;
	result.m22 = 1;
	result.m23 = 0;
	result.m24 = 0;

	result.m31 = sinf(radians);
	result.m32 = 0;
	result.m33 = cosf(radians);
	result.m34 = 0;

	result.m41 = 0;
	result.m42 = 0;
	result.m43 = 0;
	result.m44 = 1;

	return result;
}

Matrix Matrix::CreateRotationZ(float radians)
{
	Matrix result;

	result.m11 = cosf(radians);
	result.m12 = -sinf(radians);
	result.m13 = 0;
	result.m14 = 0;

	result.m21 = sinf(radians);
	result.m22 = cosf(radians);
	result.m23 = 0;
	result.m24 = 0;

	result.m31 = 0;
	result.m32 = 0;
	result.m33 = 1;
	result.m34 = 0;

	result.m41 = 0;
	result.m42 = 0;
	result.m43 = 0;
	result.m44 = 1;

	return result;
}

Matrix Matrix::Identity()
{
	Matrix result;

	result.m11 = 1;
	result.m12 = 0;
	result.m13 = 0;
	result.m14 = 0;

	result.m21 = 0;
	result.m22 = 1;
	result.m23 = 0;
	result.m24 = 0;

	result.m31 = 0;
	result.m32 = 0;
	result.m33 = 1;
	result.m34 = 0;

	result.m41 = 0;
	result.m42 = 0;
	result.m43 = 0;
	result.m44 = 1;

	return result;
}

Matrix Matrix::CreatePerspective(float fov, float aspectratio, float nearclip, float farclip)
{
	Matrix result;

	float f = 1/tan(fov/2);

	result.m11 = f/aspectratio;
	result.m12 = 0;
	result.m13 = 0;
	result.m14 = 0;

	result.m21 = 0;
	result.m22 = f;
	result.m23 = 0;
	result.m24 = 0;

	result.m31 = 0;
	result.m32 = 0;
	result.m33 = (farclip+nearclip)/(nearclip-farclip);
	result.m34 = (2*farclip*nearclip)/(nearclip-farclip);

	result.m41 = 0;
	result.m42 = 0;
	result.m43 = -1;
	result.m44 = 0;

	return result;
}

Matrix Matrix::CreateOrthographic(float left, float right, float top, float bottom, float nearclip, float farclip)
{
	Matrix result;

	result.m11 = 2/(right-left);
	result.m12 = 0;
	result.m13 = 0;
	result.m14 = -((right+left)/(right-left));

	result.m21 = 0;
	result.m22 = 2/(top-bottom);
	result.m23 = 0;
	result.m24 = -((top+bottom)/(top-bottom));

	result.m31 = 0;
	result.m32 = 0;
	result.m33 = (2)/(farclip-nearclip);
	result.m34 = (farclip+nearclip)/(farclip-nearclip);
	

	result.m41 = 0;
	result.m42 = 0;
	result.m43 = 0;
	result.m44 = 1;

	return result;
}

Matrix Matrix::CreateViewPort(float x, float y, float width, float height)
{
	Matrix result;

	result.m11 = width/(2*height);
	result.m12 = 0;
	result.m13 = 0;
	result.m14 = width/2;

	result.m21 = 0;
	result.m22 = height/(2*width);
	result.m23 = 0;
	result.m24 = height/2;

	result.m31 = 0;
	result.m32 = 0;
	result.m33 = 1;
	result.m34 = 0;
	

	result.m41 = 0;
	result.m42 = 0;
	result.m43 = 0;
	result.m44 = 1;

	return result;
}

Matrix::~Matrix(void)
{
}
