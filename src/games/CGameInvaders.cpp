#include <SDL.h>
#include <string>
#include <iostream>
#include <cmath>
#include "CGameInvaders.h"
#include "../CGame.h"
#include "../Common.h"
#include "../Vec2F.h"

using namespace std;

CGameInvaders::CGameInvaders(CGame* aGame, bool aScreenshotMode): CGameBase(aGame, GSSpaceInvaders, false, aScreenshotMode)
{
	Game = aGame;
	MusMusic = -1;
	SfxSucces = -1;
	SfxPlayerShoot = -1;
	SfxPlayerDeath = -1;
	SfxEnemyShoot = -1;
	SfxEnemyDeath = -1;
	SfxDie = -1;
	screenleft = 0;
	screenright = ScreenWidth;
	screentop = 0;
	screenbottom = ScreenHeight;

}

CGameInvaders::~CGameInvaders(){};

//explosions ----------------------------------------------------------------------------------------------------------------

void CGameInvaders::checkexplosions()
{
	for(int i = 0; i < maxexplosion; i++)
	{
		if (explosions[i].alive)
		{
			if (Game->Sprites->GetSpriteAnimFrame(explosions[i].spr) == Game->Sprites->GetSpriteAnimFrameCount(explosions[i].spr) -1)
			{
				destroyexploison(i);
			}
		}
	}
}

void CGameInvaders::destroyallexplosion()
{
	for (int i = 0; i < maxexplosion; i++)
		destroyexploison(i);
}

void CGameInvaders::destroyexploison(int index)
{
	if (explosions[index].alive)
	{
		Game->Sprites->RemoveSprite(explosions[index].spr);
		explosions[index].alive = false;
	}
}

void CGameInvaders::createexplosion(Vec2F pos)
{
	for(int i = 0; i < maxexplosion;i++)
	{
		if (!explosions[i].alive)
		{
			explosions[i].spr = Game->Sprites->CreateSprite();
			Game->Sprites->SetSpriteImage(Game->Renderer,explosions[i].spr, &spritesheetExplosion, 7, 1);
			Game->Sprites->SetSpriteScale(Game->Renderer,explosions[i].spr, enemyscale);
			Game->Sprites->SetSpriteDepth(explosions[i].spr, 1);
			Game->Sprites->SetSpriteAnimation(explosions[i].spr,2, 6, 15);
			Game->Sprites->SetSpriteLocation(explosions[i].spr, pos);
			explosions[i].alive = true;
			Game->Audio->PlaySound(SfxPlayerDeath, 0);
			break;
		}
	}
}

//enemy bullets ----------------------------------------------------------------------------------------------------------------

void CGameInvaders::destroyallenemybullet()
{
	for(int i = 0; i < maxenemybullets; i++)
		destroyenemybullet(i);
}

void CGameInvaders::destroyenemybullet(int index)
{
	if (enemybullets[index].alive)
	{
		Game->Sprites->RemoveSprite(enemybullets[index].spr);
		enemybullets[index].alive = false;
	}
}

void CGameInvaders::createnemybullet(Vec2F pos)
{
	for(int i = 0; i < maxenemybullets; i++)
	{
		if (!enemybullets[i].alive)
		{
			enemybullets[i].spr = Game->Sprites->CreateSprite();
			Game->Sprites->SetSpriteImage(Game->Renderer,enemybullets[i].spr, &spritesheetBullet, 2, 1);
			Game->Sprites->SetSpriteColour(enemybullets[i].spr, 0.75,0.65,0.65,1);
			Game->Sprites->SetSpriteAnimation(enemybullets[i].spr, 1, 1, 0);
			Game->Sprites->SetSpriteScale(Game->Renderer,enemybullets[i].spr, enemyscale);
			Game->Sprites->SetSpriteDepth(enemybullets[i].spr, -1);
			enemybullets[i].tz = Game->Sprites->TileSize(enemybullets[i].spr);
			enemybullets[i].tz.x = enemybullets[i].tz.x * enemyscale.x;
			enemybullets[i].tz.y = enemybullets[i].tz.y * enemyscale.y;
			enemybullets[i].pos = pos;
			enemybullets[i].vel = {0, bulletspeed};
			enemybullets[i].alive = true;
			Game->Audio->PlaySound(SfxEnemyShoot, 0);
			break;
		}
	}
}

