#include <SDL.h>
#include <string>
#include <iostream>
#include <cmath>
#include "CGameFastEddy.h"
#include "../CGame.h"
#include "../Common.h"
#include "../Vec2F.h"

using namespace std;

CGameFastEddy::CGameFastEddy(CGame* aGame)
{
	GameBase = new CGameBase(aGame, GSEddy, true);

	MusMusic = -1;
	SfxSucces = -1;
	SfxDie = -1;

	GameBase->playfieldwidth = ScreenWidth;
	GameBase->playfieldheight = ScreenHeight;
	GameBase->screenleft = (ScreenWidth - GameBase->playfieldwidth) / 2;
	GameBase->screenright = GameBase->screenleft + GameBase->playfieldwidth;
	GameBase->screentop = (ScreenHeight - GameBase->playfieldheight) / 2;
	GameBase->screenbottom = GameBase->screentop + GameBase->playfieldheight;
	numfloortilesperrow = GameBase->playfieldwidth / rowfloorsizex;
	maxfloortiles = numfloortilesperrow * rows;
	rowspacingsize = GameBase->playfieldheight / rows;
	ladderwidth = GameBase->playfieldwidth / laddersfitrows;
	rowsize = rowspacingsize - rowfloorsizey;
	playerheight = rowsize * 3 / 4;
	enemyheight = rowsize * 2 / 5;
	playerjumpheight = rowsize * 2/ 4;
	collectableheight = rowsize / 2;
	keyheight = rowsize / 4;
	playerjumpspeeddec = playerjumpspeed / playerjumpheight;
}

CGameFastEddy::~CGameFastEddy()
{

}



//end level key ----------------------------------------------------------------------------------------------------------------

void CGameFastEddy::createkey()
{
	key.spr = GameBase->Game->Sprites->CreateSprite();
	key.alive = true;
	SDL_Point tz = GameBase->Game->Image->ImageSize(spritesheetkey);
	Vec2F scale = {keyheight / tz.y, keyheight / tz.y};
	key.tz.x = tz.x * scale.x;
	key.tz.y = tz.y * scale.y;
	GameBase->Game->Sprites->SetSpriteImage(GameBase->Game->Renderer,key.spr, &spritesheetkey, 1, 1);
	GameBase->Game->Sprites->SetSpriteScale(GameBase->Game->Renderer,key.spr, scale);
	GameBase->Game->Sprites->SetSpriteAnimation(key.spr, 0, 0, 0);
	GameBase->Game->Sprites->SetSpriteCollisionShape(key.spr, SHAPE_BOX, tz.x - 20, tz.y, 0, 0, 0);
	key.pos.y = enemies[rowzeroenemyindex].pos.y - enemies[rowzeroenemyindex].tz.y;
	key.pos.x = enemies[rowzeroenemyindex].pos.x;
	GameBase->Game->Sprites->SetSpriteLocation(key.spr, key.pos);
}

void CGameFastEddy::destroykey()
{
	if (key.alive)
	{
		GameBase->Game->Sprites->RemoveSprite(key.spr);
		key.alive = false;
	}
}

void CGameFastEddy::updatekey()
{
	if (key.alive)
	{
		key.pos.x = enemies[rowzeroenemyindex].pos.x;
		key.pos.y = enemies[rowzeroenemyindex].pos.y - enemies[rowzeroenemyindex].tz.y;
		GameBase->Game->Sprites->SetSpriteLocation(key.spr, key.pos);

		if (GameBase->Game->Sprites->DetectSpriteCollision(key.spr, player.spr))
		{
			GameBase->Game->Audio->PlaySound(SfxSucces, 0);
			GameBase->Game->AddToScore(200);
			SDL_Delay(500);
			GameBase->level += 1;
			collecteditems = 0;
			collectedcreated = 0;
			destroyenemies();
			destroyplayer();
			destroyallcollectables();
			destroykey();
			destroyladders();
			createladders();
			createplayer();
			createenemies(true);
			createcollectables(-1);
			createkey();
		}
	}
}

//collectables ----------------------------------------------------------------------------------------------------------------

void CGameFastEddy::destroyallcollectables()
{
	for (int i = 0; i < maxcollectables; i++)
		destroycollectable(i);
}

void CGameFastEddy::destroycollectable(int index)
{
	if (collectables[index].alive)
	{
		GameBase->Game->Sprites->RemoveSprite(collectables[index].spr);
		collectables[index].alive = false;
	}
}

