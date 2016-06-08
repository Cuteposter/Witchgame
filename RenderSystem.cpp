#include "RenderSystem.h"


RenderSystem::RenderSystem(SDL_Renderer* r)
{
	renderer = r;

	//sprFontT.loadFromFile("./res/fnt/font.png", r);
	sprFontT.loadFromFileGL("./res/fnt/font.png");

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

void RenderSystem::drawSetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	glColor4f((float)r/255.0, (float)g/255.0, (float)b/255.0, (float)a/255.0);
}

void RenderSystem::drawRect(SDL_Rect* rect)
{
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINE_LOOP);
	glVertex3f(rect->x, rect->y, 0);
	glVertex3f(rect->x + rect->w-1, rect->y, 0);
	glVertex3f(rect->x + rect->w-1, rect->y + rect->h-1, 0);
	glVertex3f(rect->x, rect->y + rect->h-1, 0);
	glEnd();
}

void RenderSystem::drawRectFilled(SDL_Rect* rect)
{
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glVertex3f(rect->x, rect->y, 0);
	glVertex3f(rect->x + rect->w, rect->y, 0);
	glVertex3f(rect->x + rect->w, rect->y + rect->h, 0);
	glVertex3f(rect->x, rect->y + rect->h, 0);
	glEnd();
}

void RenderSystem::drawLine(int x1, int y1, int x2, int y2)
{
	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(x1, y1, 0);
	glVertex3f(x2, y2, 0);
	glEnd();
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
		//sprFontT.setColor(0x00, 0x00, 0x00);
		//sprFontT.setAlpha(128);
		//sprFontT.render(renderer, x + (6 * i) - adjust + 1, y + wave + 1, c);
		drawSetColor(0x00, 0x00, 0x00, 0x80);
		sprFontT.renderGL(x + (6 * i) - adjust + 1, y + wave + 1, c);
		
		//sprFontT.setColor(0xFF, 0xFF, 0xFF);
		//sprFontT.setAlpha(255);
		//sprFontT.render(renderer, x + (6 * i) - adjust, y + wave, c);
		drawSetColor(0xFF, 0xFF, 0xFF, 0xFF);
		sprFontT.renderGL(x + (6 * i) - adjust, y + wave, c);

		wave = 0;//3 * sin(i+warble);
	}
	//warble += 0.005;
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
		//sprFontT.setColor(0x00, 0x00, 0x00);
		//sprFontT.setAlpha(128);
		//sprFontT.render(renderer, x + (6 * i) - adjust + 1, y + wave + 1, c);
		drawSetColor(0x00, 0x00, 0x00, 0x80);
		sprFontT.renderGL(x + (6 * i) - adjust + 1, y + wave + 1, c);

		//sprFontT.setColor(0xFF, 0xFF, 0xFF);
		//sprFontT.setAlpha(255);
		//sprFontT.render(renderer, x + (6 * i) - adjust, y + wave, c);
		drawSetColor(color->r, color->g, color->b, color->a);
		sprFontT.renderGL(x + (6 * i) - adjust, y + wave, c);

		wave = 0;//3 * sin(warble);
	}
	warble += 0.005;
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

void RenderSystem::drawLighting(World* w, FrameBufferObject* fbo)
{
	//Set up the main fbo
	fbo->bindFrameBuffer(GL_FRAMEBUFFER_EXT);
	glPushAttrib(GL_VIEWPORT_BIT | GL_COLOR_BUFFER_BIT);

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, fbo->width, fbo->height);

	//Draw each light to a secondary texture, then draw that one to the primary
	for (int i = 0; i < w->lights.size(); i++) {
		//Draw to secondary texture
		fbo->setRenderToTexture(1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ZERO);

		//Draw the light alpha
		glColorMask(false, false, false, true);
		w->lights[i]->drawAlpha(w);

		//Draw the light color
		glColorMask(true, true, true, false);
		w->lights[i]->draw(w);

		glColorMask(true, true, true, true);

		//Draw second texture to the first one
		fbo->setRenderToTexture(0);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);
		fbo->draw(1);
	}
	fbo->unsetRenderToTexture();
	fbo->unbindFrameBuffer(GL_FRAMEBUFFER_EXT);
	glPopAttrib();
}

void RenderSystem::handle(Entity* e, Camera* cam)
{
	ComponentSprite* s = e->getComponent<ComponentSprite>();
	ComponentEmitter* emitter = e->getComponent<ComponentEmitter>();
	ComponentPosition* p = e->getComponent<ComponentPosition>();

	drawSetColor(0xFF, 0xFF, 0xFF, 0xFF);

	if (s != NULL && emitter == NULL)
	{
		SDL_Rect currentClip = s->gSpriteClips.at(s->frame / 10);

		//sprite.render(r, x, y);
		//s->sprite.render(renderer, p->x - cam->x, p->y - cam->y, &currentClip);
		s->sprite.renderGL(p->x - cam->x, p->y - cam->y, &currentClip);

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
		//SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
		drawSetColor(0xFF, 0x00, 0x00, 0xFF);
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
		//SDL_SetRenderDrawColor(renderer, 0x40, 0x40, 0x40, 0xFF);
		drawSetColor(0x40, 0x40, 0x40, 0xFF);

		ComponentColor* c = e->getComponent<ComponentColor>();
		if (c != NULL)
			drawSetColor(c->color.r, c->color.g, c->color.b, c->color.a); //SDL_SetRenderDrawColor(renderer, c->color.r, c->color.g, c->color.b, c->color.a);

		SDL_Rect rect = solid->rect;
		SDL_Rect drect = SDL_Rect{ rect.x - cam->x, rect.y - cam->y, rect.w, rect.h };

		ComponentSlope* slope = e->getComponent<ComponentSlope>();
		if (slope !=NULL)
		{
			if (slope->dir)
				drawLine(drect.x, drect.y + drect.h, drect.x + drect.w, drect.y); //SDL_RenderDrawLine(renderer, drect.x, drect.y + drect.h, drect.x + drect.w, drect.y);
			else
				drawLine(drect.x, drect.y, drect.x + drect.w, drect.y + drect.h); //SDL_RenderDrawLine(renderer, drect.x, drect.y, drect.x + drect.w, drect.y + drect.h);

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
				drawSetColor(0x80, 0x40, 0x40, 0x80); //SDL_SetRenderDrawColor(renderer, 0x80, 0x40, 0x40, 0x80);

			//SDL_RenderFillRect(renderer, &drect);
			drawSetColor(0x30, 0x30, 0x30, 0x80);
			drawRectFilled(&drect);

			//SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			drawSetColor(0xFF, 0xFF, 0xFF, 0xFF);
			if (solid->onlyTop)
				drawLine(drect.x, drect.y, drect.x + drect.w - 1, drect.y); //SDL_RenderDrawLine(renderer, drect.x, drect.y, drect.x + drect.w-1, drect.y);

			if (solid->rect.w >= 32 && solid->rect.h >= 32)
			{
				SDL_Rect r32 = { drect.x, drect.y, 33, 33 };
				for (int j = 0; j < drect.h; j += 32)
				{
					for (int i = 0; i < drect.w; i += 32)
					{
						r32.x = drect.x + i;
						r32.y = drect.y + j;
						//SDL_RenderDrawRect(renderer, &r32);
						drawRect(&r32);
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
				drawSetColor(c->color.r, c->color.g, c->color.b, c->color.a); //p->render(renderer, cam, &(c->color));
			//else
				p->render(renderer, cam);
		}
	}
}
