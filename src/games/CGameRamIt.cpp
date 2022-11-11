#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <limits.h>
#include <string>
#include <iostream>
#include "CGameRamIt.h"
#include "..\CGame.h"
#include "..\Common.h"

using namespace std;

CGameRamIt::CGameRamIt(CGame* aGame, bool aScreenshotMode): CGameBase(aGame, GSRamIt, true)
{
    Game = aGame;
	ScreenshotMode = aScreenshotMode;
    speed = 0;
	speedcount = 0;
	SfxShoot = -1;
	SfxHit = -1;
	SfxDie = -1;
	SfxSucces = -1;
	MusMusic = -1;
	riblocksize = int(ScreenHeight / (numblocks+1)) - 2 * blockspacing;
    riblocksizespacing = riblocksize + 2 * blockspacing;
	playfieldwidth = ScreenWidth - 1 * riblocksize;
    segmentwidth = int(((playfieldwidth / 2) / blocksegments));
    playfieldheight = numblocks * riblocksizespacing;
    screenleft = (ScreenWidth - playfieldwidth) / 2;
    screenright = screenleft + playfieldwidth;
    screentop = (ScreenHeight - playfieldheight) / 2;
    screenbottom = screentop + playfieldheight;   
}

CGameRamIt::~CGameRamIt() {}

bool CGameRamIt::ricreatebullet()
{
	bool result = false;
	if (!bulletalive) 
    {
		bulletalive = true;
		bulletpos = playerpos;
		bulletvel.x = playerdx * bulletspeed;
        bulletvel.y = 0;
		result = true;
    }
    return result;
}

//BULLET ----------------------------------------------------------------------------------------------------------------

void CGameRamIt::riupdatebullet()
{
	if (bulletalive)
    {
		bool destroybullet = false;
		bool playaudiobullet = false;
		for(int steps = 0; steps < bulletsteps; steps++)
        {
			bulletpos.x += bulletvel.x;
            bulletpos.y += bulletvel.y;
			if (bulletpos.x + riblocksize / 2 < screenleft)
				destroybullet = true;
			if (bulletpos.x - riblocksize / 2 > screenright)
				destroybullet = true;
		
			for (int side = 0; side < sides; side++)
            {
				for (int block = 0; block < numblocks; block++)
                {
					if (playfield[side][block].segments > 0)
                    {
						if ((bulletpos.y + riblocksize / 6 >= screentop + 
							block * riblocksizespacing + blockspacing) &&
							(bulletpos.y - riblocksize / 6 <= screentop + 
							block * riblocksizespacing + blockspacing + riblocksize))
                        {
						    if (side == 0)
                            {
								if ((bulletpos.x - riblocksize / 2) <= screenleft + 
									playfield[side][block].segments * segmentwidth)
                                {
									playfield[side][block].segments -= 1;
									if (playfield[side][block].segments == 0)
										if(!ScreenshotMode)
									    	Game->AddToScore((level + 2) * 2*playfield[side][block].maxsegments);

									playaudiobullet = true;
									destroybullet = true;
                                }
                            }
							else
                            {
								if ((bulletpos.x - riblocksize / 2) >= screenright -
									playfield[side][block].segments * segmentwidth)
                                {
									playfield[side][block].segments -= 1;
									if (playfield[side][block].segments == 0)
										if(!ScreenshotMode)
									    	Game->AddToScore((level + 3) * 2 * playfield[side][block].maxsegments);

									playaudiobullet = true;
									destroybullet = true;
                                }
                            }
                        }
                    }
                }
            }
	
			if(destroybullet) 
            {
				if (playaudiobullet) 
					Game->Audio->PlaySound(SfxHit, 0);
				bulletalive = false;
				break;
            }
        }
    }
}

