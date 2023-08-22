#include <SDL.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "CGameBreakOut.h"
#include "../CGame.h"
#include "../Common.h"
#include "../CTween.h"
#include "../Vec2F.h"



CGameBreakOut* Create_CGameBreakOut()
{
	CGameBreakOut* GameBreakOut = (CGameBreakOut*) malloc(sizeof(CGameBreakOut));
	GameBreakOut->GameBase = Create_CGameBase(GSBreakout, false);
	GameBreakOut->MusMusic = -1;
	GameBreakOut->SfxSucces = -1;
	GameBreakOut->SfxDie = -1;
	GameBreakOut->SfxBrick = -1;
	GameBreakOut->SfxBat = -1;
	GameBreakOut->GameBase->screenleft = 0;
	GameBreakOut->GameBase->screenright = ScreenWidth;
	GameBreakOut->GameBase->screentop = 0;
	GameBreakOut->GameBase->screenbottom = ScreenHeight;

	GameBreakOut->background = 0;
	GameBreakOut->spritesheetblocks = 0;
	GameBreakOut->spritesheetbat = 0;
	GameBreakOut->spritesheetball = 0;
	GameBreakOut->backgroundtz.x = 0;
	GameBreakOut->backgroundtz.y = 0;

	GameBreakOut->curballspeed = 0.0f;
	GameBreakOut->pattern = 0;
	GameBreakOut->blockinfo.mostbottom = 0;
	GameBreakOut->blockinfo.mostleft = 0;
	GameBreakOut->blockinfo.mostright = 0;

	GameBreakOut->spritescale.x = 2.5f*xscale;
	GameBreakOut->spritescale.y = 2.5f*yscale;
	GameBreakOut->blockspritecale.x = 1.0f*xscale;
	GameBreakOut->blockspritecale.y = 2.0f*yscale;

	for (int i= 0; i < CGameBreakOut_numblocks; i++)
	{
		Initialize_CSpriteObject(&GameBreakOut->blocks[i]);
		initialize_CTweenInfo(&GameBreakOut->tweens[i][0]);
		initialize_CTweenInfo(&GameBreakOut->tweens[i][1]);
	}
	
	Initialize_CSpriteObject(&GameBreakOut->player);
	Initialize_CSpriteObject(&GameBreakOut->ball);


	GameBreakOut->updateball = CGameBreakOut_updateball;
	GameBreakOut->createball = CGameBreakOut_createball;
	GameBreakOut->destroyball = CGameBreakOut_destroyball;
	GameBreakOut->updateplayer = CGameBreakOut_updateplayer;
	GameBreakOut->createplayer = CGameBreakOut_createplayer;
	GameBreakOut->destroyplayer = CGameBreakOut_destroyplayer;
	GameBreakOut->updateblocks = CGameBreakOut_updateblocks;
	GameBreakOut->createblocks = CGameBreakOut_createblocks;
	GameBreakOut->destroyblock = CGameBreakOut_destroyblock;
	GameBreakOut->destroyallblocks = CGameBreakOut_destroyallblocks;
	GameBreakOut->updateblockinfo = CGameBreakOut_updateblockinfo;
	GameBreakOut->init = CGameBreakOut_init;
	GameBreakOut->deinit = CGameBreakOut_deinit;
	GameBreakOut->UnloadGraphics = CGameBreakOut_UnloadGraphics;
	GameBreakOut->LoadGraphics = CGameBreakOut_LoadGraphics;
	GameBreakOut->LoadSound = CGameBreakOut_LoadSound;
	GameBreakOut->UnLoadSound = CGameBreakOut_UnLoadSound;
	GameBreakOut->UpdateObjects = CGameBreakOut_UpdateObjects;
	GameBreakOut->DrawBackground = CGameBreakOut_DrawBackground;
	GameBreakOut->Draw = CGameBreakOut_Draw;
	GameBreakOut->UpdateLogic = CGameBreakOut_UpdateLogic;
	return GameBreakOut;
}

void Destroy_CGameBreakOut(CGameBreakOut* GameBreakOut)
{
	Destroy_CGameBase(GameBreakOut->GameBase);
	free(GameBreakOut);
}

