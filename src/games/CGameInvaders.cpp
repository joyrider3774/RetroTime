#include <SDL.h>
#include <string>
#include <iostream>
#include <cmath>
#include "CGameInvaders.h"
#include "../CGame.h"
#include "../Common.h"
#include "../Vec2F.h"

using namespace std;

CGameInvaders* Create_CGameInvaders()
{
	CGameInvaders* GameInvaders = (CGameInvaders*) malloc(sizeof(CGameInvaders));
	GameInvaders->GameBase = Create_CGameBase(GSSpaceInvaders, false);

	GameInvaders->MusMusic = -1;
	GameInvaders->SfxSucces = -1;
	GameInvaders->SfxPlayerShoot = -1;
	GameInvaders->SfxPlayerDeath = -1;
	GameInvaders->SfxEnemyShoot = -1;
	GameInvaders->SfxEnemyDeath = -1;
	GameInvaders->SfxDie = -1;
	GameInvaders->GameBase->screenleft = 0;
	GameInvaders->GameBase->screenright = ScreenWidth;
	GameInvaders->GameBase->screentop = 0;
	GameInvaders->GameBase->screenbottom = ScreenHeight;

	Initialize_CSpriteObject(&GameInvaders->player);
	Initialize_CSpriteObject(&GameInvaders->bullet);

	for (int i = 0; i < GameInvaders->enemycols * GameInvaders->enemycols; i++)
	{
		Initialize_CSpriteObject(&GameInvaders->enemies[i]);
		initialize_CTweenInfo(&GameInvaders->tweens[i][0]);
		initialize_CTweenInfo(&GameInvaders->tweens[i][1]);
	}

	for (int i = 0; i < GameInvaders->maxexplosion; i++)
		Initialize_CSpriteObject(&GameInvaders->explosions[i]);

	for (int i = 0; i < GameInvaders->maxasteroids; i++)
		Initialize_CSpriteObject(&GameInvaders->asteroids[i]);

	for (int i = 0; i < GameInvaders->maxenemybullets; i++)
		Initialize_CSpriteObject(&GameInvaders->enemybullets[i]);


	


	GameInvaders->updateplayer = CGameInvaders_updateplayer;
	GameInvaders->createplayer = CGameInvaders_createplayer;
	GameInvaders->destroyplayer = CGameInvaders_destroyplayer;
	GameInvaders->updateinvaders = CGameInvaders_updateinvaders;
	GameInvaders->createinvaders = CGameInvaders_createinvaders;
	GameInvaders->tweenactive = CGameInvaders_tweenactive;
	GameInvaders->destroyinvader = CGameInvaders_destroyinvader;
	GameInvaders->destroyallinvaders = CGameInvaders_destroyallinvaders;
	GameInvaders->updateenemyinfo = CGameInvaders_updateenemyinfo;
	GameInvaders->updatebullet = CGameInvaders_updatebullet;
	GameInvaders->destroybullet = CGameInvaders_destroybullet;
	GameInvaders->createbullet = CGameInvaders_createbullet;
	GameInvaders->updateasteroids = CGameInvaders_updateasteroids;
	GameInvaders->createasteroids = CGameInvaders_createasteroids;
	GameInvaders->destroyasteroid = CGameInvaders_destroyasteroid;
	GameInvaders->destroyallasteroids = CGameInvaders_destroyallasteroids;
	GameInvaders->updateenemybullet = CGameInvaders_updateenemybullet;
	GameInvaders->createnemybullet = CGameInvaders_createnemybullet;
	GameInvaders->destroyenemybullet = CGameInvaders_destroyenemybullet;
	GameInvaders->destroyallenemybullet = CGameInvaders_destroyallenemybullet;
	GameInvaders->createexplosion = CGameInvaders_createexplosion;
	GameInvaders->destroyexploison = CGameInvaders_destroyexploison;
	GameInvaders->destroyallexplosion = CGameInvaders_destroyallexplosion;
	GameInvaders->checkexplosions = CGameInvaders_checkexplosions;
	GameInvaders->init = CGameInvaders_init;
	GameInvaders->deinit = CGameInvaders_deinit;
	GameInvaders->UnloadGraphics = CGameInvaders_UnloadGraphics;
	GameInvaders->LoadGraphics = CGameInvaders_LoadGraphics;
	GameInvaders->LoadSound = CGameInvaders_LoadSound;
	GameInvaders->UnLoadSound = CGameInvaders_UnLoadSound;
	GameInvaders->UpdateObjects = CGameInvaders_UpdateObjects;
	GameInvaders->DrawObjects = CGameInvaders_DrawObjects;
	GameInvaders->DrawBackground = CGameInvaders_DrawBackground;
	GameInvaders->Draw = CGameInvaders_Draw;
	GameInvaders->UpdateLogic = CGameInvaders_UpdateLogic;
	return GameInvaders;
}

void Destroy_CGameInvaders(CGameInvaders* GameInvaders)
{
	Destroy_CGameBase(GameInvaders->GameBase);
	free(GameInvaders);
}

//explosions ----------------------------------------------------------------------------------------------------------------

void CGameInvaders_checkexplosions(CGameInvaders* GameInvaders)
{
	for(int i = 0; i < GameInvaders->maxexplosion; i++)
	{
		if (GameInvaders->explosions[i].alive)
		{
			if (Sprites->GetSpriteAnimFrame(GameInvaders->explosions[i].spr) == Sprites->GetSpriteAnimFrameCount(GameInvaders->explosions[i].spr) -1)
			{
				GameInvaders->destroyexploison(GameInvaders,i);
			}
		}
	}
}

