#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include "Platform.h"
#include "Common.h"
#include "CAudio.h"
#include "CInput.h"
#include "CFont.h"
#include "CImage.h"
#include "CSprites.h"
#include "CHttp.h"

class CGameRamIt;
class CGameSnake;
class CGameBlockStacker;
class CGameBase;

class CGame {

private:
	std::string DataPath;

	Uint8 Alpha = 0;
	Uint32 AlphaTimer;
	Uint32 TimerTicks;
	Uint64 Score;
	int NextSubStateCounter, NextSubState, NextSubStateTimeAdd;
	//for main (menu) background drawing
	int menubackground, menubackgroundcounter, menubackgroundx, menubackgroundy, menubackgrounddx, menubackgrounddy;
	int pinc;
	SDL_Texture* ScreenShotRandom;

	SDL_Texture* RandomScreenshot(double Scale);
	void UpdateTimer();
	void MainLoop();
	void Init();
	void DeInit();
	void CreateCrt(int type);
	void CreateActiveGame();
public:
	CHttp *http;
	SDL_Window *SdlWindow;
	bool ShowFPS = false;
	CGameBase *ActiveGame;
	CAudio *Audio;
	CFont *Font;
	CInput *Input;
	CImage *Image;
	CSprites *Sprites;
	SDL_Renderer *Renderer;
	SDL_Texture *TexOffScreen, *TexScreen, *TexCrt, *TexTmp;
	SDL_Texture* GameScreenShots[Games];
	long long int RetroCarouselHighScore, RetroCarouselScore;
	long long int HighScores[Games][Modes];
	long long int Scores[Games][Modes];
	int GameState, SubGameState, GameMode, Game, Crt;
	char HighScoreName[16];
	float SubStateCounter;
	int MusMenu;
	int SfxConfirm, SfxBack, SfxSelect, SfxScore, SfxOne, SfxTwo, SfxThree, SfxTimeOver, SfxReadyGo, SfxOneMinute;
	double Timer;
	Uint32 SubStateTime;
	bool MotionBlur;
	int GFXFrameID, GFXMedal;
	int CurrentGameMusicID;
	int ColorModR=255, ColorModG=255, ColorModB=255;
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
	void DrawTitleBackground(bool k);
	std::string GetFilePath(std::string InputFile);
	void DrawCrt();
	void Run(int argc, char **argv);
	void ReCreateCrt();
	void CreateScreenshotsAndBackground();
	void StartCrossFade(int SetGameState, int SetNextSubState, int SetNextSubStateCounter, Uint32 SetNextSubStateTimeAdd);
	void ToggleFullscreen();
};
