#pragma once

#include <SDL.h>
#include <iostream>
#include <string>
#include "Platform.h"
#include "Vec2F.h"

constexpr int GFX_Max = 100;

class CImage {

private:
	SDL_Texture *Images[GFX_Max];
	std::string DataPath;
	bool ImgEnabled;
	bool DebugInfo;

public:
	CImage(std::string AssetsPath, bool ADebugInfo);
	~CImage();
	int LoadImg(SDL_Renderer* Renderer, std::string FileName);
	void UnLoadImg(int GFXID);
	void UnloadImages();
	void DrawImage(SDL_Renderer* Renderer, int GFXID, SDL_Rect* Src, SDL_Rect* Dst);
	void DrawImage(SDL_Renderer* Renderer, SDL_Texture*, SDL_Rect* Src, SDL_Rect* Dst);
	void DrawImageEx(SDL_Renderer* Renderer, int GFXID, SDL_Rect* Src, SDL_Rect* Dst, double Angle, SDL_Point* Center, SDL_RendererFlip Flip);
	void DrawImageEx(SDL_Renderer* Renderer, SDL_Texture *Texture, SDL_Rect* Src, SDL_Rect* Dst, double Angle, SDL_Point* Center, SDL_RendererFlip Flip);
	//tint & alpha value between 0.0 & 1.0
	void DrawImageFuzeSrcRectTintFloat(SDL_Renderer* Renderer, int GFXID, SDL_Rect *SrcRect, bool CenterImagePos, SDL_Point* Pos, double Angle, Vec2F* Scale,
		float TintR, float TintG, float TintB, float Alpha);
	//tint & alpha value between 0.0 & 1.0
	void DrawImageFuzeTintFloat(SDL_Renderer* Renderer, SDL_Texture *Texture, bool CenterImagePos, SDL_Point* Pos, double Angle, Vec2F* Scale,
		float TintR, float TintG, float TintB, float Alpha);
	//tint & alpha value between 0.0 & 1.0
	void DrawImageFuzeTintFloat(SDL_Renderer* Renderer, int GFXID, bool CenterImagePos, SDL_Point* Pos, double Angle, Vec2F* Scale,
		float TintR, float TintG, float TintB, float Alpha);
	void DrawImageFuze(SDL_Renderer* Renderer, int GFXID, bool CenterImagePos, SDL_Point* Pos, double Angle, Vec2F* Scale,
		Uint8 TintR, Uint8 TintG, Uint8 TintB, Uint8 Alpha);
	void DrawImageFuze(SDL_Renderer* Renderer, SDL_Texture *Texture, SDL_Rect *SrcRect, bool CenterImagePos, SDL_Point* Pos, double Angle, Vec2F* Scale,
		Uint8 TintR, Uint8 TintG, Uint8 TintB, Uint8 Alpha);
	SDL_Point ImageSize(int GFXID);
	SDL_Point ImageSize(SDL_Texture *Texture);
	int ImageSlotsUsed();
	int ImageSlotsMax();

};