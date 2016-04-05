#include "Ray.h"

Ray::Ray(int x, int y, int dir)
{
	x1 = x;
	y1 = y;

	y2 = y1;
	x2 = x1;

	this->dir = dir;

	if (dir == UP)
		y2 = 0;
	if (dir == DOWN)
		y2 = 9999;	//Change to room height
	if (dir == LEFT)
		x2 = 0;
	if (dir == RIGHT)
		x2 = 9999;	//Change to room width

	setColor(0x80, 0x80, 0x80, 0xFF);
}

Ray::~Ray()
{
	delete c;
}

Entity* Ray::cast(std::vector<Entity*>* list)
{
	if (hit != NULL)
	{
		int i = hit->getComponentIndex<ComponentColor>();
		if (i > -1)
			hit->components.erase(hit->components.begin() + i);
	}

	ComponentSolid* g;
	ComponentSlope* s;
	hit = NULL;
	SDL_Rect hitr;

	//Don't alter the coordinates
	int tx1, tx2, ty1, ty2;

	for (Entity* e : *list)
	{
		tx1 = x1;
		tx2 = x2;
		ty1 = y1;
		ty2 = 9999;

		//std::cout << "Checking if solid..." << e->getID() << '\n';
		g = e->getComponent<ComponentSolid>();
		if (g != NULL)
		{
			s = e->getComponent<ComponentSlope>();
			if (s != NULL)
			{
				SDL_Rect rect = g->rect;
				int sx1 = rect.x;
				int sx2 = rect.x + rect.w;

				int sy1, sy2;
				if (s->dir)
				{
					sy1 = rect.y + rect.h;
					sy2 = rect.y;
				}
				else
				{
					sy1 = rect.y;
					sy2 = rect.y + rect.h;
				}

				int dx = sx2 - sx1;
				int dy = sy2 - sy1;
				float m = (float)dy / (float)dx;
				int b;
				if (s->dir)
					b = rect.y + rect.h;
				else
					b = rect.y;

				SDL_Rect ray = SDL_Rect{ ceil(x1), y1, 1, 9999 };	//Change 9999 to level bound maybe

				if (SDL_IntersectRectAndLine(&ray, &sx1, &sy1, &sx2, &sy2))
				{
					/*int i = e->getComponentIndex<ComponentColor>();
					if (i == -1)
					e->add(new ComponentColor{ 0xFF, 0x00, 0x00, 0xFF });*/

					float ny;
					ny = ceil(m * (x1 - rect.x) + b);
					if (ny < hitr.y)
					{
						hit = e;
						rect.y = ny;
						hitr = rect;
					}

				}
			}
			else
			{
				if (SDL_IntersectRectAndLine(&(g->rect), &tx1, &ty1, &tx2, &ty2))
				{
					if (hit == NULL)
					{
						hitr = g->rect;
						hit = e;
					}
					else
					{
						if (dir == UP)
						{
							if (g->rect.y + g->rect.h > hitr.y + hitr.w)
							{
								hit = e;
								hitr = g->rect;
							}
						}

						if (dir == DOWN)
						{
							if (g->rect.y < hitr.y)
							{
								hit = e;
								hitr = g->rect;
							}
						}

						if (dir == LEFT)
						{
							if (g->rect.x + g->rect.w > hitr.x + hitr.w)
							{
								hit = e;
								hitr = g->rect;
							}
						}

						if (dir == RIGHT)
						{
							if (g->rect.x < hitr.x)
							{
								hit = e;
								hitr = g->rect;
							}
						}
					}
				}
			}
		}
	}

	if (hit != NULL)
	{
		if (dir == UP)
			y2 = hitr.y + hitr.h;

		if (dir == DOWN)
			y2 = hitr.y;

		if (dir == LEFT)
			x2 = hitr.x + hitr.w;

		if (dir == RIGHT)
			x2 = hitr.x;

		int i = hit->getComponentIndex<ComponentColor>();
		if (i == -1)
			hit->add(new ComponentColor{ c->r, c->g, c->b, (Uint8)(c->a / 2) });
	}

	return hit;
}

void Ray::step()
{
	if (dir == UP || dir == DOWN)
		x2 = x1;
	else if (dir == LEFT || dir == RIGHT)
		y2 = y1;
}

void Ray::render(SDL_Renderer* r, Camera* cam)
{
	SDL_SetRenderDrawColor(r, c->r, c->g, c->b, c->a);
	SDL_RenderDrawLine(r, x1 - cam->x, y1 - cam->y, x2 - cam->x, y2 - cam->y);
}

void Ray::setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	delete c;
	c = new SDL_Color{ r, g, b, a };
}