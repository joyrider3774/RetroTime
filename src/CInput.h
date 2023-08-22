#ifndef CINPUT_H
#define CINPUT_H

#include <SDL.h>
#include <stdbool.h>

struct SButtons {
	bool ButLeft, ButRight, ButUp, ButDown,
		 ButDpadLeft, ButDpadRight, ButDpadUp, ButDpadDown,
		 ButLeft2, ButRight2, ButUp2, ButDown2,
		 ButBack, ButStart, ButA, ButB,
		 ButX, ButY, ButLB, ButRB, ButFullscreen, ButQuit, ButRT, ButLT,
		 RenderReset;
};
typedef struct SButtons SButtons;

extern SButtons CInput_Buttons, CInput_PrevButtons;
void CInput_Init();
void CInput_DeInit();
void CInput_Update();
void CInput_ResetButtons();
void CInput_HandleJoystickAxisEvent(int Axis, int Value);
void CInput_HandleJoystickButtonEvent(int Button, bool Value);
void CInput_HandleKeyboardEvent(int Key, bool Value);
#endif