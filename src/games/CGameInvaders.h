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


struct CGameInvaders {

	CGameBase *GameBase;
	static const int tweenenemypositions = 0;
	static const int tweenenemydeath = 1;


	static const int enemyrows = 5;
	static const int enemycols = 7;
	static const int enemystartxoffset = 50*xscale;
	static const int enemystartyoffset = 50*yscale;
	static const int enemyspacing = 60*yscale;
	static const int enemyhorzspacing = 50*xscale;
	static constexpr Vec2F enemyscale = {1.5f*xscale,1.5f*yscale};
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

	void (*updateplayer)(CGameInvaders* GameInvaders);
	void (*createplayer)(CGameInvaders* GameInvaders);
	void (*destroyplayer)(CGameInvaders* GameInvaders);
	void (*updateinvaders)(CGameInvaders* GameInvaders);
	void (*createinvaders)(CGameInvaders* GameInvaders,bool setposition);
	bool (*tweenactive)(CGameInvaders* GameInvaders,int id);
	void (*destroyinvader)(CGameInvaders* GameInvaders,int index);
	void (*destroyallinvaders)(CGameInvaders* GameInvaders);
	void (*updateenemyinfo)(CGameInvaders* GameInvaders);
	void (*updatebullet)(CGameInvaders* GameInvaders);
	void (*destroybullet)(CGameInvaders* GameInvaders);
	void (*createbullet)(CGameInvaders* GameInvaders);
	void (*updateasteroids)(CGameInvaders* GameInvaders);
	void (*createasteroids)(CGameInvaders* GameInvaders);
	void (*destroyasteroid)(CGameInvaders* GameInvaders,int index);
	void (*destroyallasteroids)(CGameInvaders* GameInvaders);
	void (*updateenemybullet)(CGameInvaders* GameInvaders);
	void (*createnemybullet)(CGameInvaders* GameInvaders,Vec2F pos);
	void (*destroyenemybullet)(CGameInvaders* GameInvaders,int index);
	void (*destroyallenemybullet)(CGameInvaders* GameInvaders);
	void (*createexplosion)(CGameInvaders* GameInvaders,Vec2F pos);
	void (*destroyexploison)(CGameInvaders* GameInvaders,int index);
	void (*destroyallexplosion)(CGameInvaders* GameInvaders);
	void (*checkexplosions)(CGameInvaders* GameInvaders);


	void (*init)(CGameInvaders* GameInvaders);
	void (*deinit)(CGameInvaders* GameInvaders);
	void (*UnloadGraphics)(CGameInvaders* GameInvaders);
	void (*LoadGraphics)(CGameInvaders* GameInvaders);
	void (*LoadSound)(CGameInvaders* GameInvaders);
	void (*UnLoadSound)(CGameInvaders* GameInvaders);
	void (*UpdateObjects)(CGameInvaders* GameInvaders,bool IsGameState);
	bool (*DrawObjects)(CGameInvaders* GameInvaders);
	void (*DrawBackground)(CGameInvaders* GameInvaders);
	void (*Draw)(CGameInvaders* GameInvaders);
	void (*UpdateLogic)(CGameInvaders* GameInvaders);
};
typedef struct CGameInvaders CGameInvaders;

void CGameInvaders_Draw(CGameInvaders* GameInvaders);
bool CGameInvaders_DrawObjects(CGameInvaders* GameInvaders);
void CGameInvaders_UpdateLogic(CGameInvaders* GameInvaders);
void CGameInvaders_UpdateObjects(CGameInvaders* GameInvaders, bool IsGameState);
void CGameInvaders_UnloadGraphics(CGameInvaders* GameInvaders);
void CGameInvaders_LoadGraphics(CGameInvaders* GameInvaders);
void CGameInvaders_UnLoadSound(CGameInvaders* GameInvaders);
void CGameInvaders_LoadSound(CGameInvaders* GameInvaders);
void CGameInvaders_deinit(CGameInvaders* GameInvaders);
void CGameInvaders_init(CGameInvaders* GameInvaders);
void CGameInvaders_DrawBackground(CGameInvaders* GameInvaders);
void CGameInvaders_updateplayer(CGameInvaders* GameInvaders);
void CGameInvaders_createplayer(CGameInvaders* GameInvaders);
void CGameInvaders_destroyplayer(CGameInvaders* GameInvaders);
void CGameInvaders_updateinvaders(CGameInvaders* GameInvaders);
void CGameInvaders_createinvaders(CGameInvaders* GameInvaders, bool setposition);
bool CGameInvaders_tweenactive(CGameInvaders*, int id);
void CGameInvaders_destroyinvader(CGameInvaders* GameInvaders, int index);
void CGameInvaders_destroyallinvaders(CGameInvaders* GameInvaders);
void CGameInvaders_updateenemyinfo(CGameInvaders* GameInvaders);
void CGameInvaders_updatebullet(CGameInvaders* GameInvaders);
void CGameInvaders_destroybullet(CGameInvaders* GameInvaders);
void CGameInvaders_createbullet(CGameInvaders* GameInvaders);
void CGameInvaders_updateasteroids(CGameInvaders* GameInvaders);
void CGameInvaders_createasteroids(CGameInvaders* GameInvaders);
void CGameInvaders_destroyasteroid(CGameInvaders* GameInvaders, int index);
void CGameInvaders_destroyallasteroids(CGameInvaders* GameInvaders);
void CGameInvaders_updateenemybullet(CGameInvaders* GameInvaders);
void CGameInvaders_createnemybullet(CGameInvaders* GameInvaders, Vec2F pos);
void CGameInvaders_destroyenemybullet(CGameInvaders* GameInvaders, int index);
void CGameInvaders_destroyallenemybullet(CGameInvaders* GameInvaders);
void CGameInvaders_createexplosion(CGameInvaders* GameInvaders, Vec2F pos);
void CGameInvaders_destroyexploison(CGameInvaders* GameInvaders, int index);
void CGameInvaders_destroyallexplosion(CGameInvaders* GameInvaders);
void CGameInvaders_checkexplosions(CGameInvaders* GameInvaders);
void Destroy_CGameInvaders(CGameInvaders* GameInvaders);
CGameInvaders* Create_CGameInvaders();