#include <SDL.h>
#include <string>
#include <iostream>
#include "CGameFrog.h"
#include "../CGame.h"
#include "../Common.h"
#include "../Vec2F.h"

using namespace std;


CGameFrog::CGameFrog(CGame* aGame, bool aScreenshotMode): CGameBase(aGame, GSFrog, true, aScreenshotMode)
{
    Game = aGame;
	MusMusic = -1;
    SfxDie = -1;
    SfxCollect = -1;
	SfxPlayerMove = -1;
	prevrowtype = -1;
	playfieldwidth = numcols * playerspeed;
	playfieldheight = numrows * playerspeed;
	screenleft = (ScreenWidth - playfieldwidth) / 2;
	screenright = screenleft + playfieldwidth;
	screentop = ScreenHeight - playfieldheight;
	screenbottom = ScreenHeight;

}

CGameFrog::~CGameFrog() {};


//objects  ----------------------------------------------------------------------------------------------------------------
void CGameFrog::createobjects(bool initialize)
{
	int rowtype = prevrowtype;
	int i = 0;
	int r = 0;

	if (initialize)
	{
		nextrowtype = rowtyperoad;
		plantsspawned = 0;
		nextrowtypecountmax = rand() % (maxrowsbeforesafetyrow+1);
		nextrowtypecount = 0;
		while (i < numrows)
		{
			if (i < playerstartrow)
				createobjectrow(i, lenrowtypes - 1);
			else
			{
				if (nextrowtype == rowtyperoad)
				{
					//while rowtype == prevrowtype loop
					rowtype = rowtyperoadstart + rand() % (rowtyperoadend - rowtyperoadstart + 1);
					if (rowtype == prevrowtype)
					{
						rowtype += 1;
						if (rowtype > rowtyperoadend)
							rowtype = rowtyperoadstart;
					}
					prevrowtype = rowtype;
				}
				else
				{
					if (nextrowtype == rowtypewater)
					{						
						rowtype = rowtypewaterstart + rand() % (rowtypewaterend - rowtypewaterstart + 1);
						if (rowtype == prevrowtype)
						{
							rowtype += 1;
							if (rowtype > rowtypewaterend)
								rowtype = rowtypewaterstart;
						}
					}
					
					prevrowtype = rowtype;
				}

  			  	if(nextrowtypecount == nextrowtypecountmax)
			  	{
					rowtype = lenrowtypes -1;
					nextrowtypecountmax = 2+(rand() %(maxrowsbeforesafetyrow-1));
					nextrowtypecount = 0;

					r = rand() % 2;			

					if (r == 0)
						nextrowtype = rowtyperoad;
					else
					{
						if (r == 1)
							nextrowtype = rowtypewater;
						
					}
			  	}
				else
					nextrowtypecount += 1;

				createobjectrow(i, rowtype);
								
			}
			i+=1;

		}
	}
	else
	{
		i = visiblerows;

		while (i < numrows)
		{
			if (nextrowtype == rowtyperoad)
			{
				rowtype = rowtyperoadstart + (rand() % (rowtyperoadend - rowtyperoadstart + 1));
				if (rowtype == prevrowtype)
				{
					rowtype += 1;
					if (rowtype > rowtyperoadend)
						rowtype = rowtyperoadstart;
				}
				prevrowtype = rowtype;
			}
			else
			{
				if(nextrowtype == rowtypewater)
				{
					rowtype = rowtypewaterstart + (rand() % (rowtypewaterend - rowtypewaterstart + 1));
					if(rowtype == prevrowtype)
					{
						rowtype += 1;
						if (rowtype > rowtypewaterend)
							rowtype = rowtypewaterstart;
					}

					prevrowtype = rowtype;
				}
			}
			
			if(nextrowtypecount == nextrowtypecountmax)
			{
				rowtype = lenrowtypes -1;
				nextrowtypecountmax = 2+(rand() % (maxrowsbeforesafetyrow-1));
				//for debugging finding max objects count
//				nextrowtypecountmax = 4
				nextrowtypecount = 0;
				r = rand() % 2;			
				if (r == 0)
					nextrowtype = rowtyperoad;
				else
				{
					if (r == 1)
						nextrowtype = rowtypewater;
				}
			}
			else
				nextrowtypecount += 1;
			createobjectrow(i, rowtype);
			i += 1;
		}
	}
}

