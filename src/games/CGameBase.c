#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
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
		char Text[2000];
		SDL_Color color = {255,255,255,255};
		if (SubGameState == SGPauseMenu)
		{
			
			sprintf(Text, "Paused");
			CFont_WriteText(Renderer, "Roboto-Regular", 80*yscale, Text, strlen(Text), 510*xscale, 50*yscale, 0, color);
			int menu;
			
			for(int i = 0; i < maxmenus; i++)
			{
				menu = GPGamePauseMenus[Game].menus[i];
				if (menu == selectedmenu)
					color.a =  255;
				else
					color.a = 64;

				switch(menu)
				{
					case PMSoundVol:
						sprintf(Text, "%s %d %%", PMPauseMenus[menu].name, (int)(CAudio_GetVolumeSound()*100/128));
						CFont_WriteText(Renderer, "Roboto-Regular", menutextsize, Text, strlen(Text), 300*xscale, 185*yscale + i * menuspacing, 0, color);
						break;
					case PMMusicVol:
						sprintf(Text, "%s %d %%", PMPauseMenus[menu].name, (int)(CAudio_GetVolumeMusic()*100/128));
						CFont_WriteText(Renderer, "Roboto-Regular", menutextsize, Text, strlen(Text), 300*xscale, 185*yscale + i * menuspacing, 0, color);
						break;
					default:
						CFont_WriteText(Renderer, "Roboto-Regular", menutextsize, PMPauseMenus[menu].name, strlen(PMPauseMenus[menu].name), 300*xscale,
							185*yscale + i * menuspacing, 0, color);
						break;
				}
			}
			
			color.a = 255;
			sprintf(Text, "Use dpad to switch between options. (A) to select and (B) for back");
			CFont_WriteText(Renderer, "Roboto-Regular", 34*yscale, Text, strlen(Text), 90*xscale, 630*yscale, 0, color);
		}

		if (SubGameState == SGGameHelp)
		{
			sprintf(Text, "Game Help");
			CFont_WriteText(Renderer, "Roboto-Regular", 80*yscale, Text, strlen(Text), 485*xscale, 50*yscale, 0, color);

			sprintf(Text, "%s", GSGames[Game].name);
			CFont_WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, strlen(Text), 75*xscale, 150*yscale, 0, color);

			sprintf(Text, "%s %s High Score: %llu", GSGames[Game].name,  GMModes[GameMode].name, HighScores[Game][GameMode]);
			CFont_WriteText(Renderer, "Roboto-Regular", 38*yscale, Text, strlen(Text), 75*xscale, 210*yscale, 0, color);

			sprintf(Text, "%s", GSGames[Game].description);
			CFont_WriteText(Renderer, "Roboto-Regular", 38*yscale, Text, strlen(Text), 75*xscale, 255*yscale, 0, color);

			sprintf(Text, "Press (A) or (B) for back");
			CFont_WriteText(Renderer, "Roboto-Regular", 34*yscale, Text, strlen(Text), 485*xscale, 630*yscale, 0, color);
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
			char Text[500];
			char TmpText[100];
			Text[0] = '\0';
			sprintf(TmpText, "FPS: %d\n", Fps);
			strcat(Text, TmpText);
			if(debugInfo)
			{
				sprintf(TmpText,"FrameTime: %f.5\n",AvgFrameTime);
				strncat(Text, TmpText, 100);
				sprintf(TmpText,"GFX Slots: %d/%d\n",CImage_ImageSlotsUsed(), CImage_ImageSlotsMax());
				strncat(Text, TmpText, 100);
				sprintf(TmpText,"SND Slots: %d/%d\n",CAudio_SoundSlotsUsed(),CAudio_SoundSlotsMax());
				strncat(Text, TmpText, 100);
				sprintf(TmpText,"MUS Slots: %d/%d\n",CAudio_MusicSlotsUsed(), CAudio_MusicSlotsMax());
				strncat(Text, TmpText, 100);
				sprintf(TmpText,"SPR Slots: %d/%d\n",CSprites_SpriteSlotsUsed(),CSprites_SpriteSlotsMax());
				strncat(Text, TmpText, 100);
				sprintf(TmpText,"SPR Resets: %d\n",CSprites_UpdateImageResetsCount());
				strncat(Text, TmpText, 100);
				sprintf(TmpText,"SPR Draws: %d\n", CSprites_SpritesDrawnCount());
				strncat(Text, TmpText, 100);
				sprintf(TmpText,"SCL Loaded: %d/%d\n", CImage_ScaledImagesLoadedCount(),CImage_ScaledImagesLoadedMax());
				strncat(Text, TmpText, 100);				
			}
			int tw = CFont_TextWidth("RobotoMono-Bold", 16, Text, strlen(Text));
			color.a = 255;
			color.r = 255;
			color.g = 0;
			color.b = 255;
			CFont_WriteText(Renderer, "RobotoMono-Bold", 16, Text, strlen(Text), w - tw, 0, 0, color);
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
	char Text[2000];
	char TmpText[1000];
	Text[0] = '\0';
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	SDL_Rect r = {0, 0, ScreenWidth, ScoreBarHeight};
	SDL_RenderFillRect(Renderer, &r);
	
	if(GameBase->UsesLevels)
	{
		sprintf(TmpText,"Level: %d ",GameBase->level);
		strncat(Text, TmpText, 1000);
	}

	if (GameMode == GMGame)
	{
		sprintf(TmpText,"Lives:%d Score:%llu High Score:%llu ",  GameBase->HealthPoints,
			Scores[Game][GameMode], HighScores[Game][GameMode]);
		strncat(Text, TmpText, 1000);
	}
	else
	{
		if(GameMode == GMRetroCarousel)
		{
			sprintf(TmpText,"Timer: %.2f Total Score:%llu  Score:%llu Previous Total high score:%llu Previous High Score:%llu", 
				Timer, RetroCarouselScore, Scores[Game][GameMode], RetroCarouselHighScore,
				HighScores[Game][GameMode]);
			strncat(Text, TmpText, 1000);
		}
		else
		{
			sprintf(TmpText, "Timer: %.2f Score:%llu Previous High Score:%llu ",
				Timer,Scores[Game][GameMode], HighScores[Game][GameMode]);
			strncat(Text, TmpText, 1000);
		}
			
	}
	SDL_Color color = {255,255,255,255};
	CFont_WriteText(Renderer, "Roboto-Regular", 20*yscale, Text, strlen(Text), 0, 0, 0, color);
}

