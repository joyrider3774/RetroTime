#include <SDL.h>
#include <string.h>
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
	Audio->PlaySound(SfxScore, 0);
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
	Vec2F Scale = {SubStateCounter / 4*xscale, std::max(0.0f, SubStateCounter-3.2f)*yscale};
	CImage_DrawImageFuze(Renderer, GFXFrameID, true, &pos, 0, &Scale, 255, 255, 255, 240);
	string Text;
	SDL_Point MedalTz = CImage_ImageSize(GFXMedal);
	Vec2F MedalScale = {50.0f/MedalTz.y*xscale, 50.0f/MedalTz.y*yscale};
	if (SubGameState == SGCalcScore)
	{
		if ((Game == Games -1) && (GameMode == GMRetroCarousel))
		{
			Text = "Final Results";
			int Texw = Font->TextWidth("Roboto-Regular", 80*yscale, Text, Text.length());
			Font->WriteText(Renderer, "Roboto-Regular", 80*yscale, Text, Text.length(), (ScreenWidth-Texw)/2, 50*yscale, 0, {255,255,255,255});
			Text = "-------------------------";
			Texw = Font->TextWidth("Roboto-Regular", 80*yscale, Text, Text.length());
			Font->WriteText(Renderer, "Roboto-Regular", 80*yscale, Text, Text.length(), (ScreenWidth-Texw)/2, 85*yscale, 0, {255,255,255,255});
		}
		else
		{
			Text = "Results";
			int Texw = Font->TextWidth("Roboto-Regular", 80*yscale, Text, Text.length());
			Font->WriteText(Renderer, "Roboto-Regular", 80*yscale, Text, Text.length(), (ScreenWidth-Texw)/2, 50*yscale, 0, {255,255,255,255});
			Text = "----------------";
			Texw = Font->TextWidth("Roboto-Regular", 80*yscale, Text, Text.length());
			Font->WriteText(Renderer, "Roboto-Regular", 80*yscale, Text, Text.length(), (ScreenWidth-Texw)/2, 85*yscale, 0, {255,255,255,255});
		}
		Text = "Game: " + GSGames[Game].name;
		Font->WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 150*xscale, 180*yscale, 0, {255,255,255,255});
		Text = "Game Mode: " + GMModes[GameMode].name;
		Font->WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 150*xscale, 225*yscale, 0, {255,255,255,255});

		if (GameMode == GMRetroCarousel)
		{
			Text = "Previous Rounds Score: " + to_string(prevretrocarouselscore);
			Font->WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 150*xscale, 285*yscale, 0, {255,255,255,255});

			Text = "Game Score: " + to_string(Scores[Game][GameMode]);
			Font->WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 150*xscale, 330*yscale, 0, {255,255,255,255});

			if (Game < Games -1)
				Text = "New Total Rounds Score: " + to_string(RetroCarouselScore);
			else
				Text = "Final Total Rounds Score: " + to_string(RetroCarouselScore);
			Font->WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 150*xscale, 375*yscale, 0, {255,255,255,255});

			if (wasnewhighscoregame)
			{
				Text = "New Game High Score: " + to_string(HighScores[Game][GameMode]);
				Font->WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 150*xscale, 425*yscale, 0, {255,255,255,255});
				SDL_Point pos = {(int)(120*xscale),(int) (425*yscale)};
				CImage_DrawImageFuze(Renderer, GFXMedal, false, &pos, 0, &MedalScale, 255, 255, 255, 255);
			}
			else
			{
				Text = "Game High Score: " + to_string(HighScores[Game][GameMode]);
				Font->WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 150*xscale, 425*yscale, 0, {255,255,255,255});
			}

			if (wasnewhighscorecarousel)
			{
				Text = "New All Rounds High Score: " + to_string(RetroCarouselHighScore);
				Font->WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 150*xscale, 470*yscale, 0, {255,255,255,255});
				SDL_Point pos = {(int)(120*xscale),(int)( 470*yscale)};
				CImage_DrawImageFuze(Renderer, GFXMedal, false, &pos, 0, &MedalScale, 255, 255, 255, 255);
			}
			else
			{
				Text = "All Rounds High Score: " + to_string(RetroCarouselHighScore);
				Font->WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 150*xscale, 470*yscale, 0, {255,255,255,255});
			}

			if (Game < Games -1)
				Text = "Press (A) for next game: " + GSGames[Game+1].name;
			else
				Text = "Press (A) for titlescreen";
			int Texw = Font->TextWidth("Roboto-Regular", 34*yscale, Text, Text.length());
			Font->WriteText(Renderer, "Roboto-Regular", 34*yscale, Text, Text.length(), (ScreenWidth-Texw)/2, 630*yscale, 0, {255,255,255,255});
		}
		else
		{
			Text = "Game Score: " + to_string(Scores[Game][GameMode]);
			Font->WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 150*xscale, 285*yscale, 0, {255,255,255,255});

			if (wasnewhighscoregame)
			{
				Text = "New Game High Score: " + to_string(HighScores[Game][GameMode]);
				Font->WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 150*xscale, 330*yscale, 0, {255,255,255,255});
				SDL_Point pos = {(int)(120*xscale),(int)(330*yscale)};
				CImage_DrawImageFuze(Renderer, GFXMedal, false, &pos, 0, &MedalScale, 255, 255, 255, 255);
			}
			else
			{
				Text = "Game High Score: " + to_string(HighScores[Game][GameMode]);
				Font->WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 150*xscale, 330*yscale, 0, {255,255,255,255});
			}
			Text = "Press (A) for titlescreen";
			int Texw = Font->TextWidth("Roboto-Regular", 34*yscale, Text, Text.length());
			Font->WriteText(Renderer, "Roboto-Regular", 34*yscale, Text, Text.length(), (ScreenWidth-Texw)/2, 630*yscale, 0, {255,255,255,255});
		}
	}

	SDL_SetRenderTarget(Renderer, TexScreen);
	SDL_RenderCopy(Renderer, TexOffScreen, NULL, NULL);

	if ((!Input->PrevButtons.ButA && Input->Buttons.ButA) ||
		(!Input->PrevButtons.ButBack && Input->Buttons.ButBack) ||
		(!Input->PrevButtons.ButStart && Input->Buttons.ButStart))
	{
		Audio->PlaySound(SfxSelect, 0);

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