void CGameRamIt::ridrawbullet()
{
	if (bulletalive)
    {
		SDL_Rect r = {bulletpos.x - riblocksize / 2, bulletpos.y - riblocksize / 6, riblocksize, riblocksize / 3};
		SDL_SetRenderDrawColor(Game->Renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(Game->Renderer, &r);
    }
}

//PLAYER ----------------------------------------------------------------------------------------------------------------

void CGameRamIt::ricreateplayer()
{
	playerpos.x = screenleft + (screenright - screenleft) / 2;
    playerpos.y = screentop + (screenbottom - screentop) / 2;
	playerdx = 1;
}

void CGameRamIt::ridrawplayer()
{
	SDL_Rect r;
	
	SDL_SetRenderDrawColor(Game->Renderer, 128, 128, 128, 255);
	r =  {screenleft + (screenright - screenleft -playerrailwidth) / 2, screentop, playerrailwidth, screenbottom - screentop };  
	SDL_RenderFillRect(Game->Renderer, &r);
		
	SDL_SetRenderDrawColor(Game->Renderer, 255, 255, 255, 255);
	r = {playerpos.x - riblocksize / 2, playerpos.y - riblocksize / 2, riblocksize, riblocksize};
	SDL_RenderFillRect(Game->Renderer, &r);	

	SDL_SetRenderDrawColor(Game->Renderer, 255, 0, 0, 255);
	if (playerdx == 1)
		r ={playerpos.x + riblocksize / 2, playerpos.y - riblocksize / 4, riblocksize / 2, riblocksize / 2};
    else
		r ={playerpos.x - riblocksize, playerpos.y - riblocksize / 4, riblocksize / 2,  riblocksize / 2};
	SDL_RenderFillRect(Game->Renderer, &r);
}

void CGameRamIt::riupdateplayer()
{
	if(ScreenshotMode)
	{
		return;
	}
	
	if (Game->Input->Buttons.ButLeft)
		playerdx = -1;
	
	if (Game->Input->Buttons.ButRight)
		playerdx = 1;
	
	if (Game->Input->Buttons.ButDown)
    {
		if (playerpos.y + riblocksize / 2 + playerspeed < screenbottom)
			playerpos.y += playerspeed;
		else
			playerpos.y = screenbottom - riblocksize / 2;
    }

	if (Game->Input->Buttons.ButUp)
    {
		if (playerpos.y - riblocksize / 2 - playerspeed > screentop)
			playerpos.y -= playerspeed;
		else
			playerpos.y = screentop + riblocksize / 2;
    }

	if (Game->Input->Buttons.ButA && !Game->Input->PrevButtons.ButA)
		if (ricreatebullet())
        {
			Game->Audio->PlaySound(SfxShoot, 0);
        }
}

//playfield ----------------------------------------------------------------------------------------------------------------

void CGameRamIt::ricreateplayfield()
{
	int prevpiece = -1;
	int piece = -1;
	SDL_Color color;
	for (int side = 0 ; side < sides; side++)
    {
		for (int block = 0; block < numblocks; block++)
        {
			while(prevpiece == piece)
				piece = rand() % 17;

			prevpiece = piece;
			
			if (piece == 0)
				color = {0x00, 0x00, 0xFF, 0xFF};

			if (piece == 1)
				color = {0xFF, 0xFF, 0x00, 0xFF};

			if (piece == 2)
				color = {0x00, 0xFF, 0x00, 0xFF};

			if (piece == 3)
				color = {0x00, 0x00, 0xFF, 0xFF};

			if (piece == 4)
				color = {0xA0, 0x20, 0xF0, 0xFF};
                
			if (piece == 5)
				color = {0xA5, 0x2A, 0x2A, 0xFF};

			if (piece == 6)
				color = {0xFF, 0x00, 0xFF, 0xFF};

			if (piece == 7)
				color = {0xFF, 0xFF, 0xFF, 0xFF};

			if (piece == 8)
				color = {0x9B, 0x9B, 0x9B, 0xFF};

			if (piece == 9)
				color = {0xFF, 0xE4, 0xC4, 0xFF};

			if (piece == 10)
				color = {0xED, 0x70, 0x14, 0xFF};

			if (piece == 11)
				color = {0xCC, 0xCC, 0xFF, 0xFF};

			if (piece == 12)
				color = {0xBF, 0xFF, 0x00, 0xFF};

			if (piece == 13)
				color = {0x00, 0x80, 0x80, 0xFF};

			if (piece == 14)
				color = {0x00, 0xFF, 0xFF, 0xFF};

			if (piece == 15)
				color = {0x8A, 0x9A, 0x5B, 0xFF};

			if (piece == 16)
				color = {0xE9, 0x96, 0x7A, 0xFF};

			playfield[side][block].color = color;
			playfield[side][block].segments = 2;
			playfield[side][block].maxsegments = 2;
		}
    }
}

void CGameRamIt::riupdateplayfield(bool force)
{
	speedcount += 1;

	if (force || (speedcount >= speed - (level * tickslevelmul)))
    {
		speedcount = 0;
		int stageclear = true;		
		int numalive = 0;
        int side;
        int block;
		for (side = 0; side < sides; side++)
			for(block = 0; block < numblocks; block++)
				if (playfield[side][block].segments != 0)
                {
					stageclear = false;
					numalive += 1;
                }
		
		if (stageclear)
        {
			Game->Audio->PlaySound(SfxSucces, 0);
			ricreateplayfield();
			level += 1;
			if(!ScreenshotMode)
				Game->AddToScore(level * 100);
        }
		
		if (!stageclear)
        {			
			int r = 1;
			if (numalive <= 5)
				r = rand() % 2;

			if (r == 1)
            {
				side = rand() % sides;
				block = rand() % numblocks;
				while (playfield[side][block].segments == 0)
                {
					side = rand() % sides;
				    block = rand() % numblocks;
                }
				playfield[side][block].segments += 2;
				if (playfield[side][block].segments > blocksegments)
					playfield[side][block].segments = blocksegments;
	
				if (playfield[side][block].segments > playfield[side][block].maxsegments)
					playfield[side][block].maxsegments = playfield[side][block].segments;
			
				//death
				for (side = 0; side < sides; side++)
					for (block = 0; block < numblocks; block++)
						if (playfield[side][block].segments == blocksegments)
                        {
							playerdeath = true;
							break;
                        }
					
            }
        }
    }
}	

void CGameRamIt::ridrawplayfield()
{
	for (int side = 0; side < sides; side++)
		for (int block = 0; block < numblocks; block++)
		{
			SDL_Rect r;
			if (side == 0)
				r = {screenleft, screentop + block * riblocksizespacing + blockspacing, 
					playfield[side][block].segments * segmentwidth, 
					riblocksize};
			else
				r = { screenright - playfield[side][block].segments * segmentwidth , 
					screentop + block * riblocksizespacing + blockspacing, 
					playfield[side][block].segments * segmentwidth, 
					riblocksize};			
			SDL_SetRenderDrawColor(Game->Renderer, playfield[side][block].color.r, playfield[side][block].color.g,  playfield[side][block].color.b,  
					playfield[side][block].color.a);
			SDL_RenderFillRect(Game->Renderer, &r);
		}
}

//background ----------------------------------------------------------------------------------------------------------------

void CGameRamIt::ridrawbackground()
{
	SDL_SetRenderDrawColor(Game->Renderer, 150, 150, 150, 255);
    SDL_RenderClear(Game->Renderer);
	SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 255);
	SDL_Rect r = {screenleft, screentop, playfieldwidth, playfieldheight};
	SDL_RenderFillRect(Game->Renderer, &r);
}

