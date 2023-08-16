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

void InitTitleScreen(CGame *Game)
{
	Game->Audio->PlayMusic(Game->MusMenu, -1);
	Game->StartCrossFade(Game->GameState, SGNone, 0, 0);
}

void TitleScreen(CGame *Game)
{
	if(Game->GameState == GSTitleScreenInit)
	{
		InitTitleScreen(Game);
		Game->GameState -= initDiff;
	}
	//draw everything to offscreen surface
	Game->DrawTitleBackground(Game->Input->Buttons.ButLT && Game->Input->Buttons.ButRT);
	SDL_SetRenderDrawColor(Game->Renderer, 255, 255, 255, 235);
	//so we can can copy the transparant part with the blue and text from this image
	SDL_Point FramePos = {ScreenWidth / 2, ScreenHeight / 2};
	Vec2F FrameScale = {16.0f / 4 * xscale, 12.8f *yscale};
	Game->Image->DrawImageFuze(Game->Renderer, Game->GFXFrameID, true, &FramePos, 0, &FrameScale, 255, 255, 255, 255);

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
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 80*yscale, Text, Text.length(), 525*xscale, 50*yscale, 0, {255, 255, 255, 255});

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
						Text = OMOptionMenus[menu].name + to_string((int)(Game->Audio->GetVolumeSound()*100/128)) + "%";
						Game->Font->WriteText(Game->Renderer, "Roboto-Regular", menutextsize, Text, Text.length(), 300*xscale, 185*yscale + i * menuspacing, 0, color);
						break;
					case OMMusicVol:
						Text = OMOptionMenus[menu].name + to_string((int)(Game->Audio->GetVolumeMusic()*100/128)) + "%";
						Game->Font->WriteText(Game->Renderer, "Roboto-Regular", menutextsize, Text, Text.length(), 300*xscale, 185*yscale + i * menuspacing, 0, color);
						break;
					default:
						Game->Font->WriteText(Game->Renderer, "Roboto-Regular", menutextsize, OMOptionMenus[menu].name, OMOptionMenus[menu].name.length(), 300*xscale,
							185*yscale + i * menuspacing, 0, color);
						break;
				}
			}
			Text = "Use dpad to switch between options. (A) to select and (B) for back";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 34*yscale, Text, Text.length(), 90*xscale, 630*yscale, 0, {255, 255, 255, 255});

			if ((!Game->Input->PrevButtons.ButDown && Game->Input->Buttons.ButDown) ||
				(!Game->Input->PrevButtons.ButDown2 && Game->Input->Buttons.ButDown2) ||
				(!Game->Input->PrevButtons.ButDpadDown && Game->Input->Buttons.ButDpadDown))
			{
				Game->Audio->PlaySound(Game->SfxSelect, 0);
				SelOptions += 1;
				if (SelOptions == OptionMenus)
					SelOptions = 0;
			}

			if ((!Game->Input->PrevButtons.ButUp && Game->Input->Buttons.ButUp) ||
				(!Game->Input->PrevButtons.ButUp2 && Game->Input->Buttons.ButUp2) ||
				(!Game->Input->PrevButtons.ButDpadUp && Game->Input->Buttons.ButDpadUp))
			{
				Game->Audio->PlaySound(Game->SfxSelect, 0);

				SelOptions -= 1;
				if( SelOptions == -1)
					SelOptions = OptionMenus - 1;
			}

			if ((!Game->Input->PrevButtons.ButLeft && Game->Input->Buttons.ButLeft) ||
				(!Game->Input->PrevButtons.ButLeft2 && Game->Input->Buttons.ButLeft2) ||
				(!Game->Input->PrevButtons.ButDpadLeft && Game->Input->Buttons.ButDpadLeft))
			{
				Game->Audio->PlaySound(Game->SfxSelect, 0);
				bool wasplaying;
				switch (selectedmenu)
				{
					case OMSoundVol:
						Game->Audio->DecVolumeSound();
						break;

					case OMMusicVol:
						wasplaying = Game->Audio->IsMusicPlaying();
						Game->Audio->DecVolumeMusic();
						if (!wasplaying)
							Game->Audio->PlayMusic(Game->MusMenu, -1);
						break;

				}

				//savehighscoresoptions()
			}

			if ((!Game->Input->PrevButtons.ButRight && Game->Input->Buttons.ButRight) ||
				(!Game->Input->PrevButtons.ButRight2 && Game->Input->Buttons.ButRight2) ||
				(!Game->Input->PrevButtons.ButDpadRight && Game->Input->Buttons.ButDpadRight))
			{
				Game->Audio->PlaySound(Game->SfxSelect, 0);
				bool wasplaying;
				switch (selectedmenu)
				{
					case OMSoundVol:
						Game->Audio->IncVolumeSound();
						break;

					case OMMusicVol:
						wasplaying = Game->Audio->IsMusicPlaying();
						Game->Audio->IncVolumeMusic();
						if (!wasplaying)
							Game->Audio->PlayMusic(Game->MusMenu, -1);
						break;
				}

				//savehighscoresoptions()
			}


			if ((!Game->Input->PrevButtons.ButBack && Game->Input->Buttons.ButBack) ||
				(!Game->Input->PrevButtons.ButB && Game->Input->Buttons.ButB))
			{
				Game->Audio->PlaySound(Game->SfxBack, 0);

				CurrentMainMenu = -1;
			}

			if ((!Game->Input->PrevButtons.ButStart && Game->Input->Buttons.ButStart) ||
				(!Game->Input->PrevButtons.ButA && Game->Input->Buttons.ButA))
			{
				Game->Audio->PlaySound(Game->SfxSelect, 0);
				bool wasplaying;
				switch(selectedmenu)
				{
					case OMBack:
						CurrentMainMenu = -1;
						break;
					case OMResetHighScores:
						Game->ResetHighScores();
						//Game->savehighscoresoptions();
						break;
					case OMSoundVol:
						Game->Audio->IncVolumeSound();
						break;
					case OMMusicVol:
						wasplaying = Game->Audio->IsMusicPlaying();
						Game->Audio->IncVolumeMusic();
						if(!wasplaying)
							Game->Audio->PlayMusic(Game->MusMenu, -1);
						break;
				}
			}
			break;
		}


		case MMHelp:
		{
			Text = "Help";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 80*yscale, Text, Text.length(), 560*xscale, 50*yscale, 0, {255, 255, 255, 255});

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
			int tw = Game->Font->TextWidth("Roboto-Regular", 38*yscale, Text, Text.length());
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 38*yscale, Text, Text.length(), (ScreenWidth-tw)/2, 140*yscale, 0, {255, 255, 255, 255});


			if ((!Game->Input->PrevButtons.ButA && Game->Input->Buttons.ButA) ||
				(!Game->Input->PrevButtons.ButB && Game->Input->Buttons.ButB) ||
				(!Game->Input->PrevButtons.ButBack && Game->Input->Buttons.ButBack) ||
				(!Game->Input->PrevButtons.ButStart && Game->Input->Buttons.ButStart))
			{
				Game->Audio->PlaySound(Game->SfxBack, 0);

				CurrentMainMenu = -1;
			}
			break;
		}
		case MMCredits:
		{
			Text = "Credits";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 80*yscale, Text, Text.length(), 500*xscale, 50*yscale, 0, {255, 255, 255, 255});

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

			int tw = Game->Font->TextWidth("Roboto-Regular", 35*yscale, Text, Text.length());
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 35*yscale, Text, Text.length(), (ScreenWidth - tw) / 2, 140*yscale, 0, {255, 255, 255, 255});

			if ((!Game->Input->PrevButtons.ButA && Game->Input->Buttons.ButA) ||
				(!Game->Input->PrevButtons.ButB && Game->Input->Buttons.ButB) ||
				(!Game->Input->PrevButtons.ButBack && Game->Input->Buttons.ButBack) ||
				(!Game->Input->PrevButtons.ButStart && Game->Input->Buttons.ButStart))
			{
				Game->Audio->PlaySound(Game->SfxBack, 0);

				CurrentMainMenu = -1;
			}
			break;
		}
		case MMHighScores:
		{
			Text = "High Scores";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 80*yscale, Text, Text.length(), 400*xscale, 50*yscale, 0, {255, 255, 255, 255});

			Text = "Retro Carousel Total highscore: " + to_string(Game->RetroCarouselHighScore);
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 150*xscale, 195*yscale, 0, {255, 255, 255, 255});

			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 50*yscale, GSGames[Game->Game].name, GSGames[Game->Game].name.length(), 150*xscale, 290*yscale, 0, {255, 255, 255, 255});

			for(int mode = 0; mode < Modes; mode++)
			{
				Text = GMModes[mode].name + ": " + to_string(Game->HighScores[Game->Game][mode]);
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 150*xscale, (350 + mode * 46)*yscale, 0, {255, 255, 255, 255});
			}

			Text = "Use dpad or (A) to switch between games. (B) for back";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 34*yscale, Text, Text.length(), 200*xscale, 630*yscale, 0, {255, 255, 255, 255});

			 if ((!Game->Input->PrevButtons.ButBack && Game->Input->Buttons.ButBack) ||
				(!Game->Input->PrevButtons.ButB && Game->Input->Buttons.ButB))
			 {
				Game->Audio->PlaySound(Game->SfxBack, 0);

				CurrentMainMenu = -1;
			 }

			if ((!Game->Input->PrevButtons.ButStart && Game->Input->Buttons.ButStart) ||
				(!Game->Input->PrevButtons.ButRight && Game->Input->Buttons.ButRight) ||
				(!Game->Input->PrevButtons.ButRight2 && Game->Input->Buttons.ButRight2) ||
				(!Game->Input->PrevButtons.ButDpadRight && Game->Input->Buttons.ButDpadRight) ||
				(!Game->Input->PrevButtons.ButA && Game->Input->Buttons.ButA))
			{
				Game->Audio->PlaySound(Game->SfxSelect, 0);

				Game->Game += 1;
				if (Game->Game == Games)
					Game->Game = 0;
			}

			 if ((!Game->Input->PrevButtons.ButLeft && Game->Input->Buttons.ButLeft) ||
				 (!Game->Input->PrevButtons.ButLeft2 && Game->Input->Buttons.ButLeft2) ||
				 (!Game->Input->PrevButtons.ButDpadLeft && Game->Input->Buttons.ButDpadLeft))
			 {
				Game->Audio->PlaySound(Game->SfxSelect, 0);

				Game->Game -= 1;
				if (Game->Game == -1)
					Game->Game = Games - 1;
			 }
			 break;
		}
		case MMSelectGame:
		{
			int gamestate = GSGames[Game->Game].state;
			Text = "Select Game";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 80*yscale, Text, Text.length(), 465*xscale, 50*yscale, 0, {255, 255, 255, 255});
			Text = GSGames[Game->Game].name;
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 50*yscale, Text, Text.length(), 50*xscale, 120*yscale, 0, {255, 255, 255, 255});

			Text = GMModes[Game->GameMode].name + " High Score: " + to_string(Game->HighScores[Game->Game][Game->GameMode]);
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 40*yscale, Text, Text.length(), 50*xscale, 170*yscale, 0, {255, 255, 255, 255});

			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 38*yscale, GSGames[Game->Game].description, GSGames[Game->Game].description.length(), 50*xscale, 230*yscale, 2, {255, 255, 255, 255});

			Text = "Use dpad to switch between games. (A) to select and (B) for back";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 34*yscale, Text, Text.length(), 90*xscale, 630*yscale, 0, {255, 255, 255, 255});

			if ((!Game->Input->PrevButtons.ButBack && Game->Input->Buttons.ButBack) ||
				(!Game->Input->PrevButtons.ButB && Game->Input->Buttons.ButB))
			{
				Game->Audio->PlaySound(Game->SfxBack, 0);
				CurrentMainMenu = MMSelectGameMode;
			}

			if ((!Game->Input->PrevButtons.ButStart && Game->Input->Buttons.ButStart) ||
				(!Game->Input->PrevButtons.ButA && Game->Input->Buttons.ButA))
			{
				Game->Audio->PlaySound(Game->SfxConfirm, 0);

				Game->GameState = gamestate;
				Game->ResetScores();
				CurrentMainMenu = -1;
				Game->Input->ResetButtons();
			}

			if ((!Game->Input->PrevButtons.ButLeft && Game->Input->Buttons.ButLeft) ||
				(!Game->Input->PrevButtons.ButLeft2 && Game->Input->Buttons.ButLeft2) ||
				(!Game->Input->PrevButtons.ButDpadLeft && Game->Input->Buttons.ButDpadLeft))
			{
				Game->Audio->PlaySound(Game->SfxSelect, 0);

				Game->Game -= 1;
				if (Game->Game == -1)
					Game->Game = Games - 1;
			}
			if ((!Game->Input->PrevButtons.ButRight && Game->Input->Buttons.ButRight) ||
				(!Game->Input->PrevButtons.ButRight2 && Game->Input->Buttons.ButRight2) ||
				(!Game->Input->PrevButtons.ButDpadRight && Game->Input->Buttons.ButDpadRight))
			{
				Game->Audio->PlaySound(Game->SfxSelect, 0);

				Game->Game += 1;
				if (Game->Game == Games)
					Game->Game = 0;
			}
			break;
		}
		case MMSelectGameMode:
		{
			string Text = "Select Game Mode";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 80*yscale, Text, Text.length(), 285*xscale, 50*yscale, 0, {255, 255, 255, 255});

			int ModeIterator;
			SDL_Color color;
			for(int i = 0; i < Modes; i++)
			{
				ModeIterator = GMModes[i].mode;
				if (ModeIterator == Game->GameMode)
					color = {255, 255, 255, 255};
				else
					color = {255, 255, 255, 64};
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", menutextsize, GMModes[i].name, GMModes[i].name.length(), 470*xscale, 135*yscale + i * menuspacing, 0, color);
			}
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 40*yscale, GMModes[Game->GameMode].description, GMModes[Game->GameMode].description.length(),
				90*xscale, 400*yscale, 0, {255,255,255,255});

			Text = "Use dpad to switch between game modes. (A) to select and (B) for back";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 34*yscale, Text, Text.length(), 90*xscale, 630*yscale, 0, {255,255,255,255});

			if ((!Game->Input->PrevButtons.ButDown && Game->Input->Buttons.ButDown) ||
				(!Game->Input->PrevButtons.ButDown2 && Game->Input->Buttons.ButDown2) ||
				(!Game->Input->PrevButtons.ButDpadDown && Game->Input->Buttons.ButDpadDown))
			{
				Game->Audio->PlaySound(Game->SfxSelect, 0);
				Game->GameMode += 1;
				if (Game->GameMode == Modes)
					Game->GameMode = 0;
			}

			if ((!Game->Input->PrevButtons.ButUp && Game->Input->Buttons.ButUp) ||
				(!Game->Input->PrevButtons.ButUp2 && Game->Input->Buttons.ButUp2) ||
				(!Game->Input->PrevButtons.ButDpadUp && Game->Input->Buttons.ButDpadUp))
			{
				Game->Audio->PlaySound(Game->SfxSelect, 0);

				Game->GameMode -= 1;
				if (Game->GameMode == -1)
					Game->GameMode = Modes - 1;
			}

			if ((!Game->Input->PrevButtons.ButB && Game->Input->Buttons.ButB) ||
				(!Game->Input->PrevButtons.ButBack && Game->Input->Buttons.ButBack))
			{
				Game->Audio->PlaySound(Game->SfxBack, 0);
				CurrentMainMenu = -1;
			}

			if ((!Game->Input->PrevButtons.ButStart && Game->Input->Buttons.ButStart) ||
				(!Game->Input->PrevButtons.ButA && Game->Input->Buttons.ButA))
			{
				Game->Audio->PlaySound(Game->SfxConfirm, 0);

				if (Game->GameMode == GMRetroCarousel)
				{
					Game->Game = 0;
					Game->ResetScores();
					CurrentMainMenu = -1;
					Game->GameState = GSGames[Game->Game].state;
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
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 80*yscale, Text, Text.length(), 465*xscale, 50*yscale, 0, {255, 255, 255, 255});

			int MenuIterator;
			SDL_Color color;
			for (int i = 0; i < MainMenus; i++)
			{
				MenuIterator = MMMainMenus[i].menu;
				if (MenuIterator == SelMenu)
					color = {255, 255, 255, 255};
				else
					color = {255, 255, 255, 64};
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", menutextsize, MMMainMenus[i].name, MMMainMenus[i].name.length(), 500*xscale, 150*yscale + i * menuspacing, 0, color);
			}

			if ((!Game->Input->PrevButtons.ButDown && Game->Input->Buttons.ButDown) ||
				(!Game->Input->PrevButtons.ButDown2 && Game->Input->Buttons.ButDown2) ||
				(!Game->Input->PrevButtons.ButDpadDown && Game->Input->Buttons.ButDpadDown))
			{
				Game->Audio->PlaySound(Game->SfxSelect, 0);
				SelectedMenu += 1;
				if (SelectedMenu == MainMenus)
					SelectedMenu = 0;
			}

			if ((!Game->Input->PrevButtons.ButUp && Game->Input->Buttons.ButUp) ||
				(!Game->Input->PrevButtons.ButUp2 && Game->Input->Buttons.ButUp2) ||
				(!Game->Input->PrevButtons.ButDpadUp && Game->Input->Buttons.ButDpadUp))

			{
				Game->Audio->PlaySound(Game->SfxSelect, 0);

				SelectedMenu -= 1;
				if(SelectedMenu == -1)
					SelectedMenu = MainMenus - 1;
			}

			if ((!Game->Input->PrevButtons.ButStart && Game->Input->Buttons.ButStart) ||
				(!Game->Input->PrevButtons.ButA && Game->Input->Buttons.ButA))
			{
				CurrentMainMenu = SelMenu;
				Game->Audio->PlaySound(Game->SfxConfirm, 0);
				if (SelMenu == MMQuit)
					Game->GameState = GSQuit;

				if (SelMenu == MMStart)
					CurrentMainMenu = MMSelectGameMode;
			}
		}
	}

	//SDL_Rect SrcRect = {230*xscale,85*yscale,820*xscale,550*yscale};
	// //grab transparant part of frame + menu
	// SDL_Texture *Tmp = SDL_CreateTexture(Game->Renderer, PixelFormat, SDL_TEXTUREACCESS_TARGET, SrcRect.w, SrcRect.h);
	// SDL_Texture *TmpRender = SDL_GetRenderTarget(Game->Renderer);
	// SDL_SetRenderTarget(Game->Renderer, Tmp);
	// Game->Image->DrawImage(Game->Renderer, Game->TexOffScreen, &SrcRect, NULL);

	//draw the frame again without transparancy
	//SDL_SetRenderTarget(Game->Renderer, TmpRender);
	//Game->Image->DrawImageFuze(Game->Renderer, Game->GFXFrameID, true, &FramePos, 0, &FrameScale, 255, 255, 255, 255);

	// //and then draw the transparant part over it now
	// Game->Image->DrawImage(Game->Renderer, Tmp, NULL, &SrcRect);
	// SDL_DestroyTexture(Tmp);

//	freeImage(surface)
//	stopChannel(0)
}
