#include "CollisionSystem.h"


CollisionSystem::CollisionSystem()
{
}


CollisionSystem::~CollisionSystem()
{
}

bool CollisionSystem::handle(Entity* e1, Entity* e2)
{
	ComponentCollision* c1 = e1->getComponent<ComponentCollision>();
	ComponentCollision* c2 = e2->getComponent<ComponentCollision>();

	if (c1 != NULL && c2 != NULL)
	{
		if (SDL_HasIntersection(&(c1->rect), &(c2->rect)))
		{
			return true;
		}
	}

	return false;
}

bool CollisionSystem::collidesWithPlayer(Player* p, Entity* e)
{
	if (p->freemove)
		return false;

	SDL_Rect colFeet;
	colFeet.x = p->x + 4;
	colFeet.y = p->y + 56;
	colFeet.w = 24;
	colFeet.h = 9;

	SDL_Rect colFeetAbove;
	colFeet.x = p->x + 4;
	colFeet.y = p->y + 48;
	colFeet.w = 24;
	colFeet.h = 9;

	ComponentCollision* c = e->getComponent<ComponentCollision>();
	if (c != NULL)
	{
		if (SDL_HasIntersection(&colFeet, &(c->rect)))
		{
			ComponentType* t = e->getComponent<ComponentType>();
			if (t != NULL)
			{
				if (t->type == ENEMY)
				{
					p->recoil();
					if (p->HP > 0)
						p->HP--;
				}

				if (t->type == SIGN)
				{
					ComponentMessage* m = e->getComponent<ComponentMessage>();
					m->show = true;
				}
			}
		}
		else
		{
			ComponentType* t = e->getComponent<ComponentType>();
			if (t != NULL)
			{
				if (t->type == SIGN)
				{
					ComponentMessage* m = e->getComponent<ComponentMessage>();
					m->show = false;
				}
			}
		}

		if (SDL_HasIntersection(&colFeet, &(c->rect)))
		{
			ComponentType* t = e->getComponent<ComponentType>();
			if (t != NULL)
			{
				if (t->type == LADDER)
				{
					colLadder = true;
					grounded = true;
				}
			}
		}
		else
		{
			ComponentType* t = e->getComponent<ComponentType>();
			if (t != NULL)
			{
				if (t->type == LADDER)
				{
					colLadder |= false;
					grounded |= false;
				}
			}
		}
	}

	p->LADDER = colLadder;
	//bool grounded = false;

	ComponentSolid* g = e->getComponent<ComponentSolid>();
	if (g != NULL)
	{
		ComponentSlope* s = e->getComponent<ComponentSlope>();
		if (s != NULL)
		{
			if (false)
			{
				SDL_Rect ray1 = SDL_Rect{ p->x + 4, p->y + 52, 1, 20 };
				SDL_Rect ray2 = SDL_Rect{ p->x + 27, p->y + 52, 1, 20 };

				SDL_Rect rect = g->rect;
				int x1 = rect.x;
				int x2 = rect.x + rect.w;

				/*if (p->ground)
				{
				ray1.h = 20;
				ray2.h = 20;
				}*/

				int y1, y2;
				if (s->dir)
				{
					y1 = rect.y + rect.h;
					y2 = rect.y;
				}
				else
				{
					y1 = rect.y;
					y2 = rect.y + rect.h;
				}

				int dx = x2 - x1;
				int dy = y2 - y1;
				float m = (float)dy / (float)dx;
				int b;
				if (s->dir)
					b = rect.y + rect.h;
				else
					b = rect.y;

				p->ray1h = ray1.h;
				p->ray2h = ray2.h;

				bool onSlope = false;

				bool useRay1 = true;
				if (p->dir == 1 && !s->dir)
					useRay1 = true;
				else if (p->dir == -1 && s->dir)
					useRay1 = false;
				else if (p->dir == 1 && s->dir)
					useRay1 = false;
				else if (p->dir == -1 && !s->dir)
					useRay1 = true;

				/*if (p->dir == 1 && !s->dir)
				{
				ray1.w = 1 + abs(p->vx);
				}
				else if (p->dir == -1 && s->dir)
				ray2.w = 1 + abs(p->vx);*/

				if (SDL_IntersectRectAndLine(&ray1, &x1, &y1, &x2, &y2) && useRay1)
				{
					int i = e->getComponentIndex<ComponentColor>();
					if (i == -1)
						e->add(new ComponentColor{ 0xFF, 0x00, 0x00, 0xFF });

					//p->sdir = 1;
					//if (!s->dir)
					//p->sdir = -1;

					//p->slope = m;
					//p->y += m*p->vx;
					//if (p->y > rect.y)
					//p->y = rect.y;

					//p->y = y1 + (p->x - x1) * (rect.w / rect.h) - p->sprite.getHeight();
					float ny;
					int feet = p->y + 64;//ray1.y + ray1.h;
					//if (s->dir)
					//{
					ny = ceil(m * (ray1.x - rect.x) + b);
					//if (feet > ny)
					//{
					if (p->sdir == 0)
					{
						p->y = ny - 64;
						//std::cout << "Moving feet to " << ny << '\n';
					}
					//}
					//}

					grounded = true;
					onSlope = true;
				}
				else if (SDL_IntersectRectAndLine(&ray2, &x1, &y1, &x2, &y2) && !useRay1)
				{
					int i = e->getComponentIndex<ComponentColor>();
					if (i == -1)
						e->add(new ComponentColor{ 0x00, 0xFF, 0x00, 0xFF });

					//p->y += m*p->vx;
					//if (p->y > rect.y)
					//p->y = rect.y;

					int ny;
					int feet = p->y + 64;//ray2.y + ray2.h;
					//if (s->dir)
					//{
					ny = ceil(m * (ray2.x - rect.x) + b);
					//if (feet > ny)
					//{
					if (p->sdir == 0)
					{
						p->y = ny - 64;
						//std::cout << "Moving feet to " << ny - 63 << '\n';
					}
					//}
					//}


					grounded = true;
					onSlope = true;
				}
				else
				{
					int i = e->getComponentIndex<ComponentColor>();
					if (i > -1)
					{
						e->components.erase(e->components.begin() + i);
					}
					grounded |= false;
				}

				if (onSlope)
				{
					p->sdir = 1;
					if (!s->dir)
						p->sdir = -1;

					p->slope = m;
				}
			}
		}
		else
		{
			if (!g->onlyTop)
			{
				int collision = p->checkWall(g->rect);

				if (collision == p->LEFT)
				{
					p->x = g->rect.x + g->rect.w;
					p->vx = 0;
				}
				else if (collision == p->RIGHT)
				{
					p->x = g->rect.x - 32;
					p->vx = 0;
				}
				else if (collision == p->TOP)
				{
					p->y = g->rect.y + g->rect.h;
					if (p->vy < 0)
						p->vy = 0;
				}
			}

			//grounded |= p->checkOnGround(g->rect, g->onlyTop);
		}
	}

	//p->setGround(grounded);
	//if (!p->ground)
		//p->sdir = 0;

	return false;
}

