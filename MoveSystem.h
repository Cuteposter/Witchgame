#pragma once
#include "Entity.h"
#include "ComponentMove.h"
#include "ComponentPosition.h"
#include "ComponentCollision.h"
class MoveSystem
{
public:
	MoveSystem();
	~MoveSystem();

	void handle(Entity* e);
};

