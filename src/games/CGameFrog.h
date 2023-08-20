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

#define CEILING(x,y) (((x) + (y) + 1) / (y));
#define FLOORING(x,y) (((x) + (y) - 1) / (y));

struct SRowType {
	int type;
	int backgroundtile;
	int backgroundtileend;
	bool deadlyground;
	int id;
	float speed;
	int minspace;
	int maxrepeats;
};
typedef struct SRowType SRowType;

struct SObjectInfo {
	int mostleft = -1;
	int mostright = -1;
	int mostbottom = -1;
	int mosttop = -1;
};

typedef struct SObjectInfo SObjectInfo;


struct CGameFrog {

	CGameBase *GameBase;
	static const int debugmode = false;
	static constexpr float playerspeed = 16.0f*3.0f*yscale;
	static constexpr float globalworldspeed = 0.5f*yscale;

	static const int rowtypewater = 0;
	static const int rowtyperoad = 1;
	static const int rowtypesafety = 2;

	static const int idenemycar1 = 0;
	static const int idenemycar2 = 1;
	static const int idenemycar3 = 2;
	static const int idenemycar4 = 3;
	static const int idenemycar5 = 4;
	static const int idenemyplant = 5;
	static const int idenemylog = 6;
	static const int idnone = 7;
	static const int idwater = 8;
	static const int idroad = 9;
	static const int idgrass = 10;
	static const int idcherry = 11;
	static const int idapple = 12;
	static const int idlemon = 13;


	static const int cherryspawntrigger = 103;
	static const int applespawntrigger = 57;
	static const int lemonspawntrigger = 23;

	static const int lemonscore = 10;
	static const int applescore = 25;
	static const int cherryscore = 50;

	static const int lenlevelincspeeds = 5;
	
	static const int numcols = 14;
	static const int generatorrows = 1;
	static const int maxrowsbeforesafetyrow = 4;
	static const int visiblerows = (int)CEILING(ScreenHeight, (int)playerspeed);
	static const int numrows = visiblerows + generatorrows;
	static const int playerstartrow = (int)FLOORING(visiblerows, 2)

	static constexpr float speeddeviation = 0.25f*yscale;

	static const int rowtypewaterstart = 0;
	static const int rowtypewaterend = 2;
	static const int rowtyperoadstart = 3;
	static const int rowtyperoadend = 7;
	static const int maxobjects = (int)((visiblerows + generatorrows) * numcols * 1.57);

	static const int lenrowtypes = 9;

	SRowType rowtypes[lenrowtypes];
	float levelincspeeds[lenlevelincspeeds];
	int levelincspawns[lenlevelincspeeds];

	float worldspeed;
	int nextrowtype;
	int nextrowtypecountmax;
	int nextrowtypecount;
	int numobjects;
	int previd;
	int prevrowtype;
	int playerdeath;
	int playermaxrow;
	int playerrow;
	int plantsspawned;
	int rowsspawned;
	int dolevelinc;
	SDL_Point backgroundtz;
	CSpriteObject objects[maxobjects];
	CSpriteObject player;
	SObjectInfo objectinfo;


	int MusMusic, SfxSucces, SfxCollect, SfxDie, SfxPlayerMove;

	int background;
	int spritesheetfrog;
	int spritesheetbackground;
	int spritesheetcar1;
	int spritesheetcar2;
	int spritesheetcar3;
	int spritesheetcar4;
	int spritesheetcar5;

	int spritesheetplant;
	int spritesheetfruit1;
	int spritesheetfruit2;
	int spritesheetfruit3;


	void (*updateplayer)(CGameFrog* GameFrog);
	void (*createplayer)(CGameFrog* GameFrog);
	void (*destroyplayer)(CGameFrog* GameFrog);
	void (*updateobjects)(CGameFrog* GameFrog);
	void (*createobjectrow)(CGameFrog* GameFrog, int rownr, int arowtype);
	int (*createobject)(CGameFrog* GameFrog, int rownr, int col, int id, int arowtype, float speed, int dir, int startobjectindex);
	void (*destroyobject)(CGameFrog* GameFrog, int index);
	void (*destroyallobjects)(CGameFrog* GameFrog);
	void (*createobjects)(CGameFrog* GameFrog, bool initialize);
	void (*OnGameStart)(CGameFrog* GameFrog);
	void (*init)(CGameFrog* GameFrog);
	void (*deinit)(CGameFrog* GameFrog);
	void (*UpdateLogic)(CGameFrog* GameFrog);
	void (*Draw)(CGameFrog* GameFrog);
	void (*UnloadGraphics)(CGameFrog* GameFrog);
	void (*LoadGraphics)(CGameFrog* GameFrog);
	void (*LoadSound)(CGameFrog* GameFrog);
	void (*UnLoadSound)(CGameFrog* GameFrog);
	void (*DrawBackground)(CGameFrog* GameFrog);
};
typedef struct CGameFrog CGameFrog;

void CGameFrog_Draw(CGameFrog* GameFrog);
void CGameFrog_UpdateLogic(CGameFrog* GameFrog);
void CGameFrog_OnGameStart(CGameFrog* GameFrog);
void CGameFrog_UnloadGraphics(CGameFrog* GameFrog);
void CGameFrog_LoadGraphics(CGameFrog* GameFrog);
void CGameFrog_UnLoadSound(CGameFrog* GameFrog);
void CGameFrog_LoadSound(CGameFrog* GameFrog);
void CGameFrog_deinit(CGameFrog* GameFrog);
void CGameFrog_init(CGameFrog* GameFrog);
void CGameFrog_DrawBackground(CGameFrog* GameFrog);
void CGameFrog_updateplayer(CGameFrog* GameFrog);
void CGameFrog_createplayer(CGameFrog* GameFrog);
void CGameFrog_destroyplayer(CGameFrog* GameFrog);
void CGameFrog_updateobjects(CGameFrog* GameFrog);
void CGameFrog_createobjectrow(CGameFrog* GameFrog, int rownr, int arowtype);
int CGameFrog_createobject(CGameFrog* GameFrog, int rownr, int col, int id, int arowtype, float speed, int dir, int startobjectindex);
void CGameFrog_destroyobject(CGameFrog* GameFrog, int index);
void CGameFrog_destroyallobjects(CGameFrog* GameFrog);
void CGameFrog_createobjects(CGameFrog* GameFrog, bool initialize);
void Destroy_CGameFrog(CGameFrog* GameFrog);
CGameFrog* Create_CGameFrog();