#include <SDL.h>
#include <string>
#include <iostream>
#include <cmath>
#include "CGameFastEddy.h"
#include "../CGame.h"
#include "../Common.h"
#include "../Vec2F.h"

using namespace std;

CGameFastEddy::CGameFastEddy(CGame* aGame, bool aScreenshotMode): CGameBase(aGame, GSEddy, true, aScreenshotMode)
{
	Game = aGame;
	MusMusic = -1;
	SfxSucces = -1;
	SfxDie = -1;

	playfieldwidth = 1280;
	playfieldheight = 720;
	screenleft = (ScreenWidth - playfieldwidth) / 2;
	screenright = screenleft + playfieldwidth;
	screentop = (ScreenHeight - playfieldheight) / 2;
	screenbottom = screentop + playfieldheight;
	numfloortilesperrow = playfieldwidth / rowfloorsize;
	maxfloortiles = numfloortilesperrow * rows;
	rowspacingsize = playfieldheight / rows;
	ladderwidth = playfieldwidth / laddersfitrows;
	rowsize = rowspacingsize - rowfloorsize;
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
	key.spr = Game->Sprites->CreateSprite();
	key.alive = true;
	SDL_Point tz = Game->Image->ImageSize(spritesheetkey);
	Vec2F scale = {keyheight / tz.y, keyheight / tz.y};
	key.tz.x = tz.x * scale.x;
	key.tz.y = tz.y * scale.y;
	Game->Sprites->SetSpriteImage(key.spr, &spritesheetkey, 1, 1);
	Game->Sprites->SetSpriteScale(key.spr, scale);
	Game->Sprites->SetSpriteAnimation(key.spr, 0, 0, 0);
	Game->Sprites->SetSpriteCollisionShape(key.spr, SHAPE_BOX, tz.x - 20, tz.y, 0, 0, 0);
	key.pos.y = enemies[rowzeroenemyindex].pos.y - enemies[rowzeroenemyindex].tz.y;
	key.pos.x = enemies[rowzeroenemyindex].pos.x;
	Game->Sprites->SetSpriteLocation(key.spr, key.pos);
}

void CGameFastEddy::destroykey()
{
	if (key.alive)
	{
		Game->Sprites->RemoveSprite(key.spr);
		key.alive = false;
	}
}

