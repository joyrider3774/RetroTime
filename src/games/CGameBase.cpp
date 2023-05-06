#include "CGameBase.h"

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

void CGameBase::PauseMenu()
{
	int prevsubgamestate = Game->SubGameState;
	int prevsubstatecounter = Game->SubStateCounter;
	Game->SubGameState = SGPauseMenu;
    Game->Audio->PlaySound(Game->SfxConfirm, 0);
    //subgamestate = sgframe;
	//global.substatecounter = 10.6
	int selected  = 0;
	int selectedmenu = 0;
	int maxmenus = 7;	
	int menutextsize = 45;
	int menuspacing = 55;
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
        SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 128);
        SDL_RenderFillRect(Game->Renderer, NULL);
		
         //draw everything to offscreen surface
        SDL_SetRenderDrawColor(Game->Renderer, 25, 25, 255, 235);
        //so we can can copy the transparant part with the blue and text from this image 	
        SDL_Point FramePos = {ScreenWidth / 2, ScreenHeight / 2};
        SDL_FPoint FrameScale = {10.6f / 4, 10.6f};
        Game->Image->DrawImageFuze(Game->Renderer, Game->GFXFrameID, true, &FramePos, 0, &FrameScale, 255, 255, 255, 240);
		
        if (Game->SubGameState == SGPauseMenu)
        {
            string Text = "Paused";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 60, Text, Text.length(), 530, 110, 0, {255,255,255,255});
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
                    case PMMotionBlur:
					    if (Game->MotionBlur)
                            Text = PMPauseMenus[menu].name + "Yes";
					    else
                            Text = PMPauseMenus[menu].name + "No";
			            Game->Font->WriteText(Game->Renderer, "Roboto-Regular", menutextsize, Text, Text.length(), 300, 180 + i * menuspacing, 0, color);	
                        break;
                    case PMSoundVol:
					    Text = PMPauseMenus[menu].name + to_string(Game->Audio->GetVolumeSound());
			            Game->Font->WriteText(Game->Renderer, "Roboto-Regular", menutextsize, Text, Text.length(), 300, 180 + i * menuspacing, 0, color);	
                        break;
                    case PMMusicVol:
					    Text = PMPauseMenus[menu].name + to_string(Game->Audio->GetVolumeMusic());
			            Game->Font->WriteText(Game->Renderer, "Roboto-Regular", menutextsize, Text, Text.length(), 300, 180 + i * menuspacing, 0, color);	
                        break;
					case PMCrt:
					    Text = PMPauseMenus[menu].name + CrtOptions[Game->Crt].name;
			            Game->Font->WriteText(Game->Renderer, "Roboto-Regular", menutextsize, Text, Text.length(), 300, 180 + i * menuspacing, 0, color);	
                        break;
                    default:
                        Game->Font->WriteText(Game->Renderer, "Roboto-Regular", menutextsize, PMPauseMenus[menu].name, PMPauseMenus[menu].name.length(), 300, 180 + i * menuspacing, 0, color);	
                        break;
                }
            }
			Text = "Press (A) to select, Left joystick or dpad change values";
			Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 15, Text, Text.length(), 330, 635, 0, {1,1,1,1});	
        }

        if (Game->SubGameState == SGGameHelp)
        {
            string Text = "Game Help";
            Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 60, Text, Text.length(), 505, 110, 0, {255,255,255,255});			

            Text = GSGames[Game->Game].name;
            Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 30, Text, Text.length(), 300, 180, 0, {255,255,255,255});	

            SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 255);
            SDL_Rect r = {300, 215, int(ScreenWidth * 0.25) + 4, int(ScreenHeight * 0.25) + 4};
            SDL_RenderFillRect(Game->Renderer, &r);
            
            SDL_Rect Dst = {302, 217, int(ScreenWidth * 0.25), int(ScreenHeight * 0.25)};
            SDL_Rect Src = {0, 0, ScreenWidth, ScreenHeight};
            SDL_RenderCopy(Game->Renderer, Game->GameScreenShots[Game->Game], &Src, &Dst);
            
            Text = GMModes[Game->GameMode].name + " High Score: " + to_string(Game->HighScores[Game->Game][Game->GameMode]);
            Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 20, Text, Text.length(), 300, 410, 0, {255,255,255,255});	

            Text = GSGames[Game->Game].description;
            Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 20, Text, Text.length(), 300, 440, 0, {255,255,255,255});	

            Text = "Use (A) or (B) for back";
            Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 15, Text, Text.length(), 580, 635, 0, {255,255,255,255});
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
            if (!Game->Input->PrevButtons.ButLeft && Game->Input->Buttons.ButLeft)
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

                    case PMMotionBlur:
                    {
                        Game->MotionBlur = !Game->MotionBlur;
                        break;
                    }
                    
                    case PMCrt:
                    {
                        Game->Crt -= 1;
                        if (Game->Crt < 0) 
                            Game->Crt = Crts -1;
                        Game->ReCreateCrt();
                        break;
                    }
                }
            }
            
            if (!Game->Input->PrevButtons.ButRight && Game->Input->Buttons.ButRight)
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

                    case PMMotionBlur:
                    {
                        Game->MotionBlur = !Game->MotionBlur;
                        break;
                    }

                    case PMCrt:
                    {
                        Game->Crt += 1;
                        if (Game->Crt == Crts)
                            Game->Crt = 0;
                        Game->ReCreateCrt();
                        break;
                    }
                }
            }		
                    
            if (!Game->Input->PrevButtons.ButDown && Game->Input->Buttons.ButDown)
            {
                Game->Audio->PlaySound(Game->SfxSelect, 0);

                selected += 1;
                if (selected == maxmenus)
                    selected = 0;
                
            }
            
            if (!Game->Input->PrevButtons.ButUp && Game->Input->Buttons.ButUp)
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

                    case PMMotionBlur:
                    {
                        Game->MotionBlur = ! Game->MotionBlur;
                        break;
                    }

                    case PMCrt:
                    {
                        Game->Crt += 1;
                        if (Game->Crt == Crts)
                            Game->Crt = 0;
                        Game->ReCreateCrt();
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

		SDL_SetRenderTarget(Game->Renderer, Game->TexScreen);
        SDL_RenderCopy(Game->Renderer, Game->TexOffScreen, NULL, NULL);
        SDL_SetRenderTarget(Game->Renderer, NULL);
        SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 255);
        SDL_RenderClear(Game->Renderer);
        SDL_RenderCopy(Game->Renderer, Game->TexScreen, NULL, NULL);       

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
            Game->Font->WriteText(Game->Renderer, "RobotoMono-Bold", 16, Text, Text.length(), ScreenWidth - tw, 0, 0, {255, 0, 255, 255});
        }
        SDL_RenderPresent(Game->Renderer);
        
        Uint64 FrameEndPerf = SDL_GetPerformanceCounter();
        Uint64 FramePerf = FrameEndPerf - FrameStartPerf;
        double FrameTime = FramePerf / (double)SDL_GetPerformanceFrequency() * 1000.0f;
        TotalFramePerf += FramePerf;

        if(SDL_GetTicks() - Ticks >= 1000)
        {
            AvgFrameTime = (TotalFramePerf / TotalFrames) / (double)SDL_GetPerformanceFrequency() * 1000.0f;
            Fps = TotalFrames;
            TotalFrames = 0;
            TotalFramePerf = 0;
            Ticks = SDL_GetTicks();
        }

        int RequiredDelay = 1000.0f/DesiredFps - FrameTime;
        if (RequiredDelay > 0)
          SDL_Delay(RequiredDelay);
    }
	Game->Input->ResetButtons();
    Game->ReCreateCrt();
    //savehighscoresoptions()
}

void CGameBase::DrawScoreBar()
{
    Game->DrawCrt();
    SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 100);
	SDL_Rect r = {0, 0, ScreenWidth, 22};
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
    Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 20, Text, Text.length(), 0, 0, 0, {255,255,255,255});
}

void CGameBase::OnGameStart()
{

}

void CGameBase::DrawSubStateText()
{
    //textSize(scpregamefontsize)
    //tz = textWidth(text)
	int w = Game->Font->TextWidth("Roboto-Regular", 60, SubStateText, SubStateText.length());
    Game->Font->WriteText(Game->Renderer, "Roboto-Regular", 60, SubStateText, SubStateText.length(), screenleft + ((screenright - screenleft) / 2) - w/2,
	    screentop + ((screenbottom - screentop) / 2) - 90, 0, {255, 255, 255, 240});
}

void CGameBase::UpdateLogic()
{
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
						Game->SubStateTime =  SDL_GetTicks() + 250;
					Game->SubStateCounter -= 1;
                }
                else
                {
					if (Game->SubGameState == SGReadyGo)
                    {
                        OnGameStart();
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

void CGameBase::Draw()
{

}

SDL_Texture* CGameBase::screenshot()
{
    return nullptr;
}