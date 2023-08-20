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

struct CGamePang {

	CGameBase *GameBase;
	static const int backgroundcopyheight = 30;
	static constexpr Vec2F playerscale = {2.0f*yscale,2.0f*yscale};
	static constexpr Vec2F bulletscale = {1.0f*yscale,0.8f*yscale};
	static constexpr Vec2F ballscale = {1.5f*yscale,1.5f*yscale};

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