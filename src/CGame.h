#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include "Platform.h"
#include "CAudio.h"
#include "CInput.h"
#include "CFont.h"
#include "CImage.h"
#include "CSprites.h"
#include "Common.h"


using namespace std;

struct CGameRamIt;
class CGameSnake;
struct CGameBlockStacker;
struct CGameFastEddy;
struct CGameBreakOut;
struct CGameBase;
struct CGameFrog;
struct CGameInvaders;
struct CGamePang;

class CGame {

private:
	string DataPath;

	Uint8 Alpha = 0;
	Uint32 AlphaTimer;
	Uint32 TimerTicks;
	Uint64 Score;
	int NextSubStateCounter, NextSubState, NextSubStateTimeAdd;

	//for main (menu) background drawing
	int menubackground, menubackgroundcounter, menubackgroundx, menubackgroundy, menubackgrounddx, menubackgrounddy;
	int pinc;

	void UpdateTimer();
	void MainLoop();
	void Init();
	void DeInit();
	void CreateActiveGame();
public:
	SDL_Window *SdlWindow;
	bool ShowFPS = false;
	int ActiveGameGameStateId;
	CGameSnake *GameSnake;
	CGameBlockStacker *GameBlockStacker;
	CGameFastEddy *GameFastEddy;
	CGameBreakOut *GameBreakOut;
	CGameInvaders *GameInvaders;
	CGameRamIt *GameRamIt;
	CGamePang * GamePang;
	CGameFrog *GameFrog;
	CAudio *Audio;
	CFont *Font;
	CInput *Input;
	CImage *Image;
	CSprites *Sprites;
	SDL_Renderer *Renderer;
	SDL_Texture *TexOffScreen, *TexScreen, *TexTmp;
	long long int RetroCarouselHighScore, RetroCarouselScore;
	long long int HighScores[Games][Modes];
	long long int Scores[Games][Modes];
	int GameState, SubGameState, GameMode, Game;
	float SubStateCounter;
	int MusMenu;
	int SfxConfirm, SfxBack, SfxSelect, SfxScore, SfxOne, SfxTwo, SfxThree, SfxTimeOver, SfxReadyGo, SfxOneMinute;
	float Timer;
	Uint32 SubStateTime;
	int GFXFrameID, GFXMedal;
	int CurrentGameMusicID;
	CGame();
	~CGame();

	void InitSubScore();
	void SubScore();
	void LoadSound();
	void UnLoadSound();
	void LoadMusic();
	void UnLoadMusic();
	void UnLoadGraphics();
	void LoadGraphics();
	void LoadSettings();
	void SaveSettings();
	void LoadHighScores();
	void SaveHighScores();
	void ResetTimer();
	void ResetScores();
	void ResetHighScores();
	void AddToScore(long long int Value);
	void DrawTitleBackground();
	string GetFilePath(string InputFile);
	void Run(int argc, char **argv);
	void StartCrossFade(int SetGameState, int SetNextSubState, int SetNextSubStateCounter, Uint32 SetNextSubStateTimeAdd);
	void ToggleFullscreen();
};
