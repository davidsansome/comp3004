#include <SDL/SDL.h>

#include "engine.h"
#include "utilities.h"

#ifdef _WIN32

#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	char* argv[1] = {"cw3.exe"};
	return main(1, argv);
}

#endif

int main(int argc, char** argv)
{
	srand(time(0));
	
	/* initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		Utilities::fatal(string("Video initialization failed: ") + SDL_GetError());
	
	Engine engine(800, 600, 16);
	setupWinGLFunctions();
	return engine.exec();
}