void CGameFrog::destroyallobjects()
{
	for (int i = 0; i < maxobjects; i++)
		destroyobject(i);
}

void CGameFrog::destroyobject(int index)
{
	if (objects[index].alive)
	{
		Game->Sprites->RemoveSprite(objects[index].spr);
		objects[index].alive = false;
		numobjects -=1;
	}
}

int CGameFrog::createobject(int rownr, int col, int id, int arowtype, float speed, int dir, int startobjectindex)
{
	SDL_Point tz = {1,1};
	Vec2F scale;
	Vec2F visualscale;
	int tilenr;
	int endtilenr;
	CSpriteObject object;
	int result = 0;

	for (int i = startobjectindex; i < maxobjects; i++)
	{
		if (!objects[i].alive)
		{
			object.spr = Game->Sprites->CreateSprite(); 
			
			if ((id == idwater) || (id == idroad) || (id == idgrass))
			{
				tilenr = rowtypes[arowtype].backgroundtile;
				endtilenr = rowtypes[arowtype].backgroundtileend;
				Game->Sprites->SetSpriteImage(object.spr, &spritesheetbackground,6,3);
				tz = Game->Sprites->TileSize(object.spr);				
				Game->Sprites->SetSpriteAnimation(object.spr, tilenr, endtilenr, 6);
				Game->Sprites->SetSpriteDepth(object.spr, 0);
				object.vel = {0,0};
				scale = {playerspeed / tz.x, playerspeed / tz.y};
				visualscale = scale;
				object.tz.x = tz.x * scale.x;
				object.tz.y = tz.y * scale.y;
			}

			if (id == idenemyplant)
			{
				tilenr = 1;
				Game->Sprites->SetSpriteImage(object.spr, &spritesheetplant, 2, 1);
				tz = Game->Sprites->TileSize(object.spr);	
				
				Game->Sprites->SetSpriteAnimation(object.spr, tilenr, tilenr, 0);
				Game->Sprites->SetSpriteDepth(object.spr, 1);
				//scale = {(playerspeed - int(playerspeed / 48)) / tz.x, (playerspeed - int(playerspeed / 48)) / tz.y}
				scale = {playerspeed / tz.x, playerspeed / tz.y};
				visualscale = scale;
				object.vel = {dir*speed,0};			
				object.tz.x = tz.x * scale.x;
				object.tz.y = tz.y * scale.y;
			}

			if (id == idenemycar1)
			{	
				tilenr = 0;		
				if (dir == -1)
					tilenr = 1;
								
				Game->Sprites->SetSpriteImage(object.spr, &spritesheetcar1, 2, 1);
				tz = Game->Sprites->TileSize(object.spr);
				Game->Sprites->SetSpriteAnimation(object.spr, tilenr, tilenr, 0); 
				Game->Sprites->SetSpriteDepth(object.spr, 3);
				object.vel = {dir*speed,0};
				scale = {(playerspeed - (playerspeed / 6)) / tz.x, (playerspeed - (playerspeed / 6))/ tz.y};		
				visualscale = scale;
				object.tz.x = tz.x * scale.x;
				object.tz.y = tz.y * scale.y;
			}

			if (id == idenemycar2)
			{	
				tilenr = 0;		
				if (dir == -1)
					tilenr = 1;
								
				Game->Sprites->SetSpriteImage(object.spr, &spritesheetcar2, 2, 1);
				tz = Game->Sprites->TileSize(object.spr);
				Game->Sprites->SetSpriteAnimation(object.spr, tilenr, tilenr, 0); 
				Game->Sprites->SetSpriteDepth(object.spr, 3);
				object.vel = {dir*speed,0};
				scale = {(playerspeed - (playerspeed / 6)) / tz.x, (playerspeed - (playerspeed / 6))/ tz.y};		
				visualscale = scale;
				object.tz.x = tz.x * scale.x;
				object.tz.y = tz.y * scale.y;
			}

			if (id == idenemycar3)
			{	
				tilenr = 0;		
				if (dir == -1)
					tilenr = 1;
								
				Game->Sprites->SetSpriteImage(object.spr, &spritesheetcar3, 2, 1);
				tz = Game->Sprites->TileSize(object.spr);
				Game->Sprites->SetSpriteAnimation(object.spr, tilenr, tilenr, 0); 
				Game->Sprites->SetSpriteDepth(object.spr, 3);
				object.vel = {dir*speed,0};
				scale = {(playerspeed - (playerspeed / 6)) / tz.x, (playerspeed - (playerspeed / 6))/ tz.y};		
				visualscale = scale;
				object.tz.x = tz.x * scale.x;
				object.tz.y = tz.y * scale.y;
			}

			if (id == idenemycar4)
			{	
				tilenr = 0;		
				if (dir == -1)
					tilenr = 1;
								
				Game->Sprites->SetSpriteImage(object.spr, &spritesheetcar4, 2, 1);
				tz = Game->Sprites->TileSize(object.spr);
				Game->Sprites->SetSpriteAnimation(object.spr, tilenr, tilenr, 0); 
				Game->Sprites->SetSpriteDepth(object.spr, 3);
				object.vel = {dir*speed,0};
				scale = {(playerspeed - (playerspeed / 6)) / tz.x, (playerspeed - (playerspeed / 6))/ tz.y};		
				visualscale = scale;
				object.tz.x = tz.x * scale.x;
				object.tz.y = tz.y * scale.y;
			}

			if (id == idenemycar5)
			{	
				tilenr = 0;		
				if (dir == -1)
					tilenr = 1;
								
				Game->Sprites->SetSpriteImage(object.spr, &spritesheetcar5, 2, 1);
				tz = Game->Sprites->TileSize(object.spr);
				Game->Sprites->SetSpriteAnimation(object.spr, tilenr, tilenr, 0); 
				Game->Sprites->SetSpriteDepth(object.spr, 3);
				object.vel = {dir*speed,0};
				scale = {(playerspeed - (playerspeed / 6)) / tz.x, (playerspeed - (playerspeed / 6))/ tz.y};		
				visualscale = scale;
				object.tz.x = tz.x * scale.x;
				object.tz.y = tz.y * scale.y;
			}


			if (id == idcherry)
			{							
				tilenr = 0;

				Game->Sprites->SetSpriteImage(object.spr, &spritesheetfruit1);
				tz = Game->Sprites->TileSize(object.spr);
				Game->Sprites->SetSpriteAnimation(object.spr, tilenr, tilenr, 0);
				object.vel = {dir*speed,0};
				scale = {playerspeed  / tz.x, playerspeed / tz.y};
				visualscale = {(playerspeed - (playerspeed / 2)) / tz.x, (playerspeed - (playerspeed / 2))/ tz.y};
				Game->Sprites->SetSpriteDepth(object.spr, 3);
			}
			
			if (id == idlemon) 
			{							
				tilenr = 0;

				Game->Sprites->SetSpriteImage(object.spr, &spritesheetfruit2);
				tz = Game->Sprites->TileSize(object.spr);
				Game->Sprites->SetSpriteAnimation(object.spr, tilenr, tilenr, 0);
				object.vel = {dir*speed,0};
				scale = {playerspeed  / tz.x, playerspeed / tz.y};
				visualscale = {(playerspeed - (playerspeed / 2)) / tz.x, (playerspeed - (playerspeed / 2))/ tz.y};
				Game->Sprites->SetSpriteDepth(object.spr, 3);
			}

			if (id == idapple) 
			{							
				tilenr = 0;

				Game->Sprites->SetSpriteImage(object.spr, &spritesheetfruit3);
				tz = Game->Sprites->TileSize(object.spr);
				Game->Sprites->SetSpriteAnimation(object.spr, tilenr, tilenr, 0);
				object.vel = {dir*speed,0};
				scale = {playerspeed  / tz.x, playerspeed / tz.y};
				visualscale = {(playerspeed - (playerspeed / 2)) / tz.x, (playerspeed - (playerspeed / 2))/ tz.y};
				Game->Sprites->SetSpriteDepth(object.spr, 3);
			}
			//setSpriteRotation(objects[i].spr, 0)
			Game->Sprites->SetSpriteScale(object.spr, visualscale);
			object.tz.x = tz.x * scale.x;
			object.tz.y = tz.y * scale.y;
			object.pos = {screenleft + (col * playerspeed)  + (playerspeed / 2), 
				screenbottom - ((rownr+1) * playerspeed) + (playerspeed / 2)};
			//Game->Sprites->SetSpriteLocation(object.spr, object.pos)
			object.spr->x = (int)object.pos.x;
			object.spr->y = (int)object.pos.y;
			object.rowtype = arowtype;
			object.id = id;
			object.alive = true;
			objects[i] = object;
			numobjects +=1;
			result = i;
			break;
		}
	}
	return result;
}