//blocks ----------------------------------------------------------------------------------------------------------------

void CGameBreakOut_updateblockinfo(CGameBreakOut* GameBreakOut)
{
	GameBreakOut->blockinfo.mostleft = -1;
	GameBreakOut->blockinfo.mostright = -1;
	GameBreakOut->blockinfo.mostbottom = -1;

	int x1 = GameBreakOut->GameBase->screenright + 1;
	int x2 = GameBreakOut->GameBase->screenleft - 1;
	int y = GameBreakOut->GameBase->screentop - 1;
	for (int i = 0; i < CGameBreakOut_numblocks; i++)
	{
		if (GameBreakOut->blocks[i].alive)
		{
			if (GameBreakOut->blocks[i].pos.x < x1)
			{
				x1 = GameBreakOut->blocks[i].pos.x;
				GameBreakOut->blockinfo.mostleft = i;
			}

			if (GameBreakOut->blocks[i].pos.x > x2)
			{
				x2 = GameBreakOut->blocks[i].pos.x;
				GameBreakOut->blockinfo.mostright = i;
			}

			if (GameBreakOut->blocks[i].pos.y > y)
			{
				y = GameBreakOut->blocks[i].pos.y;
				GameBreakOut->blockinfo.mostbottom = i;
			}
		}
	}
}


void CGameBreakOut_destroyallblocks(CGameBreakOut* GameBreakOut)
{
	for (int i = 0; i < CGameBreakOut_numblocks; i++)
		GameBreakOut->destroyblock(GameBreakOut,i);
}

void CGameBreakOut_destroyblock(CGameBreakOut* GameBreakOut, int index)
{
	if(GameBreakOut->blocks[index].alive)
	{
		CSprites_RemoveSprite(GameBreakOut->blocks[index].spr);
		GameBreakOut->blocks[index].alive = false;
	}
}

void CGameBreakOut_createblocks(CGameBreakOut* GameBreakOut, bool setlocation)
{
	GameBreakOut->pattern = rand() % 5;
	for (int x = 0; x < CGameBreakOut_blockcols; x++)
	{
		for(int y = 0; y < CGameBreakOut_blockrows; y++)
		{
			GameBreakOut->tweens[x + y * CGameBreakOut_blockcols][CGameBreakOut_tweenblockpositions] = createtween(CGameBreakOut_tweenblockpositions, 1+ ((rand() %(6)) / 10), funcsmoothstop, 1, true, DesiredFps);
			GameBreakOut->blocks[x + y * CGameBreakOut_blockcols].spr = CSprites_CreateSprite();
			GameBreakOut->blocks[x + y * CGameBreakOut_blockcols].state = 0;
			GameBreakOut->blocks[x + y * CGameBreakOut_blockcols].alive = true;
			CSprites_SetSpriteImageTiles(Renderer,GameBreakOut->blocks[x + y * CGameBreakOut_blockcols].spr, &GameBreakOut->spritesheetblocks, 6, 1);
			SDL_Point tz = CSprites_TileSize(GameBreakOut->blocks[x + y * CGameBreakOut_blockcols].spr);
			tz.x = tz.x * GameBreakOut->blockspritecale.x;
			tz.y = tz.y * GameBreakOut->blockspritecale.y;
			GameBreakOut->blocks[x + y * CGameBreakOut_blockcols].tz = tz;
			CSprites_SetSpriteAnimation(GameBreakOut->blocks[x + y * CGameBreakOut_blockcols].spr, y % 6, y % 6, 0);

			CSprites_SetSpriteScale(Renderer,GameBreakOut->blocks[x + y * CGameBreakOut_blockcols].spr, GameBreakOut->blockspritecale);
			GameBreakOut->blocks[x + y * CGameBreakOut_blockcols].pos.x = GameBreakOut->GameBase->screenleft + CGameBreakOut_blockxoffset + (x * tz.x);
			GameBreakOut->blocks[x + y * CGameBreakOut_blockcols].pos.y = GameBreakOut->GameBase->screentop + CGameBreakOut_blockyoffset + y * tz.y;
			if (setlocation)
				CSprites_SetSpriteLocation(GameBreakOut->blocks[x + y * CGameBreakOut_blockcols].spr, GameBreakOut->blocks[x + y * CGameBreakOut_blockcols].pos);

		}
	}
	GameBreakOut->updateblockinfo(GameBreakOut);
}


