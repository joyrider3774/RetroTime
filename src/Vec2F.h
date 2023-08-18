#pragma once

constexpr float epsilion = 0.000001;

struct Vec2F {
	float x;
	float y;
};
typedef struct Vec2F Vec2F;

float clamp(float value, float min, float max);
Vec2F clamp(Vec2F value, Vec2F min, Vec2F max);
float length(Vec2F value);