#include <SDL.h>
#include <SDL_ttf.h>
#include <limits.h>
#include <map>
#include <iostream>
#include <string>
#include "CFont.h"
#include "Platform.h"

using namespace std;

CFont::CFont(string AssetsPath)
{
    DataPath = AssetsPath;
    GlobalFontEnabled = (TTF_Init() == 0);
    if (GlobalFontEnabled)
        SDL_Log("Succesfully initialized TTF\n");
    else
        SDL_Log("Failed initializing SDL_ttf %s\n", SDL_GetError());
}

CFont::~CFont()
{
    if(GlobalFontEnabled)
    {
        map<std::string, TTF_Font*>::iterator i;
        for (i = FontCache.begin(); i != FontCache.end(); i++)
            TTF_CloseFont(i->second); 
        TTF_Quit();
    }
}

void CFont::WriteText(SDL_Renderer *Renderer, string Font, int FontSize, string Tekst, size_t NrOfChars, int X, int Y, int YSpacing, SDL_Color ColorIn) 
{
    if(!GlobalFontEnabled || (NrOfChars == 0))
        return;
    TTF_Font *FontIn;
    map<std::string, TTF_Font*>::iterator i;
    string FontNameSize = string(Font) + to_string(FontSize);
    i = FontCache.find(FontNameSize);
    if (i != FontCache.end())
        FontIn = i->second;
    else
    { 
        string Filename = DataPath + "fonts/" + Font + ".ttf";
        FontIn = TTF_OpenFont(Filename.c_str(), FontSize);
        if (!FontIn)
        {
            SDL_Log("Failed loading fonts: %s\n", SDL_GetError());
            return;
        } 
            
        TTF_SetFontStyle(FontIn, TTF_STYLE_NORMAL);

        FontCache[FontNameSize] = FontIn;
    }

    char List[100][255];
    size_t Lines, Teller, Chars;
    SDL_Rect DstRect;
    SDL_Surface *TextureBuffer1Surface1;
    SDL_Texture *TextureBuffer1Texture;
    Lines = 0;
    Chars = 0;
    for (Teller = 0; Teller < NrOfChars; Teller++) 
    {
        if (Lines > 100)
            break;
        if ((Tekst[Teller] == '\n') || (Chars == 255)) 
        {
            List[Lines][Chars] = '\0';
            Lines++;
            Chars = 0;
        } 
        else 
        {
            List[Lines][Chars] = Tekst[Teller];
            Chars++;
        }
    }
    List[Lines][Chars] = '\0';
    for (Teller = 0; Teller <= Lines; Teller++) 
    {
        if (strlen(List[Teller]) > 0) 
        {
            TextureBuffer1Surface1 = TTF_RenderText_Blended(FontIn, List[Teller], ColorIn);
            if (TextureBuffer1Surface1) 
            {
                TextureBuffer1Texture = SDL_CreateTextureFromSurface(Renderer, TextureBuffer1Surface1);
                DstRect.x = X;
                DstRect.y = Y + (Teller)*TTF_FontLineSkip(FontIn) + (Teller * YSpacing);
                DstRect.w = TextureBuffer1Surface1->w;
                DstRect.h = TextureBuffer1Surface1->h;

                SDL_RenderCopy(Renderer, TextureBuffer1Texture, NULL, &DstRect);
                SDL_DestroyTexture(TextureBuffer1Texture);
                SDL_FreeSurface(TextureBuffer1Surface1);
            }
        }
    }
}