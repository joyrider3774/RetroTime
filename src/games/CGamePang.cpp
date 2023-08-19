#include <SDL.h>
#include <string>
#include <iostream>
#include <cmath>
#include "CGamePang.h"
#include "../CGame.h"
#include "../Common.h"
#include "../Vec2F.h"

using namespace std;

CGamePang::CGamePang(CGame* aGame)
{
	GameBase = new CGameBase(aGame, GSPang, true);

	MusMusic = -1;
	SfxSucces = -1;
	SfxDie = -1;
	SfxShoot = -1;
	SfxPop = -1;
	GameBase->screenleft = 0;
	GameBase->screenright = ScreenWidth;
	GameBase->screentop = 0;
	GameBase->screenbottom = ScreenHeight;
}

CGamePang::~CGamePang()
{

}



//balls ----------------------------------------------------------------------------------------------------------------

void CGamePang::destroyallballs()
{
	for(int i = 0; i < maxballs; i++)
		destroyball(i, true);
}

void CGamePang::destroyball(int index, bool nocreate)
{
	if (!nocreate)
	{
		if (balls[index].id > ballsmall)
		{
			createball(balls[index].id - 1, balls[index].pos.x - 25*yscale, balls[index].pos.y, -10.0f*yscale);
			createball(balls[index].id - 1, balls[index].pos.x + 25*yscale, balls[index].pos.y, 10.0f*yscale);
		}
	}
	if (balls[index].alive)
	{
		GameBase->Game->Sprites->RemoveSprite(balls[index].spr);
		balls[index].alive = false;
	}
}

void CGamePang::createball(int size, float x, float y, float speed)
{
	for (int i = 0; i < maxballs; i++)
	{
		if (!balls[i].alive)
		{
			balls[i].spr = GameBase->Game->Sprites->CreateSprite();
			GameBase->Game->Sprites->SetSpriteImage(GameBase->Game->Renderer,balls[i].spr, &spritesheetball);
			GameBase->Game->Sprites->SetSpriteCollisionShape(balls[i].spr, SHAPE_CIRCLE, 35,35,0,0,0);
			//GameBase->Game->Sprites->SetSpriteCollisionShape(balls[i].spr, SHAPE_BOX, 27,27,0,0,0);
			GameBase->Game->Sprites->SetSpriteColour(balls[i].spr, 1, 1, 1, 0.7f);
			Vec2F Scale = ballscale;
			Scale.x = Scale.x * size;
			Scale.y = Scale.y * size;
			GameBase->Game->Sprites->SetSpriteScale(GameBase->Game->Renderer,balls[i].spr, Scale);
			GameBase->Game->Sprites->SetSpriteDepth(balls[i].spr, 6);
			balls[i].tz = GameBase->Game->Image->ImageSize(spritesheetball);
			balls[i].tz.x = balls[i].tz.x * Scale.x;
			balls[i].tz.x = balls[i].tz.y * Scale.y;
			balls[i].pos = { x, y};
			GameBase->Game->Sprites->SetSpriteLocation(balls[i].spr, balls[i].pos);
			balls[i].alive = true;
			balls[i].speed = speed*0.1f;
			balls[i].force = -abs(speed);
			balls[i].curforce = balls[i].force/3;
			balls[i].id = size;
			break;
		}
	}
}

