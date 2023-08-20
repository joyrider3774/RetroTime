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


struct CGameBlockStacker {

	CGameBase *GameBase;
	static const int numcols = 12;
	static const int numrows = 18;
	static const int blocksize = ScreenHeight / numrows;
	static const int ticksidle = 3;
	static const int ticksinputidle = 4;

	int background;
	SDL_Point backgroundtz;
	int playfield[numcols * numrows];
	int currpiece, rotation,speed, speedcount, piececount, lineclear, plrx, plry;
	bool rotateblock, dropblock;
	int MusMusic, SfxDie, SfxLineClear, SfxDrop, SfxRotate;

	bool (*piecefits)(CGameBlockStacker* BlockStacker, int tetrimo, int rotation, int posx, int posy);
	void (*updateplayer)(CGameBlockStacker* BlockStacker);
	void (*createplayfield)(CGameBlockStacker* BlockStacker);
	void (*updateplayfield)(CGameBlockStacker* BlockStacker, bool force);
	void (*drawplayfieldcell)(CGameBlockStacker* BlockStacker, int x, int y, int piece);
	void (*drawplayfield)(CGameBlockStacker* BlockStacker);
	void (*DrawBackground)(CGameBlockStacker* BlockStacker);
	void (*Draw)(CGameBlockStacker* BlockStacker);
	void (*UpdateLogic)(CGameBlockStacker* BlockStacker);
	void (*init)(CGameBlockStacker* BlockStacker);
	void (*deinit)(CGameBlockStacker* BlockStacker);
	void (*UnloadGraphics)(CGameBlockStacker* BlockStacker);
	void (*LoadGraphics)(CGameBlockStacker* BlockStacker);
	void (*LoadSound)(CGameBlockStacker* BlockStacker);
	void (*UnLoadSound)(CGameBlockStacker* BlockStacker);
	void (*UpdateObjects)(CGameBlockStacker* BlockStacker, bool IsGameState);
	bool (*DrawObjects)(CGameBlockStacker* BlockStacker);
};
typedef struct CGameBlockStacker CGameBlockStacker;

bool CGameBlockStacker_DrawObjects(CGameBlockStacker* BlockStacker);
void CGameBlockStacker_UpdateObjects(CGameBlockStacker* BlockStacker, bool IsGameState);
void CGameBlockStacker_UpdateLogic(CGameBlockStacker* BlockStacker);
void CGameBlockStacker_deinit(CGameBlockStacker* BlockStacker);
void CGameBlockStacker_UnLoadSound(CGameBlockStacker* BlockStacker);
void CGameBlockStacker_LoadSound(CGameBlockStacker* BlockStacker);
void CGameBlockStacker_UnloadGraphics(CGameBlockStacker* BlockStacker);
void CGameBlockStacker_LoadGraphics(CGameBlockStacker* BlockStacker);
void CGameBlockStacker_init(CGameBlockStacker* BlockStacker);
void CGameBlockStacker_Draw(CGameBlockStacker* BlockStacker);
void CGameBlockStacker_DrawBackground(CGameBlockStacker* BlockStacker);
void CGameBlockStacker_drawplayfield(CGameBlockStacker* BlockStacker);
void CGameBlockStacker_drawplayfieldcell(CGameBlockStacker* BlockStacker, int x, int y, int piece);
void CGameBlockStacker_updateplayfield(CGameBlockStacker* BlockStacker, bool force);
void CGameBlockStacker_createplayfield(CGameBlockStacker* BlockStacker);
void CGameBlockStacker_updateplayer(CGameBlockStacker* BlockStacker);
bool CGameBlockStacker_piecefits(CGameBlockStacker* BlockStacker, int tetrimo, int rotation, int posx, int posy);
void Destroy_CGameBlockStacker(CGameBlockStacker* BlockStacker);
CGameBlockStacker* Create_CGameBlockStacker();