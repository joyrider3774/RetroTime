#pragma once

#include <SDL.h>
#include <iostream>
#include <string>
#include "..\CGame.h"
#include "..\Common.h"

using namespace std;



class CGameBase {

protected:
    CGame *Game;
    string SubStateText;
    bool UsesLevels;
    int level;
    int screenleft, screenright, screentop, screenbottom, playfieldwidth, playfieldheight, HealthPoints;
    void DrawScoreBar();
    void DrawSubStateText();
    void PauseMenu();
public:
    int GameStateID;
    CGameBase(CGame *aGame, int aGameStateID, bool aUsesLevels);
    virtual ~CGameBase();
    virtual void init();
    virtual void deinit();
    virtual void UpdateLogic();
    virtual void Draw();
    virtual void LoadGraphics();
    virtual void UnloadGraphics();
    virtual SDL_Texture* screenshot();
};