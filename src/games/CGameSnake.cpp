#include <SDL.h>
#include "CGameSnake.h"

CGameSnake::CGameSnake(CGame* aGame, bool aScreenshotMode)
{
	GameBase = new CGameBase(aGame, GSRamIt, true, aScreenshotMode);
	MusMusic = -1;
	SfxFood = -1;
	SfxDie = -1;
	GameBase->playfieldwidth = (cols) * snakesize;
	GameBase->playfieldheight = (rows) * snakesize;
	GameBase->screenleft = (ScreenWidth - GameBase->playfieldwidth) / 2;
	GameBase->screenright = GameBase->screenleft + GameBase->playfieldwidth;
	GameBase->screentop = (ScreenHeight - GameBase->playfieldheight) / 2;
	GameBase->screenbottom = GameBase->screentop + GameBase->playfieldheight;
}

CGameSnake::~CGameSnake()
{

}

//food ----------------------------------------------------------------------------------------------------------------

void CGameSnake::createfood()
{
	bool bok = false;
	while (!bok)
	{
		food = {GameBase->screenleft + int((rand() % cols) *snakesize),GameBase->screentop + int((rand() % rows) * snakesize)};
		bok = ((food.x != head.x) && (food.y != head.y));
		for (int i = 0; i < snakelength; i++)
			bok = bok && ((food.x != body[i].x) && (food.y != body[i].y));
	}
}

void CGameSnake::drawfood()
{
	SDL_SetRenderDrawColor(GameBase->Game->Renderer, snakefoodcolor.r, snakefoodcolor.g, snakefoodcolor.b, snakefoodcolor.a);
	SDL_Rect r = {food.x, food.y, snakesize, snakesize};
	SDL_RenderFillRect(GameBase->Game->Renderer, &r);
}

void CGameSnake::updatefood()
{
	if ((head.x == food.x) && (head.y == food.y))
	{
		GameBase->Game->Audio->PlaySound(SfxFood, 0);
		snakelength += 1;
		createfood();
		if(!GameBase->ScreenshotMode)
			GameBase->Game->AddToScore(snakelength*2);
	}
}

//snake ----------------------------------------------------------------------------------------------------------------

void CGameSnake::createsnake()
{
	playerdeath = false;
	snakelength = 0;
	head = {GameBase->screenleft + int(cols / 2) * snakesize,GameBase->screentop + int(rows / 2) * snakesize};
	dir = {1,0};
	ticks = 0;
}

void CGameSnake::drawsnake()
{
	SDL_Rect r;
	SDL_SetRenderDrawColor(GameBase->Game->Renderer, snakebodycolor.r, snakebodycolor.g, snakebodycolor.b, snakebodycolor.a);
	for (int i = 0; i < snakelength; i++)
	{
		r = {body[i].x, body[i].y, snakesize, snakesize};
		SDL_RenderFillRect(GameBase->Game->Renderer, &r);
	}
	SDL_SetRenderDrawColor(GameBase->Game->Renderer, snakeheadcolor.r, snakeheadcolor.g, snakeheadcolor.b, snakeheadcolor.a);
	r = {head.x, head.y, snakesize, snakesize};
	SDL_RenderFillRect(GameBase->Game->Renderer, &r);
}

void CGameSnake::updatesnake()
{
	if(!GameBase->ScreenshotMode)
	{
		if ((GameBase->Game->Input->Buttons.ButLeft) ||
			(GameBase->Game->Input->Buttons.ButLeft2) ||
			(GameBase->Game->Input->Buttons.ButDpadLeft))
		{
			if(movedone && dir.x == 0)
			{
				movedone = false;
				dir = {-1,0};
			}
		}
		else
		{
			if ((GameBase->Game->Input->Buttons.ButRight) ||
				(GameBase->Game->Input->Buttons.ButRight2) ||
				(GameBase->Game->Input->Buttons.ButDpadRight))
			{
				if(movedone && dir.x == 0)
				{
					movedone = false;
					dir = {1,0};
				}
			}
			else
			{
				if ((GameBase->Game->Input->Buttons.ButUp) ||
					(GameBase->Game->Input->Buttons.ButUp2) ||
					(GameBase->Game->Input->Buttons.ButDpadUp))
				{
					if(movedone && dir.y == 0)
					{
						movedone = false;
						dir = {0,-1};
					}
				}
				else
				{
					if ((GameBase->Game->Input->Buttons.ButDown) ||
						(GameBase->Game->Input->Buttons.ButDown2) ||
						(GameBase->Game->Input->Buttons.ButDpadDown))
					{
						if(movedone && dir.y == 0)
						{
							movedone = false;
							dir = {0,1};
						}
					}
				}
			}
		}
	}

	ticks += 1;
	if(GameBase->ScreenshotMode || (ticks >= updateticks))
	{
		movedone = true;
		ticks = 0;
		int i = snakelength;
		while (i >= 1)
		{
			body[i] = body[i-1];
			i -= 1;
		}
		body[0] = head;
		head.x += dir.x * snakesize;
		head.y += dir.y * snakesize;
		if((head.x < GameBase->screenleft) || (head.x >= GameBase->screenright) ||
			(head.y < GameBase->screentop) || (head.y >= GameBase->screenbottom))
			playerdeath = true;

		for (int i = 0; i < snakelength; i++)
			if ((head.x == body[i].x) && (head.y == body[i].y))
				playerdeath = true;
	}
}

