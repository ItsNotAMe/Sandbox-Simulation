#include "Util.h"

namespace Util {

	float Clamp(float value, float min, float max)
	{
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

	float Lerp(float v0, float v1, float t) {
		return (1 - t) * v0 + t * v1;
	}

}