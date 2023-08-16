#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <map>
#include <iostream>
#include <string>
#include "Platform.h"

class CFont {

private:
	std::string DataPath;
	bool DebugInfo;
	bool GlobalFontEnabled = true;
	std::map<std::string, TTF_Font*> FontCache;
public:
	CFont(std::string AssetsPath, bool ADebugInfo);
	~CFont();
	int TextWidth(std::string Font, int FontSize, std::string Tekst, size_t NrOfChars);
	SDL_Point TextSize(std::string Font, int FontSize, std::string Tekst, size_t NrOfChars, int YSpacing);
	void WriteText(SDL_Renderer *Renderer, std::string Font, int FontSize, std::string Tekst, size_t NrOfChars, int X, int Y, int YSpacing, SDL_Color ColorIn);
};