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

vector<pair<string, pair<Vec2F, SDL_Texture*>>> ScaledImages;

CTexture *CImage_Images[GFX_Max];
string CImage_DataPath;
bool CImage_ImgEnabled;
bool CImage_DebugInfo;
int CImage_ScaledImagesLoaded;

void CImage_Init(string AssetsPath, bool ADebugInfo)
{
	CImage_ImgEnabled = true;
	CImage_DebugInfo = ADebugInfo;
	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		CImage_ImgEnabled = false;
		SDL_Log("Failed to initialise images: %s\n", IMG_GetError());
	}
	else
	{
		SDL_Log("Images Succesfully initialised!\n");
	}
	CImage_DataPath = AssetsPath;

	for (int i=0; i < GFX_Max; i++)
		CImage_Images[i] = nullptr;
}

void CImage_DeInit()
{
	CImage_UnloadImages();

	for(auto it  = ScaledImages.begin(); it < ScaledImages.end(); ++it)
	{
		SDL_DestroyTexture(it->second.second);
	}


	IMG_Quit();
}


int CImage_ScaledImagesLoadedCount()
{
	return CImage_ScaledImagesLoaded;
}

SDL_Texture *CImage_LoadScaledImage(SDL_Renderer* Renderer, int GFXID, Vec2F Scale)
{
	if(!CImage_ImgEnabled)
		return NULL;

	if ((GFXID < 0) || (GFXID >= GFX_Max))
		return NULL;

	if (CImage_Images[GFXID] == nullptr)
		return NULL;

	
	int w, h;
	SDL_QueryTexture(CImage_Images[GFXID]->Img, NULL, NULL, &w, &h);
	w = (int)ceil(w * abs(Scale.x));
	h = (int)ceil(h * abs(Scale.y));
	Vec2F Resolution = {(float)w,(float)h};
	for(auto it  = ScaledImages.begin(); it < ScaledImages.end(); ++it)
	{
		if ((it->first == CImage_Images[GFXID]->BaseFilename) && 
		 	(it->second.first.x - Resolution.x < epsilion) &&
			(it->second.first.y - Resolution.y < epsilion))
			return it->second.second;
	}

	//not loaded yet

	string FullFileName = CImage_Images[GFXID]->BaseFilename + "_" + to_string(w)+ "x" + to_string(h) + ".bmp";
	SDL_Texture *Tmp = NULL;
	SDL_Surface* Img = IMG_Load(FullFileName.c_str());
	if(Img)
	{
		Tmp = SDL_CreateTextureFromSurface(Renderer, Img);
		if(Tmp)
		{
			ScaledImages.push_back(make_pair(CImage_Images[GFXID]->BaseFilename, make_pair(Resolution, Tmp)));
			CImage_ScaledImagesLoaded++;
		}
		SDL_FreeSurface(Img);
	}

	return Tmp;
}

int CImage_LoadImage(SDL_Renderer* Renderer, string FileName)
{
	return CImage_LoadImage(Renderer, FileName, 0, 0, false);
}

void CImage_SaveImage(SDL_Renderer* Renderer, int GFXID, Vec2F scale)
{
	if(!CImage_ImgEnabled)
		return;

	if ((GFXID < 0) || (GFXID >= GFX_Max))
		return;

	if (CImage_Images[GFXID] == nullptr)
		return;

	if (!CImage_Images[GFXID]->cansave)
		return;

	int w, h;
	SDL_QueryTexture(CImage_Images[GFXID]->Img, NULL, NULL, &w, &h);
	w = (int)ceil(w * abs(scale.x));
	h = (int)ceil(h * abs(scale.y));
	SDL_SaveBMPTextureScaled(Renderer, CImage_Images[GFXID]->BaseFilename +"_" + to_string(w)+ "x" + to_string(h) + ".bmp" ,
		CImage_Images[GFXID]->Img, abs(scale.x),abs(scale.y), true, CImage_Images[GFXID]->bayerversion, CImage_Images[GFXID]->whiteThreshold);
}

