#include <SDL.h>
#include <string.h>
#include "CGame.h"
#include "Common.h"
#include "Intro.h"
#include "CTween.h"

float alpha = 0.0f;
int maxticks = 240;
float alphastep = 1.0f/240.0f;
float archalpha = 0;
int scene = 0;
int sceneticks = 0;
int tsize = 70;
SDL_Texture* screenshot;
		

void Intro(CGame *Game)
{
    if(Game->GameState == GSIntroInit)
    {
        Game->GameState -= initDiff;
    }

    SDL_Color ClWhite = {255, 255, 255, 255};
    string s;
    int w,h;

    SDL_SetRenderTarget(Game->Renderer, Game->TexTmp);
	SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Game->Renderer);
    if (scene == 0)
    {
        s = "Willems Davy aka Joyrider3774";
        w =  Game->Font->TextWidth("Roboto-Regular", tsize, s, s.length());
        h = tsize*2;        
        Game->Font->WriteText(Game->Renderer, "Roboto-Regular", tsize, s, s.length(), (ScreenWidth - w) / 2, (ScreenHeight - h) /2, tsize, ClWhite);
        
        s = "presents";
        w =  Game->Font->TextWidth("Roboto-Regular", tsize, s, s.length());
        h = tsize;
        Game->Font->WriteText(Game->Renderer, "Roboto-Regular", tsize, s, s.length(), (ScreenWidth - w) / 2, (ScreenHeight) /2, tsize, ClWhite);
    }

    if (scene == 1)
    {
        s = "Retro Time";
        w =  Game->Font->TextWidth("Roboto-Regular", tsize, s, s.length());
        Game->Font->WriteText(Game->Renderer, "Roboto-Regular", tsize, s, s.length(), (ScreenWidth - w) / 2, 40, tsize*2, ClWhite);

        s = "A game containing";
        w =  Game->Font->TextWidth("Roboto-Regular", tsize, s, s.length());
        Game->Font->WriteText(Game->Renderer, "Roboto-Regular", tsize, s, s.length(),(ScreenWidth - w) / 2, (ScreenHeight - 3 * (tsize + 10)) /2, tsize, ClWhite);

        s = to_string(Games) + " retro based games";
        w =  Game->Font->TextWidth("Roboto-Regular", tsize, s, s.length());
        Game->Font->WriteText(Game->Renderer, "Roboto-Regular", tsize, s, s.length(), (ScreenWidth - w) / 2, (ScreenHeight - 1 * (tsize + 10)) / 2, tsize, ClWhite);
        
        s = "playable in " + to_string(Modes) + " game modes";
        w =  Game->Font->TextWidth("Roboto-Regular", tsize, s, s.length());
        Game->Font->WriteText(Game->Renderer, "Roboto-Regular", tsize, s, s.length(), (ScreenWidth - w) / 2, (ScreenHeight + 1 * (tsize + 10)) /2, tsize, ClWhite);
    }

    if (scene >= 2)
    {
        tsize = 30;

        int sheight = (0.75 * ScreenHeight);
        int swidth = (0.75 * ScreenWidth);
        int sy = (ScreenHeight - sheight) / 2 - tsize - 15;
        SDL_Rect DstRect = {(ScreenWidth - swidth) / 2  , sy, swidth, sheight};
        Game->Image->DrawImage(Game->Renderer, Game->GameScreenShots[scene -2], NULL, &DstRect);

        s = string(GSGames[scene -2].name) + " " + string(GSGames[scene -2].shortdescription);
        w =  Game->Font->TextWidth("Roboto-Regular", tsize, s, s.length());
        Game->Font->WriteText(Game->Renderer, "Roboto-Regular", tsize, s, s.length(), (ScreenWidth - w) / 2, sy + sheight + 15 + tsize/2, tsize, ClWhite);
    }

    SDL_SetRenderTarget(Game->Renderer, Game->TexOffScreen);
    //SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Game->Renderer);
    SDL_Point Pos = {0,0};
    SDL_FPoint Scale = {1,1};
    Game->Image->DrawImageFuzeTintFloat(Game->Renderer, Game->TexTmp, false, &Pos, 0, &Scale, 1, 1, 1, archalpha);
    sceneticks += 1;
            
    if (sceneticks >= maxticks)
    {
        alpha = 0;
        scene += 1;
        sceneticks = 0;
    }
    
    if (Game->Input->Buttons.ButA || Game->Input->Buttons.ButB || Game->Input->Buttons.ButBack || (scene -2 >= Games))
        Game->GameState = GSTitleScreenInit;

    if (alpha < 1)
        alpha += alphastep;
    else
        alpha = 1;

    archalpha = arch(smoothstep3(alpha));
}