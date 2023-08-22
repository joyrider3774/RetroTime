#ifndef CGAMEPANG_H
#define CGAMEPANG_H

#include <SDL.h>
#include <stdbool.h>
#include "CGameBase.h"
#include "CSpriteObject.h"
#include "../CGame.h"
#include "../Platform.h"
#include "../Common.h"
#include "../Vec2F.h"

#define CGamePang_backgroundcopyheight 30

#define CGamePang_enemyspeed 1.0f*yscale
#define CGamePang_playerspeed 8.0f*xscale
#define CGamePang_bulletspeed 10.0f*yscale

#define CGamePang_playerstateidle 0
#define CGamePang_playerstatemoveleft 2
#define CGamePang_playerstatemoveright 4
#define CGamePang_playerstateshoot 8
#define CGamePang_playerstatereviving 16

#define CGamePang_ballbig 3
#define CGamePang_ballmedium 2
#define CGamePang_ballsmall 1

#define CGamePang_maxbigballs 6
#define CGamePang_maxballs 42 //maxbigballs * 7

struct CGamePang {

	CGameBase *GameBase;

	Vec2F playerscale;
	Vec2F bulletscale;
	Vec2F ballscale;

	int background;
	int spritesheetplayer;
	int spritesheetbullet;
	int spritesheetball;


	SDL_Point backgroundtz;
	SDL_Point spritesheetballtz;

	int deaths;
	int levelcleared;
	CSpriteObject balls[CGamePang_maxballs];
	CSpriteObject player;
	CSpriteObject bullet;

	int MusMusic, SfxSucces, SfxDie, SfxShoot, SfxPop;


	void (*drawplayer)(CGamePang* GamePang);
	void (*updateplayer)(CGamePang* GamePang);
	void (*remplayerstate)(CGamePang* GamePang, int state);
	bool (*playerstate)(CGamePang* GamePang, int state);
	void (*addplayerstate)(CGamePang* GamePang, int state);
	void (*createplayer)(CGamePang* GamePang);
	void (*destroyplayer)(CGamePang* GamePang);
	void (*drawbullet)(CGamePang* GamePang);
	void (*updatebullet)(CGamePang* GamePang);
	void (*createbullet)(CGamePang* GamePang);
	void (*destroybullet)(CGamePang* GamePang);
	void (*createballs)(CGamePang* GamePang);
	void (*drawballs)(CGamePang* GamePang);
	void (*updateballs)(CGamePang* GamePang);
	void (*createball)(CGamePang* GamePang, int size, float x, float y, float speed);
	void (*destroyball)(CGamePang* GamePang, int index, bool nocreate);
	void (*destroyallballs)(CGamePang* GamePang);

	void (*init)(CGamePang* GamePang);
	void (*deinit)(CGamePang* GamePang);
	void (*UnloadGraphics)(CGamePang* GamePang);
	void (*LoadGraphics)(CGamePang* GamePang);
	void (*LoadSound)(CGamePang* GamePang);
	void (*UnLoadSound)(CGamePang* GamePang);
	void (*UpdateObjects)(CGamePang* GamePang, bool IsGameState);
	bool (*DrawObjects)(CGamePang* GamePang);
	void (*DrawBackground)(CGamePang* GamePang);
	void (*Draw)(CGamePang* GamePang);
	void (*UpdateLogic)(CGamePang* GamePang);
};

typedef struct CGamePang CGamePang;

void CGamePang_Draw(CGamePang* GamePang);
bool CGamePang_DrawObjects(CGamePang* GamePang);
void CGamePang_UpdateLogic(CGamePang* GamePang);
void CGamePang_UpdateObjects(CGamePang* GamePang, bool IsGameState);
void CGamePang_UnloadGraphics(CGamePang* GamePang);
void CGamePang_LoadGraphics(CGamePang* GamePang);
void CGamePang_UnLoadSound(CGamePang* GamePang);
void CGamePang_LoadSound(CGamePang* GamePang);
void CGamePang_deinit(CGamePang* GamePang);
void CGamePang_init(CGamePang* GamePang);
void CGamePang_DrawBackground(CGamePang* GamePang);
void CGamePang_drawplayer(CGamePang* GamePang);
void CGamePang_updateplayer(CGamePang* GamePang);
void CGamePang_remplayerstate(CGamePang* GamePang, int state);
bool CGamePang_playerstate(CGamePang* GamePang, int state);
void CGamePang_addplayerstate(CGamePang* GamePang, int state);
void CGamePang_createplayer(CGamePang* GamePang);
void CGamePang_destroyplayer(CGamePang* GamePang);
void CGamePang_drawbullet(CGamePang* GamePang);
void CGamePang_updatebullet(CGamePang* GamePang);
void CGamePang_createbullet(CGamePang* GamePang);
void CGamePang_destroybullet(CGamePang* GamePang);
void CGamePang_createballs(CGamePang* GamePang);
void CGamePang_drawballs(CGamePang* GamePang);
void CGamePang_updateballs(CGamePang* GamePang);
void CGamePang_createball(CGamePang* GamePang, int size, float x, float y, float speed);
void CGamePang_destroyball(CGamePang* GamePang, int index, bool nocreate);
void CGamePang_destroyallballs(CGamePang* GamePang);
void Destroy_CGamePang(CGamePang* GamePang);
CGamePang* Create_CGamePang();

#endif