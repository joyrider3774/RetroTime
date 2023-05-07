#include <SDL.h>
#include <string>
#include <iostream>
#include <cmath>
#include "CGamePang.h"
#include "../CGame.h"
#include "../Common.h"
#include "../Vec2F.h"

using namespace std;

CGamePang::CGamePang(CGame* aGame, bool aScreenshotMode): CGameBase(aGame, GSPang, true, aScreenshotMode)
{
    Game = aGame;
	MusMusic = -1;
    SfxSucces = -1;
    SfxDie = -1;
    SfxShoot = -1;
    SfxPop = -1;
    screenleft = 0;
    screenright = ScreenWidth;
    screentop = 0;
    screenbottom = ScreenHeight;
}

CGamePang::~CGamePang() {};



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
			createball(balls[index].id - 1, balls[index].pos.x - 25, balls[index].pos.y, -10);
			createball(balls[index].id - 1, balls[index].pos.x + 25, balls[index].pos.y, 10);
        }
    }
	if (balls[index].alive)
    { 	
		Game->Sprites->RemoveSprite(balls[index].spr);
		balls[index].alive = false;
    }
}

void CGamePang::createball(int size, float x, float y, float speed)
{
	for (int i = 0; i < maxballs; i++)
    {
		if (!balls[i].alive)
        { 
			balls[i].spr = Game->Sprites->CreateSprite();
			Game->Sprites->SetSpriteImage(balls[i].spr, &spritesheetball);
			Game->Sprites->SetSpriteCollisionShape(balls[i].spr, SHAPE_CIRCLE, 35,35,0,0,0); 
			//Game->Sprites->SetSpriteCollisionShape(balls[i].spr, SHAPE_BOX, 27,27,0,0,0);
            Game->Sprites->SetSpriteColour(balls[i].spr, 1, 1, 1, 0.7f);
			Vec2F Scale = ballscale;
            Scale.x = Scale.x * size;
            Scale.y = Scale.y * size;
            Game->Sprites->SetSpriteScale(balls[i].spr, Scale);
			Game->Sprites->SetSpriteDepth(balls[i].spr, 6);
			balls[i].tz = Game->Image->ImageSize(spritesheetball);
            balls[i].tz.x = balls[i].tz.x * Scale.x;
            balls[i].tz.x = balls[i].tz.y * Scale.y;
			balls[i].pos = { x, y};
			Game->Sprites->SetSpriteLocation(balls[i].spr, balls[i].pos);
			balls[i].alive = true;
			balls[i].speed = speed*0.1;
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
			if (balls[i].pos.x  + balls[i].speed > screenright)
            {
				if (balls[i].speed > 0)
					balls[i].speed = -abs(balls[i].speed);

            }
			
			if (balls[i].pos.x + balls[i].speed < screenleft)
            {
				if (balls[i].speed < 0)
					balls[i].speed = abs(balls[i].speed);
            }
		
						
			balls[i].pos.x += balls[i].speed*2;

			if (balls[i].id == ballbig)
				balls[i].curforce += 0.1;
			else		
            {
				if(balls[i].id == ballmedium)
					balls[i].curforce += 0.15;
				else
                {
					if (balls[i].id == ballsmall)
						balls[i].curforce += 0.25;
                }
            }
	
			balls[i].pos.y += balls[i].curforce;

			if (balls[i].id == ballbig)
				if (balls[i].pos.y >= screenbottom - 135)
					balls[i].curforce = balls[i].force;

			if (balls[i].id == ballmedium)
				if (balls[i].pos.y >= screenbottom - 100)
					balls[i].curforce = balls[i].force;
			
			if (balls[i].id == ballsmall)
				if (balls[i].pos.y >= screenbottom - 75)
					balls[i].curforce = balls[i].force;

			Game->Sprites->SetSpriteLocation(balls[i].spr, balls[i].pos);
			
			if (!playerstate(playerstatereviving) && Game->Sprites->DetectSpriteCollision(balls[i].spr, player.spr))
            {
				Game->AddToScore(-25);
				HealthPoints -= 1;
				addplayerstate(playerstatereviving);
				remplayerstate(playerstateshoot);
				player.stateticks = 90;
				Game->Audio->PlaySound(SfxDie, 0);
            }
				
			if (bullet.alive && (bullet.freeze == 0))
            { 
				if (Game->Sprites->DetectSpriteCollision(balls[i].spr, bullet.spr))
                {
					Game->AddToScore(balls[i].id * 7);
					destroyball(i, false);
					bullet.freeze = 4;
					Game->Sprites->SetSpriteAnimation(bullet.spr, 0, 1, 10);
					Game->Audio->PlaySound(SfxPop, 0);
                }
            }
        }
    }
    if (levelcleared)
    {
		if (level < maxbigballs)
			level += 1;
		Game->Audio->PlaySound(SfxSucces, 0);
		Game->AddToScore(100);
		createballs();
    }
}