void CGamePang::updateballs()
{
	levelcleared = true;
	for (int i = 0; i < maxballs; i++)
	{
		if (balls[i].alive)
		{
			levelcleared = false;
			if (balls[i].pos.x + balls[i].speed > GameBase->screenright)
			{
				if (balls[i].speed > 0)
					balls[i].speed = -abs(balls[i].speed);

			}

			if (balls[i].pos.x + balls[i].speed < GameBase->screenleft)
			{
				if (balls[i].speed < 0)
					balls[i].speed = abs(balls[i].speed);
			}


			balls[i].pos.x += balls[i].speed*2;

			if (balls[i].id == ballbig)
				balls[i].curforce += 0.1f*yscale;
			else
			{
				if(balls[i].id == ballmedium)
					balls[i].curforce += 0.15f*yscale;
				else
				{
					if (balls[i].id == ballsmall)
						balls[i].curforce += 0.25f*yscale;
				}
			}

			balls[i].pos.y += balls[i].curforce;

			if (balls[i].id == ballbig)
				if (balls[i].pos.y >= GameBase->screenbottom - 135.0f*yscale)
					balls[i].curforce = balls[i].force;

			if (balls[i].id == ballmedium)
				if (balls[i].pos.y >= GameBase->screenbottom - 100.0f*yscale)
					balls[i].curforce = balls[i].force;

			if (balls[i].id == ballsmall)
				if (balls[i].pos.y >= GameBase->screenbottom - 75.0f*yscale)
					balls[i].curforce = balls[i].force;

			GameBase->Game->Sprites->SetSpriteLocation(balls[i].spr, balls[i].pos);

			if (!playerstate(playerstatereviving) && GameBase->Game->Sprites->DetectSpriteCollision(balls[i].spr, player.spr))
			{
				GameBase->Game->AddToScore(-25);
				GameBase->HealthPoints -= 1;
				addplayerstate(playerstatereviving);
				remplayerstate(playerstateshoot);
				player.stateticks = 90;
				GameBase->Game->Audio->PlaySound(SfxDie, 0);
			}

			if (bullet.alive && (bullet.freeze == 0))
			{
				if (GameBase->Game->Sprites->DetectSpriteCollision(balls[i].spr, bullet.spr))
				{
					GameBase->Game->AddToScore(balls[i].id * 7);
					destroyball(i, false);
					bullet.freeze = 4;
					GameBase->Game->Sprites->SetSpriteAnimation(bullet.spr, 0, 1, 10);
					GameBase->Game->Audio->PlaySound(SfxPop, 0);
				}
			}
		}
	}
	if (levelcleared)
	{
		if (GameBase->level < maxbigballs)
			GameBase->level += 1;
		GameBase->Game->Audio->PlaySound(SfxSucces, 0);
		GameBase->Game->AddToScore(100);
		createballs();
	}
}

void CGamePang::drawballs()
{
	for (int i = 0; i < maxballs; i++)
	{
		if (balls[i].alive)
			GameBase->Game->Sprites->DrawSprite(GameBase->Game->Renderer, balls[i].spr);

	}
}

void CGamePang::createballs()
{
	int added = 0;
	float speed = 10.0f*yscale;
	for (int i = GameBase->screenleft; i < GameBase->screenright; i += int((GameBase->screenright - GameBase->screenleft) / (GameBase->level + 1)) + 1)
	{
		if ((i > GameBase->screenleft) && (i < GameBase->screenright))
		{
			createball(ballbig, i, 160.0f*yscale, speed);
			speed *= -1;
			added += 1;
			if (added >= GameBase->level)
				break;
		}
	}
}

//bullet ----------------------------------------------------------------------------------------------------------------

void CGamePang::destroybullet()
{
	if (bullet.alive)
	{
		GameBase->Game->Sprites->RemoveSprite(bullet.spr);
		bullet.alive = false;
	}
}

void CGamePang::createbullet()
{
	bullet.spr = GameBase->Game->Sprites->CreateSprite();
	GameBase->Game->Sprites->SetSpriteImage(GameBase->Game->Renderer,bullet.spr, &spritesheetbullet, 1, 2);
	GameBase->Game->Sprites->SetSpriteAnimation(bullet.spr, 0, 0, 0);
	GameBase->Game->Sprites->SetSpriteCollisionShape(bullet.spr, SHAPE_BOX, GameBase->Game->Sprites->TileSize(bullet.spr).y-18, GameBase->Game->Sprites->TileSize(bullet.spr).x+160,0,0,0);

	GameBase->Game->Sprites->SetSpriteRotation(bullet.spr, 90);
	GameBase->Game->Sprites->SetSpriteScale(GameBase->Game->Renderer,bullet.spr, bulletscale);
	GameBase->Game->Sprites->SetSpriteDepth(bullet.spr, 3);
	bullet.tz = GameBase->Game->Sprites->TileSize(bullet.spr);
	bullet.tz.x = bullet.tz.x * bulletscale.x;
	bullet.tz.y = bullet.tz.y * bulletscale.y;
	int tmpx = bullet.tz.x;
	bullet.tz.x = bullet.tz.y;
	bullet.tz.y = tmpx;
	bullet.pos = { player.pos.x, player.pos.y - (player.tz.y) + (bullet.tz.y /2)};
	bullet.vel = {0, -bulletspeed};
	GameBase->Game->Sprites->SetSpriteLocation(bullet.spr, bullet.pos);
	bullet.alive = true;
}