void CGameFrog::createobjectrow(int rownr, int arowtype)
{
	int type = rowtypes[arowtype].type;
	int minspace = rowtypes[arowtype].minspace;
	int maxrepeats = 1 + (rand() % (rowtypes[arowtype].maxrepeats));
	int id = rowtypes[arowtype].id;
	int rowtypemaxrepeats = rowtypes[arowtype].maxrepeats;
	int repeats = 0;
	int lastplaced = 0;
	int dir = rand() % 2;
	if (dir == 0)
		dir = -1;
	
	float speed = (rowtypes[arowtype].speed - speeddeviation) + ((rand() % (int)(speeddeviation*100)/100));
	int index = 0;
	for (int x = 0; x < numcols; x++)
	{	
		if (type == rowtyperoad)
		{
			index = createobject(rownr, x, idroad, arowtype, 0, 0, index);
				
			if ((x - lastplaced > minspace) || (repeats < maxrepeats))
			{
				if (x - lastplaced > minspace)
				{
					maxrepeats = 1 + (rand() % rowtypemaxrepeats);
					repeats = 0;
				}
				lastplaced = x;
				repeats += 1;
				index = createobject(rownr, x, id, arowtype, speed, dir, index);
			}
		}
		else
		{
			if (type == rowtypewater)
			{
				index = createobject(rownr, x, idwater, arowtype, 0, 0, index);
				if ((x - lastplaced > minspace) || (repeats < maxrepeats))
				{
					if (x - lastplaced > minspace)
					{
						maxrepeats = 1 + (rand()% rowtypemaxrepeats);
						repeats = 0;
					}
					lastplaced = x;
					repeats += 1;
					index = createobject(rownr, x, id, arowtype, speed, dir, index);
					plantsspawned += 1;
	
					if (plantsspawned % cherryspawntrigger == 0)
						index = createobject(rownr, x, idcherry, arowtype, speed, dir, index);

					if (plantsspawned % lemonspawntrigger == 0)
						index = createobject(rownr, x, idlemon, arowtype, speed, dir, index);

					if (plantsspawned % applespawntrigger == 0)
						index = createobject(rownr, x, idapple, arowtype, speed, dir, index);
				}	
			}
			else
			{
				if (type == rowtypesafety)
					index = createobject(rownr, x, idgrass, arowtype, 0, 0, index);
			}
		}
	}
}


