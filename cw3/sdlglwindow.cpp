#include <iostream>
#include <stdlib.h>

#include "sdlglwindow.h"
#include "utilities.h"

#define DESIRED_FPS 60.0

// Much of the init stuff in this constructor was lifted
// from NeHe's lesson02
SDLGLWindow::SDLGLWindow(int width, int height, int depth)
	: m_done(false),
	  m_width(width),
	  m_height(height),
	  m_depth(depth),
	  m_timeElapsed(0)
{
	/* Fetch the video info */
	const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();

	if (!videoInfo)
		Utilities::fatal(string("Video query failed: ") + SDL_GetError());

	/* the flags to pass to SDL_SetVideoMode */
	m_videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
	m_videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
	m_videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
	m_videoFlags |= SDL_RESIZABLE;       /* Enable window resizing */

	/* This checks to see if surfaces can be stored in memory */
	if (videoInfo->hw_available)
		m_videoFlags |= SDL_HWSURFACE;
	else
		m_videoFlags |= SDL_SWSURFACE;

	/* This checks if hardware blits can be done */
	if (videoInfo->blit_hw)
		m_videoFlags |= SDL_HWACCEL;

	/* Sets up OpenGL double buffering */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1);

	/* get a SDL surface */
	SDL_Surface* surface = SDL_SetVideoMode(width, height, depth, m_videoFlags);

	/* Verify there is a surface */
	if (!surface)
		Utilities::fatal(string("Video mode set failed: ") + SDL_GetError());
}

SDLGLWindow::~SDLGLWindow()
{
}

void SDLGLWindow::setTitle(const string& title, const string& iconTitle)
{
	SDL_WM_SetCaption(title.c_str(), iconTitle.c_str());
}

int SDLGLWindow::exec()
{
	/* initialize OpenGL */
	initGL();

	/* resize the initial window */
	resizeGL(m_width, m_height);
	
	int frameCount = 0;
	int frameTime = 0;
	
	SDL_Event event;
	SDL_Surface* surface;
	int tickCount = SDL_GetTicks();
	while (!m_done)
	{
		/* handle the events in the queue */

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_VIDEORESIZE:
					m_width = event.resize.w;
					m_height = event.resize.h;
					
					surface = SDL_SetVideoMode(m_width, m_height, m_depth, m_videoFlags);
					if (!surface)
						Utilities::fatal(string("Could not get a surface after resize: ") + SDL_GetError());
					
					resizeGL(m_width, m_height);
					break;
				case SDL_KEYDOWN:
					keyPressEvent(event.key.keysym);
					break;
				case SDL_KEYUP:
					keyReleaseEvent(event.key.keysym);
					break;
				case SDL_QUIT:
					m_done = true;
					break;
				default:
					break;
			}
		}
		
		int newTickCount = SDL_GetTicks();
		m_timeElapsed = newTickCount - tickCount;
		tickCount = newTickCount;
		
		if (m_timeElapsed < 1000.0/DESIRED_FPS)
			SDL_Delay(int(1000.0/DESIRED_FPS) - m_timeElapsed);
		
		frameTime += m_timeElapsed;
		frameCount++;
		if (frameTime > 1000)
		{
			cout << frameCount << " fps" << endl;
			frameCount = 0;
			frameTime = 0;
		}

		paintGL();
		SDL_GL_SwapBuffers();
	}
	
	return 0;
}