void CGameInvaders_destroyallexplosion(CGameInvaders* GameInvaders)
{
	for (int i = 0; i < GameInvaders->maxexplosion; i++)
		GameInvaders->destroyexploison(GameInvaders,i);
}

void CGameInvaders_destroyexploison(CGameInvaders* GameInvaders, int index)
{
	if (GameInvaders->explosions[index].alive)
	{
		Sprites->RemoveSprite(GameInvaders->explosions[index].spr);
		GameInvaders->explosions[index].alive = false;
	}
}

void CGameInvaders_createexplosion(CGameInvaders* GameInvaders, Vec2F pos)
{
	for(int i = 0; i < GameInvaders->maxexplosion;i++)
	{
		if (!GameInvaders->explosions[i].alive)
		{
			GameInvaders->explosions[i].spr = Sprites->CreateSprite();
			Sprites->SetSpriteImage(Renderer,GameInvaders->explosions[i].spr, &GameInvaders->spritesheetExplosion, 7, 1);
			Sprites->SetSpriteScale(Renderer,GameInvaders->explosions[i].spr, GameInvaders->enemyscale);
			Sprites->SetSpriteDepth(GameInvaders->explosions[i].spr, 1);
			Sprites->SetSpriteAnimation(GameInvaders->explosions[i].spr,2, 6, 15);
			Sprites->SetSpriteLocation(GameInvaders->explosions[i].spr, pos);
			GameInvaders->explosions[i].alive = true;
			Audio->PlaySound(GameInvaders->SfxPlayerDeath, 0);
			break;
		}
	}
}

//enemy bullets ----------------------------------------------------------------------------------------------------------------

void CGameInvaders_destroyallenemybullet(CGameInvaders* GameInvaders)
{
	for(int i = 0; i < GameInvaders->maxenemybullets; i++)
		GameInvaders->destroyenemybullet(GameInvaders,i);
}

void CGameInvaders_destroyenemybullet(CGameInvaders* GameInvaders, int index)
{
	if (GameInvaders->enemybullets[index].alive)
	{
		Sprites->RemoveSprite(GameInvaders->enemybullets[index].spr);
		GameInvaders->enemybullets[index].alive = false;
	}
}

void CGameInvaders_createnemybullet(CGameInvaders* GameInvaders, Vec2F pos)
{
	for(int i = 0; i < GameInvaders->maxenemybullets; i++)
	{
		if (!GameInvaders->enemybullets[i].alive)
		{
			GameInvaders->enemybullets[i].spr = Sprites->CreateSprite();
			Sprites->SetSpriteImage(Renderer,GameInvaders->enemybullets[i].spr, &GameInvaders->spritesheetBullet, 2, 1);
			Sprites->SetSpriteColour(GameInvaders->enemybullets[i].spr, 0.75,0.65,0.65,1);
			Sprites->SetSpriteAnimation(GameInvaders->enemybullets[i].spr, 1, 1, 0);
			Sprites->SetSpriteScale(Renderer,GameInvaders->enemybullets[i].spr, GameInvaders->enemyscale);
			Sprites->SetSpriteDepth(GameInvaders->enemybullets[i].spr, -1);
			GameInvaders->enemybullets[i].tz = Sprites->TileSize(GameInvaders->enemybullets[i].spr);
			GameInvaders->enemybullets[i].tz.x = GameInvaders->enemybullets[i].tz.x * GameInvaders->enemyscale.x;
			GameInvaders->enemybullets[i].tz.y = GameInvaders->enemybullets[i].tz.y * GameInvaders->enemyscale.y;
			GameInvaders->enemybullets[i].pos = pos;
			GameInvaders->enemybullets[i].vel = {0, GameInvaders->bulletspeed};
			GameInvaders->enemybullets[i].alive = true;
			Audio->PlaySound(GameInvaders->SfxEnemyShoot, 0);
			break;
		}
	}
}

