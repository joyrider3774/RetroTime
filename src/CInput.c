#include <SDL.h>
#include <SDL_joystick.h>
#include <stdbool.h>
#include "CInput.h"


SDL_GameController* CInput_GameController = NULL;
SButtons CInput_Buttons, CInput_PrevButtons;
int CInput_JoystickDeadZone, CInput_TriggerDeadZone;

void CInput_Init()
{
	CInput_JoystickDeadZone = 10000;
	CInput_TriggerDeadZone = 10000;
	CInput_ResetButtons();
	for (int i=0; i < SDL_NumJoysticks(); i++)
	{
		if(SDL_IsGameController(i))
		{
			CInput_GameController = SDL_GameControllerOpen(i);
			SDL_GameControllerEventState(SDL_ENABLE);
			SDL_Log("Joystick Detected!\n");
			break;
		}
	}
}

void CInput_DeInit()
{
	if(CInput_GameController)
	{
		SDL_GameControllerClose(CInput_GameController);
		CInput_GameController = NULL;
	}
}

void CInput_Update()
{
	SDL_Event Event;
	CInput_PrevButtons = CInput_Buttons;
	CInput_Buttons.ButQuit = false;
	CInput_Buttons.RenderReset = false;
	while (SDL_PollEvent(&Event))
	{
		if (Event.type == SDL_RENDER_TARGETS_RESET)
			CInput_Buttons.RenderReset = true;

		if (Event.type == SDL_QUIT)
			CInput_Buttons.ButQuit = true;

		if (Event.type == SDL_JOYDEVICEADDED)
			if(CInput_GameController == NULL)
				if(SDL_IsGameController(Event.jdevice.which))
				CInput_GameController = SDL_GameControllerOpen(Event.jdevice.which);

		if (Event.type == SDL_JOYDEVICEREMOVED)
		{
			SDL_Joystick* Joystick = SDL_GameControllerGetJoystick(CInput_GameController);
			if (Joystick)
				if (Event.jdevice.which == SDL_JoystickInstanceID(Joystick))
				{
					SDL_GameControllerClose(CInput_GameController);
					CInput_GameController = NULL;
				}
		}

		if (Event.type == SDL_CONTROLLERAXISMOTION)
			CInput_HandleJoystickAxisEvent(Event.jaxis.axis, Event.jaxis.value);

		if (Event.type == SDL_CONTROLLERBUTTONUP)
			CInput_HandleJoystickButtonEvent(Event.cbutton.button, false);

		if (Event.type == SDL_CONTROLLERBUTTONDOWN)
			CInput_HandleJoystickButtonEvent(Event.cbutton.button, true);

		if (Event.type == SDL_KEYUP)
			CInput_HandleKeyboardEvent(Event.key.keysym.sym, false);

		if (Event.type == SDL_KEYDOWN)
			CInput_HandleKeyboardEvent(Event.key.keysym.sym, true);
	}

}

void CInput_ResetButtons()
{
	CInput_Buttons.ButLeft = false;
	CInput_Buttons.ButRight = false;
	CInput_Buttons.ButUp = false;
	CInput_Buttons.ButDown = false;
	CInput_Buttons.ButLB = false;
	CInput_Buttons.ButRB = false;
	CInput_Buttons.ButLT = false;
	CInput_Buttons.ButRT = false;
	CInput_Buttons.ButBack = false;
	CInput_Buttons.ButA = false;
	CInput_Buttons.ButB = false;
	CInput_Buttons.ButX = false;
	CInput_Buttons.ButY = false;
	CInput_Buttons.ButStart = false;
	CInput_Buttons.ButQuit = false;
	CInput_Buttons.ButFullscreen = false;
	CInput_Buttons.RenderReset = false;
	CInput_Buttons.ButDpadLeft = false;
	CInput_Buttons.ButDpadRight = false;
	CInput_Buttons.ButDpadUp = false;
	CInput_Buttons.ButDpadDown = false;
	CInput_Buttons.ButLeft2 = false;
	CInput_Buttons.ButRight2 = false;
	CInput_Buttons.ButUp2 = false;
	CInput_Buttons.ButDown2 = false;
	CInput_PrevButtons = CInput_Buttons;
}