void CGameBreakOut_updateblocks(CGameBreakOut* GameBreakOut)
{
	for(int x = 0; x < CGameBreakOut_blockcols; x++)
	{
		for(int y = 0; y < CGameBreakOut_blockrows; y++)
		{
			if(GameBreakOut->blocks[x + y * CGameBreakOut_blockcols].alive)
			{
				if(GameBreakOut->tweens[x + y * CGameBreakOut_blockcols][CGameBreakOut_tweenblockpositions].active)
				{
					GameBreakOut->tweens[x + y * CGameBreakOut_blockcols][CGameBreakOut_tweenblockpositions] = updatetween(GameBreakOut->tweens[x + y * CGameBreakOut_blockcols][CGameBreakOut_tweenblockpositions]);
					Vec2F pos = GameBreakOut->blocks[x + y * CGameBreakOut_blockcols].pos;
					if (GameBreakOut->pattern == 0)
					{
						if (x < CGameBreakOut_blockcols / 3)
							pos.x = pos.x * GameBreakOut->tweens[x + y * CGameBreakOut_blockcols][CGameBreakOut_tweenblockpositions].funcval;
						else
						{
							if( x > CGameBreakOut_blockcols * 2 / 3)
								pos.x = GameBreakOut->GameBase->screenright - (GameBreakOut->GameBase->screenright - pos.x) * GameBreakOut->tweens[x + y * CGameBreakOut_blockcols][CGameBreakOut_tweenblockpositions].funcval;
						}
						if (y < CGameBreakOut_blockrows / 2)
							pos.y = pos.y * GameBreakOut->tweens[x + y * CGameBreakOut_blockcols][CGameBreakOut_tweenblockpositions].funcval;
					}
					else
					{
						if(GameBreakOut->pattern == 1)
							pos.y = pos.y * GameBreakOut->tweens[x + y * CGameBreakOut_blockcols][CGameBreakOut_tweenblockpositions].funcval;
						else
						{
							if (GameBreakOut->pattern == 2)
							{
								if (x < CGameBreakOut_blockcols / 2)
									pos.x = pos.x * GameBreakOut->tweens[x + y * CGameBreakOut_blockcols][CGameBreakOut_tweenblockpositions].funcval;
								else
									pos.x = GameBreakOut->GameBase->screenright - (GameBreakOut->GameBase->screenright - pos.x) * GameBreakOut->tweens[x + y * CGameBreakOut_blockcols][CGameBreakOut_tweenblockpositions].funcval;
							}
							else
							{
								if (GameBreakOut->pattern == 3)
									pos.x = pos.x * GameBreakOut->tweens[x + y * CGameBreakOut_blockcols][CGameBreakOut_tweenblockpositions].funcval;
								else
								{
									if (GameBreakOut->pattern == 4)
										pos.x = GameBreakOut->GameBase->screenright - (GameBreakOut->GameBase->screenright - pos.x) * GameBreakOut->tweens[x + y * CGameBreakOut_blockcols][CGameBreakOut_tweenblockpositions].funcval;
								}
							}
						}
					}

//					setSpriteLocation(GameBreakOut->blocks[x + y * CGameBreakOut_blockcols].spr, pos)
					GameBreakOut->blocks[x + y * CGameBreakOut_blockcols].spr->x = (int)(pos.x);
					GameBreakOut->blocks[x + y * CGameBreakOut_blockcols].spr->y = (int)(pos.y);

				}
				if (GameBreakOut->blocks[x + y * CGameBreakOut_blockcols].state == CGameBreakOut_blockstatedeath)
				{
					if (GameBreakOut->tweens[x + y * CGameBreakOut_blockcols][CGameBreakOut_tweenblockdeath].active)
					{
						GameBreakOut->tweens[x + y * CGameBreakOut_blockcols][CGameBreakOut_tweenblockdeath] = updatetween(GameBreakOut->tweens[x + y * CGameBreakOut_blockcols][CGameBreakOut_tweenblockdeath]);
						Vec2F pos = GameBreakOut->blocks[x + y * CGameBreakOut_blockcols].pos;
						pos.y = pos.y + (50 * GameBreakOut->tweens[x + y * CGameBreakOut_blockcols][CGameBreakOut_tweenblockdeath].funcval);
						pos.x = pos.x + (30 * GameBreakOut->tweens[x + y * CGameBreakOut_blockcols][CGameBreakOut_tweenblockdeath].funcval);
//						setSpriteLocation(GameBreakOut->blocks[x + y * CGameBreakOut_blockcols].spr, pos)
						GameBreakOut->blocks[x + y * CGameBreakOut_blockcols].spr->x = (int)(pos.x);
						GameBreakOut->blocks[x + y * CGameBreakOut_blockcols].spr->y = (int)(pos.y);
						CSprites_SetSpriteRotation(GameBreakOut->blocks[x + y * CGameBreakOut_blockcols].spr, 720 * GameBreakOut->tweens[x + y * CGameBreakOut_blockcols][CGameBreakOut_tweenblockdeath].funcval);
						CSprites_SetSpriteColour(GameBreakOut->blocks[x + y * CGameBreakOut_blockcols].spr, 1,1,1, 1- GameBreakOut->tweens[x + y * CGameBreakOut_blockcols][CGameBreakOut_tweenblockdeath].funcval);
					}
					else
					{
						GameBreakOut->destroyblock(GameBreakOut,x + y * CGameBreakOut_blockcols);
						GameBreakOut->updateblockinfo(GameBreakOut);
					}
				}

			}
		}
	}

	if (GameBreakOut->blockinfo.mostleft == -1)
	{
		CGame_AddToScore(500);
		GameBreakOut->createblocks(GameBreakOut, false);
		CAudio_PlaySound(GameBreakOut->SfxSucces, 0);
	}
}

