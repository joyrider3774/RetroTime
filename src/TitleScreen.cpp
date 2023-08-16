#include <SDL.h>
#include <json/json.h>
#include <iomanip>
#include <string>
#include "CGame.h"
#include "Common.h"
#include "TitleScreen.h"
#include "Vec2F.h"
#include "CHttp.h"

int SelectedMenu = 0;
int CurrentMainMenu = -1;
int SelMenu = 0;
int SelOptions = 0;
int OnlineScoreMode = 0;
int OnlineScoreGame = Games;
HttpData *OnlineScoreData = NULL;
Json::Value OnlineScoreJson = Json::nullValue;
int OnlineScoreStart = 0;
constexpr int OnlineScoreMaxHighScores = 100;
constexpr int OnlineScoreMaxHighScoresPerPage = 10;

constexpr int menutextsize = 50;
constexpr int menuspacing = 60;
constexpr int rcolor = 64;
constexpr int rdcolor = 1;

void RequestScoresOnline(CGame *Game, int GameNr, int GameMode)
{
	if(OnlineScoreData)
	{
		if(!OnlineScoreData->done)
		{
			Game->http->CancelRequest(OnlineScoreData);
		}

		Game->http->DestroyData(&OnlineScoreData);
	}
	std::string Url = "Https://scores.joyrider3774.xyz/leaderboard.json?score_limit=" + std::to_string(OnlineScoreMaxHighScores) +"&game=" + std::to_string(LeaderBoardIds[GameNr][GameMode]);
	OnlineScoreData = Game->http->AddRequest(Url.c_str(), false);
	OnlineScoreJson = Json::nullValue;
	OnlineScoreStart = 0;
}

