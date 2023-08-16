#pragma once

#include <SDL.h>
#include <string>
#include <sstream>

constexpr bool debugInfo = false;
constexpr bool debugShowCollisionShapes = false;

constexpr bool titleAlphaEnabled = true;

constexpr int MaxAlpha = 245;
constexpr int AlphaIncrease = 5;
constexpr int MaxAlphaTime = 750;

constexpr int ScreenWidth=1280;
constexpr int ScreenHeight=720;
constexpr int WindowWidth=400;
constexpr int WindowHeight=240;

constexpr Uint32 PixelFormat = SDL_PIXELFORMAT_ARGB8888;

constexpr float DesiredFps=60.0f;

constexpr int ScoreBarHeight=24;

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
constexpr int SGSubmitScore = 6;
constexpr int SGFadeIn = 7;
constexpr int SGPauseMenu = 8;
constexpr int SGGameHelp = 9;

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
constexpr int PMColorModR = 6;
constexpr int PMColorModG = 7;
constexpr int PMColorModB = 8;
constexpr int PMCrt = 9;


constexpr int PauseMenus = 10;

//main menus
constexpr int MMStart = 0;
constexpr int MMLocalHighScores = 1;
constexpr int MMOnlineHighScores = 2;
constexpr int MMHelp = 3;
constexpr int MMQuit = 4;
constexpr int MMOptions = 5;
constexpr int MMCredits = 6;
//Sub Main Menus
constexpr int MMSelectGameMode = 8;
constexpr int MMSelectGame = 9;
//7 because only fist 7 are real main menu
constexpr int MainMenus = 7;

//options menu
constexpr int OMMusicVol = 0;
constexpr int OMSoundVol = 1;
constexpr int OMMotionBlur = 2;
constexpr int OMCrt = 3;
constexpr int OMColorModR = 4;
constexpr int OMColorModG = 5;
constexpr int OMColorModB = 6;
constexpr int OMResetHighScores = 7;
constexpr int OMBack = 8;
constexpr int OptionMenus = 9;

//CRTS
constexpr int Crts = 6;

struct ModeDesc {
	int mode;
	std::string name;
	std::string description;
};

struct GameDesc {
	int state;
	std::string name;
	std::string description;
	std::string shortdescription;
};

struct PauseMenuDesc {
	int menu;
	std::string name;
};

struct GPGamePauseMenusDesc {
	int game;
	int menus[10];
};

struct MainMenusDesc {
	int menu;
	std::string name;
};

struct OptionMenusDesc {
	int menu;
	std::string name;
};

struct CrtOptDesc {
	int crt;
	std::string name;
};

extern GameDesc GSGames[Games];
extern ModeDesc GMModes[Modes];
extern MainMenusDesc MMMainMenus[MainMenus];
extern OptionMenusDesc OMOptionMenus[OptionMenus];
extern GPGamePauseMenusDesc GPGamePauseMenus[Games];
extern PauseMenuDesc PMPauseMenus[PauseMenus];
extern OptionMenusDesc OMOptionMenus[OptionMenus];
extern CrtOptDesc CrtOptions[Crts];
extern int LeaderBoardIds[Games+1][Modes];

extern Uint32 ColorToUint(int R, int G, int B);
extern SDL_Colour UintToColor(Uint32 colour);

extern void ditherTarget(SDL_Renderer* aRenderer, SDL_Texture *Tex, SDL_Rect* Rect,uint8_t bayerver, uint8_t whiteThreasHold);
extern void ditherSurface(SDL_Surface* Surface, SDL_Rect* Rect, uint8_t bayerver, uint8_t whiteThreasHold);
extern Uint32 GetPixel(SDL_Surface *surface, int x, int y);
extern void PutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

//https://stackoverflow.com/questions/16605967/set-precision-of-stdto-string-when-converting-floating-point-values
template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << a_value;
	return out.str();
}
