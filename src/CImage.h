#pragma once

#include <SDL.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Platform.h"
#include "Vec2F.h"


using namespace std;


constexpr int GFX_Max = 100;

class CTexture {
	public:
		SDL_Texture* Img;
		string BaseFilename;
		int whiteThreshold;
		int bayerversion;
		bool cansave;
		vector<pair<Vec2F, SDL_Texture*>> ScaledImages;
};

class CImage {

private:
	CTexture *Images[GFX_Max];
	string DataPath;
	bool ImgEnabled;
	bool DebugInfo;
public:
	CImage(string AssetsPath, bool ADebugInfo);
	~CImage();
	SDL_Texture *LoadScaledImage(SDL_Renderer* Renderer, int GFXID, Vec2F Scale);
	int LoadImage(SDL_Renderer* Renderer, string FileName);
	int LoadImage(SDL_Renderer* Renderer, string FileName, int bayerver, int whitethreshold, bool cansave);
	void SaveImage(SDL_Renderer* Renderer, int GFXID, Vec2F scale);
	void UnLoadImage(int GFXID);
	void UnloadImages();
	void DrawImage(SDL_Renderer* Renderer, int GFXID, SDL_Rect* Src, SDL_Rect* Dst);
	void DrawImage(SDL_Renderer* Renderer, SDL_Texture*, SDL_Rect* Src, SDL_Rect* Dst);
	void DrawImageEx(SDL_Renderer* Renderer, int GFXID, SDL_Rect* Src, SDL_Rect* Dst, double Angle, SDL_Point* Center, SDL_RendererFlip Flip);
	void DrawImageEx(SDL_Renderer* Renderer, SDL_Texture *Texture, SDL_Rect* Src, SDL_Rect* Dst, double Angle, SDL_Point* Center, SDL_RendererFlip Flip);
	//tint & alpha value between 0.0 & 1.0
	void DrawImageFuzeSrcRectTintFloat(SDL_Renderer* Renderer, int GFXID, SDL_Rect *SrcRect, bool CenterImagePos, SDL_Point* Pos, double Angle, Vec2F* Scale,
		float TintR, float TintG, float TintB, float Alpha);
	void DrawImageFuzeSrcRectTintFloat(SDL_Renderer* Renderer, SDL_Texture* Texture, SDL_Rect *SrcRect, bool CenterImagePos, SDL_Point* Pos, double Angle, Vec2F* Scale, 
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
	SDL_Texture* GetImage(int GFXID);

};