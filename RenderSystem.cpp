#include "RenderSystem.h"


RenderSystem::RenderSystem(SDL_Renderer* r)
{
	renderer = r;

	sprFontT.loadFromFile("./res/fnt/font.png", r);

	int w = 16;
	int h = 16;

	for (int i = 0; i < sprFontT.getHeight() / h; i++)
	{
		for (int j = 0; j < sprFontT.getWidth() / w; j++)
		{
			std::cout << (16 * i) + j << '\n';
			sprFont[(16*i)+j] = new SDL_Rect{ 16 * j, 16 * i, 8, h };
		}
	}

	//Set up font sprite clips

	//Open the font
	font = TTF_OpenFont("./res/fnt/Inconsolata-Regular.ttf", 14);
	if (font == NULL)
	{
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Render text
		SDL_Color textColor = { 0, 0, 0 };
		TTF_SetFontHinting(font, TTF_HINTING_NORMAL);
		if (!tTexture.loadFromRenderedText(r, font, "TEXT", textColor))
		{
			printf("Failed to render text texture!\n");
		}
	}
}


RenderSystem::~RenderSystem()
{
	std::cout << "Free render system memory...\n";
	for (int i = 0; i < 160; i++)
		delete sprFont[i];

	tTexture.free();
	sprFontT.free();
}

void RenderSystem::drawStringSpr(int x, int y, std::string str)
{
	//std::cout << "String to draw char " << string << " at " << x << ", " << y << '\n' << "sprFont[" << string-32 << "]\n";
	int adjust = 0;
	for (int i = 0; i < (int)str.length(); i++)
	{
		//adjust = 0;
		char cur = str.at(i);

		//Font kerning
		if (i > 0)
		{
			if (str.at(i - 1) >= 65 && str.at(i - 1) <= 90)
				adjust -= 1;
			if (str.at(i - 1) == 'I')
				adjust += 4;
			else if (str.at(i-1) == '.' || str.at(i-1) == '!' || str.at(i - 1) == 'i' || str.at(i - 1) == 'l')
				adjust += 3;
			else if (str.at(i - 1) == '\'' || str.at(i - 1) == ',' || str.at(i - 1) == ';' || str.at(i - 1) == 'j')
				adjust += 2;
			else if (str.at(i - 1) == 'r' || str.at(i - 1) == 't' || str.at(i - 1) == 'f' || str.at(i - 1) == 'E' || str.at(i - 1) == 'F' || str.at(i - 1) == 'L' || str.at(i - 1) == 'Z')
				adjust += 1;
			else if (str.at(i - 1) == 'a' || str.at(i - 1) == 'v' || str.at(i - 1) == 'A' || str.at(i - 1) == 'V')
				adjust -= 1;
			else if (str.at(i - 1) == 'M' || str.at(i - 1) == 'W' || str.at(i - 1) == '`')
				adjust -= 2;
			else if (str.at(i - 1) == 'w' || str.at(i - 1) == 'm')
				adjust -= 3;
			/*else
				adjust = 0;*/
		}

		SDL_Rect* c = sprFont[cur - 32];
		sprFontT.setColor(0x00, 0x00, 0x00);
		sprFontT.setAlpha(128);
		sprFontT.render(renderer, x + (6 * i) - adjust + 1, y + 1, c);
		
		sprFontT.setColor(0xFF, 0xFF, 0xFF);
		sprFontT.setAlpha(255);
		sprFontT.render(renderer, x + (6*i) - adjust, y, c);
	}
}