void CGameFastEddy::createcollectables(int ignorerow)
{
	SDL_Point tz = {128, 128};
	for (int i = 0; i < maxcollectables; i++)
	{
		if(collectedcreated < 10)
		{
			if (!collectables[i].alive)
			{
				bool bok = false;
				int row = ignorerow;
				while (!bok)
				{
					row = 1 + (rand() % (rows-1));
					while(row == ignorerow)
						row = 1 + (rand() % (rows-1));
					bool bnocollision = true;
					for(int j = 0; j < maxcollectables; j++)
					{
						if(collectables[j].alive)
						{
							if(collectables[j].row == row)
							{
								bnocollision = false;
								break;
							}
						}
					}
					bok = bnocollision;
				}

				collectables[i].spr = GameBase->Game->Sprites->CreateSprite();
				collectables[i].alive = true;
				collectables[i].row = row;
				collectables[i].state = rand() % 3;
				Vec2F scale = {collectableheight / tz.y, collectableheight / tz.y};
				collectables[i].tz.x = tz.x * scale.x;
				collectables[i].tz.y = tz.y * scale.y;
				GameBase->Game->Sprites->SetSpriteImage(GameBase->Game->Renderer,collectables[i].spr, &spritesheetcollectable, 5, 1);
				GameBase->Game->Sprites->SetSpriteScale(GameBase->Game->Renderer,collectables[i].spr, scale);
				GameBase->Game->Sprites->SetSpriteAnimation(collectables[i].spr, ((GameBase->level-1) % 5), ((GameBase->level-1) % 5), 0);
				GameBase->Game->Sprites->SetSpriteCollisionShape(collectables[i].spr, SHAPE_CIRCLE, tz.x - 66, tz.y - 66, 0, 0, 0);
				//Game->Sprites->SetSpriteCollisionShape(collectables[i].spr, SHAPE_BOX, tz.x - 66, tz.y - 66, 0, 0, 0);
				collectables[i].pos.y = (row) * rowspacingsize + rowfloorsizey / 2;
				collectables[i].pos.x = ((GameBase->screenright - GameBase->screenleft) /7) +
					(rand() % (GameBase->screenright - GameBase->screenleft - ((GameBase->screenright - GameBase->screenleft) /6 )));
				GameBase->Game->Sprites->SetSpriteLocation(collectables[i].spr, collectables[i].pos);
				collectedcreated += 1;
			}
		}
	}
}

void CGameFastEddy::updatecollectables()
{
	for(int i = 0; i < maxcollectables; i++)
	{
		if (collectables[i].alive)
		{
			if (collectables[i].state == collectablestatemoveleft)
			{
				collectables[i].pos.x -= collectablespeed;
				if (collectables[i].pos.x + collectables[i].tz.x / 2 < GameBase->screenleft)
					collectables[i].pos.x = GameBase->screenright + collectables[i].tz.x / 2;
				GameBase->Game->Sprites->SetSpriteLocation(collectables[i].spr, collectables[i].pos);
			}

			if (collectables[i].state == collectablestatemoveright)
			{
				collectables[i].pos.x += collectablespeed;
				if(collectables[i].pos.x - collectables[i].tz.x / 2 > GameBase->screenright)
					collectables[i].pos.x = GameBase->screenleft - collectables[i].tz.x / 2;
				GameBase->Game->Sprites->SetSpriteLocation(collectables[i].spr, collectables[i].pos);
			}

			if (GameBase->Game->Sprites->DetectSpriteCollision(collectables[i].spr, player.spr))
			{
				GameBase->Game->Audio->PlaySound(SfxCollect, 0);
				int ignorerow = collectables[i].row;
				destroycollectable(i);
				collecteditems += 1;
				GameBase->Game->AddToScore(collecteditems * 10);
				if(collecteditems >= 9)
					enemyenablelevelend();
				createcollectables(ignorerow);
			}
		}
	}
}

//enemies ----------------------------------------------------------------------------------------------------------------

void CGameFastEddy::destroyenemies()
{
	for(int i = 0; i < maxenemies; i++)
	{
		if (enemies[i].alive)
		{
			GameBase->Game->Sprites->RemoveSprite(enemies[i].spr);
			enemies[i].alive = false;
		}
	}
}

void CGameFastEddy::enemyenablelevelend()
{
	int index = rowzeroenemyindex;
	SDL_Point tz = {32, 32};
	Vec2F scale = {enemyheight / tz.y, enemyheight / tz.y};
	enemies[index].tz.x = tz.x * scale.x;
	enemies[index].tz.y = tz.y * scale.y;
	enemies[index].pos.y = (0 + 1) * rowspacingsize - rowfloorsizey / 2 - enemies[index].tz.y / 2;
	GameBase->Game->Sprites->SetSpriteScale(GameBase->Game->Renderer,enemies[index].spr, scale);
	GameBase->Game->Sprites->SetSpriteCollisionShape(enemies[index].spr, SHAPE_BOX, tz.x - 10, tz.y-10,0, 0, 0);
	GameBase->Game->Sprites->SetSpriteLocation(enemies[index].spr, enemies[index].pos);
}

