#include <SDL.h>
#include <stdlib.h>
#include "CGameBase.h"
#include "../Vec2F.h"

CGameBase* Create_CGameBase(CGame *aGame, int aGameStateID, bool aUsesLevels)
{
	CGameBase* result = (CGameBase*) malloc(sizeof(CGameBase));
	result->Game = aGame;
	result->UsesLevels = aUsesLevels;
	result->GameStateID = aGameStateID;
	result->level = 0;
	result->HealthPoints = 0;
	result->SubStateText[0] = '\0';
	result->playfieldheight = 0;
	result->playfieldwidth = 0;
	result->screenbottom = 0;
	result->screentop = 0;
	result->screenleft = 0;
	result->screenright = 0;
	result->DrawScoreBar = CGameBase_DrawScoreBar;
	result->PauseMenu = CGameBase_PauseMenu;
	result->DrawSubStateText = CGameBase_DrawSubstateText;
	result->UpdateLogic = CGameBase_UpdateLogic;
	return result;
}

void Destroy_CGameBase(CGameBase* GameBase)
{
	free(GameBase);
}

void CGameBase_PauseMenu(CGameBase* GameBase)
{
	int prevsubgamestate = GameBase->Game->SubGameState;
	int prevsubstatecounter = GameBase->Game->SubStateCounter;
	GameBase->Game->SubGameState = SGPauseMenu;
	GameBase->Game->Audio->PlaySound(GameBase->Game->SfxConfirm, 0);
	//subgamestate = sgframe;
	//global.substatecounter = 10.6
	int selected = 0;
	int selectedmenu = 0;
	int maxmenus = 5;
	int menutextsize = 60*yscale;
	int menuspacing = 85*yscale;
	GameBase->Game->Input->ResetButtons();

	Uint64 TotalFrames = 0;
	Uint64 TotalFramePerf = 0;
	Uint32 Fps = 0;
	double AvgFrameTime = 0.0f;
	Uint32 Ticks = SDL_GetTicks();

	while ((GameBase->Game->SubGameState == SGFrame) || (GameBase->Game->SubGameState == SGPauseMenu) || (GameBase->Game->SubGameState == SGGameHelp))
	{
		TotalFrames++;
		Uint64 FrameStartPerf = SDL_GetPerformanceCounter();
		selectedmenu = GPGamePauseMenus[GameBase->Game->Game].menus[selected];

		//draw everything to offscreen surface

		SDL_SetRenderTarget(GameBase->Game->Renderer, GameBase->Game->TexOffScreen);

		// this seems to cause a blackscreen somehow when certain games
		// are paused not sure as to why but i disabled it for now
		// SDL_SetRenderDrawColor(GameBase->Game->Renderer, 0, 0, 0, 128);
		// SDL_RenderFillRect(GameBase->Game->Renderer, NULL);

		 //draw everything to offscreen surface
		SDL_SetRenderDrawColor(GameBase->Game->Renderer, 255,255, 255, 255);
		//so we can can copy the transparant part with the blue and text from this image
		SDL_Point FramePos = {ScreenWidth / 2, ScreenHeight / 2};
		Vec2F FrameScale = {16.0f / 4 * xscale, 12.8f *yscale};
		GameBase->Game->Image->DrawImageFuze(GameBase->Game->Renderer, GameBase->Game->GFXFrameID, true, &FramePos, 0, &FrameScale, 255, 255, 255, 255);

		if (GameBase->Game->SubGameState == SGPauseMenu)
		{
			string Text = "Paused";
			GameBase->Game->Font->WriteText(GameBase->Game->Renderer, "Roboto-Regular", 80*yscale, Text, Text.length(), 510*xscale, 50*yscale, 0, {255,255,255,255});
			int menu;
			SDL_Color color;
			for(int i = 0; i < maxmenus; i++)
			{
				menu = GPGamePauseMenus[GameBase->Game->Game].menus[i];
				if (menu == selectedmenu)
					color = {255, 255, 255, 255};
				else
					color = {255, 255, 255, 64};

				switch(menu)
				{
					case PMSoundVol:
						Text = PMPauseMenus[menu].name + to_string((int)(GameBase->Game->Audio->GetVolumeSound()*100/128)) + "%";
						GameBase->Game->Font->WriteText(GameBase->Game->Renderer, "Roboto-Regular", menutextsize, Text, Text.length(), 300*xscale, 185*yscale + i * menuspacing, 0, color);
						break;
					case PMMusicVol:
						Text = PMPauseMenus[menu].name + to_string((int)(GameBase->Game->Audio->GetVolumeMusic()*100/128)) + "%";
						GameBase->Game->Font->WriteText(GameBase->Game->Renderer, "Roboto-Regular", menutextsize, Text, Text.length(), 300*xscale, 185*yscale + i * menuspacing, 0, color);
						break;
					default:
						GameBase->Game->Font->WriteText(GameBase->Game->Renderer, "Roboto-Regular", menutextsize, PMPauseMenus[menu].name, PMPauseMenus[menu].name.length(), 300*xscale,
							185*yscale + i * menuspacing, 0, color);
						break;
				}
			}
			Text = "Use dpad to switch between options. (A) to select and (B) for back";
			GameBase->Game->Font->WriteText(GameBase->Game->Renderer, "Roboto-Regular", 34*yscale, Text, Text.length(), 90*xscale, 630*yscale, 0, {255, 255, 255, 255});
		}

		if (GameBase->Game->SubGameState == SGGameHelp)
		{
			string Text = "Game Help";
			GameBase->Game->Font->WriteText(GameBase->Game->Renderer, "Roboto-Regular", 80*yscale, Text, Text.length(), 485*xscale, 50*yscale, 0, {255,255,255,255});

			Text = GSGames[GameBase->Game->Game].name;
			GameBase->Game->Font->WriteText(GameBase->Game->Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 75*xscale, 150*yscale, 0, {255,255,255,255});

			Text = GMModes[GameBase->Game->GameMode].name + " High Score: " + to_string(GameBase->Game->HighScores[GameBase->Game->Game][GameBase->Game->GameMode]);
			GameBase->Game->Font->WriteText(GameBase->Game->Renderer, "Roboto-Regular", 38*yscale, Text, Text.length(), 75*xscale, 210*yscale, 0, {255,255,255,255});

			Text = GSGames[GameBase->Game->Game].description;
			GameBase->Game->Font->WriteText(GameBase->Game->Renderer, "Roboto-Regular", 38*yscale, Text, Text.length(), 75*xscale, 255*yscale, 0, {255,255,255,255});

			Text = "Press (A) or (B) for back";
			GameBase->Game->Font->WriteText(GameBase->Game->Renderer, "Roboto-Regular", 34*yscale, Text, Text.length(), 485*xscale, 630*yscale, 0, {255, 255, 255, 255});
		}

		GameBase->Game->Input->Update();

		if (GameBase->Game->Input->Buttons.ButQuit)
		{
			GameBase->Game->GameState = GSQuit;
			GameBase->Game->SubGameState = GSGame;
		}

		if (GameBase->Game->SubGameState == SGGameHelp)
		{
			if ((GameBase->Game->Input->Buttons.ButB && !GameBase->Game->Input->PrevButtons.ButB) ||
				(GameBase->Game->Input->Buttons.ButA && !GameBase->Game->Input->PrevButtons.ButA) ||
				(!GameBase->Game->Input->PrevButtons.ButBack && GameBase->Game->Input->Buttons.ButBack) ||
				(!GameBase->Game->Input->PrevButtons.ButStart && GameBase->Game->Input->Buttons.ButStart))
			{
				GameBase->Game->Audio->PlaySound(GameBase->Game->SfxBack, 0);
				GameBase->Game->SubGameState = SGPauseMenu;
				GameBase->Game->Input->ResetButtons();
			}
		}

		if (GameBase->Game->SubGameState == SGPauseMenu)
		{
			if ((!GameBase->Game->Input->PrevButtons.ButLeft && GameBase->Game->Input->Buttons.ButLeft) ||
				(!GameBase->Game->Input->PrevButtons.ButLeft2 && GameBase->Game->Input->Buttons.ButLeft2) ||
				(!GameBase->Game->Input->PrevButtons.ButDpadLeft && GameBase->Game->Input->Buttons.ButDpadLeft))
			{
				GameBase->Game->Audio->PlaySound(GameBase->Game->SfxSelect, 0);

				switch(selectedmenu)
				{
					case PMSoundVol:
					{
						GameBase->Game->Audio->DecVolumeSound();
						break;
					}

					case PMMusicVol:
					{
						bool wasplaying = GameBase->Game->Audio->IsMusicPlaying();
						GameBase->Game->Audio->DecVolumeMusic();
						if (!wasplaying)
							GameBase->Game->Audio->PlayMusic(GameBase->Game->CurrentGameMusicID, -1);
						break;
					}

				}
			}

			if ((!GameBase->Game->Input->PrevButtons.ButRight && GameBase->Game->Input->Buttons.ButRight) ||
				(!GameBase->Game->Input->PrevButtons.ButRight2 && GameBase->Game->Input->Buttons.ButRight2) ||
				(!GameBase->Game->Input->PrevButtons.ButDpadRight && GameBase->Game->Input->Buttons.ButDpadRight))
			{
				GameBase->Game->Audio->PlaySound(GameBase->Game->SfxSelect, 0);
				switch(selectedmenu)
				{
					case PMSoundVol:
					{
						GameBase->Game->Audio->IncVolumeSound();
						break;
					}

					case PMMusicVol:
					{
						bool wasplaying = GameBase->Game->Audio->IsMusicPlaying();
						GameBase->Game->Audio->IncVolumeMusic();
						if (!wasplaying)
							GameBase->Game->Audio->PlayMusic(GameBase->Game->CurrentGameMusicID, -1);
						break;
					}
				}
			}

			if ((!GameBase->Game->Input->PrevButtons.ButDown && GameBase->Game->Input->Buttons.ButDown) ||
				(!GameBase->Game->Input->PrevButtons.ButDown2 && GameBase->Game->Input->Buttons.ButDown2) ||
				(!GameBase->Game->Input->PrevButtons.ButDpadDown && GameBase->Game->Input->Buttons.ButDpadDown))
			{
				GameBase->Game->Audio->PlaySound(GameBase->Game->SfxSelect, 0);

				selected += 1;
				if (selected == maxmenus)
					selected = 0;
			}

			if ((!GameBase->Game->Input->PrevButtons.ButUp && GameBase->Game->Input->Buttons.ButUp) ||
				(!GameBase->Game->Input->PrevButtons.ButUp2 && GameBase->Game->Input->Buttons.ButUp2) ||
				(!GameBase->Game->Input->PrevButtons.ButDpadUp && GameBase->Game->Input->Buttons.ButDpadUp))
			{
				GameBase->Game->Audio->PlaySound(GameBase->Game->SfxSelect, 0);

				selected -= 1;
				if (selected == -1)
					selected = maxmenus - 1;
			}

			if (!GameBase->Game->Input->PrevButtons.ButBack && GameBase->Game->Input->Buttons.ButBack)
			{
				GameBase->Game->Audio->PlaySound(GameBase->Game->SfxConfirm, 0);

				GameBase->Game->SubGameState = prevsubgamestate;
				GameBase->Game->SubStateCounter = prevsubstatecounter;
			}

			if ((!GameBase->Game->Input->PrevButtons.ButA && GameBase->Game->Input->Buttons.ButA) ||
				(!GameBase->Game->Input->PrevButtons.ButStart && GameBase->Game->Input->Buttons.ButStart))
			{
				GameBase->Game->Audio->PlaySound(GameBase->Game->SfxConfirm, 0);

				switch(selectedmenu)
				{
					case PMContinue:
					{
						//to fix tetris block rotating or dropping
						//when choosing continue with a or a with b pressed
						while (GameBase->Game->Input->Buttons.ButA || GameBase->Game->Input->Buttons.ButB)
							GameBase->Game->Input->Update();

						GameBase->Game->SubGameState = prevsubgamestate;
						GameBase->Game->SubStateCounter = prevsubstatecounter;
						break;
					}

					case PMQuit:
					{
						GameBase->Game->StartCrossFade(GSTitleScreenInit, SGNone, 3, 500);
						break;
					}

					case PMSoundVol:
					{
						GameBase->Game->Audio->IncVolumeSound();
						break;
					}

					case PMMusicVol:
					{
						bool wasplaying = GameBase->Game->Audio->IsMusicPlaying();
						GameBase->Game->Audio->IncVolumeMusic();
						if(!wasplaying)
							GameBase->Game->Audio->PlayMusic(GameBase->Game->CurrentGameMusicID, -1);
						break;
					}

					case PMGameHelp:
					{
						GameBase->Game->SubGameState = SGGameHelp;
						GameBase->Game->Input->ResetButtons();
						break;
					}
				}
			}
		}

		SDL_SetRenderTarget(GameBase->Game->Renderer, GameBase->Game->TexScreen);
		SDL_RenderCopy(GameBase->Game->Renderer, GameBase->Game->TexOffScreen, NULL, NULL);


		SDL_SetRenderTarget(GameBase->Game->Renderer, NULL);
		SDL_SetRenderDrawColor(GameBase->Game->Renderer, 0, 0, 0, 255);
		SDL_RenderClear(GameBase->Game->Renderer);

		int w, h, w2, h2, x, y;
		SDL_GetWindowSize(GameBase->Game->SdlWindow, &w , &h);
		float ScaleX = (float)w / (float)ScreenWidth;
		float ScaleY = (float)h / (float)ScreenHeight;
		h2 = ScreenHeight * ScaleY;
		w2 = ScreenWidth * ScaleY;
		if (w2 > w)
		{
			h2 = ScreenHeight * ScaleX;
			w2 = ScreenWidth * ScaleX;
		}
		x = ((w - w2) / 2);
		y = ((h - h2) / 2);

		SDL_Rect Rect = { x, y, w2, h2};
		SDL_RenderCopy(GameBase->Game->Renderer, GameBase->Game->TexScreen, NULL, &Rect);

		if (debugInfo || GameBase->Game->ShowFPS)
		{
			string Text = "FPS: " + to_string(Fps) + "\n";
			if(debugInfo)
			{
				Text += "FrameTime: " + to_string(AvgFrameTime) + "\n";
				Text += "GFX Slots: " + to_string(GameBase->Game->Image->ImageSlotsUsed()) + "/" + to_string(GameBase->Game->Image->ImageSlotsMax()) + "\n";
				Text += "SND Slots: " + to_string(GameBase->Game->Audio->SoundSlotsUsed()) + "/" + to_string(GameBase->Game->Audio->SoundSlotsMax()) + "\n";
				Text += "MUS Slots: " + to_string(GameBase->Game->Audio->MusicSlotsUsed()) + "/" + to_string(GameBase->Game->Audio->MusicSlotsMax()) + "\n";
				Text += "SPR Slots: " + to_string(GameBase->Game->Sprites->SpriteSlotsUsed()) + "/" + to_string(GameBase->Game->Sprites->SpriteSlotsMax()) + "\n";
			}
			int tw = GameBase->Game->Font->TextWidth("RobotoMono-Bold", 16, Text, Text.length());
			GameBase->Game->Font->WriteText(GameBase->Game->Renderer, "RobotoMono-Bold", 16, Text, Text.length(), w - tw, 0, 0, {255, 0, 255, 255});
		}
		SDL_RenderPresent(GameBase->Game->Renderer);

		Uint64 FrameEndPerf = SDL_GetPerformanceCounter();
		Uint64 FramePerf = FrameEndPerf - FrameStartPerf;
		double FrameTime = FramePerf / (double)SDL_GetPerformanceFrequency() * 1000.0;
		TotalFramePerf += FramePerf;

		if(SDL_GetTicks() - Ticks >= 1000)
		{
			AvgFrameTime = (TotalFramePerf / TotalFrames) / (double)SDL_GetPerformanceFrequency() * 1000.0;
			Fps = TotalFrames;
			TotalFrames = 0;
			TotalFramePerf = 0;
			Ticks = SDL_GetTicks();
		}
		if(DesiredFps > 0)
		{
			int RequiredDelay = 1000.0/DesiredFps - FrameTime;
			if (RequiredDelay > 0)
				SDL_Delay(RequiredDelay);
		}
	}
	GameBase->Game->Input->ResetButtons();
}

