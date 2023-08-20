#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>
#include "CSprites.h"
#include "Vec2F.h"
#include "Common.h"

const int SPR_Max = 1000;

const int SavedScalingsMax = 1000;

int SavedScalingsCount = 0;
struct SavedScalingsStruct {
	int gfxid;
	Vec2F scale;
};
typedef struct SavedScalingsStruct SavedScalingsStruct;

SavedScalingsStruct CSprites_SavedScalings[SavedScalingsMax];


CSprite* CSprites_Sprites[SPR_Max];
int CSprites_UpdateImageResets;
int CSprites_SpritesDrawn;
bool CSprites_ForceShowCollisionShape;
bool CSprites_needSpriteSorting;

void CSprites_Init()
{
	CSprites_UpdateImageResets = 0;
	CSprites_ForceShowCollisionShape = false;
	CSprites_needSpriteSorting = false;
	CSprites_SpritesDrawn = 0;
	for (int i=0; i < SPR_Max; i++)
	{
		CSprites_Sprites[i] = nullptr;
	}
}

void CSprites_DeInit()
{
	for (int i=0; i < SPR_Max; i++)
	{
		CSprites_RemoveSprite(CSprites_Sprites[i]);
	}
}

void CSprites_SetForceShowCollisionShape(bool val)
{
	CSprites_ForceShowCollisionShape = val;
}

int CSprites_UpdateImageResetsCount()
{
	return  CSprites_UpdateImageResets;
}

int CSprites_SpritesDrawnCount()
{
	return CSprites_SpritesDrawn;
}

CSprite* CSprites_CreateSprite()
{
	for (int i= 0; i < SPR_Max; i++)
	{
		if(CSprites_Sprites[i] == nullptr)
		{
			CSprite* Spr = new CSprite();
			Spr->index = i;
			Spr->animInc = 0;
			Spr->animTimer = 0;
			Spr->rotation = 0;
			Spr->imageID = nullptr;
			Spr->xscale = 1;
			Spr->yscale = 1;
			Spr->prevxscale = -1;
			Spr->prevyscale = -1;
			Spr->xscale_speed = 0;
			Spr->yscale_speed = 0;
			Spr->xspeed = 0;
			Spr->yspeed = 0;
			Spr->tileSizeX = 0;
			Spr->tileSizeY = 0;
			Spr->animStartTile = 0;
			Spr->animEndTile = 0;
			Spr->animSpeed = 0;
			Spr->animTile = 0;
			Spr->collisionShape = SHAPE_BOX;
			Spr->collisionAngle = 0;
			Spr->collisionWidth = 0;
			Spr->collisionHeight = 0;
			Spr->collisionxoffset = 0;
			Spr->collisionyoffset = 0;
			Spr->depth = 0;
			Spr->show = true;
			Spr->r = 1.0;
			Spr->g = 1.0;
			Spr->b = 1.0;
			Spr->a = 1.0;
			Spr->tilesX = 1;
			Spr->tilesY = 1;
			Spr->rotation_speed = 0.0;
			Spr->Img = nullptr;
			CSprites_Sprites[i] = Spr;
			return Spr;
		}
	}
	return nullptr;
}

void CSprites_RemoveSprite(CSprite* Spr)
{
	if(Spr == nullptr)
		return;

	if((Spr->index < 0) || (Spr->index >= SPR_Max))
		return;
	
	//dumped scaled bitmaps are handled in images
	if(!loadDumpedScaledBitmaps)
		if(CSprites_Sprites[Spr->index]->Img != nullptr)
		{
			SDL_DestroyTexture(CSprites_Sprites[Spr->index]->Img);
			CSprites_Sprites[Spr->index]->Img = nullptr;
		}
	
	CSprites_Sprites[Spr->index] = nullptr;
		
	delete Spr;
}

