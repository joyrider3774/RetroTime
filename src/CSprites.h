#pragma once

#include <SDL.h>
#include <limits.h>
#include <iostream>
#include <string>
#include "CImage.h"

constexpr int SPR_Max = 1000;

enum ECollisionShape
{
    SHAPE_BOX, 
    SHAPE_TRIANGLE,
    SHAPE_CIRCLE
};
typedef enum ECollisionShape ECollisionShape;

class CSprite {
    public:
        int animInc;
        Uint32 animTimer;
        int index;
        int* imageID;
        float x;
        float y;
        float xscale;
        float yscale;
        float xspeed;
        float yspeed;
        float xscale_speed;
        float yscale_speed;
        double rotation;
        int tileSizeX;
        int tileSizeY;
        int tilesX;
        int tilesY;
        int animTile;
        int animStartTile;
        int animEndTile;
        int animSpeed;
        ECollisionShape collisionShape;
        double collisionAngle;
        float collisionWidth;
        float collisionHeight;
        float collisionxoffset;
        float collisionyoffset;
        int depth;
        bool show;
        float r;
        float g;
        float b;
        float a;
        double rotation_speed;
};

class CSprites {

private:
    CSprite* Sprites[SPR_Max];
    CImage* Images;
    bool needSpriteSorting;
    void SortSprites();
public:
    CSprites(CImage* ACImage);
    ~CSprites();
    CSprite* CreateSprite();
    void RemoveSprite(CSprite* Spr);
    SDL_FPoint GetSpriteLocation(CSprite* Spr);
    void UpdateSprites();
    void DrawSprite(SDL_Renderer* Renderer, CSprite* Spr);
    void DrawSprites(SDL_Renderer* Renderer);
    int GetSpriteAnimFrame(CSprite* Spr);
    int GetSpriteAnimFrameCount(CSprite* Spr);
    void SetSpriteImage(CSprite* Spr, int *AImageID);
    void SetSpriteImage(CSprite* Spr, int *AImageID, int TilesX, int TilesY);
    void SetSpriteRotation(CSprite* Spr, double AAngle);
    void SetSpriteScale(CSprite* Spr, SDL_FPoint AScale);
    void SetSpriteAnimation(CSprite* Spr, int StartTile, int EndTile, int animSpeed);
    void SetSpriteCollisionShape(CSprite* Spr, ECollisionShape shape, double width, double height, double rotation, float xoffset, float yoffset);
    void SetSpriteLocation(CSprite* Spr, SDL_FPoint pos );
    void SetSpriteDepth(CSprite* Spr, int depth);
    void SetSpriteColour(CSprite* Spr, float red, float green, float blue, float alpha);
    void SetSpriteVisibility(CSprite* Spr, bool visibility);
    void SetSpriteRotationSpeed(CSprite* Spr,  double rotationSpeed);
    bool DetectSpriteCollision(CSprite* Spr, CSprite* SprOther);
    SDL_Point TileSize(CSprite* Spr);
    int SpriteSlotsUsed();
    int SpriteSlotsMax();
};
