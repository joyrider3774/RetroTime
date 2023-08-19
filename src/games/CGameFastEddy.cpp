#include <SDL.h>
#include <string>
#include <iostream>
#include <cmath>
#include "CGameFastEddy.h"
#include "../CGame.h"
#include "../Common.h"
#include "../Vec2F.h"

using namespace std;

CGameFastEddy* Create_CGameFastEddy(CGame* aGame)
{
	CGameFastEddy* GameFastEddy = (CGameFastEddy*) malloc(sizeof(CGameFastEddy));
	GameFastEddy->GameBase = Create_CGameBase(aGame, GSEddy, true);

	GameFastEddy->MusMusic = -1;
	GameFastEddy->SfxSucces = -1;
	GameFastEddy->SfxDie = -1;

	GameFastEddy->GameBase->playfieldwidth = ScreenWidth;
	GameFastEddy->GameBase->playfieldheight = ScreenHeight;
	GameFastEddy->GameBase->screenleft = (ScreenWidth - GameFastEddy->GameBase->playfieldwidth) / 2;
	GameFastEddy->GameBase->screenright = GameFastEddy->GameBase->screenleft + GameFastEddy->GameBase->playfieldwidth;
	GameFastEddy->GameBase->screentop = (ScreenHeight - GameFastEddy->GameBase->playfieldheight) / 2;
	GameFastEddy->GameBase->screenbottom = GameFastEddy->GameBase->screentop + GameFastEddy->GameBase->playfieldheight;
	GameFastEddy->numfloortilesperrow = GameFastEddy->GameBase->playfieldwidth / GameFastEddy->rowfloorsizex;
	GameFastEddy->maxfloortiles = GameFastEddy->numfloortilesperrow * GameFastEddy->rows;
	GameFastEddy->rowspacingsize = GameFastEddy->GameBase->playfieldheight / GameFastEddy->rows;
	GameFastEddy->ladderwidth = GameFastEddy->GameBase->playfieldwidth / GameFastEddy->laddersfitrows;
	GameFastEddy->rowsize = GameFastEddy->rowspacingsize - GameFastEddy->rowfloorsizey;
	GameFastEddy->playerheight = GameFastEddy->rowsize * 3 / 4;
	GameFastEddy->enemyheight = GameFastEddy->rowsize * 2 / 5;
	GameFastEddy->playerjumpheight = GameFastEddy->rowsize * 2/ 4;
	GameFastEddy->collectableheight = GameFastEddy->rowsize / 2;
	GameFastEddy->keyheight = GameFastEddy->rowsize / 4;
	GameFastEddy->playerjumpspeeddec = GameFastEddy->playerjumpspeed / GameFastEddy->playerjumpheight;


	Initialize_CSpriteObject(&GameFastEddy->player);
	for (int i = 0; i < GameFastEddy->maxladders; i++)
		Initialize_CSpriteObject(&GameFastEddy->ladders[i]);
	
	for (int i = 0; i < (int)(ScreenWidth / GameFastEddy->rowfloorsizex * GameFastEddy->rows); i++)
		Initialize_CSpriteObject(&GameFastEddy->floors[i]);

	for (int i = 0; i < GameFastEddy->maxenemies; i++)
		Initialize_CSpriteObject(&GameFastEddy->enemies[i]);

	for (int i = 0; i < GameFastEddy->maxcollectables; i++)
		Initialize_CSpriteObject(&GameFastEddy->collectables[i]);

	Initialize_CSpriteObject(&GameFastEddy->key);

	GameFastEddy->createkey = CGameFastEddy_createkey;
	GameFastEddy->destroykey = CGameFastEddy_destroykey;
	GameFastEddy->updatekey = CGameFastEddy_updatekey;
	GameFastEddy->createladders = CGameFastEddy_createladders;
	GameFastEddy->destroyladders = CGameFastEddy_destroyladders;
	GameFastEddy->createfloors = CGameFastEddy_createfloors;
	GameFastEddy->destroyfloors = CGameFastEddy_destroyfloors;
	GameFastEddy->updateplayer = CGameFastEddy_updateplayer;
	GameFastEddy->createplayer = CGameFastEddy_createplayer;
	GameFastEddy->destroyplayer = CGameFastEddy_destroyplayer;
	GameFastEddy->updateenemies = CGameFastEddy_updateenemies;
	GameFastEddy->createenemies = CGameFastEddy_createenemies;
	GameFastEddy->createenemy = CGameFastEddy_createenemy;
	GameFastEddy->enemyenablelevelend = CGameFastEddy_enemyenablelevelend;
	GameFastEddy->destroyenemies = CGameFastEddy_destroyenemies;
	GameFastEddy->updatecollectables = CGameFastEddy_updatecollectables;
	GameFastEddy->createcollectables = CGameFastEddy_createcollectables;
	GameFastEddy->destroycollectable = CGameFastEddy_destroycollectable;
	GameFastEddy->destroyallcollectables = CGameFastEddy_destroyallcollectables;
	GameFastEddy->init = CGameFastEddy_init;
	GameFastEddy->deinit = CGameFastEddy_deinit;
	GameFastEddy->UnloadGraphics = CGameFastEddy_UnloadGraphics;
	GameFastEddy->LoadGraphics = CGameFastEddy_LoadGraphics;
	GameFastEddy->LoadSound = CGameFastEddy_LoadSound;
	GameFastEddy->UnLoadSound = CGameFastEddy_UnLoadSound;
	GameFastEddy->UpdateObjects = CGameFastEddy_UpdateObjects;
	GameFastEddy->DrawBackground = CGameFastEddy_DrawBackground;
	GameFastEddy->Draw = CGameFastEddy_Draw;
	GameFastEddy->UpdateLogic = CGameFastEddy_UpdateLogic;
	return GameFastEddy;
}

void Destroy_CGameFastEddy(CGameFastEddy* GameFastEddy)
{
	Destroy_CGameBase(GameFastEddy->GameBase);
}



//end level key ----------------------------------------------------------------------------------------------------------------

void CGameFastEddy_createkey(CGameFastEddy* GameFastEddy)
{
	GameFastEddy->key.spr = GameFastEddy->GameBase->Game->Sprites->CreateSprite();
	GameFastEddy->key.alive = true;
	SDL_Point tz = GameFastEddy->GameBase->Game->Image->ImageSize(GameFastEddy->spritesheetkey);
	Vec2F scale = {GameFastEddy->keyheight / tz.y, GameFastEddy->keyheight / tz.y};
	GameFastEddy->key.tz.x = tz.x * scale.x;
	GameFastEddy->key.tz.y = tz.y * scale.y;
	GameFastEddy->GameBase->Game->Sprites->SetSpriteImage(GameFastEddy->GameBase->Game->Renderer,GameFastEddy->key.spr, &GameFastEddy->spritesheetkey, 1, 1);
	GameFastEddy->GameBase->Game->Sprites->SetSpriteScale(GameFastEddy->GameBase->Game->Renderer,GameFastEddy->key.spr, scale);
	GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->key.spr, 0, 0, 0);
	GameFastEddy->GameBase->Game->Sprites->SetSpriteCollisionShape(GameFastEddy->key.spr, SHAPE_BOX, tz.x - 20, tz.y, 0, 0, 0);
	GameFastEddy->key.pos.y = GameFastEddy->enemies[GameFastEddy->rowzeroenemyindex].pos.y - GameFastEddy->enemies[GameFastEddy->rowzeroenemyindex].tz.y;
	GameFastEddy->key.pos.x = GameFastEddy->enemies[GameFastEddy->rowzeroenemyindex].pos.x;
	GameFastEddy->GameBase->Game->Sprites->SetSpriteLocation(GameFastEddy->key.spr, GameFastEddy->key.pos);
}

void CGameFastEddy_destroykey(CGameFastEddy* GameFastEddy)
{
	if (GameFastEddy->key.alive)
	{
		GameFastEddy->GameBase->Game->Sprites->RemoveSprite(GameFastEddy->key.spr);
		GameFastEddy->key.alive = false;
	}
}

