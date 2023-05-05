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
#include "..\CTween.h"

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
    static const int sitweenenemypositions = 0;
    static const int sitweenenemydeath = 1;


    static const int sienemyrows = 5;
    static const int sienemycols = 7;
    static const int sienemystartxoffset = 50;
    static const int sienemystartyoffset = 50;
    static const int sienemyspacing = 60;
    static const int sienemyhorzspacing = 50;
    SDL_FPoint sienemyscale = {1.5,1.5};
    static const int sienemyspeed = 1;
    static constexpr float sienemyspeedinc = 0.15;
    static const int siplayerspeed = 8;
    static const int sibulletspeed = 10;
    static const int siasteroidsoffset = 150;
    static const int siendscreenconstant = 45 + siasteroidsoffset;
    static const int simaxexplosion = 10;
    static const int simaxasteroids = 6;
    static constexpr float siasteroidscale = 2.5  ;
    static const int siasteroidmaxhealthpoints = 50;
    static const int simaxenemybullets = 5;
    static const int sienemybulletrandomizer = 100;
    

	int deaths = 0;
	int pattern = 0;
	float backgroundfade = 0;
	float backgroundfadeinc = 0;
	SDL_FPoint enemyvel = {sienemyspeed,0};

	senemyinfo enemyinfo;
	CSpriteObject player;
	CSpriteObject bullet;
	CSpriteObject enemies[sienemycols * sienemycols];
	CSpriteObject explosions[simaxexplosion];
	CSpriteObject asteroids[simaxasteroids];
	CSpriteObject enemybullets[simaxenemybullets];
	
	CTweenInfo tweens[sienemycols * sienemycols][2];

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

    void siupdateplayer();
    void sicreateplayer();
    void sidestroyplayer();
    void siupdateinvaders();
    void sicreateinvaders(bool setposition);
    bool sitweenactive(int id);
    void sidestroyinvader(int index);
    void sidestroyallinvaders();
    void siupdateenemyinfo();
    void siupdatebullet();
    void sidestroybullet();
    void sicreatebullet();
    void siupdateasteroids();
    void sicreateasteroids();
    void sidestroyasteroid(int index);
    void sidestroyallasteroids();
    void siupdateenemybullet();
    void sicreatenemybullet(SDL_FPoint pos);
    void sidestroyenemybullet(int index);
    void sidestroyallenemybullet();
    void sicreateexplosion(SDL_FPoint pos);
    void sidestroyexploison(int index);
    void sidestroyallexplosion();
    void sicheckexplosions();
    void sidrawbackground(bool motionblur);
public:
    CGameInvaders(CGame* aGame, bool aScreenshotMode = false);
    ~CGameInvaders();
    SDL_Texture* screenshot() override;
    void init() override;
    void deinit() override;
    void UpdateLogic() override;
    void Draw() override;
    void UnloadGraphics() override;
    void LoadGraphics() override;
};