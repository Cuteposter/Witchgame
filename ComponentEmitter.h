#pragma once
#include <string>
#include <vector>
#include "Component.h"
#include "Particle.h"
/*class HelloComponent :
public Component
{
public:
std::string s = "Hello Component!";
};*/


struct ComponentEmitter : Component
{
	int MAX_PARTICLES;
	std::vector<Particle*> particles;
	int count = 0;
	int wait = 0;

	ComponentEmitter(int m) :
		MAX_PARTICLES(m)
	{}
};