void CGameInvaders_updateenemybullet(CGameInvaders* GameInvaders)
{
	if (!GameInvaders->tweenactive(GameInvaders,GameInvaders->tweenenemypositions))
	{
		if (rand() % (GameInvaders->enemybulletrandomizer) == 0)
		{
			int col = rand() % (GameInvaders->enemycols);
			int y = 0;
			bool fired = false;
			while (!fired)
			{
				y = GameInvaders->enemyrows -1;
				while (y >= 0)
				{
					if (GameInvaders->enemies[col + y * GameInvaders->enemycols].alive)
					{
						GameInvaders->createnemybullet(GameInvaders,GameInvaders->enemies[col + y * GameInvaders->enemycols].pos);
						fired = true;
						break;
					}
					y -= 1;
				}
				col += 1;
				if (col >= GameInvaders->enemycols)
					col = 0;
			}
		}
	}

	for(int i = 0; i < GameInvaders->maxenemybullets; i++)
	{
		if (GameInvaders->enemybullets[i].alive)
		{
			GameInvaders->enemybullets[i].pos.x += GameInvaders->enemybullets[i].vel.x;
			GameInvaders->enemybullets[i].pos.y += GameInvaders->enemybullets[i].vel.y;
			Sprites->SetSpriteLocation(GameInvaders->enemybullets[i].spr, GameInvaders->enemybullets[i].pos);
			if (GameInvaders->enemybullets[i].pos.y >= GameInvaders->GameBase->screenbottom)
				GameInvaders->destroyenemybullet(GameInvaders,i);

			if (GameInvaders->enemybullets[i].alive && GameInvaders->player.alive)
			{
				if (Sprites->DetectSpriteCollision(GameInvaders->enemybullets[i].spr, GameInvaders->player.spr))
				{
					GameInvaders->player.alive = false;
					GameInvaders->player.freeze = 30;
					GameInvaders->createexplosion(GameInvaders,GameInvaders->player.pos);
					GameInvaders->destroyenemybullet(GameInvaders,i);
					CGame_AddToScore(-150);
					GameInvaders->deaths += 1;
					GameInvaders->GameBase->HealthPoints -= 1;
					Audio->PlaySound(GameInvaders->SfxDie, 0);
				}
			}


			for (int j = 0; j < GameInvaders->maxasteroids; j++)
			{
				if (GameInvaders->enemybullets[i].alive && GameInvaders->asteroids[j].alive)
				{
					if (Sprites->DetectSpriteCollision(GameInvaders->enemybullets[i].spr, GameInvaders->asteroids[j].spr))
					{
						GameInvaders->createexplosion(GameInvaders,GameInvaders->enemybullets[i].pos);
						GameInvaders->destroyenemybullet(GameInvaders,i);
						GameInvaders->asteroids[j].healthpoints -= 1;
						if (GameInvaders->asteroids[j].healthpoints == 0)
							GameInvaders->destroyasteroid(GameInvaders,j);
					}
				}
			}

		}
	}
}

//asteroids ----------------------------------------------------------------------------------------------------------------

void CGameInvaders_destroyallasteroids(CGameInvaders* GameInvaders)
{
	for(int i = 0; i < GameInvaders->maxasteroids; i++)
		GameInvaders->destroyasteroid(GameInvaders,i);
}

void CGameInvaders_destroyasteroid(CGameInvaders* GameInvaders, int index)
{
	if (GameInvaders->asteroids[index].alive)
	{
		Sprites->RemoveSprite(GameInvaders->asteroids[index].spr);
		GameInvaders->asteroids[index].alive = false;
	}
}

void CGameInvaders_createasteroids(CGameInvaders* GameInvaders)
{
	for(int i = 0; i < GameInvaders->maxasteroids; i++)
	{
		GameInvaders->asteroids[i].spr = Sprites->CreateSprite();
		GameInvaders->asteroids[i].healthpoints = GameInvaders->asteroidmaxhealthpoints;
		GameInvaders->asteroids[i].alive = true;
		Sprites->SetSpriteImage(Renderer,GameInvaders->asteroids[i].spr, &GameInvaders->spritesheetAsteroid);
		Sprites->SetSpriteAnimation(GameInvaders->asteroids[i].spr, 0, 0, 0);
		Sprites->SetSpriteLocation(GameInvaders->asteroids[i].spr, {(float)i * (GameInvaders->GameBase->screenright - GameInvaders->GameBase->screenleft) / (GameInvaders->maxasteroids-1), (float)GameInvaders->GameBase->screenbottom - GameInvaders->asteroidsoffset});
		Sprites->SetSpriteScale(Renderer,GameInvaders->asteroids[i].spr, {GameInvaders->asteroidscale, GameInvaders->asteroidscale});
		Sprites->SetSpriteCollisionShape(GameInvaders->asteroids[i].spr, SHAPE_BOX, 45,45,0, 0 , 2);
		Sprites->SetSpriteRotation(GameInvaders->asteroids[i].spr, rand() % (360));
	}
}

void CGameInvaders_updateasteroids(CGameInvaders* GameInvaders)
{
	for(int i = 0; i < GameInvaders->maxasteroids;i++)
	{
		if (GameInvaders->asteroids[i].alive)
		{
			Sprites->SetSpriteScale(Renderer,GameInvaders->asteroids[i].spr,
				{GameInvaders->asteroidscale - ((GameInvaders->asteroidscale / GameInvaders->asteroidmaxhealthpoints) * (GameInvaders->asteroidmaxhealthpoints - GameInvaders->asteroids[i].healthpoints)),
				GameInvaders->asteroidscale - ((GameInvaders->asteroidscale / GameInvaders->asteroidmaxhealthpoints) * (GameInvaders->asteroidmaxhealthpoints - GameInvaders->asteroids[i].healthpoints))});
			Sprites->SetSpriteRotation(GameInvaders->asteroids[i].spr, GameInvaders->asteroids[i].spr->rotation + 0.5f);
		}
	}
}

//player bullet ----------------------------------------------------------------------------------------------------------------

void CGameInvaders_createbullet(CGameInvaders* GameInvaders)
{
	if (!GameInvaders->bullet.alive)
	{
		GameInvaders->bullet.spr = Sprites->CreateSprite();
		Sprites->SetSpriteImage(Renderer,GameInvaders->bullet.spr, &GameInvaders->spritesheetBullet, 2, 1);
		Sprites->SetSpriteAnimation(GameInvaders->bullet.spr, 1, 1, 0);
		Sprites->SetSpriteScale(Renderer,GameInvaders->bullet.spr, GameInvaders->enemyscale);
		Sprites->SetSpriteDepth(GameInvaders->bullet.spr, -1);
		GameInvaders->bullet.tz = Sprites->TileSize(GameInvaders->bullet.spr);
		GameInvaders->bullet.tz.x = GameInvaders->bullet.tz.x * GameInvaders->enemyscale.x;
		GameInvaders->bullet.tz.y = GameInvaders->bullet.tz.y * GameInvaders->enemyscale.y;
		GameInvaders->bullet.pos = GameInvaders->player.pos ;
		GameInvaders->bullet.vel = {0, -GameInvaders->bulletspeed};
		GameInvaders->bullet.alive = true;
		Audio->PlaySound(GameInvaders->SfxPlayerShoot, 0);
	}
}

