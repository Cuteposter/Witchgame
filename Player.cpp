#include "Player.h"
#include "LTexture.h"


Player::Player() {
	//Default constructor
	spr_path = "./res/spr/animwitch.png";
	frame = 0;
	x = 224;
	y = 32;

	ground = false;

	left = false;
	right = false;
	dir = 1;

	slope = 0.0f;
	onSlope = false;
	wasslope = false;
	sdir = 0;

	sndJump = Mix_LoadWAV("./res/snd/jump.wav");
	if (sndJump == NULL)
	{
		printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	}

	idle = new Animation(0, 4, 48, 64);
	walk = new Animation(4, 8, 48, 64);
	jmp = new Animation(12, 1, 48, 64);


	ray1 = new Ray(x, y + 64, 1);
	ray1->setColor(0xFF, 0x00, 0x00, 0xFF);
	ray2 = new Ray(x + 32, y + 63, 1);
	ray2->setColor(0x00, 0xFF, 0x00, 0xFF);
}

/*Player::Player(char* path)
{	
	spr_path = path;
	frame = 0;
	x = 64;
	y = 320;

	ground = true;
	onSlope = false;
	wasslope = false;

	sndJump = Mix_LoadWAV("./res/snd/jump.wav");
	if (sndJump == NULL)
	{
		printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	}

	//Clear out direction array
	left = false;
	right = false;

	ray1 = new Ray(x, y + 64, 1);
	ray1->setColor(0xFF, 0x00, 0x00, 0xFF);
	ray2 = new Ray(x + 32, y + 63, 1);
	ray2->setColor(0x00, 0xFF, 0x00, 0xFF);
}*/

Player::~Player()
{
	printf("Unload sprite %d\n", &sprite);
	//Free loaded image
	sprite.free();

	//Free SFX
	Mix_FreeChunk(sndJump);

	delete idle;
	delete walk;
	delete jmp;

	delete ray1;
	delete ray2;
}

bool Player::loadSprite(SDL_Renderer* r)
{
	if (!sprite.loadFromFile(spr_path, r))
	{
		printf("Unable to load sprite %s!", spr_path);
		return false;
	}
	else{
		printf("Loaded %s with dimensions %d x %d at address 0x%d\n", spr_path, sprite.getWidth(), sprite.getHeight(), &sprite);

		//Set sprite clips
		gSpriteClips[0].x = 0;
		gSpriteClips[0].y = 0;
		gSpriteClips[0].w = 32;
		gSpriteClips[0].h = 64;

		gSpriteClips[1].x = 32;
		gSpriteClips[1].y = 0;
		gSpriteClips[1].w = 32;
		gSpriteClips[1].h = 64;
	}
	partSprite = new ComponentSprite(r, "./res/spr/part_glowball.png", 64, 64);

	return true;
}

void Player::handleEvent(SDL_Event& e, World* w, SDL_Renderer* r)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: 
			if (freemove)
				vy = -MSPEED;

			onLadder = LADDER;
			if (onLadder)
				y -= 8;

			break;
		case SDLK_DOWN:
			if (freemove)
				vy = MSPEED;
			break;
		case SDLK_SPACE:
			if (ground && !freemove)
			{
				if (onLadder)
				{
					onLadder = false;
					break;
				}

				vy = -VSPEED;
				onSlope = false;
				sdir = 0;
				ground = false;
				jump = true;
				Mix_PlayChannel(-1, sndJump, 0);
				printf("AIR\n");
			}
			//printf("JUMP\n");
			break;
		case SDLK_LEFT: 
			//vx = -1;
			left = true;
			right = false;
			dir = -1;

			if (freemove)
				vx = -MSPEED;

			break;
		case SDLK_RIGHT: 
			//vx = 1;
			left = false;
			right = true;
			dir = 1;

			if (freemove)
				vx = MSPEED;

			break;
		case SDLK_z:
			if (!SHOOT)
			{
				SHOOT = true;
				Entity* ent = w->createEntity();
				
				ent->add(new ComponentPosition{ (int)x, (int)y });
				ent->add(partSprite);
				ent->add(new ComponentColor(0, 50, 255, 255));
				ent->add(new ComponentEmitter{ 10 });
				/*ShootyBall* shot = new ShootyBall(room->r, x+32, y+16);
				if(dir == -1)
				{
					shot->angle = 180;
				}
				else if (right)
				{
					shot->angle = 0;
				}
				room->objects.push_back(shot);*/
				animT.start();
			}
			break;
		case SDLK_x:
			x += 1 * dir;
			break;
		case SDLK_e:
			freemove = !freemove;
			vx = 0;
			vy = 0;
			break;
		case SDLK_f:
			fric = !fric;
			break;
		}
	}

	//If a key was released
	if (e.type == SDL_KEYUP)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP:
			if (freemove)
				vy = 0;

			break;
		case SDLK_DOWN:
			if (freemove)
				vy = 0;

			break;
		case SDLK_LEFT:
			left = false;

			if (freemove)
				vx = 0;

			break;
		case SDLK_RIGHT:
			right = false;

			if (freemove)
				vx = 0;

			break;
		case SDLK_SPACE:
			jump = false;
			break;
		}
	}
}

