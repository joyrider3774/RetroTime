#include <SDL.h>
#include <string.h>
#include "CGame.h"
#include "Common.h"
#include "TitleScreen.h"
#include "Types.h"

int SelectedMenu  = 0;
int CurrentMainMenu = -1;
int SelMenu = 0;
int SelOptions = 0;

constexpr int menutextsize = 50;
constexpr int menuspacing = 65;
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
    SDL_SetRenderDrawColor(Game->Renderer, 25, 25, 255, 235);
    //so we can can copy the transparant part with the blue and text from this image 	
    SDL_Point FramePos = {ScreenWidth / 2, ScreenHeight / 2};
    Vec2F FrameScale = {10.6f / 4, 10.6f};
    Game->Image->DrawImageFuze(Game->Renderer, Game->GFXFrameID, true, &FramePos, 0, &FrameScale, 255, 255, 255, 240);

    string Text = "";
    switch (CurrentMainMenu)
    {
        case MMOptions:
        {
            
            int selectedmenu = 0;
            int menutextsize = 50;
            int menuspacing = 65;

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
                        Text = OMOptionMenus[menu].name + to_string(Game->Audio->GetVolumeSound());
                        Game->Font->WriteText(Game->Renderer, "Roboto-Regular", menutextsize, Text, Text.length(), 400, 185 + i * menuspacing, 0, color);
                        break;
                    case OMMusicVol:
                        Text = OMOptionMenus[menu].name + to_string(Game->Audio->GetVolumeMusic());
                        Game->Font->WriteText(Game->Renderer, "Roboto-Regular", menutextsize, Text, Text.length(), 400, 185 + i * menuspacing, 0, color);
                        break;
                    case OMCrt:       
                        Text =  OMOptionMenus[menu].name + CrtOptions[Game->Crt].name;
                        Game->Font->WriteText(Game->Renderer, "Roboto-Regular", menutextsize, Text, Text.length(), 400, 185 + i * menuspacing, 0, color);
                        break;
                    default:
                        Game->Font->WriteText(Game->Renderer, "Roboto-Regular", menutextsize, OMOptionMenus[menu].name, OMOptionMenus[menu].name.length(), 400, 
                            185 + i * menuspacing, 0, color);
                        break;
                }
            }
            Text =  "Use left joystick or dpad to switch between options. (A) to select and (B) for back";
            Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 15, Text, Text.length(), 380, 600, 0, {255, 255, 255, 255});
            
            if (!Game->Input->PrevButtons.ButDown && Game->Input->Buttons.ButDown)
            {
                Game->Audio->PlaySound(Game->SfxSelect, 0);
                SelOptions += 1;
                if (SelOptions == OptionMenus)
                    SelOptions = 0;
            }
            
            if (!Game->Input->PrevButtons.ButUp && Game->Input->Buttons.ButUp)
            {
                Game->Audio->PlaySound(Game->SfxSelect, 0);

                SelOptions -= 1;
                if( SelOptions == -1)
                    SelOptions = OptionMenus - 1;
            }
            
            if (!Game->Input->PrevButtons.ButLeft && Game->Input->Buttons.ButLeft)
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

                    case OMMotionBlur:
                        Game->MotionBlur = !Game->MotionBlur;
                        break;

                    case OMCrt:
                        Game->Crt -= 1;
                        if(Game->Crt < 0)
                            Game->Crt = Crts -1;
                        Game->ReCreateCrt();
                        break;
                }
                //createcrt(global.crt)
                //savehighscoresoptions()
            }
								
            if (!Game->Input->PrevButtons.ButRight && Game->Input->Buttons.ButRight)
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
                    case OMMotionBlur:
                        Game->MotionBlur = !Game->MotionBlur;
                        break;
                    case OMCrt:
                        Game->Crt += 1;
                        if(Game->Crt == Crts)
                            Game->Crt = 0;
                        Game->ReCreateCrt();
                        break;
                }
                    //createcrt(global.crt)
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
                    case OMMotionBlur:
                        Game->MotionBlur = !Game->MotionBlur;
                        break;
                    case OMCrt:
                        Game->Crt += 1;
                        if (Game->Crt == Crts)
                            Game->Crt = 0;
                        Game->ReCreateCrt();
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
  		
			Text =    
		        "Retro time is a collection of retro games, each playable in different game modes.\n"
                "\n"
                "You can control players in games with either the left joystick or the dpad\n"
		        "Some games play better with the dpad, some with the joystick. Extra actions are\n"
                "available, using (A) and (B), depending on the game.\n"
                "\n"
				"During gameplay you can access the pause menu, by pressing (LB) + (RB) at the\n"
                "same time. Pressing it again will resume gameplay.\n"
                "\n"
				"There are 3 game modes: Retro Carousel, Time Attack and Lives mode.\n"
                "\n";
		    Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 17, Text, Text.length(), 320, 180, 0, {255, 255, 255, 255});
            
            Text = GMModes[0].description + "\n\n" + GMModes[1].description + "\n\n" + GMModes[2].description;
            Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 17, Text, Text.length(), 320, 400, 0, {255, 255, 255, 255});

		
		
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
            Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 60, Text, Text.length(), 540, 110, 0, {255, 255, 255, 255});

            Text = 
            "Block Stacker (logic) code is heavily inspired on this video from one lone\n"
		    "coder (javidx9) with my own adaptations and fuzification:\n"
		    "https://youtu.be/8OK8_tHeCIA\n"
		    "\n" 
		    "Snakey (logic) code is based on an edureka article (by Wajiha Urooj) about\n"
		    "making snake in python:\n"
		    "https://www.edureka.co/blog/snake-game-with-pygame/\n"
            "\n"
		    "Brick Breaker ball / block collision uses a detection seen in wireframe\n"
		    "magazine nr 11:\n"
		    "https://wireframe.raspberrypi.org\n"
            "\n"
            "Toady water and grass background tiles have been created by me,\n"
		    "Willems Davy aka joyrider3774 feel free to use them. All other assets\n"
		    "(including music and sound) are default assets provided by the FUZE program.\n"
            "\n";

			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 20, Text, Text.length(), 300, 190, 0, {255, 255, 255, 255});
    		
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
            Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 60, Text, Text.length(), 485, 110, 0, {255, 255, 255, 255});

            Text = "Retro Carousel Total highscore: " + to_string(Game->RetroCarouselHighScore);
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
                Text = GMModes[mode].name + ": " +  to_string(Game->HighScores[Game->Game][mode]);
                Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 20, Text, Text.length(), 300, 470 + mode * 25, 0, {255, 255, 255, 255});		
            }

            Text = "Use left joystick, dpad or (A) to switch between games. (B) for back";
            Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 15, Text, Text.length(), 400, 600, 0, {255, 255, 255, 255});
		
             if ((!Game->Input->PrevButtons.ButBack && Game->Input->Buttons.ButBack) ||
                (!Game->Input->PrevButtons.ButB && Game->Input->Buttons.ButB))
             {
                Game->Audio->PlaySound(Game->SfxBack, 0);

                CurrentMainMenu = -1;
             }
            
            if ((!Game->Input->PrevButtons.ButStart && Game->Input->Buttons.ButStart) ||
                (!Game->Input->PrevButtons.ButRight && Game->Input->Buttons.ButRight) ||
                (!Game->Input->PrevButtons.ButA && Game->Input->Buttons.ButA))
            {
                Game->Audio->PlaySound(Game->SfxSelect, 0);

                Game->Game += 1;
                if (Game->Game == Games)
                    Game->Game = 0;   
            }

             if (!Game->Input->PrevButtons.ButLeft && Game->Input->Buttons.ButLeft)
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
            Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 60, Text, Text.length(), 485, 110, 0, {255, 255, 255, 255});			    	
		    Text = GSGames[Game->Game].name;
            Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 180, 0, {255, 255, 255, 255});
            
            SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 255);
            SDL_Rect r = {300, 215, int(ScreenWidth * 0.25) + 4, int(ScreenHeight * 0.25) + 4};
            SDL_RenderFillRect(Game->Renderer, &r);
            
            SDL_Rect Dst = {302, 217, int(ScreenWidth * 0.25), int(ScreenHeight * 0.25)};
            SDL_Rect Src = {0, 0, ScreenWidth, ScreenHeight};
            SDL_RenderCopy(Game->Renderer, Game->GameScreenShots[Game->Game], &Src, &Dst);
            
            Text = GMModes[Game->GameMode].name + " High Score: "  + to_string(Game->HighScores[Game->Game][Game->GameMode]);
            Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 20, Text, Text.length(), 300, 410, 0, {255, 255, 255, 255});

            Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 20, GSGames[Game->Game].description, GSGames[Game->Game].description.length(), 300, 440, 2, {255, 255, 255, 255});
            
            Text = "Use left joystick or dpad to switch between games. (A) to select and (B) for back";
            Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 15, Text, Text.length(), 330, 635, 0, {255, 255, 255, 255});
                                   
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

            if (!Game->Input->PrevButtons.ButLeft && Game->Input->Buttons.ButLeft)
            {
                Game->Audio->PlaySound(Game->SfxSelect, 0);

                Game->Game -= 1;
                if (Game->Game == -1)
                    Game->Game = Games - 1;
            }
            if (!Game->Input->PrevButtons.ButRight && Game->Input->Buttons.ButRight)
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
		
            if (!Game->Input->PrevButtons.ButDown && Game->Input->Buttons.ButDown)
            {
                Game->Audio->PlaySound(Game->SfxSelect, 0);
                Game->GameMode += 1;
                if (Game->GameMode == Modes)
                    Game->GameMode = 0;
            }
		
		    if (!Game->Input->PrevButtons.ButUp && Game->Input->Buttons.ButUp)
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
            
            if (!Game->Input->PrevButtons.ButDown && Game->Input->Buttons.ButDown)
            {
                Game->Audio->PlaySound(Game->SfxSelect, 0);
                SelectedMenu += 1;
                if (SelectedMenu == MainMenus)
                    SelectedMenu = 0;
            }
            
            if (!Game->Input->PrevButtons.ButUp && Game->Input->Buttons.ButUp)
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