void InitTitleScreen(CGame *Game)
{
	Game->Audio->PlayMus(Game->MusMenu, -1);
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
	SDL_SetRenderDrawColor(Game->Renderer, 25, 25, 255, 235);
	//so we can can copy the transparant part with the blue and text from this image
	SDL_Point FramePos = {ScreenWidth / 2, ScreenHeight / 2};
	Vec2F FrameScale = {10.6f / 4, 10.6f};
	Game->Image->DrawImageFuze(Game->Renderer, Game->GFXFrameID, true, &FramePos, 0, &FrameScale, 255, 255, 255, 240);

	std::string Text = "";
	switch (CurrentMainMenu)
	{
		case MMOptions:
		{
			int selectedmenu = 0;
			int menutextsize = 35;
			int menuspacing = 45;

			selectedmenu = OMOptionMenus[SelOptions].menu;
			Text = "Options";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 60, Text, Text.length(), 535, 110, 0, {255, 255, 255, 255});

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
					case OMMotionBlur:
						if (Game->MotionBlur)
						{
							Text = OMOptionMenus[menu].name + "Yes";
							Game->Font->WriteText(Game->Renderer, "Roboto-Regular", menutextsize, Text, Text.length(), 400, 185 + i * menuspacing, 0, color);
						}
						else
						{
							Text = OMOptionMenus[menu].name + "No";
							Game->Font->WriteText(Game->Renderer, "Roboto-Regular", menutextsize, Text, Text.length(), 400, 185 + i * menuspacing, 0, color);
						}
						break;
					case OMSoundVol:
						Text = OMOptionMenus[menu].name + std::to_string((int)(Game->Audio->GetVolumeSound()*100/128)) + "%";
						Game->Font->WriteText(Game->Renderer, "Roboto-Regular", menutextsize, Text, Text.length(), 400, 185 + i * menuspacing, 0, color);
						break;
					case OMMusicVol:
						Text = OMOptionMenus[menu].name + std::to_string((int)(Game->Audio->GetVolumeMusic()*100/128)) + "%";
						Game->Font->WriteText(Game->Renderer, "Roboto-Regular", menutextsize, Text, Text.length(), 400, 185 + i * menuspacing, 0, color);
						break;
					case OMCrt:
						Text = OMOptionMenus[menu].name + CrtOptions[Game->Crt].name;
						Game->Font->WriteText(Game->Renderer, "Roboto-Regular", menutextsize, Text, Text.length(), 400, 185 + i * menuspacing, 0, color);
						break;
					case OMColorModR:
						Text = OMOptionMenus[menu].name + " " + std::to_string((int)Game->ColorModR * 100 / 255) + "%";
						Game->Font->WriteText(Game->Renderer, "Roboto-Regular", menutextsize, Text, Text.length(), 400, 185 + i * menuspacing, 0, color);
						break;
					case OMColorModG:
						Text = OMOptionMenus[menu].name + " " + std::to_string((int)Game->ColorModG * 100 / 255) + "%";
						Game->Font->WriteText(Game->Renderer, "Roboto-Regular", menutextsize, Text, Text.length(), 400, 185 + i * menuspacing, 0, color);
						break;
					case OMColorModB:
						Text = OMOptionMenus[menu].name + " " + std::to_string((int)Game->ColorModB * 100 / 255) + "%";
						Game->Font->WriteText(Game->Renderer, "Roboto-Regular", menutextsize, Text, Text.length(), 400, 185 + i * menuspacing, 0, color);
						break;
					default:
						Game->Font->WriteText(Game->Renderer, "Roboto-Regular", menutextsize, OMOptionMenus[menu].name, OMOptionMenus[menu].name.length(), 400,
							185 + i * menuspacing, 0, color);
						break;
				}
			}
			Text = "Use left joystick or dpad to switch between options. (A) to select and (B) for back";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 15, Text, Text.length(), 380, 600, 0, {255, 255, 255, 255});

			if ((!Game->Input->PrevButtons.ButDown && Game->Input->Buttons.ButDown) ||
				(!Game->Input->PrevButtons.ButDown2 && Game->Input->Buttons.ButDown2) ||
				(!Game->Input->PrevButtons.ButDpadDown && Game->Input->Buttons.ButDpadDown))
			{
				Game->Audio->PlaySnd(Game->SfxSelect, 0);
				SelOptions += 1;
				if (SelOptions == OptionMenus)
					SelOptions = 0;
			}

			if ((!Game->Input->PrevButtons.ButUp && Game->Input->Buttons.ButUp) ||
				(!Game->Input->PrevButtons.ButUp2 && Game->Input->Buttons.ButUp2) ||
				(!Game->Input->PrevButtons.ButDpadUp && Game->Input->Buttons.ButDpadUp))
			{
				Game->Audio->PlaySnd(Game->SfxSelect, 0);

				SelOptions -= 1;
				if( SelOptions == -1)
					SelOptions = OptionMenus - 1;
			}

			if ((!Game->Input->PrevButtons.ButLeft && Game->Input->Buttons.ButLeft) ||
				(!Game->Input->PrevButtons.ButLeft2 && Game->Input->Buttons.ButLeft2) ||
				(!Game->Input->PrevButtons.ButDpadLeft && Game->Input->Buttons.ButDpadLeft))
			{
				Game->Audio->PlaySnd(Game->SfxSelect, 0);
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
							Game->Audio->PlayMus(Game->MusMenu, -1);
						break;

					case OMMotionBlur:
						Game->MotionBlur = !Game->MotionBlur;
						break;

					case OMCrt:
						Game->Crt -= 1;
						if(Game->Crt < 0)
							Game->Crt = Crts -1;
						Game->ReCreateCrt();
						break;

					case OMColorModR:
						Game->ColorModR -= 5;
						if(Game->ColorModR < 15)
							Game->ColorModR = 15;
						break;

					case OMColorModG:
						Game->ColorModG -= 5;
						if(Game->ColorModG < 15)
							Game->ColorModG = 15;
						break;

					case OMColorModB:
						Game->ColorModB -= 5;
						if(Game->ColorModB < 15)
							Game->ColorModB = 15;
						break;
				}
				//createcrt(global.crt)
				//savehighscoresoptions()
			}

			if ((!Game->Input->PrevButtons.ButRight && Game->Input->Buttons.ButRight) ||
				(!Game->Input->PrevButtons.ButRight2 && Game->Input->Buttons.ButRight2) ||
				(!Game->Input->PrevButtons.ButDpadRight && Game->Input->Buttons.ButDpadRight))
			{
				Game->Audio->PlaySnd(Game->SfxSelect, 0);
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
							Game->Audio->PlayMus(Game->MusMenu, -1);
						break;
					case OMMotionBlur:
						Game->MotionBlur = !Game->MotionBlur;
						break;
					case OMCrt:
						Game->Crt += 1;
						if(Game->Crt == Crts)
							Game->Crt = 0;
						Game->ReCreateCrt();
						break;

					case OMColorModR:
						Game->ColorModR += 5;
						if(Game->ColorModR > 255)
							Game->ColorModR = 255;
						break;

					case OMColorModG:
						Game->ColorModG += 5;
						if(Game->ColorModG > 255)
							Game->ColorModG = 255;
						break;

					case OMColorModB:
						Game->ColorModB += 5;
						if(Game->ColorModB > 255)
							Game->ColorModB = 255;
						break;
				}
					//createcrt(global.crt)
				//savehighscoresoptions()
			}


			if ((!Game->Input->PrevButtons.ButBack && Game->Input->Buttons.ButBack) ||
				(!Game->Input->PrevButtons.ButB && Game->Input->Buttons.ButB))
			{
				Game->Audio->PlaySnd(Game->SfxBack, 0);

				CurrentMainMenu = -1;
			}

			if ((!Game->Input->PrevButtons.ButStart && Game->Input->Buttons.ButStart) ||
				(!Game->Input->PrevButtons.ButA && Game->Input->Buttons.ButA))
			{
				Game->Audio->PlaySnd(Game->SfxSelect, 0);
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
							Game->Audio->PlayMus(Game->MusMenu, -1);
						break;
					case OMMotionBlur:
						Game->MotionBlur = !Game->MotionBlur;
						break;
					case OMCrt:
						Game->Crt += 1;
						if (Game->Crt == Crts)
							Game->Crt = 0;
						Game->ReCreateCrt();
						break;

					case OMColorModR:
						Game->ColorModR += 5;
						if(Game->ColorModR > 255)
							Game->ColorModR = 15;
						break;

					case OMColorModG:
						Game->ColorModG += 5;
						if(Game->ColorModG > 255)
							Game->ColorModG = 15;
						break;

					case OMColorModB:
						Game->ColorModB += 5;
						if(Game->ColorModB > 255)
							Game->ColorModB = 15;
						break;
				}
				//createcrt(global.crt)
				//savehighscoresoptions()
			}
			break;
		}


		case MMHelp:
		{
			Text = "Help";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 60, Text, Text.length(), 570, 110, 0, {255, 255, 255, 255});

			Text = "Retro time is a collection of retro games, each playable in different game modes.\n"
				"\n"
				"You can control players in games with either the left joystick or the dpad\n"
				"Some games play better with the dpad, some with the joystick. Extra actions are\n"
				"available, using (A) and (B), depending on the game.\n"
				"\n"
				"During gameplay you can access the pause menu, by pressing (Start) button\n"
				"Pressing it again will resume gameplay.\n"
				"\n"
				"There are 3 game modes: Retro Carousel, Time Attack and Lives mode.\n"
				"\n";
			Text += GMModes[0].description + "\n\n" + GMModes[1].description + "\n\n" + GMModes[2].description;
			int tw = Game->Font->TextWidth("Roboto-Regular", 17, Text, Text.length());
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 17, Text, Text.length(), (ScreenWidth - tw)/2, 180, 0, {255, 255, 255, 255});


			if ((!Game->Input->PrevButtons.ButA && Game->Input->Buttons.ButA) ||
				(!Game->Input->PrevButtons.ButB && Game->Input->Buttons.ButB) ||
				(!Game->Input->PrevButtons.ButBack && Game->Input->Buttons.ButBack) ||
				(!Game->Input->PrevButtons.ButStart && Game->Input->Buttons.ButStart))
			{
				Game->Audio->PlaySnd(Game->SfxBack, 0);

				CurrentMainMenu = -1;
			}
			break;
		}
		case MMCredits:
		{
			Text = "Credits";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 60, Text, Text.length(), 540, 110, 0, {255, 255, 255, 255});

			Text =
			"Block Stacker (logic) code is heavily inspired on this video from one lone coder (javidx9)\n"
			"with my own adaptations:\n"
			"https://youtu.be/8OK8_tHeCIA\n"
			"\n"
			"Snakey (logic) code is based on an edureka article (by Wajiha Urooj) about making snake in\n"
			"python:\n"
			"https://www.edureka.co/blog/snake-game-with-pygame\n"
			"\n"
			"Brick Breaker ball / block collision uses a detection seen in wireframe magazine nr 11:\n"
			"https://wireframe.raspberrypi.org\n"
			"\n"
			"Toady water and grass background tiles have been created by me, Willems Davy aka joyrider3774\n"
			"feel free to use them. Music is created, payed and owned by me using Strofe:\n"
			"https://www.strofe.com\n"
			"\n"
			"All other assets (including sound) come from either payed or free asset packs.\n"
			"For a complete list of credits check the github or itch.io page:\n"
			"https://github.com/joyrider3774/RetroTime or https://joyrider3774.itch.io/retrotime\n"
			"\n";

			int tw = Game->Font->TextWidth("Roboto-Regular", 17, Text, Text.length());
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 17, Text, Text.length(), (ScreenWidth - tw) / 2, 225, 0, {255, 255, 255, 255});

			if ((!Game->Input->PrevButtons.ButA && Game->Input->Buttons.ButA) ||
				(!Game->Input->PrevButtons.ButB && Game->Input->Buttons.ButB) ||
				(!Game->Input->PrevButtons.ButBack && Game->Input->Buttons.ButBack) ||
				(!Game->Input->PrevButtons.ButStart && Game->Input->Buttons.ButStart))
			{
				Game->Audio->PlaySnd(Game->SfxBack, 0);

				CurrentMainMenu = -1;
			}
			break;
		}

		case MMOnlineHighScores:
		{
			if ((!Game->Input->PrevButtons.ButDown && Game->Input->Buttons.ButDown) ||
				(!Game->Input->PrevButtons.ButDown2 && Game->Input->Buttons.ButDown2) ||
				(!Game->Input->PrevButtons.ButDpadDown && Game->Input->Buttons.ButDpadDown))
			{
				Game->Audio->PlaySnd(Game->SfxSelect, 0);
				if(OnlineScoreStart < OnlineScoreMaxHighScores - OnlineScoreMaxHighScoresPerPage)
					OnlineScoreStart+=OnlineScoreMaxHighScoresPerPage;
				
			}

			if ((!Game->Input->PrevButtons.ButUp && Game->Input->Buttons.ButUp) ||
				(!Game->Input->PrevButtons.ButUp2 && Game->Input->Buttons.ButUp2) ||
				(!Game->Input->PrevButtons.ButDpadUp && Game->Input->Buttons.ButDpadUp))
			{
				Game->Audio->PlaySnd(Game->SfxSelect, 0);
				if(OnlineScoreStart > 0)
					OnlineScoreStart-=OnlineScoreMaxHighScoresPerPage;
				
			}

			if ((!Game->Input->PrevButtons.ButLeft && Game->Input->Buttons.ButLeft) ||
				(!Game->Input->PrevButtons.ButLeft2 && Game->Input->Buttons.ButLeft2) ||
				(!Game->Input->PrevButtons.ButDpadLeft && Game->Input->Buttons.ButDpadLeft))
			{
				Game->Audio->PlaySnd(Game->SfxSelect, 0);
				if(OnlineScoreGame == Games)
				{
					OnlineScoreGame = Games-1;
					OnlineScoreMode = Modes-1;
				}
				else
				{
					OnlineScoreMode--;
					if(OnlineScoreMode == -1)
					{
						OnlineScoreMode = Modes-1;
						OnlineScoreGame--;
						if(OnlineScoreGame == -1)
						{
							OnlineScoreGame = Games;
						}
					}
				}
				RequestScoresOnline(Game, OnlineScoreGame, OnlineScoreMode);
			}

			if ((!Game->Input->PrevButtons.ButRight && Game->Input->Buttons.ButRight) ||
				(!Game->Input->PrevButtons.ButRight2 && Game->Input->Buttons.ButRight2) ||
				(!Game->Input->PrevButtons.ButDpadRight && Game->Input->Buttons.ButDpadRight))
			{
				Game->Audio->PlaySnd(Game->SfxSelect, 0);
				if(OnlineScoreGame == Games)
				{
					OnlineScoreGame = 0;
					OnlineScoreMode = 0;
				}
				else
				{
					OnlineScoreMode++;
					if(OnlineScoreMode == Modes)
					{
						OnlineScoreMode = 0;
						OnlineScoreGame++;
					}
				}
				RequestScoresOnline(Game, OnlineScoreGame, OnlineScoreMode);
			}

			if ((!Game->Input->PrevButtons.ButBack && Game->Input->Buttons.ButBack) ||
				(!Game->Input->PrevButtons.ButB && Game->Input->Buttons.ButB))
			{
				Game->Audio->PlaySnd(Game->SfxBack, 0);
				if(OnlineScoreData)
				{
					if(!OnlineScoreData->done)
					{
						Game->http->CancelRequest(OnlineScoreData);
					}

					Game->http->DestroyData(&OnlineScoreData);
					OnlineScoreJson = Json::nullValue;
				}
				CurrentMainMenu = -1;
			}

			if ((!Game->Input->PrevButtons.ButStart && Game->Input->Buttons.ButStart) ||
				(!Game->Input->PrevButtons.ButA && Game->Input->Buttons.ButA))
			{
				if(OnlineScoreGame == Games)
				{
					OnlineScoreGame = 0;
					OnlineScoreMode = 0;
				}
				else
				{
					OnlineScoreMode++;
					if(OnlineScoreMode == Modes)
					{
						OnlineScoreMode = 0;
						OnlineScoreGame++;
					}
				}
				RequestScoresOnline(Game, OnlineScoreGame, OnlineScoreMode);
				Game->Audio->PlaySnd(Game->SfxSelect, 0);
			}
			
			Text = "Online High Scores";
			int w = Game->Font->TextWidth("Roboto-Regular", 60, Text, Text.length());
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 60, Text, Text.length(), (ScreenWidth>>1) - (w >> 1) , 110, 0, {255, 255, 255, 255});
			if(OnlineScoreGame == Games)
				Text = "Retro Carousel - Total Score";
			else
				Text = GSGames[OnlineScoreGame].name + " - " + GMModes[OnlineScoreMode].name;
			w = Game->Font->TextWidth("Roboto-Regular", 30, Text, Text.length());
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), (ScreenWidth>>1) - (w >> 1), 190, 0, {255, 255, 255, 255});
			
			if(!OnlineScoreData && (CurrentMainMenu == MMOnlineHighScores))
				RequestScoresOnline(Game, OnlineScoreGame, OnlineScoreMode);
			if(OnlineScoreData)
			{
				if(!OnlineScoreData->done)
				{
					Text = "Loading ...";
					w = Game->Font->TextWidth("Roboto-Regular", 30, Text, Text.length());
					Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), (ScreenWidth>>1) - (w >> 1), (ScreenHeight >> 1) , 0, {255, 255, 255, 255});			
				}
				else
				{
					if(OnlineScoreData->responsecode == 200)
					{
						if(OnlineScoreJson == Json::nullValue)
						{
							Json::Value json;
							std::stringstream(std::string(OnlineScoreData->memory)) >> json;
							Json::Value tmp;
							tmp = json.get("games", Json::nullValue);
							if(tmp != Json::nullValue)
							{
							 	if(tmp.size() == 1)
							 	{
							 		tmp = tmp[0];
							 		tmp = tmp.get("scores", Json::nullValue); 
							 		if(tmp != Json::nullValue)
							 		{
							 			OnlineScoreJson = tmp;
							 		}
								}
							}
						}

						if(OnlineScoreJson != Json::nullValue)
						{
							
							for(int i = OnlineScoreStart; i < std::min(OnlineScoreStart + 10, (int)OnlineScoreJson.size()); i++ )
							{
								Text = std::to_string(i+1) + ".";
								w = Game->Font->TextWidth("Roboto-Regular", 30, Text, Text.length());
								Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 350, 240 + ((i-OnlineScoreStart) * 35), 0, {255, 255, 255, 255});

							
								Text = OnlineScoreJson[i]["name"].asString().substr(0,15);
								w = Game->Font->TextWidth("Roboto-Regular", 30, Text, Text.length());
								Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 420, 240 + ((i-OnlineScoreStart) * 35), 0, {255, 255, 255, 255});

								Text = OnlineScoreJson[i]["score"].asString();
								w = Game->Font->TextWidth("Roboto-Regular", 30, Text, Text.length());
								Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 1000-w, 240 + ((i-OnlineScoreStart) * 35), 0, {255, 255, 255, 255});
							}
						}
						else
						{
							Text = "Failed Receiving Scores";
							w = Game->Font->TextWidth("Roboto-Regular", 30, Text, Text.length());
							Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), (ScreenWidth>>1) - (w >> 1), (ScreenHeight >> 1) , 0, {255, 255, 255, 255});								
						}
					}
					else
					{
						Text = "Failed Receiving Scores";
						w = Game->Font->TextWidth("Roboto-Regular", 30, Text, Text.length());
						Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), (ScreenWidth>>1) - (w >> 1), (ScreenHeight >> 1) , 0, {255, 255, 255, 255});								
					}
				}
			}
			break;
		}

		case MMLocalHighScores:
		{
			Text = "Local High Scores";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 60, Text, Text.length(), 465, 110, 0, {255, 255, 255, 255});

			Text = "Retro Carousel Total highscore: " + std::to_string(Game->RetroCarouselHighScore);
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 20, Text, Text.length(), 300, 195, 0, {255, 255, 255, 255});

			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, GSGames[Game->Game].name, GSGames[Game->Game].name.length(), 300, 240, 0, {255, 255, 255, 255});

			SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 255);
			SDL_Rect r = {300, 275, int(ScreenWidth * 0.25) + 4, int(ScreenHeight * 0.25) + 4};
			SDL_RenderFillRect(Game->Renderer, &r);

			SDL_Rect Dst = {302, 277, int(ScreenWidth * 0.25), int(ScreenHeight * 0.25)};
			SDL_Rect Src = {0, 0, ScreenWidth, ScreenHeight};
			SDL_RenderCopy(Game->Renderer, Game->GameScreenShots[Game->Game], &Src, &Dst);

			for(int mode = 0; mode < Modes; mode++)
			{
				Text = GMModes[mode].name + ": " + std::to_string(Game->HighScores[Game->Game][mode]);
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 20, Text, Text.length(), 300, 470 + mode * 25, 0, {255, 255, 255, 255});
			}

			Text = "Use left joystick, dpad or (A) to switch between games. (B) for back";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 15, Text, Text.length(), 400, 600, 0, {255, 255, 255, 255});

			 if ((!Game->Input->PrevButtons.ButBack && Game->Input->Buttons.ButBack) ||
				(!Game->Input->PrevButtons.ButB && Game->Input->Buttons.ButB))
			 {
				Game->Audio->PlaySnd(Game->SfxBack, 0);

				CurrentMainMenu = -1;
			 }

			if ((!Game->Input->PrevButtons.ButStart && Game->Input->Buttons.ButStart) ||
				(!Game->Input->PrevButtons.ButRight && Game->Input->Buttons.ButRight) ||
				(!Game->Input->PrevButtons.ButRight2 && Game->Input->Buttons.ButRight2) ||
				(!Game->Input->PrevButtons.ButDpadRight && Game->Input->Buttons.ButDpadRight) ||
				(!Game->Input->PrevButtons.ButA && Game->Input->Buttons.ButA))
			{
				Game->Audio->PlaySnd(Game->SfxSelect, 0);

				Game->Game += 1;
				if (Game->Game == Games)
					Game->Game = 0;
			}

			 if ((!Game->Input->PrevButtons.ButLeft && Game->Input->Buttons.ButLeft) ||
				 (!Game->Input->PrevButtons.ButLeft2 && Game->Input->Buttons.ButLeft2) ||
				 (!Game->Input->PrevButtons.ButDpadLeft && Game->Input->Buttons.ButDpadLeft))
			 {
				Game->Audio->PlaySnd(Game->SfxSelect, 0);

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
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 60, Text, Text.length(), 485, 110, 0, {255, 255, 255, 255});
			Text = GSGames[Game->Game].name;
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 180, 0, {255, 255, 255, 255});

			SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 255);
			SDL_Rect r = {300, 215, int(ScreenWidth * 0.25) + 4, int(ScreenHeight * 0.25) + 4};
			SDL_RenderFillRect(Game->Renderer, &r);

			SDL_Rect Dst = {302, 217, int(ScreenWidth * 0.25), int(ScreenHeight * 0.25)};
			SDL_Rect Src = {0, 0, ScreenWidth, ScreenHeight};
			SDL_RenderCopy(Game->Renderer, Game->GameScreenShots[Game->Game], &Src, &Dst);

			Text = GMModes[Game->GameMode].name + " High Score: " + std::to_string(Game->HighScores[Game->Game][Game->GameMode]);
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 20, Text, Text.length(), 300, 410, 0, {255, 255, 255, 255});

			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 20, GSGames[Game->Game].description, GSGames[Game->Game].description.length(), 300, 440, 2, {255, 255, 255, 255});

			Text = "Use left joystick or dpad to switch between games. (A) to select and (B) for back";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 15, Text, Text.length(), 330, 635, 0, {255, 255, 255, 255});

			if ((!Game->Input->PrevButtons.ButBack && Game->Input->Buttons.ButBack) ||
				(!Game->Input->PrevButtons.ButB && Game->Input->Buttons.ButB))
			{
				Game->Audio->PlaySnd(Game->SfxBack, 0);
				CurrentMainMenu = MMSelectGameMode;
			}

			if ((!Game->Input->PrevButtons.ButStart && Game->Input->Buttons.ButStart) ||
				(!Game->Input->PrevButtons.ButA && Game->Input->Buttons.ButA))
			{
				Game->Audio->PlaySnd(Game->SfxConfirm, 0);

				Game->GameState = gamestate;
				Game->ResetScores();
				CurrentMainMenu = -1;
				Game->Input->ResetButtons();
			}

			if ((!Game->Input->PrevButtons.ButLeft && Game->Input->Buttons.ButLeft) ||
				(!Game->Input->PrevButtons.ButLeft2 && Game->Input->Buttons.ButLeft2) ||
				(!Game->Input->PrevButtons.ButDpadLeft && Game->Input->Buttons.ButDpadLeft))
			{
				Game->Audio->PlaySnd(Game->SfxSelect, 0);

				Game->Game -= 1;
				if (Game->Game == -1)
					Game->Game = Games - 1;
			}
			if ((!Game->Input->PrevButtons.ButRight && Game->Input->Buttons.ButRight) ||
				(!Game->Input->PrevButtons.ButRight2 && Game->Input->Buttons.ButRight2) ||
				(!Game->Input->PrevButtons.ButDpadRight && Game->Input->Buttons.ButDpadRight))
			{
				Game->Audio->PlaySnd(Game->SfxSelect, 0);

				Game->Game += 1;
				if (Game->Game == Games)
					Game->Game = 0;
			}
			break;
		}
		case MMSelectGameMode:
		{
			std::string Text = "Select Game Mode";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 60, Text, Text.length(), 385, 110, 0, {255, 255, 255, 255});

			int ModeIterator;
			SDL_Color color;
			for(int i = 0; i < Modes; i++)
			{
				ModeIterator = GMModes[i].mode;
				if (ModeIterator == Game->GameMode)
					color = {255, 255, 255, 255};
				else
					color = {255, 255, 255, 64};
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", menutextsize, GMModes[i].name, GMModes[i].name.length(), 470, 225 + i * menuspacing, 0, color);
			}
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 20, GMModes[Game->GameMode].description, GMModes[Game->GameMode].description.length(),
				300, 450, 0, {255,255,255,255});

			Text = "Use left joystick or dpad to switch between game modes. (A) to select and (B) for back";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 15, Text, Text.length(), 330, 600, 0, {255,255,255,255});

			if ((!Game->Input->PrevButtons.ButDown && Game->Input->Buttons.ButDown) ||
				(!Game->Input->PrevButtons.ButDown2 && Game->Input->Buttons.ButDown2) ||
				(!Game->Input->PrevButtons.ButDpadDown && Game->Input->Buttons.ButDpadDown))
			{
				Game->Audio->PlaySnd(Game->SfxSelect, 0);
				Game->GameMode += 1;
				if (Game->GameMode == Modes)
					Game->GameMode = 0;
			}

			if ((!Game->Input->PrevButtons.ButUp && Game->Input->Buttons.ButUp) ||
				(!Game->Input->PrevButtons.ButUp2 && Game->Input->Buttons.ButUp2) ||
				(!Game->Input->PrevButtons.ButDpadUp && Game->Input->Buttons.ButDpadUp))
			{
				Game->Audio->PlaySnd(Game->SfxSelect, 0);

				Game->GameMode -= 1;
				if (Game->GameMode == -1)
					Game->GameMode = Modes - 1;
			}

			if ((!Game->Input->PrevButtons.ButB && Game->Input->Buttons.ButB) ||
				(!Game->Input->PrevButtons.ButBack && Game->Input->Buttons.ButBack))
			{
				Game->Audio->PlaySnd(Game->SfxBack, 0);
				CurrentMainMenu = -1;
			}

			if ((!Game->Input->PrevButtons.ButStart && Game->Input->Buttons.ButStart) ||
				(!Game->Input->PrevButtons.ButA && Game->Input->Buttons.ButA))
			{
				Game->Audio->PlaySnd(Game->SfxConfirm, 0);

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

			std::string Text = "Retro Time";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 60, Text, Text.length(), 485, 110, 0, {255, 255, 255, 255});

			int MenuIterator;
			SDL_Color color;
			for (int i = 0; i < MainMenus; i++)
			{
				MenuIterator = MMMainMenus[i].menu;
				if (MenuIterator == SelMenu)
					color = {255, 255, 255, 255};
				else
					color = {255, 255, 255, 64};
				Game->Font->WriteText(Game->Renderer, "Roboto-Regular", menutextsize, MMMainMenus[i].name, MMMainMenus[i].name.length(), 500, 200 + i * menuspacing, 0, color);
			}

			if ((!Game->Input->PrevButtons.ButDown && Game->Input->Buttons.ButDown) ||
				(!Game->Input->PrevButtons.ButDown2 && Game->Input->Buttons.ButDown2) ||
				(!Game->Input->PrevButtons.ButDpadDown && Game->Input->Buttons.ButDpadDown))
			{
				Game->Audio->PlaySnd(Game->SfxSelect, 0);
				SelectedMenu += 1;
				if (SelectedMenu == MainMenus)
					SelectedMenu = 0;
			}

			if ((!Game->Input->PrevButtons.ButUp && Game->Input->Buttons.ButUp) ||
				(!Game->Input->PrevButtons.ButUp2 && Game->Input->Buttons.ButUp2) ||
				(!Game->Input->PrevButtons.ButDpadUp && Game->Input->Buttons.ButDpadUp))

			{
				Game->Audio->PlaySnd(Game->SfxSelect, 0);

				SelectedMenu -= 1;
				if(SelectedMenu == -1)
					SelectedMenu = MainMenus - 1;
			}

			if ((!Game->Input->PrevButtons.ButStart && Game->Input->Buttons.ButStart) ||
				(!Game->Input->PrevButtons.ButA && Game->Input->Buttons.ButA))
			{
				CurrentMainMenu = SelMenu;
				Game->Audio->PlaySnd(Game->SfxConfirm, 0);
				if (SelMenu == MMQuit)
					Game->GameState = GSQuit;

				if (SelMenu == MMStart)
					CurrentMainMenu = MMSelectGameMode;
				
			}
		}
	}

	SDL_Rect SrcRect = {230,85,820,550};
	//grab transparant part of frame + menu
	SDL_Texture *Tmp = SDL_CreateTexture(Game->Renderer, PixelFormat, SDL_TEXTUREACCESS_TARGET, SrcRect.w, SrcRect.h);
	SDL_Texture *TmpRender = SDL_GetRenderTarget(Game->Renderer);
	SDL_SetRenderTarget(Game->Renderer, Tmp);
	Game->Image->DrawImage(Game->Renderer, Game->TexOffScreen, &SrcRect, NULL);

	//draw the frame again without transparancy
	SDL_SetRenderTarget(Game->Renderer, TmpRender);
	Game->Image->DrawImageFuze(Game->Renderer, Game->GFXFrameID, true, &FramePos, 0, &FrameScale, 255, 255, 255, 255);

	//and then draw the transparant part over it now
	Game->Image->DrawImage(Game->Renderer, Tmp, NULL, &SrcRect);
	SDL_DestroyTexture(Tmp);

//	freeImage(surface)
//	stopChannel(0)
}
