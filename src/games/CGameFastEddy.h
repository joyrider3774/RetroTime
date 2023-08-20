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

using namespace std;

struct CGameFastEddy {
	CGameBase *GameBase;
	static const int playerstateunknown = -1;
	static const int playerstateidle = 0;
	static const int playerstateleft = 1;
	static const int playerstateleftjump = 2;
	static const int playerstateright = 3;
	static const int playerstaterightjump = 4;
	static const int playerstatejump = 5;
	static const int playerstateclimbup = 6;
	static const int playerstateclimbdown = 7;

	static const int enemystateidle = 0;
	static const int enemystatemoveleft = 1;
	static const int enemystatewaitmove = 2;
	static const int enemystatemoveright = 3;

	static const int collectablestatemoveleft = 0;
	static const int collectablestatemoveright = 1;
	static const int collectablestateidle = 2;

	static const int laddersfitrows = 10;
	static constexpr float rowfloorsizex = 400.0f * xscale; //tilesize / 2 oroginals are 64x64 was 32x32 but i tried to divide by 3 but actually divided by 3.2
	static constexpr float rowfloorsizey = 32.0f * yscale; //tilesize / 2 oroginals are 64x64 was 32x32 but i tried to divide by 3 but actually divided by 3.2
	static const int rows = 5;

	static constexpr float playerspeed = 6.4f*xscale;
	static constexpr float playerjumpspeed = 2.0f;
	static constexpr float playerclimbspeed = 2.0f * yscale;
	static constexpr float collectablespeed = 2*xscale;

	static const int maxenemies = 10;
	static const int maxcollectables = 2;

	static const int maxladders = (rows - 1) * 2;
	float playerjumpspeeddec;

	int numfloortilesperrow;
	int maxfloortiles;
	float rowspacingsize;
	float ladderwidth;
	float rowsize;
	float playerheight;
	float enemyheight;
	float playerjumpheight;
	float collectableheight;
	float keyheight;

	int spritesheet;// = loadImage("EderMuniz/SnowyForestTiles")
	int spritesheetladder;// = loadImage("EderMuniz/SnowyForestLadder")
	int spritesheetplayerclimb;// = loadImage("Ravenmore/characterClimb")
	int spritesheetplayerrun;// = loadImage("Ravenmore/characterRun")
	int spritesheetplayeridle;// = loadImage("Ravenmore/characterIdle")
	int spritesheetplayerjump;// = loadImage("Ravenmore/characterJumpUp")
	int spritesheetenemy;// = loadImage("Pipo/char59")
	int spritesheetenemy2;// = loadImage("Pipo/char59") 
	int spritesheetcollectable;// = loadImage("Ravenmore/platformPickups")
	int spritesheetkey;// = loadImage("Ravenmore/PlatformDefaultProps")
	int background;// = loadImage("EderMuniz/SnowyForestBackA")
	SDL_Point backgroundtz;// = imageSize(febackground)

	int MusMusic, SfxSucces, SfxCollect, SfxDie;

	int rowzeroenemyindex = 0;
	int collecteditems = 0;
	int collectedcreated = 0;
	CSpriteObject player;
	CSpriteObject ladders[maxladders];
	CSpriteObject floors[(int)(ScreenWidth / rowfloorsizex * rows)]; //1280 is playfield size
	CSpriteObject enemies[maxenemies];
	CSpriteObject collectables[maxcollectables];
	CSpriteObject key;

