#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath>
#include "CImage.h"
#include "Platform.h"
#include "Vec2F.h"
#include "Common.h"
#include <vector>
#include <algorithm>
using namespace std;

CImage::CImage(string AssetsPath, bool ADebugInfo)
{
	ImgEnabled = true;
	DebugInfo = ADebugInfo;
	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		ImgEnabled = false;
		SDL_Log("Failed to initialise images: %s\n", IMG_GetError());
	}
	else
	{
		SDL_Log("Images Succesfully initialised!\n");
	}
	DataPath = AssetsPath;

	for (int i=0; i < GFX_Max; i++)
		Images[i] = nullptr;
}

CImage::~CImage()
{
	UnloadImages();
	IMG_Quit();
}


int CImage::ScaledImagesLoadedCount()
{
	return ScaledImagesLoaded;
}

SDL_Texture *CImage::LoadScaledImage(SDL_Renderer* Renderer, int GFXID, Vec2F Scale)
{
	if(!ImgEnabled)
		return NULL;

	if ((GFXID < 0) || (GFXID >= GFX_Max))
		return NULL;

	if (Images[GFXID] == nullptr)
		return NULL;

	
	int w, h;
	SDL_QueryTexture(Images[GFXID]->Img, NULL, NULL, &w, &h);
	w = (int)ceil(w * abs(Scale.x));
	h = (int)ceil(h * abs(Scale.y));
	Vec2F Resolution = {(float)w,(float)h};
	for(auto it  = Images[GFXID]->ScaledImages.begin(); it < Images[GFXID]->ScaledImages.end(); ++it)
	{
		if ((it->first.x - Resolution.x < epsilion) &&
			(it->first.y - Resolution.y < epsilion))
			return it->second;
	}

	//not loaded yet

	string FullFileName = Images[GFXID]->BaseFilename + "_" + to_string(w)+ "x" + to_string(h) + ".bmp";
	SDL_Texture *Tmp = NULL;
	SDL_Surface* Img = IMG_Load(FullFileName.c_str());
	if(Img)
	{
		Tmp = SDL_CreateTextureFromSurface(Renderer, Img);
		if(Tmp)
		{
			Images[GFXID]->ScaledImages.push_back(make_pair(Resolution, Tmp));
			ScaledImagesLoaded++;
		}
		SDL_FreeSurface(Img);
	}

	return Tmp;
}

int CImage::LoadImage(SDL_Renderer* Renderer, string FileName)
{
	return this->LoadImage(Renderer, FileName, 0, 0, false);
}

void CImage::SaveImage(SDL_Renderer* Renderer, int GFXID, Vec2F scale)
{
	if(!ImgEnabled)
		return;

	if ((GFXID < 0) || (GFXID >= GFX_Max))
		return;

	if (Images[GFXID] == nullptr)
		return;

	if (!Images[GFXID]->cansave)
		return;

	int w, h;
	SDL_QueryTexture(Images[GFXID]->Img, NULL, NULL, &w, &h);
	w = (int)ceil(w * abs(scale.x));
	h = (int)ceil(h * abs(scale.y));
	SDL_SaveBMPTextureScaled(Renderer, Images[GFXID]->BaseFilename +"_" + to_string(w)+ "x" + to_string(h) + ".bmp" ,
		Images[GFXID]->Img, abs(scale.x),abs(scale.y), true, Images[GFXID]->bayerversion, Images[GFXID]->whiteThreshold);
}

int CImage::LoadImage(SDL_Renderer* Renderer, string FileName, int bayerver, int whitethreshold, bool cansave)
{
	if(!ImgEnabled)
		return -1;

	string FullFileName = DataPath + "graphics/" + FileName;
	for (int i=0; i < GFX_Max; i++)
		if(Images[i] == nullptr)
		{
			SDL_Surface* Img = IMG_Load(FullFileName.c_str());
			if(Img)
			{
				Images[i] = new CTexture();
				Images[i]->BaseFilename = FullFileName.substr(0, FullFileName.find_last_of('.'));
				Images[i]->bayerversion = bayerver;
				Images[i]->whiteThreshold = whitethreshold;
				Images[i]->cansave = cansave;
				Images[i]->Img = SDL_CreateTextureFromSurface(Renderer, Img);
				SDL_FreeSurface(Img);
				if(DebugInfo)
					SDL_Log("Loaded Graphic %s\n", FullFileName.c_str());
				return i;
			}
			else
			{
				SDL_Log("Failed Loading Graphic %s\n", FullFileName.c_str());
				return -1;
			}
		}
	return -1;
}

