#include "SpriteBatch.h"


SpriteBatch::SpriteBatch()
{
	indices = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};
}


SpriteBatch::~SpriteBatch()
{
}

void SpriteBatch::genBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &scale_vbo);
	glGenBuffers(1, &EBO);
}

void SpriteBatch::setSize(float width, float height)
{

	vertices = {
		// Positions          // Colors           // Texture Coords
		width, 0, 0.0f, 1.0f, 0.0f, 0.0f, 1, 1, // Top Right
		width, height, 0.0f, 0.0f, 1.0f, 0.0f, 1, 0,	// Bottom Right
		0, height, 0.0f, 0.0f, 0.0f, 1.0f, 0, 0, // Bottom Left
		0, 0, 0.0f, 1.0f, 1.0f, 0.0f, 0, 1  // Top Left 
	};

	//in_vertices = {
	//	// Positions          // Colors           // Texture Coords
	//	width, 0, 0.0f, 1.0f, 0.0f, 0.0f, 1, 1, // Top Right
	//	width, height, 0.0f, 0.0f, 1.0f, 0.0f, 1, 0,	// Bottom Right
	//	0, height, 0.0f, 0.0f, 0.0f, 1.0f, 0, 0, // Bottom Left
	//	0, 0, 0.0f, 1.0f, 1.0f, 0.0f, 0, 1  // Top Left 
	//};
}

void SpriteBatch::addVertices(GLfloat* v)
{

}

void SpriteBatch::addIndices(GLuint* i)
{

}

void SpriteBatch::pushScale(float scale)
{
	scales.push_back(scale);
	scales.push_back(scale);
}

void SpriteBatch::pushScale(float wscale, float hscale)
{
	scales.push_back(wscale);
	scales.push_back(hscale);
}

void SpriteBatch::defaultScales(float scale)
{
	for (int i = 0; i < 100; i++)
	{
		pushScale(scale);
	}
}


void SpriteBatch::render()
{
	glBindVertexArray(VAO);

	if (scales.size() == 0)
		defaultScales();

	glBindBuffer(GL_ARRAY_BUFFER, scale_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*scales.size(), &scales[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// Scale attribute
	glBindBuffer(GL_ARRAY_BUFFER, scale_vbo);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);


	glBindVertexArray(0); // Unbind VAO

	glBindVertexArray(VAO);
	//printf("SIZE: %d\n", offsets.size());
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, offsets.size()/2);
	glBindVertexArray(0);
}