void CSprites_UpdateSprites(SDL_Renderer* renderer)
{
	CSprites_SortSprites();
	for (int i = 0; i < SPR_Max; i++)
	{
		if (CSprites_Sprites[i] == nullptr)
			continue;

		if (CSprites_Sprites[i]->animSpeed != 0)
		{
			if (SDL_GetTicks() > CSprites_Sprites[i]->animTimer)
			{
				CSprites_Sprites[i]->animTile += CSprites_Sprites[i]->animInc;
				if (CSprites_Sprites[i]->animInc > 0)
				{
					if(CSprites_Sprites[i]->animTile > CSprites_Sprites[i]->animEndTile)
						CSprites_Sprites[i]->animTile = CSprites_Sprites[i]->animStartTile;
				}
				else
				{
					if (CSprites_Sprites[i]->animInc < 0)
					{
						if(CSprites_Sprites[i]->animTile < CSprites_Sprites[i]->animEndTile)
							CSprites_Sprites[i]->animTile = CSprites_Sprites[i]->animStartTile;
					}
				}
				CSprites_Sprites[i]->animTimer = SDL_GetTicks() + (int)floor(1000 / CSprites_Sprites[i]->animSpeed);
			}
		}

		CSprites_Sprites[i]->x += CSprites_Sprites[i]->xspeed;
		CSprites_Sprites[i]->y += CSprites_Sprites[i]->yspeed;
		CSprites_Sprites[i]->xscale += CSprites_Sprites[i]->xscale_speed;
		CSprites_Sprites[i]->yscale += CSprites_Sprites[i]->yscale_speed;
		CSprites_Sprites[i]->rotation += CSprites_Sprites[i]->rotation_speed;
		CSprites_Sprites[i]->collisionAngle += CSprites_Sprites[i]->rotation_speed;
		CSprites_Sprites[i]->collisionWidth += CSprites_Sprites[i]->xscale_speed;
		CSprites_Sprites[i]->collisionHeight += CSprites_Sprites[i]->yscale_speed;

		CSprites_UpdateImage(renderer, CSprites_Sprites[i]);
	}
}

void CSprites_SortSprites()
{
	if (CSprites_needSpriteSorting)
	{
		for (int i = 0; i< SPR_Max; i++)
		{
			for(int j=i+1; j < SPR_Max; j++)
			{
				if ((CSprites_Sprites[i] != nullptr) && (CSprites_Sprites[j] != nullptr))
				{
					if(CSprites_Sprites[i]->depth > CSprites_Sprites[j]->depth)
					{
						CSprite* Tmp = CSprites_Sprites[i];
						CSprites_Sprites[i] = CSprites_Sprites[j];
						CSprites_Sprites[i]->index = i;
						CSprites_Sprites[j] = Tmp;
						CSprites_Sprites[j]->index = j;
					}
				}
			}
		}
		CSprites_needSpriteSorting = false;
	}
}

void CSprites_DrawSprite(SDL_Renderer* Renderer, CSprite* Spr)
{
	if (Spr == nullptr)
		return;

	CSprites_SortSprites();

	if (Spr->show && ((*Spr->imageID > -1) && (Spr->Img != nullptr) && (*Spr->imageID < CImage_ImageSlotsMax())))
	{
		CSprites_SpritesDrawn++;
		SDL_Point pos = {(int)(Spr->x), (int)(Spr->y)};
	
		Vec2F scale = {Spr->xscale, Spr->yscale};
		//multiply is to get the sign
		scale = {1.0f * (Spr->xscale / abs(Spr->xscale)), 1.0f * (Spr->yscale/abs(Spr->yscale))};
		int AnimTile = Spr->animTile;
		int y = (int)floor(AnimTile / Spr->tilesX);
		int x = AnimTile - (y * Spr->tilesX);
		
		SDL_Rect SrcRect = {(int)(x * Spr->tileSizeX* abs(Spr->xscale)), (int)(y* Spr->tileSizeY* abs(Spr->yscale)), (int)(Spr->tileSizeX* abs(Spr->xscale)),(int)(Spr->tileSizeY* abs(Spr->yscale))};
		CImage_DrawImageFuzeSrcRectTintFloat(Renderer, Spr->Img, &SrcRect, true, &pos, Spr->rotation, &scale, Spr->r, Spr->g, Spr->b, Spr->a);
		

		if (Spr->show_collision_shape || CSprites_ForceShowCollisionShape)
		{
			SDL_SetRenderDrawColor(Renderer, 255, 0, 255, 255);
			switch(Spr->collisionShape)
			{
				case SHAPE_BOX:
				{
					const SDL_Rect rect = {(int)(Spr->x + Spr->collisionxoffset - (Spr->collisionWidth * (Spr->xscale) / 2)), (int)(Spr->y + Spr->collisionyoffset - (Spr->collisionHeight * (Spr->yscale) / 2)), (int)(Spr->collisionWidth * (Spr->xscale)), (int)(Spr->collisionHeight * (Spr->yscale))};
					SDL_RenderDrawRect(Renderer, &rect);
					break;
				}
				case SHAPE_CIRCLE:
				{
					if ((Spr->collisionWidth == Spr->collisionHeight) && (Spr->xscale == Spr->yscale))
						circleRGBA(Renderer, Spr->x + Spr->collisionxoffset, Spr->y + Spr->collisionyoffset,(int) ((Spr->collisionWidth * Spr->xscale) / 2), 255, 0, 255, 255);
					break;
				}
				default:
					break;
			}
		}
	}
}

