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

CGame::CGame() 
{
   
}

CGame::~CGame() 
{

}

void CGame::DeInit()
{
    if (ActiveGame != nullptr)
    {
        ActiveGame->deinit();
        delete ActiveGame;
    }
    for(int i = 0; i < Games; i++)
        SDL_DestroyTexture(GameScreenShots[i]);
    SDL_DestroyTexture(ScreenShotRandom);
}

void CGame::Init()
{
    //Main State Variables and such
    CurrentGameMusicID = -1;
    GameState = GSIntroInit;
    Game = 0;
    GameMode = GMGame;
    Alpha = MaxAlpha;
    SubStateTime = 0;
    Timer = 0.0f;
    SubStateCounter = 0;
    TimerTicks = 0;
    SubGameState = SGNone;
    NextSubState = 0; 
    NextSubStateCounter = 0;
    NextSubStateTimeAdd = 0;
    TexCrt = nullptr;
    ActiveGame = nullptr;
    ReCreateCrt();
    
    //Clear score values
    ResetScores();

    //menubackground related
    pinc = ScreenWidth;
    menubackground = rand() % Games;
    menubackgroundcounter = 0;
    menubackgroundx = rand() % ScreenWidth;
    menubackgroundy = rand() % ScreenHeight;
    menubackgrounddx = rand() % 2 == 0 ? 1: -1;
    menubackgrounddy = rand() % 2 == 0 ? 1: -1;

    ActiveGame = nullptr;
    for (int i = 0; i < Games; i++)
        GameScreenShots[i] = nullptr;
    ScreenShotRandom = nullptr;

    CreateScreenshotsAndBackground();    
}

void CGame::CreateScreenshotsAndBackground()
{   
    //need to create a temporary sprites holder & replace it because
    //games could be running and already have sprites assigned
    CSprites *TmpSprites = Sprites;
    Sprites = new CSprites(Image);
    CGameRamIt *TmpGameRamIt = new CGameRamIt(this, true);
    CGameBlockStacker *TmpGameBlockStacker = new CGameBlockStacker(this, true);
    CGameSnake *TmpGameSnake = new CGameSnake(this, true);
    CGameFastEddy *TmpGameFastEddy = new CGameFastEddy(this, true);
    CGameFrog *TmpGameFrog = new CGameFrog(this, true);
    CGameBreakOut *TmpGameBreakOut = new CGameBreakOut(this, true);
    CGamePang *TmpGamePang = new CGamePang(this,true);
    CGameInvaders *TmpGameInvaders = new CGameInvaders(this,true);
    int ScreenShotNr = 0;
    SDL_DestroyTexture(GameScreenShots[ScreenShotNr]);
    GameScreenShots[ScreenShotNr++] = TmpGameInvaders->screenshot();
    SDL_DestroyTexture(GameScreenShots[ScreenShotNr]);
    GameScreenShots[ScreenShotNr++] = TmpGameBreakOut->screenshot();
    SDL_DestroyTexture(GameScreenShots[ScreenShotNr]);
    GameScreenShots[ScreenShotNr++] = TmpGameFrog->screenshot();
    SDL_DestroyTexture(GameScreenShots[ScreenShotNr]);
    GameScreenShots[ScreenShotNr++] = TmpGameSnake->screenshot();
    SDL_DestroyTexture(GameScreenShots[ScreenShotNr]);
    GameScreenShots[ScreenShotNr++] = TmpGamePang->screenshot();
    SDL_DestroyTexture(GameScreenShots[ScreenShotNr]);
    GameScreenShots[ScreenShotNr++] = TmpGameBlockStacker->screenshot();
    SDL_DestroyTexture(GameScreenShots[ScreenShotNr]);
    GameScreenShots[ScreenShotNr++] = TmpGameRamIt->screenshot();
    SDL_DestroyTexture(GameScreenShots[ScreenShotNr]);
    GameScreenShots[ScreenShotNr++] = TmpGameFastEddy->screenshot();
    SDL_DestroyTexture(ScreenShotRandom);
    ScreenShotRandom = RandomScreenshot(0.25);
    delete TmpGameInvaders;
    delete TmpGamePang;
    delete TmpGameBreakOut;
    delete TmpGameFrog;
    delete TmpGameRamIt;
    delete TmpGameBlockStacker;
    delete TmpGameSnake;
    delete TmpGameFastEddy;
    delete Sprites;
    Sprites = TmpSprites;
}