void CGameInvaders_destroybullet(CGameInvaders* GameInvaders)
{
	if (GameInvaders->bullet.alive)
	{
		Sprites->RemoveSprite(GameInvaders->bullet.spr);
		GameInvaders->bullet.alive = false;
	}
}

void CGameInvaders_updatebullet(CGameInvaders* GameInvaders)
{
	if (GameInvaders->bullet.alive)
	{
		GameInvaders->bullet.pos.x += GameInvaders->bullet.vel.x;
		GameInvaders->bullet.pos.y += GameInvaders->bullet.vel.y;
		Sprites->SetSpriteLocation(GameInvaders->bullet.spr, GameInvaders->bullet.pos);

		for (int x = 0; x < GameInvaders->enemycols; x++)
		{
			for(int y = 0; y < GameInvaders->enemyrows; y++)
			{
				if (GameInvaders->bullet.alive && GameInvaders->enemies[y * GameInvaders->enemycols + x].alive)
				{
					if (Sprites->DetectSpriteCollision(GameInvaders->bullet.spr, GameInvaders->enemies[y * GameInvaders->enemycols + x].spr))
					{
						GameInvaders->createexplosion(GameInvaders,GameInvaders->enemies[y * GameInvaders->enemycols + x].pos);
						GameInvaders->destroybullet(GameInvaders);
						GameInvaders->destroyinvader(GameInvaders,y * GameInvaders->enemycols + x);
						if (y < 1)
							CGame_AddToScore(25);
						else
						{
							if (y < 3)
								CGame_AddToScore(20);
							else
								CGame_AddToScore(10);
						}
						GameInvaders->enemyvel.x += GameInvaders->enemyvel.x / abs(GameInvaders->enemyvel.x) * GameInvaders->enemyspeedinc;
					}
				}
			}
		}

		for(int i = 0; i < GameInvaders->maxasteroids; i++)
		{
			if (GameInvaders->bullet.alive && GameInvaders->asteroids[i].alive)
			{
				if (Sprites->DetectSpriteCollision(GameInvaders->bullet.spr, GameInvaders->asteroids[i].spr))
				{
					GameInvaders->createexplosion(GameInvaders,GameInvaders->bullet.pos);
					GameInvaders->destroybullet(GameInvaders);
					GameInvaders->asteroids[i].healthpoints -= 1;
					if (GameInvaders->asteroids[i].healthpoints == 0)
						GameInvaders->destroyasteroid(GameInvaders,i);
				}
			}
		}

		if (GameInvaders->bullet.pos.y < GameInvaders->GameBase->screentop)
			GameInvaders->destroybullet(GameInvaders);
	}
}

//enemies ----------------------------------------------------------------------------------------------------------------

void CGameInvaders_updateenemyinfo(CGameInvaders* GameInvaders)
{
	GameInvaders->enemyinfo.mostleft = -1;
	GameInvaders->enemyinfo.mostright = -1;
	GameInvaders->enemyinfo.mostbottom = -1;

	float x1 = GameInvaders->GameBase->screenright + 1;
	float x2 = GameInvaders->GameBase->screenleft - 1;
	float y = GameInvaders->GameBase->screentop - 1;
	for(int i = 0; i < GameInvaders->enemycols * GameInvaders->enemyrows; i++)
	{
		if (GameInvaders->enemies[i].alive)
		{
			if (GameInvaders->enemies[i].pos.x < x1)
			{
				x1 = GameInvaders->enemies[i].pos.x;
				GameInvaders->enemyinfo.mostleft = i;
			}
			if (GameInvaders->enemies[i].pos.x > x2)
			{
				x2 = GameInvaders->enemies[i].pos.x;
				GameInvaders->enemyinfo.mostright = i;
			}
			if (GameInvaders->enemies[i].pos.y > y)
			{
				y = GameInvaders->enemies[i].pos.y;
				GameInvaders->enemyinfo.mostbottom = i;
			}
		}
	}
}

void CGameInvaders_destroyallinvaders(CGameInvaders* GameInvaders)
{
	for(int i = 0; i < GameInvaders->enemycols * GameInvaders->enemyrows; i++)
	{
		if (GameInvaders->enemies[i].alive)
		{
			GameInvaders->tweens[i][GameInvaders->tweenenemypositions].active = false;
			Sprites->RemoveSprite(GameInvaders->enemies[i].spr);
		}
	}
}

void CGameInvaders_destroyinvader(CGameInvaders* GameInvaders, int index)
{
	GameInvaders->enemies[index].alive = false;
	Sprites->RemoveSprite(GameInvaders->enemies[index].spr);
}

bool CGameInvaders_tweenactive(CGameInvaders* GameInvaders, int id)
{
	bool result = false;
	for(int i = 0; i < GameInvaders->enemyrows * GameInvaders->enemycols; i++)
	{
		if (GameInvaders->enemies[i].alive && GameInvaders->tweens[i][id].active)
		{
			result = true;
			break;
		}
	}
	return result;
}

