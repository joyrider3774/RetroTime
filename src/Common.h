#pragma once

#include <SDL.h>
#include <string.h>
#include <sstream>

using namespace std;

constexpr bool debugInfo = false;
constexpr bool debugShowCollisionShapes = false;

constexpr bool titleAlphaEnabled = true;

constexpr int MaxAlpha = 245;
constexpr int AlphaIncrease = 5;
constexpr int MaxAlphaTime = 750;

constexpr int ScreenWidth=1280;
constexpr int ScreenHeight=720;

constexpr Uint32 PixelFormat = SDL_PIXELFORMAT_ARGB8888;

constexpr float DesiredFps=60.0f;

//GameStates
constexpr int initDiff=50;

constexpr int GSQuit=0;
constexpr int GSIntro=1;
constexpr int GSGame=2;
constexpr int GSTitleScreen=3;
constexpr int GSRamIt=4;
constexpr int GSSubScore = 5;
constexpr int GSSpaceInvaders = 6;
constexpr int GSBreakout = 7;
constexpr int GSFrog = 8;
constexpr int GSSnake = 9;
constexpr int GSPang = 10;
constexpr int GSTetris = 11;
constexpr int GSEddy = 12;

constexpr int GSIntroInit = GSIntro + initDiff;
constexpr int GSGameInit = GSGame + initDiff;
constexpr int GSTitleScreenInit = GSTitleScreen + initDiff;
constexpr int GSRamItInit = GSRamIt + initDiff;
constexpr int GSSubScoreInit = GSSubScore + initDiff;

constexpr int GSSpaceInvadersInit = GSSpaceInvaders + initDiff;
constexpr int GSBreakoutInit =GSBreakout + initDiff;
constexpr int GSFrogInit = GSFrog + initDiff;
constexpr int GSSnakeInit = GSSnake + initDiff;
constexpr int GSPangInit = GSPang + initDiff;
constexpr int GSTetrisInit = GSTetris + initDiff;
constexpr int GSEddyInit = GSEddy + initDiff;

//SubGameStates
constexpr int SGNone = 0;
constexpr int SGReadyGo = 1;
constexpr int SGGame = 2;
constexpr int SGTimeUp = 3;
constexpr int SGFrame = 4;
constexpr int SGCalcScore = 5;
constexpr int SGFadeIn = 6;
constexpr int SGPauseMenu = 7;
constexpr int SGGameHelp = 8;

//GameModes
constexpr int Modes = 3;
constexpr int GMRetroCarousel = 0;
constexpr int GMGame = 1;
constexpr int GMTimeAttack = 2;

constexpr int Games = 8;

//PAUSEMENUS
constexpr int PMNone = -1;
constexpr int PMContinue = 0;
constexpr int PMQuit = 1;
constexpr int PMMusicVol = 2;
constexpr int PMSoundVol = 3;
constexpr int PMMotionBlur = 4;
constexpr int PMGameHelp = 5;
constexpr int PMCrt = 6;

constexpr int PauseMenus = 7;

//main menus
constexpr int MMStart = 0;
constexpr int MMHighScores = 1;
constexpr int MMHelp = 2;
constexpr int MMQuit = 3;
constexpr int MMOptions = 4;
constexpr int MMCredits = 5;
//Sub Main Menus
constexpr int MMSelectGameMode = 7;
constexpr int MMSelectGame = 8;
//6 because only fist 6 are real main menu
constexpr int MainMenus = 6;

//options menu
constexpr int OMMusicVol = 0;
constexpr int OMSoundVol = 1;
constexpr int OMMotionBlur = 2;
constexpr int OMCrt = 3;
constexpr int OMResetHighScores = 4;
constexpr int OMBack = 5;
constexpr int OptionMenus = 6;

//CRTS
constexpr int Crts = 6;

struct ModeDesc {
    int mode;
    string name;
    string description;
};

struct GameDesc {
    int state;
    string name;
    string description;
    string shortdescription;
};

struct PauseMenuDesc {
    int menu;
    string name;
};

struct GPGamePauseMenusDesc {
    int game;
    int menus[7];
};

struct MainMenusDesc {
    int menu;
    string name;
};

struct OptionMenusDesc {
    int menu;
    string name;
};

struct CrtOptDesc {
    int crt;
    string name;
};

extern GameDesc GSGames[Games];
extern ModeDesc GMModes[Modes];
extern MainMenusDesc MMMainMenus[MainMenus];
extern OptionMenusDesc OMOptionMenus[OptionMenus];
extern GPGamePauseMenusDesc GPGamePauseMenus[Games];
extern PauseMenuDesc PMPauseMenus[PauseMenus];
extern OptionMenusDesc OMOptionMenus[OptionMenus];
extern CrtOptDesc CrtOptions[Crts];

extern Uint32 ColorToUint(int R, int G, int B);
extern SDL_Colour UintToColor(Uint32 colour);

//https://stackoverflow.com/questions/16605967/set-precision-of-stdto-string-when-converting-floating-point-values
template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}
