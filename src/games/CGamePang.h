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

class CGamePang: public CGameBase {

private:
	static const int backgroundcopyheight = 30;
	Vec2F playerscale = {2,2};
	Vec2F bulletscale = {1,0.8};
	Vec2F ballscale = {1.5,1.5};

	static const int enemyspeed = 1;
	static const int playerspeed = 8;
	static const int bulletspeed = 10;

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
	int backgroundgrass;
	int backgroundcloud;
	int backgroundtrees;
	int backgroundtree;


	SDL_Point backgroundtz;
	SDL_Point backgroundgrasstz;
	SDL_Point backgroundcloudtz;
	SDL_Point backgroundtreestz;
	SDL_Point backgroundtreetz;
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
	CGamePang(CGame* aGame, bool aScreenshotMode = false);
	~CGamePang();
	SDL_Texture* screenshot() override;
	void init() override;
	void deinit() override;
	void UnloadGraphics() override;
	void LoadGraphics() override;
	void LoadSound() override;
	void UnLoadSound() override;
	void UpdateObjects(bool IsGameState) override;
	bool DrawObjects() override;
	void DrawBackground(bool motionblur) override;
};