void CGameFrog::updateobjects()
{
	objectinfo.mostleft = -1;
	objectinfo.mostright = -1;
	objectinfo.mostbottom = -1;
	objectinfo.mosttop = -1;

	int x1 = screenright + 1;
	int x2 = screenleft - 1;
	int y = screentop - 1;
	int y2 = screenbottom + 1;
	bool playermoved = false;
	int id = -1;
	bool plantcol = false;
	bool stopcheckplantcol = false;
	
	for (int i = 0; i < maxobjects; i++)
	{
		if(objects[i].alive)
		{
			// if ((objects[i].pos.y + 1 > player.pos.y) && (objects[i].pos.y - 1 < player.pos.y))
			// {
			// 	if(objects[i].id == idgrass)
			// 	{
			// 		printf("P:%f O:%f\n", objects[i].pos.y, player.pos.y);
			// 	}
			//}

			//if (floatequal(objects[i].pos.y,player.pos.y))
			{							
				if (Game->Sprites->DetectSpriteCollision(objects[i].spr, player.spr))
				{
					id = objects[i].id;
				
					if (!playermoved && (id == idenemyplant) && !playerdeath &&
						(player.pos.x - player.tz.x / 4 <= objects[i].pos.x + objects[i].tz.x / 2) &&
						(player.pos.x + player.tz.x / 4 >= objects[i].pos.x - objects[i].tz.x / 2))
						{
							player.pos = objects[i].pos;
							player.spr->x = (int)player.pos.x;
							player.spr->y = (int)player.pos.y;
							if ((player.pos.x < screenleft) || (player.pos.x > screenright))
								playerdeath = true;
				
							playermoved = true;
							plantcol = true;
						}

					if ((id == idenemycar1) || (id == idenemycar2) || (id == idenemycar3) ||
						(id == idenemycar4) || (id == idenemycar5))
						playerdeath = true;
					else
					{
						if ((id == idroad) or (id == idgrass))
							playermoved = true;
						else
						{
							if (id == idlemon)
							{
								Game->AddToScore(lemonscore);
								destroyobject(i);
								Game->Audio->PlaySound(SfxCollect, 0);
							}
							else
							{
								if (id == idapple)
								{
									Game->AddToScore(applescore);
									destroyobject(i);
									Game->Audio->PlaySound(SfxCollect, 0);
								}
								else
								{
									if (id == idcherry)
									{
										Game->AddToScore(cherryscore);
										destroyobject(i);
										Game->Audio->PlaySound(SfxCollect, 0);
									}
								}
							}
						}
					}			
				
				
					if ((!stopcheckplantcol)  && (!plantcol) && (id == idwater) && (!playerdeath))
					{
						stopcheckplantcol = true;
						for (int j = i + 1; j < maxobjects;j++)
						{			
							if ((objects[j].alive) /*&& floatequal(objects[j].pos.y,player.pos.y)*/)
							{
								if(Game->Sprites->DetectSpriteCollision(objects[j].spr, player.spr))
								{
									if (id == idenemyplant)
									{
										if ((player.pos.x - player.tz.x / 4 <= objects[i].pos.x + objects[i].tz.x / 2) &&
											(player.pos.x + player.tz.x / 4 >= objects[i].pos.x - objects[i].tz.x / 2))
										{
											//player.pos = objects[i].pos //player.pos + objects[i].vel
											plantcol = true;
											break;
										}
									}
								}
							}							
						}
					}
				}
			}
			
			if (objects[i].alive)
			{
				objects[i].pos.y = objects[i].pos.y + worldspeed + levelincspeeds[level-1];
				if (objects[i].pos.y - objects[i].tz.y / 2 > screenbottom)
					destroyobject(i);
				else 		
				{
					objects[i].pos.x = objects[i].pos.x +  objects[i].vel.x;
					objects[i].pos.y = objects[i].pos.y +  objects[i].vel.y;
					if (objects[i].vel.x < 0)
					{
						if (objects[i].pos.x + objects[i].tz.x / 2 < screenleft)
							objects[i].pos.x = screenright - (screenleft - objects[i].pos.x) + objects[i].tz.x / 2 + objects[i].tz.x / 2;
					}
					else
					{
						if (objects[i].pos.x - objects[i].tz.x / 2 > screenright)		
							objects[i].pos.x =  screenleft + (objects[i].pos.x - screenright) - objects[i].tz.x / 2 - objects[i].tz.x / 2;
						
					}
					objects[i].spr->x = (int)objects[i].pos.x;
					objects[i].spr->y = (int)objects[i].pos.y;

					if ((objects[i].id == idwater) || (objects[i].id == idroad) || (objects[i].id == idgrass))
					{	
						if (objects[i].pos.x < x1)
						{
							x1 = objects[i].pos.x;
							objectinfo.mostleft = i;
						}
						else
						{
							if (objects[i].pos.x > x2)
							{
								x2 = objects[i].pos.x;
								objectinfo.mostright = i;
							}
						}
						
						if (objects[i].pos.y > y)
						{
							y = objects[i].pos.y;
							objectinfo.mostbottom = i;
						}
						else 
						{
							if (objects[i].pos.y < y2)
							{
								y2 = objects[i].pos.y;
								objectinfo.mosttop = i;
							}
							
						}
					}
				}
			}
		}
	}

	if (objectinfo.mosttop > -1)
	{
		if (objects[objectinfo.mosttop].pos.y - objects[objectinfo.mosttop].tz.y / 2 == screenbottom - (visiblerows * playerspeed))
		{
			createobjects(false);
			rowsspawned += 1;
			if (rowsspawned == levelincspawns[level-1])
				dolevelinc = true;
		}
	}
	
	if (!playermoved)
		playerdeath = true;
}