void CSprites_ResetDrawTargets()
{
	for (int i = 0; i < SPR_Max; i++)
	{
		if (CSprites_Sprites[i] == nullptr)
			continue;

		//by reseting prevxscale
		//   it will be redrawn
		CSprites_Sprites[i]->prevxscale = 0;
		CSprites_Sprites[i]->prevyscale = 0;
	}
}

void CSprites_DrawSprites(SDL_Renderer* Renderer)
{
	CSprites_SpritesDrawn = 0;
	for (int i = 0; i < SPR_Max; i++)
	{
		if (CSprites_Sprites[i] == nullptr)
			continue;

		CSprites_DrawSprite(Renderer, CSprites_Sprites[i]);
	}
}

void SetSpriteRotationSpeed(CSprite* Spr, float rotationSpeed)
{
	Spr->rotation_speed = rotationSpeed;
}

void CSprites_SetSpriteColour (CSprite* Spr, float red, float green, float blue, float alpha)
{
	Spr->r = red;
	Spr->g = green;
	Spr->b = blue;
	Spr->a = alpha;
}

void CSprites_SetSpriteVisibility(CSprite* Spr, bool visibility)
{
	Spr->show = visibility;
}

Vec2F CSprites_GetSpriteLocation(CSprite* Spr)
{
	Vec2F Result = {Spr->x, Spr->y};
	return Result;
}

void CSprites_SetSpriteImage(SDL_Renderer* renderer, CSprite* Spr, int *AImageID)
{
	CSprites_SetSpriteImage(renderer, Spr, AImageID, 1, 1);
}

void CSprites_UpdateImage(SDL_Renderer* renderer, CSprite* Spr) 
{
	if(Spr == nullptr)
		return;

	if(Spr->imageID == nullptr)
		return;

	if((abs(Spr->yscale) == abs(Spr->prevyscale)) &&
		(abs(Spr->xscale) == abs(Spr->prevxscale)))
		return;

	if(loadDumpedScaledBitmaps)
	{
		Spr->Img = CImage_LoadScaledImage(renderer, *Spr->imageID, {Spr->xscale,Spr->yscale});
		//remember current scale
		Spr->prevyscale = Spr->yscale;
		Spr->prevxscale = Spr->xscale;
		CSprites_UpdateImageResets++;
	}
	else
	{
		SDL_Texture *tex = CImage_GetImage(*Spr->imageID);

		if (tex == NULL)
			return;

		Uint32 format;
		int w, h;
		SDL_BlendMode blendmode;
		SDL_Texture* renderTarget;

		// Get all properties from the texture we are duplicating
		SDL_QueryTexture(tex, &format, NULL, &w, &h);
		SDL_GetTextureBlendMode(tex, &blendmode);

		// Save the current rendering target (will be NULL if it is the current window)
		renderTarget = SDL_GetRenderTarget(renderer);

		if(Spr->Img != nullptr)
			SDL_DestroyTexture(Spr->Img);

		// Create a new texture with the same properties as the one we are duplicating
		Spr->Img = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_TARGET, w* abs(Spr->xscale), h * abs(Spr->yscale));

		// Set its blending mode and make it the render target
		SDL_SetTextureBlendMode(Spr->Img, SDL_BLENDMODE_NONE);
		SDL_SetRenderTarget(renderer, Spr->Img);

		//clear with transparant color
		uint8_t r,g,b,a;
		SDL_GetRenderDrawColor(renderer, &r,&g,&b,&a);
		SDL_SetRenderDrawColor(renderer,0,0,0,0);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, r,g,b,a);

		//calculate dest size
		SDL_Rect TmpR;
		TmpR.x = 0;
		TmpR.y = 0;
		TmpR.w = w * abs(Spr->xscale);
		TmpR.h = h * abs(Spr->yscale);

		
		// Render the full original texture onto the new one
		SDL_RenderCopy(renderer, tex, NULL, &TmpR);

		// Change the blending mode of the new texture to the same as the original one
		SDL_SetTextureBlendMode(Spr->Img, blendmode);

		// Restore the render target
		SDL_SetRenderTarget(renderer, renderTarget);

		//remember current scale
		Spr->prevyscale = Spr->yscale;
		Spr->prevxscale = Spr->xscale;
		CSprites_UpdateImageResets++;
		
		Vec2F Vec2FScale = {abs(Spr->xscale), abs(Spr->yscale)};
		
		if (SavedScalingsCount < SavedScalingsMax)
		{
			bool bfound = false;
			for (int i = 0; i < SavedScalingsCount; i++)
			{
				if ((CSprites_SavedScalings[i].gfxid == *Spr->imageID) &&
					(CSprites_SavedScalings[i].scale.x == Vec2FScale.x) &&
					(CSprites_SavedScalings[i].scale.y == Vec2FScale.y))
					{
						bfound = true;
						break;
					}
			}

			if(!bfound)
			{
				SavedScalingsCount++;
				CSprites_SavedScalings[SavedScalingsCount].gfxid = *Spr->imageID;
				CSprites_SavedScalings[SavedScalingsCount].scale.x = Vec2FScale.x;
				CSprites_SavedScalings[SavedScalingsCount].scale.y = Vec2FScale.y;
				CImage_SaveImage(renderer, *Spr->imageID, Vec2FScale);
			}
		}
	}
}

