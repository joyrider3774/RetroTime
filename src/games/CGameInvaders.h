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
#include "../CTween.h"
#include "../Vec2F.h"

using namespace std;

struct senemyinfo
{
	int mostleft;
	int mostright;
	int mostbottom;
};
typedef struct senemyinfo senemyinfo;


class CGameInvaders {

private:
	CGameBase *GameBase;
	static const int tweenenemypositions = 0;
	static const int tweenenemydeath = 1;


	static const int enemyrows = 5;
	static const int enemycols = 7;
	static const int enemystartxoffset = 50*xscale;
	static const int enemystartyoffset = 50*yscale;
	static const int enemyspacing = 60*yscale;
	static const int enemyhorzspacing = 50*xscale;
	Vec2F enemyscale = {1.5f*xscale,1.5f*yscale};
	static constexpr float enemyspeed = 1.0f*xscale;
	static constexpr float enemyspeedinc = 0.15f*xscale;
	static constexpr float playerspeed = 8.0f*xscale;
	static constexpr float bulletspeed = 10.0f*yscale;
	static const int asteroidsoffset = 150*xscale;
	static const int endscreenconstant = 45*xscale + asteroidsoffset;
	static const int maxexplosion = 10;
	static const int maxasteroids = 6;
	static constexpr float asteroidscale = 2.5f*xscale;
	static const int asteroidmaxhealthpoints = 50;
	static const int maxenemybullets = 5;
	static const int enemybulletrandomizer = 100;


	int deaths = 0;
	int pattern = 0;
	float backgroundfade = 0;
	float backgroundfadeinc = 0;
	Vec2F enemyvel = {enemyspeed,0};

	senemyinfo enemyinfo;
	CSpriteObject player;
	CSpriteObject bullet;
	CSpriteObject enemies[enemycols * enemycols];
	CSpriteObject explosions[maxexplosion];
	CSpriteObject asteroids[maxasteroids];
	CSpriteObject enemybullets[maxenemybullets];

	CTweenInfo tweens[enemycols * enemycols][2];

	int background;

	int spritesheetBullet;
	int spritesheetExplosion;
	int spritesheetAsteroid;
	int spritesheetEnemy1;
	int spritesheetEnemy2;
	int spritesheetEnemy3;
	int spritesheetPlayer;

	SDL_Point backgroundtz;

	int MusMusic, SfxPlayerShoot, SfxPlayerDeath, SfxEnemyShoot, SfxEnemyDeath, SfxSucces, SfxDie;

	void updateplayer();
	void createplayer();
	void destroyplayer();
	void updateinvaders();
	void createinvaders(bool setposition);
	bool tweenactive(int id);
	void destroyinvader(int index);
	void destroyallinvaders();
	void updateenemyinfo();
	void updatebullet();
	void destroybullet();
	void createbullet();
	void updateasteroids();
	void createasteroids();
	void destroyasteroid(int index);
	void destroyallasteroids();
	void updateenemybullet();
	void createnemybullet(Vec2F pos);
	void destroyenemybullet(int index);
	void destroyallenemybullet();
	void createexplosion(Vec2F pos);
	void destroyexploison(int index);
	void destroyallexplosion();
	void checkexplosions();
public:
	CGameInvaders(CGame* aGame, bool aScreenshotMode = false);
	~CGameInvaders();
	SDL_Texture* screenshot();
	void init();
	void deinit();
	void UnloadGraphics();
	void LoadGraphics();
	void LoadSound();
	void UnLoadSound();
	void UpdateObjects(bool IsGameState);
	bool DrawObjects();
	void DrawBackground(bool motionblur);
	void Draw();
	void UpdateLogic();
};