void CGameBase_DrawSubstateText(CGameBase* GameBase)
{
	//textSize(scpregamefontsize)
	//tz = textWidth(text)
	SDL_Color color = {255,255,255,240};
	int w = CFont_TextWidth("Roboto-Regular", 60*yscale, GameBase->SubStateText, strlen(GameBase->SubStateText));
	CFont_WriteText(Renderer, "Roboto-Regular", 60*yscale, GameBase->SubStateText, strlen(GameBase->SubStateText), GameBase->screenleft + ((GameBase->screenright - GameBase->screenleft) / 2) - w/2,
		GameBase->screentop + ((GameBase->screenbottom - GameBase->screentop) / 2) - 90*xscale, 0, color);
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
					sprintf(Tmp,"%d",(int)SubStateCounter);
					strcpy(GameBase->SubStateText, Tmp);
					if(SubStateCounter == 2)
					{
						CAudio_PlaySound(SfxReadyGo, 0);
					}

					if (SubStateCounter == 0)
					{
						if (SubGameState == SGReadyGo)
						{
							strcpy(GameBase->SubStateText, "GO");
						}
						else
						{
							if (GameMode != GMGame)
							{
								strcpy(GameBase->SubStateText, "Time Up");
							}
							else
								strcpy(GameBase->SubStateText, "Game Over");

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