//player ----------------------------------------------------------------------------------------------------------------

void CGameFrog::destroyplayer()
{	
	Game->Sprites->RemoveSprite(player.spr);
	player.alive = false;
}

void CGameFrog::createplayer()
{
	player.spr = Game->Sprites->CreateSprite();
	Game->Sprites->SetSpriteDepth(player.spr, 2);				
	Game->Sprites->SetSpriteImage(player.spr, &spritesheetfrog, 3, 4);	
	Game->Sprites->SetSpriteAnimation(player.spr, 11, 11, 10);
	SDL_Point tz = Game->Sprites->TileSize(player.spr);
	Vec2F scale = {(playerspeed -8) / tz.x, (playerspeed -8) / tz.y};
	Game->Sprites->SetSpriteScale(player.spr, scale);
	Game->Sprites->SetSpriteCollisionShape(player.spr, SHAPE_BOX, (int)(playerspeed / 3), (int)(playerspeed/3), 0, 0, (int)(playerspeed/12));
	player.tz.x = tz.x * scale.x;
	player.tz.y = tz.y * scale.y;
	player.pos = {screenleft + (numcols / 2) * playerspeed  + playerspeed / 2, screenbottom - playerstartrow * playerspeed + playerspeed / 2};
	HealthPoints = 3;
	Game->Sprites->SetSpriteLocation(player.spr, player.pos);
	playermaxrow = 0;
	playerrow = 0;
	player.alive = true;
}

