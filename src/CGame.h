#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <limits.h>
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

class CGameRamIt;
class CGameSnake;
class CGameBlockStacker;
class CGameBase;

class CGame {

private:    
    SDL_Window *SdlWindow;
    string DataPath;

    bool ShowFPS = false;
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
    CGameBase *ActiveGame;
    CAudio *Audio;
    CFont *Font;
    CInput *Input;
    CImage *Image;
    CSprites *Sprites;
    SDL_Renderer *Renderer;
    SDL_Texture *TexOffScreen, *TexScreen, *TexCrt;
    SDL_Texture* GameScreenShots[Games];
    long long int RetroCarouselHighScore, RetroCarouselScore;
    long long int HighScores[Games][Modes]; 
	long long int Scores[Games][Modes];
    int GameState, SubGameState, GameMode, Game, Crt;
    float SubStateCounter;
    int MusMenu;
    int SfxConfirm, SfxBack, SfxSelect, SfxScore;
    double Timer;
    Uint32 SubStateTime;
    bool MotionBlur;
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
    void ResetTimer();
    void ResetScores();
    void ResetHighScores();
    void AddToScore(long long int Value);
    void DrawTitleBackground(bool k);
    string GetFilePath(string InputFile);
    void DrawCrt();
    void Run(int argc, char **argv);
    void ReCreateCrt();
    void CreateScreenshotsAndBackground();
    void StartCrossFade(int SetGameState, int SetNextSubState, int SetNextSubStateCounter, Uint32 SetNextSubStateTimeAdd);
    void ToggleFullscreen();
};

