#pragma once

#include <SDL.h>
#include <iostream>
#include <string>
#include "CGameBase.h"
#include "..\CGame.h"
#include "..\Common.h"

using namespace std;


class CGameSnake: public CGameBase {

private:
    static const int snakesize = 35;
    static const int rows = int(ScreenHeight / snakesize) -1;
    static const int cols = int(ScreenWidth / snakesize) -1;
    static const int updateticks = 8;
    
    const SDL_Color snakeheadcolor = {255, 0, 0, 255};
    const SDL_Color snakebodycolor = {255, 255, 255, 255};
    const SDL_Color snakefoodcolor = {0, 255 ,0, 255};

    bool movedone = true, ScreenshotMode;
	int snakelength;
	int ticks;
	SDL_Point head;
	SDL_Point body[rows*cols+1];
	SDL_Point dir;
	SDL_Point food;	
	int playerdeath;
    int MusMusic, SfxFood, SfxDie;
    
    void sscreatefood();
    void ssdrawfood();
    void ssupdatefood();

    void sscreatesnake();
    void ssdrawsnake();
    void ssupdatesnake();
    void ssdrawbackground();
    
public:
    CGameSnake(CGame* aGame, bool aScreenshotMode = false);
    ~CGameSnake();
    SDL_Texture* screenshot() override;
    void UpdateLogic() override;
    void Draw() override;
    void init() override;
    void deinit() override;
};