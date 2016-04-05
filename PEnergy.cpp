#include "PEnergy.h"

PEnergy::PEnergy(float px, float py)
{
	frame = 0;
	ANIMATION_FRAMES = 1;
	x = px;
	y = py;
	w = 64;
	h = 64;
	angle = 0.0;
	alpha = 255;
	//dir = rand() % 2;
	//vx = 0.5f / ((rand() % 3) + 1);
	dead = false;
}


PEnergy::~PEnergy()
{
}


void PEnergy::step()
{
	alpha -= 10;
	if (alpha <= 10)
	{
		dead = true;
		alpha = 0;
	}

	w -= 2;
	x += 0.5f;
	y += 0.5f;
	h -= 2;

	if (w < 0)
		w = 0;
	if (h < 0)
		h = 0;
}

void PEnergy::render(SDL_Renderer* r)
{
	SDL_Rect currentClip = gSpriteClips.at(0);

	sprite->setBlendMode(SDL_BLENDMODE_ADD);

	sprite->setAlpha(alpha/4);
	//sprite.render(r, x, y);
	sprite->setColor(0, 128, 255);

	sprite->renderScaled(r, x, y, w/2, h/2, &currentClip, 0.0);
	//sprite.render(r, x, y, &currentClip, angle+180);
}
