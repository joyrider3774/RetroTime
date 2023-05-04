#pragma once

#include <SDL.h>
#include <limits.h>
#include <map>
#include <iostream>
#include <string>
#include "CGameBase.h"
#include "CSpriteObject.h"
#include "..\CGame.h"
#include "..\CTween.h"
#include "..\Platform.h"
#include "..\Common.h"

using namespace std;

struct sblockinfo
{
	int mostleft;
	int mostright;
	int mostbottom;
};
typedef struct sblockinfo sblockinfo;

class CGameBreakOut: public CGameBase {

private:
    static const int sbtweenblockpositions = 0;
    static const int sbtweenblockdeath = 1;
    static const int sbblockstatedeath = 1;   
    static constexpr float sbballspeed = 6.0;
    static constexpr float sbballspeedinc = 0.1;
    static const int sbballvelsegments = 5;
    static constexpr float sbplayerspeed = 8;
    static constexpr float sbblockxoffset = 120;
    static constexpr float sbblockyoffset = 80;
    static const int sbblockrows = 10;
    static const int sbblockcols = 17;
    static const int sbnumblocks = sbblockrows * sbblockcols;

    const SDL_FPoint sbspritescale = {2.5,2.5};
    const SDL_FPoint sbblockspritecale = {1,2};
    
    int MusMusic, SfxDie, SfxSucces, SfxBat, SfxBrick;
    int background, spritesheetblocks, spritesheetbat, spritesheetball;
    SDL_Point backgroundtz;

	float ballspeed;
	int pattern;
	sblockinfo blockinfo;
	CSpriteObject blocks[sbnumblocks];
	CTweenInfo tweens[sbnumblocks][2];
	CSpriteObject player;
	CSpriteObject ball;

    void sbupdateball();
    void sbcreateball();
    void sbdestroyball();

    void sbupdateplayer();
    void sbcreateplayer();
    void sbdestroyplayer();

    void sbupdateblocks();
    void sbcreateblocks(bool setlocation);
    void sbdestroyblock(int index);
    void sbdestroyallblocks();
    
    void sbdrawbackground(bool motionblur);
    void sbupdateblockinfo();
public:
    CGameBreakOut(CGame* aGame, bool aScreenshotMode = false);
    ~CGameBreakOut();
    SDL_Texture* screenshot() override;
    void init() override;
    void deinit() override;
    void UpdateLogic() override;
    void Draw() override;
    void UnloadGraphics() override;
    void LoadGraphics() override;
};