void CGameBase_DrawScoreBar(CGameBase* GameBase)
{
	SDL_SetRenderDrawColor(GameBase->Game->Renderer, 0, 0, 0, 255);
	SDL_Rect r = {0, 0, ScreenWidth, ScoreBarHeight};
	SDL_RenderFillRect(GameBase->Game->Renderer, &r);
	string Text = "";
	if(GameBase->UsesLevels)
		Text = "Level: " + to_string(GameBase->level) + " ";

	if (GameBase->Game->GameMode == GMGame)
		Text += "Lives: " + to_string(GameBase->HealthPoints) + " Score:" + to_string(GameBase->Game->Scores[GameBase->Game->Game][GameBase->Game->GameMode]) +
			" High Score: " + to_string(GameBase->Game->HighScores[GameBase->Game->Game][GameBase->Game->GameMode]);
	else
	{
		if(GameBase->Game->GameMode == GMRetroCarousel)
		{
			Text += "Timer: " + to_string_with_precision(GameBase->Game->Timer, 2) + " Total Score:" + to_string(GameBase->Game->RetroCarouselScore +
				GameBase->Game->Scores[GameBase->Game->Game][GameBase->Game->GameMode]) + " Score: " + to_string(GameBase->Game->Scores[GameBase->Game->Game][GameBase->Game->GameMode]) +
				" Previous Total high score: " + to_string(GameBase->Game->RetroCarouselHighScore) +
				" Previous High Score: " + to_string(GameBase->Game->HighScores[GameBase->Game->Game][GameBase->Game->GameMode]);
		}
		else
			Text += "Timer: " + to_string_with_precision(GameBase->Game->Timer, 2) + " Score:" + to_string(GameBase->Game->Scores[GameBase->Game->Game][GameBase->Game->GameMode]) +
				" Previous High Score: " + to_string(GameBase->Game->HighScores[GameBase->Game->Game][GameBase->Game->GameMode]);
	}
	GameBase->Game->Font->WriteText(GameBase->Game->Renderer, "Roboto-Regular", 20*yscale, Text, Text.length(), 0, 0, 0, {255,255,255,255});
}

