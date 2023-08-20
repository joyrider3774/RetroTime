#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string.h>
#include <iostream>
#include <string>
#include "Platform.h"

using namespace std;

void CFont_Init(string AssetsPath, bool DebugInfo);
void CFont_DeInit();
int CFont_TextWidth(string Font, int FontSize, string Tekst, size_t NrOfChars);
SDL_Point CFont_TextSize(string Font, int FontSize, string Tekst, size_t NrOfChars, int YSpacing);
void CFont_WriteText(SDL_Renderer *Renderer, string Font, int FontSize, string Tekst, size_t NrOfChars, int X, int Y, int YSpacing, SDL_Color ColorIn);
