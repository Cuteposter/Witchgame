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

class ComponentAnimation :
	public Component
{
public:
	ComponentAnimation();
	~ComponentAnimation();

	int frame;
	int ANIMATION_FRAMES;
	std::vector<SDL_Rect> gSpriteClips;
};