void CGameFastEddy::updatekey()
{
	if (key.alive)
	{
		key.pos.x = enemies[rowzeroenemyindex].pos.x;
		key.pos.y = enemies[rowzeroenemyindex].pos.y - enemies[rowzeroenemyindex].tz.y;
		Game->Sprites->SetSpriteLocation(key.spr, key.pos);

		if (Game->Sprites->DetectSpriteCollision(key.spr, player.spr))
		{
			Game->Audio->PlaySound(SfxSucces, 0);
			Game->AddToScore(200);
			SDL_Delay(500);
			level += 1;
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
		Game->Sprites->RemoveSprite(collectables[index].spr);
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

				collectables[i].spr = Game->Sprites->CreateSprite();
				collectables[i].alive = true;
				collectables[i].row = row;
				collectables[i].state = rand() % 3;
				Vec2F scale = {collectableheight / tz.y, collectableheight / tz.y};
				collectables[i].tz.x = tz.x * scale.x;
				collectables[i].tz.y = tz.y * scale.y;
				Game->Sprites->SetSpriteImage(collectables[i].spr, &spritesheetcollectable, 5, 1);
				Game->Sprites->SetSpriteScale(collectables[i].spr, scale);
				Game->Sprites->SetSpriteAnimation(collectables[i].spr, ((level-1) % 5), ((level-1) % 5), 0);
				Game->Sprites->SetSpriteCollisionShape(collectables[i].spr, SHAPE_CIRCLE, tz.x - 66, tz.y - 66, 0, 0, 0);
				//Game->Sprites->SetSpriteCollisionShape(collectables[i].spr, SHAPE_BOX, tz.x - 66, tz.y - 66, 0, 0, 0);
				collectables[i].pos.y = (row) * rowspacingsize + rowfloorsize / 2;
				collectables[i].pos.x = ((screenright - screenleft) /7) +
					(rand() % (screenright - screenleft - ((screenright - screenleft) /6 )));
				Game->Sprites->SetSpriteLocation(collectables[i].spr, collectables[i].pos);
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
				if (collectables[i].pos.x + collectables[i].tz.x / 2 < screenleft)
					collectables[i].pos.x = screenright + collectables[i].tz.x / 2;
				Game->Sprites->SetSpriteLocation(collectables[i].spr, collectables[i].pos);
			}

			if (collectables[i].state == collectablestatemoveright)
			{
				collectables[i].pos.x += collectablespeed;
				if(collectables[i].pos.x - collectables[i].tz.x / 2 > screenright)
					collectables[i].pos.x = screenleft - collectables[i].tz.x / 2;
				Game->Sprites->SetSpriteLocation(collectables[i].spr, collectables[i].pos);
			}

			if (Game->Sprites->DetectSpriteCollision(collectables[i].spr, player.spr))
			{
				Game->Audio->PlaySound(SfxCollect, 0);
				int ignorerow = collectables[i].row;
				destroycollectable(i);
				collecteditems += 1;
				Game->AddToScore(collecteditems * 10);
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
			Game->Sprites->RemoveSprite(enemies[i].spr);
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
	enemies[index].pos.y = (0 + 1) * rowspacingsize - rowfloorsize / 2 - enemies[index].tz.y / 2;
	Game->Sprites->SetSpriteScale(enemies[index].spr, scale);
	Game->Sprites->SetSpriteCollisionShape(enemies[index].spr, SHAPE_BOX, tz.x - 10, tz.y-10,0, 0, 0);
	Game->Sprites->SetSpriteLocation(enemies[index].spr, enemies[index].pos);
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
			enemies[i].spr = Game->Sprites->CreateSprite();
			enemies[i].alive = true;
			enemies[i].row = row;
			enemies[i].group = group;
			enemies[i].state = state;
			Vec2F scale = {enemyheight / tz.y * multiply, enemyheight / tz.y * multiply};
			enemies[i].tz.x = tz.x * scale.x;
			enemies[i].tz.y = tz.y * scale.y;
			Game->Sprites->SetSpriteImage(enemies[i].spr, &spritesheetenemy, 3, 4);
			Game->Sprites->SetSpriteScale(enemies[i].spr, scale);
			if (enemies[i].state == enemystatemoveright)
				Game->Sprites->SetSpriteAnimation(enemies[i].spr, 6, 8, 10);

			if (enemies[i].state == enemystateidle)
				Game->Sprites->SetSpriteAnimation(enemies[i].spr, 0, 2, 10);

			if (enemies[i].state == enemystatemoveleft)
				Game->Sprites->SetSpriteAnimation(enemies[i].spr, 3, 5, 10);

			if (enemies[i].state == enemystatewaitmove)
			{
				enemies[i].stateticks = 60 * 6;
				Game->Sprites->SetSpriteAnimation(enemies[i].spr, 0, 2, 10);
			}

			Game->Sprites->SetSpriteCollisionShape(enemies[i].spr, SHAPE_BOX, tz.x - 10, tz.y-10,0, 0, 0);
			enemies[i].pos.y = (row + 1) * rowspacingsize - rowfloorsize / 2 - enemies[i].tz.y / 2;
			enemies[i].pos.x = x;
			Game->Sprites->SetSpriteLocation(enemies[i].spr, enemies[i].pos);
			break;
		}
	}
}