void CGameFrog::updateplayer()
{
	Game->Sprites->SetSpriteVisibility(player.spr, player.alive);
	if (player.alive)
	{	
		if ((!Game->Input->PrevButtons.ButLeft && Game->Input->Buttons.ButLeft) ||
			(!Game->Input->PrevButtons.ButLeft2 && Game->Input->Buttons.ButLeft2) ||
			(!Game->Input->PrevButtons.ButDpadLeft && Game->Input->Buttons.ButDpadLeft))
		{
			Game->Sprites->SetSpriteAnimation(player.spr, 4, 4, 10); 

			if ((player.pos.x - player.tz.x / 2) - playerspeed >= screenleft)
			{
				if ((player.pos.x - player.tz.x / 2) - playerspeed >= 0)
				{
					player.pos.x -= playerspeed;
					Game->Audio->PlaySound(SfxPlayerMove, 0);
				}
			}
		}

		if ((!Game->Input->PrevButtons.ButRight && Game->Input->Buttons.ButRight) ||
			(!Game->Input->PrevButtons.ButRight2 && Game->Input->Buttons.ButRight2) ||
			(!Game->Input->PrevButtons.ButDpadRight && Game->Input->Buttons.ButDpadRight))
		{
			Game->Sprites->SetSpriteAnimation(player.spr, 7, 7, 10); 

			if ((player.pos.x + player.tz.x / 2) + playerspeed <= screenright)
			{
				if ((player.pos.x + player.tz.x / 2) + playerspeed <= ScreenWidth)
				{
					player.pos.x += playerspeed;
					Game->Audio->PlaySound(SfxPlayerMove, 0);
				}
			}
		}

		if ((!Game->Input->PrevButtons.ButUp && Game->Input->Buttons.ButUp) ||
			(!Game->Input->PrevButtons.ButUp2 && Game->Input->Buttons.ButUp2) ||
			(!Game->Input->PrevButtons.ButDpadUp && Game->Input->Buttons.ButDpadUp))
		{
			Game->Sprites->SetSpriteAnimation(player.spr, 11, 11, 10);

			if ((player.pos.y - player.tz.y / 2) - playerspeed >= screentop)
			{
				if ((player.pos.y - player.tz.y / 2) - playerspeed >= 0)
				{
					player.pos.y -= playerspeed;
					playerrow += 1;
					if (playerrow > playermaxrow)
					{
						Game->AddToScore(20);
						playermaxrow = playerrow;
					}
					Game->Audio->PlaySound(SfxPlayerMove, 0);
				}
			}
		}

		if ((!Game->Input->PrevButtons.ButDown && Game->Input->Buttons.ButDown) ||
			(!Game->Input->PrevButtons.ButDown2 && Game->Input->Buttons.ButDown2) ||
			(!Game->Input->PrevButtons.ButDpadDown && Game->Input->Buttons.ButDpadDown))
		{
			Game->Sprites->SetSpriteAnimation(player.spr, 2, 2, 10);

			if ((player.pos.y + player.tz.y / 2) + playerspeed <= screenbottom)
			{
				if ((player.pos.y + player.tz.y / 2) + playerspeed <= ScreenHeight)
				{
					player.pos.y += playerspeed;
					playerrow -= 1;
				}
				Game->Audio->PlaySound(SfxPlayerMove, 0);
			}
		}

		player.pos.y = player.pos.y + worldspeed + levelincspeeds[level-1];
//		Game->Sprites->SetSpriteLocation(player.spr, player.pos)
//		player.spr.x = int(player.spr.x)
//		player.spr.y = int(player.spr.y)
		player.spr->x = (int)player.pos.x;
		player.spr->y = (int)player.pos.y;

		if (player.pos.y >= screenbottom)
			playerdeath = true;
	}
	else
	{
		if (player.freeze > 0)
			player.freeze -= 1;
		else
			player.alive = true;
	}
}

