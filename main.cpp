//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <map>

//TMX parser
#include "lib/tmxparser.h"

//Classes we are using
#include "Game.h"
#include "Player.h"
#include "Background.h"
#include "Menu.h"
#include "Solid.h"
#include "Camera.h"

//ECS testing...
#include "World.h"
#include "Entity.h"

#include "Component.h"
#include "HelloComponent.h"
#include "ComponentSprite.h"
#include "ComponentMessage.h"
#include "ComponentPosition.h"
#include "ComponentSolid.h"
#include "ComponentSlope.h"
#include "ComponentEmitter.h"
#include "ComponentMove.h"
#include "ComponentCollision.h"
#include "ComponentType.h"

#include "TestSystem.h"
#include "RenderSystem.h"
#include "FireSystem.h"
#include "MoveSystem.h"
#include "CollisionSystem.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//const int SCREEN_WIDTH = 1100;
//const int SCREEN_HEIGHT = 735;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Texture renderer for scene manipulation
LTexture tRenderer;

//Globally used font
TTF_Font *gFont = NULL;

//Rendered texture
LTexture gTextTexture;

//The music that will be played
Mix_Music *gMusic = NULL;


//Object pointers
//Player p("./res/spr/witch.png");
Menu* m = new Menu();

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("Witchgame", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				if (!tRenderer.createBlank(gRenderer, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SDL_TEXTUREACCESS_TARGET))
				{
					printf("Failed to create target texture!\n");
					success = false;
				}

				//Set texture as render target for scene manipulation
				SDL_SetRenderTarget(gRenderer, tRenderer.getTexture());

				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				//SDL_RenderSetScale(gRenderer, 2.0f, 2.0f);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}

				//Initialize SDL_mixer
				//if (Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3) != MIX_INIT_OGG | MIX_INIT_MP3)
				//{
					//printf("SDL_mixer could not initialize with ogg and mp3 support! SDL_mixer Error: %s\n", Mix_GetError());
					//success = false;
				//}

				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
					success = false;
				}

				//Initialize SDL_mixer channels
				if (Mix_AllocateChannels(16) < 0)
				{
					printf("SDL_mixer could not initialize channels! SDL_mixer Error: %s\n", Mix_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//p.loadSprite(gRenderer);
	m->loadSprite(gRenderer);

	//Load music
	gMusic = Mix_LoadMUS("./res/mus/artbox.mp3");
	if (gMusic == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	//Open the font
	gFont = TTF_OpenFont("./res/fnt/Inconsolata-Regular.ttf", 14);
	if (gFont == NULL)
	{
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else
	{
		//Render text
		SDL_Color textColor = { 0, 0, 0 };
		TTF_SetFontHinting(gFont, TTF_HINTING_NORMAL);
		if (!gTextTexture.loadFromRenderedText(gRenderer, gFont, "The quick brown fox\njumps over the lazy dog", textColor))
		{
			printf("Failed to render text texture!\n");
			success = false;
		}
	}

	return success;
}

void close()
{
	//Free loaded images
	gTextTexture.free();

	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Free the music
	Mix_FreeMusic(gMusic);
	gMusic = NULL;

	//Quit SDL subsystems
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}


void printTmxMapData(const tmxparser::TmxMap* map)
{
	int depth = 0;

	printf("%s", "<map>");
	depth = 1;
	printf("Version: %s\n", map->version.c_str());
	printf("Orientation: %d\n", map->orientation);
	printf("Width: %u\n", map->width);
	printf("Height: %u\n", map->height);
	printf("TileWidth: %u\n", map->tileWidth);
	printf("TileHeight: %u\n", map->tileHeight);
	printf("BackgroundColor: %s\n", map->backgroundColor.c_str());
	/*printProperties(depth + 1, map->propertyMap);
	printTilesets(depth + 1, map->tilesetCollection);
	printLayers(depth + 1, map->layerCollection);
	printObjectGroups(depth + 1, map->objectGroupCollection);*/
}

int main(int argc, char* args[])
{
	World* w = new World();
	TestSystem* ts = new TestSystem();
	FireSystem* fs = new FireSystem();
	MoveSystem* ms = new MoveSystem();
	CollisionSystem* cs = new CollisionSystem();
	Camera* cam = new Camera(0, 0, 192, 64, 640 - (192*2), 480 - 128);
	Background* back = NULL;

	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			back = new Background("./res/bg/bg32.png", true, gRenderer);

			Entity* ent;
			RenderSystem* rs = new RenderSystem(gRenderer);

			//Ctrl + K then Ctrl + U to uncomment
			//w->createEntity();
			//w->entities.at(0)->add(new HelloComponent);
			//w->entities.at(0)->add(new HelloAnother);

			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 0, 448, 1280, 64 });
			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 1280-32, 0, 32, 480 });
			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 0, 0, 32, 480 });

			////One-way platform
			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 700-64, 320, 128, 8, true });

			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 704, 416, 128, 32, false });
			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 768, 416-32, 64, 32, false });

			////LOL
			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 800, 128, 32, 128 });
			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 832, 128+96, 64, 32 });
			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 896+32, 128, 32, 128 });
			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 896+64, 128, 32, 32 });
			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 896 + 64, 128+96, 32, 32 });
			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 896 + 96, 128, 32, 128 });
			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 896+96+64, 128, 32, 128 });
			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 896+96+96, 128 + 96, 64, 32 });

			//Entity* fire = w->createEntity();
			//fire->add(new ComponentPosition{ 832, 160 });
			//fire->add(new ComponentSprite(gRenderer, "./res/spr/part_glowball.png", 64, 64));
			//fire->add(new ComponentColor(0, 50, 255, 255));
			//fire->add(new ComponentEmitter{ 10 });
			////ent->add(new ComponentCollision{ 112, 176, 32, 32, false });
			////ent->add(new ComponentMove());

			///*ent = w->createEntity();
			//ent->add(new ComponentSolid{ 128, 416, 128, 32 });
			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 160, 384, 64, 32 });
			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 320, 336, 96, 32 });*/

			///*ent = w->createEntity();
			//ent->add(new ComponentSolid{ 80, 431, 128, 16 });
			//ent->add(new ComponentSlope{ true });
			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 208, 240, 128, 191 });
			//ent->add(new ComponentSlope{ true });
			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 336, 240, 128, 207 });
			//ent->add(new ComponentSlope{ false });*/

			///*ent = w->createEntity();
			//ent->add(new ComponentSolid{ 512, 432, 32, 16 });
			//ent->add(new ComponentSlope{ true });
			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 544, 400, 32, 32 });
			//ent->add(new ComponentSlope{ true });
			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 576, 352, 32, 48 });
			//ent->add(new ComponentSlope{ true });*/

			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 128+64+64, 416, 64, 32 });
			//ent->add(new ComponentSlope{ true });
			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 192 + 64 + 64, 352, 64, 64 });
			//ent->add(new ComponentSlope{ true });
			////ent = w->createEntity();
			////ent->add(new ComponentSolid{ 265 - 8 + 64 + 64, 352 - 64, 32, 64 });
			////ent->add(new ComponentSlope{ true });
			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 265 + 64 + 24 + 32, 352, 128, 32 });
			////ent = w->createEntity();
			////ent->add(new ComponentSolid{ 265 + 64 + 24 + 64 + 64, 352 - 64, 32, 64 });
			////ent->add(new ComponentSlope{ false });
			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 265 + 64 + 24 + 64 + 32 + 64, 352, 64, 64 });
			//ent->add(new ComponentSlope{ false });
			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 265 + 64 + 24 + 64 + 32 + 64 + 64, 416, 64, 32 });
			//ent->add(new ComponentSlope{ false });

			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 256 - 128, 400, 64, 32 });
			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 223 - 128, 400, 32, 32 });
			//ent->add(new ComponentSlope{ true });
			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 207 - 128, 432, 16, 16 });
			//ent->add(new ComponentSlope{ true });
			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 320 - 128, 400, 32, 32 });
			//ent->add(new ComponentSlope{ false });
			//ent = w->createEntity();
			//ent->add(new ComponentSolid{ 352 - 128, 432, 16, 16 });
			//ent->add(new ComponentSlope{ false });

			//Entity* sign = w->createEntity();
			//sign->add(new ComponentPosition{ 265 + 64 + 24 + 80, 416 - 96 });
			//sign->add(new ComponentType(SIGN));
			//sign->add(new ComponentSprite(gRenderer, "./res/spr/sign.png", 32, 32));
			//sign->add(new ComponentMessage{ "Fun sign. Fun animates. Wong a buttbaby." });
			//sign->add(new ComponentCollision{ 265 + 64 + 24 + 80 - 16, 416 - 96, 64, 32, false });
			////w->entities.at(1)->add(new ComponentMove());


			//ts->handle(w->entities.at(0));


			//Game control object
			Game game(gRenderer);


			//Test load map from TMX
			printf("Map parser test info\n");

			tmxparser::TmxReturn error;
			tmxparser::TmxMap map;

			// test from file
			error = tmxparser::parseFromFile("./res/map/map.tmx", &map, "./res/tile/debug.png");

			// test from memory
			/*FILE* fp;
			fp = fopen("example.tmx", "rb");
			fseek( fp, 0, SEEK_SET );
			fgetc( fp );
			fseek( fp, 0, SEEK_END );
			size_t size = ftell( fp );
			fseek( fp, 0, SEEK_SET );

			char* charBuffer = new char[size+1];
			size_t read = fread( charBuffer, 1, size, fp );

			t = tmxparser::parseFromMemory(charBuffer, size, &map);*/

			if (!error)
			{
				printTmxMapData(&map);

				tmxparser::TmxRect rect;
				rect.u = 0; rect.v = 0; rect.u2 = 0; rect.v2 = 0;

				int tw = map.tileWidth;
				int th = map.tileHeight;

				int tx = 0;
				int ty = 0;

				int tsw = map.tilesetCollection.at(0).tileWidth;
				int tsh = map.tilesetCollection.at(0).tileHeight;

				//Handle tile layer first
				for (auto layer : map.layerCollection)
				{
					tx = 0;
					ty = 0;
					//printf("%s:\n", layer.name);
					for (auto it : layer.tiles)
					{
						tmxparser::calculateTileCoordinatesUV(map.tilesetCollection[it.tilesetIndex], it.tileFlatIndex, 0.5f, true, rect);
						//printf("Tileset[%u]@Tile[%u]=Rect( (%f, %f)->(%f, %f) )\n", it.tilesetIndex, it.tileFlatIndex, rect.u, rect.v, rect.u2, rect.v2);
						printf("ID:%u, X:%d, Y:%d\n", it.tileFlatIndex, tx, ty);
						
						switch (it.gid-1)
						{
						case -1:
							break;
						case 0:
							game.p.x = tx;
							game.p.y = ty - game.p.sprite.getHeight();
							break;
						case 1:
							ent = w->createEntity();
							ent->add(new ComponentSolid{ tx, ty, tsw, tsh });
							break;
						case 2:
							ent = w->createEntity();
							ent->add(new ComponentSolid{ tx, ty, tsw, tsh });
							ent->add(new ComponentSlope(true));
							break;
						case 3:
							ent = w->createEntity();
							ent->add(new ComponentSolid{ tx, ty, tsw, tsh });
							ent->add(new ComponentSlope(false));
							break;
						case 4:
							ent = w->createEntity();
							ent->add(new ComponentPosition{ tx, ty });
							ent->add(new ComponentSprite(gRenderer, "./res/spr/part_glowball.png", 64, 64));
							ent->add(new ComponentColor(255, 50, 0, 255));
							ent->add(new ComponentEmitter{ 10 });
							break;
						case 5:
							break;
						case 6:
							ent = w->createEntity();
							ent->add(new ComponentPosition{ tx, ty });
							ent->add(new ComponentType(LADDER));
							ent->add(new ComponentSprite(gRenderer, "./res/spr/ladder.png", tw, th));
							ent->add(new ComponentCollision{ tx, ty, tw, th, false });
							break;
						}

						tx += tw;
						if (tx > (map.width-1) * tw)
						{
							tx = 0;
							ty += th;
						}
					}
				}

				//Then handle object layer
				for (auto layer : map.objectGroupCollection)
				{
					//printf("%s:\n", layer.name);
					for (auto obj : layer.objects)
					{
						//tmxparser::calculateTileCoordinatesUV(map.tilesetCollection[it.tilesetIndex], it.tileFlatIndex, 0.5f, true, rect);
						//printf("Tileset[%u]@Tile[%u]=Rect( (%f, %f)->(%f, %f) )\n", it.tilesetIndex, it.tileFlatIndex, rect.u, rect.v, rect.u2, rect.v2);
						printf("ID:%u, X:%d, Y:%d\n", obj.referenceGid, obj.x, obj.y);

						int ox = int(obj.x);
						int oy = int(obj.y) - th;

						int r = 0;
						int g = 0;
						int b = 0;
						int a = 0;

						switch (obj.referenceGid - 1)
						{
						case 4:
							ent = w->createEntity();
							ent->add(new ComponentPosition{ ox, oy });
							ent->add(new ComponentSprite(gRenderer, "./res/spr/part_glowball.png", 64, 64));

							//Get color properties for fire emitter
							for (auto p : obj.propertyMap)
							{
								//std::cout << "Property " << p.first << ": " << p.second << "\n";
								if (p.first == "R")
									r = atoi(p.second.c_str());
								else if (p.first == "G")
									g = atoi(p.second.c_str());
								else if (p.first == "B")
									b = atoi(p.second.c_str());
								else if (p.first == "A")
									a = atoi(p.second.c_str());

							}

							ent->add(new ComponentColor(r, g, b, a));
							ent->add(new ComponentEmitter{ 10 });
							break;
						case 5:
							ent = w->createEntity();
							ent->add(new ComponentPosition{ox, oy});
							ent->add(new ComponentType(SIGN));
							ent->add(new ComponentSprite(gRenderer, "./res/spr/sign.png", 32, 32));

							if (obj.propertyMap.size() > 0)
								ent->add(new ComponentMessage{ obj.propertyMap.at("text") });
							else
								ent->add(new ComponentMessage{ "[NO MESSAGE]" });

							ent->add(new ComponentCollision{ ox, oy, 64, 32, false });
							//w->entities.at(1)->add(new ComponentMove());

							break;
						}
					}
				}
			}
			else
			{
				printf("error parsing file");
			}

			//while(true);

			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//Play the music
			//Mix_PlayMusic(gMusic, -1);

			//The frames per second timer
			LTimer fpsTimer;

			//Start counting frames per second
			int countedFrames = 0;
			fpsTimer.start();

			/*DEBUG TOGGLES*/
			bool h = false;
			int val = 0;

			//While application is running
			while (!quit)
			{
				//Calculate and correct fps
				float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
				if (avgFPS > 2000000)
				{
					avgFPS = 0;
				}

				char fps[16];
				sprintf_s(fps, "%.1f", avgFPS);
				char title[32] = "Witchgame FPS: ";
				strcat_s(title, fps);
				SDL_SetWindowTitle(gWindow, title);

				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
					{
						if (e.key.keysym.sym == SDLK_ESCAPE)
						{
							quit = true;
							//delete rs;
						}

						//if (e.key.keysym.sym == SDLK_q)
						//{
						//	int i = fire->getComponentIndex<ComponentColor>();
						//	if (i > -1)
						//	{
						//		fire->components.erase(fire->components.begin() + i);
						//		fire->add(new ComponentMove());
						//	}
						//}

						if (e.key.keysym.sym == SDLK_w)
						{
							h = !h;	//Toggle debug display
						}

						if (e.key.keysym.sym == SDLK_TAB)
						{
							val++;
							if (val > 5)
								val = 0;
						}

						if (e.key.keysym.sym == SDLK_UP)
						{
							switch (val)
							{
							case 0:
								game.p.HSPEED += 0.1f;
								break;
							case 1:
								game.p.VSPEED += 0.1f;
								break;
							case 2:
								game.p.GRAVITY += 0.01f;
								break;
							case 3:
								game.p.TVSPEED += 0.1f;
								break;
							case 4:
								game.p.FRICTION += 0.1f;
								break;
							case 5:
								game.p.ACCEL += 0.1f;
								break;
							}
						}

						if (e.key.keysym.sym == SDLK_DOWN)
						{
							switch (val)
							{
							case 0:
								game.p.HSPEED -= 0.1f;
								break;
							case 1:
								game.p.VSPEED -= 0.1f;
								break;
							case 2:
								game.p.GRAVITY -= 0.01f;
								break;
							case 3:
								game.p.TVSPEED -= 0.1f;
								break;
							case 4:
								game.p.FRICTION -= 0.1f;
								break;
							case 5:
								game.p.ACCEL -= 0.1f;
								break;
							}
						}
					}

					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					game.handleEvent(e, w);
				}
				//Don't leave dangling pointers. Just clean it up.
				if (quit)
					break;

				//Game logic increment
				
				//Make a reset method for this
				cs->grounded = false;
				cs->colLadder = false;
				cs->aboveSlope = false;

				//game.step(w);
				game.p.step();
				
				for (Entity* en : w->entities)
				{
					fs->handle(en);
					//cs->playerOnSlope(&(game.p), en);
					cs->collidesWithPlayer(&(game.p), en);
					ms->handle(en);
				}

				//game.p.moveRays();
				game.p.moveRays();
				cs->playerAboveSlope(&game.p, &w->entities);
				

				//game.p.ray1->cast(&w->entities);
				//game.p.ray2->cast(&w->entities);

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0x40, 0x40, 0x40, 0xFF);
				SDL_RenderClear(gRenderer);

				/*UPDATE*/
				//Camera
				cam->update((int)game.p.x, (int)game.p.y);
				//Background
				back->update(cam);

				/*if (cs->handle(w->entities.at(1), w->entities.at(5)))
				{
					int i = w->entities.at(5)->getComponentIndex<ComponentColor>();
					if (i > -1)
					{
						w->entities.at(5)->components.erase(w->entities.at(5)->components.begin() + i);
					}
				}*/

				/*RENDER BACKGROUND*/
				back->render(gRenderer);

				/*DEBUG HEIGHT TESTING*/
				if (h)
				{
					for (int i = 0; i < 10; i++)
					{
						SDL_SetRenderDrawColor(gRenderer, 64 + i * 10, 0 + i * 10, 0 + i * 10, 255);
						//SDL_RenderDrawLine(gRenderer, 0, 416 - (32 * i), 640, 416 - (32 * i));
					}
				}

				for (Entity* en : w->entities)
				{
					rs->handle(en, cam);
				}

				game.render(cam);

				/*DEBUG HEIGHT TESTING*/
				if (h)
				{
					for (int i = 0; i < 10; i++)
					{
						rs->drawStringSpr(8, 416 - (32 * i) - 12, std::to_string(32 * i));
					}
				}
				SDL_Color red = SDL_Color{ 255, 0, 0, 255 };
				std::string hp = "` ";
				for (int i = 0; i < game.p.HP; i++)
				{
					hp.append("|");
				}
				rs->drawStringSprExt(4, 20, hp, &red);

				//Update screen
				SDL_SetRenderTarget(gRenderer, NULL);

				/*EXPERIMENTAL PIXEL DOUBLING! MAYBE IGNORE?*/
				SDL_Point screenCenter = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
				//tRenderer.renderScaled(gRenderer, -SCREEN_WIDTH / 4, -SCREEN_HEIGHT / 4, SCREEN_WIDTH * 4, SCREEN_HEIGHT * 4);
				tRenderer.renderScaled(gRenderer, 0, 0, SCREEN_WIDTH*2, SCREEN_HEIGHT*2);

				if (h)
				{
					/*DEBUG PRINTING*/
					char sbuff[16];
					SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
					sprintf_s(sbuff, "%.1f", game.p.x);
					std::string posx(sbuff);
					rs->drawStringSpr(64, 20, "X: " + posx);

					sprintf_s(sbuff, "%.1f", game.p.y);
					std::string posy(sbuff);
					rs->drawStringSpr(64, 36, "Y: " + posy);

					sprintf_s(sbuff, "%.1f", game.p.vx);
					std::string pvx(sbuff);
					rs->drawStringSpr(64, 52, "VX: " + pvx);

					sprintf_s(sbuff, "%.1f", game.p.vy);
					std::string pvy(sbuff);
					rs->drawStringSpr(64, 68, "VY: " + pvy);

					rs->drawStringSpr(64, 84, "GROUND: " + std::to_string(game.p.ground));

					std::string pdir = "-*-";
					if (game.p.dir == -1)
						pdir = "<--";
					if (game.p.dir == 1)
						pdir = "-->";

					rs->drawStringSpr(124, 4, "FACING: " + pdir);

					std::string sdir = "-";
					if (game.p.sdir == 1)
						sdir = "/";
					if (game.p.sdir == -1)
						sdir = "\\";

					rs->drawStringSpr(124, 20, "SLOPE DIR: " + sdir);

					sprintf_s(sbuff, "%.1f", game.p.slope);
					std::string pslope(sbuff);
					rs->drawStringSpr(124, 36, "SLOPE: " + pslope);
					rs->drawStringSpr(124, 52, "ON SLOPE: " + std::to_string(game.p.onSlope));
					rs->drawStringSpr(124, 68, "LADDER: " + std::to_string(game.p.LADDER));

					//Physics
					rs->drawStringSpr(202, 4 + (16 * val), ">");

					sprintf_s(sbuff, "%.1f", game.p.HSPEED);
					std::string phspd(sbuff);
					rs->drawStringSpr(210, 4, "HSPEED: " + phspd);

					sprintf_s(sbuff, "%.1f", game.p.VSPEED);
					std::string pvspd(sbuff);
					rs->drawStringSpr(210, 20, "VSPEED: " + pvspd);

					sprintf_s(sbuff, "%.2f", game.p.GRAVITY);
					std::string grav(sbuff);
					rs->drawStringSpr(210, 36, "GRAVITY: " + grav);

					sprintf_s(sbuff, "%.1f", game.p.TVSPEED);
					std::string tvspd(sbuff);
					rs->drawStringSpr(210, 52, "TERMINAL: " + tvspd);

					sprintf_s(sbuff, "%.1f", game.p.FRICTION);
					std::string fric(sbuff);
					rs->drawStringSpr(210, 68, "FRICTION: " + fric);

					sprintf_s(sbuff, "%.1f", game.p.ACCEL);
					std::string accel(sbuff);
					rs->drawStringSpr(210, 84, "ACCELERATION: " + accel);

					/*DEBUG CAMERA*/
					cam->render(gRenderer);
					rs->drawStringSpr(12, 4, std::to_string(cam->x) + ", " + std::to_string(cam->y));
				}

				SDL_RenderPresent(gRenderer);
				SDL_SetRenderTarget(gRenderer, tRenderer.getTexture());
				++countedFrames;
			}
		}
	}

	//Free resources and close SDL
	close();
	delete back;
	delete cam;
	delete fs;
	delete ts;
	delete w;

	return 0;
}