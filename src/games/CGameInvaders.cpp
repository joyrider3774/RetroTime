#include <SDL.h>
#include <limits.h>
#include <string>
#include <iostream>
#include "CGameInvaders.h"
#include "..\CGame.h"
#include "..\Common.h"

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
    screenleft = 0;
    screenright = ScreenWidth;
    screentop = 0;
    screenbottom = ScreenHeight;

}

CGameInvaders::~CGameInvaders(){};

//explosions ----------------------------------------------------------------------------------------------------------------

void CGameInvaders::sicheckexplosions()
{
	for(int i = 0; i < simaxexplosion; i++)
    {
		if (explosions[i].alive)
        {
			if (Game->Sprites->GetSpriteAnimFrame(explosions[i].spr) == Game->Sprites->GetSpriteAnimFrameCount(explosions[i].spr) -1)
            {
				sidestroyexploison(i);
            }
        }
    }
}

void CGameInvaders::sidestroyallexplosion()
{
	for (int i = 0; i < simaxexplosion; i++)
		sidestroyexploison(i);
}

void CGameInvaders::sidestroyexploison(int index)
{
	if (explosions[index].alive)
    { 
		Game->Sprites->RemoveSprite(explosions[index].spr);
		explosions[index].alive = false;
	}
}

void CGameInvaders::sicreateexplosion(SDL_FPoint pos)
{
	for(int i = 0; i < simaxexplosion;i++)
    {
		if (!explosions[i].alive)
        {
			explosions[i].spr = Game->Sprites->CreateSprite();
			Game->Sprites->SetSpriteImage(explosions[i].spr, &spritesheetExplosion, 7, 1);
			Game->Sprites->SetSpriteScale(explosions[i].spr, {1.5,1.5});
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

void CGameInvaders::sidestroyallenemybullet()
{
	for(int i = 0; i <  simaxenemybullets; i++)
		sidestroyenemybullet(i);
}

void CGameInvaders::sidestroyenemybullet(int index)
{
    if (enemybullets[index].alive)
    {
		Game->Sprites->RemoveSprite(enemybullets[index].spr);
		enemybullets[index].alive = false;
	}
}
	
void CGameInvaders::sicreatenemybullet(SDL_FPoint pos)
{
	for(int i = 0; i <  simaxenemybullets; i++)
    {
		if (!enemybullets[i].alive)
        {
			enemybullets[i].spr = Game->Sprites->CreateSprite();
			Game->Sprites->SetSpriteImage(enemybullets[i].spr, &spritesheetBullet, 2, 1);
			Game->Sprites->SetSpriteColour(enemybullets[i].spr, 1,0,0,1);
			Game->Sprites->SetSpriteAnimation(enemybullets[i].spr, 1, 1, 0);
			Game->Sprites->SetSpriteScale(enemybullets[i].spr, sienemyscale);
			Game->Sprites->SetSpriteDepth(enemybullets[i].spr, -1);
			enemybullets[i].tz = Game->Sprites->TileSize(enemybullets[i].spr);
            enemybullets[i].tz.x = enemybullets[i].tz.x * sienemyscale.x;
            enemybullets[i].tz.y = enemybullets[i].tz.y * sienemyscale.y;
			enemybullets[i].pos = pos;
			enemybullets[i].vel = {0, sibulletspeed};
			enemybullets[i].alive = true;
			Game->Audio->PlaySound(SfxEnemyShoot, 0);
			break;
		}
	}
}

void CGameInvaders::siupdateenemybullet()
{
	if (!sitweenactive(sitweenenemypositions))
    {
		if (rand() % (sienemybulletrandomizer) == 0)
        {
			int col = rand() % (sienemycols);
			int y = 0;
			bool fired = false;
			while (!fired)
            {
				y = sienemyrows -1;
				while (y >= 0)
                {
					if (enemies[col + y * sienemycols].alive)
                    {
						sicreatenemybullet(enemies[col + y * sienemycols].pos);
						fired = true;
						break;
					}
					y -= 1;
				}
				col += 1;
				if (col >= sienemycols)
					col = 0;
			}
		}
	}
	
	for(int i = 0; i <  simaxenemybullets; i++)
    {
		if (enemybullets[i].alive)
        {
			enemybullets[i].pos.x += enemybullets[i].vel.x;
            enemybullets[i].pos.y += enemybullets[i].vel.y;
			Game->Sprites->SetSpriteLocation(enemybullets[i].spr, enemybullets[i].pos);
			if (enemybullets[i].pos.y >= screenbottom)
				sidestroyenemybullet(i);
		
			if (enemybullets[i].alive && player.alive)
            {
				if (Game->Sprites->DetectSpriteCollision(enemybullets[i].spr, player.spr))
                {
					player.alive = false;
					player.freeze = 30;
					sicreateexplosion(player.pos);
					sidestroyenemybullet(i);
					Game->AddToScore(-150);
					deaths += 1;
					HealthPoints -= 1;
					Game->Audio->PlaySound(SfxDie, 0);
				}
			}
		
			
			for (int j = 0; j < simaxasteroids; j++)
            {			
				if (enemybullets[i].alive && asteroids[j].alive)
                {
					if (Game->Sprites->DetectSpriteCollision(enemybullets[i].spr, asteroids[j].spr))
                    {
						sicreateexplosion(enemybullets[i].pos);
						sidestroyenemybullet(i);
						asteroids[j].healthpoints -= 1;
						if (asteroids[j].healthpoints == 0)
							sidestroyasteroid(j);
					}
				} 
			}

		}
	}
}

//asteroids ----------------------------------------------------------------------------------------------------------------

void CGameInvaders::sidestroyallasteroids()
{
	for(int i = 0; i <  simaxasteroids; i++)
		sidestroyasteroid(i);
}

void CGameInvaders::sidestroyasteroid(int index)
{
	if (asteroids[index].alive)
    {
		Game->Sprites->RemoveSprite(asteroids[index].spr);
		asteroids[index].alive = false;
	}
}

void CGameInvaders::sicreateasteroids()
{
	for(int i = 0; i < simaxasteroids; i++)
    {
		asteroids[i].spr = Game->Sprites->CreateSprite();
		asteroids[i].healthpoints = siasteroidmaxhealthpoints;
		asteroids[i].alive = true;
		Game->Sprites->SetSpriteImage(asteroids[i].spr, &spritesheetAsteroid);
		Game->Sprites->SetSpriteAnimation(asteroids[i].spr, 0, 0, 0);
		Game->Sprites->SetSpriteLocation(asteroids[i].spr, {(float)i * (screenright - screenleft) / (simaxasteroids-1), (float)screenbottom - siasteroidsoffset});
		Game->Sprites->SetSpriteScale(asteroids[i].spr, {siasteroidscale, siasteroidscale});
		Game->Sprites->SetSpriteCollisionShape(asteroids[i].spr, SHAPE_BOX, 45,45,0, 0 , 2);
		Game->Sprites->SetSpriteRotation(asteroids[i].spr, rand() % (360));
	}
}

void CGameInvaders::siupdateasteroids()
{
	for(int i = 0; i < simaxasteroids;i++)
    { 
		if (asteroids[i].alive)
        {
			Game->Sprites->SetSpriteScale(asteroids[i].spr, 
				{siasteroidscale - ((siasteroidscale / siasteroidmaxhealthpoints) * (siasteroidmaxhealthpoints - asteroids[i].healthpoints)) ,
				siasteroidscale - ((siasteroidscale / siasteroidmaxhealthpoints) * (siasteroidmaxhealthpoints - asteroids[i].healthpoints))});
			Game->Sprites->SetSpriteRotation(asteroids[i].spr, asteroids[i].spr->rotation + 0.5);
		}
	}
}

//player bullet ----------------------------------------------------------------------------------------------------------------

void CGameInvaders::sicreatebullet()
{
	if (!bullet.alive)
    {
		bullet.spr = Game->Sprites->CreateSprite();
		Game->Sprites->SetSpriteImage(bullet.spr, &spritesheetBullet, 2, 1);
		Game->Sprites->SetSpriteAnimation(bullet.spr, 1, 1, 0);
		Game->Sprites->SetSpriteScale(bullet.spr, sienemyscale);
		Game->Sprites->SetSpriteDepth(bullet.spr, -1);
		bullet.tz = Game->Sprites->TileSize(bullet.spr);
        bullet.tz.x = bullet.tz.x * sienemyscale.x;
        bullet.tz.y = bullet.tz.y * sienemyscale.y;
		bullet.pos = player.pos ;
		bullet.vel = {0, -sibulletspeed};
		bullet.alive = true;
		Game->Audio->PlaySound(SfxPlayerShoot, 0);
	}
}

void CGameInvaders::sidestroybullet()
{
	if (bullet.alive)
    {
		Game->Sprites->RemoveSprite(bullet.spr);
		bullet.alive = false;
	}
}

void CGameInvaders::siupdatebullet()
{
	if (bullet.alive)
    {
		bullet.pos.x += bullet.vel.x;
        bullet.pos.y += bullet.vel.y;
		Game->Sprites->SetSpriteLocation(bullet.spr, bullet.pos);
	
		for (int x = 0; x < sienemycols; x++)
        {
			for(int y = 0; y < sienemyrows; y++)
            {
				if (bullet.alive && enemies[y * sienemycols + x].alive)
                {
					if (Game->Sprites->DetectSpriteCollision(bullet.spr, enemies[y * sienemycols + x].spr))
                    {
						sicreateexplosion(enemies[y * sienemycols + x].pos);
						sidestroybullet();
						sidestroyinvader(y * sienemycols + x);
						if (y < 1)
							Game->AddToScore(25);
						else
                        {
							if (y < 3)
								Game->AddToScore(20);
							else
								Game->AddToScore(10);
						}
						enemyvel.x += enemyvel.x / abs(enemyvel.x) * sienemyspeedinc;
					}
				}
			}
		}
		
		for(int i = 0; i < simaxasteroids; i++)
        {
			if (bullet.alive && asteroids[i].alive)
            {
				if (Game->Sprites->DetectSpriteCollision(bullet.spr, asteroids[i].spr))
                {
					sicreateexplosion(bullet.pos);
					sidestroybullet();
					asteroids[i].healthpoints -= 1;
					if (asteroids[i].healthpoints == 0)
						sidestroyasteroid(i);
				}
			} 
		}
		
		if (bullet.pos.y < screentop)
			sidestroybullet();
	}
}

//enemies ----------------------------------------------------------------------------------------------------------------

void CGameInvaders::siupdateenemyinfo()
{
	enemyinfo.mostleft = -1;
	enemyinfo.mostright = -1;
	enemyinfo.mostbottom = -1;

	float x1 = screenright + 1;
	float x2 = screenleft - 1;
    float y = screentop - 1;
	for(int i = 0; i < sienemycols * sienemyrows; i++)
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

void CGameInvaders::sidestroyallinvaders()
{
	for(int i = 0; i <  sienemycols * sienemyrows; i++)
    {
		if (enemies[i].alive)
        {
			tweens[i][sitweenenemypositions].active = false;
			Game->Sprites->RemoveSprite(enemies[i].spr);
		}
	}	
}

void CGameInvaders::sidestroyinvader(int index)
{
	enemies[index].alive = false;
	Game->Sprites->RemoveSprite(enemies[index].spr);
}

bool CGameInvaders::sitweenactive(int id)
{
	bool result = false;
	for(int i = 0; i < sienemyrows * sienemycols; i++)
    {
		if (enemies[i].alive && tweens[i][id].active)
        {
			result = true;
			break;
		}
	} 
    return result;
}

void CGameInvaders::sicreateinvaders(bool setposition)
{
	pattern = rand() % (5);
	for (int x = 0; x < sienemycols; x++)
    {
		for (int y = 0; y < sienemyrows; y++)
        {
			tweens[x + y * sienemycols][sitweenenemypositions] = createtween(sitweenenemypositions, 1+ (rand() % ((int)DesiredFps))/100, funcsmoothstop, 1, true, DesiredFps);
			enemies[x + y * sienemycols].alive = true;
			enemies[x + y * sienemycols].spr = Game->Sprites->CreateSprite();
			enemies[x + y * sienemycols].pos = {(float) x * (sienemyspacing + sienemyhorzspacing)  + sienemystartxoffset,(float)y * sienemyspacing + sienemystartyoffset};
			if (setposition)
				Game->Sprites->SetSpriteLocation(enemies[x + y * sienemycols].spr, enemies[x + y * sienemycols].pos);
			SDL_Point tz = {1,1};
			if (y < 1)
            {
       			Game->Sprites->SetSpriteImage(enemies[x + y * sienemycols].spr, &spritesheetEnemy3, 4, 1);
    			Game->Sprites->SetSpriteScale(enemies[x + y * sienemycols].spr, sienemyscale);
				Game->Sprites->SetSpriteAnimation(enemies[x + y * sienemycols].spr, 0, 3, 5);
				tz = Game->Sprites->TileSize(enemies[x + y * sienemycols].spr);
				Game->Sprites->SetSpriteCollisionShape(enemies[x + y * sienemycols].spr, SHAPE_BOX, tz.x-8, tz.y - 26,0, 0 , 6);
            }
			else
            {
				if (y < 3 )
                {
                    Game->Sprites->SetSpriteImage(enemies[x + y * sienemycols].spr, &spritesheetEnemy2, 4, 1);
    			    Game->Sprites->SetSpriteScale(enemies[x + y * sienemycols].spr, sienemyscale);
				
					Game->Sprites->SetSpriteAnimation(enemies[x + y * sienemycols].spr, 0, 3, 5);
					tz = Game->Sprites->TileSize(enemies[x + y * sienemycols].spr);
					Game->Sprites->SetSpriteCollisionShape(enemies[x + y * sienemycols].spr, SHAPE_BOX, tz.x-15, tz.y - 18,0, 0 , -2);
                }
				else
                {
                    Game->Sprites->SetSpriteImage(enemies[x + y * sienemycols].spr, &spritesheetEnemy1, 5, 1);
    			    Game->Sprites->SetSpriteScale(enemies[x + y * sienemycols].spr, sienemyscale);
				
					Game->Sprites->SetSpriteAnimation(enemies[x + y * sienemycols].spr,0, 4, 5);
					tz = Game->Sprites->TileSize(enemies[x + y * sienemycols].spr);
					Game->Sprites->SetSpriteCollisionShape(enemies[x + y * sienemycols].spr, SHAPE_BOX, tz.x-17, tz.y - 12,0, 0 , -2);
				}
			}
			enemies[x + y * sienemycols].tz.x = tz.x * sienemyscale.x;
            enemies[x + y * sienemycols].tz.y = tz.y * sienemyscale.y;
		}
	}
}

void CGameInvaders::siupdateinvaders()
{
	siupdateenemyinfo();
	
	//all enemies cleared
	if (enemyinfo.mostleft == -1)
    {
		Game->AddToScore(250);
		enemyvel = {sienemyspeed, 0};
		sicreateinvaders(false);
		sidestroybullet();
		sidestroyallenemybullet();
		Game->Audio->PlaySound(SfxSucces, 0);
	}
	
	bool tweenactive = sitweenactive(sitweenenemypositions);
	
	//enemies reached horizontal screen boundaries
	if (((enemies[enemyinfo.mostleft].pos.x - enemies[enemyinfo.mostleft].tz.x / 2 + enemyvel.x < screenleft) ||
		(enemies[enemyinfo.mostright].pos.x + enemies[enemyinfo.mostright].tz.x / 2 + enemyvel.x > screenright)) && !tweenactive)
    {
		for(int i = 0; i < sienemycols * sienemyrows; i++)
        {
			if (enemies[i].alive)
				enemies[i].pos.y += sienemyspacing / 4;
		}
		enemyvel.x *= -1;
        enemyvel.y *= -1;
	}
	
	//update enemy positions
	for(int i = 0; i < sienemycols * sienemyrows; i++)
    {
		if (enemies[i].alive)
        {
			if (tweenactive)
            {
				tweens[i][sitweenenemypositions] = updatetween( tweens[i][sitweenenemypositions]);
				SDL_FPoint pos = enemies[i].pos;
				
				if (pattern == 0)
                {
					if ((i % sienemycols) < sienemycols / 3)
						pos.x = pos.x * tweens[i][sitweenenemypositions].funcval;
					else 
                    {
						if ((i % sienemycols) > sienemycols * 2 / 3)
							pos.x = screenright - (screenright - pos.x) * tweens[i][sitweenenemypositions].funcval;
					}
					pos.y = pos.y * tweens[i][sitweenenemypositions].funcval;
                }
				else 
                {
					if(pattern == 1)
						pos.y = pos.y * tweens[i][sitweenenemypositions].funcval;
					else
                    {
						if (pattern == 2)
                        {
							if (i % sienemycols < sienemycols / 2)
								pos.x = pos.x * tweens[i][sitweenenemypositions].funcval;
							else 
								pos.x = screenright - (screenright - pos.x) * tweens[i][sitweenenemypositions].funcval;
						}
						else
                        {
							if( pattern == 3)
								pos.x = pos.x * tweens[i][sitweenenemypositions].funcval;
							else
                            {
								if (pattern == 4)
									pos.x = screenright - (screenright - pos.x) * tweens[i][sitweenenemypositions].funcval;
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
	if ((player.pos.y - enemies[enemyinfo.mostbottom].pos.y) < siendscreenconstant)
    {
		Game->AddToScore(-250);
		enemyvel = {sienemyspeed, 0};
		sidestroyallinvaders();
		sicreateinvaders(false);
		sidestroybullet();
		sidestroyallenemybullet();
		HealthPoints -= 1;
		Game->Audio->PlaySound(SfxDie, 0);
	}
}

//player ----------------------------------------------------------------------------------------------------------------

void CGameInvaders::sidestroyplayer()
{
	Game->Sprites->RemoveSprite(player.spr);
	player.alive = false;
}

void CGameInvaders::sicreateplayer()
{
	player.spr = Game->Sprites->CreateSprite();
	Game->Sprites->SetSpriteImage(player.spr, &spritesheetPlayer, 5,1);
	Game->Sprites->SetSpriteAnimation(player.spr, 0, 0, 0);
	Game->Sprites->SetSpriteScale(player.spr, sienemyscale);
	player.tz = Game->Sprites->TileSize(player.spr);
    Game->Sprites->SetSpriteCollisionShape(player.spr, SHAPE_BOX, player.tz.x-20, player.tz.y - 12,0, 0, 12);
    player.tz.x = player.tz.x * sienemyscale.x;
    player.tz.y = player.tz.y * sienemyscale.y;
	player.pos = { (float)(screenright - screenleft) / 2, (float)screenbottom - player.tz.y  / 2};
	HealthPoints = 3;
	player.freeze = 0;
	Game->Sprites->SetSpriteLocation(player.spr, player.pos);
}

void CGameInvaders::siupdateplayer()
{
	Game->Sprites->SetSpriteVisibility(player.spr, player.alive);
	if (player.alive)
    {
		Game->Sprites->SetSpriteAnimation(player.spr, 0, 0, 0);

		if (Game->Input->Buttons.ButLeft)
        {
			if (player.pos.x - player.tz.x / 2 - siplayerspeed > screenleft)
				player.pos.x -= siplayerspeed;
			else
				player.pos.x = screenleft + player.tz.x / 2;
			Game->Sprites->SetSpriteAnimation(player.spr, 1, 1, 0);
		}

		if (Game->Input->Buttons.ButRight)
        {
			if ( player.pos.x + player.tz.x / 2 + siplayerspeed < screenright)
				player.pos.x += siplayerspeed;
			else
				player.pos.x = screenright - player.tz.x / 2;
			Game->Sprites->SetSpriteAnimation(player.spr, 4, 4, 0);
		}
		Game->Sprites->SetSpriteLocation(player.spr, player.pos);
	
		if (!Game->Input->PrevButtons.ButA && Game->Input->Buttons.ButA)
			sicreatebullet();
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

void CGameInvaders::sidrawbackground(bool motionblur)
{
	float alpha = 1;
	if ((motionblur) && (Game->MotionBlur))
		alpha = 0.3;

    backgroundfade += backgroundfadeinc;
	if (backgroundfade >= 100)
    {
		backgroundfadeinc *= -1;
		backgroundfade = 100;
    }

	if (backgroundfade <= 0)
    {
		backgroundfadeinc *= -1;
		backgroundfade = 0;
    }
    SDL_Point Pos = { ScreenWidth / 2, ScreenHeight / 2};
    SDL_FPoint Scale =  {(float)ScreenWidth / backgroundtz.x, (float)ScreenHeight / backgroundtz.y};
	Game->Image->DrawImageFuzeTintFloat(Game->Renderer, background, true, &Pos, 0, &Scale, 1, 1, 1, alpha);

    Scale =  {(float)ScreenWidth / background2tz.x, (float)ScreenHeight / background2tz.y};
	Game->Image->DrawImageFuzeTintFloat(Game->Renderer, background2, true, &Pos, 0, &Scale, 1, 1, 1, 0.3 - (backgroundfade / 450));


    SDL_Rect SrcRect = {0, 0, background3tz.x, background3tz.y};
    SDL_Rect DstRect =  {0, 0, ScreenWidth, ScreenHeight};
	Game->Image->DrawImage(Game->Renderer, background3, &SrcRect , &DstRect);

    Scale =  {4,4};
    Pos = {800, 400};
    Game->Image->DrawImageFuzeTintFloat(Game->Renderer, backgrounditem, true, &Pos, 45, &Scale, 1, 1, 1,1);
}

//init - deinit ----------------------------------------------------------------------------------------------------------------

void CGameInvaders::init()
{
	SDL_Texture* prev = SDL_GetRenderTarget(Game->Renderer);
    SDL_SetRenderTarget(Game->Renderer, Game->TexTmp);
    SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Game->Renderer);
    SDL_SetRenderTarget(Game->Renderer, prev);
  
    LoadGraphics();

    deaths = 0;
	backgroundfade = 0;
	backgroundfadeinc = 1;
	enemyvel = {sienemyspeed, 0};
	
    sicreateinvaders(ScreenshotMode);
	sicreateplayer();
	sicreateasteroids();

	if(!ScreenshotMode)
	{
        SfxDie = Game->Audio->LoadSound("common/die.wav");
        SfxPlayerShoot = Game->Audio->LoadSound("invaders/playershoot.mp3");
        SfxPlayerDeath = Game->Audio->LoadSound("invaders/playerdeath.wav");
        SfxEnemyShoot = Game->Audio->LoadSound("invaders/enemyshoot.mp3");
        SfxEnemyDeath = Game->Audio->LoadSound("invaders/enemydeath.wav");
		SfxSucces = Game->Audio->LoadSound("common/succes.wav");
		MusMusic = Game->Audio->LoadMusic("invaders/music.mp3");
        //SfxExplosion = 
		Game->CurrentGameMusicID = MusMusic;
		Game->Audio->PlayMusic(MusMusic, -1);
	}
}

void CGameInvaders::deinit()
{
    sidestroyplayer();
	sidestroyallinvaders();
	sidestroybullet();
	sidestroyallasteroids();
	sidestroyallenemybullet();
	sidestroyallexplosion();
	if (!ScreenshotMode)
	{
		Game->Audio->StopMusic();
		Game->Audio->StopSound();
		Game->Audio->UnLoadMusic(MusMusic);
		Game->SubStateCounter = 0;
		Game->SubGameState = SGNone;
		Game->CurrentGameMusicID = -1;
        UnloadGraphics();
	}
}

void CGameInvaders::LoadGraphics()
{
    spritesheetBullet = Game->Image->LoadImage(Game->Renderer, "invaders/bullet.png");
    spritesheetExplosion = Game->Image->LoadImage(Game->Renderer, "invaders/explosion.png");
    spritesheetAsteroid = Game->Image->LoadImage(Game->Renderer, "invaders/asteroid-01.png");
    spritesheetEnemy1 = Game->Image->LoadImage(Game->Renderer, "invaders/enemy1.png"); //bottom
    spritesheetEnemy2 = Game->Image->LoadImage(Game->Renderer, "invaders/enemy2.png"); //middle
    spritesheetEnemy3 = Game->Image->LoadImage(Game->Renderer, "invaders/enemy3.png"); //top
    spritesheetPlayer = Game->Image->LoadImage(Game->Renderer, "invaders/player.png"); //top
    background = Game->Image->LoadImage(Game->Renderer, "invaders/background.png");
    background2 = Game->Image->LoadImage(Game->Renderer, "invaders/parallax-space-stars.png");
    background3 = Game->Image->LoadImage(Game->Renderer, "invaders/parallax-space-far-planets.png");
    backgrounditem = Game->Image->LoadImage(Game->Renderer, "invaders/parallax-space-ring-planet.png"),

    backgroundtz = Game->Image->ImageSize(background);
    background2tz = Game->Image->ImageSize(background2);
    background3tz = Game->Image->ImageSize(background3);
    backgrounditemtz = Game->Image->ImageSize(backgrounditem);


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
    Game->Image->UnLoadImage(background2);
    Game->Image->UnLoadImage(background3);
    Game->Image->UnLoadImage(backgrounditem);
}

SDL_Texture* CGameInvaders::screenshot()
{
	SDL_Texture* prev = SDL_GetRenderTarget(Game->Renderer);
	SDL_Texture* image = SDL_CreateTexture(Game->Renderer, PixelFormat, SDL_TEXTUREACCESS_TARGET, ScreenWidth, ScreenHeight);
    SDL_SetRenderTarget(Game->Renderer, image);
	SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 255);
	SDL_RenderClear(Game->Renderer);
	init();
    
	sidrawbackground(false);
	Game->Sprites->DrawSprites(Game->Renderer);
	
	SDL_RenderPresent(Game->Renderer);
	SDL_SetRenderTarget(Game->Renderer, prev);
	deinit();	
	return image;
}

//Update ----------------------------------------------------------------------------------------------------------------

void CGameInvaders::UpdateLogic()
{
    CGameBase::UpdateLogic();
	siupdateinvaders();
    if (Game->SubGameState == SGGame)
	{
        siupdateenemybullet();
		siupdateplayer();
		siupdatebullet();
		siupdateasteroids();
		Game->Sprites->UpdateSprites();
	}
}

void CGameInvaders::Draw()
{
    sidrawbackground(Game->SubGameState == SGGame);
    Game->Sprites->DrawSprites(Game->Renderer);
    //need todo this here so last frame is also drawn
    sicheckexplosions();
	DrawScoreBar();
	DrawSubStateText();
}
