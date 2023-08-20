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

struct CGameRamIt {

	CGameBase *GameBase;
	static const int bulletsteps = 8;
	static const int playerrailwidth = 10*xscale;
	static constexpr float playerspeed = 10*xscale;
	static constexpr float bulletspeed = 5*xscale;
	static const int numblocks = 16;
	static const int blocksegments = 20;
	static const int sides = 2;
	static const int blockspacing = 5*yscale;
	static const int ticksidle = 50;
	static const int tickslevelmul = 5;

	int SfxShoot, SfxHit, SfxDie, SfxSucces, MusMusic;
	int speed, speedcount, playerdx, playerdeath, riblocksize, riblocksizespacing, segmentwidth;
	SDL_Point playerpos;
	SDL_Point bulletpos;
	SDL_Point bulletvel;
	bool bulletalive = false;
	ramitblock playfield[sides][numblocks];

	bool (*createbullet)(CGameRamIt* GameRamIt);
	void (*updatebullet)(CGameRamIt* GameRamIt);
	void (*drawbullet)(CGameRamIt* GameRamIt);

	void (*createplayer)(CGameRamIt* GameRamIt);
	void (*drawplayer)(CGameRamIt* GameRamIt);
	void (*updateplayer)(CGameRamIt* GameRamIt);

	void (*createplayfield)(CGameRamIt* GameRamIt);
	void (*updateplayfield)(CGameRamIt* GameRamIt, bool force);
	void (*drawplayfield)(CGameRamIt* GameRamIt);

	void (*init)(CGameRamIt* GameRamIt);
	void (*deinit)(CGameRamIt* GameRamIt);
	void (*LoadSound)(CGameRamIt* GameRamIt);
	void (*UnLoadSound)(CGameRamIt* GameRamIt);
	void (*UpdateObjects)(CGameRamIt* GameRamIt,bool IsGameState);
	bool (*DrawObjects)(CGameRamIt* GameRamIt);
	void (*DrawBackground)(CGameRamIt* GameRamIt);
	void (*Draw)(CGameRamIt* GameRamIt);
	void (*UpdateLogic)(CGameRamIt* GameRamIt);
};

typedef struct CGameRamIt CGameRamIt;

void CGameRamIt_Draw(CGameRamIt* GameRamIt);
bool CGameRamIt_DrawObjects(CGameRamIt* GameRamIt);
void CGameRamIt_UpdateLogic(CGameRamIt* GameRamIt);
void CGameRamIt_UpdateObjects(CGameRamIt* GameRamIt, bool IsGameState);
void CGameRamIt_UnLoadSound(CGameRamIt* GameRamIt);
void CGameRamIt_LoadSound(CGameRamIt* GameRamIt);
void CGameRamIt_deinit(CGameRamIt* GameRamIt);
void CGameRamIt_init(CGameRamIt* GameRamIt);
void CGameRamIt_DrawBackground(CGameRamIt* GameRamIt);
void CGameRamIt_drawplayfield(CGameRamIt* GameRamIt);
void CGameRamIt_updateplayfield(CGameRamIt* GameRamIt, bool force);
void CGameRamIt_createplayfield(CGameRamIt* GameRamIt);
void CGameRamIt_updateplayer(CGameRamIt* GameRamIt);
void CGameRamIt_drawplayer(CGameRamIt* GameRamIt);
void CGameRamIt_createplayer(CGameRamIt* GameRamIt);
void CGameRamIt_drawbullet(CGameRamIt* GameRamIt);
void CGameRamIt_updatebullet(CGameRamIt* GameRamIt);
bool CGameRamIt_createbullet(CGameRamIt* GameRamIt);
void Destroy_CGameRamIt(CGameRamIt* GameRamIt);
CGameRamIt* Create_CGameRamIt();