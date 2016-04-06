#pragma once
#include <vector>
#include "Player.h"
#include "Entity.h"
#include "ComponentMessage.h"
#include "ComponentPosition.h"
#include "ComponentCollision.h"
#include "ComponentSolid.h"
#include "ComponentSlope.h"
#include "ComponentType.h"

#include "ComponentColor.h"
class CollisionSystem
{
public:
	CollisionSystem();
	~CollisionSystem();

	bool grounded = false;
	bool colLadder = false;
	bool aboveSlope = false;

	bool handle(Entity* e1, Entity* e2);
	bool collidesWithPlayer(Player* p, Entity* e);
	bool playerOnSlope(Player* p, Entity* e);
	bool playerAboveSlope(Player* p, std::vector<Entity*>* list);
	void playerSetSlope(Player* p, Ray* r, Entity* e);
};

