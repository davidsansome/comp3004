#include "sdlwindow.h"
#include "utilities.h"
#include "gp2x.h"
#include "texture.h"
#include "contextstate.h"

#include <math.h>

using std::string;

SDLWindow::SDLWindow(int w, int h, string title)
	: m_quitNowPlease(false),
	  m_width(w),
	  m_height(h)
{
	int desiredBpp = 0;
	Uint32 videoFlags = 0;
	if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0 )
		Utilities::fatal(string("Couldn't initialize SDL: ") + SDL_GetError());
	
	SDL_JoystickEventState(SDL_ENABLE);
	m_joyStick = SDL_JoystickOpen(0);
	
	m_screen = SDL_SetVideoMode(w, h, desiredBpp, videoFlags);
	if ( m_screen == NULL )
		Utilities::fatal(string("Couldn't set desired video mode: ") + SDL_GetError());
	
	SDL_WM_SetCaption(title.c_str(), "testwin");
	
	m_edgeBuffer = new EdgeBuffer(m_height);
	m_depthBuffer = new float[w*h];
	m_context = new ContextState();
}


SDLWindow::~SDLWindow()
{
	SDL_FreeSurface(m_screen);
	delete m_edgeBuffer;
	delete m_depthBuffer;
}

void SDLWindow::lockScreen()
{
	if ( SDL_MUSTLOCK(m_screen) )
		if ( SDL_LockSurface(m_screen) < 0 )
			Utilities::fatal(std::string("Can't lock screen: ") + SDL_GetError());
}

void SDLWindow::unlockScreen()
{
	if ( SDL_MUSTLOCK(m_screen) )
		SDL_UnlockSurface(m_screen);
	
	// Update the whole screen
	SDL_UpdateRect(m_screen, 0, 0, 0, 0);
}

void SDLWindow::exec()
{
	lockScreen();
	init();
	unlockScreen();
	
	int tickCount = SDL_GetTicks();
	SDL_Event e;
	while (!m_quitNowPlease)
	{
		if (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_KEYDOWN:
				keyPressEvent(e.key.keysym.sym);
				break;
			case SDL_QUIT:
				return;
			case SDL_JOYBUTTONDOWN:
				switch (e.jbutton.button)
				{
				case GP2X_BUTTON_START:
					return;
				case GP2X_BUTTON_SELECT:
					keyPressEvent(SDLK_m);
					break;
				case GP2X_BUTTON_A:
					keyPressEvent(SDLK_w);
					break;
				case GP2X_BUTTON_X:
					keyPressEvent(SDLK_p);
					break;
				default:
					break;
				}
				break;
			}
		}
		
		lockScreen();
		draw();
		unlockScreen();
		
		int newTickCount = SDL_GetTicks();
		m_timeElapsed = newTickCount - tickCount;
		tickCount = newTickCount;
		
	#ifdef GP2X_PLATFORM
		SDL_Flip(m_screen);
		SDL_Delay(10);
	#endif
	}
}

uint32 SDLWindow::color(uint8 r, uint8 g, uint8 b)
{
	return SDL_MapRGB(m_screen->format, (Uint8) r, (Uint8) g, (Uint8) b);
}

uint32 SDLWindow::color(Vec3 c)
{
	return color(uint8(c[0]*255), uint8(c[1]*255), uint8(c[2]*255));
}

void SDLWindow::init()
{
	clear();
}

void SDLWindow::clear()
{
	SDL_FillRect(m_screen, NULL, color(0, 0, 0));
	
	static const float maxFloat = std::numeric_limits<float>::max();
	for (int y=0 ; y<m_height ; ++y)
		for (int x=0 ; x<m_width ; ++x)
			setDepth(x, y, maxFloat);
}

void SDLWindow::setDepth(int x, int y, float value)
{
	m_depthBuffer[y*m_width + x] = value;
}

float SDLWindow::depth(int x, int y)
{
	return m_depthBuffer[y*m_width + x];
}

void SDLWindow::quit()
{
	m_quitNowPlease = true;
}



