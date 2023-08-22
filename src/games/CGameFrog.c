#include <SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include "CGameFrog.h"
#include "../CGame.h"
#include "../Common.h"
#include "../Vec2F.h"




CGameFrog* Create_CGameFrog()
{
	CGameFrog* GameFrog = (CGameFrog*) malloc(sizeof(CGameFrog));
	GameFrog->GameBase = Create_CGameBase(GSFrog, true);
	GameFrog->MusMusic = -1;
	GameFrog->SfxDie = -1;
	GameFrog->SfxCollect = -1;
	GameFrog->SfxPlayerMove = -1;
	GameFrog->prevrowtype = -1;
	GameFrog->GameBase->playfieldwidth = CGameFrog_numcols * CGameFrog_playerspeed;
	GameFrog->GameBase->playfieldheight = CGameFrog_numrows * CGameFrog_playerspeed;
	GameFrog->GameBase->screenleft = (ScreenWidth - GameFrog->GameBase->playfieldwidth) / 2;
	GameFrog->GameBase->screenright = GameFrog->GameBase->screenleft + GameFrog->GameBase->playfieldwidth;
	GameFrog->GameBase->screentop = ScreenHeight - GameFrog->GameBase->playfieldheight;
	GameFrog->GameBase->screenbottom = ScreenHeight;


	GameFrog->rowtypes[0].type = CGameFrog_rowtypewater;
	GameFrog->rowtypes[0].backgroundtile = 5;
	GameFrog->rowtypes[0].backgroundtileend = 9;
	GameFrog->rowtypes[0].deadlyground = true;
	GameFrog->rowtypes[0].id = CGameFrog_idenemyplant;
	GameFrog->rowtypes[0].speed = 1.0f*yscale;
	GameFrog->rowtypes[0].minspace = 2;
	GameFrog->rowtypes[0].maxrepeats = 4;

	GameFrog->rowtypes[1].type = CGameFrog_rowtypewater;
	GameFrog->rowtypes[1].backgroundtile = 5;
	GameFrog->rowtypes[1].backgroundtileend = 9;
	GameFrog->rowtypes[1].deadlyground = true;
	GameFrog->rowtypes[1].id = CGameFrog_idenemyplant;
	GameFrog->rowtypes[1].speed = 2.0f*yscale;
	GameFrog->rowtypes[1].minspace = 3;
	GameFrog->rowtypes[1].maxrepeats = 2;

	GameFrog->rowtypes[2].type = CGameFrog_rowtypewater;
	GameFrog->rowtypes[2].backgroundtile = 5;
	GameFrog->rowtypes[2].backgroundtileend = 9;
	GameFrog->rowtypes[2].deadlyground = true;
	GameFrog->rowtypes[2].id = CGameFrog_idenemyplant;
	GameFrog->rowtypes[2].speed = 3.0f*yscale;
	GameFrog->rowtypes[2].minspace = 4;
	GameFrog->rowtypes[2].maxrepeats = 3;

	GameFrog->rowtypes[3].type = CGameFrog_rowtyperoad;
	GameFrog->rowtypes[3].backgroundtile = 0;
	GameFrog->rowtypes[3].backgroundtileend = 0;
	GameFrog->rowtypes[3].deadlyground = false;
	GameFrog->rowtypes[3].id = CGameFrog_idenemycar1;
	GameFrog->rowtypes[3].speed = 2.0f*yscale;
	GameFrog->rowtypes[3].minspace = 4;
	GameFrog->rowtypes[3].maxrepeats = 2;

	GameFrog->rowtypes[4].type = CGameFrog_rowtyperoad;
	GameFrog->rowtypes[4].backgroundtile = 0;
	GameFrog->rowtypes[4].backgroundtileend = 0;
	GameFrog->rowtypes[4].deadlyground = false;
	GameFrog->rowtypes[4].id = CGameFrog_idenemycar2;
	GameFrog->rowtypes[4].speed = 1.0f*yscale;
	GameFrog->rowtypes[4].minspace = 3;
	GameFrog->rowtypes[4].maxrepeats = 2;

	GameFrog->rowtypes[5].type = CGameFrog_rowtyperoad;
	GameFrog->rowtypes[5].backgroundtile = 0;
	GameFrog->rowtypes[5].backgroundtileend = 0;
	GameFrog->rowtypes[5].deadlyground = false;
	GameFrog->rowtypes[5].id = CGameFrog_idenemycar3;
	GameFrog->rowtypes[5].speed = 7.0f*yscale;
	GameFrog->rowtypes[5].minspace = CGameFrog_numrows;
	GameFrog->rowtypes[5].maxrepeats = 1;

	GameFrog->rowtypes[6].type = CGameFrog_rowtyperoad;
	GameFrog->rowtypes[6].backgroundtile = 0;
	GameFrog->rowtypes[6].backgroundtileend = 0;
	GameFrog->rowtypes[6].deadlyground = false;
	GameFrog->rowtypes[6].id = CGameFrog_idenemycar4;
	GameFrog->rowtypes[6].speed = 1.5f*yscale;
	GameFrog->rowtypes[6].minspace = 4;
	GameFrog->rowtypes[6].maxrepeats = 2;
	
	GameFrog->rowtypes[7].type = CGameFrog_rowtyperoad;
	GameFrog->rowtypes[7].backgroundtile = 0;
	GameFrog->rowtypes[7].backgroundtileend = 0;
	GameFrog->rowtypes[7].deadlyground = false;
	GameFrog->rowtypes[7].id = CGameFrog_idenemycar5;
	GameFrog->rowtypes[7].speed = 2.5f*yscale;
	GameFrog->rowtypes[7].minspace = 3;
	GameFrog->rowtypes[7].maxrepeats = 2;
	
	GameFrog->rowtypes[8].type = CGameFrog_rowtypesafety;
	GameFrog->rowtypes[8].backgroundtile = 10;
	GameFrog->rowtypes[8].backgroundtileend = 10;
	GameFrog->rowtypes[8].deadlyground = false;
	GameFrog->rowtypes[8].id = CGameFrog_idnone;
	GameFrog->rowtypes[8].speed = 2.0f*yscale;
	GameFrog->rowtypes[8].minspace = 2;
	GameFrog->rowtypes[8].maxrepeats = 2;

	GameFrog->levelincspeeds[0] = 0.0f;
	GameFrog->levelincspeeds[1] = 0.25f*yscale;
	GameFrog->levelincspeeds[2] = 0.5f*yscale;
	GameFrog->levelincspeeds[3] = 1.0f*yscale;
	GameFrog->levelincspeeds[4] = 1.5f*yscale;

	GameFrog->levelincspawns[0] = 30;
	GameFrog->levelincspawns[1] = 90;
	GameFrog->levelincspawns[2] = 240;
	GameFrog->levelincspawns[3] = 570;
	GameFrog->levelincspawns[4] = 0;


	for (int i= 0; i < CGameFrog_maxobjects; i++)
		Initialize_CSpriteObject(&GameFrog->objects[i]);
	Initialize_CSpriteObject(&GameFrog->player);


	GameFrog->updateplayer = CGameFrog_updateplayer;
	GameFrog->createplayer = CGameFrog_createplayer;
	GameFrog->destroyplayer = CGameFrog_destroyplayer;
	GameFrog->updateobjects = CGameFrog_updateobjects;
	GameFrog->createobjectrow = CGameFrog_createobjectrow;
	GameFrog->createobject = CGameFrog_createobject;
	GameFrog->destroyobject = CGameFrog_destroyobject;
	GameFrog->destroyallobjects = CGameFrog_destroyallobjects;
	GameFrog->createobjects = CGameFrog_createobjects;
	GameFrog->OnGameStart = CGameFrog_OnGameStart;
	GameFrog->init = CGameFrog_init;
	GameFrog->deinit = CGameFrog_deinit;
	GameFrog->UpdateLogic = CGameFrog_UpdateLogic;
	GameFrog->Draw = CGameFrog_Draw;
	GameFrog->UnloadGraphics = CGameFrog_UnloadGraphics;
	GameFrog->LoadGraphics = CGameFrog_LoadGraphics;
	GameFrog->LoadSound = CGameFrog_LoadSound;
	GameFrog->UnLoadSound = CGameFrog_UnLoadSound;
	GameFrog->DrawBackground = CGameFrog_DrawBackground;
	return GameFrog;
}