void CGameInvaders_createinvaders(CGameInvaders* GameInvaders, bool setposition)
{
	GameInvaders->pattern = rand() % (5);
	for (int x = 0; x < GameInvaders->enemycols; x++)
	{
		for (int y = 0; y < GameInvaders->enemyrows; y++)
		{
			GameInvaders->tweens[x + y * GameInvaders->enemycols][GameInvaders->tweenenemypositions] = createtween(GameInvaders->tweenenemypositions, 1+ (rand() % ((int)DesiredFps))/100, funcsmoothstop, 1, true, DesiredFps);
			GameInvaders->enemies[x + y * GameInvaders->enemycols].alive = true;
			GameInvaders->enemies[x + y * GameInvaders->enemycols].spr = Sprites->CreateSprite();
			GameInvaders->enemies[x + y * GameInvaders->enemycols].pos = {(float) x * (GameInvaders->enemyspacing + GameInvaders->enemyhorzspacing) + GameInvaders->enemystartxoffset,(float)y * GameInvaders->enemyspacing + GameInvaders->enemystartyoffset};
			if (setposition)
				Sprites->SetSpriteLocation(GameInvaders->enemies[x + y * GameInvaders->enemycols].spr, GameInvaders->enemies[x + y * GameInvaders->enemycols].pos);
			SDL_Point tz = {1,1};
			if (y < 1)
			{
				Sprites->SetSpriteImage(Renderer,GameInvaders->enemies[x + y * GameInvaders->enemycols].spr, &GameInvaders->spritesheetEnemy3, 4, 1);
				Sprites->SetSpriteScale(Renderer,GameInvaders->enemies[x + y * GameInvaders->enemycols].spr, GameInvaders->enemyscale);
				Sprites->SetSpriteAnimation(GameInvaders->enemies[x + y * GameInvaders->enemycols].spr, 0, 3, 5);
				tz = Sprites->TileSize(GameInvaders->enemies[x + y * GameInvaders->enemycols].spr);
				Sprites->SetSpriteCollisionShape(GameInvaders->enemies[x + y * GameInvaders->enemycols].spr, SHAPE_BOX, tz.x-8, tz.y - 26,0, 0 , 6);
			}
			else
			{
				if (y < 3 )
				{
					Sprites->SetSpriteImage(Renderer,GameInvaders->enemies[x + y * GameInvaders->enemycols].spr, &GameInvaders->spritesheetEnemy2, 4, 1);
					Sprites->SetSpriteScale(Renderer,GameInvaders->enemies[x + y * GameInvaders->enemycols].spr, GameInvaders->enemyscale);

					Sprites->SetSpriteAnimation(GameInvaders->enemies[x + y * GameInvaders->enemycols].spr, 0, 3, 5);
					tz = Sprites->TileSize(GameInvaders->enemies[x + y * GameInvaders->enemycols].spr);
					Sprites->SetSpriteCollisionShape(GameInvaders->enemies[x + y * GameInvaders->enemycols].spr, SHAPE_BOX, tz.x-15, tz.y - 18,0, 0 , -2);
				}
				else
				{
					Sprites->SetSpriteImage(Renderer,GameInvaders->enemies[x + y * GameInvaders->enemycols].spr, &GameInvaders->spritesheetEnemy1, 5, 1);
					Sprites->SetSpriteScale(Renderer,GameInvaders->enemies[x + y * GameInvaders->enemycols].spr, GameInvaders->enemyscale);

					Sprites->SetSpriteAnimation(GameInvaders->enemies[x + y * GameInvaders->enemycols].spr,0, 4, 5);
					tz = Sprites->TileSize(GameInvaders->enemies[x + y * GameInvaders->enemycols].spr);
					Sprites->SetSpriteCollisionShape(GameInvaders->enemies[x + y * GameInvaders->enemycols].spr, SHAPE_BOX, tz.x-17, tz.y - 12,0, 0 , -2);
				}
			}
			GameInvaders->enemies[x + y * GameInvaders->enemycols].tz.x = tz.x * GameInvaders->enemyscale.x;
			GameInvaders->enemies[x + y * GameInvaders->enemycols].tz.y = tz.y * GameInvaders->enemyscale.y;
		}
	}
}

