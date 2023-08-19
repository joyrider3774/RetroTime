#include <SDL.h>
#include "CGameBase.h"
#include "../Vec2F.h"

CGameBase::CGameBase(CGame *aGame, int aGameStateID, bool aUsesLevels, bool aScreenshotMode)
{
	Game = aGame;
	ScreenshotMode = aScreenshotMode;
	UsesLevels = aUsesLevels;
	GameStateID = aGameStateID;
	level = 0;
	HealthPoints = 0;
}

CGameBase::~CGameBase()
{

}

void CGameBase::init()
{

}

void CGameBase::deinit()
{

}

void CGameBase::LoadGraphics()
{

}

void CGameBase::UnloadGraphics()
{

}

void CGameBase::LoadSound()
{

}

void CGameBase::UnLoadSound()
{

}

void CGameBase::UpdateObjects(bool IsGameState)
{

}

void CGameBase::PauseMenu()
{
	int prevsubgamestate = Game->SubGameState;
	int prevsubstatecounter = Game->SubStateCounter;
	Game->SubGameState = SGPauseMenu;
	Game->Audio->PlaySound(Game->SfxConfirm, 0);
	//subgamestate = sgframe;
	//global.substatecounter = 10.6
	int selected = 0;
	int selectedmenu = 0;
	int maxmenus = 5;
	int menutextsize = 60*yscale;
	int menuspacing = 85*yscale;
	Game->Input->ResetButtons();

	Uint64 TotalFrames = 0;
	Uint64 TotalFramePerf = 0;
	Uint32 Fps = 0;
	double AvgFrameTime = 0.0f;
	Uint32 Ticks = SDL_GetTicks();

	while ((Game->SubGameState == SGFrame) || (Game->SubGameState == SGPauseMenu) || (Game->SubGameState == SGGameHelp))
	{
		TotalFrames++;
		Uint64 FrameStartPerf = SDL_GetPerformanceCounter();
		selectedmenu = GPGamePauseMenus[Game->Game].menus[selected];

		//draw everything to offscreen surface

		SDL_SetRenderTarget(Game->Renderer, Game->TexOffScreen);
		Draw(); //game

		// this seems to cause a blackscreen somehow when certain games
		// are paused not sure as to why but i disabled it for now
		// SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 128);
		// SDL_RenderFillRect(Game->Renderer, NULL);

		 //draw everything to offscreen surface
		SDL_SetRenderDrawColor(Game->Renderer, 255,255, 255, 255);
		//so we can can copy the transparant part with the blue and text from this image
		SDL_Point FramePos = {ScreenWidth / 2, ScreenHeight / 2};
		Vec2F FrameScale = {16.0f / 4 * xscale, 12.8f *yscale};
		Game->Image->DrawImageFuze(Game->Renderer, Game->GFXFrameID, true, &FramePos, 0, &FrameScale, 255, 255, 255, 255);

		if (Game->SubGameState == SGPauseMenu)
		{
			string Text = "Paused";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 80*yscale, Text, Text.length(), 510*xscale, 50*yscale, 0, {255,255,255,255});
			int menu;
			SDL_Color color;
			for(int i = 0; i < maxmenus; i++)
			{
				menu = GPGamePauseMenus[Game->Game].menus[i];
				if (menu == selectedmenu)
					color = {255, 255, 255, 255};
				else
					color = {255, 255, 255, 64};

				switch(menu)
				{
					case PMSoundVol:
						Text = PMPauseMenus[menu].name + to_string((int)(Game->Audio->GetVolumeSound()*100/128)) + "%";
						Game->Font->WriteText(Game->Renderer, "Roboto-Regular", menutextsize, Text, Text.length(), 300*xscale, 185*yscale + i * menuspacing, 0, color);
						break;
					case PMMusicVol:
						Text = PMPauseMenus[menu].name + to_string((int)(Game->Audio->GetVolumeMusic()*100/128)) + "%";
						Game->Font->WriteText(Game->Renderer, "Roboto-Regular", menutextsize, Text, Text.length(), 300*xscale, 185*yscale + i * menuspacing, 0, color);
						break;
					default:
						Game->Font->WriteText(Game->Renderer, "Roboto-Regular", menutextsize, PMPauseMenus[menu].name, PMPauseMenus[menu].name.length(), 300*xscale,
							185*yscale + i * menuspacing, 0, color);
						break;
				}
			}
			Text = "Use dpad to switch between options. (A) to select and (B) for back";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 34*yscale, Text, Text.length(), 90*xscale, 630*yscale, 0, {255, 255, 255, 255});
		}

		if (Game->SubGameState == SGGameHelp)
		{
			string Text = "Game Help";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 80*yscale, Text, Text.length(), 485*xscale, 50*yscale, 0, {255,255,255,255});

			Text = GSGames[Game->Game].name;
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 75*xscale, 150*yscale, 0, {255,255,255,255});

			Text = GMModes[Game->GameMode].name + " High Score: " + to_string(Game->HighScores[Game->Game][Game->GameMode]);
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 38*yscale, Text, Text.length(), 75*xscale, 210*yscale, 0, {255,255,255,255});

			Text = GSGames[Game->Game].description;
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 38*yscale, Text, Text.length(), 75*xscale, 255*yscale, 0, {255,255,255,255});

			Text = "Press (A) or (B) for back";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 34*yscale, Text, Text.length(), 485*xscale, 630*yscale, 0, {255, 255, 255, 255});
		}

		Game->Input->Update();

		if (Game->Input->Buttons.ButQuit)
		{
			Game->GameState = GSQuit;
			Game->SubGameState = GSGame;
		}

		//Need to recreate screenshots and background
		if(Game->Input->Buttons.RenderReset)
		{
			SDL_Log("Render Reset, Recreating crt and background, Reloading Game Graphics");
			Game->Image->UnloadImages();
			LoadGraphics();
			Game->LoadGraphics();
			Game->CreateScreenshotsAndBackground();
			Game->ReCreateCrt();
		}

		if (Game->SubGameState == SGGameHelp)
		{
			if ((Game->Input->Buttons.ButB && !Game->Input->PrevButtons.ButB) ||
				(Game->Input->Buttons.ButA && !Game->Input->PrevButtons.ButA) ||
				(!Game->Input->PrevButtons.ButBack && Game->Input->Buttons.ButBack) ||
				(!Game->Input->PrevButtons.ButStart && Game->Input->Buttons.ButStart))
			{
				Game->Audio->PlaySound(Game->SfxBack, 0);
				Game->SubGameState = SGPauseMenu;
				Game->Input->ResetButtons();
			}
		}

		if (Game->SubGameState == SGPauseMenu)
		{
			if ((!Game->Input->PrevButtons.ButLeft && Game->Input->Buttons.ButLeft) ||
				(!Game->Input->PrevButtons.ButLeft2 && Game->Input->Buttons.ButLeft2) ||
				(!Game->Input->PrevButtons.ButDpadLeft && Game->Input->Buttons.ButDpadLeft))
			{
				Game->Audio->PlaySound(Game->SfxSelect, 0);

				switch(selectedmenu)
				{
					case PMSoundVol:
					{
						Game->Audio->DecVolumeSound();
						break;
					}

					case PMMusicVol:
					{
						bool wasplaying = Game->Audio->IsMusicPlaying();
						Game->Audio->DecVolumeMusic();
						if (!wasplaying)
							Game->Audio->PlayMusic(Game->CurrentGameMusicID, -1);
						break;
					}

				}
			}

			if ((!Game->Input->PrevButtons.ButRight && Game->Input->Buttons.ButRight) ||
				(!Game->Input->PrevButtons.ButRight2 && Game->Input->Buttons.ButRight2) ||
				(!Game->Input->PrevButtons.ButDpadRight && Game->Input->Buttons.ButDpadRight))
			{
				Game->Audio->PlaySound(Game->SfxSelect, 0);
				switch(selectedmenu)
				{
					case PMSoundVol:
					{
						Game->Audio->IncVolumeSound();
						break;
					}

					case PMMusicVol:
					{
						bool wasplaying = Game->Audio->IsMusicPlaying();
						Game->Audio->IncVolumeMusic();
						if (!wasplaying)
							Game->Audio->PlayMusic(Game->CurrentGameMusicID, -1);
						break;
					}
				}
			}

			if ((!Game->Input->PrevButtons.ButDown && Game->Input->Buttons.ButDown) ||
				(!Game->Input->PrevButtons.ButDown2 && Game->Input->Buttons.ButDown2) ||
				(!Game->Input->PrevButtons.ButDpadDown && Game->Input->Buttons.ButDpadDown))
			{
				Game->Audio->PlaySound(Game->SfxSelect, 0);

				selected += 1;
				if (selected == maxmenus)
					selected = 0;
			}

			if ((!Game->Input->PrevButtons.ButUp && Game->Input->Buttons.ButUp) ||
				(!Game->Input->PrevButtons.ButUp2 && Game->Input->Buttons.ButUp2) ||
				(!Game->Input->PrevButtons.ButDpadUp && Game->Input->Buttons.ButDpadUp))
			{
				Game->Audio->PlaySound(Game->SfxSelect, 0);

				selected -= 1;
				if (selected == -1)
					selected = maxmenus - 1;
			}

			if (!Game->Input->PrevButtons.ButBack && Game->Input->Buttons.ButBack)
			{
				Game->Audio->PlaySound(Game->SfxConfirm, 0);

				Game->SubGameState = prevsubgamestate;
				Game->SubStateCounter = prevsubstatecounter;
			}

			if ((!Game->Input->PrevButtons.ButA && Game->Input->Buttons.ButA) ||
				(!Game->Input->PrevButtons.ButStart && Game->Input->Buttons.ButStart))
			{
				Game->Audio->PlaySound(Game->SfxConfirm, 0);

				switch(selectedmenu)
				{
					case PMContinue:
					{
						//to fix tetris block rotating or dropping
						//when choosing continue with a or a with b pressed
						while (Game->Input->Buttons.ButA || Game->Input->Buttons.ButB)
							Game->Input->Update();

						Game->SubGameState = prevsubgamestate;
						Game->SubStateCounter = prevsubstatecounter;
						break;
					}

					case PMQuit:
					{
						Game->StartCrossFade(GSTitleScreenInit, SGNone, 3, 500);
						break;
					}

					case PMSoundVol:
					{
						Game->Audio->IncVolumeSound();
						break;
					}

					case PMMusicVol:
					{
						bool wasplaying = Game->Audio->IsMusicPlaying();
						Game->Audio->IncVolumeMusic();
						if(!wasplaying)
							Game->Audio->PlayMusic(Game->CurrentGameMusicID, -1);
						break;
					}

					case PMGameHelp:
					{
						Game->SubGameState = SGGameHelp;
						Game->Input->ResetButtons();
						break;
					}
				}
			}
		}

		SDL_SetRenderTarget(Game->Renderer, Game->TexScreen);
		SDL_RenderCopy(Game->Renderer, Game->TexOffScreen, NULL, NULL);


		SDL_SetRenderTarget(Game->Renderer, NULL);
		SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 255);
		SDL_RenderClear(Game->Renderer);

		int w, h, w2, h2, x, y;
		SDL_GetWindowSize(Game->SdlWindow, &w , &h);
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
		SDL_RenderCopy(Game->Renderer, Game->TexScreen, NULL, &Rect);

		if (debugInfo || Game->ShowFPS)
		{
			string Text = "FPS: " + to_string(Fps) + "\n";
			if(debugInfo)
			{
				Text += "FrameTime: " + to_string(AvgFrameTime) + "\n";
				Text += "GFX Slots: " + to_string(Game->Image->ImageSlotsUsed()) + "/" + to_string(Game->Image->ImageSlotsMax()) + "\n";
				Text += "SND Slots: " + to_string(Game->Audio->SoundSlotsUsed()) + "/" + to_string(Game->Audio->SoundSlotsMax()) + "\n";
				Text += "MUS Slots: " + to_string(Game->Audio->MusicSlotsUsed()) + "/" + to_string(Game->Audio->MusicSlotsMax()) + "\n";
				Text += "SPR Slots: " + to_string(Game->Sprites->SpriteSlotsUsed()) + "/" + to_string(Game->Sprites->SpriteSlotsMax()) + "\n";
			}
			int tw = Game->Font->TextWidth("RobotoMono-Bold", 16, Text, Text.length());
			Game->Font->WriteText(Game->Renderer, "RobotoMono-Bold", 16, Text, Text.length(), w - tw, 0, 0, {255, 0, 255, 255});
		}
		SDL_RenderPresent(Game->Renderer);

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
	Game->Input->ResetButtons();
}

