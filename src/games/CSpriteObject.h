#ifndef CSPRITEOBJECT_H
#define CSPRITEOBJECT_H

#include <SDL.h>
#include "../CSprites.h"
#include <stdbool.h>
#include "../Vec2F.h"
struct CSpriteObject
{
		bool alive;
		Vec2F vel;
		Vec2F pos;
		SDL_Point tz;
		CSprite* spr;
		int healthpoints;
		int freeze;
		int state;
		int stateticks;
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
typedef struct CSpriteObject CSpriteObject;

void Initialize_CSpriteObject(CSpriteObject *SpriteObject);
#endif