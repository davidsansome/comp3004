#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <svl/SVL.h>
#include "array.h"
#include "math.h"

class PixelDrawer;

typedef unsigned char uchar;
typedef uchar uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

typedef std::pair<Vec3, std::pair<Vec3, Vec2> > Vertex;

#define TWO_PI (M_PI*2.0)

namespace Utilities
{
	void fatal(std::string message);
	
	float length(int x, int y);
	Vertex newVertex(const Vec3& coord, const Vec3& color, const Vec2& texCoord);
	
	template<class T>
	T clamp(const T& mi, const T& v, const T& ma)
	{
		return std::max(mi, std::min(ma, v));
	}
	
	void drawLine(PixelDrawer* p, Vec3 a, Vec3 b, Vec3 colorA, Vec3 colorB, Vec2 texA, Vec2 texB);
};

#endif
