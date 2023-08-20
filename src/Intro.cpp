#include <SDL.h>
#include <string.h>
#include "CGame.h"
#include "Common.h"
#include "Intro.h"
#include "CTween.h"
#include "Vec2F.h"

float alpha = 0.0f;
int maxticks = 240;
float alphastep = 1.0f/240.0f;
float archalpha = 0;
int scene = 0;
int sceneticks = 0;
int tsize = 70;


void Intro()
{
	if(GameState == GSIntroInit)
	{
		GameState -= initDiff;
	}

	SDL_Color ClWhite = {255, 255, 255, 255};
	string s;
	int w,h;

	SDL_SetRenderTarget(Renderer, TexTmp);
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	SDL_RenderClear(Renderer);
	if (scene == 0)
	{
		s = "Willems Davy aka Joyrider3774";
		w = CFont_TextWidth("Roboto-Regular", tsize, s, s.length());
		h = tsize*2;
		CFont_WriteText(Renderer, "Roboto-Regular", tsize, s, s.length(), (ScreenWidth - w) / 2, (ScreenHeight - h) /2, tsize, ClWhite);

		s = "presents";
		w = CFont_TextWidth("Roboto-Regular", tsize, s, s.length());
		h = tsize;
		CFont_WriteText(Renderer, "Roboto-Regular", tsize, s, s.length(), (ScreenWidth - w) / 2, (ScreenHeight) /2, tsize, ClWhite);
	}

	if (scene == 1)
	{
		s = "Retro Time";
		w = CFont_TextWidth("Roboto-Regular", tsize, s, s.length());
		CFont_WriteText(Renderer, "Roboto-Regular", tsize, s, s.length(), (ScreenWidth - w) / 2, 40, tsize*2, ClWhite);

		s = "A game containing";
		w = CFont_TextWidth("Roboto-Regular", tsize, s, s.length());
		CFont_WriteText(Renderer, "Roboto-Regular", tsize, s, s.length(),(ScreenWidth - w) / 2, (ScreenHeight - 3 * (tsize + 10)) /2, tsize, ClWhite);

		s = to_string(Games) + " retro based games";
		w = CFont_TextWidth("Roboto-Regular", tsize, s, s.length());
		CFont_WriteText(Renderer, "Roboto-Regular", tsize, s, s.length(), (ScreenWidth - w) / 2, (ScreenHeight - 1 * (tsize + 10)) / 2, tsize, ClWhite);

		s = "playable in " + to_string(Modes) + " game modes";
		w = CFont_TextWidth("Roboto-Regular", tsize, s, s.length());
		CFont_WriteText(Renderer, "Roboto-Regular", tsize, s, s.length(), (ScreenWidth - w) / 2, (ScreenHeight + 1 * (tsize + 10)) /2, tsize, ClWhite);
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
	CImage_DrawImageFuzeTintFloat(Renderer, TexTmp, false, &Pos, 0, &Scale, 1, 1, 1, archalpha);
	sceneticks += 1;

	if (sceneticks >= maxticks)
	{
		alpha = 0;
		scene += 1;
		sceneticks = 0;
	}

	if (Input->Buttons.ButA || Input->Buttons.ButB || Input->Buttons.ButBack || Input->Buttons.ButStart || (scene -2 >= Games))
		GameState = GSTitleScreenInit;

	if (alpha < 1)
		alpha += alphastep;
	else
		alpha = 1;

	archalpha = arch(smoothstep3(alpha));
}