//init - deinit ----------------------------------------------------------------------------------------------------------------

void CGameRamIt::init()
{
    
	bulletalive = false;
	speed = ticksidle;
	level = 0;
	playerdeath = false;
	if(!ScreenshotMode)
	{
		HealthPoints = 3;
		SfxShoot = Game->Audio->LoadSound("ramit/shoot.wav");
		SfxHit = Game->Audio->LoadSound("ramit/hit.wav");
		SfxDie = Game->Audio->LoadSound("common/die.wav");
		SfxSucces = Game->Audio->LoadSound("common/succes.mp3");
		MusMusic = Game->Audio->LoadMusic("ramit/music.mp3");	
		Game->CurrentGameMusicID = MusMusic;
		ricreateplayer();
		ricreateplayfield();
		Game->Audio->PlayMusic(MusMusic, -1);
	}
}

void CGameRamIt::deinit()
{
	if (!ScreenshotMode)
	{
		Game->Audio->StopMusic();
		Game->Audio->StopSound();
		Game->Audio->UnLoadMusic(MusMusic);
		Game->Audio->UnLoadSound(SfxShoot);
		Game->Audio->UnLoadSound(SfxHit);
		Game->Audio->UnLoadSound(SfxDie);
		Game->Audio->UnLoadSound(SfxSucces);

		Game->SubStateCounter = 0;
		Game->SubGameState = SGNone;
		Game->CurrentGameMusicID = -1;
	}
}

SDL_Texture* CGameRamIt::screenshot()
{
	SDL_Texture* prev = SDL_GetRenderTarget(Game->Renderer);
	SDL_Texture* image = SDL_CreateTexture(Game->Renderer, PixelFormat, SDL_TEXTUREACCESS_TARGET, ScreenWidth, ScreenHeight);
    SDL_SetRenderTarget(Game->Renderer, image);
	SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 255);
	SDL_RenderClear(Game->Renderer);
	init();
	ricreateplayer();
	ricreateplayfield();
	for(int i = 0; i < 30; i++)
		riupdateplayfield(true);
	ricreatebullet();
	riupdatebullet();
	riupdatebullet();
	riupdatebullet();
	ridrawbackground();
	ridrawbullet();
	ridrawplayer();
	ridrawplayfield();
	SDL_RenderPresent(Game->Renderer);
	SDL_SetRenderTarget(Game->Renderer, prev);
	deinit();	
	return image;
}

//Update ----------------------------------------------------------------------------------------------------------------

void CGameRamIt::UpdateLogic()
{
	//initramit()
	CGameBase::UpdateLogic();
    if (Game->SubGameState == SGGame)
    {
        riupdateplayer();
        riupdateplayfield(false);
        riupdatebullet();
    
	    // if global.gamestate == gsramit then
		// 	setdrawtarget(framebuffer)
		// 	drawimageex(screen,{screenwidth/2,screenheight/2},0,{1,1},{1,1,1,alpha})
		// 	update()
		// endif
		
		if (playerdeath)
        {
			Game->Audio->PlaySound(SfxDie, 0);
			if (Game->GameMode == GMGame)
				if (!ScreenshotMode)
					HealthPoints -= 1;
			
			if (HealthPoints > 0)
			{
            	SDL_Delay(500);
				ricreateplayfield();
            }
			playerdeath = false;
        }
	}
}

void CGameRamIt::Draw()
{
	SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Game->Renderer);
    ridrawbackground();
    ridrawbullet();
    ridrawplayer();
    ridrawplayfield();
	Game->DrawCrt();
    DrawScoreBar();
	DrawSubStateText();
}