void CGameFastEddy::createenemy(int row, float x, int state, int group, int multiply)
{
	SDL_Point tz = {32, 32};
	for (int i = 0; i < maxenemies; i++)
	{
		if (!enemies[i].alive)
		{
			if(row == 0)
				rowzeroenemyindex = i;
			enemies[i].spr = GameBase->Game->Sprites->CreateSprite();
			enemies[i].alive = true;
			enemies[i].row = row;
			enemies[i].group = group;
			enemies[i].state = state;
			Vec2F scale = {enemyheight / tz.y * multiply, enemyheight / tz.y * multiply};
			enemies[i].tz.x = tz.x * scale.x;
			enemies[i].tz.y = tz.y * scale.y;
			GameBase->Game->Sprites->SetSpriteImage(GameBase->Game->Renderer,enemies[i].spr, &spritesheetenemy, 3, 4);
			GameBase->Game->Sprites->SetSpriteScale(GameBase->Game->Renderer,enemies[i].spr, scale);
			if (enemies[i].state == enemystatemoveright)
				GameBase->Game->Sprites->SetSpriteAnimation(enemies[i].spr, 6, 8, 10);

			if (enemies[i].state == enemystateidle)
				GameBase->Game->Sprites->SetSpriteAnimation(enemies[i].spr, 0, 2, 10);

			if (enemies[i].state == enemystatemoveleft)
				GameBase->Game->Sprites->SetSpriteAnimation(enemies[i].spr, 3, 5, 10);

			if (enemies[i].state == enemystatewaitmove)
			{
				enemies[i].stateticks = 60 * 6;
				GameBase->Game->Sprites->SetSpriteAnimation(enemies[i].spr, 0, 2, 10);
			}

			GameBase->Game->Sprites->SetSpriteCollisionShape(enemies[i].spr, SHAPE_BOX, tz.x - 10, tz.y-10,0, 0, 0);
			enemies[i].pos.y = (row + 1) * rowspacingsize - rowfloorsizey / 2 - enemies[i].tz.y / 2;
			enemies[i].pos.x = x;
			GameBase->Game->Sprites->SetSpriteLocation(enemies[i].spr, enemies[i].pos);
			break;
		}
	}
}

void CGameFastEddy::createenemies(bool levelsucces)
{
	if (GameBase->level-1 < 5)
	{
		createenemy(0, (GameBase->screenright - GameBase->screenleft) / 2, enemystatemoveright, 0, 2);
		if (levelsucces)
			createenemy(1, (GameBase->screenright - GameBase->screenleft) * 4 / 10, enemystatemoveright, 1, 1);
		else
			createenemy(1, (GameBase->screenright - GameBase->screenleft) / 10, enemystateidle, 1, 1);

		createenemy(2, (GameBase->screenright - GameBase->screenleft) / 2, enemystatemoveright, 2, 1);
		if (levelsucces)
			createenemy(3, (GameBase->screenright - GameBase->screenleft) * 6 / 10, enemystatemoveright, 3, 1);
		else
			createenemy(3, (GameBase->screenright - GameBase->screenleft) * 6 / 10, enemystateidle, 3, 1);

		createenemy(4, (GameBase->screenright - GameBase->screenleft) / 10, enemystatewaitmove, 4, 1);
	}
	else
	{
		if (GameBase->level-1 < 10)
		{
			createenemy(0, (GameBase->screenright - GameBase->screenleft) / 2, enemystatemoveright, 0, 2);
			createenemy(1, (GameBase->screenright - GameBase->screenleft) * 4 / 10, enemystatemoveright, 1, 1);
			createenemy(1, (GameBase->screenright - GameBase->screenleft) * 5 / 10, enemystatemoveright, 1, 1);
			createenemy(2, (GameBase->screenright - GameBase->screenleft) / 2, enemystatemoveright, 2, 1);
			createenemy(3, (GameBase->screenright - GameBase->screenleft) * 6 / 10, enemystatemoveleft, 3, 1);
			createenemy(4, (GameBase->screenright - GameBase->screenleft) / 10, enemystatewaitmove, 4, 1);
		}
		else
		{
			if (GameBase->level-1 < 15)
			{
				createenemy(0, (GameBase->screenright - GameBase->screenleft) / 2, enemystatemoveright, 0, 2);
				createenemy(1, (GameBase->screenright - GameBase->screenleft) * 4 / 10, enemystatemoveright, 1, 1);
				createenemy(1, (GameBase->screenright - GameBase->screenleft) * 5 / 10, enemystatemoveright, 1, 1);
				createenemy(2, (GameBase->screenright - GameBase->screenleft) / 2, enemystatemoveright, 2, 1);
				createenemy(3, (GameBase->screenright - GameBase->screenleft) * 6 / 10, enemystatemoveleft, 3, 1);
				createenemy(3, (GameBase->screenright - GameBase->screenleft) * 7 / 10, enemystatemoveleft, 3, 1);
				createenemy(4, (GameBase->screenright - GameBase->screenleft) / 10, enemystatewaitmove, 4, 1);
			}
			else
			{
				createenemy(0, (GameBase->screenright - GameBase->screenleft) / 2, enemystatemoveright, 0, 2);
				createenemy(1, (GameBase->screenright - GameBase->screenleft) * 4 / 10, enemystatemoveright, 1, 1);
				createenemy(1, (GameBase->screenright - GameBase->screenleft) * 5 / 10, enemystatemoveright, 1, 1);
				createenemy(2, (GameBase->screenright - GameBase->screenleft) / 2, enemystatemoveright, 2, 1);
				createenemy(3, (GameBase->screenright - GameBase->screenleft) * 1 / 10, enemystatemoveright, 3, 1);
				createenemy(3, (GameBase->screenright - GameBase->screenleft) * 6 / 10, enemystatemoveright, 4, 1);
				createenemy(4, (GameBase->screenright - GameBase->screenleft) / 10, enemystatewaitmove, 5, 1);
			}
		}
	}
}