void Player::step()
{	
	xp = x;
	yp = y;

	if (!LADDER)
		onLadder = false;

	if (!freemove)
	{
		stepx();
		stepy();

		//Cycle animation
		if (WALKING)
		{
			if (frame / 10 >= WALKING_ANIMATION_FRAMES)
			{
				frame = 0;
			}
		}

		if (SHOOT)
		{
			if (animT.getTicks() >= 300)
			{
				animT.stop();
				SHOOT = false;
			}
		}

		if (hurtT.getTicks() >= 300)
		{
			HURT = false;
		}
	}
	else
	{
		x += vx;
		y += vy;
	}
	
	ray1->step();
	ray2->step();
}

void Player::stepx()
{
	if (vx != 0)
		frame++;

	if (!(left || right))
		WALKING = false;
	else
		WALKING = true;

	if (HURT && !ground)	//If we're recoiling we can't move
		WALKING = false;

	if (WALKING)
	{
		if (left)
		{
			if (vx > 0)
				vx = -vx / 10;
			else
				vx -= ACCEL;
				//vx = -HSPEED;

			if (vx < -HSPEED)
				vx = -HSPEED;

			if (abs(slope) >= 1.5f && onSlope)
			{
				if (sdir == -1)
					vx = vx/3;
			}
		}

		if (right)
		{
			if (vx < 0)
				vx = -vx / 10;
			else
				vx += ACCEL;
			//vx = -HSPEED;

			if (vx > HSPEED)
				vx = HSPEED;

			if (abs(slope) >= 1.5f && onSlope)
			{
				if (sdir == 1)
					vx = vx/3;
			}
		}

		//In air immediate velocity fall-off
		//if (!ground)
			vx *= 0.9f;


		if (onSlope && sdir == - dir)	//if ((sdir != 0) && (sdir == -dir))
			y += slope*vx;	//Maybe ceil() this?

		/*if (onSlope && !freemove)
		{
			if (sdir == -dir)
			{
				if (dir < 0)
					y = ray2->y2 - 64;
				else if (dir > 0)
					y = ray1->y2 - 64;

				//ground = true;
			}
		}*/
	}

	if (!WALKING)
	{
		if (fric)
		{
			if (vx > 0)
			{
				vx -= FRICTION;
				if (ground)
				{
					if (vx < 0)
					{
						vx = 0;
					}
				}
				else
				{
					if (vx < HSPEED / 3)
					{
						vx = HSPEED / 3;
					}
				}
			}

			if (vx < 0)
			{
				vx += FRICTION;
				if (ground)
				{
					if (vx < 0)
					{
						vx = 0;
					}
				}
				else
				{
					if (vx > -HSPEED / 3)
					{
						vx = -HSPEED / 3;
					}
				}
			}
		}
		else
			vx = 0;

		if(onSlope && sdir == -!dir)	//if ((sdir != 0) && (sdir == -dir))
			y += slope*vx;	//Maybe ceil() this?

		/*if (onSlope && !freemove)
		{
			if (sdir == -dir)
			{
				if (dir < 0)
					y = ray2->y2 - 64;
				else if (dir > 0)
					y = ray1->y2 - 64;

				//ground = true;
			}
		}*/

		//vx = 0;
	}



	x += vx;
	/*if (sdir == -1)	//For steep slopes (BROKEN AND HORRIBLE THERE'S A REASON WHY THEY DON'T DO THIS)
		x = floor(x);
	else if (sdir == 1)
		x = ceil(x);*/
}

