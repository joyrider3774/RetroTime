#include <SDL.h>
#include <SDL_joystick.h>
#include "CInput.h"

CInput::CInput()
{
    JoystickDeadZone = 10000;
    TriggerDeadZone = 10000;
    ResetButtons();
    for (int i=0; i < SDL_NumJoysticks(); i++)
    {
        if(SDL_IsGameController(i))
        {
            GameController = SDL_GameControllerOpen(i);
            SDL_GameControllerEventState(SDL_ENABLE);
            SDL_Log("Joystick Detected!\n");
            break;
        }
    }
}

CInput::~CInput()
{
    if(GameController) 
    {
        SDL_GameControllerClose(GameController);
        GameController = NULL;
    }
}

void CInput::Update()
{
    SDL_Event Event;
    PrevButtons = Buttons;
    Buttons.ButQuit = false;
    Buttons.RenderReset = false;
    while (SDL_PollEvent(&Event))
    {
        if (Event.type == SDL_RENDER_TARGETS_RESET)
            Buttons.RenderReset = true;

        if (Event.type == SDL_QUIT)
            Buttons.ButQuit = true;
      
        if (Event.type == SDL_JOYDEVICEADDED)
            if(GameController == NULL)
                if(SDL_IsGameController(Event.jdevice.which))
                GameController = SDL_GameControllerOpen(Event.jdevice.which);

        if (Event.type == SDL_JOYDEVICEREMOVED)
        {
            SDL_Joystick* Joystick = SDL_GameControllerGetJoystick(GameController);
            if (Joystick)
                if (Event.jdevice.which == SDL_JoystickInstanceID(Joystick))
                {
                    SDL_GameControllerClose(GameController);
                    GameController = NULL;
                }
        }

        if (Event.type == SDL_CONTROLLERAXISMOTION)
            HandleJoystickAxisEvent(Event.jaxis.axis, Event.jaxis.value);

        if (Event.type == SDL_CONTROLLERBUTTONUP)
            HandleJoystickButtonEvent(Event.cbutton.button, false);

        if (Event.type == SDL_CONTROLLERBUTTONDOWN)
            HandleJoystickButtonEvent(Event.cbutton.button, true);

        if (Event.type == SDL_KEYUP)
            HandleKeyboardEvent(Event.key.keysym.sym, false);
        
        if (Event.type == SDL_KEYDOWN)
            HandleKeyboardEvent(Event.key.keysym.sym, true);
    }

}

void CInput::ResetButtons()
{
    Buttons.ButLeft = false;
    Buttons.ButRight = false;
    Buttons.ButUp = false;
    Buttons.ButDown = false;
    Buttons.ButLB = false; 
    Buttons.ButRB = false;
    Buttons.ButLT = false;
    Buttons.ButRT = false;
    Buttons.ButBack = false; 
    Buttons.ButA = false; 
    Buttons.ButB = false;
    Buttons.ButX = false;
    Buttons.ButY = false;
    Buttons.ButStart = false;
    Buttons.ButQuit = false;
    Buttons.ButFullscreen = false;
    Buttons.RenderReset = false;
    PrevButtons = Buttons;
}

void CInput::HandleJoystickButtonEvent(int Button, bool Value) 
{
    switch (Button)
    {
        case SDL_CONTROLLER_BUTTON_Y:
            Buttons.ButY = Value;
            break;
        case SDL_CONTROLLER_BUTTON_X:
            Buttons.ButX = Value;
            break;
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
            Buttons.ButLB = Value;
            break;
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
            Buttons.ButRB = Value;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_UP:
            Buttons.ButUp = Value;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            Buttons.ButDown = Value;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
            Buttons.ButLeft = Value;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            Buttons.ButRight = Value;
            break;
        case SDL_CONTROLLER_BUTTON_A:
            Buttons.ButA = Value;
            break;
        case SDL_CONTROLLER_BUTTON_B:
            Buttons.ButB = Value;
            break;
        case SDL_CONTROLLER_BUTTON_START:
            Buttons.ButStart = Value;
            break;
        case SDL_CONTROLLER_BUTTON_BACK:
            Buttons.ButBack = Value;
            break;
        default:
            break;
    }
}

void CInput::HandleKeyboardEvent(int Key, bool Value) 
{
    switch (Key)
    {
        case SDLK_f:
            Buttons.ButFullscreen = Value;
            break;
        case SDLK_PAGEDOWN:
            Buttons.ButRB = Value;
            break;
        case SDLK_PAGEUP:
            Buttons.ButLB = Value;
            break;
        case SDLK_UP:
            Buttons.ButUp = Value;
            break;
        case SDLK_DOWN:
            Buttons.ButDown = Value;
            break;
        case SDLK_LEFT:
            Buttons.ButLeft = Value;
            break;
        case SDLK_RIGHT:
            Buttons.ButRight = Value;
            break;
        case SDLK_RETURN:
            Buttons.ButStart = Value;
            break;
        case SDLK_ESCAPE:
            Buttons.ButBack = Value;
            break;
        case SDLK_SPACE:
            Buttons.ButA = Value;
            break;
        case SDLK_b:
            Buttons.ButB = Value;
            break;
        default:
            break;
    }
}

void CInput::HandleJoystickAxisEvent(int Axis, int Value)
{
    switch(Axis)
    {
        case SDL_CONTROLLER_AXIS_RIGHTX:
        case SDL_CONTROLLER_AXIS_LEFTX:
            if (abs(Value) < JoystickDeadZone)
            {
                Buttons.ButRight = false;
                Buttons.ButLeft = false;
                return;
            }
            if(Value > 0)
                Buttons.ButRight = true;
            else
                Buttons.ButLeft = true;
            break;
 
        case SDL_CONTROLLER_AXIS_RIGHTY:
        case SDL_CONTROLLER_AXIS_LEFTY:
            if (abs(Value) < JoystickDeadZone)
            {
                Buttons.ButUp = false;
                Buttons.ButDown = false;
                return;
            }
            if(Value < 0)
                Buttons.ButUp = true;
            else
                Buttons.ButDown = true;
            break;

        case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
            if (abs(Value) < TriggerDeadZone)
            {
                Buttons.ButLT = false;
                return;
            }
            Buttons.ButLT = true;
            break;

        case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
            if (abs(Value) < TriggerDeadZone)
            {
                Buttons.ButRT = false;
                return;
            }
            Buttons.ButRT = true;
            break;
    }
}