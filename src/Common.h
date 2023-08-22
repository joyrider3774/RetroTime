#ifndef COMMON_H
#define COMMON_H

#include <string.h>
#include <SDL.h>
#include <stdbool.h>

#define debugInfo true
#define debugShowCollisionShapes false
#define dumpScaledBitmaps false
#define useDefaultColorAssets false
#define loadDumpedScaledBitmaps true

#define epsilion 0.0001f

#define titleAlphaEnabled true

#define MaxAlpha 245
#define AlphaIncrease 5
#define MaxAlphaTime 750

#define ScreenWidth 400
#define ScreenHeight 240
#define WindowWidth 400
#define WindowHeight 240
#define xscale (ScreenWidth / 1280.0f)
#define yscale (ScreenHeight / 720.0f)

#define PixelFormat SDL_PIXELFORMAT_ARGB8888

#define DesiredFps 60.0

#define ScoreBarHeight (24*yscale)

//GameStates
#define initDiff 50

#define GSQuit 0
#define GSIntro 1
#define GSGame 2
#define GSTitleScreen 3
#define GSRamIt 4
#define GSSubScore 5
#define GSSpaceInvaders 6
#define GSBreakout 7
#define GSFrog 8
#define GSSnake 9
#define GSPang 10
#define GSTetris 11
#define GSEddy 12

#define GSIntroInit GSIntro + initDiff
#define GSGameInit GSGame + initDiff
#define GSTitleScreenInit GSTitleScreen + initDiff
#define GSRamItInit GSRamIt + initDiff
#define GSSubScoreInit GSSubScore + initDiff

#define GSSpaceInvadersInit GSSpaceInvaders + initDiff
#define GSBreakoutInit GSBreakout + initDiff
#define GSFrogInit GSFrog + initDiff
#define GSSnakeInit GSSnake + initDiff
#define GSPangInit GSPang + initDiff
#define GSTetrisInit GSTetris + initDiff
#define GSEddyInit GSEddy + initDiff

//SubGameStates
#define SGNone 0
#define SGReadyGo 1
#define SGGame 2
#define SGTimeUp 3
#define SGFrame 4
#define SGCalcScore 5
#define SGFadeIn 6
#define SGPauseMenu 7
#define SGGameHelp 8

//GameModes
#define Modes 3
#define GMRetroCarousel 0
#define GMGame 1
#define GMTimeAttack 2

#define Games 8

//PAUSEMENUS
#define PMNone -1
#define PMContinue 0
#define PMQuit 1
#define PMMusicVol 2
#define PMSoundVol 3
#define PMGameHelp 4


#define PauseMenus 10

//main menus
#define MMStart 0
#define MMHighScores 1
#define MMHelp 2
#define MMQuit 3
#define MMOptions 4
#define MMCredits 5
//Sub Main Menus
#define MMSelectGameMode 7
#define MMSelectGame 8
//6 because only fist 6 are real main menu
#define MainMenus 6

//options menu
#define OMMusicVol 0
#define OMSoundVol 1
#define OMResetHighScores 2
#define OMBack 3
#define OptionMenus 4

struct ModeDesc {
	int mode;
	char* name;
	char* description;
};
typedef struct ModeDesc ModeDesc;

struct GameDesc {
	int state;
	char* name;
	char* description;
	char* shortdescription;
};
typedef struct GameDesc GameDesc;

struct PauseMenuDesc {
	int menu;
	char* name;
};
typedef struct PauseMenuDesc PauseMenuDesc;

struct GPGamePauseMenusDesc {
	int game;
	int menus[5];
};
typedef struct GPGamePauseMenusDesc GPGamePauseMenusDesc;

struct MainMenusDesc {
	int menu;
	char* name;
};
typedef struct MainMenusDesc MainMenusDesc;

struct OptionMenusDesc {
	int menu;
	char* name;
};
typedef struct OptionMenusDesc OptionMenusDesc;

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
extern Uint32 SDL_GetPixel(SDL_Surface *surface, int x, int y);
extern void SDL_PutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
extern void SDL_SaveBMPTextureScaled(SDL_Renderer *Renderer, const char *File, SDL_Texture* Tex, float ScaleX, float ScaleY, bool ApplyDither,int ditherversion, int whiteThresHold);

#endif