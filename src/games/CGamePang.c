#include <SDL.h>
#include <stdbool.h>
#include "CGamePang.h"
#include "CGameBase.h"
#include "../CGame.h"
#include "../Common.h"
#include "../Vec2F.h"



CGamePang* Create_CGamePang()
{
	CGamePang* GamePang = (CGamePang*) malloc(sizeof(CGamePang));
	GamePang->GameBase = Create_CGameBase(GSPang, true);

	GamePang->MusMusic = -1;
	GamePang->SfxSucces = -1;
	GamePang->SfxDie = -1;
	GamePang->SfxShoot = -1;
	GamePang->SfxPop = -1;
	GamePang->GameBase->screenleft = 0;
	GamePang->GameBase->screenright = ScreenWidth;
	GamePang->GameBase->screentop = 0;
	GamePang->GameBase->screenbottom = ScreenHeight;

	GamePang->playerscale.x = 2.0f*yscale;
	GamePang->playerscale.y = 2.0f*yscale;
	GamePang->bulletscale.x = 1.0f*yscale;
	GamePang->bulletscale.y = 0.8f*yscale;
	GamePang->ballscale.x = 1.5f*yscale;
	GamePang->ballscale.y = 1.5f*yscale;

	GamePang->deaths = 0;
	GamePang->levelcleared = false;

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

	for (int i=0; i < CGamePang_maxballs; i++)
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
	for(int i = 0; i < CGamePang_maxballs; i++)
		GamePang->destroyball(GamePang,i, true);
}

void CGamePang_destroyball(CGamePang* GamePang, int index, bool nocreate)
{
	if (!nocreate)
	{
		if (GamePang->balls[index].id > CGamePang_ballsmall)
		{
			GamePang->createball(GamePang,GamePang->balls[index].id - 1, GamePang->balls[index].pos.x - 25*yscale, GamePang->balls[index].pos.y, -10.0f*yscale);
			GamePang->createball(GamePang,GamePang->balls[index].id - 1, GamePang->balls[index].pos.x + 25*yscale, GamePang->balls[index].pos.y, 10.0f*yscale);
		}
	}
	if (GamePang->balls[index].alive)
	{
		CSprites_RemoveSprite(GamePang->balls[index].spr);
		GamePang->balls[index].alive = false;
	}
}

void CGamePang_createball(CGamePang* GamePang, int size, float x, float y, float speed)
{
	for (int i = 0; i < CGamePang_maxballs; i++)
	{
		if (!GamePang->balls[i].alive)
		{
			GamePang->balls[i].spr = CSprites_CreateSprite();
			CSprites_SetSpriteImage(Renderer,GamePang->balls[i].spr, &GamePang->spritesheetball);
			CSprites_SetSpriteCollisionShape(GamePang->balls[i].spr, SHAPE_CIRCLE, 35,35,0,0,0);
			//CSprites_SetSpriteCollisionShape(GamePang->balls[i].spr, SHAPE_BOX, 27,27,0,0,0);
			CSprites_SetSpriteColour(GamePang->balls[i].spr, 1, 1, 1, 0.7f);
			Vec2F Scale = GamePang->ballscale;
			Scale.x = Scale.x * size;
			Scale.y = Scale.y * size;
			CSprites_SetSpriteScale(Renderer,GamePang->balls[i].spr, Scale);
			CSprites_SetSpriteDepth(GamePang->balls[i].spr, 6);
			GamePang->balls[i].tz = CImage_ImageSize(GamePang->spritesheetball);
			GamePang->balls[i].tz.x = GamePang->balls[i].tz.x * Scale.x;
			GamePang->balls[i].tz.x = GamePang->balls[i].tz.y * Scale.y;
			GamePang->balls[i].pos.x = x;
			GamePang->balls[i].pos.y = y;
			CSprites_SetSpriteLocation(GamePang->balls[i].spr, GamePang->balls[i].pos);
			GamePang->balls[i].alive = true;
			GamePang->balls[i].speed = speed*0.1f;
			GamePang->balls[i].force = -(float)fabs(speed);
			GamePang->balls[i].curforce = GamePang->balls[i].force/3;
			GamePang->balls[i].id = size;
			break;
		}
	}
}

