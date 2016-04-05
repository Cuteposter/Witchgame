#include "ShootyBall.h"


ShootyBall::ShootyBall(SDL_Renderer* renderer, int px, int py)
{
	//wait = 0;
	r = renderer;
	x = px;
	y = py;
	ANIMATION_FRAMES = 1;
	w = 64;
	h = 64;
	spr_path = "./res/spr/part_glowball.png";
	loadSprite(renderer);
	count = 0;
	angle = 0;

	colRect = SDL_Rect{x, y, 32, 32};
}


ShootyBall::~ShootyBall()
{
	sprite.free();
}

void ShootyBall::step()
{
	vx = cos(angle * M_PI / 180);
	vy = sin(angle * M_PI / 180);

	x += vx*4;
	y += vy*4;

	if (count < 25)
	{
		particles[count] = new PEnergy(x, y);
		particles[count]->sprite = &sprite;
		particles[count]->gSpriteClips = gSpriteClips;
		particles[count]->angle = angle;
		count++;
		wait = 0;
	}


	for (int i = 0; i < count; i++)
	{
		particles[i]->step();
	}
	
	colRect.x = x;
	colRect.y = y;
}

void ShootyBall::render(SDL_Renderer* r, Camera* cam)
{
	//First, clean up dead particles
	for (int i = 0; i < count; i++)
	{
		if (particles[i]->dead)	
		{
			delete particles[i];
			particles[i] = new PEnergy(x, y);
			particles[i]->sprite = &sprite;
			particles[i]->gSpriteClips = gSpriteClips;
			particles[i]->angle = angle;
		}
	}

	for (int i = 0; i < count; i++)
	{
		particles[i]->render(r, cam);
	}

	SDL_SetRenderDrawColor(r, 0xFF, 0x00, 0x00, 0xFF);
	//SDL_RenderDrawRect(r, &colRect);
	SDL_SetRenderDrawColor(r, 0x40, 0x40, 0x40, 0xFF);
}