//player ----------------------------------------------------------------------------------------------------------------

void CGameBreakOut_destroyplayer(CGameBreakOut* GameBreakOut)
{
	if (GameBreakOut->player.alive)
	{
		CSprites_RemoveSprite(GameBreakOut->player.spr);
		GameBreakOut->player.alive = false;
	}
}

void CGameBreakOut_createplayer(CGameBreakOut* GameBreakOut)
{
	GameBreakOut->player.spr = CSprites_CreateSprite();
	CSprites_SetSpriteImage(Renderer,GameBreakOut->player.spr, &GameBreakOut->spritesheetbat);
	CSprites_SetSpriteScale(Renderer,GameBreakOut->player.spr, GameBreakOut->spritescale);
	GameBreakOut->player.tz = CImage_ImageSize(GameBreakOut->spritesheetbat);
	GameBreakOut->player.tz.x = GameBreakOut->player.tz.x * GameBreakOut->spritescale.x;
	GameBreakOut->player.tz.y = GameBreakOut->player.tz.y * GameBreakOut->spritescale.y;
	GameBreakOut->player.pos.x = (float)(GameBreakOut->GameBase->screenright - GameBreakOut->GameBase->screenleft) / 2;
	GameBreakOut->player.pos.y = (float)GameBreakOut->GameBase->screenbottom - 20*yscale - (GameBreakOut->player.tz.y / 2);
	GameBreakOut->GameBase->HealthPoints = 5;
	CSprites_SetSpriteLocation(GameBreakOut->player.spr, GameBreakOut->player.pos);
	GameBreakOut->player.alive = true;
}

