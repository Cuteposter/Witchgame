#include "Room.h"

#include "StarPal.h"
#include "Enemy.h"
#include "ShootyBall.h"

Room::Room()
{
	solids = { Solid(0, 0, 32, 344), Solid(0, 344, 256, 32), Solid(200, 320, 256, 32), Solid(192, 304, 64, 64), Solid(104, 240, 32, 32), Solid(136, 256, 16, 16), Solid(456, 0, 32, 320) };
	objects.push_back(new Enemy(272, 320 - 32));
	objects.push_back(new StarPal());
}


Room::~Room()
{
}

bool Room::load()
{
	bool success = true;

	for (WorldEntity* o : objects)
	{
		success = o->loadSprite(r);
	}

	if (!success)
	{
		exit(-1);
	}

	return success;
}

void Room::step()
{
	//Check for dead objects and delete them
	for (std::vector<WorldEntity*>::iterator i = objects.begin(); i != objects.end();)
	{
		if ((*i)->etype == GET_Enemy)
		{
			std::cout << "Found an enemy\n";
		}

		(*i)->step();
		
		//Do some type specific stuff like this
		ShootyBall* s = dynamic_cast<ShootyBall*>(*i);

		if (s)
		{

		}

		Enemy* e = dynamic_cast<Enemy*>(*i);

		if (e)
		{
			
		}

		if ((*i)->dead)
		{
			WorldEntity* temp = (*i);
			i = objects.erase(i);
			delete temp;
		}
		else
		{
			i++;
		}
	}
}

void Room::render(SDL_Renderer* r)
{
	for (Solid g : solids)
	{
		g.render(r);
	}

	for (WorldEntity* o : objects)
	{
		o->render(r);
	}
}
