#include "FrameBufferObject.h"
#include "Game.h"

FrameBufferObject::FrameBufferObject(int w, int h, int numTextures) {
	width = w;
	height = h;
	assert(numTextures > 0 && numTextures < GL_MAX_COLOR_ATTACHMENTS_EXT);
	textures = std::vector<GLuint>(numTextures);

	//First create the frame buffer
	glGenFramebuffersEXT(1, &frameBufferId);
	bindFrameBuffer(GL_FRAMEBUFFER_EXT);
	//Create the depth buffer
	glGenRenderbuffersEXT(1, &depthBufferId);
	bindRenderBuffer(GL_RENDERBUFFER_EXT);
	//Allocate the depth buffer
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);
	//Attach the depth buffer to the frame buffer
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER_EXT, depthBufferId);

	//Make textures
	for (int i = 0; i < numTextures; i++) {
		//Attach a texture to the frame buffer
		glGenTextures(1, &textures[i]);
		bindTexture(i);
		//Create the actual texture with no data
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		//Attach the texture to the frame buffer
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, GL_TEXTURE_2D, textures[i], 0);
		unbindTexture();
	}

	//Make sure everything worked correctly
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	std::cout << "Frame buffer with size " << width << "x" << height;
	if (!checkStatus(status)) {
		std::cout << " NOT created succesfully\n";
	}
	else {
		std::cout << " created succesfully\n";
	}

	unbindTexture();
	unbindRenderBuffer(GL_RENDERBUFFER_EXT);
	unbindFrameBuffer(GL_FRAMEBUFFER_EXT);
}

FrameBufferObject::~FrameBufferObject() {
	glDeleteTextures(textures.size(), &textures[0]);
	glDeleteRenderbuffersEXT(1, &depthBufferId);
	glDeleteFramebuffersEXT(1, &frameBufferId);
}

void FrameBufferObject::draw(int id) {
	glEnable(GL_TEXTURE_2D);
	//bindTexture(id);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	//glBegin(GL_QUADS);
	//glTexCoord2i(0, 0);
	//glVertex2i(0, height);

	//glTexCoord2i(1, 0);
	//glVertex2i(width, height);

	//glTexCoord2i(1, 1);
	//glVertex2i(width, 0);

	//glTexCoord2i(0, 1);
	//glVertex2i(0, 0);
	
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrtho(0.0, width, height, 0.0, -1.0, 1.0);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	//glBegin(GL_QUADS);
	////glVertexAttrib2f(0, -1, -1);
	//glTexCoord2f(0, 0); glVertex2f(0, 0);
	//glTexCoord2f(1, 0); glVertex2f(width, 0);
	//glTexCoord2f(1, 1); glVertex2f(width, height);
	//glTexCoord2f(0, 1); glVertex2f(0, height);
	//glEnd();

	//glBegin(GL_TRIANGLE_STRIP);
	////glVertexAttrib2f(0, -1, -1);
	//glTexCoord2f(0, 1); glVertex2f(0, 0);
	//glTexCoord2f(0, 0); glVertex2f(0, height);
	//glTexCoord2f(1, 1); glVertex2f(width, 0);
	//glTexCoord2f(1, 0); glVertex2f(width, height);
	//glEnd();

	//TODO: This should be how it's done. Fix it.
	if (quadVAO == 0)
	{
		// Set up vertex data (and buffer(s)) and attribute pointers
		GLfloat vertices[] = {
			// Positions          // Colors           // Texture Coords
			width, 0, 0.0f, 1.0f, 0.0f, 0.0f, 1, 1, // Top Right
			width, height, 0.0f, 0.0f, 1.0f, 0.0f, 1, 0,	// Bottom Right
			0, height, 0.0f, 0.0f, 0.0f, 1.0f, 0, 0, // Bottom Left
			0, 0, 0.0f, 1.0f, 1.0f, 0.0f, 0, 1  // Top Left 
		};
		GLuint indices[] = {  // Note that we start from 0!
			0, 1, 3, // First Triangle
			1, 2, 3  // Second Triangle
		};
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glGenBuffers(1, &quadEBO);

		glBindVertexArray(quadVAO);

		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		// Color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		// TexCoord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0); // Unbind VAO

	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	bindTexture(id);
	glBindVertexArray(quadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	unbindTexture();
}

void FrameBufferObject::bindFrameBuffer(GLenum mode) {
	glBindFramebufferEXT(mode, frameBufferId);
}

void FrameBufferObject::unbindFrameBuffer(GLenum mode) {
	glBindFramebufferEXT(mode, 0);
}

void FrameBufferObject::bindRenderBuffer(GLenum mode) {
	glBindRenderbufferEXT(mode, depthBufferId);
}

void FrameBufferObject::unbindRenderBuffer(GLenum mode) {
	glBindRenderbufferEXT(mode, 0);
}

void FrameBufferObject::bindTexture(int id) {
	assert(id >= 0 && id < textures.size());
	glBindTexture(GL_TEXTURE_2D, textures[id]);
}

void FrameBufferObject::unbindTexture() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void FrameBufferObject::setRenderToTexture(int id) {
	assert(id >= 0 && id < textures.size());
	GLenum bufs[] = { GL_COLOR_ATTACHMENT0_EXT + id };
	glDrawBuffers(1, bufs);
}

void FrameBufferObject::unsetRenderToTexture() {
	std::vector<GLenum> bufs(textures.size());
	for (int i = 0; i < bufs.size(); i++) {
		bufs[i] = GL_COLOR_ATTACHMENT0_EXT + i;
	}
	glDrawBuffers(bufs.size(), &bufs[0]);
}

bool FrameBufferObject::checkStatus(GLenum status) {
	return status == GL_FRAMEBUFFER_COMPLETE_EXT;
}