void CGameInvaders::updateenemybullet()
{
	if (!tweenactive(tweenenemypositions))
	{
		if (rand() % (enemybulletrandomizer) == 0)
		{
			int col = rand() % (enemycols);
			int y = 0;
			bool fired = false;
			while (!fired)
			{
				y = enemyrows -1;
				while (y >= 0)
				{
					if (enemies[col + y * enemycols].alive)
					{
						createnemybullet(enemies[col + y * enemycols].pos);
						fired = true;
						break;
					}
					y -= 1;
				}
				col += 1;
				if (col >= enemycols)
					col = 0;
			}
		}
	}

	for(int i = 0; i < maxenemybullets; i++)
	{
		if (enemybullets[i].alive)
		{
			enemybullets[i].pos.x += enemybullets[i].vel.x;
			enemybullets[i].pos.y += enemybullets[i].vel.y;
			Game->Sprites->SetSpriteLocation(enemybullets[i].spr, enemybullets[i].pos);
			if (enemybullets[i].pos.y >= screenbottom)
				destroyenemybullet(i);

			if (enemybullets[i].alive && player.alive)
			{
				if (Game->Sprites->DetectSpriteCollision(enemybullets[i].spr, player.spr))
				{
					player.alive = false;
					player.freeze = 30;
					createexplosion(player.pos);
					destroyenemybullet(i);
					Game->AddToScore(-150);
					deaths += 1;
					HealthPoints -= 1;
					Game->Audio->PlaySound(SfxDie, 0);
				}
			}


			for (int j = 0; j < maxasteroids; j++)
			{
				if (enemybullets[i].alive && asteroids[j].alive)
				{
					if (Game->Sprites->DetectSpriteCollision(enemybullets[i].spr, asteroids[j].spr))
					{
						createexplosion(enemybullets[i].pos);
						destroyenemybullet(i);
						asteroids[j].healthpoints -= 1;
						if (asteroids[j].healthpoints == 0)
							destroyasteroid(j);
					}
				}
			}

		}
	}
}

//asteroids ----------------------------------------------------------------------------------------------------------------

void CGameInvaders::destroyallasteroids()
{
	for(int i = 0; i < maxasteroids; i++)
		destroyasteroid(i);
}

void CGameInvaders::destroyasteroid(int index)
{
	if (asteroids[index].alive)
	{
		Game->Sprites->RemoveSprite(asteroids[index].spr);
		asteroids[index].alive = false;
	}
}

void CGameInvaders::createasteroids()
{
	for(int i = 0; i < maxasteroids; i++)
	{
		asteroids[i].spr = Game->Sprites->CreateSprite();
		asteroids[i].healthpoints = asteroidmaxhealthpoints;
		asteroids[i].alive = true;
		Game->Sprites->SetSpriteImage(Game->Renderer,asteroids[i].spr, &spritesheetAsteroid);
		Game->Sprites->SetSpriteAnimation(asteroids[i].spr, 0, 0, 0);
		Game->Sprites->SetSpriteLocation(asteroids[i].spr, {(float)i * (screenright - screenleft) / (maxasteroids-1), (float)screenbottom - asteroidsoffset});
		Game->Sprites->SetSpriteScale(Game->Renderer,asteroids[i].spr, {asteroidscale, asteroidscale});
		Game->Sprites->SetSpriteCollisionShape(asteroids[i].spr, SHAPE_BOX, 45,45,0, 0 , 2);
		Game->Sprites->SetSpriteRotation(asteroids[i].spr, rand() % (360));
	}
}

void CGameInvaders::updateasteroids()
{
	for(int i = 0; i < maxasteroids;i++)
	{
		if (asteroids[i].alive)
		{
			Game->Sprites->SetSpriteScale(Game->Renderer,asteroids[i].spr,
				{asteroidscale - ((asteroidscale / asteroidmaxhealthpoints) * (asteroidmaxhealthpoints - asteroids[i].healthpoints)),
				asteroidscale - ((asteroidscale / asteroidmaxhealthpoints) * (asteroidmaxhealthpoints - asteroids[i].healthpoints))});
			Game->Sprites->SetSpriteRotation(asteroids[i].spr, asteroids[i].spr->rotation + 0.5f);
		}
	}
}

