#include "texture.h"
#include "utilities.h"

Texture::Texture(std::string filename)
{
#ifndef GP2X_PLATFORM
	m_surface = IMG_Load(filename.c_str());
	if (m_surface == NULL)
		Utilities::fatal("Couldn't load texture " + filename);
#endif
}

Texture::~Texture()
{
#ifndef GP2X_PLATFORM
	SDL_FreeSurface(m_surface);
#endif
}

Vec3 Texture::sample(const Vec2& coord) const
{
#ifndef GP2X_PLATFORM
	int x = int(coord[0] * m_surface->w);
	int y = int(coord[1] * m_surface->h);
	
	uint8* p = ((uint8*)m_surface->pixels) + (y*m_surface->w + x)*m_surface->format->BytesPerPixel;
	
	switch (m_surface->format->BitsPerPixel)
	{
	case 24:
	case 32:
		return Vec3(float(p[0])/255.0, float(p[1])/255.0, float(p[2])/255.0);
	default:
		return Vec3(0.0, 0.0, 0.0);
	}
#else
	return Vec3(0.0, 0.0, 0.0);
#endif
}


