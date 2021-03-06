#include "Background.h"


Background::Background(char* path, bool parallax, SDL_Renderer* renderer)
{
	//if (bg.loadFromFile(path, renderer))
	if (bg.loadFromFileGL(path))
		printf("Loaded background from: %s\n", path);

	this->parallax = parallax;
}


Background::~Background()
{
}

void Background::update(Camera* cam)
{
	if (parallax)
	{
		x += xp - cam->x / 2;
		y += yp - cam->y / 2;

		xp = cam->x / 2;
		yp = cam->y / 2;

		if (x < -bg.getWidth())
			x += bg.getWidth();

		if (x > bg.getWidth())
			x -= bg.getWidth();

		if (y < -bg.getHeight())
			y += bg.getHeight();

		if (y > bg.getHeight())
			y -= bg.getHeight();
	}
}

void Background::render(SDL_Renderer* r)
{
	//if (parallax)
	//{
	//	//Draw 9 times to create illusion of seamless background
	//	bg.render(r, x, y - bg.getHeight());
	//	bg.render(r, x - bg.getWidth(), y - bg.getHeight());
	//	bg.render(r, x + bg.getWidth(), y - bg.getHeight());

	//	bg.render(r, x, y);
	//	bg.render(r, x - bg.getWidth(), y);
	//	bg.render(r, x + bg.getWidth(), y);

	//	bg.render(r, x, y + bg.getHeight());
	//	bg.render(r, x, y - bg.getHeight());
	//	bg.render(r, x - bg.getWidth(), y + bg.getHeight());
	//	bg.render(r, x + bg.getWidth(), y + bg.getHeight());
	//}
	//else
	//{
	//	//Static background
	//	bg.render(r, 0, 0);
	//}

	if (parallax)
	{
		//Draw 9 times to create illusion of seamless background
		bg.renderGL(x, y - bg.getHeight());
		bg.renderGL(x - bg.getWidth(), y - bg.getHeight());
		bg.renderGL(x + bg.getWidth(), y - bg.getHeight());

		bg.renderGL(x, y);
		bg.renderGL(x - bg.getWidth(), y);
		bg.renderGL(x + bg.getWidth(), y);

		bg.renderGL(x, y + bg.getHeight());
		bg.renderGL(x, y - bg.getHeight());
		bg.renderGL(x - bg.getWidth(), y + bg.getHeight());
		bg.renderGL(x + bg.getWidth(), y + bg.getHeight());
	}
	else
	{
		//Static background
		SDL_Rect cliptest = SDL_Rect{ 0, 0, bg.getWidth(), bg.getHeight() };
		bg.renderGL(0, 0, &cliptest);
	}
}