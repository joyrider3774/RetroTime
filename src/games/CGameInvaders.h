#ifndef CGAMEINVADERS_H
#define CGAMEINVADERS_H

#include <SDL.h>
#include <stdbool.h>
#include "CGameBase.h"
#include "CSpriteObject.h"
#include "../CGame.h"
#include "../Platform.h"
#include "../Common.h"
#include "../CTween.h"
#include "../Vec2F.h"


#define CGameInvaders_tweenenemypositions 0
#define CGameInvaders_tweenenemydeath 1

#define CGameInvaders_enemyrows 5
#define CGameInvaders_enemycols 7
#define CGameInvaders_enemystartxoffset (50*xscale)
#define CGameInvaders_enemystartyoffset (50*yscale)
#define CGameInvaders_enemyspacing (60*yscale)
#define CGameInvaders_enemyhorzspacing (50*xscale)

#define CGameInvaders_enemyspeed (1.0f*xscale)
#define CGameInvaders_enemyspeedinc (0.15f*xscale)
#define CGameInvaders_playerspeed (8.0f*xscale)
#define CGameInvaders_bulletspeed (10.0f*yscale)
#define CGameInvaders_asteroidsoffset (150*xscale)
#define CGameInvaders_endscreenconstant ((45*xscale) + 150*xscale)
#define CGameInvaders_maxexplosion 10
#define CGameInvaders_maxasteroids 6
#define CGameInvaders_asteroidscale (2.5f*xscale)
#define CGameInvaders_asteroidmaxhealthpoints 50
#define CGameInvaders_maxenemybullets 5
#define CGameInvaders_enemybulletrandomizer 100

#define CGameInvaders_maxenemies 35 //rows * cols

struct senemyinfo
{
	int mostleft;
	int mostright;
	int mostbottom;
};
typedef struct senemyinfo senemyinfo;


struct CGameInvaders {

	CGameBase *GameBase;
	
	int deaths;
	int pattern;
	float backgroundfade;
	float backgroundfadeinc;
	Vec2F enemyvel;
	Vec2F enemyscale;
	
	senemyinfo enemyinfo;
	CSpriteObject player;
	CSpriteObject bullet;
	CSpriteObject enemies[CGameInvaders_maxenemies];
	CSpriteObject explosions[CGameInvaders_maxexplosion];
	CSpriteObject asteroids[CGameInvaders_maxasteroids];
	CSpriteObject enemybullets[CGameInvaders_maxenemybullets];

	CTweenInfo tweens[CGameInvaders_maxenemies][2];

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

#endif