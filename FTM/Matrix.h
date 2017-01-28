#pragma once

#include <math.h>

class Matrix
{
	public:
		Matrix(void);
		
		float m11;
		float m12;
		float m13;
		float m14;

		float m21;
		float m22;
		float m23;
		float m24;

		float m31;
		float m32;
		float m33;
		float m34;

		float m41;
		float m42;
		float m43;
		float m44;

		Matrix operator * (Matrix m);

		static Matrix CreateScale(float xScale, float yScale, float zScale);
		static Matrix CreateTranslation(float xPosition, float yPosition, float zPosition);
		static Matrix CreateRotationX(float radians);
		static Matrix CreateRotationY(float radians);
		static Matrix CreateRotationZ(float radians);

		static Matrix CreatePerspective(float fov, float aspectratio, float nearclip, float farclip);
		static Matrix CreateOrthographic(float left, float right, float top, float bottom, float nearclip, float farclip);
		static Matrix CreateViewPort(float x, float y, float width, float height);

		static Matrix Identity();

		~Matrix(void);
};