void CGameFastEddy::updateenemies()
{
	for (int i = 0; i < maxenemies; i++)
	{
		if (enemies[i].alive)
		{
			if (enemies[i].state == enemystatewaitmove)
			{
				if (enemies[i].stateticks > 0)
					enemies[i].stateticks -= 1;
				else
				{
					enemies[i].state = enemystatemoveright;
					GameBase->Game->Sprites->SetSpriteAnimation(enemies[i].spr, 6, 8, 10);
				}
			}

			if (enemies[i].state == enemystatemoveright)
			{
				if (enemies[i].pos.x + enemies[i].tz.x / 2 + playerspeed < GameBase->screenright)
					enemies[i].pos.x += playerspeed;
				else
				{
					for(int j = 0; j < maxenemies; j++)
					{
						if (enemies[j].alive)
						{
							if(enemies[i].group == enemies[j].group)
							{
								enemies[j].state = enemystatemoveleft;
								GameBase->Game->Sprites->SetSpriteAnimation(enemies[j].spr, 3, 5, 10);
							}
						}
					}
				}
			}

			if(enemies[i].state == enemystatemoveleft)
			{
				if ( enemies[i].pos.x - enemies[i].tz.x / 2 - playerspeed > GameBase->screenleft)
					enemies[i].pos.x -= playerspeed;
				else
				{
					for(int j = 0; j < maxenemies; j++)
					{
						if(enemies[j].alive)
						{
							if(enemies[i].group == enemies[j].group)
							{
								enemies[j].state = enemystatemoveright;
								GameBase->Game->Sprites->SetSpriteAnimation(enemies[j].spr, 6, 8, 10);
							}
						}
					}
				}
			}

			GameBase->Game->Sprites->SetSpriteLocation(enemies[i].spr, enemies[i].pos);

			if (GameBase->Game->Sprites->DetectSpriteCollision(enemies[i].spr, player.spr))
			{
				if (((player.state != playerstateclimbup) &&
					(player.state != playerstateclimbdown) &&
					(player.state != playerstateunknown)) ||
					(enemies[i].row == 0))
				{
					GameBase->Game->Audio->PlaySound(SfxDie, 0);
					if (GameBase->Game->GameMode == GMGame)
						GameBase->HealthPoints -= 1;

					if (GameBase->HealthPoints > 0)
					{
						GameBase->Game->AddToScore(-100);
						SDL_Delay(500);
						destroyenemies();
						destroyplayer();
						destroykey();
						createplayer();
						createenemies(false);
						createkey();
						if (collecteditems >= 9)
							enemyenablelevelend();
					}
					break;
				}
			}
		}
	}
}

//player ----------------------------------------------------------------------------------------------------------------

void CGameFastEddy::destroyplayer()
{
	if (player.alive)
	{
		GameBase->Game->Sprites->RemoveSprite(player.spr);
		player.alive = false;
	}
}

void CGameFastEddy::createplayer()
{
	player.spr = GameBase->Game->Sprites->CreateSprite();
	player.alive = true;
	SDL_Point tz = {238, 342};
	Vec2F scale = {playerheight / tz.y, playerheight / tz.y};
	player.tz.x = tz.x * scale.x;
	player.tz.y = tz.y * scale.y;
	player.state = playerstateunknown;
	GameBase->Game->Sprites->SetSpriteDepth(player.spr, 5);
	GameBase->Game->Sprites->SetSpriteImage(GameBase->Game->Renderer,player.spr, &spritesheetplayeridle, 1, 15);
	GameBase->Game->Sprites->SetSpriteScale(GameBase->Game->Renderer,player.spr, scale);
	GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 0, 14, 10);
	GameBase->Game->Sprites->SetSpriteCollisionShape(player.spr, SHAPE_BOX, tz.x - 120, tz.y-30,0,0,0);
	player.pos = { (float)(GameBase->screenright - GameBase->screenleft) / 2, 5 * rowspacingsize - rowfloorsizey / 2 - player.tz.y / 2};
	GameBase->Game->Sprites->SetSpriteLocation(player.spr, player.pos);
}

