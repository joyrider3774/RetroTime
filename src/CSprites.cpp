#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "CSprites.h"

CSprites::CSprites(CImage* ACImage)
{
    Images = ACImage;
    for (int i=0; i < SPR_Max; i++)
    {
        Sprites[i] = nullptr;
    }
}

CSprites::~CSprites()
{
    for (int i=0; i < SPR_Max; i++)
    {
        RemoveSprite(Sprites[i]);
    }
}

CSprite* CSprites::CreateSprite()
{
    for (int i= 0; i < SPR_Max; i++)
    {
        if(Sprites[i] == nullptr)
        {
            CSprite* Spr =  new CSprite();
            Spr->index = i;
            Spr->animInc = 0;
            Spr->animTimer = 0;
            Spr->rotation = 0;
            Spr->imageID = nullptr;
            Spr->xscale = 1;
            Spr->yscale = 1;
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
            Sprites[i] = Spr;
            return Spr;
        }
    }
    return nullptr;
}

void CSprites::RemoveSprite(CSprite* Spr)
{
    if(Spr == nullptr)
        return;

    if((Spr->index < 0) || (Spr->index >= SPR_Max))
        return;

    Sprites[Spr->index] = nullptr;
    delete Spr;
}

void CSprites::UpdateSprites()
{
    SortSprites();
    for (int i = 0; i < SPR_Max; i++)
    {
        if (Sprites[i] == nullptr)
            continue;

        if (Sprites[i]->animSpeed != 0)
        {
            if (SDL_GetTicks() > Sprites[i]->animTimer)
            {
                Sprites[i]->animTile += Sprites[i]->animInc;
                if (Sprites[i]->animInc > 0)
                {
                    if(Sprites[i]->animTile > Sprites[i]->animEndTile)
                        Sprites[i]->animTile = Sprites[i]->animStartTile;
                }
                else
                {
                    if (Sprites[i]->animInc < 0)
                    {
                        if(Sprites[i]->animTile < Sprites[i]->animEndTile)
                            Sprites[i]->animTile = Sprites[i]->animStartTile;
                    }   
                }
                Sprites[i]->animTimer = SDL_GetTicks() + (int)floor(1000 / Sprites[i]->animSpeed);
            }
        }

        Sprites[i]->x += Sprites[i]->xspeed;
        Sprites[i]->y += Sprites[i]->yspeed;
        Sprites[i]->xscale += Sprites[i]->xscale_speed;
        Sprites[i]->yscale += Sprites[i]->yscale_speed;
        Sprites[i]->rotation += Sprites[i]->rotation_speed;
        Sprites[i]->collisionAngle += Sprites[i]->rotation_speed;
        Sprites[i]->collisionWidth += Sprites[i]->xscale_speed;
        Sprites[i]->collisionHeight += Sprites[i]->yscale_speed;
    }
}

void CSprites::SortSprites()
{
    if (needSpriteSorting)
    {
        for (int i = 0; i< SPR_Max; i++)
        {
            for(int j=i+1; j < SPR_Max; j++)
            {
                if ((Sprites[i] != nullptr) && (Sprites[j] != nullptr))
                {
                    if(Sprites[i]->depth > Sprites[j]->depth)
                    {
                        CSprite* Tmp = Sprites[i];
                        Sprites[i] = Sprites[j];
                        Sprites[i]->index = i;
                        Sprites[j] = Tmp;
                        Sprites[j]->index = j;
                    }
                }
            }
        }
        needSpriteSorting = false;
    }
}

void CSprites::DrawSprite(SDL_Renderer* Renderer, CSprite* Spr)
{
    if (Spr == nullptr)
        return;

    SortSprites();
   
    if (Spr->show && ((*Spr->imageID > -1) && (*Spr->imageID < Images->ImageSlotsMax())))
    {
        SDL_Point pos = {(int)(Spr->x), (int)(Spr->y)};
        SDL_FPoint scale = {Spr->xscale, Spr->yscale};
        int AnimTile = Spr->animTile;
        int y = (int)floor(AnimTile / Spr->tilesX);
        int x = AnimTile - (y * Spr->tilesX);
        SDL_Rect SrcRect = {x * Spr->tileSizeX, y* Spr->tileSizeY, Spr->tileSizeX, Spr->tileSizeY};
        Images->DrawImageFuzeSrcRectTintFloat(Renderer, *Spr->imageID, &SrcRect, true, &pos, Spr->rotation, &scale, Spr->r, Spr->g, Spr->b, Spr->a);
        // const SDL_Rect rect = {(int)(Spr->x + Spr->collisionxoffset - (Spr->collisionWidth * (Spr->xscale) / 2)), (int)(Spr->y + Spr->collisionyoffset - (Spr->collisionHeight * (Spr->yscale) / 2)), (int)(Spr->collisionWidth * (Spr->xscale)),  (int)(Spr->collisionHeight * (Spr->yscale))};
        // SDL_SetRenderDrawColor(Renderer, 255, 0, 255, 255);
        // SDL_RenderDrawRect(Renderer, &rect);
    }
}

