/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 *                                                                         *
 * This file was taken from Baarbarity, (c) David Sansome 2006, and        *
 * modified for COMP3004.                                                  *
 * http://fluffy.ecs.soton.ac.uk/~ds505/svn/baarbarity/                    *
 *                                                                         *
 ***************************************************************************/

#include "utilities.h"
#include "terrain.h"
#include "shader.h"
#include "texture.h"

#include <math.h>
#include <stdlib.h>
#include <sstream>


Terrain::Terrain(int xTiles, int yTiles)
{
	m_sandTexture = Texture::load("sand.jpg");
	m_mossTexture = Texture::load("moss.jpg");
	
	m_shader = new Shader("terrain_vert", "terrain_frag");
	m_lightVectorLoc = m_shader->uniformLocation("lightVector");
	m_terrainSizeLoc = m_shader->uniformLocation("terrainSize");
	m_sandTexLoc = m_shader->uniformLocation("sandTex");
	m_mossTexLoc = m_shader->uniformLocation("mossTex");
	m_caustTexLoc = m_shader->uniformLocation("caustic");
	
	m_shaderr = new Shader("terrainr_vert", "terrainr_frag");
	m_lightVectorLocR = m_shaderr->uniformLocation("lightVector");
	m_terrainSizeLocR = m_shaderr->uniformLocation("terrainSize");
	m_sandTexLocR = m_shaderr->uniformLocation("sandTex");
	m_mossTexLocR = m_shaderr->uniformLocation("mossTex");
	m_caustTexLocR = m_shaderr->uniformLocation("caustic");
	
	for (int i=0 ; i<32 ; ++i)
	{
		stringstream filename;
		filename << "caust" << i << ".jpg";
		m_caustTextures[i] = Texture::load(filename.str());
	}
	
	m_xTiles = xTiles;
	m_yTiles = yTiles;
	m_width = xTiles * TILE_SIZE + 1;
	m_height = yTiles * TILE_SIZE + 1;

	m_heightMap = new float[m_width * m_height];
	m_normalMap = new float[m_width * m_height * 3];

	for (int i=0 ; i<m_width*m_height ; ++i)
		m_heightMap[i] = 0.0f;
}

Terrain::~Terrain()
{
	delete[] m_heightMap;
	delete[] m_normalMap;
	
	delete m_sandTexture;
	delete m_mossTexture;
	for (int i=0 ; i<32 ; ++i)
		delete m_caustTextures[i];
	
	delete m_shader;
	delete m_shaderr;

	glDeleteBuffers(3, (GLuint*)m_bufferObjects);
}

void Terrain::generate(int iterations, int minDistance, int maxDistance, float flattenFactor)
{
	// Generate heightmap
	for (int i=0 ; i<iterations ; ++i)
	{
		int centerX = rand() % m_width;
		int centerY = rand() % m_height;
		int radius;
		if (maxDistance == minDistance)
			radius = maxDistance;
		else
			radius = minDistance + rand() % (maxDistance - minDistance);
		int radiusSquared = int(pow(float(radius), 2));
		
		int x = max(0, centerX-radius);
		int initialY = max(0, centerY-radius);
		int maxX = min(m_width, centerX+radius);
		int maxY = min(m_height, centerY+radius);
		
		for ( ; x<maxX ; ++x)
		{
			for (int y=initialY ; y<maxY ; ++y)
			{
				int heightMod = radiusSquared - (int(pow(float(x - centerX), 2)) + int(pow(float(y - centerY), 2)));
				if (heightMod > 0)
					setHeightAt(x, y, heightAt(x, y) + heightMod);
			}
		}
	}
	
	normalize();
	flatten(flattenFactor);
	calculateNormals();
	finalize();
}

void Terrain::calculateNormals()
{
	// Calculate vertex normals
	for (int x=0 ; x<m_width ; ++x)
		for (int y=0 ; y<m_height ; ++y)
			calculateVertexNormal(x, y);
}

