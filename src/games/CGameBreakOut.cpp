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

CGameBreakOut::CGameBreakOut(CGame* aGame, bool aScreenshotMode)
{
	GameBase = new CGameBase(aGame, GSBreakout, false, aScreenshotMode);
	MusMusic = -1;
	SfxSucces = -1;
	SfxDie = -1;
	SfxBrick = -1;
	SfxBat = -1;
	GameBase->screenleft = 0;
	GameBase->screenright = ScreenWidth;
	GameBase->screentop = 0;
	GameBase->screenbottom = ScreenHeight;
}

CGameBreakOut::~CGameBreakOut()
{

}

//blocks ----------------------------------------------------------------------------------------------------------------

void CGameBreakOut::updateblockinfo()
{
	blockinfo.mostleft = -1;
	blockinfo.mostright = -1;
	blockinfo.mostbottom = -1;

	int x1 = GameBase->screenright + 1;
	int x2 = GameBase->screenleft - 1;
	int y = GameBase->screentop - 1;
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
		GameBase->Game->Sprites->RemoveSprite(blocks[index].spr);
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
			blocks[x + y * blockcols].spr = GameBase->Game->Sprites->CreateSprite();
			blocks[x + y * blockcols].state = 0;
			blocks[x + y * blockcols].alive = true;
			GameBase->Game->Sprites->SetSpriteImage(GameBase->Game->Renderer,blocks[x + y * blockcols].spr, &spritesheetblocks, 6, 1);
			SDL_Point tz = GameBase->Game->Sprites->TileSize(blocks[x + y * blockcols].spr);
			tz.x = tz.x * blockspritecale.x;
			tz.y = tz.y * blockspritecale.y;
			blocks[x + y * blockcols].tz = tz;
			GameBase->Game->Sprites->SetSpriteAnimation(blocks[x + y * blockcols].spr, y % 6, y % 6, 0);

			GameBase->Game->Sprites->SetSpriteScale(GameBase->Game->Renderer,blocks[x + y * blockcols].spr, blockspritecale);
			blocks[x + y * blockcols].pos = { GameBase->screenleft + blockxoffset + (x * tz.x), GameBase->screentop + blockyoffset + y * tz.y};
			if (setlocation)
				GameBase->Game->Sprites->SetSpriteLocation(blocks[x + y * blockcols].spr, blocks[x + y * blockcols].pos);

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
								pos.x = GameBase->screenright - (GameBase->screenright - pos.x) * tweens[x + y * blockcols][tweenblockpositions].funcval;
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
									pos.x = GameBase->screenright - (GameBase->screenright - pos.x) * tweens[x + y * blockcols][tweenblockpositions].funcval;
							}
							else
							{
								if (pattern == 3)
									pos.x = pos.x * tweens[x + y * blockcols][tweenblockpositions].funcval;
								else
								{
									if (pattern == 4)
										pos.x = GameBase->screenright - (GameBase->screenright - pos.x) * tweens[x + y * blockcols][tweenblockpositions].funcval;
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
						GameBase->Game->Sprites->SetSpriteRotation(blocks[x + y * blockcols].spr, 720 * tweens[x + y * blockcols][tweenblockdeath].funcval);
						GameBase->Game->Sprites->SetSpriteColour(blocks[x + y * blockcols].spr, 1,1,1, 1- tweens[x + y * blockcols][tweenblockdeath].funcval);
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
		GameBase->Game->AddToScore(500);
		createblocks(false);
		GameBase->Game->Audio->PlaySound(SfxSucces, 0);
	}
}

//player ----------------------------------------------------------------------------------------------------------------

void CGameBreakOut::destroyplayer()
{
	if (player.alive)
	{
		GameBase->Game->Sprites->RemoveSprite(player.spr);
		player.alive = false;
	}
}

