#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include <iostream>

#include "Solid.h"
#include "Particle.h"
#include "WorldEntity.h"

/*
Contains solids, enemies, and any other objects that may exist
Manage player in Game class(?)
*/
class Room
{
public:
	Room();
	~Room();

	//Renderer to render to
	SDL_Renderer* r;

	//Vector cotaining room environment collision data
	std::vector<Solid> solids;

	//Vector cotaining current objects in room
	std::vector<WorldEntity*> objects;

	//Load all object sprites
	bool load();

	//Game logic step for all objects in the room
	void step();

	//Check world object collisions
	bool checkCollision(SDL_Rect* a, SDL_Rect* b);

	//Render all objects in the room
	void render(SDL_Renderer* r);
};

