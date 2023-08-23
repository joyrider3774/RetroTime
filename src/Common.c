#include <SDL.h>
#include <string.h>
#include "Common.h"
#include <stdbool.h>

//Modes
ModeDesc GMModes[Modes] = {
	{
		GMRetroCarousel,
		"Retro Carousel",
		"In Retro Carousel mode you will play all games for 2 minutes\n"
		"after each other. You can not die by losing lives. Points will\n"
		"be deducted from the score when dying. The aim is to get the\n"
		"highest score, accumulated over all the games"
	},
	{
		GMGame,
		"Lives Mode",
		"In Lives mode you play a single game and start with a fixed\n"
		"amount of lives. Dying will lose you one live. The game is\n"
		"over if the lives reaches 0. The aim of the game is to get\n"
		"the highest score possible without dying."
	},
	{
		GMTimeAttack,
		"Time Attack",
		"In Time Attack mode you play a single game for 5 minutes.\n"
		"You can not die by losing lives. Points will be deducted\n"
		"from the score when dying. The aim is to get the highest\n"
		"score during these 5 minutes of playing."
	}
};

//GameDescriptions
GameDesc GSGames[Games] = {
	{
		GSSpaceInvadersInit,//GSSpaceInvaders,
		"Invaders",
		"Invaders is a game based on Space Invaders. Rows of enemies\n"
		"are closing in you. You have to shoot them down before they\n"
		"reach your mining asteroids. Shooting down enemies gains\n"
		"points, shooting down all enemies gains extra points\n"
		"You can move with dpad and shoot with (A).\n"
		"If enemies reach your mining asteroids you will lose a life.\n"
		"Avoid being shot or you will also lose a life.",
		"a game based on Space Invaders (Arcade)"
	},
	{
		GSBreakoutInit, //gsbreakout,
		"Brick Breaker",
		"Brick Breaker is a game based on Breakout. Rows of blocks\n"
		"have to be destroyed by touching them with the ball.\n"
		"You control the paddle at the bottom of the screen with\n"
		"the dpad. Keeping (A) pressed speeds up the paddle.\n"
		"You have to let the ball bounce of your paddle to\n"
		"send it back in the playfield. Destroying blocks gains points.\n"
		"Fail to bounce back the ball and you lose a life",
		"a game based on Breakout (Arcade)"

	},
	{
		GSFrogInit,//gsfrog,
		"Toady",
		"Toady is a game based on Frogger. You control the robot\n"
		"by the dpad and you have to keep moving up.\n"
		"Each time you move up to a place you had not reached yet,\n"
		"you will gain points. Avoid getting hit by traffic, don't\n"
		"fall into water, don't exceed the edges of the playfield\n"
		"or you will lose a life. Collect diffrent fruit to gain\n"
		"extra score.",
		"a game based on Frogger (Arcade)"
	},
	{
		GSSnakeInit,//gssnake,
		"Snakey",
		"Snakey is a game based on well snake. You control a snake's\n"
		"head (red block) by the dpad. The snake\n"
		"can move in four directions and wants food (green blocks).\n"
		"Each time you eat food the snake's body will grow. Eat\n"
		"food to gain points, the longer your snake is to more\n"
		"points you will gain. Touching the snake's body or going\n"
		"outside the playfield will lose you a life.",
		"a game based on Snake (Nokia 3310)",
	},
	{
		GSPangInit,//gspang,
		"Bubble Buster",
		"Bubble Buster is a game based on Pang. You control a player\n"
		"that needs to pop bubbles. You can control the player using the\n"
		"dpad and shoot at bubbles using (A) button. If you hit a\n"
		"bubble it will split up into smaller bubbles and you gain points. Get\n"
		"hit by a bubble and you will lose a life, making you temporary\n"
		"invincible but you can not shoot during this time. Clearing all\n"
		"bubbles gives you extra points.",
		"a game based on Pang (Arcade)",
	},
	{
		GSTetrisInit,
		"Block Stacker",
		"Block Stacker is a game of tetris. You control the blocks\n"
		"with the dpad. You need to stack them in such a manner that\n"
		"they form complete rows. Once this is done that row will be\n"
		"removed. Speed increases gradually and having no place at\n"
		"the top to place a block will lose you a life. You can rotate\n"
		"blocks using (A) and quick drop them using (B).",
		"a game based on Tetris"
	},
	{
		GSRamItInit,
		"Color Invasion",
		"Color Invasion is a game based on Ram-It. You control the\n"
		"cannon with the dpad. You need to shoot the invading\n"
		"colored bars completely of the screen using (A). Points\n"
		"are awarded when the colored bar is completely of the\n"
		"screen, the closer a bar had gotten to the cannon the more\n"
		"points you will gain. You also gain extra points for clearing\n"
		"all bars from the screen. When a colored bar reaches the\n"
		"cannon rail you will lose a life.",
		"a game based on Ram-It (Atari 2600)"
	},
	{
		GSEddyInit,//gseddy,
		"Faster Dave",
		"Faster Dave is a game based on Fast Eddie. You control Dave with\n"
		"dpad. You need to avoid or jump over enemies with the (A)\n"
		"button while collecting the magic balls. When you collect 9\n"
		"balls the end level key is reachable. You are safe on ladders. Points\n"
		"are received for collecting magic balls and getting the end level\n"
		"key. Collecting the 10th magic ball is not required. When you touch\n"
		"an enemy you will a lose life. Every 5 levels, level layout changes.",
		"a game based on Fast Eddie (Atari 2600)"
	}
};

