#include "Solid.h"


Solid::Solid(int x, int y, int w, int h)
{
	colRect.x = x;
	colRect.y = y;
	colRect.w = w;
	colRect.h = h;
}


Solid::~Solid()
{
}

SDL_Rect* Solid::getColRect()
{
	return &colRect;
}

void Solid::render(SDL_Renderer* r)
{
	SDL_SetRenderDrawColor(r, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(r, &colRect);
}
