#pragma once

#include <SDL.h>
#include <iostream>
#include <string>
#include "Platform.h"
#include "Vec2F.h"


using namespace std;


constexpr int GFX_Max = 100;

struct CTexture {
	public:
		SDL_Texture* Img;
		string BaseFilename;
		int whiteThreshold;
		int bayerversion;
		bool cansave;
};

typedef struct CTexture CTexture;

extern CTexture *CImage_Images[GFX_Max];
extern string CImage_DataPath;
extern bool CImage_ImgEnabled;
extern bool CImage_DebugInfo;
extern int CImage_ScaledImagesLoaded;

void CImage_Init(string AssetsPath, bool ADebugInfo);
void CImage_DeInit();
SDL_Texture *CImage_LoadScaledImage(SDL_Renderer* Renderer, int GFXID, Vec2F Scale);
int CImage_LoadImage(SDL_Renderer* Renderer, string FileName);
int CImage_LoadImage(SDL_Renderer* Renderer, string FileName, int bayerver, int whitethreshold, bool cansave);
void CImage_SaveImage(SDL_Renderer* Renderer, int GFXID, Vec2F scale);
void CImage_UnLoadImage(int GFXID);
void CImage_UnloadImages();
void CImage_DrawImage(SDL_Renderer* Renderer, int GFXID, SDL_Rect* Src, SDL_Rect* Dst);
void CImage_DrawImage(SDL_Renderer* Renderer, SDL_Texture*, SDL_Rect* Src, SDL_Rect* Dst);
void CImage_DrawImageEx(SDL_Renderer* Renderer, int GFXID, SDL_Rect* Src, SDL_Rect* Dst, float Angle, SDL_Point* Center, SDL_RendererFlip Flip);
void CImage_DrawImageEx(SDL_Renderer* Renderer, SDL_Texture *Texture, SDL_Rect* Src, SDL_Rect* Dst, float Angle, SDL_Point* Center, SDL_RendererFlip Flip);
//tint & alpha value between 0.0 & 1.0
void CImage_DrawImageFuzeSrcRectTintFloat(SDL_Renderer* Renderer, int GFXID, SDL_Rect *SrcRect, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale,
	float TintR, float TintG, float TintB, float Alpha);
void CImage_DrawImageFuzeSrcRectTintFloat(SDL_Renderer* Renderer, SDL_Texture* Texture, SDL_Rect *SrcRect, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale, 
	float TintR, float TintG, float TintB, float Alpha);
//tint & alpha value between 0.0 & 1.0
void CImage_DrawImageFuzeTintFloat(SDL_Renderer* Renderer, SDL_Texture *Texture, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale,
	float TintR, float TintG, float TintB, float Alpha);
//tint & alpha value between 0.0 & 1.0
void CImage_DrawImageFuzeTintFloat(SDL_Renderer* Renderer, int GFXID, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale,
	float TintR, float TintG, float TintB, float Alpha);
void CImage_DrawImageFuze(SDL_Renderer* Renderer, int GFXID, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale,
	Uint8 TintR, Uint8 TintG, Uint8 TintB, Uint8 Alpha);
void CImage_DrawImageFuze(SDL_Renderer* Renderer, SDL_Texture *Texture, SDL_Rect *SrcRect, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale,
	Uint8 TintR, Uint8 TintG, Uint8 TintB, Uint8 Alpha);
SDL_Point CImage_ImageSize(int GFXID);
SDL_Point CImage_ImageSize(SDL_Texture *Texture);
int CImage_ImageSlotsUsed();
int CImage_ImageSlotsMax();
int CImage_ScaledImagesLoadedCount();
SDL_Texture* CImage_GetImage(int GFXID);

