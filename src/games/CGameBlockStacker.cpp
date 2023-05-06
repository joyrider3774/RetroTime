#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <string>
#include <iostream>
#include "CGameBlockStacker.h"
#include "BlockStackerBlocks.h"
#include "../CGame.h"
#include "../Common.h"
#include "../Types.h"

using namespace std;

CGameBlockStacker::CGameBlockStacker(CGame* aGame, bool aScreenshotMode): CGameBase(aGame, GSTetris, true, aScreenshotMode)
{
    Game = aGame;
	MusMusic = -1;
	SfxDie = -1;
	SfxLineClear = -1;
	SfxDrop = -1;
	SfxRotate = -1;
    playfieldwidth = numcols * blocksize;
    playfieldheight = numrows * blocksize;
	screenleft = (ScreenWidth - playfieldwidth) / 2;
	screenright = screenleft + playfieldwidth;
	screentop = (ScreenHeight - playfieldheight) / 2;
	screenbottom = screentop + playfieldheight;
}

CGameBlockStacker::~CGameBlockStacker() {};

//helper funcs ----------------------------------------------------------------------------------------------------------------

bool CGameBlockStacker::piecefits(int tetrimo, int rotation, int posx, int posy)
{
	bool result = true;
	for(int x = 0; x < 4; x++)
	{
		for(int y = 0; y < 4; y++)
		{
			int piece = y * 4 + x;
			int field = (posy + y) * numcols + posx + x;
			
			if ((posx + x >= 0)  && (posx + x < numcols) &&
				(posy + y >= 0) && (posy + y < numrows) &&
				(tstetrimos[tetrimo][rotation % 4][piece] && (playfield[field] != -1)))
			{
				result = false;
				break;
			}
		}
		if (!result)
			break;
	}
	return result;
}

//player ----------------------------------------------------------------------------------------------------------------

void CGameBlockStacker::updateplayer()
{
	if(ScreenshotMode)
	{
		return;
	}

	if (Game->Input->Buttons.ButLeft) 
		if (piecefits(currpiece, rotation, plrx - 1, plry))
			plrx -= 1;
	
	if (Game->Input->Buttons.ButRight) 
		if (piecefits(currpiece, rotation, plrx + 1, plry))
			plrx += 1;
	
	if (Game->Input->Buttons.ButDown) 
		if (piecefits(currpiece, rotation, plrx, plry + 1))
			plry += 1;

	if (Game->Input->Buttons.ButA) 
	{
		if (rotateblock && piecefits(currpiece, rotation +1, plrx, plry))
		{
			Game->Audio->PlaySound(SfxRotate, 0);
			rotation += 1;
			rotateblock = false;
		}
	}
	else
		rotateblock = true;

	if (Game->Input->Buttons.ButB) 
	{
		if (dropblock)
		{
			while (piecefits(currpiece, rotation, plrx, plry +1))
				plry += 1;
			dropblock = false;
			speedcount = 10000;
		}
	}
	else
		dropblock = true;
}

//playfield ----------------------------------------------------------------------------------------------------------------

void CGameBlockStacker::createplayfield()
{
	for(int y = 0; y < numrows; y++)
		for(int x = 0; x < numcols; x++)
			if ((x == 0) || (x == numcols -1) || (y == numrows -1))
				playfield[y*numcols +x] = -2;
			else
				playfield[y*numcols +x] = -1;
}

