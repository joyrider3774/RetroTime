#include <SDL.h>
#include <SDL_ttf.h>
#include <string.h>
#include <stdbool.h>
#include "CFont.h"
#include "Platform.h"



#define FontCacheMax 50
char CFont_DataPath[500];
bool CFont_DebugInfo = false;
bool CFont_GlobalFontEnabled = true;

int fontCacheItems = 0;

struct FontCacheItem {
	TTF_Font* Font;
	char Filename[1000];
};

typedef struct FontCacheItem FontCacheItem;

FontCacheItem CFont_FontCache[FontCacheMax];

void CFont_Init(char* AssetsPath, bool DebugInfo)
{
	strcpy(CFont_DataPath, AssetsPath);
	CFont_DebugInfo = DebugInfo;
	CFont_GlobalFontEnabled = (TTF_Init() == 0);
	if (CFont_GlobalFontEnabled)
		SDL_Log("Succesfully initialized TTF\n");
	else
		SDL_Log("Failed initializing SDL_ttf %s\n", SDL_GetError());

	for (int i = 0; i < FontCacheMax; i++)
	{
		CFont_FontCache[i].Font = NULL;
		CFont_FontCache[i].Filename[0] = '\0';
	}

}

void CFont_DeInit()
{
	if(CFont_GlobalFontEnabled)
	{
		for (int i = 0; i < fontCacheItems; i++)
			TTF_CloseFont(CFont_FontCache[i].Font);
		TTF_Quit();
	}
}

int CFont_TextWidth(char* Font, int FontSize, char* Tekst, size_t NrOfChars)
{
	SDL_Point Tmp = CFont_TextSize(Font, FontSize, Tekst, NrOfChars, 0);
	return Tmp.x;
}

SDL_Point CFont_TextSize(char* Font, int FontSize, char* Tekst, size_t NrOfChars, int YSpacing)
{
	SDL_Point Result = {0,0};
	if(!CFont_GlobalFontEnabled || (NrOfChars == 0))
		return Result;

	TTF_Font *FontIn;
	char FontNameSize[1000];
	sprintf(FontNameSize, "%s_%d", Font, FontSize);
	
	bool bfound = false;
	for (int i = 0; i < fontCacheItems; i++)
		if(strcmp(CFont_FontCache[i].Filename, FontNameSize) == 0)
		{
			FontIn = CFont_FontCache[i].Font; 
			bfound = true;
			break;
		}

	if (fontCacheItems < FontCacheMax)
	{
		if(!bfound)
		{
			char Filename[1000];
			sprintf(Filename, "%sfonts/%s.ttf", CFont_DataPath, Font);
			FontIn = TTF_OpenFont(Filename, FontSize);
			if (!FontIn)
			{
				SDL_Log("Failed Loading Font %s %d\n", SDL_GetError(), FontSize);
				return Result;
			}
			if(CFont_DebugInfo)
				SDL_Log("Loaded Font %s %d\n", Filename, FontSize);
			TTF_SetFontStyle(FontIn, TTF_STYLE_NORMAL);

			CFont_FontCache[fontCacheItems].Font = FontIn;
			strcpy(CFont_FontCache[fontCacheItems].Filename, FontNameSize);
			fontCacheItems++;
		}
	}

	char List[100][255];
	size_t Lines, Teller, Chars;
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
	int w,h;
	Result.y = (Lines)*TTF_FontLineSkip(FontIn) + (Lines * YSpacing);
	for (Teller = 0; Teller <= Lines; Teller++)
	{
		TTF_SizeText(FontIn, List[Teller], &w, &h);
		if (w > Result.x)
			Result.x = w;
	}
	return Result;
}

void CFont_WriteText(SDL_Renderer *Renderer, char* Font, int FontSize, char* Tekst, size_t NrOfChars, int X, int Y, int YSpacing, SDL_Color ColorIn)
{
	if(!CFont_GlobalFontEnabled || (NrOfChars == 0))
		return;
	TTF_Font *FontIn;
	char FontNameSize[1000];
	sprintf(FontNameSize,"%s_%d", Font, FontSize);
	bool bfound = false;
	for (int i = 0; i < fontCacheItems; i++)
		if(strcmp(CFont_FontCache[i].Filename, FontNameSize) == 0)
		{
			FontIn = CFont_FontCache[i].Font; 
			bfound = true;
			break;
		}
	if (fontCacheItems < FontCacheMax)
	{
		if(!bfound)
		{
			char Filename[1000];
			sprintf(Filename,"%sfonts/%s.ttf", CFont_DataPath, Font);
			FontIn = TTF_OpenFont(Filename, FontSize);
			if (!FontIn)
			{
				SDL_Log("Failed Loading Font %s %d\n", Filename, FontSize);
				return;
			}
			if(CFont_DebugInfo)
				SDL_Log("Loaded Font %s %d\n", Filename, FontSize);
			TTF_SetFontStyle(FontIn, TTF_STYLE_NORMAL);

			CFont_FontCache[fontCacheItems].Font = FontIn;
			strcpy(CFont_FontCache[fontCacheItems].Filename, FontNameSize);
			fontCacheItems++;
		}
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