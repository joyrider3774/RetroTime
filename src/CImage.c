#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "CImage.h"
#include "Platform.h"
#include "Vec2F.h"
#include "Common.h"


#define GFX_Max 100
#define CImage_ScaledImagesLoadedMaxCount 200

struct ScaledImagesStruct {
	char basefilename[1000];
	Vec2F resolution;
	SDL_Texture* texture;
};

typedef struct ScaledImagesStruct ScaledImagesStruct;

ScaledImagesStruct ScaledImages[CImage_ScaledImagesLoadedMaxCount];

CTexture *CImage_Images[GFX_Max];
char CImage_DataPath[500];
bool CImage_ImgEnabled = false;
bool CImage_DebugInfo = false;
int CImage_ScaledImagesLoaded = 0;

void CImage_Init(char* AssetsPath, bool ADebugInfo)
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
	strcpy(CImage_DataPath, AssetsPath);

	for (int i=0; i < GFX_Max; i++)
		CImage_Images[i] = NULL;
	
	for (int i=0; i < CImage_ScaledImagesLoadedMaxCount; i++)
	{
		ScaledImages[i].basefilename[0] = '\0';
		ScaledImages[i].resolution.x = 0;
		ScaledImages[i].resolution.y = 0;
		ScaledImages[i].texture = NULL;
	}
}

void CImage_DeInit()
{
	CImage_UnloadImages();

	for(int i = 0; i < CImage_ScaledImagesLoaded; i++)
	{
		if (ScaledImages[i].texture != NULL)
			SDL_DestroyTexture(ScaledImages[i].texture);
	}

	IMG_Quit();
}

int CImage_ScaledImagesLoadedMax()
{
	return CImage_ScaledImagesLoadedMaxCount;
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

	if (CImage_Images[GFXID] == NULL)
		return NULL;

	
	int w, h;
	SDL_QueryTexture(CImage_Images[GFXID]->Img, NULL, NULL, &w, &h);
	w = (int)ceil(w * fabs(Scale.x));
	h = (int)ceil(h * fabs(Scale.y));
	Vec2F Resolution = {(float)w,(float)h};

	for(int i = 0; i < CImage_ScaledImagesLoaded; i++)
	{
		if ((strcmp(ScaledImages[i].basefilename, CImage_Images[GFXID]->BaseFilename) ==0) &&
			(ScaledImages[i].resolution.x - Resolution.x < epsilion) &&
			(ScaledImages[i].resolution.y - Resolution.y < epsilion))
			return ScaledImages[i].texture;
	}

	SDL_Texture *Tmp = NULL;
	//not loaded yet
	if (CImage_ScaledImagesLoaded < CImage_ScaledImagesLoadedMaxCount)
	{
		char FullFileName[1000];
		sprintf(FullFileName, "%s_%dx%d.bmp", CImage_Images[GFXID]->BaseFilename, w, h);
		
		SDL_Surface* Img = IMG_Load(FullFileName);
		if(Img)
		{
			Tmp = SDL_CreateTextureFromSurface(Renderer, Img);
			if(Tmp)
			{
				strcpy(ScaledImages[CImage_ScaledImagesLoaded].basefilename, CImage_Images[GFXID]->BaseFilename);
				ScaledImages[CImage_ScaledImagesLoaded].resolution.x = Resolution.x;
				ScaledImages[CImage_ScaledImagesLoaded].resolution.y = Resolution.y;
				ScaledImages[CImage_ScaledImagesLoaded].texture = Tmp;
				CImage_ScaledImagesLoaded++;
			}
			SDL_FreeSurface(Img);
		}
	}

	return Tmp;
}

int CImage_LoadImage(SDL_Renderer* Renderer, const char* FileName)
{
	return CImage_LoadImageEx(Renderer, FileName, 0, 0, false);
}

void CImage_SaveImage(SDL_Renderer* Renderer, int GFXID, Vec2F scale)
{
	if(!CImage_ImgEnabled)
		return;

	if ((GFXID < 0) || (GFXID >= GFX_Max))
		return;

	if (CImage_Images[GFXID] == NULL)
		return;

	if (!CImage_Images[GFXID]->cansave)
		return;

	int w, h;
	SDL_QueryTexture(CImage_Images[GFXID]->Img, NULL, NULL, &w, &h);
	w = (int)ceil(w * fabs(scale.x));
	h = (int)ceil(h * fabs(scale.y));
	char FileName[1000];
	sprintf(FileName,"%s_%dx%d.bmp", CImage_Images[GFXID]->BaseFilename, w,h);
	SDL_SaveBMPTextureScaled(Renderer, FileName,
		CImage_Images[GFXID]->Img, fabs(scale.x),fabs(scale.y), true, CImage_Images[GFXID]->bayerversion, CImage_Images[GFXID]->whiteThreshold);
}

