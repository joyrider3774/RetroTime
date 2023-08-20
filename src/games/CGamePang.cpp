#include <SDL.h>
#include <string>
#include <iostream>
#include <cmath>
#include "CGamePang.h"
#include "CGameBase.h"
#include "../CGame.h"
#include "../Common.h"
#include "../Vec2F.h"

using namespace std;

CGamePang* Create_CGamePang(CGame* aGame)
{
	CGamePang* GamePang = (CGamePang*) malloc(sizeof(CGamePang));
	GamePang->GameBase = Create_CGameBase(aGame, GSPang, true);

	GamePang->MusMusic = -1;
	GamePang->SfxSucces = -1;
	GamePang->SfxDie = -1;
	GamePang->SfxShoot = -1;
	GamePang->SfxPop = -1;
	GamePang->GameBase->screenleft = 0;
	GamePang->GameBase->screenright = ScreenWidth;
	GamePang->GameBase->screentop = 0;
	GamePang->GameBase->screenbottom = ScreenHeight;

	GamePang->drawplayer = CGamePang_drawplayer;
	GamePang->updateplayer = CGamePang_updateplayer;
	GamePang->remplayerstate = CGamePang_remplayerstate;
	GamePang->playerstate = CGamePang_playerstate;
	GamePang->addplayerstate = CGamePang_addplayerstate;
	GamePang->createplayer = CGamePang_createplayer;
	GamePang->destroyplayer = CGamePang_destroyplayer;
	GamePang->drawbullet = CGamePang_drawbullet;
	GamePang->updatebullet = CGamePang_updatebullet;
	GamePang->createbullet = CGamePang_createbullet;
	GamePang->destroybullet = CGamePang_destroybullet;
	GamePang->createballs = CGamePang_createballs;
	GamePang->drawballs = CGamePang_drawballs;
	GamePang->updateballs = CGamePang_updateballs;
	GamePang->createball = CGamePang_createball;
	GamePang->destroyball = CGamePang_destroyball;
	GamePang->destroyallballs = CGamePang_destroyallballs;

	for (int i=0; i < GamePang->maxballs; i++)
		Initialize_CSpriteObject(&GamePang->balls[i]);
	
	Initialize_CSpriteObject(&GamePang->player);
	Initialize_CSpriteObject(&GamePang->bullet);

	GamePang->init = CGamePang_init;
	GamePang->deinit = CGamePang_deinit;
	GamePang->UnloadGraphics = CGamePang_UnloadGraphics;
	GamePang->LoadGraphics = CGamePang_LoadGraphics;
	GamePang->LoadSound = CGamePang_LoadSound;
	GamePang->UnLoadSound = CGamePang_UnLoadSound;
	GamePang->UpdateObjects = CGamePang_UpdateObjects;
	GamePang->DrawObjects = CGamePang_DrawObjects;
	GamePang->DrawBackground = CGamePang_DrawBackground;
	GamePang->Draw = CGamePang_Draw;
	GamePang->UpdateLogic = CGamePang_UpdateLogic;
	return GamePang;
}

void Destroy_CGamePang(CGamePang* GamePang)
{
	Destroy_CGameBase(GamePang->GameBase);
	free(GamePang);
}



//balls ----------------------------------------------------------------------------------------------------------------

void CGamePang_destroyallballs(CGamePang* GamePang)
{
	for(int i = 0; i < GamePang->maxballs; i++)
		GamePang->destroyball(GamePang,i, true);
}

void CGamePang_destroyball(CGamePang* GamePang, int index, bool nocreate)
{
	if (!nocreate)
	{
		if (GamePang->balls[index].id > GamePang->ballsmall)
		{
			GamePang->createball(GamePang,GamePang->balls[index].id - 1, GamePang->balls[index].pos.x - 25*yscale, GamePang->balls[index].pos.y, -10.0f*yscale);
			GamePang->createball(GamePang,GamePang->balls[index].id - 1, GamePang->balls[index].pos.x + 25*yscale, GamePang->balls[index].pos.y, 10.0f*yscale);
		}
	}
	if (GamePang->balls[index].alive)
	{
		GamePang->GameBase->Game->Sprites->RemoveSprite(GamePang->balls[index].spr);
		GamePang->balls[index].alive = false;
	}
}

