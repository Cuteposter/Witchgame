#pragma once
#include <SDL.h>
#include "Camera.h"
#include "Entity.h"
#include "ComponentSolid.h"
#include "ComponentSlope.h"
#include "ComponentCollision.h"
#include "ComponentColor.h"
class Ray
{
public:
	Ray(int x, int y, int dir);
	~Ray();

	SDL_Color* c;

	enum Direction
	{
		UP = 0,
		DOWN = 1,
		LEFT = 2,
		RIGHT = 3
	};

	int dir;
	int x1, y1;
	int x2, y2;

	Entity* hit = NULL;
	Entity* cast(std::vector<Entity*>* list);
	void step();
	void render(SDL_Renderer* r, Camera* cam);
	void setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
};

