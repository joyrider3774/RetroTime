#pragma once

#include <SDL.h>
#include <limits.h>
#include <map>
#include <iostream>
#include <string>
#include "CGameBase.h"
#include "CSpriteObject.h"
#include "..\CGame.h"
#include "..\Platform.h"
#include "..\Common.h"

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


class CGameFrog: public CGameBase {

private:
           
    static const int debugmode = false;
    static constexpr float playerspeed = 16*3;
    static constexpr float globalworldspeed = 0.5;

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
    float levelincspeeds[lenlevelincspeeds] = {0, 0.25, 0.5, 1.0, 1.5};
    int levelincspawns[lenlevelincspeeds] = {30,  90,  240, 570,   0};

    static const int numcols = 14;
    static const int generatorrows = 1;
    static const int maxrowsbeforesafetyrow = 4;
    static const int visiblerows = (int)CEILING(ScreenHeight, (int)playerspeed);
    static const int numrows = visiblerows + generatorrows;
    static const int playerstartrow = (int)FLOORING(visiblerows, 2)

    static constexpr float speeddeviation = 0.25;

    static const int rowtypewaterstart = 0;
    static const int rowtypewaterend = 2;
    static const int rowtyperoadstart = 3;
    static const int rowtyperoadend = 7;
    static const int maxobjects = (int)((visiblerows + generatorrows) * numcols * 1.57);
    
    static const int lenrowtypes = 9;

    SRowType rowtypes[lenrowtypes] = {
	{rowtypewater, 6, 10, true, idenemyplant, 1, 2, 4},
	{rowtypewater, 6, 10, true, idenemyplant, 2, 3, 2},
	{rowtypewater, 6, 10, true, idenemyplant, 3, 4, 3},

	{rowtyperoad, 0, 0, false, idenemycar1, 2, 4, 2}, //ambulance
	{rowtyperoad, 0, 0, false, idenemycar2, 1, 3, 2}, //truck
	{rowtyperoad, 0, 0, false, idenemycar3, 7, numrows, 1}, //formula 1
	{rowtyperoad, 0, 0, false, idenemycar4, 1.5, 4, 2}, //dumptruck
	{rowtyperoad, 0, 0, false, idenemycar5, 2.5, 3, 2}, //jeep

	{rowtypesafety, 12, 12,false, idnone, 2, 2, 2}
    };

    
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
    
    
    void updateplayer();
    void createplayer();
    void destroyplayer();	
    void updateobjects();
    void createobjectrow(int rownr, int arowtype);
    int createobject(int rownr, int col, int id, int arowtype, float speed, int dir, int startobjectindex);
    void destroyobject(int index);
    void destroyallobjects();
    void createobjects(bool initialize);
    void drawbackground();
    void OnGameStart() override;
public:
    CGameFrog(CGame* aGame, bool aScreenshotMode = false);
    ~CGameFrog();
    SDL_Texture* screenshot() override;
    void init() override;
    void deinit() override;
    void UpdateLogic() override;
    void Draw() override;
    void UnloadGraphics() override;
    void LoadGraphics() override;
};