void Terrain::finalize()
{
	// Generate vertex buffers
	int horizEdgeElements = m_width*2;
	int vertEdgeElements = m_height*2;
	int elementsPerStrip3 = m_height*2 * 3;
	int elementsPerStrip2 = m_height*2 * 2;
	int reflections = m_height*2 * TILE_SIZE * 2 + m_width*2 * TILE_SIZE * 2;
	int size3 = elementsPerStrip3 * (m_width-1) + horizEdgeElements*3*2 + vertEdgeElements*3*2 + reflections*3;
	int size2 = elementsPerStrip2 * (m_width-1) + horizEdgeElements*2*2 + vertEdgeElements*2*2 + reflections*3;
	float* vertexData     = new float[size3];
	float* normalData     = new float[size3];
	float* texCoordData   = new float[size2];

	float* v = vertexData;
	float* n = normalData;
	float* t = texCoordData;
	
	for (int x=0 ; x<(m_width-1) ; ++x)
	{
		for (int y=0 ; y<m_height ; ++y)
		{
			addVertexToBuffer(v, n, t, x+1, y);
			v+=3; n+=3; t+=2;
			addVertexToBuffer(v, n, t, x, y);
			v+=3; n+=3; t+=2;
		}
	}

	finalizeHorizEdge(&v, &n, &t, /*&a,*/ 0, 1.0f, 1);
	finalizeVertEdge(&v, &n, &t, /*&a,*/ m_width-1, 1.0f, 1);
	finalizeHorizEdge(&v, &n, &t, /*&a,*/ m_height-1, -1.0f, -1);
	finalizeVertEdge(&v, &n, &t, /*&a,*/ 0, -1.0f, -1);
	
	// Left side mirrored
	for (int x=0 ; x<TILE_SIZE ; ++x)
	{
		for (int y=0 ; y<m_height ; ++y)
		{
			addVertexToBufferMirrored(v, n, t, -(x+1), y, x+1, y);
			v+=3; n+=3; t+=2;
			addVertexToBufferMirrored(v, n, t, -x, y, x, y);
			v+=3; n+=3; t+=2;
		}
	}
	
	// Right side mirrored
	for (int x=m_width-TILE_SIZE_PLUS_1 ; x<(m_width-1) ; ++x)
	{
		for (int y=0 ; y<m_height ; ++y)
		{
			addVertexToBufferMirrored(v, n, t, 2*m_width - x - 3, y, x+1, y);
			v+=3; n+=3; t+=2;
			addVertexToBufferMirrored(v, n, t, 2*m_width - x - 2, y, x, y);
			v+=3; n+=3; t+=2;
		}
	}
	
	// Top edge mirrored
	for (int y=0 ; y<TILE_SIZE ; ++y)
	{
		for (int x=0 ; x<m_width ; ++x)
		{
			addVertexToBufferMirrored(v, n, t, x, -y, x, y);
			v+=3; n+=3; t+=2;
			addVertexToBufferMirrored(v, n, t, x, -(y+1), x, y+1);
			v+=3; n+=3; t+=2;
		}
	}
	
	// Bottom edge mirrored
	for (int y=m_height-TILE_SIZE_PLUS_1 ; y<m_height-1 ; ++y)
	{
		for (int x=0 ; x<m_width ; ++x)
		{
			addVertexToBufferMirrored(v, n, t, x, 2*m_height - y - 2, x, y);
			v+=3; n+=3; t+=2;
			addVertexToBufferMirrored(v, n, t, x, 2*m_height - y - 3, x, y+1);
			v+=3; n+=3; t+=2;
		}
	}
	
	glGenBuffers(3, (GLuint *)m_bufferObjects);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[0]);
	glBufferData(GL_ARRAY_BUFFER, size3 * sizeof(float), vertexData, GL_STATIC_DRAW);
	delete[] vertexData;

	glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[1]);
	glBufferData(GL_ARRAY_BUFFER, size3 * sizeof(float), normalData, GL_STATIC_DRAW);
	delete[] normalData;

	glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[2]);
	glBufferData(GL_ARRAY_BUFFER, size2 * sizeof(float), texCoordData, GL_STATIC_DRAW);
	delete[] texCoordData;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Terrain::finalizeHorizEdge(float** v, float** n, float** t, /*float** a,*/ int y, float normalDirection, int incDirection)
{
	int start = incDirection>0 ? 0 : m_width-1;
	for (int x=start ; x<m_width && x>=0 ; x+=incDirection)
	{
		float h = max(0.0f, heightAt(x, y));
		(*v)[0] = x;      (*v)[1] = -SAND_DEPTH;            (*v)[2] = y;
		(*n)[0] = 0.0f;   (*n)[1] = 0.0f;              (*n)[2] = normalDirection;
		(*t)[0] = float(x) / float(TILE_SIZE);
		(*t)[1] = (h + SAND_DEPTH) / float(TILE_SIZE);

		(*v)+=3; (*n)+=3; (*t)+=2;

		(*v)[0] = x;      (*v)[1] = h;                 (*v)[2] = y;
		(*n)[0] = 0.0f;   (*n)[1] = 0.0f;              (*n)[2] = normalDirection;
		(*t)[0] = float(x) / float(TILE_SIZE);
		(*t)[1] = 0.0f;

		(*v)+=3; (*n)+=3; (*t)+=2;
	}
}

