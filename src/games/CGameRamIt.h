#pragma once

#include <SDL.h>
#include <map>
#include <iostream>
#include <string>
#include "CGameBase.h"
#include "../CGame.h"
#include "../Platform.h"
#include "../Common.h"


using namespace std;

struct ramitblock 
{
	int maxsegments;
	int segments;
	SDL_Color color;
};

class CGameRamIt: public CGameBase {

private:
    static const int bulletsteps = 8;
    static const int playerrailwidth = 10;
    static const int playerspeed = 10;
    static const int bulletspeed = 5;
    static const int numblocks = 16;
    static const int blocksegments = 20;
    static const int sides = 2;
    static const int blockspacing = 5;
    static const int ticksidle = 50;
    static const int tickslevelmul = 5;

    int SfxShoot, SfxHit, SfxDie, SfxSucces, MusMusic;
    int speed, speedcount, playerdx, playerdeath, riblocksize, riblocksizespacing, segmentwidth;
	SDL_Point playerpos;
	SDL_Point bulletpos;
	SDL_Point bulletvel;
	bool bulletalive = false;
    ramitblock playfield[sides][numblocks];

    bool createbullet();
    void updatebullet();
    void drawbullet();

    void createplayer();
    void drawplayer();
    void updateplayer();

    void createplayfield();
    void updateplayfield(bool force);
    void drawplayfield();

    void drawbackground();

public:
    CGameRamIt(CGame* aGame, bool aScreenshotMode = false);
    ~CGameRamIt();
    
    SDL_Texture* screenshot() override;   
    void init()  override;
    void deinit()  override;
    void UpdateLogic()  override;
    void Draw() override;
};