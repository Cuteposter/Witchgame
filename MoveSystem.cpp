#include "MoveSystem.h"


MoveSystem::MoveSystem()
{
}


MoveSystem::~MoveSystem()
{
}

void MoveSystem::handle(Entity* e)
{
	ComponentMove* m = e->getComponent<ComponentMove>();
	if (m != NULL)
	{
		//std::cout << "WHY DON'T I WORK!?";
		ComponentPosition* p = e->getComponent<ComponentPosition>();
		ComponentCollision* c = e->getComponent<ComponentCollision>();

		p->x += m->dir;
		if (p->x > 900)
		{
			m->dir = -1;
		}
		if (p->x <= 700)
		{
			m->dir = 1;
		}

		if (c != NULL)
		{
			c->rect.x = p->x;
			c->rect.y = p->y;
		}
	}
}
