#include "WorldEntity.h"


WorldEntity::WorldEntity()
{
}


WorldEntity::~WorldEntity()
{

}

bool WorldEntity::loadSprite(SDL_Renderer* r)
{
	if (!sprite.loadFromFile(spr_path, r))
	{
		//printf("Unable to load sprite %s!", spr_path);
		return false;
	}
	else{
		//printf("Loaded %s with dimensions %d x %d at address 0x%x\n", spr_path, sprite.getWidth(), sprite.getHeight(), &sprite);
	}

	for (int i = 0; i < ANIMATION_FRAMES; i++)
	{
		gSpriteClips.push_back(SDL_Rect{ w*i, 0, w, h });
	}
	printf("Loaded %s with %d frames\n", spr_path, ANIMATION_FRAMES);

	return true;
}

void WorldEntity::step()
{

}

bool WorldEntity::checkCollision(SDL_Rect* b)
{
	SDL_Rect a = SDL_Rect{ x, y, w, h };
	return SDL_HasIntersection(&a, b);
}

void WorldEntity::render(SDL_Renderer* r)
{
	//printf("lol\n");
	//printf("x=%d\ny=%d\npath=%s\nframe=%d\nmax frames=%d\n",x,y,spr_path,frame,ANIMATION_FRAMES);
	SDL_Rect currentClip = gSpriteClips.at(frame / 10);

	//sprite.render(r, x, y);
	sprite.render(r, x, y, &currentClip);

	//Go to next frame
	++frame;

	//Cycle animation
	if (frame / 10 >= ANIMATION_FRAMES)
	{
		frame = 0;
	}
}

int WorldEntity::getWidth()
{
	return w;
}

int WorldEntity::getHeight()
{
	return h;
}