void CGamePang_updateballs(CGamePang* GamePang)
{
	GamePang->levelcleared = true;
	for (int i = 0; i < CGamePang_maxballs; i++)
	{
		if (GamePang->balls[i].alive)
		{
			GamePang->levelcleared = false;
			if (GamePang->balls[i].pos.x + GamePang->balls[i].speed > GamePang->GameBase->screenright)
			{
				if (GamePang->balls[i].speed > 0)
					GamePang->balls[i].speed = -(float)fabs(GamePang->balls[i].speed);

			}

			if (GamePang->balls[i].pos.x + GamePang->balls[i].speed < GamePang->GameBase->screenleft)
			{
				if (GamePang->balls[i].speed < 0)
					GamePang->balls[i].speed = (float)fabs(GamePang->balls[i].speed);
			}


			GamePang->balls[i].pos.x += GamePang->balls[i].speed*2;

			if (GamePang->balls[i].id == CGamePang_ballbig)
				GamePang->balls[i].curforce += 0.1f*yscale;
			else
			{
				if(GamePang->balls[i].id == CGamePang_ballmedium)
					GamePang->balls[i].curforce += 0.15f*yscale;
				else
				{
					if (GamePang->balls[i].id == CGamePang_ballsmall)
						GamePang->balls[i].curforce += 0.25f*yscale;
				}
			}

			GamePang->balls[i].pos.y += GamePang->balls[i].curforce;

			if (GamePang->balls[i].id == CGamePang_ballbig)
				if (GamePang->balls[i].pos.y >= GamePang->GameBase->screenbottom - 135.0f*yscale)
					GamePang->balls[i].curforce = GamePang->balls[i].force;

			if (GamePang->balls[i].id == CGamePang_ballmedium)
				if (GamePang->balls[i].pos.y >= GamePang->GameBase->screenbottom - 100.0f*yscale)
					GamePang->balls[i].curforce = GamePang->balls[i].force;

			if (GamePang->balls[i].id == CGamePang_ballsmall)
				if (GamePang->balls[i].pos.y >= GamePang->GameBase->screenbottom - 75.0f*yscale)
					GamePang->balls[i].curforce = GamePang->balls[i].force;

			CSprites_SetSpriteLocation(GamePang->balls[i].spr, GamePang->balls[i].pos);

			if (!GamePang->playerstate(GamePang,CGamePang_playerstatereviving) && CSprites_DetectSpriteCollision(GamePang->balls[i].spr, GamePang->player.spr))
			{
				CGame_AddToScore(-25);
				GamePang->GameBase->HealthPoints -= 1;
				GamePang->addplayerstate(GamePang,CGamePang_playerstatereviving);
				GamePang->remplayerstate(GamePang,CGamePang_playerstateshoot);
				GamePang->player.stateticks = 90;
				CAudio_PlaySound(GamePang->SfxDie, 0);
			}

			if (GamePang->bullet.alive && (GamePang->bullet.freeze == 0))
			{
				if (CSprites_DetectSpriteCollision(GamePang->balls[i].spr, GamePang->bullet.spr))
				{
					CGame_AddToScore(GamePang->balls[i].id * 7);
					GamePang->destroyball(GamePang,i, false);
					GamePang->bullet.freeze = 4;
					CSprites_SetSpriteAnimation(GamePang->bullet.spr, 0, 1, 10);
					CAudio_PlaySound(GamePang->SfxPop, 0);
				}
			}
		}
	}
	if (GamePang->levelcleared)
	{
		if (GamePang->GameBase->level < CGamePang_maxbigballs)
			GamePang->GameBase->level += 1;
		CAudio_PlaySound(GamePang->SfxSucces, 0);
		CGame_AddToScore(100);
		GamePang->createballs(GamePang);
	}
}

