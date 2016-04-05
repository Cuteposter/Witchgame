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


struct ComponentColor : Component
{
	SDL_Color color;

	ComponentColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) :
		color(SDL_Color{ r, g, b, a })
	{}
};