#pragma once
#include "WorldEntity.h"

//A class to test inheritence

class StarPal :
	public WorldEntity
{
public:
	StarPal();
	~StarPal();

	//Load sprite
	bool loadSprite(SDL_Renderer* r);

	void step();

	void render(SDL_Renderer* r);
};

