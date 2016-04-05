#pragma once
#include <math.h>
#include "PEngine.h"
#include "PEnergy.h"
class ShootyBall :
	public PEngine
{
public:
	ShootyBall(SDL_Renderer* r, int px, int py);
	~ShootyBall();

	void step();
	void render(SDL_Renderer* r, Camera* cam);

	SDL_Rect colRect;

	float x, y, vx, vy;
};