Vec3 SDLWindow::objectToWindow(Vec3 objectCoords)
{
	return objectToWindow(Vec4(objectCoords, 1.0));
}

Vec3 SDLWindow::objectToWindow(Vec4 objectCoords)
{
	Vec4 clip = m_context->projectionMatrix() * (m_context->modelViewMatrix() * objectCoords);
	return Vec3
	(
		(clip[0]/clip[3] + 1.0) * (float(width())/2.0),
		(clip[1]/clip[3] + 1.0) * (float(height())/2.0),
		clip[2]/clip[3] * -1.0
	);
}

void SDLWindow::drawLine(const Vertex& a, const Vertex& b)
{
	// Transform a and b from object space to window space
	Vec3 a2(objectToWindow(a.first));
	Vec3 b2(objectToWindow(b.first));
	
	// Draw the line into the edge buffer
	//std::cout << "Line from " << a2 << " to " << b2 << " colors " << a.second << " and " << b.second << std::endl;
	
	PixelDrawer* drawer = NULL;
	switch (m_context->polygonMode())
	{
		case ContextState::Fill: drawer = m_edgeBuffer; break;
		case ContextState::Line: drawer = this;         break;
	}
	Utilities::drawLine(drawer, a2, b2, a.second.first, b.second.first, a.second.second, b.second.second);
}

void SDLWindow::renderEdgeBuffer()
{
	for (int y=0 ; y<m_height ; ++y)
	{
		if (m_edgeBuffer->m_minX[y] == -1)
			continue;
		
		Vec3 dColor = m_edgeBuffer->m_maxColor[y] - m_edgeBuffer->m_minColor[y];
		float dz = m_edgeBuffer->m_maxZ[y] - m_edgeBuffer->m_minZ[y];
		Vec2 dTex = m_edgeBuffer->m_maxTex[y] - m_edgeBuffer->m_minTex[y];
		
		int min = std::max(0, m_edgeBuffer->m_minX[y]);
		int max = std::min(m_width, m_edgeBuffer->m_maxX[y]);
		int len = max - min + 1;
		
		for (int x=min ; x<=max ; ++x)
		{
			Vec3 c = m_edgeBuffer->m_minColor[y] + dColor*(float(x-m_edgeBuffer->m_minX[y])/len);
			float z = m_edgeBuffer->m_minZ[y] + dz*(float(x-m_edgeBuffer->m_minX[y])/len);
			
			if (m_context->texture() != NULL)
			{
				Vec2 t = m_edgeBuffer->m_minTex[y] + dTex*(float(x-m_edgeBuffer->m_minX[y])/len);
				c = m_context->texture()->sample(t);
			}
			setPixel(x, y, z, c);
		}
	}
}

void SDLWindow::drawPixel(int x, int y, float z, const Vec3& color, const Vec2& tex)
{
	if (m_context->texture() != NULL)
		setPixel(x, y, z, m_context->texture()->sample(tex));
	else
		setPixel(x, y, z, color);
}

void SDLWindow::setPixel(int x, int y, float z, Vec3 c)
{
	setPixel(x, y, z, color(c));
}

void SDLWindow::setPixel(int x, int y, float z, uint32 c)
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height)
		return;
	
	if (z > depth(x, y))
	{
		//std::cout << "Failed depth test at " << x << "," << y << "," << z << " value was " << depth(x, y) << std::endl;
		return;
	}
	//std::cout << "Passed depth test at " << x << "," << y << "," << z << " value was " << depth(x, y) << std::endl;
	setDepth(x, y, z);
	
	//uint8 zc = std::max(0, std::min(255, int((z-1.8)*128)));
	//c = color(zc, zc, zc);
	
#ifdef GP2X_PLATFORM
	uint8 *p = (uint8 *)m_screen->pixels + y * m_screen->pitch + x * 2;
	*(uint16 *)p = c;
#else
	Uint8 *p = (Uint8 *)m_screen->pixels + y * m_screen->pitch + x * 4;
	*(uint32 *)p = c;
#endif
}