void CGameFastEddy::updateplayer()
{
	bool jump = false;
	bool left = false;
	bool right = false;
	bool down = false;
	bool up = false;
	if (player.alive)
	{
		if ((player.state != playerstatejump) &&
			(player.state != playerstateleftjump) &&
			(player.state != playerstaterightjump) &&
			(player.state != playerstateclimbup) &&
			(player.state != playerstateclimbdown))
		{

			//if fedebugmode and (!global.prevc.b and global.c.b) then
			//	feenemyenablelevelend()
			//endif

			if (!GameBase->Game->Input->PrevButtons.ButA && GameBase->Game->Input->Buttons.ButA)
				jump = true;

			if ((GameBase->Game->Input->Buttons.ButUp) ||
				(GameBase->Game->Input->Buttons.ButUp2) ||
				(GameBase->Game->Input->Buttons.ButDpadUp))
				up = true;

			if ((GameBase->Game->Input->Buttons.ButDown) ||
				(GameBase->Game->Input->Buttons.ButDown2) ||
				(GameBase->Game->Input->Buttons.ButDpadDown))
				down = true;

			if ((GameBase->Game->Input->Buttons.ButLeft) ||
				(GameBase->Game->Input->Buttons.ButLeft2) ||
				(GameBase->Game->Input->Buttons.ButDpadLeft))
				left = true;

			if ((GameBase->Game->Input->Buttons.ButRight) ||
				(GameBase->Game->Input->Buttons.ButRight2) ||
				(GameBase->Game->Input->Buttons.ButDpadRight))
				right = true;

			if (jump)
			{
				if (left)
				{
					if (player.state != playerstateleftjump)
					{
						GameBase->Game->Sprites->SetSpriteImage(GameBase->Game->Renderer,player.spr, &spritesheetplayerjump, 1, 6);
						GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 3, 3, 10);

						player.state = playerstateleftjump;
						player.floory = player.pos.y;
						player.jumpdown = false;
						player.force = playerjumpspeed;
					}
				}
				else
				{
					if (right)
					{
						if (player.state != playerstaterightjump)
						{
							GameBase->Game->Sprites->SetSpriteImage(GameBase->Game->Renderer,player.spr, &spritesheetplayerjump, 1, 6);
							GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 3, 3, 10);

							player.floory = player.pos.y;
							player.state = playerstaterightjump;
							player.jumpdown = false;
							player.force = playerjumpspeed;
						}
					}
					else
					{
						if(player.state != playerstatejump)
						{
							GameBase->Game->Sprites->SetSpriteImage(GameBase->Game->Renderer,player.spr, &spritesheetplayerjump, 1, 6);
							GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 3, 3, 10);
							player.floory = player.pos.y;
							player.state = playerstatejump;
							player.jumpdown = false;
							player.force = playerjumpspeed;
						}
					}
				}
			}
			else
			{
				if(up)
				{
					player.pos.y -= rowspacingsize/4;
					GameBase->Game->Sprites->SetSpriteLocation(player.spr, player.pos);
					bool found = false;
					for (int i = 0; i < maxladders; i++)
					{
						if (GameBase->Game->Sprites->DetectSpriteCollision(player.spr, ladders[i].spr))
						{
							GameBase->Game->Sprites->SetSpriteImage(GameBase->Game->Renderer,player.spr, &spritesheetplayerclimb, 1, 15);
							GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 0, 14, 20);

							player.pos.x = ladders[i].pos.x;
							player.state = playerstateclimbup;
							player.floory = player.pos.y + rowspacingsize/4;
							found = true;
							break;
						};
					};
					player.pos.y += rowspacingsize/4;
					GameBase->Game->Sprites->SetSpriteLocation(player.spr, player.pos);
					if (!found)
					{
						if (player.state != playerstateidle)
						{
							GameBase->Game->Sprites->SetSpriteImage(GameBase->Game->Renderer,player.spr, &spritesheetplayeridle, 1, 15);
							GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 0, 14, 10);
							player.state = playerstateidle;
						}
					}
				}
				else
				{
					if (down)
					{
						player.pos.y += rowspacingsize - playerheight / 4;
						GameBase->Game->Sprites->SetSpriteLocation(player.spr, player.pos);
						bool found = false;
						for (int i = 0; i < maxladders; i++)
						{
							if (GameBase->Game->Sprites->DetectSpriteCollision(player.spr, ladders[i].spr))
							{
								GameBase->Game->Sprites->SetSpriteImage(GameBase->Game->Renderer,player.spr, &spritesheetplayerclimb, 1, 15);
								GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 14, 0, 20);

								player.pos.x = ladders[i].pos.x;
								player.state = playerstateclimbdown;
								player.floory = player.pos.y - rowspacingsize + playerheight / 4;
								found = true;
								break;
							};
						};
						player.pos.y -= rowspacingsize - playerheight / 4;
						GameBase->Game->Sprites->SetSpriteLocation(player.spr, player.pos);
						if (!found)
						{
							if (player.state != playerstateidle)
							{
								GameBase->Game->Sprites->SetSpriteImage(GameBase->Game->Renderer,player.spr, &spritesheetplayeridle, 1, 15);
								GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 0, 14, 10);
								player.state = playerstateidle;
							}
						}
					}
					else
					{
						if (left)
						{
							if (player.state != playerstateleft)
							{
								GameBase->Game->Sprites->SetSpriteImage(GameBase->Game->Renderer,player.spr, &spritesheetplayerrun, 1, 7);
								GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 0, 6, 10);
								player.spr->xscale = -(abs(player.spr->xscale));
								player.state = playerstateleft;
							}
						}
						else
						{
							if (right)
							{
								if (player.state != playerstateright)
								{
									GameBase->Game->Sprites->SetSpriteImage(GameBase->Game->Renderer,player.spr, &spritesheetplayerrun, 1, 7);
									GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 0, 6, 10);
									player.spr->xscale = (abs(player.spr->xscale));
									player.state = playerstateright;
								}
							}
							else
							{
								if (player.state != playerstateidle)
								{
									GameBase->Game->Sprites->SetSpriteImage(GameBase->Game->Renderer,player.spr, &spritesheetplayeridle, 1, 15);
									GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 0, 14, 10);
									player.state = playerstateidle;
								}
							}
						}
					}
				}
			}
		}

		if (player.state == playerstateclimbup)
		{
			if (player.pos.y > player.floory - rowspacingsize)
				player.pos.y -= playerclimbspeed;
			else
			{
				player.pos.y = player.floory - rowspacingsize;
				player.state = playerstateunknown;
			}

			GameBase->Game->Sprites->SetSpriteLocation(player.spr, player.pos);
		}

		if (player.state == playerstateclimbdown)
		{
			if (player.pos.y < player.floory + rowspacingsize)
				player.pos.y += playerclimbspeed;
			else
			{
				player.pos.y = player.floory + rowspacingsize;
				player.state = playerstateunknown;
			}
			GameBase->Game->Sprites->SetSpriteLocation(player.spr, player.pos);
		};

		if (player.state == playerstatejump)
		{
			if (!player.jumpdown)
			{
				if (player.pos.y > player.floory - playerjumpheight)
				{
					player.pos.y -= player.force;
					player.force -= playerjumpspeeddec;
				}
				else
					player.jumpdown = true;
			}
			else
			{
				if (player.pos.y < player.floory)
				{
					player.pos.y += player.force;
					player.force += playerjumpspeeddec;
				}
				else
				{
					player.pos.y = player.floory;
					player.state = playerstateunknown;
				}
			}
			GameBase->Game->Sprites->SetSpriteLocation(player.spr, player.pos);
			if (player.pos.y < player.floory - 5/6 * playerjumpheight)
				GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 5, 5, 0);
			else
			{
				if (player.pos.y < player.floory - 4/6 * playerjumpheight)
					GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 4, 4, 0);
				else
				{
					if(player.pos.y < player.floory - 3/6 * playerjumpheight)
						GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 3, 3, 0);
					else
					{
						if(player.pos.y < player.floory - 2/6 * playerjumpheight)
							GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 2, 2, 0);
						else
						{
							if(player.pos.y < player.floory - 1/6 * playerjumpheight)
								GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 1, 1, 0);
							else
							{
								if (player.pos.y < player.floory - 0/6 * playerjumpheight)
									GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 0, 0, 0);
							}
						}
					}
				}
			}
		}

		if ((player.state == playerstateleftjump) ||
			(player.state == playerstaterightjump))
		{
			if (! player.jumpdown)
			{
				if (player.pos.y > player.floory - playerjumpheight)
				{
					player.pos.y -= player.force;
					player.force -= playerjumpspeeddec;
				}
				else
					player.jumpdown = true;
			}
			else
			{
				if (player.pos.y < player.floory)
				{
					player.pos.y += player.force;
					player.force += playerjumpspeeddec;
				}
				else
				{
					player.pos.y = player.floory;
					player.state = playerstateunknown;
				}
			}

			if (player.spr->xscale / abs(player.spr->xscale) == 1)
			{
				if ( player.pos.x + player.tz.x / 2 + playerspeed < GameBase->screenright)
					player.pos.x += playerspeed ;
				else
					player.pos.x = GameBase->screenright - player.tz.x / 2;
			}
			else
			{
				if ( player.pos.x - player.tz.x / 2 - playerspeed > GameBase->screenleft)
					player.pos.x -= playerspeed;
				else
					player.pos.x = GameBase->screenleft + player.tz.x / 2;
			}

			if (player.pos.y < player.floory - 5/6 * playerjumpheight)
				GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 5, 5, 0);
			else
			{
				if (player.pos.y < player.floory - 4/6 * playerjumpheight)
					GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 4, 4, 0);
				else
				{
					if (player.pos.y < player.floory - 3/6 * playerjumpheight)
						GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 3, 3, 0);
					else
					{
						if (player.pos.y < player.floory - 2/6 * playerjumpheight)
							GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 2, 2, 0);
						else
						{
							if(player.pos.y < player.floory - 1/6 * playerjumpheight)
								GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 1, 1, 0);
							else
							{
								if(player.pos.y < player.floory - 0/6 * playerjumpheight)
									GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 0, 0, 0);
							}
						}
					}
				}
			}

			GameBase->Game->Sprites->SetSpriteLocation(player.spr, player.pos);
		}

		if (player.state == playerstateright)
		{
			if ( player.pos.x + player.tz.x / 2 + playerspeed < GameBase->screenright)
				player.pos.x += playerspeed;
			else
				player.pos.x = GameBase->screenright - player.tz.x / 2;
			GameBase->Game->Sprites->SetSpriteLocation(player.spr, player.pos);
		}

		if (player.state == playerstateleft)
		{
			if ( player.pos.x - player.tz.x / 2 - playerspeed > GameBase->screenleft)
				player.pos.x -= playerspeed;
			else
				player.pos.x = GameBase->screenleft + player.tz.x / 2;
			GameBase->Game->Sprites->SetSpriteLocation(player.spr, player.pos);
		}
	}
}



