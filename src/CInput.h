#pragma once

#include <SDL.h>

struct SButtons {
	bool ButLeft, ButRight, ButUp, ButDown,
		 ButDpadLeft, ButDpadRight, ButDpadUp, ButDpadDown,
		 ButLeft2, ButRight2, ButUp2, ButDown2,
		 ButBack, ButStart, ButA, ButB,
		 ButX, ButY, ButLB, ButRB, ButFullscreen, ButQuit, ButRT, ButLT,
		 RenderReset;
};

class CInput {

private:
	SDL_GameController* GameController = NULL;
	void HandleJoystickAxisEvent(int Axis, int Value);
	void HandleJoystickButtonEvent(int Button, bool Value);
	void HandleKeyboardEvent(int Key, bool Value);
public:
	SButtons Buttons, PrevButtons;
	int JoystickDeadZone, TriggerDeadZone;
	CInput();
	~CInput();
	void Update();
	void ResetButtons();
};