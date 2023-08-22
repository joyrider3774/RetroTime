#include <SDL.h>
#include <string.h>
#include <stdbool.h>
#include "CGame.h"
#include "games/CGameBase.h"
#include "Common.h"
#include "SubScoreScreen.h"
#include "Vec2F.h"
#include "games/CGameBlockStacker.h"
#include "games/CGameFastEddy.h"
#include "games/CGameFrog.h"
#include "games/CGameBreakOut.h"
#include "games/CGameInvaders.h"
#include "games/CGamePang.h"
#include "games/CGameRamIt.h"
#include "games/CGameSnake.h"

bool wasnewhighscorecarousel;
bool wasnewhighscoregame;
int prevretrocarouselscore;

void InitSubScoreScreen()
{
	SubGameState = SGFrame;
	SubStateCounter = 0;
	wasnewhighscorecarousel = false;
	wasnewhighscoregame = false;
	prevretrocarouselscore = RetroCarouselScore;
	if (GameMode == GMRetroCarousel)
		RetroCarouselScore += Scores[Game][GameMode];

	if (RetroCarouselScore > RetroCarouselHighScore)
	{
		RetroCarouselHighScore = RetroCarouselScore;
		wasnewhighscorecarousel = true;
	}

	if (Scores[Game][GameMode] > HighScores[Game][GameMode])
	{
		HighScores[Game][GameMode] = Scores[Game][GameMode];
		wasnewhighscoregame = true;
	}

	//savehighscoresoptions()
	CAudio_PlaySound(SfxScore, 0);
}

