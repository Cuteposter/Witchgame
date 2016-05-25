#pragma once

#include "Vector2.h"

#include <math.h>

//The number of points to draw a circle
#define NUM_POINTS 30

class World;

class Light {
public:
	Light(vector2f, float, float);
	Light(vector2f, float, float, float, float, float);

	void tick();

	void draw(World*);
	void drawAlpha(World*);
	void drawShadows(World*);

	vector2f pos;
	float radius;
	float intensity;

	float red, green, blue;

	bool castShadows;

private:
	void init();
};