void Player::stepy()
{
	//if (sdir == 0)
	//{
		//std::cout << "LOL ";
	
		if (!ground && !onLadder)
		{
			if (vy < 0 && jump == false)
				vy += 1.0f;
			//sdir = 0;
			vy += GRAVITY;
			if (vy > TVSPEED)
				vy = TVSPEED;

			if (!onSlope)
				y += vy;
		}
	//}

	if (ground || onLadder)
	{
		vy = 0;
	}
}

void Player::moveRays()
{
	ray1->x1 = x + 4;
	ray2->x1 = x + 28;

	ray1->y1 = y + 52;
	ray2->y1 = y + 52;
	ray1->step();
	ray2->step();
}

int Player::checkOnGround(SDL_Rect b, bool onlyTop)
{
	SDL_Rect colFeet;
	colFeet.x = x+4;
	colFeet.y = y + 56;
	colFeet.w = 24;
	colFeet.h = 9;

	if (onlyTop)
	{
		int x1 = b.x;
		int y1 = b.y;
		int x2 = b.x + b.w;
		if (SDL_HasIntersection(&colFeet, &b) && vy >= 0 && yp+63 < b.y)	//if (SDL_IntersectRectAndLine(&colFeet, &x1, &y1, &x2, &y1) && vy >= 0 && yp + 63 < b.y)
		{
			y = b.y - 64;
			wasslope = false;
			return BOTTOM;
		}
	}
	else
	{
		if (SDL_HasIntersection(&colFeet, &b) && vy >= 0)
		{
			/*float penetration = b.y - (y + 64);
			//system("cls");
			y += penetration;
			printf("Penetration between floor: %d and player %f: %f\n", b.y, (y + 64) - penetration, penetration);*/

			//Simpler
			y = b.y - 64;
			wasslope = false;
			return BOTTOM;
		}
	}

	return NONE;
}

int Player::checkWall(SDL_Rect b)
{
	SDL_Rect colLeft;
	colLeft.x = x;
	colLeft.y = y + 16;
	colLeft.w = 8;
	colLeft.h = 32;

	SDL_Rect colRight;
	colRight.x = x + 24;
	colRight.y = y + 16;
	colRight.w = 8;
	colRight.h = 32;

	SDL_Rect colHead;
	colHead.x = x+2;
	colHead.y = y;
	colHead.w = 28;
	colHead.h = 8;

	if (SDL_HasIntersection(&colLeft, &b))
	{
		return LEFT;
	}

	if (SDL_HasIntersection(&colRight, &b))
	{
		return RIGHT;
	}

	if (SDL_HasIntersection(&colHead, &b))
	{
		return TOP;
	}
	
	return NONE;
}

int Player::checkLadder(SDL_Rect b)
{
	SDL_Rect colLeft;
	colLeft.x = x;
	colLeft.y = y + 16;
	colLeft.w = 8;
	colLeft.h = 32;

	SDL_Rect colRight;
	colRight.x = x + 24;
	colRight.y = y + 16;
	colRight.w = 8;
	colRight.h = 32;

	SDL_Rect colHead;
	colHead.x = x + 2;
	colHead.y = y;
	colHead.w = 28;
	colHead.h = 8;

	if (SDL_HasIntersection(&colLeft, &b))
	{
		return LEFT;
	}

	if (SDL_HasIntersection(&colRight, &b))
	{
		return RIGHT;
	}

	if (SDL_HasIntersection(&colHead, &b))
	{
		return TOP;
	}

	return NONE;
}