//background ----------------------------------------------------------------------------------------------------------------

void CGameFrog::DrawBackground(bool motionblur)
{
    Vec2F Scale = {(float)ScreenWidth / backgroundtz.x, (float)ScreenHeight / backgroundtz.x};
	SDL_Point Pos = { 0, 0};
	SDL_Rect Rect = {0, 0, (int)(screenleft / Scale.x), (int)(ScreenHeight / Scale.y)};
	Game->Image->DrawImageFuzeSrcRectTintFloat(Game->Renderer, background, &Rect, false, &Pos, 0, &Scale, 1, 1, 1, 1);
	Pos = { screenright, 0};
	Rect = {(int)(screenright / Scale.x), 0, (int)((ScreenWidth - screenright) / Scale.x), (int)(ScreenHeight / Scale.y)};
	Game->Image->DrawImageFuzeSrcRectTintFloat(Game->Renderer, background, &Rect, false, &Pos, 0, &Scale, 1, 1, 1, 1);
	Rect = {screenleft,0,1, ScreenHeight};
	SDL_SetRenderDrawColor(Game->Renderer, 128,128,128,255);
	SDL_RenderDrawRect(Game->Renderer, &Rect);
	Rect = {screenright,0,1, ScreenHeight};
	SDL_RenderDrawRect(Game->Renderer, &Rect);
}

//init - deinit ----------------------------------------------------------------------------------------------------------------

void CGameFrog::init()
{   
	LoadGraphics();
	level = 1;
	dolevelinc = false;
	rowsspawned = 0;
	plantsspawned = 0;
	numobjects = 0;	
	worldspeed = globalworldspeed;
	createplayer();
	createobjects(true);
	if(!ScreenshotMode)
	{
		LoadSound();
		Game->CurrentGameMusicID = MusMusic;
		Game->Audio->PlayMusic(MusMusic, -1);
	}
}

void CGameFrog::deinit()
{
	destroyplayer();
	destroyallobjects();
	if (!ScreenshotMode)
	{
		UnLoadSound();
		Game->SubStateCounter = 0;
		Game->SubGameState = SGNone;
		Game->CurrentGameMusicID = -1;
	}
	UnloadGraphics();
}

void CGameFrog::LoadSound()
{
	SfxDie = Game->Audio->LoadSound("common/die.wav");
	SfxCollect = Game->Audio->LoadSound("common/coin.wav");
	SfxPlayerMove = Game->Audio->LoadSound("frog/move.wav");
	MusMusic = Game->Audio->LoadMusic("frog/music.ogg");
}

void CGameFrog::UnLoadSound()
{
	Game->Audio->StopMusic();
	Game->Audio->StopSound();
	Game->Audio->UnLoadMusic(MusMusic);
	Game->Audio->UnLoadSound(SfxDie);
	Game->Audio->UnLoadSound(SfxCollect);
	Game->Audio->UnLoadSound(SfxPlayerMove);
}