void CGame::ResetHighScores()
{
	for(int x = 0; x < Games; x++)
		for(int y = 0; y < Modes; y++)
			HighScores[x][y] = 0;
	RetroCarouselHighScore = 0;
}

void CGame::ResetScores()
{
	for(int x = 0; x < Games; x++)
		for(int y = 0; y < Modes; y++)
			Scores[x][y] = 0;
	RetroCarouselScore = 0;
}
void CGame::LoadMusic()
{
    MusMenu = Audio->LoadMusic("main/music.ogg");
}

void CGame::UnLoadMusic()
{
    Audio->UnloadMusics();
}

void CGame::UnLoadGraphics() 
{
    Image->UnloadImages();
}

void CGame::DrawTitleBackground(bool k)
{
	string msg = "Happy Easter (egg) to everyone on Fuze Arena and thanks to the FUZE Team and Wireframe magazine for doing this competition!!!!";
	menubackgroundx += menubackgrounddx;
	menubackgroundy += menubackgrounddy;
	if ((menubackgroundx == 0) || (menubackgroundx == ScreenWidth))
		menubackgrounddx *= -1;

	if ((menubackgroundy == 0) || (menubackgroundy == ScreenHeight))
		menubackgrounddy *= -1;

	SDL_Rect Src = {menubackgroundx, menubackgroundy, ScreenWidth, ScreenHeight};
    SDL_Rect Dst = {0, 0, ScreenWidth, ScreenHeight};
    SDL_RenderCopy(Renderer, ScreenShotRandom, &Src, &Dst);
    
    SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(Renderer, 0,0,0,128);
    SDL_RenderFillRect(Renderer, &Dst);
    
    // //DrawCrt();
    
    // SDL_Point Pos = {ScreenWidth / 2, ScreenHeight / 2};
    // Vec2F Scale = {10.6f / 4, 10.6f};
    // Image->DrawImageFuze(Renderer, GFXFrameID, &Pos, 0, &Scale, 255, 255, 255, 255);

    if (k)
    {
	    for(size_t i = 0; i < msg.length(); i++)
        {
            SDL_Color color = {255, Uint8(128 + (sin(i*25 % 360))*127), 0, 64*3};
            string T = msg.substr(i, 1);
            Font->WriteText(Renderer, "Roboto-Regular", 100, T, T.length(), pinc + i * 75, ((ScreenHeight / 2) -100) + (sin((((pinc + (i*15)) % 360)*3.14159265/180)) * 200), 0, color);
        }
		pinc = pinc- 3;
		if (pinc < int(msg.length() * -75) )
        {
		    pinc = ScreenWidth;
        }
    }
}

SDL_Texture* CGame::RandomScreenshot(double Scale)
{
	SDL_Texture* prev = SDL_GetRenderTarget(Renderer);
    SDL_Texture* image = SDL_CreateTexture(Renderer, PixelFormat, SDL_TEXTUREACCESS_TARGET, ScreenWidth*2, ScreenHeight*2);
    SDL_SetRenderTarget(Renderer, image);
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Renderer);
	int x = 0;
	int y = 0;
	int r = rand() % Games;
	int prevr = r;

	while (y < ScreenHeight * 2)
    {
		x = 0;
		while(r == prevr)
			r = rand() % Games;
		prevr = r;
		
        while (x < ScreenWidth * 2)
        {
			r += 1;
			if (r == Games)
				r = 0;
            SDL_Rect Dst = {x, y, int(ScreenWidth * Scale), int(ScreenHeight * Scale)};
            SDL_Rect Src = {0, 0, ScreenWidth, ScreenHeight};
			SDL_RenderCopy(Renderer, GameScreenShots[r], &Src, &Dst);
			x += int(ScreenWidth * Scale);
        }
		y += int(ScreenHeight * Scale);
    }
    SDL_SetRenderTarget(Renderer, prev);	
    return image;
}	

void CGame::AddToScore(long long int Value)
{
	long long int AScore = Value;
	if (AScore < 0) 
		if (GameMode == GMGame)
			AScore = 0;
	
	Scores[Game][GameMode] += AScore;
	if (Scores[Game][GameMode] < 0)
		Scores[Game][GameMode] = 0;
}

