#pragma once
#include "WorldEntity.h"
#include "Particle.h"
#include "Camera.h"
class PEngine :
	public WorldEntity
{
public:
	PEngine();
	PEngine(SDL_Renderer* r, char* s);
	~PEngine();

	SDL_Renderer* r;

	//Vector cotaining particles
	//arbitrary 500 particles
	Particle* particles[500];
	int count;
	int wait;

	virtual void step();
	virtual void render(SDL_Renderer* r, Camera* cam);

	double angle;
};