void CGameInvaders_updateinvaders(CGameInvaders* GameInvaders)
{
	GameInvaders->updateenemyinfo(GameInvaders);

	//all enemies cleared
	if (GameInvaders->enemyinfo.mostleft == -1)
	{
		CGame_AddToScore(250);
		GameInvaders->enemyvel = {GameInvaders->enemyspeed, 0};
		GameInvaders->createinvaders(GameInvaders,false);
		GameInvaders->destroybullet(GameInvaders);
		GameInvaders->destroyallenemybullet(GameInvaders);
		Audio->PlaySound(GameInvaders->SfxSucces, 0);
	}

	bool btweenactive = GameInvaders->tweenactive(GameInvaders,GameInvaders->tweenenemypositions);

	//enemies reached horizontal screen boundaries
	if (((GameInvaders->enemies[GameInvaders->enemyinfo.mostleft].pos.x - GameInvaders->enemies[GameInvaders->enemyinfo.mostleft].tz.x / 2 + GameInvaders->enemyvel.x < GameInvaders->GameBase->screenleft) ||
		(GameInvaders->enemies[GameInvaders->enemyinfo.mostright].pos.x + GameInvaders->enemies[GameInvaders->enemyinfo.mostright].tz.x / 2 + GameInvaders->enemyvel.x > GameInvaders->GameBase->screenright)) && !btweenactive)
	{
		for(int i = 0; i < GameInvaders->enemycols * GameInvaders->enemyrows; i++)
		{
			if (GameInvaders->enemies[i].alive)
				GameInvaders->enemies[i].pos.y += GameInvaders->enemyspacing / 4;
		}
		GameInvaders->enemyvel.x *= -1;
		GameInvaders->enemyvel.y *= -1;
	}

	//update enemy positions
	for(int i = 0; i < GameInvaders->enemycols * GameInvaders->enemyrows; i++)
	{
		if (GameInvaders->enemies[i].alive)
		{
			if (btweenactive)
			{
				GameInvaders->tweens[i][GameInvaders->tweenenemypositions] = updatetween( GameInvaders->tweens[i][GameInvaders->tweenenemypositions]);
				Vec2F pos = GameInvaders->enemies[i].pos;

				if (GameInvaders->pattern == 0)
				{
					if ((i % GameInvaders->enemycols) < GameInvaders->enemycols / 3)
						pos.x = pos.x * GameInvaders->tweens[i][GameInvaders->tweenenemypositions].funcval;
					else
					{
						if ((i % GameInvaders->enemycols) > GameInvaders->enemycols * 2 / 3)
							pos.x = GameInvaders->GameBase->screenright - (GameInvaders->GameBase->screenright - pos.x) * GameInvaders->tweens[i][GameInvaders->tweenenemypositions].funcval;
					}
					pos.y = pos.y * GameInvaders->tweens[i][GameInvaders->tweenenemypositions].funcval;
				}
				else
				{
					if(GameInvaders->pattern == 1)
						pos.y = pos.y * GameInvaders->tweens[i][GameInvaders->tweenenemypositions].funcval;
					else
					{
						if (GameInvaders->pattern == 2)
						{
							if (i % GameInvaders->enemycols < GameInvaders->enemycols / 2)
								pos.x = pos.x * GameInvaders->tweens[i][GameInvaders->tweenenemypositions].funcval;
							else
								pos.x = GameInvaders->GameBase->screenright - (GameInvaders->GameBase->screenright - pos.x) * GameInvaders->tweens[i][GameInvaders->tweenenemypositions].funcval;
						}
						else
						{
							if( GameInvaders->pattern == 3)
								pos.x = pos.x * GameInvaders->tweens[i][GameInvaders->tweenenemypositions].funcval;
							else
							{
								if (GameInvaders->pattern == 4)
									pos.x = GameInvaders->GameBase->screenright - (GameInvaders->GameBase->screenright - pos.x) * GameInvaders->tweens[i][GameInvaders->tweenenemypositions].funcval;
							}
						}
					}
				}
				Sprites->SetSpriteLocation(GameInvaders->enemies[i].spr, pos);
			}
			else
			{
				if (SubGameState == SGGame)
				{
					GameInvaders->enemies[i].pos.x += GameInvaders->enemyvel.x;
					GameInvaders->enemies[i].pos.y += GameInvaders->enemyvel.y;
					Sprites->SetSpriteLocation(GameInvaders->enemies[i].spr, GameInvaders->enemies[i].pos);
				}
			}
		}
	}

	//enemies reached bottom
	if ((GameInvaders->player.pos.y - GameInvaders->enemies[GameInvaders->enemyinfo.mostbottom].pos.y) < GameInvaders->endscreenconstant)
	{
		CGame_AddToScore(-250);
		GameInvaders->enemyvel = {GameInvaders->enemyspeed, 0};
		GameInvaders->destroyallinvaders(GameInvaders);
		GameInvaders->createinvaders(GameInvaders,false);
		GameInvaders->destroybullet(GameInvaders);
		GameInvaders->destroyallenemybullet(GameInvaders);
		GameInvaders->GameBase->HealthPoints -= 1;
		Audio->PlaySound(GameInvaders->SfxDie, 0);
	}
}

//player ----------------------------------------------------------------------------------------------------------------

void CGameInvaders_destroyplayer(CGameInvaders* GameInvaders)
{
	Sprites->RemoveSprite(GameInvaders->player.spr);
	GameInvaders->player.alive = false;
}

void CGameInvaders_createplayer(CGameInvaders* GameInvaders)
{
	GameInvaders->player.spr = Sprites->CreateSprite();
	Sprites->SetSpriteImage(Renderer,GameInvaders->player.spr, &GameInvaders->spritesheetPlayer, 5,1);
	Sprites->SetSpriteAnimation(GameInvaders->player.spr, 0, 0, 0);
	Sprites->SetSpriteScale(Renderer,GameInvaders->player.spr, GameInvaders->enemyscale);
	GameInvaders->player.tz = Sprites->TileSize(GameInvaders->player.spr);
	Sprites->SetSpriteCollisionShape(GameInvaders->player.spr, SHAPE_BOX, GameInvaders->player.tz.x-10, GameInvaders->player.tz.y - 6,0, 0, 12);
	GameInvaders->player.tz.x = GameInvaders->player.tz.x * GameInvaders->enemyscale.x;
	GameInvaders->player.tz.y = GameInvaders->player.tz.y * GameInvaders->enemyscale.y;
	GameInvaders->player.pos = { (float)(GameInvaders->GameBase->screenright - GameInvaders->GameBase->screenleft) / 2, (float)GameInvaders->GameBase->screenbottom - GameInvaders->player.tz.y / 2};
	GameInvaders->GameBase->HealthPoints = 3;
	GameInvaders->player.freeze = 0;
	Sprites->SetSpriteLocation(GameInvaders->player.spr, GameInvaders->player.pos);
}

