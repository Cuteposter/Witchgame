#pragma once
#include <iostream>
#include <vector>
#include "Entity.h"
#include "HelloComponent.h"
#include "HelloAnother.h"
class TestSystem
{
public:
	TestSystem();
	~TestSystem();
	
	void handle(Entity* e);
};

