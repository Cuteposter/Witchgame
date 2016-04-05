#pragma once
#include <string>
#include "Component.h"
/*class HelloComponent :
public Component
{
public:
std::string s = "Hello Component!";
};*/

struct ComponentMessage : Component
{
	std::string s;
	bool show;

	ComponentMessage(std::string s) :
		s(s)
	{}
};