#ifndef CGAMEFROG_H
#define CGAMEFROG_H

#include <SDL.h>
#include <stdbool.h>
#include "CGameBase.h"
#include "CSpriteObject.h"
#include "../CGame.h"
#include "../Platform.h"
#include "../Common.h"



#define CEILING(x,y) (((x) + (y) + 1) / (y))
#define FLOORING(x,y) (((x) + (y) - 1) / (y))

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
	int mostleft;
	int mostright;
	int mostbottom;
	int mosttop;
};

typedef struct SObjectInfo SObjectInfo;


#define CGameFrog_debugmode false
#define CGameFrog_playerspeed 16.0f*3.0f*yscale
#define CGameFrog_globalworldspeed 0.5f*yscale

#define CGameFrog_rowtypewater 0
#define CGameFrog_rowtyperoad 1
#define CGameFrog_rowtypesafety 2

#define CGameFrog_idenemycar1 0
#define CGameFrog_idenemycar2 1
#define CGameFrog_idenemycar3 2
#define CGameFrog_idenemycar4 3
#define CGameFrog_idenemycar5 4
#define CGameFrog_idenemyplant 5
#define CGameFrog_idenemylog 6
#define CGameFrog_idnone 7
#define CGameFrog_idwater 8
#define CGameFrog_idroad 9
#define CGameFrog_idgrass 10
#define CGameFrog_idcherry 11
#define CGameFrog_idapple 12
#define CGameFrog_idlemon 13


#define CGameFrog_cherryspawntrigger 103
#define CGameFrog_applespawntrigger 57
#define CGameFrog_lemonspawntrigger 23

#define CGameFrog_lemonscore 10
#define CGameFrog_applescore 25
#define CGameFrog_cherryscore 50

#define CGameFrog_lenlevelincspeeds 5

#define CGameFrog_numcols 14
#define CGameFrog_generatorrows 1
#define CGameFrog_maxrowsbeforesafetyrow 4
#define CGameFrog_visiblerows 16 //(int)CEILING(ScreenHeight, (int)CGameFrog_playerspeed) or screensheight / playerspeed ceiled
#define CGameFrog_numrows 17 //CGameFrog_visiblerows + CGameFrog_generatorrows
#define CGameFrog_playerstartrow 7 //(int)FLOORING(CGameFrog_visiblerows, 2) or visible rows / 2 floored

#define CGameFrog_speeddeviation 0.25f*yscale

#define CGameFrog_rowtypewaterstart 0
#define CGameFrog_rowtypewaterend 2
#define CGameFrog_rowtyperoadstart 3
#define CGameFrog_rowtyperoadend 7
#define CGameFrog_maxobjects 1000

#define CGameFrog_lenrowtypes 9

struct CGameFrog {

	CGameBase *GameBase;

	SRowType rowtypes[CGameFrog_lenrowtypes];
	float levelincspeeds[CGameFrog_lenlevelincspeeds];
	int levelincspawns[CGameFrog_lenlevelincspeeds];

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
	CSpriteObject objects[CGameFrog_maxobjects];
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

#endif