#include "TestSystem.h"


TestSystem::TestSystem()
{
}


TestSystem::~TestSystem()
{
}

void TestSystem::handle(Entity* e)
{
	std::vector<HelloComponent*> hellos = e->get<HelloComponent>();
	for (HelloComponent* h : hellos)
	{
		std::cout << h->s << "\n";
	}

	std::vector<HelloAnother*> hello2 = e->get<HelloAnother>();
	for (HelloAnother* h : hello2)
	{
		std::cout << h->s << "\n";
	}
}
