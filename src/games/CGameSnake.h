#pragma once

#include <SDL.h>
#include <iostream>
#include <string>
#include "CGameBase.h"
#include "../CGame.h"
#include "../Common.h"

using namespace std;


struct CGameSnake {

	CGameBase *GameBase;
	static constexpr int snakesize = 35 * yscale;
	static constexpr int rows = int(ScreenHeight / snakesize) -1;
	static constexpr int cols = int(ScreenWidth / snakesize) -1;
	static constexpr int updateticks = 8;

	static constexpr SDL_Color snakeheadcolor = {0xFF, 0x65, 0x65, 0xFF};
	static constexpr SDL_Color snakebodycolor = {0xFF, 0xFF, 0xFF, 0xFF};
	static constexpr SDL_Color snakefoodcolor = {0x65, 0xFF, 0x65, 0xFF};

	bool movedone = true;
	int snakelength;
	int ticks;
	SDL_Point head;
	SDL_Point body[rows*cols+1];
	SDL_Point dir;
	SDL_Point food;
	int playerdeath;
	int MusMusic, SfxFood, SfxDie;

	void (*createfood)(CGameSnake* GameSnake);
	void (*drawfood)(CGameSnake* GameSnake);
	void (*updatefood)(CGameSnake* GameSnake);

	void (*createsnake)(CGameSnake* GameSnake);
	void (*drawsnake)(CGameSnake* GameSnake);
	void (*updatesnake)(CGameSnake* GameSnake);

	void (*init)(CGameSnake* GameSnake);
	void (*deinit)(CGameSnake* GameSnake);
	void (*LoadSound)(CGameSnake* GameSnake);
	void (*UnLoadSound)(CGameSnake* GameSnake);
	void (*UpdateObjects)(CGameSnake* GameSnake, bool IsGameState);
	bool (*DrawObjects)(CGameSnake* GameSnake);
	void (*DrawBackground)(CGameSnake* GameSnake);
	void (*Draw)(CGameSnake* GameSnake);
	void (*UpdateLogic)(CGameSnake* GameSnake);
};

typedef struct CGameSnake CGameSnake;

void CGameSnake_Draw(CGameSnake* GameSnake);
bool CGameSnake_DrawObjects(CGameSnake* GameSnake);
void CGameSnake_UpdateLogic(CGameSnake* GameSnake);
void CGameSnake_UpdateObjects(CGameSnake* GameSnake, bool IsGameState);
void CGameSnake_UnLoadSound(CGameSnake* GameSnake);
void CGameSnake_LoadSound(CGameSnake* GameSnake);
void CGameSnake_deinit(CGameSnake* GameSnake);
void CGameSnake_init(CGameSnake* GameSnake);
void CGameSnake_DrawBackground(CGameSnake* GameSnake);
void CGameSnake_updatesnake(CGameSnake* GameSnake);
void CGameSnake_drawsnake(CGameSnake* GameSnake);
void CGameSnake_createsnake(CGameSnake* GameSnake);
void CGameSnake_updatefood(CGameSnake* GameSnake);
void CGameSnake_drawfood(CGameSnake* GameSnake);
void CGameSnake_createfood(CGameSnake* GameSnake);
void Destroy_CGameSnake(CGameSnake* GameSnake);
CGameSnake* Create_CGameSnake(CGame* aGame);