#pragma once
#include <vector>
#include "Entity.h"
class World
{
public:
	World();
	~World();
	
	//Counter for IDs (this should never really overflow so it's probably safe)
	int c;

	std::vector<Entity*> entities;
	Entity* createEntity();
};

