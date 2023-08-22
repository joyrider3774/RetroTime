#ifndef CGAME_H
#define CGAME_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string.h>
#include <stdbool.h>
#include "Platform.h"
#include "CAudio.h"
#include "CInput.h"
#include "CFont.h"
#include "CImage.h"
#include "CSprites.h"
#include "Common.h"

typedef struct CGameRamIt CGameRamIt;
typedef struct CGameSnake CGameSnake;
typedef struct CGameBlockStacker CGameBlockStacker;
typedef struct CGameFastEddy CGameFastEddy;
typedef struct CGameBreakOut CGameBreakOut;
typedef struct CGameBase CGameBase;
typedef struct CGameFrog CGameFrog;
typedef struct CGameInvaders CGameInvaders;
typedef struct CGamePang CGamePang;

extern Uint8 GameAlpha;
extern Uint32 AlphaTimer;
extern Uint32 TimerTicks;
extern Uint64 Score;
extern int NextSubStateCounter, NextSubState, NextSubStateTimeAdd;

//for main (menu) background drawing
extern int menubackground, menubackgroundcounter, menubackgroundx, menubackgroundy, menubackgrounddx, menubackgrounddy;
extern int pinc;

extern SDL_Window *SdlWindow;
extern bool ShowFPS;
extern int ActiveGameGameStateId;
extern CGameSnake *GameSnake;
extern CGameBlockStacker *GameBlockStacker;
extern CGameFastEddy *GameFastEddy;
extern CGameBreakOut *GameBreakOut;
extern CGameInvaders *GameInvaders;
extern CGameRamIt *GameRamIt;
extern CGamePang * GamePang;
extern CGameFrog *GameFrog;
extern SDL_Renderer *Renderer;
extern SDL_Texture *TexOffScreen, *TexScreen, *TexTmp;
extern long long int RetroCarouselHighScore, RetroCarouselScore;
extern long long int HighScores[Games][Modes];
extern long long int Scores[Games][Modes];
extern int GameState, SubGameState, GameMode, Game;
extern float SubStateCounter;
extern int MusMenu;
extern int SfxConfirm, SfxBack, SfxSelect, SfxScore, SfxOne, SfxTwo, SfxThree, SfxTimeOver, SfxReadyGo, SfxOneMinute;
extern float Timer;
extern Uint32 SubStateTime;
extern int GFXFrameID, GFXMedal;
extern int CurrentGameMusicID;


void CGame_UpdateTimer();
void CGame_MainLoop();
void CGame_Init();
void CGame_DeInit();
void CGame_CreateActiveGame();


void CGame_InitSubScore();
void CGame_SubScore();
void CGame_LoadSound();
void CGame_UnLoadSound();
void CGame_LoadMusic();
void CGame_UnLoadMusic();
void CGame_UnLoadGraphics();
void CGame_LoadGraphics();
void CGame_LoadSettings();
void CGame_SaveSettings();
void CGame_LoadHighScores();
void CGame_SaveHighScores();
void CGame_ResetTimer();
void CGame_ResetScores();
void CGame_ResetHighScores();
void CGame_AddToScore(long long int Value);
void CGame_DrawTitleBackground();
void CGame_GetFilePath(char* InputFile, char** Result);
void CGame_Run(int argc, char **argv);
void CGame_StartCrossFade(int SetGameState, int SetNextSubState, int SetNextSubStateCounter, Uint32 SetNextSubStateTimeAdd);
void CGame_ToggleFullscreen();

#endif