void SubScoreScreen()
{
	if(GameState == GSSubScoreInit)
	{
		InitSubScoreScreen();
		GameState -= initDiff;
	}

	SDL_SetRenderTarget(Renderer, TexOffScreen);
	switch (ActiveGameGameStateId)
	{
		case GSSnake:
			GameSnake->Draw(GameSnake);
			break;
		case GSRamIt:
			GameRamIt->Draw(GameRamIt);
			break;
		case GSPang:
			GamePang->Draw(GamePang);
			break;
		case GSSpaceInvaders:
			GameInvaders->Draw(GameInvaders);
			break;
		case GSEddy:
			GameFastEddy->Draw(GameFastEddy);
			break;
		case GSBreakout:
			GameBreakOut->Draw(GameBreakOut);
			break;
		case GSFrog:
			GameFrog->Draw(GameFrog);
			break;
		case GSTetris:
			GameBlockStacker->Draw(GameBlockStacker);
			break;
	}
	// SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 128);
	// SDL_RenderFillRect(Renderer, NULL);

	if (SubGameState == SGFrame)
	{
		if (SubStateCounter < 16.0f)
			SubStateCounter += 0.2f;
		else
			SubGameState = SGCalcScore;
	}

	//so we can can copy the transparant part with the blue and text from this image
	SDL_Point pos = {ScreenWidth / 2, ScreenHeight / 2};
	Vec2F Scale = {SubStateCounter / 4*xscale, (0.0f > SubStateCounter-3.2f ? 0.0f:SubStateCounter-3.2f)*yscale};
	CImage_DrawImageFuze(Renderer, GFXFrameID, true, &pos, 0, &Scale, 255, 255, 255, 240);
	char Text[1000];
	SDL_Color color = {255,255,255,255};
	SDL_Point MedalTz = CImage_ImageSize(GFXMedal);
	Vec2F MedalScale = {50.0f/MedalTz.y*xscale, 50.0f/MedalTz.y*yscale};
	if (SubGameState == SGCalcScore)
	{
		if ((Game == Games -1) && (GameMode == GMRetroCarousel))
		{
			strcpy(Text, "Final Results");
			int Texw = CFont_TextWidth("Roboto-Regular", 80*yscale, Text, strlen(Text));
			CFont_WriteText(Renderer, "Roboto-Regular", 80*yscale, Text, strlen(Text), (ScreenWidth-Texw)/2, 50*yscale, 0, color);
			strcpy(Text,"-------------------------");
			Texw = CFont_TextWidth("Roboto-Regular", 80*yscale, Text, strlen(Text));
			CFont_WriteText(Renderer, "Roboto-Regular", 80*yscale, Text, strlen(Text), (ScreenWidth-Texw)/2, 85*yscale, 0, color);
		}
		else
		{
			strcpy(Text,"Results");
			int Texw = CFont_TextWidth("Roboto-Regular", 80*yscale, Text, strlen(Text));
			CFont_WriteText(Renderer, "Roboto-Regular", 80*yscale, Text, strlen(Text), (ScreenWidth-Texw)/2, 50*yscale, 0, color);
			strcpy(Text, "----------------");
			Texw = CFont_TextWidth("Roboto-Regular", 80*yscale, Text, strlen(Text));
			CFont_WriteText(Renderer, "Roboto-Regular", 80*yscale, Text, strlen(Text), (ScreenWidth-Texw)/2, 85*yscale, 0, color);
		}
		sprintf(Text, "Game: %s", GSGames[Game].name);
		CFont_WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, strlen(Text), 150*xscale, 180*yscale, 0, color);
		sprintf(Text, "Game Mode: %s", GMModes[GameMode].name);
		CFont_WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, strlen(Text), 150*xscale, 225*yscale, 0, color);

		if (GameMode == GMRetroCarousel)
		{
			sprintf(Text, "Previous Rounds Score: %d", prevretrocarouselscore);
			CFont_WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, strlen(Text), 150*xscale, 285*yscale, 0, color);

			sprintf(Text, "Game Score: %llu", Scores[Game][GameMode]);
			CFont_WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, strlen(Text), 150*xscale, 330*yscale, 0, color);

			if (Game < Games -1)
				sprintf(Text, "New Total Rounds Score: %llu", RetroCarouselScore);
			else
				sprintf(Text, "Final Total Rounds Score: %llu", RetroCarouselScore);
			CFont_WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, strlen(Text), 150*xscale, 375*yscale, 0, color);

			if (wasnewhighscoregame)
			{
				sprintf(Text, "New Game High Score: %llu", HighScores[Game][GameMode]);
				CFont_WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, strlen(Text), 150*xscale, 425*yscale, 0, color);
				SDL_Point pos = {(int)(120*xscale),(int) (425*yscale)};
				CImage_DrawImageFuze(Renderer, GFXMedal, false, &pos, 0, &MedalScale, 255, 255, 255, 255);
			}
			else
			{
				sprintf(Text, "Game High Score: %llu", HighScores[Game][GameMode]);
				CFont_WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, strlen(Text), 150*xscale, 425*yscale, 0, color);
			}

			if (wasnewhighscorecarousel)
			{
				sprintf(Text, "New All Rounds High Score: %llu", RetroCarouselHighScore);
				CFont_WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, strlen(Text), 150*xscale, 470*yscale, 0, color);
				SDL_Point pos = {(int)(120*xscale),(int)( 470*yscale)};
				CImage_DrawImageFuze(Renderer, GFXMedal, false, &pos, 0, &MedalScale, 255, 255, 255, 255);
			}
			else
			{
				sprintf(Text, "All Rounds High Score: %llu", RetroCarouselHighScore);
				CFont_WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, strlen(Text), 150*xscale, 470*yscale, 0, color);
			}

			if (Game < Games -1)
				sprintf(Text, "Press (A) for next game: %s", GSGames[Game+1].name);
			else
				strcpy(Text, "Press (A) for titlescreen");
			int Texw = CFont_TextWidth("Roboto-Regular", 34*yscale, Text, strlen(Text));
			CFont_WriteText(Renderer, "Roboto-Regular", 34*yscale, Text, strlen(Text), (ScreenWidth-Texw)/2, 630*yscale, 0, color);
		}
		else
		{
			sprintf(Text, "Game Score: %llu", Scores[Game][GameMode]);
			CFont_WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, strlen(Text), 150*xscale, 285*yscale, 0, color);

			if (wasnewhighscoregame)
			{
				sprintf(Text, "New Game High Score: %llu", HighScores[Game][GameMode]);
				CFont_WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, strlen(Text), 150*xscale, 330*yscale, 0, color);
				SDL_Point pos = {(int)(120*xscale),(int)(330*yscale)};
				CImage_DrawImageFuze(Renderer, GFXMedal, false, &pos, 0, &MedalScale, 255, 255, 255, 255);
			}
			else
			{
				sprintf(Text, "Game High Score: %llu", HighScores[Game][GameMode]);
				CFont_WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, strlen(Text), 150*xscale, 330*yscale, 0, color);
			}
			strcpy(Text, "Press (A) for titlescreen");
			int Texw = CFont_TextWidth("Roboto-Regular", 34*yscale, Text, strlen(Text));
			CFont_WriteText(Renderer, "Roboto-Regular", 34*yscale, Text, strlen(Text), (ScreenWidth-Texw)/2, 630*yscale, 0, color);
		}
	}

	SDL_SetRenderTarget(Renderer, TexScreen);
	SDL_RenderCopy(Renderer, TexOffScreen, NULL, NULL);

	if ((!CInput_PrevButtons.ButA && CInput_Buttons.ButA) ||
		(!CInput_PrevButtons.ButBack && CInput_Buttons.ButBack) ||
		(!CInput_PrevButtons.ButStart && CInput_Buttons.ButStart))
	{
		CAudio_PlaySound(SfxSelect, 0);

		GameState = GSTitleScreenInit;
		if(GameMode == GMRetroCarousel)
		{
			if(Game < Games -1)
			{
				Game += 1;
				GameState = GSGames[Game].state;
			}
		}
	}
}