void CGamePang_createball(CGamePang* GamePang, int size, float x, float y, float speed)
{
	for (int i = 0; i < GamePang->maxballs; i++)
	{
		if (!GamePang->balls[i].alive)
		{
			GamePang->balls[i].spr = GamePang->GameBase->Game->Sprites->CreateSprite();
			GamePang->GameBase->Game->Sprites->SetSpriteImage(GamePang->GameBase->Game->Renderer,GamePang->balls[i].spr, &GamePang->spritesheetball);
			GamePang->GameBase->Game->Sprites->SetSpriteCollisionShape(GamePang->balls[i].spr, SHAPE_CIRCLE, 35,35,0,0,0);
			//GamePang->GameBase->Game->Sprites->SetSpriteCollisionShape(GamePang->balls[i].spr, SHAPE_BOX, 27,27,0,0,0);
			GamePang->GameBase->Game->Sprites->SetSpriteColour(GamePang->balls[i].spr, 1, 1, 1, 0.7f);
			Vec2F Scale = GamePang->ballscale;
			Scale.x = Scale.x * size;
			Scale.y = Scale.y * size;
			GamePang->GameBase->Game->Sprites->SetSpriteScale(GamePang->GameBase->Game->Renderer,GamePang->balls[i].spr, Scale);
			GamePang->GameBase->Game->Sprites->SetSpriteDepth(GamePang->balls[i].spr, 6);
			GamePang->balls[i].tz = GamePang->GameBase->Game->Image->ImageSize(GamePang->spritesheetball);
			GamePang->balls[i].tz.x = GamePang->balls[i].tz.x * Scale.x;
			GamePang->balls[i].tz.x = GamePang->balls[i].tz.y * Scale.y;
			GamePang->balls[i].pos = { x, y};
			GamePang->GameBase->Game->Sprites->SetSpriteLocation(GamePang->balls[i].spr, GamePang->balls[i].pos);
			GamePang->balls[i].alive = true;
			GamePang->balls[i].speed = speed*0.1f;
			GamePang->balls[i].force = -abs(speed);
			GamePang->balls[i].curforce = GamePang->balls[i].force/3;
			GamePang->balls[i].id = size;
			break;
		}
	}
}