int CImage_LoadImage(SDL_Renderer* Renderer, string FileName, int bayerver, int whitethreshold, bool cansave)
{
	if(!CImage_ImgEnabled)
		return -1;

	string FullFileName = CImage_DataPath + "graphics/" + FileName;
	for (int i=0; i < GFX_Max; i++)
		if(CImage_Images[i] == nullptr)
		{
			SDL_Surface* Img = IMG_Load(FullFileName.c_str());
			if(Img)
			{
				CImage_Images[i] = new CTexture();
				CImage_Images[i]->BaseFilename = FullFileName.substr(0, FullFileName.find_last_of('.'));
				CImage_Images[i]->bayerversion = bayerver;
				CImage_Images[i]->whiteThreshold = whitethreshold;
				CImage_Images[i]->cansave = cansave;
				CImage_Images[i]->Img = SDL_CreateTextureFromSurface(Renderer, Img);
				SDL_FreeSurface(Img);
				if(CImage_DebugInfo)
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

void CImage_UnLoadImage(int GFXID)
{
	if(!CImage_ImgEnabled)
		return;

	if ((GFXID < 0) || (GFXID >= GFX_Max))
		return;

	if (CImage_Images[GFXID] != nullptr)
	{
		SDL_DestroyTexture(CImage_Images[GFXID]->Img);
		
		delete CImage_Images[GFXID];
		CImage_Images[GFXID] = nullptr;
	}


}

void CImage_UnloadImages()
{
	for (int i=0; i < GFX_Max; i++)
		CImage_UnLoadImage(i);
}

void CImage_DrawImage(SDL_Renderer* Renderer, int GFXID, SDL_Rect* Src, SDL_Rect* Dst)
{
	if((GFXID < 0) || (GFXID >= GFX_Max))
		return;

	CImage_DrawImage(Renderer, CImage_Images[GFXID]->Img, Src, Dst);
}

void CImage_DrawImage(SDL_Renderer* Renderer, SDL_Texture *Texture, SDL_Rect* Src, SDL_Rect* Dst)
{
	if(!CImage_ImgEnabled)
		return;

	if(Texture == nullptr)
		return;

	SDL_RenderCopy(Renderer, Texture, Src, Dst);
}

void CImage_DrawImageEx(SDL_Renderer* Renderer, int GFXID, SDL_Rect* Src, SDL_Rect* Dst, float Angle, SDL_Point* Center, SDL_RendererFlip Flip)
{
	if((GFXID < 0) || (GFXID >= GFX_Max))
		return;

	CImage_DrawImageEx(Renderer, CImage_Images[GFXID]->Img, Src, Dst, Angle, Center, Flip);
}

void CImage_DrawImageEx(SDL_Renderer* Renderer, SDL_Texture *Texture, SDL_Rect* Src, SDL_Rect* Dst, float Angle, SDL_Point* Center, SDL_RendererFlip Flip)
{
	if(!CImage_ImgEnabled)
		return;

	if(Texture == nullptr)
		return;

	SDL_RenderCopyEx(Renderer, Texture, Src, Dst, Angle, Center, Flip);
}

//fuze used center points for positions and a floating point scale vector
void CImage_DrawImageFuzeSrcRectTintFloat(SDL_Renderer* Renderer, int GFXID, SDL_Rect *SrcRect, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale, float TintR, float TintG, float TintB, float Alpha)
{
	if((GFXID < 0) || (GFXID >= GFX_Max))
		return;

	CImage_DrawImageFuze(Renderer, CImage_Images[GFXID]->Img, SrcRect, CenterImagePos, Pos, Angle, Scale, (Uint8)floor(255.0f * TintR) , (Uint8)floor(255.0f * TintG), (Uint8)floor(255.0f * TintB) , (Uint8)floor(255.0f * Alpha));
}

void CImage_DrawImageFuzeSrcRectTintFloat(SDL_Renderer* Renderer, SDL_Texture* Texture, SDL_Rect *SrcRect, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale, float TintR, float TintG, float TintB, float Alpha)
{
	if(Texture == nullptr)
		return;

	CImage_DrawImageFuze(Renderer, Texture, SrcRect, CenterImagePos, Pos, Angle, Scale, (Uint8)floor(255.0f * TintR) , (Uint8)floor(255.0f * TintG), (Uint8)floor(255.0f * TintB) , (Uint8)floor(255.0f * Alpha));
}

//fuze used center points for positions and a floating point scale vector
void CImage_DrawImageFuzeTintFloat(SDL_Renderer* Renderer, int GFXID, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale, float TintR, float TintG, float TintB, float Alpha)
{
	if((GFXID < 0) || (GFXID >= GFX_Max))
		return;

	CImage_DrawImageFuze(Renderer, CImage_Images[GFXID]->Img, NULL, CenterImagePos, Pos, Angle, Scale, (Uint8)floor(255.0f * TintR) , (Uint8)floor(255.0f * TintG), (Uint8)floor(255.0f * TintB) , (Uint8)floor(255.0f * Alpha));
}

void CImage_DrawImageFuzeTintFloat(SDL_Renderer* Renderer, SDL_Texture *Texture, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale, float TintR, float TintG, float TintB, float Alpha)
{
	if(Texture == nullptr)
		return;

	CImage_DrawImageFuze(Renderer, Texture, NULL, CenterImagePos, Pos, Angle, Scale, (Uint8)floor(255.0f * TintR) , (Uint8)floor(255.0f * TintG), (Uint8)floor(255.0f * TintB) , (Uint8)floor(255.0f * Alpha));
}

void CImage_DrawImageFuze(SDL_Renderer* Renderer, int GFXID, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale, Uint8 TintR, Uint8 TintG, Uint8 TintB, Uint8 Alpha)
{
	if((GFXID < 0) || (GFXID >= GFX_Max))
		return;

	CImage_DrawImageFuze(Renderer, CImage_Images[GFXID]->Img, NULL, CenterImagePos, Pos, Angle, Scale, TintR, TintG, TintB, Alpha);
}


//fuze used center points for positions and a floating point scale vector
void CImage_DrawImageFuze(SDL_Renderer* Renderer, SDL_Texture *Texture, SDL_Rect *SrcRect, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale, Uint8 TintR, Uint8 TintG, Uint8 TintB, Uint8 Alpha)
{
	if(!CImage_ImgEnabled)
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

		SDL_Point ImageTz = CImage_ImageSize(Texture);
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

SDL_Point CImage_ImageSize(int GFXID)
{

	if((GFXID < 0) || (GFXID >= GFX_Max))
		return {-1, -1};

	return CImage_ImageSize(CImage_Images[GFXID]->Img);
}


SDL_Point CImage_ImageSize(SDL_Texture *Texture)
{

	if(!CImage_ImgEnabled)
		return {-1, -1};

	if(Texture == nullptr)
		return {-1, -1};

	SDL_Point size;
	SDL_QueryTexture(Texture, NULL, NULL, &size.x, &size.y);
	return size;
}

int CImage_ImageSlotsUsed()
{
	int c = 0;
	for (int i=0; i < GFX_Max; i++)
	{
		if(CImage_Images[i] != nullptr)
			c++;
	}
	return c;
}

int CImage_ImageSlotsMax()
{
	return GFX_Max;
}

SDL_Texture* CImage_GetImage(int GFXID)
{
	if((GFXID < 0) || (GFXID >= GFX_Max))
		return NULL;
	
	if (CImage_Images[GFXID] == NULL)
		return nullptr;

	return CImage_Images[GFXID]->Img;
}