//floors ----------------------------------------------------------------------------------------------------------------

void CGameFastEddy::destroyfloors()
{
	for (int i = 0; i < maxfloortiles; i++)
		GameBase->Game->Sprites->RemoveSprite(floors[i].spr);
}

void CGameFastEddy::createfloors()
{
	int tilenr = 0;
	int i = 0;
	//SDL_Point tz = {64,64};
	SDL_Point tz = GameBase->Game->Image->ImageSize(spritesheet);
	tz.x /= 9*4;
	tz.y /= 10/2;
	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < numfloortilesperrow; x++)
		{
			floors[i].spr = GameBase->Game->Sprites->CreateSprite();
			floors[i].alive = true;
			GameBase->Game->Sprites->SetSpriteImage(GameBase->Game->Renderer,floors[i].spr, &spritesheet, 9, 10);
			GameBase->Game->Sprites->SetSpriteLocation(floors[i].spr, {(float)x * rowfloorsizex + rowfloorsizex / 2, (float)(y + 1) * rowspacingsize - rowfloorsizey / 2});
			GameBase->Game->Sprites->SetSpriteScale(GameBase->Game->Renderer,floors[i].spr, {(float)rowfloorsizex / tz.x , (float)rowfloorsizey / tz.y});
			if (x == 0)
				tilenr = 36;
			else
			{
				if (x == numfloortilesperrow -1)
					tilenr = 38;
				else
					tilenr = 37;
			}
			GameBase->Game->Sprites->SetSpriteAnimation(floors[i].spr, tilenr, tilenr, 0);
			i += 1;
		}
	}
}