void CGameInvaders_updateplayer(CGameInvaders* GameInvaders)
{
	Sprites->SetSpriteVisibility(GameInvaders->player.spr, GameInvaders->player.alive);
	if (GameInvaders->player.alive)
	{
		Sprites->SetSpriteAnimation(GameInvaders->player.spr, 0, 0, 0);

		if ((Input->Buttons.ButLeft) ||
			(Input->Buttons.ButLeft2) ||
			(Input->Buttons.ButDpadLeft))
		{
			if (GameInvaders->player.pos.x - GameInvaders->player.tz.x / 2 - GameInvaders->playerspeed > GameInvaders->GameBase->screenleft)
				GameInvaders->player.pos.x -= GameInvaders->playerspeed;
			else
				GameInvaders->player.pos.x = GameInvaders->GameBase->screenleft + GameInvaders->player.tz.x / 2;
			Sprites->SetSpriteAnimation(GameInvaders->player.spr, 1, 1, 0);
		}

		if ((Input->Buttons.ButRight) ||
			(Input->Buttons.ButRight2) ||
			(Input->Buttons.ButDpadRight))
		{
			if ( GameInvaders->player.pos.x + GameInvaders->player.tz.x / 2 + GameInvaders->playerspeed < GameInvaders->GameBase->screenright)
				GameInvaders->player.pos.x += GameInvaders->playerspeed;
			else
				GameInvaders->player.pos.x = GameInvaders->GameBase->screenright - GameInvaders->player.tz.x / 2;
			Sprites->SetSpriteAnimation(GameInvaders->player.spr, 4, 4, 0);
		}
		Sprites->SetSpriteLocation(GameInvaders->player.spr, GameInvaders->player.pos);

		if (!Input->PrevButtons.ButA && Input->Buttons.ButA)
			GameInvaders->createbullet(GameInvaders);
	}
	else
	{
		if (GameInvaders->player.freeze > 0)
			GameInvaders->player.freeze -= 1;
		else
			GameInvaders->player.alive = true;
	}
}


//background ----------------------------------------------------------------------------------------------------------------

void CGameInvaders_DrawBackground(CGameInvaders* GameInvaders)
{
	Image->DrawImage(Renderer, GameInvaders->background, NULL, NULL);
}

//init - deinit ----------------------------------------------------------------------------------------------------------------

void CGameInvaders_init(CGameInvaders* GameInvaders)
{
	GameInvaders->LoadGraphics(GameInvaders);

	GameInvaders->deaths = 0;
	GameInvaders->backgroundfade = 0;
	GameInvaders->backgroundfadeinc = 1;
	GameInvaders->enemyvel = {GameInvaders->enemyspeed, 0};

	GameInvaders->createinvaders(GameInvaders,false);
	GameInvaders->createplayer(GameInvaders);
	GameInvaders->createasteroids(GameInvaders);

	GameInvaders->LoadSound(GameInvaders);
	CurrentGameMusicID = GameInvaders->MusMusic;
	Audio->PlayMusic(GameInvaders->MusMusic, -1);
}

void CGameInvaders_deinit(CGameInvaders* GameInvaders)
{
	GameInvaders->destroyplayer(GameInvaders);
	GameInvaders->destroyallinvaders(GameInvaders);
	GameInvaders->destroybullet(GameInvaders);
	GameInvaders->destroyallasteroids(GameInvaders);
	GameInvaders->destroyallenemybullet(GameInvaders);
	GameInvaders->destroyallexplosion(GameInvaders);
	GameInvaders->UnLoadSound(GameInvaders);
	SubStateCounter = 0;
	SubGameState = SGNone;
	CurrentGameMusicID = -1;
	GameInvaders->UnloadGraphics(GameInvaders);
}

void CGameInvaders_LoadSound(CGameInvaders* GameInvaders)
{
	GameInvaders->SfxDie = Audio->LoadSound("common/die.wav");
	GameInvaders->SfxPlayerShoot = Audio->LoadSound("invaders/playershoot.wav");
	GameInvaders->SfxPlayerDeath = Audio->LoadSound("invaders/playerdeath.wav");
	GameInvaders->SfxEnemyShoot = Audio->LoadSound("invaders/enemyshoot.wav");
	GameInvaders->SfxEnemyDeath = Audio->LoadSound("invaders/enemydeath.wav");
	GameInvaders->SfxSucces = Audio->LoadSound("common/succes.wav");
	GameInvaders->MusMusic = Audio->LoadMusic("invaders/music.ogg");
}

void CGameInvaders_UnLoadSound(CGameInvaders* GameInvaders)
{
	Audio->StopMusic();
	Audio->StopSound();
	Audio->UnLoadMusic(GameInvaders->MusMusic);
	Audio->UnLoadSound(GameInvaders->SfxDie);
	Audio->UnLoadSound(GameInvaders->SfxPlayerShoot);
	Audio->UnLoadSound(GameInvaders->SfxPlayerDeath);
	Audio->UnLoadSound(GameInvaders->SfxEnemyShoot);
	Audio->UnLoadSound(GameInvaders->SfxEnemyDeath);
	Audio->UnLoadSound(GameInvaders->SfxSucces);
}