void CGamePang_updateballs(CGamePang* GamePang)
{
	GamePang->levelcleared = true;
	for (int i = 0; i < GamePang->maxballs; i++)
	{
		if (GamePang->balls[i].alive)
		{
			GamePang->levelcleared = false;
			if (GamePang->balls[i].pos.x + GamePang->balls[i].speed > GamePang->GameBase->screenright)
			{
				if (GamePang->balls[i].speed > 0)
					GamePang->balls[i].speed = -abs(GamePang->balls[i].speed);

			}

			if (GamePang->balls[i].pos.x + GamePang->balls[i].speed < GamePang->GameBase->screenleft)
			{
				if (GamePang->balls[i].speed < 0)
					GamePang->balls[i].speed = abs(GamePang->balls[i].speed);
			}


			GamePang->balls[i].pos.x += GamePang->balls[i].speed*2;

			if (GamePang->balls[i].id == GamePang->ballbig)
				GamePang->balls[i].curforce += 0.1f*yscale;
			else
			{
				if(GamePang->balls[i].id == GamePang->ballmedium)
					GamePang->balls[i].curforce += 0.15f*yscale;
				else
				{
					if (GamePang->balls[i].id == GamePang->ballsmall)
						GamePang->balls[i].curforce += 0.25f*yscale;
				}
			}

			GamePang->balls[i].pos.y += GamePang->balls[i].curforce;

			if (GamePang->balls[i].id == GamePang->ballbig)
				if (GamePang->balls[i].pos.y >= GamePang->GameBase->screenbottom - 135.0f*yscale)
					GamePang->balls[i].curforce = GamePang->balls[i].force;

			if (GamePang->balls[i].id == GamePang->ballmedium)
				if (GamePang->balls[i].pos.y >= GamePang->GameBase->screenbottom - 100.0f*yscale)
					GamePang->balls[i].curforce = GamePang->balls[i].force;

			if (GamePang->balls[i].id == GamePang->ballsmall)
				if (GamePang->balls[i].pos.y >= GamePang->GameBase->screenbottom - 75.0f*yscale)
					GamePang->balls[i].curforce = GamePang->balls[i].force;

			GamePang->GameBase->Game->Sprites->SetSpriteLocation(GamePang->balls[i].spr, GamePang->balls[i].pos);

			if (!GamePang->playerstate(GamePang,GamePang->playerstatereviving) && GamePang->GameBase->Game->Sprites->DetectSpriteCollision(GamePang->balls[i].spr, GamePang->player.spr))
			{
				GamePang->GameBase->Game->AddToScore(-25);
				GamePang->GameBase->HealthPoints -= 1;
				GamePang->addplayerstate(GamePang,GamePang->playerstatereviving);
				GamePang->remplayerstate(GamePang,GamePang->playerstateshoot);
				GamePang->player.stateticks = 90;
				GamePang->GameBase->Game->Audio->PlaySound(GamePang->SfxDie, 0);
			}

			if (GamePang->bullet.alive && (GamePang->bullet.freeze == 0))
			{
				if (GamePang->GameBase->Game->Sprites->DetectSpriteCollision(GamePang->balls[i].spr, GamePang->bullet.spr))
				{
					GamePang->GameBase->Game->AddToScore(GamePang->balls[i].id * 7);
					GamePang->destroyball(GamePang,i, false);
					GamePang->bullet.freeze = 4;
					GamePang->GameBase->Game->Sprites->SetSpriteAnimation(GamePang->bullet.spr, 0, 1, 10);
					GamePang->GameBase->Game->Audio->PlaySound(GamePang->SfxPop, 0);
				}
			}
		}
	}
	if (GamePang->levelcleared)
	{
		if (GamePang->GameBase->level < GamePang->maxbigballs)
			GamePang->GameBase->level += 1;
		GamePang->GameBase->Game->Audio->PlaySound(GamePang->SfxSucces, 0);
		GamePang->GameBase->Game->AddToScore(100);
		GamePang->createballs(GamePang);
	}
}

void CGamePang_drawballs(CGamePang* GamePang)
{
	for (int i = 0; i < GamePang->maxballs; i++)
	{
		if (GamePang->balls[i].alive)
			GamePang->GameBase->Game->Sprites->DrawSprite(GamePang->GameBase->Game->Renderer, GamePang->balls[i].spr);

	}
}

void CGamePang_createballs(CGamePang* GamePang)
{
	int added = 0;
	float speed = 10.0f*yscale;
	for (int i = GamePang->GameBase->screenleft; i < GamePang->GameBase->screenright; i += int((GamePang->GameBase->screenright - GamePang->GameBase->screenleft) / (GamePang->GameBase->level + 1)) + 1)
	{
		if ((i > GamePang->GameBase->screenleft) && (i < GamePang->GameBase->screenright))
		{
			GamePang->createball(GamePang,GamePang->ballbig, i, 160.0f*yscale, speed);
			speed *= -1;
			added += 1;
			if (added >= GamePang->GameBase->level)
				break;
		}
	}
}

//bullet ----------------------------------------------------------------------------------------------------------------

void CGamePang_destroybullet(CGamePang* GamePang)
{
	if (GamePang->bullet.alive)
	{
		GamePang->GameBase->Game->Sprites->RemoveSprite(GamePang->bullet.spr);
		GamePang->bullet.alive = false;
	}
}

