#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <iostream>
#include <stdlib.h>

#include "sdlglwindow.h"

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
		fatal(string("Video query failed: ") + SDL_GetError());

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

	/* get a SDL surface */
	SDL_Surface* surface = SDL_SetVideoMode(width, height, depth, m_videoFlags);

	/* Verify there is a surface */
	if (!surface)
		fatal(string("Video mode set failed: ") + SDL_GetError());
}

SDLGLWindow::~SDLGLWindow()
{
}

void SDLGLWindow::fatal(string message)
{
	cerr << message << endl;
	exit(1);
}

void SDLGLWindow::setTitle(string title, string iconTitle)
{
	SDL_WM_SetCaption(title.c_str(), iconTitle.c_str());
}

int SDLGLWindow::exec()
{
	/* initialize OpenGL */
	initGL();

	/* resize the initial window */
	resizeGL(m_width, m_height);
	
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
						fatal(string("Could not get a surface after resize: ") + SDL_GetError());
					
					resizeGL(m_width, m_height);
					break;
				case SDL_KEYDOWN:
					keyPressEvent(event.key.keysym);
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

		paintGL();
		SDL_GL_SwapBuffers();
	}
	
	return 0;
}
