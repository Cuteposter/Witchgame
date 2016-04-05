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
	SDL_Rect rect;
	bool solid;

	ComponentCollision(int x, int y, int w, int h, bool solid) :
		rect(SDL_Rect{ x, y, w, h }),
		solid(solid)
	{}
};