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
#include "../Vec2F.h"

using namespace std;

class CGamePang {

private:
	CGameBase *GameBase;
	static const int backgroundcopyheight = 30;
	Vec2F playerscale = {2.0f*yscale,2.0f*yscale};
	Vec2F bulletscale = {1.0f*yscale,0.8f*yscale};
	Vec2F ballscale = {1.5f*yscale,1.5f*yscale};

	static constexpr float enemyspeed = 1.0f*yscale;
	static constexpr float playerspeed = 8.0f*xscale;
	static constexpr float bulletspeed = 10.0f*yscale;

	static const int playerstateidle = 0;
	static const int playerstatemoveleft = 2;
	static const int playerstatemoveright = 4;
	static const int playerstateshoot = 8;
	static const int playerstatereviving = 16;


	static const int ballbig = 3;
	static const int ballmedium = 2;
	static const int ballsmall = 1;

	static const int maxbigballs = 6;
	static const int maxballs = maxbigballs * 7;


	int background;
	int spritesheetplayer;
	int spritesheetbullet;
	int spritesheetball;


	SDL_Point backgroundtz;
	SDL_Point spritesheetballtz;

	int deaths = 0;
	int levelcleared = false;
	CSpriteObject balls[maxballs];
	CSpriteObject player;
	CSpriteObject bullet;

	int MusMusic, SfxSucces, SfxDie, SfxShoot, SfxPop;


	void drawplayer();
	void updateplayer();
	void remplayerstate(int state);
	bool playerstate(int state);
	void addplayerstate(int state);
	void createplayer();
	void destroyplayer();
	void drawbullet();
	void updatebullet();
	void createbullet();
	void destroybullet();
	void createballs();
	void drawballs();
	void updateballs();
	void createball(int size, float x, float y, float speed);
	void destroyball(int index, bool nocreate);
	void destroyallballs();
public:
	CGamePang(CGame* aGame);
	~CGamePang();
	void init();
	void deinit();
	void UnloadGraphics();
	void LoadGraphics();
	void LoadSound();
	void UnLoadSound();
	void UpdateObjects(bool IsGameState);
	bool DrawObjects();
	void DrawBackground();
	void Draw();
	void UpdateLogic();
};