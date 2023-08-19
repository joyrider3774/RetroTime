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
	virtual void init();
	virtual void deinit();
	virtual bool UpdateLogic();
	virtual void Draw();
	virtual void DrawBackground(bool motionblur);
	virtual void LoadGraphics();
	virtual void UnloadGraphics();
	virtual void LoadSound();
	virtual void UnLoadSound();
	virtual void UpdateObjects(bool IsGameState);
	virtual bool DrawObjects();
	virtual SDL_Texture* screenshot();
};