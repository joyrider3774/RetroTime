#pragma once

#include <SDL.h>
#include <iostream>
#include <string>
#include "../CGame.h"
#include "../Common.h"

using namespace std;

class CGameBase {
	
public:
    void DrawScoreBar();
	void DrawSubStateText();
	void PauseMenu();
	CGame *Game;
	string SubStateText;
	bool UsesLevels;
	int level;
	int screenleft, screenright, screentop, screenbottom, playfieldwidth, playfieldheight, HealthPoints;
	int GameStateID;
	CGameBase(CGame *aGame, int aGameStateID, bool aUsesLevels);
	~CGameBase();
	bool UpdateLogic();
};