	void (*createkey)(CGameFastEddy* GameFastEddy);
	void (*destroykey)(CGameFastEddy* GameFastEddy);
	void (*updatekey)(CGameFastEddy* GameFastEddy);
	void (*createladders)(CGameFastEddy* GameFastEddy);
	void (*destroyladders)(CGameFastEddy* GameFastEddy);
	void (*createfloors)(CGameFastEddy* GameFastEddy);
	void (*destroyfloors)(CGameFastEddy* GameFastEddy);
	void (*updateplayer)(CGameFastEddy* GameFastEddy);
	void (*createplayer)(CGameFastEddy* GameFastEddy);
	void (*destroyplayer)(CGameFastEddy* GameFastEddy);
	void (*updateenemies)(CGameFastEddy* GameFastEddy);
	void (*createenemies)(CGameFastEddy* GameFastEddy,bool levelsucces);
	void (*createenemy)(CGameFastEddy* GameFastEddy,int row, float x, int state, int group, int multiply);
	void (*enemyenablelevelend)(CGameFastEddy* GameFastEddy);
	void (*destroyenemies)(CGameFastEddy* GameFastEddy);
	void (*updatecollectables)(CGameFastEddy* GameFastEddy);
	void (*createcollectables)(CGameFastEddy* GameFastEddy,int ignorerow);
	void (*destroycollectable)(CGameFastEddy* GameFastEddy,int index);
	void (*destroyallcollectables)(CGameFastEddy* GameFastEddy);
	void (*init)(CGameFastEddy* GameFastEddy);
	void (*deinit)(CGameFastEddy* GameFastEddy);
	void (*UnloadGraphics)(CGameFastEddy* GameFastEddy);
	void (*LoadGraphics)(CGameFastEddy* GameFastEddy);
	void (*LoadSound)(CGameFastEddy* GameFastEddy);
	void (*UnLoadSound)(CGameFastEddy* GameFastEddy);
	void (*UpdateObjects)(CGameFastEddy* GameFastEddy, bool IsGameState);
	void (*DrawBackground)(CGameFastEddy* GameFastEddy);
	void (*Draw)(CGameFastEddy* GameFastEddy);
	void (*UpdateLogic)(CGameFastEddy* GameFastEddy);
};
typedef struct CGameFastEddy CGameFastEddy;

void CGameFastEddy_Draw(CGameFastEddy* GameFastEddy);
void CGameFastEddy_UpdateLogic(CGameFastEddy* GameFastEddy);
void CGameFastEddy_UpdateObjects(CGameFastEddy* GameFastEddy, bool IsGameState);
void CGameFastEddy_UnloadGraphics(CGameFastEddy* GameFastEddy);
void CGameFastEddy_LoadGraphics(CGameFastEddy* GameFastEddy);
void CGameFastEddy_UnLoadSound(CGameFastEddy* GameFastEddy);
void CGameFastEddy_LoadSound(CGameFastEddy* GameFastEddy);
void CGameFastEddy_deinit(CGameFastEddy* GameFastEddy);
void CGameFastEddy_init(CGameFastEddy* GameFastEddy);
void CGameFastEddy_DrawBackground(CGameFastEddy* GameFastEddy);
void CGameFastEddy_createladders(CGameFastEddy* GameFastEddy);
void CGameFastEddy_destroyladders(CGameFastEddy* GameFastEddy);
void CGameFastEddy_createfloors(CGameFastEddy* GameFastEddy);
void CGameFastEddy_destroyfloors(CGameFastEddy* GameFastEddy);
void CGameFastEddy_updateplayer(CGameFastEddy* GameFastEddy);
void CGameFastEddy_createplayer(CGameFastEddy* GameFastEddy);
void CGameFastEddy_destroyplayer(CGameFastEddy* GameFastEddy);
void CGameFastEddy_updateenemies(CGameFastEddy* GameFastEddy);
void CGameFastEddy_createenemies(CGameFastEddy* GameFastEddy, bool levelsucces);
void CGameFastEddy_createenemy(CGameFastEddy* GameFastEddy, int row, float x, int state, int group, int multiply);
void CGameFastEddy_enemyenablelevelend(CGameFastEddy* GameFastEddy);
void CGameFastEddy_destroyenemies(CGameFastEddy* GameFastEddy);
void CGameFastEddy_updatecollectables(CGameFastEddy* GameFastEddy);
void CGameFastEddy_createcollectables(CGameFastEddy* GameFastEddy, int ignorerow);
void CGameFastEddy_destroycollectable(CGameFastEddy* GameFastEddy, int index);
void CGameFastEddy_destroyallcollectables(CGameFastEddy* GameFastEddy);
void CGameFastEddy_updatekey(CGameFastEddy* GameFastEddy);
void CGameFastEddy_destroykey(CGameFastEddy* GameFastEddy);
void CGameFastEddy_createkey(CGameFastEddy* GameFastEddy);
void Destroy_CGameFastEddy(CGameFastEddy* GameFastEddy);
CGameFastEddy* Create_CGameFastEddy();