void CSprites::DrawSprites(SDL_Renderer* Renderer)
{
    for (int i = 0; i < SPR_Max; i++)
    {
        if (Sprites[i] == nullptr)
            continue;

        DrawSprite(Renderer, Sprites[i]);
    }
}

void SetSpriteRotationSpeed(CSprite* Spr,  double rotationSpeed)
{
    Spr->rotation_speed = rotationSpeed;
}

void CSprites::SetSpriteColour (CSprite* Spr, float red, float green, float blue, float alpha)
{
    Spr->r = red;
    Spr->g = green;
    Spr->b = blue;
    Spr->a = alpha;
}

void CSprites::SetSpriteVisibility(CSprite* Spr, bool visibility)
{
    Spr->show = visibility;
}

SDL_FPoint CSprites::GetSpriteLocation(CSprite* Spr)
{
    SDL_FPoint Result = {Spr->x, Spr->y};
    return Result;
}

void CSprites::SetSpriteImage(CSprite* Spr, int *AImageID)
{
    SetSpriteImage(Spr, AImageID, 1, 1);
}

void CSprites::SetSpriteImage(CSprite* Spr, int *AImageID, int TilesX, int TilesY)
{
    Spr->imageID = AImageID;
    SDL_Point Tz = Images->ImageSize(*AImageID);
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

void CSprites::SetSpriteScale(CSprite* Spr, SDL_FPoint AScale)
{  
    Spr->xscale = AScale.x;
    Spr->yscale = AScale.y;
}

void  CSprites::SetSpriteRotation(CSprite* Spr, double AAngle)
{
    double diffAngle = AAngle - Spr->rotation;
    Spr->collisionAngle += diffAngle;
    Spr->rotation = AAngle;
}

void CSprites::SetSpriteAnimation(CSprite* Spr, int StartTile, int EndTile, int animSpeed)
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

void CSprites::SetSpriteCollisionShape(CSprite* Spr, ECollisionShape shape, double width, double height, double rotation, float xoffset, float yoffset)
{
    Spr->collisionShape = shape;
    Spr->collisionWidth = width;
    Spr->collisionHeight = height;
    Spr->collisionAngle = rotation;
    Spr->collisionxoffset = xoffset;
    Spr->collisionyoffset = yoffset;
}

void CSprites::SetSpriteLocation(CSprite* Spr, SDL_FPoint pos )
{
    Spr->x = pos.x;
    Spr->y = pos.y;
}

void CSprites::SetSpriteDepth(CSprite* Spr, int depth)
{
    Spr->depth = depth;
    needSpriteSorting = true;
}

int CSprites::GetSpriteAnimFrameCount(CSprite* Spr)
{
    return max(Spr->animEndTile, Spr->animStartTile) - min(Spr->animEndTile, Spr->animStartTile) + 1;
}

int CSprites::GetSpriteAnimFrame(CSprite* Spr)
{
    return Spr->animTile - min(Spr->animEndTile, Spr->animStartTile);
}

SDL_Point CSprites::TileSize(CSprite* Spr)
{
    return {Spr->tileSizeX, Spr->tileSizeY};
}

bool CSprites::DetectSpriteCollision(CSprite* Spr, CSprite* SprOther)
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
                {
                    float widthA = (abs(Spr->collisionWidth) * abs(Spr->xscale));
                    float heightA = (abs(Spr->collisionHeight) * abs(Spr->yscale));
                    float minAx = Spr->x + Spr->collisionxoffset - (abs(Spr->collisionWidth) * abs(Spr->xscale) / 2);
                    float minAy = Spr->y + Spr->collisionyoffset - (abs(Spr->collisionHeight) * abs(Spr->yscale) / 2);
                    
                    float widthB = (abs(SprOther->collisionWidth) * abs(SprOther->xscale));
                    float heightB = (abs(SprOther->collisionHeight) * abs(SprOther->yscale));
                    float minBx = SprOther->x + SprOther->collisionxoffset - (abs(SprOther->collisionWidth) * abs(SprOther->xscale) / 2);
                    float minBy = SprOther->y + SprOther->collisionyoffset - (abs(SprOther->collisionHeight) * abs(SprOther->yscale) / 2);
                                        
                    bool xOverlap = ((minAx >= minBx) && (minAx <= minBx + widthB))  ||
                                    ((minBx >= minAx) && (minBx <= minAx + widthA));

                    bool yOverlap = ((minAy >= minBy) && (minAy <= minBy + heightB)) ||
                                    ((minBy >= minAy) && (minBy <= minAy + heightA));

                    return xOverlap && yOverlap;
                }
                break;
                default:
                    return false;

            }
        }
        break;
        default:
            return false;
    }
}

int CSprites::SpriteSlotsUsed()
{
    int c = 0;
    for (int i=0; i < SPR_Max; i++)
    {
        if(Sprites[i] != nullptr)
            c++;
    }
    return c;
}

int CSprites::SpriteSlotsMax()
{
    return SPR_Max;
}