void CGameBreakOut::createplayer()
{
	player.spr = GameBase->Game->Sprites->CreateSprite();
	GameBase->Game->Sprites->SetSpriteImage(GameBase->Game->Renderer,player.spr, &spritesheetbat);
	GameBase->Game->Sprites->SetSpriteScale(GameBase->Game->Renderer,player.spr, spritescale);
	player.tz = GameBase->Game->Image->ImageSize(spritesheetbat);
	player.tz.x = player.tz.x * spritescale.x;
	player.tz.y = player.tz.y * spritescale.y;
	player.pos = { (float)(GameBase->screenright - GameBase->screenleft) / 2,(float)GameBase->screenbottom - 20*yscale - (player.tz.y / 2)};
	GameBase->HealthPoints = 5;
	GameBase->Game->Sprites->SetSpriteLocation(player.spr, player.pos);
	player.alive = true;
}

void CGameBreakOut::updateplayer()
{
	GameBase->Game->Sprites->SetSpriteVisibility(player.spr, player.alive);
	if (player.alive)
	{
		float speedmultiplier = 1;

		if (GameBase->Game->Input->Buttons.ButA)
			speedmultiplier = 2;

		if ((GameBase->Game->Input->Buttons.ButLeft) ||
			(GameBase->Game->Input->Buttons.ButLeft2) ||
			(GameBase->Game->Input->Buttons.ButDpadLeft))
		{
			if (player.pos.x - player.tz.x / 2 - (playerspeed * speedmultiplier) > GameBase->screenleft)
				player.pos.x -= playerspeed * speedmultiplier;
			else
				player.pos.x = GameBase->screenleft + player.tz.x / 2;
		}

		if ((GameBase->Game->Input->Buttons.ButRight) ||
			(GameBase->Game->Input->Buttons.ButRight2) ||
			(GameBase->Game->Input->Buttons.ButDpadRight))
		{
			if ( player.pos.x + player.tz.x / 2 + (playerspeed * speedmultiplier) < GameBase->screenright)
				player.pos.x += playerspeed * speedmultiplier;
			else
				player.pos.x = GameBase->screenright - player.tz.x / 2;
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
		GameBase->Game->Sprites->RemoveSprite(ball.spr);
		ball.alive = false;
	}
}

void CGameBreakOut::createball()
{
	ball.spr = GameBase->Game->Sprites->CreateSprite();
	GameBase->Game->Sprites->SetSpriteImage(GameBase->Game->Renderer,ball.spr, &spritesheetball);
	GameBase->Game->Sprites->SetSpriteScale(GameBase->Game->Renderer,ball.spr, {xscale,yscale});
	ball.tz = GameBase->Game->Image->ImageSize(spritesheetball);
	ball.tz.x = ball.tz.x * xscale;
	ball.tz.y = ball.tz.y * yscale;
	
	GameBase->Game->Sprites->SetSpriteCollisionShape(ball.spr, SHAPE_BOX, 4/xscale, 4/yscale, 0, 0, 0);
	ball.pos = { (float)((GameBase->screenright - GameBase->screenleft) / 2) + 250*xscale, (float)GameBase->screenbottom - 250*yscale - 20*yscale};
	ball.vel = {-0.5,0.5};
	curballspeed = ballspeed;
	ball.alive = true;
	GameBase->Game->Sprites->SetSpriteLocation(ball.spr, ball.pos);
}

void CGameBreakOut::updateball()
{
	if (ball.alive)
	{
		if (ball.freeze > 0)
			ball.freeze -= 1;
		else
		{
			float steps = curballspeed / ballvelsegments;

			for (int j = 1; j <ballvelsegments + 1; j++)
			{
				ball.pos.x += (steps * ball.vel.x);
				ball.pos.y += (steps * ball.vel.y);

				if (ball.pos.x < GameBase->screenleft)
				{
					ball.vel.x *= -1;
					ball.pos.x = GameBase->screenleft;
				}

				if (ball.pos.x > GameBase->screenright)
				{
					ball.vel.x *= -1;
					ball.pos.x = GameBase->screenright;
				}

				if (ball.pos.y < GameBase->screentop)
				{
					ball.pos.y = GameBase->screentop;
					ball.vel.y *= -1;
				}

				GameBase->Game->Sprites->SetSpriteLocation(ball.spr, ball.pos);

				if (ball.pos.y >= GameBase->screenbottom)
				{
					destroyball();
					createball();
					ball.freeze = 45;
					GameBase->HealthPoints -= 1;
					GameBase->Game->AddToScore(-100);
					GameBase->Game->Audio->PlaySound(SfxDie, 0);
				}

				for (int k = 0; k < numblocks; k++)
				{
					if (blocks[k].alive && (blocks[k].state != blockstatedeath))
					{
						if (GameBase->Game->Sprites->DetectSpriteCollision(ball.spr, blocks[k].spr))
						{
							//seen this in wireframe issue 11 not sure what it actually
							//calculates but seems to work more or less ok
							float dx = (ball.pos.x - blocks[k].pos.x) / blocks[k].tz.x;
							float dy = (ball.pos.y - blocks[k].pos.y) / blocks[k].tz.y;
							if (abs(dx) > abs(dy))
								ball.vel.x = abs(ball.vel.x) * dx / abs(dx);
							else
								ball.vel.y = abs(ball.vel.y) * dy / abs(dy);

							GameBase->Game->AddToScore(20);
							//inc ballspeed
							curballspeed += ballspeedinc;
							GameBase->Game->Audio->PlaySound(SfxBrick, 0);
							blocks[k].state = blockstatedeath;
							GameBase->Game->Sprites->SetSpriteDepth(blocks[k].spr, 5);
							tweens[k][tweenblockdeath] = createtween(tweenblockdeath, 1, funcsmoothstep, 1, true, DesiredFps);
						}
					}
				}

				if (player.alive)
				{
					if (GameBase->Game->Sprites->DetectSpriteCollision(ball.spr, player.spr))
					{
						//touched top part of bat
						if (ball.pos.y < player.pos.y + player.tz.y / 3)
						{
							//set ball to top part - 1 so it can't callide again
							ball.pos.y = player.pos.y - player.tz.y / 2 - 1;
							GameBase->Game->Sprites->SetSpriteLocation(ball.spr, ball.pos);

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
									if (rand() % 2 == 0)
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
										if (rand() % 2 == 0)
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
						while (GameBase->Game->Sprites->DetectSpriteCollision(ball.spr, player.spr))
						{
							ball.pos.x += (steps * ball.vel.x);
							ball.pos.y += (steps * ball.vel.y);
							GameBase->Game->Sprites->SetSpriteLocation(ball.spr, ball.pos);
						}

						GameBase->Game->Audio->PlaySound(SfxBat, 0);
					}
				}
			}
		}
	}
}

//background ----------------------------------------------------------------------------------------------------------------

void CGameBreakOut::DrawBackground(bool motionblur)
{
	GameBase->Game->Image->DrawImage(GameBase->Game->Renderer, background, NULL, NULL);
}

void CGameBreakOut::Draw()
{
	DrawBackground((GameBase->Game->SubGameState == SGGame) && !GameBase->ScreenshotMode);
	GameBase->Game->Sprites->DrawSprites(GameBase->Game->Renderer);
	if(!GameBase->ScreenshotMode)
	{
		GameBase->DrawScoreBar();
		GameBase->DrawSubStateText();
	}
}
//init - deinit ----------------------------------------------------------------------------------------------------------------

void CGameBreakOut::init()
{
	LoadGraphics();
	createblocks(GameBase->ScreenshotMode);
	createplayer();
	createball();

	if(!GameBase->ScreenshotMode)
	{
		LoadSound();
		GameBase->Game->CurrentGameMusicID = MusMusic;
		GameBase->Game->Audio->PlayMusic(MusMusic, -1);
	}
}

void CGameBreakOut::LoadSound()
{
	SfxDie = GameBase->Game->Audio->LoadSound("common/die.wav");
	SfxSucces = GameBase->Game->Audio->LoadSound("common/succes.wav");
	SfxBat = GameBase->Game->Audio->LoadSound("breakout/bat.wav");
	SfxBrick = GameBase->Game->Audio->LoadSound("breakout/brick.wav");
	MusMusic = GameBase->Game->Audio->LoadMusic("breakout/music.ogg");
}

void CGameBreakOut::UnLoadSound()
{
	GameBase->Game->Audio->StopMusic();
	GameBase->Game->Audio->StopSound();
	GameBase->Game->Audio->UnLoadMusic(MusMusic);
	GameBase->Game->Audio->UnLoadSound(SfxSucces);
	GameBase->Game->Audio->UnLoadSound(SfxDie);
	GameBase->Game->Audio->UnLoadSound(SfxBrick);
	GameBase->Game->Audio->UnLoadSound(SfxBat);
}

void CGameBreakOut::deinit()
{
	destroyplayer();
	destroyallblocks();
	destroyball();
	if (!GameBase->ScreenshotMode)
	{
		UnLoadSound();
		GameBase->Game->SubStateCounter = 0;
		GameBase->Game->SubGameState = SGNone;
		GameBase->Game->CurrentGameMusicID = -1;
	}
	UnloadGraphics();
}

void CGameBreakOut::LoadGraphics()
{
	background = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "breakout/background.png");
	backgroundtz = GameBase->Game->Image->ImageSize(background);
	spritesheetblocks = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "breakout/blocks.png", 8, 128, dumpScaledBitmaps); 
	spritesheetbat = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "breakout/paddle.png",0, 128, dumpScaledBitmaps);
	spritesheetball = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "breakout/ball.png", 0, 128, dumpScaledBitmaps);

	//SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/breakout/blocks.bmp", Game->Image->GetImage(spritesheetblocks), 1,1, true,8, 128); //0 80
	//SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/breakout/paddle.bmp", Game->Image->GetImage(spritesheetbat), 1,1, true,0, 128); //173
	//SDL_SaveBMPTextureScaled(Game->Renderer, "./retrotimefs/graphics/breakout/ball.bmp", Game->Image->GetImage(spritesheetball), 1,1, true,0, 128); //173

	if(!useDefaultColorAssets)
	{
		UnloadGraphics();
		background = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "breakout/background.png");
		backgroundtz = GameBase->Game->Image->ImageSize(background);
		spritesheetblocks = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "breakout/blocks.bmp");
		spritesheetbat = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "breakout/paddle.bmp");
		spritesheetball = GameBase->Game->Image->LoadImage(GameBase->Game->Renderer, "breakout/ball.bmp");
	}


}