void CInput_HandleJoystickButtonEvent(int Button, bool Value)
{
	switch (Button)
	{
		case SDL_CONTROLLER_BUTTON_Y:
			CInput_Buttons.ButY = Value;
			break;
		case SDL_CONTROLLER_BUTTON_X:
			CInput_Buttons.ButX = Value;
			break;
		case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
			CInput_Buttons.ButLB = Value;
			break;
		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
			CInput_Buttons.ButRB = Value;
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_UP:
			CInput_Buttons.ButDpadUp = Value;
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
			CInput_Buttons.ButDpadDown = Value;
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
			CInput_Buttons.ButDpadLeft = Value;
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
			CInput_Buttons.ButDpadRight = Value;
			break;
		case SDL_CONTROLLER_BUTTON_A:
			CInput_Buttons.ButA = Value;
			break;
		case SDL_CONTROLLER_BUTTON_B:
			CInput_Buttons.ButB = Value;
			break;
		case SDL_CONTROLLER_BUTTON_START:
			CInput_Buttons.ButStart = Value;
			break;
		case SDL_CONTROLLER_BUTTON_BACK:
			CInput_Buttons.ButBack = Value;
			break;
		default:
			break;
	}
}

void CInput_HandleKeyboardEvent(int Key, bool Value)
{
	switch (Key)
	{
		case SDLK_F4:
			CInput_Buttons.ButQuit = Value;
			break;
		case SDLK_F3:
			CInput_Buttons.ButFullscreen = Value;
			break;
		case SDLK_b:
			CInput_Buttons.ButRB = Value;
			break;
		case SDLK_x:
			CInput_Buttons.ButLB = Value;
			break;
		case SDLK_UP:
			CInput_Buttons.ButUp = Value;
			break;
		case SDLK_DOWN:
			CInput_Buttons.ButDown = Value;
			break;
		case SDLK_LEFT:
			CInput_Buttons.ButLeft = Value;
			break;
		case SDLK_RIGHT:
			CInput_Buttons.ButRight = Value;
			break;
		case SDLK_RETURN:
			CInput_Buttons.ButStart = Value;
			break;
		case SDLK_ESCAPE:
			CInput_Buttons.ButBack = Value;
			break;
		case SDLK_SPACE:
		case SDLK_c:
			CInput_Buttons.ButA = Value;
			break;
		case SDLK_LCTRL:
		case SDLK_RCTRL:
		case SDLK_v:
			CInput_Buttons.ButB = Value;
			break;
		case SDLK_LALT:
		case SDLK_RALT:
		case SDLK_d:
			CInput_Buttons.ButX = Value;
			break;
		case SDLK_f:
			CInput_Buttons.ButY = Value;
			break;
		case SDLK_s:
			CInput_Buttons.ButLT = Value;
			break;
		case SDLK_g:
			CInput_Buttons.ButRT = Value;
			break;
		default:
			break;
	}
}

void CInput_HandleJoystickAxisEvent(int Axis, int Value)
{
	switch(Axis)
	{
		case SDL_CONTROLLER_AXIS_LEFTX:
			if (abs(Value) < CInput_JoystickDeadZone)
			{
				CInput_Buttons.ButRight = false;
				CInput_Buttons.ButLeft = false;
				return;
			}
			if(Value > 0)
				CInput_Buttons.ButRight = true;
			else
				CInput_Buttons.ButLeft = true;
			break;

		case SDL_CONTROLLER_AXIS_LEFTY:
			if (abs(Value) < CInput_JoystickDeadZone)
			{
				CInput_Buttons.ButUp = false;
				CInput_Buttons.ButDown = false;
				return;
			}
			if(Value < 0)
				CInput_Buttons.ButUp = true;
			else
				CInput_Buttons.ButDown = true;
			break;

		case SDL_CONTROLLER_AXIS_RIGHTX:
			if (abs(Value) < CInput_JoystickDeadZone)
			{
				CInput_Buttons.ButRight2 = false;
				CInput_Buttons.ButLeft2 = false;
				return;
			}
			if(Value > 0)
				CInput_Buttons.ButRight2 = true;
			else
				CInput_Buttons.ButLeft2 = true;
			break;

		case SDL_CONTROLLER_AXIS_RIGHTY:
			if (abs(Value) < CInput_JoystickDeadZone)
			{
				CInput_Buttons.ButUp2 = false;
				CInput_Buttons.ButDown2 = false;
				return;
			}
			if(Value < 0)
				CInput_Buttons.ButUp2 = true;
			else
				CInput_Buttons.ButDown2 = true;
			break;

		case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
			if (abs(Value) < CInput_TriggerDeadZone)
			{
				CInput_Buttons.ButLT = false;
				return;
			}
			CInput_Buttons.ButLT = true;
			break;

		case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
			if (abs(Value) < CInput_TriggerDeadZone)
			{
				CInput_Buttons.ButRT = false;
				return;
			}
			CInput_Buttons.ButRT = true;
			break;
	}
}