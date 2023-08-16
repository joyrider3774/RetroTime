#pragma once

#include <SDL.h>
#include <iostream>
#include <string>
#include "../CGame.h"
#include "../Common.h"


class CGameBase {

protected:
	CGame *Game;
	std::string SubStateText;
	bool ScreenshotMode;
	bool UsesLevels;
	int level;
	int screenleft, screenright, screentop, screenbottom, playfieldwidth, playfieldheight, HealthPoints;
	void DrawScoreBar();
	void DrawSubStateText();
	void PauseMenu();
	virtual void OnGameStart();
public:
	int GameStateID;
	CGameBase(CGame *aGame, int aGameStateID, bool aUsesLevels, bool aScreenshotMode);
	virtual ~CGameBase();
	virtual void init();
	virtual void deinit();
	virtual void UpdateLogic();
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