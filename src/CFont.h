#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <limits.h>
#include <map>
#include <iostream>
#include <string>
#include "Platform.h"

using namespace std;

class CFont {

private:
    string DataPath;
    bool GlobalFontEnabled = true;
    map<string, TTF_Font*> FontCache;
public:
    CFont(string AssetsPath);
    ~CFont();
    int TextWidth(string Font, int FontSize, string Tekst, size_t NrOfChars);
    SDL_Point TextSize(string Font, int FontSize, string Tekst, size_t NrOfChars, int YSpacing);
    void WriteText(SDL_Renderer *Renderer, string Font, int FontSize, string Tekst, size_t NrOfChars, int X, int Y, int YSpacing, SDL_Color ColorIn);
};