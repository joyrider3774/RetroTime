#include <SDL.h>
#include <SDL_joystick.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <inttypes.h>
#include "games/CGameBase.h"
#include "games/CGameFrog.h"
#include "games/CGameFastEddy.h"
#include "games/CGameRamIt.h"
#include "games/CGameSnake.h"
#include "games/CGameBlockStacker.h"
#include "games/CGameBreakOut.h"
#include "games/CGamePang.h"
#include "games/CGameInvaders.h"

#include "CGame.h"
#include "Common.h"
#include "TitleScreen.h"
#include "SubScoreScreen.h"
#include "Intro.h"

using namespace std;

#undef LoadImage
#undef PlaySound

bool ShowFPS = false;
Uint8 GameAlpha = 0;
string DataPath;

Uint32 AlphaTimer;
Uint32 TimerTicks;
Uint64 Score;
int NextSubStateCounter, NextSubState, NextSubStateTimeAdd;

//for main (menu) background drawing
int menubackground, menubackgroundcounter, menubackgroundx, menubackgroundy, menubackgrounddx, menubackgrounddy;
int pinc;

SDL_Window *SdlWindow;
int ActiveGameGameStateId;
CGameSnake *GameSnake;
CGameBlockStacker *GameBlockStacker;
CGameFastEddy *GameFastEddy;
CGameBreakOut *GameBreakOut;
CGameInvaders *GameInvaders;
CGameRamIt *GameRamIt;
CGamePang * GamePang;
CGameFrog *GameFrog;
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

void CGame_DeInit()
{
	switch(ActiveGameGameStateId)
	{
		case GSSnake:
			GameSnake->deinit(GameSnake);
			Destroy_CGameSnake(GameSnake);
			break;
		case GSRamIt:
			GameRamIt->deinit(GameRamIt);
			Destroy_CGameRamIt(GameRamIt);
			break;
		case GSPang:
			GamePang->deinit(GamePang);
			Destroy_CGamePang(GamePang);
			break;
		case GSSpaceInvaders:
			GameInvaders->deinit(GameInvaders);
			Destroy_CGameInvaders(GameInvaders);
			break;
		case GSFrog:
			GameFrog->deinit(GameFrog);
			Destroy_CGameFrog(GameFrog);
			break;
		case GSEddy:
			GameFastEddy->deinit(GameFastEddy);
			Destroy_CGameFastEddy(GameFastEddy);
			break;
		case GSBreakout:
			GameBreakOut->deinit(GameBreakOut);
			Destroy_CGameBreakOut(GameBreakOut);
			break;
		case GSTetris:
			GameBlockStacker->deinit(GameBlockStacker);
			Destroy_CGameBlockStacker(GameBlockStacker);
			break;
		default:
			break;
	}
}

void CGame_Init()
{
	//Main State Variables and such
	CurrentGameMusicID = -1;
	GameState = GSIntroInit;
	Game = 0;
	GameMode = GMGame;
	GameAlpha = MaxAlpha;
	SubStateTime = 0;
	Timer = 0.0f;
	SubStateCounter = 0;
	TimerTicks = 0;
	SubGameState = SGNone;
	NextSubState = 0;
	NextSubStateCounter = 0;
	NextSubStateTimeAdd = 0;
	ActiveGameGameStateId = -1;

	//Clear score values
	CGame_ResetScores();

	//menubackground related
	pinc = ScreenWidth;
	menubackground = rand() % Games;
	menubackgroundcounter = 0;
	menubackgroundx = rand() % ScreenWidth;
	menubackgroundy = rand() % ScreenHeight;
	menubackgrounddx = rand() % 2 == 0 ? 1: -1;
	menubackgrounddy = rand() % 2 == 0 ? 1: -1;

	ActiveGameGameStateId = -1;
}

void CGame_ResetHighScores()
{
	for(int x = 0; x < Games; x++)
		for(int y = 0; y < Modes; y++)
			HighScores[x][y] = 0;
	RetroCarouselHighScore = 0;
}

void CGame_ResetScores()
{
	for(int x = 0; x < Games; x++)
		for(int y = 0; y < Modes; y++)
			Scores[x][y] = 0;
	RetroCarouselScore = 0;
}
void CGame_LoadMusic()
{
	MusMenu = CAudio_LoadMusic("main/music.ogg");
}