//player bullet ----------------------------------------------------------------------------------------------------------------

void CGameInvaders::createbullet()
{
	if (!bullet.alive)
	{
		bullet.spr = Game->Sprites->CreateSprite();
		Game->Sprites->SetSpriteImage(Game->Renderer,bullet.spr, &spritesheetBullet, 2, 1);
		Game->Sprites->SetSpriteAnimation(bullet.spr, 1, 1, 0);
		Game->Sprites->SetSpriteScale(Game->Renderer,bullet.spr, enemyscale);
		Game->Sprites->SetSpriteDepth(bullet.spr, -1);
		bullet.tz = Game->Sprites->TileSize(bullet.spr);
		bullet.tz.x = bullet.tz.x * enemyscale.x;
		bullet.tz.y = bullet.tz.y * enemyscale.y;
		bullet.pos = player.pos ;
		bullet.vel = {0, -bulletspeed};
		bullet.alive = true;
		Game->Audio->PlaySound(SfxPlayerShoot, 0);
	}
}

void CGameInvaders::destroybullet()
{
	if (bullet.alive)
	{
		Game->Sprites->RemoveSprite(bullet.spr);
		bullet.alive = false;
	}
}

void CGameInvaders::updatebullet()
{
	if (bullet.alive)
	{
		bullet.pos.x += bullet.vel.x;
		bullet.pos.y += bullet.vel.y;
		Game->Sprites->SetSpriteLocation(bullet.spr, bullet.pos);

		for (int x = 0; x < enemycols; x++)
		{
			for(int y = 0; y < enemyrows; y++)
			{
				if (bullet.alive && enemies[y * enemycols + x].alive)
				{
					if (Game->Sprites->DetectSpriteCollision(bullet.spr, enemies[y * enemycols + x].spr))
					{
						createexplosion(enemies[y * enemycols + x].pos);
						destroybullet();
						destroyinvader(y * enemycols + x);
						if (y < 1)
							Game->AddToScore(25);
						else
						{
							if (y < 3)
								Game->AddToScore(20);
							else
								Game->AddToScore(10);
						}
						enemyvel.x += enemyvel.x / abs(enemyvel.x) * enemyspeedinc;
					}
				}
			}
		}

		for(int i = 0; i < maxasteroids; i++)
		{
			if (bullet.alive && asteroids[i].alive)
			{
				if (Game->Sprites->DetectSpriteCollision(bullet.spr, asteroids[i].spr))
				{
					createexplosion(bullet.pos);
					destroybullet();
					asteroids[i].healthpoints -= 1;
					if (asteroids[i].healthpoints == 0)
						destroyasteroid(i);
				}
			}
		}

		if (bullet.pos.y < screentop)
			destroybullet();
	}
}

//enemies ----------------------------------------------------------------------------------------------------------------

void CGameInvaders::updateenemyinfo()
{
	enemyinfo.mostleft = -1;
	enemyinfo.mostright = -1;
	enemyinfo.mostbottom = -1;

	float x1 = screenright + 1;
	float x2 = screenleft - 1;
	float y = screentop - 1;
	for(int i = 0; i < enemycols * enemyrows; i++)
	{
		if (enemies[i].alive)
		{
			if (enemies[i].pos.x < x1)
			{
				x1 = enemies[i].pos.x;
				enemyinfo.mostleft = i;
			}
			if (enemies[i].pos.x > x2)
			{
				x2 = enemies[i].pos.x;
				enemyinfo.mostright = i;
			}
			if (enemies[i].pos.y > y)
			{
				y = enemies[i].pos.y;
				enemyinfo.mostbottom = i;
			}
		}
	}
}

void CGameInvaders::destroyallinvaders()
{
	for(int i = 0; i < enemycols * enemyrows; i++)
	{
		if (enemies[i].alive)
		{
			tweens[i][tweenenemypositions].active = false;
			Game->Sprites->RemoveSprite(enemies[i].spr);
		}
	}
}

void CGameInvaders::destroyinvader(int index)
{
	enemies[index].alive = false;
	Game->Sprites->RemoveSprite(enemies[index].spr);
}

