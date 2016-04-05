#include "Animation.h"

Animation::Animation(int offset, int count, int w, int h, int time, bool loop)
{
	SDL_Rect clip;
	for (int i = 0; i < count; i++)
	{
		clip = SDL_Rect{ (offset*w) + (w*i), 0, w, h };
		clips.push_back(clip);
		timing.push_back(time);
		control.push_back(-1);
	}

	if (loop)
		control.at(count-1) = 0;

	frame = 0;
}


Animation::~Animation()
{
}

void Animation::play()
{
	playing = true;
	std::cout << "NOW PLAYING... \n";
}

void Animation::stop()
{
	playing = false;
	tick = 0;
	frame = 0;
	std::cout << "STOPPED! \n";
}

bool Animation::isPlaying()
{
	return playing;
}

void Animation::update()
{
	if (playing)
	{
		tick++;
		//std::cout << "tick = " << tick << "\nframe = " << frame << "\nframe timing = " << timing.at(frame) << '\n';
		if (tick >= timing.at(frame))
		{
			std::cout << frame << '\n';
			tick = 0;

			if (control.at(frame) != -1)
			{
				//Jump to frame
				frame = control.at(frame);
			}
			else
			{
				if (frame < clips.size()-1)
					frame++;
				else
					playing = false;
			}
		}
	}
}

SDL_Rect* Animation::getFrame()
{
	return &clips.at(frame);
}
