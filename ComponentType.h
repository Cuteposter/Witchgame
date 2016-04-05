#pragma once
#include "EntityTypes.h"

//Perhaps expand this class into an entity property component?
//Might defeat purpose of ECS
class ComponentType :
	public Component
{
public:
	ComponentType(EntityType type) :
		type(type)
	{}

	EntityType type;
};