void CGamePang_createbullet(CGamePang* GamePang)
{
	GamePang->bullet.spr = GamePang->GameBase->Game->Sprites->CreateSprite();
	GamePang->GameBase->Game->Sprites->SetSpriteImage(GamePang->GameBase->Game->Renderer,GamePang->bullet.spr, &GamePang->spritesheetbullet, 1, 2);
	GamePang->GameBase->Game->Sprites->SetSpriteAnimation(GamePang->bullet.spr, 0, 0, 0);
	GamePang->GameBase->Game->Sprites->SetSpriteCollisionShape(GamePang->bullet.spr, SHAPE_BOX, GamePang->GameBase->Game->Sprites->TileSize(GamePang->bullet.spr).y-18, GamePang->GameBase->Game->Sprites->TileSize(GamePang->bullet.spr).x+160,0,0,0);

	GamePang->GameBase->Game->Sprites->SetSpriteRotation(GamePang->bullet.spr, 90);
	GamePang->GameBase->Game->Sprites->SetSpriteScale(GamePang->GameBase->Game->Renderer,GamePang->bullet.spr, GamePang->bulletscale);
	GamePang->GameBase->Game->Sprites->SetSpriteDepth(GamePang->bullet.spr, 3);
	GamePang->bullet.tz = GamePang->GameBase->Game->Sprites->TileSize(GamePang->bullet.spr);
	GamePang->bullet.tz.x = GamePang->bullet.tz.x * GamePang->bulletscale.x;
	GamePang->bullet.tz.y = GamePang->bullet.tz.y * GamePang->bulletscale.y;
	int tmpx = GamePang->bullet.tz.x;
	GamePang->bullet.tz.x = GamePang->bullet.tz.y;
	GamePang->bullet.tz.y = tmpx;
	GamePang->bullet.pos = { GamePang->player.pos.x, GamePang->player.pos.y - (GamePang->player.tz.y) + (GamePang->bullet.tz.y /2)};
	GamePang->bullet.vel = {0, -GamePang->bulletspeed};
	GamePang->GameBase->Game->Sprites->SetSpriteLocation(GamePang->bullet.spr, GamePang->bullet.pos);
	GamePang->bullet.alive = true;
}

void CGamePang_updatebullet(CGamePang* GamePang)
{
	if (GamePang->bullet.alive)
	{
		if (GamePang->bullet.freeze > 0)
		{
			GamePang->bullet.freeze -= 1;
			if (GamePang->bullet.freeze == 0)
			{
				GamePang->remplayerstate(GamePang,GamePang->playerstateshoot);
				GamePang->destroybullet(GamePang);
			}
		}
		else
		{
			GamePang->bullet.pos.x += GamePang->bullet.vel.x;
			GamePang->bullet.pos.y += GamePang->bullet.vel.y;
			if (GamePang->bullet.pos.y - GamePang->bullet.tz.y /2 <= GamePang->GameBase->screentop)
			{
				GamePang->bullet.freeze = 6;
				GamePang->GameBase->Game->Sprites->SetSpriteAnimation(GamePang->bullet.spr, 0, 1, 10);
			}
			GamePang->GameBase->Game->Sprites->SetSpriteLocation(GamePang->bullet.spr, GamePang->bullet.pos);
		}
	}
}

void CGamePang_drawbullet(CGamePang* GamePang)
{
	if (GamePang->bullet.alive)
	{
		//copy whats on screen to a temporary buffer (i know this already contains the background)
		SDL_Texture* prev = SDL_GetRenderTarget(GamePang->GameBase->Game->Renderer);
		SDL_SetRenderTarget(GamePang->GameBase->Game->Renderer, GamePang->GameBase->Game->TexTmp);
		GamePang->GameBase->Game->Image->DrawImage(GamePang->GameBase->Game->Renderer, prev, NULL, NULL);
		SDL_SetRenderTarget(GamePang->GameBase->Game->Renderer, prev);
		//draw the sprite
		GamePang->GameBase->Game->Sprites->DrawSprite(GamePang->GameBase->Game->Renderer, GamePang->bullet.spr);
		//draw bottom part of what was previously on screen back to the screen to obscure bottom part of the chain texture
		//this makes it seem as if the texture is created on the ground instead of at the bottom of the screen, like it is
		//in real time.
		SDL_Rect Rect = {0, GamePang->GameBase->screenbottom - GamePang->backgroundcopyheight, GamePang->GameBase->screenright, GamePang->backgroundcopyheight};
		GamePang->GameBase->Game->Image->DrawImage(GamePang->GameBase->Game->Renderer, GamePang->GameBase->Game->TexTmp, &Rect, &Rect);
	}
}