void CGamePang::updatebullet()
{
	if (bullet.alive)
	{
		if (bullet.freeze > 0)
		{
			bullet.freeze -= 1;
			if (bullet.freeze == 0)
			{
				remplayerstate(playerstateshoot);
				destroybullet();
			}
		}
		else
		{
			bullet.pos.x += bullet.vel.x;
			bullet.pos.y += bullet.vel.y;
			if (bullet.pos.y - bullet.tz.y /2 <= GameBase->screentop)
			{
				bullet.freeze = 6;
				GameBase->Game->Sprites->SetSpriteAnimation(bullet.spr, 0, 1, 10);
			}
			GameBase->Game->Sprites->SetSpriteLocation(bullet.spr, bullet.pos);
		}
	}
}

void CGamePang::drawbullet()
{
	if (bullet.alive)
	{
		//copy whats on screen to a temporary buffer (i know this already contains the background)
		SDL_Texture* prev = SDL_GetRenderTarget(GameBase->Game->Renderer);
		SDL_SetRenderTarget(GameBase->Game->Renderer, GameBase->Game->TexTmp);
		GameBase->Game->Image->DrawImage(GameBase->Game->Renderer, prev, NULL, NULL);
		SDL_SetRenderTarget(GameBase->Game->Renderer, prev);
		//draw the sprite
		GameBase->Game->Sprites->DrawSprite(GameBase->Game->Renderer, bullet.spr);
		//draw bottom part of what was previously on screen back to the screen to obscure bottom part of the chain texture
		//this makes it seem as if the texture is created on the ground instead of at the bottom of the screen, like it is
		//in real time.
		SDL_Rect Rect = {0, GameBase->screenbottom - backgroundcopyheight, GameBase->screenright, backgroundcopyheight};
		GameBase->Game->Image->DrawImage(GameBase->Game->Renderer, GameBase->Game->TexTmp, &Rect, &Rect);
	}
}

//player ----------------------------------------------------------------------------------------------------------------

void CGamePang::destroyplayer()
{
	if(player.alive)
	{
		GameBase->Game->Sprites->RemoveSprite(player.spr);
		player.alive = false;
	}
}

void CGamePang::createplayer()
{
	player.spr = GameBase->Game->Sprites->CreateSprite();
	GameBase->Game->Sprites->SetSpriteImage(GameBase->Game->Renderer,player.spr, &spritesheetplayer, 12, 8);
	GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 37, 37, 0);
	GameBase->Game->Sprites->SetSpriteCollisionShape(player.spr, SHAPE_BOX, GameBase->Game->Sprites->TileSize(player.spr).x - 24, GameBase->Game->Sprites->TileSize(player.spr).y-14,0,0,14*yscale);

	GameBase->Game->Sprites->SetSpriteScale(GameBase->Game->Renderer,player.spr, playerscale);
	GameBase->Game->Sprites->SetSpriteDepth(player.spr, 37);
	player.state = playerstateidle;
	player.tz = GameBase->Game->Sprites->TileSize(player.spr);
	player.tz.x = player.tz.x * playerscale.x;
	player.tz.y = player.tz.y * playerscale.y;
	player.pos = { (float)(GameBase->screenright - GameBase->screenleft) / 2,(float)GameBase->screenbottom -10- player.tz.y / 2};
	GameBase->HealthPoints = 3;
	GameBase->Game->Sprites->SetSpriteLocation(player.spr, player.pos);
	player.alive = true;
	player.state = playerstateidle;
}

void CGamePang::addplayerstate(int state)
{
	player.state |= state;
}

bool CGamePang::playerstate(int state)
{
	return ((player.state & state) == state);
}

