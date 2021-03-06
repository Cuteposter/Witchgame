#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#define NO_SDL_GLEXT
#include <SDL_opengl.h>

//Texture wrapper class
class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Creates a blank texture with renderer access available
	bool createBlank(SDL_Renderer* gRenderer, int width, int height, SDL_TextureAccess access);

	//Loads image at specified path
	bool loadFromFile(std::string path, SDL_Renderer* gRenderer);
	bool loadFromFileGL(std::string path);

#ifdef _SDL_TTF_H
	//Creates image from font string
	bool loadFromRenderedText(SDL_Renderer* gRenderer, TTF_Font* gFont, std::string textureText, SDL_Color textColor);
#endif

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(SDL_Renderer* r, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void renderGL(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);


	//Renders texture at a given point with given dimensions
	void renderScaled(SDL_Renderer* r, int x, int y, int w, int h, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void renderScaledGL(int x, int y, int w, int h, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Move about the vertices we will pass to the GPU for batch rendering
	void updateVertices(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void updateVerticesScaled(int x, int y, int w, int h, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();

	//Sets texture as target, if it was created as a renderer targetable texture
	SDL_Texture* getTexture();
	GLuint getTextureID();
	
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> in_vertices;
	std::vector<GLuint> indices;

private:
	//The renderer that will render this texture to the screen
	SDL_Renderer* gRenderer;

	GLuint VBO, VAO, EBO;
	
	//The actual hardware texture
	SDL_Texture* mTexture;

	//OpenGL texture
	GLuint TextureID;

	//Image dimensions
	int mWidth;
	int mHeight;
};