void CGamePang_drawballs(CGamePang* GamePang)
{
	for (int i = 0; i < CGamePang_maxballs; i++)
	{
		if (GamePang->balls[i].alive)
			CSprites_DrawSprite(Renderer, GamePang->balls[i].spr);

	}
}

void CGamePang_createballs(CGamePang* GamePang)
{
	int added = 0;
	float speed = 10.0f*yscale;
	for (int i = GamePang->GameBase->screenleft; i < GamePang->GameBase->screenright; i += (int)((GamePang->GameBase->screenright - GamePang->GameBase->screenleft) / (GamePang->GameBase->level + 1)) + 1)
	{
		if ((i > GamePang->GameBase->screenleft) && (i < GamePang->GameBase->screenright))
		{
			GamePang->createball(GamePang,CGamePang_ballbig, i, 160.0f*yscale, speed);
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
		CSprites_RemoveSprite(GamePang->bullet.spr);
		GamePang->bullet.alive = false;
	}
}

void CGamePang_createbullet(CGamePang* GamePang)
{
	GamePang->bullet.spr = CSprites_CreateSprite();
	CSprites_SetSpriteImageTiles(Renderer,GamePang->bullet.spr, &GamePang->spritesheetbullet, 1, 2);
	CSprites_SetSpriteAnimation(GamePang->bullet.spr, 0, 0, 0);
	CSprites_SetSpriteCollisionShape(GamePang->bullet.spr, SHAPE_BOX, CSprites_TileSize(GamePang->bullet.spr).y-18, CSprites_TileSize(GamePang->bullet.spr).x+160,0,0,0);

	CSprites_SetSpriteRotation(GamePang->bullet.spr, 90);
	CSprites_SetSpriteScale(Renderer,GamePang->bullet.spr, GamePang->bulletscale);
	CSprites_SetSpriteDepth(GamePang->bullet.spr, 3);
	GamePang->bullet.tz = CSprites_TileSize(GamePang->bullet.spr);
	GamePang->bullet.tz.x = GamePang->bullet.tz.x * GamePang->bulletscale.x;
	GamePang->bullet.tz.y = GamePang->bullet.tz.y * GamePang->bulletscale.y;
	int tmpx = GamePang->bullet.tz.x;
	GamePang->bullet.tz.x = GamePang->bullet.tz.y;
	GamePang->bullet.tz.y = tmpx;
	GamePang->bullet.pos.x = GamePang->player.pos.x;
	GamePang->bullet.pos.y = GamePang->player.pos.y - (GamePang->player.tz.y) + (GamePang->bullet.tz.y /2);
	GamePang->bullet.vel.x = 0;
	GamePang->bullet.vel.y = -CGamePang_bulletspeed;
	CSprites_SetSpriteLocation(GamePang->bullet.spr, GamePang->bullet.pos);
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
				GamePang->remplayerstate(GamePang,CGamePang_playerstateshoot);
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
				CSprites_SetSpriteAnimation(GamePang->bullet.spr, 0, 1, 10);
			}
			CSprites_SetSpriteLocation(GamePang->bullet.spr, GamePang->bullet.pos);
		}
	}
}

void CGamePang_drawbullet(CGamePang* GamePang)
{
	if (GamePang->bullet.alive)
	{
		//copy whats on screen to a temporary buffer (i know this already contains the background)
		SDL_Texture* prev = SDL_GetRenderTarget(Renderer);
		SDL_SetRenderTarget(Renderer, TexTmp);
		CImage_DrawImageTex(Renderer, prev, NULL, NULL);
		SDL_SetRenderTarget(Renderer, prev);
		//draw the sprite
		CSprites_DrawSprite(Renderer, GamePang->bullet.spr);
		//draw bottom part of what was previously on screen back to the screen to obscure bottom part of the chain texture
		//this makes it seem as if the texture is created on the ground instead of at the bottom of the screen, like it is
		//in real time.
		SDL_Rect Rect = {0, GamePang->GameBase->screenbottom - CGamePang_backgroundcopyheight, GamePang->GameBase->screenright, CGamePang_backgroundcopyheight};
		CImage_DrawImageTex(Renderer, TexTmp, &Rect, &Rect);
	}
}

