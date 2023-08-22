#include "CTween.h"
#include <stdbool.h>
void initialize_CTweenInfo(CTweenInfo *TweenInfo)
{
	TweenInfo->val = 0.0f;
	TweenInfo->ticks = 0.0f;
	TweenInfo->maxticks = 0.0f;
	TweenInfo->inc = 0.0f;
	TweenInfo->active = false;
	TweenInfo->func = funcsmoothstart;
	TweenInfo->id = -1;
	TweenInfo->funcval = 0.0f;
	TweenInfo->multiplier = 0.0f;
}

float smoothstart2(float val)
{
	return val*val;
}

float smoothstop2(float val)
{
	return 1-((1-val)*(1-val));
}

float smoothstartflip2(float val)
{
	return 1 - smoothstart2(val);
}

float smoothstopflip2(float val)
{
	return 1 - smoothstop2(val);
}

float mix(float val1, float val2, float blendval)
{
	return (1-blendval)*val1 + blendval*val2;
}

float smoothstep3(float val)
{
	return mix(smoothstart2(val), smoothstop2(val), val);
}

float arch(float val)
{
	return 4*(val)*(1-val);
}

float tweencalcstepinc(float seconds, float FPS)
{
	return 1/ (FPS * seconds);
}

float tweencalcmaxticks(float seconds, float FPS)
{
	return seconds * FPS;
}

CTweenInfo createtween(int id, float duration, int tweenfunc, float multiplier, bool active, float FPS)
{
	CTweenInfo tween;
	tween.maxticks = tweencalcmaxticks(duration, FPS);
	tween.ticks = 0;
	tween.inc = tweencalcstepinc(duration, FPS);
	tween.multiplier = multiplier;
	tween.val = 0;
	tween.func = tweenfunc;
	tween.funcval = 0;
	tween.active = active;
	tween.id = id;
	return tween;
}

CTweenInfo updatetween(CTweenInfo tween)
{
	if (tween.active)
	{
		if (tween.ticks < tween.maxticks)
		{
			tween.ticks += 1;
			tween.val += tween.inc;
			switch(tween.func)
			{
				case funcsmoothstart:
					tween.funcval = smoothstart2(tween.val) * tween.multiplier;
					break;
				case funcsmoothstop:
					tween.funcval = smoothstop2(tween.val) * tween.multiplier;
					break;
				case funcsmoothstartflip:
					tween.funcval = smoothstartflip2(tween.val) * tween.multiplier;
					break;
				case funcsmoothstopflip:
					tween.funcval = smoothstopflip2(tween.val) * tween.multiplier;
					break;
				case funcsmoothstep:
					tween.funcval = smoothstep3(tween.val) * tween.multiplier;
					break;
				case funcarchstep:
					tween.funcval = arch(smoothstep3(tween.val)) * tween.multiplier;
					break;
			}
		}
		else
			tween.active = false;
	}
	return tween;
}