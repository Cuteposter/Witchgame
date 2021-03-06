#pragma once
#include <string>
#include <SDL.h>
#include "Component.h"
#include "LTexture.h"
/*class HelloComponent :
public Component
{
public:
std::string s = "Hello Component!";
};*/

class ComponentSprite :
	public Component
{
public:
	ComponentSprite(SDL_Renderer *r, char* path, int w, int h, float scale=1.0f, bool glow=false);
	~ComponentSprite();
	
	LTexture sprite;
	LTexture gSpriteSheetTexture;
	float scale;
	bool glow;
	int frame;
	int ANIMATION_FRAMES;
	std::vector<SDL_Rect> gSpriteClips;
};