void CGame_UnLoadMusic()
{
	CAudio_UnloadMusics();
}

void CGame_UnLoadGraphics()
{
	CImage_UnloadImages();
}

void CGame_DrawTitleBackground()
{
	SDL_Rect Dst = {0, 0, ScreenWidth, ScreenHeight};

	SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Renderer, 0,0,0,255);
	SDL_RenderFillRect(Renderer, &Dst);

	// SDL_Point Pos = {ScreenWidth / 2, ScreenHeight / 2};
	// Vec2F Scale = {10.6f / 4, 10.6f};
	// CImage_DrawImageFuze(Renderer, GFXFrameID, &Pos, 0, &Scale, 255, 255, 255, 255);
}


void CGame_AddToScore(long long int Value)
{
	long long int AScore = Value;
	if (AScore < 0)
		if (GameMode == GMGame)
			AScore = 0;

	Scores[Game][GameMode] += AScore;
	if (Scores[Game][GameMode] < 0)
		Scores[Game][GameMode] = 0;
}

void CGame_LoadSound()
{
	SfxTimeOver = CAudio_LoadSound("common/timeover.wav");
	SfxReadyGo = CAudio_LoadSound("common/readygo.wav");
	SfxOne = CAudio_LoadSound("common/one.wav");
	SfxTwo = CAudio_LoadSound("common/two.wav");
	SfxThree = CAudio_LoadSound("common/three.wav");
	SfxOneMinute = CAudio_LoadSound("common/oneminute.wav");
	SfxConfirm = CAudio_LoadSound("main/confirm.wav");
	SfxBack = CAudio_LoadSound("main/back.wav");
	SfxSelect = CAudio_LoadSound("main/select.wav");
	SfxScore = CAudio_LoadSound("main/score.ogg");
}

void CGame_UnLoadSound()
{
	CAudio_UnloadSounds();
}

void CGame_LoadGraphics()
{
	GFXFrameID = CImage_LoadImage(Renderer, "main/frame.png");
	GFXMedal = CImage_LoadImage(Renderer, "main/medal.png");

	//SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/main/frame.bmp", CImage_GetImage(GFXFrameID), 1,1, true, 0, 40);
	//SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/main/medal.bmp", CImage_GetImage(GFXMedal), 1,1, true, 0, 160);
	CImage_UnLoadImage(GFXFrameID);
	CImage_UnLoadImage(GFXMedal);

	GFXFrameID = CImage_LoadImage(Renderer, "main/frame.bmp");
	GFXMedal = CImage_LoadImage(Renderer, "main/medal.bmp");
}

