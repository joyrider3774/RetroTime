#include <SDL.h>
#include <math.h>
#include <stdbool.h>
#include "CGameInvaders.h"
#include "../CGame.h"
#include "../Common.h"
#include "../Vec2F.h"



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

	GameInvaders->enemyvel.x = CGameInvaders_enemyspeed;
	GameInvaders->enemyvel.y = 0;
	GameInvaders->enemyscale.x = 1.5f*xscale;
	GameInvaders->enemyscale.y = 1.5f*yscale;

	GameInvaders->deaths = 0;
	GameInvaders->pattern = 0;
	GameInvaders->backgroundfade = 0.0f;
	GameInvaders->backgroundfadeinc = 0.0f;
	

	Initialize_CSpriteObject(&GameInvaders->player);
	Initialize_CSpriteObject(&GameInvaders->bullet);

	for (int i = 0; i < CGameInvaders_maxenemies; i++)
	{
		Initialize_CSpriteObject(&GameInvaders->enemies[i]);
		initialize_CTweenInfo(&GameInvaders->tweens[i][0]);
		initialize_CTweenInfo(&GameInvaders->tweens[i][1]);
	}

	for (int i = 0; i < CGameInvaders_maxexplosion; i++)
		Initialize_CSpriteObject(&GameInvaders->explosions[i]);

	for (int i = 0; i < CGameInvaders_maxasteroids; i++)
		Initialize_CSpriteObject(&GameInvaders->asteroids[i]);

	for (int i = 0; i < CGameInvaders_maxenemybullets; i++)
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
	for(int i = 0; i < CGameInvaders_maxexplosion; i++)
	{
		if (GameInvaders->explosions[i].alive)
		{
			if (CSprites_GetSpriteAnimFrame(GameInvaders->explosions[i].spr) == CSprites_GetSpriteAnimFrameCount(GameInvaders->explosions[i].spr) -1)
			{
				GameInvaders->destroyexploison(GameInvaders,i);
			}
		}
	}
}

void CGameInvaders_destroyallexplosion(CGameInvaders* GameInvaders)
{
	for (int i = 0; i < CGameInvaders_maxexplosion; i++)
		GameInvaders->destroyexploison(GameInvaders,i);
}

void CGameInvaders_destroyexploison(CGameInvaders* GameInvaders, int index)
{
	if (GameInvaders->explosions[index].alive)
	{
		CSprites_RemoveSprite(GameInvaders->explosions[index].spr);
		GameInvaders->explosions[index].alive = false;
	}
}

void CGameInvaders_createexplosion(CGameInvaders* GameInvaders, Vec2F pos)
{
	for(int i = 0; i < CGameInvaders_maxexplosion;i++)
	{
		if (!GameInvaders->explosions[i].alive)
		{
			GameInvaders->explosions[i].spr = CSprites_CreateSprite();
			CSprites_SetSpriteImageTiles(Renderer,GameInvaders->explosions[i].spr, &GameInvaders->spritesheetExplosion, 7, 1);
			CSprites_SetSpriteScale(Renderer,GameInvaders->explosions[i].spr, GameInvaders->enemyscale);
			CSprites_SetSpriteDepth(GameInvaders->explosions[i].spr, 1);
			CSprites_SetSpriteAnimation(GameInvaders->explosions[i].spr,2, 6, 15);
			CSprites_SetSpriteLocation(GameInvaders->explosions[i].spr, pos);
			GameInvaders->explosions[i].alive = true;
			CAudio_PlaySound(GameInvaders->SfxPlayerDeath, 0);
			break;
		}
	}
}

//enemy bullets ----------------------------------------------------------------------------------------------------------------

void CGameInvaders_destroyallenemybullet(CGameInvaders* GameInvaders)
{
	for(int i = 0; i < CGameInvaders_maxenemybullets; i++)
		GameInvaders->destroyenemybullet(GameInvaders,i);
}

void CGameInvaders_destroyenemybullet(CGameInvaders* GameInvaders, int index)
{
	if (GameInvaders->enemybullets[index].alive)
	{
		CSprites_RemoveSprite(GameInvaders->enemybullets[index].spr);
		GameInvaders->enemybullets[index].alive = false;
	}
}

