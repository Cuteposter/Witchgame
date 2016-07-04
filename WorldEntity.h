#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include <iostream>

#include "LTexture.h"
#include "GameEntities.h"

//A base class for all objects in the world

class WorldEntity
{
public:
	WorldEntity();
	~WorldEntity();

	int etype;
	
	char* spr_path;
	int frame;
	bool dead;
	LTexture sprite;
	SDL_Rect colRect;

	int ANIMATION_FRAMES;
	std::vector<SDL_Rect> gSpriteClips;
	LTexture gSpriteSheetTexture;

	//Load sprite
	virtual bool loadSprite(SDL_Renderer* r);

	//Game logic step
	virtual void step();

	//Collision check between own collision rectangle "a" and other rectangle "b"
	virtual bool checkCollision(SDL_Rect* b);

	//Render object
	virtual void render(SDL_Renderer* r);

	//Getters
	int getWidth();
	int getHeight();
//protected:
	float x, y;
	int w, h;
};

