#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include "CGameBlockStacker.h"
#include "BlockStackerBlocks.h"
#include "../CGame.h"
#include "../Common.h"
#include "../Vec2F.h"

using namespace std;

CGameBlockStacker* Create_CGameBlockStacker()
{
	CGameBlockStacker* BlockStacker = (CGameBlockStacker*) malloc(sizeof(CGameBlockStacker));
	BlockStacker->GameBase = Create_CGameBase(GSTetris, true);

	BlockStacker->MusMusic = -1;
	BlockStacker->SfxDie = -1;
	BlockStacker->SfxLineClear = -1;
	BlockStacker->SfxDrop = -1;
	BlockStacker->SfxRotate = -1;
	BlockStacker->GameBase->playfieldwidth = BlockStacker->numcols * BlockStacker->blocksize;
	BlockStacker->GameBase->playfieldheight = BlockStacker->numrows * BlockStacker->blocksize;
	BlockStacker->GameBase->screenleft = (ScreenWidth - BlockStacker->GameBase->playfieldwidth) / 2;
	BlockStacker->GameBase->screenright = BlockStacker->GameBase->screenleft + BlockStacker->GameBase->playfieldwidth;
	BlockStacker->GameBase->screentop = (ScreenHeight - BlockStacker->GameBase->playfieldheight) / 2;
	BlockStacker->GameBase->screenbottom = BlockStacker->GameBase->screentop + BlockStacker->GameBase->playfieldheight;

	BlockStacker->piecefits = CGameBlockStacker_piecefits;
	BlockStacker->updateplayer = CGameBlockStacker_updateplayer;
	BlockStacker->createplayfield = CGameBlockStacker_createplayfield;
	BlockStacker->updateplayfield = CGameBlockStacker_updateplayfield;
	BlockStacker->drawplayfieldcell = CGameBlockStacker_drawplayfieldcell;
	BlockStacker->drawplayfield = CGameBlockStacker_drawplayfield;
	BlockStacker->DrawBackground = CGameBlockStacker_DrawBackground;
	BlockStacker->Draw = CGameBlockStacker_Draw;
	BlockStacker->UpdateLogic = CGameBlockStacker_UpdateLogic;
	BlockStacker->init = CGameBlockStacker_init;
	BlockStacker->deinit = CGameBlockStacker_deinit;
	BlockStacker->UnloadGraphics = CGameBlockStacker_UnloadGraphics;
	BlockStacker->LoadGraphics = CGameBlockStacker_LoadGraphics;
	BlockStacker->LoadSound = CGameBlockStacker_LoadSound;
	BlockStacker->UnLoadSound = CGameBlockStacker_UnLoadSound;
	BlockStacker->UpdateObjects = CGameBlockStacker_UpdateObjects;
	BlockStacker->DrawObjects = CGameBlockStacker_DrawObjects;
	return BlockStacker;
}

void Destroy_CGameBlockStacker(CGameBlockStacker* BlockStacker)
{
	Destroy_CGameBase(BlockStacker->GameBase);
}

//helper funcs ----------------------------------------------------------------------------------------------------------------