//ladders ----------------------------------------------------------------------------------------------------------------

void CGameFastEddy::destroyladders()
{
	for (int i = 0; i < maxladders; i++)
		GameBase->Game->Sprites->RemoveSprite(ladders[i].spr);
}

void CGameFastEddy::createladders()
{
	int i = 0;
	SDL_Point tz = GameBase->Game->Image->ImageSize(spritesheetladder);
	for (int y = 1; y < rows; y++)
	{
		int x1 = 1 + rand() % (int(laddersfitrows / 2)-1);
		ladders[i].spr = GameBase->Game->Sprites->CreateSprite();
		ladders[i].alive = true;
		ladders[i].pos = {x1 * ladderwidth + ladderwidth / 2 , y * rowspacingsize + rowspacingsize / 2 - rowfloorsizey / 2};
		GameBase->Game->Sprites->SetSpriteImage(GameBase->Game->Renderer,ladders[i].spr, &spritesheetladder);
		GameBase->Game->Sprites->SetSpriteCollisionShape(ladders[i].spr, SHAPE_BOX, tz.x - 20, tz.y-10,0,0,0);

		GameBase->Game->Sprites->SetSpriteLocation(ladders[i].spr, ladders[i].pos );
		GameBase->Game->Sprites->SetSpriteScale(GameBase->Game->Renderer,ladders[i].spr, {ladderwidth / tz.x , (rowspacingsize + rowfloorsizey/2) / tz.y});

		i += 1;
		int x2 = x1 + int(laddersfitrows / 2);
		ladders[i].spr = GameBase->Game->Sprites->CreateSprite();
		ladders[i].alive = true;
		ladders[i].pos = {x2 * ladderwidth + ladderwidth / 2 , y * rowspacingsize + rowspacingsize / 2 - rowfloorsizey / 2};
		GameBase->Game->Sprites->SetSpriteImage(GameBase->Game->Renderer,ladders[i].spr, &spritesheetladder);
		GameBase->Game->Sprites->SetSpriteCollisionShape(ladders[i].spr, SHAPE_BOX, tz.x - 20, tz.y-10,0,0,0);
		GameBase->Game->Sprites->SetSpriteLocation(ladders[i].spr, ladders[i].pos );
		GameBase->Game->Sprites->SetSpriteScale(GameBase->Game->Renderer,ladders[i].spr, {ladderwidth / tz.x , (rowspacingsize + rowfloorsizey/2) / tz.y});
		i += 1;
	}
}

//background ----------------------------------------------------------------------------------------------------------------

void CGameFastEddy::DrawBackground(bool motionblur)
{
	GameBase->Game->Image->DrawImage(GameBase->Game->Renderer, background, NULL, NULL);
}

//init - deinit ----------------------------------------------------------------------------------------------------------------

void CGameFastEddy::init()
{
	collecteditems = 0;
	collectedcreated = 0;
	GameBase->level = 1;
	GameBase->HealthPoints = 3;
	LoadGraphics();
	createfloors();
	createladders();
	createenemies(false);
	createcollectables(-1);
	createkey();
	createplayer();
	LoadSound();
	GameBase->Game->CurrentGameMusicID = MusMusic;
	GameBase->Game->Audio->PlayMusic(MusMusic, -1);
}

void CGameFastEddy::deinit()
{
	destroyfloors();
	destroyladders();
	destroyenemies();
	destroyallcollectables();
	destroykey();
	destroyplayer();
	UnLoadSound();
	GameBase->Game->SubStateCounter = 0;
	GameBase->Game->SubGameState = SGNone;
	GameBase->Game->CurrentGameMusicID = -1;
	UnloadGraphics();
}

void CGameFastEddy::LoadSound()
{
	SfxDie = GameBase->Game->Audio->LoadSound("common/die.wav");
	SfxSucces = GameBase->Game->Audio->LoadSound("common/succes.wav");
	SfxCollect = GameBase->Game->Audio->LoadSound("common/coin.wav");
	MusMusic = GameBase->Game->Audio->LoadMusic("fasterdave/music.ogg");
}

