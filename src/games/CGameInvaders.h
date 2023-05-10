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


class CGameInvaders: public CGameBase {

private:
	static const int tweenenemypositions = 0;
	static const int tweenenemydeath = 1;


	static const int enemyrows = 5;
	static const int enemycols = 7;
	static const int enemystartxoffset = 50;
	static const int enemystartyoffset = 50;
	static const int enemyspacing = 60;
	static const int enemyhorzspacing = 50;
	Vec2F enemyscale = {1.5,1.5};
	static const int enemyspeed = 1;
	static constexpr float enemyspeedinc = 0.15;
	static const int playerspeed = 8;
	static const int bulletspeed = 10;
	static const int asteroidsoffset = 150;
	static const int endscreenconstant = 45 + asteroidsoffset;
	static const int maxexplosion = 10;
	static const int maxasteroids = 6;
	static constexpr float asteroidscale = 2.5;
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
	int background2;
	int background3;
	int backgrounditem;

	int spritesheetBullet;
	int spritesheetExplosion;
	int spritesheetAsteroid;
	int spritesheetEnemy1;
	int spritesheetEnemy2;
	int spritesheetEnemy3;
	int spritesheetPlayer;

	SDL_Point backgroundtz;
	SDL_Point background2tz;
	SDL_Point background3tz;
	SDL_Point backgrounditemtz;

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
	SDL_Texture* screenshot() override;
	void init() override;
	void deinit() override;
	void UnloadGraphics() override;
	void LoadGraphics() override;
	void LoadSound() override;
	void UnLoadSound() override;
	void UpdateObjects(bool IsGameState) override;
	bool DrawObjects() override;
	void DrawBackground(bool motionblur);
};