void CImage::UnLoadImage(int GFXID)
{
	if(!ImgEnabled)
		return;

	if ((GFXID < 0) || (GFXID >= GFX_Max))
		return;

	if (Images[GFXID] != nullptr)
	{
		SDL_DestroyTexture(Images[GFXID]->Img);
		
		for(auto it  = Images[GFXID]->ScaledImages.begin(); it < Images[GFXID]->ScaledImages.end(); ++it)
		{
			SDL_DestroyTexture(it->second);
		}
		
		delete Images[GFXID];
		Images[GFXID] = nullptr;
	}


}

void CImage::UnloadImages()
{
	for (int i=0; i < GFX_Max; i++)
		UnLoadImage(i);
}

void CImage::DrawImage(SDL_Renderer* Renderer, int GFXID, SDL_Rect* Src, SDL_Rect* Dst)
{
	if((GFXID < 0) || (GFXID >= GFX_Max))
		return;

	DrawImage(Renderer, Images[GFXID]->Img, Src, Dst);
}

void CImage::DrawImage(SDL_Renderer* Renderer, SDL_Texture *Texture, SDL_Rect* Src, SDL_Rect* Dst)
{
	if(!ImgEnabled)
		return;

	if(Texture == nullptr)
		return;

	SDL_RenderCopy(Renderer, Texture, Src, Dst);
}

void CImage::DrawImageEx(SDL_Renderer* Renderer, int GFXID, SDL_Rect* Src, SDL_Rect* Dst, float Angle, SDL_Point* Center, SDL_RendererFlip Flip)
{
	if((GFXID < 0) || (GFXID >= GFX_Max))
		return;

	DrawImageEx(Renderer, Images[GFXID]->Img, Src, Dst, Angle, Center, Flip);
}

void CImage::DrawImageEx(SDL_Renderer* Renderer, SDL_Texture *Texture, SDL_Rect* Src, SDL_Rect* Dst, float Angle, SDL_Point* Center, SDL_RendererFlip Flip)
{
	if(!ImgEnabled)
		return;

	if(Texture == nullptr)
		return;

	SDL_RenderCopyEx(Renderer, Texture, Src, Dst, Angle, Center, Flip);
}

//fuze used center points for positions and a floating point scale vector
void CImage::DrawImageFuzeSrcRectTintFloat(SDL_Renderer* Renderer, int GFXID, SDL_Rect *SrcRect, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale, float TintR, float TintG, float TintB, float Alpha)
{
	if((GFXID < 0) || (GFXID >= GFX_Max))
		return;

	DrawImageFuze(Renderer, Images[GFXID]->Img, SrcRect, CenterImagePos, Pos, Angle, Scale, (Uint8)floor(255.0f * TintR) , (Uint8)floor(255.0f * TintG), (Uint8)floor(255.0f * TintB) , (Uint8)floor(255.0f * Alpha));
}

void CImage::DrawImageFuzeSrcRectTintFloat(SDL_Renderer* Renderer, SDL_Texture* Texture, SDL_Rect *SrcRect, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale, float TintR, float TintG, float TintB, float Alpha)
{
	if(Texture == nullptr)
		return;

	DrawImageFuze(Renderer, Texture, SrcRect, CenterImagePos, Pos, Angle, Scale, (Uint8)floor(255.0f * TintR) , (Uint8)floor(255.0f * TintG), (Uint8)floor(255.0f * TintB) , (Uint8)floor(255.0f * Alpha));
}

//fuze used center points for positions and a floating point scale vector
void CImage::DrawImageFuzeTintFloat(SDL_Renderer* Renderer, int GFXID, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale, float TintR, float TintG, float TintB, float Alpha)
{
	if((GFXID < 0) || (GFXID >= GFX_Max))
		return;

	DrawImageFuze(Renderer, Images[GFXID]->Img, NULL, CenterImagePos, Pos, Angle, Scale, (Uint8)floor(255.0f * TintR) , (Uint8)floor(255.0f * TintG), (Uint8)floor(255.0f * TintB) , (Uint8)floor(255.0f * Alpha));
}

