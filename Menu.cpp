#include "Menu.h"


Menu::Menu()
{
	frame = 0;
	x = 0;
	y = 0;
	x2 = 3*96;
	y2 = 84;
	w = 3;
	h = 8;
}


Menu::~Menu()
{
	sprite.free();
}

bool Menu::loadSprite(SDL_Renderer* r)
{
	if (!sprite.loadFromFile("./res/spr/cursor2.png", r))
	{
		printf("Unable to load cursor sprite!");
		return false;
	}
	else{
		printf("Loaded cursor sprite with dimensions %d x %d at address 0x%d\n", sprite.getWidth(), sprite.getHeight(), &sprite);

		//Set sprite clips
		gSpriteClips[0].x = 0;
		gSpriteClips[0].y = 0;
		gSpriteClips[0].w = 96;
		gSpriteClips[0].h = 82;

		gSpriteClips[1].x = 96;
		gSpriteClips[1].y = 0;
		gSpriteClips[1].w = 96;
		gSpriteClips[1].h = 82;
	}

	return true;
}

void Menu::handleEvent(SDL_Event& e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP:
			if(y > 0)
				y -= 84;
			break;
		case SDLK_DOWN:
			if (y < 84 * h)
				y += 84;
			break;
		case SDLK_LEFT:
			if (x > 0)
				x -= 96;
			break;
		case SDLK_RIGHT:
			if (x < 96 * w)
				x += 96;
			break;
		case SDLK_w:
			if (y2 > 0)
				y2 -= 84;
			break;
		case SDLK_s:
			if (y2 < 84 * h)
				y2 += 84;
			break;
		case SDLK_a:
			if (x2 > 0)
				x2 -= 96;
			break;
		case SDLK_d:
			if (x2 < 96 * w)
				x2 += 96;
			break;
		}
	}
}

void Menu::render(SDL_Renderer* r)
{
	//sprite.render(r, x, y);
	sprite.render(r, 355 + x, 175 + y, &gSpriteClips[0]);
	sprite.render(r, 355 + x2, 175 + y2, &gSpriteClips[1]);

	//Go to next frame
	//++frame;

	//Cycle animation
	if (frame / 20 >= ANIMATION_FRAMES)
	{
	frame = 0;
	}
}