//player ----------------------------------------------------------------------------------------------------------------

void CGamePang_destroyplayer(CGamePang* GamePang)
{
	if(GamePang->player.alive)
	{
		GamePang->GameBase->Game->Sprites->RemoveSprite(GamePang->player.spr);
		GamePang->player.alive = false;
	}
}

void CGamePang_createplayer(CGamePang* GamePang)
{
	GamePang->player.spr = GamePang->GameBase->Game->Sprites->CreateSprite();
	GamePang->GameBase->Game->Sprites->SetSpriteImage(GamePang->GameBase->Game->Renderer,GamePang->player.spr, &GamePang->spritesheetplayer, 12, 8);
	GamePang->GameBase->Game->Sprites->SetSpriteAnimation(GamePang->player.spr, 37, 37, 0);
	GamePang->GameBase->Game->Sprites->SetSpriteCollisionShape(GamePang->player.spr, SHAPE_BOX, GamePang->GameBase->Game->Sprites->TileSize(GamePang->player.spr).x - 24, GamePang->GameBase->Game->Sprites->TileSize(GamePang->player.spr).y-14,0,0,14*yscale);

	GamePang->GameBase->Game->Sprites->SetSpriteScale(GamePang->GameBase->Game->Renderer,GamePang->player.spr, GamePang->playerscale);
	GamePang->GameBase->Game->Sprites->SetSpriteDepth(GamePang->player.spr, 37);
	GamePang->player.state = GamePang->playerstateidle;
	GamePang->player.tz = GamePang->GameBase->Game->Sprites->TileSize(GamePang->player.spr);
	GamePang->player.tz.x = GamePang->player.tz.x * GamePang->playerscale.x;
	GamePang->player.tz.y = GamePang->player.tz.y * GamePang->playerscale.y;
	GamePang->player.pos = { (float)(GamePang->GameBase->screenright - GamePang->GameBase->screenleft) / 2,(float)GamePang->GameBase->screenbottom -10- GamePang->player.tz.y / 2};
	GamePang->GameBase->HealthPoints = 3;
	GamePang->GameBase->Game->Sprites->SetSpriteLocation(GamePang->player.spr, GamePang->player.pos);
	GamePang->player.alive = true;
	GamePang->player.state = GamePang->playerstateidle;
}

void CGamePang_addplayerstate(CGamePang* GamePang, int state)
{
	GamePang->player.state |= state;
}

bool CGamePang_playerstate(CGamePang* GamePang, int state)
{
	return ((GamePang->player.state & state) == state);
}

void CGamePang_remplayerstate(CGamePang* GamePang, int state)
{
	if ((GamePang->player.state & state) == state)
		GamePang->player.state -= state;
}

