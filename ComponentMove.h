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

class ComponentMove :
	public Component
{
public:
	ComponentMove();
	~ComponentMove();

	int movement;
	int dir;
};