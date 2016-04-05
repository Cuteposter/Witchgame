#pragma once
#include <SDL.h>
#include "Camera.h"
#include "LTexture.h"
class Background
{
public:
	Background(char* path, bool parallax, SDL_Renderer* renderer);
	~Background();

	LTexture bg;

	bool parallax;

	int x = 0;
	int y = 0;
	int xp = 0;
	int yp = 0;

	void update(Camera* cam);
	void render(SDL_Renderer* r);
};