void CGameFastEddy_updatekey(CGameFastEddy* GameFastEddy)
{
	if (GameFastEddy->key.alive)
	{
		GameFastEddy->key.pos.x = GameFastEddy->enemies[GameFastEddy->rowzeroenemyindex].pos.x;
		GameFastEddy->key.pos.y = GameFastEddy->enemies[GameFastEddy->rowzeroenemyindex].pos.y - GameFastEddy->enemies[GameFastEddy->rowzeroenemyindex].tz.y;
		GameFastEddy->GameBase->Game->Sprites->SetSpriteLocation(GameFastEddy->key.spr, GameFastEddy->key.pos);

		if (GameFastEddy->GameBase->Game->Sprites->DetectSpriteCollision(GameFastEddy->key.spr, GameFastEddy->player.spr))
		{
			GameFastEddy->GameBase->Game->Audio->PlaySound(GameFastEddy->SfxSucces, 0);
			GameFastEddy->GameBase->Game->AddToScore(200);
			SDL_Delay(500);
			GameFastEddy->GameBase->level += 1;
			GameFastEddy->collecteditems = 0;
			GameFastEddy->collectedcreated = 0;
			GameFastEddy->destroyenemies(GameFastEddy);
			GameFastEddy->destroyplayer(GameFastEddy);
			GameFastEddy->destroyallcollectables(GameFastEddy);
			GameFastEddy->destroykey(GameFastEddy);
			GameFastEddy->destroyladders(GameFastEddy);
			GameFastEddy->createladders(GameFastEddy);
			GameFastEddy->createplayer(GameFastEddy);
			GameFastEddy->createenemies(GameFastEddy,true);
			GameFastEddy->createcollectables(GameFastEddy,-1);
			GameFastEddy->createkey(GameFastEddy);
		}
	}
}

//collectables ----------------------------------------------------------------------------------------------------------------

void CGameFastEddy_destroyallcollectables(CGameFastEddy* GameFastEddy)
{
	for (int i = 0; i < GameFastEddy->maxcollectables; i++)
		GameFastEddy->destroycollectable(GameFastEddy,i);
}

void CGameFastEddy_destroycollectable(CGameFastEddy* GameFastEddy, int index)
{
	if (GameFastEddy->collectables[index].alive)
	{
		GameFastEddy->GameBase->Game->Sprites->RemoveSprite(GameFastEddy->collectables[index].spr);
		GameFastEddy->collectables[index].alive = false;
	}
}

void CGameFastEddy_createcollectables(CGameFastEddy* GameFastEddy, int ignorerow)
{
	SDL_Point tz = {128, 128};
	for (int i = 0; i < GameFastEddy->maxcollectables; i++)
	{
		if(GameFastEddy->collectedcreated < 10)
		{
			if (!GameFastEddy->collectables[i].alive)
			{
				bool bok = false;
				int row = ignorerow;
				while (!bok)
				{
					row = 1 + (rand() % (GameFastEddy->rows-1));
					while(row == ignorerow)
						row = 1 + (rand() % (GameFastEddy->rows-1));
					bool bnocollision = true;
					for(int j = 0; j < GameFastEddy->maxcollectables; j++)
					{
						if(GameFastEddy->collectables[j].alive)
						{
							if(GameFastEddy->collectables[j].row == row)
							{
								bnocollision = false;
								break;
							}
						}
					}
					bok = bnocollision;
				}

				GameFastEddy->collectables[i].spr = GameFastEddy->GameBase->Game->Sprites->CreateSprite();
				GameFastEddy->collectables[i].alive = true;
				GameFastEddy->collectables[i].row = row;
				GameFastEddy->collectables[i].state = rand() % 3;
				Vec2F scale = {GameFastEddy->collectableheight / tz.y, GameFastEddy->collectableheight / tz.y};
				GameFastEddy->collectables[i].tz.x = tz.x * scale.x;
				GameFastEddy->collectables[i].tz.y = tz.y * scale.y;
				GameFastEddy->GameBase->Game->Sprites->SetSpriteImage(GameFastEddy->GameBase->Game->Renderer,GameFastEddy->collectables[i].spr, &GameFastEddy->spritesheetcollectable, 5, 1);
				GameFastEddy->GameBase->Game->Sprites->SetSpriteScale(GameFastEddy->GameBase->Game->Renderer,GameFastEddy->collectables[i].spr, scale);
				GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->collectables[i].spr, ((GameFastEddy->GameBase->level-1) % 5), ((GameFastEddy->GameBase->level-1) % 5), 0);
				GameFastEddy->GameBase->Game->Sprites->SetSpriteCollisionShape(GameFastEddy->collectables[i].spr, SHAPE_CIRCLE, tz.x - 66, tz.y - 66, 0, 0, 0);
				//Game->Sprites->SetSpriteCollisionShape(GameFastEddy->collectables[i].spr, SHAPE_BOX, tz.x - 66, tz.y - 66, 0, 0, 0);
				GameFastEddy->collectables[i].pos.y = (row) * GameFastEddy->rowspacingsize + GameFastEddy->rowfloorsizey / 2;
				GameFastEddy->collectables[i].pos.x = ((GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) /7) +
					(rand() % (GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft - ((GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) /6 )));
				GameFastEddy->GameBase->Game->Sprites->SetSpriteLocation(GameFastEddy->collectables[i].spr, GameFastEddy->collectables[i].pos);
				GameFastEddy->collectedcreated += 1;
			}
		}
	}
}

void CGameFastEddy_updatecollectables(CGameFastEddy* GameFastEddy)
{
	for(int i = 0; i < GameFastEddy->maxcollectables; i++)
	{
		if (GameFastEddy->collectables[i].alive)
		{
			if (GameFastEddy->collectables[i].state == GameFastEddy->collectablestatemoveleft)
			{
				GameFastEddy->collectables[i].pos.x -= GameFastEddy->collectablespeed;
				if (GameFastEddy->collectables[i].pos.x + GameFastEddy->collectables[i].tz.x / 2 < GameFastEddy->GameBase->screenleft)
					GameFastEddy->collectables[i].pos.x = GameFastEddy->GameBase->screenright + GameFastEddy->collectables[i].tz.x / 2;
				GameFastEddy->GameBase->Game->Sprites->SetSpriteLocation(GameFastEddy->collectables[i].spr, GameFastEddy->collectables[i].pos);
			}

			if (GameFastEddy->collectables[i].state == GameFastEddy->collectablestatemoveright)
			{
				GameFastEddy->collectables[i].pos.x += GameFastEddy->collectablespeed;
				if(GameFastEddy->collectables[i].pos.x - GameFastEddy->collectables[i].tz.x / 2 > GameFastEddy->GameBase->screenright)
					GameFastEddy->collectables[i].pos.x = GameFastEddy->GameBase->screenleft - GameFastEddy->collectables[i].tz.x / 2;
				GameFastEddy->GameBase->Game->Sprites->SetSpriteLocation(GameFastEddy->collectables[i].spr, GameFastEddy->collectables[i].pos);
			}

			if (GameFastEddy->GameBase->Game->Sprites->DetectSpriteCollision(GameFastEddy->collectables[i].spr, GameFastEddy->player.spr))
			{
				GameFastEddy->GameBase->Game->Audio->PlaySound(GameFastEddy->SfxCollect, 0);
				int ignorerow = GameFastEddy->collectables[i].row;
				GameFastEddy->destroycollectable(GameFastEddy, i);
				GameFastEddy->collecteditems += 1;
				GameFastEddy->GameBase->Game->AddToScore(GameFastEddy->collecteditems * 10);
				if(GameFastEddy->collecteditems >= 9)
					GameFastEddy->enemyenablelevelend(GameFastEddy);
				GameFastEddy->createcollectables(GameFastEddy,ignorerow);
			}
		}
	}
}

//enemies ----------------------------------------------------------------------------------------------------------------

void CGameFastEddy_destroyenemies(CGameFastEddy* GameFastEddy)
{
	for(int i = 0; i < GameFastEddy->maxenemies; i++)
	{
		if (GameFastEddy->enemies[i].alive)
		{
			GameFastEddy->GameBase->Game->Sprites->RemoveSprite(GameFastEddy->enemies[i].spr);
			GameFastEddy->enemies[i].alive = false;
		}
	}
}