void Destroy_CGameFrog(CGameFrog* GameFrog)
{
	Destroy_CGameBase(GameFrog->GameBase);
	free(GameFrog);
}


//objects ----------------------------------------------------------------------------------------------------------------

void CGameFrog_createobjects(CGameFrog* GameFrog, bool initialize)
{
	int rowtype = GameFrog->prevrowtype;
	int i = 0;
	int r = 0;

	if (initialize)
	{
		GameFrog->nextrowtype = CGameFrog_rowtyperoad;
		GameFrog->plantsspawned = 0;
		GameFrog->nextrowtypecountmax = rand() % (CGameFrog_maxrowsbeforesafetyrow+1);
		GameFrog->nextrowtypecount = 0;
		while (i < CGameFrog_numrows)
		{
			if (i < CGameFrog_playerstartrow)
				GameFrog->createobjectrow(GameFrog,i, CGameFrog_lenrowtypes - 1);
			else
			{
				if (GameFrog->nextrowtype == CGameFrog_rowtyperoad)
				{
					//while rowtype == GameFrog->prevrowtype loop
					rowtype = CGameFrog_rowtyperoadstart + rand() % (CGameFrog_rowtyperoadend - CGameFrog_rowtyperoadstart + 1);
					if (rowtype == GameFrog->prevrowtype)
					{
						rowtype += 1;
						if (rowtype > CGameFrog_rowtyperoadend)
							rowtype = CGameFrog_rowtyperoadstart;
					}
					GameFrog->prevrowtype = rowtype;
				}
				else
				{
					if (GameFrog->nextrowtype == CGameFrog_rowtypewater)
					{
						rowtype = CGameFrog_rowtypewaterstart + rand() % (CGameFrog_rowtypewaterend - CGameFrog_rowtypewaterstart + 1);
						if (rowtype == GameFrog->prevrowtype)
						{
							rowtype += 1;
							if (rowtype > CGameFrog_rowtypewaterend)
								rowtype = CGameFrog_rowtypewaterstart;
						}
					}

					GameFrog->prevrowtype = rowtype;
				}

				if(GameFrog->nextrowtypecount == GameFrog->nextrowtypecountmax)
				{
					rowtype = CGameFrog_lenrowtypes -1;
					GameFrog->nextrowtypecountmax = 2+(rand() %(CGameFrog_maxrowsbeforesafetyrow-1));
					GameFrog->nextrowtypecount = 0;

					r = rand() % 2;

					if (r == 0)
						GameFrog->nextrowtype = CGameFrog_rowtyperoad;
					else
					{
						if (r == 1)
							GameFrog->nextrowtype = CGameFrog_rowtypewater;

					}
				}
				else
					GameFrog->nextrowtypecount += 1;

				GameFrog->createobjectrow(GameFrog,i, rowtype);

			}
			i+=1;

		}
	}
	else
	{
		i = CGameFrog_visiblerows;

		while (i < CGameFrog_numrows)
		{
			if (GameFrog->nextrowtype == CGameFrog_rowtyperoad)
			{
				rowtype = CGameFrog_rowtyperoadstart + (rand() % (CGameFrog_rowtyperoadend - CGameFrog_rowtyperoadstart + 1));
				if (rowtype == GameFrog->prevrowtype)
				{
					rowtype += 1;
					if (rowtype > CGameFrog_rowtyperoadend)
						rowtype = CGameFrog_rowtyperoadstart;
				}
				GameFrog->prevrowtype = rowtype;
			}
			else
			{
				if(GameFrog->nextrowtype == CGameFrog_rowtypewater)
				{
					rowtype = CGameFrog_rowtypewaterstart + (rand() % (CGameFrog_rowtypewaterend - CGameFrog_rowtypewaterstart + 1));
					if(rowtype == GameFrog->prevrowtype)
					{
						rowtype += 1;
						if (rowtype > CGameFrog_rowtypewaterend)
							rowtype = CGameFrog_rowtypewaterstart;
					}

					GameFrog->prevrowtype = rowtype;
				}
			}

			if(GameFrog->nextrowtypecount == GameFrog->nextrowtypecountmax)
			{
				rowtype = CGameFrog_lenrowtypes -1;
				GameFrog->nextrowtypecountmax = 2+(rand() % (CGameFrog_maxrowsbeforesafetyrow-1));
				//for debugging finding max objects count
				//GameFrog->nextrowtypecountmax = 4
				GameFrog->nextrowtypecount = 0;
				r = rand() % 2;
				if (r == 0)
					GameFrog->nextrowtype = CGameFrog_rowtyperoad;
				else
				{
					if (r == 1)
						GameFrog->nextrowtype = CGameFrog_rowtypewater;
				}
			}
			else
				GameFrog->nextrowtypecount += 1;
			GameFrog->createobjectrow(GameFrog,i, rowtype);
			i += 1;
		}
	}
}

void CGameFrog_destroyallobjects(CGameFrog* GameFrog)
{
	for (int i = 0; i < CGameFrog_maxobjects; i++)
		GameFrog->destroyobject(GameFrog,i);
}

void CGameFrog_destroyobject(CGameFrog* GameFrog, int index)
{
	if (GameFrog->objects[index].alive)
	{
		CSprites_RemoveSprite(GameFrog->objects[index].spr);
		GameFrog->objects[index].alive = false;
		GameFrog->numobjects -=1;
	}
}