void CGameBreakOut_updateplayer(CGameBreakOut* GameBreakOut)
{
	CSprites_SetSpriteVisibility(GameBreakOut->player.spr, GameBreakOut->player.alive);
	if (GameBreakOut->player.alive)
	{
		float speedmultiplier = 1;

		if (CInput_Buttons.ButA)
			speedmultiplier = 2;

		if ((CInput_Buttons.ButLeft) ||
			(CInput_Buttons.ButLeft2) ||
			(CInput_Buttons.ButDpadLeft))
		{
			if (GameBreakOut->player.pos.x - GameBreakOut->player.tz.x / 2 - (CGameBreakOut_playerspeed * speedmultiplier) > GameBreakOut->GameBase->screenleft)
				GameBreakOut->player.pos.x -= CGameBreakOut_playerspeed * speedmultiplier;
			else
				GameBreakOut->player.pos.x = GameBreakOut->GameBase->screenleft + GameBreakOut->player.tz.x / 2;
		}

		if ((CInput_Buttons.ButRight) ||
			(CInput_Buttons.ButRight2) ||
			(CInput_Buttons.ButDpadRight))
		{
			if ( GameBreakOut->player.pos.x + GameBreakOut->player.tz.x / 2 + (CGameBreakOut_playerspeed * speedmultiplier) < GameBreakOut->GameBase->screenright)
				GameBreakOut->player.pos.x += CGameBreakOut_playerspeed * speedmultiplier;
			else
				GameBreakOut->player.pos.x = GameBreakOut->GameBase->screenright - GameBreakOut->player.tz.x / 2;
		}

		//setSpriteLocation(GameBreakOut->player.spr, GameBreakOut->player.pos)
		GameBreakOut->player.spr->x = (int)(GameBreakOut->player.pos.x);
		GameBreakOut->player.spr->y = (int)(GameBreakOut->player.pos.y);
	}
	else
	{
		if (GameBreakOut->player.freeze > 0)
			GameBreakOut->player.freeze -= 1;
		else
			GameBreakOut->player.alive = true;
	}
}


//ball ----------------------------------------------------------------------------------------------------------------

void CGameBreakOut_destroyball(CGameBreakOut* GameBreakOut)
{
	if (GameBreakOut->ball.alive)
	{
		CSprites_RemoveSprite(GameBreakOut->ball.spr);
		GameBreakOut->ball.alive = false;
	}
}

void CGameBreakOut_createball(CGameBreakOut* GameBreakOut)
{
	GameBreakOut->ball.spr = CSprites_CreateSprite();
	CSprites_SetSpriteImage(Renderer,GameBreakOut->ball.spr, &GameBreakOut->spritesheetball);
	Vec2F scale = {xscale,yscale};
	CSprites_SetSpriteScale(Renderer,GameBreakOut->ball.spr, scale);
	GameBreakOut->ball.tz = CImage_ImageSize(GameBreakOut->spritesheetball);
	GameBreakOut->ball.tz.x = GameBreakOut->ball.tz.x * xscale;
	GameBreakOut->ball.tz.y = GameBreakOut->ball.tz.y * yscale;
	
	CSprites_SetSpriteCollisionShape(GameBreakOut->ball.spr, SHAPE_BOX, 4/xscale, 4/yscale, 0, 0, 0);
	GameBreakOut->ball.pos.x = (float)((GameBreakOut->GameBase->screenright - GameBreakOut->GameBase->screenleft) / 2) + 250*xscale;
	GameBreakOut->ball.pos.y = (float)GameBreakOut->GameBase->screenbottom - 250*yscale - 20*yscale;
	GameBreakOut->ball.vel.x = -0.5;
	GameBreakOut->ball.vel.y = 0.5;
	GameBreakOut->curballspeed = CGameBreakOut_ballspeed;
	GameBreakOut->ball.alive = true;
	CSprites_SetSpriteLocation(GameBreakOut->ball.spr, GameBreakOut->ball.pos);
}