bool CGameBlockStacker_piecefits(CGameBlockStacker* BlockStacker, int tetrimo, int rotation, int posx, int posy)
{
	bool result = true;
	for(int x = 0; x < 4; x++)
	{
		for(int y = 0; y < 4; y++)
		{
			int piece = y * 4 + x;
			int field = (posy + y) * BlockStacker->numcols + posx + x;

			if ((posx + x >= 0) && (posx + x < BlockStacker->numcols) &&
				(posy + y >= 0) && (posy + y < BlockStacker->numrows) &&
				(tstetrimos[tetrimo][rotation % 4][piece] && (BlockStacker->playfield[field] != -1)))
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

void CGameBlockStacker_updateplayer(CGameBlockStacker* BlockStacker)
{
	if ((CInput_Buttons.ButLeft) ||
		(CInput_Buttons.ButLeft2) ||
		(CInput_Buttons.ButDpadLeft))
		if (BlockStacker->piecefits(BlockStacker, BlockStacker->currpiece, BlockStacker->rotation, BlockStacker->plrx - 1, BlockStacker->plry))
			BlockStacker->plrx -= 1;

	if ((CInput_Buttons.ButRight) ||
		(CInput_Buttons.ButRight2) ||
		(CInput_Buttons.ButDpadRight))
		if (BlockStacker->piecefits(BlockStacker, BlockStacker->currpiece, BlockStacker->rotation, BlockStacker->plrx + 1, BlockStacker->plry))
			BlockStacker->plrx += 1;

	if ((CInput_Buttons.ButDown) ||
		(CInput_Buttons.ButDown2) ||
		(CInput_Buttons.ButDpadDown))
		if (BlockStacker->piecefits(BlockStacker, BlockStacker->currpiece, BlockStacker->rotation, BlockStacker->plrx, BlockStacker->plry + 1))
			BlockStacker->plry += 1;

	if (CInput_Buttons.ButA)
	{
		if (BlockStacker->rotateblock && BlockStacker->piecefits(BlockStacker, BlockStacker->currpiece, BlockStacker->rotation +1, BlockStacker->plrx, BlockStacker->plry))
		{
			CAudio_PlaySound(BlockStacker->SfxRotate, 0);
			BlockStacker->rotation += 1;
			BlockStacker->rotateblock = false;
		}
	}
	else
		BlockStacker->rotateblock = true;

	if (CInput_Buttons.ButB)
	{
		if (BlockStacker->dropblock)
		{
			while (BlockStacker->piecefits(BlockStacker, BlockStacker->currpiece, BlockStacker->rotation, BlockStacker->plrx, BlockStacker->plry +1))
				BlockStacker->plry += 1;
			BlockStacker->dropblock = false;
			BlockStacker->speedcount = 10000;
		}
	}
	else
		BlockStacker->dropblock = true;
}

//playfield ----------------------------------------------------------------------------------------------------------------

void CGameBlockStacker_createplayfield(CGameBlockStacker* BlockStacker)
{
	for(int y = 0; y < BlockStacker->numrows; y++)
		for(int x = 0; x < BlockStacker->numcols; x++)
			if ((x == 0) || (x == BlockStacker->numcols -1) || (y == BlockStacker->numrows -1))
				BlockStacker->playfield[y*BlockStacker->numcols +x] = -2;
			else
				BlockStacker->playfield[y*BlockStacker->numcols +x] = -1;
}

void CGameBlockStacker_updateplayfield(CGameBlockStacker* BlockStacker, bool force)
{

	if (BlockStacker->lineclear > -1)
	{
		BlockStacker->lineclear -= 1;

		if (BlockStacker->lineclear == 0)
		{
			int y = BlockStacker->numrows -2;
			while(y > 0)
			{
				if(BlockStacker->playfield[y * BlockStacker->numcols + 1] == -3)
				{
					int y2 = y;
					//set all rows equal to row above
					while (y2 > 0)
					{
						for(int x = 1; x < BlockStacker->numcols -1; x++)
							BlockStacker->playfield[y2 * BlockStacker->numcols + x] = BlockStacker->playfield[(y2 -1) * BlockStacker->numcols + x];
						y2 -= 1;
					}
					//clear top row
					for(int x = 1; x < BlockStacker->numcols -1; x++)
						BlockStacker->playfield[x] = -1;
					y += 1;
				}
				y -=1;
			}
		}
	}
	else
	{
		BlockStacker->speedcount += 1;

		if (BlockStacker->speedcount % BlockStacker->ticksinputidle == 0)
			BlockStacker->updateplayer(BlockStacker);

		if (force || (BlockStacker->speedcount >= BlockStacker->speed*BlockStacker->ticksidle))
		{
			BlockStacker->speedcount = 0;
			BlockStacker->piececount += 1;
			//level increase
			if (BlockStacker->piececount % 40 == 0)
				if (BlockStacker->speed >= 5)
				{
					BlockStacker->speed -= 1;
					BlockStacker->GameBase->level += 1;
				}

			//can we move the piece down ?
			if (BlockStacker->piecefits(BlockStacker, BlockStacker->currpiece, BlockStacker->rotation, BlockStacker->plrx, BlockStacker->plry +1))
				BlockStacker->plry += 1;
			else
			{
				if(!force)
					CAudio_PlaySound(BlockStacker->SfxDrop, 0);


				//lock it in place
				for(int x = 0; x < 4; x++)
					for(int y = 0; y < 4; y++)
					{
						int piece = y * 4 + x;
						if (tstetrimos[BlockStacker->currpiece][BlockStacker->rotation % 4][piece])
							BlockStacker->playfield[(BlockStacker->plry + y) * BlockStacker->numcols + BlockStacker->plrx + x] = BlockStacker->currpiece;
					}

				//check for lines
				int numlines = 0;
				bool linedone = true;
				for(int y = 0; y < 4; y++)
				{
					if (BlockStacker->plry + y < BlockStacker->numrows -1)
					{
						linedone = true;
						for(int x = 1; x < BlockStacker->numcols -1; x++)
							linedone = linedone && (BlockStacker->playfield[(BlockStacker->plry + y) * BlockStacker->numcols + x] > -1);

						if (linedone)
						{
							numlines += 1;
							for(int x = 1; x < BlockStacker->numcols -1; x++)
								BlockStacker->playfield[(BlockStacker->plry + y) * BlockStacker->numcols + x] = -3;
						}
					}
				}
				CGame_AddToScore(7);

				if (numlines > 0)
				{
					CGame_AddToScore((1 << numlines) * 20);
					BlockStacker->lineclear = 30;
					CAudio_PlaySound(BlockStacker->SfxLineClear, 0);
				}

				BlockStacker->plrx = BlockStacker->numcols / 2 -2;
				BlockStacker->plry = 0;
				BlockStacker->rotation = 0;
				BlockStacker->currpiece = rand() % 7;

				if (!BlockStacker->piecefits(BlockStacker, BlockStacker->currpiece, BlockStacker->rotation, BlockStacker->plrx, BlockStacker->plry))
				{
					CAudio_PlaySound(BlockStacker->SfxDie, 0);
					if(GameMode == GMGame)
					{
						BlockStacker->GameBase->HealthPoints -= 1;
					}
					else
					{
						CGame_AddToScore(-250);
						BlockStacker->createplayfield(BlockStacker);
					}
				}
			}
		}
	}
}

void CGameBlockStacker_drawplayfieldcell(CGameBlockStacker* BlockStacker, int x, int y, int piece)
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

		SDL_Rect r = {BlockStacker->GameBase->screenleft + x * BlockStacker->blocksize, BlockStacker->GameBase->screentop + y * BlockStacker->blocksize, BlockStacker->blocksize, BlockStacker->blocksize};
		SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(Renderer, &r);

		r = {BlockStacker->GameBase->screenleft +1 + x * BlockStacker->blocksize, BlockStacker->GameBase->screentop +1 + y * BlockStacker->blocksize, BlockStacker->blocksize-2, BlockStacker->blocksize-2};
		SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(Renderer, &r);
	}
}

void CGameBlockStacker_drawplayfield(CGameBlockStacker* BlockStacker)
{
	for(int x = 0; x < BlockStacker->numcols; x++)
		for(int y = 0; y < BlockStacker->numrows; y++)
		{
			int piece = BlockStacker->playfield[y * BlockStacker->numcols + x];
			BlockStacker->drawplayfieldcell(BlockStacker,x,y, piece);
		}

	for(int x = 0; x < 4; x++)
		for(int y = 0; y < 4; y++)
		{
			int piece = y * 4 + x;
			if(tstetrimos[BlockStacker->currpiece][BlockStacker->rotation % 4][piece])
				BlockStacker->drawplayfieldcell(BlockStacker,BlockStacker->plrx + x, BlockStacker->plry + y, BlockStacker->currpiece);

		}
}

//Drawing ----------------------------------------------------------------------------------------------------------------

void CGameBlockStacker_DrawBackground(CGameBlockStacker* BlockStacker)
{
	CImage_DrawImage(Renderer, BlockStacker->background, NULL, NULL);
}

void CGameBlockStacker_Draw(CGameBlockStacker* BlockStacker)
{
	BlockStacker->DrawBackground(BlockStacker);
	if (BlockStacker->DrawObjects(BlockStacker))
		Sprites->DrawSprites(Renderer);
	BlockStacker->GameBase->DrawScoreBar(BlockStacker->GameBase);
	BlockStacker->GameBase->DrawSubStateText(BlockStacker->GameBase);
}

//init - deinit ----------------------------------------------------------------------------------------------------------------

void CGameBlockStacker_init(CGameBlockStacker* BlockStacker)
{
	BlockStacker->LoadGraphics(BlockStacker);
	BlockStacker->LoadSound(BlockStacker);
	CurrentGameMusicID = BlockStacker->MusMusic;
	CAudio_PlayMusic(BlockStacker->MusMusic, -1);
	BlockStacker->GameBase->HealthPoints = 1;
	BlockStacker->currpiece = rand() % 7;
	BlockStacker->rotation = 0;
	BlockStacker->plrx = BlockStacker->numcols / 2 -2;
	BlockStacker->plry = 0;
	BlockStacker->speed = 20;
	BlockStacker->speedcount = 0;
	BlockStacker->piececount = 0;
	BlockStacker->rotateblock = true;
	BlockStacker->dropblock = true;
	BlockStacker->GameBase->level = 1;
	BlockStacker->lineclear = 0;
	BlockStacker->createplayfield(BlockStacker);
}

void CGameBlockStacker_LoadGraphics(CGameBlockStacker* BlockStacker)
{
	BlockStacker->background = CImage_LoadImage(Renderer, "blockstacker/background.png");
	BlockStacker->backgroundtz = CImage_ImageSize(BlockStacker->background);
}

void CGameBlockStacker_UnloadGraphics(CGameBlockStacker* BlockStacker)
{
	CImage_UnLoadImage(BlockStacker->background);
}

void CGameBlockStacker_LoadSound(CGameBlockStacker* BlockStacker)
{
	BlockStacker->SfxLineClear = CAudio_LoadSound("blockstacker/lineclear.ogg");
	BlockStacker->SfxDrop = CAudio_LoadSound("blockstacker/drop.wav");
	BlockStacker->SfxRotate = CAudio_LoadSound("blockstacker/rotate.wav");
	BlockStacker->MusMusic = CAudio_LoadMusic("blockstacker/music.ogg");
	BlockStacker->SfxDie = CAudio_LoadSound("common/die.wav");
}

void CGameBlockStacker_UnLoadSound(CGameBlockStacker* BlockStacker)
{
	CAudio_StopMusic();
	CAudio_StopSound();
	CAudio_UnLoadMusic(BlockStacker->MusMusic);
	CAudio_UnLoadSound(BlockStacker->SfxLineClear);
	CAudio_UnLoadSound(BlockStacker->SfxDrop);
	CAudio_UnLoadSound(BlockStacker->SfxRotate);
	CAudio_UnLoadSound(BlockStacker->SfxDie);
}

void CGameBlockStacker_deinit(CGameBlockStacker* BlockStacker)
{
	BlockStacker->UnLoadSound(BlockStacker);
	SubStateCounter = 0;
	SubGameState = SGNone;
	CurrentGameMusicID = -1;
	BlockStacker->UnloadGraphics(BlockStacker);
}

//Update ----------------------------------------------------------------------------------------------------------------

void CGameBlockStacker_UpdateLogic(CGameBlockStacker* BlockStacker)
{
	BlockStacker->GameBase->UpdateLogic(BlockStacker->GameBase);
	BlockStacker->UpdateObjects(BlockStacker, SubGameState == SGGame);
	if(SubGameState == SGGame)
		Sprites->UpdateSprites(Renderer);
}

void CGameBlockStacker_UpdateObjects(CGameBlockStacker* BlockStacker, bool IsGameState)
{
	if(IsGameState)
		BlockStacker->updateplayfield(BlockStacker,false);
}

bool CGameBlockStacker_DrawObjects(CGameBlockStacker* BlockStacker)
{
	BlockStacker->drawplayfield(BlockStacker);
	//don't call drawsprites
	return false;
}


