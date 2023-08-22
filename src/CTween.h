#ifndef CTWEEN_H
#define CTWEEN_H

#include <stdbool.h>

#define funcsmoothstart 0
#define funcsmoothstop 1
#define funcsmoothstartflip 3
#define funcsmoothstopflip 4
#define funcsmoothstep 5
#define funcarchstep 6

typedef enum TweenFunc TweenFunc;

struct CTweenInfo
{
	float val;
	float ticks;
	float maxticks;
	float inc;
	bool active;
	int func;
	int id;
	float funcval;
	float multiplier;
};
typedef struct CTweenInfo CTweenInfo;

void initialize_CTweenInfo(CTweenInfo *TweenInfo);
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
CTweenInfo createtween(int id, float duration, int tweenfunc, float multiplier, bool active, float FPS);

#endif