void CGameBreakOut_updateball(CGameBreakOut* GameBreakOut)
{
	if (GameBreakOut->ball.alive)
	{
		if (GameBreakOut->ball.freeze > 0)
			GameBreakOut->ball.freeze -= 1;
		else
		{
			float steps = GameBreakOut->curballspeed / CGameBreakOut_ballvelsegments;

			for (int j = 1; j <CGameBreakOut_ballvelsegments + 1; j++)
			{
				GameBreakOut->ball.pos.x += (steps * GameBreakOut->ball.vel.x);
				GameBreakOut->ball.pos.y += (steps * GameBreakOut->ball.vel.y);

				if (GameBreakOut->ball.pos.x < GameBreakOut->GameBase->screenleft)
				{
					GameBreakOut->ball.vel.x *= -1;
					GameBreakOut->ball.pos.x = GameBreakOut->GameBase->screenleft;
				}

				if (GameBreakOut->ball.pos.x > GameBreakOut->GameBase->screenright)
				{
					GameBreakOut->ball.vel.x *= -1;
					GameBreakOut->ball.pos.x = GameBreakOut->GameBase->screenright;
				}

				if (GameBreakOut->ball.pos.y < GameBreakOut->GameBase->screentop)
				{
					GameBreakOut->ball.pos.y = GameBreakOut->GameBase->screentop;
					GameBreakOut->ball.vel.y *= -1;
				}

				CSprites_SetSpriteLocation(GameBreakOut->ball.spr, GameBreakOut->ball.pos);

				if (GameBreakOut->ball.pos.y >= GameBreakOut->GameBase->screenbottom)
				{
					GameBreakOut->destroyball(GameBreakOut);
					GameBreakOut->createball(GameBreakOut);
					GameBreakOut->ball.freeze = 45;
					GameBreakOut->GameBase->HealthPoints -= 1;
					CGame_AddToScore(-100);
					CAudio_PlaySound(GameBreakOut->SfxDie, 0);
				}

				for (int k = 0; k < CGameBreakOut_numblocks; k++)
				{
					if (GameBreakOut->blocks[k].alive && (GameBreakOut->blocks[k].state != CGameBreakOut_blockstatedeath))
					{
						if (CSprites_DetectSpriteCollision(GameBreakOut->ball.spr, GameBreakOut->blocks[k].spr))
						{
							//seen this in wireframe issue 11 not sure what it actually
							//calculates but seems to work more or less ok
							float dx = (GameBreakOut->ball.pos.x - GameBreakOut->blocks[k].pos.x) / GameBreakOut->blocks[k].tz.x;
							float dy = (GameBreakOut->ball.pos.y - GameBreakOut->blocks[k].pos.y) / GameBreakOut->blocks[k].tz.y;
							if ((float)fabs(dx) > (float)fabs(dy))
								GameBreakOut->ball.vel.x = (float)fabs(GameBreakOut->ball.vel.x) * dx / (float)fabs(dx);
							else
								GameBreakOut->ball.vel.y = (float)fabs(GameBreakOut->ball.vel.y) * dy / (float)fabs(dy);

							CGame_AddToScore(20);
							//inc CGameBreakOut_ballspeed
							GameBreakOut->curballspeed += CGameBreakOut_ballspeedinc;
							CAudio_PlaySound(GameBreakOut->SfxBrick, 0);
							GameBreakOut->blocks[k].state = CGameBreakOut_blockstatedeath;
							CSprites_SetSpriteDepth(GameBreakOut->blocks[k].spr, 5);
							GameBreakOut->tweens[k][CGameBreakOut_tweenblockdeath] = createtween(CGameBreakOut_tweenblockdeath, 1, funcsmoothstep, 1, true, DesiredFps);
						}
					}
				}

				if (GameBreakOut->player.alive)
				{
					if (CSprites_DetectSpriteCollision(GameBreakOut->ball.spr, GameBreakOut->player.spr))
					{
						//touched top part of bat
						if (GameBreakOut->ball.pos.y < GameBreakOut->player.pos.y + GameBreakOut->player.tz.y / 3)
						{
							//set ball to top part - 1 so it can't callide again
							GameBreakOut->ball.pos.y = GameBreakOut->player.pos.y - GameBreakOut->player.tz.y / 2 - 1;
							CSprites_SetSpriteLocation(GameBreakOut->ball.spr, GameBreakOut->ball.pos);

							//inverrt y velocity
							GameBreakOut->ball.vel.y = -GameBreakOut->ball.vel.y;
							//touch far right of bat
							if (GameBreakOut->ball.pos.x >= GameBreakOut->player.pos.x + ((GameBreakOut->player.tz.x / 2) * 5 / 7))
							{
								//and bal is moving to the left
								if (GameBreakOut->ball.vel.x < 0)
								{
									//reverse x velocity
									GameBreakOut->ball.vel.x = -GameBreakOut->ball.vel.x;
									//add some randomness
									float rnd = (rand() % 100) / 500;
									if (rand() % 2 == 0)
										rnd *= -1;

									GameBreakOut->ball.vel.x += rnd;
								}
							}
							else
							{
								//touched far left of bat
								if (GameBreakOut->ball.pos.x <= GameBreakOut->player.pos.x - ((GameBreakOut->player.tz.x / 2) * 5 / 7))
								{
									//and ball moving to the right
									if (GameBreakOut->ball.vel.x > 0)
									{
										//reverse x velocity
										GameBreakOut->ball.vel.x = -GameBreakOut->ball.vel.x;
										//add some randomness
										float rnd = (rand() % 100) / 500;
										if (rand() % 2 == 0)
											rnd *= -1;

										GameBreakOut->ball.vel.x += rnd;
									}
								}
							}
						}
						else
						{
							//bottom parts = loss of ball
							// ball is on the right
							if (GameBreakOut->ball.pos.x > GameBreakOut->player.pos.x)
							{
								//ball is moving towards bat to left
								if (GameBreakOut->ball.vel.x < 0)
									//reverse x velocity
									GameBreakOut->ball.vel.x = -GameBreakOut->ball.vel.x;
							}
							else
							{
								//ball is on the left and moving to the right
								if (GameBreakOut->ball.vel.x > 0)
									//reverse x velocity
									GameBreakOut->ball.vel.x = -GameBreakOut->ball.vel.x;
							}
						}

						//increase CGameBreakOut_ballspeed
						GameBreakOut->curballspeed += CGameBreakOut_ballspeedinc;

						//just a safety
						while (CSprites_DetectSpriteCollision(GameBreakOut->ball.spr, GameBreakOut->player.spr))
						{
							GameBreakOut->ball.pos.x += (steps * GameBreakOut->ball.vel.x);
							GameBreakOut->ball.pos.y += (steps * GameBreakOut->ball.vel.y);
							CSprites_SetSpriteLocation(GameBreakOut->ball.spr, GameBreakOut->ball.pos);
						}

						CAudio_PlaySound(GameBreakOut->SfxBat, 0);
					}
				}
			}
		}
	}
}