void CImage::DrawImageFuzeTintFloat(SDL_Renderer* Renderer, SDL_Texture *Texture, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale, float TintR, float TintG, float TintB, float Alpha)
{
	if(Texture == nullptr)
		return;

	DrawImageFuze(Renderer, Texture, NULL, CenterImagePos, Pos, Angle, Scale, (Uint8)floor(255.0f * TintR) , (Uint8)floor(255.0f * TintG), (Uint8)floor(255.0f * TintB) , (Uint8)floor(255.0f * Alpha));
}

void CImage::DrawImageFuze(SDL_Renderer* Renderer, int GFXID, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale, Uint8 TintR, Uint8 TintG, Uint8 TintB, Uint8 Alpha)
{
	if((GFXID < 0) || (GFXID >= GFX_Max))
		return;

	DrawImageFuze(Renderer, Images[GFXID]->Img, NULL, CenterImagePos, Pos, Angle, Scale, TintR, TintG, TintB, Alpha);
}


//fuze used center points for positions and a floating point scale vector
void CImage::DrawImageFuze(SDL_Renderer* Renderer, SDL_Texture *Texture, SDL_Rect *SrcRect, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale, Uint8 TintR, Uint8 TintG, Uint8 TintB, Uint8 Alpha)
{
	if(!ImgEnabled)
		return;

	if(Texture == nullptr)
		return;
	float dstW;
	float dstH;

	if(SrcRect)
	{
		dstW = (SrcRect->w * abs(Scale->x));
		dstH = (SrcRect->h * abs(Scale->y));
	}
	else
	{

		SDL_Point ImageTz = ImageSize(Texture);
		dstW = (ImageTz.x * abs(Scale->x));
		dstH = (ImageTz.y * abs(Scale->y));
	}
	SDL_Rect Dst;
	if(CenterImagePos)
		Dst = {(int)(Pos->x - ((dstW) / 2)), (int)(Pos->y - ((dstH) / 2)), (int)(dstW), (int)(dstH)};
	else
		Dst = {Pos->x, Pos->y, (int)(dstW), (int)(dstH)};

	//save values
	SDL_BlendMode Mode;
	Uint8 R,G,B,A;

	SDL_GetTextureAlphaMod(Texture, &A);
	SDL_GetTextureColorMod(Texture, &R, &G, &B);
	SDL_GetTextureBlendMode(Texture, &Mode);

	//Set our values
	SDL_SetTextureColorMod(Texture, TintR, TintG, TintB);
	SDL_SetTextureAlphaMod(Texture, Alpha);
	SDL_SetTextureBlendMode(Texture, SDL_BLENDMODE_BLEND);

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	if ((Scale->x < 0) && (Scale->y < 0))
		flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
	else
	{
		if (Scale->x < 0)
			flip = SDL_FLIP_HORIZONTAL;
		else
		{
			if (Scale->y < 0)
				flip = SDL_FLIP_VERTICAL;
		}
	}

	//Draw
	SDL_RenderCopyEx(Renderer, Texture, SrcRect, &Dst, Angle, NULL, flip);

	//restore values
	SDL_SetTextureBlendMode(Texture, Mode);
	SDL_SetTextureColorMod(Texture, R, G, B);
	SDL_SetTextureAlphaMod(Texture, A);
}

SDL_Point CImage::ImageSize(int GFXID)
{

	if((GFXID < 0) || (GFXID >= GFX_Max))
		return {-1, -1};

	return ImageSize(Images[GFXID]->Img);
}


SDL_Point CImage::ImageSize(SDL_Texture *Texture)
{

	if(!ImgEnabled)
		return {-1, -1};

	if(Texture == nullptr)
		return {-1, -1};

	SDL_Point size;
	SDL_QueryTexture(Texture, NULL, NULL, &size.x, &size.y);
	return size;
}

int CImage::ImageSlotsUsed()
{
	int c = 0;
	for (int i=0; i < GFX_Max; i++)
	{
		if(Images[i] != nullptr)
			c++;
	}
	return c;
}

int CImage::ImageSlotsMax()
{
	return GFX_Max;
}

SDL_Texture* CImage::GetImage(int GFXID)
{
	if((GFXID < 0) || (GFXID >= GFX_Max))
		return NULL;
	
	if (Images[GFXID] == NULL)
		return nullptr;

	return Images[GFXID]->Img;
}