void CGame::DrawCrt()
{   
    int w,h;
    SDL_GetWindowSize(SdlWindow, &w , &h); 
    SDL_Point size;
    SDL_QueryTexture(TexCrt, NULL, NULL, &size.x, &size.y);
    if ((w != size.x) || (h != size.y))
        ReCreateCrt();

    //don't put it over scorebar
    int w2;
    float ScaleX = (float)w / (float)ScreenWidth;
    float ScaleY = (float)h / (float)ScreenHeight;
    int ScaledScoreBarHeight = (ScoreBarHeight) * ScaleY;
    w2 = ScreenWidth * ScaleY;
    if (w2 > w)
        ScaledScoreBarHeight = (ScoreBarHeight) * ScaleX;

    SDL_Rect Rect = {0,ScaledScoreBarHeight,w,h-ScaledScoreBarHeight};
    SDL_RenderCopy(Renderer, TexCrt, &Rect, &Rect);
}


void CGame::CreateCrt(int type)
{
	SDL_Texture* prev = SDL_GetRenderTarget(Renderer);
    SDL_DestroyTexture(TexCrt);
    int w,h;
    SDL_GetWindowSize(SdlWindow, &w , &h);
    printf("CreateCrt w: %d h:%d\n",w,h);
    TexCrt = SDL_CreateTexture(Renderer, PixelFormat, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetTextureBlendMode(TexCrt, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(Renderer, TexCrt);
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 0);
    SDL_RenderClear(Renderer);
	if (type > 0)
    {	
		if (type < 4)
        {
			int steps = 2;
			if (type == 1)
				steps = 2;
				
			if (type == 2)
				steps = 4;
			
			if (type == 3)
				steps = 8;

            SDL_SetRenderDrawColor(Renderer, 0,0,0, 255);
			for(int y = 0; y < h; y+=steps)
                SDL_RenderDrawLine(Renderer, 0, y, w, y);
        }
		else
        {
		    if(type < 5) //dots
            {
			    int size = 2;
			    int xsteps = 1;
			    int ysteps = 1;
                SDL_Texture* img = SDL_CreateTexture(Renderer, PixelFormat, SDL_TEXTUREACCESS_TARGET, w, ysteps+size);
                SDL_SetRenderTarget(Renderer, img);
                SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 0);
                SDL_RenderClear(Renderer);
                SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
			    for(int x = 0; x < w + xsteps + size; x+= xsteps + size)
                {
                    SDL_RenderDrawPoint(Renderer, x, 0);
				    SDL_RenderDrawPoint(Renderer, x+1, 0);
                    SDL_RenderDrawPoint(Renderer, x, 1);
                    SDL_RenderDrawPoint(Renderer, x+1, 1);
                }
            
			    SDL_SetRenderTarget(Renderer, TexCrt);
			    for(int y = 0; y < h + ysteps + size; y+= ysteps + size)
				{
                    SDL_Rect r = {0, y, w, ysteps + size};
                    SDL_RenderCopy(Renderer, img, NULL, &r);
                }
			    SDL_DestroyTexture(img);
            }
		    else		
            {
                if  (type < 6)  //cross
                {
                    int size = 3;
                    int xsteps = 1;
                    int ysteps = 1;
                    SDL_Texture* img = SDL_CreateTexture(Renderer, PixelFormat, SDL_TEXTUREACCESS_TARGET, w, ysteps+size);
                    SDL_SetRenderTarget(Renderer, img);
                    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 0);
                    SDL_RenderClear(Renderer);
                    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
                    
                    for(int x = 0; x < w + xsteps + size; x+= xsteps + size)
                    {
                        SDL_RenderDrawLine(Renderer, x + 1, 1, x + size, size);
                        SDL_RenderDrawLine(Renderer, x + 1, size, x + size, 1);
                    }
                    SDL_SetRenderTarget(Renderer, TexCrt);
                    for(int y = 0; y < h +  ysteps + size; y+= ysteps + size)
                    {
                        SDL_Rect r = {0, y, w, ysteps + size};
                        SDL_RenderCopy(Renderer, img, NULL, &r);
                    }
                    SDL_DestroyTexture(img);
                }
            }
        }
    }
	SDL_SetRenderTarget(Renderer, prev);
}

void CGame::ReCreateCrt()
{
    CreateCrt(Crt);
}

void CGame::LoadSound()
{
    SfxTimeOver = Audio->LoadSound("common/timeover.wav");
    SfxReadyGo = Audio->LoadSound("common/readygo.wav");
    SfxOne = Audio->LoadSound("common/one.wav");
    SfxTwo = Audio->LoadSound("common/two.wav");
    SfxThree = Audio->LoadSound("common/three.wav");
    SfxOneMinute = Audio->LoadSound("common/oneminute.wav");
    SfxConfirm = Audio->LoadSound("main/confirm.wav");
    SfxBack = Audio->LoadSound("main/back.wav"); 
    SfxSelect = Audio->LoadSound("main/select.wav");
    SfxScore = Audio->LoadSound("main/score.ogg");
}