void CGamePang::remplayerstate(int state)
{
	if ((player.state & state) == state)
		player.state -= state;
}

void CGamePang::updateplayer()
{
	GameBase->Game->Sprites->SetSpriteVisibility(player.spr, player.alive);

	if (player.alive)
	{
		if (player.stateticks > 0)
			player.stateticks -= 1;
		else
		{
			if (playerstate(playerstateshoot))
				remplayerstate(playerstateshoot);
			else
			{
				if (playerstate(playerstatereviving))
					remplayerstate(playerstatereviving);
			}
		}

		if (!playerstate(playerstateshoot))
		{
			if ((GameBase->Game->Input->Buttons.ButLeft) ||
				(GameBase->Game->Input->Buttons.ButLeft2) ||
				(GameBase->Game->Input->Buttons.ButDpadLeft))
			{
				if ( player.pos.x - player.tz.x / 2 - playerspeed > GameBase->screenleft)
				{
					player.pos.x -=playerspeed;
					if (!playerstate(playerstatemoveleft))
					{
						addplayerstate(playerstatemoveleft);
						GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 12, 14, 10);
					}
				}
				else
				{
					player.pos.x = GameBase->screenleft + player.tz.x / 2;
					if (playerstate(playerstatemoveleft))
					{
						remplayerstate(playerstatemoveleft);
						GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 37, 37, 0);
					}
				}
			}
			else
			{
				if ((GameBase->Game->Input->Buttons.ButRight) ||
					(GameBase->Game->Input->Buttons.ButRight2) ||
					(GameBase->Game->Input->Buttons.ButDpadRight))
				{
					if ( player.pos.x + player.tz.x / 2 + playerspeed < GameBase->screenright)
					{
						player.pos.x += playerspeed;
						if (!playerstate(playerstatemoveright))
						{
							addplayerstate(playerstatemoveright);
							GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 24, 26, 10);
						}
					}
					else
					{
						player.pos.x = GameBase->screenright - player.tz.x / 2;
						if (playerstate(playerstatemoveleft))
						{
							remplayerstate(playerstatemoveright);
							GameBase->Game->Sprites->SetSpriteAnimation(player.spr,37, 37, 0);
						}
					}
				}
				else
				{
					remplayerstate(playerstatemoveright);
					remplayerstate(playerstatemoveleft);
					GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 37, 37, 0);
				}
			}

			if ((!playerstate(playerstatereviving)) && (!GameBase->Game->Input->PrevButtons.ButA && GameBase->Game->Input->Buttons.ButA))
			{
				if (!bullet.alive)
				{
					addplayerstate(playerstateshoot);
					remplayerstate(playerstatemoveright);
					remplayerstate(playerstatemoveleft);
					GameBase->Game->Sprites->SetSpriteAnimation(player.spr, 37, 37, 10);
					player.stateticks = 15;
					createbullet();
					GameBase->Game->Audio->PlaySound(SfxShoot, 0);
				}
			}
			GameBase->Game->Sprites->SetSpriteLocation(player.spr, player.pos);
		}
	}
	else
	{
		if (player.freeze > 0)
			player.freeze -= 1;
		else
			player.alive = true;
	}
}

void CGamePang::drawplayer()
{
	if (player.alive)
	{
		if (playerstate(playerstatereviving))
		{
			if (player.stateticks % 3 == 0)
				GameBase->Game->Sprites->DrawSprite(GameBase->Game->Renderer, player.spr);
		}
		else
			GameBase->Game->Sprites->DrawSprite(GameBase->Game->Renderer, player.spr);
	}
}


//background ----------------------------------------------------------------------------------------------------------------

void CGamePang::DrawBackground(bool motionblur)
{
	GameBase->Game->Image->DrawImage(GameBase->Game->Renderer, background, NULL, NULL);
}

//init - deinit ----------------------------------------------------------------------------------------------------------------

void CGamePang::init()
{
	LoadGraphics();
	GameBase->level = 1;
	LoadSound();
	createplayer();
	createballs();
	GameBase->Game->CurrentGameMusicID = MusMusic;
	GameBase->Game->Audio->PlayMusic(MusMusic, -1);
}

