#ifndef CFONT_H
#define CFONT_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <string.h>
#include "Platform.h"



void CFont_Init(char* AssetsPath, bool DebugInfo);
void CFont_DeInit();
int CFont_TextWidth(char* Font, int FontSize, char* Tekst, size_t NrOfChars);
SDL_Point CFont_TextSize(char* Font, int FontSize, char* Tekst, size_t NrOfChars, int YSpacing);
void CFont_WriteText(SDL_Renderer *Renderer, char* Font, int FontSize, char* Tekst, size_t NrOfChars, int X, int Y, int YSpacing, SDL_Color ColorIn);

#endif