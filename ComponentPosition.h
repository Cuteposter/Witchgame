#pragma once
#include <string>
#include "Component.h"
/*class HelloComponent :
public Component
{
public:
std::string s = "Hello Component!";
};*/


struct ComponentPosition : Component
{
	int x;
	int y;

	ComponentPosition(int x, int y) :
		x(x),
		y(y)
	{}
};