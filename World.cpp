#include "World.h"


World::World()
{
}


World::~World()
{
	for (Entity* e : entities)
	{
		delete e;
	}
}


Entity* World::createEntity()
{
	Entity *e = new Entity(c++);
	entities.push_back(e);
	return e;
}

