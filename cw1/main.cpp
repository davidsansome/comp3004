#include <SDL/SDL.h>
#include <iostream>
#include <stdlib.h>

#include "engine.h"

int main(int argc, char** argv)
{
	/* initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cerr << "Video initialization failed: " << SDL_GetError() << endl;
		exit(1);
	}

	Engine engine(640, 480, 16);
	return engine.exec();
}

