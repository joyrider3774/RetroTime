#include <SDL.h>
#include "CGameSnake.h"

CGameSnake* Create_CGameSnake(CGame* aGame)
{
	CGameSnake* GameSnake = (CGameSnake*) malloc(sizeof(CGameSnake));
	GameSnake->GameBase = Create_CGameBase(aGame, GSRamIt, true);
	GameSnake->MusMusic = -1;
	GameSnake->SfxFood = -1;
	GameSnake->SfxDie = -1;
	GameSnake->GameBase->playfieldwidth = (GameSnake->cols) * GameSnake->snakesize;
	GameSnake->GameBase->playfieldheight = (GameSnake->rows) * GameSnake->snakesize;
	GameSnake->GameBase->screenleft = (ScreenWidth - GameSnake->GameBase->playfieldwidth) / 2;
	GameSnake->GameBase->screenright = GameSnake->GameBase->screenleft + GameSnake->GameBase->playfieldwidth;
	GameSnake->GameBase->screentop = (ScreenHeight - GameSnake->GameBase->playfieldheight) / 2;
	GameSnake->GameBase->screenbottom = GameSnake->GameBase->screentop + GameSnake->GameBase->playfieldheight;

	
	GameSnake->createfood = CGameSnake_createfood;
	GameSnake->drawfood = CGameSnake_drawfood;
	GameSnake->updatefood = CGameSnake_updatefood;

	GameSnake->createsnake = CGameSnake_createsnake;
	GameSnake->drawsnake = CGameSnake_drawsnake;
	GameSnake->updatesnake = CGameSnake_updatesnake;

	GameSnake->init = CGameSnake_init;
	GameSnake->deinit = CGameSnake_deinit;
	GameSnake->LoadSound = CGameSnake_LoadSound;
	GameSnake->UnLoadSound = CGameSnake_UnLoadSound;
	GameSnake->UpdateObjects = CGameSnake_UpdateObjects;
	GameSnake->DrawObjects = CGameSnake_DrawObjects;
	GameSnake->DrawBackground = CGameSnake_DrawBackground;
	GameSnake->Draw = CGameSnake_Draw;
	GameSnake->UpdateLogic = CGameSnake_UpdateLogic;

	return GameSnake;
}

void Destroy_CGameSnake(CGameSnake* GameSnake)
{
	Destroy_CGameBase(GameSnake->GameBase);
	free(GameSnake);
}

//GameSnake->food ----------------------------------------------------------------------------------------------------------------

void CGameSnake_createfood(CGameSnake* GameSnake)
{
	bool bok = false;
	while (!bok)
	{
		GameSnake->food = {GameSnake->GameBase->screenleft + int((rand() % GameSnake->cols) *GameSnake->snakesize),GameSnake->GameBase->screentop + int((rand() % GameSnake->rows) * GameSnake->snakesize)};
		bok = ((GameSnake->food.x != GameSnake->head.x) && (GameSnake->food.y != GameSnake->head.y));
		for (int i = 0; i < GameSnake->snakelength; i++)
			bok = bok && ((GameSnake->food.x != GameSnake->body[i].x) && (GameSnake->food.y != GameSnake->body[i].y));
	}
}

void CGameSnake_drawfood(CGameSnake* GameSnake)
{
	SDL_SetRenderDrawColor(GameSnake->GameBase->Game->Renderer, GameSnake->snakefoodcolor.r, GameSnake->snakefoodcolor.g, GameSnake->snakefoodcolor.b, GameSnake->snakefoodcolor.a);
	SDL_Rect r = {GameSnake->food.x, GameSnake->food.y, GameSnake->snakesize, GameSnake->snakesize};
	SDL_RenderFillRect(GameSnake->GameBase->Game->Renderer, &r);
}

void CGameSnake_updatefood(CGameSnake* GameSnake)
{
	if ((GameSnake->head.x == GameSnake->food.x) && (GameSnake->head.y == GameSnake->food.y))
	{
		GameSnake->GameBase->Game->Audio->PlaySound(GameSnake->SfxFood, 0);
		GameSnake->snakelength += 1;
		GameSnake->createfood(GameSnake);
		GameSnake->GameBase->Game->AddToScore(GameSnake->snakelength*2);
	}
}

//snake ----------------------------------------------------------------------------------------------------------------

void CGameSnake_createsnake(CGameSnake* GameSnake)
{
	GameSnake->playerdeath = false;
	GameSnake->snakelength = 0;
	GameSnake->head = {GameSnake->GameBase->screenleft + int(GameSnake->cols / 2) * GameSnake->snakesize,GameSnake->GameBase->screentop + int(GameSnake->rows / 2) * GameSnake->snakesize};
	GameSnake->dir = {1,0};
	GameSnake->ticks = 0;
}