void CGameInvaders_createnemybullet(CGameInvaders* GameInvaders, Vec2F pos)
{
	for(int i = 0; i < CGameInvaders_maxenemybullets; i++)
	{
		if (!GameInvaders->enemybullets[i].alive)
		{
			GameInvaders->enemybullets[i].spr = CSprites_CreateSprite();
			CSprites_SetSpriteImageTiles(Renderer,GameInvaders->enemybullets[i].spr, &GameInvaders->spritesheetBullet, 2, 1);
			CSprites_SetSpriteColour(GameInvaders->enemybullets[i].spr, 0.75,0.65,0.65,1);
			CSprites_SetSpriteAnimation(GameInvaders->enemybullets[i].spr, 1, 1, 0);
			CSprites_SetSpriteScale(Renderer,GameInvaders->enemybullets[i].spr, GameInvaders->enemyscale);
			CSprites_SetSpriteDepth(GameInvaders->enemybullets[i].spr, -1);
			GameInvaders->enemybullets[i].tz = CSprites_TileSize(GameInvaders->enemybullets[i].spr);
			GameInvaders->enemybullets[i].tz.x = GameInvaders->enemybullets[i].tz.x * GameInvaders->enemyscale.x;
			GameInvaders->enemybullets[i].tz.y = GameInvaders->enemybullets[i].tz.y * GameInvaders->enemyscale.y;
			GameInvaders->enemybullets[i].pos = pos;
			GameInvaders->enemybullets[i].vel.x = 0;
			GameInvaders->enemybullets[i].vel.y = CGameInvaders_bulletspeed;
			GameInvaders->enemybullets[i].alive = true;
			CAudio_PlaySound(GameInvaders->SfxEnemyShoot, 0);
			break;
		}
	}
}

