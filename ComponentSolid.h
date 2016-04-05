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


struct ComponentSolid : Component
{
	SDL_Rect rect;
	bool onlyTop = false;

	ComponentSolid(int x, int y, int w, int h) :
		rect(SDL_Rect{ x, y, w, h })
	{}

	ComponentSolid(int x, int y, int w, int h, bool t) :
		rect(SDL_Rect{ x, y, w, h }),
		onlyTop(t)
	{}
};