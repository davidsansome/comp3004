#ifndef TEXTURE_H
#define TEXTURE_H

#ifndef GP2X_PLATFORM
#include <SDL/SDL_image.h>
#endif

#include <string>
#include <svl/SVL.h>

class Texture
{
public:
	Texture(std::string filename);
	~Texture();
	
	Vec3 sample(const Vec2& coord) const;

private:
#ifndef GP2X_PLATFORM
	SDL_Surface* m_surface;
#endif
};

#endif