void CGame::UnLoadSound()
{
    Audio->UnloadSounds();
}

void CGame::LoadGraphics() {
    GFXFrameID = Image->LoadImage(Renderer, "main/frame.png");
    GFXMedal = Image->LoadImage(Renderer, "main/medal.png");
}

void CGame::ToggleFullscreen() 
{
    
    //return;
    Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN_DESKTOP;
    bool IsFullscreen = SDL_GetWindowFlags(SdlWindow) & FullscreenFlag;
    // reset window size first before we go fullscreen
    // it will give more fps on the rpi if we for example scaled the window
    // first    
  //  if (!IsFullscreen)
   //     SDL_SetWindowSize(SdlWindow, ScreenWidth, ScreenWidth);
    SDL_SetWindowFullscreen(SdlWindow, IsFullscreen ? 0 : FullscreenFlag);
    IsFullscreen = SDL_GetWindowFlags(SdlWindow) & FullscreenFlag;
    if (!IsFullscreen) 
    {
        //SDL_SetWindowSize(SdlWindow, ScreenWidth, ScreenHeight);
        //SDL_SetWindowPosition(SdlWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    }
    //SDL_ShowCursor(SDL_ENABLE);
    //ScreenShotRandom = RandomScreenshot(0.25);
}

void CGame::LoadHighScores() 
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
        ResetHighScores();
    }
}

void CGame::SaveHighScores() {
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

void CGame::LoadSettings() {
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
        int VolumeMusic, VolumeSound, aMotionBlur;
        fscanf(SettingsFile, "VolumeMusic=%d\nVolumeSound=%d\nCrt=%d\nSpriteGhosting=%d\n", &VolumeMusic, &VolumeSound, &Crt, &aMotionBlur);
        MotionBlur = (aMotionBlur == 1);
        Audio->SetVolumeSound(VolumeSound);
        Audio->SetVolumeMusic(VolumeMusic);
        fclose(SettingsFile);
    } 
    else
    {
        Audio->SetVolumeMusic(128);
        Audio->SetVolumeSound(128);
        MotionBlur = false;
        Crt = 0;
    }
}

void CGame::SaveSettings() {
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
        int VolumeMusic = Audio->GetVolumeMusic();
        int VolumeSound = Audio->GetVolumeSound();
        fprintf(SettingsFile, "VolumeMusic=%d\nVolumeSound=%d\nCrt=%d\nSpriteGhosting=%d\n", VolumeMusic, VolumeSound, Crt, MotionBlur?1:0);
        fclose(SettingsFile);
    }
}

void CGame::StartCrossFade(int SetGameState, int SetNextSubState, int SetNextSubStateCounter, Uint32 SetNextSubStateTimeAdd)
{
    AlphaTimer = SDL_GetTicks();
    Alpha = 0;
    SubGameState = SGFadeIn;
    GameState = SetGameState;
    NextSubState = SetNextSubState;
    NextSubStateTimeAdd = SetNextSubStateTimeAdd;
    NextSubStateCounter = SetNextSubStateCounter;
}

string CGame::GetFilePath(string InputFile) {
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

void CGame::ResetTimer()
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

void CGame::UpdateTimer()
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
                    Audio->PlaySound(SfxOneMinute, 0);

                if (Timer == 3)
                    Audio->PlaySound(SfxThree, 0);

                if (Timer == 2)
                    Audio->PlaySound(SfxTwo, 0);

                if (Timer == 1)
                    Audio->PlaySound(SfxOne, 0);


                if (Timer <= 0)
                {
                    Audio->PlaySound(SfxTimeOver, 0);
                    SubGameState = SGTimeUp;
                    SubStateTime = SDL_GetTicks() + 750;
                    SubStateCounter = 0;
                }
            }
        }
    }
}