void CGameFastEddy_enemyenablelevelend(CGameFastEddy* GameFastEddy)
{
	int index = GameFastEddy->rowzeroenemyindex;
	SDL_Point tz = {32, 32};
	Vec2F scale = {GameFastEddy->enemyheight / tz.y, GameFastEddy->enemyheight / tz.y};
	GameFastEddy->enemies[index].tz.x = tz.x * scale.x;
	GameFastEddy->enemies[index].tz.y = tz.y * scale.y;
	GameFastEddy->enemies[index].pos.y = (0 + 1) * GameFastEddy->rowspacingsize - GameFastEddy->rowfloorsizey / 2 - GameFastEddy->enemies[index].tz.y / 2;
	GameFastEddy->GameBase->Game->Sprites->SetSpriteScale(GameFastEddy->GameBase->Game->Renderer,GameFastEddy->enemies[index].spr, scale);
	GameFastEddy->GameBase->Game->Sprites->SetSpriteCollisionShape(GameFastEddy->enemies[index].spr, SHAPE_BOX, tz.x - 10, tz.y-10,0, 0, 0);
	GameFastEddy->GameBase->Game->Sprites->SetSpriteLocation(GameFastEddy->enemies[index].spr, GameFastEddy->enemies[index].pos);
}

void CGameFastEddy_createenemy(CGameFastEddy* GameFastEddy, int row, float x, int state, int group, int multiply)
{
	SDL_Point tz = {32, 32};
	for (int i = 0; i < GameFastEddy->maxenemies; i++)
	{
		if (!GameFastEddy->enemies[i].alive)
		{
			if(row == 0)
				GameFastEddy->rowzeroenemyindex = i;
			GameFastEddy->enemies[i].spr = GameFastEddy->GameBase->Game->Sprites->CreateSprite();
			GameFastEddy->enemies[i].alive = true;
			GameFastEddy->enemies[i].row = row;
			GameFastEddy->enemies[i].group = group;
			GameFastEddy->enemies[i].state = state;
			Vec2F scale = {GameFastEddy->enemyheight / tz.y * multiply, GameFastEddy->enemyheight / tz.y * multiply};
			GameFastEddy->enemies[i].tz.x = tz.x * scale.x;
			GameFastEddy->enemies[i].tz.y = tz.y * scale.y;
			GameFastEddy->GameBase->Game->Sprites->SetSpriteImage(GameFastEddy->GameBase->Game->Renderer,GameFastEddy->enemies[i].spr, &GameFastEddy->spritesheetenemy, 3, 4);
			GameFastEddy->GameBase->Game->Sprites->SetSpriteScale(GameFastEddy->GameBase->Game->Renderer,GameFastEddy->enemies[i].spr, scale);
			if (GameFastEddy->enemies[i].state == GameFastEddy->enemystatemoveright)
				GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->enemies[i].spr, 6, 8, 10);

			if (GameFastEddy->enemies[i].state == GameFastEddy->enemystateidle)
				GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->enemies[i].spr, 0, 2, 10);

			if (GameFastEddy->enemies[i].state == GameFastEddy->enemystatemoveleft)
				GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->enemies[i].spr, 3, 5, 10);

			if (GameFastEddy->enemies[i].state == GameFastEddy->enemystatewaitmove)
			{
				GameFastEddy->enemies[i].stateticks = 60 * 6;
				GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->enemies[i].spr, 0, 2, 10);
			}

			GameFastEddy->GameBase->Game->Sprites->SetSpriteCollisionShape(GameFastEddy->enemies[i].spr, SHAPE_BOX, tz.x - 10, tz.y-10,0, 0, 0);
			GameFastEddy->enemies[i].pos.y = (row + 1) * GameFastEddy->rowspacingsize - GameFastEddy->rowfloorsizey / 2 - GameFastEddy->enemies[i].tz.y / 2;
			GameFastEddy->enemies[i].pos.x = x;
			GameFastEddy->GameBase->Game->Sprites->SetSpriteLocation(GameFastEddy->enemies[i].spr, GameFastEddy->enemies[i].pos);
			break;
		}
	}
}

void CGameFastEddy_createenemies(CGameFastEddy* GameFastEddy, bool levelsucces)
{
	if (GameFastEddy->GameBase->level-1 < 5)
	{
		GameFastEddy->createenemy(GameFastEddy,0, (GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) / 2, GameFastEddy->enemystatemoveright, 0, 2);
		if (levelsucces)
			GameFastEddy->createenemy(GameFastEddy,1, (GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) * 4 / 10, GameFastEddy->enemystatemoveright, 1, 1);
		else
			GameFastEddy->createenemy(GameFastEddy,1, (GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) / 10, GameFastEddy->enemystateidle, 1, 1);

		GameFastEddy->createenemy(GameFastEddy,2, (GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) / 2, GameFastEddy->enemystatemoveright, 2, 1);
		if (levelsucces)
			GameFastEddy->createenemy(GameFastEddy,3, (GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) * 6 / 10, GameFastEddy->enemystatemoveright, 3, 1);
		else
			GameFastEddy->createenemy(GameFastEddy,3, (GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) * 6 / 10, GameFastEddy->enemystateidle, 3, 1);

		GameFastEddy->createenemy(GameFastEddy,4, (GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) / 10, GameFastEddy->enemystatewaitmove, 4, 1);
	}
	else
	{
		if (GameFastEddy->GameBase->level-1 < 10)
		{
			GameFastEddy->createenemy(GameFastEddy,0, (GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) / 2, GameFastEddy->enemystatemoveright, 0, 2);
			GameFastEddy->createenemy(GameFastEddy,1, (GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) * 4 / 10, GameFastEddy->enemystatemoveright, 1, 1);
			GameFastEddy->createenemy(GameFastEddy,1, (GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) * 5 / 10, GameFastEddy->enemystatemoveright, 1, 1);
			GameFastEddy->createenemy(GameFastEddy,2, (GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) / 2, GameFastEddy->enemystatemoveright, 2, 1);
			GameFastEddy->createenemy(GameFastEddy,3, (GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) * 6 / 10, GameFastEddy->enemystatemoveleft, 3, 1);
			GameFastEddy->createenemy(GameFastEddy,4, (GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) / 10, GameFastEddy->enemystatewaitmove, 4, 1);
		}
		else
		{
			if (GameFastEddy->GameBase->level-1 < 15)
			{
				GameFastEddy->createenemy(GameFastEddy,0, (GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) / 2, GameFastEddy->enemystatemoveright, 0, 2);
				GameFastEddy->createenemy(GameFastEddy,1, (GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) * 4 / 10, GameFastEddy->enemystatemoveright, 1, 1);
				GameFastEddy->createenemy(GameFastEddy,1, (GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) * 5 / 10, GameFastEddy->enemystatemoveright, 1, 1);
				GameFastEddy->createenemy(GameFastEddy,2, (GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) / 2, GameFastEddy->enemystatemoveright, 2, 1);
				GameFastEddy->createenemy(GameFastEddy,3, (GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) * 6 / 10, GameFastEddy->enemystatemoveleft, 3, 1);
				GameFastEddy->createenemy(GameFastEddy,3, (GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) * 7 / 10, GameFastEddy->enemystatemoveleft, 3, 1);
				GameFastEddy->createenemy(GameFastEddy,4, (GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) / 10, GameFastEddy->enemystatewaitmove, 4, 1);
			}
			else
			{
				GameFastEddy->createenemy(GameFastEddy,0, (GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) / 2, GameFastEddy->enemystatemoveright, 0, 2);
				GameFastEddy->createenemy(GameFastEddy,1, (GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) * 4 / 10, GameFastEddy->enemystatemoveright, 1, 1);
				GameFastEddy->createenemy(GameFastEddy,1, (GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) * 5 / 10, GameFastEddy->enemystatemoveright, 1, 1);
				GameFastEddy->createenemy(GameFastEddy,2, (GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) / 2, GameFastEddy->enemystatemoveright, 2, 1);
				GameFastEddy->createenemy(GameFastEddy,3, (GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) * 1 / 10, GameFastEddy->enemystatemoveright, 3, 1);
				GameFastEddy->createenemy(GameFastEddy,3, (GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) * 6 / 10, GameFastEddy->enemystatemoveright, 4, 1);
				GameFastEddy->createenemy(GameFastEddy,4, (GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) / 10, GameFastEddy->enemystatewaitmove, 5, 1);
			}
		}
	}
}

