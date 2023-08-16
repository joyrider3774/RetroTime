#pragma once

#include <SDL.h>
#include <iostream>
#include <string>
#include "CGameBase.h"
#include "../CGame.h"
#include "../Common.h"

using namespace std;


class CGameSnake: public CGameBase {

private:
	static const int snakesize = 35 * yscale;
	static const int rows = int(ScreenHeight / snakesize) -1;
	static const int cols = int(ScreenWidth / snakesize) -1;
	static const int updateticks = 8;

	const SDL_Color snakeheadcolor = {0xFF, 0x65, 0x65, 0xFF};
	const SDL_Color snakebodycolor = {0xFF, 0xFF, 0xFF, 0xFF};
	const SDL_Color snakefoodcolor = {0x65, 0xFF, 0x65, 0xFF};

	bool movedone = true;
	int snakelength;
	int ticks;
	SDL_Point head;
	SDL_Point body[rows*cols+1];
	SDL_Point dir;
	SDL_Point food;
	int playerdeath;
	int MusMusic, SfxFood, SfxDie;

	void createfood();
	void drawfood();
	void updatefood();

	void createsnake();
	void drawsnake();
	void updatesnake();
public:
	CGameSnake(CGame* aGame, bool aScreenshotMode = false);
	~CGameSnake();
	SDL_Texture* screenshot() override;
	void init() override;
	void deinit() override;
	void LoadSound() override;
	void UnLoadSound() override;
	void UpdateObjects(bool IsGameState) override;
	bool DrawObjects() override;
	void DrawBackground(bool motionblur);
};