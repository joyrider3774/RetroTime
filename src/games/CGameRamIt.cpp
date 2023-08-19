#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <string>
#include <iostream>
#include "CGameRamIt.h"
#include "../CGame.h"
#include "../Common.h"

using namespace std;

CGameRamIt::CGameRamIt(CGame* aGame)
{
	GameBase = Create_CGameBase(aGame, GSRamIt, true);

	speed = 0;
	speedcount = 0;
	SfxShoot = -1;
	SfxHit = -1;
	SfxDie = -1;
	SfxSucces = -1;
	MusMusic = -1;
	riblocksize = int(ScreenHeight / (numblocks+1)) - 2 * blockspacing;
	riblocksizespacing = riblocksize + 2 * blockspacing;
	GameBase->playfieldwidth = ScreenWidth - 1 * riblocksize;
	segmentwidth = int(((GameBase->playfieldwidth / 2) / blocksegments));
	GameBase->playfieldheight = numblocks * riblocksizespacing;
	GameBase->screenleft = (ScreenWidth - GameBase->playfieldwidth) / 2;
	GameBase->screenright = GameBase->screenleft + GameBase->playfieldwidth;
	GameBase->screentop = (ScreenHeight - GameBase->playfieldheight) / 2;
	GameBase->screenbottom = GameBase->screentop + GameBase->playfieldheight;
}

CGameRamIt::~CGameRamIt() 
{
	Destroy_CGameBase(GameBase);
}

bool CGameRamIt::createbullet()
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

