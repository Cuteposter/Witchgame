#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Camera.h"

Camera::Camera(int x, int y, int vx, int vy, int vw, int vh)
{
	this->x = x;
	this->y = y;
	this->vx = vx;
	this->vy = vy;
	view = SDL_Rect{ vx, vy, vw, vh };
}


Camera::~Camera()
{
}

void Camera::update(int px, int py)
{
	if (px < x + vx)
		x = px - vx;

	if (py < y + vy)
		y = py - vy;

	if (px + 32 > x + vx + view.w)
		x = px + 32 - (vx + view.w);

	if (py + 64 > y + vy + view.h)
		y = py + 64 - (vy + view.h);

	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;

	//DEBUG THING
	//if (y + vy*2 + view.h > 480)
		//y = 480 - (vy*2 + view.h);
}

void Camera::render(SDL_Renderer* r)
{
	//SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
	//SDL_Rect pos = { 0, 0, 8, 8 };
	//SDL_Rect v = { vx, vy, view.w, view.h };
	//SDL_RenderFillRect(r, &pos);
	//SDL_RenderDrawRect(r, &v);

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINE_LOOP);
	glVertex3f(vx, vy, 0);
	glVertex3f(vx + view.w - 1, vy, 0);
	glVertex3f(vx + view.w - 1, vy + view.h - 1, 0);
	glVertex3f(vx, vy + view.h- 1, 0);
	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
