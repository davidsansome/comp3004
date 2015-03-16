#ifndef SDLGLWINDOW_H
#define SDLGLWINDOW_H

#include <SDL/SDL.h>
#include "utilities.h"

// Abstract base class for using OpenGL on SDL
class SDLGLWindow
{
public:
	SDLGLWindow(int width, int height, int depth);
	virtual ~SDLGLWindow();
	
	int exec();
	void close() { m_done = true; }
	void setTitle(const string& title, const string& iconTitle);
	
	virtual void initGL() = 0;
	virtual void resizeGL(int width, int height) = 0;
	virtual void paintGL() = 0;
	
	int width() const { return m_width; }
	int height() const { return m_height; }
	int depth() const { return m_depth; }
	float timeDelta() const { return float(m_timeElapsed) / 1000.0; }
	int timeElapsed() const { return m_timeElapsed; }

protected:
	virtual void keyPressEvent(const SDL_keysym& keySym) {}
	virtual void keyReleaseEvent(const SDL_keysym& keySym) {}

private:
	bool m_done;
	int m_width;
	int m_height;
	int m_depth;
	int m_videoFlags;
	int m_timeElapsed;
};

#endif