bool Player::onGround()
{
	return ground;
}

void Player::setGround(bool g)
{
	ground = g;
}

void Player::recoil()
{
	if (!HURT)
	{
		vx = 8.0f * -dir;
		vy = -4.0f;
		ground = false;
		HURT = true;
		hurtT.start();
	}
}

void Player::render(SDL_Renderer* r, Camera* cam)
{
	SDL_Rect* currentClip;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (dir == -1)
	{
		flip = SDL_FLIP_HORIZONTAL;
	}

	if (!WALKING && walk->isPlaying())
		walk->stop();

	//sprite.render(r, x, y);
	if (ground)
	{
		if (!WALKING && !SHOOT)
		{
			if (!idle->isPlaying())
				idle->play();

			idle->update();
			currentClip = idle->getFrame();	//&gSpriteClips[0];
			sprite.render(r, x-8 - cam->x, y - cam->y, currentClip, 0.0, (SDL_Point*)0, flip);
		}

		if (WALKING && !SHOOT)
		{
			if (!walk->isPlaying())
				walk->play();

			walk->update();
			currentClip = walk->getFrame(); //&gSpriteClips[frame / 10];
			sprite.render(r, x-8 - cam->x, y - cam->y, currentClip, 0.0, (SDL_Point*)0, flip);
		}
	}
	else
	{
		currentClip = jmp->getFrame();
		sprite.render(r, x-8 - cam->x, y - cam->y, currentClip, 0.0, (SDL_Point*)0, flip);
	}
	
	if (SHOOT)
	{
		SDL_Rect shoot = SDL_Rect{ 64, 0, 32, 64 };
		sprite.render(r, x - cam->x, y - cam->y, &shoot, 0.0, (SDL_Point*)0, flip);
	}

	SDL_Rect rect;
	rect.x = x+4 - cam->x;
	rect.y = y + 64 - cam->y;
	rect.w = 24;
	rect.h = 1;

	SDL_Rect colLeft;
	colLeft.x = x - cam->x;
	colLeft.y = y + 16 - cam->y;
	colLeft.w = 1;
	colLeft.h = 32;

	SDL_Rect colRight;
	colRight.x = x + 31 - cam->x;
	colRight.y = y + 16 - cam->y;
	colRight.w = 1;
	colRight.h = 32;

	SDL_Rect colHead;
	colHead.x = x + 2 - cam->x;
	colHead.y = y - cam->y;
	colHead.w = 28;
	colHead.h = 8;

	/*SDL_SetRenderDrawColor(r, 0x00, 0x00, 0xFF, 0xFF);
	SDL_RenderDrawRect(r, &rect);

	SDL_SetRenderDrawColor(r, 0xFF, 0x00, 0xFF, 0xFF);
	SDL_RenderDrawRect(r, &colLeft);
	SDL_SetRenderDrawColor(r, 0xFF, 0xFF, 0x00, 0xFF);
	SDL_RenderDrawRect(r, &colRight);
	SDL_SetRenderDrawColor(r, 0xFF, 0x44, 0x00, 0xFF);
	SDL_RenderDrawRect(r, &colHead);*/

	/*SDL_Rect colFeetT;
	colFeetT.x = x + 0 - cam->x;
	colFeetT.y = y + 52 - cam->y;
	colFeetT.w = 1;
	colFeetT.h = ray1h;

	SDL_SetRenderDrawColor(r, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRect(r, &colFeetT);

	colFeetT.x = x + 31 - cam->x;
	colFeetT.y = y + 52 - cam->y;
	colFeetT.w = 1;
	colFeetT.h = ray2h;

	SDL_SetRenderDrawColor(r, 0x00, 0xFF, 0x00, 0xFF);
	SDL_RenderDrawRect(r, &colFeetT);*/

	//ray1->render(r, cam);
	//ray2->render(r, cam);
}