int CImage_LoadImageEx(SDL_Renderer* Renderer, const char* FileName, int bayerver, int whitethreshold, bool cansave)
{
	if(!CImage_ImgEnabled)
		return -1;

	char FullFileName[1000];
	char BaseFileName[1000];
	sprintf(FullFileName, "%sgraphics/%s",CImage_DataPath, FileName);
	sprintf(BaseFileName, "%sgraphics/%s",CImage_DataPath, FileName);
	for (int i=0; i < GFX_Max; i++)
	{
		if(CImage_Images[i] == NULL)
		{
			SDL_Surface* Img = IMG_Load(FullFileName);
			if(Img)
			{
				for (int j = strlen(BaseFileName) -1; j > 0; j--)
					if(BaseFileName[j] == '.')
					{
						BaseFileName[j] = '\0';
						break;
					}
				CImage_Images[i] = (CTexture*) malloc(sizeof(CTexture));
				strcpy(CImage_Images[i]->BaseFilename, BaseFileName);
				CImage_Images[i]->bayerversion = bayerver;
				CImage_Images[i]->whiteThreshold = whitethreshold;
				CImage_Images[i]->cansave = cansave;
				CImage_Images[i]->Img = SDL_CreateTextureFromSurface(Renderer, Img);
				SDL_FreeSurface(Img);
				if(CImage_DebugInfo)
					SDL_Log("Loaded Graphic %s\n", FullFileName);
				return i;
			}
			else
			{
				SDL_Log("Failed Loading Graphic %s\n", FullFileName);
				return -1;
			}
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

	if (CImage_Images[GFXID] != NULL)
	{
		SDL_DestroyTexture(CImage_Images[GFXID]->Img);
		
		free(CImage_Images[GFXID]);
		CImage_Images[GFXID] = NULL;
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

	CImage_DrawImageTex(Renderer, CImage_Images[GFXID]->Img, Src, Dst);
}

void CImage_DrawImageTex(SDL_Renderer* Renderer, SDL_Texture *Texture, SDL_Rect* Src, SDL_Rect* Dst)
{
	if(!CImage_ImgEnabled)
		return;

	if(Texture == NULL)
		return;

	SDL_RenderCopy(Renderer, Texture, Src, Dst);
}

void CImage_DrawImageEx(SDL_Renderer* Renderer, int GFXID, SDL_Rect* Src, SDL_Rect* Dst, float Angle, SDL_Point* Center, SDL_RendererFlip Flip)
{
	if((GFXID < 0) || (GFXID >= GFX_Max))
		return;

	CImage_DrawImageExTex(Renderer, CImage_Images[GFXID]->Img, Src, Dst, Angle, Center, Flip);
}

void CImage_DrawImageExTex(SDL_Renderer* Renderer, SDL_Texture *Texture, SDL_Rect* Src, SDL_Rect* Dst, float Angle, SDL_Point* Center, SDL_RendererFlip Flip)
{
	if(!CImage_ImgEnabled)
		return;

	if(Texture == NULL)
		return;

	SDL_RenderCopyEx(Renderer, Texture, Src, Dst, Angle, Center, Flip);
}

//fuze used center points for positions and a floating point scale vector
void CImage_DrawImageFuzeSrcRectTintFloat(SDL_Renderer* Renderer, int GFXID, SDL_Rect *SrcRect, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale, float TintR, float TintG, float TintB, float Alpha)
{
	if((GFXID < 0) || (GFXID >= GFX_Max))
		return;

	CImage_DrawImageFuzeTex(Renderer, CImage_Images[GFXID]->Img, SrcRect, CenterImagePos, Pos, Angle, Scale, (Uint8)floor(255.0f * TintR) , (Uint8)floor(255.0f * TintG), (Uint8)floor(255.0f * TintB) , (Uint8)floor(255.0f * Alpha));
}

void CImage_DrawImageFuzeSrcRectTintFloatTex(SDL_Renderer* Renderer, SDL_Texture* Texture, SDL_Rect *SrcRect, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale, float TintR, float TintG, float TintB, float Alpha)
{
	if(Texture == NULL)
		return;

	CImage_DrawImageFuzeTex(Renderer, Texture, SrcRect, CenterImagePos, Pos, Angle, Scale, (Uint8)floor(255.0f * TintR) , (Uint8)floor(255.0f * TintG), (Uint8)floor(255.0f * TintB) , (Uint8)floor(255.0f * Alpha));
}

//fuze used center points for positions and a floating point scale vector
void CImage_DrawImageFuzeTintFloat(SDL_Renderer* Renderer, int GFXID, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale, float TintR, float TintG, float TintB, float Alpha)
{
	if((GFXID < 0) || (GFXID >= GFX_Max))
		return;

	CImage_DrawImageFuzeTex(Renderer, CImage_Images[GFXID]->Img, NULL, CenterImagePos, Pos, Angle, Scale, (Uint8)floor(255.0f * TintR) , (Uint8)floor(255.0f * TintG), (Uint8)floor(255.0f * TintB) , (Uint8)floor(255.0f * Alpha));
}

void CImage_DrawImageFuzeTintFloatTex(SDL_Renderer* Renderer, SDL_Texture *Texture, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale, float TintR, float TintG, float TintB, float Alpha)
{
	if(Texture == NULL)
		return;

	CImage_DrawImageFuzeTex(Renderer, Texture, NULL, CenterImagePos, Pos, Angle, Scale, (Uint8)floor(255.0f * TintR) , (Uint8)floor(255.0f * TintG), (Uint8)floor(255.0f * TintB) , (Uint8)floor(255.0f * Alpha));
}

void CImage_DrawImageFuze(SDL_Renderer* Renderer, int GFXID, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale, Uint8 TintR, Uint8 TintG, Uint8 TintB, Uint8 Alpha)
{
	if((GFXID < 0) || (GFXID >= GFX_Max))
		return;

	CImage_DrawImageFuzeTex(Renderer, CImage_Images[GFXID]->Img, NULL, CenterImagePos, Pos, Angle, Scale, TintR, TintG, TintB, Alpha);
}


//fuze used center points for positions and a floating point scale vector
void CImage_DrawImageFuzeTex(SDL_Renderer* Renderer, SDL_Texture *Texture, SDL_Rect *SrcRect, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale, Uint8 TintR, Uint8 TintG, Uint8 TintB, Uint8 Alpha)
{
	if(!CImage_ImgEnabled)
		return;

	if(Texture == NULL)
		return;
	float dstW;
	float dstH;

	if(SrcRect)
	{
		dstW = (SrcRect->w * fabs(Scale->x));
		dstH = (SrcRect->h * fabs(Scale->y));
	}
	else
	{

		SDL_Point ImageTz = CImage_ImageSizeTex(Texture);
		dstW = (ImageTz.x * fabs(Scale->x));
		dstH = (ImageTz.y * fabs(Scale->y));
	}
	SDL_Rect Dst;
	if(CenterImagePos)
	{
		Dst.x = (int)(Pos->x - ((dstW) / 2));
		Dst.y = (int)(Pos->y - ((dstH) / 2));
		Dst.w = (int)(dstW);
		Dst.h = (int)(dstH);
	}
	else
	{
		Dst.x = Pos->x;
		Dst.y = Pos->y;
		Dst.w = (int)(dstW);
		Dst.h = (int)(dstH);
	}

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
	SDL_Point Result = {-1, -1};
	if((GFXID < 0) || (GFXID >= GFX_Max))
		return Result;

	return CImage_ImageSizeTex(CImage_Images[GFXID]->Img);
}


SDL_Point CImage_ImageSizeTex(SDL_Texture *Texture)
{
	SDL_Point Result = {-1, -1};
	if(!CImage_ImgEnabled)
		return Result;

	if(Texture == NULL)
		return Result;

	SDL_Point size;
	SDL_QueryTexture(Texture, NULL, NULL, &size.x, &size.y);
	return size;
}

int CImage_ImageSlotsUsed()
{
	int c = 0;
	for (int i=0; i < GFX_Max; i++)
	{
		if(CImage_Images[i] != NULL)
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
		return NULL;

	return CImage_Images[GFXID]->Img;
}