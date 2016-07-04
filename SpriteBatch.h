#pragma once
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
class SpriteBatch
{
public:
	SpriteBatch();
	~SpriteBatch();

	GLuint VBO, VAO, EBO, scale_vbo;
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	std::vector<float> offsets;
	std::vector<GLfloat> scales;	//Sprite scale

	void genBuffers();
	void setSize(float width, float height);	//Initalizes vertices
	void addVertices(GLfloat* v);
	void addIndices(GLuint* i);
	void pushScale(float scale);
	void pushScale(float wscale, float hscale);
	void defaultScales(float scale=1.0f);
	void render();
};

