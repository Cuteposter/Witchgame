#pragma once
#include <SDL.h>
#include <SDL_image.h>

class Solid
{
public:
	Solid(int x, int y, int w, int h);
	~Solid();

	SDL_Rect* getColRect();
	void render(SDL_Renderer* r);
private:
	SDL_Rect colRect;
};

