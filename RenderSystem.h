#pragma once
#include <SDL.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

#include "ShaderManager.h"
#include "Shader.h"
#include "FrameBufferObject.h"
#include "SpriteBatch.h";

class RenderSystem
{
public:
	RenderSystem(SDL_Renderer* r, int width, int height);
	~RenderSystem();

	SDL_Renderer* renderer;

	//Set up matrices
	glm::mat4 projmat;
	glm::mat4 modelmat;
	glm::mat4 orthomat;

	GLfloat pm[16];
	GLfloat mm[16];

	/*SHADERS*/
	CShader* tshader = NULL;	//Basic texture shader
	CShader* tshader_i = NULL;	//Instanced basic texture shader
	CShader* gaussh = NULL;
	CShader* gaussv = NULL;

	/*FRAMEBUFFER*/
	FrameBufferObject* fbo = NULL;

	/*SPRITE BATCHER*/
	SpriteBatch batch;

	TTF_Font *font = NULL;
	LTexture tTexture;

	LTexture sprFontT;
	SDL_Rect* sprFont[160];

	/*Abstract some OpenGL shit for practicality sake*/
	void drawSetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	void drawLine(int x1, int y1, int x2, int y2);
	void drawRect(SDL_Rect* rect);
	void drawRectFilled(SDL_Rect* rect);
	void drawCircleFilled(int x, int y, int r);

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

