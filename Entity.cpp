#include "Entity.h"


Entity::Entity()
{
	id = -1;
}

Entity::Entity(int eid)
{
	id = eid;
}


Entity::~Entity()
{
}

int Entity::getID()
{
	return id;
}

void Entity::add(Component* c)
{
	components.push_back(c);
}
