#pragma once
#include <math.h>
#include "WorldEntity.h"
#include "Camera.h"
class Particle :
	public WorldEntity
{
public:
	Particle();
	Particle(float px, float py);
	~Particle();

	const float GRAVITY = 0.1f;
	int dir;
	Uint8 alpha;
	Uint8 color;
	double angle;
	bool dead;

	LTexture* sprite;

	void step();
	void render(SDL_Renderer* r, Camera* cam);
	void render(SDL_Renderer* r, Camera* cam, SDL_Color* color);
	bool loadSprite(SDL_Renderer* r);
	float vx, vy;
};