void CGameBase_DrawSubstateText(CGameBase* GameBase)
{
	//textSize(scpregamefontsize)
	//tz = textWidth(text)
	int w = GameBase->Game->Font->TextWidth("Roboto-Regular", 60*yscale, GameBase->SubStateText, strlen(GameBase->SubStateText));
	GameBase->Game->Font->WriteText(GameBase->Game->Renderer, "Roboto-Regular", 60*yscale, GameBase->SubStateText, strlen(GameBase->SubStateText), GameBase->screenleft + ((GameBase->screenright - GameBase->screenleft) / 2) - w/2,
		GameBase->screentop + ((GameBase->screenbottom - GameBase->screentop) / 2) - 90*xscale, 0, {255, 255, 255, 240});
}

bool CGameBase_UpdateLogic(CGameBase* GameBase)
{
	bool result = false;
	if ((GameBase->Game->Input->Buttons.ButStart && !GameBase->Game->Input->PrevButtons.ButStart) ||
		(GameBase->Game->Input->Buttons.ButBack && !GameBase->Game->Input->PrevButtons.ButBack))
	 	GameBase->PauseMenu(GameBase);

	if (GameBase->Game->GameMode == GMGame)
	{
		if (GameBase->Game->SubGameState == SGGame)
		{
			if(GameBase->HealthPoints == 0)
			{
				GameBase->Game->SubGameState = SGTimeUp;
				GameBase->Game->SubStateTime = SDL_GetTicks() + 750;
				GameBase->Game->SubStateCounter = 0;
			}
		}
	}

	GameBase->SubStateText[0] = '\0';
	if (GameBase->Game->SubGameState != SGFadeIn)
	{
		if ((GameBase->Game->SubGameState == SGReadyGo)||
			(GameBase->Game->SubGameState == SGTimeUp))
		{
			if (GameBase->Game->SubStateTime > SDL_GetTicks())
			{
				if (GameBase->Game->SubStateCounter >= 0)
				{
					char Tmp[50];
					itoa(GameBase->Game->SubStateCounter, Tmp, 10);
					strcpy(GameBase->SubStateText, Tmp);
					if(GameBase->Game->SubStateCounter == 2)
					{
						GameBase->Game->Audio->PlaySound(GameBase->Game->SfxReadyGo, 0);
					}

					if (GameBase->Game->SubStateCounter == 0)
					{
						if (GameBase->Game->SubGameState == SGReadyGo)
						{
							strcpy(GameBase->SubStateText, string("GO").c_str());
						}
						else
						{
							if (GameBase->Game->GameMode != GMGame)
							{
								strcpy(GameBase->SubStateText, string("Time Up").c_str());
							}
							else
								strcpy(GameBase->SubStateText, string("Game Over").c_str());

						}
					}
				}
			}
			else
			{
				if (GameBase->Game->SubStateCounter > 0)
				{
					if (GameBase->Game->SubStateCounter > 1)
						GameBase->Game->SubStateTime = SDL_GetTicks() + 500;
					else
						GameBase->Game->SubStateTime = SDL_GetTicks() + 250;
					GameBase->Game->SubStateCounter -= 1;
				}
				else
				{
					if (GameBase->Game->SubGameState == SGReadyGo)
					{
						result = true;//OnGameStart();
						GameBase->Game->SubGameState = SGGame;
					}
					else
					{
						GameBase->Game->GameState = GSSubScoreInit;
					}
				}
			}
		}
	}
	return result;
}