#pragma once
#include <string>
#include "Component.h"
/*class HelloComponent :
public Component
{
public:
std::string s = "Hello Component!";
};*/


struct ComponentSlope : Component
{
	bool dir;	//false - top to bottom, true - bottom to top

	ComponentSlope(bool dir) :
		dir(dir)
	{}
};