//background ----------------------------------------------------------------------------------------------------------------

void CGameBreakOut_DrawBackground(CGameBreakOut* GameBreakOut)
{
	CImage_DrawImage(Renderer, GameBreakOut->background, NULL, NULL);
}

void CGameBreakOut_Draw(CGameBreakOut* GameBreakOut)
{
	GameBreakOut->DrawBackground(GameBreakOut);
	CSprites_DrawSprites(Renderer);
	GameBreakOut->GameBase->DrawScoreBar(GameBreakOut->GameBase);
	GameBreakOut->GameBase->DrawSubStateText(GameBreakOut->GameBase);
}
//init - deinit ----------------------------------------------------------------------------------------------------------------

void CGameBreakOut_init(CGameBreakOut* GameBreakOut)
{
	GameBreakOut->LoadGraphics(GameBreakOut);
	GameBreakOut->createblocks(GameBreakOut, false);
	GameBreakOut->createplayer(GameBreakOut);
	GameBreakOut->createball(GameBreakOut);

	GameBreakOut->LoadSound(GameBreakOut);
	CurrentGameMusicID = GameBreakOut->MusMusic;
	CAudio_PlayMusic(GameBreakOut->MusMusic, -1);
}

void CGameBreakOut_LoadSound(CGameBreakOut* GameBreakOut)
{
	GameBreakOut->SfxDie = CAudio_LoadSound("common/die.wav");
	GameBreakOut->SfxSucces = CAudio_LoadSound("common/succes.wav");
	GameBreakOut->SfxBat = CAudio_LoadSound("breakout/bat.wav");
	GameBreakOut->SfxBrick = CAudio_LoadSound("breakout/brick.wav");
	GameBreakOut->MusMusic = CAudio_LoadMusic("breakout/music.ogg");
}

