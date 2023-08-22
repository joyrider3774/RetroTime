#include <SDL.h>
#include <string.h>
#include "CGame.h"
#include "Common.h"
#include "Intro.h"
#include "CTween.h"
#include "Vec2F.h"

int maxticks = 240;
int scene = 0;
int sceneticks = 0;
int tsize = 70*yscale;


void Intro()
{
	if(GameState == GSIntroInit)
	{
		GameState -= initDiff;
	}

	SDL_Color ClWhite = {255, 255, 255, 255};
	char s[100];
	int w,h;

	SDL_SetRenderTarget(Renderer, TexTmp);
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	SDL_RenderClear(Renderer);
	if (scene == 0)
	{
		strcpy(s,"Willems Davy aka Joyrider3774");
		w = CFont_TextWidth("Roboto-Regular", tsize, s, strlen(s));
		h = tsize*2;
		CFont_WriteText(Renderer, "Roboto-Regular", tsize, s, strlen(s), (ScreenWidth - w) / 2, (ScreenHeight - h) /2, tsize, ClWhite);

		strcpy(s,"presents");
		w = CFont_TextWidth("Roboto-Regular", tsize, s, strlen(s));
		h = tsize;
		CFont_WriteText(Renderer, "Roboto-Regular", tsize, s, strlen(s), (ScreenWidth - w) / 2, (ScreenHeight) /2, tsize, ClWhite);
	}

	if (scene == 1)
	{
		strcpy(s,"Retro Time");
		w = CFont_TextWidth("Roboto-Regular", tsize, s, strlen(s));
		CFont_WriteText(Renderer, "Roboto-Regular", tsize, s, strlen(s), (ScreenWidth - w) / 2, 40*yscale, tsize*2, ClWhite);

		strcpy(s,"A game containing");
		w = CFont_TextWidth("Roboto-Regular", tsize, s, strlen(s));
		CFont_WriteText(Renderer, "Roboto-Regular", tsize, s, strlen(s),(ScreenWidth - w) / 2, ((ScreenHeight - 3 * (tsize + 10*yscale)) /2), tsize, ClWhite);

		sprintf(s,"%d retro based games", Games);
		w = CFont_TextWidth("Roboto-Regular", tsize, s, strlen(s));
		CFont_WriteText(Renderer, "Roboto-Regular", tsize, s, strlen(s), (ScreenWidth - w) / 2, (ScreenHeight - 1 * (tsize + 10*yscale)) / 2, tsize, ClWhite);

		sprintf(s, "playable in %d game modes", Modes);
		w = CFont_TextWidth("Roboto-Regular", tsize, s, strlen(s));
		CFont_WriteText(Renderer, "Roboto-Regular", tsize, s, strlen(s), (ScreenWidth - w) / 2, (ScreenHeight + 1 * (tsize + 10*yscale)) /2, tsize, ClWhite);
	}

	if (scene >= 2)
	{
		GameState = GSTitleScreenInit;
	}

	SDL_SetRenderTarget(Renderer, TexOffScreen);
	//SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	SDL_RenderClear(Renderer);
	SDL_Point Pos = {0,0};
	Vec2F Scale = {1,1};
	CImage_DrawImageFuzeTintFloatTex(Renderer, TexTmp, false, &Pos, 0, &Scale, 1, 1, 1, 1);
	sceneticks += 1;

	if (sceneticks >= maxticks)
	{
		scene += 1;
		sceneticks = 0;
	}

	if (CInput_Buttons.ButA || CInput_Buttons.ButB || CInput_Buttons.ButBack || CInput_Buttons.ButStart || (scene -2 >= Games))
		GameState = GSTitleScreenInit;
}