int CGameFrog_createobject(CGameFrog* GameFrog, int rownr, int col, int id, int arowtype, float speed, int dir, int startobjectindex)
{
	SDL_Point tz = {1,1};
	Vec2F scale;
	Vec2F visualscale;
	int tilenr;
	int endtilenr;
	CSpriteObject *object;
	int result = 0;

	for (int i = startobjectindex; i < CGameFrog_maxobjects; i++)
	{
		if (!GameFrog->objects[i].alive)
		{
			object = &(GameFrog->objects)[i];
			Initialize_CSpriteObject(object);

			object->spr = CSprites_CreateSprite();

			if ((id == CGameFrog_idwater) || (id == CGameFrog_idroad) || (id == CGameFrog_idgrass))
			{
				tilenr = GameFrog->rowtypes[arowtype].backgroundtile;
				endtilenr = GameFrog->rowtypes[arowtype].backgroundtileend;
				CSprites_SetSpriteImageTiles(Renderer,object->spr, &GameFrog->spritesheetbackground,5,3);
				tz = CSprites_TileSize(object->spr);
				CSprites_SetSpriteAnimation(object->spr, tilenr, endtilenr, 6);
				CSprites_SetSpriteDepth(object->spr, 0);
				//col == 0 is hack to have less drawing calls for the sprites
				//i made the road water and grass tile equal in size as the width needed for a row
				//so we only need the 1st tile to be visible
				//however i was not able to not create the other sprite parts somehow
				//it introduced a bug with row generating and i have not figured out yet
				//whats causing that so this is a nasty workaround but still have an optimazation
				//of only doing 1 big draw call for the road / water per row instead of multiple small ones
				CSprites_SetSpriteVisibility(object->spr, col == 0);
				object->vel.x = 0.0f;
				object->vel.y = 0.0f;
				//scale.x = {CGameFrog_playerspeed / tz.x, CGameFrog_playerspeed / tz.y}
				scale.x = 1.0f;
				scale.y = 1.0f;
				visualscale = scale;
				object->tz.x = tz.x * scale.x;
				object->tz.y = tz.y * scale.y;
			}

			if (id == CGameFrog_idenemyplant)
			{
				tilenr = 1;
				CSprites_SetSpriteImageTiles(Renderer,object->spr, &GameFrog->spritesheetplant, 2, 1);
				tz = CSprites_TileSize(object->spr);

				CSprites_SetSpriteAnimation(object->spr, tilenr, tilenr, 0);
				CSprites_SetSpriteDepth(object->spr, 1);
				//scale = {(CGameFrog_playerspeed - int(CGameFrog_playerspeed / 48)) / tz.x, (CGameFrog_playerspeed - int(CGameFrog_playerspeed / 48)) / tz.y}
				scale.x = CGameFrog_playerspeed / tz.x;
				scale.y = CGameFrog_playerspeed / tz.y;
				visualscale = scale;
				object->vel.x = dir*speed;
				object->vel.y = 0.0f;
				object->tz.x = tz.x * scale.x;
				object->tz.y = tz.y * scale.y;
			}

			if (id == CGameFrog_idenemycar1)
			{
				tilenr = 0;
				if (dir == -1)
					tilenr = 1;

				CSprites_SetSpriteImageTiles(Renderer,object->spr, &GameFrog->spritesheetcar1, 2, 1);
				tz = CSprites_TileSize(object->spr);
				CSprites_SetSpriteAnimation(object->spr, tilenr, tilenr, 0);
				CSprites_SetSpriteDepth(object->spr, 3);
				object->vel.x = dir*speed;
				object->vel.y = 0.0f;
				scale.x = (CGameFrog_playerspeed - (CGameFrog_playerspeed / 6)) / tz.x;
				scale.y = (CGameFrog_playerspeed - (CGameFrog_playerspeed / 6)) / tz.y;
				visualscale = scale;
				object->tz.x = tz.x * scale.x;
				object->tz.y = tz.y * scale.y;
			}

			if (id == CGameFrog_idenemycar2)
			{
				tilenr = 0;
				if (dir == -1)
					tilenr = 1;

				CSprites_SetSpriteImageTiles(Renderer,object->spr, &GameFrog->spritesheetcar2, 2, 1);
				tz = CSprites_TileSize(object->spr);
				CSprites_SetSpriteAnimation(object->spr, tilenr, tilenr, 0);
				CSprites_SetSpriteDepth(object->spr, 3);
				object->vel.x = dir*speed;
				object->vel.y = 0.0f;
				scale.x = (CGameFrog_playerspeed - (CGameFrog_playerspeed / 6)) / tz.x;
				scale.y = (CGameFrog_playerspeed - (CGameFrog_playerspeed / 6)) / tz.y;
				visualscale = scale;
				object->tz.x = tz.x * scale.x;
				object->tz.y = tz.y * scale.y;
			}

			if (id == CGameFrog_idenemycar3)
			{
				tilenr = 0;
				if (dir == -1)
					tilenr = 1;

				CSprites_SetSpriteImageTiles(Renderer,object->spr, &GameFrog->spritesheetcar3, 2, 1);
				tz = CSprites_TileSize(object->spr);
				CSprites_SetSpriteAnimation(object->spr, tilenr, tilenr, 0);
				CSprites_SetSpriteDepth(object->spr, 3);
				object->vel.x = dir*speed;
				object->vel.y = 0.0f;
				scale.x = (CGameFrog_playerspeed - (CGameFrog_playerspeed / 6)) / tz.x;
				scale.y = (CGameFrog_playerspeed - (CGameFrog_playerspeed / 6)) / tz.y;
				visualscale = scale;
				object->tz.x = tz.x * scale.x;
				object->tz.y = tz.y * scale.y;
			}

			if (id == CGameFrog_idenemycar4)
			{
				tilenr = 0;
				if (dir == -1)
					tilenr = 1;

				CSprites_SetSpriteImageTiles(Renderer,object->spr, &GameFrog->spritesheetcar4, 2, 1);
				tz = CSprites_TileSize(object->spr);
				CSprites_SetSpriteAnimation(object->spr, tilenr, tilenr, 0);
				CSprites_SetSpriteDepth(object->spr, 3);
				object->vel.x = dir*speed;
				object->vel.y = 0.0f;
				scale.x = (CGameFrog_playerspeed - (CGameFrog_playerspeed / 6)) / tz.x;
				scale.y = (CGameFrog_playerspeed - (CGameFrog_playerspeed / 6)) / tz.y;
				visualscale = scale;
				object->tz.x = tz.x * scale.x;
				object->tz.y = tz.y * scale.y;
			}

			if (id == CGameFrog_idenemycar5)
			{
				tilenr = 0;
				if (dir == -1)
					tilenr = 1;

				CSprites_SetSpriteImageTiles(Renderer,object->spr, &GameFrog->spritesheetcar5, 2, 1);
				tz = CSprites_TileSize(object->spr);
				CSprites_SetSpriteAnimation(object->spr, tilenr, tilenr, 0);
				CSprites_SetSpriteDepth(object->spr, 3);
				object->vel.x = dir*speed;
				object->vel.y = 0.0f;
				scale.x = (CGameFrog_playerspeed - (CGameFrog_playerspeed / 6)) / tz.x;
				scale.y = (CGameFrog_playerspeed - (CGameFrog_playerspeed / 6)) / tz.y;
				visualscale = scale;
				object->tz.x = tz.x * scale.x;
				object->tz.y = tz.y * scale.y;
			}


			if (id == CGameFrog_idcherry)
			{
				tilenr = 0;

				CSprites_SetSpriteImage(Renderer,object->spr, &GameFrog->spritesheetfruit1);
				tz = CSprites_TileSize(object->spr);
				CSprites_SetSpriteAnimation(object->spr, tilenr, tilenr, 0);
				object->vel.x = dir*speed;
				object->vel.y = 0.0f;
				scale.x = CGameFrog_playerspeed / tz.x;
				scale.y = CGameFrog_playerspeed / tz.y;
				visualscale.x = (CGameFrog_playerspeed - (CGameFrog_playerspeed / 2)) / tz.x;
				visualscale.y = (CGameFrog_playerspeed - (CGameFrog_playerspeed / 2)) / tz.y;
				CSprites_SetSpriteDepth(object->spr, 3);
			}

			if (id == CGameFrog_idlemon)
			{
				tilenr = 0;

				CSprites_SetSpriteImage(Renderer,object->spr, &GameFrog->spritesheetfruit2);
				tz = CSprites_TileSize(object->spr);
				CSprites_SetSpriteAnimation(object->spr, tilenr, tilenr, 0);
				object->vel.x = dir*speed;
				object->vel.y = 0.0f;
				scale.x = CGameFrog_playerspeed / tz.x;
				scale.y = CGameFrog_playerspeed / tz.y;
				visualscale.x = (CGameFrog_playerspeed - (CGameFrog_playerspeed / 2)) / tz.x;
				visualscale.y = (CGameFrog_playerspeed - (CGameFrog_playerspeed / 2)) / tz.y;
				CSprites_SetSpriteDepth(object->spr, 3);
			}

			if (id == CGameFrog_idapple)
			{
				tilenr = 0;

				CSprites_SetSpriteImage(Renderer,object->spr, &GameFrog->spritesheetfruit3);
				tz = CSprites_TileSize(object->spr);
				CSprites_SetSpriteAnimation(object->spr, tilenr, tilenr, 0);
				object->vel.x = dir*speed;
				object->vel.y = 0.0f;
				scale.x = CGameFrog_playerspeed / tz.x;
				scale.y = CGameFrog_playerspeed / tz.y;
				visualscale.x = (CGameFrog_playerspeed - (CGameFrog_playerspeed / 2)) / tz.x;
				visualscale.y = (CGameFrog_playerspeed - (CGameFrog_playerspeed / 2)) / tz.y;
				CSprites_SetSpriteDepth(object->spr, 3);
			}
			//setSpriteRotation(GameFrog->object->s[i].spr, 0)
			CSprites_SetSpriteScale(Renderer,object->spr, visualscale);
			object->tz.x = tz.x * scale.x;
			object->tz.y = tz.y * scale.y;
			object->pos.x = GameFrog->GameBase->screenleft + (col * CGameFrog_playerspeed) + (CGameFrog_playerspeed / 2);
			object->pos.y = GameFrog->GameBase->screenbottom - ((rownr+1) * CGameFrog_playerspeed) + (CGameFrog_playerspeed / 2);
			//CSprites_SetSpriteLocation(object->spr, object->pos)
			object->spr->x = (int)object->pos.x;
			object->spr->y = (int)object->pos.y;
			object->rowtype = arowtype;
			object->id = id;
			object->alive = true;
			GameFrog->numobjects +=1;
			result = i;
			break;
		}
	}
	return result;
}