void CGameBreakOut::UnloadGraphics()
{
	GameBase->Game->Image->UnLoadImage(background);
	GameBase->Game->Image->UnLoadImage(spritesheetblocks);
	GameBase->Game->Image->UnLoadImage(spritesheetbat);
	GameBase->Game->Image->UnLoadImage(spritesheetball);
}

SDL_Texture* CGameBreakOut::screenshot()
{
	SDL_Texture* prev = SDL_GetRenderTarget(GameBase->Game->Renderer);
	SDL_Texture* image = SDL_CreateTexture(GameBase->Game->Renderer, PixelFormat, SDL_TEXTUREACCESS_TARGET, ScreenWidth, ScreenHeight);
	SDL_SetRenderTarget(GameBase->Game->Renderer, image);
	SDL_SetRenderDrawColor(GameBase->Game->Renderer, 0, 0, 0, 255);
	SDL_RenderClear(GameBase->Game->Renderer);
	init();

	Draw();

	SDL_RenderPresent(GameBase->Game->Renderer);
	SDL_SetRenderTarget(GameBase->Game->Renderer, prev);
	deinit();
	return image;
}

//Update ----------------------------------------------------------------------------------------------------------------

void CGameBreakOut::UpdateObjects(bool IsGameState)
{
	updateblocks();
	if (IsGameState)
	{
		updateplayer();
		updateball();
	}
}

void CGameBreakOut::UpdateLogic()
{
	GameBase->UpdateLogic();
	UpdateObjects(GameBase->Game->SubGameState == SGGame);
	if(GameBase->Game->SubGameState == SGGame)
		GameBase->Game->Sprites->UpdateSprites(GameBase->Game->Renderer);
}
