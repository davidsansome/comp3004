#ifndef SDLWINDOW_H
#define SDLWINDOW_H

#include <SDL/SDL.h>
#include <svl/SVL.h>
#include <string>
#include <utility>

#include "utilities.h"
#include "array.h"
#include "edgebuffer.h"

class ContextState;

class SDLWindow : public PixelDrawer
{
public:
	SDLWindow(int width, int height, std::string title);
	virtual ~SDLWindow();
	
	void exec();
	void quit();
	
	int width() const { return m_width; }
	int height() const { return m_height; }
	ContextState* context() const { return m_context; }
	
	void drawPixel(int x, int y, float z, const Vec3& color, const Vec2& tex);

protected:
	virtual void init(); // Default implementation clears the screen
	virtual void draw() = 0;
	
	virtual void keyPressEvent(SDLKey keysym) {}
	int timeElapsed() const { return m_timeElapsed; }
	float timeDelta() const { return float(std::max(1, m_timeElapsed)) / 1000.0; }
	
	void clear();
	
	template<unsigned int N>
	void drawPoly(array<Vertex, N> data)
	{
		m_edgeBuffer->clear();
		for (unsigned int i=0 ; i<N ; ++i)
			drawLine(data[i], data[(i+1) % N]);
		renderEdgeBuffer();
	}

private:
	uint32 color(uint8 r, uint8 g, uint8 b);
	uint32 color(Vec3 c);
	void setPixel(int x, int y, float z, uint32 c);
	void setPixel(int x, int y, float z, Vec3 c);
	void setDepth(int x, int y, float value);
	float depth(int x, int y);
	
	void lockScreen();
	void unlockScreen();
	void drawLine(const Vertex& a, const Vertex& b);
	void renderEdgeBuffer();
	Vec3 objectToWindow(Vec3 objectCoords);
	Vec3 objectToWindow(Vec4 objectCoords);
	
	SDL_Surface* m_screen;
	SDL_Joystick* m_joyStick;
	bool m_quitNowPlease;
	int m_timeElapsed;
	
	ContextState* m_context;
	EdgeBuffer* m_edgeBuffer;
	float* m_depthBuffer;
	
	int m_width;
	int m_height;
};

#endif