void CGameBase::DrawScoreBar()
{
	SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 255);
	SDL_Rect r = {0, 0, ScreenWidth, ScoreBarHeight};
	SDL_RenderFillRect(Game->Renderer, &r);
	string Text = "";
	if(UsesLevels)
		Text = "Level: " + to_string(level) + " ";

	if (Game->GameMode == GMGame)
		Text += "Lives: " + to_string(HealthPoints) + " Score:" + to_string(Game->Scores[Game->Game][Game->GameMode]) +
			" High Score: " + to_string(Game->HighScores[Game->Game][Game->GameMode]);
	else
	{
		if(Game->GameMode == GMRetroCarousel)
		{
			Text += "Timer: " + to_string_with_precision(Game->Timer, 2) + " Total Score:" + to_string(Game->RetroCarouselScore +
				Game->Scores[Game->Game][Game->GameMode]) + " Score: " + to_string(Game->Scores[Game->Game][Game->GameMode]) +
				" Previous Total high score: " + to_string(Game->RetroCarouselHighScore) +
				" Previous High Score: " + to_string(Game->HighScores[Game->Game][Game->GameMode]);
		}
		else
			Text += "Timer: " + to_string_with_precision(Game->Timer, 2) + " Score:" + to_string(Game->Scores[Game->Game][Game->GameMode]) +
				" Previous High Score: " + to_string(Game->HighScores[Game->Game][Game->GameMode]);
	}
	Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 20*yscale, Text, Text.length(), 0, 0, 0, {255,255,255,255});
}

