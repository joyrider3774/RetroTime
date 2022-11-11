#include <SDL.h>
#include "CGameSnake.h"

CGameSnake::CGameSnake(CGame* aGame, bool aScreenshotMode) : CGameBase(aGame, GSSnake, false)
{
    Game = aGame;
    ScreenshotMode = aScreenshotMode;
    MusMusic = -1;
    SfxFood = -1;
    SfxDie = -1;
    playfieldwidth = (cols) * snakesize;
    playfieldheight = (rows) * snakesize;
    screenleft = (ScreenWidth - playfieldwidth) / 2;
    screenright = screenleft + playfieldwidth;
    screentop = (ScreenHeight - playfieldheight) / 2;
    screenbottom = screentop + playfieldheight;
}

CGameSnake::~CGameSnake()
{

}

//food  ----------------------------------------------------------------------------------------------------------------

void CGameSnake::sscreatefood()
{
	bool bok = false;
	while (!bok) 
    {
		food = {screenleft + int((rand() % cols) *snakesize),screentop + int((rand() % rows) * snakesize)};
		bok = ((food.x != head.x) && (food.y != head.y));
		for (int i = 0; i < snakelength; i++)
			bok = bok && ((food.x != body[i].x) && (food.y != body[i].y));
    }
}

void CGameSnake::ssdrawfood()
{
    SDL_SetRenderDrawColor(Game->Renderer, snakefoodcolor.r, snakefoodcolor.g, snakefoodcolor.b, snakefoodcolor.a);
	SDL_Rect r = {food.x, food.y, snakesize, snakesize};
	SDL_RenderFillRect(Game->Renderer, &r);
}

void CGameSnake::ssupdatefood()
{
	if ((head.x == food.x) && (head.y == food.y))
    {
		Game->Audio->PlaySound(SfxFood, 0);
		snakelength += 1;
		sscreatefood();
        if(!ScreenshotMode)
		    Game->AddToScore(snakelength*2);
	}
}

//snake  ----------------------------------------------------------------------------------------------------------------

void CGameSnake::sscreatesnake()
{
	playerdeath = false;
	snakelength = 0;
	head = {screenleft + int(cols / 2) * snakesize,screentop + int(rows / 2) * snakesize};
	dir = {1,0};
	ticks = 0;
}

void CGameSnake::ssdrawsnake()
{
    SDL_Rect r;
    SDL_SetRenderDrawColor(Game->Renderer, snakebodycolor.r, snakebodycolor.g, snakebodycolor.b, snakebodycolor.a);
	for (int i = 0; i < snakelength; i++)
    {
        r = {body[i].x, body[i].y, snakesize, snakesize};
        SDL_RenderFillRect(Game->Renderer, &r);
    }
    SDL_SetRenderDrawColor(Game->Renderer, snakeheadcolor.r, snakeheadcolor.g, snakeheadcolor.b, snakeheadcolor.a);
	r = {head.x, head.y, snakesize, snakesize};
    SDL_RenderFillRect(Game->Renderer, &r);
}

void CGameSnake::ssupdatesnake()
{
	if(!ScreenshotMode)
    {
		if (Game->Input->Buttons.ButLeft)
        {
			if(movedone && dir.x == 0)
            {
				movedone = false;
				dir = {-1,0};
            }
        }
		else
        {
			if (Game->Input->Buttons.ButRight)
            {
				if(movedone && dir.x == 0)
                {
					movedone = false;
					dir = {1,0};
                }
            }
            else
            {
				if (Game->Input->Buttons.ButUp)
                {
					if(movedone && dir.y == 0)
                    {
						movedone = false;
						dir = {0,-1};
                    }
                }
				else
                {
					if (Game->Input->Buttons.ButDown)
                    {
						if(movedone && dir.y == 0)
						{	
                            movedone = false;
							dir = {0,1};
                        }
                    }
                }
            }
        }
    }		

    ticks += 1;
	if(ScreenshotMode || (ticks >= updateticks))
    {
		movedone = true;
		ticks = 0;
		int i = snakelength;
		while (i >= 1)
        {
			body[i] = body[i-1];
			i -= 1;
        }
		body[0] = head;
		head.x += dir.x * snakesize;
		head.y += dir.y * snakesize;
		if((head.x < screenleft) || (head.x >= screenright) ||
			(head.y < screentop) || (head.y >= screenbottom))
			playerdeath = true;

		for (int i = 0; i < snakelength; i++)
			if ((head.x == body[i].x) && (head.y == body[i].y))
				playerdeath = true;
						
	}
}