void CGamePang::drawballs()
{
	for (int i = 0; i < maxballs; i++)
    {
		if (balls[i].alive)
			Game->Sprites->DrawSprite(Game->Renderer, balls[i].spr);
		
    }
}

void CGamePang::createballs()
{
	int added = 0;
	int speed = 10;
	for (int i = screenleft; i < screenright; i += int((screenright - screenleft) / (level + 1)) + 1)
    {
		if ((i > screenleft) && (i < screenright))
        {
			createball(ballbig, i, 160, speed);
			speed *= -1;
			added += 1;
			if (added >= level)
				break;
		}
    }
}

//bullet ----------------------------------------------------------------------------------------------------------------

void CGamePang::destroybullet()
{
	if (bullet.alive)
    {
		Game->Sprites->RemoveSprite(bullet.spr);
		bullet.alive = false;
    }
}

void CGamePang::createbullet()
{
	bullet.spr = Game->Sprites->CreateSprite();
	Game->Sprites->SetSpriteImage(bullet.spr, &spritesheetbullet, 1, 2);
	Game->Sprites->SetSpriteAnimation(bullet.spr, 0, 0, 0);
	Game->Sprites->SetSpriteCollisionShape(bullet.spr, SHAPE_BOX, Game->Sprites->TileSize(bullet.spr).y-18, Game->Sprites->TileSize(bullet.spr).x+160,0,0,0);

	Game->Sprites->SetSpriteRotation(bullet.spr, 90);
	Game->Sprites->SetSpriteScale(bullet.spr, bulletscale);
	Game->Sprites->SetSpriteDepth(bullet.spr, 3);
	bullet.tz = Game->Sprites->TileSize(bullet.spr);
    bullet.tz.x = bullet.tz.x * bulletscale.x;
    bullet.tz.y = bullet.tz.y * bulletscale.y;
	int tmpx = bullet.tz.x;
	bullet.tz.x = bullet.tz.y;
	bullet.tz.y = tmpx;
	bullet.pos = { player.pos.x, player.pos.y - (player.tz.y) + (bullet.tz.y /2)};
	bullet.vel = {0, -bulletspeed};
	Game->Sprites->SetSpriteLocation(bullet.spr, bullet.pos);
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
			if (bullet.pos.y - bullet.tz.y /2 <= screentop)
            {
				bullet.freeze = 6;
				Game->Sprites->SetSpriteAnimation(bullet.spr, 0, 1, 10);
            }
			Game->Sprites->SetSpriteLocation(bullet.spr, bullet.pos);
        }
    }
}