void CGameSnake_drawsnake(CGameSnake* GameSnake)
{
	SDL_Rect r;
	SDL_SetRenderDrawColor(GameSnake->GameBase->Game->Renderer, GameSnake->snakebodycolor.r, GameSnake->snakebodycolor.g, GameSnake->snakebodycolor.b, GameSnake->snakebodycolor.a);
	for (int i = 0; i < GameSnake->snakelength; i++)
	{
		r = {GameSnake->body[i].x, GameSnake->body[i].y, GameSnake->snakesize, GameSnake->snakesize};
		SDL_RenderFillRect(GameSnake->GameBase->Game->Renderer, &r);
	}
	SDL_SetRenderDrawColor(GameSnake->GameBase->Game->Renderer, GameSnake->snakeheadcolor.r, GameSnake->snakeheadcolor.g, GameSnake->snakeheadcolor.b, GameSnake->snakeheadcolor.a);
	r = {GameSnake->head.x, GameSnake->head.y, GameSnake->snakesize, GameSnake->snakesize};
	SDL_RenderFillRect(GameSnake->GameBase->Game->Renderer, &r);
}

void CGameSnake_updatesnake(CGameSnake* GameSnake)
{
	if ((GameSnake->GameBase->Game->Input->Buttons.ButLeft) ||
		(GameSnake->GameBase->Game->Input->Buttons.ButLeft2) ||
		(GameSnake->GameBase->Game->Input->Buttons.ButDpadLeft))
	{
		if(GameSnake->movedone && GameSnake->dir.x == 0)
		{
			GameSnake->movedone = false;
			GameSnake->dir = {-1,0};
		}
	}
	else
	{
		if ((GameSnake->GameBase->Game->Input->Buttons.ButRight) ||
			(GameSnake->GameBase->Game->Input->Buttons.ButRight2) ||
			(GameSnake->GameBase->Game->Input->Buttons.ButDpadRight))
		{
			if(GameSnake->movedone && GameSnake->dir.x == 0)
			{
				GameSnake->movedone = false;
				GameSnake->dir = {1,0};
			}
		}
		else
		{
			if ((GameSnake->GameBase->Game->Input->Buttons.ButUp) ||
				(GameSnake->GameBase->Game->Input->Buttons.ButUp2) ||
				(GameSnake->GameBase->Game->Input->Buttons.ButDpadUp))
			{
				if(GameSnake->movedone && GameSnake->dir.y == 0)
				{
					GameSnake->movedone = false;
					GameSnake->dir = {0,-1};
				}
			}
			else
			{
				if ((GameSnake->GameBase->Game->Input->Buttons.ButDown) ||
					(GameSnake->GameBase->Game->Input->Buttons.ButDown2) ||
					(GameSnake->GameBase->Game->Input->Buttons.ButDpadDown))
				{
					if(GameSnake->movedone && GameSnake->dir.y == 0)
					{
						GameSnake->movedone = false;
						GameSnake->dir = {0,1};
					}
				}
			}
		}
	}

	GameSnake->ticks += 1;

	if(GameSnake->ticks >= GameSnake->updateticks)
	{
		GameSnake->movedone = true;
		GameSnake->ticks = 0;
		int i = GameSnake->snakelength;
		while (i >= 1)
		{
			GameSnake->body[i] = GameSnake->body[i-1];
			i -= 1;
		}
		GameSnake->body[0] = GameSnake->head;
		GameSnake->head.x += GameSnake->dir.x * GameSnake->snakesize;
		GameSnake->head.y += GameSnake->dir.y * GameSnake->snakesize;
		if((GameSnake->head.x < GameSnake->GameBase->screenleft) || (GameSnake->head.x >= GameSnake->GameBase->screenright) ||
			(GameSnake->head.y < GameSnake->GameBase->screentop) || (GameSnake->head.y >= GameSnake->GameBase->screenbottom))
			GameSnake->playerdeath = true;

		for (int i = 0; i < GameSnake->snakelength; i++)
			if ((GameSnake->head.x == GameSnake->body[i].x) && (GameSnake->head.y == GameSnake->body[i].y))
				GameSnake->playerdeath = true;
	}
}

//background ----------------------------------------------------------------------------------------------------------------

