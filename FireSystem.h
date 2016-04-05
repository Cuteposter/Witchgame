#pragma once
#include "Entity.h"
#include "ComponentEmitter.h"
#include "ComponentPosition.h"
#include "ComponentSprite.h"
#include "Particle.h"
class FireSystem
{
public:
	FireSystem();
	~FireSystem();

	void handle(Entity* e);
};