void CGamePang::drawbullet()
{
	if (bullet.alive)
    {
        //copy whats on screen to a temporary buffer (i know this already contains the background)
        SDL_Texture* prev = SDL_GetRenderTarget(Game->Renderer);
        SDL_SetRenderTarget(Game->Renderer, Game->TexTmp);
        Game->Image->DrawImage(Game->Renderer, prev, NULL, NULL);
        SDL_SetRenderTarget(Game->Renderer, prev);
        //draw the sprite
		Game->Sprites->DrawSprite(Game->Renderer, bullet.spr);
        //draw bottom part of what was previously on screen back to the screen to obscure bottom part of the chain texture
        //this makes it seem as if the texture is created on the ground instead of at the bottom of the screen, like it is
        //in real time.
        SDL_Rect Rect = {0, screenbottom - backgroundcopyheight, screenright, backgroundcopyheight};
        Game->Image->DrawImage(Game->Renderer, Game->TexTmp, &Rect, &Rect);
    }
}

//player ----------------------------------------------------------------------------------------------------------------

void CGamePang::destroyplayer()
{
	if(player.alive)
    {
		Game->Sprites->RemoveSprite(player.spr);
		player.alive = false;
    }
}

void CGamePang::createplayer()
{
	player.spr = Game->Sprites->CreateSprite();
	Game->Sprites->SetSpriteImage(player.spr, &spritesheetplayer, 12, 8);
	Game->Sprites->SetSpriteAnimation(player.spr, 37, 37, 0);
	Game->Sprites->SetSpriteCollisionShape(player.spr, SHAPE_BOX, Game->Sprites->TileSize(player.spr).x - 24, Game->Sprites->TileSize(player.spr).y-14,0,0,14);

	Game->Sprites->SetSpriteScale(player.spr, playerscale);
	Game->Sprites->SetSpriteDepth(player.spr, 37);
	player.state = playerstateidle;
	player.tz = Game->Sprites->TileSize(player.spr);
    player.tz.x = player.tz.x * playerscale.x;
    player.tz.y = player.tz.y * playerscale.y;
	player.pos = { (float)(screenright - screenleft) / 2,(float)screenbottom -10- player.tz.y  / 2};
	HealthPoints = 3;
	Game->Sprites->SetSpriteLocation(player.spr, player.pos);
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
	Game->Sprites->SetSpriteVisibility(player.spr, player.alive);

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
			if (Game->Input->Buttons.ButLeft)
            {
				if ( player.pos.x - player.tz.x / 2 - playerspeed > screenleft)
                {
					player.pos.x -=playerspeed;
					if (!playerstate(playerstatemoveleft))
                    {
						addplayerstate(playerstatemoveleft);
						Game->Sprites->SetSpriteAnimation(player.spr, 12, 14, 10);
                    }
                }
				else
                {
					player.pos.x = screenleft + player.tz.x / 2;
					if (playerstate(playerstatemoveleft))
                    {
						remplayerstate(playerstatemoveleft);
						Game->Sprites->SetSpriteAnimation(player.spr, 37, 37, 0);
                    }
                }
            }
			else
            {
				if (Game->Input->Buttons.ButRight)
                {
					if ( player.pos.x + player.tz.x / 2 + playerspeed < screenright)
                    {
						player.pos.x += playerspeed;
						if (!playerstate(playerstatemoveright))
                        {
							addplayerstate(playerstatemoveright);
							Game->Sprites->SetSpriteAnimation(player.spr, 24, 26, 10);
                        }
                    }
					else
                    {
						player.pos.x = screenright - player.tz.x / 2;
						if (playerstate(playerstatemoveleft))
                        {
							remplayerstate(playerstatemoveright);
							Game->Sprites->SetSpriteAnimation(player.spr,37, 37, 0);
                        }
                    }
                }
				else
                {
					remplayerstate(playerstatemoveright);
					remplayerstate(playerstatemoveleft);
					Game->Sprites->SetSpriteAnimation(player.spr, 37, 37, 0);
                }
            }

			if ((!playerstate(playerstatereviving)) && (!Game->Input->PrevButtons.ButA && Game->Input->Buttons.ButA))
            {
				if (!bullet.alive)
                {
					addplayerstate(playerstateshoot);
					remplayerstate(playerstatemoveright);
					remplayerstate(playerstatemoveleft);
					Game->Sprites->SetSpriteAnimation(player.spr, 37, 37, 10);
					player.stateticks = 15;
					createbullet();
					Game->Audio->PlaySound(SfxShoot, 0);
                }
            }
			Game->Sprites->SetSpriteLocation(player.spr, player.pos);
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
				Game->Sprites->DrawSprite(Game->Renderer, player.spr);
        }
		else
			Game->Sprites->DrawSprite(Game->Renderer, player.spr);
    }
}


