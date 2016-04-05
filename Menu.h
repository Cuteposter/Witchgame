#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>

#include "LTexture.h"

class Menu
{
public:
	Menu();
	~Menu();

	LTexture sprite;
	int frame;
	static const int ANIMATION_FRAMES = 2;
	SDL_Rect gSpriteClips[ANIMATION_FRAMES];
	LTexture gSpriteSheetTexture;

	//Load sprite
	bool loadSprite(SDL_Renderer* r);

	//Takes key presses
	void handleEvent(SDL_Event& e);

	//Render sprite
	void render(SDL_Renderer* r);

private:
	int w, h, x, y;
	int x2, y2;
};

