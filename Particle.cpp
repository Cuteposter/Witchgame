#include "Particle.h"


Particle::Particle()
{
	spr_path = "./res/spr/part_glow.png";
	frame = 0;
	ANIMATION_FRAMES = 1;
	w = 7;
	h = 7;
	angle = 0.0;
	alpha = 255;
	dir = rand() % 2;
	dead = false;
}

Particle::Particle(float px, float py)
{
	frame = 0;
	ANIMATION_FRAMES = 1;
	x = px;
	y = py;
	w = 64;
	h = 64;
	angle = 0.0;
	alpha = 255;
	dir = rand() % 2;
	vx = 0.5f / ((rand() % 3)+1);
	dead = false;
}

bool Particle::loadSprite(SDL_Renderer* r)
{
	//The particle emitter sets the particle sprite
	return true;
}


Particle::~Particle()
{
	//printf("Killed particle. ");
	//sprite.free();
}

void Particle::step()
{

	if (!dead){
		if (dir == 0)
		{
			angle -= 4;
			x -= vx;
		}
		else
		{
			angle += 4;
			x += vx;
		}
		vy += GRAVITY;
		if (vy > 3)
			vy = 3;
		y -= vy;

		alpha -= 10;
		if (alpha <= 10)
		{
			dead = true;
			alpha = 0;
		}

		w -= 2;
		x += 1;
		h -= 2;

		if (w < 0)
			w = 0;
		if (h < 0)
			h = 0;
	}

	/*
	vx = cos(angle * M_PI/180);
	vy = sin(angle * M_PI/180);

	x += vx;
	y += vy;

	alpha -= 15;
	if (alpha <= 15)
	{
		dead = true;
		alpha = 0;
	}
	*/
}

void Particle::render(SDL_Renderer* r, Camera* cam)
{
	SDL_Rect currentClip = gSpriteClips.at(0);
	//printf("%d\n", gSpriteClips.at(0).w);
	//printf("%d\n", sprite);

	sprite->setBlendMode(SDL_BLENDMODE_ADD);
	
	sprite->setAlpha(alpha);
	//sprite.render(r, x, y);
	sprite->setColor(255, 192 - w * 2, 0);

	sprite->renderScaled(r, x-cam->x, y-cam->y, w, h, &currentClip);
	//sprite.render(r, x, y, &currentClip, angle+180);

	//Go to next frame
	++frame;

	//Cycle animation
	if (frame / 10 >= ANIMATION_FRAMES)
	{
		frame = 0;
	}
}

void Particle::render(SDL_Renderer* r, Camera* cam, SDL_Color* color)
{
	SDL_Rect currentClip = gSpriteClips.at(0);
	//printf("%d\n", gSpriteClips.at(0).w);
	//printf("%d\n", sprite);

	sprite->setBlendMode(SDL_BLENDMODE_ADD);

	sprite->setAlpha(alpha);
	//sprite.render(r, x, y);
	sprite->setColor(255, 192 - w * 2, 0);
	sprite->setColor(color->r, color->g, color->b);

	sprite->renderScaled(r, x - cam->x, y - cam->y, w, h, &currentClip);
	//sprite.render(r, x, y, &currentClip, angle+180);

	//Go to next frame
	++frame;

	//Cycle animation
	if (frame / 10 >= ANIMATION_FRAMES)
	{
		frame = 0;
	}
}
