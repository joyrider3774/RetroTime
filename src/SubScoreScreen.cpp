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

void InitSubScoreScreen(CGame *Game)
{
	Game->SubGameState = SGFrame;
	Game->SubStateCounter = 0;
	wasnewhighscorecarousel = false;
	wasnewhighscoregame = false;
	prevretrocarouselscore = Game->RetroCarouselScore;
	if (Game->GameMode == GMRetroCarousel)
		Game->RetroCarouselScore += Game->Scores[Game->Game][Game->GameMode];

	if (Game->RetroCarouselScore > Game->RetroCarouselHighScore)
	{
		Game->RetroCarouselHighScore = Game->RetroCarouselScore;
		wasnewhighscorecarousel = true;
	}

	if (Game->Scores[Game->Game][Game->GameMode] > Game->HighScores[Game->Game][Game->GameMode])
	{
		Game->HighScores[Game->Game][Game->GameMode] = Game->Scores[Game->Game][Game->GameMode];
		wasnewhighscoregame = true;
	}

	//savehighscoresoptions()
	Game->Audio->PlaySound(Game->SfxScore, 0);
}

void SubScoreScreen(CGame *Game)
{
	if(Game->GameState == GSSubScoreInit)
	{
		InitSubScoreScreen(Game);
		Game->GameState -= initDiff;
	}

	SDL_SetRenderTarget(Game->Renderer, Game->TexOffScreen);
	switch (Game->ActiveGameGameStateId)
	{
		case GSSnake:
			Game->GameSnake->Draw(Game->GameSnake);
			break;
		case GSRamIt:
			Game->GameRamIt->Draw(Game->GameRamIt);
			break;
		case GSPang:
			Game->GamePang->Draw(Game->GamePang);
			break;
		case GSSpaceInvaders:
			Game->GameInvaders->Draw(Game->GameInvaders);
			break;
		case GSEddy:
			Game->GameFastEddy->Draw(Game->GameFastEddy);
			break;
		case GSBreakout:
			Game->GameBreakOut->Draw(Game->GameBreakOut);
			break;
		case GSFrog:
			Game->GameFrog->Draw(Game->GameFrog);
			break;
		case GSTetris:
			Game->GameBlockStacker->Draw(Game->GameBlockStacker);
			break;
	}
	// SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 128);
	// SDL_RenderFillRect(Renderer, NULL);

	if (Game->SubGameState == SGFrame)
	{
		if (Game->SubStateCounter < 16.0f)
			Game->SubStateCounter += 0.2f;
		else
			Game->SubGameState = SGCalcScore;
	}

	//so we can can copy the transparant part with the blue and text from this image
	SDL_Point pos = {ScreenWidth / 2, ScreenHeight / 2};
	Vec2F Scale = {Game->SubStateCounter / 4*xscale, std::max(0.0f, Game->SubStateCounter-3.2f)*yscale};
	Game->Image->DrawImageFuze(Game->Renderer, Game->GFXFrameID, true, &pos, 0, &Scale, 255, 255, 255, 240);
	string Text;
	SDL_Point MedalTz = Game->Image->ImageSize(Game->GFXMedal);
	Vec2F MedalScale = {50.0f/MedalTz.y*xscale, 50.0f/MedalTz.y*yscale};
	if (Game->SubGameState == SGCalcScore)
	{
		if ((Game->Game == Games -1) && (Game->GameMode == GMRetroCarousel))
		{
			Text = "Final Results";
			int Texw = Game->Font->TextWidth("Roboto-Regular", 80*yscale, Text, Text.length());
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 80*yscale, Text, Text.length(), (ScreenWidth-Texw)/2, 50*yscale, 0, {255,255,255,255});
			Text = "-------------------------";
			Texw = Game->Font->TextWidth("Roboto-Regular", 80*yscale, Text, Text.length());
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 80*yscale, Text, Text.length(), (ScreenWidth-Texw)/2, 85*yscale, 0, {255,255,255,255});
		}
		else
		{
			Text = "Results";
			int Texw = Game->Font->TextWidth("Roboto-Regular", 80*yscale, Text, Text.length());
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 80*yscale, Text, Text.length(), (ScreenWidth-Texw)/2, 50*yscale, 0, {255,255,255,255});
			Text = "----------------";
			Texw = Game->Font->TextWidth("Roboto-Regular", 80*yscale, Text, Text.length());
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 80*yscale, Text, Text.length(), (ScreenWidth-Texw)/2, 85*yscale, 0, {255,255,255,255});
		}
		Text = "Game: " + GSGames[Game->Game].name;
		Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 150*xscale, 180*yscale, 0, {255,255,255,255});
		Text = "Game Mode: " + GMModes[Game->GameMode].name;
		Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 150*xscale, 225*yscale, 0, {255,255,255,255});

		if (Game->GameMode == GMRetroCarousel)
		{
			Text = "Previous Rounds Score: " + to_string(prevretrocarouselscore);
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 150*xscale, 285*yscale, 0, {255,255,255,255});

			Text = "Game Score: " + to_string(Game->Scores[Game->Game][Game->GameMode]);
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 150*xscale, 330*yscale, 0, {255,255,255,255});

			if (Game->Game < Games -1)
				Text = "New Total Rounds Score: " + to_string(Game->RetroCarouselScore);
			else
				Text = "Final Total Rounds Score: " + to_string(Game->RetroCarouselScore);
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 150*xscale, 375*yscale, 0, {255,255,255,255});

			if (wasnewhighscoregame)
			{
				Text = "New Game High Score: " + to_string(Game->HighScores[Game->Game][Game->GameMode]);
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 150*xscale, 425*yscale, 0, {255,255,255,255});
				SDL_Point pos = {(int)(120*xscale),(int) (425*yscale)};
				Game->Image->DrawImageFuze(Game->Renderer, Game->GFXMedal, false, &pos, 0, &MedalScale, 255, 255, 255, 255);
			}
			else
			{
				Text = "Game High Score: " + to_string(Game->HighScores[Game->Game][Game->GameMode]);
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 150*xscale, 425*yscale, 0, {255,255,255,255});
			}

			if (wasnewhighscorecarousel)
			{
				Text = "New All Rounds High Score: " + to_string(Game->RetroCarouselHighScore);
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 150*xscale, 470*yscale, 0, {255,255,255,255});
				SDL_Point pos = {(int)(120*xscale),(int)( 470*yscale)};
				Game->Image->DrawImageFuze(Game->Renderer, Game->GFXMedal, false, &pos, 0, &MedalScale, 255, 255, 255, 255);
			}
			else
			{
				Text = "All Rounds High Score: " + to_string(Game->RetroCarouselHighScore);
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 150*xscale, 470*yscale, 0, {255,255,255,255});
			}

			if (Game->Game < Games -1)
				Text = "Press (A) for next game: " + GSGames[Game->Game+1].name;
			else
				Text = "Press (A) for titlescreen";
			int Texw = Game->Font->TextWidth("Roboto-Regular", 34*yscale, Text, Text.length());
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 34*yscale, Text, Text.length(), (ScreenWidth-Texw)/2, 630*yscale, 0, {255,255,255,255});
		}
		else
		{
			Text = "Game Score: " + to_string(Game->Scores[Game->Game][Game->GameMode]);
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 150*xscale, 285*yscale, 0, {255,255,255,255});

			if (wasnewhighscoregame)
			{
				Text = "New Game High Score: " + to_string(Game->HighScores[Game->Game][Game->GameMode]);
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 150*xscale, 330*yscale, 0, {255,255,255,255});
				SDL_Point pos = {(int)(120*xscale),(int)(330*yscale)};
				Game->Image->DrawImageFuze(Game->Renderer, Game->GFXMedal, false, &pos, 0, &MedalScale, 255, 255, 255, 255);
			}
			else
			{
				Text = "Game High Score: " + to_string(Game->HighScores[Game->Game][Game->GameMode]);
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 150*xscale, 330*yscale, 0, {255,255,255,255});
			}
			Text = "Press (A) for titlescreen";
			int Texw = Game->Font->TextWidth("Roboto-Regular", 34*yscale, Text, Text.length());
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 34*yscale, Text, Text.length(), (ScreenWidth-Texw)/2, 630*yscale, 0, {255,255,255,255});
		}
	}

	SDL_SetRenderTarget(Game->Renderer, Game->TexScreen);
	SDL_RenderCopy(Game->Renderer, Game->TexOffScreen, NULL, NULL);

	if ((!Game->Input->PrevButtons.ButA && Game->Input->Buttons.ButA) ||
		(!Game->Input->PrevButtons.ButBack && Game->Input->Buttons.ButBack) ||
		(!Game->Input->PrevButtons.ButStart && Game->Input->Buttons.ButStart))
	{
		Game->Audio->PlaySound(Game->SfxSelect, 0);

		Game->GameState = GSTitleScreenInit;
		if(Game->GameMode == GMRetroCarousel)
		{
			if(Game->Game < Games -1)
			{
				Game->Game += 1;
				Game->GameState = GSGames[Game->Game].state;
			}
		}
	}
}