void CGameBase::OnGameStart()
{

}

void CGameBase::DrawSubStateText()
{
	//textSize(scpregamefontsize)
	//tz = textWidth(text)
	int w = Game->Font->TextWidth("Roboto-Regular", 60*yscale, SubStateText, SubStateText.length());
	Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 60*yscale, SubStateText, SubStateText.length(), screenleft + ((screenright - screenleft) / 2) - w/2,
		screentop + ((screenbottom - screentop) / 2) - 90*xscale, 0, {255, 255, 255, 240});
}

bool CGameBase::UpdateLogic()
{
	bool result = false;
	if ((Game->Input->Buttons.ButStart && !Game->Input->PrevButtons.ButStart) ||
		(Game->Input->Buttons.ButBack && !Game->Input->PrevButtons.ButBack))
	 	PauseMenu();

	if (Game->GameMode == GMGame)
	{
		if (Game->SubGameState == SGGame)
		{
			if(HealthPoints == 0)
			{
				Game->SubGameState = SGTimeUp;
				Game->SubStateTime = SDL_GetTicks() + 750;
				Game->SubStateCounter = 0;
			}
		}
	}

	SubStateText = "";
	if (Game->SubGameState != SGFadeIn)
	{
		if ((Game->SubGameState == SGReadyGo)||
			(Game->SubGameState == SGTimeUp))
		{
			if (Game->SubStateTime > SDL_GetTicks())
			{
				if (Game->SubStateCounter >= 0)
				{
					SubStateText = to_string((int)Game->SubStateCounter);
					if(Game->SubStateCounter == 2)
					{
						Game->Audio->PlaySound(Game->SfxReadyGo, 0);
					}

					if (Game->SubStateCounter == 0)
					{
						if (Game->SubGameState == SGReadyGo)
						{
							SubStateText = "GO";
						}
						else
						{
							if (Game->GameMode != GMGame)
							{
								SubStateText = "Time Up";
							}
							else
								SubStateText = "Game Over";

						}
					}
				}
			}
			else
			{
				if (Game->SubStateCounter > 0)
				{
					if (Game->SubStateCounter > 1)
						Game->SubStateTime = SDL_GetTicks() + 500;
					else
						Game->SubStateTime = SDL_GetTicks() + 250;
					Game->SubStateCounter -= 1;
				}
				else
				{
					if (Game->SubGameState == SGReadyGo)
					{
						result = true;//OnGameStart();
						Game->SubGameState = SGGame;
					}
					else
					{
						Game->GameState = GSSubScoreInit;
					}
				}
			}
		}
	}
	return result;
}

void CGameBase::Draw()
{
	DrawBackground((Game->SubGameState == SGGame) && !ScreenshotMode);
	if (DrawObjects())
		Game->Sprites->DrawSprites(Game->Renderer);
	if(!ScreenshotMode)
	{
		DrawScoreBar();
		DrawSubStateText();
	}
}

bool CGameBase::DrawObjects()
{
	//call drawsprites
	return true;
}

void CGameBase::DrawBackground(bool motionblur)
{

}

SDL_Texture* CGameBase::screenshot()
{
	return nullptr;
}