#pragma once
#include <vector>
#include "Entity.h"
#include "Polygon.h"
#include "Lighting.h"
class World
{
public:
	World();
	~World();
	
	//Counter for IDs (this should never really overflow so it's probably safe)
	int c;

	std::vector<Entity*> entities;
	std::vector<Light*> lights;
	std::vector<m_Polygon*> polygons;
	Entity* createEntity();
};

