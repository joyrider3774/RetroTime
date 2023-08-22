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

const int menutextsize = 60*yscale;
const int menuspacing = 85*yscale;
const int rcolor = 64;
const int rdcolor = 1;

void InitTitleScreen()
{
	CAudio_PlayMusic(MusMenu, -1);
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
	SDL_Color color = {255, 255, 255, 255};
	char Text[1000];
	switch (CurrentMainMenu)
	{
		case MMOptions:
		{
			int selectedmenu = 0;
			//int menutextsize = (int)(35.0f*yscale);
			//int menuspacing = (int)(45.0f*yscale);

			selectedmenu = OMOptionMenus[SelOptions].menu;
			strcpy(Text, "Options");
			CFont_WriteText(Renderer, "Roboto-Regular", 80*yscale, Text, strlen(Text), 525*xscale, 50*yscale, 0, color);

			int menu;
			SDL_Color color = {255, 255, 255, 255};
			for(int i = 0; i < OptionMenus; i++)
			{
				menu = OMOptionMenus[i].menu;
				if(menu == selectedmenu)
					color.a = 255;
				else
					color.a = 64;

				switch(menu)
				{
					case OMSoundVol:
						sprintf(Text, "%s%d%%", OMOptionMenus[menu].name,(int)(CAudio_GetVolumeSound()*100/128));
						CFont_WriteText(Renderer, "Roboto-Regular", menutextsize, Text, strlen(Text), 300*xscale, 185*yscale + i * menuspacing, 0, color);
						break;
					case OMMusicVol:
						sprintf(Text, "%s%d%%", OMOptionMenus[menu].name,(int)(CAudio_GetVolumeMusic()*100/128));
						CFont_WriteText(Renderer, "Roboto-Regular", menutextsize, Text, strlen(Text), 300*xscale, 185*yscale + i * menuspacing, 0, color);
						break;
					default:
						CFont_WriteText(Renderer, "Roboto-Regular", menutextsize, OMOptionMenus[menu].name, strlen(OMOptionMenus[menu].name), 300*xscale,
							185*yscale + i * menuspacing, 0, color);
						break;
				}
			}
			strcpy(Text, "Use dpad to switch between options. (A) to select and (B) for back");
			CFont_WriteText(Renderer, "Roboto-Regular", 34*yscale, Text, strlen(Text), 90*xscale, 630*yscale, 0, color);

			if ((!CInput_PrevButtons.ButDown && CInput_Buttons.ButDown) ||
				(!CInput_PrevButtons.ButDown2 && CInput_Buttons.ButDown2) ||
				(!CInput_PrevButtons.ButDpadDown && CInput_Buttons.ButDpadDown))
			{
				CAudio_PlaySound(SfxSelect, 0);
				SelOptions += 1;
				if (SelOptions == OptionMenus)
					SelOptions = 0;
			}

			if ((!CInput_PrevButtons.ButUp && CInput_Buttons.ButUp) ||
				(!CInput_PrevButtons.ButUp2 && CInput_Buttons.ButUp2) ||
				(!CInput_PrevButtons.ButDpadUp && CInput_Buttons.ButDpadUp))
			{
				CAudio_PlaySound(SfxSelect, 0);

				SelOptions -= 1;
				if( SelOptions == -1)
					SelOptions = OptionMenus - 1;
			}

			if ((!CInput_PrevButtons.ButLeft && CInput_Buttons.ButLeft) ||
				(!CInput_PrevButtons.ButLeft2 && CInput_Buttons.ButLeft2) ||
				(!CInput_PrevButtons.ButDpadLeft && CInput_Buttons.ButDpadLeft))
			{
				CAudio_PlaySound(SfxSelect, 0);
				bool wasplaying;
				switch (selectedmenu)
				{
					case OMSoundVol:
						CAudio_DecVolumeSound();
						break;

					case OMMusicVol:
						wasplaying = CAudio_IsMusicPlaying();
						CAudio_DecVolumeMusic();
						if (!wasplaying)
							CAudio_PlayMusic(MusMenu, -1);
						break;

				}

				//savehighscoresoptions()
			}

			if ((!CInput_PrevButtons.ButRight && CInput_Buttons.ButRight) ||
				(!CInput_PrevButtons.ButRight2 && CInput_Buttons.ButRight2) ||
				(!CInput_PrevButtons.ButDpadRight && CInput_Buttons.ButDpadRight))
			{
				CAudio_PlaySound(SfxSelect, 0);
				bool wasplaying;
				switch (selectedmenu)
				{
					case OMSoundVol:
						CAudio_IncVolumeSound();
						break;

					case OMMusicVol:
						wasplaying = CAudio_IsMusicPlaying();
						CAudio_IncVolumeMusic();
						if (!wasplaying)
							CAudio_PlayMusic(MusMenu, -1);
						break;
				}

				//savehighscoresoptions()
			}


			if ((!CInput_PrevButtons.ButBack && CInput_Buttons.ButBack) ||
				(!CInput_PrevButtons.ButB && CInput_Buttons.ButB))
			{
				CAudio_PlaySound(SfxBack, 0);

				CurrentMainMenu = -1;
			}

			if ((!CInput_PrevButtons.ButStart && CInput_Buttons.ButStart) ||
				(!CInput_PrevButtons.ButA && CInput_Buttons.ButA))
			{
				CAudio_PlaySound(SfxSelect, 0);
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
						CAudio_IncVolumeSound();
						break;
					case OMMusicVol:
						wasplaying = CAudio_IsMusicPlaying();
						CAudio_IncVolumeMusic();
						if(!wasplaying)
							CAudio_PlayMusic(MusMenu, -1);
						break;
				}
			}
			break;
		}


		case MMHelp:
		{
			strcpy(Text,"Help");
			CFont_WriteText(Renderer, "Roboto-Regular", 80*yscale, Text, strlen(Text), 560*xscale, 50*yscale, 0, color);

			strcpy(Text, "Retro time is a collection of retro games, each playable in different\n"
				"game modes.\n"
				"\n"
				"You can control players in games with the dpad. Extra actions are\n"
				"available, using (A), depending on the game.\n"
				"\n"
				"During gameplay you can access the pause menu, by pressing\n"
				"(B) button. Pressing it again will resume gameplay.\n"
				"\n"
				"There are 3 game modes: Retro Carousel, Time Attack and\n"
				"Lives mode.");
			int tw = CFont_TextWidth("Roboto-Regular", 38*yscale, Text, strlen(Text));
			CFont_WriteText(Renderer, "Roboto-Regular", 38*yscale, Text, strlen(Text), (ScreenWidth-tw)/2, 140*yscale, 0, color);


			if ((!CInput_PrevButtons.ButA && CInput_Buttons.ButA) ||
				(!CInput_PrevButtons.ButB && CInput_Buttons.ButB) ||
				(!CInput_PrevButtons.ButBack && CInput_Buttons.ButBack) ||
				(!CInput_PrevButtons.ButStart && CInput_Buttons.ButStart))
			{
				CAudio_PlaySound(SfxBack, 0);

				CurrentMainMenu = -1;
			}
			break;
		}
		case MMCredits:
		{
			strcpy(Text, "Credits");
			CFont_WriteText(Renderer, "Roboto-Regular", 80*yscale, Text, strlen(Text), 500*xscale, 50*yscale, 0, color);

			strcpy(Text,
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
			"https://joyrider3774.itch.io/retrotime\n");

			int tw = CFont_TextWidth("Roboto-Regular", 35*yscale, Text, strlen(Text));
			CFont_WriteText(Renderer, "Roboto-Regular", 35*yscale, Text, strlen(Text), (ScreenWidth - tw) / 2, 140*yscale, 0, color);

			if ((!CInput_PrevButtons.ButA && CInput_Buttons.ButA) ||
				(!CInput_PrevButtons.ButB && CInput_Buttons.ButB) ||
				(!CInput_PrevButtons.ButBack && CInput_Buttons.ButBack) ||
				(!CInput_PrevButtons.ButStart && CInput_Buttons.ButStart))
			{
				CAudio_PlaySound(SfxBack, 0);

				CurrentMainMenu = -1;
			}
			break;
		}
		case MMHighScores:
		{
			strcpy(Text,"High Scores");
			CFont_WriteText(Renderer, "Roboto-Regular", 80*yscale, Text, strlen(Text), 400*xscale, 50*yscale, 0, color);

			sprintf(Text, "Retro Carousel Total highscore: %llu", RetroCarouselHighScore);
			CFont_WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, strlen(Text), 150*xscale, 195*yscale, 0, color);

			CFont_WriteText(Renderer, "Roboto-Regular", 50*yscale, GSGames[Game].name, strlen(GSGames[Game].name), 150*xscale, 290*yscale, 0, color);

			for(int mode = 0; mode < Modes; mode++)
			{
				sprintf(Text, "%s: %llu", GMModes[mode].name, HighScores[Game][mode]);
				CFont_WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, strlen(Text), 150*xscale, (350 + mode * 46)*yscale, 0, color);
			}

			strcpy(Text, "Use dpad or (A) to switch between games. (B) for back");
			CFont_WriteText(Renderer, "Roboto-Regular", 34*yscale, Text, strlen(Text), 200*xscale, 630*yscale, 0, color);

			 if ((!CInput_PrevButtons.ButBack && CInput_Buttons.ButBack) ||
				(!CInput_PrevButtons.ButB && CInput_Buttons.ButB))
			 {
				CAudio_PlaySound(SfxBack, 0);

				CurrentMainMenu = -1;
			 }

			if ((!CInput_PrevButtons.ButStart && CInput_Buttons.ButStart) ||
				(!CInput_PrevButtons.ButRight && CInput_Buttons.ButRight) ||
				(!CInput_PrevButtons.ButRight2 && CInput_Buttons.ButRight2) ||
				(!CInput_PrevButtons.ButDpadRight && CInput_Buttons.ButDpadRight) ||
				(!CInput_PrevButtons.ButA && CInput_Buttons.ButA))
			{
				CAudio_PlaySound(SfxSelect, 0);

				Game += 1;
				if (Game == Games)
					Game = 0;
			}

			 if ((!CInput_PrevButtons.ButLeft && CInput_Buttons.ButLeft) ||
				 (!CInput_PrevButtons.ButLeft2 && CInput_Buttons.ButLeft2) ||
				 (!CInput_PrevButtons.ButDpadLeft && CInput_Buttons.ButDpadLeft))
			 {
				CAudio_PlaySound(SfxSelect, 0);

				Game -= 1;
				if (Game == -1)
					Game = Games - 1;
			 }
			 break;
		}
		case MMSelectGame:
		{
			int gamestate = GSGames[Game].state;
			strcpy(Text, "Select Game");
			CFont_WriteText(Renderer, "Roboto-Regular", 80*yscale, Text, strlen(Text), 465*xscale, 50*yscale, 0, color);
			strcpy(Text, GSGames[Game].name);
			CFont_WriteText(Renderer, "Roboto-Regular", 50*yscale, Text, strlen(Text), 50*xscale, 120*yscale, 0, color);

			sprintf(Text, "%s High Score: %llu", GMModes[GameMode].name, HighScores[Game][GameMode]);
			CFont_WriteText(Renderer, "Roboto-Regular", 40*yscale, Text, strlen(Text), 50*xscale, 170*yscale, 0, color);

			CFont_WriteText(Renderer, "Roboto-Regular", 38*yscale, GSGames[Game].description, strlen(GSGames[Game].description), 50*xscale, 230*yscale, 2, color);

			strcpy(Text, "Use dpad to switch between games. (A) to select and (B) for back");
			CFont_WriteText(Renderer, "Roboto-Regular", 34*yscale, Text, strlen(Text), 90*xscale, 630*yscale, 0, color);

			if ((!CInput_PrevButtons.ButBack && CInput_Buttons.ButBack) ||
				(!CInput_PrevButtons.ButB && CInput_Buttons.ButB))
			{
				CAudio_PlaySound(SfxBack, 0);
				CurrentMainMenu = MMSelectGameMode;
			}

			if ((!CInput_PrevButtons.ButStart && CInput_Buttons.ButStart) ||
				(!CInput_PrevButtons.ButA && CInput_Buttons.ButA))
			{
				CAudio_PlaySound(SfxConfirm, 0);

				GameState = gamestate;
				CGame_ResetScores();
				CurrentMainMenu = -1;
				CInput_ResetButtons();
			}

			if ((!CInput_PrevButtons.ButLeft && CInput_Buttons.ButLeft) ||
				(!CInput_PrevButtons.ButLeft2 && CInput_Buttons.ButLeft2) ||
				(!CInput_PrevButtons.ButDpadLeft && CInput_Buttons.ButDpadLeft))
			{
				CAudio_PlaySound(SfxSelect, 0);

				Game -= 1;
				if (Game == -1)
					Game = Games - 1;
			}
			if ((!CInput_PrevButtons.ButRight && CInput_Buttons.ButRight) ||
				(!CInput_PrevButtons.ButRight2 && CInput_Buttons.ButRight2) ||
				(!CInput_PrevButtons.ButDpadRight && CInput_Buttons.ButDpadRight))
			{
				CAudio_PlaySound(SfxSelect, 0);

				Game += 1;
				if (Game == Games)
					Game = 0;
			}
			break;
		}
		case MMSelectGameMode:
		{
			strcpy(Text, "Select Game Mode");
			CFont_WriteText(Renderer, "Roboto-Regular", 80*yscale, Text, strlen(Text), 285*xscale, 50*yscale, 0, color);

			int ModeIterator;
			for(int i = 0; i < Modes; i++)
			{
				ModeIterator = GMModes[i].mode;
				if (ModeIterator == GameMode)
					color.a = 255;
				else
					color.a =  64;
				CFont_WriteText(Renderer, "Roboto-Regular", menutextsize, GMModes[i].name, strlen(GMModes[i].name), 470*xscale, 135*yscale + i * menuspacing, 0, color);
			}
			color.a = 255;
			CFont_WriteText(Renderer, "Roboto-Regular", 40*yscale, GMModes[GameMode].description, strlen(GMModes[GameMode].description),
				90*xscale, 400*yscale, 0, color);

			strcpy(Text, "Use dpad to switch between game modes. (A) to select and (B) for back");
			CFont_WriteText(Renderer, "Roboto-Regular", 34*yscale, Text, strlen(Text), 90*xscale, 630*yscale, 0, color);

			if ((!CInput_PrevButtons.ButDown && CInput_Buttons.ButDown) ||
				(!CInput_PrevButtons.ButDown2 && CInput_Buttons.ButDown2) ||
				(!CInput_PrevButtons.ButDpadDown && CInput_Buttons.ButDpadDown))
			{
				CAudio_PlaySound(SfxSelect, 0);
				GameMode += 1;
				if (GameMode == Modes)
					GameMode = 0;
			}

			if ((!CInput_PrevButtons.ButUp && CInput_Buttons.ButUp) ||
				(!CInput_PrevButtons.ButUp2 && CInput_Buttons.ButUp2) ||
				(!CInput_PrevButtons.ButDpadUp && CInput_Buttons.ButDpadUp))
			{
				CAudio_PlaySound(SfxSelect, 0);

				GameMode -= 1;
				if (GameMode == -1)
					GameMode = Modes - 1;
			}

			if ((!CInput_PrevButtons.ButB && CInput_Buttons.ButB) ||
				(!CInput_PrevButtons.ButBack && CInput_Buttons.ButBack))
			{
				CAudio_PlaySound(SfxBack, 0);
				CurrentMainMenu = -1;
			}

			if ((!CInput_PrevButtons.ButStart && CInput_Buttons.ButStart) ||
				(!CInput_PrevButtons.ButA && CInput_Buttons.ButA))
			{
				CAudio_PlaySound(SfxConfirm, 0);

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

			strcpy(Text, "Retro Time");
			CFont_WriteText(Renderer, "Roboto-Regular", 80*yscale, Text, strlen(Text), 465*xscale, 50*yscale, 0, color);

			int MenuIterator;
			for (int i = 0; i < MainMenus; i++)
			{
				MenuIterator = MMMainMenus[i].menu;
				if (MenuIterator == SelMenu)
					color.a = 255;
				else
					color.a = 64;
				CFont_WriteText(Renderer, "Roboto-Regular", menutextsize, MMMainMenus[i].name, strlen(MMMainMenus[i].name), 500*xscale, 150*yscale + i * menuspacing, 0, color);
			}

			if ((!CInput_PrevButtons.ButDown && CInput_Buttons.ButDown) ||
				(!CInput_PrevButtons.ButDown2 && CInput_Buttons.ButDown2) ||
				(!CInput_PrevButtons.ButDpadDown && CInput_Buttons.ButDpadDown))
			{
				CAudio_PlaySound(SfxSelect, 0);
				SelectedMenu += 1;
				if (SelectedMenu == MainMenus)
					SelectedMenu = 0;
			}

			if ((!CInput_PrevButtons.ButUp && CInput_Buttons.ButUp) ||
				(!CInput_PrevButtons.ButUp2 && CInput_Buttons.ButUp2) ||
				(!CInput_PrevButtons.ButDpadUp && CInput_Buttons.ButDpadUp))

			{
				CAudio_PlaySound(SfxSelect, 0);

				SelectedMenu -= 1;
				if(SelectedMenu == -1)
					SelectedMenu = MainMenus - 1;
			}

			if ((!CInput_PrevButtons.ButStart && CInput_Buttons.ButStart) ||
				(!CInput_PrevButtons.ButA && CInput_Buttons.ButA))
			{
				CurrentMainMenu = SelMenu;
				CAudio_PlaySound(SfxConfirm, 0);
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
