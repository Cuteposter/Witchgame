#pragma once
#include <SDL.h>

#include "Camera.h"

#include "FrameBufferObject.h"

#include "World.h"
#include "Entity.h"
#include "ComponentSprite.h"
#include "ComponentMessage.h"
#include "ComponentAnimation.h"
#include "ComponentPosition.h"
#include "ComponentSolid.h"
#include "ComponentSlope.h"
#include "ComponentEmitter.h"
#include "ComponentColor.h"
#include "ComponentCollision.h"
class RenderSystem
{
public:
	RenderSystem(SDL_Renderer* r);
	~RenderSystem();

	SDL_Renderer* renderer;

	TTF_Font *font = NULL;
	LTexture tTexture;

	LTexture sprFontT;
	SDL_Rect* sprFont[160];

	/*Abstract some OpenGL shit for practicality sake*/
	void drawSetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	void drawLine(int x1, int y1, int x2, int y2);
	void drawRect(SDL_Rect* rect);
	void drawRectFilled(SDL_Rect* rect);

	//Stupid wavey text
	double warble = 0;
	double wave = 0;

	//Lighting engine
	void drawLighting(World* w, FrameBufferObject* fbo);

	void drawStringSpr(int x, int y, std::string str);
	void drawStringSprExt(int x, int y, std::string str, SDL_Color* color);
	int sprstrlen(std::string str);
	void handle(Entity* e, Camera* cam);
};