void CGameFastEddy_updateenemies(CGameFastEddy* GameFastEddy)
{
	for (int i = 0; i < GameFastEddy->maxenemies; i++)
	{
		if (GameFastEddy->enemies[i].alive)
		{
			if (GameFastEddy->enemies[i].state == GameFastEddy->enemystatewaitmove)
			{
				if (GameFastEddy->enemies[i].stateticks > 0)
					GameFastEddy->enemies[i].stateticks -= 1;
				else
				{
					GameFastEddy->enemies[i].state = GameFastEddy->enemystatemoveright;
					GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->enemies[i].spr, 6, 8, 10);
				}
			}

			if (GameFastEddy->enemies[i].state == GameFastEddy->enemystatemoveright)
			{
				if (GameFastEddy->enemies[i].pos.x + GameFastEddy->enemies[i].tz.x / 2 + GameFastEddy->playerspeed < GameFastEddy->GameBase->screenright)
					GameFastEddy->enemies[i].pos.x += GameFastEddy->playerspeed;
				else
				{
					for(int j = 0; j < GameFastEddy->maxenemies; j++)
					{
						if (GameFastEddy->enemies[j].alive)
						{
							if(GameFastEddy->enemies[i].group == GameFastEddy->enemies[j].group)
							{
								GameFastEddy->enemies[j].state = GameFastEddy->enemystatemoveleft;
								GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->enemies[j].spr, 3, 5, 10);
							}
						}
					}
				}
			}

			if(GameFastEddy->enemies[i].state == GameFastEddy->enemystatemoveleft)
			{
				if ( GameFastEddy->enemies[i].pos.x - GameFastEddy->enemies[i].tz.x / 2 - GameFastEddy->playerspeed > GameFastEddy->GameBase->screenleft)
					GameFastEddy->enemies[i].pos.x -= GameFastEddy->playerspeed;
				else
				{
					for(int j = 0; j < GameFastEddy->maxenemies; j++)
					{
						if(GameFastEddy->enemies[j].alive)
						{
							if(GameFastEddy->enemies[i].group == GameFastEddy->enemies[j].group)
							{
								GameFastEddy->enemies[j].state = GameFastEddy->enemystatemoveright;
								GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->enemies[j].spr, 6, 8, 10);
							}
						}
					}
				}
			}

			GameFastEddy->GameBase->Game->Sprites->SetSpriteLocation(GameFastEddy->enemies[i].spr, GameFastEddy->enemies[i].pos);

			if (GameFastEddy->GameBase->Game->Sprites->DetectSpriteCollision(GameFastEddy->enemies[i].spr, GameFastEddy->player.spr))
			{
				if (((GameFastEddy->player.state != GameFastEddy->playerstateclimbup) &&
					(GameFastEddy->player.state != GameFastEddy->playerstateclimbdown) &&
					(GameFastEddy->player.state != GameFastEddy->playerstateunknown)) ||
					(GameFastEddy->enemies[i].row == 0))
				{
					GameFastEddy->GameBase->Game->Audio->PlaySound(GameFastEddy->SfxDie, 0);
					if (GameFastEddy->GameBase->Game->GameMode == GMGame)
						GameFastEddy->GameBase->HealthPoints -= 1;

					if (GameFastEddy->GameBase->HealthPoints > 0)
					{
						GameFastEddy->GameBase->Game->AddToScore(-100);
						SDL_Delay(500);
						GameFastEddy->destroyenemies(GameFastEddy);
						GameFastEddy->destroyplayer(GameFastEddy);
						GameFastEddy->destroykey(GameFastEddy);
						GameFastEddy->createplayer(GameFastEddy);
						GameFastEddy->createenemies(GameFastEddy,false);
						GameFastEddy->createkey(GameFastEddy);
						if (GameFastEddy->collecteditems >= 9)
							GameFastEddy->enemyenablelevelend(GameFastEddy);
					}
					break;
				}
			}
		}
	}
}

//player ----------------------------------------------------------------------------------------------------------------

void CGameFastEddy_destroyplayer(CGameFastEddy* GameFastEddy)
{
	if (GameFastEddy->player.alive)
	{
		GameFastEddy->GameBase->Game->Sprites->RemoveSprite(GameFastEddy->player.spr);
		GameFastEddy->player.alive = false;
	}
}

void CGameFastEddy_createplayer(CGameFastEddy* GameFastEddy)
{
	GameFastEddy->player.spr = GameFastEddy->GameBase->Game->Sprites->CreateSprite();
	GameFastEddy->player.alive = true;
	SDL_Point tz = {238, 342};
	Vec2F scale = {GameFastEddy->playerheight / tz.y, GameFastEddy->playerheight / tz.y};
	GameFastEddy->player.tz.x = tz.x * scale.x;
	GameFastEddy->player.tz.y = tz.y * scale.y;
	GameFastEddy->player.state = GameFastEddy->playerstateunknown;
	GameFastEddy->GameBase->Game->Sprites->SetSpriteDepth(GameFastEddy->player.spr, 5);
	GameFastEddy->GameBase->Game->Sprites->SetSpriteImage(GameFastEddy->GameBase->Game->Renderer,GameFastEddy->player.spr, &GameFastEddy->spritesheetplayeridle, 1, 15);
	GameFastEddy->GameBase->Game->Sprites->SetSpriteScale(GameFastEddy->GameBase->Game->Renderer,GameFastEddy->player.spr, scale);
	GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->player.spr, 0, 14, 10);
	GameFastEddy->GameBase->Game->Sprites->SetSpriteCollisionShape(GameFastEddy->player.spr, SHAPE_BOX, tz.x - 120, tz.y-30,0,0,0);
	GameFastEddy->player.pos = { (float)(GameFastEddy->GameBase->screenright - GameFastEddy->GameBase->screenleft) / 2, 5 * GameFastEddy->rowspacingsize - GameFastEddy->rowfloorsizey / 2 - GameFastEddy->player.tz.y / 2};
	GameFastEddy->GameBase->Game->Sprites->SetSpriteLocation(GameFastEddy->player.spr, GameFastEddy->player.pos);
}

