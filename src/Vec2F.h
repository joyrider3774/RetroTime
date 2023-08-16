#pragma once

constexpr float epsilion = 0.000001;

struct Vec2F {
	float x;
	float y;

	inline Vec2F operator-() {
		return {-x,-y};
	}

	inline Vec2F operator-(float a) {
		return {x-a,y-a};
	}

	inline Vec2F operator+(float a) {
		return {x+a,y+a};
	}

	inline Vec2F operator*(float a) {
		return {x*a,y*a};
	}

	inline Vec2F operator/(float a) {
		return {x/a,y/a};
	}

	inline Vec2F operator-(int a) {
		return {x-(float)a,y-(float)a};
	}

	inline Vec2F operator+(int a) {
		return {x+(float)a,y+(float)a};
	}

	inline Vec2F operator*(int a) {
		return {x*(float)a,y*(float)a};
	}

	inline Vec2F operator/(int a) {
		return {x/(float)a,y/(float)a};
	}

	inline Vec2F operator+(Vec2F a) {
		return {a.x+x,a.y+y};
	}

	inline Vec2F operator-(Vec2F a) {
		return {x-a.x,y-a.y};
	}

	inline Vec2F operator*(Vec2F a) {
		return {a.x*x,a.y*y};
	}

	inline Vec2F operator/(Vec2F a) {
		return {x / a.x, y / a.y};
	}

	inline bool operator==(Vec2F a) {
		if ((a.x - x < epsilion) && (a.y - y < epsilion))
			return true;
		else
			return false;
	}

	inline bool operator!=(Vec2F a) {
		if ((a.x - x < epsilion) && (a.y - y < epsilion))
			return false;
		else
			return true;
	}

};
typedef struct Vec2F Vec2F;

float clamp(float value, float min, float max);
Vec2F clamp(Vec2F value, Vec2F min, Vec2F max);
float length(Vec2F value);