#ifndef EDGEBUFFER_H
#define EDGEBUFFER_H

#include <svl/SVL.h>

class SDLWindow;

class PixelDrawer
{
public:
	virtual ~PixelDrawer() {}
	virtual void drawPixel(int x, int y, float z, const Vec3& color, const Vec2& tex) = 0;
};

class EdgeBuffer : public PixelDrawer
{
public:
	EdgeBuffer(int height);
	~EdgeBuffer();
	
	void clear();
	
	void drawPixel(int x, int y, float z, const Vec3& color, const Vec2& tex);
	
	int* m_minX;
	int* m_maxX;
	float* m_minZ;
	float* m_maxZ;
	Vec3* m_minColor;
	Vec3* m_maxColor;
	Vec2* m_minTex;
	Vec2* m_maxTex;
	
	int m_height;
};

#endif
