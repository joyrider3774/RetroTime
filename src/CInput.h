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

extern SDL_GameController* CInput_GameController;
extern SButtons CInput_Buttons, CInput_PrevButtons;
extern int CInput_JoystickDeadZone, CInput_TriggerDeadZone;
void CInput_Init();
void CInput_DeInit();
void CInput_Update();
void CInput_ResetButtons();
void CInput_HandleJoystickAxisEvent(int Axis, int Value);
void CInput_HandleJoystickButtonEvent(int Button, bool Value);
void CInput_HandleKeyboardEvent(int Key, bool Value);
