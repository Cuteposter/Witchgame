#include "Enemy.h"

Enemy::Enemy()
{
	spr_path = "./res/spr/monster.png";
	ANIMATION_FRAMES = 2;
	frame = 0;

	w = 32;
	h = 32;

	colRect = SDL_Rect{ x, y, w, h };
}

Enemy::Enemy(int sx, int sy)
{
	spr_path = "./res/spr/monster.png";
	ANIMATION_FRAMES = 2;
	frame = 0;
	x = sx;
	y = sy;

	w = 32;
	h = 32;

	colRect = SDL_Rect{ x, y, w, h };
}


Enemy::~Enemy()
{
	sprite.free();
}

void Enemy::step()
{
	x += 1 * dir;
	c++;

	if (c > 32)
	{
		dir = -dir;
		c = 0;
	}

	colRect.x = x;
	colRect.y = y;
	//std::cout << tag.compare("Enemy") << "\n";
}

void Enemy::render(SDL_Renderer* r)
{
	SDL_Rect* currentClip = &gSpriteClips[frame / 10];
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (dir == -1)
	{
		flip = SDL_FLIP_HORIZONTAL;
	}


	sprite.render(r, x, y, currentClip, 0.0, (SDL_Point*)0, flip);

	//Go to next frame
	++frame;

	//Cycle animation
	if (frame / 10 >= ANIMATION_FRAMES)
	{
		frame = 0;
	}
}