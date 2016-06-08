#include "ComponentSprite.h"

ComponentSprite::ComponentSprite(SDL_Renderer *r, char* path, int w, int h)
{
	//bool success = sprite.loadFromFile(path, r);
	bool success = sprite.loadFromFileGL(path);
	if (!success)
		printf("Failed to load %s!\n", path);

	ANIMATION_FRAMES = sprite.getWidth() / w;
	for (int i = 0; i < ANIMATION_FRAMES; i++)
	{
		gSpriteClips.push_back(SDL_Rect{ w*i, 0, w, h });
	}
	printf("Loaded %s with %d frames\n", path, ANIMATION_FRAMES);
}

ComponentSprite::~ComponentSprite()
{
	
}