void CGame::CreateActiveGame()
{
    if(ActiveGame != nullptr)
    {
        ActiveGame->deinit();
        delete ActiveGame;
        ActiveGame = nullptr;
    }

    switch (GameState) 
    {
        case GSSnakeInit:
            ActiveGame = new CGameSnake(this);
            break;
        case GSTetrisInit:
            ActiveGame = new CGameBlockStacker(this); 
            break;
        case GSRamItInit:
            ActiveGame = new CGameRamIt(this);
            break;
        case GSEddyInit:
            ActiveGame = new CGameFastEddy(this);
            break;
        case GSFrogInit:
            ActiveGame = new CGameFrog(this);
            break;
        case GSBreakoutInit:
            ActiveGame = new CGameBreakOut(this);
            break;
        case GSPangInit:
            ActiveGame = new CGamePang(this);
            break;
        case GSSpaceInvadersInit:
            ActiveGame = new CGameInvaders(this);
            break;
        default:
            ActiveGame = nullptr;
    }
}

void CGame::MainLoop()
{
    Init();
	Uint64 TotalFrames = 0;
    Uint64 TotalFramePerf = 0;
    Uint32 Fps = 0;
    double AvgFrameTime = 0.0f;
    Uint32 Ticks = SDL_GetTicks();
    while (GameState != GSQuit) 
    {
        TotalFrames++;
        Uint64 FrameStartPerf = SDL_GetPerformanceCounter();

        UpdateTimer();

        Input->Update();

        //Need to recreate screenshots and background
        if(Input->Buttons.RenderReset)
        {
            SDL_Log("Render Reset, Recreating crt and background, Reloading Game Graphics");
            Image->UnloadImages();
            LoadGraphics();
            if(ActiveGame != nullptr)
                ActiveGame->LoadGraphics();
            CreateScreenshotsAndBackground();
            ReCreateCrt();
        }

        if(Input->Buttons.ButFullscreen && !Input->PrevButtons.ButFullscreen)
            ToggleFullscreen();
        
        if(Input->Buttons.ButQuit)
            GameState = GSQuit;

        SDL_SetRenderTarget(Renderer, TexOffScreen);   
        
        switch (GameState) 
        {
            case GSIntroInit:
            case GSIntro:
                Intro(this);
                break;

            case GSSubScoreInit:
            case GSSubScore:
                SubScoreScreen(this);
                break;

            case GSTitleScreenInit:
            case GSTitleScreen:
                //to clear the game data & set nullptr to ActiveGame
                CreateActiveGame();
                TitleScreen(this);
                break;

            case GSPangInit:
            case GSBreakoutInit:
            case GSFrogInit:
            case GSEddyInit:
            case GSSnakeInit:
            case GSTetrisInit:
            case GSRamItInit:
            case GSSpaceInvadersInit:
                CreateActiveGame();
                ActiveGame->init();
                ResetTimer();
	            StartCrossFade(ActiveGame->GameStateID, SGReadyGo, 3, 500);
                break;

            case GSPang:
            case GSBreakout:
            case GSFrog:
            case GSEddy:
            case GSSnake:
            case GSTetris:
            case GSRamIt:
            case GSSpaceInvaders:
                ActiveGame->UpdateLogic();
                ActiveGame->Draw();
                break;
            default:
                break;
        }
        if (Alpha < MaxAlpha) 
        {
            Alpha = trunc(MaxAlpha * ((double)(SDL_GetTicks() - AlphaTimer) / MaxAlphaTime));
            if (Alpha + AlphaIncrease >= MaxAlpha) 
            {
                //SDL_SetTextureBlendMode(TexOffScreen, SDL_BLENDMODE_NONE);
                Alpha = MaxAlpha;
                SubGameState = NextSubState;
                SubStateTime = SDL_GetTicks() + NextSubStateTimeAdd; 
                SubStateCounter = NextSubStateCounter;
            }
            else
            {
                SDL_SetTextureBlendMode(TexOffScreen, SDL_BLENDMODE_BLEND);
                SDL_SetTextureAlphaMod(TexOffScreen, Alpha);
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
        SDL_RenderCopy(Renderer, TexScreen, NULL, &Rect);

        if((ActiveGame != nullptr) && (GameState != GSSubScore) && (GameState != GSTitleScreenInit))
            DrawCrt();

        if (debugInfo || ShowFPS)
        {
            string Text = "FPS: " + to_string(Fps) + "\n";
            if(debugInfo)
            {
                Text += "FrameTime: " + to_string(AvgFrameTime) + "\n";
                Text += "GFX Slots: " + to_string(Image->ImageSlotsUsed()) + "/" + to_string(Image->ImageSlotsMax()) + "\n";
                Text += "SND Slots: " + to_string(Audio->SoundSlotsUsed()) + "/" + to_string(Audio->SoundSlotsMax()) + "\n";
                Text += "MUS Slots: " + to_string(Audio->MusicSlotsUsed()) + "/" + to_string(Audio->MusicSlotsMax()) + "\n";
                Text += "SPR Slots: " + to_string(Sprites->SpriteSlotsUsed()) + "/" + to_string(Sprites->SpriteSlotsMax()) + "\n";
            }
            int tw = Font->TextWidth("RobotoMono-Bold", 16, Text, Text.length());
            Font->WriteText(Renderer, "RobotoMono-Bold", 16, Text, Text.length(), ScreenWidth - tw, 0, 0, {255, 0, 255, 255});
        }
        SDL_RenderPresent(Renderer);
        
        Uint64 FrameEndPerf = SDL_GetPerformanceCounter();
        Uint64 FramePerf = FrameEndPerf - FrameStartPerf;
        double FrameTime = FramePerf / (double)SDL_GetPerformanceFrequency() * 1000.0f;
        TotalFramePerf += FramePerf;

        if(SDL_GetTicks() - Ticks >= 1000)
        {
            AvgFrameTime = (TotalFramePerf / TotalFrames) / (double)SDL_GetPerformanceFrequency() * 1000.0f;
            Fps = TotalFrames;
            TotalFrames = 0;
            TotalFramePerf = 0;
            Ticks = SDL_GetTicks();
        }

        int RequiredDelay = 1000.0f/DesiredFps - FrameTime;
        if (RequiredDelay > 0)
          SDL_Delay(RequiredDelay);
    }
    DeInit();
}

void CGame::Run(int argc, char *argv[]) {

    bool useSoftwareRenderer = false;
    bool useLinear = false; //causes issues in for example frog from scaling textures and then bleeding into each other
    bool useVsync = false;
    bool useFullScreenAtStartup = true;
    string StartPath = GetFilePath(string(argv[0]));
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
        Uint32 WindowFlags = SDL_WINDOW_RESIZABLE;
        if (useFullScreenAtStartup) {
            WindowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;            
        }

        SdlWindow = SDL_CreateWindow("RetroTime", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ScreenWidth, ScreenHeight, WindowFlags);
        
        if (SdlWindow) 
        {
            Uint32 flags = 0;
            if (useSoftwareRenderer) 
                flags |= SDL_RENDERER_SOFTWARE;
            else 
                flags |= SDL_RENDERER_ACCELERATED;
            if (useVsync)
                flags |= SDL_RENDERER_PRESENTVSYNC;

            SDL_Log("Succesfully Set %dx%d\n", ScreenWidth, ScreenHeight);
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

                Audio = new CAudio(DataPath);
                Font = new CFont(DataPath);
                Image = new CImage(DataPath);
                Input = new CInput();
                Sprites = new CSprites(Image);
                Sprites->SetForceShowCollisionShape(debugShowCollisionShapes);
                
                // Main game loop that loops untile the gamestate = GSQuit
                // and calls the procedure according to the gamestate.
                LoadSettings();
                LoadHighScores();
                LoadGraphics();
                LoadMusic();
                LoadSound();

                SDL_SetRenderTarget(Renderer, NULL);
                SDL_SetRenderDrawColor(Renderer, 0x00, 0x00, 0x00, 0xFF);
                SDL_RenderClear(Renderer);
                
                TexScreen = SDL_CreateTexture(Renderer, PixelFormat, SDL_TEXTUREACCESS_TARGET, ScreenWidth, ScreenHeight);
                SDL_SetRenderTarget(Renderer, TexScreen);
                SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
                SDL_RenderClear(Renderer);

                TexCrt = SDL_CreateTexture(Renderer, PixelFormat, SDL_TEXTUREACCESS_TARGET, ScreenWidth, ScreenHeight);
                SDL_SetTextureBlendMode(TexCrt, SDL_BLENDMODE_BLEND);
                SDL_SetRenderTarget(Renderer, TexCrt);
                SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 0);
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

                MainLoop(); 

                SDL_DestroyTexture(TexCrt);
                SDL_DestroyTexture(TexOffScreen);
                SDL_DestroyTexture(TexScreen);
                SDL_DestroyTexture(TexTmp);
                UnLoadMusic();
                UnLoadGraphics();
                UnLoadSound();
                SaveSettings();
                SaveHighScores();
                
                delete Audio;
                delete Font; 
                delete Image;
                delete Input;
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
        SDL_Quit();
    } 
    else 
        SDL_Log("Failed to initialise SDL: %s\n", SDL_GetError());
}


