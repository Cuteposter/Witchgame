#pragma once
#include <string>
#include <SDL.h>
#include "Component.h"
/*class HelloComponent :
public Component
{
public:
std::string s = "Hello Component!";
};*/


struct ComponentCollision : Component
{
	//Create a cicle struct for circular collision
	struct Circle
	{
		int x, y;
		int r;
	};

	SDL_Rect rect;
	Circle circ;
	bool solid;

	ComponentCollision(int x, int y, int w, int h, bool solid) :
		rect(SDL_Rect{ x, y, w, h }),
		solid(solid)
	{}

	ComponentCollision(int x, int y, int r, bool solid) :
		circ(Circle{ x, y, r }),
		solid(solid)
	{}
};