bool CollisionSystem::playerOnSlope(Player* p, Entity* e)
{
	if (p->freemove)
		return false;

	SDL_Rect colFeet;
	colFeet.x = p->x + 4;
	colFeet.y = p->y + 58;
	colFeet.w = 24;
	colFeet.h = 9;

	ComponentSolid* g = e->getComponent<ComponentSolid>();
	if (g != NULL)
	{
		ComponentSlope* s = e->getComponent<ComponentSlope>();
		if (s != NULL)
		{
			SDL_Rect ray1 = SDL_Rect{ floor(p->x) + 4, p->y + 56, 1, 12 };
			SDL_Rect ray2 = SDL_Rect{ floor(p->x) + 27, p->y + 56, 1, 12 };

			SDL_Rect rect = g->rect;
			int x1 = rect.x;
			int x2 = rect.x + rect.w;

			int y1, y2;
			if (s->dir)
			{
				y1 = rect.y + rect.h;
				y2 = rect.y;
			}
			else
			{
				y1 = rect.y;
				y2 = rect.y + rect.h;
			}

			int dx = x2 - x1;
			int dy = y2 - y1;
			float m = (float)dy / (float)dx;
			int b;
			if (s->dir)
				b = rect.y + rect.h;
			else
				b = rect.y;

			p->ray1h = ray1.h;
			p->ray2h = ray2.h;

			bool onSlope = false;

			bool useRay1 = true;
			if (p->dir == 1 && !s->dir)
				useRay1 = true;
			else if (p->dir == -1 && s->dir)
				useRay1 = false;
			else if (p->dir == 1 && s->dir)
				useRay1 = false;
			else if (p->dir == -1 && !s->dir)
				useRay1 = true;
			//if (SDL_IntersectRectAndLine(&ray1, &x1, &y1, &x2, &y2))
			if (SDL_IntersectRectAndLine(&ray1, &x1, &y1, &x2, &y2) && useRay1)
			{
				int i = e->getComponentIndex<ComponentColor>();
				if (i == -1)
					e->add(new ComponentColor{ 0xFF, 0x00, 0x00, 0xFF });

				/*p->sdir = 1;
				if (!s->dir)
				p->sdir = -1;

				p->slope = m;*/
				/*p->y += m*p->vx;
				if (p->y > rect.y)
				p->y = rect.y;*/

				//p->y = y1 + (p->x - x1) * (rect.w / rect.h) - p->sprite.getHeight();
				float ny;
				int feet = p->y + 64;//ray1.y + ray1.h;
				//if (s->dir)
				//{
				ny = ceil(m * (ray1.x - rect.x) + b);
				//if (feet > ny)
				//{
				if (p->sdir == 0)
				//{
				//if (SDL_IntersectRectAndLine(&colFeet, &x1, &y1, &x2, &y2))
					p->y = ny - 64;
					//std::cout << "Moving feet to " << ny << '\n';
				//}
				//}
				//}

				grounded = true;
				onSlope = true;
			}
			else if (SDL_IntersectRectAndLine(&ray2, &x1, &y1, &x2, &y2) && !useRay1)
			{
				int i = e->getComponentIndex<ComponentColor>();
				if (i == -1)
					e->add(new ComponentColor{ 0x00, 0xFF, 0x00, 0xFF });

				/*p->y += m*p->vx;
				if (p->y > rect.y)
				p->y = rect.y;*/

				int ny;
				int feet = p->y + 64;//ray2.y + ray2.h;
				//if (s->dir)
				//{
				ny = ceil(m * (ray2.x - rect.x) + b);
				//if (feet > ny)
				//{
				if (p->sdir == 0)
				//{
				//if (SDL_IntersectRectAndLine(&colFeet, &x1, &y1, &x2, &y2))
					p->y = ny - 64;
					//std::cout << "Moving feet to " << ny - 63 << '\n';
				//}
				//}
				//}


				grounded = true;
				onSlope = true;
			}
			else
			{
				int i = e->getComponentIndex<ComponentColor>();
				if (i > -1)
				{
					e->components.erase(e->components.begin() + i);
				}
				grounded |= false;
			}

			if (onSlope)
			{
				p->sdir = 1;
				if (!s->dir)
					p->sdir = -1;

				p->slope = m;
			}
		}
	}
	//std::cout << grounded << '\n';
	p->setGround(grounded);
	if (!p->ground)
		p->onSlope = false;

	return false;
}