void CGameFastEddy_updateplayer(CGameFastEddy* GameFastEddy)
{
	bool jump = false;
	bool left = false;
	bool right = false;
	bool down = false;
	bool up = false;
	if (GameFastEddy->player.alive)
	{
		if ((GameFastEddy->player.state != GameFastEddy->playerstatejump) &&
			(GameFastEddy->player.state != GameFastEddy->playerstateleftjump) &&
			(GameFastEddy->player.state != GameFastEddy->playerstaterightjump) &&
			(GameFastEddy->player.state != GameFastEddy->playerstateclimbup) &&
			(GameFastEddy->player.state != GameFastEddy->playerstateclimbdown))
		{

			//if fedebugmode and (!global.prevc.b and global.c.b) then
			//	feenemyenablelevelend()
			//endif

			if (!GameFastEddy->GameBase->Game->Input->PrevButtons.ButA && GameFastEddy->GameBase->Game->Input->Buttons.ButA)
				jump = true;

			if ((GameFastEddy->GameBase->Game->Input->Buttons.ButUp) ||
				(GameFastEddy->GameBase->Game->Input->Buttons.ButUp2) ||
				(GameFastEddy->GameBase->Game->Input->Buttons.ButDpadUp))
				up = true;

			if ((GameFastEddy->GameBase->Game->Input->Buttons.ButDown) ||
				(GameFastEddy->GameBase->Game->Input->Buttons.ButDown2) ||
				(GameFastEddy->GameBase->Game->Input->Buttons.ButDpadDown))
				down = true;

			if ((GameFastEddy->GameBase->Game->Input->Buttons.ButLeft) ||
				(GameFastEddy->GameBase->Game->Input->Buttons.ButLeft2) ||
				(GameFastEddy->GameBase->Game->Input->Buttons.ButDpadLeft))
				left = true;

			if ((GameFastEddy->GameBase->Game->Input->Buttons.ButRight) ||
				(GameFastEddy->GameBase->Game->Input->Buttons.ButRight2) ||
				(GameFastEddy->GameBase->Game->Input->Buttons.ButDpadRight))
				right = true;

			if (jump)
			{
				if (left)
				{
					if (GameFastEddy->player.state != GameFastEddy->playerstateleftjump)
					{
						GameFastEddy->GameBase->Game->Sprites->SetSpriteImage(GameFastEddy->GameBase->Game->Renderer,GameFastEddy->player.spr, &GameFastEddy->spritesheetplayerjump, 1, 6);
						GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->player.spr, 3, 3, 10);

						GameFastEddy->player.state = GameFastEddy->playerstateleftjump;
						GameFastEddy->player.floory = GameFastEddy->player.pos.y;
						GameFastEddy->player.jumpdown = false;
						GameFastEddy->player.force = GameFastEddy->playerjumpspeed;
					}
				}
				else
				{
					if (right)
					{
						if (GameFastEddy->player.state != GameFastEddy->playerstaterightjump)
						{
							GameFastEddy->GameBase->Game->Sprites->SetSpriteImage(GameFastEddy->GameBase->Game->Renderer,GameFastEddy->player.spr, &GameFastEddy->spritesheetplayerjump, 1, 6);
							GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->player.spr, 3, 3, 10);

							GameFastEddy->player.floory = GameFastEddy->player.pos.y;
							GameFastEddy->player.state = GameFastEddy->playerstaterightjump;
							GameFastEddy->player.jumpdown = false;
							GameFastEddy->player.force = GameFastEddy->playerjumpspeed;
						}
					}
					else
					{
						if(GameFastEddy->player.state != GameFastEddy->playerstatejump)
						{
							GameFastEddy->GameBase->Game->Sprites->SetSpriteImage(GameFastEddy->GameBase->Game->Renderer,GameFastEddy->player.spr, &GameFastEddy->spritesheetplayerjump, 1, 6);
							GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->player.spr, 3, 3, 10);
							GameFastEddy->player.floory = GameFastEddy->player.pos.y;
							GameFastEddy->player.state = GameFastEddy->playerstatejump;
							GameFastEddy->player.jumpdown = false;
							GameFastEddy->player.force = GameFastEddy->playerjumpspeed;
						}
					}
				}
			}
			else
			{
				if(up)
				{
					GameFastEddy->player.pos.y -= GameFastEddy->rowspacingsize/4;
					GameFastEddy->GameBase->Game->Sprites->SetSpriteLocation(GameFastEddy->player.spr, GameFastEddy->player.pos);
					bool found = false;
					for (int i = 0; i < GameFastEddy->maxladders; i++)
					{
						if (GameFastEddy->GameBase->Game->Sprites->DetectSpriteCollision(GameFastEddy->player.spr, GameFastEddy->ladders[i].spr))
						{
							GameFastEddy->GameBase->Game->Sprites->SetSpriteImage(GameFastEddy->GameBase->Game->Renderer,GameFastEddy->player.spr, &GameFastEddy->spritesheetplayerclimb, 1, 15);
							GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->player.spr, 0, 14, 20);

							GameFastEddy->player.pos.x = GameFastEddy->ladders[i].pos.x;
							GameFastEddy->player.state = GameFastEddy->playerstateclimbup;
							GameFastEddy->player.floory = GameFastEddy->player.pos.y + GameFastEddy->rowspacingsize/4;
							found = true;
							break;
						};
					};
					GameFastEddy->player.pos.y += GameFastEddy->rowspacingsize/4;
					GameFastEddy->GameBase->Game->Sprites->SetSpriteLocation(GameFastEddy->player.spr, GameFastEddy->player.pos);
					if (!found)
					{
						if (GameFastEddy->player.state != GameFastEddy->playerstateidle)
						{
							GameFastEddy->GameBase->Game->Sprites->SetSpriteImage(GameFastEddy->GameBase->Game->Renderer,GameFastEddy->player.spr, &GameFastEddy->spritesheetplayeridle, 1, 15);
							GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->player.spr, 0, 14, 10);
							GameFastEddy->player.state = GameFastEddy->playerstateidle;
						}
					}
				}
				else
				{
					if (down)
					{
						GameFastEddy->player.pos.y += GameFastEddy->rowspacingsize - GameFastEddy->playerheight / 4;
						GameFastEddy->GameBase->Game->Sprites->SetSpriteLocation(GameFastEddy->player.spr, GameFastEddy->player.pos);
						bool found = false;
						for (int i = 0; i < GameFastEddy->maxladders; i++)
						{
							if (GameFastEddy->GameBase->Game->Sprites->DetectSpriteCollision(GameFastEddy->player.spr, GameFastEddy->ladders[i].spr))
							{
								GameFastEddy->GameBase->Game->Sprites->SetSpriteImage(GameFastEddy->GameBase->Game->Renderer,GameFastEddy->player.spr, &GameFastEddy->spritesheetplayerclimb, 1, 15);
								GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->player.spr, 14, 0, 20);

								GameFastEddy->player.pos.x = GameFastEddy->ladders[i].pos.x;
								GameFastEddy->player.state = GameFastEddy->playerstateclimbdown;
								GameFastEddy->player.floory = GameFastEddy->player.pos.y - GameFastEddy->rowspacingsize + GameFastEddy->playerheight / 4;
								found = true;
								break;
							};
						};
						GameFastEddy->player.pos.y -= GameFastEddy->rowspacingsize - GameFastEddy->playerheight / 4;
						GameFastEddy->GameBase->Game->Sprites->SetSpriteLocation(GameFastEddy->player.spr, GameFastEddy->player.pos);
						if (!found)
						{
							if (GameFastEddy->player.state != GameFastEddy->playerstateidle)
							{
								GameFastEddy->GameBase->Game->Sprites->SetSpriteImage(GameFastEddy->GameBase->Game->Renderer,GameFastEddy->player.spr, &GameFastEddy->spritesheetplayeridle, 1, 15);
								GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->player.spr, 0, 14, 10);
								GameFastEddy->player.state = GameFastEddy->playerstateidle;
							}
						}
					}
					else
					{
						if (left)
						{
							if (GameFastEddy->player.state != GameFastEddy->playerstateleft)
							{
								GameFastEddy->GameBase->Game->Sprites->SetSpriteImage(GameFastEddy->GameBase->Game->Renderer,GameFastEddy->player.spr, &GameFastEddy->spritesheetplayerrun, 1, 7);
								GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->player.spr, 0, 6, 10);
								GameFastEddy->player.spr->xscale = -(abs(GameFastEddy->player.spr->xscale));
								GameFastEddy->player.state = GameFastEddy->playerstateleft;
							}
						}
						else
						{
							if (right)
							{
								if (GameFastEddy->player.state != GameFastEddy->playerstateright)
								{
									GameFastEddy->GameBase->Game->Sprites->SetSpriteImage(GameFastEddy->GameBase->Game->Renderer,GameFastEddy->player.spr, &GameFastEddy->spritesheetplayerrun, 1, 7);
									GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->player.spr, 0, 6, 10);
									GameFastEddy->player.spr->xscale = (abs(GameFastEddy->player.spr->xscale));
									GameFastEddy->player.state = GameFastEddy->playerstateright;
								}
							}
							else
							{
								if (GameFastEddy->player.state != GameFastEddy->playerstateidle)
								{
									GameFastEddy->GameBase->Game->Sprites->SetSpriteImage(GameFastEddy->GameBase->Game->Renderer,GameFastEddy->player.spr, &GameFastEddy->spritesheetplayeridle, 1, 15);
									GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->player.spr, 0, 14, 10);
									GameFastEddy->player.state = GameFastEddy->playerstateidle;
								}
							}
						}
					}
				}
			}
		}

		if (GameFastEddy->player.state == GameFastEddy->playerstateclimbup)
		{
			if (GameFastEddy->player.pos.y > GameFastEddy->player.floory - GameFastEddy->rowspacingsize)
				GameFastEddy->player.pos.y -= GameFastEddy->playerclimbspeed;
			else
			{
				GameFastEddy->player.pos.y = GameFastEddy->player.floory - GameFastEddy->rowspacingsize;
				GameFastEddy->player.state = GameFastEddy->playerstateunknown;
			}

			GameFastEddy->GameBase->Game->Sprites->SetSpriteLocation(GameFastEddy->player.spr, GameFastEddy->player.pos);
		}

		if (GameFastEddy->player.state == GameFastEddy->playerstateclimbdown)
		{
			if (GameFastEddy->player.pos.y < GameFastEddy->player.floory + GameFastEddy->rowspacingsize)
				GameFastEddy->player.pos.y += GameFastEddy->playerclimbspeed;
			else
			{
				GameFastEddy->player.pos.y = GameFastEddy->player.floory + GameFastEddy->rowspacingsize;
				GameFastEddy->player.state = GameFastEddy->playerstateunknown;
			}
			GameFastEddy->GameBase->Game->Sprites->SetSpriteLocation(GameFastEddy->player.spr, GameFastEddy->player.pos);
		};

		if (GameFastEddy->player.state == GameFastEddy->playerstatejump)
		{
			if (!GameFastEddy->player.jumpdown)
			{
				if (GameFastEddy->player.pos.y > GameFastEddy->player.floory - GameFastEddy->playerjumpheight)
				{
					GameFastEddy->player.pos.y -= GameFastEddy->player.force;
					GameFastEddy->player.force -= GameFastEddy->playerjumpspeeddec;
				}
				else
					GameFastEddy->player.jumpdown = true;
			}
			else
			{
				if (GameFastEddy->player.pos.y < GameFastEddy->player.floory)
				{
					GameFastEddy->player.pos.y += GameFastEddy->player.force;
					GameFastEddy->player.force += GameFastEddy->playerjumpspeeddec;
				}
				else
				{
					GameFastEddy->player.pos.y = GameFastEddy->player.floory;
					GameFastEddy->player.state = GameFastEddy->playerstateunknown;
				}
			}
			GameFastEddy->GameBase->Game->Sprites->SetSpriteLocation(GameFastEddy->player.spr, GameFastEddy->player.pos);
			if (GameFastEddy->player.pos.y < GameFastEddy->player.floory - 5/6 * GameFastEddy->playerjumpheight)
				GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->player.spr, 5, 5, 0);
			else
			{
				if (GameFastEddy->player.pos.y < GameFastEddy->player.floory - 4/6 * GameFastEddy->playerjumpheight)
					GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->player.spr, 4, 4, 0);
				else
				{
					if(GameFastEddy->player.pos.y < GameFastEddy->player.floory - 3/6 * GameFastEddy->playerjumpheight)
						GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->player.spr, 3, 3, 0);
					else
					{
						if(GameFastEddy->player.pos.y < GameFastEddy->player.floory - 2/6 * GameFastEddy->playerjumpheight)
							GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->player.spr, 2, 2, 0);
						else
						{
							if(GameFastEddy->player.pos.y < GameFastEddy->player.floory - 1/6 * GameFastEddy->playerjumpheight)
								GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->player.spr, 1, 1, 0);
							else
							{
								if (GameFastEddy->player.pos.y < GameFastEddy->player.floory - 0/6 * GameFastEddy->playerjumpheight)
									GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->player.spr, 0, 0, 0);
							}
						}
					}
				}
			}
		}

		if ((GameFastEddy->player.state == GameFastEddy->playerstateleftjump) ||
			(GameFastEddy->player.state == GameFastEddy->playerstaterightjump))
		{
			if (! GameFastEddy->player.jumpdown)
			{
				if (GameFastEddy->player.pos.y > GameFastEddy->player.floory - GameFastEddy->playerjumpheight)
				{
					GameFastEddy->player.pos.y -= GameFastEddy->player.force;
					GameFastEddy->player.force -= GameFastEddy->playerjumpspeeddec;
				}
				else
					GameFastEddy->player.jumpdown = true;
			}
			else
			{
				if (GameFastEddy->player.pos.y < GameFastEddy->player.floory)
				{
					GameFastEddy->player.pos.y += GameFastEddy->player.force;
					GameFastEddy->player.force += GameFastEddy->playerjumpspeeddec;
				}
				else
				{
					GameFastEddy->player.pos.y = GameFastEddy->player.floory;
					GameFastEddy->player.state = GameFastEddy->playerstateunknown;
				}
			}

			if (GameFastEddy->player.spr->xscale / abs(GameFastEddy->player.spr->xscale) == 1)
			{
				if ( GameFastEddy->player.pos.x + GameFastEddy->player.tz.x / 2 + GameFastEddy->playerspeed < GameFastEddy->GameBase->screenright)
					GameFastEddy->player.pos.x += GameFastEddy->playerspeed ;
				else
					GameFastEddy->player.pos.x = GameFastEddy->GameBase->screenright - GameFastEddy->player.tz.x / 2;
			}
			else
			{
				if ( GameFastEddy->player.pos.x - GameFastEddy->player.tz.x / 2 - GameFastEddy->playerspeed > GameFastEddy->GameBase->screenleft)
					GameFastEddy->player.pos.x -= GameFastEddy->playerspeed;
				else
					GameFastEddy->player.pos.x = GameFastEddy->GameBase->screenleft + GameFastEddy->player.tz.x / 2;
			}

			if (GameFastEddy->player.pos.y < GameFastEddy->player.floory - 5/6 * GameFastEddy->playerjumpheight)
				GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->player.spr, 5, 5, 0);
			else
			{
				if (GameFastEddy->player.pos.y < GameFastEddy->player.floory - 4/6 * GameFastEddy->playerjumpheight)
					GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->player.spr, 4, 4, 0);
				else
				{
					if (GameFastEddy->player.pos.y < GameFastEddy->player.floory - 3/6 * GameFastEddy->playerjumpheight)
						GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->player.spr, 3, 3, 0);
					else
					{
						if (GameFastEddy->player.pos.y < GameFastEddy->player.floory - 2/6 * GameFastEddy->playerjumpheight)
							GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->player.spr, 2, 2, 0);
						else
						{
							if(GameFastEddy->player.pos.y < GameFastEddy->player.floory - 1/6 * GameFastEddy->playerjumpheight)
								GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->player.spr, 1, 1, 0);
							else
							{
								if(GameFastEddy->player.pos.y < GameFastEddy->player.floory - 0/6 * GameFastEddy->playerjumpheight)
									GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->player.spr, 0, 0, 0);
							}
						}
					}
				}
			}

			GameFastEddy->GameBase->Game->Sprites->SetSpriteLocation(GameFastEddy->player.spr, GameFastEddy->player.pos);
		}

		if (GameFastEddy->player.state == GameFastEddy->playerstateright)
		{
			if ( GameFastEddy->player.pos.x + GameFastEddy->player.tz.x / 2 + GameFastEddy->playerspeed < GameFastEddy->GameBase->screenright)
				GameFastEddy->player.pos.x += GameFastEddy->playerspeed;
			else
				GameFastEddy->player.pos.x = GameFastEddy->GameBase->screenright - GameFastEddy->player.tz.x / 2;
			GameFastEddy->GameBase->Game->Sprites->SetSpriteLocation(GameFastEddy->player.spr, GameFastEddy->player.pos);
		}

		if (GameFastEddy->player.state == GameFastEddy->playerstateleft)
		{
			if ( GameFastEddy->player.pos.x - GameFastEddy->player.tz.x / 2 - GameFastEddy->playerspeed > GameFastEddy->GameBase->screenleft)
				GameFastEddy->player.pos.x -= GameFastEddy->playerspeed;
			else
				GameFastEddy->player.pos.x = GameFastEddy->GameBase->screenleft + GameFastEddy->player.tz.x / 2;
			GameFastEddy->GameBase->Game->Sprites->SetSpriteLocation(GameFastEddy->player.spr, GameFastEddy->player.pos);
		}
	}
}



