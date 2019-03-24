#pragma once
#include "Vector3f.h"
#include <cstdint>

	class Color {
	public:
		Color(const vec3& color, float alpha) :data(color), alpha(alpha) {

		};
		inline  float operator[](uint32_t index)const;
	private:
		vec3  data;
		float alpha;
	};




	inline  float Color::operator[](uint32_t index)const {
		return data[index];
	}