void CGameInvaders_LoadGraphics(CGameInvaders* GameInvaders)
{
	GameInvaders->background = Image->LoadImage(Renderer, "invaders/background.png");

	GameInvaders->spritesheetBullet = Image->LoadImage(Renderer, "invaders/bullet.png", 0, 100, dumpScaledBitmaps);
	GameInvaders->spritesheetExplosion = Image->LoadImage(Renderer, "invaders/explosion.png", 0, 100, dumpScaledBitmaps);
	GameInvaders->spritesheetAsteroid = Image->LoadImage(Renderer, "invaders/asteroid-01.png", 0, 75, dumpScaledBitmaps);
	GameInvaders->spritesheetEnemy1 = Image->LoadImage(Renderer, "invaders/enemy1.png",0, 80, dumpScaledBitmaps); //bottom
	GameInvaders->spritesheetEnemy2 = Image->LoadImage(Renderer, "invaders/enemy2.png",0, 80, dumpScaledBitmaps); //middle
	GameInvaders->spritesheetEnemy3 = Image->LoadImage(Renderer, "invaders/enemy3.png",0, 80, dumpScaledBitmaps); //top
	GameInvaders->spritesheetPlayer = Image->LoadImage(Renderer, "invaders/player.png",0, 80, dumpScaledBitmaps); //top
	
	// SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/invaders/bullet.bmp", Image->GetImage(GameInvaders->spritesheetBullet), 1,1, true, 0, 100);
	// SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/invaders/explosion.bmp", Image->GetImage(GameInvaders->spritesheetExplosion), 1,1,true, 0, 100);
	// SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/invaders/asteroid-01.bmp", Image->GetImage(GameInvaders->spritesheetAsteroid), 1,1,true, 0, 75);
	// SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/invaders/enemy1.bmp", Image->GetImage(GameInvaders->spritesheetEnemy1), 1,1,true, 0, 80);
	// SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/invaders/enemy2.bmp", Image->GetImage(GameInvaders->spritesheetEnemy2), 1,1,true, 0, 80);
	// SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/invaders/enemy3.bmp", Image->GetImage(GameInvaders->spritesheetEnemy3), 1,1,true, 0, 80);
	// SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/invaders/player.bmp", Image->GetImage(GameInvaders->spritesheetPlayer), 1,1,true, 0, 80);
	
	if(!useDefaultColorAssets)
	{
    	GameInvaders->UnloadGraphics(GameInvaders);
		GameInvaders->background = Image->LoadImage(Renderer, "invaders/background.png");
		GameInvaders->spritesheetBullet = Image->LoadImage(Renderer, "invaders/bullet.bmp");
		GameInvaders->spritesheetExplosion = Image->LoadImage(Renderer, "invaders/explosion.bmp");
		GameInvaders->spritesheetAsteroid = Image->LoadImage(Renderer, "invaders/asteroid-01.bmp");
		GameInvaders->spritesheetEnemy1 = Image->LoadImage(Renderer, "invaders/enemy1.bmp"); //bottom
		GameInvaders->spritesheetEnemy2 = Image->LoadImage(Renderer, "invaders/enemy2.bmp"); //middle
		GameInvaders->spritesheetEnemy3 = Image->LoadImage(Renderer, "invaders/enemy3.bmp"); //top
		GameInvaders->spritesheetPlayer = Image->LoadImage(Renderer, "invaders/player.bmp"); //top
	}
	GameInvaders->backgroundtz = Image->ImageSize(GameInvaders->background);
}

void CGameInvaders_UnloadGraphics(CGameInvaders* GameInvaders)
{
	Image->UnLoadImage(GameInvaders->spritesheetBullet);
	Image->UnLoadImage(GameInvaders->spritesheetExplosion);
	Image->UnLoadImage(GameInvaders->spritesheetAsteroid);
	Image->UnLoadImage(GameInvaders->spritesheetEnemy1);
	Image->UnLoadImage(GameInvaders->spritesheetEnemy2);
	Image->UnLoadImage(GameInvaders->spritesheetEnemy3);
	Image->UnLoadImage(GameInvaders->spritesheetPlayer);
	Image->UnLoadImage(GameInvaders->background);
}

//Update ----------------------------------------------------------------------------------------------------------------

void CGameInvaders_UpdateObjects(CGameInvaders* GameInvaders, bool IsGameState)
{
	GameInvaders->updateinvaders(GameInvaders);
	if (IsGameState)
	{
		GameInvaders->updateenemybullet(GameInvaders);
		GameInvaders->updateplayer(GameInvaders);
		GameInvaders->updatebullet(GameInvaders);
		GameInvaders->updateasteroids(GameInvaders);
	}
}

void CGameInvaders_UpdateLogic(CGameInvaders* GameInvaders)
{
	GameInvaders->GameBase->UpdateLogic(GameInvaders->GameBase);
	GameInvaders->UpdateObjects(GameInvaders,SubGameState == SGGame);
	if(SubGameState == SGGame)
		Sprites->UpdateSprites(Renderer);
}

bool CGameInvaders_DrawObjects(CGameInvaders* GameInvaders)
{
	//need todo this here so last frame is also drawn
	GameInvaders->checkexplosions(GameInvaders);
	//call drawsprites in base class
	return true;
}

void CGameInvaders_Draw(CGameInvaders* GameInvaders)
{
	GameInvaders->DrawBackground(GameInvaders);
	if (GameInvaders->DrawObjects(GameInvaders))
		Sprites->DrawSprites(Renderer);
	GameInvaders->GameBase->DrawScoreBar(GameInvaders->GameBase);
	GameInvaders->GameBase->DrawSubStateText(GameInvaders->GameBase);
}