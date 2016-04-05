#include "PEngine.h"

PEngine::PEngine()
{

}


PEngine::PEngine(SDL_Renderer* renderer, char* s)
{
	//wait = 0;
	r = renderer;
	spr_path = s;
	w = 64;
	h = 64;
	ANIMATION_FRAMES = 1;
	count = 0;
	angle = 0;
}


PEngine::~PEngine()
{
	for (int i = 0; i < 500; i++)
	{
		delete particles[i];
	}

	sprite.free();
}

void PEngine::step()
{
	wait++;
	if (wait == 5 && count < 10)
	{
		particles[count] = new Particle(64, 296);
		particles[count]->sprite = &sprite;
		particles[count]->gSpriteClips = gSpriteClips;

		angle += 36;
		if (angle > 360)
			angle = 0;
		particles[count]->angle = angle;

		wait = 0;
		printf("%d\n%f\n",count++, angle);
		//if (count >= 10)
			//count = 0;
	}


	for (int i = 0; i < count; i++)
	{
		particles[i]->step();
	}
}

void PEngine::render(SDL_Renderer* r, Camera* cam)
{
	//First, clean up dead particles
	for (int i = 0; i < count; i++)
	{
		if (particles[i]->dead)
		{
			delete particles[i];
			particles[i] = new Particle(64, 296);
			particles[i]->sprite = &sprite;
			particles[i]->gSpriteClips = gSpriteClips;

			angle += 36;
			if (angle > 360)
				angle = 0;

			
			particles[i]->angle = angle;
		}
	}

	for (int i = 0; i < count; i++)
	{
		particles[i]->render(r, cam);
	}
}
