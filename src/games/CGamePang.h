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

class CGamePang: public CGameBase {

private:
    static const int pabackgroundcopyheight = 30;
    SDL_FPoint paplayerscale = {2,2};
    SDL_FPoint pabulletscale = {1,0.8};
    SDL_FPoint paballscale = {1.5,1.5};

    static const int paenemyspeed = 1;
    static const int paplayerspeed = 8;
    static const int pabulletspeed = 10;

    static const int paplayerstateidle = 0;
    static const int paplayerstatemoveleft = 2;
    static const int paplayerstatemoveright = 4;
    static const int paplayerstateshoot = 8;
    static const int paplayerstatereviving = 16;


    static const int paballbig = 3;
    static const int paballmedium = 2;
    static const int paballsmall = 1;

    static const int pamaxbigballs = 6;
    static const int pamaxballs = pamaxbigballs * 7;


    int background;
    int paspritesheetplayer;
    int paspritesheetbullet;
    int paspritesheetball;
    int pabackgroundgrass;
    int pabackgroundcloud;
    int pabackgroundtrees;
    int pabackgroundtree;


    SDL_Point backgroundtz;
	SDL_Point pabackgroundgrasstz;
    SDL_Point pabackgroundcloudtz;
    SDL_Point pabackgroundtreestz;
    SDL_Point pabackgroundtreetz;
    SDL_Point paspritesheetballtz;

    int deaths = 0;
	int levelcleared = false;
	CSpriteObject balls[pamaxballs];
	CSpriteObject player;
	CSpriteObject bullet;

    int MusMusic, SfxSucces, SfxDie, SfxShoot, SfxPop;


    void padrawplayer();
    void paupdateplayer();
    void paremplayerstate(int state);
    bool paplayerstate(int state);
    void paaddplayerstate(int state);
    void pacreateplayer();
    void padestroyplayer();
    void padrawbullet();
    void paupdatebullet();
    void pacreatebullet();
    void padestroybullet();    
    void pacreateballs();
    void padrawballs();
    void paupdateballs();
    void pacreateball(int size, float x, float y, float speed);
    void padestroyball(int index, bool nocreate);
    void padestroyallballs();
    void padrawbackground(bool motionblur);
public:
    CGamePang(CGame* aGame, bool aScreenshotMode = false);
    ~CGamePang();
    SDL_Texture* screenshot() override;
    void init() override;
    void deinit() override;
    void UpdateLogic() override;
    void Draw() override;
    void UnloadGraphics() override;
    void LoadGraphics() override;
};