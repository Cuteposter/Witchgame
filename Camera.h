#pragma once
#include <SDL.h>
class Camera
{
public:
	//Consider making this an entity at some point?
	Camera(int x, int y, int vx, int vy, int vw, int vh);
	~Camera();

	int x = 0;
	int y = 0;

	int vx = 0;
	int vy = 0;
	
	SDL_Rect view;	//Viewport rectangle

	void update(int px, int py);	//Pass in player coordinates to avoid recursive dependency
	void render(SDL_Renderer* r);
};

