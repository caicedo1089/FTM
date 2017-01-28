#include <math.h>
#pragma once
const float PI = 3.141592f;

class Quaternion
{
	public:
		Quaternion(void);

		float x;
		float y;
		float z;
		float w;

		void CreateFromAxisAngle(float X, float Y, float Z, float degree);
		void CreateMatrix(float *pMatrix);

		~Quaternion(void);
};
