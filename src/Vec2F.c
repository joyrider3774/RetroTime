#include <math.h>
#include "Vec2F.h"

float clampFloat(float value, float min, float max)
{
	//return std::max(min, std::min(max, value));
	float tmpmin = max > value ? value: max;
	return min > tmpmin ? min : tmpmin;
}

Vec2F clampVec2F(Vec2F value, Vec2F min, Vec2F max)
{
	Vec2F Result = {clampFloat(value.x, min.x, max.x), clampFloat(value.y, min.y, max.y)};
	return Result;
}

float length(Vec2F value)
{
	return sqrt(value.x*value.x + value.y*value.y);
}