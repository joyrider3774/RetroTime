#pragma once

#include <SDL.h>
#include "../CSprites.h"

class CSpriteObject
{
public:
        bool alive = false;
        SDL_FPoint vel = {0,0};
        SDL_FPoint pos = {0,0};
        SDL_Point tz = {0,0};
        CSprite* spr = nullptr;
        int healthpoints = 0;
        int freeze = 0;
        int state = 0;
        int stateticks = 0;
        //used in toad
        int rowtype;
        int id;
        //used in pang
        float force;
        float speed;
        float curforce;
        //used in fast eddy
        int floory;
        int jumpdown;
        int group;
        int row;
};
