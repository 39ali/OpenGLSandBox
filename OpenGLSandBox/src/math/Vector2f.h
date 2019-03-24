#pragma once 
#include <math.h>
class Vector2f {
public:
	Vector2f();
	Vector2f(float val);
	Vector2f(float _x, float _y);
	Vector2f(const Vector2f& vecIn);
	inline float dot(const Vector2f& vecIn) const;
	inline Vector2f cross(const Vector2f& vecIn) const;
	inline Vector2f normalize() const;
	inline float length() const;

	Vector2f operator+(const Vector2f& vec2) const;
	Vector2f& operator+=(const Vector2f& vec2);

	Vector2f operator-(const Vector2f& vec2) const;
	Vector2f& operator-=(const Vector2f& vec2);

	Vector2f operator*(const Vector2f& vec2) const;
	Vector2f operator*(const float value) const;
	Vector2f& operator*=(const Vector2f& vec2);
	Vector2f& operator*=(const float value);

	Vector2f& operator/=(float);
	Vector2f operator/(float) const;

	inline const float * operator&(void)const
	{
		return &v[0];
	}

	inline const float operator[](unsigned int index)const {
		return v[index];
	}

	union {
		struct {
			float x, y;
		};
		float v[2];
	};
};

inline float Vector2f::dot(const Vector2f& vecIn) const {
	return x * vecIn.x + y * vecIn.y;
}


inline Vector2f Vector2f::normalize() const {
	return Vector2f(*this) / length();
}

inline float Vector2f::length() const { return sqrt(x * x + y * y ); }

using vec2 = Vector2f;
