#include "FireSystem.h"


FireSystem::FireSystem()
{
}


FireSystem::~FireSystem()
{
}

void FireSystem::handle(Entity* e)
{
	ComponentEmitter* emitter = e->getComponent<ComponentEmitter>();
	if (emitter != NULL)
	{
		ComponentPosition* pos = e->getComponent<ComponentPosition>();
		ComponentSprite* sprite = e->getComponent<ComponentSprite>();

		//First, clean up dead particles
		for (int i = 0; i < emitter->count; i++)
		{
			if (emitter->particles.at(i)->dead)
			{
				//std::cout << "Killing dead particle at " << i << '\n';
				delete emitter->particles.at(i);
				emitter->particles.erase(emitter->particles.begin() + i);
				emitter->particles.push_back(new Particle(pos->x, pos->y));
				emitter->particles.at(emitter->count - 1)->sprite = &(sprite->sprite);
				emitter->particles.at(emitter->count - 1)->gSpriteClips = sprite->gSpriteClips;
				//std::cout << "Replaced dead particle at " << i << '\n';
			}
		}

		emitter->wait++;
		if (emitter->wait >= 5)
		{
			if (emitter->count < emitter->MAX_PARTICLES)
			{
				//std::cout << "Adding particle at " << emitter->count << '\n';
				emitter->particles.push_back(new Particle(pos->x, pos->y));
				emitter->particles.at(emitter->count)->sprite = &(sprite->sprite);
				emitter->particles.at(emitter->count)->gSpriteClips = sprite->gSpriteClips;
				emitter->count++;
			}
			emitter->wait = 0;
		}

		//std::cout << "Wait " << emitter->wait << " Count " << emitter->count << '\n';

		//ComponentPosition* pos = e->getComponent<ComponentPosition>();
		for (Particle* p : emitter->particles)
		{
			p->step();
		}
	}
}