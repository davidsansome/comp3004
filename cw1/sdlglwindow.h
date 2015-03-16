#ifndef SDLGLWINDOW_H
#define SDLGLWINDOW_H

#include <string>

#ifdef __APPLE__
#include <SDL.h>
#include <SDL_OpenGL.h>
#else
#include <SDL/SDL.h>
#endif

using namespace std;

// Abstract base class for using OpenGL on SDL
class SDLGLWindow
{
public:
	SDLGLWindow(int width, int height, int depth);
	virtual ~SDLGLWindow();
	
	int exec();
	void close() { m_done = true; }
	void setTitle(string title, string iconTitle);
	
	virtual void initGL() = 0;
	virtual void resizeGL(int width, int height) = 0;
	virtual void paintGL() = 0;
	
	int width() { return m_width; }
	int height() { return m_height; }
	int depth() { return m_depth; }
	float timeDelta() { return float(m_timeElapsed) / 1000.0; }
	int timeElapsed() { return m_timeElapsed; }

protected:
	virtual void keyPressEvent(const SDL_keysym& keySym) {}

private:
	void fatal(string message);
	
	bool m_done;
	int m_width;
	int m_height;
	int m_depth;
	int m_videoFlags;
	int m_timeElapsed;
};

#endif