//player ----------------------------------------------------------------------------------------------------------------

void CGamePang_destroyplayer(CGamePang* GamePang)
{
	if(GamePang->player.alive)
	{
		CSprites_RemoveSprite(GamePang->player.spr);
		GamePang->player.alive = false;
	}
}

void CGamePang_createplayer(CGamePang* GamePang)
{
	GamePang->player.spr = CSprites_CreateSprite();
	CSprites_SetSpriteImageTiles(Renderer,GamePang->player.spr, &GamePang->spritesheetplayer, 12, 8);
	CSprites_SetSpriteAnimation(GamePang->player.spr, 37, 37, 0);
	CSprites_SetSpriteCollisionShape(GamePang->player.spr, SHAPE_BOX, CSprites_TileSize(GamePang->player.spr).x - 24, CSprites_TileSize(GamePang->player.spr).y-14,0,0,14*yscale);

	CSprites_SetSpriteScale(Renderer,GamePang->player.spr, GamePang->playerscale);
	CSprites_SetSpriteDepth(GamePang->player.spr, 37);
	GamePang->player.state = CGamePang_playerstateidle;
	GamePang->player.tz = CSprites_TileSize(GamePang->player.spr);
	GamePang->player.tz.x = GamePang->player.tz.x * GamePang->playerscale.x;
	GamePang->player.tz.y = GamePang->player.tz.y * GamePang->playerscale.y;
	GamePang->player.pos.x = (float)(GamePang->GameBase->screenright - GamePang->GameBase->screenleft) / 2;
	GamePang->player.pos.y = (float)GamePang->GameBase->screenbottom -10- GamePang->player.tz.y / 2;
	GamePang->GameBase->HealthPoints = 3;
	CSprites_SetSpriteLocation(GamePang->player.spr, GamePang->player.pos);
	GamePang->player.alive = true;
	GamePang->player.state = CGamePang_playerstateidle;
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
	CSprites_SetSpriteVisibility(GamePang->player.spr, GamePang->player.alive);

	if (GamePang->player.alive)
	{
		if (GamePang->player.stateticks > 0)
			GamePang->player.stateticks -= 1;
		else
		{
			if (GamePang->playerstate(GamePang,CGamePang_playerstateshoot))
				GamePang->remplayerstate(GamePang,CGamePang_playerstateshoot);
			else
			{
				if (GamePang->playerstate(GamePang,CGamePang_playerstatereviving))
					GamePang->remplayerstate(GamePang,CGamePang_playerstatereviving);
			}
		}

		if (!GamePang->playerstate(GamePang,CGamePang_playerstateshoot))
		{
			if ((CInput_Buttons.ButLeft) ||
				(CInput_Buttons.ButLeft2) ||
				(CInput_Buttons.ButDpadLeft))
			{
				if ( GamePang->player.pos.x - GamePang->player.tz.x / 2 - CGamePang_playerspeed > GamePang->GameBase->screenleft)
				{
					GamePang->player.pos.x -=CGamePang_playerspeed;
					if (!GamePang->playerstate(GamePang,CGamePang_playerstatemoveleft))
					{
						GamePang->addplayerstate(GamePang,CGamePang_playerstatemoveleft);
						CSprites_SetSpriteAnimation(GamePang->player.spr, 12, 14, 10);
					}
				}
				else
				{
					GamePang->player.pos.x = GamePang->GameBase->screenleft + GamePang->player.tz.x / 2;
					if (GamePang->playerstate(GamePang,CGamePang_playerstatemoveleft))
					{
						GamePang->remplayerstate(GamePang,CGamePang_playerstatemoveleft);
						CSprites_SetSpriteAnimation(GamePang->player.spr, 37, 37, 0);
					}
				}
			}
			else
			{
				if ((CInput_Buttons.ButRight) ||
					(CInput_Buttons.ButRight2) ||
					(CInput_Buttons.ButDpadRight))
				{
					if ( GamePang->player.pos.x + GamePang->player.tz.x / 2 + CGamePang_playerspeed < GamePang->GameBase->screenright)
					{
						GamePang->player.pos.x += CGamePang_playerspeed;
						if (!GamePang->playerstate(GamePang,CGamePang_playerstatemoveright))
						{
							GamePang->addplayerstate(GamePang,CGamePang_playerstatemoveright);
							CSprites_SetSpriteAnimation(GamePang->player.spr, 24, 26, 10);
						}
					}
					else
					{
						GamePang->player.pos.x = GamePang->GameBase->screenright - GamePang->player.tz.x / 2;
						if (GamePang->playerstate(GamePang,CGamePang_playerstatemoveleft))
						{
							GamePang->remplayerstate(GamePang,CGamePang_playerstatemoveright);
							CSprites_SetSpriteAnimation(GamePang->player.spr,37, 37, 0);
						}
					}
				}
				else
				{
					GamePang->remplayerstate(GamePang,CGamePang_playerstatemoveright);
					GamePang->remplayerstate(GamePang,CGamePang_playerstatemoveleft);
					CSprites_SetSpriteAnimation(GamePang->player.spr, 37, 37, 0);
				}
			}

			if ((!GamePang->playerstate(GamePang,CGamePang_playerstatereviving)) && (!CInput_PrevButtons.ButA && CInput_Buttons.ButA))
			{
				if (!GamePang->bullet.alive)
				{
					GamePang->addplayerstate(GamePang,CGamePang_playerstateshoot);
					GamePang->remplayerstate(GamePang,CGamePang_playerstatemoveright);
					GamePang->remplayerstate(GamePang,CGamePang_playerstatemoveleft);
					CSprites_SetSpriteAnimation(GamePang->player.spr, 37, 37, 10);
					GamePang->player.stateticks = 15;
					GamePang->createbullet(GamePang);
					CAudio_PlaySound(GamePang->SfxShoot, 0);
				}
			}
			CSprites_SetSpriteLocation(GamePang->player.spr, GamePang->player.pos);
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
		if (GamePang->playerstate(GamePang,CGamePang_playerstatereviving))
		{
			if (GamePang->player.stateticks % 3 == 0)
				CSprites_DrawSprite(Renderer, GamePang->player.spr);
		}
		else
			CSprites_DrawSprite(Renderer, GamePang->player.spr);
	}
}


//background ----------------------------------------------------------------------------------------------------------------

void CGamePang_DrawBackground(CGamePang* GamePang)
{
	CImage_DrawImage(Renderer, GamePang->background, NULL, NULL);
}

//init - deinit ----------------------------------------------------------------------------------------------------------------

void CGamePang_init(CGamePang* GamePang)
{
	GamePang->LoadGraphics(GamePang);
	GamePang->GameBase->level = 1;
	GamePang->LoadSound(GamePang);
	GamePang->createplayer(GamePang);
	GamePang->createballs(GamePang);
	CurrentGameMusicID = GamePang->MusMusic;
	CAudio_PlayMusic(GamePang->MusMusic, -1);
}

void CGamePang_deinit(CGamePang* GamePang)
{
	GamePang->destroyplayer(GamePang);
	GamePang->destroyallballs(GamePang);
	GamePang->destroybullet(GamePang);
	GamePang->UnLoadSound(GamePang);
	SubStateCounter = 0;
	SubGameState = SGNone;
	CurrentGameMusicID = -1;
	GamePang->UnloadGraphics(GamePang);
}

void CGamePang_LoadSound(CGamePang* GamePang)
{
	GamePang->SfxDie = CAudio_LoadSound("common/die.wav");
	GamePang->SfxSucces = CAudio_LoadSound("common/succes.wav");
	GamePang->SfxShoot = CAudio_LoadSound("pang/shoot.wav");
	GamePang->SfxPop = CAudio_LoadSound("pang/pop.wav");
	GamePang->MusMusic = CAudio_LoadMusic("pang/music.ogg");
}

void CGamePang_UnLoadSound(CGamePang* GamePang)
{
	CAudio_StopMusic();
	CAudio_StopSound();
	CAudio_UnLoadMusic(GamePang->MusMusic);
	CAudio_UnLoadSound(GamePang->SfxDie);
	CAudio_UnLoadSound(GamePang->SfxSucces);
	CAudio_UnLoadSound(GamePang->SfxShoot);
	CAudio_UnLoadSound(GamePang->SfxPop);
}

void CGamePang_LoadGraphics(CGamePang* GamePang)
{
	GamePang->background = CImage_LoadImage(Renderer, "pang/background.png");
	GamePang->spritesheetplayer = CImage_LoadImageEx(Renderer, "pang/character.png",0, 118, dumpScaledBitmaps);
	GamePang->spritesheetbullet = CImage_LoadImageEx(Renderer, "pang/weapon.png", 0, 128, dumpScaledBitmaps);
	GamePang->spritesheetball = CImage_LoadImageEx(Renderer, "pang/ball.png",0, 128, dumpScaledBitmaps);
	
	// SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/pang/character.bmp", CImage_GetImage(GamePang->spritesheetplayer), 1,1, true, 0, 118);
	// SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/pang/weapon.bmp", CImage_GetImage(GamePang->spritesheetbullet), 1,1, true, 0, 128);
	// SDL_SaveBMPTextureScaled(Renderer, "./retrotimefs/graphics/pang/ball.bmp", CImage_GetImage(GamePang->spritesheetball), 1,1, true, 0, 128);


	if (!useDefaultColorAssets)
	{
		GamePang->UnloadGraphics(GamePang);
		GamePang->background = CImage_LoadImage(Renderer, "pang/background.png");
		GamePang->spritesheetplayer = CImage_LoadImage(Renderer, "pang/character.bmp");
		GamePang->spritesheetbullet = CImage_LoadImage(Renderer, "pang/weapon.bmp");
	 	GamePang->spritesheetball = CImage_LoadImage(Renderer, "pang/ball.bmp");
	}
	
	
	GamePang->spritesheetballtz = CImage_ImageSize(GamePang->spritesheetball);

}

void CGamePang_UnloadGraphics(CGamePang* GamePang)
{
	CImage_UnLoadImage(GamePang->spritesheetplayer);
	CImage_UnLoadImage(GamePang->spritesheetbullet);
	CImage_UnLoadImage(GamePang->spritesheetball);
	CImage_UnLoadImage(GamePang->background);
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
	GamePang->UpdateObjects(GamePang, SubGameState == SGGame);
	if(SubGameState == SGGame)
		CSprites_UpdateSprites(Renderer);
}

bool CGamePang_DrawObjects(CGamePang* GamePang)
{
	GamePang->drawballs(GamePang);
	GamePang->drawbullet(GamePang);
	GamePang->drawplayer(GamePang);
	//don't call drawsprites
	return false;
}

void CGamePang_Draw(CGamePang* GamePang)
{
	GamePang->DrawBackground(GamePang);
	if (GamePang->DrawObjects(GamePang))
		CSprites_DrawSprites(Renderer);
	GamePang->GameBase->DrawScoreBar(GamePang->GameBase);
	GamePang->GameBase->DrawSubStateText(GamePang->GameBase);
}