#pragma once

#include <SDL.h>
#include <iostream>
#include <string>
#include <vector>
#include "CImage.h"
#include "Vec2F.h"

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
		SDL_Texture *Img;
		int animInc;
		Uint32 animTimer;
		int index;
		int* imageID;
		float x;
		float y;
		float prevxscale;
		float prevyscale;
		float xscale;
		float yscale;
		float xspeed;
		float yspeed;
		float xscale_speed;
		float yscale_speed;
		float rotation;
		int tileSizeX;
		int tileSizeY;
		int tilesX;
		int tilesY;
		int animTile;
		int animStartTile;
		int animEndTile;
		int animSpeed;
		ECollisionShape collisionShape;
		float collisionAngle;
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
		float rotation_speed;
		bool show_collision_shape;		
};

class CSprites {

private:
	CSprite* Sprites[SPR_Max];
	vector<pair<int,pair<float,float>>> SavedScalings;
	vector<pair<SDL_Texture*,pair<int, Vec2F>>> LoadedScaledTextures;
	CImage* Images;
	int UpdateImageResets;
	int SpritesDrawn;
	bool ForceShowCollisionShape;
	bool needSpriteSorting;
	void SortSprites();
	bool DetectRectRectCollsion(CSprite* Spr, CSprite* SprOther);
	bool DetectRectCircleCollsion(CSprite* SprRect, CSprite* SprCircle);
public:
	CSprites(CImage* ACImage);
	~CSprites();
	void ResetDrawTargets();
	void UpdateImage(SDL_Renderer* renderer, CSprite* Spr);
	CSprite* CreateSprite();
	void RemoveSprite(CSprite* Spr);
	Vec2F GetSpriteLocation(CSprite* Spr);
	void SetForceShowCollisionShape(bool val);
	void UpdateSprites(SDL_Renderer* renderer);
	void DrawSprite(SDL_Renderer* Renderer, CSprite* Spr);
	void DrawSprites(SDL_Renderer* Renderer);
	int GetSpriteAnimFrame(CSprite* Spr);
	int GetSpriteAnimFrameCount(CSprite* Spr);
	void SetSpriteImage(SDL_Renderer* renderer, CSprite* Spr, int *AImageID);
	void SetSpriteImage(SDL_Renderer* renderer, CSprite* Spr, int *AImageID, int TilesX, int TilesY);
	void SetSpriteRotation(CSprite* Spr, float AAngle);
	void SetSpriteScale(SDL_Renderer* renderer, CSprite* Spr, Vec2F AScale);
	void SetSpriteAnimation(CSprite* Spr, int StartTile, int EndTile, int animSpeed);
	void SetSpriteCollisionShape(CSprite* Spr, ECollisionShape shape, float width, float height, float rotation, float xoffset, float yoffset);
	void SetSpriteLocation(CSprite* Spr, Vec2F pos );
	void SetSpriteDepth(CSprite* Spr, int depth);
	void SetSpriteColour(CSprite* Spr, float red, float green, float blue, float alpha);
	void SetSpriteVisibility(CSprite* Spr, bool visibility);
	void SetSpriteRotationSpeed(CSprite* Spr, float rotationSpeed);
	bool DetectSpriteCollision(CSprite* Spr, CSprite* SprOther);
	SDL_Point TileSize(CSprite* Spr);
	int SpriteSlotsUsed();
	int SpriteSlotsMax();
	int UpdateImageResetsCount();
	int SpritesDrawnCount();
};
