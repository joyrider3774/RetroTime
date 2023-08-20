#include <SDL.h>
#include <stdlib.h>
#include "CGameBase.h"
#include "../Vec2F.h"

CGameBase* Create_CGameBase(int aGameStateID, bool aUsesLevels)
{
	CGameBase* result = (CGameBase*) malloc(sizeof(CGameBase));
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
	int prevsubgamestate = SubGameState;
	int prevsubstatecounter = SubStateCounter;
	SubGameState = SGPauseMenu;
	CAudio_PlaySound(SfxConfirm, 0);
	//subgamestate = sgframe;
	//global.substatecounter = 10.6
	int selected = 0;
	int selectedmenu = 0;
	int maxmenus = 5;
	int menutextsize = 60*yscale;
	int menuspacing = 85*yscale;
	CInput_ResetButtons();

	Uint64 TotalFrames = 0;
	Uint64 TotalFramePerf = 0;
	Uint32 Fps = 0;
	double AvgFrameTime = 0.0f;
	Uint32 Ticks = SDL_GetTicks();

	while ((SubGameState == SGFrame) || (SubGameState == SGPauseMenu) || (SubGameState == SGGameHelp))
	{
		TotalFrames++;
		Uint64 FrameStartPerf = SDL_GetPerformanceCounter();
		selectedmenu = GPGamePauseMenus[Game].menus[selected];

		//draw everything to offscreen surface

		SDL_SetRenderTarget(Renderer, TexOffScreen);

		// this seems to cause a blackscreen somehow when certain games
		// are paused not sure as to why but i disabled it for now
		// SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 128);
		// SDL_RenderFillRect(Renderer, NULL);

		 //draw everything to offscreen surface
		SDL_SetRenderDrawColor(Renderer, 255,255, 255, 255);
		//so we can can copy the transparant part with the blue and text from this image
		SDL_Point FramePos = {ScreenWidth / 2, ScreenHeight / 2};
		Vec2F FrameScale = {16.0f / 4 * xscale, 12.8f *yscale};
		CImage_DrawImageFuze(Renderer, GFXFrameID, true, &FramePos, 0, &FrameScale, 255, 255, 255, 255);

		if (SubGameState == SGPauseMenu)
		{
			string Text = "Paused";
			CFont_WriteText(Renderer, "Roboto-Regular", 80*yscale, Text, Text.length(), 510*xscale, 50*yscale, 0, {255,255,255,255});
			int menu;
			SDL_Color color;
			for(int i = 0; i < maxmenus; i++)
			{
				menu = GPGamePauseMenus[Game].menus[i];
				if (menu == selectedmenu)
					color = {255, 255, 255, 255};
				else
					color = {255, 255, 255, 64};

				switch(menu)
				{
					case PMSoundVol:
						Text = PMPauseMenus[menu].name + to_string((int)(CAudio_GetVolumeSound()*100/128)) + "%";
						CFont_WriteText(Renderer, "Roboto-Regular", menutextsize, Text, Text.length(), 300*xscale, 185*yscale + i * menuspacing, 0, color);
						break;
					case PMMusicVol:
						Text = PMPauseMenus[menu].name + to_string((int)(CAudio_GetVolumeMusic()*100/128)) + "%";
						CFont_WriteText(Renderer, "Roboto-Regular", menutextsize, Text, Text.length(), 300*xscale, 185*yscale + i * menuspacing, 0, color);
						break;
					default:
						CFont_WriteText(Renderer, "Roboto-Regular", menutextsize, PMPauseMenus[menu].name, PMPauseMenus[menu].name.length(), 300*xscale,
							185*yscale + i * menuspacing, 0, color);
						break;
				}
			}
			Text = "Use dpad to switch between options. (A) to select and (B) for back";
			CFont_WriteText(Renderer, "Roboto-Regular", 34*yscale, Text, Text.length(), 90*xscale, 630*yscale, 0, {255, 255, 255, 255});
		}

		if (SubGameState == SGGameHelp)
		{
			string Text = "Game Help";
			CFont_WriteText(Renderer, "Roboto-Regular", 80*yscale, Text, Text.length(), 485*xscale, 50*yscale, 0, {255,255,255,255});

			Text = GSGames[Game].name;
			CFont_WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 75*xscale, 150*yscale, 0, {255,255,255,255});

			Text = GMModes[GameMode].name + " High Score: " + to_string(HighScores[Game][GameMode]);
			CFont_WriteText(Renderer, "Roboto-Regular", 38*yscale, Text, Text.length(), 75*xscale, 210*yscale, 0, {255,255,255,255});

			Text = GSGames[Game].description;
			CFont_WriteText(Renderer, "Roboto-Regular", 38*yscale, Text, Text.length(), 75*xscale, 255*yscale, 0, {255,255,255,255});

			Text = "Press (A) or (B) for back";
			CFont_WriteText(Renderer, "Roboto-Regular", 34*yscale, Text, Text.length(), 485*xscale, 630*yscale, 0, {255, 255, 255, 255});
		}

		CInput_Update();

		if (CInput_Buttons.ButQuit)
		{
			GameState = GSQuit;
			SubGameState = GSGame;
		}

		if (SubGameState == SGGameHelp)
		{
			if ((CInput_Buttons.ButB && !CInput_PrevButtons.ButB) ||
				(CInput_Buttons.ButA && !CInput_PrevButtons.ButA) ||
				(!CInput_PrevButtons.ButBack && CInput_Buttons.ButBack) ||
				(!CInput_PrevButtons.ButStart && CInput_Buttons.ButStart))
			{
				CAudio_PlaySound(SfxBack, 0);
				SubGameState = SGPauseMenu;
				CInput_ResetButtons();
			}
		}

		if (SubGameState == SGPauseMenu)
		{
			if ((!CInput_PrevButtons.ButLeft && CInput_Buttons.ButLeft) ||
				(!CInput_PrevButtons.ButLeft2 && CInput_Buttons.ButLeft2) ||
				(!CInput_PrevButtons.ButDpadLeft && CInput_Buttons.ButDpadLeft))
			{
				CAudio_PlaySound(SfxSelect, 0);

				switch(selectedmenu)
				{
					case PMSoundVol:
					{
						CAudio_DecVolumeSound();
						break;
					}

					case PMMusicVol:
					{
						bool wasplaying = CAudio_IsMusicPlaying();
						CAudio_DecVolumeMusic();
						if (!wasplaying)
							CAudio_PlayMusic(CurrentGameMusicID, -1);
						break;
					}

				}
			}

			if ((!CInput_PrevButtons.ButRight && CInput_Buttons.ButRight) ||
				(!CInput_PrevButtons.ButRight2 && CInput_Buttons.ButRight2) ||
				(!CInput_PrevButtons.ButDpadRight && CInput_Buttons.ButDpadRight))
			{
				CAudio_PlaySound(SfxSelect, 0);
				switch(selectedmenu)
				{
					case PMSoundVol:
					{
						CAudio_IncVolumeSound();
						break;
					}

					case PMMusicVol:
					{
						bool wasplaying = CAudio_IsMusicPlaying();
						CAudio_IncVolumeMusic();
						if (!wasplaying)
							CAudio_PlayMusic(CurrentGameMusicID, -1);
						break;
					}
				}
			}

			if ((!CInput_PrevButtons.ButDown && CInput_Buttons.ButDown) ||
				(!CInput_PrevButtons.ButDown2 && CInput_Buttons.ButDown2) ||
				(!CInput_PrevButtons.ButDpadDown && CInput_Buttons.ButDpadDown))
			{
				CAudio_PlaySound(SfxSelect, 0);

				selected += 1;
				if (selected == maxmenus)
					selected = 0;
			}

			if ((!CInput_PrevButtons.ButUp && CInput_Buttons.ButUp) ||
				(!CInput_PrevButtons.ButUp2 && CInput_Buttons.ButUp2) ||
				(!CInput_PrevButtons.ButDpadUp && CInput_Buttons.ButDpadUp))
			{
				CAudio_PlaySound(SfxSelect, 0);

				selected -= 1;
				if (selected == -1)
					selected = maxmenus - 1;
			}

			if (!CInput_PrevButtons.ButBack && CInput_Buttons.ButBack)
			{
				CAudio_PlaySound(SfxConfirm, 0);

				SubGameState = prevsubgamestate;
				SubStateCounter = prevsubstatecounter;
			}

			if ((!CInput_PrevButtons.ButA && CInput_Buttons.ButA) ||
				(!CInput_PrevButtons.ButStart && CInput_Buttons.ButStart))
			{
				CAudio_PlaySound(SfxConfirm, 0);

				switch(selectedmenu)
				{
					case PMContinue:
					{
						//to fix tetris block rotating or dropping
						//when choosing continue with a or a with b pressed
						while (CInput_Buttons.ButA || CInput_Buttons.ButB)
							CInput_Update();

						SubGameState = prevsubgamestate;
						SubStateCounter = prevsubstatecounter;
						break;
					}

					case PMQuit:
					{
						CGame_StartCrossFade(GSTitleScreenInit, SGNone, 3, 500);
						break;
					}

					case PMSoundVol:
					{
						CAudio_IncVolumeSound();
						break;
					}

					case PMMusicVol:
					{
						bool wasplaying = CAudio_IsMusicPlaying();
						CAudio_IncVolumeMusic();
						if(!wasplaying)
							CAudio_PlayMusic(CurrentGameMusicID, -1);
						break;
					}

					case PMGameHelp:
					{
						SubGameState = SGGameHelp;
						CInput_ResetButtons();
						break;
					}
				}
			}
		}

		SDL_SetRenderTarget(Renderer, TexScreen);
		SDL_RenderCopy(Renderer, TexOffScreen, NULL, NULL);


		SDL_SetRenderTarget(Renderer, NULL);
		SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
		SDL_RenderClear(Renderer);

		int w, h, w2, h2, x, y;
		SDL_GetWindowSize(SdlWindow, &w , &h);
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
		SDL_RenderCopy(Renderer, TexScreen, NULL, &Rect);

		if (debugInfo || ShowFPS)
		{
			string Text = "FPS: " + to_string(Fps) + "\n";
			if(debugInfo)
			{
				Text += "FrameTime: " + to_string(AvgFrameTime) + "\n";
				Text += "GFX Slots: " + to_string(CImage_ImageSlotsUsed()) + "/" + to_string(CImage_ImageSlotsMax()) + "\n";
				Text += "SND Slots: " + to_string(CAudio_SoundSlotsUsed()) + "/" + to_string(CAudio_SoundSlotsMax()) + "\n";
				Text += "MUS Slots: " + to_string(CAudio_MusicSlotsUsed()) + "/" + to_string(CAudio_MusicSlotsMax()) + "\n";
				Text += "SPR Slots: " + to_string(Sprites->SpriteSlotsUsed()) + "/" + to_string(Sprites->SpriteSlotsMax()) + "\n";
			}
			int tw = CFont_TextWidth("RobotoMono-Bold", 16, Text, Text.length());
			CFont_WriteText(Renderer, "RobotoMono-Bold", 16, Text, Text.length(), w - tw, 0, 0, {255, 0, 255, 255});
		}
		SDL_RenderPresent(Renderer);

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
	CInput_ResetButtons();
}

