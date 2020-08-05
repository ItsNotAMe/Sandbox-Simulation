#pragma once

struct vec2
{
	float x, y;

	vec2()
		: x(0), y(0) 
	{
	}

	vec2(float x, float y)
		: x(x), y(y)
	{
	}

	vec2(const vec2&) = default;
};

namespace Util {

	float Clamp(float value, float min, float max);

	float Lerp(float v0, float v1, float t);

}