#include <algorithm>
#include <math.h>
#include "Vec2F.h"

float clamp(float value, float min, float max)
{
	return std::max(min, std::min(max, value));
}

Vec2F clamp(Vec2F value, Vec2F min, Vec2F max)
{
	return {clamp(value.x, min.x, max.x), clamp(value.y, min.y, max.y)};
}

float length(Vec2F value)
{
	return sqrt(value.x*value.x + value.y*value.y);
}