#pragma once
#include <string>
#include "Component.h"
/*class HelloComponent :
public Component
{
public:
std::string s = "Hello Component!";
};*/

struct HelloAnother : Component
{
	std::string s = "Press w to toggle Debug Mode\nPress e to toggle free move\n";
};