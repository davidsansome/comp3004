#include "utilities.h"
#include "edgebuffer.h"
#include <iostream>
#include <math.h>

using std::string;

void Utilities::fatal(string message)
{
	std::cerr << message << std::endl;
	exit(1);
}

float Utilities::length(int x, int y)
{
	return sqrt(x*x + y*y);
}

Vertex Utilities::newVertex(const Vec3& coord, const Vec3& color, const Vec2& texCoord)
{
	return Vertex(coord, std::pair<Vec3, Vec2>(color, texCoord));
}

void Utilities::drawLine(PixelDrawer* p, Vec3 a, Vec3 b, Vec3 colorA, Vec3 colorB, Vec2 texA, Vec2 texB)
{
	int x0 = int(a[0]);
	int x1 = int(b[0]);
	int y0 = int(a[1]);
	int y1 = int(b[1]);
	float z0 = a[2];
	float z1 = b[2];

	int dx, dy, incrE, incrNE, d, x, y, x_end;
	int incrN, incrEN, y_end;
	int Xinc, Yinc;
	dx = x1-x0;
	dy = y1-y0;

	if (abs(dx) > abs(dy))
	{
		if (dx<0)
		{
			std::swap(x0, x1);
			std::swap(y0, y1);
			std::swap(z0, z1);
			std::swap(colorA, colorB);
			std::swap(texA, texB);
		}
		if (y1>y0)
			Yinc = 1;
		else
			Yinc = -1;
	
		dx = abs(dx);
		dy = abs(dy);
	
		d = 2*dy-dx;
		incrE = 2*dy;
		incrNE = 2 * (dy-dx);
		Vec3 dColor = colorB - colorA;
		Vec2 dTex = texB - texA;
		float dz = z1 - z0;
	
		x_end = x1;
		x = x0;
		y = y0;
		
		float interp = float(x-x0)/(x_end-x0);
		p->drawPixel(x, y, z0 + dz*interp, colorA + dColor*interp, texA + dTex*interp);
		while (x < x_end)
		{
			if ( d <= 0 )
			{
				d = d + incrE;
				x = x + 1;
			}
			else
			{
				d = d + incrNE;
				x = x + 1;
				y = y +Yinc;
			}
			interp = float(x-x0)/(x_end-x0);
			p->drawPixel(x, y, z0 + dz*interp, colorA + dColor*interp, texA + dTex*interp);
		}
	}
	else
	{
		if (dy<0)
		{
			std::swap(x0, x1);
			std::swap(y0, y1);
			std::swap(z0, z1);
			std::swap(colorA, colorB);
			std::swap(texA, texB);
		}
		if (x1>x0)
			Xinc = 1;
		else
			Xinc = -1;
	
		dx = abs(dx);
		dy = abs(dy);
	
		d = 2*dx-dy;
		incrN = 2*dx;
		incrEN = 2 * (dx-dy);
		Vec3 dColor = colorB - colorA;
		Vec2 dTex = texB - texA;
		float dz = z1 - z0;
	
		y_end = y1;
		x = x0;
		y = y0;
	
		float interp = float(y-y0)/(y_end-y0);
		p->drawPixel(x, y, z0 + dz*interp, colorA + dColor*interp, texA + dTex*interp);
		while (y < y_end)
		{
			if ( d <= 0 )
			{
				d = d + incrN;
				y = y + 1;
			}
			else
			{
				d = d + incrEN;
				y = y + 1;
				x = x + Xinc;
			}
			interp = float(y-y0)/(y_end-y0);
			p->drawPixel(x, y, z0 + dz*interp, colorA + dColor*interp, texA + dTex*interp);
		}
	}
}


