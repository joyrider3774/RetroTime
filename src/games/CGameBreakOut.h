#ifndef CGAMEBREAKOUT_H
#define CGAMEBREAKOUT_H

#include <SDL.h>
#include <stdbool.h>
#include "CGameBase.h"
#include "CSpriteObject.h"
#include "../CGame.h"
#include "../CTween.h"
#include "../Platform.h"
#include "../Common.h"

#define CGameBreakOut_tweenblockpositions 0
#define CGameBreakOut_tweenblockdeath 1
#define CGameBreakOut_blockstatedeath 1
#define CGameBreakOut_ballspeed (6.0f*yscale)
#define CGameBreakOut_ballspeedinc (0.1f*yscale)
#define CGameBreakOut_ballvelsegments 5
#define CGameBreakOut_playerspeed (8.0f*xscale)
#define CGameBreakOut_blockxoffset (120.0f*xscale)
#define CGameBreakOut_blockyoffset (80.0f*yscale)
#define CGameBreakOut_blockrows 10
#define CGameBreakOut_blockcols 17
#define CGameBreakOut_numblocks 170 //blockrows * blockcols;

struct sblockinfo
{
	int mostleft;
	int mostright;
	int mostbottom;
};
typedef struct sblockinfo sblockinfo;

struct CGameBreakOut {
	CGameBase *GameBase;

	Vec2F spritescale; 
	Vec2F blockspritecale;

	int MusMusic, SfxDie, SfxSucces, SfxBat, SfxBrick;
	int background, spritesheetblocks, spritesheetbat, spritesheetball;
	SDL_Point backgroundtz;

	float curballspeed;
	int pattern;
	sblockinfo blockinfo;
	CSpriteObject blocks[CGameBreakOut_numblocks];
	CTweenInfo tweens[CGameBreakOut_numblocks][2];
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
CGameBreakOut* Create_CGameBreakOut();

#endif