bool CGameInvaders::tweenactive(int id)
{
	bool result = false;
	for(int i = 0; i < enemyrows * enemycols; i++)
	{
		if (enemies[i].alive && tweens[i][id].active)
		{
			result = true;
			break;
		}
	}
	return result;
}

void CGameInvaders::createinvaders(bool setposition)
{
	pattern = rand() % (5);
	for (int x = 0; x < enemycols; x++)
	{
		for (int y = 0; y < enemyrows; y++)
		{
			tweens[x + y * enemycols][tweenenemypositions] = createtween(tweenenemypositions, 1+ (rand() % ((int)DesiredFps))/100, funcsmoothstop, 1, true, DesiredFps);
			enemies[x + y * enemycols].alive = true;
			enemies[x + y * enemycols].spr = Game->Sprites->CreateSprite();
			enemies[x + y * enemycols].pos = {(float) x * (enemyspacing + enemyhorzspacing) + enemystartxoffset,(float)y * enemyspacing + enemystartyoffset};
			if (setposition)
				Game->Sprites->SetSpriteLocation(enemies[x + y * enemycols].spr, enemies[x + y * enemycols].pos);
			SDL_Point tz = {1,1};
			if (y < 1)
			{
				Game->Sprites->SetSpriteImage(Game->Renderer,enemies[x + y * enemycols].spr, &spritesheetEnemy3, 4, 1);
				Game->Sprites->SetSpriteScale(Game->Renderer,enemies[x + y * enemycols].spr, enemyscale);
				Game->Sprites->SetSpriteAnimation(enemies[x + y * enemycols].spr, 0, 3, 5);
				tz = Game->Sprites->TileSize(enemies[x + y * enemycols].spr);
				Game->Sprites->SetSpriteCollisionShape(enemies[x + y * enemycols].spr, SHAPE_BOX, tz.x-8, tz.y - 26,0, 0 , 6);
			}
			else
			{
				if (y < 3 )
				{
					Game->Sprites->SetSpriteImage(Game->Renderer,enemies[x + y * enemycols].spr, &spritesheetEnemy2, 4, 1);
					Game->Sprites->SetSpriteScale(Game->Renderer,enemies[x + y * enemycols].spr, enemyscale);

					Game->Sprites->SetSpriteAnimation(enemies[x + y * enemycols].spr, 0, 3, 5);
					tz = Game->Sprites->TileSize(enemies[x + y * enemycols].spr);
					Game->Sprites->SetSpriteCollisionShape(enemies[x + y * enemycols].spr, SHAPE_BOX, tz.x-15, tz.y - 18,0, 0 , -2);
				}
				else
				{
					Game->Sprites->SetSpriteImage(Game->Renderer,enemies[x + y * enemycols].spr, &spritesheetEnemy1, 5, 1);
					Game->Sprites->SetSpriteScale(Game->Renderer,enemies[x + y * enemycols].spr, enemyscale);

					Game->Sprites->SetSpriteAnimation(enemies[x + y * enemycols].spr,0, 4, 5);
					tz = Game->Sprites->TileSize(enemies[x + y * enemycols].spr);
					Game->Sprites->SetSpriteCollisionShape(enemies[x + y * enemycols].spr, SHAPE_BOX, tz.x-17, tz.y - 12,0, 0 , -2);
				}
			}
			enemies[x + y * enemycols].tz.x = tz.x * enemyscale.x;
			enemies[x + y * enemycols].tz.y = tz.y * enemyscale.y;
		}
	}
}