//background ----------------------------------------------------------------------------------------------------------------

void CGamePang::drawbackground(bool motionblur)
{
	float alpha = 255;
	if ((motionblur) && (Game->MotionBlur))
		alpha = 0.3*255;

    SDL_Texture* prev = SDL_GetRenderTarget(Game->Renderer);
    SDL_SetRenderTarget(Game->Renderer, Game->TexTmp);
    SDL_SetRenderDrawColor(Game->Renderer, 0.3*255, 0.6*255,0.9*255,255);
    SDL_RenderClear(Game->Renderer);
    SDL_Point Pos = { 0, 0};
    Vec2F Scale = {(float)(screenright - screenleft) / backgroundcloudtz.x, 4};
	Game->Image->DrawImageFuze(Game->Renderer, backgroundcloud, false, &Pos, 0, &Scale, 255, 255, 255, 255 );
	
    Scale = {2,2};
	for (int x = -20; x < screenright; x += backgroundtreestz.x)
    {
        Pos = {x, screenbottom - 345-160};
        
		Game->Image->DrawImageFuze(Game->Renderer, backgroundtrees, false, &Pos, 0, &Scale, 255, 255, 255, 255);
    }

    Scale = {2,2};
	for (int x = -20; x < screenright; x += backgroundtreestz.x)
    {
        Pos = {x, screenbottom - 315-130};
		Game->Image->DrawImageFuze(Game->Renderer, backgroundtrees, false, &Pos, 0, &Scale, 255, 255, 255, 255);
    }
	
    Scale = {2,2};
	for (int x = -60; x < screenright; x +=  backgroundtreestz.x)
    {
        Pos = {x, screenbottom - 275-100};
		Game->Image->DrawImageFuze(Game->Renderer, backgroundtrees, false, &Pos, 0, &Scale, 255, 255, 255, 255);
    }

    Scale = {2,2};
	for (int x = -35; x < screenright; x += backgroundtreestz.x)
    {
        Pos = {x, screenbottom - 225-150};
		Game->Image->DrawImageFuze(Game->Renderer, backgroundtrees, false, &Pos, 0, &Scale, 255, 255, 255, 255);
    }

    Pos = {(int)((screenright - screenleft) * 0.55), screenbottom - 390};
    Scale = {3.5,3.5};
	Game->Image->DrawImageFuze(Game->Renderer, backgroundtree, false, &Pos, 0, &Scale, 255, 255, 255, 255);
	
    Scale = {1,1};
	for (int x = 0; x < screenright; x+= backgroundgrasstz.x)
    {
        Pos = {x, screenbottom - backgroundgrasstz.y};
		Game->Image->DrawImageFuze(Game->Renderer, backgroundgrass, false, &Pos, 0, &Scale, 255, 255, 255, 255);
    }   

    SDL_SetRenderTarget(Game->Renderer, prev);
    Scale = {1,1};
    Pos = {0,0};
    Game->Image->DrawImageFuze(Game->Renderer, Game->TexTmp, NULL, false, &Pos, 0, &Scale, 255, 255, 255, alpha);
	
}

//init - deinit ----------------------------------------------------------------------------------------------------------------