void CSprites_SetSpriteImage(SDL_Renderer* renderer, CSprite* Spr, int *AImageID, int TilesX, int TilesY)
{
	bool needCSprites_UpdateImage = Spr->imageID != AImageID;
	Spr->imageID = AImageID;
	SDL_Point Tz = CImage_ImageSize(*AImageID);
	if(needCSprites_UpdateImage)
	{
		//to force an update
		Spr->prevyscale = 0;
		Spr->prevxscale = 0;
		CSprites_UpdateImage(renderer, Spr);
	}
	Spr->tileSizeX = (int)floor(Tz.x / TilesX);
	Spr->tileSizeY = (int)floor(Tz.y / TilesY);
	Spr->tilesX = TilesX;
	Spr->tilesY = TilesY;
	if ((Spr->collisionHeight == 0) && (Spr->collisionWidth == 0))
	{
		Spr->collisionHeight = Spr->tileSizeY;
		Spr->collisionWidth = Spr->tileSizeX;
	}
}

void CSprites_SetSpriteScale(SDL_Renderer* renderer, CSprite* Spr, Vec2F AScale)
{
	Spr->xscale = AScale.x;
	Spr->yscale = AScale.y;
	CSprites_UpdateImage(renderer, Spr);
}

void CSprites_SetSpriteRotation(CSprite* Spr, float AAngle)
{
	float diffAngle = AAngle - Spr->rotation;
	Spr->collisionAngle += diffAngle;
	Spr->rotation = AAngle;
}

void CSprites_SetSpriteAnimation(CSprite* Spr, int StartTile, int EndTile, int animSpeed)
{
	Spr->animStartTile = StartTile;
	Spr->animEndTile = EndTile;
	Spr->animSpeed = animSpeed;
	Spr->animTile = StartTile;
	if (animSpeed != 0)
	{
		Spr->animTimer = SDL_GetTicks() + (int)floor(1000 / animSpeed);

		if(EndTile < StartTile)
			Spr->animInc = -1;
		else
		{
			if(EndTile > StartTile)
				Spr->animInc = 1;
			else
				Spr->animInc = 0;
		}
	}
}

void CSprites_SetSpriteCollisionShape(CSprite* Spr, ECollisionShape shape, float width, float height, float rotation, float xoffset, float yoffset)
{
	Spr->collisionShape = shape;
	Spr->collisionWidth = width;
	Spr->collisionHeight = height;
	Spr->collisionAngle = rotation;
	Spr->collisionxoffset = xoffset;
	Spr->collisionyoffset = yoffset;
}

void CSprites_SetSpriteLocation(CSprite* Spr, Vec2F pos )
{
	Spr->x = pos.x;
	Spr->y = pos.y;
}

void CSprites_SetSpriteDepth(CSprite* Spr, int depth)
{
	Spr->depth = depth;
	CSprites_needSpriteSorting = true;
}

int CSprites_GetSpriteAnimFrameCount(CSprite* Spr)
{
	return max(Spr->animEndTile, Spr->animStartTile) - min(Spr->animEndTile, Spr->animStartTile) + 1;
}

int CSprites_GetSpriteAnimFrame(CSprite* Spr)
{
	return Spr->animTile - min(Spr->animEndTile, Spr->animStartTile);
}

SDL_Point CSprites_TileSize(CSprite* Spr)
{
	return {Spr->tileSizeX, Spr->tileSizeY};
}