//background ----------------------------------------------------------------------------------------------------------------

void CGameSnake::ssdrawbackground()
{
    SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 255, 255);
	SDL_RenderClear(Game->Renderer);
	SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 255);
	SDL_Rect r = {screenleft, screentop, playfieldwidth, playfieldheight};
    SDL_RenderFillRect(Game->Renderer, &r);
}
		
SDL_Texture* CGameSnake::screenshot()
{
    SDL_Texture* prev = SDL_GetRenderTarget(Game->Renderer);
    SDL_Texture* image = SDL_CreateTexture(Game->Renderer, PixelFormat, SDL_TEXTUREACCESS_TARGET, ScreenWidth, ScreenHeight);
    SDL_SetRenderTarget(Game->Renderer, image);
	SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Game->Renderer);
    init();
    sscreatesnake();
	sscreatefood();
	for (int i = 0; i < 7; i++)
    {
		ssupdatesnake();
		snakelength += 1;
    }
	food = {screenleft + (int(cols / 2)-2) * snakesize, screentop + (int(rows / 2) -2) * snakesize};

	ssdrawbackground();
	ssdrawsnake();
	ssdrawfood();
    SDL_RenderPresent(Game->Renderer);
    SDL_SetRenderTarget(Game->Renderer, prev);
    deinit();
    return image;
}

//init - deinit ----------------------------------------------------------------------------------------------------------------

void CGameSnake::init()
{

    if(!ScreenshotMode)
    {
	    sscreatesnake();
	    sscreatefood();
    }
	
    movedone = true;
    if (!ScreenshotMode)
    {
        HealthPoints = 2;
        SfxFood = Game->Audio->LoadSound("snakey/food.wav");
        SfxDie = Game->Audio->LoadSound("common/die.wav");
        MusMusic = Game->Audio->LoadMusic("snakey/music.mp3");	
        Game->CurrentGameMusicID = MusMusic;
        Game->Audio->PlayMusic(MusMusic, -1);
    }
}

void CGameSnake::deinit()
{
    if (!ScreenshotMode)
    {
        Game->Audio->StopMusic();
        Game->Audio->StopSound();
        Game->Audio->UnLoadMusic(MusMusic);
        Game->Audio->UnLoadSound(SfxFood);
        Game->Audio->UnLoadSound(SfxDie);

        Game->SubStateCounter = 0;
        Game->SubGameState = SGNone;
        Game->CurrentGameMusicID = -1;
    }
}

//main loop ----------------------------------------------------------------------------------------------------------------
void CGameSnake::UpdateLogic()
{
	//initsnake()

    CGameBase::UpdateLogic();

    if (Game->SubGameState == SGGame)
    {
        ssupdatesnake();
        ssupdatefood();
    }

    if ((Game->SubGameState == SGGame) && playerdeath)
    {			
        Game->Audio->PlaySound(SfxDie, 0);
        if(!ScreenshotMode)
            Game->AddToScore(-50);
        //setdrawtarget(framebuffer)
        //drawimageex(screen,{screenwidth/2,screenheight/2},0,{1,1},{1,1,1,alpha})
        //update()
        //setdrawtarget(screen)		
        //sleep(1)
        if (HealthPoints > 1)
        {
            sscreatesnake();
            sscreatefood();
            if (Game->GameMode == GMGame)
                HealthPoints -= 1;
            Game->SubGameState = SGReadyGo;
            Game->SubStateTime = SDL_GetTicks() + 500;
        }
        else
            if(Game->GameMode == GMGame)
                if (!ScreenshotMode)
                    HealthPoints -= 1;
    }

  
}
void CGameSnake::Draw()
{
	SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Game->Renderer);
    ssdrawbackground();
    ssdrawfood();
    ssdrawsnake();
    DrawScoreBar();
    DrawSubStateText();    
}