void RenderSystem::drawStringSprExt(int x, int y, std::string str, SDL_Color* color)
{
	//std::cout << "String to draw char " << string << " at " << x << ", " << y << '\n' << "sprFont[" << string-32 << "]\n";
	int adjust = 0;
	for (int i = 0; i < (int)str.length(); i++)
	{
		//adjust = 0;
		char cur = str.at(i);

		//Font kerning
		if (i > 0)
		{
			if (str.at(i - 1) >= 65 && str.at(i - 1) <= 90)
				adjust -= 1;
			if (str.at(i - 1) == 'I')
				adjust += 4;
			else if (str.at(i - 1) == '.' || str.at(i - 1) == '!' || str.at(i - 1) == 'i' || str.at(i - 1) == 'l')
				adjust += 3;
			else if (str.at(i - 1) == '\'' || str.at(i - 1) == ',' || str.at(i - 1) == ';' || str.at(i - 1) == 'j')
				adjust += 2;
			else if (str.at(i - 1) == 'r' || str.at(i - 1) == 't' || str.at(i - 1) == 'f' || str.at(i - 1) == 'E' || str.at(i - 1) == 'F' || str.at(i - 1) == 'L' || str.at(i - 1) == 'Z')
				adjust += 1;
			else if (str.at(i - 1) == 'a' || str.at(i - 1) == 'v' || str.at(i - 1) == 'A' || str.at(i - 1) == 'V')
				adjust -= 1;
			else if (str.at(i - 1) == 'M' || str.at(i - 1) == 'W' || str.at(i - 1) == '`')
				adjust -= 2;
			else if (str.at(i - 1) == 'w' || str.at(i - 1) == 'm')
				adjust -= 3;
			/*else
			adjust = 0;*/
		}

		SDL_Rect* c = sprFont[cur - 32];
		sprFontT.setColor(0x00, 0x00, 0x00);
		sprFontT.setAlpha(128);
		sprFontT.render(renderer, x + (6 * i) - adjust + 1, y + 1, c);

		sprFontT.setColor(color->r, color->g, color->b);
		sprFontT.setAlpha(color->a);
		sprFontT.render(renderer, x + (6 * i) - adjust, y, c);
	}
}

int RenderSystem::sprstrlen(std::string str)
{
	//std::cout << "String to draw char " << string << " at " << x << ", " << y << '\n' << "sprFont[" << string-32 << "]\n";
	int adjust = 0;
	int width = 0;
	for (int i = 0; i < (int)str.length(); i++)
	{
		//adjust = 0;
		char cur = str.at(i);

		//Font kerning
		if (i > 0)
		{
			if (str.at(i - 1) >= 65 && str.at(i - 1) <= 90)
				adjust -= 1;
			if (str.at(i - 1) == 'I')
				adjust += 4;
			else if (str.at(i - 1) == '.' || str.at(i - 1) == '!' || str.at(i - 1) == 'i' || str.at(i - 1) == 'l')
				adjust += 3;
			else if (str.at(i - 1) == '\'' || str.at(i - 1) == ',' || str.at(i - 1) == ';' || str.at(i - 1) == 'j')
				adjust += 2;
			else if (str.at(i - 1) == 'r' || str.at(i - 1) == 't' || str.at(i - 1) == 'f' || str.at(i - 1) == 'E' || str.at(i - 1) == 'F' || str.at(i - 1) == 'L' || str.at(i - 1) == 'Z')
				adjust += 1;
			else if (str.at(i - 1) == 'a' || str.at(i - 1) == 'v' || str.at(i - 1) == 'A' || str.at(i - 1) == 'V')
				adjust -= 1;
			else if (str.at(i - 1) == 'M' || str.at(i - 1) == 'W' || str.at(i - 1) == '`')
				adjust -= 2;
			else if (str.at(i - 1) == 'w' || str.at(i - 1) == 'm')
				adjust -= 3;
			/*else
			adjust = 0;*/
		}

		width += 6;
	}

	return width - adjust;
}