void CGameRamIt::updatebullet()
{
	if (bulletalive)
	{
		bool destroybullet = false;
		bool playaudiobullet = false;
		for(int steps = 0; steps < bulletsteps; steps++)
		{
			bulletpos.x += bulletvel.x;
			bulletpos.y += bulletvel.y;
			if (bulletpos.x + riblocksize / 2 < GameBase->screenleft)
				destroybullet = true;
			if (bulletpos.x - riblocksize / 2 > GameBase->screenright)
				destroybullet = true;

			for (int side = 0; side < sides; side++)
			{
				for (int block = 0; block < numblocks; block++)
				{
					if (playfield[side][block].segments > 0)
					{
						if ((bulletpos.y + riblocksize / 6 >= GameBase->screentop +
							block * riblocksizespacing + blockspacing) &&
							(bulletpos.y - riblocksize / 6 <= GameBase->screentop +
							block * riblocksizespacing + blockspacing + riblocksize))
						{
							if (side == 0)
							{
								if ((bulletpos.x - riblocksize / 2) <= GameBase->screenleft +
									playfield[side][block].segments * segmentwidth)
								{
									playfield[side][block].segments -= 1;
									if (playfield[side][block].segments == 0)
										GameBase->Game->AddToScore((GameBase->level + 1) * 2*playfield[side][block].maxsegments);

									playaudiobullet = true;
									destroybullet = true;
								}
							}
							else
							{
								if ((bulletpos.x - riblocksize / 2) >= GameBase->screenright -
									playfield[side][block].segments * segmentwidth)
								{
									playfield[side][block].segments -= 1;
									if (playfield[side][block].segments == 0)
										GameBase->Game->AddToScore((GameBase->level + 2) * 2 * playfield[side][block].maxsegments);

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
					GameBase->Game->Audio->PlaySound(SfxHit, 0);
				bulletalive = false;
				break;
			}
		}
	}
}

void CGameRamIt::drawbullet()
{
	if (bulletalive)
	{
		SDL_Rect r = {bulletpos.x - riblocksize / 2, bulletpos.y - riblocksize / 6, riblocksize, riblocksize / 3};
		SDL_SetRenderDrawColor(GameBase->Game->Renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(GameBase->Game->Renderer, &r);
	}
}

//PLAYER ----------------------------------------------------------------------------------------------------------------

void CGameRamIt::createplayer()
{
	playerpos.x = GameBase->screenleft + (GameBase->screenright - GameBase->screenleft) / 2;
	playerpos.y = GameBase->screentop + (GameBase->screenbottom - GameBase->screentop) / 2;
	playerdx = 1;
}

void CGameRamIt::drawplayer()
{
	SDL_Rect r;

	SDL_SetRenderDrawColor(GameBase->Game->Renderer, 0x80, 0x80, 0x80, 0xFF);
	r = {GameBase->screenleft + (GameBase->screenright - GameBase->screenleft -playerrailwidth) / 2, GameBase->screentop, playerrailwidth, GameBase->screenbottom - GameBase->screentop };
	SDL_RenderFillRect(GameBase->Game->Renderer, &r);

	SDL_SetRenderDrawColor(GameBase->Game->Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	r = {playerpos.x - riblocksize / 2, playerpos.y - riblocksize / 2, riblocksize, riblocksize};
	SDL_RenderFillRect(GameBase->Game->Renderer, &r);

	SDL_SetRenderDrawColor(GameBase->Game->Renderer, 0xFF, 0x40, 0x40, 0xFF);
	if (playerdx == 1)
		r ={playerpos.x + riblocksize / 2, playerpos.y - riblocksize / 4, riblocksize / 2, riblocksize / 2};
	else
		r ={playerpos.x - riblocksize, playerpos.y - riblocksize / 4, riblocksize / 2, riblocksize / 2};
	SDL_RenderFillRect(GameBase->Game->Renderer, &r);
}

void CGameRamIt::updateplayer()
{
	if ((GameBase->Game->Input->Buttons.ButLeft) ||
		(GameBase->Game->Input->Buttons.ButLeft2) ||
		(GameBase->Game->Input->Buttons.ButDpadLeft))
		playerdx = -1;

	if ((GameBase->Game->Input->Buttons.ButRight) ||
		(GameBase->Game->Input->Buttons.ButRight2) ||
		(GameBase->Game->Input->Buttons.ButDpadRight))
		playerdx = 1;

	if ((GameBase->Game->Input->Buttons.ButDown) ||
		(GameBase->Game->Input->Buttons.ButDown2) ||
		(GameBase->Game->Input->Buttons.ButDpadDown))
	{
		if (playerpos.y + riblocksize / 2 + playerspeed < GameBase->screenbottom)
			playerpos.y += playerspeed;
		else
			playerpos.y = GameBase->screenbottom - riblocksize / 2;
	}

	if ((GameBase->Game->Input->Buttons.ButUp) ||
		(GameBase->Game->Input->Buttons.ButUp2) ||
		(GameBase->Game->Input->Buttons.ButDpadUp))
	{
		if (playerpos.y - riblocksize / 2 - playerspeed > GameBase->screentop)
			playerpos.y -= playerspeed;
		else
			playerpos.y = GameBase->screentop + riblocksize / 2;
	}

	if (GameBase->Game->Input->Buttons.ButA && !GameBase->Game->Input->PrevButtons.ButA)
		if (createbullet())
		{
			GameBase->Game->Audio->PlaySound(SfxShoot, 0);
		}
}

//playfield ----------------------------------------------------------------------------------------------------------------

void CGameRamIt::createplayfield()
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

			playfield[side][block].color = color;
			playfield[side][block].segments = 2;
			playfield[side][block].maxsegments = 2;
		}
	}
}

void CGameRamIt::updateplayfield(bool force)
{
	speedcount += 1;

	if (force || (speedcount >= speed - ((GameBase->level-1) * tickslevelmul)))
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
			GameBase->Game->Audio->PlaySound(SfxSucces, 0);
			createplayfield();
			GameBase->level += 1;
			GameBase->Game->AddToScore((GameBase->level-1) * 100);
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

void CGameRamIt::drawplayfield()
{
	for (int side = 0; side < sides; side++)
		for (int block = 0; block < numblocks; block++)
		{
			SDL_Rect r;
			if (side == 0)
				r = {GameBase->screenleft, GameBase->screentop + block * riblocksizespacing + blockspacing,
					playfield[side][block].segments * segmentwidth,
					riblocksize};
			else
				r = { GameBase->screenright - playfield[side][block].segments * segmentwidth ,
					GameBase->screentop + block * riblocksizespacing + blockspacing,
					playfield[side][block].segments * segmentwidth,
					riblocksize};
			SDL_SetRenderDrawColor(GameBase->Game->Renderer, playfield[side][block].color.r, playfield[side][block].color.g, playfield[side][block].color.b,
					playfield[side][block].color.a);
			SDL_RenderFillRect(GameBase->Game->Renderer, &r);
		}
}

//background ----------------------------------------------------------------------------------------------------------------

void CGameRamIt::DrawBackground()
{
	SDL_SetRenderDrawColor(GameBase->Game->Renderer, 150, 150, 150, 255);
	SDL_RenderClear(GameBase->Game->Renderer);
	SDL_SetRenderDrawColor(GameBase->Game->Renderer, 0, 0, 0, 255);
	SDL_Rect r = {GameBase->screenleft, GameBase->screentop, GameBase->playfieldwidth, GameBase->playfieldheight};
	SDL_RenderFillRect(GameBase->Game->Renderer, &r);
}

//init - deinit ----------------------------------------------------------------------------------------------------------------

void CGameRamIt::init()
{
	bulletalive = false;
	speed = ticksidle;
	GameBase->level = 1;
	playerdeath = false;
	createplayer();
	createplayfield();
	GameBase->HealthPoints = 3;
	LoadSound();
	GameBase->Game->CurrentGameMusicID = MusMusic;
	GameBase->Game->Audio->PlayMusic(MusMusic, -1);
}

void CGameRamIt::deinit()
{
	UnLoadSound();
	GameBase->Game->SubStateCounter = 0;
	GameBase->Game->SubGameState = SGNone;
	GameBase->Game->CurrentGameMusicID = -1;
}

void CGameRamIt::LoadSound()
{
	SfxShoot = GameBase->Game->Audio->LoadSound("ramit/shoot.wav");
	SfxHit = GameBase->Game->Audio->LoadSound("ramit/hit.wav");
	SfxDie = GameBase->Game->Audio->LoadSound("common/die.wav");
	SfxSucces = GameBase->Game->Audio->LoadSound("common/succes.wav");
	MusMusic = GameBase->Game->Audio->LoadMusic("ramit/music.ogg");
}

void CGameRamIt::UnLoadSound()
{
	GameBase->Game->Audio->StopMusic();
	GameBase->Game->Audio->StopSound();
	GameBase->Game->Audio->UnLoadMusic(MusMusic);
	GameBase->Game->Audio->UnLoadSound(SfxShoot);
	GameBase->Game->Audio->UnLoadSound(SfxHit);
	GameBase->Game->Audio->UnLoadSound(SfxDie);
	GameBase->Game->Audio->UnLoadSound(SfxSucces);
}

//Update ----------------------------------------------------------------------------------------------------------------

void CGameRamIt::UpdateObjects(bool IsGameState)
{
	if (IsGameState)
	{
		updateplayer();
		updateplayfield(false);
		updatebullet();

		if (playerdeath)
		{
			GameBase->Game->Audio->PlaySound(SfxDie, 0);
			if (GameBase->Game->GameMode == GMGame)
				GameBase->HealthPoints -= 1;

			if (GameBase->HealthPoints > 0)
			{
				SDL_Delay(500);
				createplayfield();
			}
			playerdeath = false;
		}
	}
}

void CGameRamIt::UpdateLogic()
{
	GameBase->UpdateLogic(GameBase);
	UpdateObjects(GameBase->Game->SubGameState == SGGame);
	if(GameBase->Game->SubGameState == SGGame)
		GameBase->Game->Sprites->UpdateSprites(GameBase->Game->Renderer);
}

bool CGameRamIt::DrawObjects()
{
	drawbullet();
	drawplayer();
	drawplayfield();
	//don't call drawsprites in base class
	return false;
}


void CGameRamIt::Draw()
{
	DrawBackground();
	if (DrawObjects())
		GameBase->Game->Sprites->DrawSprites(GameBase->Game->Renderer);
	GameBase->DrawScoreBar(GameBase);
	GameBase->DrawSubStateText(GameBase);
}