//background ----------------------------------------------------------------------------------------------------------------

void CGameSnake::DrawBackground(bool motionblur)
{
	SDL_SetRenderDrawColor(GameBase->Game->Renderer, 0x65, 0x65, 0xFF, 0xFF);
	SDL_RenderClear(GameBase->Game->Renderer);
	SDL_SetRenderDrawColor(GameBase->Game->Renderer, 0, 0, 0, 255);
	SDL_Rect r = {GameBase->screenleft, GameBase->screentop, GameBase->playfieldwidth, GameBase->playfieldheight};
	SDL_RenderFillRect(GameBase->Game->Renderer, &r);
}

SDL_Texture* CGameSnake::screenshot()
{
	SDL_Texture* prev = SDL_GetRenderTarget(GameBase->Game->Renderer);
	SDL_Texture* image = SDL_CreateTexture(GameBase->Game->Renderer, PixelFormat, SDL_TEXTUREACCESS_TARGET, ScreenWidth, ScreenHeight);
	SDL_SetRenderTarget(GameBase->Game->Renderer, image);
	SDL_SetRenderDrawColor(GameBase->Game->Renderer, 0, 0, 0, 255);
	SDL_RenderClear(GameBase->Game->Renderer);
	init();

	for (int i = 0; i < 7; i++)
	{
		updatesnake();
		snakelength += 1;
	}
	food = {GameBase->screenleft + (int(cols / 2)-2) * snakesize, GameBase->screentop + (int(rows / 2) -2) * snakesize};

	Draw();

	SDL_RenderPresent(GameBase->Game->Renderer);
	SDL_SetRenderTarget(GameBase->Game->Renderer, prev);
	deinit();
	return image;
}

//init - deinit ----------------------------------------------------------------------------------------------------------------

void CGameSnake::init()
{

	createsnake();
	createfood();

	movedone = true;
	if (!GameBase->ScreenshotMode)
	{
		GameBase->HealthPoints = 2;
		LoadSound();
		GameBase->Game->CurrentGameMusicID = MusMusic;
		GameBase->Game->Audio->PlayMusic(MusMusic, -1);
	}
}

void CGameSnake::deinit()
{
	if (!GameBase->ScreenshotMode)
	{
		UnLoadSound();
		GameBase->Game->SubStateCounter = 0;
		GameBase->Game->SubGameState = SGNone;
		GameBase->Game->CurrentGameMusicID = -1;
	}
}

void CGameSnake::LoadSound()
{
	SfxFood = GameBase->Game->Audio->LoadSound("snakey/food.wav");
	SfxDie = GameBase->Game->Audio->LoadSound("common/die.wav");
	MusMusic = GameBase->Game->Audio->LoadMusic("snakey/music.ogg");
}

void CGameSnake::UnLoadSound()
{
	GameBase->Game->Audio->StopMusic();
	GameBase->Game->Audio->StopSound();
	GameBase->Game->Audio->UnLoadMusic(MusMusic);
	GameBase->Game->Audio->UnLoadSound(SfxFood);
	GameBase->Game->Audio->UnLoadSound(SfxDie);
}

//Update ----------------------------------------------------------------------------------------------------------------

void CGameSnake::UpdateObjects(bool IsGameState)
{
	if (IsGameState)
	{
		updatesnake();
		updatefood();
	}

	if (IsGameState && playerdeath)
	{
		GameBase->Game->Audio->PlaySound(SfxDie, 0);
		if(!GameBase->ScreenshotMode)
			GameBase->Game->AddToScore(-50);

		if (GameBase->HealthPoints > 1)
		{
			createsnake();
			createfood();
			if (GameBase->Game->GameMode == GMGame)
				GameBase->HealthPoints -= 1;
			GameBase->Game->SubGameState = SGReadyGo;
			GameBase->Game->SubStateTime = SDL_GetTicks() + 500;
		}
		else
			if(GameBase->Game->GameMode == GMGame)
				if (!GameBase->ScreenshotMode)
					GameBase->HealthPoints -= 1;
	}
}

void CGameSnake::UpdateLogic()
{
	GameBase->UpdateLogic();
	UpdateObjects(GameBase->Game->SubGameState == SGGame);
	if(GameBase->Game->SubGameState == SGGame)
		GameBase->Game->Sprites->UpdateSprites(GameBase->Game->Renderer);
}


bool CGameSnake::DrawObjects()
{
	drawfood();
	drawsnake();
	//don't call drawsprites in base object
	return false;
}

void CGameSnake::Draw()
{
	DrawBackground((GameBase->Game->SubGameState == SGGame) && !GameBase->ScreenshotMode);
	if (DrawObjects())
		GameBase->Game->Sprites->DrawSprites(GameBase->Game->Renderer);
	if(!GameBase->ScreenshotMode)
	{
		GameBase->DrawScoreBar();
		GameBase->DrawSubStateText();
	}
}