void CGameFrog_createobjectrow(CGameFrog* GameFrog, int rownr, int arowtype)
{
	int type = GameFrog->rowtypes[arowtype].type;
	int minspace = GameFrog->rowtypes[arowtype].minspace;
	int maxrepeats = 1 + (rand() % (GameFrog->rowtypes[arowtype].maxrepeats));
	int id = GameFrog->rowtypes[arowtype].id;
	int rowtypemaxrepeats = GameFrog->rowtypes[arowtype].maxrepeats;
	int repeats = 0;
	int lastplaced = 0;
	int dir = rand() % 2;
	if (dir == 0)
		dir = -1;

	float speed = (GameFrog->rowtypes[arowtype].speed - CGameFrog_speeddeviation) + ((rand() % (int)(CGameFrog_speeddeviation*100)/100));
	int index = 0;
	for (int x = 0; x < CGameFrog_numcols; x++)
	{
		if (type == CGameFrog_rowtyperoad)
		{
			index = GameFrog->createobject(GameFrog,rownr, x, CGameFrog_idroad, arowtype, 0, 0, index);

			if ((x - lastplaced > minspace) || (repeats < maxrepeats))
			{
				if (x - lastplaced > minspace)
				{
					maxrepeats = 1 + (rand() % rowtypemaxrepeats);
					repeats = 0;
				}
				lastplaced = x;
				repeats += 1;
				index = GameFrog->createobject(GameFrog,rownr, x, id, arowtype, speed, dir, index);
			}
		}
		else
		{
			if (type == CGameFrog_rowtypewater)
			{
				index = GameFrog->createobject(GameFrog,rownr, x, CGameFrog_idwater, arowtype, 0, 0, index);
				if ((x - lastplaced > minspace) || (repeats < maxrepeats))
				{
					if (x - lastplaced > minspace)
					{
						maxrepeats = 1 + (rand()% rowtypemaxrepeats);
						repeats = 0;
					}
					lastplaced = x;
					repeats += 1;
					index = GameFrog->createobject(GameFrog,rownr, x, id, arowtype, speed, dir, index);
					GameFrog->plantsspawned += 1;

					if (GameFrog->plantsspawned % CGameFrog_cherryspawntrigger == 0)
						index = GameFrog->createobject(GameFrog,rownr, x, CGameFrog_idcherry, arowtype, speed, dir, index);

					if (GameFrog->plantsspawned % CGameFrog_lemonspawntrigger == 0)
						index = GameFrog->createobject(GameFrog,rownr, x, CGameFrog_idlemon, arowtype, speed, dir, index);

					if (GameFrog->plantsspawned % CGameFrog_applespawntrigger == 0)
						index = GameFrog->createobject(GameFrog,rownr, x, CGameFrog_idapple, arowtype, speed, dir, index);
				}
			}
			else
			{
				if (type == CGameFrog_rowtypesafety)
					index = GameFrog->createobject(GameFrog,rownr, x, CGameFrog_idgrass, arowtype, 0, 0, index);
			}
		}
	}
}