void CGameBlockStacker::updateplayfield(bool force)
{

    if (lineclear > -1)
	{
		lineclear -= 1;

		if (lineclear == 0)
		{
			int y = numrows -2;
			while(y > 0)
			{
				if(playfield[y * numcols + 1] == -3)
				{
					int y2 = y;
					//set all rows equal to row above
					while (y2 > 0)
					{
						for(int x = 1; x < numcols -1; x++)
							playfield[y2 * numcols + x] = playfield[(y2 -1) * numcols + x];
						y2 -= 1;
					}
					//clear top row
					for(int x = 1; x < numcols -1; x++)
						playfield[x] = -1;
					y += 1;
				}
				y -=1;
			}
		}   
	}
	else
	{
    	speedcount += 1;

		if (speedcount % ticksinputidle == 0)
			updateplayer();

		if (force || (speedcount >= speed*ticksidle))
		{
			speedcount = 0;
			piececount += 1;
									
			//level increase
			if (piececount % 40 == 0)
				if (speed >= 5) 
				{
					speed -= 1;
					level += 1;
				}	
		
			//can we move the piece down ?
			if (piecefits(currpiece, rotation, plrx, plry +1))
				plry += 1;
			else
			{
				if(!force)
					Game->Audio->PlaySound(SfxDrop, 0);
				

				//lock it in place
				for(int x = 0; x < 4; x++)
					for(int y = 0; y < 4; y++)
					{
						int piece = y * 4 + x;
						if (tstetrimos[currpiece][rotation % 4][piece])
							playfield[(plry + y) * numcols + plrx + x] = currpiece;
					}
			
				//check for lines
				int numlines = 0;
				bool linedone = true;
				for(int y = 0; y < 4; y++)
				{
					if (plry + y < numrows -1)
					{
						linedone = true;
						for(int x = 1; x < numcols -1; x++)
							linedone = linedone && (playfield[(plry + y) * numcols + x] > -1);
					
						if (linedone)
						{
							numlines += 1;
							for(int x = 1; x < numcols -1; x++)
								playfield[(plry + y) * numcols + x] = -3;	
						}
					}
				}
				if(!ScreenshotMode)
					Game->AddToScore(7);

				if (numlines > 0)
				{
					if(!ScreenshotMode)
						Game->AddToScore((1 << numlines) * 20);
					lineclear = 30;
					Game->Audio->PlaySound(SfxLineClear, 0);
				}
			
				plrx = numcols / 2 -2;
				plry = 0;
				rotation = 0;
				currpiece = rand() % 7;
			
				if (!piecefits(currpiece, rotation, plrx, plry))
				{
					Game->Audio->PlaySound(SfxDie, 0);
					if(Game->GameMode == GMGame)
					{
						if(!ScreenshotMode)
							HealthPoints -= 1;
					}
					else
					{
						if(!ScreenshotMode)
							Game->AddToScore(-250);
						createplayfield();
					}
				}
			}
		}
	}
}

