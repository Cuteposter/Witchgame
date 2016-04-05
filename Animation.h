#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include <iostream>

#include "LTimer.h"
class Animation
{
public:
	Animation(int offset, int count, int w, int h, int time = 6, bool loop = true);
	~Animation();

	std::vector<SDL_Rect> clips;
	std::vector<int> timing;	//in frames
	std::vector<int> control;	//Frame control: if -1 do nothing, otherwise jump to frame number

	bool playing = false;

	int frame, tick;
	int w, h;

	void play();
	void stop();
	bool isPlaying();
	void update();
	SDL_Rect* getFrame();
};

