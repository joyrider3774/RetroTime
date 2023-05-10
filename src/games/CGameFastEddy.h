#pragma once

#include <SDL.h>
#include <map>
#include <iostream>
#include <string>
#include "CGameBase.h"
#include "CSpriteObject.h"
#include "../CGame.h"
#include "../Platform.h"
#include "../Common.h"

using namespace std;

class CGameFastEddy: public CGameBase {

private:
    static const int playerstateunknown = -1;
    static const int playerstateidle = 0;
    static const int playerstateleft = 1;
    static const int playerstateleftjump = 2;
    static const int playerstateright = 3;
    static const int playerstaterightjump = 4;
    static const int playerstatejump = 5;
    static const int playerstateclimbup = 6;
    static const int playerstateclimbdown = 7;

    static const int enemystateidle = 0;
    static const int enemystatemoveleft = 1;
    static const int enemystatewaitmove = 2;
    static const int enemystatemoveright = 3;

    static const int collectablestatemoveleft = 0;
    static const int collectablestatemoveright = 1;
    static const int collectablestateidle = 2;

    static const int laddersfitrows = 10;
    static const int rowfloorsize = 32; //tilesize / 2 oroginals are 64x64
    static const int rows = 5;

    static constexpr float playerspeed = 6;
    static constexpr float playerjumpspeed = 2.8;
    static constexpr float playerclimbspeed = 2;
    static constexpr float collectablespeed = 2;

    static const int maxenemies = 10;
    static const int maxcollectables = 2;

    static const int maxladders = (rows - 1) * 2;
    float playerjumpspeeddec;

    int numfloortilesperrow;
    int maxfloortiles;
    float rowspacingsize;
    float ladderwidth;
    float rowsize;
    float playerheight;
    float enemyheight;
    float playerjumpheight;
    float collectableheight;
    float keyheight;
  
    int spritesheet;// = loadImage("EderMuniz/SnowyForestTiles")
    int spritesheetladder;// = loadImage("EderMuniz/SnowyForestLadder")
    int spritesheetplayerclimb;// = loadImage("Ravenmore/characterClimb")
    int spritesheetplayerrun;// = loadImage("Ravenmore/characterRun")
    int spritesheetplayeridle;// = loadImage("Ravenmore/characterIdle")
    int spritesheetplayerjump;// = loadImage("Ravenmore/characterJumpUp")
    int spritesheetenemy;// = loadImage("Pipo/char59")
    int spritesheetcollectable;// = loadImage("Ravenmore/platformPickups")
    int spritesheetkey;// = loadImage("Ravenmore/PlatformDefaultProps")
    int background;// = loadImage("EderMuniz/SnowyForestBackA")
    SDL_Point backgroundtz;// = imageSize(febackground)

    int MusMusic, SfxSucces, SfxCollect, SfxDie;

    int rowzeroenemyindex = 0;
	int collecteditems = 0;
	int collectedcreated = 0;
	CSpriteObject player;
	CSpriteObject ladders[maxladders];
	CSpriteObject floors[1280 / rowfloorsize * rows]; //1280 is playfield size
	CSpriteObject enemies[maxenemies];
	CSpriteObject collectables[maxcollectables];
	CSpriteObject key;

    void createkey();
    void destroykey();
    void updatekey();
    void drawbackground(bool motionblur);
    void createladders();
    void destroyladders();
    void createfloors();
    void destroyfloors();
    void updateplayer();
    void createplayer();
    void destroyplayer();
    void updateenemies();
    void createenemies(bool levelsucces);
    void createenemy(int row, float x, int state, int group, int multiply);
    void enemyenablelevelend();
    void destroyenemies();
    void updatecollectables();
    void createcollectables(int ignorerow);
    void destroycollectable(int index);
    void destroyallcollectables();
public:
    CGameFastEddy(CGame* aGame, bool aScreenshotMode = false);
    ~CGameFastEddy();
    SDL_Texture* screenshot() override;
    void init() override;
    void deinit() override;
    void UpdateLogic() override;
    void Draw() override;
    void UnloadGraphics() override;
    void LoadGraphics() override;
    void LoadSound() override;
    void UnLoadSound() override;
};