void CGamePang::deinit()
{
	destroyplayer();
	destroyallballs();
	destroybullet();
	UnLoadSound();
	GameBase->Game->SubStateCounter = 0;
	GameBase->Game->SubGameState = SGNone;
	GameBase->Game->CurrentGameMusicID = -1;
	UnloadGraphics();
}

void CGamePang::LoadSound()
{
	SfxDie = GameBase->Game->Audio->LoadSound("common/die.wav");
	SfxSucces = GameBase->Game->Audio->LoadSound("common/succes.wav");
	SfxShoot = GameBase->Game->Audio->LoadSound("pang/shoot.wav");
	SfxPop = GameBase->Game->Audio->LoadSound("pang/pop.wav");
	MusMusic = GameBase->Game->Audio->LoadMusic("pang/music.ogg");
}

void CGamePang::UnLoadSound()
{
	GameBase->Game->Audio->StopMusic();
	GameBase->Game->Audio->StopSound();
	GameBase->Game->Audio->UnLoadMusic(MusMusic);
	GameBase->Game->Audio->UnLoadSound(SfxDie);
	GameBase->Game->Audio->UnLoadSound(SfxSucces);
	GameBase->Game->Audio->UnLoadSound(SfxShoot);
	GameBase->Game->Audio->UnLoadSound(SfxPop);
}

void CGamePang::LoadGraphics()
{
	background = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "pang/background.png");
	spritesheetplayer = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "pang/character.png",0, 118, dumpScaledBitmaps);
	spritesheetbullet = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "pang/weapon.png", 0, 128, dumpScaledBitmaps);
	spritesheetball = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "pang/ball.png",0, 128, dumpScaledBitmaps);
	
	// SDL_SaveBMPTextureScaled(GameBase->Game->Renderer, "./retrotimefs/graphics/pang/character.bmp", GameBase->Game->Image->GetImage(spritesheetplayer), 1,1, true, 0, 118);
	// SDL_SaveBMPTextureScaled(GameBase->Game->Renderer, "./retrotimefs/graphics/pang/weapon.bmp", GameBase->Game->Image->GetImage(spritesheetbullet), 1,1, true, 0, 128);
	// SDL_SaveBMPTextureScaled(GameBase->Game->Renderer, "./retrotimefs/graphics/pang/ball.bmp", GameBase->Game->Image->GetImage(spritesheetball), 1,1, true, 0, 128);


	if (!useDefaultColorAssets)
	{
		UnloadGraphics();
		background = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "pang/background.png");
		spritesheetplayer = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "pang/character.bmp");
		spritesheetbullet = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "pang/weapon.bmp");
	 	spritesheetball = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "pang/ball.bmp");
	}
	
	
	spritesheetballtz = GameBase->Game->Image->ImageSize(spritesheetball);

}

void CGamePang::UnloadGraphics()
{
	GameBase->Game->Image->UnLoadImage(spritesheetplayer);
	GameBase->Game->Image->UnLoadImage(spritesheetbullet);
	GameBase->Game->Image->UnLoadImage(spritesheetball);
	GameBase->Game->Image->UnLoadImage(background);
}

//Update ----------------------------------------------------------------------------------------------------------------

void CGamePang::UpdateObjects(bool IsGameState)
{
	if (IsGameState)
	{
		updateplayer();
		updateballs();
		updatebullet();
	}
}

void CGamePang::UpdateLogic()
{
	GameBase->UpdateLogic();
	UpdateObjects(GameBase->Game->SubGameState == SGGame);
	if(GameBase->Game->SubGameState == SGGame)
		GameBase->Game->Sprites->UpdateSprites(GameBase->Game->Renderer);
}

bool CGamePang::DrawObjects()
{
	drawballs();
	drawbullet();
	drawplayer();
	//don't call drawsprites in base class
	return false;
}

void CGamePang::Draw()
{
	DrawBackground((GameBase->Game->SubGameState == SGGame));
	if (DrawObjects())
		GameBase->Game->Sprites->DrawSprites(GameBase->Game->Renderer);
	GameBase->DrawScoreBar();
	GameBase->DrawSubStateText();
}