void CGamePang_updateplayer(CGamePang* GamePang)
{
	GamePang->GameBase->Game->Sprites->SetSpriteVisibility(GamePang->player.spr, GamePang->player.alive);

	if (GamePang->player.alive)
	{
		if (GamePang->player.stateticks > 0)
			GamePang->player.stateticks -= 1;
		else
		{
			if (GamePang->playerstate(GamePang,GamePang->playerstateshoot))
				GamePang->remplayerstate(GamePang,GamePang->playerstateshoot);
			else
			{
				if (GamePang->playerstate(GamePang,GamePang->playerstatereviving))
					GamePang->remplayerstate(GamePang,GamePang->playerstatereviving);
			}
		}

		if (!GamePang->playerstate(GamePang,GamePang->playerstateshoot))
		{
			if ((GamePang->GameBase->Game->Input->Buttons.ButLeft) ||
				(GamePang->GameBase->Game->Input->Buttons.ButLeft2) ||
				(GamePang->GameBase->Game->Input->Buttons.ButDpadLeft))
			{
				if ( GamePang->player.pos.x - GamePang->player.tz.x / 2 - GamePang->playerspeed > GamePang->GameBase->screenleft)
				{
					GamePang->player.pos.x -=GamePang->playerspeed;
					if (!GamePang->playerstate(GamePang,GamePang->playerstatemoveleft))
					{
						GamePang->addplayerstate(GamePang,GamePang->playerstatemoveleft);
						GamePang->GameBase->Game->Sprites->SetSpriteAnimation(GamePang->player.spr, 12, 14, 10);
					}
				}
				else
				{
					GamePang->player.pos.x = GamePang->GameBase->screenleft + GamePang->player.tz.x / 2;
					if (GamePang->playerstate(GamePang,GamePang->playerstatemoveleft))
					{
						GamePang->remplayerstate(GamePang,GamePang->playerstatemoveleft);
						GamePang->GameBase->Game->Sprites->SetSpriteAnimation(GamePang->player.spr, 37, 37, 0);
					}
				}
			}
			else
			{
				if ((GamePang->GameBase->Game->Input->Buttons.ButRight) ||
					(GamePang->GameBase->Game->Input->Buttons.ButRight2) ||
					(GamePang->GameBase->Game->Input->Buttons.ButDpadRight))
				{
					if ( GamePang->player.pos.x + GamePang->player.tz.x / 2 + GamePang->playerspeed < GamePang->GameBase->screenright)
					{
						GamePang->player.pos.x += GamePang->playerspeed;
						if (!GamePang->playerstate(GamePang,GamePang->playerstatemoveright))
						{
							GamePang->addplayerstate(GamePang,GamePang->playerstatemoveright);
							GamePang->GameBase->Game->Sprites->SetSpriteAnimation(GamePang->player.spr, 24, 26, 10);
						}
					}
					else
					{
						GamePang->player.pos.x = GamePang->GameBase->screenright - GamePang->player.tz.x / 2;
						if (GamePang->playerstate(GamePang,GamePang->playerstatemoveleft))
						{
							GamePang->remplayerstate(GamePang,GamePang->playerstatemoveright);
							GamePang->GameBase->Game->Sprites->SetSpriteAnimation(GamePang->player.spr,37, 37, 0);
						}
					}
				}
				else
				{
					GamePang->remplayerstate(GamePang,GamePang->playerstatemoveright);
					GamePang->remplayerstate(GamePang,GamePang->playerstatemoveleft);
					GamePang->GameBase->Game->Sprites->SetSpriteAnimation(GamePang->player.spr, 37, 37, 0);
				}
			}

			if ((!GamePang->playerstate(GamePang,GamePang->playerstatereviving)) && (!GamePang->GameBase->Game->Input->PrevButtons.ButA && GamePang->GameBase->Game->Input->Buttons.ButA))
			{
				if (!GamePang->bullet.alive)
				{
					GamePang->addplayerstate(GamePang,GamePang->playerstateshoot);
					GamePang->remplayerstate(GamePang,GamePang->playerstatemoveright);
					GamePang->remplayerstate(GamePang,GamePang->playerstatemoveleft);
					GamePang->GameBase->Game->Sprites->SetSpriteAnimation(GamePang->player.spr, 37, 37, 10);
					GamePang->player.stateticks = 15;
					GamePang->createbullet(GamePang);
					GamePang->GameBase->Game->Audio->PlaySound(GamePang->SfxShoot, 0);
				}
			}
			GamePang->GameBase->Game->Sprites->SetSpriteLocation(GamePang->player.spr, GamePang->player.pos);
		}
	}
	else
	{
		if (GamePang->player.freeze > 0)
			GamePang->player.freeze -= 1;
		else
			GamePang->player.alive = true;
	}
}