void CGameBase_DrawScoreBar(CGameBase* GameBase)
{
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	SDL_Rect r = {0, 0, ScreenWidth, ScoreBarHeight};
	SDL_RenderFillRect(Renderer, &r);
	string Text = "";
	if(GameBase->UsesLevels)
		Text = "Level: " + to_string(GameBase->level) + " ";

	if (GameMode == GMGame)
		Text += "Lives: " + to_string(GameBase->HealthPoints) + " Score:" + to_string(Scores[Game][GameMode]) +
			" High Score: " + to_string(HighScores[Game][GameMode]);
	else
	{
		if(GameMode == GMRetroCarousel)
		{
			Text += "Timer: " + to_string_with_precision(Timer, 2) + " Total Score:" + to_string(RetroCarouselScore +
				Scores[Game][GameMode]) + " Score: " + to_string(Scores[Game][GameMode]) +
				" Previous Total high score: " + to_string(RetroCarouselHighScore) +
				" Previous High Score: " + to_string(HighScores[Game][GameMode]);
		}
		else
			Text += "Timer: " + to_string_with_precision(Timer, 2) + " Score:" + to_string(Scores[Game][GameMode]) +
				" Previous High Score: " + to_string(HighScores[Game][GameMode]);
	}
	CFont_WriteText(Renderer, "Roboto-Regular", 20*yscale, Text, Text.length(), 0, 0, 0, {255,255,255,255});
}

