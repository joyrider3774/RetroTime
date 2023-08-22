#ifndef VEC2F_H
#define VEC2F_H

struct Vec2F {
	float x;
	float y;
};
typedef struct Vec2F Vec2F;

float clampFloat(float value, float min, float max);
Vec2F clampVec2F(Vec2F value, Vec2F min, Vec2F max);
float length(Vec2F value);

#endif