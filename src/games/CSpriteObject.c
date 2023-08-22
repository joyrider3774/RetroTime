#include "CSpriteObject.h"

#include <stdbool.h>

void Initialize_CSpriteObject(CSpriteObject *SpriteObject)
{
	SpriteObject->alive = false;
	SpriteObject->vel.x =0;
	SpriteObject->vel.y =0;
	SpriteObject->pos.x =0;
	SpriteObject->pos.y =0;
	SpriteObject->tz.x = 0;
	SpriteObject->tz.y = 0;
	SpriteObject->spr = NULL;
	SpriteObject->healthpoints = 0;
	SpriteObject->freeze = 0;
	SpriteObject->state = 0;
	SpriteObject->stateticks = 0;
	//used in toad
	SpriteObject->rowtype = 0;
	SpriteObject->id = 0;
	//used in pang
	SpriteObject->force = 0.0f;
	SpriteObject->speed = 0.0f;
	SpriteObject->curforce = 0.0f;
	//used in fast eddy
	SpriteObject->floory = 0;
	SpriteObject->jumpdown = 0;
	SpriteObject->group = 0;
	SpriteObject->row = 0;
}