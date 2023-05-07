#include <SDL.h>
#include <string>
#include <iostream>
#include <cmath>
#include "CGameBreakOut.h"
#include "../CGame.h"
#include "../Common.h"
#include "../CTween.h"
#include "../Vec2F.h"

using namespace std;

CGameBreakOut::CGameBreakOut(CGame* aGame, bool aScreenshotMode): CGameBase(aGame, GSBreakout, false, aScreenshotMode)
{
    Game = aGame;
	MusMusic = -1;
    SfxSucces = -1;
    SfxDie = -1;
    SfxBrick = -1;
    SfxBat = -1;
    screenleft = 0;
    screenright = ScreenWidth;
    screentop = 0;
    screenbottom = ScreenHeight;
}

CGameBreakOut::~CGameBreakOut() {};


//blocks  ---------------------------------------------------------------------------------------------------------------- 

void CGameBreakOut::updateblockinfo()
{
	blockinfo.mostleft = -1;
	blockinfo.mostright = -1;
	blockinfo.mostbottom = -1;

	int x1 = screenright + 1;
	int x2 = screenleft - 1;
	int y = screentop - 1;
	for (int i = 0; i < numblocks; i++)
    {	
		if (blocks[i].alive)
        {
			if (blocks[i].pos.x < x1)
            {
				x1 = blocks[i].pos.x;
				blockinfo.mostleft = i;
            }

			if (blocks[i].pos.x > x2)
            {
				x2 = blocks[i].pos.x;
				blockinfo.mostright = i;
            }
		
			if (blocks[i].pos.y > y)
            {
				y = blocks[i].pos.y;
				blockinfo.mostbottom = i;
            }
        }
    }
}


void CGameBreakOut::destroyallblocks()
{
	for (int i = 0; i < numblocks; i++)
		destroyblock(i);
}

void CGameBreakOut::destroyblock(int index)
{
	if(blocks[index].alive)
    {
		Game->Sprites->RemoveSprite(blocks[index].spr);
		blocks[index].alive = false;
    }
}

void CGameBreakOut::createblocks(bool setlocation)
{
	pattern = rand() % 5;
	for (int x = 0; x < blockcols; x++)
    {
		for(int y = 0; y < blockrows; y++)
        {			
			tweens[x + y * blockcols][tweenblockpositions] = createtween(tweenblockpositions, 1+ ((rand() %(6)) / 10), funcsmoothstop, 1, true, DesiredFps);
			blocks[x + y * blockcols].spr = Game->Sprites->CreateSprite();
			blocks[x + y * blockcols].state = 0;
			blocks[x + y * blockcols].alive = true;
			Game->Sprites->SetSpriteImage(blocks[x + y * blockcols].spr, &spritesheetblocks, 6, 1);
			SDL_Point tz = Game->Sprites->TileSize(blocks[x + y * blockcols].spr);
            tz.x =  tz.x * blockspritecale.x;
            tz.y =  tz.y * blockspritecale.y;
			blocks[x + y * blockcols].tz = tz;
			Game->Sprites->SetSpriteAnimation(blocks[x + y * blockcols].spr, y % 6, y % 6, 0);

			Game->Sprites->SetSpriteScale(blocks[x + y * blockcols].spr, blockspritecale);
			blocks[x + y * blockcols].pos = { screenleft + blockxoffset + (x * tz.x), screentop + blockyoffset + y * tz.y}; 
			if (setlocation)
				Game->Sprites->SetSpriteLocation(blocks[x + y * blockcols].spr, blocks[x + y * blockcols].pos);
		
        }
    }
	updateblockinfo();
}