void CGameFastEddy::UnLoadSound()
{
	GameBase->Game->Audio->StopMusic();
	GameBase->Game->Audio->StopSound();
	GameBase->Game->Audio->UnLoadMusic(MusMusic);
	GameBase->Game->Audio->UnLoadSound(SfxDie);
	GameBase->Game->Audio->UnLoadSound(SfxSucces);
	GameBase->Game->Audio->UnLoadSound(SfxCollect);
}

void CGameFastEddy::LoadGraphics()
{
	background = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "fasterdave/background.png");
	spritesheet = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "fasterdave/floortileset.png",0, 128, dumpScaledBitmaps);
	spritesheetladder = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "fasterdave/ladder.png",0, 128, dumpScaledBitmaps);
	spritesheetplayerclimb = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "fasterdave/Character_character_climb.png",0, 128, dumpScaledBitmaps);
	spritesheetplayerrun = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "fasterdave/Character_character_run.png",0, 128, dumpScaledBitmaps);
	spritesheetplayeridle = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "fasterdave/Character_character_idle.png",0, 128, dumpScaledBitmaps);
	spritesheetplayerjump = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "fasterdave/Character_character_jump_up.png",0, 128, dumpScaledBitmaps);
	spritesheetenemy = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "fasterdave/enemy.png",0, 128, dumpScaledBitmaps);
	spritesheetcollectable = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "fasterdave/orbs.png",0, 128, dumpScaledBitmaps);
	spritesheetkey = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "fasterdave/key.png",0, 128, dumpScaledBitmaps);

    // SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/fasterdave/ladder.bmp", Game->Image->GetImage(spritesheetladder), 1,1, true,0, 128);
	// SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/fasterdave/floortileset.bmp", Game->Image->GetImage(spritesheet), 1,1, true,0, 128);
	// SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/fasterdave/Character_character_climb.bmp", Game->Image->GetImage(spritesheetplayerclimb), 1,1, true,0, 128);
	// SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/fasterdave/Character_character_run.bmp", Game->Image->GetImage(spritesheetplayerrun), 1,1, true,0, 128);
	// SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/fasterdave/Character_character_idle.bmp", Game->Image->GetImage(spritesheetplayeridle), 1,1, true,0, 128);
	// SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/fasterdave/Character_character_jump_up.bmp", Game->Image->GetImage(spritesheetplayerjump), 1,1, true,0, 128);
	// SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/fasterdave/enemy.bmp", Game->Image->GetImage(spritesheetenemy), 1,1, true, 0, 128);
	// SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/fasterdave/orbs.bmp", Game->Image->GetImage(spritesheetcollectable), 1,1, true,0, 128);
	// SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/fasterdave/key.bmp", Game->Image->GetImage(spritesheetkey), 1,1, true,0, 128);
	
	if(!useDefaultColorAssets)
	{
		UnloadGraphics();
		background = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "fasterdave/background.png");	
		spritesheetladder = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "fasterdave/ladder.bmp");
		spritesheet = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "fasterdave/floortileset.bmp");
		spritesheetplayerclimb = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "fasterdave/Character_character_climb.bmp");
		spritesheetplayerrun = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "fasterdave/Character_character_run.bmp");
		spritesheetplayeridle = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "fasterdave/Character_character_idle.bmp");
		spritesheetplayerjump = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "fasterdave/Character_character_jump_up.bmp");
		spritesheetenemy = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "fasterdave/enemy.bmp");
		spritesheetcollectable = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "fasterdave/orbs.bmp");
		spritesheetkey = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "fasterdave/key.bmp");
	}

	

}

void CGameFastEddy::UnloadGraphics()
{
	GameBase->Game->Image->UnLoadImage(background);
	GameBase->Game->Image->UnLoadImage(spritesheet);
	GameBase->Game->Image->UnLoadImage(spritesheetladder);
	GameBase->Game->Image->UnLoadImage(spritesheetplayerclimb);
	GameBase->Game->Image->UnLoadImage(spritesheetplayerrun);
	GameBase->Game->Image->UnLoadImage(spritesheetplayeridle);
	GameBase->Game->Image->UnLoadImage(spritesheetplayerjump);
	GameBase->Game->Image->UnLoadImage(spritesheetenemy);
	GameBase->Game->Image->UnLoadImage(spritesheetcollectable);
	GameBase->Game->Image->UnLoadImage(spritesheetkey);
}

//Update ----------------------------------------------------------------------------------------------------------------

void CGameFastEddy::UpdateObjects(bool IsGameState)
{
	if (IsGameState)
	{
		updateplayer();
		updateenemies();
		updatecollectables();
		updatekey();
	}
}

void CGameFastEddy::UpdateLogic()
{
	GameBase->UpdateLogic();
	UpdateObjects(GameBase->Game->SubGameState == SGGame);
	if(GameBase->Game->SubGameState == SGGame)
		GameBase->Game->Sprites->UpdateSprites(GameBase->Game->Renderer);
}

void CGameFastEddy::Draw()
{
	DrawBackground((GameBase->Game->SubGameState == SGGame));
	GameBase->Game->Sprites->DrawSprites(GameBase->Game->Renderer);
	GameBase->DrawScoreBar();
	GameBase->DrawSubStateText();
}