void CGameFastEddy::createenemies(bool levelsucces)
{
	if (level-1 < 5)
	{
		createenemy(0, (screenright - screenleft) / 2, enemystatemoveright, 0, 2);
		if (levelsucces)
			createenemy(1, (screenright - screenleft) * 4 / 10, enemystatemoveright, 1, 1);
		else
			createenemy(1, (screenright - screenleft) / 10, enemystateidle, 1, 1);

		createenemy(2, (screenright - screenleft) / 2, enemystatemoveright, 2, 1);
		if (levelsucces)
			createenemy(3, (screenright - screenleft) * 6 / 10, enemystatemoveright, 3, 1);
		else
			createenemy(3, (screenright - screenleft) * 6 / 10, enemystateidle, 3, 1);

		createenemy(4, (screenright - screenleft) / 10, enemystatewaitmove, 4, 1);
	}
	else
	{
		if (level-1 < 10)
		{
			createenemy(0, (screenright - screenleft) / 2, enemystatemoveright, 0, 2);
			createenemy(1, (screenright - screenleft) * 4 / 10, enemystatemoveright, 1, 1);
			createenemy(1, (screenright - screenleft) * 5 / 10, enemystatemoveright, 1, 1);
			createenemy(2, (screenright - screenleft) / 2, enemystatemoveright, 2, 1);
			createenemy(3, (screenright - screenleft) * 6 / 10, enemystatemoveleft, 3, 1);
			createenemy(4, (screenright - screenleft) / 10, enemystatewaitmove, 4, 1);
		}
		else
		{
			if (level-1 < 15)
			{
				createenemy(0, (screenright - screenleft) / 2, enemystatemoveright, 0, 2);
				createenemy(1, (screenright - screenleft) * 4 / 10, enemystatemoveright, 1, 1);
				createenemy(1, (screenright - screenleft) * 5 / 10, enemystatemoveright, 1, 1);
				createenemy(2, (screenright - screenleft) / 2, enemystatemoveright, 2, 1);
				createenemy(3, (screenright - screenleft) * 6 / 10, enemystatemoveleft, 3, 1);
				createenemy(3, (screenright - screenleft) * 7 / 10, enemystatemoveleft, 3, 1);
				createenemy(4, (screenright - screenleft) / 10, enemystatewaitmove, 4, 1);
			}
			else
			{
				createenemy(0, (screenright - screenleft) / 2, enemystatemoveright, 0, 2);
				createenemy(1, (screenright - screenleft) * 4 / 10, enemystatemoveright, 1, 1);
				createenemy(1, (screenright - screenleft) * 5 / 10, enemystatemoveright, 1, 1);
				createenemy(2, (screenright - screenleft) / 2, enemystatemoveright, 2, 1);
				createenemy(3, (screenright - screenleft) * 1 / 10, enemystatemoveright, 3, 1);
				createenemy(3, (screenright - screenleft) * 6 / 10, enemystatemoveright, 4, 1);
				createenemy(4, (screenright - screenleft) / 10, enemystatewaitmove, 5, 1);
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
					Game->Sprites->SetSpriteAnimation(enemies[i].spr, 6, 8, 10);
				}
			}

			if (enemies[i].state == enemystatemoveright)
			{
				if (enemies[i].pos.x + enemies[i].tz.x / 2 + playerspeed < screenright)
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
								Game->Sprites->SetSpriteAnimation(enemies[j].spr, 3, 5, 10);
							}
						}
					}
				}
			}

			if(enemies[i].state == enemystatemoveleft)
			{
				if ( enemies[i].pos.x - enemies[i].tz.x / 2 - playerspeed > screenleft)
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
								Game->Sprites->SetSpriteAnimation(enemies[j].spr, 6, 8, 10);
							}
						}
					}
				}
			}

			Game->Sprites->SetSpriteLocation(enemies[i].spr, enemies[i].pos);

			if (Game->Sprites->DetectSpriteCollision(enemies[i].spr, player.spr))
			{
				if (((player.state != playerstateclimbup) &&
					(player.state != playerstateclimbdown) &&
					(player.state != playerstateunknown)) ||
					(enemies[i].row == 0))
				{
					Game->Audio->PlaySound(SfxDie, 0);
					if (Game->GameMode == GMGame)
						HealthPoints -= 1;

					if (HealthPoints > 0)
					{
						Game->AddToScore(-100);
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
		Game->Sprites->RemoveSprite(player.spr);
		player.alive = false;
	}
}

void CGameFastEddy::createplayer()
{
	player.spr = Game->Sprites->CreateSprite();
	player.alive = true;
	SDL_Point tz = {238/2, 342/2};
	Vec2F scale = {playerheight / tz.y, playerheight / tz.y};
	player.tz.x = tz.x * scale.x;
	player.tz.y = tz.y * scale.y;
	player.state = playerstateunknown;
	Game->Sprites->SetSpriteDepth(player.spr, 5);
	Game->Sprites->SetSpriteImage(player.spr, &spritesheetplayeridle, 1, 15);
	Game->Sprites->SetSpriteScale(player.spr, scale);
	Game->Sprites->SetSpriteAnimation(player.spr, 0, 14, 10);
	Game->Sprites->SetSpriteCollisionShape(player.spr, SHAPE_BOX, tz.x - 120, tz.y-30,0,0,0);
	player.pos = { (float)(screenright - screenleft) / 2, 5 * rowspacingsize - rowfloorsize / 2 - player.tz.y / 2};
	Game->Sprites->SetSpriteLocation(player.spr, player.pos);
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

			if (!Game->Input->PrevButtons.ButA && Game->Input->Buttons.ButA)
				jump = true;

			if ((Game->Input->Buttons.ButUp) ||
				(Game->Input->Buttons.ButUp2) ||
				(Game->Input->Buttons.ButDpadUp))
				up = true;

			if ((Game->Input->Buttons.ButDown) ||
				(Game->Input->Buttons.ButDown2) ||
				(Game->Input->Buttons.ButDpadDown))
				down = true;

			if ((Game->Input->Buttons.ButLeft) ||
				(Game->Input->Buttons.ButLeft2) ||
				(Game->Input->Buttons.ButDpadLeft))
				left = true;

			if ((Game->Input->Buttons.ButRight) ||
				(Game->Input->Buttons.ButRight2) ||
				(Game->Input->Buttons.ButDpadRight))
				right = true;

			if (jump)
			{
				if (left)
				{
					if (player.state != playerstateleftjump)
					{
						Game->Sprites->SetSpriteImage(player.spr, &spritesheetplayerjump, 1, 6);
						Game->Sprites->SetSpriteAnimation(player.spr, 3, 3, 10);

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
							Game->Sprites->SetSpriteImage(player.spr, &spritesheetplayerjump, 1, 6);
							Game->Sprites->SetSpriteAnimation(player.spr, 3, 3, 10);

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
							Game->Sprites->SetSpriteImage(player.spr, &spritesheetplayerjump, 1, 6);
							Game->Sprites->SetSpriteAnimation(player.spr, 3, 3, 10);
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
					Game->Sprites->SetSpriteLocation(player.spr, player.pos);
					bool found = false;
					for (int i = 0; i < maxladders; i++)
					{
						if (Game->Sprites->DetectSpriteCollision(player.spr, ladders[i].spr))
						{
							Game->Sprites->SetSpriteImage(player.spr, &spritesheetplayerclimb, 1, 15);
							Game->Sprites->SetSpriteAnimation(player.spr, 0, 14, 20);

							player.pos.x = ladders[i].pos.x;
							player.state = playerstateclimbup;
							player.floory = player.pos.y + rowspacingsize/4;
							found = true;
							break;
						};
					};
					player.pos.y += rowspacingsize/4;
					Game->Sprites->SetSpriteLocation(player.spr, player.pos);
					if (!found)
					{
						if (player.state != playerstateidle)
						{
							Game->Sprites->SetSpriteImage(player.spr, &spritesheetplayeridle, 1, 15);
							Game->Sprites->SetSpriteAnimation(player.spr, 0, 14, 10);
							player.state = playerstateidle;
						}
					}
				}
				else
				{
					if (down)
					{
						player.pos.y += rowspacingsize - playerheight / 4;
						Game->Sprites->SetSpriteLocation(player.spr, player.pos);
						bool found = false;
						for (int i = 0; i < maxladders; i++)
						{
							if (Game->Sprites->DetectSpriteCollision(player.spr, ladders[i].spr))
							{
								Game->Sprites->SetSpriteImage(player.spr, &spritesheetplayerclimb, 1, 15);
								Game->Sprites->SetSpriteAnimation(player.spr, 14, 0, 20);

								player.pos.x = ladders[i].pos.x;
								player.state = playerstateclimbdown;
								player.floory = player.pos.y - rowspacingsize + playerheight / 4;
								found = true;
								break;
							};
						};
						player.pos.y -= rowspacingsize - playerheight / 4;
						Game->Sprites->SetSpriteLocation(player.spr, player.pos);
						if (!found)
						{
							if (player.state != playerstateidle)
							{
								Game->Sprites->SetSpriteImage(player.spr, &spritesheetplayeridle, 1, 15);
								Game->Sprites->SetSpriteAnimation(player.spr, 0, 14, 10);
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
								Game->Sprites->SetSpriteImage(player.spr, &spritesheetplayerrun, 1, 7);
								Game->Sprites->SetSpriteAnimation(player.spr, 0, 6, 10);
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
									Game->Sprites->SetSpriteImage(player.spr, &spritesheetplayerrun, 1, 7);
									Game->Sprites->SetSpriteAnimation(player.spr, 0, 6, 10);
									player.spr->xscale = (abs(player.spr->xscale));
									player.state = playerstateright;
								}
							}
							else
							{
								if (player.state != playerstateidle)
								{
									Game->Sprites->SetSpriteImage(player.spr, &spritesheetplayeridle, 1, 15);
									Game->Sprites->SetSpriteAnimation(player.spr, 0, 14, 10);
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

			Game->Sprites->SetSpriteLocation(player.spr, player.pos);
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
			Game->Sprites->SetSpriteLocation(player.spr, player.pos);
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
			Game->Sprites->SetSpriteLocation(player.spr, player.pos);
			if (player.pos.y < player.floory - 5/6 * playerjumpheight)
				Game->Sprites->SetSpriteAnimation(player.spr, 5, 5, 0);
			else
			{
				if (player.pos.y < player.floory - 4/6 * playerjumpheight)
					Game->Sprites->SetSpriteAnimation(player.spr, 4, 4, 0);
				else
				{
					if(player.pos.y < player.floory - 3/6 * playerjumpheight)
						Game->Sprites->SetSpriteAnimation(player.spr, 3, 3, 0);
					else
					{
						if(player.pos.y < player.floory - 2/6 * playerjumpheight)
							Game->Sprites->SetSpriteAnimation(player.spr, 2, 2, 0);
						else
						{
							if(player.pos.y < player.floory - 1/6 * playerjumpheight)
								Game->Sprites->SetSpriteAnimation(player.spr, 1, 1, 0);
							else
							{
								if (player.pos.y < player.floory - 0/6 * playerjumpheight)
									Game->Sprites->SetSpriteAnimation(player.spr, 0, 0, 0);
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
				if ( player.pos.x + player.tz.x / 2 + playerspeed < screenright)
					player.pos.x += playerspeed ;
				else
					player.pos.x = screenright - player.tz.x / 2;
			}
			else
			{
				if ( player.pos.x - player.tz.x / 2 - playerspeed > screenleft)
					player.pos.x -= playerspeed;
				else
					player.pos.x = screenleft + player.tz.x / 2;
			}

			if (player.pos.y < player.floory - 5/6 * playerjumpheight)
				Game->Sprites->SetSpriteAnimation(player.spr, 5, 5, 0);
			else
			{
				if (player.pos.y < player.floory - 4/6 * playerjumpheight)
					Game->Sprites->SetSpriteAnimation(player.spr, 4, 4, 0);
				else
				{
					if (player.pos.y < player.floory - 3/6 * playerjumpheight)
						Game->Sprites->SetSpriteAnimation(player.spr, 3, 3, 0);
					else
					{
						if (player.pos.y < player.floory - 2/6 * playerjumpheight)
							Game->Sprites->SetSpriteAnimation(player.spr, 2, 2, 0);
						else
						{
							if(player.pos.y < player.floory - 1/6 * playerjumpheight)
								Game->Sprites->SetSpriteAnimation(player.spr, 1, 1, 0);
							else
							{
								if(player.pos.y < player.floory - 0/6 * playerjumpheight)
									Game->Sprites->SetSpriteAnimation(player.spr, 0, 0, 0);
							}
						}
					}
				}
			}

			Game->Sprites->SetSpriteLocation(player.spr, player.pos);
		}

		if (player.state == playerstateright)
		{
			if ( player.pos.x + player.tz.x / 2 + playerspeed < screenright)
				player.pos.x += playerspeed;
			else
				player.pos.x = screenright - player.tz.x / 2;
			Game->Sprites->SetSpriteLocation(player.spr, player.pos);
		}

		if (player.state == playerstateleft)
		{
			if ( player.pos.x - player.tz.x / 2 - playerspeed > screenleft)
				player.pos.x -= playerspeed;
			else
				player.pos.x = screenleft + player.tz.x / 2;
			Game->Sprites->SetSpriteLocation(player.spr, player.pos);
		}
	}
}



//floors ----------------------------------------------------------------------------------------------------------------

void CGameFastEddy::destroyfloors()
{
	for (int i = 0; i < maxfloortiles; i++)
		Game->Sprites->RemoveSprite(floors[i].spr);
}

void CGameFastEddy::createfloors()
{
	int tilenr = 0;
	int i = 0;
	//SDL_Point tz = {64,64};
	SDL_Point tz = Game->Image->ImageSize(spritesheetladder);
	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < numfloortilesperrow; x++)
		{
			floors[i].spr = Game->Sprites->CreateSprite();
			floors[i].alive = true;
			Game->Sprites->SetSpriteImage(floors[i].spr, &spritesheet, 9, 10);
			Game->Sprites->SetSpriteLocation(floors[i].spr, {(float)x * rowfloorsize + rowfloorsize / 2, (float)(y + 1) * rowspacingsize - rowfloorsize / 2});
			Game->Sprites->SetSpriteScale(floors[i].spr, {(float)rowfloorsize / tz.x , (float)rowfloorsize / tz.y});
			if (x == 0)
				tilenr = 36;
			else
			{
				if (x == numfloortilesperrow -1)
					tilenr = 38;
				else
					tilenr = 37;
			}
			Game->Sprites->SetSpriteAnimation(floors[i].spr, tilenr, tilenr, 0);
			i += 1;
		}
	}
}

//ladders ----------------------------------------------------------------------------------------------------------------

void CGameFastEddy::destroyladders()
{
	for (int i = 0; i < maxladders; i++)
		Game->Sprites->RemoveSprite(ladders[i].spr);
}

void CGameFastEddy::createladders()
{
	int i = 0;
	SDL_Point tz = Game->Image->ImageSize(spritesheetladder);
	for (int y = 1; y < rows; y++)
	{
		int x1 = 1 + rand() % (int(laddersfitrows / 2)-1);
		ladders[i].spr = Game->Sprites->CreateSprite();
		ladders[i].alive = true;
		ladders[i].pos = {x1 * ladderwidth + ladderwidth / 2 , y * rowspacingsize + rowspacingsize / 2 - rowfloorsize / 2};
		Game->Sprites->SetSpriteImage(ladders[i].spr, &spritesheetladder);
		Game->Sprites->SetSpriteCollisionShape(ladders[i].spr, SHAPE_BOX, tz.x - 20, tz.y-10,0,0,0);

		Game->Sprites->SetSpriteLocation(ladders[i].spr, ladders[i].pos );
		Game->Sprites->SetSpriteScale(ladders[i].spr, {ladderwidth / tz.x , (rowspacingsize + rowfloorsize/2) / tz.y});

		i += 1;
		int x2 = x1 + int(laddersfitrows / 2);
		ladders[i].spr = Game->Sprites->CreateSprite();
		ladders[i].alive = true;
		ladders[i].pos = {x2 * ladderwidth + ladderwidth / 2 , y * rowspacingsize + rowspacingsize / 2 - rowfloorsize / 2};
		Game->Sprites->SetSpriteImage(ladders[i].spr, &spritesheetladder);
		Game->Sprites->SetSpriteCollisionShape(ladders[i].spr, SHAPE_BOX, tz.x - 20, tz.y-10,0,0,0);
		Game->Sprites->SetSpriteLocation(ladders[i].spr, ladders[i].pos );
		Game->Sprites->SetSpriteScale(ladders[i].spr, {ladderwidth / tz.x , (rowspacingsize + rowfloorsize/2) / tz.y});
		i += 1;
	}
}

//background ----------------------------------------------------------------------------------------------------------------

void CGameFastEddy::DrawBackground(bool motionblur)
{
	float alpha = 1;
	if ((motionblur) && (Game->MotionBlur))
		alpha = 0.3;
	SDL_Point Pos = { ScreenWidth / 2, ScreenHeight / 2};
	Vec2F Scale = {(float)ScreenWidth / backgroundtz.x, (float)ScreenHeight / backgroundtz.y};
	Game->Image->DrawImageFuzeTintFloat(Game->Renderer, background, true, &Pos, 0, &Scale, 1, 1, 1, alpha);
}

//init - deinit ----------------------------------------------------------------------------------------------------------------

void CGameFastEddy::init()
{
	collecteditems = 0;
	collectedcreated = 0;
	level = 1;
	HealthPoints = 3;
	LoadGraphics();
	createfloors();
	createladders();
	createenemies(false);
	createcollectables(-1);
	createkey();
	createplayer();
	if(!ScreenshotMode)
	{
		LoadSound();
		Game->CurrentGameMusicID = MusMusic;
		Game->Audio->PlayMusic(MusMusic, -1);
	}
}

void CGameFastEddy::deinit()
{
	destroyfloors();
	destroyladders();
	destroyenemies();
	destroyallcollectables();
	destroykey();
	destroyplayer();
	if (!ScreenshotMode)
	{
		UnLoadSound();
		Game->SubStateCounter = 0;
		Game->SubGameState = SGNone;
		Game->CurrentGameMusicID = -1;
	}
	UnloadGraphics();
}

void CGameFastEddy::LoadSound()
{
	SfxDie = Game->Audio->LoadSound("common/die.wav");
	SfxSucces = Game->Audio->LoadSound("common/succes.wav");
	SfxCollect = Game->Audio->LoadSound("common/coin.wav");
	MusMusic = Game->Audio->LoadMusic("fasterdave/music.ogg");
}

void CGameFastEddy::UnLoadSound()
{
	Game->Audio->StopMusic();
	Game->Audio->StopSound();
	Game->Audio->UnLoadMusic(MusMusic);
	Game->Audio->UnLoadSound(SfxDie);
	Game->Audio->UnLoadSound(SfxSucces);
	Game->Audio->UnLoadSound(SfxCollect);
}

void CGameFastEddy::LoadGraphics()
{
	background = Game->Image->LoadImage(Game->Renderer, "fasterdave/background.png");
	backgroundtz = Game->Image->ImageSize(background);

	spritesheet = Game->Image->LoadImage(Game->Renderer, "fasterdave/floortileset.png");
	spritesheetladder = Game->Image->LoadImage(Game->Renderer, "fasterdave/ladder.png");

	spritesheetplayerclimb = Game->Image->LoadImage(Game->Renderer, "fasterdave/Character_character_climb.png");
	spritesheetplayerrun = Game->Image->LoadImage(Game->Renderer, "fasterdave/Character_character_run.png");
	spritesheetplayeridle = Game->Image->LoadImage(Game->Renderer, "fasterdave/Character_character_idle.png");
	spritesheetplayerjump = Game->Image->LoadImage(Game->Renderer, "fasterdave/Character_character_jump_up.png");
	spritesheetenemy = Game->Image->LoadImage(Game->Renderer, "fasterdave/enemy.png");
	spritesheetcollectable = Game->Image->LoadImage(Game->Renderer, "fasterdave/orbs.png");
	spritesheetkey = Game->Image->LoadImage(Game->Renderer, "fasterdave/key.png");
}

void CGameFastEddy::UnloadGraphics()
{
	Game->Image->UnLoadImage(background);
	Game->Image->UnLoadImage(spritesheet);
	Game->Image->UnLoadImage(spritesheetladder);
	Game->Image->UnLoadImage(spritesheetplayerclimb);
	Game->Image->UnLoadImage(spritesheetplayerrun);
	Game->Image->UnLoadImage(spritesheetplayeridle);
	Game->Image->UnLoadImage(spritesheetplayerjump);
	Game->Image->UnLoadImage(spritesheetenemy);
	Game->Image->UnLoadImage(spritesheetcollectable);
	Game->Image->UnLoadImage(spritesheetkey);
}

SDL_Texture* CGameFastEddy::screenshot()
{
	SDL_Texture* prev = SDL_GetRenderTarget(Game->Renderer);
	SDL_Texture* image = SDL_CreateTexture(Game->Renderer, PixelFormat, SDL_TEXTUREACCESS_TARGET, ScreenWidth, ScreenHeight);
	SDL_SetRenderTarget(Game->Renderer, image);
	SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 255);
	SDL_RenderClear(Game->Renderer);
	init();

	Draw();

	//this crashes metal backend on mac and seems is not required !
	//SDL_RenderPresent(Game->Renderer);
	SDL_SetRenderTarget(Game->Renderer, prev);
	deinit();
	return image;
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

