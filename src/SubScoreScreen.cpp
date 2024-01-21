#include <SDL.h>
#include <string.h>
#include "CGame.h"
#include "games/CGameBase.h"
#include "Common.h"
#include "SubScoreScreen.h"
#include "Vec2F.h"

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
	Game->ActiveGame->Draw();
	// SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 128);
	// SDL_RenderFillRect(Renderer, NULL);

	if (Game->SubGameState == SGFrame)
	{
		if (Game->SubStateCounter < 10.4f)
			Game->SubStateCounter += 0.2f;
		else
			Game->SubGameState = SGCalcScore;
	}

	//so we can can copy the transparant part with the blue and text from this image
	SDL_Point pos = {ScreenWidth / 2, ScreenHeight / 2};
	Vec2F Scale = {Game->SubStateCounter / 4, Game->SubStateCounter};
	Game->Image->DrawImageFuze(Game->Renderer, Game->GFXFrameID, true, &pos, 0, &Scale, 255, 255, 255, 240);
	string Text;
	SDL_Point MedalTz = Game->Image->ImageSize(Game->GFXMedal);
	Vec2F MedalScale = {30.0f/MedalTz.y, 30.0f/MedalTz.y};
	if (Game->SubGameState == SGCalcScore)
	{
		if ((Game->Game == Games -1) && (Game->GameMode == GMRetroCarousel))
		{
			Text = "Final Results";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 45, Text, Text.length(), 500, 110, 0, {255,255,255,255});
			Text = "-------------------------";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 45, Text, Text.length(), 475, 135, 0, {255,255,255,255});
		}
		else
		{
			Text = "Results";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 45, Text, Text.length(), 560, 110, 0, {255,255,255,255});
			Text = "----------------";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 45, Text, Text.length(), 535, 135, 0, {255,255,255,255});
		}
		Text = "Game: " + GSGames[Game->Game].name;
		Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 180, 0, {255,255,255,255});
		Text = "Game Mode: " + GMModes[Game->GameMode].name;
		Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 225, 0, {255,255,255,255});

		if (Game->GameMode == GMRetroCarousel)
		{
			Text = "Previous Rounds Score: " + to_string(prevretrocarouselscore);
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 285, 0, {255,255,255,255});

			Text = "Game Score: " + to_string(Game->Scores[Game->Game][Game->GameMode]);
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 320, 0, {255,255,255,255});

			if (Game->Game < Games -1)
				Text = "New Total Rounds Score: " + to_string(Game->RetroCarouselScore);
			else
				Text = "Final Total Rounds Score: " + to_string(Game->RetroCarouselScore);
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 355, 0, {255,255,255,255});

			if (wasnewhighscoregame)
			{
				Text = "New Game High Score: " + to_string(Game->HighScores[Game->Game][Game->GameMode]);
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 415, 0, {255,255,255,255});
				SDL_Point pos = {270, 415};
				Game->Image->DrawImageFuze(Game->Renderer, Game->GFXMedal, false, &pos, 0, &MedalScale, 255, 255, 255, 255);
			}
			else
			{
				Text = "Game High Score: " + to_string(Game->HighScores[Game->Game][Game->GameMode]);
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 415, 0, {255,255,255,255});
			}

			if (wasnewhighscorecarousel)
			{
				Text = "New All Rounds High Score: " + to_string(Game->RetroCarouselHighScore);
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 450, 0, {255,255,255,255});
				SDL_Point pos = {270, 450};
				Game->Image->DrawImageFuze(Game->Renderer, Game->GFXMedal, false, &pos, 0, &MedalScale, 255, 255, 255, 255);
			}
			else
			{
				Text = "All Rounds High Score: " + to_string(Game->RetroCarouselHighScore);
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 450, 0, {255,255,255,255});
			}

			if (Game->Game < Games -1)
				Text = "Press (A) for next game: " + GSGames[Game->Game+1].name;
			else
				Text = "Press (A) for titlescreen";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 545, 0, {255,255,255,255});
		}
		else
		{
			Text = "Game Score: " + to_string(Game->Scores[Game->Game][Game->GameMode]);
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 285, 0, {255,255,255,255});

			if (wasnewhighscoregame)
			{
				Text = "New Game High Score: " + to_string(Game->HighScores[Game->Game][Game->GameMode]);
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 320, 0, {255,255,255,255});
				SDL_Point pos = {270, 320};
				Game->Image->DrawImageFuze(Game->Renderer, Game->GFXMedal, false, &pos, 0, &MedalScale, 255, 255, 255, 255);
			}
			else
			{
				Text = "Game High Score: " + to_string(Game->HighScores[Game->Game][Game->GameMode]);
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 320, 0, {255,255,255,255});
			}
			Text = "Press (A) for titlescreen";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 545, 0, {255,255,255,255});
		}
	}
#ifndef FULLMENUTRANSPARANCY
	SDL_Rect SrcRect = {245,95,790,530};
	//grab transparant part of frame + menu
	SDL_Texture *Tmp = SDL_CreateTexture(Game->Renderer, PixelFormat, SDL_TEXTUREACCESS_TARGET, SrcRect.w, SrcRect.h);
	SDL_Texture *TmpRender = SDL_GetRenderTarget(Game->Renderer);
	SDL_SetRenderTarget(Game->Renderer, Tmp);
	Game->Image->DrawImage(Game->Renderer, Game->TexOffScreen, &SrcRect, NULL);

	//draw the frame again without transparancy
	SDL_SetRenderTarget(Game->Renderer, TmpRender);
	Game->Image->DrawImageFuze(Game->Renderer, Game->GFXFrameID, true, &pos, 0, &Scale, 255, 255, 255, 255);

	//and then draw the transparant part over it now
	Game->Image->DrawImage(Game->Renderer, Tmp, NULL, &SrcRect);
	SDL_DestroyTexture(Tmp);
#endif
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
