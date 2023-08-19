#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <string>
#include <iostream>
#include "CGameBlockStacker.h"
#include "BlockStackerBlocks.h"
#include "../CGame.h"
#include "../Common.h"
#include "../Vec2F.h"

using namespace std;

CGameBlockStacker::CGameBlockStacker(CGame* aGame)
{
	GameBase = new CGameBase(aGame, GSTetris, true);

	MusMusic = -1;
	SfxDie = -1;
	SfxLineClear = -1;
	SfxDrop = -1;
	SfxRotate = -1;
	GameBase->playfieldwidth = numcols * blocksize;
	GameBase->playfieldheight = numrows * blocksize;
	GameBase->screenleft = (ScreenWidth - GameBase->playfieldwidth) / 2;
	GameBase->screenright = GameBase->screenleft + GameBase->playfieldwidth;
	GameBase->screentop = (ScreenHeight - GameBase->playfieldheight) / 2;
	GameBase->screenbottom = GameBase->screentop + GameBase->playfieldheight;
}

CGameBlockStacker::~CGameBlockStacker()
{

}

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

			if ((posx + x >= 0) && (posx + x < numcols) &&
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
	if ((GameBase->Game->Input->Buttons.ButLeft) ||
		(GameBase->Game->Input->Buttons.ButLeft2) ||
		(GameBase->Game->Input->Buttons.ButDpadLeft))
		if (piecefits(currpiece, rotation, plrx - 1, plry))
			plrx -= 1;

	if ((GameBase->Game->Input->Buttons.ButRight) ||
		(GameBase->Game->Input->Buttons.ButRight2) ||
		(GameBase->Game->Input->Buttons.ButDpadRight))
		if (piecefits(currpiece, rotation, plrx + 1, plry))
			plrx += 1;

	if ((GameBase->Game->Input->Buttons.ButDown) ||
		(GameBase->Game->Input->Buttons.ButDown2) ||
		(GameBase->Game->Input->Buttons.ButDpadDown))
		if (piecefits(currpiece, rotation, plrx, plry + 1))
			plry += 1;

	if (GameBase->Game->Input->Buttons.ButA)
	{
		if (rotateblock && piecefits(currpiece, rotation +1, plrx, plry))
		{
			GameBase->Game->Audio->PlaySound(SfxRotate, 0);
			rotation += 1;
			rotateblock = false;
		}
	}
	else
		rotateblock = true;

	if (GameBase->Game->Input->Buttons.ButB)
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
					GameBase->level += 1;
				}

			//can we move the piece down ?
			if (piecefits(currpiece, rotation, plrx, plry +1))
				plry += 1;
			else
			{
				if(!force)
					GameBase->Game->Audio->PlaySound(SfxDrop, 0);


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
				GameBase->Game->AddToScore(7);

				if (numlines > 0)
				{
					GameBase->Game->AddToScore((1 << numlines) * 20);
					lineclear = 30;
					GameBase->Game->Audio->PlaySound(SfxLineClear, 0);
				}

				plrx = numcols / 2 -2;
				plry = 0;
				rotation = 0;
				currpiece = rand() % 7;

				if (!piecefits(currpiece, rotation, plrx, plry))
				{
					GameBase->Game->Audio->PlaySound(SfxDie, 0);
					if(GameBase->Game->GameMode == GMGame)
					{
						GameBase->HealthPoints -= 1;
					}
					else
					{
						GameBase->Game->AddToScore(-250);
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
			color = {0x65, 0x65, 0xFF, 0xFF};

		if (piece == 1)
			color = {0xFF, 0xFF, 0x65, 0xFF};

		if (piece == 2)
			color = {0x30, 0xFF, 0x65, 0xFF};

		if (piece == 3)
			color = {0xFF, 0x65, 0x65, 0xFF};

		if (piece == 4)
			color = {0xA0, 0x40, 0xF0, 0xFF};

		if (piece == 5)
			color = {0xA5, 0x3A, 0x3A, 0xFF};

		if (piece == 6)
			color = {0xFF, 0x65, 0xFF, 0xFF};

		if (piece == -2)
			color = {0x80, 0x80, 0x80, 0xFF};

		if (piece == -3)
			color = {0xFF, 0xFF, 0xFF, 0xFF};

		SDL_Rect r = {GameBase->screenleft + x * blocksize, GameBase->screentop + y * blocksize, blocksize, blocksize};
		SDL_SetRenderDrawColor(GameBase->Game->Renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(GameBase->Game->Renderer, &r);

		r = {GameBase->screenleft +1 + x * blocksize, GameBase->screentop +1 + y * blocksize, blocksize-2, blocksize-2};
		SDL_SetRenderDrawColor(GameBase->Game->Renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(GameBase->Game->Renderer, &r);
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

//Drawing ----------------------------------------------------------------------------------------------------------------

void CGameBlockStacker::DrawBackground()
{
	GameBase->Game->Image->DrawImage(GameBase->Game->Renderer, background, NULL, NULL);
}

void CGameBlockStacker::Draw()
{
	DrawBackground();
	if (DrawObjects())
		GameBase->Game->Sprites->DrawSprites(GameBase->Game->Renderer);
	GameBase->DrawScoreBar();
	GameBase->DrawSubStateText();
}

//init - deinit ----------------------------------------------------------------------------------------------------------------

void CGameBlockStacker::init()
{
	LoadGraphics();
	LoadSound();
	GameBase->Game->CurrentGameMusicID = MusMusic;
	GameBase->Game->Audio->PlayMusic(MusMusic, -1);
	GameBase->HealthPoints = 1;
	currpiece = rand() % 7;
	rotation = 0;
	plrx = numcols / 2 -2;
	plry = 0;
	speed = 20;
	speedcount = 0;
	piececount = 0;
	rotateblock = true;
	dropblock = true;
	GameBase->level = 1;
	lineclear = 0;
	createplayfield();
}

void CGameBlockStacker::LoadGraphics()
{
	background = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "blockstacker/background.png");
	backgroundtz = GameBase->Game->Image->ImageSize(background);
}

void CGameBlockStacker::UnloadGraphics()
{
	GameBase->Game->Image->UnLoadImage(background);
}

void CGameBlockStacker::LoadSound()
{
	SfxLineClear = GameBase->Game->Audio->LoadSound("blockstacker/lineclear.ogg");
	SfxDrop = GameBase->Game->Audio->LoadSound("blockstacker/drop.wav");
	SfxRotate = GameBase->Game->Audio->LoadSound("blockstacker/rotate.wav");
	MusMusic = GameBase->Game->Audio->LoadMusic("blockstacker/music.ogg");
	SfxDie = GameBase->Game->Audio->LoadSound("common/die.wav");
}

void CGameBlockStacker::UnLoadSound()
{
	GameBase->Game->Audio->StopMusic();
	GameBase->Game->Audio->StopSound();
	GameBase->Game->Audio->UnLoadMusic(MusMusic);
	GameBase->Game->Audio->UnLoadSound(SfxLineClear);
	GameBase->Game->Audio->UnLoadSound(SfxDrop);
	GameBase->Game->Audio->UnLoadSound(SfxRotate);
	GameBase->Game->Audio->UnLoadSound(SfxDie);
}

void CGameBlockStacker::deinit()
{
	UnLoadSound();
	GameBase->Game->SubStateCounter = 0;
	GameBase->Game->SubGameState = SGNone;
	GameBase->Game->CurrentGameMusicID = -1;
	UnloadGraphics();
}

//Update ----------------------------------------------------------------------------------------------------------------

void CGameBlockStacker::UpdateLogic()
{
	GameBase->UpdateLogic();
	UpdateObjects(GameBase->Game->SubGameState == SGGame);
	if(GameBase->Game->SubGameState == SGGame)
		GameBase->Game->Sprites->UpdateSprites(GameBase->Game->Renderer);
}

void CGameBlockStacker::UpdateObjects(bool IsGameState)
{
	if(IsGameState)
		updateplayfield(false);
}

bool CGameBlockStacker::DrawObjects()
{
	drawplayfield();
	//don't call drawsprites
	return false;
}