void CGame_ToggleFullscreen()
{

	//return;
	Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN_DESKTOP;
	bool IsFullscreen = SDL_GetWindowFlags(SdlWindow) & FullscreenFlag;
	// reset window size first before we go fullscreen
	// it will give more fps on the rpi if we for example scaled the window
	// first
	// if (!IsFullscreen)
	//	 SDL_SetWindowSize(SdlWindow, ScreenWidth, ScreenWidth);
	SDL_SetWindowFullscreen(SdlWindow, IsFullscreen ? 0 : FullscreenFlag);
	IsFullscreen = SDL_GetWindowFlags(SdlWindow) & FullscreenFlag;
	if (!IsFullscreen)
	{
		//SDL_SetWindowSize(SdlWindow, ScreenWidth, ScreenHeight);
		//SDL_SetWindowPosition(SdlWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}
	//SDL_ShowCursor(SDL_ENABLE);
}

void CGame_LoadHighScores()
{
	FILE *ScoreFile;
	string FileName = "./.retrotimesscores";

	char *EnvHome = getenv("HOME");
	char *EnvHomeDrive = getenv("HOMEDRIVE");
	char *EnvHomePath = getenv("HOMEPATH");

	if (EnvHome) //linux systems normally
		FileName = string(EnvHome) + "/.retrotimesscores";
	else
		if(EnvHomeDrive && EnvHomePath) //windows systems normally
			FileName = string(EnvHomeDrive) + string(EnvHomePath) + "/.retrotimesscores";

	ScoreFile = fopen(FileName.c_str(), "r");
	if (ScoreFile)
	{
		fscanf(ScoreFile, "RetroCarousel=%llu\n", &RetroCarouselHighScore);
		for (int i = 0; i < Games; i++)
			for (int j = 0; j < Modes; j++)
				fscanf(ScoreFile, string("Game_" + to_string(i) + "_Mode_" + to_string(j) + "=%llu\n").c_str(), &HighScores[i][j]);
		fclose(ScoreFile);
	}
	else
	{
		CGame_ResetHighScores();
	}
}

void CGame_SaveHighScores()
{
	FILE *ScoreFile;
	string FileName = "./.retrotimesscores";

	char *EnvHome = getenv("HOME");
	char *EnvHomeDrive = getenv("HOMEDRIVE");
	char *EnvHomePath = getenv("HOMEPATH");

	if (EnvHome) //linux systems normally
		FileName = string(EnvHome) + "/.retrotimesscores";
	else
		if(EnvHomeDrive && EnvHomePath) //windows systems normally
			FileName = string(EnvHomeDrive) + string(EnvHomePath) + "/.retrotimesscores";

	ScoreFile = fopen(FileName.c_str(), "w");
	if (ScoreFile)
	{
		fprintf(ScoreFile, "RetroCarousel=%llu\n", RetroCarouselHighScore);
		for (int i = 0; i < Games; i++)
			for (int j = 0; j < Modes; j++)
				fprintf(ScoreFile, string("Game_" + to_string(i) + "_Mode_" + to_string(j) + "=%llu\n").c_str(), HighScores[i][j]);
		fclose(ScoreFile);
	}
}

void CGame_LoadSettings()
{
	FILE *SettingsFile;
	string FileName = "./.retrotimesettings";

	char *EnvHome = getenv("HOME");
	char *EnvHomeDrive = getenv("HOMEDRIVE");
	char *EnvHomePath = getenv("HOMEPATH");

	if (EnvHome) //linux systems normally
		FileName = string(EnvHome) + "/.retrotimesettings";
	else
		if(EnvHomeDrive && EnvHomePath) //windows systems normally
			FileName = string(EnvHomeDrive) + string(EnvHomePath) + "/.retrotimesettings";

	SettingsFile = fopen(FileName.c_str(), "r");
	if (SettingsFile)
	{
		int VolumeMusic, VolumeSound;
		int ret = fscanf(SettingsFile, "VolumeMusic=%d\nVolumeSound=%d\n", 
			&VolumeMusic, &VolumeSound);

		if(ret > 0)
			CAudio_SetVolumeMusic(VolumeMusic);
		if(ret > 1)
			CAudio_SetVolumeSound(VolumeSound);
		fclose(SettingsFile);
	}
	else
	{
		CAudio_SetVolumeMusic(128);
		CAudio_SetVolumeSound(128);
	}
}

void CGame_SaveSettings()
{
	FILE *SettingsFile;
	string FileName = "./.retrotimesettings";

	char *EnvHome = getenv("HOME");
	char *EnvHomeDrive = getenv("HOMEDRIVE");
	char *EnvHomePath = getenv("HOMEPATH");

	if (EnvHome) //linux systems normally
		FileName = string(EnvHome) + "/.retrotimesettings";
	else
		if(EnvHomeDrive && EnvHomePath) //windows systems normally
			FileName = string(EnvHomeDrive) + string(EnvHomePath) + "/.retrotimesettings";

	SettingsFile = fopen(FileName.c_str(), "w");
	if (SettingsFile)
	{
		int VolumeMusic = CAudio_GetVolumeMusic();
		int VolumeSound = CAudio_GetVolumeSound();
		fprintf(SettingsFile, "VolumeMusic=%d\nVolumeSound=%d\n",
			VolumeMusic, VolumeSound);
		fclose(SettingsFile);
	}
}

void CGame_StartCrossFade(int SetGameState, int SetNextSubState, int SetNextSubStateCounter, Uint32 SetNextSubStateTimeAdd)
{
	AlphaTimer = SDL_GetTicks();
	GameAlpha = 0;
	SubGameState = SGFadeIn;
	GameState = SetGameState;
	NextSubState = SetNextSubState;
	NextSubStateTimeAdd = SetNextSubStateTimeAdd;
	NextSubStateCounter = SetNextSubStateCounter;
}

string CGame_GetFilePath(string InputFile)
{
	int Teller, Pos = 0;
	string Result = InputFile;
	for (Teller = InputFile.length() - 1; Teller >= 0; Teller--)
		if ((InputFile[Teller] == '/') || (InputFile[Teller] == '\\'))
		{
			Pos = Teller + 1;
			break;
		}
	Result.resize(Pos);
	return Result;
}

void CGame_ResetTimer()
{
	Timer = 120;
	if (GameMode == GMRetroCarousel)
	{
		Timer = 2 * 60;
	}
	if (GameMode == GMTimeAttack)
	{
		Timer = 5 * 60;
	}
	TimerTicks = SDL_GetTicks();
}

void CGame_UpdateTimer()
{
	if (TimerTicks + 250 < SDL_GetTicks())
	{
		TimerTicks = SDL_GetTicks();
		if (SubGameState == SGGame)
		{
			if (GameMode == GMGame)
				Timer += 0.25f;
			else
			{
				Timer -= 0.25f;

				if (Timer == 60)
					CAudio_PlaySound(SfxOneMinute, 0);

				if (Timer == 3)
					CAudio_PlaySound(SfxThree, 0);

				if (Timer == 2)
					CAudio_PlaySound(SfxTwo, 0);

				if (Timer == 1)
					CAudio_PlaySound(SfxOne, 0);


				if (Timer <= 0)
				{
					CAudio_PlaySound(SfxTimeOver, 0);
					SubGameState = SGTimeUp;
					SubStateTime = SDL_GetTicks() + 750;
					SubStateCounter = 0;
				}
			}
		}
	}
}

void CGame_CreateActiveGame()
{
	switch(ActiveGameGameStateId)
	{
		case GSSnake:
			GameSnake->deinit(GameSnake);
			Destroy_CGameSnake(GameSnake);
			ActiveGameGameStateId = -1;
			break;
		case GSRamIt:
			GameRamIt->deinit(GameRamIt);
			Destroy_CGameRamIt(GameRamIt);
			ActiveGameGameStateId = -1;
			break;
		case GSPang:
			GamePang->deinit(GamePang);
			Destroy_CGamePang(GamePang);
			ActiveGameGameStateId = -1;
			break;
		case GSSpaceInvaders:
			GameInvaders->deinit(GameInvaders);
			delete GameInvaders;
			ActiveGameGameStateId = -1;
			break;
		case GSFrog:
			GameFrog->deinit(GameFrog);
			Destroy_CGameFrog(GameFrog);
			ActiveGameGameStateId = -1;
			break;
		case GSEddy:
			GameFastEddy->deinit(GameFastEddy);
			Destroy_CGameFastEddy(GameFastEddy);
			ActiveGameGameStateId = -1;
			break;
		case GSBreakout:
			GameBreakOut->deinit(GameBreakOut);
			Destroy_CGameBreakOut(GameBreakOut);
			ActiveGameGameStateId = -1;
			break;
		case GSTetris:
			GameBlockStacker->deinit(GameBlockStacker);
			Destroy_CGameBlockStacker(GameBlockStacker);
			ActiveGameGameStateId = -1;
			break;
		default:			
			ActiveGameGameStateId = -1;
			break;
	}

	switch (GameState)
	{
		case GSSnakeInit:
			GameSnake = Create_CGameSnake();
			ActiveGameGameStateId = GSSnake;
			break;
		case GSTetrisInit:
			GameBlockStacker = Create_CGameBlockStacker();
			ActiveGameGameStateId = GSTetris;
			break;
		case GSRamItInit:
			GameRamIt = Create_CGameRamIt();
			ActiveGameGameStateId = GSRamIt;
			break;
		case GSEddyInit:
			GameFastEddy = Create_CGameFastEddy();
			ActiveGameGameStateId = GSEddy;
			break;
		case GSFrogInit:
			GameFrog = Create_CGameFrog();
			ActiveGameGameStateId = GSFrog;
			break;
		case GSBreakoutInit:
			GameBreakOut = Create_CGameBreakOut();
			ActiveGameGameStateId = GSBreakout;
			break;
		case GSPangInit:
			GamePang = Create_CGamePang();
			ActiveGameGameStateId = GSPang;
			break;
		case GSSpaceInvadersInit:
			GameInvaders = Create_CGameInvaders();
			ActiveGameGameStateId = GSSpaceInvaders;
			break;
		default:
			ActiveGameGameStateId = -1;
			break;
	}
}

void CGame_MainLoop()
{
	CGame_Init();
	Uint64 TotalFrames = 0;
	Uint64 TotalFramePerf = 0;
	Uint32 Fps = 0;
	double AvgFrameTime = 0.0f;
	Uint32 Ticks = SDL_GetTicks();
	uint8_t wtrs = 128;
	uint8_t ver = 2;
	
	string Text = "wtrs: " + std::to_string(wtrs) + "ver: " + std::to_string(ver) + "\n";
	printf("%s\n", Text.c_str());
		
	while (GameState != GSQuit)
	{
		TotalFrames++;
		Uint64 FrameStartPerf = SDL_GetPerformanceCounter();

		CGame_UpdateTimer();

		CInput_Update();

		if(CInput_Buttons.RenderReset)
		{
			SDL_Log("Render Reset, Reloading Game Graphics");
			CImage_UnloadImages();
			CGame_LoadGraphics();
			switch(ActiveGameGameStateId)
			{
				case GSPang:
					GamePang->LoadGraphics(GamePang);
					break;
				case GSSpaceInvaders:
					GameInvaders->LoadGraphics(GameInvaders);
					break;
				case GSFrog:
					GameFrog->LoadGraphics(GameFrog);
					break;
				case GSEddy:
					GameFastEddy->LoadGraphics(GameFastEddy);
					break;
				case GSBreakout:
					GameBreakOut->LoadGraphics(GameBreakOut);
					break;
				case GSTetris:
					GameBlockStacker->LoadGraphics(GameBlockStacker);
					break;
				default:
					break;
			}

			Sprites->ResetDrawTargets();
		}

		if(CInput_Buttons.ButLB && !CInput_PrevButtons.ButLB)
		{

			wtrs -= 1;
			Text = "wtrs: " + std::to_string(wtrs) + "ver: " + std::to_string(ver) + "\n";
			printf("%s\n", Text.c_str());
		}

		if(CInput_Buttons.ButRB && !CInput_PrevButtons.ButRB)
		{
			wtrs += 1;
			Text = "wtrs: " + std::to_string(wtrs) + "ver: " + std::to_string(ver) + "\n";
			printf("%s\n", Text.c_str());

		}

		if(CInput_Buttons.ButLT && !CInput_PrevButtons.ButLT)
		{
			if(ver == 2)
			{
				ver = 8;
			}
			else
			{
				if(ver == 8)
					ver = 4;
				else
					ver = 2;
			}
			Text = "wtrs: " + std::to_string(wtrs) + "ver: " + std::to_string(ver) + "\n";
			printf("%s\n", Text.c_str());
		}

		if(CInput_Buttons.ButRT && !CInput_PrevButtons.ButRT)
		{
			if(ver == 2)
			{
				ver = 4;
			}
			else
			{
				if(ver == 4)
					ver = 8;
				else
					ver = 2;
			}
			Text = "wtrs: " + std::to_string(wtrs) + "ver: " + std::to_string(ver) + "\n";
			printf("%s\n", Text.c_str());
		}



		if(CInput_Buttons.ButFullscreen && !CInput_PrevButtons.ButFullscreen)
			CGame_ToggleFullscreen();

		if(CInput_Buttons.ButQuit)
			GameState = GSQuit;

		SDL_SetRenderTarget(Renderer, TexOffScreen);

		switch (GameState)
		{
			case GSIntroInit:
			case GSIntro:
				Intro();
				break;

			case GSSubScoreInit:
			case GSSubScore:
				SubScoreScreen();
				break;

			case GSTitleScreenInit:
			case GSTitleScreen:
				//to clear the game data & set nullptr to ActiveGame
				CGame_CreateActiveGame();
				TitleScreen();
				break;

			case GSSnakeInit:
			case GSRamItInit:
			case GSPangInit:
			case GSSpaceInvadersInit:
			case GSFrogInit:
			case GSEddyInit:
			case GSBreakoutInit:
			case GSTetrisInit:
				CGame_CreateActiveGame();
				switch (ActiveGameGameStateId)
				{
					case GSSnake:
						GameSnake->init(GameSnake);
						break;
					case GSRamIt:
						GameRamIt->init(GameRamIt);
						break;
					case GSPang:
						GamePang->init(GamePang);
						break;
					case GSSpaceInvaders:
						GameInvaders->init(GameInvaders);
						break;
					case GSFrog:
						GameFrog->init(GameFrog);
						break;
					case GSEddy:
						GameFastEddy->init(GameFastEddy);
						break;
					case GSBreakout:
						GameBreakOut->init(GameBreakOut);
						break;
					case GSTetris:
						GameBlockStacker->init(GameBlockStacker);
						break;
				}
				CGame_ResetTimer();
				CGame_StartCrossFade(ActiveGameGameStateId, SGReadyGo, 3, 500);
				break;			
	
			case GSSnake:
			case GSRamIt:
			case GSPang:
			case GSSpaceInvaders:
			case GSFrog:
			case GSEddy:
			case GSBreakout:
			case GSTetris:
				switch (ActiveGameGameStateId)
				{
					case GSSnake:
						GameSnake->UpdateLogic(GameSnake);
						GameSnake->Draw(GameSnake);
						break;
					case GSRamIt:
						GameRamIt->UpdateLogic(GameRamIt);
						GameRamIt->Draw(GameRamIt);
						break;
					case GSPang:
						GamePang->UpdateLogic(GamePang);
						GamePang->Draw(GamePang);
						break;
					case GSSpaceInvaders:
						GameInvaders->UpdateLogic(GameInvaders);
						GameInvaders->Draw(GameInvaders);
						break;
					case GSFrog:
						GameFrog->UpdateLogic(GameFrog);
						GameFrog->Draw(GameFrog);
						break;
					case GSEddy:
						GameFastEddy->UpdateLogic(GameFastEddy);
						GameFastEddy->Draw(GameFastEddy);
						break;
					case GSBreakout:
						GameBreakOut->UpdateLogic(GameBreakOut);
						GameBreakOut->Draw(GameBreakOut);
						break;
					case GSTetris:
						GameBlockStacker->UpdateLogic(GameBlockStacker);
						GameBlockStacker->Draw(GameBlockStacker);
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
		if (GameAlpha < MaxAlpha)
		{
			GameAlpha = trunc(MaxAlpha * ((double)(SDL_GetTicks() - AlphaTimer) / MaxAlphaTime));
			if (GameAlpha >= MaxAlpha)
			{
				//SDL_SetTextureBlendMode(TexOffScreen, SDL_BLENDMODE_NONE);
				GameAlpha = MaxAlpha;
				SubGameState = NextSubState;
				SubStateTime = SDL_GetTicks() + NextSubStateTimeAdd;
				SubStateCounter = NextSubStateCounter;
			}
			else
			{
				SDL_SetTextureBlendMode(TexOffScreen, SDL_BLENDMODE_BLEND);
				SDL_SetTextureAlphaMod(TexOffScreen, GameAlpha);
			}
		}
		SDL_SetRenderTarget(Renderer, TexScreen);
		SDL_RenderCopy(Renderer, TexOffScreen, NULL, NULL);

		SDL_SetRenderTarget(Renderer, NULL);
		SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
		SDL_RenderClear(Renderer);

		int w, h, w2, h2, x, y;
		SDL_GetWindowSize(SdlWindow, &w , &h);
		float ScaleX = (float)w / (float)ScreenWidth;
		float ScaleY = (float)h / (float)ScreenHeight;
		h2 = ScreenHeight * ScaleY;
		w2 = ScreenWidth * ScaleY;
		if (w2 > w)
		{
			h2 = ScreenHeight * ScaleX;
			w2 = ScreenWidth * ScaleX;
		}
		x = ((w - w2) / 2);
		y = ((h - h2) / 2);

		SDL_Rect Rect = { x, y, w2, h2};
		//SDL_Rect DithRect = { x, y, w2, h2};

		SDL_SetRenderTarget(Renderer, NULL);

		SDL_RenderCopy(Renderer, TexScreen, NULL, &Rect);
		//ditherTarget(Renderer, SDL_GetRenderTarget(Renderer), &DithRect, ver, wtrs);

		//string Text = "wtrs: " + std::to_string(wtrs) + "ver: " + std::to_string(ver) + "\n";
		//CFont_WriteText(Renderer, "RobotoMono-Bold", 16, Text, Text.length(), 0, 0, 0, {255, 0, 255, 255});

		if (debugInfo || ShowFPS)
		{
			Text = "FPS: " + to_string(Fps) + "\n";
			if(debugInfo)
			{
				Text += "FrameTime: " + to_string(AvgFrameTime) + "\n";
				Text += "GFX Slots: " + to_string(CImage_ImageSlotsUsed()) + "/" + to_string(CImage_ImageSlotsMax()) + "\n";
				Text += "SND Slots: " + to_string(CAudio_SoundSlotsUsed()) + "/" + to_string(CAudio_SoundSlotsMax()) + "\n";
				Text += "MUS Slots: " + to_string(CAudio_MusicSlotsUsed()) + "/" + to_string(CAudio_MusicSlotsMax()) + "\n";
				Text += "SPR Slots: " + to_string(Sprites->SpriteSlotsUsed()) + "/" + to_string(Sprites->SpriteSlotsMax()) + "\n";
				Text += "SPR Resets: " + to_string(Sprites->UpdateImageResetsCount()) + "\n";
				Text += "SPR Draws: " + to_string(Sprites->SpritesDrawnCount()) + "\n";
				Text += "SCL Loaded: " + to_string(CImage_ScaledImagesLoadedCount()) + "\n";
				
			}
			int tw = CFont_TextWidth("RobotoMono-Bold", 14, Text, Text.length());
			CFont_WriteText(Renderer, "RobotoMono-Bold", 14, Text, Text.length(), w - tw, 0, 0, {255, 0, 255, 255});
		}
		SDL_RenderPresent(Renderer);

		Uint64 FrameEndPerf = SDL_GetPerformanceCounter();
		Uint64 FramePerf = FrameEndPerf - FrameStartPerf;
		double FrameTime = FramePerf / (double)SDL_GetPerformanceFrequency() * 1000.0;
		TotalFramePerf += FramePerf;

		if(SDL_GetTicks() - Ticks >= 1000)
		{
			AvgFrameTime = (TotalFramePerf / TotalFrames) / (double)SDL_GetPerformanceFrequency() * 1000.0;
			Fps = TotalFrames;
			TotalFrames = 0;
			TotalFramePerf = 0;
			Ticks = SDL_GetTicks();
		}

		if(DesiredFps > 0)
		{
			int RequiredDelay = 1000.0/DesiredFps - FrameTime;
			if (RequiredDelay > 0)
				SDL_Delay(RequiredDelay);
		}
	}
	CGame_DeInit();
}

void CGame_Run(int argc, char *argv[])
{
	bool useSoftwareRenderer = false;
	bool useLinear = false; //causes issues in for example frog from scaling textures and then bleeding into each other
	bool useVsync = false;
	bool useFullScreenAtStartup = true;
	string StartPath = CGame_GetFilePath(string(argv[0]));
	DataPath = StartPath + "retrotimefs/";
	int c;
	while ((c = getopt(argc, argv, "?dsfw")) != -1)
	{
		switch (c)
		{
			case '?':
				// i use sdl log because on windows printf did not show up
				SDL_Log("\nZnax Version 1.0.2\n\n\
Usage: Znax [Options]\n\n\
Possible options are:\n\
  -?: show this help message\n\
  -s: Use Software Renderer\n\
  -f: Show FPS\n\
  -w: Run windowed at startup (by default starts up fullscreen)\n");
				exit(0);
				break;
			case 'd':
				useLinear = false;
				break;
			case 's':
				useSoftwareRenderer = true;
				break;
			case 'f':
				ShowFPS = true;
				break;
			case 'w':
				useFullScreenAtStartup = false;
				break;
		}
	}

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) == 0)
	{
		string controllerdb = DataPath + "data/gamecontrollerdb.txt";
		if (SDL_GameControllerAddMappingsFromFile(controllerdb.c_str()) == -1)
			SDL_Log("Warning: Failed to load game controller mappings: %s", SDL_GetError());

		Uint32 WindowFlags = SDL_WINDOW_RESIZABLE;
		if (useFullScreenAtStartup)
		{
			WindowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		SdlWindow = SDL_CreateWindow("RetroTime", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowWidth, WindowHeight, WindowFlags);

		if (SdlWindow)
		{
			Uint32 flags = 0;
			if (useSoftwareRenderer)
				flags |= SDL_RENDERER_SOFTWARE;
			else
				flags |= SDL_RENDERER_ACCELERATED;
			if (useVsync)
				flags |= SDL_RENDERER_PRESENTVSYNC;

			SDL_Log("Succesfully Set %dx%d\n", WindowWidth, WindowHeight);
			Renderer = SDL_CreateRenderer(SdlWindow, -1, flags);
			if (Renderer)
			{
				SDL_RendererInfo rendererInfo;
				SDL_GetRendererInfo(Renderer, &rendererInfo);
				SDL_Log("Using Renderer:%s\n", rendererInfo.name);

				if (useLinear)
					SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

				SDL_Log("Succesfully Created Buffer\n");
				srand(time(NULL));

				CAudio_Init(DataPath, debugInfo);
				CFont_Init(DataPath, debugInfo);
				CImage_Init(DataPath, debugInfo);
				CInput_Init();
				Sprites = new CSprites();
				Sprites->SetForceShowCollisionShape(debugShowCollisionShapes);

				// Main game loop that loops untile the gamestate = GSQuit
				// and calls the procedure according to the gamestate.
				CGame_LoadSettings();
				CGame_LoadHighScores();
				CGame_LoadGraphics();
				CGame_LoadMusic();
				CGame_LoadSound();

				SDL_SetRenderTarget(Renderer, NULL);
				SDL_SetRenderDrawColor(Renderer, 0x00, 0x00, 0x00, 0xFF);
				SDL_RenderClear(Renderer);

				TexScreen = SDL_CreateTexture(Renderer, PixelFormat, SDL_TEXTUREACCESS_TARGET, ScreenWidth, ScreenHeight);
				SDL_SetRenderTarget(Renderer, TexScreen);
				SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
				SDL_RenderClear(Renderer);

				TexTmp = SDL_CreateTexture(Renderer, PixelFormat, SDL_TEXTUREACCESS_TARGET, ScreenWidth, ScreenHeight);
				SDL_SetTextureBlendMode(TexTmp, SDL_BLENDMODE_BLEND);
				SDL_SetRenderTarget(Renderer, TexTmp);
				SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
				SDL_RenderClear(Renderer);

				TexOffScreen = SDL_CreateTexture(Renderer, PixelFormat, SDL_TEXTUREACCESS_TARGET, ScreenWidth, ScreenHeight);
				SDL_SetRenderTarget(Renderer, TexOffScreen);
				SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
				SDL_RenderClear(Renderer);

				SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);

				CGame_MainLoop();

				SDL_DestroyTexture(TexOffScreen);
				SDL_DestroyTexture(TexScreen);
				SDL_DestroyTexture(TexTmp);
				CGame_UnLoadMusic();
				CGame_UnLoadGraphics();
				CGame_UnLoadSound();
				CGame_SaveSettings();
				CGame_SaveHighScores();

				CAudio_DeInit();
				CFont_DeInit();
				CImage_DeInit();
				CInput_DeInit();
				delete Sprites;
				SDL_DestroyRenderer(Renderer);
			}
			else
			{
				SDL_Log("Failed to create renderer: %s\n", SDL_GetError());
			}
		}
		else
		{
			SDL_Log("Failed to Set Videomode (%dx%d): %s\n", ScreenWidth, ScreenHeight, SDL_GetError());
		}
		SDL_DestroyWindow(SdlWindow);
		SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
		SDL_Quit();
	}
	else
		SDL_Log("Failed to initialise SDL: %s\n", SDL_GetError());
}