void CGameInvaders::updateinvaders()
{
	updateenemyinfo();

	//all enemies cleared
	if (enemyinfo.mostleft == -1)
	{
		Game->AddToScore(250);
		enemyvel = {enemyspeed, 0};
		createinvaders(false);
		destroybullet();
		destroyallenemybullet();
		Game->Audio->PlaySound(SfxSucces, 0);
	}

	bool btweenactive = tweenactive(tweenenemypositions);

	//enemies reached horizontal screen boundaries
	if (((enemies[enemyinfo.mostleft].pos.x - enemies[enemyinfo.mostleft].tz.x / 2 + enemyvel.x < screenleft) ||
		(enemies[enemyinfo.mostright].pos.x + enemies[enemyinfo.mostright].tz.x / 2 + enemyvel.x > screenright)) && !btweenactive)
	{
		for(int i = 0; i < enemycols * enemyrows; i++)
		{
			if (enemies[i].alive)
				enemies[i].pos.y += enemyspacing / 4;
		}
		enemyvel.x *= -1;
		enemyvel.y *= -1;
	}

	//update enemy positions
	for(int i = 0; i < enemycols * enemyrows; i++)
	{
		if (enemies[i].alive)
		{
			if (btweenactive)
			{
				tweens[i][tweenenemypositions] = updatetween( tweens[i][tweenenemypositions]);
				Vec2F pos = enemies[i].pos;

				if (pattern == 0)
				{
					if ((i % enemycols) < enemycols / 3)
						pos.x = pos.x * tweens[i][tweenenemypositions].funcval;
					else
					{
						if ((i % enemycols) > enemycols * 2 / 3)
							pos.x = screenright - (screenright - pos.x) * tweens[i][tweenenemypositions].funcval;
					}
					pos.y = pos.y * tweens[i][tweenenemypositions].funcval;
				}
				else
				{
					if(pattern == 1)
						pos.y = pos.y * tweens[i][tweenenemypositions].funcval;
					else
					{
						if (pattern == 2)
						{
							if (i % enemycols < enemycols / 2)
								pos.x = pos.x * tweens[i][tweenenemypositions].funcval;
							else
								pos.x = screenright - (screenright - pos.x) * tweens[i][tweenenemypositions].funcval;
						}
						else
						{
							if( pattern == 3)
								pos.x = pos.x * tweens[i][tweenenemypositions].funcval;
							else
							{
								if (pattern == 4)
									pos.x = screenright - (screenright - pos.x) * tweens[i][tweenenemypositions].funcval;
							}
						}
					}
				}
				Game->Sprites->SetSpriteLocation(enemies[i].spr, pos);
			}
			else
			{
				if (Game->SubGameState == SGGame)
				{
					enemies[i].pos.x += enemyvel.x;
					enemies[i].pos.y += enemyvel.y;
					Game->Sprites->SetSpriteLocation(enemies[i].spr, enemies[i].pos);
				}
			}
		}
	}

	//enemies reached bottom
	if ((player.pos.y - enemies[enemyinfo.mostbottom].pos.y) < endscreenconstant)
	{
		Game->AddToScore(-250);
		enemyvel = {enemyspeed, 0};
		destroyallinvaders();
		createinvaders(false);
		destroybullet();
		destroyallenemybullet();
		HealthPoints -= 1;
		Game->Audio->PlaySound(SfxDie, 0);
	}
}

//player ----------------------------------------------------------------------------------------------------------------

void CGameInvaders::destroyplayer()
{
	Game->Sprites->RemoveSprite(player.spr);
	player.alive = false;
}

void CGameInvaders::createplayer()
{
	player.spr = Game->Sprites->CreateSprite();
	Game->Sprites->SetSpriteImage(Game->Renderer,player.spr, &spritesheetPlayer, 5,1);
	Game->Sprites->SetSpriteAnimation(player.spr, 0, 0, 0);
	Game->Sprites->SetSpriteScale(Game->Renderer,player.spr, enemyscale);
	player.tz = Game->Sprites->TileSize(player.spr);
	Game->Sprites->SetSpriteCollisionShape(player.spr, SHAPE_BOX, player.tz.x-10, player.tz.y - 6,0, 0, 12);
	player.tz.x = player.tz.x * enemyscale.x;
	player.tz.y = player.tz.y * enemyscale.y;
	player.pos = { (float)(screenright - screenleft) / 2, (float)screenbottom - player.tz.y / 2};
	HealthPoints = 3;
	player.freeze = 0;
	Game->Sprites->SetSpriteLocation(player.spr, player.pos);
}

