#pragma once

#include <SDL.h>
#include <map>
#include <iostream>
#include <string>
#include "CGameBase.h"
#include "CSpriteObject.h"
#include "../CGame.h"
#include "../CTween.h"
#include "../Platform.h"
#include "../Common.h"

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
	static const int tweenblockpositions = 0;
	static const int tweenblockdeath = 1;
	static const int blockstatedeath = 1;
	static constexpr float ballspeed = 6.0;
	static constexpr float ballspeedinc = 0.1;
	static const int ballvelsegments = 5;
	static constexpr float playerspeed = 8;
	static constexpr float blockxoffset = 120;
	static constexpr float blockyoffset = 80;
	static const int blockrows = 10;
	static const int blockcols = 17;
	static const int numblocks = blockrows * blockcols;

	const Vec2F spritescale = {2.5,2.5};
	const Vec2F blockspritecale = {1,2};

	int MusMusic, SfxDie, SfxSucces, SfxBat, SfxBrick;
	int background, spritesheetblocks, spritesheetbat, spritesheetball;
	SDL_Point backgroundtz;

	float curballspeed;
	int pattern;
	sblockinfo blockinfo;
	CSpriteObject blocks[numblocks];
	CTweenInfo tweens[numblocks][2];
	CSpriteObject player;
	CSpriteObject ball;

	void updateball();
	void createball();
	void destroyball();

	void updateplayer();
	void createplayer();
	void destroyplayer();

	void updateblocks();
	void createblocks(bool setlocation);
	void destroyblock(int index);
	void destroyallblocks();

	void drawbackground(bool motionblur);
	void updateblockinfo();
public:
	CGameBreakOut(CGame* aGame, bool aScreenshotMode = false);
	~CGameBreakOut();
	SDL_Texture* screenshot() override;
	void init() override;
	void deinit() override;
	void UnloadGraphics() override;
	void LoadGraphics() override;
	void LoadSound() override;
	void UnLoadSound() override;
	void UpdateObjects(bool IsGameState) override;
	void DrawBackground(bool motionblur) override;
};