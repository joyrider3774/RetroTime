#pragma once

#include <SDL.h>
#include <limits.h>
#include <iostream>
#include <string>
#include "Platform.h"

using namespace std;

constexpr int GFX_Max = 100;

class CImage {

private:
    SDL_Texture *Images[GFX_Max];
    string DataPath;
    bool ImgEnabled;

public:
    CImage(string AssetsPath);
    ~CImage();
    int LoadImage(SDL_Renderer* Renderer, string FileName);
    void UnLoadImage(int GFXID);
    void UnloadImages();
    void DrawImage(SDL_Renderer* Renderer, int GFXID, SDL_Rect* Src, SDL_Rect* Dst);
    void DrawImage(SDL_Renderer* Renderer, SDL_Texture*, SDL_Rect* Src, SDL_Rect* Dst);
    void DrawImageEx(SDL_Renderer* Renderer, int GFXID, SDL_Rect* Src, SDL_Rect* Dst, double Angle, SDL_Point* Center, SDL_RendererFlip Flip);
    void DrawImageEx(SDL_Renderer* Renderer, SDL_Texture *Texture, SDL_Rect* Src, SDL_Rect* Dst, double Angle, SDL_Point* Center, SDL_RendererFlip Flip);
    void DrawImageFuze(SDL_Renderer* Renderer, int GFXID, bool CenterImagePos, SDL_Point* Pos, double Angle, SDL_FPoint* Scale, 
        Uint8 TintR, Uint8 TintG, Uint8 TintB, Uint8 Alpha);
    void  DrawImageFuze(SDL_Renderer* Renderer, SDL_Texture *Texture, bool CenterImagePos, SDL_Point* Pos, double Angle, SDL_FPoint* Scale, 
        Uint8 TintR, Uint8 TintG, Uint8 TintB, Uint8 Alpha);
    SDL_Point ImageSize(int GFXID);
    SDL_Point ImageSize(SDL_Texture *Texture);
    int ImageSlotsUsed();
    int ImageSlotsMax();

};