void CGameBase_DrawSubstateText(CGameBase* GameBase)
{
	//textSize(scpregamefontsize)
	//tz = textWidth(text)
	int w = CFont_TextWidth("Roboto-Regular", 60*yscale, GameBase->SubStateText, strlen(GameBase->SubStateText));
	CFont_WriteText(Renderer, "Roboto-Regular", 60*yscale, GameBase->SubStateText, strlen(GameBase->SubStateText), GameBase->screenleft + ((GameBase->screenright - GameBase->screenleft) / 2) - w/2,
		GameBase->screentop + ((GameBase->screenbottom - GameBase->screentop) / 2) - 90*xscale, 0, {255, 255, 255, 240});
}

bool CGameBase_UpdateLogic(CGameBase* GameBase)
{
	bool result = false;
	if ((CInput_Buttons.ButStart && !CInput_PrevButtons.ButStart) ||
		(CInput_Buttons.ButBack && !CInput_PrevButtons.ButBack))
	 	GameBase->PauseMenu(GameBase);

	if (GameMode == GMGame)
	{
		if (SubGameState == SGGame)
		{
			if(GameBase->HealthPoints == 0)
			{
				SubGameState = SGTimeUp;
				SubStateTime = SDL_GetTicks() + 750;
				SubStateCounter = 0;
			}
		}
	}

	GameBase->SubStateText[0] = '\0';
	if (SubGameState != SGFadeIn)
	{
		if ((SubGameState == SGReadyGo)||
			(SubGameState == SGTimeUp))
		{
			if (SubStateTime > SDL_GetTicks())
			{
				if (SubStateCounter >= 0)
				{
					char Tmp[50];
					itoa(SubStateCounter, Tmp, 10);
					strcpy(GameBase->SubStateText, Tmp);
					if(SubStateCounter == 2)
					{
						CAudio_PlaySound(SfxReadyGo, 0);
					}

					if (SubStateCounter == 0)
					{
						if (SubGameState == SGReadyGo)
						{
							strcpy(GameBase->SubStateText, string("GO").c_str());
						}
						else
						{
							if (GameMode != GMGame)
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
				if (SubStateCounter > 0)
				{
					if (SubStateCounter > 1)
						SubStateTime = SDL_GetTicks() + 500;
					else
						SubStateTime = SDL_GetTicks() + 250;
					SubStateCounter -= 1;
				}
				else
				{
					if (SubGameState == SGReadyGo)
					{
						result = true;//OnGameStart();
						SubGameState = SGGame;
					}
					else
					{
						GameState = GSSubScoreInit;
					}
				}
			}
		}
	}
	return result;
}