//https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-Detection#
bool CSprites_DetectRectCircleCollsion(CSprite* SprRect, CSprite* SprCircle)
{
	Vec2F center = {SprCircle->x + SprCircle->collisionxoffset /2.0f, SprCircle->y + SprCircle->collisionyoffset / 2.0f};
	// calculate AABB info (center, half-extents)
	Vec2F aabb_half_extents = {abs(SprRect->collisionWidth) * abs(SprRect->xscale) / 2.0f, abs(SprRect->collisionHeight) * abs(SprRect->yscale) / 2.0f};
	Vec2F aabb_center = {SprRect->x + SprRect->collisionxoffset / 2.0f, SprRect->y + SprRect->collisionyoffset / 2.0f};

	// get difference vector between both centers
	Vec2F difference;
	difference.x = center.x - aabb_center.x;
	difference.y = center.y - aabb_center.y;
	Vec2F neg_half_extends;
	neg_half_extends.x = -aabb_half_extents.x;
	neg_half_extends.y = -aabb_half_extents.y;
	Vec2F clamped = clamp(difference, neg_half_extends, aabb_half_extents);
	// add clamped value to AABB_center and we get the value of box closest to circle
	Vec2F closest;
	closest.x = aabb_center.x + clamped.x;
	closest.y = aabb_center.y + clamped.y;
	// retrieve vector between center circle and closest point AABB and check if length <= radius
	difference.x = closest.x - center.x;
	difference.y = closest.y - center.y;

	return length(difference) < (abs(SprCircle->collisionWidth) * abs(SprCircle->xscale) / 2.0f);
}

bool CSprites_DetectRectRectCollsion(CSprite* Spr, CSprite* SprOther)
{
	float widthA = (abs(Spr->collisionWidth) * abs(Spr->xscale));
	float heightA = (abs(Spr->collisionHeight) * abs(Spr->yscale));
	float minAx = Spr->x + Spr->collisionxoffset - (abs(Spr->collisionWidth) * abs(Spr->xscale) / 2);
	float minAy = Spr->y + Spr->collisionyoffset - (abs(Spr->collisionHeight) * abs(Spr->yscale) / 2);

	float widthB = (abs(SprOther->collisionWidth) * abs(SprOther->xscale));
	float heightB = (abs(SprOther->collisionHeight) * abs(SprOther->yscale));
	float minBx = SprOther->x + SprOther->collisionxoffset - (abs(SprOther->collisionWidth) * abs(SprOther->xscale) / 2);
	float minBy = SprOther->y + SprOther->collisionyoffset - (abs(SprOther->collisionHeight) * abs(SprOther->yscale) / 2);

	bool xOverlap = ((minAx >= minBx) && (minAx <= minBx + widthB)) ||
					((minBx >= minAx) && (minBx <= minAx + widthA));

	bool yOverlap = ((minAy >= minBy) && (minAy <= minBy + heightB)) ||
					((minBy >= minAy) && (minBy <= minAy + heightA));

	return xOverlap && yOverlap;
}

//takes no rotations into account !
bool CSprites_DetectSpriteCollision(CSprite* Spr, CSprite* SprOther)
{
	if((Spr == nullptr) || (SprOther == nullptr))
		return false;

	switch(Spr->collisionShape)
	{
		case SHAPE_BOX:
		{
			switch(SprOther->collisionShape)
			{
				case SHAPE_BOX:
					return CSprites_DetectRectRectCollsion(Spr, SprOther);
					break;
				case SHAPE_CIRCLE:
					//only works for true circles not ovals!
					if ((SprOther->collisionWidth == SprOther->collisionHeight) && (SprOther->xscale == SprOther->yscale))
					{	// check normal rect first
						//if (CSprites_DetectRectRectCollsion(Spr, SprOther))
							return CSprites_DetectRectCircleCollsion(Spr, SprOther);
						//else
						//	return false;
					}
					else
						return false;
					break;
				default:
					return false;
			}
			break;
		}
		case SHAPE_CIRCLE:
		{
			switch(SprOther->collisionShape)
			{
				case SHAPE_BOX:
					//only works for true circles not ovals!
					if ((Spr->collisionWidth == Spr->collisionHeight) && (Spr->xscale == Spr->yscale))
					{
						// check normal rect first
						//if (CSprites_DetectRectRectCollsion(Spr, SprOther))
							return CSprites_DetectRectCircleCollsion(SprOther, Spr);
						//else
						//	return false;
					}
					else
						return false;
					break;
				default:
					return false;
			}
			break;
		}
		default:
			return false;
	}
}

int CSprites_SpriteSlotsUsed()
{
	int c = 0;
	for (int i=0; i < SPR_Max; i++)
	{
		if(CSprites_Sprites[i] != nullptr)
			c++;
	}
	return c;
}

int CSprites_SpriteSlotsMax()
{
	return SPR_Max;
}

