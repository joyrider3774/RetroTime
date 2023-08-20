#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <string>
#include <iostream>
#include "CGameRamIt.h"
#include "../CGame.h"
#include "../Common.h"

using namespace std;

CGameRamIt* Create_CGameRamIt()
{
	CGameRamIt* GameRamIt = (CGameRamIt*) malloc(sizeof(CGameRamIt));
	GameRamIt->GameBase = Create_CGameBase(GSRamIt, true);

	GameRamIt->speed = 0;
	GameRamIt->speedcount = 0;
	GameRamIt->SfxShoot = -1;
	GameRamIt->SfxHit = -1;
	GameRamIt->SfxDie = -1;
	GameRamIt->SfxSucces = -1;
	GameRamIt->MusMusic = -1;
	GameRamIt->riblocksize = int(ScreenHeight / (GameRamIt->numblocks+1)) - 2 * GameRamIt->blockspacing;
	GameRamIt->riblocksizespacing = GameRamIt->riblocksize + 2 * GameRamIt->blockspacing;
	GameRamIt->GameBase->playfieldwidth = ScreenWidth - 1 * GameRamIt->riblocksize;
	GameRamIt->segmentwidth = int(((GameRamIt->GameBase->playfieldwidth / 2) / GameRamIt->blocksegments));
	GameRamIt->GameBase->playfieldheight = GameRamIt->numblocks * GameRamIt->riblocksizespacing;
	GameRamIt->GameBase->screenleft = (ScreenWidth - GameRamIt->GameBase->playfieldwidth) / 2;
	GameRamIt->GameBase->screenright = GameRamIt->GameBase->screenleft + GameRamIt->GameBase->playfieldwidth;
	GameRamIt->GameBase->screentop = (ScreenHeight - GameRamIt->GameBase->playfieldheight) / 2;
	GameRamIt->GameBase->screenbottom = GameRamIt->GameBase->screentop + GameRamIt->GameBase->playfieldheight;


	GameRamIt->createbullet = CGameRamIt_createbullet;
	GameRamIt->updatebullet = CGameRamIt_updatebullet;
	GameRamIt->drawbullet = CGameRamIt_drawbullet;

	GameRamIt->createplayer = CGameRamIt_createplayer;
	GameRamIt->drawplayer = CGameRamIt_drawplayer;
	GameRamIt->updateplayer = CGameRamIt_updateplayer;

	GameRamIt->createplayfield = CGameRamIt_createplayfield;
	GameRamIt->updateplayfield = CGameRamIt_updateplayfield;
	GameRamIt->drawplayfield = CGameRamIt_drawplayfield;

	GameRamIt->init = CGameRamIt_init;
	GameRamIt->deinit = CGameRamIt_deinit;
	GameRamIt->LoadSound = CGameRamIt_LoadSound;
	GameRamIt->UnLoadSound = CGameRamIt_UnLoadSound;
	GameRamIt->UpdateObjects = CGameRamIt_UpdateObjects;
	GameRamIt->DrawObjects = CGameRamIt_DrawObjects;
	GameRamIt->DrawBackground = CGameRamIt_DrawBackground;
	GameRamIt->Draw = CGameRamIt_Draw;
	GameRamIt->UpdateLogic = CGameRamIt_UpdateLogic;
	return GameRamIt;
}

void Destroy_CGameRamIt(CGameRamIt* GameRamIt) 
{
	Destroy_CGameBase(GameRamIt->GameBase);
	free(GameRamIt);
}

bool CGameRamIt_createbullet(CGameRamIt* GameRamIt)
{
	bool result = false;
	if (!GameRamIt->bulletalive)
	{
		GameRamIt->bulletalive = true;
		GameRamIt->bulletpos = GameRamIt->playerpos;
		GameRamIt->bulletvel.x = GameRamIt->playerdx * GameRamIt->bulletspeed;
		GameRamIt->bulletvel.y = 0;
		result = true;
	}
	return result;
}

//BULLET ----------------------------------------------------------------------------------------------------------------