void CGameBreakOut::updateblocks()
{
	for(int x = 0; x < blockcols; x++)
    {
		for(int y = 0; y < blockrows; y++)
        {			
			if(blocks[x + y * blockcols].alive)
            {
				if(tweens[x + y * blockcols][tweenblockpositions].active)
                {
					tweens[x + y * blockcols][tweenblockpositions] = updatetween(tweens[x + y * blockcols][tweenblockpositions]);
					Vec2F pos = blocks[x + y * blockcols].pos;
					if (pattern == 0)
                    {
						if (x < blockcols / 3)
                            pos.x = pos.x * tweens[x + y * blockcols][tweenblockpositions].funcval;
						else
                        {
							if( x > blockcols * 2 / 3)
								pos.x = screenright - (screenright - pos.x) * tweens[x + y * blockcols][tweenblockpositions].funcval;
							
                        }
						if (y < blockrows / 2)
							pos.y = pos.y * tweens[x + y * blockcols][tweenblockpositions].funcval;
                    }
					else 
                    {
						if(pattern == 1)
							pos.y = pos.y * tweens[x + y * blockcols][tweenblockpositions].funcval;
						else
                        {
							if (pattern == 2)
                            {
								if (x < blockcols / 2)                            
									pos.x = pos.x * tweens[x + y * blockcols][tweenblockpositions].funcval;
								else 
									pos.x = screenright - (screenright - pos.x) * tweens[x + y * blockcols][tweenblockpositions].funcval;
                            }
							else
                            {
								if (pattern == 3)
									pos.x = pos.x * tweens[x + y * blockcols][tweenblockpositions].funcval;
								else
                                {
									if (pattern == 4)
										pos.x = screenright - (screenright - pos.x) * tweens[x + y * blockcols][tweenblockpositions].funcval;
                                }
                            }
                        }
                    }
	
//					setSpriteLocation(blocks[x + y * blockcols].spr, pos)
					blocks[x + y * blockcols].spr->x = int(pos.x);
					blocks[x + y * blockcols].spr->y = int(pos.y);
					
                }
				if (blocks[x + y * blockcols].state == blockstatedeath)
                {
					if (tweens[x + y * blockcols][tweenblockdeath].active)
                    {
						tweens[x + y * blockcols][tweenblockdeath] = updatetween(tweens[x + y * blockcols][tweenblockdeath]);
						Vec2F pos = blocks[x + y * blockcols].pos;
						pos.y = pos.y + (50 * tweens[x + y * blockcols][tweenblockdeath].funcval);
						pos.x = pos.x + (30 * tweens[x + y * blockcols][tweenblockdeath].funcval);
//						setSpriteLocation(blocks[x + y * blockcols].spr, pos)
						blocks[x + y * blockcols].spr->x = int(pos.x);
						blocks[x + y * blockcols].spr->y = int(pos.y);
						Game->Sprites->SetSpriteRotation(blocks[x + y * blockcols].spr, 720 * tweens[x + y * blockcols][tweenblockdeath].funcval);
						Game->Sprites->SetSpriteColour(blocks[x + y * blockcols].spr, 1,1,1, 1- tweens[x + y * blockcols][tweenblockdeath].funcval);
                    }
					else
                    {
						destroyblock(x + y * blockcols);
						updateblockinfo();
                    }
                }

            }
        }
    }

	if (blockinfo.mostleft == -1)
    {
		Game->AddToScore(500);
		createblocks(false);
		Game->Audio->PlaySound(SfxSucces, 0);
    }
}

//player ----------------------------------------------------------------------------------------------------------------

void CGameBreakOut::destroyplayer()
{
	if (player.alive)
    {
		Game->Sprites->RemoveSprite(player.spr);
		player.alive = false;
    }
}

void CGameBreakOut::createplayer()
{
	player.spr = Game->Sprites->CreateSprite();
	Game->Sprites->SetSpriteImage(player.spr, &spritesheetbat);
	Game->Sprites->SetSpriteScale(player.spr, spritescale);
	player.tz = Game->Image->ImageSize(spritesheetbat);
    player.tz.x = player.tz.x * spritescale.x;
    player.tz.y = player.tz.y * spritescale.y;
	player.pos = { (float)(screenright - screenleft) / 2,(float)screenbottom - 20 - (player.tz.y  / 2)};
	HealthPoints = 5;
	Game->Sprites->SetSpriteLocation(player.spr, player.pos);
	player.alive = true;
}