void CGamePang::init()
{
    SDL_Texture* prev = SDL_GetRenderTarget(Game->Renderer);
    SDL_SetRenderTarget(Game->Renderer, Game->TexTmp);
    SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Game->Renderer);
    SDL_SetRenderTarget(Game->Renderer, prev);
    LoadGraphics();
    level = 1;
	if(!ScreenshotMode)
	{        
        createplayer();
	    createballs();
		SfxDie = Game->Audio->LoadSound("common/die.wav");
		SfxSucces = Game->Audio->LoadSound("common/succes.wav");
        SfxShoot = Game->Audio->LoadSound("pang/shoot.wav");
        SfxPop = Game->Audio->LoadSound("pang/pop.wav");
		MusMusic = Game->Audio->LoadMusic("pang/music.ogg");	
		Game->CurrentGameMusicID = MusMusic;
		Game->Audio->PlayMusic(MusMusic, -1);
	}
}

void CGamePang::deinit()
{  
    destroyplayer();
	destroyallballs();
	destroybullet();
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

void CGamePang::LoadGraphics()
{
    spritesheetplayer = Game->Image->LoadImage(Game->Renderer, "pang/character.png");
    spritesheetbullet = Game->Image->LoadImage(Game->Renderer, "pang/weapon.png");
    spritesheetball = Game->Image->LoadImage(Game->Renderer, "pang/ball.png");
    backgroundgrass = Game->Image->LoadImage(Game->Renderer, "pang/grass.png");
    backgroundcloud = Game->Image->LoadImage(Game->Renderer, "pang/clouds.png");
    backgroundtrees = Game->Image->LoadImage(Game->Renderer, "pang/trees.png");
    backgroundtree = Game->Image->LoadImage(Game->Renderer, "pang/tree.png");
    backgroundgrasstz = Game->Image->ImageSize(backgroundgrass);
    backgroundcloudtz = Game->Image->ImageSize(backgroundcloud);
    backgroundtreestz = Game->Image->ImageSize(backgroundtrees);
    backgroundtreetz = Game->Image->ImageSize(backgroundtree);
    spritesheetballtz = Game->Image->ImageSize(spritesheetball);
}

void CGamePang::UnloadGraphics()
{
	Game->Image->UnLoadImage(spritesheetplayer);
    Game->Image->UnLoadImage(spritesheetbullet);
    Game->Image->UnLoadImage(spritesheetball);
    Game->Image->UnLoadImage(backgroundgrass);
    Game->Image->UnLoadImage(backgroundcloud);
    Game->Image->UnLoadImage(backgroundtrees);
    Game->Image->UnLoadImage(backgroundtree);
}

SDL_Texture* CGamePang::screenshot()
{
	SDL_Texture* prev = SDL_GetRenderTarget(Game->Renderer);
	SDL_Texture* image = SDL_CreateTexture(Game->Renderer, PixelFormat, SDL_TEXTUREACCESS_TARGET, ScreenWidth, ScreenHeight);
    SDL_SetRenderTarget(Game->Renderer, image);
	SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 255);
	SDL_RenderClear(Game->Renderer);
	init();
    level = 3;
	
    createplayer();
	createballs();
	createbullet();

	for (int i = 0; i < 35; i++)
    {
		updateballs();
		updatebullet();
    }
	
    drawbackground(false);
	drawballs();
    drawbullet();
	
    drawplayer();
    
	SDL_RenderPresent(Game->Renderer);
	SDL_SetRenderTarget(Game->Renderer, prev);
	deinit();	
	return image;
}

//Update ----------------------------------------------------------------------------------------------------------------

void CGamePang::UpdateLogic()
{
    CGameBase::UpdateLogic();
	if (Game->SubGameState == SGGame)
	{
        updateplayer();
		updateballs();
		updatebullet();			
		Game->Sprites->UpdateSprites();
	}
}

void CGamePang::Draw()
{
    drawbackground(Game->SubGameState == SGGame);

    drawballs();
	drawbullet();
	drawplayer();

    DrawScoreBar();
	DrawSubStateText();
}
