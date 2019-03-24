#include "Vector2f.h"

#include "Vector3f.h"

Vector2f::Vector2f() : x(0), y(0) {}
Vector2f::Vector2f(float val) : x(val), y(val) {}
Vector2f::Vector2f(float _x, float _y) : x(_x), y(_y) {}
Vector2f::Vector2f(const Vector2f &vecIn) : x(vecIn.x), y(vecIn.y) {}

Vector2f Vector2f::operator+(const Vector2f &vec3) const {
  return Vector2f(x + vec3.x, y + vec3.y);
}
Vector2f &Vector2f::operator+=(const Vector2f &vec3) {
  x += vec3.x;
  y += vec3.y;

  return *this;
}

Vector2f Vector2f::operator-(const Vector2f &vec3) const {
  return Vector2f(x - vec3.x, y - vec3.y);
}
Vector2f &Vector2f::operator-=(const Vector2f &vec3) {
  x -= vec3.x;
  y -= vec3.y;

  return *this;
}

Vector2f Vector2f::operator*(const Vector2f &vec3) const {
  return Vector2f(x * vec3.x, y * vec3.y);
}
Vector2f Vector2f::operator*(const float value) const {
  return Vector2f(x * value, y * value);
}
Vector2f &Vector2f::operator*=(const Vector2f &vec3) {
  x *= vec3.x;
  y *= vec3.y;

  return *this;
}
Vector2f &Vector2f::operator*=(const float value) {

  x -= value;
  y -= value;

  return *this;
}

Vector2f &Vector2f::operator/=(float f) {
  x /= f;
  y /= f;

  return *this;
}
Vector2f Vector2f::operator/(float f) const { return vec2(x / f, y / f); }