void CGameBlockStacker::drawplayfieldcell(int x, int y, int piece)
{
	SDL_Color color;

	if (piece != -1)
	{
		if(piece == 0)
			color = {0, 0, 255, 255};

		if (piece == 1)
			color = {255, 255, 0, 255};

		if (piece == 2)
			color = {0, 255, 0, 255};

		if (piece == 3)
			color = {255, 0, 0, 255};

		if (piece == 4)
			color = {0xA0, 0x20, 0xF0, 255};

		if (piece == 5)
			color = {0xA5, 0x2A, 0x2A, 255};
		
		if (piece == 6)
			color = {255, 0, 255, 255};

		if (piece == -2)
			color = {128, 128, 128, 255};
		
		if (piece == -3) 
			color = {255, 255, 255, 255};
		
		SDL_Rect r = {screenleft + x * blocksize, screentop + y * blocksize, blocksize, blocksize};
		SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(Game->Renderer, &r);

		r = {screenleft +1 + x * blocksize, screentop +1 + y * blocksize, blocksize-2, blocksize-2};
		SDL_SetRenderDrawColor(Game->Renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(Game->Renderer, &r);
	}
}

void CGameBlockStacker::drawplayfield()
{
	for(int x = 0; x < numcols; x++)
		for(int y = 0; y < numrows; y++)
		{
			int piece = playfield[y * numcols + x];
			drawplayfieldcell(x,y, piece);	
		}
	
	for(int x = 0; x < 4; x++)
		for(int y = 0; y < 4; y++)
		{
			int piece = y * 4 + x;
			if(tstetrimos[currpiece][rotation % 4][piece])
				drawplayfieldcell(plrx + x, plry + y, currpiece);
			
		}				
}

//background ----------------------------------------------------------------------------------------------------------------

void CGameBlockStacker::drawbackground()
{
	SDL_Point pos = { ScreenWidth / 2, ScreenHeight / 2};
	Vec2F scale = {(float)ScreenWidth / backgroundtz.x, (float)ScreenHeight / backgroundtz.y};
	Game->Image->DrawImageFuze(Game->Renderer, background, true, &pos , 0, &scale, 255,255,255,255);
	SDL_Rect r = {screenleft, screentop, playfieldwidth, playfieldheight};
	SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(Game->Renderer, &r);
}


//init - deinit ----------------------------------------------------------------------------------------------------------------

void CGameBlockStacker::init()
{
	LoadGraphics();
	if(!ScreenshotMode)
	{
		SfxLineClear = Game->Audio->LoadSound("blockstacker/lineclear.ogg");
		SfxDrop = Game->Audio->LoadSound("blockbreaker/drop.wav");
		SfxRotate = Game->Audio->LoadSound("blockstacker/rotate.wav");
		MusMusic = Game->Audio->LoadMusic("blockstacker/music.ogg");
		SfxDie = Game->Audio->LoadSound("common/die.wav");
		Game->CurrentGameMusicID = MusMusic;
		Game->Audio->PlayMusic(MusMusic, -1);
		HealthPoints = 1;
	}
	currpiece = rand() % 7;
	rotation = 0;
	plrx = numcols / 2 -2;
	plry = 0;
	speed = 20;
	speedcount = 0;
	piececount = 0;
	rotateblock = true;
	dropblock = true;
	level = 1;
	lineclear = 0;
	createplayfield();	
}

void CGameBlockStacker::LoadGraphics()
{
	background = Game->Image->LoadImage(Game->Renderer, "blockstacker/background.png");
	backgroundtz = Game->Image->ImageSize(background);
}

void CGameBlockStacker::UnloadGraphics()
{
	Game->Image->UnLoadImage(background);
}

void CGameBlockStacker::deinit()
{
	if (!ScreenshotMode)
	{
		Game->Audio->StopMusic();
		Game->Audio->StopSound();
		Game->Audio->UnLoadMusic(MusMusic);
		Game->Audio->UnLoadSound(SfxLineClear);
		Game->Audio->UnLoadSound(SfxDrop);
		Game->Audio->UnLoadSound(SfxRotate);
		Game->Audio->UnLoadSound(SfxDie);
		Game->SubStateCounter = 0;
		Game->SubGameState = SGNone;
		Game->CurrentGameMusicID = -1;
		UnloadGraphics();
	}
}
	

SDL_Texture* CGameBlockStacker::screenshot()
{
	SDL_Texture* prev = SDL_GetRenderTarget(Game->Renderer);
	SDL_Texture* image = SDL_CreateTexture(Game->Renderer, PixelFormat, SDL_TEXTUREACCESS_TARGET, ScreenWidth, ScreenHeight);
    SDL_SetRenderTarget(Game->Renderer, image);
	SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 255);
	SDL_RenderClear(Game->Renderer);
	init();

	for(int i = 0; i < numrows * 2; i++)
		updateplayfield(true);

	drawbackground();
	drawplayfield();	

	SDL_RenderPresent(Game->Renderer);
	SDL_SetRenderTarget(Game->Renderer, prev);
	deinit();
	return image;
}

//Update ----------------------------------------------------------------------------------------------------------------

void CGameBlockStacker::UpdateLogic()
{
    CGameBase::UpdateLogic();
	if (Game->SubGameState == SGGame)
	{
		updateplayfield(false);	
	}
}

void CGameBlockStacker::Draw()
{
	drawbackground();
	drawplayfield();
	DrawScoreBar();
	DrawSubStateText();
}