void Terrain::finalizeVertEdge(float** v, float** n, float** t, /*float** a,*/ int x, float normalDirection, int incDirection)
{
	int start = incDirection>0 ? 0 : m_height-1;
	for (int y=start ; y<m_height && y>=0 ; y+=incDirection)
	{
		float h = max(0.0f, heightAt(x, y));
		(*v)[0] = x;                 (*v)[1] = -SAND_DEPTH;              (*v)[2] = y;
		(*n)[0] = normalDirection;   (*n)[1] = 0.0f;              (*n)[2] = 0.0f;
		(*t)[0] = (h + SAND_DEPTH) / float(TILE_SIZE);
		(*t)[1] = float(y) / float(TILE_SIZE);

		(*v)+=3; (*n)+=3; (*t)+=2;
		
		(*v)[0] = x;                 (*v)[1] = h;                 (*v)[2] = y;
		(*n)[0] = normalDirection;   (*n)[1] = 0.0f;              (*n)[2] = 0.0f;
		(*t)[0] = 0.0f;
		(*t)[1] = float(y) / float(TILE_SIZE);

		(*v)+=3; (*n)+=3; (*t)+=2;
	}
}

void Terrain::addVertexToBuffer(float* v, float* n, float* t, int x, int y)
{
	v[0] = x;
	v[1] = heightAt(x, y);
	v[2] = y;

	Vec3 normal(normalAt(x, y));
	n[0] = normal[0];
	n[1] = normal[1];
	n[2] = normal[2];

	t[0] = float(x) / float(TILE_SIZE);
	t[1] = float(y) / float(TILE_SIZE);
}

void Terrain::addVertexToBufferMirrored(float* v, float* n, float* t, int x, int y, int realX, int realY)
{
	v[0] = x;
	v[1] = heightAt(realX, realY);
	v[2] = y;

	Vec3 normal(normalAt(realX, realY));
	n[0] = normal[0];
	n[1] = normal[1];
	n[2] = normal[2];

	t[0] = float(x) / float(TILE_SIZE);
	t[1] = float(y) / float(TILE_SIZE);
}

void Terrain::calculateVertexNormal(int x, int y)
{
	float result[3];
	
	result[0] = 0.0f;
	result[1] = 0.0f;
	result[2] = 0.0f;

	// First quadrant
	if (x < m_width-1 && y < m_height-1)
	{
		calculatePlaneNormal(x, y, x+1, y, x+1, y+1, result);
		calculatePlaneNormal(x, y, x+1, y+1, x, y+1, result);
	}

	// Second quadrant
	if (x > 0 && y < m_height-1)
		calculatePlaneNormal(x, y, x, y+1, x-1, y, result);

	// Third quadrant
	if (x > 0 && y > 0)
	{
		calculatePlaneNormal(x, y, x-1, y, x-1, y-1, result);
		calculatePlaneNormal(x, y, x-1, y-1, x, y-1, result);
	}

	// Fourth quadrant
	if (x < m_width-1 && y > 0)
		calculatePlaneNormal(x, y, x, y-1, x+1, y, result);

	Utilities::normalizeVector(result);

	setNormalAt(x, y, result);
}