void CGameInvaders::updateplayer()
{
	Game->Sprites->SetSpriteVisibility(player.spr, player.alive);
	if (player.alive)
	{
		Game->Sprites->SetSpriteAnimation(player.spr, 0, 0, 0);

		if ((Game->Input->Buttons.ButLeft) ||
			(Game->Input->Buttons.ButLeft2) ||
			(Game->Input->Buttons.ButDpadLeft))
		{
			if (player.pos.x - player.tz.x / 2 - playerspeed > screenleft)
				player.pos.x -= playerspeed;
			else
				player.pos.x = screenleft + player.tz.x / 2;
			Game->Sprites->SetSpriteAnimation(player.spr, 1, 1, 0);
		}

		if ((Game->Input->Buttons.ButRight) ||
			(Game->Input->Buttons.ButRight2) ||
			(Game->Input->Buttons.ButDpadRight))
		{
			if ( player.pos.x + player.tz.x / 2 + playerspeed < screenright)
				player.pos.x += playerspeed;
			else
				player.pos.x = screenright - player.tz.x / 2;
			Game->Sprites->SetSpriteAnimation(player.spr, 4, 4, 0);
		}
		Game->Sprites->SetSpriteLocation(player.spr, player.pos);

		if (!Game->Input->PrevButtons.ButA && Game->Input->Buttons.ButA)
			createbullet();
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

void CGameInvaders::DrawBackground(bool motionblur)
{
	Game->Image->DrawImage(Game->Renderer, background, NULL, NULL);
}

//init - deinit ----------------------------------------------------------------------------------------------------------------

void CGameInvaders::init()
{
	LoadGraphics();

	deaths = 0;
	backgroundfade = 0;
	backgroundfadeinc = 1;
	enemyvel = {enemyspeed, 0};

	createinvaders(ScreenshotMode);
	createplayer();
	createasteroids();

	if(!ScreenshotMode)
	{
		LoadSound();
		Game->CurrentGameMusicID = MusMusic;
		Game->Audio->PlayMusic(MusMusic, -1);
	}
}

void CGameInvaders::deinit()
{
	destroyplayer();
	destroyallinvaders();
	destroybullet();
	destroyallasteroids();
	destroyallenemybullet();
	destroyallexplosion();
	if (!ScreenshotMode)
	{
		UnLoadSound();
		Game->SubStateCounter = 0;
		Game->SubGameState = SGNone;
		Game->CurrentGameMusicID = -1;
	}
	UnloadGraphics();
}

void CGameInvaders::LoadSound()
{
	SfxDie = Game->Audio->LoadSound("common/die.wav");
	SfxPlayerShoot = Game->Audio->LoadSound("invaders/playershoot.wav");
	SfxPlayerDeath = Game->Audio->LoadSound("invaders/playerdeath.wav");
	SfxEnemyShoot = Game->Audio->LoadSound("invaders/enemyshoot.wav");
	SfxEnemyDeath = Game->Audio->LoadSound("invaders/enemydeath.wav");
	SfxSucces = Game->Audio->LoadSound("common/succes.wav");
	MusMusic = Game->Audio->LoadMusic("invaders/music.ogg");
}

void CGameInvaders::UnLoadSound()
{
	Game->Audio->StopMusic();
	Game->Audio->StopSound();
	Game->Audio->UnLoadMusic(MusMusic);
	Game->Audio->UnLoadSound(SfxDie);
	Game->Audio->UnLoadSound(SfxPlayerShoot);
	Game->Audio->UnLoadSound(SfxPlayerDeath);
	Game->Audio->UnLoadSound(SfxEnemyShoot);
	Game->Audio->UnLoadSound(SfxEnemyDeath);
	Game->Audio->UnLoadSound(SfxSucces);
}

void CGameInvaders::LoadGraphics()
{
	background = Game->Image->LoadImage(Game->Renderer, "invaders/background.png");

	spritesheetBullet = Game->Image->LoadImage(Game->Renderer, "invaders/bullet.png", 0, 100, dumpScaledBitmaps);
	spritesheetExplosion = Game->Image->LoadImage(Game->Renderer, "invaders/explosion.png", 0, 100, dumpScaledBitmaps);
	spritesheetAsteroid = Game->Image->LoadImage(Game->Renderer, "invaders/asteroid-01.png", 0, 75, dumpScaledBitmaps);
	spritesheetEnemy1 = Game->Image->LoadImage(Game->Renderer, "invaders/enemy1.png",0, 80, dumpScaledBitmaps); //bottom
	spritesheetEnemy2 = Game->Image->LoadImage(Game->Renderer, "invaders/enemy2.png",0, 80, dumpScaledBitmaps); //middle
	spritesheetEnemy3 = Game->Image->LoadImage(Game->Renderer, "invaders/enemy3.png",0, 80, dumpScaledBitmaps); //top
	spritesheetPlayer = Game->Image->LoadImage(Game->Renderer, "invaders/player.png",0, 80, dumpScaledBitmaps); //top
	
	// SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/invaders/bullet.bmp", Game->Image->GetImage(spritesheetBullet), 1,1, true, 0, 100);
	// SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/invaders/explosion.bmp", Game->Image->GetImage(spritesheetExplosion), 1,1,true, 0, 100);
	// SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/invaders/asteroid-01.bmp", Game->Image->GetImage(spritesheetAsteroid), 1,1,true, 0, 75);
	// SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/invaders/enemy1.bmp", Game->Image->GetImage(spritesheetEnemy1), 1,1,true, 0, 80);
	// SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/invaders/enemy2.bmp", Game->Image->GetImage(spritesheetEnemy2), 1,1,true, 0, 80);
	// SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/invaders/enemy3.bmp", Game->Image->GetImage(spritesheetEnemy3), 1,1,true, 0, 80);
	// SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/invaders/player.bmp", Game->Image->GetImage(spritesheetPlayer), 1,1,true, 0, 80);
	
	if(!useDefaultColorAssets)
	{
    	UnloadGraphics();
		background = Game->Image->LoadImage(Game->Renderer, "invaders/background.png");
		spritesheetBullet = Game->Image->LoadImage(Game->Renderer, "invaders/bullet.bmp");
		spritesheetExplosion = Game->Image->LoadImage(Game->Renderer, "invaders/explosion.bmp");
		spritesheetAsteroid = Game->Image->LoadImage(Game->Renderer, "invaders/asteroid-01.bmp");
		spritesheetEnemy1 = Game->Image->LoadImage(Game->Renderer, "invaders/enemy1.bmp"); //bottom
		spritesheetEnemy2 = Game->Image->LoadImage(Game->Renderer, "invaders/enemy2.bmp"); //middle
		spritesheetEnemy3 = Game->Image->LoadImage(Game->Renderer, "invaders/enemy3.bmp"); //top
		spritesheetPlayer = Game->Image->LoadImage(Game->Renderer, "invaders/player.bmp"); //top
	}
	backgroundtz = Game->Image->ImageSize(background);
}

void CGameInvaders::UnloadGraphics()
{
	Game->Image->UnLoadImage(spritesheetBullet);
	Game->Image->UnLoadImage(spritesheetExplosion);
	Game->Image->UnLoadImage(spritesheetAsteroid);
	Game->Image->UnLoadImage(spritesheetEnemy1);
	Game->Image->UnLoadImage(spritesheetEnemy2);
	Game->Image->UnLoadImage(spritesheetEnemy3);
	Game->Image->UnLoadImage(spritesheetPlayer);
	Game->Image->UnLoadImage(background);
}

SDL_Texture* CGameInvaders::screenshot()
{
	SDL_Texture* prev = SDL_GetRenderTarget(Game->Renderer);
	SDL_Texture* image = SDL_CreateTexture(Game->Renderer, PixelFormat, SDL_TEXTUREACCESS_TARGET, ScreenWidth, ScreenHeight);
	SDL_SetRenderTarget(Game->Renderer, image);
	SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 255);
	SDL_RenderClear(Game->Renderer);
	init();

	Draw();

	SDL_RenderPresent(Game->Renderer);
	SDL_SetRenderTarget(Game->Renderer, prev);
	deinit();
	return image;
}

//Update ----------------------------------------------------------------------------------------------------------------

void CGameInvaders::UpdateObjects(bool IsGameState)
{
	updateinvaders();
	if (IsGameState)
	{
		updateenemybullet();
		updateplayer();
		updatebullet();
		updateasteroids();
	}
}

bool CGameInvaders::DrawObjects()
{
	//need todo this here so last frame is also drawn
	checkexplosions();
	//call drawsprites in base class
	return true;
}