void CGameBreakOut::updateplayer()
{
	Game->Sprites->SetSpriteVisibility(player.spr, player.alive);
	if (player.alive)
    {
		float speedmultiplier = 1;
		
		if (Game->Input->Buttons.ButA)
			speedmultiplier = 2;
		
		if (Game->Input->Buttons.ButLeft)
        {
			if (player.pos.x - player.tz.x / 2 - (playerspeed * speedmultiplier) > screenleft)
				player.pos.x -= playerspeed * speedmultiplier;
			else
				player.pos.x = screenleft + player.tz.x / 2;
        }

		if (Game->Input->Buttons.ButRight)
        {
			if ( player.pos.x + player.tz.x / 2 + (playerspeed * speedmultiplier) < screenright)
				player.pos.x += playerspeed * speedmultiplier;
			else
				player.pos.x = screenright - player.tz.x / 2;
        }

		//setSpriteLocation(player.spr, player.pos)
		player.spr->x = int(player.pos.x);
		player.spr->y = int(player.pos.y);
    }
	else
    {
		if (player.freeze > 0)
			player.freeze -= 1;
		else
			player.alive = true;
    }
}


//ball ----------------------------------------------------------------------------------------------------------------

void CGameBreakOut::destroyball()
{
	if (ball.alive)
    {
		Game->Sprites->RemoveSprite(ball.spr);
		ball.alive = false;
    }
}
			
void CGameBreakOut::createball()
{
	ball.spr = Game->Sprites->CreateSprite();
	Game->Sprites->SetSpriteImage(ball.spr, &spritesheetball);
	ball.tz = Game->Image->ImageSize(spritesheetball);
	Game->Sprites->SetSpriteCollisionShape(ball.spr, SHAPE_CIRCLE, ball.tz.x, ball.tz.y, 0, 0, 0);
	ball.pos = { (float)((screenright - screenleft) / 2) + 250, (float)screenbottom - 250 - 20};
	ball.vel = {-0.5,0.5};
	curballspeed = ballspeed;
	ball.alive = true;
	Game->Sprites->SetSpriteLocation(ball.spr, ball.pos);
}