bool CollisionSystem::playerAboveSlope(Player* p, std::vector<Entity*>* list)
{
	if (p->freemove)
		return false;

	Entity* ray1 = p->ray1->cast(list);
	Entity* ray2 = p->ray2->cast(list);

	if (ray1 != NULL && ray2 != NULL)
	{
		ComponentSlope* left = ray1->getComponent<ComponentSlope>();
		ComponentSlope* right = ray2->getComponent<ComponentSlope>();

		//Snap the player 
		int dist = p->TVSPEED;
		if (p->wasslope && !p->ground)
			dist = 1;

		if (left != NULL && right == NULL)
		{
			if (p->ray1->y2 < p->ray2->y2)
			{
				//std::cout << "ABOVE SLOPE RAY 1\n";
				aboveSlope = true;
				if (p->ray1->y2 - (p->y + 56) < dist)
					playerSetSlope(p, p->ray1, ray1);
			}
		}
		else if (left == NULL && right != NULL)
		{
			if (p->ray2->y2 < p->ray1->y2)
			{
				//std::cout << "ABOVE SLOPE RAY 2\n";
				aboveSlope = true;
				if (p->ray2->y2 - (p->y + 56) < dist)
					playerSetSlope(p, p->ray2, ray2);
			}
		}
		else if (left != NULL && right != NULL)
		{
			//std::cout << "ABOVE SLOPE BOTH RAYS\n";
			aboveSlope = true;
			if (p->ray1->y2 < p->ray2->y2)
				if (p->ray1->y2 - (p->y + 56) < dist)
				{
					playerSetSlope(p, p->ray1, ray1);
				}


			if (p->ray2->y2 < p->ray1->y2)
				if (p->ray2->y2 - (p->y + 56) < dist)
				{
					playerSetSlope(p, p->ray2, ray2);
				}
		}
	}

	ComponentSolid* g;
	ComponentSlope* s;
	for (Entity* e : *list)
	{
		g = e->getComponent<ComponentSolid>();
		s = e->getComponent<ComponentSlope>();
		if (g != NULL)
		{
			if (s == NULL)
				grounded |= p->checkOnGround(g->rect, g->onlyTop);
		}
	}

	
	if (!aboveSlope || !grounded)
		p->onSlope = false;

	p->setGround(grounded);


	return false;
}