//PauseMenus
//order must equal order from pmvalues !!!
PauseMenuDesc PMPauseMenus[PauseMenus] =
{
	{PMContinue,	"Continue"},
	{PMQuit,		"Quit"},
	{PMMusicVol,	"Music Volume: "},
	{PMSoundVol,	"Sound Volume: "},
	{PMGameHelp,	"Game Help"},
};

//GamePauseMenus
GPGamePauseMenusDesc GPGamePauseMenus[Games] = {
	{GSSpaceInvaders,	{PMContinue, PMGameHelp, PMMusicVol, PMSoundVol, PMQuit}},
	{GSBreakout,		{PMContinue, PMGameHelp, PMMusicVol, PMSoundVol, PMQuit}},
	{GSFrog,			{PMContinue, PMGameHelp, PMMusicVol, PMSoundVol, PMQuit}},
	{GSSnake,			{PMContinue, PMGameHelp, PMMusicVol, PMSoundVol, PMQuit}},
	{GSPang,			{PMContinue, PMGameHelp, PMMusicVol, PMSoundVol, PMQuit}},
	{GSTetris,			{PMContinue, PMGameHelp, PMMusicVol, PMSoundVol, PMQuit}},
	{GSRamIt,			{PMContinue, PMGameHelp, PMMusicVol, PMSoundVol, PMQuit}},
	{GSEddy,			{PMContinue, PMGameHelp, PMMusicVol, PMSoundVol, PMQuit}}
};

//MainMenus
MainMenusDesc MMMainMenus[MainMenus] =
{
	{MMStart,				"Start Game"},
	{MMHighScores,	"High Scores"},
	{MMOptions,				"Options"},
	{MMHelp,				"Help"},
	{MMCredits,				"Credits"},
	{MMQuit,				"Quit"}
};

//OptionsMenus
OptionMenusDesc OMOptionMenus[OptionMenus] =
{
	{OMMusicVol,		"Music Volume: "},
	{OMSoundVol,		"Sound Volume: "},
	{OMResetHighScores, "Reset Highscores"},
	{OMBack,			"Back"},
};


Uint32 ColorToUint(int R, int G, int B)
{
	return (Uint32)((R << 16) + (G << 8) + (B << 0));
}

SDL_Color UintToColour(Uint32 colour)
{
	SDL_Color tempcol;
	tempcol.a = 255;
	tempcol.r = (colour >> 16) & 0xFF;
	tempcol.g = (colour >> 8) & 0xFF;
	tempcol.b = colour & 0xFF;
	return tempcol;
}




