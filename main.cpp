//Using OpenGL, SDL, SDL_image, standard IO, and strings
#ifdef _WIN32
#include <Windows.h>
#endif
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/vec2.hpp>
#define NO_SDL_GLEXT
#include <SDL_opengl.h>


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

#include "ShaderManager.h"
#include "Shader.h"
#include "FrameBufferObject.h"
#include "SpriteBatch.h"

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

//OpenGL shit
bool initGL();

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


//OpenGL rendering context
SDL_GLContext glcontext;

/*TEST*/
SpriteBatch batch;
LTexture dummy;
CShader* testshader = NULL;

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
		gWindow = SDL_CreateWindow("Witchgame", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
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
				//Create OpenGL context
				if ((glcontext = SDL_GL_CreateContext(gWindow)) == NULL) {
					std::cout << "Error creating OpenGL context: " << SDL_GetError() << std::endl;
					success = false;
				}
				glewInit();

				//Initialize OpenGL
				initGL();


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

bool initGL()
{
	//glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();

	//glOrtho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f);


	bool success = true;
	GLenum error = GL_NO_ERROR;

	//Initialize Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Check for error
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		success = false;
	}

	glOrtho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f);

	//Initialize Modelview Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Check for error
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		success = false;
	}

	//Initialize clear color
	glClearColor(0.f, 0.f, 1.f, 1.f);
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();

	//This enables using alpha channel
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Alpha blending
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glLoadIdentity();

	//Check for error
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		success = false;
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
	//Dispose of OpenGL context
	SDL_GL_DeleteContext(glcontext);

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
	Camera* cam = new Camera(0, 0, 192, 64, 640 - (192 * 2), 480 - 128);
	Background* back = NULL;

	Uint32 lastMouseState = 0;

	Light *l = new Light(vector2f(800, 600), 200, .6f);
	w->lights.push_back(l);

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
			RenderSystem* rs = new RenderSystem(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);

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

			game.fbo = new FrameBufferObject(SCREEN_WIDTH, SCREEN_HEIGHT, 5);

			m_Polygon *poly = new m_Polygon(5);
			poly->setVertex(0, vector2f(100, 100));
			poly->setVertex(1, vector2f(100, 200));
			poly->setVertex(2, vector2f(200, 200));
			poly->setVertex(3, vector2f(250, 150));
			poly->setVertex(4, vector2f(200, 100));
			w->polygons.push_back(poly);

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
						//printf("ID:%u, X:%d, Y:%d\n", it.tileFlatIndex, tx, ty);

						switch (it.gid - 1)
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
							ent->add(new ComponentSolid{ tx, ty, tsw, 8, true });
							break;
						case 5:
							break;
						case 11:
							ent = w->createEntity();
							ent->add(new ComponentPosition{ tx, ty });
							ent->add(new ComponentType(LADDER));
							ent->add(new ComponentSprite(gRenderer, "./res/spr/ladder.png", tw, th));
							ent->add(new ComponentCollision{ tx, ty, tw, th, false });
							ent->add(new ComponentSolid{ tx, ty, tsw, 4, true });
							break;
						default:
							std::cerr << "ERROR: Tile ID" << it.gid - 1 << " not recognized!\n";
							break;
						}

						tx += tw;
						if (tx > (map.width - 1) * tw)
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
						//printf("ID:%u, X:%d, Y:%d\n", obj.referenceGid, obj.x, obj.y);

						int ox = int(obj.x);
						int oy = int(obj.y) - th;

						int r = 0;
						int g = 0;
						int b = 0;
						int a = 0;

						switch (obj.referenceGid - 1)
						{
						case 9:
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
						case 10:
							ent = w->createEntity();
							ent->add(new ComponentPosition{ ox, oy });
							ent->add(new ComponentType(SIGN));
							ent->add(new ComponentSprite(gRenderer, "./res/spr/sign.png", 32, 32));

							if (obj.propertyMap.size() > 0)
								ent->add(new ComponentMessage{ obj.propertyMap.at("text") });
							else
								ent->add(new ComponentMessage{ "[NO MESSAGE]" });

							ent->add(new ComponentCollision{ ox, oy, 64, 32, false });
							//w->entities.at(1)->add(new ComponentMove());

							break;
						case 8:
							ent = w->createEntity();
							ent->add(new ComponentCollision{ ox, oy, atoi(obj.propertyMap.at("radius").c_str()), false });
							break;
						case 13:
							ent = w->createEntity();
							ent->add(new ComponentPosition{ ox, oy });
							ent->add(new ComponentSprite(gRenderer, "./res/spr/part_sparkledrop.png", 32, 32, 1.0f, obj.propertyMap.at("glow").compare("false")));

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
							break;
						default:
							std::cerr << "ERROR: Object ID" << obj.referenceGid - 1 << " not recognized!\n";
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

				//Move light around
				int mx, my;
				Uint32 mouse = SDL_GetMouseState(&mx, &my);
				//std::cout << mx << ", " << my << "\n";
				//When the mouse is clicked add a light
				//Control the top light
				if (w->lights.size() > 0)
					w->lights[w->lights.size() - 1]->pos = vector2f(mx, my);
				for (int i = 0; i < w->lights.size(); i++) {
					w->lights[i]->tick();
				}

				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
					{
						//std::cout << "NEW LIGHT!\n";
						//w->lights.push_back(new Light(vector2f(mx, my), 400, .8f));
					}

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
				cs->belowLadder = false;
				cs->aboveLadder = false;
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

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				rs->drawSetColor(0xFF, 0xFF, 0xFF, 0xFF);

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

				//test FBO here
				game.fbo->bindFrameBuffer(GL_FRAMEBUFFER_EXT);
				glPushAttrib(GL_VIEWPORT_BIT | GL_COLOR_BUFFER_BIT);

				glClearColor(0, 0, 0, 0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				//Draw to secondary texture
				game.fbo->setRenderToTexture(1);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glBlendEquation(GL_FUNC_ADD);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				//Set up matrices
				glm::mat4 projmat;
				glm::mat4 modelmat;
				glm::mat4 orthomat;
				projmat = glm::mat4(1.0);  //loadIdentity
				projmat *= glm::perspective(45.0f, float(SCREEN_WIDTH / SCREEN_HEIGHT), 0.1f, 100.0f);
				modelmat = glm::mat4(1.0);

				orthomat = glm::mat4(1.0);
				orthomat *= glm::ortho<float>(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f, 1.0f, 0.0f);

				GLfloat p[16];
				glGetFloatv(GL_PROJECTION_MATRIX, p);
				GLfloat m[16];
				glGetFloatv(GL_MODELVIEW_MATRIX, m);


				testshader = CShaderManager::GetInstance()->GetShader("./shaders/texture.vert", "./shaders/texture.frag", NULL);

				glUseProgram(testshader->GetProgram());
				//glUniformMatrix4fv(testshader->GetUniformIndex("originalTexture"), , 1, GL_FALSE, glm::value_ptr(Projection));
				glUniformMatrix4fv(testshader->GetUniformIndex("projmat"), 1, GL_FALSE, p);
				glUniformMatrix4fv(testshader->GetUniformIndex("modelmat"), 1, GL_FALSE, m);
				glUniformMatrix4fv(testshader->GetUniformIndex("orthomat"), 1, GL_FALSE, &orthomat[0][0]);
				glUniform1f(testshader->GetUniformIndex("ourTexture"), back->bg.getTextureID());
				glUniform1f(testshader->GetUniformIndex("targetWidth"), game.fbo->width);
				glUniform1f(testshader->GetUniformIndex("fbo"), false);

				back->render(gRenderer);
				//game.render(cam);

				glUseProgram(0);

				//Draw second texture to the first one
				game.fbo->setRenderToTexture(0);

				testshader = CShaderManager::GetInstance()->GetShader("./shaders/hblur.vert", "./shaders/test.frag", NULL);
				glUseProgram(testshader->GetProgram());
				//glUniformMatrix4fv(testshader->GetUniformIndex("originalTexture"), , 1, GL_FALSE, glm::value_ptr(Projection));
				glUniformMatrix4fv(testshader->GetUniformIndex("projmat"), 1, GL_FALSE, p);
				glUniformMatrix4fv(testshader->GetUniformIndex("modelmat"), 1, GL_FALSE, m);
				glUniformMatrix4fv(testshader->GetUniformIndex("orthomat"), 1, GL_FALSE, &orthomat[0][0]);
				glUniform1f(testshader->GetUniformIndex("originalTexture"), game.fbo->textures[1]);
				glUniform1f(testshader->GetUniformIndex("targetWidth"), game.fbo->width);

				game.fbo->draw(1);

				testshader = CShaderManager::GetInstance()->GetShader("./shaders/vblur.vert", "./shaders/test.frag", NULL);
				glUseProgram(testshader->GetProgram());
				glUniformMatrix4fv(testshader->GetUniformIndex("projmat"), 1, GL_FALSE, p);
				glUniformMatrix4fv(testshader->GetUniformIndex("modelmat"), 1, GL_FALSE, m);
				glUniformMatrix4fv(testshader->GetUniformIndex("orthomat"), 1, GL_FALSE, &orthomat[0][0]);
				glUniform1f(testshader->GetUniformIndex("originalTexture"), game.fbo->textures[0]);
				glUniform1f(testshader->GetUniformIndex("targetHeight"), game.fbo->height);

				game.fbo->draw(0);
				glUseProgram(0);

				game.fbo->unsetRenderToTexture();
				game.fbo->unbindFrameBuffer(GL_FRAMEBUFFER_EXT);
				glPopAttrib();

				testshader = CShaderManager::GetInstance()->GetShader("./shaders/texture.vert", "./shaders/texture.frag", NULL);
				glUseProgram(testshader->GetProgram());
				glUniformMatrix4fv(testshader->GetUniformIndex("projmat"), 1, GL_FALSE, p);
				glUniformMatrix4fv(testshader->GetUniformIndex("modelmat"), 1, GL_FALSE, m);
				glUniformMatrix4fv(testshader->GetUniformIndex("orthomat"), 1, GL_FALSE, &orthomat[0][0]);
				glUniform1f(testshader->GetUniformIndex("ourTexture"), game.fbo->textures[0]);
				glUniform1f(testshader->GetUniformIndex("targetWidth"), game.fbo->width);
				glUniform1f(testshader->GetUniformIndex("fbo"), true);
				game.fbo->draw(0);
				glUniform1f(testshader->GetUniformIndex("fbo"), false);
				glUseProgram(0);

				//HORSESHIT
				if (dummy.getTextureID() == 0)
				{
					dummy.loadFromFileGL("./res/spr/part_glowball.png");
					std::cout << "LOADED\n";

					batch.genBuffers();
					//batch.vertices = dummy.vertices;
					//batch.indices = dummy.indices;
				}
				SDL_Rect cliptest = SDL_Rect{ 96*2, 82*1, 96, 82 };

				batch.offsets.clear();
				batch.scales.clear();
				batch.setSize(dummy.getWidth(), dummy.getHeight());
				//glm::vec2 translations[100];
				//int index = 0;
				GLfloat offset = 64;
				for (GLint y = 0; y < 10; y += 1)
				{
					for (GLint x = 0; x < 10; x += 1)
					{
						glm::vec2 translation;
						translation.x = (GLfloat)x * offset + rand() % 4;
						translation.y = (GLfloat)y * offset + rand() % 4;
						//translations[index++] = translation;
						batch.offsets.push_back(translation.x);
						batch.offsets.push_back(translation.y);
					}
				}

				batch.defaultScales(0.5f);

				testshader = CShaderManager::GetInstance()->GetShader("./shaders/texturei.vert", "./shaders/texture.frag", NULL);
				glUseProgram(testshader->GetProgram());

				//for (GLuint i = 0; i < 100; i++)
				//{

				//	GLint location = glGetUniformLocation(testshader->GetProgram(), ("offsets[" + std::to_string(i) + "]").c_str());
				//	std::cout << "LOCATION: " << location << "\n";
				//	glUniform2f(location, translations[i].x, translations[i].y);
				//}

				glUniform2fv(testshader->GetUniformIndex("offsets"), 100, &batch.offsets[0]);

				//glUniformMatrix4fv(testshader->GetUniformIndex("originalTexture"), , 1, GL_FALSE, glm::value_ptr(Projection));
				glUniformMatrix4fv(testshader->GetUniformIndex("projmat"), 1, GL_FALSE, p);
				glUniformMatrix4fv(testshader->GetUniformIndex("modelmat"), 1, GL_FALSE, m);
				glUniformMatrix4fv(testshader->GetUniformIndex("orthomat"), 1, GL_FALSE, &orthomat[0][0]);
				glUniform1f(testshader->GetUniformIndex("ourTexture"), dummy.getTextureID());
				glUniform1f(testshader->GetUniformIndex("targetWidth"), game.fbo->width);
				glBindTexture(GL_TEXTURE_2D, dummy.getTextureID());

				glPushAttrib(GL_COLOR_BUFFER_BIT);	//Save current blending function
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);	//Additive blending
				//batch.render();
				glPopAttrib();	//Restore blending function
				
				//dummy.renderGL(80, 80);

				glUseProgram(0);

				/*RENDER LIGHTING (Experimental, OpenGL. Render code must be changed to finish porting)*/
				//glLoadIdentity();
				//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				//rs->drawLighting(w, game.fbo);

				////Draw the scene objects
				//game.fbo->bindFrameBuffer(GL_FRAMEBUFFER_EXT);
				//game.fbo->setRenderToTexture(0);
				//glPushAttrib(GL_COLOR_BUFFER_BIT);
				//glBlendFunc(GL_DST_COLOR, GL_DST_ALPHA); //Blends the scene objects very nicely with the color of the light

				//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
				//for (int i = 0; i < w->polygons.size(); i++) {
				//	float cols[] = { 1.0f, 0.0f, .5f, 1.0f,
				//		1.0f, 1.0f, .5f, 1.0f,
				//		0.0f, 1.0f, 1.0f, 1.0f,
				//		1.0f, 0.0f, .5f, 1.0f,
				//		1.0f, 0.0f, .5f, 1.0f };
				//	w->polygons[i]->draw(cols);
				//}

				//glPopAttrib();
				//game.fbo->unbindFrameBuffer(GL_FRAMEBUFFER_EXT);
				//game.fbo->draw(0);

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
				//game.fbo->unsetRenderToTexture();

				rs->drawSetColor(0xFF, 0xFF, 0xFF, 0xFF);

				testshader = CShaderManager::GetInstance()->GetShader("./shaders/texture.vert", "./shaders/texture.frag", NULL);

				glUseProgram(testshader->GetProgram());
				//glUniformMatrix4fv(testshader->GetUniformIndex("originalTexture"), , 1, GL_FALSE, glm::value_ptr(Projection));
				glUniformMatrix4fv(testshader->GetUniformIndex("projmat"), 1, GL_FALSE, p);
				glUniformMatrix4fv(testshader->GetUniformIndex("modelmat"), 1, GL_FALSE, m);
				glUniformMatrix4fv(testshader->GetUniformIndex("orthomat"), 1, GL_FALSE, &orthomat[0][0]);
				glUniform1f(testshader->GetUniformIndex("ourTexture"), game.p.sprite.getTextureID());
				glUniform1f(testshader->GetUniformIndex("targetWidth"), game.fbo->width);

				game.render(cam);

				glUseProgram(0);

				//Update screen
				//SDL_SetRenderTarget(gRenderer, NULL);

				/*EXPERIMENTAL PIXEL DOUBLING! MAYBE IGNORE?*/
				SDL_Point screenCenter = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
				//tRenderer.renderScaled(gRenderer, -SCREEN_WIDTH / 4, -SCREEN_HEIGHT / 4, SCREEN_WIDTH * 4, SCREEN_HEIGHT * 4);
				tRenderer.renderScaled(gRenderer, 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2);

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


				//Shader test
				//testshader = CShaderManager::GetInstance()->GetShader("./shaders/opacity.vert", "./shaders/opacity.frag", NULL);
				//glUseProgram(testshader->GetProgram());
				//glUniform1f(testshader->GetUniformIndex("texture"), dummy.getTextureID());
				//glUniform1f(testshader->GetUniformIndex("opacity"), 0.5);
				//dummy.renderGL(100, 100, &cliptest);
				//glUseProgram(0);

				//if (dummy.getTextureID() == 0)
				//{
				//	dummy.loadFromFileGL("./res/spr/cursor2.png");
				//	std::cout << "LOADED\n";
				//}
				//SDL_Rect cliptest = SDL_Rect{ 0, 0, 96, 80 };


				SDL_GL_SwapWindow(gWindow);
				//SDL_RenderPresent(gRenderer);
				//SDL_SetRenderTarget(gRenderer, tRenderer.getTexture());
				++countedFrames;


				lastMouseState = SDL_GetMouseState(NULL, NULL);
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