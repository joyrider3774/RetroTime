#pragma once

#include <SDL.h>
#include <iostream>
#include <string>
#include "../CGame.h"
#include "../Common.h"

using namespace std;

typedef void (*UpdateObjectsFunc)(bool IsGameState);
typedef void (*OnGameStartFunc)();


class CGameBase {
	
public:
    void DrawScoreBar();
	void DrawSubStateText();
	void PauseMenu();
	virtual void OnGameStart();
	CGame *Game;
	string SubStateText;
	bool ScreenshotMode;
	bool UsesLevels;
	int level;
	int screenleft, screenright, screentop, screenbottom, playfieldwidth, playfieldheight, HealthPoints;
	int GameStateID;
	CGameBase(CGame *aGame, int aGameStateID, bool aUsesLevels, bool aScreenshotMode);
	virtual ~CGameBase();
	virtual bool UpdateLogic();
};