void RenderSystem::handle(Entity* e, Camera* cam)
{
	ComponentSprite* s = e->getComponent<ComponentSprite>();
	ComponentEmitter* emitter = e->getComponent<ComponentEmitter>();
	ComponentPosition* p = e->getComponent<ComponentPosition>();

	if (s != NULL && emitter == NULL)
	{
		SDL_Rect currentClip = s->gSpriteClips.at(s->frame / 10);

		//sprite.render(r, x, y);
		s->sprite.render(renderer, p->x - cam->x, p->y - cam->y, &currentClip);

		//Go to next frame
		s->frame++;

		//Cycle animation
		if (s->frame / 3 >= s->ANIMATION_FRAMES)
		{
			s->frame = 0;
		}
	}

	ComponentCollision* col = e->getComponent<ComponentCollision>();
	if (col != NULL)
	{
		SDL_Rect colrect = SDL_Rect{ col->rect.x - cam->x, col->rect.y - cam->y, col->rect.w, col->rect.h };
		SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
		//SDL_RenderDrawRect(renderer, &colrect);
	}

	ComponentMessage* m = e->getComponent<ComponentMessage>();
	if (m != NULL)
	{
		if (m->show)
			if (col != NULL)
				drawStringSpr(col->rect.x - cam->x + (col->rect.w / 2) - (sprstrlen(m->s) / 2), col->rect.y - cam->y - 48, m->s);
			else
				drawStringSpr(p->x - cam->x + (sprstrlen(m->s) / 2), p->y - cam->y - 16, m->s);
	}

	ComponentSolid* solid = e->getComponent<ComponentSolid>();
	if (solid != NULL)
	{
		SDL_SetRenderDrawColor(renderer, 0x40, 0x40, 0x40, 0xFF);

		ComponentColor* c = e->getComponent<ComponentColor>();
		if (c != NULL)
			SDL_SetRenderDrawColor(renderer, c->color.r, c->color.g, c->color.b, c->color.a);

		SDL_Rect rect = solid->rect;
		SDL_Rect drect = SDL_Rect{ rect.x - cam->x, rect.y - cam->y, rect.w, rect.h };

		ComponentSlope* slope = e->getComponent<ComponentSlope>();
		if (slope !=NULL)
		{
			if (slope->dir)
				SDL_RenderDrawLine(renderer, drect.x, drect.y + drect.h, drect.x + drect.w, drect.y);
			else
				SDL_RenderDrawLine(renderer, drect.x, drect.y, drect.x + drect.w, drect.y + drect.h);

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
			

			char sbuff[16];
			SDL_Color textColor = { 0xFF, 0xFF, 0xFF, 0xFF };
			if (c != NULL)
				textColor = { c->color.r, c->color.g, c->color.b, 0xFF };

			sprintf_s(sbuff, "%.1f", m);
			std::string mslope(sbuff);
			//std::cout << mslope;
			//drawStringSprExt(rect.x - sprstrlen("slope:" + mslope), rect.y, "slope:" + mslope, &textColor);
			//tTexture.loadFromRenderedText(renderer, font, "slope: " + mslope, textColor);
			//tTexture.render(renderer, rect.x - tTexture.getWidth(), rect.y - tTexture.getHeight());
		}
		else
		{
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			if (solid->onlyTop)
				SDL_SetRenderDrawColor(renderer, 0x80, 0x40, 0x40, 0x80);

			SDL_RenderFillRect(renderer, &drect);

			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			if (solid->onlyTop)
				SDL_RenderDrawLine(renderer, drect.x, drect.y, drect.x + drect.w-1, drect.y);

			if (solid->rect.w >= 32 && solid->rect.h >= 32)
			{
				SDL_Rect r32 = { drect.x, drect.y, 33, 33 };
				for (int j = 0; j < drect.h; j += 32)
				{
					for (int i = 0; i < drect.w; i += 32)
					{
						r32.x = drect.x + i;
						r32.y = drect.y + j;
						SDL_RenderDrawRect(renderer, &r32);
					}
				}
			}
		}
	}

	//ComponentEmitter* emitter = e->getComponent<ComponentEmitter>();
	if (emitter != NULL)
	{
		ComponentColor* c = e->getComponent<ComponentColor>();
		for (Particle* p : emitter->particles)
		{
			if (c != NULL)
				p->render(renderer, cam, &(c->color));
			else
				p->render(renderer, cam);
		}
	}
}
