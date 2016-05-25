#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>

#include "Ray.h"

#include "Camera.h"
#include "World.h"
#include "Entity.h"
#include "Animation.h"

//vvv FIGURE OUT HOW TO HANDEL THIS BETTER vvv
#include "ComponentSprite.h"
#include "ComponentEmitter.h"
#include "ComponentPosition.h"
#include "ComponentColor.h"

#include "LTexture.h"
#include "LTimer.h"


class Player :
	public Entity	//Make the player a special, weird entity(?)
{
public:
	Player();
	//Player(char* path);
	~Player();

	enum ColType
	{
		NONE = 0,
		TOP = 1,
		LEFT = 2,
		RIGHT = 3,
		BOTTOM = 4
	};

	float x, y;		//Current position
	float xp, yp;	//Previous position
	float vx, vy;	//Current x and y velocity
	bool ground, moving;
	SDL_Rect colFeet, colLeft, colRight;

	float slope;
	bool wasslope;
	bool onSlope;
	bool onLadder;
	bool aboveLadder;
	int sdir; 

	LTimer animT;
	LTimer hurtT;

	/*DEBUG*/
	bool freemove = false;
	bool fric = true;

	//Collision rays
	Ray* ray1;
	Ray* ray2;

	//Player stats
	int HP = 6;

	//Player direction
	bool left;
	bool right;
	bool jump;
	int dir;

	//Scene textures
	char* spr_path;
	LTexture sprite;

	//Player states
	bool IDLE = true;
	bool WALKING = false;
	bool HURT = false;
	bool SHOOT = false;
	bool LADDER = false;

	//New animation code
	Animation *idle, *walk, *jmp;

	//Walking animation
	int frame;
	static const int WALKING_ANIMATION_FRAMES = 2;
	SDL_Rect gSpriteClips[WALKING_ANIMATION_FRAMES];
	LTexture gSpriteSheetTexture;

	//Sound effects
	Mix_Chunk *sndJump;

	//Load sprite
	bool loadSprite(SDL_Renderer* r);

	//Takes key presses
	ComponentSprite* partSprite;	//LOL CHANGE THIS LATER
	void handleEvent(SDL_Event& e, World* w, SDL_Renderer* r);

	//Player logic
	void step();
	void moveRays();

	//Abstract the update step for x and y. Easier?
	void stepx();
	void stepy();

	//Collision check
	int checkOnGround(SDL_Rect b, bool onlyTop);
	int checkWall(SDL_Rect b);
	int checkLadder(SDL_Rect b);
	bool onGround();
	void setGround(bool g);

	//Interaction and physics
	void recoil();

	//Render sprite
	void render(SDL_Renderer* r, Camera* cam);

	//Physics values
	float HSPEED = 5.0f;
	float VSPEED = 9.0f;
	float GRAVITY = 0.5f;
	float TVSPEED = 11.5f;
	float ACCEL = 0.7f;
	float FRICTION = 0.9f;

	//DEBUG SHEIT
	int ray1h = 9;
	int ray2h = 9;

private:
	const float MSPEED = 4.0f;

	//float x, y;
	//float vx, vy;
	//bool ground;
	//SDL_Rect colRect;
};

