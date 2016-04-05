#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <vector>

#include "GameEntities.h"
#include "ComponentSolid.h"

#include "World.h"

#include "Room.h"
#include "Player.h"
#include "Camera.h"
#include "Solid.h"
#include "PEngine.h"

class Game
{
public:
	Game(SDL_Renderer* renderer);
	~Game();

	//Current game room
	Room room;

	//Particle engine
	PEngine* emitter;

	SDL_Renderer* r;
	Player p;
	LTexture bg;

	//Vector of all game object types. Used to create object instances
	//std::vector<WorldObject*> objTypes;
	//void initObjTypes(SDL_Renderer* r);

	void render(Camera* cam);
	void handleEvent(SDL_Event& e, World* w);
	void step(World* w);
};