void CGameFrog_updateobjects(CGameFrog* GameFrog)
{
	GameFrog->objectinfo.mostleft = -1;
	GameFrog->objectinfo.mostright = -1;
	GameFrog->objectinfo.mostbottom = -1;
	GameFrog->objectinfo.mosttop = -1;

	int x1 = GameFrog->GameBase->screenright + 1;
	int x2 = GameFrog->GameBase->screenleft - 1;
	int y1 = GameFrog->GameBase->screentop - 1;
	int y2 = GameFrog->GameBase->screenbottom + 1;
	bool playermoved = false;
	int id = -1;
	bool plantcol = false;
	bool stopcheckplantcol = false;

	for (int i = 0; i < CGameFrog_maxobjects; i++)
	{
		if(GameFrog->objects[i].alive)
		{
			// if ((GameFrog->objects[i].pos.y + 1 > GameFrog->player.pos.y) && (GameFrog->objects[i].pos.y - 1 < GameFrog->player.pos.y))
			// {
			// 	if(GameFrog->objects[i].id == CGameFrog_idgrass)
			// 	{
			// 		printf("P:%f O:%f\n", GameFrog->objects[i].pos.y, GameFrog->player.pos.y);
			// 	}
			//}

			//if (floatequal(GameFrog->objects[i].pos.y,GameFrog->player.pos.y))
			if(true)
			{
				if (CSprites_DetectSpriteCollision(GameFrog->objects[i].spr, GameFrog->player.spr))
				{
					id = GameFrog->objects[i].id;

					if (!playermoved && (id == CGameFrog_idenemyplant) && !GameFrog->playerdeath &&
						(GameFrog->player.pos.x - GameFrog->player.tz.x / 4 <= GameFrog->objects[i].pos.x + GameFrog->objects[i].tz.x / 2) &&
						(GameFrog->player.pos.x + GameFrog->player.tz.x / 4 >= GameFrog->objects[i].pos.x - GameFrog->objects[i].tz.x / 2))
						{
							GameFrog->player.pos = GameFrog->objects[i].pos;
							GameFrog->player.spr->x = (int)GameFrog->player.pos.x;
							GameFrog->player.spr->y = (int)GameFrog->player.pos.y;
							if ((GameFrog->player.pos.x < GameFrog->GameBase->screenleft) || (GameFrog->player.pos.x > GameFrog->GameBase->screenright))
								GameFrog->playerdeath = true;

							playermoved = true;
							plantcol = true;
						}

					if ((id == CGameFrog_idenemycar1) || (id == CGameFrog_idenemycar2) || (id == CGameFrog_idenemycar3) ||
						(id == CGameFrog_idenemycar4) || (id == CGameFrog_idenemycar5))
						GameFrog->playerdeath = true;
					else
					{
						if ((id == CGameFrog_idroad) || (id == CGameFrog_idgrass))
							playermoved = true;
						else
						{
							if (id == CGameFrog_idlemon)
							{
								CGame_AddToScore(CGameFrog_lemonscore);
								GameFrog->destroyobject(GameFrog,i);
								CAudio_PlaySound(GameFrog->SfxCollect, 0);
							}
							else
							{
								if (id == CGameFrog_idapple)
								{
									CGame_AddToScore(CGameFrog_applescore);
									GameFrog->destroyobject(GameFrog,i);
									CAudio_PlaySound(GameFrog->SfxCollect, 0);
								}
								else
								{
									if (id == CGameFrog_idcherry)
									{
										CGame_AddToScore(CGameFrog_cherryscore);
										GameFrog->destroyobject(GameFrog,i);
										CAudio_PlaySound(GameFrog->SfxCollect, 0);
									}
								}
							}
						}
					}


					if ((!stopcheckplantcol) && (!plantcol) && (id == CGameFrog_idwater) && (!GameFrog->playerdeath))
					{
						stopcheckplantcol = true;
						for (int j = i + 1; j < CGameFrog_maxobjects;j++)
						{
							if ((GameFrog->objects[j].alive) /*&& floatequal(GameFrog->objects[j].pos.y,GameFrog->player.pos.y)*/)
							{
								if(CSprites_DetectSpriteCollision(GameFrog->objects[j].spr, GameFrog->player.spr))
								{
									if (id == CGameFrog_idenemyplant)
									{
										if ((GameFrog->player.pos.x - GameFrog->player.tz.x / 4 <= GameFrog->objects[i].pos.x + GameFrog->objects[i].tz.x / 2) &&
											(GameFrog->player.pos.x + GameFrog->player.tz.x / 4 >= GameFrog->objects[i].pos.x - GameFrog->objects[i].tz.x / 2))
										{
											//GameFrog->player.pos = GameFrog->objects[i].pos //GameFrog->player.pos + GameFrog->objects[i].vel
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

			if (GameFrog->objects[i].alive)
			{
				GameFrog->objects[i].pos.y = GameFrog->objects[i].pos.y + GameFrog->worldspeed + GameFrog->levelincspeeds[GameFrog->GameBase->level-1];
				if (GameFrog->objects[i].pos.y - GameFrog->objects[i].tz.y / 2 > GameFrog->GameBase->screenbottom)
					GameFrog->destroyobject(GameFrog,i);
				else
				{
					GameFrog->objects[i].pos.x = GameFrog->objects[i].pos.x + GameFrog->objects[i].vel.x;
					GameFrog->objects[i].pos.y = GameFrog->objects[i].pos.y + GameFrog->objects[i].vel.y;
					if (GameFrog->objects[i].vel.x < 0)
					{
						if (GameFrog->objects[i].pos.x + GameFrog->objects[i].tz.x / 2 < GameFrog->GameBase->screenleft)
							GameFrog->objects[i].pos.x = GameFrog->GameBase->screenright - (GameFrog->GameBase->screenleft - GameFrog->objects[i].pos.x) + GameFrog->objects[i].tz.x / 2 + GameFrog->objects[i].tz.x / 2;
					}
					else
					{
						if (GameFrog->objects[i].pos.x - GameFrog->objects[i].tz.x / 2 > GameFrog->GameBase->screenright)
							GameFrog->objects[i].pos.x = GameFrog->GameBase->screenleft + (GameFrog->objects[i].pos.x - GameFrog->GameBase->screenright) - GameFrog->objects[i].tz.x / 2 - GameFrog->objects[i].tz.x / 2;
					}
					GameFrog->objects[i].spr->x = (int)GameFrog->objects[i].pos.x;
					GameFrog->objects[i].spr->y = (int)GameFrog->objects[i].pos.y;

					if ((GameFrog->objects[i].id == CGameFrog_idwater) || (GameFrog->objects[i].id == CGameFrog_idroad) || (GameFrog->objects[i].id == CGameFrog_idgrass))
					{
						if (GameFrog->objects[i].pos.x < x1)
						{
							x1 = GameFrog->objects[i].pos.x;
							GameFrog->objectinfo.mostleft = i;
						}
						else
						{
							if (GameFrog->objects[i].pos.x > x2)
							{
								x2 = GameFrog->objects[i].pos.x;
								GameFrog->objectinfo.mostright = i;
							}
						}

						if (GameFrog->objects[i].pos.y > y1)
						{
							y1 = GameFrog->objects[i].pos.y;
							GameFrog->objectinfo.mostbottom = i;
						}
						else
						{
							if (GameFrog->objects[i].pos.y < y2)
							{
								y2 = GameFrog->objects[i].pos.y;
								GameFrog->objectinfo.mosttop = i;
							}
						}
					}
				}
			}
		}
	}

	if (GameFrog->objectinfo.mosttop > -1)
	{
		//printf("%f %f (%f - %f)\n", (GameFrog->objects[GameFrog->objectinfo.mosttop].pos.y - GameFrog->objects[GameFrog->objectinfo.mosttop].tz.y / 2), ((GameFrog->GameBase->screenbottom - (CGameFrog_visiblerows * CGameFrog_playerspeed))), ((GameFrog->GameBase->screenbottom - (CGameFrog_visiblerows * CGameFrog_playerspeed))) - epsilion, ((GameFrog->GameBase->screenbottom - (CGameFrog_visiblerows * CGameFrog_playerspeed)))+ epsilion);
		if ((GameFrog->objects[GameFrog->objectinfo.mosttop].pos.y - GameFrog->objects[GameFrog->objectinfo.mosttop].tz.y / 2 <= (GameFrog->GameBase->screenbottom - (CGameFrog_visiblerows * CGameFrog_playerspeed) + epsilion)) &&
			(GameFrog->objects[GameFrog->objectinfo.mosttop].pos.y - GameFrog->objects[GameFrog->objectinfo.mosttop].tz.y / 2 >= (GameFrog->GameBase->screenbottom - (CGameFrog_visiblerows * CGameFrog_playerspeed) - epsilion)))
		{
			//printf("%f %f\n", GameFrog->objects[GameFrog->objectinfo.mosttop].pos.y - (float)GameFrog->objects[GameFrog->objectinfo.mosttop].tz.y / 2.0f , (float)GameFrog->GameBase->screenbottom  - ((float)CGameFrog_visiblerows * CGameFrog_playerspeed));
			GameFrog->createobjects(GameFrog,false);
			GameFrog->rowsspawned += 1;
			if (GameFrog->rowsspawned == GameFrog->levelincspawns[GameFrog->GameBase->level-1])
				GameFrog->dolevelinc = true;
		}
	}

	if (!playermoved)
	{
		GameFrog->playerdeath = true;
	}
}

//player ----------------------------------------------------------------------------------------------------------------

void CGameFrog_destroyplayer(CGameFrog* GameFrog)
{
	CSprites_RemoveSprite(GameFrog->player.spr);
	GameFrog->player.alive = false;
}

void CGameFrog_createplayer(CGameFrog* GameFrog)
{
	GameFrog->player.spr = CSprites_CreateSprite();
	CSprites_SetSpriteDepth(GameFrog->player.spr, 2);
	CSprites_SetSpriteImageTiles(Renderer,GameFrog->player.spr, &GameFrog->spritesheetfrog, 3, 4);
	CSprites_SetSpriteAnimation(GameFrog->player.spr, 11, 11, 10);
	SDL_Point tz = CSprites_TileSize(GameFrog->player.spr);
	Vec2F scale = {((CGameFrog_playerspeed -8*xscale)) / tz.x, ((CGameFrog_playerspeed -8*yscale)) / tz.y};
	CSprites_SetSpriteScale(Renderer,GameFrog->player.spr, scale);
	CSprites_SetSpriteCollisionShape(GameFrog->player.spr, SHAPE_BOX, (int)(CGameFrog_playerspeed / 3.0f), (int)(CGameFrog_playerspeed/3.0f), 0, 0, (int)(CGameFrog_playerspeed/12.0f));
	GameFrog->player.tz.x = tz.x * scale.x;
	GameFrog->player.tz.y = tz.y * scale.y;
	GameFrog->player.pos.x = GameFrog->GameBase->screenleft + (CGameFrog_numcols / 2) * CGameFrog_playerspeed + CGameFrog_playerspeed / 2;
	GameFrog->player.pos.y = GameFrog->GameBase->screenbottom - CGameFrog_playerstartrow * CGameFrog_playerspeed + CGameFrog_playerspeed / 2;
	GameFrog->GameBase->HealthPoints = 3;
	CSprites_SetSpriteLocation(GameFrog->player.spr, GameFrog->player.pos);
	GameFrog->playermaxrow = 0;
	GameFrog->playerrow = 0;
	GameFrog->player.alive = true;
}

void CGameFrog_updateplayer(CGameFrog* GameFrog)
{
	CSprites_SetSpriteVisibility(GameFrog->player.spr, GameFrog->player.alive);
	if (GameFrog->player.alive)
	{
		if ((!CInput_PrevButtons.ButLeft && CInput_Buttons.ButLeft) ||
			(!CInput_PrevButtons.ButLeft2 && CInput_Buttons.ButLeft2) ||
			(!CInput_PrevButtons.ButDpadLeft && CInput_Buttons.ButDpadLeft))
		{
			CSprites_SetSpriteAnimation(GameFrog->player.spr, 4, 4, 10);

			if ((GameFrog->player.pos.x - GameFrog->player.tz.x / 2) - CGameFrog_playerspeed >= GameFrog->GameBase->screenleft)
			{
				if ((GameFrog->player.pos.x - GameFrog->player.tz.x / 2) - CGameFrog_playerspeed >= 0)
				{
					GameFrog->player.pos.x -= CGameFrog_playerspeed;
					CAudio_PlaySound(GameFrog->SfxPlayerMove, 0);
				}
			}
		}

		if ((!CInput_PrevButtons.ButRight && CInput_Buttons.ButRight) ||
			(!CInput_PrevButtons.ButRight2 && CInput_Buttons.ButRight2) ||
			(!CInput_PrevButtons.ButDpadRight && CInput_Buttons.ButDpadRight))
		{
			CSprites_SetSpriteAnimation(GameFrog->player.spr, 7, 7, 10);

			if ((GameFrog->player.pos.x + GameFrog->player.tz.x / 2) + CGameFrog_playerspeed <= GameFrog->GameBase->screenright)
			{
				if ((GameFrog->player.pos.x + GameFrog->player.tz.x / 2) + CGameFrog_playerspeed <= ScreenWidth)
				{
					GameFrog->player.pos.x += CGameFrog_playerspeed;
					CAudio_PlaySound(GameFrog->SfxPlayerMove, 0);
				}
			}
		}

		if ((!CInput_PrevButtons.ButUp && CInput_Buttons.ButUp) ||
			(!CInput_PrevButtons.ButUp2 && CInput_Buttons.ButUp2) ||
			(!CInput_PrevButtons.ButDpadUp && CInput_Buttons.ButDpadUp))
		{
			CSprites_SetSpriteAnimation(GameFrog->player.spr, 11, 11, 10);

			if ((GameFrog->player.pos.y - GameFrog->player.tz.y / 2) - CGameFrog_playerspeed >= GameFrog->GameBase->screentop)
			{
				if ((GameFrog->player.pos.y - GameFrog->player.tz.y / 2) - CGameFrog_playerspeed >= 0)
				{
					GameFrog->player.pos.y -= CGameFrog_playerspeed;
					GameFrog->playerrow += 1;
					if (GameFrog->playerrow > GameFrog->playermaxrow)
					{
						CGame_AddToScore(20);
						GameFrog->playermaxrow = GameFrog->playerrow;
					}
					CAudio_PlaySound(GameFrog->SfxPlayerMove, 0);
				}
			}
		}

		if ((!CInput_PrevButtons.ButDown && CInput_Buttons.ButDown) ||
			(!CInput_PrevButtons.ButDown2 && CInput_Buttons.ButDown2) ||
			(!CInput_PrevButtons.ButDpadDown && CInput_Buttons.ButDpadDown))
		{
			CSprites_SetSpriteAnimation(GameFrog->player.spr, 2, 2, 10);

			if ((GameFrog->player.pos.y + GameFrog->player.tz.y / 2) + CGameFrog_playerspeed <= GameFrog->GameBase->screenbottom)
			{
				if ((GameFrog->player.pos.y + GameFrog->player.tz.y / 2) + CGameFrog_playerspeed <= ScreenHeight)
				{
					GameFrog->player.pos.y += CGameFrog_playerspeed;
					GameFrog->playerrow -= 1;
				}
				CAudio_PlaySound(GameFrog->SfxPlayerMove, 0);
			}
		}

		GameFrog->player.pos.y = GameFrog->player.pos.y + GameFrog->worldspeed + GameFrog->levelincspeeds[GameFrog->GameBase->level-1];
//		CSprites_SetSpriteLocation(GameFrog->player.spr, GameFrog->player.pos)
//		GameFrog->player.spr.x = int(GameFrog->player.spr.x)
//		GameFrog->player.spr.y = int(GameFrog->player.spr.y)
		GameFrog->player.spr->x = (int)GameFrog->player.pos.x;
		GameFrog->player.spr->y = (int)GameFrog->player.pos.y;

		if (GameFrog->player.pos.y >= GameFrog->GameBase->screenbottom)
			GameFrog->playerdeath = true;
	}
	else
	{
		if (GameFrog->player.freeze > 0)
			GameFrog->player.freeze -= 1;
		else
			GameFrog->player.alive = true;
	}
}

//background ----------------------------------------------------------------------------------------------------------------

void CGameFrog_DrawBackground(CGameFrog* GameFrog)
{
	CImage_DrawImage(Renderer, GameFrog->background, NULL, NULL);
}

//init - deinit ----------------------------------------------------------------------------------------------------------------

void CGameFrog_init(CGameFrog* GameFrog)
{
	GameFrog->LoadGraphics(GameFrog);
	GameFrog->GameBase->level = 1;
	GameFrog->dolevelinc = false;
	GameFrog->rowsspawned = 0;
	GameFrog->plantsspawned = 0;
	GameFrog->numobjects = 0;
	GameFrog->worldspeed = CGameFrog_globalworldspeed;
	GameFrog->createplayer(GameFrog);
	GameFrog->createobjects(GameFrog,true);
	GameFrog->LoadSound(GameFrog);
	CurrentGameMusicID = GameFrog->MusMusic;
	CAudio_PlayMusic(GameFrog->MusMusic, -1);
}

void CGameFrog_deinit(CGameFrog* GameFrog)
{
	GameFrog->destroyplayer(GameFrog);
	GameFrog->destroyallobjects(GameFrog);
	GameFrog->UnLoadSound(GameFrog);
	SubStateCounter = 0;
	SubGameState = SGNone;
	CurrentGameMusicID = -1;
	GameFrog->UnloadGraphics(GameFrog);
}

void CGameFrog_LoadSound(CGameFrog* GameFrog)
{
	GameFrog->SfxDie = CAudio_LoadSound("common/die.wav");
	GameFrog->SfxCollect = CAudio_LoadSound("common/coin.wav");
	GameFrog->SfxPlayerMove = CAudio_LoadSound("frog/move.wav");
	GameFrog->MusMusic = CAudio_LoadMusic("frog/music.ogg");
}

void CGameFrog_UnLoadSound(CGameFrog* GameFrog)
{
	CAudio_StopMusic();
	CAudio_StopSound();
	CAudio_UnLoadMusic(GameFrog->MusMusic);
	CAudio_UnLoadSound(GameFrog->SfxDie);
	CAudio_UnLoadSound(GameFrog->SfxCollect);
	CAudio_UnLoadSound(GameFrog->SfxPlayerMove);
}

void CGameFrog_LoadGraphics(CGameFrog* GameFrog)
{
	GameFrog->background = CImage_LoadImage(Renderer, "frog/background.png");
	GameFrog->backgroundtz = CImage_ImageSize(GameFrog->background);


	GameFrog->spritesheetfrog = CImage_LoadImageEx(Renderer, "frog/player.png", 0, 128, dumpScaledBitmaps);
	GameFrog->spritesheetbackground = CImage_LoadImageEx(Renderer, "frog/watergrass.png", 0, 128, dumpScaledBitmaps);
	GameFrog->spritesheetcar1 = CImage_LoadImageEx(Renderer, "frog/carblue.png", 0, 80, dumpScaledBitmaps);
	GameFrog->spritesheetcar2 = CImage_LoadImageEx(Renderer, "frog/garbagetruck.png", 0, 80, dumpScaledBitmaps);
	GameFrog->spritesheetcar3 = CImage_LoadImageEx(Renderer, "frog/ambulance.png", 0, 80, dumpScaledBitmaps);
	GameFrog->spritesheetcar4 = CImage_LoadImageEx(Renderer, "frog/taxi.png", 0, 80, dumpScaledBitmaps);
	GameFrog->spritesheetcar5 = CImage_LoadImageEx(Renderer, "frog/police.png", 0, 80, dumpScaledBitmaps);
	GameFrog->spritesheetplant = CImage_LoadImageEx(Renderer, "frog/waterplant.png", 0, 140, dumpScaledBitmaps);
	GameFrog->spritesheetfruit1 = CImage_LoadImageEx(Renderer, "frog/apple.png",0, 10, dumpScaledBitmaps);
	GameFrog->spritesheetfruit2 = CImage_LoadImageEx(Renderer, "frog/lemon.png",0, 10, dumpScaledBitmaps);
	GameFrog->spritesheetfruit3 = CImage_LoadImageEx(Renderer, "frog/cherry.png",0, 10, dumpScaledBitmaps);

	// SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/frog/player.bmp", CImage_GetImage(GameFrog->spritesheetfrog), 1,1, true, 0, 128);
	// SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/frog/watergrass.bmp", CImage_GetImage(GameFrog->spritesheetbackground), 1,1, true, 0, 128);
	// SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/frog/carblue.bmp", CImage_GetImage(GameFrog->spritesheetcar1), 1,1, true, 0, 80);
	// SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/frog/garbagetruck.bmp", CImage_GetImage(GameFrog->spritesheetcar2), 1,1, true, 0, 80);
	// SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/frog/ambulance.bmp", CImage_GetImage(GameFrog->spritesheetcar3), 1,1, true,0,  80);
	// SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/frog/taxi.bmp", CImage_GetImage(GameFrog->spritesheetcar4), 1,1, true, 0, 80);
	// SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/frog/police.bmp", CImage_GetImage(GameFrog->spritesheetcar5), 1,1, true, 0, 80);
	// SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/frog/waterplant.bmp", CImage_GetImage(GameFrog->spritesheetplant), 1,1, true,0, 140); //173
	// SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/frog/apple.bmp", CImage_GetImage(GameFrog->spritesheetfruit1), 1,1, true,0, 10);
	// SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/frog/lemon.bmp", CImage_GetImage(GameFrog->spritesheetfruit2), 1,1, true,0, 10);
	// SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/frog/cherry.bmp", CImage_GetImage(GameFrog->spritesheetfruit3), 1,1, true,0, 10);
	if(!useDefaultColorAssets)
	{
		GameFrog->UnloadGraphics(GameFrog);
		GameFrog->background = CImage_LoadImage(Renderer, "frog/background.png");
		GameFrog->backgroundtz = CImage_ImageSize(GameFrog->background);
		GameFrog->spritesheetfrog = CImage_LoadImage(Renderer, "frog/player.bmp");
		GameFrog->spritesheetbackground = CImage_LoadImage(Renderer, "frog/watergrass.bmp");
		GameFrog->spritesheetcar1 = CImage_LoadImage(Renderer, "frog/carblue.bmp");
		GameFrog->spritesheetcar2 = CImage_LoadImage(Renderer, "frog/garbagetruck.bmp");
		GameFrog->spritesheetcar3 = CImage_LoadImage(Renderer, "frog/ambulance.bmp");
		GameFrog->spritesheetcar4 = CImage_LoadImage(Renderer, "frog/taxi.bmp");
		GameFrog->spritesheetcar5 = CImage_LoadImage(Renderer, "frog/police.bmp");
		GameFrog->spritesheetplant = CImage_LoadImage(Renderer, "frog/waterplant.bmp");
		GameFrog->spritesheetfruit1 = CImage_LoadImage(Renderer, "frog/apple.bmp");
		GameFrog->spritesheetfruit2 = CImage_LoadImage(Renderer, "frog/lemon.bmp");
		GameFrog->spritesheetfruit3 = CImage_LoadImage(Renderer, "frog/cherry.bmp");
	}

}

void CGameFrog_UnloadGraphics(CGameFrog* GameFrog)
{
	CImage_UnLoadImage(GameFrog->background);
	CImage_UnLoadImage(GameFrog->spritesheetfrog);
	CImage_UnLoadImage(GameFrog->spritesheetbackground);
	CImage_UnLoadImage(GameFrog->spritesheetcar1);
	CImage_UnLoadImage(GameFrog->spritesheetcar2);
	CImage_UnLoadImage(GameFrog->spritesheetcar3);
	CImage_UnLoadImage(GameFrog->spritesheetcar4);
	CImage_UnLoadImage(GameFrog->spritesheetcar5);
	CImage_UnLoadImage(GameFrog->spritesheetplant);
	CImage_UnLoadImage(GameFrog->spritesheetfruit1);
	CImage_UnLoadImage(GameFrog->spritesheetfruit2);
	CImage_UnLoadImage(GameFrog->spritesheetfruit3);
}

//Update ----------------------------------------------------------------------------------------------------------------

void CGameFrog_OnGameStart(CGameFrog* GameFrog)
{
	GameFrog->playerdeath = false;
}

void CGameFrog_UpdateLogic(CGameFrog* GameFrog)
{
	if (GameFrog->GameBase->UpdateLogic(GameFrog->GameBase))
		GameFrog->OnGameStart(GameFrog);
	if (SubGameState == SGGame)
	{
		GameFrog->updateplayer(GameFrog);
		//needs to be done after player update and before object update
		//object update checks collisions etc and thus also player death
		//it does this before adjusting GameFrog->worldspeed to y value
		//so if not done here weird collisions and false deaths happen
		if (GameFrog->dolevelinc)
		{
			if (GameFrog->GameBase->level-1 < CGameFrog_lenlevelincspeeds - 1)
			{
				GameFrog->GameBase->level += 1;
				GameFrog->dolevelinc = false;
			}
		}

		GameFrog->updateobjects(GameFrog);
		CSprites_UpdateSprites(Renderer);


		if (GameFrog->playerdeath)
		{
			CAudio_PlaySound(GameFrog->SfxDie, 0);
			CGame_AddToScore(-150);
			if(GameFrog->GameBase->HealthPoints > 1)
			{
				GameFrog->destroyallobjects(GameFrog);
				GameFrog->destroyplayer(GameFrog);
				GameFrog->createobjects(GameFrog,true);
				GameFrog->createplayer(GameFrog);
				GameFrog->GameBase->HealthPoints = GameFrog->GameBase->HealthPoints - 1;
				SubGameState = SGReadyGo;
				SubStateTime = SDL_GetTicks() + 500;
			}
			if(GameMode == GMGame)
				GameFrog->GameBase->HealthPoints -= 1;
		}
	}
}

void CGameFrog_Draw(CGameFrog* GameFrog)
{
	CSprites_DrawSprites(Renderer);
	GameFrog->DrawBackground(GameFrog);
	GameFrog->GameBase->DrawScoreBar(GameFrog->GameBase);
	GameFrog->GameBase->DrawSubStateText(GameFrog->GameBase);
}
