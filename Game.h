#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <vector>

#include "FrameBufferObject.h"

#include "GameEntities.h"
#include "ComponentSolid.h"

#include "World.h"

#include "Room.h"
#include "Player.h"
#include "Camera.h"
#include "Solid.h"
#include "PEngine.h"

#ifdef _WIN32
#include <Windows.h>
#endif
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#define NO_SDL_GLEXT
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>

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
	FrameBufferObject* fbo;
	Player p;
	LTexture bg;

	//Vector of all game object types. Used to create object instances
	//std::vector<WorldObject*> objTypes;
	//void initObjTypes(SDL_Renderer* r);

	void render(Camera* cam);
	void handleEvent(SDL_Event& e, World* w);
	void step(World* w);
};

