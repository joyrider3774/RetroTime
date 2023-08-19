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

struct CGameBreakOut {
	CGameBase *GameBase;
	static const int tweenblockpositions = 0;
	static const int tweenblockdeath = 1;
	static const int blockstatedeath = 1;
	static constexpr float ballspeed = 6.0f*yscale;
	static constexpr float ballspeedinc = 0.1f*yscale;
	static const int ballvelsegments = 5;
	static constexpr float playerspeed = 8.0f*xscale;
	static constexpr float blockxoffset = 120.0f*xscale;
	static constexpr float blockyoffset = 80.0f*yscale;
	static const int blockrows = 10;
	static const int blockcols = 17;
	static const int numblocks = blockrows * blockcols;
	static constexpr Vec2F spritescale = {2.5f*xscale,2.5f*yscale};
	static constexpr Vec2F blockspritecale = {1.0f*xscale,2.0f*yscale};

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

	void (*updateball)(CGameBreakOut* GameBreakOut);
	void (*createball)(CGameBreakOut* GameBreakOut);
	void (*destroyball)(CGameBreakOut* GameBreakOut);
	void (*updateplayer)(CGameBreakOut* GameBreakOut);
	void (*createplayer)(CGameBreakOut* GameBreakOut);
	void (*destroyplayer)(CGameBreakOut* GameBreakOut);
	void (*updateblocks)(CGameBreakOut* GameBreakOut);
	void (*createblocks)(CGameBreakOut* GameBreakOut, bool setlocation);
	void (*destroyblock)(CGameBreakOut* GameBreakOut,int index);
	void (*destroyallblocks)(CGameBreakOut* GameBreakOut);
	void (*updateblockinfo)(CGameBreakOut* GameBreakOut);
	void (*init)(CGameBreakOut* GameBreakOut);
	void (*deinit)(CGameBreakOut* GameBreakOut);
	void (*UnloadGraphics)(CGameBreakOut* GameBreakOut);
	void (*LoadGraphics)(CGameBreakOut* GameBreakOut);
	void (*LoadSound)(CGameBreakOut* GameBreakOut);
	void (*UnLoadSound)(CGameBreakOut* GameBreakOut);
	void (*UpdateObjects)(CGameBreakOut* GameBreakOut, bool IsGameState);
	void (*DrawBackground)(CGameBreakOut* GameBreakOut);
	void (*Draw)(CGameBreakOut* GameBreakOut);
	void (*UpdateLogic)(CGameBreakOut* GameBreakOut);
};
typedef struct CGameBreakout CGameBreakout;

void CGameBreakOut_UpdateLogic(CGameBreakOut* GameBreakOut);
void CGameBreakOut_UpdateObjects(CGameBreakOut* GameBreakOut, bool IsGameState);
void CGameBreakOut_UnloadGraphics(CGameBreakOut* GameBreakOut);
void CGameBreakOut_LoadGraphics(CGameBreakOut* GameBreakOut);
void CGameBreakOut_deinit(CGameBreakOut* GameBreakOut);
void CGameBreakOut_UnLoadSound(CGameBreakOut* GameBreakOut);
void CGameBreakOut_LoadSound(CGameBreakOut* GameBreakOut);
void CGameBreakOut_init(CGameBreakOut* GameBreakOut);
void CGameBreakOut_Draw(CGameBreakOut* GameBreakOut);
void CGameBreakOut_DrawBackground(CGameBreakOut* GameBreakOut);
void CGameBreakOut_updateball(CGameBreakOut* GameBreakOut);
void CGameBreakOut_createball(CGameBreakOut* GameBreakOut);
void CGameBreakOut_destroyball(CGameBreakOut* GameBreakOut);
void CGameBreakOut_updateplayer(CGameBreakOut* GameBreakOut);
void CGameBreakOut_createplayer(CGameBreakOut* GameBreakOut);
void CGameBreakOut_destroyplayer(CGameBreakOut* GameBreakOut);
void CGameBreakOut_updateblocks(CGameBreakOut* GameBreakOut);
void CGameBreakOut_createblocks(CGameBreakOut* GameBreakOut, bool setlocation);
void CGameBreakOut_destroyblock(CGameBreakOut* GameBreakOut, int index);
void CGameBreakOut_destroyallblocks(CGameBreakOut* GameBreakOut);
void CGameBreakOut_updateblockinfo(CGameBreakOut* GameBreakOut);
void Destroy_CGameBreakOut(CGameBreakOut* GameBreakOut);
CGameBreakOut* Create_CGameBreakOut(CGame* aGame);