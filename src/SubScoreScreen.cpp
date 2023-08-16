#include <SDL.h>
#include <string>
#include <cstring>
#include "games/CGameBase.h"
#include "scoresubmit/scoresubmit.h"
#include "SubScoreScreen.h"
#include "CGame.h"
#include "Common.h"
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
	Game->Audio->PlaySnd(Game->SfxScore, 0);
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
	std::string Text;
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
			Text = "Previous Rounds Score: " + std::to_string(prevretrocarouselscore);
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 285, 0, {255,255,255,255});

			Text = "Game Score: " + std::to_string(Game->Scores[Game->Game][Game->GameMode]);
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 320, 0, {255,255,255,255});

			if (Game->Game < Games -1)
				Text = "New Total Rounds Score: " + std::to_string(Game->RetroCarouselScore);
			else
				Text = "Final Total Rounds Score: " + std::to_string(Game->RetroCarouselScore);
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 355, 0, {255,255,255,255});

			if (wasnewhighscoregame)
			{
				Text = "New Game High Score: " + std::to_string(Game->HighScores[Game->Game][Game->GameMode]);
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 415, 0, {255,255,255,255});
				SDL_Point pos = {270, 415};
				Game->Image->DrawImageFuze(Game->Renderer, Game->GFXMedal, false, &pos, 0, &MedalScale, 255, 255, 255, 255);
			}
			else
			{
				Text = "Game High Score: " + std::to_string(Game->HighScores[Game->Game][Game->GameMode]);
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 415, 0, {255,255,255,255});
			}

			if (wasnewhighscorecarousel)
			{
				Text = "New All Rounds High Score: " + std::to_string(Game->RetroCarouselHighScore);
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 450, 0, {255,255,255,255});
				SDL_Point pos = {270, 450};
				Game->Image->DrawImageFuze(Game->Renderer, Game->GFXMedal, false, &pos, 0, &MedalScale, 255, 255, 255, 255);
			}
			else
			{
				Text = "All Rounds High Score: " + std::to_string(Game->RetroCarouselHighScore);
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 450, 0, {255,255,255,255});
			}

			if (Game->Game < Games -1)
				Text = "Press (A) for next game: " + GSGames[Game->Game+1].name;
			else
				Text = "Press (A) for titlescreen";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 20, Text, Text.length(), 300, 555, 0, {255,255,255,255});
		}
		else
		{
			Text = "Game Score: " + std::to_string(Game->Scores[Game->Game][Game->GameMode]);
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 285, 0, {255,255,255,255});

			if (wasnewhighscoregame)
			{
				Text = "New Game High Score: " + std::to_string(Game->HighScores[Game->Game][Game->GameMode]);
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 320, 0, {255,255,255,255});
				SDL_Point pos = {270, 320};
				Game->Image->DrawImageFuze(Game->Renderer, Game->GFXMedal, false, &pos, 0, &MedalScale, 255, 255, 255, 255);
			}
			else
			{
				Text = "Game High Score: " + std::to_string(Game->HighScores[Game->Game][Game->GameMode]);
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 320, 0, {255,255,255,255});
			}
			Text = "Press (A) for titlescreen";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 20, Text, Text.length(), 300, 555, 0, {255,255,255,255});
		}	
	}
	else
	{
		if (Game->SubGameState == SGSubmitScore)
		{
			
			Text = "Submit Your High Score Online";
			int w = Game->Font->TextWidth("Roboto-Regular", 45, Text, Text.length());
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 45, Text, Text.length(), (ScreenWidth - w) >> 1, 110, 0, {255,255,255,255});
			Text = "------------------------------------------------------";
			w = Game->Font->TextWidth("Roboto-Regular", 45, Text, Text.length());
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 45, Text, Text.length(), (ScreenWidth - w) >> 1, 135, 0, {255,255,255,255});
			
			Text = "Game: " + GSGames[Game->Game].name;
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 180, 0, {255,255,255,255});
			Text = "Game Mode: " + GMModes[Game->GameMode].name;
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 225, 0, {255,255,255,255});

			if (Game->GameMode == GMRetroCarousel)
			{
				if (wasnewhighscoregame)
				{
					Text = "New Game High Score: " + std::to_string(Game->HighScores[Game->Game][Game->GameMode]);
					Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 275, 0, {255,255,255,255});
					SDL_Point pos = {270, 275};
					Game->Image->DrawImageFuze(Game->Renderer, Game->GFXMedal, false, &pos, 0, &MedalScale, 255, 255, 255, 255);
				}
				
				if ((Game->Game == Games -1) && wasnewhighscorecarousel)
				{
					Text = "New All Rounds High Score: " + std::to_string(Game->RetroCarouselHighScore);
					Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 330, 0, {255,255,255,255});
					SDL_Point pos = {270, 330};
					Game->Image->DrawImageFuze(Game->Renderer, Game->GFXMedal, false, &pos, 0, &MedalScale, 255, 255, 255, 255);
				}

				if (Game->Game < Games -1)
					Text = "Press (A) to submit, (B) to not submit and start next game: " + GSGames[Game->Game+1].name;
				else
					Text = "Press (A) to submit, (B) to not submit and goto titlescreen";
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 20, Text, Text.length(), 300, 555, 0, {255,255,255,255});
				Text = "Use Up, Down, Left, Right to enter name";
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 20, Text, Text.length(), 300, 525, 0, {255,255,255,255});
			}
			else
			{
				if (wasnewhighscoregame)
				{
					Text = "New Game High Score: " + std::to_string(Game->HighScores[Game->Game][Game->GameMode]);
					Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 275, 0, {255,255,255,255});
					SDL_Point pos = {270, 275};
					Game->Image->DrawImageFuze(Game->Renderer, Game->GFXMedal, false, &pos, 0, &MedalScale, 255, 255, 255, 255);
				}
				Text = "Press (A) to submit, (B) to not submit and goto titlescreen";
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 20, Text, Text.length(), 300, 555, 0, {255,255,255,255});
				Text = "Use Up, Down, Left, Right to enter name";
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 20, Text, Text.length(), 300, 525, 0, {255,255,255,255});
			}

			Text = "Enter Your Name:";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 385, 0, {255,255,255,255});

			int spacing = 30;
			Text = std::string(Game->HighScoreName);
			for (size_t i = 0; i < Text.length(); i++)
			{
				std::string Character;
				Character.push_back(Text[i]);
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Character, Character.length(), 300 + (i * spacing ), 440, 0, {255,255,255,255});
			}

			std::string Text2 = "_";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text2, Text2.length(), 303 + ((Text.length()-1) * spacing ), 445, 0, {255,255,255,255});
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text2, Text2.length(), 303 + ((Text.length()-1) * spacing ), 410, 0, {255,255,255,255});			
		}
	}

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

	SDL_SetRenderTarget(Game->Renderer, Game->TexScreen);
	SDL_RenderCopy(Game->Renderer, Game->TexOffScreen, NULL, NULL);


	if ((!Game->Input->PrevButtons.ButA && Game->Input->Buttons.ButA) ||
		(!Game->Input->PrevButtons.ButB && Game->Input->Buttons.ButB) ||
		(!Game->Input->PrevButtons.ButBack && Game->Input->Buttons.ButBack) ||
		(!Game->Input->PrevButtons.ButStart && Game->Input->Buttons.ButStart))
	{
		Game->Audio->PlaySnd(Game->SfxSelect, 0);

		if((Game->SubGameState == SGSubmitScore) || ((Game->GameMode == GMRetroCarousel) && (Game->Game == Games -1) && !wasnewhighscorecarousel) || !wasnewhighscoregame)
		{
			if (Game->Input->Buttons.ButA || Game->Input->Buttons.ButStart)
			{
				if ((Game->GameMode == GMRetroCarousel) && (Game->Game == Games -1) && wasnewhighscorecarousel)
				{
					std::string Url = GetSubmitScoreUrl("E7LL=K67sZIBEXspsi9yry3t=3?Fd%sO", LeaderBoardIds[8][0], Game->RetroCarouselHighScore, std::string(Game->HighScoreName));
					Game->http->AddRequest(Url.c_str(), true);
				}

				if (wasnewhighscoregame)
				{
					std::string Url = GetSubmitScoreUrl("E7LL=K67sZIBEXspsi9yry3t=3?Fd%sO", LeaderBoardIds[Game->Game][Game->GameMode], Game->HighScores[Game->Game][Game->GameMode], std::string(Game->HighScoreName));
					Game->http->AddRequest(Url.c_str(), true);
				}
			}


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

		if(((Game->GameMode == GMRetroCarousel) && (Game->Game == Games -1) && wasnewhighscorecarousel) || wasnewhighscoregame)
		{
			Game->SubGameState = SGSubmitScore;
		}
	}


	if ((Game->SubGameState == SGSubmitScore) &&
		((!Game->Input->PrevButtons.ButLeft && Game->Input->Buttons.ButLeft) ||
		(!Game->Input->PrevButtons.ButLeft2 && Game->Input->Buttons.ButLeft2) ||
		(!Game->Input->PrevButtons.ButDpadLeft && Game->Input->Buttons.ButDpadLeft)))
		{
			size_t len = std::strlen(Game->HighScoreName);
			if (len > 1)
			{
				Game->HighScoreName[len-1] = '\0';
			}
		}		
	
	if ((Game->SubGameState == SGSubmitScore) &&
		((!Game->Input->PrevButtons.ButRight && Game->Input->Buttons.ButRight) ||
		(!Game->Input->PrevButtons.ButLeft2 && Game->Input->Buttons.ButRight2) ||
		(!Game->Input->PrevButtons.ButDpadRight && Game->Input->Buttons.ButDpadRight)))
		{
			size_t len = std::strlen(Game->HighScoreName);
			if (len < 15)
			{
				Game->HighScoreName[len+1] = '\0';
				Game->HighScoreName[len] = Game->HighScoreName[len-1];
			}
		}
		
	if ((Game->SubGameState == SGSubmitScore) &&
		((!Game->Input->PrevButtons.ButUp && Game->Input->Buttons.ButUp) ||
		(!Game->Input->PrevButtons.ButUp2 && Game->Input->Buttons.ButUp2) ||
		(!Game->Input->PrevButtons.ButDpadUp && Game->Input->Buttons.ButDpadUp)))
		{
			size_t len = std::strlen(Game->HighScoreName);
			Game->HighScoreName[len-1]++;
			if (Game->HighScoreName[len-1] == 'z' + 1)
				Game->HighScoreName[len-1] = ' ';
			else
			{
				if (Game->HighScoreName[len-1] == 'Z' + 1)
					Game->HighScoreName[len-1] = 'a';
				else
				{
					if (Game->HighScoreName[len-1] == '9' + 1)
						Game->HighScoreName[len-1] = 'A';
					else
					{
						if (Game->HighScoreName[len-1] == ' ' + 1)
							Game->HighScoreName[len-1] = '0';
			
					}
				}
			}
		}

	
	if ((Game->SubGameState == SGSubmitScore) &&
		((!Game->Input->PrevButtons.ButDown && Game->Input->Buttons.ButDown) ||
		(!Game->Input->PrevButtons.ButDown2 && Game->Input->Buttons.ButDown2) ||
		(!Game->Input->PrevButtons.ButDpadDown && Game->Input->Buttons.ButDpadDown)))
		{
			size_t len = std::strlen(Game->HighScoreName);
			Game->HighScoreName[len-1]--;
			if (Game->HighScoreName[len-1] == ' ' - 1)
				Game->HighScoreName[len-1] = 'z';
			else
			{
				if (Game->HighScoreName[len-1] == 'a' - 1)
					Game->HighScoreName[len-1] = 'Z';
				else
				{
					if (Game->HighScoreName[len-1] == 'A' - 1)
						Game->HighScoreName[len-1] = '9';
					else
					{
						if (Game->HighScoreName[len-1] == '0' - 1) 
							Game->HighScoreName[len-1] = ' ';
					}
				}
			}	
		}
}