void CGameBreakOut::updateball()
{
	if (ball.alive)
    {			
		if (ball.freeze > 0)
			ball.freeze -= 1;
		else
        {
			float steps = curballspeed  / ballvelsegments;
			
			for (int j = 1; j <ballvelsegments + 1; j++)
            {
				ball.pos.x += (steps * ball.vel.x);
                ball.pos.y += (steps * ball.vel.y);

				if (ball.pos.x < screenleft)
                {
					ball.vel.x *= -1;
					ball.pos.x = screenleft;
                }
		
				if (ball.pos.x > screenright)
                {
					ball.vel.x *= -1;
					ball.pos.x = screenright;
                }
				
				if (ball.pos.y < screentop)
                {
					ball.pos.y = screentop;
					ball.vel.y *= -1;
                }

				Game->Sprites->SetSpriteLocation(ball.spr, ball.pos);
		
				if (ball.pos.y >= screenbottom)
                {
					destroyball();
					createball();
					ball.freeze = 45;
					HealthPoints -= 1;
					Game->AddToScore(-100);
					Game->Audio->PlaySound(SfxDie, 0);
                }

				for (int k = 0; k < numblocks; k++)
                {				
					if (blocks[k].alive && (blocks[k].state != blockstatedeath))
                    {	
						if (Game->Sprites->DetectSpriteCollision(ball.spr, blocks[k].spr))
                        {
							//seen this in wireframe issue 11 not sure what it actually
							//calculates but seems to work more or less ok
							float dx = (ball.pos.x - blocks[k].pos.x) / blocks[k].tz.x;
							float dy = (ball.pos.y - blocks[k].pos.y) / blocks[k].tz.y;
							if (abs(dx) > abs(dy))
								ball.vel.x = abs(ball.vel.x) * dx / abs(dx);
                            else
								ball.vel.y = abs(ball.vel.y) * dy / abs(dy);

							Game->AddToScore(20);
							//inc ballspeed
							curballspeed += ballspeedinc;
							Game->Audio->PlaySound(SfxBrick, 0);
							blocks[k].state = blockstatedeath;
							Game->Sprites->SetSpriteDepth(blocks[k].spr, 5);
							tweens[k][tweenblockdeath] = createtween(tweenblockdeath, 1, funcsmoothstep, 1, true, DesiredFps);
                        }
                    }
                }
	
				if (player.alive)
                {	
					if (Game->Sprites->DetectSpriteCollision(ball.spr, player.spr))
                    {	
						//touched top part of bat
						if (ball.pos.y < player.pos.y + player.tz.y / 3)
                        {
							//set ball to top part - 1 so it can't callide again
							ball.pos.y = player.pos.y - player.tz.y / 2 - 1;
							Game->Sprites->SetSpriteLocation(ball.spr, ball.pos);
		
							//inverrt y velocity
							ball.vel.y = -ball.vel.y;	
							//touch far right of bat
							if (ball.pos.x >= player.pos.x + ((player.tz.x / 2) * 5 / 7))
                            {
								//and bal is moving to the left
								if (ball.vel.x < 0)
                                {
									//reverse x velocity
									ball.vel.x = -ball.vel.x;
									//add some randomness
									float rnd = (rand() % 100) / 500;
									if (rand() % 2  == 0)
										rnd *= -1;

									ball.vel.x += rnd;
                                }
                            }
							else
                            {
								//touched far left of bat
								if (ball.pos.x <= player.pos.x - ((player.tz.x / 2) * 5 / 7))
                                {
									//and ball moving to the right
									if (ball.vel.x > 0)
                                    {
										//reverse x velocity
										ball.vel.x = -ball.vel.x;
										//add some randomness
										float rnd = (rand() % 100) / 500;
                                        if (rand() % 2  == 0)
                                            rnd *= -1;

										ball.vel.x += rnd;
                                    }
                                }
                            }
                        }
						else
                        {
							//bottom parts = loss of ball
							// ball is on the right
							if (ball.pos.x > player.pos.x)
                            { 
								//ball is moving towards bat to left
								if (ball.vel.x < 0)
									//reverse x velocity
									ball.vel.x = -ball.vel.x;
                            }
							else
                            {
								//ball is on the left and moving to the right
								if (ball.vel.x > 0)
									//reverse x velocity
									ball.vel.x = -ball.vel.x;
                            }
                        }
						
						//increase ballspeed	
						curballspeed += ballspeedinc;
						
						//just a safety
						while (Game->Sprites->DetectSpriteCollision(ball.spr, player.spr))
                        {
                            ball.pos.x += (steps * ball.vel.x);
                            ball.pos.y += (steps * ball.vel.y);
							Game->Sprites->SetSpriteLocation(ball.spr, ball.pos);
                        }
	
						Game->Audio->PlaySound(SfxBat, 0);
                    }
                }
            }
        }
    }
}
	
//background ----------------------------------------------------------------------------------------------------------------

void CGameBreakOut::drawbackground(bool motionblur)
{
	float alpha = 1;
	if ((motionblur) && (Game->MotionBlur))
		alpha = 0.3;
    SDL_Point Pos = { ScreenWidth / 2, ScreenHeight / 2};
    Vec2F Scale = {(float)ScreenWidth / backgroundtz.x, (float)ScreenHeight / backgroundtz.y};
	Game->Image->DrawImageFuzeTintFloat(Game->Renderer, background, true, &Pos, 0, &Scale, 1, 1, 1, alpha);
}

