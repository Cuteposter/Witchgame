#pragma once
#include <string>
#include "Component.h"
/*class HelloComponent :
	public Component
{
public:
	std::string s = "Hello Component!";
};*/

struct HelloComponent : Component
{
	std::string s = "Hello Component!";
};