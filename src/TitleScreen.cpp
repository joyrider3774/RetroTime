#include <SDL.h>
#include <string.h>
#include "CGame.h"
#include "Common.h"
#include "TitleScreen.h"
#include "Vec2F.h"

int SelectedMenu = 0;
int CurrentMainMenu = -1;
int SelMenu = 0;
int SelOptions = 0;

constexpr int menutextsize = 60*yscale;
constexpr int menuspacing = 85*yscale;
constexpr int rcolor = 64;
constexpr int rdcolor = 1;

void InitTitleScreen()
{
	Audio->PlayMusic(MusMenu, -1);
	CGame_StartCrossFade(GameState, SGNone, 0, 0);
}

void TitleScreen()
{
	if(GameState == GSTitleScreenInit)
	{
		InitTitleScreen();
		GameState -= initDiff;
	}
	//draw everything to offscreen surface
	CGame_DrawTitleBackground();
	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 235);
	//so we can can copy the transparant part with the blue and text from this image
	SDL_Point FramePos = {ScreenWidth / 2, ScreenHeight / 2};
	Vec2F FrameScale = {16.0f / 4 * xscale, 12.8f *yscale};
	CImage_DrawImageFuze(Renderer, GFXFrameID, true, &FramePos, 0, &FrameScale, 255, 255, 255, 255);

	string Text = "";
	switch (CurrentMainMenu)
	{
		case MMOptions:
		{
			int selectedmenu = 0;
			//int menutextsize = (int)(35.0f*yscale);
			//int menuspacing = (int)(45.0f*yscale);

			selectedmenu = OMOptionMenus[SelOptions].menu;
			Text = "Options";
			Font->WriteText(Renderer, "Roboto-Regular", 80*yscale, Text, Text.length(), 525*xscale, 50*yscale, 0, {255, 255, 255, 255});

			int menu;
			SDL_Color color;
			for(int i = 0; i < OptionMenus; i++)
			{
				menu = OMOptionMenus[i].menu;
				if(menu == selectedmenu)
					color = {255, 255, 255, 255};
				else
					color = {255, 255, 255, 64};

				switch(menu)
				{
					case OMSoundVol:
						Text = OMOptionMenus[menu].name + to_string((int)(Audio->GetVolumeSound()*100/128)) + "%";
						Font->WriteText(Renderer, "Roboto-Regular", menutextsize, Text, Text.length(), 300*xscale, 185*yscale + i * menuspacing, 0, color);
						break;
					case OMMusicVol:
						Text = OMOptionMenus[menu].name + to_string((int)(Audio->GetVolumeMusic()*100/128)) + "%";
						Font->WriteText(Renderer, "Roboto-Regular", menutextsize, Text, Text.length(), 300*xscale, 185*yscale + i * menuspacing, 0, color);
						break;
					default:
						Font->WriteText(Renderer, "Roboto-Regular", menutextsize, OMOptionMenus[menu].name, OMOptionMenus[menu].name.length(), 300*xscale,
							185*yscale + i * menuspacing, 0, color);
						break;
				}
			}
			Text = "Use dpad to switch between options. (A) to select and (B) for back";
			Font->WriteText(Renderer, "Roboto-Regular", 34*yscale, Text, Text.length(), 90*xscale, 630*yscale, 0, {255, 255, 255, 255});

			if ((!Input->PrevButtons.ButDown && Input->Buttons.ButDown) ||
				(!Input->PrevButtons.ButDown2 && Input->Buttons.ButDown2) ||
				(!Input->PrevButtons.ButDpadDown && Input->Buttons.ButDpadDown))
			{
				Audio->PlaySound(SfxSelect, 0);
				SelOptions += 1;
				if (SelOptions == OptionMenus)
					SelOptions = 0;
			}

			if ((!Input->PrevButtons.ButUp && Input->Buttons.ButUp) ||
				(!Input->PrevButtons.ButUp2 && Input->Buttons.ButUp2) ||
				(!Input->PrevButtons.ButDpadUp && Input->Buttons.ButDpadUp))
			{
				Audio->PlaySound(SfxSelect, 0);

				SelOptions -= 1;
				if( SelOptions == -1)
					SelOptions = OptionMenus - 1;
			}

			if ((!Input->PrevButtons.ButLeft && Input->Buttons.ButLeft) ||
				(!Input->PrevButtons.ButLeft2 && Input->Buttons.ButLeft2) ||
				(!Input->PrevButtons.ButDpadLeft && Input->Buttons.ButDpadLeft))
			{
				Audio->PlaySound(SfxSelect, 0);
				bool wasplaying;
				switch (selectedmenu)
				{
					case OMSoundVol:
						Audio->DecVolumeSound();
						break;

					case OMMusicVol:
						wasplaying = Audio->IsMusicPlaying();
						Audio->DecVolumeMusic();
						if (!wasplaying)
							Audio->PlayMusic(MusMenu, -1);
						break;

				}

				//savehighscoresoptions()
			}

			if ((!Input->PrevButtons.ButRight && Input->Buttons.ButRight) ||
				(!Input->PrevButtons.ButRight2 && Input->Buttons.ButRight2) ||
				(!Input->PrevButtons.ButDpadRight && Input->Buttons.ButDpadRight))
			{
				Audio->PlaySound(SfxSelect, 0);
				bool wasplaying;
				switch (selectedmenu)
				{
					case OMSoundVol:
						Audio->IncVolumeSound();
						break;

					case OMMusicVol:
						wasplaying = Audio->IsMusicPlaying();
						Audio->IncVolumeMusic();
						if (!wasplaying)
							Audio->PlayMusic(MusMenu, -1);
						break;
				}

				//savehighscoresoptions()
			}


			if ((!Input->PrevButtons.ButBack && Input->Buttons.ButBack) ||
				(!Input->PrevButtons.ButB && Input->Buttons.ButB))
			{
				Audio->PlaySound(SfxBack, 0);

				CurrentMainMenu = -1;
			}

			if ((!Input->PrevButtons.ButStart && Input->Buttons.ButStart) ||
				(!Input->PrevButtons.ButA && Input->Buttons.ButA))
			{
				Audio->PlaySound(SfxSelect, 0);
				bool wasplaying;
				switch(selectedmenu)
				{
					case OMBack:
						CurrentMainMenu = -1;
						break;
					case OMResetHighScores:
						CGame_ResetHighScores();
						//savehighscoresoptions();
						break;
					case OMSoundVol:
						Audio->IncVolumeSound();
						break;
					case OMMusicVol:
						wasplaying = Audio->IsMusicPlaying();
						Audio->IncVolumeMusic();
						if(!wasplaying)
							Audio->PlayMusic(MusMenu, -1);
						break;
				}
			}
			break;
		}


		case MMHelp:
		{
			Text = "Help";
			Font->WriteText(Renderer, "Roboto-Regular", 80*yscale, Text, Text.length(), 560*xscale, 50*yscale, 0, {255, 255, 255, 255});

			Text = "Retro time is a collection of retro games, each playable in different\n"
				"game modes.\n"
				"\n"
				"You can control players in games with the dpad. Extra actions are\n"
				"available, using (A), depending on the game.\n"
				"\n"
				"During gameplay you can access the pause menu, by pressing\n"
				"(B) button. Pressing it again will resume gameplay.\n"
				"\n"
				"There are 3 game modes: Retro Carousel, Time Attack and\n"
				"Lives mode.";
			int tw = Font->TextWidth("Roboto-Regular", 38*yscale, Text, Text.length());
			Font->WriteText(Renderer, "Roboto-Regular", 38*yscale, Text, Text.length(), (ScreenWidth-tw)/2, 140*yscale, 0, {255, 255, 255, 255});


			if ((!Input->PrevButtons.ButA && Input->Buttons.ButA) ||
				(!Input->PrevButtons.ButB && Input->Buttons.ButB) ||
				(!Input->PrevButtons.ButBack && Input->Buttons.ButBack) ||
				(!Input->PrevButtons.ButStart && Input->Buttons.ButStart))
			{
				Audio->PlaySound(SfxBack, 0);

				CurrentMainMenu = -1;
			}
			break;
		}
		case MMCredits:
		{
			Text = "Credits";
			Font->WriteText(Renderer, "Roboto-Regular", 80*yscale, Text, Text.length(), 500*xscale, 50*yscale, 0, {255, 255, 255, 255});

			Text =
			"Block Stacker code is heavily inspired on this video from one lone\n"
			"coder (javidx9) with my own adaptations: https://youtu.be/8OK8_tHeCIA\n"
	//		"\n"
			"Snakey code is based on an edureka article (by Wajiha Urooj) about making\n"
			"python snake: https://www.edureka.co/blog/snake-game-with-pygame\n"
	//		"\n"
			"Brick Breaker ball / block collision uses a detection seen in wireframe\n"
			"magazine nr 11: https://wireframe.raspberrypi.org\n"
	//		"\n"
			"Toady water and grass background tiles have been created by me,\n"
			"Willems Davy aka joyrider3774 feel free to use them. Music is\n"
			"created, payed and owned by me using Strofe: https://www.strofe.com\n"
	//		"\n"
			"All other assets (including sound) come from either payed or free\n"
			"asset packs. For a complete list check the github or itch.io page:\n"
			"https://github.com/joyrider3774/RetroTime or\n"
			"https://joyrider3774.itch.io/retrotime\n";

			int tw = Font->TextWidth("Roboto-Regular", 35*yscale, Text, Text.length());
			Font->WriteText(Renderer, "Roboto-Regular", 35*yscale, Text, Text.length(), (ScreenWidth - tw) / 2, 140*yscale, 0, {255, 255, 255, 255});

			if ((!Input->PrevButtons.ButA && Input->Buttons.ButA) ||
				(!Input->PrevButtons.ButB && Input->Buttons.ButB) ||
				(!Input->PrevButtons.ButBack && Input->Buttons.ButBack) ||
				(!Input->PrevButtons.ButStart && Input->Buttons.ButStart))
			{
				Audio->PlaySound(SfxBack, 0);

				CurrentMainMenu = -1;
			}
			break;
		}
		case MMHighScores:
		{
			Text = "High Scores";
			Font->WriteText(Renderer, "Roboto-Regular", 80*yscale, Text, Text.length(), 400*xscale, 50*yscale, 0, {255, 255, 255, 255});

			Text = "Retro Carousel Total highscore: " + to_string(RetroCarouselHighScore);
			Font->WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 150*xscale, 195*yscale, 0, {255, 255, 255, 255});

			Font->WriteText(Renderer, "Roboto-Regular", 50*yscale, GSGames[Game].name, GSGames[Game].name.length(), 150*xscale, 290*yscale, 0, {255, 255, 255, 255});

			for(int mode = 0; mode < Modes; mode++)
			{
				Text = GMModes[mode].name + ": " + to_string(HighScores[Game][mode]);
				Font->WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 150*xscale, (350 + mode * 46)*yscale, 0, {255, 255, 255, 255});
			}

			Text = "Use dpad or (A) to switch between games. (B) for back";
			Font->WriteText(Renderer, "Roboto-Regular", 34*yscale, Text, Text.length(), 200*xscale, 630*yscale, 0, {255, 255, 255, 255});

			 if ((!Input->PrevButtons.ButBack && Input->Buttons.ButBack) ||
				(!Input->PrevButtons.ButB && Input->Buttons.ButB))
			 {
				Audio->PlaySound(SfxBack, 0);

				CurrentMainMenu = -1;
			 }

			if ((!Input->PrevButtons.ButStart && Input->Buttons.ButStart) ||
				(!Input->PrevButtons.ButRight && Input->Buttons.ButRight) ||
				(!Input->PrevButtons.ButRight2 && Input->Buttons.ButRight2) ||
				(!Input->PrevButtons.ButDpadRight && Input->Buttons.ButDpadRight) ||
				(!Input->PrevButtons.ButA && Input->Buttons.ButA))
			{
				Audio->PlaySound(SfxSelect, 0);

				Game += 1;
				if (Game == Games)
					Game = 0;
			}

			 if ((!Input->PrevButtons.ButLeft && Input->Buttons.ButLeft) ||
				 (!Input->PrevButtons.ButLeft2 && Input->Buttons.ButLeft2) ||
				 (!Input->PrevButtons.ButDpadLeft && Input->Buttons.ButDpadLeft))
			 {
				Audio->PlaySound(SfxSelect, 0);

				Game -= 1;
				if (Game == -1)
					Game = Games - 1;
			 }
			 break;
		}
		case MMSelectGame:
		{
			int gamestate = GSGames[Game].state;
			Text = "Select Game";
			Font->WriteText(Renderer, "Roboto-Regular", 80*yscale, Text, Text.length(), 465*xscale, 50*yscale, 0, {255, 255, 255, 255});
			Text = GSGames[Game].name;
			Font->WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 50*xscale, 120*yscale, 0, {255, 255, 255, 255});

			Text = GMModes[GameMode].name + " High Score: " + to_string(HighScores[Game][GameMode]);
			Font->WriteText(Renderer, "Roboto-Regular", 40*yscale, Text, Text.length(), 50*xscale, 170*yscale, 0, {255, 255, 255, 255});

			Font->WriteText(Renderer, "Roboto-Regular", 38*yscale, GSGames[Game].description, GSGames[Game].description.length(), 50*xscale, 230*yscale, 2, {255, 255, 255, 255});

			Text = "Use dpad to switch between games. (A) to select and (B) for back";
			Font->WriteText(Renderer, "Roboto-Regular", 34*yscale, Text, Text.length(), 90*xscale, 630*yscale, 0, {255, 255, 255, 255});

			if ((!Input->PrevButtons.ButBack && Input->Buttons.ButBack) ||
				(!Input->PrevButtons.ButB && Input->Buttons.ButB))
			{
				Audio->PlaySound(SfxBack, 0);
				CurrentMainMenu = MMSelectGameMode;
			}

			if ((!Input->PrevButtons.ButStart && Input->Buttons.ButStart) ||
				(!Input->PrevButtons.ButA && Input->Buttons.ButA))
			{
				Audio->PlaySound(SfxConfirm, 0);

				GameState = gamestate;
				CGame_ResetScores();
				CurrentMainMenu = -1;
				Input->ResetButtons();
			}

			if ((!Input->PrevButtons.ButLeft && Input->Buttons.ButLeft) ||
				(!Input->PrevButtons.ButLeft2 && Input->Buttons.ButLeft2) ||
				(!Input->PrevButtons.ButDpadLeft && Input->Buttons.ButDpadLeft))
			{
				Audio->PlaySound(SfxSelect, 0);

				Game -= 1;
				if (Game == -1)
					Game = Games - 1;
			}
			if ((!Input->PrevButtons.ButRight && Input->Buttons.ButRight) ||
				(!Input->PrevButtons.ButRight2 && Input->Buttons.ButRight2) ||
				(!Input->PrevButtons.ButDpadRight && Input->Buttons.ButDpadRight))
			{
				Audio->PlaySound(SfxSelect, 0);

				Game += 1;
				if (Game == Games)
					Game = 0;
			}
			break;
		}
		case MMSelectGameMode:
		{
			string Text = "Select Game Mode";
			Font->WriteText(Renderer, "Roboto-Regular", 80*yscale, Text, Text.length(), 285*xscale, 50*yscale, 0, {255, 255, 255, 255});

			int ModeIterator;
			SDL_Color color;
			for(int i = 0; i < Modes; i++)
			{
				ModeIterator = GMModes[i].mode;
				if (ModeIterator == GameMode)
					color = {255, 255, 255, 255};
				else
					color = {255, 255, 255, 64};
				Font->WriteText(Renderer, "Roboto-Regular", menutextsize, GMModes[i].name, GMModes[i].name.length(), 470*xscale, 135*yscale + i * menuspacing, 0, color);
			}
			Font->WriteText(Renderer, "Roboto-Regular", 40*yscale, GMModes[GameMode].description, GMModes[GameMode].description.length(),
				90*xscale, 400*yscale, 0, {255,255,255,255});

			Text = "Use dpad to switch between game modes. (A) to select and (B) for back";
			Font->WriteText(Renderer, "Roboto-Regular", 34*yscale, Text, Text.length(), 90*xscale, 630*yscale, 0, {255,255,255,255});

			if ((!Input->PrevButtons.ButDown && Input->Buttons.ButDown) ||
				(!Input->PrevButtons.ButDown2 && Input->Buttons.ButDown2) ||
				(!Input->PrevButtons.ButDpadDown && Input->Buttons.ButDpadDown))
			{
				Audio->PlaySound(SfxSelect, 0);
				GameMode += 1;
				if (GameMode == Modes)
					GameMode = 0;
			}

			if ((!Input->PrevButtons.ButUp && Input->Buttons.ButUp) ||
				(!Input->PrevButtons.ButUp2 && Input->Buttons.ButUp2) ||
				(!Input->PrevButtons.ButDpadUp && Input->Buttons.ButDpadUp))
			{
				Audio->PlaySound(SfxSelect, 0);

				GameMode -= 1;
				if (GameMode == -1)
					GameMode = Modes - 1;
			}

			if ((!Input->PrevButtons.ButB && Input->Buttons.ButB) ||
				(!Input->PrevButtons.ButBack && Input->Buttons.ButBack))
			{
				Audio->PlaySound(SfxBack, 0);
				CurrentMainMenu = -1;
			}

			if ((!Input->PrevButtons.ButStart && Input->Buttons.ButStart) ||
				(!Input->PrevButtons.ButA && Input->Buttons.ButA))
			{
				Audio->PlaySound(SfxConfirm, 0);

				if (GameMode == GMRetroCarousel)
				{
					Game = 0;
					CGame_ResetScores();
					CurrentMainMenu = -1;
					GameState = GSGames[Game].state;
				}
				else
					CurrentMainMenu = MMSelectGame;
			}
			break;
		}
		default:
		{
			SelMenu = MMMainMenus[SelectedMenu].menu;

			string Text = "Retro Time";
			Font->WriteText(Renderer, "Roboto-Regular", 80*yscale, Text, Text.length(), 465*xscale, 50*yscale, 0, {255, 255, 255, 255});

			int MenuIterator;
			SDL_Color color;
			for (int i = 0; i < MainMenus; i++)
			{
				MenuIterator = MMMainMenus[i].menu;
				if (MenuIterator == SelMenu)
					color = {255, 255, 255, 255};
				else
					color = {255, 255, 255, 64};
				Font->WriteText(Renderer, "Roboto-Regular", menutextsize, MMMainMenus[i].name, MMMainMenus[i].name.length(), 500*xscale, 150*yscale + i * menuspacing, 0, color);
			}

			if ((!Input->PrevButtons.ButDown && Input->Buttons.ButDown) ||
				(!Input->PrevButtons.ButDown2 && Input->Buttons.ButDown2) ||
				(!Input->PrevButtons.ButDpadDown && Input->Buttons.ButDpadDown))
			{
				Audio->PlaySound(SfxSelect, 0);
				SelectedMenu += 1;
				if (SelectedMenu == MainMenus)
					SelectedMenu = 0;
			}

			if ((!Input->PrevButtons.ButUp && Input->Buttons.ButUp) ||
				(!Input->PrevButtons.ButUp2 && Input->Buttons.ButUp2) ||
				(!Input->PrevButtons.ButDpadUp && Input->Buttons.ButDpadUp))

			{
				Audio->PlaySound(SfxSelect, 0);

				SelectedMenu -= 1;
				if(SelectedMenu == -1)
					SelectedMenu = MainMenus - 1;
			}

			if ((!Input->PrevButtons.ButStart && Input->Buttons.ButStart) ||
				(!Input->PrevButtons.ButA && Input->Buttons.ButA))
			{
				CurrentMainMenu = SelMenu;
				Audio->PlaySound(SfxConfirm, 0);
				if (SelMenu == MMQuit)
					GameState = GSQuit;

				if (SelMenu == MMStart)
					CurrentMainMenu = MMSelectGameMode;
			}
		}
	}

	//SDL_Rect SrcRect = {230*xscale,85*yscale,820*xscale,550*yscale};
	// //grab transparant part of frame + menu
	// SDL_Texture *Tmp = SDL_CreateTexture(Renderer, PixelFormat, SDL_TEXTUREACCESS_TARGET, SrcRect.w, SrcRect.h);
	// SDL_Texture *TmpRender = SDL_GetRenderTarget(Renderer);
	// SDL_SetRenderTarget(Renderer, Tmp);
	// CImage_DrawImage(Renderer, TexOffScreen, &SrcRect, NULL);

	//draw the frame again without transparancy
	//SDL_SetRenderTarget(Renderer, TmpRender);
	//CImage_DrawImageFuze(Renderer, GFXFrameID, true, &FramePos, 0, &FrameScale, 255, 255, 255, 255);

	// //and then draw the transparant part over it now
	// CImage_DrawImage(Renderer, Tmp, NULL, &SrcRect);
	// SDL_DestroyTexture(Tmp);

//	freeImage(surface)
//	stopChannel(0)
}
