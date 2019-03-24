#pragma once

#include "Vector3f.h"


	class Quaternion {
public :
	Quaternion() { x = y = z = w = 0.0f; }
  Quaternion(float w,float _x, float _y, float _z);
  Quaternion Normalize()const ;
  Quaternion Conjugate()const ;
  vec3 QuatToDegrees()const;
  static vec3 Rotate(float angle, const vec3& axe, const vec3& v);
  float x, y, z, w;
};

	Quaternion operator*(const Quaternion& q1, const Quaternion& q2);

	Quaternion operator*(const Quaternion& q, const vec3& v);