void CollisionSystem::playerSetSlope(Player* p, Ray* r, Entity* e)
{
	ComponentSolid* solid = e->getComponent<ComponentSolid>();
	ComponentSlope* slope = e->getComponent<ComponentSlope>();

	SDL_Rect rect = solid->rect;
	int x1 = rect.x;
	int x2 = rect.x + rect.w;

	int y1, y2;
	if (slope->dir)
	{
		y1 = rect.y + rect.h;
		y2 = rect.y;
	}
	else
	{
		y1 = rect.y;
		y2 = rect.y + rect.h;
	}

	int dx = x2 - x1;
	int dy = y2 - y1;
	float m = (float)dy / (float)dx;
	int b;
	if (slope->dir)
		b = rect.y + rect.h;
	else
		b = rect.y;

	SDL_Rect ray = SDL_Rect{ r->x1, r->y1, 1, r->y2+1 };	//Change 9999 to level bound maybe
	//std::cout << "Trying to snap...\n";
	if (SDL_IntersectRectAndLine(&ray, &x1, &y1, &x2, &y2))
	{
		/*int i = e->getComponentIndex<ComponentColor>();
		if (i == -1)
			e->add(new ComponentColor{ 0xFF, 0x00, 0x00, 0xFF });*/

		//if (p->yp != p->y)
			p->y = r->y2 - 64;//ny - 64;
			//std::cout << "Snapped!\n";
		grounded = true;
	}

	if (grounded && aboveSlope)
	{
		p->onSlope = true;
		p->sdir = 1;
		if (!slope->dir)
			p->sdir = -1;

		p->slope = m;
	}
}