void CGameFrog::LoadGraphics()
{
	background = Game->Image->LoadImage(Game->Renderer, "frog/background.png");
    backgroundtz = Game->Image->ImageSize(background);
	spritesheetfrog = Game->Image->LoadImage(Game->Renderer, "frog/player.png");
	spritesheetbackground = Game->Image->LoadImage(Game->Renderer, "frog/watergrass.png");
	spritesheetcar1 = Game->Image->LoadImage(Game->Renderer, "frog/carblue.png");
	spritesheetcar2 = Game->Image->LoadImage(Game->Renderer, "frog/garbagetruck.png");
	spritesheetcar3 = Game->Image->LoadImage(Game->Renderer, "frog/ambulance.png");
	spritesheetcar4 = Game->Image->LoadImage(Game->Renderer, "frog/taxi.png");
	spritesheetcar5 = Game->Image->LoadImage(Game->Renderer, "frog/police.png");

	spritesheetplant = Game->Image->LoadImage(Game->Renderer, "frog/waterplant.png");
	spritesheetfruit1 = Game->Image->LoadImage(Game->Renderer, "frog/apple.png");
	spritesheetfruit2 = Game->Image->LoadImage(Game->Renderer, "frog/lemon.png");
	spritesheetfruit3 = Game->Image->LoadImage(Game->Renderer, "frog/cherry.png") ;   
}

void CGameFrog::UnloadGraphics()
{
	Game->Image->UnLoadImage(background);
	Game->Image->UnLoadImage(spritesheetfrog);
	Game->Image->UnLoadImage(spritesheetbackground);
	Game->Image->UnLoadImage(spritesheetcar1);
	Game->Image->UnLoadImage(spritesheetcar2);
	Game->Image->UnLoadImage(spritesheetcar3);
	Game->Image->UnLoadImage(spritesheetcar4);
	Game->Image->UnLoadImage(spritesheetcar5);
	Game->Image->UnLoadImage(spritesheetplant);
	Game->Image->UnLoadImage(spritesheetfruit1);
	Game->Image->UnLoadImage(spritesheetfruit2);
	Game->Image->UnLoadImage(spritesheetfruit3);
    
}

SDL_Texture* CGameFrog::screenshot()
{
	SDL_Texture* prev = SDL_GetRenderTarget(Game->Renderer);
	SDL_Texture* image = SDL_CreateTexture(Game->Renderer, PixelFormat, SDL_TEXTUREACCESS_TARGET, ScreenWidth, ScreenHeight);
    SDL_SetRenderTarget(Game->Renderer, image);
	SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 255);
	SDL_RenderClear(Game->Renderer);
	init();
    
    worldspeed = playerspeed;
	for (int i = 0; i < 5; i++)
	{
		updateplayer();
		updateobjects();
		Game->Sprites->UpdateSprites();
	}	
	Draw();
	
	SDL_RenderPresent(Game->Renderer);
	SDL_SetRenderTarget(Game->Renderer, prev);
	deinit();	
	return image;
}

//Update ----------------------------------------------------------------------------------------------------------------

void CGameFrog::OnGameStart()
{
	playerdeath = false;
}

void CGameFrog::UpdateLogic()
{
    CGameBase::UpdateLogic();
	if (Game->SubGameState == SGGame)
	{
		updateplayer();
		//needs to be done after player update and before object update
		//object update checks collisions etc and thus also player death
		//it does this before adjusting worldspeed to y value
		//so if not done here weird collisions and false deaths happen
		if (dolevelinc)
		{
			if (level-1 < lenlevelincspeeds - 1)
			{
				level += 1;
				dolevelinc = false;
			}
		}

		updateobjects();
		Game->Sprites->UpdateSprites();


		if (playerdeath)
		{		
			Game->Audio->PlaySound(SfxDie, 0);
			Game->AddToScore(-150);
			if(HealthPoints > 1)
			{
				destroyallobjects();
				destroyplayer();
				createobjects(true);
				createplayer();
				HealthPoints = HealthPoints - 1;
				Game->SubGameState = SGReadyGo;
				Game->SubStateTime = SDL_GetTicks() + 500;
			}
			if(Game->GameMode == GMGame)
				HealthPoints -= 1;
		}
	}
}

void CGameFrog::Draw()
{
	Game->Sprites->DrawSprites(Game->Renderer);
	DrawBackground(false);
	 if(!ScreenshotMode)
    {
		DrawScoreBar();
		DrawSubStateText();
	}
}
