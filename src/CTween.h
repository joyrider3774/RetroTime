#pragma once

enum TweenFunc
{
    funcsmoothstart = 0,
    funcsmoothstop = 1,
    funcsmoothstartflip = 3,
    funcsmoothstopflip = 4,
    funcsmoothstep = 5,
    funcarchstep = 6
};

class CTweenInfo
{
public:
	float val;
	float ticks;
	float maxticks;
	float inc;
	bool active = false;
	TweenFunc func;
	int id = -1;
	float funcval;
	float multiplier;
};

float tweencalcmaxticks(float seconds, float FPS);
float tweencalcstepinc(float seconds, float FPS);
float arch(float val);
float smoothstep3(float val);
float mix(float val1, float val2, float blendval);
float smoothstopflip2(float val);
float smoothstartflip2(float val);
float smoothstop2(float val);
float smoothstart2(float val);
CTweenInfo updatetween(CTweenInfo tween);
CTweenInfo createtween(int id, float duration, TweenFunc tweenfunc, float multiplier, bool active, float FPS);