void SDL_PutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

/*
 * Return the pixel value at (x, y)
 * NOTE: The surface must be locked before calling this!
 */
Uint32 SDL_GetPixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

void ditherSurface(SDL_Surface* Surface, SDL_Rect* Rect, uint8_t bayerver, uint8_t whiteThreasHold)
{

	static const uint8_t bayer2[2][2] = {
		{0, 3},
		{2, 1}
	};

	static const uint8_t bayer4[4][4] = {
		{0, 8, 2 ,10},
		{12, 4, 14, 6},
		{3, 11, 1, 9},
		{15, 7, 13, 5}
	};

	static const uint8_t bayer8[8][8] = {
		{0, 32, 8, 40 ,2 ,34 ,10 , 42},
		{48, 16, 56, 24, 50, 18, 58, 26},
		{12, 44, 4, 36, 14, 46, 6, 38},
		{60, 28, 52, 20, 62, 30, 54, 22},
		{3, 35, 11, 43, 1, 33, 9, 41},
		{51, 19, 59, 27, 49, 17, 57, 25},
		{15, 47, 7, 39, 13, 45, 5, 37},
		{63, 31, 55, 23, 61, 29, 53, 21}
	};
	int x, y;


	SDL_Rect tmp;
	tmp.x = 0;
	tmp.y = 0;
	tmp.w = Surface->w;
	tmp.h = Surface->h;
	if(Rect)
	{
		tmp.x = Rect->x;
		tmp.y = Rect->y;
		tmp.w = Rect->w;
		tmp.h = Rect->h;
	}
	for(y = tmp.y; y < tmp.h; ++y) 
	{
		for(x = tmp.x; x < tmp.w; ++x) 
		{
			uint32_t pix = SDL_GetPixel(Surface, x, y);
			uint8_t r, g, b, a;
			SDL_GetRGBA(pix, Surface->format, &r, &g, &b, &a);
			

			// Convert the pixel value to grayscale i.e. intensity
			float lum = 0.212671f  *r + 0.715160f  * g + 0.072169f  *b;

			if (bayerver >= 8)
			{
				lum = lum + lum* bayer8[y % 8][x % 8] / 64;
			}
			else
			{
				if (bayerver >= 4)
				{
					lum = lum + lum* bayer4[y % 4][x % 4] / 16;
				}
				else if (bayerver >=1)
				{
					lum = lum + lum* bayer2[y % 2][x % 2] / 4;
				}
			}

			if(lum >=  whiteThreasHold)
				lum = 255;
			else
				lum = 0;

			// Put the pixel back in the image
			SDL_PutPixel(Surface, x, y, SDL_MapRGBA(Surface->format, lum, lum, lum, a));

		}
	}
}

void ditherTarget(SDL_Renderer* aRenderer, SDL_Texture *Tex, SDL_Rect* Rect, uint8_t bayerver, uint8_t whiteThreasHold)
{
    SDL_Texture* tmptarget = SDL_GetRenderTarget(aRenderer);
	SDL_SetRenderTarget(aRenderer, Tex);
	SDL_Texture* streamingTexture = NULL;
    int w = ScreenWidth;
	int h = ScreenHeight;
	uint32_t f = PixelFormat;
	if (Tex != NULL)
		SDL_QueryTexture(Tex, &f, NULL, &w, &h);
	SDL_Rect tmp;
	tmp.x = 0;
	tmp.y = 0;
	tmp.w = w;
	tmp.h = h;
	if(Rect)
	{
		tmp.x = Rect->x;
		tmp.y = Rect->y;
		tmp.w = Rect->w;
		tmp.h = Rect->h;
	}

	//get Streaming Texture from drawTarget
    streamingTexture = SDL_CreateTexture( aRenderer, f, SDL_TEXTUREACCESS_STREAMING, tmp.w, tmp.h );
    //SDL_SetTextureBlendMode(streamingTextureDrawTarget, SDL_BLENDMODE_BLEND);
    void* streamingPixelsDrawTarget;
    int streamingPitchDrawTarget;
    SDL_LockTexture(streamingTexture, NULL, &streamingPixelsDrawTarget, &streamingPitchDrawTarget );
    SDL_RenderReadPixels(aRenderer, &tmp, f, streamingPixelsDrawTarget, streamingPitchDrawTarget);
    SDL_UnlockTexture(streamingTexture);
	SDL_Surface *Surface;
	SDL_LockTextureToSurface(streamingTexture, NULL, &Surface);

	ditherSurface(Surface, NULL, bayerver, whiteThreasHold);

	SDL_UnlockTexture(streamingTexture);

	SDL_SetRenderTarget(aRenderer, Tex);
	SDL_RenderCopy(aRenderer, streamingTexture, NULL, Rect);
	SDL_DestroyTexture(streamingTexture);
	SDL_SetRenderTarget(aRenderer, tmptarget);
}