//init - deinit ----------------------------------------------------------------------------------------------------------------

void CGameBreakOut::init()
{
    SDL_Texture* prev = SDL_GetRenderTarget(Game->Renderer);
    SDL_SetRenderTarget(Game->Renderer, Game->TexTmp);
    SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Game->Renderer);
    SDL_SetRenderTarget(Game->Renderer, prev);
    LoadGraphics();
    createblocks(ScreenshotMode);
	createplayer();
	createball();

    if(!ScreenshotMode)
	{
		SfxDie = Game->Audio->LoadSound("common/die.wav");
		SfxSucces = Game->Audio->LoadSound("common/succes.wav");
        SfxBat = Game->Audio->LoadSound("breakout/bat.wav");
        SfxBrick = Game->Audio->LoadSound("breakout/brick.wav");
        MusMusic = Game->Audio->LoadMusic("breakout/music.ogg");	
		Game->CurrentGameMusicID = MusMusic;
		Game->Audio->PlayMusic(MusMusic, -1);
	}
}

void CGameBreakOut::deinit()
{
    destroyplayer();
	destroyallblocks();
	destroyball();
	if (!ScreenshotMode)
	{
		Game->Audio->StopMusic();
		Game->Audio->StopSound();
		Game->Audio->UnLoadMusic(MusMusic);
        Game->Audio->UnLoadSound(SfxSucces);
        Game->Audio->UnLoadSound(SfxDie);
        Game->Audio->UnLoadSound(SfxBrick);
        Game->Audio->UnLoadSound(SfxBat);
		Game->SubStateCounter = 0;
		Game->SubGameState = SGNone;
		Game->CurrentGameMusicID = -1;
        UnloadGraphics();       
	}   
}

void CGameBreakOut::LoadGraphics()
{
	background = Game->Image->LoadImage(Game->Renderer, "breakout/background.png");
	backgroundtz = Game->Image->ImageSize(background);
    spritesheetblocks = Game->Image->LoadImage(Game->Renderer, "breakout/blocks.png");
    spritesheetbat = Game->Image->LoadImage(Game->Renderer, "breakout/paddle.png");
    spritesheetball = Game->Image->LoadImage(Game->Renderer, "breakout/ball.png");
}

void CGameBreakOut::UnloadGraphics()
{
	Game->Image->UnLoadImage(background);
    Game->Image->UnLoadImage(spritesheetblocks);
    Game->Image->UnLoadImage(spritesheetbat);
    Game->Image->UnLoadImage(spritesheetball);
}

SDL_Texture* CGameBreakOut::screenshot()
{
	SDL_Texture* prev = SDL_GetRenderTarget(Game->Renderer);
	SDL_Texture* image = SDL_CreateTexture(Game->Renderer, PixelFormat, SDL_TEXTUREACCESS_TARGET, ScreenWidth, ScreenHeight);
    SDL_SetRenderTarget(Game->Renderer, image);
	SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 255);
	SDL_RenderClear(Game->Renderer);
	init();
    
    drawbackground(false);
	Game->Sprites->DrawSprites(Game->Renderer);

	
	SDL_RenderPresent(Game->Renderer);
	SDL_SetRenderTarget(Game->Renderer, prev);
	deinit();	
	return image;
}

//Update ----------------------------------------------------------------------------------------------------------------

void CGameBreakOut::UpdateLogic()
{
    CGameBase::UpdateLogic();
	updateblocks();
    if (Game->SubGameState == SGGame)
	{
        updateplayer();
        updateball();
        Game->Sprites->UpdateSprites();
	}
}

void CGameBreakOut::Draw()
{
    drawbackground(Game->SubGameState == SGGame);
	Game->Sprites->DrawSprites(Game->Renderer);
	
    DrawScoreBar();
	DrawSubStateText();
}