//floors ----------------------------------------------------------------------------------------------------------------

void CGameFastEddy_destroyfloors(CGameFastEddy* GameFastEddy)
{
	for (int i = 0; i < GameFastEddy->maxfloortiles; i++)
		GameFastEddy->GameBase->Game->Sprites->RemoveSprite(GameFastEddy->floors[i].spr);
}

void CGameFastEddy_createfloors(CGameFastEddy* GameFastEddy)
{
	int tilenr = 0;
	int i = 0;
	//SDL_Point tz = {64,64};
	SDL_Point tz = GameFastEddy->GameBase->Game->Image->ImageSize(GameFastEddy->spritesheet);
	tz.x /= 9*4;
	tz.y /= 10/2;
	for (int y = 0; y < GameFastEddy->rows; y++)
	{
		for (int x = 0; x < GameFastEddy->numfloortilesperrow; x++)
		{
			GameFastEddy->floors[i].spr = GameFastEddy->GameBase->Game->Sprites->CreateSprite();
			GameFastEddy->floors[i].alive = true;
			GameFastEddy->GameBase->Game->Sprites->SetSpriteImage(GameFastEddy->GameBase->Game->Renderer,GameFastEddy->floors[i].spr, &GameFastEddy->spritesheet, 9, 10);
			GameFastEddy->GameBase->Game->Sprites->SetSpriteLocation(GameFastEddy->floors[i].spr, {(float)x * GameFastEddy->rowfloorsizex + GameFastEddy->rowfloorsizex / 2, (float)(y + 1) * GameFastEddy->rowspacingsize - GameFastEddy->rowfloorsizey / 2});
			GameFastEddy->GameBase->Game->Sprites->SetSpriteScale(GameFastEddy->GameBase->Game->Renderer,GameFastEddy->floors[i].spr, {(float)GameFastEddy->rowfloorsizex / tz.x , (float)GameFastEddy->rowfloorsizey / tz.y});
			if (x == 0)
				tilenr = 36;
			else
			{
				if (x == GameFastEddy->numfloortilesperrow -1)
					tilenr = 38;
				else
					tilenr = 37;
			}
			GameFastEddy->GameBase->Game->Sprites->SetSpriteAnimation(GameFastEddy->floors[i].spr, tilenr, tilenr, 0);
			i += 1;
		}
	}
}

