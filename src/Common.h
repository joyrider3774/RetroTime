#pragma once

#include <SDL.h>
#include <string>
#include <sstream>

constexpr bool debugInfo = true;
constexpr bool debugShowCollisionShapes = false;
constexpr bool dumpScaledBitmaps = false;
constexpr bool useDefaultColorAssets = false;
constexpr bool loadDumpedScaledBitmaps = true;

constexpr bool titleAlphaEnabled = true;

constexpr int MaxAlpha = 245;
constexpr int AlphaIncrease = 5;
constexpr int MaxAlphaTime = 750;

constexpr int ScreenWidth=400;
constexpr int ScreenHeight=240;
constexpr int WindowWidth=400;
constexpr int WindowHeight=240;
constexpr float xscale = ScreenWidth / 1280.0f;
constexpr float yscale = ScreenHeight / 720.0f;

constexpr Uint32 PixelFormat = SDL_PIXELFORMAT_ARGB8888;

constexpr double DesiredFps=60.0;

constexpr int ScoreBarHeight=24*yscale;

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
constexpr int PMGameHelp = 4;


constexpr int PauseMenus = 10;

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
constexpr int OMResetHighScores = 2;
constexpr int OMBack = 3;
constexpr int OptionMenus = 4;

using namespace std;


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
	int menus[5];
};

struct MainMenusDesc {
	int menu;
	string name;
};

struct OptionMenusDesc {
	int menu;
	string name;
};

extern GameDesc GSGames[Games];
extern ModeDesc GMModes[Modes];
extern MainMenusDesc MMMainMenus[MainMenus];
extern OptionMenusDesc OMOptionMenus[OptionMenus];
extern GPGamePauseMenusDesc GPGamePauseMenus[Games];
extern PauseMenuDesc PMPauseMenus[PauseMenus];
extern OptionMenusDesc OMOptionMenus[OptionMenus];

extern Uint32 ColorToUint(int R, int G, int B);
extern SDL_Colour UintToColor(Uint32 colour);

extern void ditherTarget(SDL_Renderer* aRenderer, SDL_Texture *Tex, SDL_Rect* Rect,uint8_t bayerver, uint8_t whiteThreasHold);
extern void ditherSurface(SDL_Surface* Surface, SDL_Rect* Rect, uint8_t bayerver, uint8_t whiteThreasHold);
extern Uint32 GetPixel(SDL_Surface *surface, int x, int y);
extern void PutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
extern void SDL_SaveBMPTextureScaled(SDL_Renderer *Renderer, const char *File, SDL_Texture* Tex, float ScaleX, float ScaleY, bool ApplyDither,int ditherversion, int whiteThresHold);
extern void SDL_SaveBMPTextureScaled(SDL_Renderer *Renderer, string File, SDL_Texture* Tex, float ScaleX, float ScaleY, bool ApplyDither, int ditherversion, int whiteThresHold);
//https://stackoverflow.com/questions/16605967/set-precision-of-stdto-string-when-converting-floating-point-values
template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << a_value;
	return out.str();
}