void Terrain::calculatePlaneNormal(int x, int y, int x1, int y1, int x2, int y2, float* result)
{
	float v1[3] = {x1 - x,
	              heightAt(x1, y1) - heightAt(x, y),
	              y - y1};
	float v2[3] = {x2 - x,
	              heightAt(x2, y2) - heightAt(x, y),
	              y - y2};
	
	Utilities::normalizeVector(v1);
	Utilities::normalizeVector(v2);
	
	result[0] += v1[1] * v2[2] - v1[2] * v2[1];
	result[1] += v1[2] * v2[0] - v1[0] * v2[2];
	result[2] -= v1[0] * v2[1] - v1[1] * v2[0];
}

void Terrain::setHeightAt(int x, int y, float value)
{
	m_heightMap[y*m_width + x] = value;
}

void Terrain::setNormalAt(int x, int y, float* value)
{
	float* targetValue = m_normalMap + (y*m_width + x)*3;
	targetValue[0] = value[0];
	targetValue[1] = value[1];
	targetValue[2] = value[2];
}

void Terrain::normalize()
{
	float max = 0.0f;
	float min = HUGE_VAL;
	
	// Find max and min
	for (int x=0 ; x<m_width ; ++x)
	{
		for (int y=0 ; y<m_height ; ++y)
		{
			float val = heightAt(x, y);
			
			if (val > max)
				max = val;
			if (val < min)
				min = val;
		}
	}
	
	// Normalize
	float diff = max - min;
	
	for (int x=0 ; x<m_width ; ++x)
	{
		for (int y=0 ; y<m_height ; ++y)
		{
			setHeightAt(x, y, (heightAt(x, y) - min) / diff * MAX_HEIGHT);
		}
	}
}

void Terrain::flatten(float power)
{
	for (int x=0 ; x<m_width ; ++x)
		for (int y=0 ; y<m_height ; ++y)
			setHeightAt(x, y, pow(float(heightAt(x, y) / MAX_HEIGHT), power) * MAX_HEIGHT);
}

float Terrain::heightAt(int x, int y) const
{
	if (x<0 || y<0 || x>=m_width || y>=m_width)
		return 0.0f;
	
	return m_heightMap[y*m_width + x];
}

float Terrain::heightAt(const Vec2& pos) const
{
	float tl = heightAt(int(pos[0]), int(pos[1]));
	float tr = heightAt(int(pos[0]) + 1, int(pos[1]));
	float bl = heightAt(int(pos[0]), int(pos[1]) + 1);
	float br = heightAt(int(pos[0]) + 1, int(pos[1]) + 1);
	float weightX = (pos[0] - float(int(pos[0])));
	float weightY = (pos[1] - float(int(pos[1])));

	return Utilities::interpolate(tl, tr, bl, br,
	                              weightX, weightY,
	                              1.0f - weightX, 1.0f - weightY);
}

Vec3 Terrain::normalAt(int x, int y) const
{
	if (x<0 || y<0 || x>=m_width || y>=m_width)
		return Vec3(0.0f, 1.0f, 0.0f);
	
	float* p = m_normalMap + (y*m_width + x)*3;
	return Vec3(p[0], p[1], p[2]);
}

