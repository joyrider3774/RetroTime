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


class CGameBlockStacker {

private:
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

	bool piecefits(int tetrimo, int rotation, int posx, int posy);
	void updateplayer();

	void createplayfield();
	void updateplayfield(bool force);
	void drawplayfieldcell(int x, int y, int piece);
	void drawplayfield();
public:
	CGameBlockStacker(CGame* aGame);
	~CGameBlockStacker();
	void DrawBackground(bool motionblur);
	void Draw();
	void UpdateLogic();
	void init();
	void deinit();
	void UnloadGraphics();
	void LoadGraphics();
	void LoadSound();
	void UnLoadSound();
	void UpdateObjects(bool IsGameState);
	bool DrawObjects();
};