//ladders ----------------------------------------------------------------------------------------------------------------

void CGameFastEddy_destroyladders(CGameFastEddy* GameFastEddy)
{
	for (int i = 0; i < GameFastEddy->maxladders; i++)
		GameFastEddy->GameBase->Game->Sprites->RemoveSprite(GameFastEddy->ladders[i].spr);
}

void CGameFastEddy_createladders(CGameFastEddy* GameFastEddy)
{
	int i = 0;
	SDL_Point tz = GameFastEddy->GameBase->Game->Image->ImageSize(GameFastEddy->spritesheetladder);
	for (int y = 1; y < GameFastEddy->rows; y++)
	{
		int x1 = 1 + rand() % (int(GameFastEddy->laddersfitrows / 2)-1);
		GameFastEddy->ladders[i].spr = GameFastEddy->GameBase->Game->Sprites->CreateSprite();
		GameFastEddy->ladders[i].alive = true;
		GameFastEddy->ladders[i].pos = {x1 * GameFastEddy->ladderwidth + GameFastEddy->ladderwidth / 2 , y * GameFastEddy->rowspacingsize + GameFastEddy->rowspacingsize / 2 - GameFastEddy->rowfloorsizey / 2};
		GameFastEddy->GameBase->Game->Sprites->SetSpriteImage(GameFastEddy->GameBase->Game->Renderer,GameFastEddy->ladders[i].spr, &GameFastEddy->spritesheetladder);
		GameFastEddy->GameBase->Game->Sprites->SetSpriteCollisionShape(GameFastEddy->ladders[i].spr, SHAPE_BOX, tz.x - 20, tz.y-10,0,0,0);

		GameFastEddy->GameBase->Game->Sprites->SetSpriteLocation(GameFastEddy->ladders[i].spr, GameFastEddy->ladders[i].pos );
		GameFastEddy->GameBase->Game->Sprites->SetSpriteScale(GameFastEddy->GameBase->Game->Renderer,GameFastEddy->ladders[i].spr, {GameFastEddy->ladderwidth / tz.x , (GameFastEddy->rowspacingsize + GameFastEddy->rowfloorsizey/2) / tz.y});

		i += 1;
		int x2 = x1 + int(GameFastEddy->laddersfitrows / 2);
		GameFastEddy->ladders[i].spr = GameFastEddy->GameBase->Game->Sprites->CreateSprite();
		GameFastEddy->ladders[i].alive = true;
		GameFastEddy->ladders[i].pos = {x2 * GameFastEddy->ladderwidth + GameFastEddy->ladderwidth / 2 , y * GameFastEddy->rowspacingsize + GameFastEddy->rowspacingsize / 2 - GameFastEddy->rowfloorsizey / 2};
		GameFastEddy->GameBase->Game->Sprites->SetSpriteImage(GameFastEddy->GameBase->Game->Renderer,GameFastEddy->ladders[i].spr, &GameFastEddy->spritesheetladder);
		GameFastEddy->GameBase->Game->Sprites->SetSpriteCollisionShape(GameFastEddy->ladders[i].spr, SHAPE_BOX, tz.x - 20, tz.y-10,0,0,0);
		GameFastEddy->GameBase->Game->Sprites->SetSpriteLocation(GameFastEddy->ladders[i].spr, GameFastEddy->ladders[i].pos );
		GameFastEddy->GameBase->Game->Sprites->SetSpriteScale(GameFastEddy->GameBase->Game->Renderer,GameFastEddy->ladders[i].spr, {GameFastEddy->ladderwidth / tz.x , (GameFastEddy->rowspacingsize + GameFastEddy->rowfloorsizey/2) / tz.y});
		i += 1;
	}
}

//background ----------------------------------------------------------------------------------------------------------------

void CGameFastEddy_DrawBackground(CGameFastEddy* GameFastEddy)
{
	GameFastEddy->GameBase->Game->Image->DrawImage(GameFastEddy->GameBase->Game->Renderer, GameFastEddy->background, NULL, NULL);
}

//init - deinit ----------------------------------------------------------------------------------------------------------------

void CGameFastEddy_init(CGameFastEddy* GameFastEddy)
{
	GameFastEddy->collecteditems = 0;
	GameFastEddy->collectedcreated = 0;
	GameFastEddy->GameBase->level = 1;
	GameFastEddy->GameBase->HealthPoints = 3;
	GameFastEddy->LoadGraphics(GameFastEddy);
	GameFastEddy->createfloors(GameFastEddy);
	GameFastEddy->createladders(GameFastEddy);
	GameFastEddy->createenemies(GameFastEddy,false);
	GameFastEddy->createcollectables(GameFastEddy,-1);
	GameFastEddy->createkey(GameFastEddy);
	GameFastEddy->createplayer(GameFastEddy);
	GameFastEddy->LoadSound(GameFastEddy);
	GameFastEddy->GameBase->Game->CurrentGameMusicID = GameFastEddy->MusMusic;
	GameFastEddy->GameBase->Game->Audio->PlayMusic(GameFastEddy->MusMusic, -1);
}

void CGameFastEddy_deinit(CGameFastEddy* GameFastEddy)
{
	GameFastEddy->destroyfloors(GameFastEddy);
	GameFastEddy->destroyladders(GameFastEddy);
	GameFastEddy->destroyenemies(GameFastEddy);
	GameFastEddy->destroyallcollectables(GameFastEddy);
	GameFastEddy->destroykey(GameFastEddy);
	GameFastEddy->destroyplayer(GameFastEddy);
	GameFastEddy->UnLoadSound(GameFastEddy);
	GameFastEddy->GameBase->Game->SubStateCounter = 0;
	GameFastEddy->GameBase->Game->SubGameState = SGNone;
	GameFastEddy->GameBase->Game->CurrentGameMusicID = -1;
	GameFastEddy->UnloadGraphics(GameFastEddy);
}

void CGameFastEddy_LoadSound(CGameFastEddy* GameFastEddy)
{
	GameFastEddy->SfxDie = GameFastEddy->GameBase->Game->Audio->LoadSound("common/die.wav");
	GameFastEddy->SfxSucces = GameFastEddy->GameBase->Game->Audio->LoadSound("common/succes.wav");
	GameFastEddy->SfxCollect = GameFastEddy->GameBase->Game->Audio->LoadSound("common/coin.wav");
	GameFastEddy->MusMusic = GameFastEddy->GameBase->Game->Audio->LoadMusic("fasterdave/music.ogg");
}

void CGameFastEddy_UnLoadSound(CGameFastEddy* GameFastEddy)
{
	GameFastEddy->GameBase->Game->Audio->StopMusic();
	GameFastEddy->GameBase->Game->Audio->StopSound();
	GameFastEddy->GameBase->Game->Audio->UnLoadMusic(GameFastEddy->MusMusic);
	GameFastEddy->GameBase->Game->Audio->UnLoadSound(GameFastEddy->SfxDie);
	GameFastEddy->GameBase->Game->Audio->UnLoadSound(GameFastEddy->SfxSucces);
	GameFastEddy->GameBase->Game->Audio->UnLoadSound(GameFastEddy->SfxCollect);
}