Vec3 Terrain::normalAt(const Vec2& pos) const
{
	Vec3 tl(normalAt(int(pos[0]), int(pos[1])));
	Vec3 tr(normalAt(int(pos[0]) + 1, int(pos[1])));
	Vec3 bl(normalAt(int(pos[0]), int(pos[1]) + 1));
	Vec3 br(normalAt(int(pos[0]) + 1, int(pos[1]) + 1));
	float weightX = (pos[0] - float(int(pos[0])));
	float weightY = (pos[1] - float(int(pos[1])));
	float cWeightX = 1.0f - weightX;
	float cWeightY = 1.0f - weightY;
	
	return Vec3(Utilities::interpolate(tl[0], tr[0], bl[0], br[0],
	                                   weightX, weightY, cWeightX, cWeightY),
	            Utilities::interpolate(tl[1], tr[1], bl[1], br[1],
	                                   weightX, weightY, cWeightX, cWeightY),
	            Utilities::interpolate(tl[2], tr[2], bl[2], br[2],
	                                   weightX, weightY, cWeightX, cWeightY));
}

void Terrain::drawStrip(int strip)
{
	int size = m_height*2;
	int first = strip * size;
	glDrawArrays(GL_TRIANGLE_STRIP, first, size);
}

void Terrain::drawEdges()
{
	int stripSize = m_height*2;
	int first = stripSize * (m_width-1);
	int horizEdgeElements = m_width*2;
	int vertEdgeElements = m_height*2;
	int size = horizEdgeElements*2 + vertEdgeElements*2;
	glDrawArrays(GL_TRIANGLE_STRIP, first, size);
}

void Terrain::bind()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[1]);
	glNormalPointer(GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[2]);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);
	
	m_shader->bind();
	glUniform2f(m_terrainSizeLoc, m_width-1, m_height-1);
	glUniform3f(m_lightVectorLoc, m_lightVector[0], m_lightVector[1], m_lightVector[2]);
	
	m_sandTexture->bind(0, m_sandTexLoc);
	m_mossTexture->bind(1, m_mossTexLoc);
	m_caustTextures[m_caustic]->bind(2, m_caustTexLoc);
	
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 0x80, 0x80);
	glEnable(GL_STENCIL_TEST);
}

void Terrain::bindReflections()
{
	m_shaderr->bind();
	glUniform2f(m_terrainSizeLocR, m_width-1, m_height-1);
	glUniform3f(m_lightVectorLocR, m_lightVector[0], m_lightVector[1], m_lightVector[2]);
	
	m_sandTexture->bind(0, m_sandTexLocR);
	m_mossTexture->bind(1, m_mossTexLocR);
	m_caustTextures[m_caustic]->bind(2, m_caustTexLocR);
	
	glEnable(GL_BLEND);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

void Terrain::unbind()
{
	glDisable(GL_BLEND);
	glDisable(GL_STENCIL_TEST);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	m_shaderr->unbind();
}

void Terrain::draw()
{
	int w = m_width - 1;
	for (int x=0 ; x<w ; ++x)
		drawStrip(x);
	drawEdges();
}

void Terrain::drawReflections()
{
	int stripSize = m_height*2;
	int horizEdgeElements = m_width*2;
	int vertEdgeElements = m_height*2;
	int first = stripSize * (m_width-1) + horizEdgeElements*2 + vertEdgeElements*2;
	int vertReflectionElements = m_height*2;
	int horizReflectionElements = m_width*2;
	
	glStencilFunc(GL_EQUAL, 0xC0, 0xF0);
	drawReflection(&first, vertReflectionElements);
	glStencilFunc(GL_EQUAL, 0xA0, 0xF0);
	drawReflection(&first, vertReflectionElements);
	glStencilFunc(GL_EQUAL, 0x90, 0xF0);
	drawReflection(&first, horizReflectionElements);
	glStencilFunc(GL_EQUAL, 0xB0, 0xF0);
	drawReflection(&first, horizReflectionElements);
}

void Terrain::drawReflection(int* first, int count)
{
	for (int i=0 ; i<TILE_SIZE ; ++i)
	{
		glDrawArrays(GL_TRIANGLE_STRIP, *first, count);
		(*first) += count;
	}
}

void Terrain::setParams(const Vec3& lightVector)
{
	m_lightVector = lightVector;
}
