#pragma once
#include "WorldEntity.h"
class Enemy :
	public WorldEntity
{
public:
	Enemy();
	Enemy(int sx, int sy);
	~Enemy();

	void step();
	void render(SDL_Renderer* r);

	SDL_Rect colRect;

	int dir = 1;
	int c = 0;
};