void CGameInvaders_updateenemybullet(CGameInvaders* GameInvaders)
{
	if (!GameInvaders->tweenactive(GameInvaders,CGameInvaders_tweenenemypositions))
	{
		if (rand() % (CGameInvaders_enemybulletrandomizer) == 0)
		{
			int col = rand() % (CGameInvaders_enemycols);
			int y = 0;
			bool fired = false;
			while (!fired)
			{
				y = CGameInvaders_enemyrows -1;
				while (y >= 0)
				{
					if (GameInvaders->enemies[col + y * CGameInvaders_enemycols].alive)
					{
						GameInvaders->createnemybullet(GameInvaders,GameInvaders->enemies[col + y * CGameInvaders_enemycols].pos);
						fired = true;
						break;
					}
					y -= 1;
				}
				col += 1;
				if (col >= CGameInvaders_enemycols)
					col = 0;
			}
		}
	}

	for(int i = 0; i < CGameInvaders_maxenemybullets; i++)
	{
		if (GameInvaders->enemybullets[i].alive)
		{
			GameInvaders->enemybullets[i].pos.x += GameInvaders->enemybullets[i].vel.x;
			GameInvaders->enemybullets[i].pos.y += GameInvaders->enemybullets[i].vel.y;
			CSprites_SetSpriteLocation(GameInvaders->enemybullets[i].spr, GameInvaders->enemybullets[i].pos);
			if (GameInvaders->enemybullets[i].pos.y >= GameInvaders->GameBase->screenbottom)
				GameInvaders->destroyenemybullet(GameInvaders,i);

			if (GameInvaders->enemybullets[i].alive && GameInvaders->player.alive)
			{
				if (CSprites_DetectSpriteCollision(GameInvaders->enemybullets[i].spr, GameInvaders->player.spr))
				{
					GameInvaders->player.alive = false;
					GameInvaders->player.freeze = 30;
					GameInvaders->createexplosion(GameInvaders,GameInvaders->player.pos);
					GameInvaders->destroyenemybullet(GameInvaders,i);
					CGame_AddToScore(-150);
					GameInvaders->deaths += 1;
					GameInvaders->GameBase->HealthPoints -= 1;
					CAudio_PlaySound(GameInvaders->SfxDie, 0);
				}
			}


			for (int j = 0; j < CGameInvaders_maxasteroids; j++)
			{
				if (GameInvaders->enemybullets[i].alive && GameInvaders->asteroids[j].alive)
				{
					if (CSprites_DetectSpriteCollision(GameInvaders->enemybullets[i].spr, GameInvaders->asteroids[j].spr))
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
	for(int i = 0; i < CGameInvaders_maxasteroids; i++)
		GameInvaders->destroyasteroid(GameInvaders,i);
}

void CGameInvaders_destroyasteroid(CGameInvaders* GameInvaders, int index)
{
	if (GameInvaders->asteroids[index].alive)
	{
		CSprites_RemoveSprite(GameInvaders->asteroids[index].spr);
		GameInvaders->asteroids[index].alive = false;
	}
}

void CGameInvaders_createasteroids(CGameInvaders* GameInvaders)
{
	for(int i = 0; i < CGameInvaders_maxasteroids; i++)
	{
		GameInvaders->asteroids[i].spr = CSprites_CreateSprite();
		GameInvaders->asteroids[i].healthpoints = CGameInvaders_asteroidmaxhealthpoints;
		GameInvaders->asteroids[i].alive = true;
		CSprites_SetSpriteImage(Renderer,GameInvaders->asteroids[i].spr, &GameInvaders->spritesheetAsteroid);
		CSprites_SetSpriteAnimation(GameInvaders->asteroids[i].spr, 0, 0, 0);
		Vec2F pos = {(float)i * (GameInvaders->GameBase->screenright - GameInvaders->GameBase->screenleft) / (CGameInvaders_maxasteroids-1), (float)GameInvaders->GameBase->screenbottom - CGameInvaders_asteroidsoffset};
		CSprites_SetSpriteLocation(GameInvaders->asteroids[i].spr, pos);
		Vec2F scale = {CGameInvaders_asteroidscale, CGameInvaders_asteroidscale};
		CSprites_SetSpriteScale(Renderer,GameInvaders->asteroids[i].spr, scale);
		CSprites_SetSpriteCollisionShape(GameInvaders->asteroids[i].spr, SHAPE_BOX, 45,45,0, 0 , 2);
		CSprites_SetSpriteRotation(GameInvaders->asteroids[i].spr, rand() % (360));
	}
}

void CGameInvaders_updateasteroids(CGameInvaders* GameInvaders)
{
	for(int i = 0; i < CGameInvaders_maxasteroids;i++)
	{
		if (GameInvaders->asteroids[i].alive)
		{
			Vec2F scale = {CGameInvaders_asteroidscale - ((CGameInvaders_asteroidscale / CGameInvaders_asteroidmaxhealthpoints) * (CGameInvaders_asteroidmaxhealthpoints - GameInvaders->asteroids[i].healthpoints)),
				CGameInvaders_asteroidscale - ((CGameInvaders_asteroidscale / CGameInvaders_asteroidmaxhealthpoints) * (CGameInvaders_asteroidmaxhealthpoints - GameInvaders->asteroids[i].healthpoints))};
			CSprites_SetSpriteScale(Renderer,GameInvaders->asteroids[i].spr,scale);
			CSprites_SetSpriteRotation(GameInvaders->asteroids[i].spr, GameInvaders->asteroids[i].spr->rotation + 0.5f);
		}
	}
}

//player bullet ----------------------------------------------------------------------------------------------------------------

void CGameInvaders_createbullet(CGameInvaders* GameInvaders)
{
	if (!GameInvaders->bullet.alive)
	{
		GameInvaders->bullet.spr = CSprites_CreateSprite();
		CSprites_SetSpriteImageTiles(Renderer,GameInvaders->bullet.spr, &GameInvaders->spritesheetBullet, 2, 1);
		CSprites_SetSpriteAnimation(GameInvaders->bullet.spr, 1, 1, 0);
		CSprites_SetSpriteScale(Renderer,GameInvaders->bullet.spr, GameInvaders->enemyscale);
		CSprites_SetSpriteDepth(GameInvaders->bullet.spr, -1);
		GameInvaders->bullet.tz = CSprites_TileSize(GameInvaders->bullet.spr);
		GameInvaders->bullet.tz.x = GameInvaders->bullet.tz.x * GameInvaders->enemyscale.x;
		GameInvaders->bullet.tz.y = GameInvaders->bullet.tz.y * GameInvaders->enemyscale.y;
		GameInvaders->bullet.pos = GameInvaders->player.pos ;
		GameInvaders->bullet.vel.x = 0;
		GameInvaders->bullet.vel.y = -CGameInvaders_bulletspeed;
		GameInvaders->bullet.alive = true;
		CAudio_PlaySound(GameInvaders->SfxPlayerShoot, 0);
	}
}

void CGameInvaders_destroybullet(CGameInvaders* GameInvaders)
{
	if (GameInvaders->bullet.alive)
	{
		CSprites_RemoveSprite(GameInvaders->bullet.spr);
		GameInvaders->bullet.alive = false;
	}
}

void CGameInvaders_updatebullet(CGameInvaders* GameInvaders)
{
	if (GameInvaders->bullet.alive)
	{
		GameInvaders->bullet.pos.x += GameInvaders->bullet.vel.x;
		GameInvaders->bullet.pos.y += GameInvaders->bullet.vel.y;
		CSprites_SetSpriteLocation(GameInvaders->bullet.spr, GameInvaders->bullet.pos);

		for (int x = 0; x < CGameInvaders_enemycols; x++)
		{
			for(int y = 0; y < CGameInvaders_enemyrows; y++)
			{
				if (GameInvaders->bullet.alive && GameInvaders->enemies[y * CGameInvaders_enemycols + x].alive)
				{
					if (CSprites_DetectSpriteCollision(GameInvaders->bullet.spr, GameInvaders->enemies[y * CGameInvaders_enemycols + x].spr))
					{
						GameInvaders->createexplosion(GameInvaders,GameInvaders->enemies[y * CGameInvaders_enemycols + x].pos);
						GameInvaders->destroybullet(GameInvaders);
						GameInvaders->destroyinvader(GameInvaders,y * CGameInvaders_enemycols + x);
						if (y < 1)
							CGame_AddToScore(25);
						else
						{
							if (y < 3)
								CGame_AddToScore(20);
							else
								CGame_AddToScore(10);
						}
						GameInvaders->enemyvel.x += GameInvaders->enemyvel.x / (float)fabs(GameInvaders->enemyvel.x) * CGameInvaders_enemyspeedinc;
					}
				}
			}
		}

		for(int i = 0; i < CGameInvaders_maxasteroids; i++)
		{
			if (GameInvaders->bullet.alive && GameInvaders->asteroids[i].alive)
			{
				if (CSprites_DetectSpriteCollision(GameInvaders->bullet.spr, GameInvaders->asteroids[i].spr))
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
	for(int i = 0; i < CGameInvaders_maxenemies; i++)
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
	for(int i = 0; i < CGameInvaders_maxenemies; i++)
	{
		if (GameInvaders->enemies[i].alive)
		{
			GameInvaders->tweens[i][CGameInvaders_tweenenemypositions].active = false;
			CSprites_RemoveSprite(GameInvaders->enemies[i].spr);
		}
	}
}

void CGameInvaders_destroyinvader(CGameInvaders* GameInvaders, int index)
{
	GameInvaders->enemies[index].alive = false;
	CSprites_RemoveSprite(GameInvaders->enemies[index].spr);
}

bool CGameInvaders_tweenactive(CGameInvaders* GameInvaders, int id)
{
	bool result = false;
	for(int i = 0; i < CGameInvaders_maxenemies; i++)
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
	for (int x = 0; x < CGameInvaders_enemycols; x++)
	{
		for (int y = 0; y < CGameInvaders_enemyrows; y++)
		{
			GameInvaders->tweens[x + y * CGameInvaders_enemycols][CGameInvaders_tweenenemypositions] = createtween(CGameInvaders_tweenenemypositions, 1+ (rand() % ((int)DesiredFps))/100, funcsmoothstop, 1, true, DesiredFps);
			GameInvaders->enemies[x + y * CGameInvaders_enemycols].alive = true;
			GameInvaders->enemies[x + y * CGameInvaders_enemycols].spr = CSprites_CreateSprite();
			GameInvaders->enemies[x + y * CGameInvaders_enemycols].pos.x = (float) x * (CGameInvaders_enemyspacing + CGameInvaders_enemyhorzspacing) + CGameInvaders_enemystartxoffset;
			GameInvaders->enemies[x + y * CGameInvaders_enemycols].pos.y = (float) y * CGameInvaders_enemyspacing + CGameInvaders_enemystartyoffset;
			if (setposition)
				CSprites_SetSpriteLocation(GameInvaders->enemies[x + y * CGameInvaders_enemycols].spr, GameInvaders->enemies[x + y * CGameInvaders_enemycols].pos);
			SDL_Point tz = {1,1};
			if (y < 1)
			{
				CSprites_SetSpriteImageTiles(Renderer,GameInvaders->enemies[x + y * CGameInvaders_enemycols].spr, &GameInvaders->spritesheetEnemy3, 4, 1);
				CSprites_SetSpriteScale(Renderer,GameInvaders->enemies[x + y * CGameInvaders_enemycols].spr, GameInvaders->enemyscale);
				CSprites_SetSpriteAnimation(GameInvaders->enemies[x + y * CGameInvaders_enemycols].spr, 0, 3, 5);
				tz = CSprites_TileSize(GameInvaders->enemies[x + y * CGameInvaders_enemycols].spr);
				CSprites_SetSpriteCollisionShape(GameInvaders->enemies[x + y * CGameInvaders_enemycols].spr, SHAPE_BOX, tz.x-8, tz.y - 26,0, 0 , 6);
			}
			else
			{
				if (y < 3 )
				{
					CSprites_SetSpriteImageTiles(Renderer,GameInvaders->enemies[x + y * CGameInvaders_enemycols].spr, &GameInvaders->spritesheetEnemy2, 4, 1);
					CSprites_SetSpriteScale(Renderer,GameInvaders->enemies[x + y * CGameInvaders_enemycols].spr, GameInvaders->enemyscale);

					CSprites_SetSpriteAnimation(GameInvaders->enemies[x + y * CGameInvaders_enemycols].spr, 0, 3, 5);
					tz = CSprites_TileSize(GameInvaders->enemies[x + y * CGameInvaders_enemycols].spr);
					CSprites_SetSpriteCollisionShape(GameInvaders->enemies[x + y * CGameInvaders_enemycols].spr, SHAPE_BOX, tz.x-15, tz.y - 18,0, 0 , -2);
				}
				else
				{
					CSprites_SetSpriteImageTiles(Renderer,GameInvaders->enemies[x + y * CGameInvaders_enemycols].spr, &GameInvaders->spritesheetEnemy1, 5, 1);
					CSprites_SetSpriteScale(Renderer,GameInvaders->enemies[x + y * CGameInvaders_enemycols].spr, GameInvaders->enemyscale);

					CSprites_SetSpriteAnimation(GameInvaders->enemies[x + y * CGameInvaders_enemycols].spr,0, 4, 5);
					tz = CSprites_TileSize(GameInvaders->enemies[x + y * CGameInvaders_enemycols].spr);
					CSprites_SetSpriteCollisionShape(GameInvaders->enemies[x + y * CGameInvaders_enemycols].spr, SHAPE_BOX, tz.x-17, tz.y - 12,0, 0 , -2);
				}
			}
			GameInvaders->enemies[x + y * CGameInvaders_enemycols].tz.x = tz.x * GameInvaders->enemyscale.x;
			GameInvaders->enemies[x + y * CGameInvaders_enemycols].tz.y = tz.y * GameInvaders->enemyscale.y;
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
		GameInvaders->enemyvel.x = CGameInvaders_enemyspeed;
		GameInvaders->enemyvel.y = 0;
		GameInvaders->createinvaders(GameInvaders,false);
		GameInvaders->destroybullet(GameInvaders);
		GameInvaders->destroyallenemybullet(GameInvaders);
		CAudio_PlaySound(GameInvaders->SfxSucces, 0);
	}

	bool btweenactive = GameInvaders->tweenactive(GameInvaders,CGameInvaders_tweenenemypositions);

	//enemies reached horizontal screen boundaries
	if (((GameInvaders->enemies[GameInvaders->enemyinfo.mostleft].pos.x - GameInvaders->enemies[GameInvaders->enemyinfo.mostleft].tz.x / 2 + GameInvaders->enemyvel.x < GameInvaders->GameBase->screenleft) ||
		(GameInvaders->enemies[GameInvaders->enemyinfo.mostright].pos.x + GameInvaders->enemies[GameInvaders->enemyinfo.mostright].tz.x / 2 + GameInvaders->enemyvel.x > GameInvaders->GameBase->screenright)) && !btweenactive)
	{
		for(int i = 0; i < CGameInvaders_maxenemies; i++)
		{
			if (GameInvaders->enemies[i].alive)
				GameInvaders->enemies[i].pos.y += CGameInvaders_enemyspacing / 4;
		}
		GameInvaders->enemyvel.x *= -1;
		GameInvaders->enemyvel.y *= -1;
	}

	//update enemy positions
	for(int i = 0; i < CGameInvaders_maxenemies; i++)
	{
		if (GameInvaders->enemies[i].alive)
		{
			if (btweenactive)
			{
				GameInvaders->tweens[i][CGameInvaders_tweenenemypositions] = updatetween( GameInvaders->tweens[i][CGameInvaders_tweenenemypositions]);
				Vec2F pos = GameInvaders->enemies[i].pos;

				if (GameInvaders->pattern == 0)
				{
					if ((i % CGameInvaders_enemycols) < CGameInvaders_enemycols / 3)
						pos.x = pos.x * GameInvaders->tweens[i][CGameInvaders_tweenenemypositions].funcval;
					else
					{
						if ((i % CGameInvaders_enemycols) > CGameInvaders_enemycols * 2 / 3)
							pos.x = GameInvaders->GameBase->screenright - (GameInvaders->GameBase->screenright - pos.x) * GameInvaders->tweens[i][CGameInvaders_tweenenemypositions].funcval;
					}
					pos.y = pos.y * GameInvaders->tweens[i][CGameInvaders_tweenenemypositions].funcval;
				}
				else
				{
					if(GameInvaders->pattern == 1)
						pos.y = pos.y * GameInvaders->tweens[i][CGameInvaders_tweenenemypositions].funcval;
					else
					{
						if (GameInvaders->pattern == 2)
						{
							if (i % CGameInvaders_enemycols < CGameInvaders_enemycols / 2)
								pos.x = pos.x * GameInvaders->tweens[i][CGameInvaders_tweenenemypositions].funcval;
							else
								pos.x = GameInvaders->GameBase->screenright - (GameInvaders->GameBase->screenright - pos.x) * GameInvaders->tweens[i][CGameInvaders_tweenenemypositions].funcval;
						}
						else
						{
							if( GameInvaders->pattern == 3)
								pos.x = pos.x * GameInvaders->tweens[i][CGameInvaders_tweenenemypositions].funcval;
							else
							{
								if (GameInvaders->pattern == 4)
									pos.x = GameInvaders->GameBase->screenright - (GameInvaders->GameBase->screenright - pos.x) * GameInvaders->tweens[i][CGameInvaders_tweenenemypositions].funcval;
							}
						}
					}
				}
				CSprites_SetSpriteLocation(GameInvaders->enemies[i].spr, pos);
			}
			else
			{
				if (SubGameState == SGGame)
				{
					GameInvaders->enemies[i].pos.x += GameInvaders->enemyvel.x;
					GameInvaders->enemies[i].pos.y += GameInvaders->enemyvel.y;
					CSprites_SetSpriteLocation(GameInvaders->enemies[i].spr, GameInvaders->enemies[i].pos);
				}
			}
		}
	}

	//enemies reached bottom
	if ((GameInvaders->player.pos.y - GameInvaders->enemies[GameInvaders->enemyinfo.mostbottom].pos.y) < CGameInvaders_endscreenconstant)
	{
		CGame_AddToScore(-250);
		GameInvaders->enemyvel.x = CGameInvaders_enemyspeed;
		GameInvaders->enemyvel.y = 0;
		GameInvaders->destroyallinvaders(GameInvaders);
		GameInvaders->createinvaders(GameInvaders,false);
		GameInvaders->destroybullet(GameInvaders);
		GameInvaders->destroyallenemybullet(GameInvaders);
		GameInvaders->GameBase->HealthPoints -= 1;
		CAudio_PlaySound(GameInvaders->SfxDie, 0);
	}
}

//player ----------------------------------------------------------------------------------------------------------------

void CGameInvaders_destroyplayer(CGameInvaders* GameInvaders)
{
	CSprites_RemoveSprite(GameInvaders->player.spr);
	GameInvaders->player.alive = false;
}

void CGameInvaders_createplayer(CGameInvaders* GameInvaders)
{
	GameInvaders->player.spr = CSprites_CreateSprite();
	CSprites_SetSpriteImageTiles(Renderer,GameInvaders->player.spr, &GameInvaders->spritesheetPlayer, 5,1);
	CSprites_SetSpriteAnimation(GameInvaders->player.spr, 0, 0, 0);
	CSprites_SetSpriteScale(Renderer,GameInvaders->player.spr, GameInvaders->enemyscale);
	GameInvaders->player.tz = CSprites_TileSize(GameInvaders->player.spr);
	CSprites_SetSpriteCollisionShape(GameInvaders->player.spr, SHAPE_BOX, GameInvaders->player.tz.x-10, GameInvaders->player.tz.y - 6,0, 0, 12);
	GameInvaders->player.tz.x = GameInvaders->player.tz.x * GameInvaders->enemyscale.x;
	GameInvaders->player.tz.y = GameInvaders->player.tz.y * GameInvaders->enemyscale.y;
	GameInvaders->player.pos.x = (float)(GameInvaders->GameBase->screenright - GameInvaders->GameBase->screenleft) / 2;
	GameInvaders->player.pos.y = (float)GameInvaders->GameBase->screenbottom - GameInvaders->player.tz.y / 2;
	GameInvaders->GameBase->HealthPoints = 3;
	GameInvaders->player.freeze = 0;
	CSprites_SetSpriteLocation(GameInvaders->player.spr, GameInvaders->player.pos);
}

void CGameInvaders_updateplayer(CGameInvaders* GameInvaders)
{
	CSprites_SetSpriteVisibility(GameInvaders->player.spr, GameInvaders->player.alive);
	if (GameInvaders->player.alive)
	{
		CSprites_SetSpriteAnimation(GameInvaders->player.spr, 0, 0, 0);

		if ((CInput_Buttons.ButLeft) ||
			(CInput_Buttons.ButLeft2) ||
			(CInput_Buttons.ButDpadLeft))
		{
			if (GameInvaders->player.pos.x - GameInvaders->player.tz.x / 2 - CGameInvaders_playerspeed > GameInvaders->GameBase->screenleft)
				GameInvaders->player.pos.x -= CGameInvaders_playerspeed;
			else
				GameInvaders->player.pos.x = GameInvaders->GameBase->screenleft + GameInvaders->player.tz.x / 2;
			CSprites_SetSpriteAnimation(GameInvaders->player.spr, 1, 1, 0);
		}

		if ((CInput_Buttons.ButRight) ||
			(CInput_Buttons.ButRight2) ||
			(CInput_Buttons.ButDpadRight))
		{
			if ( GameInvaders->player.pos.x + GameInvaders->player.tz.x / 2 + CGameInvaders_playerspeed < GameInvaders->GameBase->screenright)
				GameInvaders->player.pos.x += CGameInvaders_playerspeed;
			else
				GameInvaders->player.pos.x = GameInvaders->GameBase->screenright - GameInvaders->player.tz.x / 2;
			CSprites_SetSpriteAnimation(GameInvaders->player.spr, 4, 4, 0);
		}
		CSprites_SetSpriteLocation(GameInvaders->player.spr, GameInvaders->player.pos);

		if (!CInput_PrevButtons.ButA && CInput_Buttons.ButA)
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
	CImage_DrawImage(Renderer, GameInvaders->background, NULL, NULL);
}

//init - deinit ----------------------------------------------------------------------------------------------------------------

void CGameInvaders_init(CGameInvaders* GameInvaders)
{
	GameInvaders->LoadGraphics(GameInvaders);

	GameInvaders->deaths = 0;
	GameInvaders->backgroundfade = 0;
	GameInvaders->backgroundfadeinc = 1;
	GameInvaders->enemyvel.x = CGameInvaders_enemyspeed;
	GameInvaders->enemyvel.y = 0;

	GameInvaders->createinvaders(GameInvaders,false);
	GameInvaders->createplayer(GameInvaders);
	GameInvaders->createasteroids(GameInvaders);

	GameInvaders->LoadSound(GameInvaders);
	CurrentGameMusicID = GameInvaders->MusMusic;
	CAudio_PlayMusic(GameInvaders->MusMusic, -1);
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
	GameInvaders->SfxDie = CAudio_LoadSound("common/die.wav");
	GameInvaders->SfxPlayerShoot = CAudio_LoadSound("invaders/playershoot.wav");
	GameInvaders->SfxPlayerDeath = CAudio_LoadSound("invaders/playerdeath.wav");
	GameInvaders->SfxEnemyShoot = CAudio_LoadSound("invaders/enemyshoot.wav");
	GameInvaders->SfxEnemyDeath = CAudio_LoadSound("invaders/enemydeath.wav");
	GameInvaders->SfxSucces = CAudio_LoadSound("common/succes.wav");
	GameInvaders->MusMusic = CAudio_LoadMusic("invaders/music.ogg");
}

void CGameInvaders_UnLoadSound(CGameInvaders* GameInvaders)
{
	CAudio_StopMusic();
	CAudio_StopSound();
	CAudio_UnLoadMusic(GameInvaders->MusMusic);
	CAudio_UnLoadSound(GameInvaders->SfxDie);
	CAudio_UnLoadSound(GameInvaders->SfxPlayerShoot);
	CAudio_UnLoadSound(GameInvaders->SfxPlayerDeath);
	CAudio_UnLoadSound(GameInvaders->SfxEnemyShoot);
	CAudio_UnLoadSound(GameInvaders->SfxEnemyDeath);
	CAudio_UnLoadSound(GameInvaders->SfxSucces);
}

void CGameInvaders_LoadGraphics(CGameInvaders* GameInvaders)
{
	GameInvaders->background = CImage_LoadImage(Renderer, "invaders/background.png");

	GameInvaders->spritesheetBullet = CImage_LoadImageEx(Renderer, "invaders/bullet.png", 0, 100, dumpScaledBitmaps);
	GameInvaders->spritesheetExplosion = CImage_LoadImageEx(Renderer, "invaders/explosion.png", 0, 100, dumpScaledBitmaps);
	GameInvaders->spritesheetAsteroid = CImage_LoadImageEx(Renderer, "invaders/asteroid-01.png", 0, 75, dumpScaledBitmaps);
	GameInvaders->spritesheetEnemy1 = CImage_LoadImageEx(Renderer, "invaders/enemy1.png",0, 80, dumpScaledBitmaps); //bottom
	GameInvaders->spritesheetEnemy2 = CImage_LoadImageEx(Renderer, "invaders/enemy2.png",0, 80, dumpScaledBitmaps); //middle
	GameInvaders->spritesheetEnemy3 = CImage_LoadImageEx(Renderer, "invaders/enemy3.png",0, 80, dumpScaledBitmaps); //top
	GameInvaders->spritesheetPlayer = CImage_LoadImageEx(Renderer, "invaders/player.png",0, 80, dumpScaledBitmaps); //top
	
	// SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/invaders/bullet.bmp", CImage_GetImage(GameInvaders->spritesheetBullet), 1,1, true, 0, 100);
	// SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/invaders/explosion.bmp", CImage_GetImage(GameInvaders->spritesheetExplosion), 1,1,true, 0, 100);
	// SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/invaders/asteroid-01.bmp", CImage_GetImage(GameInvaders->spritesheetAsteroid), 1,1,true, 0, 75);
	// SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/invaders/enemy1.bmp", CImage_GetImage(GameInvaders->spritesheetEnemy1), 1,1,true, 0, 80);
	// SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/invaders/enemy2.bmp", CImage_GetImage(GameInvaders->spritesheetEnemy2), 1,1,true, 0, 80);
	// SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/invaders/enemy3.bmp", CImage_GetImage(GameInvaders->spritesheetEnemy3), 1,1,true, 0, 80);
	// SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/invaders/player.bmp", CImage_GetImage(GameInvaders->spritesheetPlayer), 1,1,true, 0, 80);
	
	if(!useDefaultColorAssets)
	{
    	GameInvaders->UnloadGraphics(GameInvaders);
		GameInvaders->background = CImage_LoadImage(Renderer, "invaders/background.png");
		GameInvaders->spritesheetBullet = CImage_LoadImage(Renderer, "invaders/bullet.bmp");
		GameInvaders->spritesheetExplosion = CImage_LoadImage(Renderer, "invaders/explosion.bmp");
		GameInvaders->spritesheetAsteroid = CImage_LoadImage(Renderer, "invaders/asteroid-01.bmp");
		GameInvaders->spritesheetEnemy1 = CImage_LoadImage(Renderer, "invaders/enemy1.bmp"); //bottom
		GameInvaders->spritesheetEnemy2 = CImage_LoadImage(Renderer, "invaders/enemy2.bmp"); //middle
		GameInvaders->spritesheetEnemy3 = CImage_LoadImage(Renderer, "invaders/enemy3.bmp"); //top
		GameInvaders->spritesheetPlayer = CImage_LoadImage(Renderer, "invaders/player.bmp"); //top
	}
	GameInvaders->backgroundtz = CImage_ImageSize(GameInvaders->background);
}

void CGameInvaders_UnloadGraphics(CGameInvaders* GameInvaders)
{
	CImage_UnLoadImage(GameInvaders->spritesheetBullet);
	CImage_UnLoadImage(GameInvaders->spritesheetExplosion);
	CImage_UnLoadImage(GameInvaders->spritesheetAsteroid);
	CImage_UnLoadImage(GameInvaders->spritesheetEnemy1);
	CImage_UnLoadImage(GameInvaders->spritesheetEnemy2);
	CImage_UnLoadImage(GameInvaders->spritesheetEnemy3);
	CImage_UnLoadImage(GameInvaders->spritesheetPlayer);
	CImage_UnLoadImage(GameInvaders->background);
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
		CSprites_UpdateSprites(Renderer);
}

bool CGameInvaders_DrawObjects(CGameInvaders* GameInvaders)
{
	//need todo this here so last frame is also drawn
	GameInvaders->checkexplosions(GameInvaders);
	//call drawsprites
	return true;
}

void CGameInvaders_Draw(CGameInvaders* GameInvaders)
{
	GameInvaders->DrawBackground(GameInvaders);
	if (GameInvaders->DrawObjects(GameInvaders))
		CSprites_DrawSprites(Renderer);
	GameInvaders->GameBase->DrawScoreBar(GameInvaders->GameBase);
	GameInvaders->GameBase->DrawSubStateText(GameInvaders->GameBase);
}