void CGamePang_drawplayer(CGamePang* GamePang)
{
	if (GamePang->player.alive)
	{
		if (GamePang->playerstate(GamePang,GamePang->playerstatereviving))
		{
			if (GamePang->player.stateticks % 3 == 0)
				GamePang->GameBase->Game->Sprites->DrawSprite(GamePang->GameBase->Game->Renderer, GamePang->player.spr);
		}
		else
			GamePang->GameBase->Game->Sprites->DrawSprite(GamePang->GameBase->Game->Renderer, GamePang->player.spr);
	}
}


//background ----------------------------------------------------------------------------------------------------------------

void CGamePang_DrawBackground(CGamePang* GamePang)
{
	GamePang->GameBase->Game->Image->DrawImage(GamePang->GameBase->Game->Renderer, GamePang->background, NULL, NULL);
}

//init - deinit ----------------------------------------------------------------------------------------------------------------

void CGamePang_init(CGamePang* GamePang)
{
	GamePang->LoadGraphics(GamePang);
	GamePang->GameBase->level = 1;
	GamePang->LoadSound(GamePang);
	GamePang->createplayer(GamePang);
	GamePang->createballs(GamePang);
	GamePang->GameBase->Game->CurrentGameMusicID = GamePang->MusMusic;
	GamePang->GameBase->Game->Audio->PlayMusic(GamePang->MusMusic, -1);
}

void CGamePang_deinit(CGamePang* GamePang)
{
	GamePang->destroyplayer(GamePang);
	GamePang->destroyallballs(GamePang);
	GamePang->destroybullet(GamePang);
	GamePang->UnLoadSound(GamePang);
	GamePang->GameBase->Game->SubStateCounter = 0;
	GamePang->GameBase->Game->SubGameState = SGNone;
	GamePang->GameBase->Game->CurrentGameMusicID = -1;
	GamePang->UnloadGraphics(GamePang);
}

void CGamePang_LoadSound(CGamePang* GamePang)
{
	GamePang->SfxDie = GamePang->GameBase->Game->Audio->LoadSound("common/die.wav");
	GamePang->SfxSucces = GamePang->GameBase->Game->Audio->LoadSound("common/succes.wav");
	GamePang->SfxShoot = GamePang->GameBase->Game->Audio->LoadSound("pang/shoot.wav");
	GamePang->SfxPop = GamePang->GameBase->Game->Audio->LoadSound("pang/pop.wav");
	GamePang->MusMusic = GamePang->GameBase->Game->Audio->LoadMusic("pang/music.ogg");
}

void CGamePang_UnLoadSound(CGamePang* GamePang)
{
	GamePang->GameBase->Game->Audio->StopMusic();
	GamePang->GameBase->Game->Audio->StopSound();
	GamePang->GameBase->Game->Audio->UnLoadMusic(GamePang->MusMusic);
	GamePang->GameBase->Game->Audio->UnLoadSound(GamePang->SfxDie);
	GamePang->GameBase->Game->Audio->UnLoadSound(GamePang->SfxSucces);
	GamePang->GameBase->Game->Audio->UnLoadSound(GamePang->SfxShoot);
	GamePang->GameBase->Game->Audio->UnLoadSound(GamePang->SfxPop);
}

