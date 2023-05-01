#include <SDL.h>
#include <limits.h>
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
            Spr->imageID = -1;
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
            Spr->collisionShape = SHAPE_NONE;
            Spr->collisionAngle = 0;
            Spr->collisionWidth = 0;
            Spr->collisionHeight = 0;
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
    free(Spr);
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
                    else
                        break;
                }
                else
                {
                    if ((Sprites[i] == nullptr) && (Sprites[j] != nullptr))
                    {
                        CSprite* Tmp = Sprites[i];
                        Sprites[i] = Sprites[j];
                        Sprites[i]->index = i;
                        Sprites[j] = Tmp;
                        Sprites[j]->index = j;
                    }
                    else
                        break;
                }
            }
        }
        needSpriteSorting = false;
    }
}

void CSprites::DrawSprites(SDL_Renderer* Renderer)
{
    SortSprites();
    for (int i = 0; i < SPR_Max; i++)
    {
        if (Sprites[i] == nullptr)
            continue;
        if (Sprites[i]->show && ((Sprites[i]->imageID > -1) && (Sprites[i]->imageID < Images->ImageSlotsMax())))
        {
            SDL_Point pos = {(int)floor(Sprites[i]->x), (int)floor(Sprites[i]->y)};
            SDL_FPoint scale = {Sprites[i]->xscale, Sprites[i]->yscale};
            int AnimTile = Sprites[i]->animTile;
            int y = (int)floor(AnimTile / Sprites[i]->tilesX);
            int x = AnimTile - (y * Sprites[i]->tilesX);
            SDL_Rect SrcRect = {x * Sprites[i]->tileSizeX, y* Sprites[i]->tileSizeY, Sprites[i]->tileSizeX, Sprites[i]->tileSizeY};
            Images->DrawImageFuzeSrcRectTintFloat(Renderer, Sprites[i]->imageID, &SrcRect, true, &pos, Sprites[i]->rotation, &scale, Sprites[i]->r, Sprites[i]->g, Sprites[i]->b, Sprites[i]->a);
            // const SDL_Rect rect = {(int)(Sprites[i]->x - (Sprites[i]->collisionWidth * (Sprites[i]->xscale) / 2)), (int)(Sprites[i]->y - (Sprites[i]->collisionHeight * (Sprites[i]->yscale) / 2)), (int)(Sprites[i]->collisionWidth * (Sprites[i]->xscale)),  (int)(Sprites[i]->collisionHeight * (Sprites[i]->yscale))};
            // SDL_SetRenderDrawColor(Renderer, 255, 0, 255, 255);
            // SDL_RenderDrawRect(Renderer, &rect);
        }        
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

void CSprites::SetSpriteImage(CSprite* Spr, int AImageID)
{
    SetSpriteImage(Spr, AImageID, 1, 1);
}

void CSprites::SetSpriteImage(CSprite* Spr, int AImageID, int TilesX, int TilesY)
{
    Spr->imageID = AImageID;
    SDL_Point Tz = Images->ImageSize(AImageID);
    Spr->tileSizeX = (int)floor(Tz.x / TilesX);
    Spr->tileSizeY = (int)floor(Tz.y / TilesY);
    Spr->tilesX = TilesX;
    Spr->tilesY = TilesY;
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

void CSprites::SetSpriteCollisionShape(CSprite* Spr, ECollisionShape shape, double width, double height, double rotation)
{
    Spr->collisionShape = shape;
    Spr->collisionWidth = width;
    Spr->collisionHeight = height;
    Spr->collisionAngle = rotation;
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
    return Spr->animEndTile - Spr->animEndTile + 1;
}

int CSprites::GetSpriteAnimFrame(CSprite* Spr)
{
    return Spr->animTile;
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
                    float minAx = Spr->x - (abs(Spr->collisionWidth) * abs(Spr->xscale) / 2);
                    float minAy = Spr->y - (abs(Spr->collisionHeight) * abs(Spr->yscale) / 2);
                    
                    float widthB = (abs(SprOther->collisionWidth) * abs(SprOther->xscale));
                    float heightB = (abs(SprOther->collisionHeight) * abs(SprOther->yscale));
                    float minBx = SprOther->x - (abs(SprOther->collisionWidth) * abs(SprOther->xscale) / 2);
                    float minBy = SprOther->y - (abs(SprOther->collisionHeight) * abs(SprOther->yscale) / 2);
                                        
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

