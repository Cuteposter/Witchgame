#pragma once
#include "Particle.h"
class PEnergy :
	public Particle
{
public:
	PEnergy(float px, float py);
	~PEnergy();

	void step();
	void render(SDL_Renderer* r);
};