void CGameFastEddy_LoadGraphics(CGameFastEddy* GameFastEddy)
{
	GameFastEddy->background = GameFastEddy->GameBase->Game->Image->LoadImage(GameFastEddy->GameBase->Game->Renderer, "fasterdave/background.png");
	GameFastEddy->spritesheet = GameFastEddy->GameBase->Game->Image->LoadImage(GameFastEddy->GameBase->Game->Renderer, "fasterdave/floortileset.png",0, 128, dumpScaledBitmaps);
	GameFastEddy->spritesheetladder = GameFastEddy->GameBase->Game->Image->LoadImage(GameFastEddy->GameBase->Game->Renderer, "fasterdave/ladder.png",0, 128, dumpScaledBitmaps);
	GameFastEddy->spritesheetplayerclimb = GameFastEddy->GameBase->Game->Image->LoadImage(GameFastEddy->GameBase->Game->Renderer, "fasterdave/Character_character_climb.png",0, 128, dumpScaledBitmaps);
	GameFastEddy->spritesheetplayerrun = GameFastEddy->GameBase->Game->Image->LoadImage(GameFastEddy->GameBase->Game->Renderer, "fasterdave/Character_character_run.png",0, 128, dumpScaledBitmaps);
	GameFastEddy->spritesheetplayeridle = GameFastEddy->GameBase->Game->Image->LoadImage(GameFastEddy->GameBase->Game->Renderer, "fasterdave/Character_character_idle.png",0, 128, dumpScaledBitmaps);
	GameFastEddy->spritesheetplayerjump = GameFastEddy->GameBase->Game->Image->LoadImage(GameFastEddy->GameBase->Game->Renderer, "fasterdave/Character_character_jump_up.png",0, 128, dumpScaledBitmaps);
	GameFastEddy->spritesheetenemy = GameFastEddy->GameBase->Game->Image->LoadImage(GameFastEddy->GameBase->Game->Renderer, "fasterdave/enemy.png",0, 128, dumpScaledBitmaps);
	GameFastEddy->spritesheetcollectable = GameFastEddy->GameBase->Game->Image->LoadImage(GameFastEddy->GameBase->Game->Renderer, "fasterdave/orbs.png",0, 128, dumpScaledBitmaps);
	GameFastEddy->spritesheetkey = GameFastEddy->GameBase->Game->Image->LoadImage(GameFastEddy->GameBase->Game->Renderer, "fasterdave/key.png",0, 128, dumpScaledBitmaps);

    // SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/fasterdave/ladder.bmp", Game->Image->GetImage(GameFastEddy->spritesheetladder), 1,1, true,0, 128);
	// SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/fasterdave/floortileset.bmp", Game->Image->GetImage(GameFastEddy->spritesheet), 1,1, true,0, 128);
	// SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/fasterdave/Character_character_climb.bmp", Game->Image->GetImage(GameFastEddy->spritesheetplayerclimb), 1,1, true,0, 128);
	// SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/fasterdave/Character_character_run.bmp", Game->Image->GetImage(GameFastEddy->spritesheetplayerrun), 1,1, true,0, 128);
	// SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/fasterdave/Character_character_idle.bmp", Game->Image->GetImage(GameFastEddy->spritesheetplayeridle), 1,1, true,0, 128);
	// SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/fasterdave/Character_character_jump_up.bmp", Game->Image->GetImage(GameFastEddy->spritesheetplayerjump), 1,1, true,0, 128);
	// SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/fasterdave/enemy.bmp", Game->Image->GetImage(GameFastEddy->spritesheetenemy), 1,1, true, 0, 128);
	// SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/fasterdave/orbs.bmp", Game->Image->GetImage(GameFastEddy->spritesheetcollectable), 1,1, true,0, 128);
	// SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/fasterdave/GameFastEddy->key.bmp", Game->Image->GetImage(GameFastEddy->spritesheetkey), 1,1, true,0, 128);
	
	if(!useDefaultColorAssets)
	{
		GameFastEddy->UnloadGraphics(GameFastEddy);
		GameFastEddy->background = GameFastEddy->GameBase->Game->Image->LoadImage(GameFastEddy->GameBase->Game->Renderer, "fasterdave/background.png");	
		GameFastEddy->spritesheetladder = GameFastEddy->GameBase->Game->Image->LoadImage(GameFastEddy->GameBase->Game->Renderer, "fasterdave/ladder.bmp");
		GameFastEddy->spritesheet = GameFastEddy->GameBase->Game->Image->LoadImage(GameFastEddy->GameBase->Game->Renderer, "fasterdave/floortileset.bmp");
		GameFastEddy->spritesheetplayerclimb = GameFastEddy->GameBase->Game->Image->LoadImage(GameFastEddy->GameBase->Game->Renderer, "fasterdave/Character_character_climb.bmp");
		GameFastEddy->spritesheetplayerrun = GameFastEddy->GameBase->Game->Image->LoadImage(GameFastEddy->GameBase->Game->Renderer, "fasterdave/Character_character_run.bmp");
		GameFastEddy->spritesheetplayeridle = GameFastEddy->GameBase->Game->Image->LoadImage(GameFastEddy->GameBase->Game->Renderer, "fasterdave/Character_character_idle.bmp");
		GameFastEddy->spritesheetplayerjump = GameFastEddy->GameBase->Game->Image->LoadImage(GameFastEddy->GameBase->Game->Renderer, "fasterdave/Character_character_jump_up.bmp");
		GameFastEddy->spritesheetenemy = GameFastEddy->GameBase->Game->Image->LoadImage(GameFastEddy->GameBase->Game->Renderer, "fasterdave/enemy.bmp");
		GameFastEddy->spritesheetcollectable = GameFastEddy->GameBase->Game->Image->LoadImage(GameFastEddy->GameBase->Game->Renderer, "fasterdave/orbs.bmp");
		GameFastEddy->spritesheetkey = GameFastEddy->GameBase->Game->Image->LoadImage(GameFastEddy->GameBase->Game->Renderer, "fasterdave/key.bmp");
	}

	

}

void CGameFastEddy_UnloadGraphics(CGameFastEddy* GameFastEddy)
{
	GameFastEddy->GameBase->Game->Image->UnLoadImage(GameFastEddy->background);
	GameFastEddy->GameBase->Game->Image->UnLoadImage(GameFastEddy->spritesheet);
	GameFastEddy->GameBase->Game->Image->UnLoadImage(GameFastEddy->spritesheetladder);
	GameFastEddy->GameBase->Game->Image->UnLoadImage(GameFastEddy->spritesheetplayerclimb);
	GameFastEddy->GameBase->Game->Image->UnLoadImage(GameFastEddy->spritesheetplayerrun);
	GameFastEddy->GameBase->Game->Image->UnLoadImage(GameFastEddy->spritesheetplayeridle);
	GameFastEddy->GameBase->Game->Image->UnLoadImage(GameFastEddy->spritesheetplayerjump);
	GameFastEddy->GameBase->Game->Image->UnLoadImage(GameFastEddy->spritesheetenemy);
	GameFastEddy->GameBase->Game->Image->UnLoadImage(GameFastEddy->spritesheetcollectable);
	GameFastEddy->GameBase->Game->Image->UnLoadImage(GameFastEddy->spritesheetkey);
}

//Update ----------------------------------------------------------------------------------------------------------------

void CGameFastEddy_UpdateObjects(CGameFastEddy* GameFastEddy, bool IsGameState)
{
	if (IsGameState)
	{
		GameFastEddy->updateplayer(GameFastEddy);
		GameFastEddy->updateenemies(GameFastEddy);
		GameFastEddy->updatecollectables(GameFastEddy);
		GameFastEddy->updatekey(GameFastEddy);
	}
}

void CGameFastEddy_UpdateLogic(CGameFastEddy* GameFastEddy)
{
	GameFastEddy->GameBase->UpdateLogic(GameFastEddy->GameBase);
	GameFastEddy->UpdateObjects(GameFastEddy, GameFastEddy->GameBase->Game->SubGameState == SGGame);
	if(GameFastEddy->GameBase->Game->SubGameState == SGGame)
		GameFastEddy->GameBase->Game->Sprites->UpdateSprites(GameFastEddy->GameBase->Game->Renderer);
}

void CGameFastEddy_Draw(CGameFastEddy* GameFastEddy)
{
	GameFastEddy->DrawBackground(GameFastEddy);
	GameFastEddy->GameBase->Game->Sprites->DrawSprites(GameFastEddy->GameBase->Game->Renderer);
	GameFastEddy->GameBase->DrawScoreBar(GameFastEddy->GameBase);
	GameFastEddy->GameBase->DrawSubStateText(GameFastEddy->GameBase);
}