void CGameSnake_DrawBackground(CGameSnake* GameSnake)
{
	SDL_SetRenderDrawColor(GameSnake->GameBase->Game->Renderer, 0x65, 0x65, 0xFF, 0xFF);
	SDL_RenderClear(GameSnake->GameBase->Game->Renderer);
	SDL_SetRenderDrawColor(GameSnake->GameBase->Game->Renderer, 0, 0, 0, 255);
	SDL_Rect r = {GameSnake->GameBase->screenleft, GameSnake->GameBase->screentop, GameSnake->GameBase->playfieldwidth, GameSnake->GameBase->playfieldheight};
	SDL_RenderFillRect(GameSnake->GameBase->Game->Renderer, &r);
}

//init - deinit ----------------------------------------------------------------------------------------------------------------

void CGameSnake_init(CGameSnake* GameSnake)
{

	GameSnake->createsnake(GameSnake);
	GameSnake->createfood(GameSnake);

	GameSnake->movedone = true;
	GameSnake->GameBase->HealthPoints = 2;
	GameSnake->LoadSound(GameSnake);
	GameSnake->GameBase->Game->CurrentGameMusicID = GameSnake->MusMusic;
	GameSnake->GameBase->Game->Audio->PlayMusic(GameSnake->MusMusic, -1);
}

void CGameSnake_deinit(CGameSnake* GameSnake)
{
	GameSnake->UnLoadSound(GameSnake);
	GameSnake->GameBase->Game->SubStateCounter = 0;
	GameSnake->GameBase->Game->SubGameState = SGNone;
	GameSnake->GameBase->Game->CurrentGameMusicID = -1;
}

void CGameSnake_LoadSound(CGameSnake* GameSnake)
{
	GameSnake->SfxFood = GameSnake->GameBase->Game->Audio->LoadSound("snakey/food.wav");
	GameSnake->SfxDie = GameSnake->GameBase->Game->Audio->LoadSound("common/die.wav");
	GameSnake->MusMusic = GameSnake->GameBase->Game->Audio->LoadMusic("snakey/music.ogg");
}

void CGameSnake_UnLoadSound(CGameSnake* GameSnake)
{
	GameSnake->GameBase->Game->Audio->StopMusic();
	GameSnake->GameBase->Game->Audio->StopSound();
	GameSnake->GameBase->Game->Audio->UnLoadMusic(GameSnake->MusMusic);
	GameSnake->GameBase->Game->Audio->UnLoadSound(GameSnake->SfxFood);
	GameSnake->GameBase->Game->Audio->UnLoadSound(GameSnake->SfxDie);
}

//Update ----------------------------------------------------------------------------------------------------------------

void CGameSnake_UpdateObjects(CGameSnake* GameSnake, bool IsGameState)
{
	if (IsGameState)
	{
		GameSnake->updatesnake(GameSnake);
		GameSnake->updatefood(GameSnake);
	}

	if (IsGameState && GameSnake->playerdeath)
	{
		GameSnake->GameBase->Game->Audio->PlaySound(GameSnake->SfxDie, 0);
		GameSnake->GameBase->Game->AddToScore(-50);

		if (GameSnake->GameBase->HealthPoints > 1)
		{
			GameSnake->createsnake(GameSnake);
			GameSnake->createfood(GameSnake);
			if (GameSnake->GameBase->Game->GameMode == GMGame)
				GameSnake->GameBase->HealthPoints -= 1;
			GameSnake->GameBase->Game->SubGameState = SGReadyGo;
			GameSnake->GameBase->Game->SubStateTime = SDL_GetTicks() + 500;
		}
		else
			if(GameSnake->GameBase->Game->GameMode == GMGame)
				GameSnake->GameBase->HealthPoints -= 1;
	}
}

void CGameSnake_UpdateLogic(CGameSnake* GameSnake)
{
	GameSnake->GameBase->UpdateLogic(GameSnake->GameBase);
	GameSnake->UpdateObjects(GameSnake, GameSnake->GameBase->Game->SubGameState == SGGame);
	if(GameSnake->GameBase->Game->SubGameState == SGGame)
		GameSnake->GameBase->Game->Sprites->UpdateSprites(GameSnake->GameBase->Game->Renderer);
}


bool CGameSnake_DrawObjects(CGameSnake* GameSnake)
{
	GameSnake->drawfood(GameSnake);
	GameSnake->drawsnake(GameSnake);
	//don't call drawsprites in base object
	return false;
}

void CGameSnake_Draw(CGameSnake* GameSnake)
{
	GameSnake->DrawBackground(GameSnake);
	if (GameSnake->DrawObjects(GameSnake))
		GameSnake->GameBase->Game->Sprites->DrawSprites(GameSnake->GameBase->Game->Renderer);
	GameSnake->GameBase->DrawScoreBar(GameSnake->GameBase);
	GameSnake->GameBase->DrawSubStateText(GameSnake->GameBase);
}