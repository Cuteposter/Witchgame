#include "Game.h"

Game::Game(SDL_Renderer* renderer)
{
	r = renderer;

	//initObjTypes(r);

	//emitter = new PEngine(r, "./res/spr/part_glowball.png");
	//room.r = r;
	//room.load();
	p.loadSprite(r);
	bg.loadFromFile("./res/spr/bg.png", r);
	//emitter->loadSprite(r);
}

/*void Game::initObjTypes(SDL_Renderer* r)
{
	int get = GET_Enemy;
	Enemy *e = new Enemy();
	e->loadSprite(r);

	objTypes.insert(objTypes.begin()+get, e);
}*/

void Game::render(Camera * cam)
{
	//bg.render(r, 0, 0);
	//room.render(r);
	p.render(r, cam);
	//emitter->render(r);
}

void Game::step(World* w)
{
	//room.step();
	p.step();
	//emitter->step();
	/*ComponentSolid* g;
	bool grounded = false;
	for (Entity* e : w->entities)
	{
		g = e->getComponent<ComponentSolid>();
		if (g != NULL)
		{
			if (p.checkWall(g->rect))
			{
				p.vx = 0;
			}

			//if (!p.onGround())
			//{
			grounded |= p.checkOnGround(g->rect);
			//}
			else
			{
			if (!p.checkOnGround(*(g.getColRect())))
			{
			p.setGround(false);
			}
			}
		}
	}

	p.setGround(grounded);*/

	//system("cls");
	//printf("ground=%d\nx=%f\nvx=%f\ny=%f\nvy=%f\n", grounded, p.x, p.vx, p.y, p.vy);
}

void Game::handleEvent(SDL_Event& e, World* w)
{
	//Handle player events and give the player object access to the current game room
	p.handleEvent(e, w, r);
}

Game::~Game()
{
	
}
