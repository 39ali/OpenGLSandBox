#include "Quaternion.h"
#include <math.h>

#include "CommonMath.h"


Quaternion::Quaternion( float _w,float _x, float _y, float _z)
    : x(_x), y(_y), z(_z), w(_w){};
Quaternion Quaternion::Normalize() const {
  const float length = sqrtf(x * x + y * y + z * z + w * w);

  return Quaternion(w / length,x / length, y / length, z / length);
}
Quaternion Quaternion::Conjugate() const { return Quaternion(w,-x, -y, -z); }

vec3 Quaternion::QuatToDegrees() const {
  float x1 = atan2(x * z + y * w, x * w - y * z);
  float x2 = acos(-x * x - y * y - z * z - w * w);
  float x3 = atan2(x * z - y * w, x * w + y * z);

  return vec3(toDegrees(x1), toDegrees(x2), toDegrees(x3));
}

// angle in degrees
 vec3 Quaternion::Rotate(float angle, const vec3& axe, const vec3& v)  {
  const float sinHalfangle = sinf(toRadian(angle / 2));
  const float cosHalfAngle = cosf(toRadian(angle / 2));

  const float rx = axe.x * sinHalfangle;
  const float ry = axe.y * sinHalfangle;
  const float rz = axe.z * sinHalfangle;
  const float rw = cosHalfAngle;
  Quaternion rotationQ(rw,rx, ry, rz);

  Quaternion Qconjugate = rotationQ.Conjugate();
  Quaternion w = rotationQ * v * Qconjugate;

  return vec3(w.x, w.y, w.z);
}

Quaternion operator*(const Quaternion& q1, const Quaternion& q2) {
  return Quaternion(q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z,
					q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
                    q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x,
                    q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w);
}

Quaternion operator*(const Quaternion& q, const vec3& v) {
  const float w = -(q.x * v.x) - (q.y * v.y) - (q.z * v.z);
  const float x = (q.w * v.x) + (q.y * v.z) - (q.z * v.y);
  const float y = (q.w * v.y) + (q.z * v.x) - (q.x * v.z);
  const float z = (q.w * v.z) + (q.x * v.y) - (q.y * v.x);
 
  return Quaternion(w,x,y,z);
}


