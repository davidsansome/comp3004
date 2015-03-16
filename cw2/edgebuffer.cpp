#include "edgebuffer.h"
#include "sdlwindow.h"

#include <math.h>
#include <stdlib.h>
#include <algorithm>

EdgeBuffer::EdgeBuffer(int height)
	: m_height(height)
{
	m_minX = new int[height];
	m_maxX = new int[height];
	m_minZ = new float[height];
	m_maxZ = new float[height];
	m_minColor = new Vec3[height];
	m_maxColor = new Vec3[height];
	m_minTex = new Vec2[height];
	m_maxTex = new Vec2[height];
}

EdgeBuffer::~EdgeBuffer()
{
	delete[] m_minX;
	delete[] m_maxX;
	delete[] m_minZ;
	delete[] m_maxZ;
	delete[] m_minColor;
	delete[] m_maxColor;
	delete[] m_minTex;
	delete[] m_maxTex;
}

void EdgeBuffer::clear()
{
	for (int i=0 ; i<m_height ; ++i)
	{
		m_minX[i] = -1;
		m_maxX[i] = -1;
	}
}

void EdgeBuffer::drawPixel(int x, int y, float z, const Vec3& color, const Vec2& tex)
{
	if (y < 0 || y >= m_height)
		return;
	
	if (m_minX[y] == -1 || m_minX[y] > x)
	{
		m_minX[y] = x;
		m_minZ[y] = z;
		m_minColor[y] = color;
		m_minTex[y] = tex;
	}
	
	if (m_maxX[y] == -1 || m_maxX[y] < x)
	{
		m_maxX[y] = x;
		m_maxZ[y] = z;
		m_maxColor[y] = color;
		m_maxTex[y] = tex;
	}
}