// void SDL_SaveBMPTextureScaled(SDL_Renderer *Renderer, const char* File, SDL_Texture* Tex, float ScaleX, float ScaleY, bool ApplyDither, int ditherversion, int whiteThresHold)
// {
// 	SDL_SaveBMPTextureScaled(Renderer, File, Tex,ScaleX, ScaleY, ApplyDither, ditherversion, whiteThresHold);
// }

void SDL_SaveBMPTextureScaled(SDL_Renderer *Renderer, const char *File, SDL_Texture* Tex, float ScaleX, float ScaleY, bool ApplyDither, int ditherversion, int whiteThresHold)
{
	if (!Renderer)
		return;
	
	if (!Tex)
		return;

	
	int w,h;
	uint32_t f;
	SDL_QueryTexture(Tex, &f, NULL, &w, &h);
	if(((int)ceil(w * ScaleX) <= 0) || ((int)ceil(h * ScaleY) <= 0))
		return;
	
	
	SDL_Texture *Tmp = SDL_CreateTexture(Renderer, f, SDL_TEXTUREACCESS_TARGET, (int)ceil(w * ScaleX), (int)ceil(h * ScaleY));
	SDL_Texture *TmpR = SDL_GetRenderTarget(Renderer);
	
	SDL_SetRenderTarget(Renderer, Tmp);

	//clear with transparant color
	uint8_t r,g,b,a;
	SDL_GetRenderDrawColor(Renderer, &r,&g,&b,&a);
	SDL_SetRenderDrawColor(Renderer,0,0,0,0);
	SDL_RenderClear(Renderer);
	SDL_SetRenderDrawColor(Renderer, r,g,b,a);

	
	SDL_Rect Dst;
	Dst.x = 0;
	Dst.y = 0;
	Dst.w = (int)ceil(w * ScaleX);
	Dst.h = (int)ceil(h * ScaleY);
	SDL_RenderCopy(Renderer, Tex, NULL, &Dst);
    SDL_Texture* streamingTexture = SDL_CreateTexture(Renderer, f, SDL_TEXTUREACCESS_STREAMING, Dst.w, Dst.h );
    
	void* streamingPixelsDrawTarget;
    int streamingPitchDrawTarget;
    SDL_LockTexture(streamingTexture, NULL, &streamingPixelsDrawTarget, &streamingPitchDrawTarget );
    SDL_RenderReadPixels(Renderer, NULL, f, streamingPixelsDrawTarget, streamingPitchDrawTarget);
    SDL_UnlockTexture(streamingTexture);
	SDL_Surface *Surface;
	SDL_LockTextureToSurface(streamingTexture, NULL, &Surface);

	if (ApplyDither)
		ditherSurface(Surface, NULL, ditherversion, whiteThresHold);
	SDL_SaveBMP(Surface, File);

	SDL_UnlockTexture(streamingTexture);

	
	SDL_DestroyTexture(streamingTexture);
	SDL_DestroyTexture(Tmp);
	SDL_SetRenderTarget(Renderer, TmpR);
}