void CGameBreakOut_UnLoadSound(CGameBreakOut* GameBreakOut)
{
	CAudio_StopMusic();
	CAudio_StopSound();
	CAudio_UnLoadMusic(GameBreakOut->MusMusic);
	CAudio_UnLoadSound(GameBreakOut->SfxSucces);
	CAudio_UnLoadSound(GameBreakOut->SfxDie);
	CAudio_UnLoadSound(GameBreakOut->SfxBrick);
	CAudio_UnLoadSound(GameBreakOut->SfxBat);
}

void CGameBreakOut_deinit(CGameBreakOut* GameBreakOut)
{
	GameBreakOut->destroyplayer(GameBreakOut);
	GameBreakOut->destroyallblocks(GameBreakOut);
	GameBreakOut->destroyball(GameBreakOut);
	GameBreakOut->UnLoadSound(GameBreakOut);
	SubStateCounter = 0;
	SubGameState = SGNone;
	CurrentGameMusicID = -1;
	GameBreakOut->UnloadGraphics(GameBreakOut);
}

void CGameBreakOut_LoadGraphics(CGameBreakOut* GameBreakOut)
{
	GameBreakOut->background = CImage_LoadImage(Renderer, "breakout/background.png");
	GameBreakOut->backgroundtz = CImage_ImageSize(GameBreakOut->background);
	GameBreakOut->spritesheetblocks = CImage_LoadImageEx(Renderer, "breakout/blocks.png", 8, 128, dumpScaledBitmaps); 
	GameBreakOut->spritesheetbat = CImage_LoadImageEx(Renderer, "breakout/paddle.png",0, 128, dumpScaledBitmaps);
	GameBreakOut->spritesheetball = CImage_LoadImageEx(Renderer, "breakout/ball.png", 0, 128, dumpScaledBitmaps);

	//SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/breakout/blocks.bmp", Game->CImage_GetImage(GameBreakOut->spritesheetblocks), 1,1, true,8, 128); //0 80
	//SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/breakout/paddle.bmp", Game->CImage_GetImage(GameBreakOut->spritesheetbat), 1,1, true,0, 128); //173
	//SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/breakout/ball.bmp", Game->CImage_GetImage(GameBreakOut->spritesheetball), 1,1, true,0, 128); //173

	if(!useDefaultColorAssets)
	{
		GameBreakOut->UnloadGraphics(GameBreakOut);
		GameBreakOut->background = CImage_LoadImage(Renderer, "breakout/background.png");
		GameBreakOut->backgroundtz = CImage_ImageSize(GameBreakOut->background);
		GameBreakOut->spritesheetblocks = CImage_LoadImage(Renderer, "breakout/blocks.bmp");
		GameBreakOut->spritesheetbat = CImage_LoadImage(Renderer, "breakout/paddle.bmp");
		GameBreakOut->spritesheetball = CImage_LoadImage(Renderer, "breakout/ball.bmp");
	}


}

void CGameBreakOut_UnloadGraphics(CGameBreakOut* GameBreakOut)
{
	CImage_UnLoadImage(GameBreakOut->background);
	CImage_UnLoadImage(GameBreakOut->spritesheetblocks);
	CImage_UnLoadImage(GameBreakOut->spritesheetbat);
	CImage_UnLoadImage(GameBreakOut->spritesheetball);
}

//Update ----------------------------------------------------------------------------------------------------------------

void CGameBreakOut_UpdateObjects(CGameBreakOut* GameBreakOut, bool IsGameState)
{
	GameBreakOut->updateblocks(GameBreakOut);
	if (IsGameState)
	{
		GameBreakOut->updateplayer(GameBreakOut);
		GameBreakOut->updateball(GameBreakOut);
	}
}

void CGameBreakOut_UpdateLogic(CGameBreakOut* GameBreakOut)
{
	GameBreakOut->GameBase->UpdateLogic(GameBreakOut->GameBase);
	GameBreakOut->UpdateObjects(GameBreakOut, SubGameState == SGGame);
	if(SubGameState == SGGame)
		CSprites_UpdateSprites(Renderer);
}
