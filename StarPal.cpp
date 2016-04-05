#include "StarPal.h"


StarPal::StarPal()
{
	//Default constructor
	spr_path = "./res/spr/part_star.png";
	ANIMATION_FRAMES = 4;
	frame = 0;
	x = 64;
	y = 64;
}

bool StarPal::loadSprite(SDL_Renderer* r)
{
	if (!sprite.loadFromFile(spr_path, r))
	{
		printf("Unable to load sprite %s!", spr_path);
		return false;
	}
	else{
		printf("Loaded %s with dimensions %d x %d at address 0x%d\n", spr_path, sprite.getWidth(), sprite.getHeight(), &sprite);

		//Set sprite clips
		gSpriteClips.push_back(SDL_Rect{ 0, 0, 16, 16 });
		gSpriteClips.push_back(SDL_Rect{ 16, 0, 16, 16 });
		gSpriteClips.push_back(SDL_Rect{ 32, 0, 16, 16 });
		gSpriteClips.push_back(SDL_Rect{ 16, 0, 16, 16 });
	}

	return true;
}

void StarPal::step()
{

}


void StarPal::render(SDL_Renderer* r)
{
	SDL_Rect currentClip = gSpriteClips.at(frame / 3);

	//sprite.render(r, x, y);
	sprite.render(r, x, y, &currentClip);

	//Go to next frame
	++frame;

	//Cycle animation
	if (frame / 3 >= ANIMATION_FRAMES)
	{
		frame = 0;
	}
}

StarPal::~StarPal()
{
	printf("I GOT UNLOADED!\n");
}