void CGameRamIt_updatebullet(CGameRamIt* GameRamIt)
{
	if (GameRamIt->bulletalive)
	{
		bool destroybullet = false;
		bool playaudiobullet = false;
		for(int steps = 0; steps < GameRamIt->bulletsteps; steps++)
		{
			GameRamIt->bulletpos.x += GameRamIt->bulletvel.x;
			GameRamIt->bulletpos.y += GameRamIt->bulletvel.y;
			if (GameRamIt->bulletpos.x + GameRamIt->riblocksize / 2 < GameRamIt->GameBase->screenleft)
				destroybullet = true;
			if (GameRamIt->bulletpos.x - GameRamIt->riblocksize / 2 > GameRamIt->GameBase->screenright)
				destroybullet = true;

			for (int side = 0; side < GameRamIt->sides; side++)
			{
				for (int block = 0; block < GameRamIt->numblocks; block++)
				{
					if (GameRamIt->playfield[side][block].segments > 0)
					{
						if ((GameRamIt->bulletpos.y + GameRamIt->riblocksize / 6 >= GameRamIt->GameBase->screentop +
							block * GameRamIt->riblocksizespacing + GameRamIt->blockspacing) &&
							(GameRamIt->bulletpos.y - GameRamIt->riblocksize / 6 <= GameRamIt->GameBase->screentop +
							block * GameRamIt->riblocksizespacing + GameRamIt->blockspacing + GameRamIt->riblocksize))
						{
							if (side == 0)
							{
								if ((GameRamIt->bulletpos.x - GameRamIt->riblocksize / 2) <= GameRamIt->GameBase->screenleft +
									GameRamIt->playfield[side][block].segments * GameRamIt->segmentwidth)
								{
									GameRamIt->playfield[side][block].segments -= 1;
									if (GameRamIt->playfield[side][block].segments == 0)
										CGame_AddToScore((GameRamIt->GameBase->level + 1) * 2*GameRamIt->playfield[side][block].maxsegments);

									playaudiobullet = true;
									destroybullet = true;
								}
							}
							else
							{
								if ((GameRamIt->bulletpos.x - GameRamIt->riblocksize / 2) >= GameRamIt->GameBase->screenright -
									GameRamIt->playfield[side][block].segments * GameRamIt->segmentwidth)
								{
									GameRamIt->playfield[side][block].segments -= 1;
									if (GameRamIt->playfield[side][block].segments == 0)
										CGame_AddToScore((GameRamIt->GameBase->level + 2) * 2 * GameRamIt->playfield[side][block].maxsegments);

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
					Audio->PlaySound(GameRamIt->SfxHit, 0);
				GameRamIt->bulletalive = false;
				break;
			}
		}
	}
}

void CGameRamIt_drawbullet(CGameRamIt* GameRamIt)
{
	if (GameRamIt->bulletalive)
	{
		SDL_Rect r = {GameRamIt->bulletpos.x - GameRamIt->riblocksize / 2, GameRamIt->bulletpos.y - GameRamIt->riblocksize / 6, GameRamIt->riblocksize, GameRamIt->riblocksize / 3};
		SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(Renderer, &r);
	}
}

//PLAYER ----------------------------------------------------------------------------------------------------------------

void CGameRamIt_createplayer(CGameRamIt* GameRamIt)
{
	GameRamIt->playerpos.x = GameRamIt->GameBase->screenleft + (GameRamIt->GameBase->screenright - GameRamIt->GameBase->screenleft) / 2;
	GameRamIt->playerpos.y = GameRamIt->GameBase->screentop + (GameRamIt->GameBase->screenbottom - GameRamIt->GameBase->screentop) / 2;
	GameRamIt->playerdx = 1;
}

void CGameRamIt_drawplayer(CGameRamIt* GameRamIt)
{
	SDL_Rect r;

	SDL_SetRenderDrawColor(Renderer, 0x80, 0x80, 0x80, 0xFF);
	r = {GameRamIt->GameBase->screenleft + (GameRamIt->GameBase->screenright - GameRamIt->GameBase->screenleft -GameRamIt->playerrailwidth) / 2, GameRamIt->GameBase->screentop, GameRamIt->playerrailwidth, GameRamIt->GameBase->screenbottom - GameRamIt->GameBase->screentop };
	SDL_RenderFillRect(Renderer, &r);

	SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	r = {GameRamIt->playerpos.x - GameRamIt->riblocksize / 2, GameRamIt->playerpos.y - GameRamIt->riblocksize / 2, GameRamIt->riblocksize, GameRamIt->riblocksize};
	SDL_RenderFillRect(Renderer, &r);

	SDL_SetRenderDrawColor(Renderer, 0xFF, 0x40, 0x40, 0xFF);
	if (GameRamIt->playerdx == 1)
		r ={GameRamIt->playerpos.x + GameRamIt->riblocksize / 2, GameRamIt->playerpos.y - GameRamIt->riblocksize / 4, GameRamIt->riblocksize / 2, GameRamIt->riblocksize / 2};
	else
		r ={GameRamIt->playerpos.x - GameRamIt->riblocksize, GameRamIt->playerpos.y - GameRamIt->riblocksize / 4, GameRamIt->riblocksize / 2, GameRamIt->riblocksize / 2};
	SDL_RenderFillRect(Renderer, &r);
}

void CGameRamIt_updateplayer(CGameRamIt* GameRamIt)
{
	if ((Input->Buttons.ButLeft) ||
		(Input->Buttons.ButLeft2) ||
		(Input->Buttons.ButDpadLeft))
		GameRamIt->playerdx = -1;

	if ((Input->Buttons.ButRight) ||
		(Input->Buttons.ButRight2) ||
		(Input->Buttons.ButDpadRight))
		GameRamIt->playerdx = 1;

	if ((Input->Buttons.ButDown) ||
		(Input->Buttons.ButDown2) ||
		(Input->Buttons.ButDpadDown))
	{
		if (GameRamIt->playerpos.y + GameRamIt->riblocksize / 2 + GameRamIt->playerspeed < GameRamIt->GameBase->screenbottom)
			GameRamIt->playerpos.y += GameRamIt->playerspeed;
		else
			GameRamIt->playerpos.y = GameRamIt->GameBase->screenbottom - GameRamIt->riblocksize / 2;
	}

	if ((Input->Buttons.ButUp) ||
		(Input->Buttons.ButUp2) ||
		(Input->Buttons.ButDpadUp))
	{
		if (GameRamIt->playerpos.y - GameRamIt->riblocksize / 2 - GameRamIt->playerspeed > GameRamIt->GameBase->screentop)
			GameRamIt->playerpos.y -= GameRamIt->playerspeed;
		else
			GameRamIt->playerpos.y = GameRamIt->GameBase->screentop + GameRamIt->riblocksize / 2;
	}

	if (Input->Buttons.ButA && !Input->PrevButtons.ButA)
		if (GameRamIt->createbullet(GameRamIt))
		{
			Audio->PlaySound(GameRamIt->SfxShoot, 0);
		}
}

//playfield ----------------------------------------------------------------------------------------------------------------

void CGameRamIt_createplayfield(CGameRamIt* GameRamIt)
{
	int prevpiece = -1;
	int piece = -1;
	SDL_Color color;
	for (int side = 0 ; side < GameRamIt->sides; side++)
	{
		for (int block = 0; block < GameRamIt->numblocks; block++)
		{
			while(prevpiece == piece)
				piece = rand() % 17;

			prevpiece = piece;

			if (piece == 0)
				color = {0x65, 0x65, 0xFF, 0xFF};

			if (piece == 1)
				color = {0xFF, 0xFF, 0x65, 0xFF};

			if (piece == 2)
				color = {0x65, 0xFF, 0x65, 0xFF};

			if (piece == 3)
				color = {0x65, 0x65, 0xFF, 0xFF};

			if (piece == 4)
				color = {0xA0, 0x20, 0xF0, 0xFF};

			if (piece == 5)
				color = {0xA5, 0x2A, 0x2A, 0xFF};

			if (piece == 6)
				color = {0xFF, 0x65, 0xFF, 0xFF};

			if (piece == 7)
				color = {0xFF, 0xFF, 0xFF, 0xFF};

			if (piece == 8)
				color = {0x9B, 0x9B, 0x9B, 0xFF};

			if (piece == 9)
				color = {0xDD, 0xE4, 0xC4, 0xFF};

			if (piece == 10)
				color = {0xED, 0x70, 0x24, 0xFF};

			if (piece == 11)
				color = {0xCC, 0xCC, 0xFF, 0xFF};

			if (piece == 12)
				color = {0xBF, 0xDD, 0x65, 0xFF};

			if (piece == 13)
				color = {0x65, 0x80, 0x80, 0xFF};

			if (piece == 14)
				color = {0x65, 0xFF, 0xFF, 0xFF};

			if (piece == 15)
				color = {0x8A, 0x9A, 0x5B, 0xFF};

			if (piece == 16)
				color = {0xD9, 0x96, 0x7A, 0xFF};

			GameRamIt->playfield[side][block].color = color;
			GameRamIt->playfield[side][block].segments = 2;
			GameRamIt->playfield[side][block].maxsegments = 2;
		}
	}
}

void CGameRamIt_updateplayfield(CGameRamIt* GameRamIt, bool force)
{
	GameRamIt->speedcount += 1;

	if (force || (GameRamIt->speedcount >= GameRamIt->speed - ((GameRamIt->GameBase->level-1) * GameRamIt->tickslevelmul)))
	{
		GameRamIt->speedcount = 0;
		int stageclear = true;
		int numalive = 0;
		int side;
		int block;
		for (side = 0; side < GameRamIt->sides; side++)
			for(block = 0; block < GameRamIt->numblocks; block++)
				if (GameRamIt->playfield[side][block].segments != 0)
				{
					stageclear = false;
					numalive += 1;
				}

		if (stageclear)
		{
			Audio->PlaySound(GameRamIt->SfxSucces, 0);
			GameRamIt->createplayfield(GameRamIt);
			GameRamIt->GameBase->level += 1;
			CGame_AddToScore((GameRamIt->GameBase->level-1) * 100);
		}

		if (!stageclear)
		{
			int r = 1;
			if (numalive <= 5)
				r = rand() % 2;

			if (r == 1)
			{
				side = rand() % GameRamIt->sides;
				block = rand() % GameRamIt->numblocks;
				while (GameRamIt->playfield[side][block].segments == 0)
				{
					side = rand() % GameRamIt->sides;
					block = rand() % GameRamIt->numblocks;
				}
				GameRamIt->playfield[side][block].segments += 2;
				if (GameRamIt->playfield[side][block].segments > GameRamIt->blocksegments)
					GameRamIt->playfield[side][block].segments = GameRamIt->blocksegments;

				if (GameRamIt->playfield[side][block].segments > GameRamIt->playfield[side][block].maxsegments)
					GameRamIt->playfield[side][block].maxsegments = GameRamIt->playfield[side][block].segments;

				//death
				for (side = 0; side < GameRamIt->sides; side++)
					for (block = 0; block < GameRamIt->numblocks; block++)
						if (GameRamIt->playfield[side][block].segments == GameRamIt->blocksegments)
						{
							GameRamIt->playerdeath = true;
							break;
						}
			}
		}
	}
}

void CGameRamIt_drawplayfield(CGameRamIt* GameRamIt)
{
	for (int side = 0; side < GameRamIt->sides; side++)
		for (int block = 0; block < GameRamIt->numblocks; block++)
		{
			SDL_Rect r;
			if (side == 0)
				r = {GameRamIt->GameBase->screenleft, GameRamIt->GameBase->screentop + block * GameRamIt->riblocksizespacing + GameRamIt->blockspacing,
					GameRamIt->playfield[side][block].segments * GameRamIt->segmentwidth,
					GameRamIt->riblocksize};
			else
				r = { GameRamIt->GameBase->screenright - GameRamIt->playfield[side][block].segments * GameRamIt->segmentwidth ,
					GameRamIt->GameBase->screentop + block * GameRamIt->riblocksizespacing + GameRamIt->blockspacing,
					GameRamIt->playfield[side][block].segments * GameRamIt->segmentwidth,
					GameRamIt->riblocksize};
			SDL_SetRenderDrawColor(Renderer, GameRamIt->playfield[side][block].color.r, GameRamIt->playfield[side][block].color.g, GameRamIt->playfield[side][block].color.b,
					GameRamIt->playfield[side][block].color.a);
			SDL_RenderFillRect(Renderer, &r);
		}
}

//background ----------------------------------------------------------------------------------------------------------------

void CGameRamIt_DrawBackground(CGameRamIt* GameRamIt)
{
	SDL_SetRenderDrawColor(Renderer, 150, 150, 150, 255);
	SDL_RenderClear(Renderer);
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	SDL_Rect r = {GameRamIt->GameBase->screenleft, GameRamIt->GameBase->screentop, GameRamIt->GameBase->playfieldwidth, GameRamIt->GameBase->playfieldheight};
	SDL_RenderFillRect(Renderer, &r);
}

//init - deinit ----------------------------------------------------------------------------------------------------------------

void CGameRamIt_init(CGameRamIt* GameRamIt)
{
	GameRamIt->bulletalive = false;
	GameRamIt->speed = GameRamIt->ticksidle;
	GameRamIt->GameBase->level = 1;
	GameRamIt->playerdeath = false;
	GameRamIt->createplayer(GameRamIt);
	GameRamIt->createplayfield(GameRamIt);
	GameRamIt->GameBase->HealthPoints = 3;
	GameRamIt->LoadSound(GameRamIt);
	CurrentGameMusicID = GameRamIt->MusMusic;
	Audio->PlayMusic(GameRamIt->MusMusic, -1);
}

void CGameRamIt_deinit(CGameRamIt* GameRamIt)
{
	GameRamIt->UnLoadSound(GameRamIt);
	SubStateCounter = 0;
	SubGameState = SGNone;
	CurrentGameMusicID = -1;
}

void CGameRamIt_LoadSound(CGameRamIt* GameRamIt)
{
	GameRamIt->SfxShoot = Audio->LoadSound("ramit/shoot.wav");
	GameRamIt->SfxHit = Audio->LoadSound("ramit/hit.wav");
	GameRamIt->SfxDie = Audio->LoadSound("common/die.wav");
	GameRamIt->SfxSucces = Audio->LoadSound("common/succes.wav");
	GameRamIt->MusMusic = Audio->LoadMusic("ramit/music.ogg");
}

void CGameRamIt_UnLoadSound(CGameRamIt* GameRamIt)
{
	Audio->StopMusic();
	Audio->StopSound();
	Audio->UnLoadMusic(GameRamIt->MusMusic);
	Audio->UnLoadSound(GameRamIt->SfxShoot);
	Audio->UnLoadSound(GameRamIt->SfxHit);
	Audio->UnLoadSound(GameRamIt->SfxDie);
	Audio->UnLoadSound(GameRamIt->SfxSucces);
}

//Update ----------------------------------------------------------------------------------------------------------------

void CGameRamIt_UpdateObjects(CGameRamIt* GameRamIt, bool IsGameState)
{
	if (IsGameState)
	{
		GameRamIt->updateplayer(GameRamIt);
		GameRamIt->updateplayfield(GameRamIt,false);
		GameRamIt->updatebullet(GameRamIt);

		if (GameRamIt->playerdeath)
		{
			Audio->PlaySound(GameRamIt->SfxDie, 0);
			if (GameMode == GMGame)
				GameRamIt->GameBase->HealthPoints -= 1;

			if (GameRamIt->GameBase->HealthPoints > 0)
			{
				SDL_Delay(500);
				GameRamIt->createplayfield(GameRamIt);
			}
			GameRamIt->playerdeath = false;
		}
	}
}

void CGameRamIt_UpdateLogic(CGameRamIt* GameRamIt)
{
	GameRamIt->GameBase->UpdateLogic(GameRamIt->GameBase);
	GameRamIt->UpdateObjects(GameRamIt, SubGameState == SGGame);
	if(SubGameState == SGGame)
		Sprites->UpdateSprites(Renderer);
}

bool CGameRamIt_DrawObjects(CGameRamIt* GameRamIt)
{
	GameRamIt->drawbullet(GameRamIt);
	GameRamIt->drawplayer(GameRamIt);
	GameRamIt->drawplayfield(GameRamIt);
	//don't call drawsprites in base class
	return false;
}


void CGameRamIt_Draw(CGameRamIt* GameRamIt)
{
	GameRamIt->DrawBackground(GameRamIt);
	if (GameRamIt->DrawObjects(GameRamIt))
		Sprites->DrawSprites(Renderer);
	GameRamIt->GameBase->DrawScoreBar(GameRamIt->GameBase);
	GameRamIt->GameBase->DrawSubStateText(GameRamIt->GameBase);
}