void CGamePang_LoadGraphics(CGamePang* GamePang)
{
	GamePang->background = GamePang->GameBase->Game->Image->LoadImage(GamePang->GameBase->Game->Renderer, "pang/background.png");
	GamePang->spritesheetplayer = GamePang->GameBase->Game->Image->LoadImage(GamePang->GameBase->Game->Renderer, "pang/character.png",0, 118, dumpScaledBitmaps);
	GamePang->spritesheetbullet = GamePang->GameBase->Game->Image->LoadImage(GamePang->GameBase->Game->Renderer, "pang/weapon.png", 0, 128, dumpScaledBitmaps);
	GamePang->spritesheetball = GamePang->GameBase->Game->Image->LoadImage(GamePang->GameBase->Game->Renderer, "pang/ball.png",0, 128, dumpScaledBitmaps);
	
	// SDL_SaveBMPTextureScaled(GamePang->GameBase->Game->Renderer, "./retrotimefs/graphics/pang/character.bmp", GamePang->GameBase->Game->Image->GetImage(GamePang->spritesheetplayer), 1,1, true, 0, 118);
	// SDL_SaveBMPTextureScaled(GamePang->GameBase->Game->Renderer, "./retrotimefs/graphics/pang/weapon.bmp", GamePang->GameBase->Game->Image->GetImage(GamePang->spritesheetbullet), 1,1, true, 0, 128);
	// SDL_SaveBMPTextureScaled(GamePang->GameBase->Game->Renderer, "./retrotimefs/graphics/pang/ball.bmp", GamePang->GameBase->Game->Image->GetImage(GamePang->spritesheetball), 1,1, true, 0, 128);


	if (!useDefaultColorAssets)
	{
		GamePang->UnloadGraphics(GamePang);
		GamePang->background = GamePang->GameBase->Game->Image->LoadImage(GamePang->GameBase->Game->Renderer, "pang/background.png");
		GamePang->spritesheetplayer = GamePang->GameBase->Game->Image->LoadImage(GamePang->GameBase->Game->Renderer, "pang/character.bmp");
		GamePang->spritesheetbullet = GamePang->GameBase->Game->Image->LoadImage(GamePang->GameBase->Game->Renderer, "pang/weapon.bmp");
	 	GamePang->spritesheetball = GamePang->GameBase->Game->Image->LoadImage(GamePang->GameBase->Game->Renderer, "pang/ball.bmp");
	}
	
	
	GamePang->spritesheetballtz = GamePang->GameBase->Game->Image->ImageSize(GamePang->spritesheetball);

}

void CGamePang_UnloadGraphics(CGamePang* GamePang)
{
	GamePang->GameBase->Game->Image->UnLoadImage(GamePang->spritesheetplayer);
	GamePang->GameBase->Game->Image->UnLoadImage(GamePang->spritesheetbullet);
	GamePang->GameBase->Game->Image->UnLoadImage(GamePang->spritesheetball);
	GamePang->GameBase->Game->Image->UnLoadImage(GamePang->background);
}

//Update ----------------------------------------------------------------------------------------------------------------

void CGamePang_UpdateObjects(CGamePang* GamePang, bool IsGameState)
{
	if (IsGameState)
	{
		GamePang->updateplayer(GamePang);
		GamePang->updateballs(GamePang);
		GamePang->updatebullet(GamePang);
	}
}

void CGamePang_UpdateLogic(CGamePang* GamePang)
{
	GamePang->GameBase->UpdateLogic(GamePang->GameBase);
	GamePang->UpdateObjects(GamePang, GamePang->GameBase->Game->SubGameState == SGGame);
	if(GamePang->GameBase->Game->SubGameState == SGGame)
		GamePang->GameBase->Game->Sprites->UpdateSprites(GamePang->GameBase->Game->Renderer);
}

bool CGamePang_DrawObjects(CGamePang* GamePang)
{
	GamePang->drawballs(GamePang);
	GamePang->drawbullet(GamePang);
	GamePang->drawplayer(GamePang);
	//don't call drawsprites in base class
	return false;
}

void CGamePang_Draw(CGamePang* GamePang)
{
	GamePang->DrawBackground(GamePang);
	if (GamePang->DrawObjects(GamePang))
		GamePang->GameBase->Game->Sprites->DrawSprites(GamePang->GameBase->Game->Renderer);
	GamePang->GameBase->DrawScoreBar(GamePang->GameBase);
	GamePang->GameBase->DrawSubStateText(GamePang->GameBase);
}